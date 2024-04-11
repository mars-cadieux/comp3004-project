#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include "session.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->sessionFrame->setVisible(false);
    ui->dateFrame->setVisible(false);
    ui->mainMenu->setCurrentRow(0);
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
    ui->reconnectButton->setEnabled(false);
    control = new Controller(this);
    control->getNeureset()->setDateTime(ui->dateTimeEdit->dateTime());

    connect(ui->menuButton, &QPushButton::clicked, this, &MainWindow::handleMenuButton);
    connect(ui->navigateDown, &QPushButton::clicked, this, &MainWindow::handleNavigateDown);
    connect(ui->navigateUp, &QPushButton::clicked, this, &MainWindow::handleNavigateUp);
    connect(ui->pauseButton, &QPushButton::clicked, this, &MainWindow::handlePauseButton);
    connect(ui->powerButton , &QPushButton::clicked, this, &MainWindow::handlePowerButton);
    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::handleStartButton);
    connect(ui->stopButton, &QPushButton::clicked, this, &MainWindow::handleStopButton);
    connect(ui->selectButton, &QPushButton::clicked, this, &MainWindow::handleSelectButton);
    connect(ui->disconnectButton, &QPushButton::clicked, this, &MainWindow::handleDisconnectButton);
    connect(ui->reconnectButton, &QPushButton::clicked, this, &MainWindow::handleReconnectButton);
    connect(ui->battery10Button, &QPushButton::clicked, this, &MainWindow::handleBattery10Button);
    connect(ui->battery0Button, &QPushButton::clicked, this, &MainWindow::handleBattery0Button);

    windowThread = QThread::create([this]{ updateWindow(); });
    windowThread->start();
    control->launch();

    ui->startButton->setEnabled(false);
    ui->pauseButton->setEnabled(false);
    ui->stopButton->setEnabled(false);
    ui->menuButton->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleMenuButton(){
    //Returns to main menu, despite anything that is currently happening. Does not suspend session as of this time.
    ui->sessionFrame->setVisible(false);
    ui->dateFrame->setVisible(false);
    ui->mainMenu->setVisible(true);
    ui->mainMenu->clear();
    ui->mainMenu->addItem("NEW SESSION");
    ui->mainMenu->addItem("SESSION LOG");
    ui->mainMenu->addItem("TIME AND DATE");
    ui->mainMenu->setCurrentRow(0);
    ui->startButton->setEnabled(false);
    ui->pauseButton->setEnabled(false);
    ui->stopButton->setEnabled(false);
    ui->menuButton->setEnabled(false);
    qInfo()<< "menu button pressed";
    emit menuButtonPressed();
}

void MainWindow::handleNavigateDown(){
    //functionality
    if(ui->mainMenu->currentRow() == ui->mainMenu->count() - 1)
    {
        ui->mainMenu->setCurrentRow(0); // Resets the menu selection back to the top
    }
    else
    {
        ui->mainMenu->setCurrentRow(ui->mainMenu->currentRow() + 1);
    }

    qInfo()<< "navigate down button pressed";
    emit downButtonPressed();
}

void MainWindow::handleNavigateUp(){
    //functionality
    if(ui->mainMenu->currentRow() == 0)
    {
        ui->mainMenu->setCurrentRow(ui->mainMenu->count() - 1);  // Resets the menu selection back to the bottom
    }
    else
    {
        ui->mainMenu->setCurrentRow(ui->mainMenu->currentRow() - 1);
    }

    qInfo()<< "navigate up button pressed";
    emit upButtonPressed();
}

void MainWindow::handlePauseButton(){
    //functionality
    qInfo()<< "pause button pressed";
    emit pauseButtonPressed();
}

void MainWindow::handlePowerButton(){
    //Power off disables all interfaces the user can access on the neureset. When turned back on, it is returned to the main menu.
    power = !power;
    ui->navigateDown->setDisabled(!power);
    ui->navigateUp->setDisabled(!power);
    ui->menuButton->setDisabled(!power);
    ui->selectButton->setDisabled(!power);
    ui->startButton->setDisabled(!power);
    ui->pauseButton->setDisabled(!power);
    ui->stopButton->setDisabled(!power);

    if(power){
        ui->mainMenu->addItem("NEW SESSION");
        ui->mainMenu->addItem("SESSION LOG");
        ui->mainMenu->addItem("TIME AND DATE");
        ui->mainMenu->setCurrentRow(0);
        ui->disconnectButton->setDisabled(false);
        ui->reconnectButton->setDisabled(true);
    }else{
        ui->mainMenu->clear();
        ui->sessionFrame->setVisible(false);
        ui->dateFrame->setVisible(false);
        ui->reconnectButton->setEnabled(false);
        ui->reconnectButton->setEnabled(false);
    }

    qInfo()<< "power button pressed";
    emit powerButtonPressed();
}

void MainWindow::handleStartButton(){
    //functionality

    ui->startButton->setEnabled(false);
    ui->stopButton->setEnabled(true);
    ui->pauseButton->setEnabled(true);
    ui->menuButton->setEnabled(false);

    qInfo()<< "start button pressed";
    emit startButtonPressed();
}

void MainWindow::handleStopButton(){
    //functionality
    qInfo()<< "stop button pressed";
    emit stopButtonPressed();
}

void MainWindow::handleSelectButton(){
    //functionality
    std::string selection = ui->mainMenu->currentItem()->data(Qt::DisplayRole).toString().toStdString();

    if(selection == "NEW SESSION")
    {
        ui->sessionFrame->setVisible(true);
        ui->menuButton->setEnabled(true);
        ui->startButton->setEnabled(true);
    }
    else if(selection == "SESSION LOG")
    {
        ui->mainMenu->clear();
        ui->mainMenu->addItem("UPLOAD");
        ui->mainMenu->addItem("BACK");
        ui->mainMenu->setCurrentRow(0);
        ui->menuButton->setEnabled(true);
    }
    else if(selection == "TIME AND DATE")
    {
        ui->mainMenu->clear();
        ui->mainMenu->addItem("CONFIRM");
        ui->mainMenu->setCurrentRow(0);
        ui->dateFrame->setVisible(true);
        ui->menuButton->setEnabled(true);
    }
    else if(selection == "BACK" || selection == "CONFIRM")
    {
        ui->mainMenu->clear();
        ui->mainMenu->addItem("NEW SESSION");
        ui->mainMenu->addItem("SESSION LOG");
        ui->mainMenu->addItem("TIME AND DATE");
        ui->mainMenu->setCurrentRow(0);
        ui->menuButton->setEnabled(false);

        if(selection == "CONFIRM")
        {
            control->getNeureset()->setDateTime(ui->dateTimeEdit->dateTime());
            ui->dateFrame->setVisible(false);
        }
    }
    else if(selection == "UPLOAD")
    {
        control->getNeureset()->receiveDataRequest();
    }


    qInfo()<< "select button pressed";
    emit selectButtonPressed();
}

void MainWindow::updateWindow(){
    while(windowThread->isRunning())
    {
        control->getNeureset()->getMutex()->lock();

        ui->contactLight->setChecked(control->getNeureset()->getContactLight()->isLit());
        ui->treatmentSignalLight->setChecked(control->getNeureset()->getTSLight()->isLit());
        ui->connectionLight->setChecked(control->getNeureset()->getConnLight()->isLit());
        //Session* s = control->getNeureset()->getCurrentSession();
        //float prog = s->getProgress();
        ui->sessionProgressBar->setValue(control->getNeureset()->getCurrSessionProgress());
        ui->timerDisplay->setText(control->getNeureset()->getCurrSessionTime());
        ui->batteryBar->setValue(control->getNeureset()->getBattery());

        control->getNeureset()->getMutex()->unlock();
    }
}

void MainWindow::handleDisconnectButton(){
    //functionality
    qInfo()<< "disconnect button pressed";
    ui->disconnectButton->setEnabled(false);
    ui->reconnectButton->setEnabled(true);
    control->getNeureset()->getConnLight()->startFlashing();
    emit disconnectButtonPressed();
}

void MainWindow::handleReconnectButton(){
    //functionality
    qInfo()<< "reconnect button pressed";
    ui->reconnectButton->setEnabled(false);
    ui->disconnectButton->setEnabled(true);
    control->getNeureset()->getConnLight()->stopFlashing();
    emit reconnectButtonPressed();
}

void MainWindow::turnOff(){
    //Power off disables all interfaces the user can access on the neureset. When turned back on, it is returned to the main menu.
    power = false;
    ui->navigateDown->setDisabled(true);
    ui->navigateUp->setDisabled(true);
    ui->menuButton->setDisabled(true);
    ui->selectButton->setDisabled(true);
    ui->startButton->setDisabled(true);
    ui->pauseButton->setDisabled(true);
    ui->stopButton->setDisabled(true);
    ui->reconnectButton->setDisabled(true);
    ui->reconnectButton->setDisabled(true);
    control->getNeureset()->getConnLight()->stopFlashing();
    control->getNeureset()->getContactLight()->stopFlashing();
    control->getNeureset()->getTSLight()->stopFlashing();

    ui->mainMenu->clear();
    ui->sessionFrame->setVisible(false);
    ui->dateFrame->setVisible(false);
}

void MainWindow::handleBattery10Button()
{
    control->getNeureset()->setBattery(10);
    ui->batteryBar->setValue(10);
}

void MainWindow::handleBattery0Button()
{
    control->getNeureset()->setBattery(0);
    ui->batteryBar->setValue(0);
}

void MainWindow::sessionComplete()
{
    ui->startButton->setEnabled(true);
    ui->pauseButton->setEnabled(false);
    ui->stopButton->setEnabled(false);
    ui->menuButton->setEnabled(true);
}
