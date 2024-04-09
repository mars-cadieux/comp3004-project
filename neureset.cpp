#include "neureset.h"


Neureset::Neureset()
{
    connectionLight = new DeviceLight();
    contactLight = new DeviceLight();
    tsLight = new DeviceLight();
    battery = 100;

    //timer to decrease battery life by 1 percent every 10 seconds
    batteryTimer = new QTimer(this);
    disconnectTimer = new QTimer(this);
    beepTimer = new QTimer(this);
    connect(batteryTimer, &QTimer::timeout, this, &Neureset::decreaseBatteryByTime);
    connect(disconnectTimer, &QTimer::timeout, this, &Neureset::shutDown);
    connect(beepTimer, &QTimer::timeout, this, &Neureset::beep);
    batteryTimer->start(10000); // 10 seconds
    contact = true;
    power = true;

    QObject::connect(&headset, &EEGHeadset::sendBaseline, this, &Neureset::baselineReceived);
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
}

void Neureset::powerButtonPressed(){
    power = !power;
    qInfo("powerButtonPressed from neureset class");
}

void Neureset::startButtonPressed(){
    qInfo("startButtonPressed from neureset class");
    //add handling so that this function only starts the session if "new session" is currently selected
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

    //qInfo("in startSession"); //debugging
    Session* currSession = new Session(this);
    sessions.push_back(currSession);
    currentSession = currSession;

    currentSession->setDateTime(dateTime);

    //float baselineBefore = headset.measureBaseline();
    headset.measureBaseline();
    //currSession->setBaselineBefore(baselineBefore);

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

    if(battery < 0)
    {
        battery = 0;
    }

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
            shutDown();
        }
    }

    mutex.unlock();

    // Potentially update battery level display or trigger low battery warning here
}

// Method to decrease battery because of time
void Neureset::decreaseBatteryByTime() {
    decreaseBattery(1);
    batteryTimer->start(10000);
}

void Neureset::shutDown()
{
    eraseSessionData();
    power = false;
    batteryTimer->stop();
    beepTimer->stop();
    disconnectTimer->stop();
    emit connectionLost();
}

void Neureset::eraseSessionData()
{
    sessions.clear();
}

void Neureset::beep()
{
    qInfo("*BEEP*");
    beepTimer->start(2000);
}

void Neureset::baselineReceived()
{
    //get the baseline from the headset (the sender of the signal)
    EEGHeadset* hs = qobject_cast<EEGHeadset*>(sender());
    float base = hs->getBaseline();

    //if the "before" baseline is 0, it hasn't been calculated yet. store received baseline in the beforeBaseline variable and start the treatment
    if(currentSession->getBaselineBefore() == 0.0){
        currentSession->setBaselineBefore(base);

        //do the treatment
        for (int i = 1; i < 5; i++) {
            qInfo("Treatment round %d ",i);
            headset.beginTreatment(i);
        }
        //measure the "after" baseline
        headset.measureBaseline();

        //currentSession->setBaselineAfter(base);
//        currentSession->print();

//        mutex.unlock();
//        //decrease battery by 5% every session
//        decreaseBattery(5);
    }
    //otherwise, we have just calculated the after baseline. store received baseline in the afterBaseline variable and finish the session
    else{
        currentSession->setBaselineAfter(base);
        currentSession->print();

        mutex.unlock();
        //decrease battery by 5% every session
        decreaseBattery(5);
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
