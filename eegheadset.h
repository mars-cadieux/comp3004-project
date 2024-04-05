#ifndef EEGHEADSET_H
#define EEGHEADSET_H

#include <QObject>

class EEGHeadset
{
    Q_OBJECT
public:
    EEGHeadset();

private:

public slots:
    void measureFrequency();

signals:
    void sendHz();


};

#endif // EEGHEADSET_H
