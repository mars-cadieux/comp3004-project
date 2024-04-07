#include "neureset.h"

Neureset::Neureset()
{
    connectionLight = new DeviceLight();
    battery = 100;
}

Neureset::~Neureset()
{
    for(unsigned int i=0; i<sessions.size(); ++i){
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
}

void Neureset::stopButtonPressed(){
    qInfo("stopButtonPressed from neureset class");
}

void Neureset::selectButtonPressed(){
    qInfo("stopButtonPressed from neureset class");
}

void Neureset::startSession()
{
    Session* currSession = new Session(this);
    sessions.push_back(currSession);

    float baselineBefore = headset.measureBaseline();
    currSession->setBaselineBefore(baselineBefore);
    //do the treatment
    float baselineAfter = headset.measureBaseline();
    currSession->setBaselineAfter(baselineAfter);
}

DeviceLight* Neureset::getConnLight()
{
    return connectionLight;
}

void Neureset::setDateTime(QDate newDate, QTime newTime)
{
    date = newDate;
    time = newTime;
    qInfo(date.toString().toStdString().c_str());
}

float Neureset::getBattery()
{
    return battery;
}
