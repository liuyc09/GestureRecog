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


#include "../src/gesturedetector.h"

// for recording and documentation purposes
static const char *DESKTOP = "/Users/on2valhalla/Desktop/pass";

GestureDetector::GestureDetector(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::GestureDetector)
{
	ui->setupUi(this);
	timer = new QTimer();

	cascadeFace = cv::CascadeClassifier(FACEFILE);

	connect(timer, SIGNAL(timeout()), this, SLOT(updateTimer()));
	connect(ui->btnPause, SIGNAL(clicked()), this, SLOT(pause()));

    setCount = 0;

    intro = "Welcome!\n"
			"You can display any combination of the following "
			"to attempt to unlock the password (1 or 2 at a time):\n"
			"{PALM, FIST, POINT UP, NONE, OTHER}\n\n"
            "To check your entry hold 2 palms up.";

}

GestureDetector::~GestureDetector()
{
	delete timer;
	delete ui;
}

//On form close button, clean up
void GestureDetector::reject()
{
	timer->stop();
	ui->textEdit->setText("");
	QDialog::reject();
}

//Set the video capture device
void GestureDetector::setCap(cv::VideoCapture &cap)
{
	this->cap = cap;
}

//Start the timer
void GestureDetector::start()
{
	if(cap.isOpened())
		timer->start(DELAY);
	timeCount = 0;
	warnCount = 0;
	ui->textEdit->setText(QString::fromStdString(intro));
	imageCache.clear();
	pw.reset();
	ui->textEdit->append("\n\nPREPARE FOR CAPTURE IN....\n3....");
}

// Stop and Start the video
void GestureDetector::pause()
{
	if(timer->isActive())
	{
		timer->stop();
		ui->btnPause->setText("Play");
	}
	else
	{
		start();
		ui->btnPause->setText("Pause");
	}
}




//Convert cv::Mat to QImage and display
void GestureDetector::displayMat(const cv::Mat& image)
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
	this->ui->lblImage->setPixmap(img_pix.scaled(ui->lblImage->size(), Qt::KeepAspectRatio));
}

void GestureDetector::updateTimer()
{
	timeCount = (timeCount + DELAY) % RECINT; //increment counter on a cycle
	warnCount = (warnCount + DELAY) % WARNINT; //do same with the warning count
	cv::Mat image, filtered;
    cap >> image;

	//set the image from the camera, process it and get filtered
	SkinDetectController::getInstance()->setInputImage(image);
	SkinDetectController::getInstance()->process();
	filtered = SkinDetectController::getInstance()->getLastResult(); //binary image of blobs

	//retrieve the hand gestures from the image from the image
	std::vector<Hand> hands = detect(image, filtered);

	if(timeCount < DELAY)
	{ // Only store the hands when the timeCount
		//rolls over after iterating to the record
		//interval (RECINT)

		pw.addHandSet(hands);

		imageCache.push_back(image.clone());

		//print out the captured hands
		if(hands.size() > 1)
			ui->textEdit->append(hands[0].toQString()
						 + "\n  and: " + hands[1].toQString());
		else
			ui->textEdit->append(hands[0].toQString());

	
	    // Determine if it is time to check the password
	    if(pw.doCheck(hands))
	    {
			if(pw.checkPassword())
				ui->textEdit->append("----------------------\n"
	                                "PASSWORD ACCEPTED!!!!!!\n"
	                                "----------------------");
			else
	            ui->textEdit->append("----------------------\n"
	                                "INTRUDER... INTRUDER....\n"
	                                "----------------------");

	        char filename[200];
	        //record pictures for documentation
	        for(int i = 0; i < imageCache.size(); i++)
	        {
	        	sprintf(filename, "%s_%d_%d.jpg", DESKTOP, setCount, i);
                cvSaveImage(filename, &(IplImage(imageCache[i])));
            }


			pw.reset();
			setCount++;
			imageCache.clear();
	     }
	}

	// warn the user of the time to capture
	// ticks down from WARNMAX to capture
	if(warnCount < DELAY)
	{
		int timeLeft = WARNMAX - timeCount/WARNINT;
		if(timeLeft == WARNMAX)
			ui->textEdit->append("\n\nPREPARE FOR CAPTURE IN....");
		ui->textEdit->append(QString("%1...........").arg(timeLeft));
	}
	
	// display the processed image
	displayMat(image);
}


// Detect and Identify the hand gesture present in the image
std::vector<Hand> GestureDetector::detect(cv::Mat &image, cv::Mat &filtered)
{
	std::vector<Hand> hands;

	// find contours in the filtered image
	std::vector< std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours(filtered,
					contours, // a vector of contours
					hierarchy, // a hierarchy of contours if there are parent
								//child relations in the image
					CV_RETR_EXTERNAL, // retrieve the external contours
					CV_CHAIN_APPROX_TC89_L1); // an approximation algorithm



	//------------------Find Face----------------
	//preprocess for face recognition
	std::vector< cv::Rect > faces;
	cv::Mat gray;
	//shrink the image for speed
	cv::resize(image, gray, cv::Size2i(image.cols/4, image.rows/4));
	cv::cvtColor(gray, gray, CV_BGR2GRAY);
	cv::equalizeHist(gray, gray);
	cascadeFace.detectMultiScale(gray, faces);

	// draw bounds for faces
    for (unsigned int i = 0; i < faces.size(); i++ )
	{
		//resize the rectangle to match the image
		faces[i] += cv::Point(faces[i].x * 3,faces[i].y * 3);
		faces[i] += faces[i].size() +faces[i].size() +faces[i].size();
		rectangle(image, faces[i], cv::Scalar(0,0,204), 3);
	}
	//----------------End Face--------------------



	//------------Find all skin regions-----------
	QString join("");
	int massMin = 5000, massMax = 24000;  // minimum contour mass
    cv::Scalar color( 100, 150, 255 ); //random color
	int idx = 0;

	// iterate through all the top-level contours
	for( ; idx >= 0; idx = hierarchy[idx][0] )
	{
		// compute all moments and mass
		cv::Moments mom = cv::moments(cv::Mat(contours[idx]));
		cv::Point center = cv::Point(mom.m10/mom.m00,mom.m01/mom.m00);
		int cMass = mom.m00;

		//calculate the rotated and bounding rectangles of the blobs
		cv::RotatedRect rotRect = cv::minAreaRect( cv::Mat(contours[idx]) );
		cv::Rect bRect = rotRect.boundingRect();


		// find intersections of detected faces and blobs
		bool faceOverlap = false;
        for(unsigned int i = 0; i < faces.size(); i++)
		{
			cv::Rect intersect = faces[i] & bRect;
			if(intersect.width > 30 || intersect.height > 30)
				faceOverlap = true;
		}

		// Eliminate contours that are too small,
		// and ones that coincide with one of the found faces
		if(cMass > massMin && cMass < massMax && !faceOverlap)
		{
			Hand curHand(rotRect, mom);
			hands.push_back(curHand); //push the detected hand onto the list

			// draw each connected component
			cv::Mat contourImg(image.size(), image.type(), cv::Scalar(0));
			cv::drawContours( contourImg, contours, idx, color, CV_FILLED, 8, hierarchy );
			cv::GaussianBlur(contourImg, contourImg, cv::Size(5,5), 0);
			image += contourImg;

			// draw bounding rotated rectangles
			for( int j = 0; j < 4; j++ )
				line( image, curHand.rotPoints[j], curHand.rotPoints[(j+1)%4], cv::Scalar(0,102,204), 3, 8 );

			rectangle(image, curHand.boxRect, cv::Scalar(0,204,102), 3);

		}
	}
	if(hands.size() == 0)
	{  //if no hands were found create a 'NONE' hand
		hands.push_back(Hand());
	}

	return hands;
}















