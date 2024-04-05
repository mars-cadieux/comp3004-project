#ifndef ELECTRODE_H
#define ELECTRODE_H

#include <QObject>

class Electrode
{
    Q_OBJECT
public:
    Electrode();

private:
    void attach();
    void applyOffsetFrequency(float frequency);
    void receiveBrainwave(float frequency);

signals:
    void sendBrainwave();
};

#endif // ELECTRODE_H
