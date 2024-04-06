#include "controller.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Controller control;
    control.launch();

    return a.exec();
}
