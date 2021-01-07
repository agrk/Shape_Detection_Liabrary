#include<opencv2/opencv.hpp>
#include<math.h>

#include "shapeContour.h"

#define PI 3.141592653589793238462643383279502884197169399375105820974944592307816406286


using cv::Point;    using std::string;
using cv::Mat;      using std::vector;



double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0)
{
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}


void setLabel(Mat& im, const string label, std::vector<cv::Point>& contour)
{
    int fontface = cv::FONT_HERSHEY_SIMPLEX;
    double scale = 0.4;
    int thickness = 1.5;
    int baseline = 0;



    cv::Size text = cv::getTextSize(label, fontface, scale, thickness, &baseline);
    cv::Rect r = cv::boundingRect(contour);

    cv::Point pt(r.x + ((r.width - text.width)/2), r.y + ((r.height + text.height)));
    cv::rectangle(im, pt + cv::Point(0, baseline), pt + cv::Point(text.width, -text.height), CV_RGB(255,255,255), CV_FILLED);
    cv::putText(im, label, pt, fontface, scale, CV_RGB(255,100,100), thickness, 8);
}

int lengthSquare(cv::Point p1, cv::Point p2){
    int xDif = p1.x - p2.x;
    int yDif = p1.y - p2.y;

    return xDif*xDif + yDif*yDif;
}
float printAngle(cv::Point p0, cv::Point p1, cv::Point p2){
    float a2 = lengthSquare(p1,p2);
    float b2 = lengthSquare(p0,p2);
    float c2 = lengthSquare(p0,p1);

    float a = sqrt(a2);
    float b = sqrt(b2);
    float c = sqrt(c2);

    float angle1 = acos((b2 + c2 - a2)/(2*b*c));
    float angle2 = acos((a2 + c2 - b2)/(2*a*c));
    float angle3 = acos((a2 + b2 - c2)/(2*a*b));


    angle1 = angle1 * 180 / PI;
    angle2 = angle2 * 180 / PI;
    angle3 = angle3 * 180 / PI;


//   std::cout << "angle1 : " << angle1 << std::endl;
//   std::cout << "angle2 : " << angle2 << std::endl;
//   std::cout << "angle3 : " << angle3 << std::endl;

   float angle = angle1 + angle2 + angle3;
//   std::cout<<"total: "<<angle<<std::endl;


   float kenar = (a*b)*0.5;
   float Area = kenar * sin(angle3*PI/180);

   return Area;
}

bool compareContourAreas ( std::vector<cv::Point> contour1, std::vector<cv::Point> contour2 ) {
    double i = fabs( contourArea(cv::Mat(contour1)) );
    double j = fabs( contourArea(cv::Mat(contour2)) );

    return ( i < j );
}

void function()
{
    long long number = 0;

    for( long long i = 0; i != 2000000; ++i )
    {
       number += 5;
    }
}





