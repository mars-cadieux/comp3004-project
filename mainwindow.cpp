#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->sessionFrame->setVisible(false);

    connect(ui->menuButton, &QPushButton::clicked, this, &MainWindow::handleMenuButton);
    connect(ui->navigateDown, &QPushButton::clicked, this, &MainWindow::handleNavigateDown);
    connect(ui->navigateUp, &QPushButton::clicked, this, &MainWindow::handleNavigateUp);
    connect(ui->pauseButton, &QPushButton::clicked, this, &MainWindow::handlePauseButton);
    connect(ui->powerButton , &QPushButton::clicked, this, &MainWindow::handlePowerButton);
    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::handleStartButton);
    connect(ui->stopButton, &QPushButton::clicked, this, &MainWindow::handleStopButton);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleMenuButton(){
    //functionality
    qInfo()<< "menu button pressed";
}

void MainWindow::handleNavigateDown(){
    //functionality
    qInfo()<< "navigate down button pressed";
}

void MainWindow::handleNavigateUp(){
    //functionality
    qInfo()<< "navigate up button pressed";
}

void MainWindow::handlePauseButton(){
    //functionality
    qInfo()<< "pause button pressed";
}

void MainWindow::handlePowerButton(){
    //functionality
    qInfo()<< "power button pressed";
}

void MainWindow::handleStartButton(){
    //functionality
    qInfo()<< "start button pressed";
}

void MainWindow::handleStopButton(){
    //functionality
    qInfo()<< "stop button pressed";
}

