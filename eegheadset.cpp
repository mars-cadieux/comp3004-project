#include "eegheadset.h"
#include "defs.h"

#include <QFloat16>

EEGHeadset::EEGHeadset(QObject *parent)
    : QObject{parent}
{
    for(int i=0; i<NUM_ELECTRODES; ++i){
        Electrode* electrode = new Electrode();
        electrodes.push_back(electrode);
    }
}

EEGHeadset::~EEGHeadset()
{
    for(unsigned int i=0; i<electrodes.size(); ++i){
        delete electrodes[i];
    }
    electrodes.clear();
}

float EEGHeadset::measureBaseline()
{

    //loop through all electrodes, start work in a thread, store return values in a vector and then calculate average. return average
    for(unsigned int i=0; i<electrodes.size(); ++i){
        ElectrodeThread* eThread = new ElectrodeThread(this, electrodes[i]);
        QObject::connect(eThread, &ElectrodeThread::resultReady, this, &EEGHeadset::handleResults);
        QObject::connect(eThread, &ElectrodeThread::finished, eThread, &QObject::deleteLater);
    }

    //I didn't set this up very well, with the way I did the threading we now can't return the average from within this function. will need some time to think of how to refactor this

    float avg = 0.0; //placeholder for now
    return avg;
}

void EEGHeadset::handleResults(qfloat16 freq)
{
    float convertedFreq;
    //convert the qfloat to a regular float
    //again, unsure if I'm using the third parameter correctly
    qFloatFromFloat16(&convertedFreq, &freq, 16);

    frequencies.push_back(convertedFreq);
    if(frequencies.size() == NUM_ELECTRODES){
        //calculate average and somehow return it
    }
}
