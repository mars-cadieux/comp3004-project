#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDateTime>
#include "controller.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class Controller;
struct Sinewave;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void updateWindow();


private:
    Ui::MainWindow *ui;
    Controller* control;
    QThread* windowThread;
    bool power = true;

signals:
    void menuButtonPressed();
    void downButtonPressed();
    void upButtonPressed();
    void pauseButtonPressed();
    void powerButtonPressed();
    void startButtonPressed();
    void stopButtonPressed();
    void selectButtonPressed();
    void disconnectButtonPressed();
    void reconnectButtonPressed();
    void showWaveform(const QString& elecNum);

public slots:
    void turnOff();
    void sessionComplete();
    void updateGraph(QVector<Sinewave> bWave);

private slots:
    void handleMenuButton();
    void handleNavigateDown();
    void handleNavigateUp();
    void handlePauseButton();
    void handlePowerButton();
    void handleStartButton();
    void handleStopButton();
    void handleSelectButton();
    void handleDisconnectButton();
    void handleReconnectButton();
    void handleBattery10Button();
    void handleBattery0Button();
    void on_showWaveformButton_released();
};
#endif // MAINWINDOW_H
