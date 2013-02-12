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
    // default skin tone matching
    ui->verticalSlider_Threshold->setSliderPosition(THRESHOLD);
    setColor(QColor(RED, GREEN, BLUE));
    process = false;


    // slot action methods -------------
    //select color
    connect(ui->pushButton_Color, SIGNAL(clicked()), this, SLOT(setColor()));
    //open image
    connect(ui->pushButton_OpenImage, SIGNAL(clicked()), this, SLOT(setImage()));
    //process Color Detection
    connect(ui->pushButton_Process, SIGNAL(clicked()), this, SLOT(processColorDetection()));
    //process Color Detection
    connect(ui->pushButton_Camera, SIGNAL(clicked()), this, SLOT(toggleCamera()));
    //end slots ----------------


    //set up video ------------------
    //get camera
    cap.open(CAMERA);
    // check if we succeeded, if not do not enable camera toggle
    if(!cap.isOpened())
        ui->pushButton_Camera->setEnabled(false);

    //set up timer for camera display
    timer = new QTimer(this);
    //slot action method
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTimer()));
    //end video ---------------------
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
    QFileDialog::Options options;
    QString selectedFilter;
    QString fileName = QFileDialog::getOpenFileName(this,
                                tr("Open Image Files"),
                                "",
                                tr("Image files (*.jpg *.jpeg *.png *.gif *.bmp)"),
                                &selectedFilter,
                                options);
    if (!fileName.isEmpty()){
        cv::Mat img_mat = cv::imread(fileName.toStdString(),1); //0 for grayscale
        displayMat(img_mat);
    }
    //Set Filename
    ColorDetectController::getInstance()->setInputImage(fileName.toStdString());
}

void GestureRecognition::toggleCamera()
{
    if(!cap.isOpened())
        return;
    if(timer->isActive())
        timer->stop();
    else
    {
        timer->start(50);
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

void GestureRecognition::on_verticalSlider_Threshold_valueChanged(int value)
{
    QString thresh("Threshold: ");
    thresh.append(QString::number(value));
    this->ui->label_Threshold->setText(thresh);
    //processColorDetection();
}

void GestureRecognition::setColor()
{
    QString clrTxt("Color:\n");
    cv::Vec3b prevColor = ColorDetectController::getInstance()->getTargetColor();
    QColor color = QColorDialog::getColor(QColor(prevColor[2], prevColor[1], prevColor[0]), this);
    if (color.isValid()) {
       ColorDetectController::getInstance()->setTargetColor(color.red(),color.green(),color.blue());

       clrTxt.append(QString::number(color.red()) + ", " + QString::number(color.green()) + ", " + QString::number(color.blue()));
       this->ui->label_Color->setText(clrTxt);
       ui->pushButton_Process->setEnabled(true);
    }
    else
    {
        ui->pushButton_Process->setEnabled(false);
    }
}

void GestureRecognition::setColor(QColor color)
{
    if (color.isValid()) 
    {
        QString clrTxt("Color:\n");
        ColorDetectController::getInstance()->setTargetColor(color.red(),
                                        color.green(),color.blue());
        clrTxt.append(QString::number(color.red()) + ", " 
            + QString::number(color.green()) + ", " 
            + QString::number(color.blue()));
        this->ui->label_Color->setText(clrTxt);
        ui->pushButton_Process->setEnabled(true);
    }
    else
    {
        ui->pushButton_Process->setEnabled(false);
    }
}

void GestureRecognition::processColorDetection()
{
    ColorDetectController::getInstance()->setColorDistanceThreshold(
                                ui->verticalSlider_Threshold->value());
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
    if(process)
    {
        ColorDetectController::getInstance()->setColorDistanceThreshold(
                                ui->verticalSlider_Threshold->value());
        ColorDetectController::getInstance()->process();
    
        cv::Mat resulting = ColorDetectController::getInstance()->getLastResult();
        if (!resulting.empty())
            img = resulting;
    }
    displayMat(img);
}
