#ifndef NEURESET_H
#define NEURESET_H

#include <QObject>
#include "eegheadset.h"
#include "electrode.h"

class Neureset: public QObject
{
    Q_OBJECT
public:
    Neureset();

public slots:
    void menuButtonPressed();
    void downButtonPressed();
    void upButtonPressed();
    void pauseButtonPressed();
    void powerButtonPressed();
    void startButtonPressed();
    void stopButtonPressed();

private:
    void startSession();
    float measureBaseline;
    void startTreatment(Electrode electrode);
    void baseLineCalculated();
    void treatmentStarted();
    void treatmentFinished();
    void electrodeFinished();

};

#endif // NEURESET_H
