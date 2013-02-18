#include "gesturedetector.h"
#include "ui_gesturedetector.h"

GestureDetector::GestureDetector(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::GestureDetector)
{
	ui->setupUi(this);
	timer = new QTimer();

	cascadeFace = cv::CascadeClassifier(FACEFILE);

	connect(timer, SIGNAL(timeout()), this, SLOT(updateTimer()));
	connect(ui->btnPause, SIGNAL(clicked()), this, SLOT(pause()));

	ui->textEdit->setText("detector started\n");

	timeCount = 0;

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
	QDialog::reject();
}

void GestureDetector::setCap(cv::VideoCapture &cap)
{
	this->cap = cap;
}

void GestureDetector::start()
{
	if(cap.isOpened())
		timer->start(DELAY);
	timeCount = 0;
}

void GestureDetector::pause()
{
	if(timer->isActive())
	{
		timer->stop();
		ui->btnPause->setText("Play");
	}
	else
	{
		timeCount = 0;
		timer->start(DELAY);
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
	timeCount = (timeCount + DELAY) % PASSINT;
	cv::Mat image, filtered;
    cap >> image;

	//set the image from the camera, process it and get filtered
	SkinDetectController::getInstance()->setInputImage(image);
	SkinDetectController::getInstance()->process();
	filtered = SkinDetectController::getInstance()->getLastResult(); //binary image of blobs

	//retrieve the hand gestures from the image from the image
	std::vector<Hand> hands = detect(image, filtered);
	pw.addHandSet(hands);

    //if(hands.size() == 2 && hands[0].type == PALM && hands[1].type == PALM)
    if(timeCount < DELAY)
    {
		if(pw.checkPassword())
		{
			ui->textEdit->append("YAAAAAAAAAAAAAAAAYYYYYAYAYAYAYAYAYAYAY");
			pw.reset();
		}
		else
		{
            ui->textEdit->append(QString("Nope %1").arg(timeCount));
            pw.reset();
        }
     }

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
	if(faces.size() > 1)
		ui->textEdit->append("!!!!!!Tell the other guy to leave!!!!!!!");
	// draw bounds for faces
    for (unsigned int i = 0; i < faces.size(); i++ )
	{
		//resize the rectangle to match the image
		faces[i] += cv::Point(faces[i].x * 3,faces[i].y * 3);
		faces[i] += faces[i].size() +faces[i].size() +faces[i].size();
		//rectangle(image, faces[i], cv::Scalar(0,0,204), 3);
	}
	//----------------End Face--------------------



	//------------Find all skin regions-----------
	QString join("");
	int massMin = 2500, massMax = 24000;  // minimum contour mass
	cv::Scalar color( 234, 180, 194 ); //random color
	int idx = 0;

	// iterate through all the top-level contours
	for( ; idx >= 0; idx = hierarchy[idx][0] )
	{
		if(hands.size() > 0)
			join = "\tand: ";

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

			// print out the type of gesture found
            if(curHand.type == FIST)
				ui->textEdit->append(join + "+++FIST+++" + curHand.toQString());
            else if(curHand.type == PALM)
				ui->textEdit->append(join + "---PALM---" + curHand.toQString());
            else if(curHand.type == POINT)
				ui->textEdit->append(join + "===POINT===" + curHand.toQString());
			else //this is an unknown, non face gesture, print its details
				ui->textEdit->append(join + curHand.toQString());
		}
		else if(faceOverlap)
		{
			// this blob is a face, so draw an ellipse around it
			cv::ellipse(image, rotRect, cv::Scalar(206,151,90), 3, 8);
		}
	}
	if(hands.size() == 0)
	{
		hands.push_back(Hand());
		ui->textEdit->append("\\NONE\\");
	}

	return hands;
}















