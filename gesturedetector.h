#ifndef GESTUREDETECTION_H
#define GESTUREDETECTION_H

#include <QDialog>
#include <QTimer>

//sys
#include <iostream>
#include <string>

//OpenCV
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

//color detector, controller, histogram
#include "colorDetectController.h"
#include "colordetector.h"
#include "colorhistogram.h"

namespace Ui {
	class GestureDetector;
}

class GestureDetector : public QDialog
{
    Q_OBJECT
    
public:
    explicit GestureDetector(QWidget *parent = 0);
    ~GestureDetector();

    
private:
    Ui::GestureDetector *ui;

    void displayMat(const cv::Mat &image);
};

#endif // GESTUREDETECTION_H
