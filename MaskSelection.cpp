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

#include "gesturerecognition.h"
#include "ui_gesturerecognition.h"

GestureRecognition::GestureRecognition(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GestureRecognition)
{
    ui->setupUi(this);

    //default settings
    process = false;
    histEnable = false;
    cHist = ColorHistogram();
    //RGB defaults *** Change for Hsv
    color[0] = 198;
    color[1] = 158;
    color[2] = 93;
    setColor();
    threshold = 50;


    // defaults for sliders
    ui->verticalSlider_Threshold->setSliderPosition(threshold);
    ui->verticalSlider_Color1->setSliderPosition(color[0]);
    ui->verticalSlider_Color2->setSliderPosition(color[1]);
    ui->verticalSlider_Color3->setSliderPosition(color[2]);


    //set up video ------------------
    //get camera
    cap.open(CAMERA);
    // check if we succeeded, if not do not enable camera toggle
    if(!cap.isOpened())
        ui->pushButton_Camera->setEnabled(false);
    //set up timer for camera display
    timer = new QTimer(this);
    //end video ---------------------


    // slot action methods -------------
    //select color
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTimer()));
    connect(ui->pushButton_Color, SIGNAL(clicked()), this, SLOT(pickColor()));
    connect(ui->pushButton_OpenImage, SIGNAL(clicked()), this, SLOT(setImage()));
    connect(ui->pushButton_Camera, SIGNAL(clicked()), this, SLOT(toggleCamera()));
    connect(ui->pushButton_Process, SIGNAL(clicked()), this, SLOT(processColorDetection()));
    connect(ui->pushButton_Histogram, SIGNAL(clicked()), this, SLOT(showHistogram()));
    connect(ui->verticalSlider_Threshold, SIGNAL(valueChanged(int)), 
                                            this, SLOT(setThreshold(int)));
    connect(ui->verticalSlider_Color1, SIGNAL(valueChanged(int)), 
                                            this, SLOT(setColor1(int)));
    connect(ui->verticalSlider_Color2, SIGNAL(valueChanged(int)), 
                                            this, SLOT(setColor2(int)));
    connect(ui->verticalSlider_Color3, SIGNAL(valueChanged(int)), 
                                            this, SLOT(setColor3(int)));
    //end slots ----------------
}

GestureRecognition::~GestureRecognition()
{
    delete ui;
}

void GestureRecognition::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void GestureRecognition::setImage()
{
    timer->stop();
    QFileDialog::Options options;
    QString selectedFilter;
    QString fileName = QFileDialog::getOpenFileName(this,
                                tr("Open Image Files"),
                                "",
                                tr("Image files (*.jpg *.jpeg *.png *.gif *.bmp)"),
                                &selectedFilter,
                                options);
    if (!fileName.isEmpty()){
        cv::Mat image = cv::imread(fileName.toStdString(),1); //0 for grayscale
        displayMat(image);
        //Set Filename
        ColorDetectController::getInstance()->setInputImage(fileName.toStdString());
    }
}

void GestureRecognition::toggleCamera()
{
    if(!cap.isOpened())
        return;
    if(timer->isActive())
        timer->stop();
    else
    {
        timer->start(25);
    }

}

//Convert cv::Mat to QImage and display
void GestureRecognition::displayMat(const cv::Mat& image)
{
    //BGR openCV Mat to QImage
    QImage img_qt = QImage((const unsigned char*)image.data,image.cols, image.rows, image.step, QImage::Format_RGB888);

    //For Binary Images
    if (img_qt.isNull()){
        //ColorTable for Binary Images
        QVector<QRgb> colorTable;
        for (int i = 0; i < 256; i++)
            colorTable.push_back(qRgb(i, i, i));

        img_qt = QImage((const unsigned char*)image.data,image.cols, image.rows, image.step, QImage::Format_Indexed8);
        img_qt.setColorTable(colorTable);
        }

    //Display the QImage in the Label
    QPixmap img_pix = QPixmap::fromImage(img_qt.rgbSwapped()); //BGR to RGB
    this->ui->label->setPixmap(img_pix.scaled(ui->label->size(), Qt::KeepAspectRatio));
}


void GestureRecognition::pickColor()
{
    QColor picked = QColorDialog::getColor(QColor(color[0], color[1], color[2]), this);
    ColorDetectController::getInstance()->setTargetColor(picked.red(),picked.green(),picked.blue());
}

void GestureRecognition::setColor()
{
    ColorDetectController::getInstance()->setTargetColor(color[0],
                                        color[1],color[2]);
}

void GestureRecognition::processColorDetection()
{
    if(timer->isActive())
        process = !process;
    else
    {
        ColorDetectController::getInstance()->process();
        cv::Mat resulting = ColorDetectController::getInstance()->getLastResult();
        if (!resulting.empty())
            displayMat(resulting);
    }
}

void GestureRecognition::updateTimer()
{
    //translate to mat
    cv::Mat img;
    cap >> img;
    ColorDetectController::getInstance()->setInputImage(img);
    if(histEnable)
    {
        histogram = cHist.getHistogramImage(img);
        cv::imshow("Histogram", histogram);
    }
    if(process)
    {
        ColorDetectController::getInstance()->process();
    
        cv::Mat resulting = ColorDetectController::getInstance()->getLastResult();
        if (!resulting.empty())
            img = resulting;
    }
    displayMat(img);
}

void GestureRecognition::showHistogram()
{
    if(timer->isActive() && !histEnable)
    {
        cv::namedWindow("Histogram", cv::WINDOW_AUTOSIZE);
        histEnable = true;
    }
    else if (!timer->isActive())
    {
        histogram = cHist.getHistogramImage(ColorDetectController::getInstance()->getInputImage());
        cv::imshow("Histogram", histogram);
        histEnable = false;
    }
    else
    {
        cv::destroyWindow("Histogram");
        histEnable = false;
    }
}

//Threshold slider change method
void GestureRecognition::setThreshold(int value)
{
    threshold = value;
    this->ui->label_Threshold->setText(QString::number(value));
    ColorDetectController::getInstance()->setColorDistanceThreshold(threshold);
    if(!timer->isActive())
        processColorDetection();
}

//Color Slider change methods
void GestureRecognition::setColor1(int value)
{
    color[0] = value;
    this->ui->label_Color1_2->setText(QString::number(value));
    setColor();
    if(!timer->isActive())
        processColorDetection();
}
void GestureRecognition::setColor2(int value)
{
    color[1] = value;
    this->ui->label_Color2_2->setText(QString::number(value));
    setColor();
    if(!timer->isActive())
        processColorDetection();
}
void GestureRecognition::setColor3(int value)
{
    color[2] = value;
    this->ui->label_Color3_2->setText(QString::number(value));
    setColor();
    if(!timer->isActive())
        processColorDetection();
}
