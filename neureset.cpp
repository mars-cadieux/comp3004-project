#include "neureset.h"

Neureset::Neureset()
{

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
