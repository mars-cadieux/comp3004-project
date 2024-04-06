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
    qfloat16 result;
    qInfo("Reading frequency..."); //we should see 21 of these printed

    //using a 10 second delay instead of a whole minute
    delay(10);

    float freq = electrode->receiveBrainwave();

    //I'm unsure if I'm using this function correctly, I'm trying to convert a float to a qFloat16 but this function needs a third argument which represents the number of allocated entries and I'm unsure what that means
    //we want this thread to emit the frequency read by the electrode. a QThread has to emit some kind of Qobject though, it can't just send a regular float/string etc. in A3 I had it emit a QString which indicated which elevator a request got allocated to
    qFloatToFloat16(&result, &freq, 16);
    emit resultReady(result);
}

//adds a delay for the specified number of seconds
void ElectrodeThread::delay(int seconds)
{
    QTime dieTime = QTime::currentTime().addSecs(seconds);
    while (QTime::currentTime() < dieTime){
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}


