#ifndef GUARD_shapeContour_h
#define GUARD_shapeContour_h

#include <opencv2/opencv.hpp>
#include<string>
#include<stdlib.h>


double angle(cv::Point,cv::Point,cv::Point);
void setLabel(cv::Mat&,const std::string,std::vector<cv::Point>&);
int lengthSquare(cv::Point, cv::Point);
float printAngle(cv::Point,cv::Point,cv::Point);
bool compareContourAreas ( std::vector<cv::Point> contour1, std::vector<cv::Point> contour2 );
void function();



#endif
