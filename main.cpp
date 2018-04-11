#include "widget.h"
#include <QApplication>
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LaserTrack laserTracker;
    Widget w(&laserTracker);
    w.show();

    return a.exec();
}
