#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->sessionFrame->setVisible(false);
    ui->dateFrame->setVisible(false);
    ui->mainMenu->setCurrentRow(0);
    neureset = new Neureset();

    connect(ui->menuButton, &QPushButton::clicked, this, &MainWindow::handleMenuButton);
    connect(ui->navigateDown, &QPushButton::clicked, this, &MainWindow::handleNavigateDown);
    connect(ui->navigateUp, &QPushButton::clicked, this, &MainWindow::handleNavigateUp);
    connect(ui->pauseButton, &QPushButton::clicked, this, &MainWindow::handlePauseButton);
    connect(ui->powerButton , &QPushButton::clicked, this, &MainWindow::handlePowerButton);
    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::handleStartButton);
    connect(ui->stopButton, &QPushButton::clicked, this, &MainWindow::handleStopButton);
    connect(ui->selectButton, &QPushButton::clicked, this, &MainWindow::handleSelectButton);

    windowThread = QThread::create([this]{ updateWindow(); });
    windowThread->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleMenuButton(){
    //functionality
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
    //functionality
    qInfo()<< "power button pressed";
    emit powerButtonPressed();
}

void MainWindow::handleStartButton(){
    //functionality
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
    }
    else if(selection == "SESSION LOG")
    {
        ui->mainMenu->clear();
        ui->mainMenu->addItem("UPLOAD");
        ui->mainMenu->addItem("BACK");
        ui->mainMenu->setCurrentRow(0);
    }
    else if(selection == "TIME AND DATE")
    {
        ui->mainMenu->clear();
        ui->mainMenu->addItem("CONFIRM");
        ui->mainMenu->setCurrentRow(0);
        ui->dateFrame->setVisible(true);
    }
    else if(selection == "BACK" || selection == "CONFIRM")
    {
        ui->mainMenu->clear();
        ui->mainMenu->addItem("NEW SESSION");
        ui->mainMenu->addItem("SESSION LOG");
        ui->mainMenu->addItem("TIME AND DATE");
        ui->mainMenu->setCurrentRow(0);

        if(selection == "CONFIRM")
        {
            neureset->setDateTime(ui->dateTimeEdit->date(), ui->dateTimeEdit->time());
            ui->dateFrame->setVisible(false);
        }
    }

    qInfo()<< "select button pressed";
    emit selectButtonPressed();
}

void MainWindow::updateWindow(){
    while(windowThread->isRunning())
    {
        ui->connectionLight->setChecked(neureset->getConnLight()->isLit());
        ui->batteryBar->setValue(neureset->getBattery());
    }
}

