#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <algorithm>


using namespace std;
using namespace cv;

#define MAX_HUE_RED 170
#define MIN_HUE_RED 12

// TODO define the range for the H channel of  the green, search on internet information about the HSV color space
#define MAX_HUE_GREEN 47
#define MIN_HUE_GREEN 40

// TODO define the range for the H channel of  the blue, search on internet information about the HSV color space
#define MAX_HUE_BLUE 115
#define MIN_HUE_BLUE 100

void find_Contours(Mat &frame);

void calc_hist_hsv(Mat &roi){

  Mat hsv_image;
  int b=0,g=0,r=0,playertrash=250;
  cvtColor(roi,hsv_image,CV_BGR2HSV);
  Vec3b blackPX={0,0,0};
  Vec3b whitePX={255,255,255};
  Vec3b bluePX={255,0,0};
  Vec3b redPX={0,0,255};
  Vec3b greenPX={0,255,0};
  Mat res=Mat(roi.rows,roi.cols,CV_8UC3,blackPX);

  for (size_t i = 0; i < roi.rows; i++) {
    for (size_t j = 0; j < roi.cols; j++) {

      Vec3b pixel=hsv_image.at<cv::Vec3b>(i,j);
      uchar hue_part=pixel[0];
      uchar saturation_part=pixel[1];
      uchar value_part=pixel[2];

      if((hue_part<MAX_HUE_BLUE && hue_part>MIN_HUE_BLUE) && saturation_part>50 ){
        //hey man you are a blue pixel! set white
        b++;
        res.at<cv::Vec3b>(i,j)=bluePX;

      }
      if((hue_part>MAX_HUE_RED || hue_part<MIN_HUE_RED) && saturation_part>50){
        //hey man you are a green pixel! set white
        r++;
        res.at<cv::Vec3b>(i,j)=redPX;
      }
      if((hue_part<MAX_HUE_GREEN && hue_part>MIN_HUE_GREEN) && saturation_part>50){
        //hey man you are a red pixel! set white
        g++;
        //res.at<cv::Vec3b>(i,j)=greenPX;
      }

    }
  }

  find_Contours(res);

  printf("r: %d    g: %d   b: %d\n",r , g , b );
  imshow("res",res);
  imshow("roi",roi);
  if( r > playertrash ) std::cout << "c'è un giocatore rosso" << std::endl;
  if ( b > playertrash ) std::cout << "c'è un giocatore blu " << std::endl;
  waitKey(0);

}


void find_Contours(Mat &frame){


  Mat cannygrey,grey;
  vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    cvtColor(frame,grey,CV_BGR2GRAY);

    Canny(grey,cannygrey,50,150,3);

    findContours( cannygrey, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE, Point(0, 0) );
    Moments moms;
    double area;
    double centroidex;
    double centroidey;
    int cy, cx;


        for( int i=0; i>contours.size(); i++ )
        {
          moms=moments(Mat(contours[i]));
          area=moms.m00;
          centroidex =moms.m01;
          centroidey =moms.m10;
          cx=centroidex/area;
          cy=centroidey/area;

          drawContours( frame, contours, i, Scalar(0,0,255), 2, 8, hierarchy, 0, Point() );
          circle(frame,Point(cx,cy),7,Scalar(0,0,0),8);


        }
}


cv::Mat apply_contours(vector<Vec4i> lines,Mat& frame){

  for (size_t i = 0; i < lines.size(); i++) {
    Scalar color=Scalar(0,255,0);

    Point a=Point(lines[i][0],lines[i][1]);
    Point b=Point(lines[i][2],lines[i][3]);

    line( frame, a , b , color, 3, 8 );

  }
  return frame;
}



Mat manipulate_bin(Mat &frame){

  Vec3b blackPX={0,0,0};
  Vec3b whitePX={255,255,255};

  Mat res=Mat(frame.rows,frame.cols,CV_8UC3,Scalar(0,0,0));

  for (size_t x = 0; x < frame.cols; x++) {

    bool found=false;

    for (size_t y = 0; y < frame.rows; y++) {

      Vec3b pixel=frame.at<Vec3b>(y,x);
      if( pixel[0]==255 || found){
        if(!found){
          found=true;
        }
        res.at<Vec3b>(y,x)=whitePX;

      }
      else {
        res.at<Vec3b>(y,x)=blackPX;
      }

    }
  }

  return res;
}




std::vector<Point> scomponi(vector<Vec4i> lines){
  std::cout << "scomponi"<< lines.size()<< std::endl;
  std::vector<Point> res;
  for (size_t i = 0; i < lines.size(); i++) {

    Point a=Point(lines[i][0],lines[i][1]);
    Point b=Point(lines[i][2],lines[i][3]);
    std::cout << "a "<< a.x <<" " <<a.y  << std::endl;
    std::cout << "b "<< b.x <<" " <<b.y  << std::endl;
    res.push_back(a);
    res.push_back(b);
  }
  return res;
}


bool compara(Point a, Point b){
  return (a.x < b.x);
}


std::vector<Point> filtra(std::vector<Point> v){
  std::cout << "filtra" << std::endl;
  std::vector<Point> filtered;
  std::sort(v.begin(),v.end(),compara);
  std::cout << "filra sort "<<v.size() << std::endl;
  for (size_t i = 0; i < v.size()-1; i++) {
    Point a=v[i];
    Point b=v[i+1];
    float dx=b.x-a.x;
    float dy= b.y - a.y;
    if ( dx!=0 && dy/dx <= 0.5 ){

      filtered.push_back(v[i]);
    }
  }
  std::cout << "filtra end" << std::endl;
  return filtered;
}


cv::Mat applica(std::vector<Point> v,Mat &frame){
  std::cout << "applica" << v.size()<< std::endl;
  for (size_t i = 0; i < v.size()-1; i++) {
    Scalar color1=Scalar(0,0,255);
    Scalar color2=Scalar(255,255,0);

    Point a=Point(v[i].x,v[i].y);
    Point b=Point(v[i+1].x,v[i+1].y);

    line( frame, a , a , color1, 3, 8 );
    line( frame, b , b , color2, 3, 8 );

  }
  return frame;

}


cv::Mat bin_mask_green_2(Mat &frame){

  Mat hsv_image, bin_mask;
  cvtColor(frame,hsv_image,COLOR_BGR2HSV);

  //GaussianBlur( hsv_image, hsv_image, Size( 5, 5 ), 0, 0 );

  inRange(hsv_image,Scalar(MIN_HUE_GREEN,100,100),Scalar(MAX_HUE_GREEN,255,255),bin_mask);

  return bin_mask;
}


std::vector<Vec4i>  find_contours_canny(Mat &frame){

  Mat gray, drawing;
  if( frame.channels() > 1 )  cvtColor(frame,gray,CV_BGR2GRAY);
  else gray=frame.clone();

  Canny( gray, gray, 50, 150, 3);

  gray.convertTo(drawing, CV_8U);
  vector<Vec4i> lines;
  HoughLinesP(drawing, lines, 1, CV_PI/180, 80, 40, 10 );

  return lines;
}



cv::Mat bin_mask_green(Mat &frame){
  Mat hsv_image,bin_mask;

  Vec3b blackPX={0,0,0};
  Vec3b whitePX={255,255,255};

  cvtColor(frame, hsv_image, COLOR_BGR2HSV);

  bin_mask=frame.clone();

  for (size_t i = 0; i < bin_mask.rows; i++) {
    for (size_t j = 0; j < bin_mask.cols; j++) {

      Vec3b pixel=hsv_image.at<cv::Vec3b>(i,j);
      uchar hue_part=pixel[0];
      uchar saturation_part=pixel[1];
      uchar value_part=pixel[2];

      if((hue_part<MAX_HUE_GREEN && hue_part>MIN_HUE_GREEN) && saturation_part>50 ){
        //hey man you are a green pixel set green
        bin_mask.at<cv::Vec3b>(i,j)=Vec3b(0, 255 ,0);
      }
      else {
        bin_mask.at<cv::Vec3b>(i,j)=blackPX;
      }
    }
  }
  //imshow("bin mask",bin_mask);
  return bin_mask;
}

vector<Vec4i> double_filter_contourn(Mat &frame ){

  std::vector<Vec4i> res;
    /**/

}



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



int main(int argc, char const *argv[]) {


  Mat src;

  /// Load image
  src = imread( argv[1], 1 );

  if( !src.data )
    { return -1; }

    //calc_hist_hsv(src);
    /*
    doppia passata :
    mask green
    lines = find cont ( binmask)
    bin_mask = apply contours verde!
    lines 2 = find contours ( binmask  )
    final res apply contourns (lines2 originalochittepare)
    */
    /*Mat res=Mat(src.rows,src.cols,CV_8UC3,Scalar(0,0,0));
    Mat binmask=bin_mask_green(src);
    std::vector<Vec4i> lines=find_contours_canny(binmask);
    //std::vector<Point> v=scomponi(lines);
    binmask=apply_contours(lines,binmask);
    imshow("bin",binmask);

    std::vector<Vec4i> lines2=find_contours_canny(binmask);
    res=apply_contours(lines2,src);
    //std::vector<Point> f=filtra(v);
    //applica(v,res);*/
    //project_bin(src);
    find_Contours(src);
    imshow("frame",src);
    //res=manipulate_bin(binmask);
    //imshow("res",res);
    waitKey(0);
  return 0;
}
