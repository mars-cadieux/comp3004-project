#ifndef EEGHEADSET_H
#define EEGHEADSET_H

#include "electrode.h"
#include "defs.h"
#include "session.h"

#include <QObject>
#include <QVector>
#include <QThread>

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
