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


#include "../src/maskselection.h"
#include "../src/ui_maskselection.h"

// Constructor
MaskSelection::MaskSelection(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MaskSelection)
{
    //setup forms
    ui->setupUi(this);


    //set up video ------------------
    //get camera
    cap.open(CAMERA);
    // check if we succeeded, if not do not enable camera toggle
    if(!cap.isOpened())
        ui->pushButton_Camera->setEnabled(false);
    //set up timer for camera display
    timer = new QTimer(this);
    //end setup video ---------------------


    // slot action methods -------------
    // select color
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTimer()));
    connect(ui->pushButton_OpenImage, SIGNAL(clicked()), this, SLOT(setImage()));
    connect(ui->pushButton_Camera, SIGNAL(clicked()), this, SLOT(toggleCamera()));
    connect(ui->pushButton_Process, SIGNAL(clicked()), this, SLOT(processColorDetection()));
    connect(ui->pushButton_Histogram, SIGNAL(clicked()), this, SLOT(showHistogram()));
    connect(ui->pushButton_Detect, SIGNAL(clicked()), this, SLOT(beginDetection()));
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

    // Hsv default filter (for default environment)
    min[0] = 0;
    min[1] = 60;
    min[2] = 97;
    max[0] = 180;
    max[1] = 255;
    max[2] = 255;
    setThreshold();

    
    // set defaults for sliders
    ui->verticalSlider_MinHue->setSliderPosition(min[0]);
    ui->verticalSlider_MinSat->setSliderPosition(min[1]);
    ui->verticalSlider_MinValue->setSliderPosition(min[2]);
    ui->verticalSlider_MaxHue->setSliderPosition(max[0]);
    ui->verticalSlider_MaxSat->setSliderPosition(max[1]);
    ui->verticalSlider_MaxValue->setSliderPosition(max[2]);
}

// Destructor
MaskSelection::~MaskSelection()
{
    delete ui;
    delete timer;
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

/*
    This method is called by a button on the form
    and in turn pulls up an open file dialog, where
    the user can select a photo for thresholding
*/
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
        //Send Filename to the skin detector
        SkinDetectController::getInstance()->setInputImage(fileName.toStdString());
    }
}

/*
    Called by the Camera button, toggles the video feed
    on and off by toggling the timer. It will only attempt to
    turn on the timer if the cv::VideoCapture is opened
*/
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

/*
    A simple conversion method to display a cv::Mat image (BGR or binary)
    in a QLabel, that only takes a QImage as a pixelmap.
*/
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

/*
    Called by the process button, this method toggles the processing 
    and the display of the processed images/video. This method calls
    the process function from the SkinDetector class
*/
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

/*
    Controls the display of the video, on the interval set in
    toggleCamera above. It also regulates the display of the 
    histogram, and processing of the video.
*/
void MaskSelection::updateTimer()
{
    cv::Mat img;
    cap >> img; //capture a frame

    //send SkinDetector the frame
    SkinDetectController::getInstance()->setInputImage(img);
    if(histEnable)
    {   // update the histogram
        histogram = cHist.getHistogramImage(img);
        cv::imshow("Histogram", histogram);
    }
    if(process)
    {   //process the frame
        SkinDetectController::getInstance()->process();
        
        //retrieve the processed frame
        cv::Mat result = SkinDetectController::getInstance()->getLastResult();
        if (!result.empty())
            img = result;
    }
    displayMat(img);
}

/*
    Toggles the histogram window on and off
*/
void MaskSelection::showHistogram()
{
    if(timer->isActive() && !histEnable)
    {   //create histogram window for video display
        cv::namedWindow("Histogram", cv::WINDOW_AUTOSIZE);
        histEnable = true;
    }
    else if (!timer->isActive())
    {   //create histogram for image display
        histogram = cHist.getHistogramImage(SkinDetectController::getInstance()->getHSVImage());
        cv::imshow("Histogram", histogram);
        histEnable = false;
    }
    else
    {   //destroy histogram window
        cv::destroyWindow("Histogram");
        histEnable = false;
    }
}

/*
    Sets the skin detector's masks min and max from
    their cached values
*/
void MaskSelection::setThreshold()
{
    SkinDetectController::getInstance()->setThreshold(min, max);
    if(!timer->isActive() && process)
        processColorDetection();
}

/*
    Changes minHue from slider
*/
void MaskSelection::setMinHue(int value)
{
    min[0] = value;
    this->ui->label_MinHue->setText(QString::number(value));
    setThreshold();
}

/*
    Changes minimum saturation from slider
*/
void MaskSelection::setMinSat(int value)
{
    min[1] = value;
    this->ui->label_MinSat->setText(QString::number(value));
    setThreshold();
}

/*
    Changes minimum value from slider
*/
void MaskSelection::setMinValue(int value)
{
    min[2] = value;
    this->ui->label_MinValue->setText(QString::number(value));
    setThreshold();
}

/*
    Changes maximum hue from slider
*/
void MaskSelection::setMaxHue(int value)
{
    max[0] = value;
    this->ui->label_MaxHue->setText(QString::number(value));
    setThreshold();
}

/*
    Changes maximum saturation from slider
*/
void MaskSelection::setMaxSat(int value)
{
    max[1] = value;
    this->ui->label_MaxSat->setText(QString::number(value));
    setThreshold();
}

/*
    Changes maximum value from slider
*/
void MaskSelection::setMaxValue(int value)
{
    max[2] = value;
    this->ui->label_MaxValue->setText(QString::number(value));
    setThreshold();
}

/*
    Creates a new detection form if necessary, 
    and sets up the form to begin recording hand
    gestures
*/
void MaskSelection::beginDetection()
{
    if(!gd)
        gd = new GestureDetector(this);

    gd->show();

    if(cap.isOpened())
    {
        toggleCamera();
        gd->setCap(cap);
        gd->start();
    }

}








