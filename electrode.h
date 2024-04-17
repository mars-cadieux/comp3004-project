#ifndef ELECTRODE_H
#define ELECTRODE_H

#include <QObject>
#include <QVector>
#include <QThread>

/* Electode Class
 * Class Purpose: Holds the info for each Electrode, which collects (generates) data and applies treatment to an individual site.
 *
 * Data Members:
 * QVector<Sinewave> brainwave: Contains brainwave data collected during a session
 * const int id: Contains the unique identifier for this electrode
 * static int nextID: Contains the next ID in the list of electrodes
 * QThread zapThread: Thread to allow electrode to asynchronously collect data and apply treatment with other electrodes
 *
 * Class Functions:
 * Getters for id and brainwave
 * void receiveBrainwave(): Generates brainwave data at 4 different wavelengths, then stores it in brainwave vector
 * void applyOffsetFrequency(): Initializes zapThread to apply treatment asynchronously to Electrode site
 * void zap(): Called by applyOffsetFrequency, outputs to console the signal frequency it emits
 * void attach(): Not currently in use
 *
 * Class Signals:
 * sendBrainwave(): Not currently in use
 * */

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
    void receiveBrainwave();
    void applyOffsetFrequency(float frequency);
    void zap(float frequency = 0.0);

    int getId() const;
    QVector<Sinewave> getBrainwave() const;

private:
    void attach();
    QVector<Sinewave> brainwave;
    const int id;
    static int nextID;

    QThread* zapThread;

signals:
    void sendBrainwave(QVector<Sinewave> brainwave);
};

#endif // ELECTRODE_H
