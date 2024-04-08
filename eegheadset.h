#ifndef EEGHEADSET_H
#define EEGHEADSET_H

#include "electrode.h"
#include "defs.h"

#include <QObject>
#include <QVector>

class EEGHeadset : public QObject
{
    Q_OBJECT
public:
    explicit EEGHeadset(QObject *parent = nullptr);
    ~EEGHeadset();
    //Signal processing
    void measureFrequency();
    float measureBaseline();

private:
    QVector<Electrode*> electrodes;
    //Fixed size frequencies, changes can be made and discussed
    float frequenciesHz[NUM_ELECTRODES][4];
    float dominantFrequencies[NUM_ELECTRODES];

signals:
    void sendHz();

};

#endif // EEGHEADSET_H
