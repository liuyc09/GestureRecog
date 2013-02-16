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

#include "maskselection.h"
#include "ui_maskselection.h"

MaskSelection::MaskSelection(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MaskSelection)
{
    //setup forms
    ui->setupUi(this);
    gd = new GestureDetector();


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
    connect(ui->pushButton_OpenImage, SIGNAL(clicked()), this, SLOT(setImage()));
    connect(ui->pushButton_Camera, SIGNAL(clicked()), this, SLOT(toggleCamera()));
    connect(ui->pushButton_Process, SIGNAL(clicked()), this, SLOT(processColorDetection()));
    connect(ui->pushButton_Histogram, SIGNAL(clicked()), this, SLOT(showHistogram()));
    connect(ui->verticalSlider_MinHue, SIGNAL(valueChanged(int)), 
                                            this, SLOT(setMinHue(int)));
    connect(ui->verticalSlider_MinSat, SIGNAL(valueChanged(int)), 
                                            this, SLOT(setMinSat(int)));
    connect(ui->verticalSlider_MinValue, SIGNAL(valueChanged(int)), 
                                            this, SLOT(setMinValue(int)));
    connect(ui->verticalSlider_MaxHue, SIGNAL(valueChanged(int)), 
                                            this, SLOT(setMaxHue(int)));
    connect(ui->verticalSlider_MaxSat, SIGNAL(valueChanged(int)), 
                                            this, SLOT(setMaxSat(int)));
    connect(ui->verticalSlider_MaxValue, SIGNAL(valueChanged(int)), 
                                            this, SLOT(setMaxValue(int)));
    //end slots ----------------


    //default settings
    process = false;
    histEnable = false;
    cHist = ColorHistogram();

    //RGB defaults *** Change for Hsv
    min[0] = 9;
    min[1] = 51;
    min[2] = 156;
    max[0] = 23;
    max[1] = 160;
    max[2] = 237;
    setThreshold();

    
    // defaults for sliders
    ui->verticalSlider_MinHue->setSliderPosition(min[0]);
    ui->verticalSlider_MinSat->setSliderPosition(min[1]);
    ui->verticalSlider_MinValue->setSliderPosition(min[2]);
    ui->verticalSlider_MaxHue->setSliderPosition(max[0]);
    ui->verticalSlider_MaxSat->setSliderPosition(max[1]);
    ui->verticalSlider_MaxValue->setSliderPosition(max[2]);
}

MaskSelection::~MaskSelection()
{
    delete ui;
    delete gd;
}

void MaskSelection::changeEvent(QEvent *e)
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

void MaskSelection::setImage()
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
        SkinDetectController::getInstance()->setInputImage(fileName.toStdString());
    }
}

void MaskSelection::toggleCamera()
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
void MaskSelection::displayMat(const cv::Mat& image)
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

void MaskSelection::processColorDetection()
{
    if(timer->isActive())
        process = !process;
    else
    {
        SkinDetectController::getInstance()->process();
        cv::Mat resulting = SkinDetectController::getInstance()->getLastResult();
        if (!resulting.empty())
            displayMat(resulting);
    }
}

void MaskSelection::updateTimer()
{
    //translate to mat
    cv::Mat img;
    cap >> img;
    SkinDetectController::getInstance()->setInputImage(img);
    if(histEnable)
    {
        histogram = cHist.getHistogramImage(img);
        cv::imshow("Histogram", histogram);
    }
    if(process)
    {
        SkinDetectController::getInstance()->process();
    
        cv::Mat result = SkinDetectController::getInstance()->getLastResult();
        if (!result.empty())
            img = result;
    }
    displayMat(img);
}

void MaskSelection::showHistogram()
{
    if(timer->isActive() && !histEnable)
    {
        cv::namedWindow("Histogram", cv::WINDOW_AUTOSIZE);
        histEnable = true;
    }
    else if (!timer->isActive())
    {
        histogram = cHist.getHistogramImage(SkinDetectController::getInstance()->getHSVImage());
        cv::imshow("Histogram", histogram);
        histEnable = false;
    }
    else
    {
        cv::destroyWindow("Histogram");
        histEnable = false;
    }
}


// Sets the skin detector masks min and max
void MaskSelection::setThreshold()
{
    SkinDetectController::getInstance()->setThreshold(min, max);
    if(!timer->isActive() && process)
        processColorDetection();
}

void MaskSelection::setMinHue(int value)
{
    min[0] = value;
    this->ui->label_MinHue->setText(QString::number(value));
    setThreshold();
}

void MaskSelection::setMinSat(int value)
{
    min[1] = value;
    this->ui->label_MinSat->setText(QString::number(value));
    setThreshold();
}

void MaskSelection::setMinValue(int value)
{
    min[2] = value;
    this->ui->label_MinValue->setText(QString::number(value));
    setThreshold();
}

void MaskSelection::setMaxHue(int value)
{
    max[0] = value;
    this->ui->label_MaxHue->setText(QString::number(value));
    setThreshold();
}

void MaskSelection::setMaxSat(int value)
{
    max[1] = value;
    this->ui->label_MaxSat->setText(QString::number(value));
    setThreshold();
}

void MaskSelection::setMaxValue(int value)
{
    max[2] = value;
    this->ui->label_MaxValue->setText(QString::number(value));
    setThreshold();
}


//Display the detection form
void MaskSelection::beginDetection()
{
    gd->show();
}








