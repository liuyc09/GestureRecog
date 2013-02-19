/*------------------------------------------------------------------------------------------*\
   This file contains material supporting chapter 3 of the cookbook:  
   Computer Vision Programming using the OpenCV Library. 
   by Robert Laganiere, Packt Publishing, 2011.

   This program is free software; permission is hereby granted to use, copy, modify, 
   and distribute this source code, or portions thereof, for any purpose, without fee, 
   subject to the restriction that the copyright notice may not be removed 
   or altered from any source or altered source distribution. 
   The software is released on an as-is basis and without any warranties of any kind. 
   In particular, the software is not guaranteed to be fault-tolerant or free from failure. 
   The author disclaims all warranties with regard to this software, any use, 
   and any consequent failure, is purely the responsibility of the user.
 
   Copyright (C) 2010-2011 Robert Laganiere, www.laganiere.name
\*------------------------------------------------------------------------------------------*/

#ifndef MASKSELECTION_H
#define MASKSELECTION_H

#include <QMainWindow>
#include <QFileDialog>
#include <QColorDialog>
#include <QTimer>

//sys
#include <iostream>
#include <string>

//OpenCV
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

//color detector, controller, histogram
#include "../include/skindetectcontroller.h"
#include "../include/colorhistogram.h"
#include "../src/gesturedetector.h"

namespace Ui {
    class MaskSelection;
}

class MaskSelection : public QMainWindow{
    Q_OBJECT
public:
    MaskSelection(QWidget *parent = 0);
    ~MaskSelection();

protected:
    void changeEvent(QEvent *e);
    void displayMat(const cv::Mat& img);

    // camera vars
    cv::VideoCapture cap;
    static const int CAMERA = 1;

    // timer vars
    QTimer* timer;
    bool process;
    bool histEnable;




private:
    Ui::MaskSelection *ui;
    GestureDetector *gd;

    // the thresholding masks
    cv::Scalar min, max;

    cv::Mat histogram;
    ColorHistogram cHist;

private slots:
    void processColorDetection();
    void showHistogram();
    void setImage();
    void toggleCamera();
    void updateTimer();
    void setThreshold();
    void beginDetection();
    void setMinHue(int value);
    void setMinSat(int value);
    void setMinValue(int value);
    void setMaxHue(int value);
    void setMaxSat(int value);
    void setMaxValue(int value);
};

#endif // MASKSELECTION_H
