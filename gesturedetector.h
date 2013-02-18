#ifndef GESTUREDETECTION_H
#define GESTUREDETECTION_H

#include <QDialog>
#include <QTimer>

//sys
#include <iostream>
#include <string>
#include <cmath>

//OpenCV
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>

//color detector, controller, histogram
#include "colorhistogram.h"
#include "skindetectcontroller.h"

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
	void beginPicSequence();


	
	// Easy Calculation of Euclidean Distance
	double pointDist(cv::Point2f &p1, cv::Point2f &p2)
	{
		double dx = p1.x - p2.x;
		double dy = p1.y - p2.y;
		return sqrt(dx * dx + dy * dy);
	}

	
private:
	Ui::GestureDetector *ui;
	QTimer *timer;
	cv::VideoCapture cap;
	cv::CascadeClassifier cascadeFace;

	static const int DELAY = 1000;

	//display an image in the dialog
	void displayMat(const cv::Mat &image);

	// Detect and Identify the hand gesture present in the image
	void detect(cv::Mat &image, cv::Mat &filtered);


private slots:
	void updateTimer();
	void reject();
	void pause();
};


#endif // GESTUREDETECTION_H
