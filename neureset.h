#ifndef NEURESET_H
#define NEURESET_H

#include <QObject>
#include <QVector>

#include "eegheadset.h"
#include "electrode.h" //I don't think the neureset should have direct access to the electrodes, the headset should be a mediator between the two and the neureset shouldn't be aware of the electrodes existence. open to discussion though ofc. -mars
#include "session.h"
#include "devicelight.h"
#include <QTimer>
#include <QMutex>

class Neureset: public QObject
{
    Q_OBJECT
public:
    Neureset();
    ~Neureset();
    void setDateTime(QDateTime);
    void receiveDataRequest();
    DeviceLight* getConnLight();
    DeviceLight* getContactLight();
    DeviceLight* getTSLight();
    void setBattery(int percent);
    QMutex* getMutex();

signals:
    void uploadData(QVector<Session*> sessions);
    void connectionLost();

public slots:
    void menuButtonPressed();
    void downButtonPressed();
    void upButtonPressed();
    void pauseButtonPressed();
    void powerButtonPressed();
    void startButtonPressed();
    void stopButtonPressed();
    void selectButtonPressed();
    void disconnectButtonPressed();
    void reconnectButtonPressed();
    float getBattery();
    void beep();

private:
    EEGHeadset headset;
    QVector<Session*> sessions;
    DeviceLight* connectionLight;
    DeviceLight* contactLight;
    DeviceLight* tsLight;
    QDateTime dateTime;
    int battery;
    QTimer* batteryTimer;
    QTimer* disconnectTimer;
    QTimer* beepTimer;
    bool contact;
    bool power;
    QMutex mutex;

    void startSession();
    float measureBaseline();
    void startTreatment(Electrode electrode);
    void baseLineCalculated();
    void treatmentStarted();
    void treatmentFinished();
    void electrodeFinished();
    void decreaseBattery(int decreaseAmount);
    void decreaseBatteryByTime();
    void shutDown();
    void eraseSessionData();
};

#endif // NEURESET_H
