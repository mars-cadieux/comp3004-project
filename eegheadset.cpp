#include "eegheadset.h"
#include "electrodethread.h"

EEGHeadset::EEGHeadset(QObject *parent)
    : QObject{parent}
{
//Create electrodes here from the main branch
    for(int i=0; i<NUM_ELECTRODES; ++i){
        Electrode* elec = new Electrode(this);
        electrodes.push_back(elec);

//        QThread* elecThread = new QThread(this);
//        electrodeThreads.push_back(elecThread);
//        elec->moveToThread(elecThread);

//        connect(elecThread, &QThread::started, elec, &Electrode::receiveBrainwave);
//        connect(elecThread, &QThread::finished, elecThread, &QThread::deleteLater);
//        elecThread->start();
    }
}

EEGHeadset::~EEGHeadset()
{
//Clear electrodes here from the main branch
//    for(int i=0; i<electrodes.size(); ++i){
//        delete electrodes[i];
//    }
//    electrodes.clear();

    //electrode now has headset as parent so they should all get deleted automatically
}

//Signal processing AND calculating dominant frequency from the signals.
//Stores the hz for each electrode, and dominant frequency
//we can comment out this function
//void EEGHeadset::measureFrequency()
//{
//    for (int i = 0; i < electrodes.size(); i++) {

//        //make a new thread and offload the work of this function to said thread
//        ElectrodeThread* elecThread = new ElectrodeThread(this, electrodes[i]);
//        QObject::connect(elecThread, &ElectrodeThread::resultReady, this, &EEGHeadset::calculateDominantFreq);
//        QObject::connect(elecThread, &ElectrodeThread::finished, elecThread, &QObject::deleteLater);
//        elecThread->start();

//        //QVector<Sinewave> brainwave = electrodes[i]->receiveBrainwave();

//    }
//}


//Finds the average of the dominant frequencies
void EEGHeadset::measureBaseline() {
    //measureFrequency();

    //initialize all dominant frequencies to 0. this is needed later on in order to check if all electrodes have finished measuring their dominant frequency
    for(int i=0; i<NUM_ELECTRODES; ++i){
        dominantFrequencies[i] = 0.0;
    }

    for (int i = 0; i < electrodes.size(); ++i) {

        //make a new thread and offload the work of this function to said thread
        ElectrodeThread* elecThread = new ElectrodeThread(this, electrodes[i]);
        QObject::connect(elecThread, &ElectrodeThread::resultReady, this, &EEGHeadset::calculateDominantFreq);
        QObject::connect(elecThread, &ElectrodeThread::finished, elecThread, &QObject::deleteLater);
        elecThread->start();

        //QVector<Sinewave> brainwave = electrodes[i]->receiveBrainwave()
    }

}

//Begins treatment with offset frequency counted from Neureset class
void EEGHeadset::beginTreatment(int count) {
    for (int i = 0; i < NUM_ELECTRODES; i++) {
        //electrodes[i]->applyOffsetFrequency(dominantFrequencies[i] + (count*5), i + 1);
        //changed electrode class so they each have an id and changed function signature so that we print the electrode's id instead of passing in the counter. -mars
        electrodes[i]->applyOffsetFrequency(dominantFrequencies[i] + (count*5));
    }
    qInfo("");
}

//get electrode by id. takes in a qstring because this function is a helper for a thread signal/slot connection, which can't queue int arguments
Electrode *EEGHeadset::getElectrodeById(const QString &s)
{
    //convert the qstring to an int
    bool ok;
    int elecId = s.toInt(&ok);
    //if the conversion failed, print an error message and return a null pointer
    if(!ok){
        qInfo("Invalid electrode ID, could not convert to int.");
        return nullptr;
    }
    //electrode with id 1 has index 0 in array, etc, so we subtract 1 from id
    return electrodes[elecId-1];
}

//checks whether all electrodes have finished measuring their baselines
bool EEGHeadset::allDone() const
{
    for(int i=0; i<NUM_ELECTRODES; ++i){
        if(dominantFrequencies[i] == 0.0){
            return false;
        }
    }
    return true;
}

float EEGHeadset::getBaseline() const
{
    return baseline;
}

void EEGHeadset::calculateDominantFreq(const QString& s)
{
    qInfo("in calculateDominantFreq");
    Electrode* e = getElectrodeById(s);
    int elecId = e->getId();
    QVector<Sinewave> brainwave = e->getBrainwave();
    //Necessary for the dominant frequency formula posted on neureset testing
    float sumsSquared = 0.0;
    float weightedFrequencySum = 0.0;

    for (int j = 0; j < 4; j++) {
        //"Processing" the signal from frequency to hz, and grabbing the amplitude for the dominant frequency formula
        float frequencyHz = brainwave[j].frequency;
        float amplitude = brainwave[j].amplitude;

        //Storing the hz for each band
        frequenciesHz[elecId][j] = frequencyHz;

        //Calculations for dominant frequency formula
        sumsSquared += amplitude * amplitude;
        weightedFrequencySum += frequencyHz * amplitude * amplitude;
    }
    //Storing the dominant frequencies
    dominantFrequencies[elecId] = weightedFrequencySum / sumsSquared;

    if(allDone()){
        calculateBaseline();
    }
}

void EEGHeadset::calculateBaseline()
{
    qInfo("in calculateBaseline");
    float sum = 0.0;

    //Can modify this for more dynamic number of electrodes
    for (int i = 0; i < NUM_ELECTRODES; i++) {
        sum += dominantFrequencies[i];
    }
    baseline = sum / NUM_ELECTRODES;
    emit sendBaseline();
    //return overallBaseline;
}

