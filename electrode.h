#ifndef ELECTRODE_H
#define ELECTRODE_H

#include <QObject>

class Electrode: public QObject
{
    Q_OBJECT
public:
    Electrode();
    float receiveBrainwave();

private:
    void attach();
    void applyOffsetFrequency(float frequency); //why is this private? I think it should either be public or it should be a slot. open to discussion ofc. - mars


signals:
    void sendBrainwave();


};

#endif // ELECTRODE_H
