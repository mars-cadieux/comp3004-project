#ifndef PCUI_H
#define PCUI_H

#include <QWidget>
#include <QVector>

#include "qcustomplot.h"

class PCSoftware;
class Session;

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

    PCSoftware* owner; //pointer to the PCSoftware that "owns" this ui. this makes it so that the ui can comunicate to software directly instead of having to emit signals
    QCustomPlot*  customPlot;

    void setupPlot();
};

#endif // PCUI_H
