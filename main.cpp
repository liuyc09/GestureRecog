#include "maskselection.h"
#include <QApplication>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MaskSelection w;
    w.show();
    
    return a.exec();
}
