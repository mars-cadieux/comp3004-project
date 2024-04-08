#ifndef NEURESET_H
#define NEURESET_H

#include <QObject>
#include <QVector>

#include "eegheadset.h"
#include "electrode.h" //I don't think the neureset should have direct access to the electrodes, the headset should be a mediator between the two and the neureset shouldn't be aware of the electrodes existence. open to discussion though ofc. -mars
#include "session.h"
#include "devicelight.h"

class Neureset: public QObject
{
    Q_OBJECT
public:
    Neureset();
    ~Neureset();
    void setDateTime(QDateTime);
    void receiveDataRequest();

signals:
    void uploadData(QVector<Session*> sessions);

public slots:
    void menuButtonPressed();
    void downButtonPressed();
    void upButtonPressed();
    void pauseButtonPressed();
    void powerButtonPressed();
    void startButtonPressed();
    void stopButtonPressed();
    void selectButtonPressed();
    DeviceLight* getConnLight();
    float getBattery();

private:
    EEGHeadset headset;
    QVector<Session*> sessions;
    DeviceLight* connectionLight;
    QDateTime dateTime;
    float battery;

    void startSession();
    float measureBaseline();
    void startTreatment(Electrode electrode);
    void baseLineCalculated();
    void treatmentStarted();
    void treatmentFinished();
    void electrodeFinished();

};

#endif // NEURESET_H
