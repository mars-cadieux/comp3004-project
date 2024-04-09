#include "neureset.h"

Neureset::Neureset()
{
    connectionLight = new DeviceLight();
    contactLight = new DeviceLight();
    tsLight = new DeviceLight();
    battery = 100;
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
    qInfo("stopButtonPressed from neureset class");
}

void Neureset::startSession()
{
    qInfo("in startSession"); //debugging
    Session* currSession = new Session(this);
    sessions.push_back(currSession);

    currSession->setDateTime(dateTime);

    float baselineBefore = headset.measureBaseline();
    currSession->setBaselineBefore(baselineBefore);
    //do the treatment
    float baselineAfter = headset.measureBaseline();
    currSession->setBaselineAfter(baselineAfter);

    currSession->print();
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
