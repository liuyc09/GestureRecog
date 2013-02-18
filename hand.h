#ifndef HAND_H
#define HAND_H

#include "gesturedetector.h"
#include <opencv2/core/core.hpp>


enum HandType{
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
    HandType type;
	cv::RotatedRect rotRect;
	cv::Point2f rotPoints[4];
    cv::Rect boxRect;
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
            mom.m00 < 8500) //mass of the gesture
			type = FIST;
		//decision for palm gesture
		else if(bRatio > .4 && bRatio < .9 &&
					rRatio > 1.5 && rRatio < 2.1 &&
                    mom.m00 > 8000 && mom.m00 < 15000) //mass of the gesture
			type = PALM;
		//decision for point gesture
		//need two ranges for the inverse
		//because the rotated rect gives off opposite 
		//width and height sometimes.
		else if(bRatio > .4 && bRatio < .9 &&
					((rRatio > 1.9 && rRatio < 2.6) ||
						(rRatio > .35 && rRatio < .55)) && 
					mom.m00 > 3000 && mom.m00 < 9000)
			type = POINT;
		//else print out 
		else
			type = UNK;
	}

	QString toQString()
	{
		return QString("{area: %1  bratio: %2  rratio: %3  rot: %4}")
											.arg(mom.m00)
											.arg(bRatio,0,'f',3)
											.arg(rRatio,0,'f',3)
											.arg(rotRect.angle);
	}







	//Default constructor
	Hand()
	{
		type = NONE;
	}

	//Constructor
	Hand(cv::RotatedRect r, cv::Moments m)
	{
		rotRect = r;
		mom = m;

		rotRect.points(rotPoints);
		boxRect = rotRect.boundingRect();

		rRatio = pointDist(rotPoints[2],rotPoints[1]) / pointDist(rotPoints[2], rotPoints[3]);
		bRatio = static_cast<double>(boxRect.width)/boxRect.height;

		setType();
	}

	//copy constructor
	Hand(const Hand& h)
	{
		type = h.type;
		rotRect = h.rotRect;
		for(int i = 0; i < sizeof(rotPoints); i++)
			rotPoints[i] = h.rotPoints[i];
		boxRect = h.boxRect;
		mom = h.mom;
		bRatio = h.bRatio;
		rRatio = h.rRatio;
	}

	//assignment operator
	Hand& operator=(const Hand& rhs)
	{
		if(this == &rhs)
			return *this;

		type = rhs.type;
		rotRect = rhs.rotRect;
		for(int i = 0; i < sizeof(rotPoints); i++)
			rotPoints[i] = rhs.rotPoints[i];
		boxRect = rhs.boxRect;
		mom = rhs.mom;
		bRatio = rhs.bRatio;
		rRatio = rhs.rRatio;

		return *this;
	}

	//destructor
	~Hand()
	{

	}
};


#endif
