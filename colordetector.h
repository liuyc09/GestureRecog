/*------------------------------------------------------------------------------------------*\
   This file contains material supporting chapter 3 of the cookbook:
   Computer Vision Programming using the OpenCV Library.
   by Robert Laganiere, Packt Publishing, 2011.

   This program is free software; permission is hereby granted to use, copy, modify,
   and distribute this source code, or portions thereof, for any purpose, without fee,
   subject to the restriction that the copyright notice may not be removed
   or altered from any source or altered source distribution.
   The software is released on an as-is basis and without any warranties of any kind.
   In particular, the software is not guaranteed to be fault-tolerant or free from failure.
   The author disclaims all warranties with regard to this software, any use,
   and any consequent failure, is purely the responsibility of the user.

   Copyright (C) 2010-2011 Robert Laganiere, www.laganiere.name
\*------------------------------------------------------------------------------------------*/

#if !defined COLORDETECT
#define COLORDETECT

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

class ColorDetector {

  private:

      // minimum acceptable distance
      int minDist;

      // target color in HSV!!!
      cv::Vec3b target;

      // image containing resulting binary map
      cv::Mat result;

      // image containing converted color space
      cv::Mat converted;

      // inline private member function
      // Computes the distance from target color.
      int getDistance(const cv::Vec3b& color) const {
        return static_cast<int>(cv::norm<int,3>(
            cv::Vec3i(color[0]-target[0],
                    color[1]-target[1],
                    color[2]-target[2])));
      }

  public:

      // empty constructor
      ColorDetector() : minDist(100) {

          // default parameter initialization here
          target[0]= target[1]= target[2]= 0;
      }

      // Getters and setters

      // Sets the color distance threshold.
      // Threshold must be positive, otherwise distance threshold
      // is set to 0.
      void setColorDistanceThreshold(int distance) {

          if (distance<0)
              distance=0;
          minDist= distance;
      }

      // Gets the color distance threshold
      int getColorDistanceThreshold() const {

          return minDist;
      }

      // Sets the color to be detected
      void setTargetColor(unsigned char red, unsigned char green, unsigned char blue)
      {
        // Temporary 1-pixel image
        cv::Mat tmp(1,1,CV_8UC3);
        tmp.at<cv::Vec3b>(0,0)[0]= blue;
        tmp.at<cv::Vec3b>(0,0)[1]= green;
        tmp.at<cv::Vec3b>(0,0)[2]= red;

        // Converting the target to Lab color space
        cv::cvtColor(tmp, tmp, CV_BGR2HSV);
        target= tmp.at<cv::Vec3b>(0,0);
      }

        // Sets the color to be detected
        void setTargetColor(cv::Vec3b color)
        {

          target= color;
      }

      // Gets the color to be detected
      cv::Vec3b getTargetColor() const {

          if(target[0] == 0 && target[1] == 0 && target[2] == 0)
              return target;

          // Temporary 1-pixel image
          cv::Mat tmp(1,1,CV_8UC3);
          tmp.at<cv::Vec3b>(0,0) = target;


          // Converting the target to Lab color space
          cv::cvtColor(tmp, tmp, CV_HSV2BGR);
          return tmp.at<cv::Vec3b>(0,0);
      }

      // Processes the image. Returns a 1-channel binary image.
      cv::Mat process(const cv::Mat &image);
};


#endif
