/*
	Modified from ColorDetector from chapter 3 of the cookbook:
	Computer Vision Programming using the OpenCV Library.
	by Robert Laganiere, Packt Publishing, 2011.
*/

#include "skindetector.h"
#include "colorhistogram.h"

cv::Mat SkinDetector::processHSV(const cv::Mat &hsvImg)
{
	//filtering parameter, increase size for greater effect
	cv::Mat element3(3,3,CV_8U,cv::Scalar(1));

	//re-allocate binary map if necessary
	//if so create one channel image with 
	//same cols and rows as original
	resultImg.create(hsvImg.rows, hsvImg.cols, CV_8U);

	ColorHistogram h;

	//not sure if this will help *** test
    h.colorReduce(hsvImg, 12);

	cv::inRange(hsvImg, hsvThreshold[0], hsvThreshold[1], resultImg);

    
    cv::blur(hsvImg, hsvImg, element3.size());
	// cv::erode(resultImg, resultImg, element5);
	// cv::erode(resultImg, resultImg, element5);
	// cv::dilate(resultImg, resultImg, element5);
	// cv::dilate(resultImg, resultImg, element5);
	// cv::morphologyEx(resultImg,resultImg,cv::MORPH_CLOSE, cv::Mat());
    

	return resultImg;
}
