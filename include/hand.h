#ifndef HAND_H
#define HAND_H

#include "../src/gesturedetector.h"
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
		//need two ranges for the rRatio and inverse
		//because the rotated rect gives off opposite 
		//width and height sometimes.
		if(bRatio > .7 && bRatio < 1.5 &&
				((rRatio >.7 && rRatio < 1.7) ||
				(rRatio > .55 && rRatio < 1.5)) &&
            	mom.m00 < 8500) //mass of the gesture
			type = FIST;
		//decision for palm gesture
		else if(mom.m00 > 8000 && mom.m00 < 15000 &&
					((rRatio > 1.5 && rRatio < 2.1) ||
						(rRatio > .45 && rRatio < .66)) &&
                    bRatio > .4 && bRatio < .9) //mass of the gesture
			type = PALM;
		//decision for point gesture
		else if(mom.m00 > 3000 && mom.m00 < 9000 &&
					((rRatio > 1.6 && rRatio < 2.6) ||
						(rRatio > .35 && rRatio < .55)) && 
					bRatio > .4 && bRatio < .9)
			type = POINT;
		//else print out 
		else
			type = UNK;
	}

	QString toQString()
	{
		QString typeStr("");
		switch (type) 
		{
			case FIST:
				typeStr = "FIST";
				break;
			case PALM:
				typeStr = "PALM";
				break;
			case POINT:
				typeStr = "POINT";
				break;
			case UNK:
				typeStr = "UNK";
				break;
			default:
				typeStr = "NONE";
				break;
		}
		return QString("%1 {area: %2  bratio: %3  rratio: %4  rot: %5}")
											.arg(typeStr)
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
        for(unsigned int i = 0; i < sizeof(rotPoints); i++)
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
        for(unsigned int i = 0; i < sizeof(rotPoints); i++)
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
