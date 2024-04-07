#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "neureset.h"
#include "mainwindow.h"
#include "pcsoftware.h"

#include <QObject>

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = nullptr);
    ~Controller();
    void launch();

signals:

private:
    Neureset neureset;
    PCSoftware pcSoft;
    MainWindow w;

};

#endif // CONTROLLER_H
