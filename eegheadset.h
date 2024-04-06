#ifndef EEGHEADSET_H
#define EEGHEADSET_H

#include "electrode.h"

#include <QObject>

class EEGHeadset: public QObject
{
    Q_OBJECT
public:
    explicit EEGHeadset(QObject *parent = nullptr);
    ~EEGHeadset();
    float measureBaseline();

private:
    std::vector<Electrode*> electrodes;

//public slots:
    //void measureFrequency();

signals:
    void sendHz();


};

#endif // EEGHEADSET_H
