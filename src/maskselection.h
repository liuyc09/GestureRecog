/*------------------------------------------------------------------------------------------*\
    Created by: Jason Carlisle Mann (on2valhalla | jcm2207@columbia.edu)

    Some inspiration for design and organization from Robert Laganiere (OpenCV2 Cookbook)

    This class is a QT based form, with sliders and a image display area
    so that you can actively select a mask to create a binary skin image from
    a standard RGB. The primary purpose of the form is to serve video, but
    it also has an option to display photos. You select a mask by moving 
    sliders (in HSV values) for the min and max thresholds sent to the 
    SkinDetector class

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
