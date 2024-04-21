#include "neureset.h"
#include <QCoreApplication>

Neureset::Neureset()
{
    contact = true;
    power = true;
    inSession = false;

    currentSession = nullptr;

    connectionLight = new DeviceLight(this, "connection", false);
    contactLight = new DeviceLight(this, "contact", contact);
    tsLight = new DeviceLight(this, "ts", false);

    QObject::connect(connectionLight, &DeviceLight::lightChanged, this, &Neureset::lightUpdated);
    QObject::connect(contactLight, &DeviceLight::lightChanged, this, &Neureset::lightUpdated);
    QObject::connect(tsLight, &DeviceLight::lightChanged, this, &Neureset::lightUpdated);


    battery = 100;
    sessionTime = 0;

    //instatiate all timers
    batteryTimer = new QTimer(this);
    disconnectTimer = new QTimer(this);
    beepTimer = new QTimer(this);
    sessionTimer = new QTimer(this);
    pauseTimer = new QTimer(this);
    tsLightTimer = new QTimer(this);

    //connect all timers' timeout signals to the appropriate neureset slots
    connect(disconnectTimer, &QTimer::timeout, this, &Neureset::shutDown);
    connect(beepTimer, &QTimer::timeout, this, &Neureset::beep);
    connect(sessionTimer, &QTimer::timeout, this, &Neureset::updateSessionTime);
    connect(pauseTimer, &QTimer::timeout, this, &Neureset::shutDown);
    connect(tsLightTimer, &QTimer::timeout, this, &Neureset::treatmentRoundOver);

    //connect the "no battery" signal to the "stop" slot of all our timers, so they stop when the device shuts down
    QObject::connect(this, &Neureset::noBattery, batteryTimer, &QTimer::stop);
    QObject::connect(this, &Neureset::noBattery, disconnectTimer, &QTimer::stop);
    QObject::connect(this, &Neureset::noBattery, beepTimer, &QTimer::stop);
    QObject::connect(this, &Neureset::noBattery, sessionTimer, &QTimer::stop);
    QObject::connect(this, &Neureset::noBattery, pauseTimer, &QTimer::stop);
    QObject::connect(this, &Neureset::noBattery, tsLightTimer, &QTimer::stop);

    //connect the "low battery" signal to the neureset slot to start the beep timer
    //we use signals and slots here rather than direct function calls because the low battery stuff happens in its own thread, but the timers run in the main thread
    QObject::connect(this, &Neureset::lowBattery, this, &Neureset::startBeepTimer);

    //create a thread for updating the battery so it can be done concurrently alongside other processes
    batteryThread = QThread::create([this]{ decreaseBatteryByTime(); });
    batteryThread->start();

    //create a thread for updating the session progress bar
    progressThread = QThread::create([this]{ updateProgressByTime(); });

    batteryTimer->start(10000); // 10 seconds

    QObject::connect(&headset, &EEGHeadset::sendBaseline, this, &Neureset::baselineReceived);
    connect(this, &Neureset::updateSessionPaused, &headset, &EEGHeadset::recieveSessionPaused);
    connect(this, &Neureset::updateSessionStopped, &headset, &EEGHeadset::recieveSessionStopped);
}

Neureset::~Neureset()
{
    for(int i=0; i<sessions.size(); ++i){
        delete sessions[i];
    }
    sessions.clear();
}


void Neureset::pauseButtonPressed(){
    sessionsPaused = true;
    sessionTimer->stop();
    pauseTimer->start(10000);
    emit updateSessionPaused(sessionsPaused);
}

void Neureset::pauseSession(){
    sessionsPaused = !sessionsPaused;

    if(sessionsPaused)
    {
        sessionTimer->stop();
    }
    else
    {
        sessionTimer->start(1000);
    }

    emit updateSessionPaused(sessionsPaused);
}

void Neureset::powerButtonPressed(){
    power = !power;

    if(!power)
    {
        shutDown();
    }
}

void Neureset::startButtonPressed(){
    mutex.lock();
    //if session is currently paused, resume it. otherwise, start a new session
    if(sessionsPaused){
        sessionsPaused = false;
        sessionStopped = false;
        sessionTimer->start(1000);
        emit updateSessionPaused(sessionsPaused);
        pauseTimer->stop();
        mutex.unlock();
        return;
    }

    sessionsPaused = false;
    sessionStopped = false;
    emit updateSessionPaused(sessionsPaused);
    mutex.unlock();
    startSession();
}

void Neureset::stopButtonPressed(){
    mutex.lock();
    sessionsPaused = false;
    sessionStopped = true;

    sessionTimer->stop();
    emit updateSessionStopped(sessionStopped);
    //set the session timer on the front end to 0:00
    emit sessionTimeUpdated(QStringLiteral("%1:%2").arg(0).arg(0, 2, 10, QLatin1Char('0')));
    //set the session progress bar on the front end to 0
    emit progressUpdated(0);
    sessions.pop_back();                // remove current session data
    inSession = false;
    progressThread->requestInterruption(); // stop progress thread
    mutex.unlock();
}

void Neureset::selectButtonPressed(){
    //qInfo("selectButtonPressed from neureset class");
    //delete later
}

void Neureset::disconnectButtonPressed(){
    qInfo("disconnectButtonPressed from neureset class");
    contact = false;

    //if contact is lost but the user is not currently in a session, we don't need to initate the timer procedure
    if(inSession){
        contactLight->setLit(false);
        connectionLight->startFlashing();
        disconnectTimer->start(10000); // 10 seconds
        beepTimer->start(2000);

        //since pauseSession() toggles between paused and not paused, we only want to call it if the session is not already paused. if the session is paused and the user disconnects an electrode, we don't want to resume the session
        if(!sessionsPaused){
            pauseSession();
        }
    }
}

void Neureset::reconnectButtonPressed(){
    qInfo("reconnectButtonPressed from neureset class");

    contact = true;

    if(inSession){
        contactLight->setLit(true);
        connectionLight->stopFlashing();
        disconnectTimer->stop();
        beepTimer->stop();

        pauseSession();
    }
}

void Neureset::startSession()
{
    mutex.lock();
    inSession = true;

    baselines.clear();

    //qInfo("in startSession"); //debugging
    Session* currSession = new Session(this);
    sessions.push_back(currSession);
    currentSession = currSession;

    currentSession->setDateTime(dateTime);

    contactLight->setLit(contact);

    //float baselineBefore = headset.measureBaseline();
    headset.measureBaseline();
    //currSession->setBaselineBefore(baselineBefore);

    mutex.unlock();
    delete progressThread;
    progressThread = QThread::create([this]{ updateProgressByTime(); });
    progressThread->start();
    sessionTime = 0;
    sessionTimer->start(1000);
}

DeviceLight* Neureset::getConnLight()
{
    return connectionLight;
}

DeviceLight* Neureset::getContactLight()
{
    return contactLight;
}

DeviceLight* Neureset::getTSLight()
{
    return tsLight;
}

void Neureset::setDateTime(QDateTime newDateTime)
{
    dateTime = newDateTime;
}

float Neureset::getBattery()
{
    return battery;
}

void Neureset::receiveDataRequest()
{
    emit uploadData(sessions);
}

void Neureset::decreaseBattery(int decreaseAmount) {
    mutex.lock();

    if(power)
    {
        battery -= decreaseAmount;
        emit batteryChanged(battery);
        //display the low battery message only once, when battery is at 10%
        if(battery==10){
            qInfo("Low battery. Please charge the device.");
        }

        if(battery > 0 && battery <= 10 && !connectionLight->isFlashing())
        {
            connectionLight->startFlashing();
            emit lowBattery();
        }
        else if(battery <= 0)
        {
            battery = 0;
            batteryThread->requestInterruption();
            mutex.unlock();
            shutDown();
        }
    }

    mutex.unlock();

    // Potentially update battery level display or trigger low battery warning here
}

// Method to decrease battery repeatedly after a fixed amoount of seconds
void Neureset::decreaseBatteryByTime() {

    while(batteryThread->isRunning() && !batteryThread->isInterruptionRequested())
    {
        batteryThread->sleep(5);
        decreaseBattery(1);
    }
}

void Neureset::shutDown()
{
    mutex.lock();
    emit noBattery();

    eraseSessionData();
    power = false;
    inSession = false;

    sessionsPaused = false;
    sessionStopped = true;
    sessionTime = 0;

    connectionLight->stopFlashing();
    contactLight->stopFlashing();
    tsLight->stopFlashing();

    emit updateSessionStopped(sessionStopped);
    progressThread->requestInterruption(); // stop progress thread
    mutex.unlock();
    qInfo("Device shutting down.");

    emit connectionLost();
}

void Neureset::eraseSessionData()
{
    sessions.clear();
}

void Neureset::updateProgress(int prog)
{
    currentSession->updateProgress(prog);
    //emit signal to main window to update progress bar on front-end
    emit progressUpdated(currentSession->getProgress());
}

void Neureset::updateProgressByTime()
{
    while(progressThread->isRunning() && !progressThread->isInterruptionRequested())
    {
        if(sessionsPaused || sessionStopped){
            continue;
        }
        progressThread->sleep(1);
        updateProgress(2);
    }

    progressThread->quit();
}

void Neureset::updateSessionTime()
{
    sessionTime += 1;

    int seconds = sessionTime % 60;
    int minutes = (sessionTime - seconds) / 60;
    emit sessionTimeUpdated(QStringLiteral("%1:%2").arg(minutes).arg(seconds, 2, 10, QLatin1Char('0')));
}

QString Neureset::getCurrSessionTime()
{
    if(currentSession != nullptr)
    {
        int seconds = sessionTime % 60;
        int minutes = (sessionTime - seconds) / 60;
        return QStringLiteral("%1:%2").arg(minutes).arg(seconds, 2, 10, QLatin1Char('0'));
    }

    return QStringLiteral("%1:%2").arg(0).arg(0, 2, 10, QLatin1Char('0'));
}

void Neureset::lightUpdated(bool lit, QString t)
{
    emit lightChanged(lit, t);
}

void Neureset::treatmentRoundOver()
{
    tsLight->setLit(false);
}


void Neureset::beep()
{
    qInfo("*BEEP*");
    beepTimer->start(2000);
}

void Neureset::waveformRequested(const QString &elecNum)
{
    //qInfo("in waveformRequested");
    Electrode* elec = headset.getElectrodeById(elecNum);
    QVector<Sinewave> bWave = elec->getBrainwave();
    emit sendBrainwave(bWave);
}

void Neureset::baselineReceived()
{
    //get the baseline from the headset
    float base = headset.getBaseline();
    baselines.push_back(base);

    //add 10% to the session progress
    updateProgress(10);

    //we calculate one baseline before each round of treatment (and there are 4 rounds) + a final "after" baseline. if we have less than 5 baselines in our vector, we are not done treatment
    if(baselines.size() < 5){
        int i = baselines.size();

        //do the treatment
        qInfo("\nTreatment round %d ",i);
        tsLight->setLit(true);
        tsLightTimer->start(1000);
        headset.beginTreatment(i);


        //measure the baseline for the next round
        headset.measureBaseline();
    }
    //otherwise, we have just calculated the final baseline. store received baseline in the afterBaseline variable and finish the session
    else if (baselines.size() == 5){

        progressThread->requestInterruption(); // stop progress thread

        //decrease battery by 10% every session
        decreaseBattery(10);

        currentSession->setBaselineBefore(baselines[0]);
        currentSession->setBaselineAfter(base);
        currentSession->print();

        mutex.unlock();

        //session is now done, top off progress to 100%
        updateProgress(100);

        sessionTimer->stop();

        emit sessionComplete();
        inSession = false;
    }
}

void Neureset::startBeepTimer()
{
    beepTimer->start(2000);
}


void Neureset::setBattery(int percent)
{
    decreaseBattery(battery - percent);
}

QMutex* Neureset::getMutex()
{
    return &mutex;
}

float Neureset::getCurrSessionProgress()
{
    if(sessions.size() == 0 || currentSession == nullptr){
        return 0;
    }
    else{
        return currentSession->getProgress();
    }
}


//adds a delay for the specified number of seconds
void Neureset::delay(int seconds)
{
    QTime dieTime = QTime::currentTime().addSecs(seconds);
    while (QTime::currentTime() < dieTime){
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}

QVector<Session*>* Neureset::getSessions()
{
    return &sessions;
}
