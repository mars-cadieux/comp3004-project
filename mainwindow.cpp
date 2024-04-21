#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include "session.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->sessionFrame->setVisible(false);
    ui->dateFrame->setVisible(false);
    ui->historyFrame->setVisible(false);
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

    control->launch();

    ui->startButton->setEnabled(false);
    ui->pauseButton->setEnabled(false);
    ui->stopButton->setEnabled(false);
    ui->menuButton->setEnabled(false);

    ui->connectionLight->setChecked(false);
    ui->contactLight->setChecked(true);
    ui->treatmentSignalLight->setChecked(false);

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
    ui->historyFrame->setVisible(false);
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
    ui->navigateUp->setEnabled(true);
    ui->navigateDown->setEnabled(true);
    ui->selectButton->setEnabled(true);
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
}

void MainWindow::handlePauseButton(){
    // Pauses the session
    ui->startButton->setEnabled(true);
    ui->pauseButton->setEnabled(false);
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
        ui->startButton->setEnabled(false);
        ui->pauseButton->setEnabled(false);
        ui->stopButton->setEnabled(false);
        ui->menuButton->setEnabled(false);
        ui->reconnectButton->setDisabled(true);
        ui->navigateUp->setEnabled(true);
        ui->navigateDown->setEnabled(true);
        ui->selectButton->setEnabled(true);
    }else{
        ui->mainMenu->clear();
        ui->sessionFrame->setVisible(false);
        ui->dateFrame->setVisible(false);
        ui->reconnectButton->setEnabled(false);
        ui->reconnectButton->setEnabled(false);
    }

    emit powerButtonPressed();
}

void MainWindow::handleStartButton(){
    //Starts the session

    ui->startButton->setEnabled(false);
    ui->stopButton->setEnabled(true);
    ui->pauseButton->setEnabled(true);
    ui->menuButton->setEnabled(false);

    emit startButtonPressed();
}

void MainWindow::handleStopButton(){
    //Stops the session
    ui->startButton ->setEnabled(true);
    ui->stopButton->setEnabled(false);
    ui->pauseButton->setEnabled(false);
    ui->menuButton->setEnabled(true);
    ui->sessionProgressBar->setValue(0);
    emit stopButtonPressed();
}

void MainWindow::handleSelectButton(){
    //Sends a signal to neureset or updates UI based on current selection
    std::string selection = ui->mainMenu->currentItem()->data(Qt::DisplayRole).toString().toStdString();

    if(selection == "NEW SESSION")
    {
        ui->sessionFrame->setVisible(true);
        ui->menuButton->setEnabled(true);
        ui->startButton->setEnabled(true);
        ui->navigateUp->setEnabled(false);
        ui->navigateDown->setEnabled(false);
        ui->selectButton->setEnabled(false);

        ui->sessionProgressBar->setValue(0);
        ui->timerDisplay->setText(QStringLiteral("%1:%2").arg(0).arg(0, 2, 10, QLatin1Char('0')));
    }
    else if(selection == "SESSION LOG")
    {
        ui->mainMenu->clear();
        ui->mainMenu->addItem("UPLOAD");
        ui->mainMenu->addItem("HISTORY");
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
        ui->navigateUp->setEnabled(false);
        ui->navigateDown->setEnabled(false);
        ui->selectButton->setEnabled(false);
    }
    else if(selection == "CONFIRM")
    {
        ui->mainMenu->clear();
        ui->mainMenu->addItem("NEW SESSION");
        ui->mainMenu->addItem("SESSION LOG");
        ui->mainMenu->addItem("TIME AND DATE");
        ui->navigateUp->setEnabled(true);
        ui->navigateDown->setEnabled(true);
        ui->selectButton->setEnabled(true);
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
    else if(selection == "HISTORY")
    {
        ui->historyList->clear();
        ui->navigateUp->setEnabled(false);
        ui->navigateDown->setEnabled(false);
        ui->selectButton->setEnabled(false);

        for(int i = 0; i < control->getNeureset()->getSessions()->size(); i ++)
        {
            ui->historyList->addItem(control->getNeureset()->getSessions()->at(i)->getDateTime().toString(DATE_FORMAT));
        }

        ui->historyFrame->setVisible(true);
    }
    emit selectButtonPressed();
}

void MainWindow::updateGraph(QVector<Sinewave> bWave)
{
    if(ui->customPlot->graphCount() == 0)
    {
        ui->customPlot->addGraph();
    }

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

void MainWindow::updateBattery(float b)
{
    ui->batteryBar->setValue(b);
}

void MainWindow::updateSessionTimer(QString t)
{
    ui->timerDisplay->setText(t);
}

void MainWindow::updateSessionProgress(float p)
{
    ui->sessionProgressBar->setValue(p);
}

void MainWindow::updateLight(bool l, QString t)
{
    //qInfo("in updatelight main window"); //debugging
    if(QString::compare(t, "contact") == 0){
        ui->contactLight->setChecked(l);
    }
    else if(QString::compare(t, "connection") == 0){
        ui->connectionLight->setChecked(l);
    }
    else if(QString::compare(t, "ts") == 0){
        ui->treatmentSignalLight->setChecked(l);
    }
}

void MainWindow::handleDisconnectButton(){
    // Disconnects the Electrodes, emits signal to neureset
    ui->disconnectButton->setEnabled(false);
    ui->reconnectButton->setEnabled(true);

    emit disconnectButtonPressed();
}

void MainWindow::handleReconnectButton(){
    // Reconnects the Electrodes, emits signal to neureset
    ui->reconnectButton->setEnabled(false);
    ui->disconnectButton->setEnabled(true);

    //if the session is paused, reconnecting the electrodes resumes the session, so we want to disable/enable the appropriate buttonss
    if(!(ui->pauseButton->isEnabled())){
        ui->pauseButton->setEnabled(true);
    }
    if(ui->startButton->isEnabled()){
        ui->startButton->setEnabled(false);
    }

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

    ui->mainMenu->clear();
    ui->sessionFrame->setVisible(false);
    ui->dateFrame->setVisible(false);
}

void MainWindow::handleBattery10Button()
{
    // Sets battery value to 10%
    control->getNeureset()->setBattery(10);
    ui->batteryBar->setValue(10);
}

void MainWindow::handleBattery0Button()
{
    // Sets battery value to 0%
    control->getNeureset()->setBattery(0);
    ui->batteryBar->setValue(0);
}

void MainWindow::sessionComplete()
{
    // Re-enables buttons on session completion.
    ui->startButton->setEnabled(true);
    ui->pauseButton->setEnabled(false);
    ui->stopButton->setEnabled(false);
    ui->menuButton->setEnabled(true);
    ui->customPlot->removeGraph(0);
}

void MainWindow::on_showWaveformButton_released()
{
    // Updates waveform display
    int elecNum = ui->electrodeSelector->value();
    emit showWaveform(QString::number(elecNum));
}

