#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "neureset.h"
#include "pcsoftware.h"

#include <QObject>

class MainWindow;

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(MainWindow* w, QObject *parent = nullptr);
    ~Controller();
    void launch();
    Neureset* getNeureset();
    PCSoftware* getPCSoftware();

signals:

private:
    Neureset neureset;
    PCSoftware pcSoft;
    MainWindow* window;

};

#endif // CONTROLLER_H
