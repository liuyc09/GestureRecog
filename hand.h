#ifndef HAND_H
#define HAND_H

#include "gesturedetector.h"
#include <opencv2/core/core.hpp>


enum HandType
{
	FIST,
	PALM,
	POINT,
	UNK,
	NONE
};

class Hand
{

public:

	// Easy Calculation of Euclidean Distance
	double pointDist(cv::Point2f &p1, cv::Point2f &p2)
	{
		double dx = p1.x - p2.x;
		double dy = p1.y - p2.y;
		return sqrt(dx * dx + dy * dy);
	}







	//Constructor
	Hand(cv::RotatedRect r, cv::Moments m)
	{
		rotRect = r;
		mom = m;

		rotRect.points(rotPoints);
		boxRect = rotRect.boundingRect();

		rRatio = pointDist(rectPoints[2],rectPoints[1]) / pointDist(rectPoints[2], rectPoints[3]);
		bRatio = static_cast<double>(boxRect.width)/boxRect.height;
	}






private:
	GestureDetector::HandType type;
	cv::RotatedRect rotRect;
	cv::Point2f rotPoints[4];
	cv:Rect boxRect;
	cv::Moments mom;
	double rRatio;
	double bRatio;





};


#endif