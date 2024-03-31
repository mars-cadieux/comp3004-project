#include "mainwindow.h"

#include <QApplication>

//test comment - mars

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
