#include "gesturerecognition.h"
#include <QApplication>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GestureRecognition w;
    w.show();
    
    return a.exec();
}
