/*
	Modified from ColorDetector from chapter 3 of the cookbook:
	Computer Vision Programming using the OpenCV Library.
	by Robert Laganiere, Packt Publishing, 2011.
*/

#include "skindetector.h"
#include "colorhistogram.h"

cv::Mat SkinDetector::processHSV(const cv::Mat &hsvImg)
{
	//re-allocate binary map if necessary
	//if so create one channel image with 
	//same cols and rows as original
	resultImg.create(hsvImg.rows, hsvImg.cols, CV_8U);

	ColorHistogram h();

	//not sure if this will help *** test
	//h.colorReduce(hsvImg);

	cv::inRange(hsvImg, hsvThreshold[0], hsvThreshold[1], resultImg);

	return resultImg;
}