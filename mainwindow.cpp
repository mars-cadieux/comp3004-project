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

    ui->customPlot->addGraph();

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
        ui->sessionProgressBar->setValue(control->getNeureset()->getCurrSessionProgress());
        ui->batteryBar->setValue(control->getNeureset()->getBattery());

        control->getNeureset()->getMutex()->unlock();
    }
}

void MainWindow::updateGraph(QVector<Sinewave> bWave)
{
    //if the electrode has measured a brainwave, the bWave vector will be non-empty and we want to update the graph. Otherwise, do nothing
    if(bWave.size() > 0){

        ui->customPlot->graph(0)->setPen(QPen(Qt::blue)); // line color blue

        // generate some points of data
        QVector<double> x(51), y(51);
        //this temp value will be used to accumulate all of the sine waves
        double temp = 0;

        for (int i=0; i<51; ++i)
        {
          // x-axis will go from 0 to 1
          x[i] = i/50.0;
          //the EEG waveform will be the sum of all four brainwaves (alpha, beta, delta, theta). loop through all brainwaves and sum their values
          //recall that the formula for a sinwave (in terms of i) is (amplitude)*(sin((frequency)*i))
          for(int j=0; j<bWave.size(); ++j){
              //qInfo("Amp: %f", bWave[j].amplitude); //debugging
              //qInfo("Freq: %f", bWave[j].frequency); //debugging
              temp += (bWave[j].amplitude)*qSin((bWave[j].frequency)*i);
          }
          y[i] = temp;

        }
        ui->customPlot->graph(0)->setData(x, y);

        // let the ranges scale themselves so graph 0 fits perfectly in the visible area:
        ui->customPlot->graph(0)->rescaleAxes();
        //hide the labels (x-values) along the x-axis
        ui->customPlot->xAxis->setTickLabels(false);

        ui->customPlot->replot();
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

void MainWindow::on_showWaveformButton_released()
{
    int elecNum = ui->electrodeSelector->value();
    emit showWaveform(QString::number(elecNum));
}

