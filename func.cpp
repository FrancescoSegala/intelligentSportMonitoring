/*
FILE func.cpp
contains lots of (and maybe useless) auxiliary an unused function
*/


#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

#include <fstream>

using namespace std;
using namespace cv;
// hue boundary for red green and blue value hsv
#define MAX_HUE_RED 170
#define MIN_HUE_RED 12

#define MAX_HUE_GREEN 47
#define MIN_HUE_GREEN 40

#define MAX_HUE_BLUE 115
#define MIN_HUE_BLUE 100


cv::Mat bin_mask_white(Mat &frame);
std::vector<Vec4i> find_contours_playground(Mat &frame);
std::vector<Vec4i> find_contours_canny(Mat &frame);
void draw_perimeter(Mat &frame,Mat &original);

/*
@params Mat: frame to calculate the binary mask
return : bynary mask of the input frame with white pixel
*/
cv::Mat bin_mask_white(Mat &frame){
  Mat gray, bin_mask;
  cvtColor(frame,gray,COLOR_BGR2GRAY);

  GaussianBlur( gray, gray, Size( 5, 5 ), 0, 0 );

  threshold( gray , bin_mask ,150,255,THRESH_BINARY);

  return bin_mask;
}


/*
@params lines: vector of vector with 4 integer ax ay bx by of the contours
@params Mat : frame to apply the lines
return : draw the lines on the frame
*/
cv::Mat apply_contours(vector<Vec4i> lines,Mat& frame){

  for (size_t i = 0; i < lines.size(); i++) {
    Scalar color=Scalar(0,0,255);

    Point a=Point(lines[i][0],lines[i][1]);
    Point b=Point(lines[i][2],lines[i][3]);

    line( frame, a , b , color, 3, 8 );

  }
  return frame;
}



/*
@params Mat frame: input frame (it can be BGR or Grayscale ) to compute the contourns calculation
return: vector of Vec4i (vector of 4 integer) ax ay bx by
*/
//pipeline: blur--> canny-->hough transforn-->lines
std::vector<Vec4i>  find_contours_canny(Mat &frame){

  Mat gray, drawing;
  if( frame.channels() > 1 )  cvtColor(frame,gray,CV_BGR2GRAY);
  else gray=frame.clone();
  //dovrei fare il test qui
  Canny( gray, gray, 50, 150, 3);

  gray.convertTo(drawing, CV_8U);
  vector<Vec4i> lines;
  HoughLinesP(drawing, lines, 1, CV_PI/180, 80, 75, 10 );

  return lines;
}


/*
@params Mat: frame to calculate the contourns, it must be a binary mask
@params Mat: original frame for drawing contourns
return : void
*/
void draw_perimeter(Mat &frame , Mat &original){
  Mat gray;
  cvtColor(frame,gray,CV_BGR2GRAY);
  Canny(gray,gray,50,150,3);
  vector< vector<Point> > contours;
  vector<Vec4i> hierarchy;
  findContours( gray, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE, Point(0, 0) );
  for (size_t i = 0; i < contours.size(); i++) {

    approxPolyDP(contours[i] , contours[i] ,1 , false );
    drawContours( original, contours, i, Scalar(0,0,255), 2, 8, hierarchy, 0, Point() );
  }
  //imshow("original+",original);
}


/*
@params Mat : frame to calculate the contourns, it must be BGR image
return : vector<Vec4i> vector of vector of 4 int value : ax ay bx by
*/
//usa canny--> find contourns --> draw contourns --> hough lines --> lines
vector<Vec4i> find_contours_playground(Mat &frame){

  Mat gray,drawing;
  if( frame.channels() > 1 )  cvtColor(frame,gray,CV_BGR2GRAY);
  else gray=frame.clone();

  //detect edge
  Canny( gray, gray, 50, 150, 3);
  //find contours
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;
  Moments moms;
  double area;
  double centroidex;
  double centroidey;

  int cy, cx;

  findContours( gray, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE, Point(0, 0) );
  for (size_t i = 0; i < contours.size(); i++) {

    moms=moments(Mat(contours[i]));
    area=moms.m00;
    centroidex =moms.m01;
    centroidey =moms.m10;
    cx=centroidex/area;
    cy=centroidey/area;
    if ( area > 30 ){

      drawContours( frame, contours, i, Scalar(0,0,255), 2, 8, hierarchy, 0, Point() );
      //circle(frame,Point(cx,cy),7,Scalar(0,0,0),8);
    }

  }

  vector<Vec4i> lines;
  HoughLinesP(gray, lines, 1, CV_PI/180, 80, 70 , 10 );

  return lines;
}



/*
@params frame: frame to calculate the binary mask and re-project on it
return void (make side effect)
*/
void project_bin(Mat &frame){

  Vec3b blackPX={0,0,0};
  Vec3b whitePX={255,255,255};
  Vec3b greenPX={0,255,0};

  Mat bin=bin_mask_green(frame);

  for (size_t i = 0; i < frame.rows; i++) {
    for (size_t j = 0; j < frame.cols; j++) {

      if( bin.at<Vec3b>(i,j)==greenPX ) frame.at<Vec3b>(i,j)=greenPX;
    }
  }
}



void split_purple(Mat & roi){

  Mat red=bin_mask_red(roi);
  //prendi solo bianco
  Rect red_rect=boundingRect(red);
  Mat blue=bin_mask_blue(roi);
  //prendi solo bianco
  Rect blue_rect=boundingRect(blue);
  rectangle(frame,red_rect,redPX,2);
  rectangle(frame,blue_rect,bluePX,2);
}



/*

*/
Scalar eval_black_box(Mat &roi){
    Scalar color;
    Mat red=bin_mask_red(roi);
    float avg_red=white_average(red);

    Mat blue=bin_mask_blue(roi);
    float avg_blue=white_average(blue);
    float min_avg_2=0.30f;
    if ( avg_blue > min_avg_2 && avg_red < min_avg_2 ) color=bluePX;
    if ( avg_red > min_avg_2 && avg_blue < min_avg_2 ) color=redPX;
    if ( avg_red > min_avg_2 && avg_blue > min_avg_2 ) color=purplePX;

  return color
}




/*
*/
