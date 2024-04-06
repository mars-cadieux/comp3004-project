#include "eegheadset.h"
#include "defs.h"

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

    float avg = 0.0; //placeholder for now
    return avg;
}
