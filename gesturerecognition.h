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

#ifndef GESTURERECOGNITION_H
#define GESTURERECOGNITION_H

#include <QMainWindow>
#include <QFileDialog>
#include <QColorDialog>
#include <QTimer>

//sys
#include <iostream>

//OpenCV
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

//color detector, controller
#include "colorDetectController.h"
#include "colordetector.h"

namespace Ui {
    class GestureRecognition;
}

class GestureRecognition : public QMainWindow{
    Q_OBJECT
public:
    GestureRecognition(QWidget *parent = 0);
    ~GestureRecognition();

protected:
    void changeEvent(QEvent *e);
    void displayMat(const cv::Mat& img);

    // camera vars
    cv::VideoCapture cap;

    //timer
    QTimer* timer;
    bool process;


    //Main Image
    //cv::Mat img_mat;

private:
    Ui::GestureRecognition *ui;

    //DEFAULTS!!!
    static const int CAMERA = 1;
    static const int THRESHOLD = 139;
    static const int RED = 255, GREEN = 222, BLUE = 196;

private slots:
    void processColorDetection();
    void on_verticalSlider_Threshold_valueChanged(int value);
    void setColor();
    void setColor(QColor color);
    void setImage();
    void toggleCamera();
    void updateTimer();
};

#endif // GESTURERECOGNITION_H
