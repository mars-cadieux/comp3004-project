#ifndef ELECTRODETHREAD_H
#define ELECTRODETHREAD_H

#include <QObject>
#include <QThread>
#include <QFloat16>


class Electrode;

class ElectrodeThread : public QThread
{
    Q_OBJECT
public:
    explicit ElectrodeThread(QObject *parent, Electrode* elec);
    void run() override;

signals:
    void resultReady(const qfloat16 &freq);

private:
    Electrode* electrode; //pointer to the electrode that this thread is doing the work for
    void delay(int seconds); //adds a delay, used to simulate the electrode taking 1 min to calculate frequency

};

#endif // ELECTRODETHREAD_H
