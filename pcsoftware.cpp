#include "pcsoftware.h"
#include "session.h"
#include "pcui.h"

PCSoftware::PCSoftware(QObject *parent)
    : QObject{parent}
{
    pcui = new PCui(nullptr, this);
    pcui->show();
}

PCSoftware::~PCSoftware()
{
    for(int i=0; i<sessionData.size(); ++i){
        delete sessionData[i];
    }
    sessionData.clear();

    delete pcui;
}

void PCSoftware::handleDownloadButton()
{
    emit requestData();
}

void PCSoftware::printSessions() const
{
    for(int i=0; i<sessionData.size(); ++i){
        sessionData[i]->print();
    }
}

void PCSoftware::downloadData(QVector<Session *> sessions)
{
    //make a deep copy of each session and store it
    for(int i=0; i<sessions.size(); ++i){
        if(!(sessions[i]->isUploaded())){
            Session* newSession = new Session(*(sessions[i]));
            sessionData.push_back(newSession);
            sessions[i]->setuploaded(true);
        }
    }
    //printSessions();
    pcui->updateView(sessionData);
}
