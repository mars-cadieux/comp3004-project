#ifndef PCUI_H
#define PCUI_H

#include <QWidget>
#include <QVector>

#include "qcustomplot.h"

class PCSoftware;
class Session;

/* PCui Class
 * Class Purpose: Display and manage the UI for the PC.
 *
 * Data Members:
 * PCui *ui: pointer to the ui
 * PCSoftware* owner: pointer to the PCSoftware that "owns" this ui. this makes it so that the ui can comunicate to software directly instead of having to emit signals
 * QCustomPlot* customPlot: plot object for displaying the graph
 *
 * Class Functions:
 * void updateView(): Updates the view with new session data.
 * void setupPlot(): Sets up the QCustomPlot.
 * */

namespace Ui {
class PCui;
}

class PCui : public QWidget
{
    Q_OBJECT

public:
    explicit PCui(QWidget *parent = nullptr, PCSoftware* owner = nullptr);
    ~PCui();
    void updateView(QVector<Session*> s);

private slots:
    //void on_downloadButton_released();

private:
    Ui::PCui *ui;

    PCSoftware* owner;
    QCustomPlot*  customPlot;

    void setupPlot();
};

#endif // PCUI_H
