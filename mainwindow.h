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

/* MainWindow Class
 * Class Purpose: Displays the UI for the Neureset, and sends user input to the controller.
 *
 * Data Members:
 * MainWindow *ui: Pointer to UI object
 * Controller* control: Pointer to controller object
 * QThread windowThread: Runs the window asynchronously
 * bool power: True if device is powered
 *
 * Class Functions:
 * void updateWindow(): Runs asynchronously to continuously update the window.
 *
 * Slots:
 * Private slots to handle buttons and other user input
 * void turnOff(): Signalled by controller when power runs out, shuts down the UI
 * void sessionComplete(): Signalled when neureset finishes a session
 * void updateGraph(): Signalled when a wavelength is requested, updates the graph with given Sinewave.
 * */

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    const QString DATE_FORMAT = "yyyy-MM-dd hh:mm";
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void updateWindow();


private:
    Ui::MainWindow *ui;
    Controller* control;
    QThread* windowThread;
    bool power = true;

signals:
    //void menuButtonPressed();
    //void downButtonPressed();
    //void upButtonPressed();
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
    void updateBattery(float b);
    void updateSessionTimer(QString t);
    void updateSessionProgress(float p);
    void updateLight(bool l, QString t);

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
