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
}

void GestureDetector::beginPicSequence()
{}

void GestureDetector::pause()
{
	if(timer->isActive())
		timer->stop();
	else
		timer->start(DELAY);
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
	cv::Mat image, filtered;
	cap >> image;

	//set the image from the camera, process it and get filtered
	SkinDetectController::getInstance()->setInputImage(image);
	SkinDetectController::getInstance()->process();
	filtered = SkinDetectController::getInstance()->getLastResult();

	detect(image, filtered);

	displayMat(image);
}


// Detect and Identify the hand gesture present in the image
void GestureDetector::detect(cv::Mat &image, cv::Mat &filtered)
{
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
	for (int i = 0; i < faces.size(); i++ )
	{
		//resize the rectangle to match the image
		faces[i] += cv::Point(faces[i].x * 3,faces[i].y * 3);
		faces[i] += faces[i].size() +faces[i].size() +faces[i].size();
		//rectangle(image, faces[i], cv::Scalar(0,0,204), 3);
	}
	//----------------End Face--------------------



	//------------Find all skin regions-----------

	int massMin = 2500;  // minimum contour mass
	cv::Scalar color( 234, 180, 194 ); //random color
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
		for(int i = 0; i < faces.size(); i++)
		{
			cv::Rect intersect = faces[i] & bRect;
			if(intersect.width > 30 && intersect.height > 30)
				faceOverlap = true;
		}

		// Eliminate contours that are too small,
		// and ones that coincide with one of the found faces
		if(cMass > massMin && !faceOverlap)
		{
			double bRatio = static_cast<double>(bRect.width)/bRect.height;

			//find the ratio of the rotated rect
			cv::Point2f rectPoints[4];
			rotRect.points(rectPoints);
			double rRatio = pointDist(rectPoints[2],rectPoints[1]) / pointDist(rectPoints[2], rectPoints[3]);


			// draw each connected component
			cv::Mat contourImg(image.size(),image.type(), cv::Scalar(0));
			cv::drawContours( contourImg, contours, idx, color, CV_FILLED, 8, hierarchy );
			cv::GaussianBlur(contourImg, contourImg, cv::Size(5,5), 0);
			image += contourImg;

			// draw mass center
			cv::circle(image,
					// position of mass center converted to integer
					center,
					2, cv::Scalar(255),2); // draw blue dot

			// draw bounding rotated rectangles
			for( int j = 0; j < 4; j++ )
				line( image, rectPoints[j], rectPoints[(j+1)%4], cv::Scalar(0,102,204), 3, 8 );

			rectangle(image, bRect, cv::Scalar(0,204,102), 3);


			//decision for fist gesture
			if(bRatio > .7 && bRatio < 1.5 &&
						rRatio > .7 && rRatio < 1.7 &&
						cMass < 8500)
				ui->textEdit->append("+++++++FIST+++++++");
			//decision for palm gesture
			else if(bRatio > .4 && bRatio < .9 &&
						rRatio > 1.5 && rRatio < 2.1 &&
						cMass > 8000 && cMass < 15000)
				ui->textEdit->append("-----PALM------");
			//decision for palm gesture
			//need two ranges for the inverse
			//because the rotated rect gives off opposite 
			//width and height sometimes.
			else if(bRatio > .4 && bRatio < .9 &&
						((rRatio > 1.9 && rRatio < 2.6) ||
							(rRatio > .35 && rRatio < .55)) && 
						cMass > 3000 && cMass < 9000)
				ui->textEdit->append("=====POINT======");
			//else print out 
			else
				ui->textEdit->append(QString("contour: %1\tarea: %2\tbratio: %3\trratio: %4\trot: %5")
											.arg(idx).arg(cMass)
											.arg(bRatio,0,'f',3)
											.arg(rRatio,0,'f',3)
											.arg(rotRect.angle));
		}
		else if(faceOverlap)
		{
			cv::ellipse(image, rotRect, cv::Scalar(0,0,255), 3, 8);
		}
	}
}















