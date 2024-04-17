#ifndef ELECTRODETHREAD_H
#define ELECTRODETHREAD_H

#include <QObject>
#include <QThread>
#include <QFloat16>

/* ElectrodeThread Class
 * Class Purpose: Manages runtime of an Electrode during a session so it can be easily stopped/started and delayed for treatment time
 *
 * Data Members:
 * Electrode electrode: Pointer to the electrode object the thread is doing the work for
 * sessionPaused_: true if session is paused
 * sessionStopped_: true if session is stopped
 *
 * Class Functions:
 * void run(): Runs the session process for electrode, emits signal to headset when process is finished.
 * void delay(): Adds a delay to simulate the electrode taking the time to calculate frequency
 *
 * void updateSessionPaused: Updates pause status
 * void updateSessionStopped: Updates stopped status
 *
 * Signals:
 * resultReady(): Signals to headset that the electrode has calculated a result.
 * */

class Electrode;

class ElectrodeThread : public QThread
{
    Q_OBJECT
public:
    explicit ElectrodeThread(QObject *parent, Electrode* elec);
    void run() override;

signals:
    void resultReady(const QString &s);

private:
    Electrode* electrode; //pointer to the electrode that this thread is doing the work for
    void delay(int seconds); //adds a delay, used to simulate the electrode taking 1 min to calculate frequency
    bool sessionPaused_ = false;
    bool sessionStopped_ = false;

public slots:
    void updateSessionPaused(bool sessionPaused);
    void updateSessionStopped(bool sessionStopped);

};

#endif // ELECTRODETHREAD_H
