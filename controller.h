#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "neureset.h"
#include "pcsoftware.h"

#include <QObject>

/* Controller Class
 * Class Purpose: Connects the Neureset UI and PCSoftware to the Neureset class, allowing easy communication through signals.
 *
 * Data Members:
 * Neureset neureset: Contains the neureset object
 * PCSoftware pcSoft: Contains the PCSoftware object
 * MainWindow window: Contains the neureset UI
 *
 * Class Functions:
 * Getters for neureset and pcSoft
 *
 * void launch(): Connects all signals from the neurest, pcSoft, and window to their respective slots.
 * */

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
