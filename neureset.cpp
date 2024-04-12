#include "neureset.h"
#include <QCoreApplication>

Neureset::Neureset()
{
    connectionLight = new DeviceLight();
    contactLight = new DeviceLight();
    tsLight = new DeviceLight();
    battery = 100;
    sessionTime = 0;

    //timer to decrease battery life by 1 percent every 10 seconds
    //batteryTimer = new QTimer(this);
    disconnectTimer = new QTimer(this);
    beepTimer = new QTimer(this);
    sessionTimer = new QTimer(this);

    //create a thread for updating the battery so it can be done concurrently alongside other processes
    batteryThread = QThread::create([this]{ decreaseBatteryByTime(); });
    batteryThread->start();

    //create a thread for updating the session progress bar
    progressThread = QThread::create([this]{ updateProgressByTime(); });



    //connect(batteryTimer, &QTimer::timeout, this, &Neureset::decreaseBatteryByTime);
    connect(disconnectTimer, &QTimer::timeout, this, &Neureset::shutDown);
    connect(beepTimer, &QTimer::timeout, this, &Neureset::beep);
    connect(sessionTimer, &QTimer::timeout, this, &Neureset::updateSessionTime);
    //batteryTimer->start(10000); // 10 seconds
    contact = true;
    power = true;

    QObject::connect(&headset, &EEGHeadset::sendBaseline, this, &Neureset::baselineReceived);
    connect(this, &Neureset::updateSessionPaused, &headset, &EEGHeadset::recieveSessionPaused);
}

Neureset::~Neureset()
{
    for(int i=0; i<sessions.size(); ++i){
        delete sessions[i];
    }
    sessions.clear();

}


void Neureset::menuButtonPressed(){
    qInfo("menuButtonPressed from neureset class");
}

void Neureset::downButtonPressed(){
    qInfo("downButtonPressed from neureset class");
    //needed to tell neureset which sessions to query when user is scrolling through session log
}

void Neureset::upButtonPressed(){
    qInfo("upButtonPressed from neureset class");
    //needed to tell neureset which sessions to query when user is scrolling through session log
}

void Neureset::pauseButtonPressed(){
    qInfo("pauseButtonPressed from neureset class");
    sessionsPaused = true;
    sessionTimer->stop();
    emit updateSessionPaused(sessionsPaused);
}

void Neureset::powerButtonPressed(){
    power = !power;
    qInfo("powerButtonPressed from neureset class");
}

void Neureset::startButtonPressed(){
    mutex.lock();
    qInfo("startButtonPressed from neureset class");
    //add handling so that this function only starts the session if "new session" is currently selected
    if(sessionsPaused){
        sessionsPaused = false;
        sessionTimer->start();
        emit updateSessionPaused(sessionsPaused);
        mutex.unlock();
        return;
    }

    sessionsPaused = false;
    emit updateSessionPaused(sessionsPaused);
    mutex.unlock();
    startSession();
}

void Neureset::stopButtonPressed(){
    qInfo("stopButtonPressed from neureset class");
}

void Neureset::selectButtonPressed(){
    qInfo("selectButtonPressed from neureset class");
}

void Neureset::disconnectButtonPressed(){
    qInfo("disconnectButtonPressed from neureset class");
    disconnectTimer->start(10000); // 10 seconds
    beepTimer->start(2000);
    contact = false;
}

void Neureset::reconnectButtonPressed(){
    qInfo("reconnectButtonPressed from neureset class");
    disconnectTimer->stop();
    beepTimer->stop();
    contact = true;
}

void Neureset::startSession()
{
    mutex.lock();

    baselines.clear();

    //qInfo("in startSession"); //debugging
    Session* currSession = new Session(this);
    sessions.push_back(currSession);
    currentSession = currSession;

    currentSession->setDateTime(dateTime);

    //float baselineBefore = headset.measureBaseline();
    headset.measureBaseline();
    //currSession->setBaselineBefore(baselineBefore);

    mutex.unlock();

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

        if(battery > 0 && battery <= 10 && !connectionLight->isFlashing())
        {
            connectionLight->startFlashing();
            beepTimer->start(2000);
        }
        else if(battery <= 0)
        {
            battery = 0;
            beepTimer->stop();
            disconnectTimer->stop();
            mutex.unlock();
            shutDown();
        }
    }

    mutex.unlock();

    // Potentially update battery level display or trigger low battery warning here
}

// Method to decrease battery because of time
void Neureset::decreaseBatteryByTime() {

    while(batteryThread->isRunning())
    {
        batteryThread->sleep(5);
        decreaseBattery(1);
    }
}

void Neureset::shutDown()
{
    eraseSessionData();
    power = false;
    //batteryTimer->stop();
    //beepTimer->stop();
    //disconnectTimer->stop();
    emit connectionLost();
}

void Neureset::eraseSessionData()
{
    sessions.clear();
}

void Neureset::updateProgress(int prog)
{
    mutex.lock();
    currentSession->updateProgress(prog);
    mutex.unlock();
}

void Neureset::updateProgressByTime()
{
    while(progressThread->isRunning())
    {
        if(sessionsPaused){
            continue;
        }
        progressThread->sleep(1);
        updateProgress(2);
    }
}

void Neureset::updateSessionTime()
{
    sessionTime += 1;
}

QString Neureset::getCurrSessionTime()
{
    int seconds = sessionTime % 60;
    int minutes = (sessionTime - seconds) / 60;
    return QStringLiteral("%1:%2").arg(minutes).arg(seconds, 2, 10, QLatin1Char('0'));
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
        headset.beginTreatment(i);

        //measure the baseline for the next round
        headset.measureBaseline();
    }
    //otherwise, we have just calculated the final baseline. store received baseline in the afterBaseline variable and finish the session
    else if (baselines.size() == 5){
        currentSession->setBaselineBefore(baselines[0]);
        currentSession->setBaselineAfter(base);
        currentSession->print();

        mutex.unlock();

        //decrease battery by 10% every session
        decreaseBattery(10);

        //session is now done, top off progress to 100%
        updateProgress(100);
        progressThread->quit();

        sessionTimer->stop();


        emit sessionComplete();
    }
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
