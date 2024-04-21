#include "controller.h"
#include "mainwindow.h"

Controller::Controller(MainWindow* w, QObject *parent)
    : QObject{parent}
{
    window = w;
}
Controller::~Controller(){
    //I think QObjects delete all their children so we don't need to delete mainwindow and neureset, I could be wrong tho
}

void Controller::launch(){
    //connect all of the button press signals from main window to neureset slots
    //these slots will eventually be replaced by the functions that actually handle the functionality (startSession etc), I just gave them the same name for now so it's easy to read/track
    //QObject::connect(window, &MainWindow::menuButtonPressed, &neureset, &Neureset::menuButtonPressed);
    //QObject::connect(window, &MainWindow::downButtonPressed, &neureset, &Neureset::downButtonPressed);
    //QObject::connect(window, &MainWindow::upButtonPressed, &neureset, &Neureset::upButtonPressed);
    QObject::connect(window, &MainWindow::pauseButtonPressed, &neureset, &Neureset::pauseButtonPressed);
    QObject::connect(window, &MainWindow::powerButtonPressed, &neureset, &Neureset::powerButtonPressed);
    QObject::connect(window, &MainWindow::startButtonPressed, &neureset, &Neureset::startButtonPressed);
    QObject::connect(window, &MainWindow::stopButtonPressed, &neureset, &Neureset::stopButtonPressed);
    QObject::connect(window, &MainWindow::selectButtonPressed, &neureset, &Neureset::selectButtonPressed);
    QObject::connect(window, &MainWindow::disconnectButtonPressed, &neureset, &Neureset::disconnectButtonPressed);
    QObject::connect(window, &MainWindow::reconnectButtonPressed, &neureset, &Neureset::reconnectButtonPressed);

    //connect signals and slots for plotting the waveforms
    QObject::connect(window, &MainWindow::showWaveform, &neureset, &Neureset::waveformRequested);
    QObject::connect(&neureset, &Neureset::sendBrainwave, window, &MainWindow::updateGraph);

    QObject::connect(&neureset, &Neureset::connectionLost, window, &MainWindow::turnOff);
    QObject::connect(&neureset, &Neureset::sessionComplete, window, &MainWindow::sessionComplete);

    //connect signals and slots for updating widgets such as battery bar, progress bar, etc
    QObject::connect(&neureset, &Neureset::batteryChanged, window, &MainWindow::updateBattery);

    //connect the necessary signals/slots between the PCSoftware and Neureset classes
    QObject::connect(&neureset, &Neureset::uploadData, &pcSoft, &PCSoftware::downloadData);

    window->show();
}

Neureset* Controller::getNeureset(){
    return &neureset;
}

PCSoftware* Controller::getPCSoftware(){
    return &pcSoft;
}
