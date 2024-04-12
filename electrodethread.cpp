#include "electrodethread.h"
#include "electrode.h"
#include <QTime>
#include <QCoreApplication>

ElectrodeThread::ElectrodeThread(QObject *parent, Electrode *elec)
    : QThread{parent}
{
    electrode = elec;
}

void ElectrodeThread::run()
{

    int elecId = electrode->getId();
    qInfo("Electrode %d reading frequency...", elecId); //we should see 7 of these printed

    //using a 5 second delay
    delay(5);
    while(sessionPaused_){
        qInfo("session paused");
        delay(1);
    }
    electrode->receiveBrainwave(); //the brainwave will be stored in the electrode object

    emit resultReady(QString::number(elecId));
    //qInfo("result ready");
}

//adds a delay for the specified number of seconds
void ElectrodeThread::delay(int seconds)
{
    QTime dieTime = QTime::currentTime().addSecs(seconds);
    while (QTime::currentTime() < dieTime){
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}

void ElectrodeThread::updateSessionPaused(bool sessionPaused){
    sessionPaused_ = sessionPaused;
}


