#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cmath>
#include <iostream>
#include <chrono>
#include <unistd.h>
#include "shapeContour.h"


int main(int argc, char** argv)
{
    cv::VideoCapture cap;
    if(!cap.open(0))
           return 0;
    for(;;){
        cv::Mat frame;
        cap>>frame;
        if( frame.empty() ) break; // end of video stream

//    auto start = std::chrono::steady_clock::now();

    std::vector<cv::Point> allCenter;
    std::vector<cv::Point> triangleCenter;
    std::vector<cv::Point> rectangleCenter;
    std::vector<cv::Point> rec;

//    cv::Mat src = cv::imread("/home/jasmine/Belgeler/images2/test7.jpeg");
//    if(src.empty())
//    {
//        std::cout << "Problem loading image!!!" << std::endl;
//        return -1;
//    }

    cv::Mat gray;
    cvtColor(frame, gray, cv::COLOR_BGR2GRAY,2);

    cv::Mat bilateral_output;
    cv::bilateralFilter(gray,bilateral_output,5,75,75,cv::BORDER_DEFAULT);


    cv::Mat threshold_output;
    cv::adaptiveThreshold(bilateral_output, threshold_output, 240, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY,11,3);

    std::vector<std::vector<cv::Point> > contours;
    findContours(threshold_output, contours, cv::RETR_LIST,cv:: CHAIN_APPROX_NONE);

    std::vector<cv::Point> approx;
    std::vector<int> triangle;
    std::vector<int> circle;
    std::vector<int> rectangle;
    std::vector<int> square;





        for (int i = 0; i <contours.size(); i++)
        {

            // Approximate contour with accuracy proportional to the contour perimeter
            cv::approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true)*0.04, true);

            if (std::fabs(cv::contourArea(contours[i])) < 5000)
                continue;


            if (approx.size() == 3)
            {


                float Ox = (approx[0].x + approx[1].x + approx[2].x)/3;
                float Oy = (approx[0].y + approx[1].y + approx[2].y)/3;

                cv::Point center = cv::Point(Ox,Oy);
//                std::cout<<"triangle center: "<<center<<std::endl;

               float Area = printAngle(approx[0],approx[1],approx[2]);
               if(cv::contourArea(contours[i])-Area<=1000){

//                  std::cout << "Tcenter: "<<allCenter.size()<< std::endl;
                  int ccc = 0;

                  if(triangleCenter.size()==0){
                      triangleCenter.push_back(center);
                      triangle.push_back(i);
                  }
                  else{
                    for(size_t n=0; n < triangleCenter.size(); n++){
                        if(std::abs(center.y - triangleCenter[n].y) <5){
                            std::cout << "triangle"<< std::endl;
                            ccc++;
                        }
                    }
                    if(ccc==0)
                    {
                        triangleCenter.push_back(center);
                        triangle.push_back(i);

                    }

                   }


            }
        }



            else if (approx.size() >= 4 && approx.size() <= 6)
            {
                // Number of vertices of polygonal curve
                int vtc = approx.size();

                // Get the cosines of all corners
                std::vector<double> cos;
                for (int j = 2; j < vtc+1; j++)
                    cos.push_back(angle(approx[j%vtc], approx[j-2], approx[j-1]));

                // Sort ascending the cosine values
                std::sort(cos.begin(), cos.end());

                // Get the lowest and the highest cosine
                double mincos = cos.front();
                double maxcos = cos.back();


                if (vtc == 4 && mincos >= -0.1 && maxcos <= 0.3){

                    cv::Rect r=cv::boundingRect(contours[i]);
                    double ratio=std::abs(1-(double)r.width/r.height);


                    //approx değerlerim benim köşe noktalarım ve approx değerleri random olarak geliyor
                    //bunu düzeltmek için approxları x lerin yakınlık durumuna göre sıralamaya çalışma

                      if(std::abs(approx[0].x-approx[1].x)<3){
                          continue;
                      }
                      else if(std::abs(approx[0].x-approx[2].x)<3){
                          rec.push_back(approx[1]);
                          approx[1]=approx[2];
                          approx[2]=rec[0];
                          continue;
                      }
                      else if(std::abs(approx[0].x-approx[3].x)<3){
                          rec.push_back(approx[1]);
                          approx[1]=approx[3];
                          approx[3]=rec[0];
                      }


                      std::cout<<"\n"<<std::endl;

                      float Mx = (approx[0].x + approx[2].x)/2;
                      float My = (approx[0].y + approx[2].y)/2;

                      cv::Point Rcenter = cv::Point(Mx,My);


                      int count=0;
                      if(rectangleCenter.size()==0){
                          rectangleCenter.push_back(Rcenter);
                         if(ratio<=0.05){
                             square.push_back(i);
                         }else{
                          rectangle.push_back(i);
                         }

                      }
                      else{
                          for(size_t j=0;j<rectangleCenter.size();j++){

                              if(std::abs(Rcenter.x - rectangleCenter[j].x)<5)
                                {
                                    std::cout << "rectangle"<< std::endl;
                                    count++;
                                 }
                            }

                          }
                          if(count==0){
                              rectangleCenter.push_back(Rcenter);
                              if(ratio<=0.05){
                                  square.push_back(i);
                              }else{
                              rectangle.push_back(i);
                              }
                          }
                      }


                else if (vtc == 5 && mincos >= -0.34 && maxcos <= -0.27){
                    cv::Scalar color (145, 60, 255);
                    cv::drawContours( frame, contours,static_cast<int>(i), color,2 );
                    setLabel(frame, "Pentagon", contours[i]);
                }
                else if (vtc == 6 && mincos >= -0.55 && maxcos <= -0.45){
                    cv::Scalar color (255,153,255);
                    cv::drawContours( frame, contours, static_cast<int>(i), color,2 );
                    setLabel(frame, "Hexagon", contours[i]);
                }

            }
            else
            {
                // Detect and label circles
                double area = cv:: contourArea(contours[i]);
                cv::Rect r = cv::boundingRect(contours[i]);
                int radius = r.width /2;

                 if (std::abs(1 - ((double)r.width / r.height)) <= 0.2 && std::abs(1 - (area / (CV_PI * pow(radius, 2)))) <= 0.2){

                       float x = (r.x + (r.width)/2);
                       float y = (r.y + (r.height)/2);
                       cv::Point points = cv::Point(x,y);

                       std::cout << "Ccenter: "<<allCenter.size()<< std::endl;
                       int ccc=0;
                       if(allCenter.size()==0){
                           allCenter.push_back(points);
                           circle.push_back(i);
                       }
                       else{
                           for(size_t j=0;j<allCenter.size();j++){

                               if(std::abs(points.x - allCenter[j].x)<5)
                                 {
                                     std::cout << "circle"<< std::endl;
                                     ccc++;
                                  }
                             }

                           }
                           if(ccc==0){
                               allCenter.push_back(points);
                               circle.push_back(i);
                           }
                       }


}
}
        for(int t=0; t<triangle.size();t++){
            cv::Scalar color (55, 100, 255);

            setLabel(frame, "Triangle", contours[triangle[t]]);
            cv::drawContours( frame, contours,triangle[t], color,2 );
        }

        for(int n=0; n<circle.size();n++){
            cv::Scalar color(255,100,100);
            drawContours(frame,contours,circle[n],color,2);
            setLabel(frame, "Circle", contours[circle[n]]);

        }

        for(int n=0; n<rectangle.size();n++){
            cv::Scalar color (80, 220, 140);
            drawContours(frame,contours,rectangle[n],color,2);
            setLabel(frame, "Rectangle", contours[rectangle[n]]);

        }
        for(int n=0; n<square.size();n++){
            cv::Scalar color (0, 0, 255);
            drawContours(frame,contours,square[n],color,2);
            setLabel(frame, "Square", contours[square[n]]);

        }



//        std::cout<<"\n"<<std::endl;
//        auto end= std::chrono::steady_clock::now();

//        std::cout<<"Elapsed time in milliseconds : "
//                <<std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count()
//               <<"ms"<<std::endl;


    cv::imshow("threshold",threshold_output);
    cv::imshow("output", frame);
    if( cv::waitKey(30) >= 0 ) break;
    }
        return 0;
}
