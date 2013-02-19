/*
	Modified from ColorDetector from chapter 3 of the cookbook:
	Computer Vision Programming using the OpenCV Library.
	by Robert Laganiere, Packt Publishing, 2011.
*/

#include "../include/skindetector.h"
#include "../include/colorhistogram.h"

cv::Mat SkinDetector::processHSV(const cv::Mat &hsvImg)
{
	//filtering parameter, increase size for greater effect
	cv::Mat morpElement(5,5,CV_8U,cv::Scalar(1));

	//re-allocate binary map if necessary
	//if so create one channel image with 
	//same cols and rows as original
	resultImg.create(hsvImg.rows, hsvImg.cols, CV_8U);

	ColorHistogram h;

	//not sure if this will help *** test
    h.colorReduce(hsvImg, 12);

	cv::inRange(hsvImg, hsvThreshold[0], hsvThreshold[1], resultImg);

	cv::bitwise_not(resultImg, resultImg);
	cv::erode(resultImg, resultImg, morpElement);
    cv::blur(resultImg, resultImg, morpElement.size());
	// cv::erode(resultImg, resultImg, morpElement);
	// cv::dilate(resultImg, resultImg, morpElement);
	// cv::dilate(resultImg, resultImg, morpElement);
	//cv::morphologyEx(resultImg,resultImg,cv::MORPH_CLOSE, morpElement);
    

	return resultImg;
}
