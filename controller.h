#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "neureset.h"
#include "mainwindow.h"

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
    MainWindow w;

};

#endif // CONTROLLER_H
