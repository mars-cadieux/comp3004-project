#ifndef EEGHEADSET_H
#define EEGHEADSET_H

#include "electrode.h"
#include "electrodethread.h"

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
    std::vector<float> frequencies;

public slots:
    void handleResults(qfloat16 freq); //handle results of the electrode thread

signals:
    void sendHz();


};

#endif // EEGHEADSET_H
