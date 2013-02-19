/*

Created by: Jason Carlisle Mann (on2valhalla | jcm2207@columbia.edu)

Main method, and start of all the fun

*/


#include "../src/maskselection.h"
#include <QApplication>
#include <iostream>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MaskSelection w;
    w.show();
    
    return a.exec();
}
