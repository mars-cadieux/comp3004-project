#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDateTime>
#include "controller.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class Controller;

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
};
#endif // MAINWINDOW_H
