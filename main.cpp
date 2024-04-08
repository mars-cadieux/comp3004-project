#include "controller.h"

#include <bits/stdc++.h> //idk if we're allowed to use this, needed for random number generation -mars
#include <QApplication>


int main(int argc, char *argv[])
{
    //seeds the random number generator (need to do this once in main()0 before we can generate random numbers)
    srand(time(0));


    QApplication a(argc, argv);
    Controller control;
    control.launch();

    return a.exec();
}
