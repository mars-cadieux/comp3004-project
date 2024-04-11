#ifndef NEURESET_H
#define NEURESET_H

#include <QObject>
#include <QVector>
#include <QThread>

#include "eegheadset.h"
#include "electrode.h"
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
    float getCurrSessionProgress();



signals:
    void uploadData(QVector<Session*> sessions);
    void connectionLost();
    void sessionComplete();

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


    void baselineReceived();

private:
    EEGHeadset headset;
    QVector<Session*> sessions;
    Session* currentSession;

    QVector<float> baselines;   //stores the baselines calculated before each round of treatment + final baseline

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
    QThread* batteryThread;
    QThread* progressThread;

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
    void updateProgress(int prog);
    void updateProgressByTime();
    void delay(int seconds);
};

#endif // NEURESET_H
