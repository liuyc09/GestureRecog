/*

Created by: Jason Carlisle Mann (on2valhalla | jcm2207@columbia.edu)

This class is a QT based form (QDialog), with a display area for images
or video, and a text pane for output

Using the PasswordCheck class, it samples a video feed provided by
cv::VideoCapture and analyzes it for skin blobs based on the SkinDetector
class. It identifies faces first and labels them with a rectangle.
The remaining skin blobs are analyzed as hand gestures, for the purpose
of completing a password.

*/

#ifndef GESTUREDETECTION_H
#define GESTUREDETECTION_H

#include <QDialog>
#include <QTimer>
#include <QDebug>

//sys
#include <iostream>
#include <string>
#include <cmath>
#include <stdio.h>

//OpenCV
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/highgui.h>
#include <opencv2/objdetect/objdetect.hpp>

//locals
#include "../src/ui_gesturedetector.h"
#include "../include/colorhistogram.h"
#include "../include/skindetectcontroller.h"
#include "../src/passwordcheck.h"
#include "../include/hand.h"

// Haar Cascade Classifier face file location
static std::string FACEFILE = "/opt/local/share/OpenCV/haarcascades/haarcascade_frontalface_alt_tree.xml";

namespace Ui {
	class GestureDetector;
}

class GestureDetector : public QDialog
{
	Q_OBJECT
	
public:
	explicit GestureDetector(QWidget *parent = 0);
	~GestureDetector();

	void start();
	void setCap(cv::VideoCapture &cap);

	
private:
	Ui::GestureDetector *ui;
	QTimer *timer;
	cv::VideoCapture cap;
	cv::CascadeClassifier cascadeFace; //HAAR Cascade for detecting faces
	PasswordCheck pw;  //Class containing the passwords
	std::string intro;

	// TIMER VARS
	static const int DELAY = 30; //Timer delay, determines how fast video plays
	static const int RECINT = 3000; //Interval between gesture captures
	static const int WARNINT = 1000; //Interval to print warnings to user
	static const int WARNMAX = RECINT / WARNINT; //Maximum warning num
	int timeCount;
	int warnCount;

	//for recording and documentation
	int setCount;
	std::vector<cv::Mat> imageCache;


	//display an image in the dialog
	void displayMat(const cv::Mat &image);

	// Detect and Identify the hand gesture present in the image
	std::vector<Hand> detect(cv::Mat &image, cv::Mat &filtered);


private slots:
	void updateTimer();
	void reject();
	void pause();
};


#endif // GESTUREDETECTION_H
