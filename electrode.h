#ifndef ELECTRODE_H
#define ELECTRODE_H

#include <QObject>
#include <QVector>

//Structure for the sine wave, including frequency and amplitude.
//Changes to be made in the future if sine waves need more detail?
struct Sinewave {
    float frequency;
    float amplitude;
};

class Electrode : public QObject
{
    Q_OBJECT

public:
    explicit Electrode(QObject *parent = nullptr);
    QVector<Sinewave> receiveBrainwave();
    void applyOffsetFrequency(float frequency, int i);

private:
    void attach();
    QVector<Sinewave> brainwave;

signals:
    void sendBrainwave(QVector<Sinewave> brainwave);
};

#endif // ELECTRODE_H
