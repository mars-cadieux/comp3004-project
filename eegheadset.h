#ifndef EEGHEADSET_H
#define EEGHEADSET_H

#include <QObject>
#include "electrode.h"
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
    float frequenciesHz[7][4];
    float dominantFrequencies[7];

signals:
    void sendHz();


};

#endif // EEGHEADSET_H
