#include "electrode.h"
#include <QRandomGenerator>

Electrode::Electrode(QObject *parent)
{

}

QVector<Sinewave> Electrode::receiveBrainwave() {

    //Clear any previous brainwaves
    brainwave.clear();

    //Frequency ranges for delta, theta, alpha, beta
    float frequencies[4][2] = {
        {1, 4},
        {4, 8},
        {8, 12},
        {12, 30}
    };

    //"Realistic" amplitudes for delta, theta, alpha, beta
    float amplitudes[4][2] = {
        {20, 100},
        {5, 10},
        {5, 50},
        {5, 20}
    };

    //Generating a random frequency and amplitude for each frequency band in the brainwave
    for (int i = 0; i < 4; i++) {
        float minF = frequencies[i][0];
        float maxF = frequencies[i][1];
        float minA = amplitudes[i][0];
        float maxA = amplitudes[i][1];

        float frequency = minF + static_cast<float>(QRandomGenerator::global()->generateDouble() * (maxF - minF));
        float amplitude = minA + static_cast<float>(QRandomGenerator::global()->generateDouble() * (maxA - minA));

        brainwave.push_back({frequency, amplitude});
    }
    //Can remove this emit part here and in header files
    emit sendBrainwave(brainwave);
    return brainwave;
}
