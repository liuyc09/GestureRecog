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
	// VARIABLES
	GestureDetector::HandType type;
	cv::RotatedRect rotRect;
	cv::Point2f rotPoints[4];
	cv:Rect boxRect;
	cv::Moments mom;	
	double rRatio;
	double bRatio;


	// Easy Calculation of Euclidean Distance
	double pointDist(cv::Point2f &p1, cv::Point2f &p2)
	{
		double dx = p1.x - p2.x;
		double dy = p1.y - p2.y;
		return sqrt(dx * dx + dy * dy);
	}

	void setType()
	{
		//decision for fist gesture
		if(bRatio > .7 && bRatio < 1.5 &&
			rRatio > .7 && rRatio < 1.7 &&
			cMass < 8500)
			type = HandType.FIST;
		//decision for palm gesture
		else if(bRatio > .4 && bRatio < .9 &&
					rRatio > 1.5 && rRatio < 2.1 &&
					cMass > 8000 && cMass < 15000)
			type = HandType.PALM;
		//decision for point gesture
		//need two ranges for the inverse
		//because the rotated rect gives off opposite 
		//width and height sometimes.
		else if(bRatio > .4 && bRatio < .9 &&
					((rRatio > 1.9 && rRatio < 2.6) ||
						(rRatio > .35 && rRatio < .55)) && 
					cMass > 3000 && cMass < 9000)
			type = HandType.POINT;
		//else print out 
		else
			type = HandType.UNK;
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

		setType();
	}

	Hand()
	{
		type = HandType.NONE;
	}

};


#endif