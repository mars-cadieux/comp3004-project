#ifndef EEGHEADSET_H
#define EEGHEADSET_H

#include "electrode.h"
#include "defs.h"
#include "session.h"

#include <QObject>
#include <QVector>
#include <QThread>

/* EEGHeadset Class
 * Class Purpose: Contains the electrodes, calculates and applies treatment.
 *
 * Data Members:
 * QVector electrodes: Contains the electrode objects
 * QThread electrodeThreads[]: Contains the threads for running the electrodes asynchronously.
 * float frequenciesHz[][4]: Contains an array of float arrays, each containing frequencies for all 4 measured brainwave wavelengths for an electrode.
 * float dominantFrequencies[]: Contains an array of the dominant frequencies for each electrode
 * float baseline: Stores the calculates baseline frequency
 *
 * Class Functions:
 * Getters for Electrodes and baseline value
 * void measureFrequency: Removed for redundancy
 * void measureBaseline: Begins threads for each electrode, which will collect brainwave data at each site. Brainwaves are sent to calculateDominantFreq through a signal when finished
 * void beginTreatment: Sends offset frequencies to each electrode for treatment.
 * bool allDone(): Returns true if all Electrodes have finished calculating their dominant frequency.
 *
 * void calculateDominantFreq(): Called by electrode signal, calculates the dominant frequency from collected brainwave data and checks if all calculations are complete before calculating baseline.
 * void calculateBaseline(): Called when all dominant freqs are calculated, determines baseline frequency from collected data.
 * void recieveSessionPaused(): Called by pause signal from user interface, suspends session until start signal is recieved again.
 * void recieveSessionStopped(): Called by stop signal from user interface, ends the session early.
 *
 * Class Signals:
 * sendBaseline(): Lets the Neureset know that the baseline can be retrieved.
 * allFrequenciesReceived: Not in use.
 * updateSessionPaused: Allows the headset to communicate to Electrodes that a session has been paused.
 * updateSessionStopped: Allows the headset to communicate to Electrodes that a session has been stopped.
 * */

class EEGHeadset : public QObject
{
    Q_OBJECT
public:
    explicit EEGHeadset(QObject *parent = nullptr);
    ~EEGHeadset();
    //Signal processing
    void measureFrequency();
    void measureBaseline();
    void beginTreatment(int count);
    Electrode* getElectrodeById(const QString& s);
    bool allDone() const;

    float getBaseline() const;

private:
    QVector<Electrode*> electrodes;
    QThread* electrodeThreads[NUM_ELECTRODES];
    //Fixed size frequencies, changes can be made and discussed
    float frequenciesHz[NUM_ELECTRODES][4];
    float dominantFrequencies[NUM_ELECTRODES];
    float baseline;


signals:
    void sendBaseline();
    void allFrequenciesReceived();
    void updateSessionPaused(bool sessionPaused);
    void updateSessionStopped(bool sessionStopped);

public slots:
    void calculateDominantFreq(const QString& s);
    void calculateBaseline();
    void recieveSessionPaused(bool sessionPaused);
    void recieveSessionStopped(bool sessionStopped);

};

#endif // EEGHEADSET_H
