#include "controller.h"

Controller::Controller(QObject *parent)
    : QObject{parent}
{

}
Controller::~Controller(){
    //I think QObjects delete all their children so we don't need to delete mainwindow and neureset, I could be wrong tho
}

void Controller::launch(){
    QObject::connect(&w, &MainWindow::menuButtonPressed, &neureset, &Neureset::stopButtonPressed);
    QObject::connect(&w, &MainWindow::downButtonPressed, &neureset, &Neureset::startButtonPressed);
    QObject::connect(&w, &MainWindow::upButtonPressed, &neureset, &Neureset::powerButtonPressed);
    QObject::connect(&w, &MainWindow::pauseButtonPressed, &neureset, &Neureset::pauseButtonPressed);
    QObject::connect(&w, &MainWindow::powerButtonPressed, &neureset, &Neureset::upButtonPressed);
    QObject::connect(&w, &MainWindow::startButtonPressed, &neureset, &Neureset::downButtonPressed);
    QObject::connect(&w, &MainWindow::stopButtonPressed, &neureset, &Neureset::menuButtonPressed);

    w.show();
}
