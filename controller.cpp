#include "controller.h"

Controller::Controller(QObject *parent)
    : QObject{parent}
{

}
Controller::~Controller(){
    //I think QObjects delete all their children so we don't need to delete mainwindow and neureset, I could be wrong tho
}

void Controller::launch(){
    //connect all of the button press signals from main window to neureset slots
    //these slots will eventually be replaced by the functions that actually handle the functionality (startSession etc), I just gave them the same name for now so it's easy to read/track
    QObject::connect(&w, &MainWindow::menuButtonPressed, &neureset, &Neureset::menuButtonPressed);
    QObject::connect(&w, &MainWindow::downButtonPressed, &neureset, &Neureset::downButtonPressed);
    QObject::connect(&w, &MainWindow::upButtonPressed, &neureset, &Neureset::upButtonPressed);
    QObject::connect(&w, &MainWindow::pauseButtonPressed, &neureset, &Neureset::pauseButtonPressed);
    QObject::connect(&w, &MainWindow::powerButtonPressed, &neureset, &Neureset::powerButtonPressed);
    QObject::connect(&w, &MainWindow::startButtonPressed, &neureset, &Neureset::startButtonPressed);
    QObject::connect(&w, &MainWindow::stopButtonPressed, &neureset, &Neureset::stopButtonPressed);

    w.show();
}
