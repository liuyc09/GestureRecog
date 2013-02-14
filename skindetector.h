/*
	Modified from ColorDetector from chapter 3 of the cookbook:
	Computer Vision Programming using the OpenCV Library.
	by Robert Laganiere, Packt Publishing, 2011.

	This Class uses inputted ranges to detect skin from
	HSV images
*/

#if !defined SKINDETECT
#define SKINDETECT

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>



class SkinDetector
{

	private:

		// HSV min and max limits as array of Scalars
        cv::Vec3b hsvThreshold[2];

		// image containing result of processing
		cv::Mat resultImg;

		// image containing converted color space
		cv::Mat converted;


	public:
		//empty Constructor
		SkinDetector()
		{
			// initialize default parameters
			hsvThreshold[0][0] = 50;
			hsvThreshold[0][1] = 50;
			hsvThreshold[0][2] = 50;
			hsvThreshold[1][0] = 150;
			hsvThreshold[1][1] = 150;
			hsvThreshold[1][2] = 50;
		}

        void setThreshold(cv::Vec3b min, cv::Vec3b max)
		{
			hsvThreshold[0] = min;
			hsvThreshold[1] = max;
		}

		void getThreshold(cv::Vec3b &min, cv::Vec3b &max)
		{
			min = hsvThreshold[0];
			max = hsvThreshold[1];
		}

		// Processes a already HSV image. Returns a 1-channel binary image.
		cv::Mat processHSV(const cv::Mat &image);
};

#endif
