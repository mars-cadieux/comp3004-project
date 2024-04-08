#include "eegheadset.h"

EEGHeadset::EEGHeadset(QObject *parent)
    : QObject{parent}
{
//Create electrodes here from the main branch
    for(int i=0; i<NUM_ELECTRODES; ++i){
        Electrode* elec = new Electrode();
        electrodes.push_back(elec);
    }
}

EEGHeadset::~EEGHeadset()
{
//Clear electrodes here from the main branch
    for(int i=0; i<electrodes.size(); ++i){
        delete electrodes[i];
    }
    electrodes.clear();
}

//Signal processing AND calculating dominant frequency from the signals.
//Stores the hz for each electrode, and dominant frequency
void EEGHeadset::measureFrequency()
{
    for (int i = 0; i < electrodes.size(); i++) {

        QVector<Sinewave> brainwave = electrodes[i]->receiveBrainwave();

        //Necessary for the dominant frequency formula posted on neureset testing
        float sumsSquared = 0.0;
        float weightedFrequencySum = 0.0;

        for (int j = 0; j < 4; j++) {
            //"Processing" the signal from frequency to hz, and grabbing the amplitude for the dominant frequency formula
            float frequencyHz = brainwave[j].frequency;
            float amplitude = brainwave[j].amplitude;

            //Storing the hz for each band
            frequenciesHz[i][j] = frequencyHz;

            //Calculations for dominant frequency formula
            sumsSquared += amplitude * amplitude;
            weightedFrequencySum += frequencyHz * amplitude * amplitude;
        }
        //Storing the dominant frequencies
        dominantFrequencies[i] = weightedFrequencySum / sumsSquared;
    }
}
//Finds the average of the dominant frequencies
float EEGHeadset::measureBaseline() {
    measureFrequency();
    float sum = 0.0;

    //Can modify this for more dynamic number of electrodes
    for (int i = 0; i < NUM_ELECTRODES; i++) {
        sum += dominantFrequencies[i];
    }
    float overallBaseline = sum / NUM_ELECTRODES;
    return overallBaseline;
}
