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

/* Neureset Class
 * Class Purpose: Simulates the functions of a Neureset device. Takes input from UI, runs sessions, contains session results, controls lights and power status.
 *
 * Data Members:
 * EEGHeadset headset: Contains the headset object.
 * QVector<Session*> sessions: Contains a list of session data
 * Session* currentSession: Contains the current session data during ongoing session.
 *
 * QVector<float> baselines: Stores the baselines calculated before each round of treatment + final baseline
 *
 * DeviceLight* connectionLight: Stores light that signals connection
 * DeviceLight* contactLight: Stores light that signals headset contact
 * DeviceLight* tsLight: Stores light for treatment signal.
 *
 * QDateTime dateTime: Stores current Date
 * int battery: Stores battery percentage as an integer
 * QTimer* batteryTimer: Timer to deduct battery life over time
 * QTimer* disconnectTimer: Timer to end session when headset is disconnected too long
 * QTimer* beepTimer: Timer to output beeps to console if battery is low or headset is disconnected.
 * QTimer* sessionTimer: Timer to update session time.
 * int sessionTime: Holds the current time length of current session
 * bool contact: True if headset is connected
 * bool power: True if battery is above 0
 * bool sessionsPaused: True if session is paused
 * bool sessionStopped: True if session is stopped
 * QMutex mutex: Mutex to perform thread safe actions such as pause and stop.
 * QThread* batteryThread: Thread to run gradual battery decrease asynchronously.
 * QThread* progressThread: Thread to run progress bar UI asynchronously.
 *
 * Class Functions:
 * Setters for dateTime and battery
 * Getters for lights, mutex, session progress, and session time
 * void receiveDataRequest(): Emits a signal to upload stored session data to PC database
 *
 * void startSession(): Begins a session, calls headset to collect baselines.
 * float measureBaseline(): Not currently in use
 * void startTreatment(): Not currently in use
 * void baseLineCalculated(): Not currently in use
 * void treatmentStarted(): Not currently in use
 * void treatmentFinished(): Not currently in use
 * void electrodeFinished(): Not currently in use
 * void decreaseBattery(): Decreases battery by given amount
 * void decreaseBatteryByTime(): Run constantly by batteryThread to asynchronously decrease battery over time.
 * void shutDown(): Shuts down the device, erasing any unfinished session data
 * void eraseSessionData(): Erases data of all sessions in array
 * void updateProgress(): Updates progress bar for current session by given amount.
 * void updateProgressByTime(): Updates progress bar from timer to count up during calculations.
 * void delay(int seconds): Delays the calling thread by given time.
 *
 * Class Slots:
 * Slots to handle button functionality
 * Getter for battery
 * void beep(): Outputs beep to console on timer
 * void waveformRequested(): Displays requested waveform to UI.
 *
 * Class Signals:
 * void uploadData(): Uploads session data to PC database
 * void connectionLost(): Informs headset that it has lost connection
 * void sessionComplete(): Signals session end
 * void updateSessionPaused(): Signals session paused
 * void updateSessionStopped()" Signals session stopped
 * void sendBrainwave(): Sends a brainwave for display on UI
 * */

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
    QVector<Session*>* getSessions();
    void setBattery(int percent);
    QMutex* getMutex();
    float getCurrSessionProgress();
    QString getCurrSessionTime();

signals:
    void uploadData(QVector<Session*> sessions);
    void connectionLost();
    void sessionComplete();
    void updateSessionPaused(bool sessionPaused);
    void updateSessionStopped(bool sessionStopped);
    void sendBrainwave(QVector<Sinewave> bWave);

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
    void waveformRequested(const QString& elecNum);


    void updateSessionTime();
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
    QTimer* sessionTimer;
    QTimer* pauseTimer;
    int sessionTime;
    bool contact;
    bool power;
    bool sessionsPaused;
    bool sessionStopped;
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
    void pauseSession();
};

#endif // NEURESET_H
