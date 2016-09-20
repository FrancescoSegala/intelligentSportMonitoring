/**
 * @file playerdetector.cpp
 * @author D.D. Bloisi
 * @brief load a cascade classifier and find soccer players in a video stream
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

#define MAX_HUE_JERSEY_HOME MAX_HUE_BLUE
#define MIN_HUE_JERSEY_HOME MIN_HUE_BLUE

#define MAX_HUE_JERSEY_AWAY MAX_HUE_RED
#define MIN_HUE_JERSEY_AWAY MIN_HUE_RED

/** Function Headers */
void do_job(Mat & frame);
void detectAndDisplay( Mat &frame);
void detect_and_display_players(Mat &frame);
cv::Mat bin_mask_green(Mat &frame);
cv::Mat bin_mask_red(Mat &frame);
cv::Mat bin_mask_blue(Mat &frame);
cv::Mat bin_mask_green_2(Mat &frame);
Mat find_n_draw_closed_areas(Mat &frame);
Vec3i calc_hist_hsv(Mat &roi);
int conta_white(Mat &roi);
float white_average(Mat &roi);
/** Global variables */

String player_cascade_name = "./classifier/haarcascade_player.xml";

String video_name = "./data/vardy.avi";

String saving_dir= "./test/";

CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;

string window_name = "Player detection";
RNG rng(12345);

int counter;
int frame_number;
bool save=false;
bool new_detector=true;

#define REC_VIDEO 0
/**
 * @function main
 */
int main( int argc, const char** argv ){

    /*
    parsing command line
    */
    if(argc > 1 && strcmp(argv[1],"--save")==0 ){
      save=true;
      std::cout << "saving 1 frame every 20 frame" << std::endl;
    }

    if ( argc > 1 && strcmp(argv[1],"--save-old")==0 ){
      save=true;
      new_detector=false;
      std::cout << "saving 1 frame every 20 frame of opencv detector" << std::endl;

    }


    //-- 1. Load the cascades

    if (!face_cascade.load(player_cascade_name)) { printf("--(!)Error loading player cascade\n"); return -1; };

    //-- 2. Read the video stream
    VideoCapture capture(video_name); // open the default camera
    if( !capture.isOpened() ) {
        cout << "Capture failed to open!" << endl;
        return EXIT_FAILURE;
    }

    Mat frame;
    capture >> frame; // get first frame for size

  #if REC_VIDEO
      // record video
      VideoWriter record("results.avi", CV_FOURCC('D','I','V','X'), 5, frame.size(), true);
      if( !record.isOpened() ) {
          cout << "VideoWriter failed to open!" << endl;
          return EXIT_FAILURE;
      }
  #endif

      counter = 0;
      frame_number = 0;

      while( true ) {
          // get a new frame from camera
          capture >> frame;

          string n;
          ostringstream oss;  //temp as in temporary
          oss << frame_number;
          n = oss.str();      //str is temp as string

          cout << "frame: " << n << endl;

          //-- 3. Apply the classifier to the frame
          if( !frame.empty() ) {

              do_job(frame);

              //----------------saving-------------------
              if ( save ){

                if ( new_detector){

                  String name= "0"+n+"_color_processing"+".jpg";
                  if(frame_number % 20 == 0){
                    //save samples every 20 frames
                    std::cout << "frame saved" << std::endl;
                    imwrite(saving_dir+name,frame);
                  }
                }

                else {

                  String name= "0"+n+"_opencv"+".jpg";
                  if(frame_number % 20 == 0){
                    //save samples every 20 frames
                    std::cout << "frame saved" << std::endl;
                    imwrite(saving_dir+name,frame);
                  }

                }
              }
              //------------------------------------------


          }
          else {
              printf(" --(!) No captured frame -- Break!");
              break;
          }

  #if REC_VIDEO
          // add frame to recorded video
          record << frame;
  #endif

          int c = waitKey(30);
          if( (char)c == 'q' || (char)c == 27) {
              break;
          }

          frame_number++;

      }

      return EXIT_SUCCESS;
}



/*
 * @function detectAndDisplay
 */
void detectAndDisplay( Mat &frame)
{
    std::vector<Rect> players;
    Mat frame_gray;

    cvtColor( frame, frame_gray, CV_BGR2GRAY );

    face_cascade.detectMultiScale(frame_gray, players, 1.1, 3, 0, Size(32, 64));


    for( int i = 0; i < players.size(); i++, counter++ )
    {
        //ellipse( frame, center, Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar( 255, 0, 255 ), 2, 8, 0 );

        rectangle(frame, players[i], Scalar(255, 255, 255), 2);
    }

    //-- Show what you got
    imshow( window_name, frame );

}

/*
@params: input frame to compute people detection
return: void
*/
/*
this is core function, it will draw red box for liverpool, blue box for leicester and purple box for both
improve the original OpenCV's players detector
*/
void detect_and_display_players( Mat &frame){

    std::vector<Rect> players;
    Mat frame_gray, bin_mask;

    Scalar blackPX=Scalar(0,0,0);
    Scalar whitePX=Scalar(255,255,255);
    Scalar redPX=Scalar(0,0,255);
    Scalar greenPX=Scalar(0,255,0);
    Scalar bluePX=Scalar(255,0,0);
    Scalar purplePX=Scalar(255,0,255);

    cvtColor( frame, frame_gray, CV_BGR2GRAY );

    bin_mask=bin_mask_green_2(frame);

    face_cascade.detectMultiScale(frame_gray, players, 1.1, 3, 0, Size(32, 64));

    int tresh_player=30;//250
    float min_avg=0.55f;
    Mat roi, roi_bin, roi_closed_areas;
    float avg_white;

    for( int i = 0; i < players.size(); i++, counter++ ){

        Scalar color=whitePX;
        Vec3i hist_b_g_r;

        roi=frame(players[i]);
        roi_bin=bin_mask(players[i]);

        roi_closed_areas=find_n_draw_closed_areas(roi);

        avg_white=white_average(roi_bin);

        hist_b_g_r=calc_hist_hsv(roi_closed_areas);
         //cc
        if( hist_b_g_r[0] > tresh_player && hist_b_g_r[2] < tresh_player && avg_white > min_avg ) color=bluePX;
        //cc
        if( hist_b_g_r[2] > tresh_player && hist_b_g_r[0] < tresh_player && avg_white > min_avg ) color=redPX;
        //cc
        if( hist_b_g_r[0] > tresh_player && hist_b_g_r[2] > tresh_player && avg_white > min_avg ) color=purplePX;
        //cc
        if( (hist_b_g_r[0] < tresh_player && hist_b_g_r[2] < tresh_player) || avg_white < min_avg ) color=blackPX;


        rectangle(frame, players[i], color, 2);

    }

    //-- Show what you got
    imshow( "new player detection", frame );

}

/*
-----aux function for statistical analisys of ROI's color-----------------------
*/
int conta_white(Mat &roi){

  Vec3b whitePX={255,255,255};
  int res=0;
  for (size_t i = 0; i < roi.rows; i++) {
    for (size_t j = 0; j < roi.cols; j++) {

      if ( roi.at<Vec3b>(i,j)==whitePX ) res++;

    }
  }

  return res;
}

float white_average(Mat &roi){

  float num=conta_white(roi);
  float den=roi.cols*roi.rows;
  return num/den;
}
/*
--------------------------------------------------------------------------------
*/


/*
@params Mat: roi to calculate number of red green and blue pixel
return : vector of 3 int containing those values
*/
Vec3i calc_hist_hsv(Mat &roi){

  Mat hsv_image;
  int b=0,g=0,r=0,playertrash=250;
  cvtColor(roi,hsv_image,CV_BGR2HSV);
  Vec3i res;

  for (size_t i = 0; i < roi.rows; i++) {
    for (size_t j = 0; j < roi.cols; j++) {

      Vec3b pixel=hsv_image.at<cv::Vec3b>(i,j);
      uchar hue_part=pixel.val[0];
      uchar saturation_part=pixel.val[1];
      uchar value_part=pixel.val[2];

      if((hue_part<MAX_HUE_BLUE && hue_part>MIN_HUE_BLUE) && saturation_part>50 ) b++;
      if((hue_part>MAX_HUE_RED || hue_part<MIN_HUE_RED) && saturation_part>50) r++;
      if((hue_part<MAX_HUE_GREEN && hue_part>MIN_HUE_GREEN) && saturation_part>50) g++;

    }
  }

  res[0]=b;
  res[1]=g;
  res[2]=r;
  return res;
}


/*
@params Mat: original frame to calculate the binary mask
return : binary mask of original frame whith white pixels
*/
cv::Mat bin_mask_green_2(Mat &frame){

  Mat hsv_image, bin_mask;
  cvtColor(frame,hsv_image,COLOR_BGR2HSV);

  //GaussianBlur( hsv_image, hsv_image, Size( 5, 5 ), 0, 0 );

  inRange(hsv_image,Scalar(MIN_HUE_GREEN,100,100),Scalar(MAX_HUE_GREEN,255,255),bin_mask);

  return bin_mask;
}


/*
@params : frame to calculate binary mask
return : binary mask with 1 for red 0 anyway
*/
cv::Mat bin_mask_red(Mat &frame){

  Mat hsv_image, bin_mask;
  cvtColor(frame,hsv_image,COLOR_BGR2HSV);

  //GaussianBlur( hsv_image, hsv_image, Size( 5, 5 ), 0, 0 );

  inRange(hsv_image,Scalar(MIN_HUE_RED,100,100),Scalar(MAX_HUE_RED,255,255),bin_mask);

  return bin_mask;
}


/*
@params : frame to calculate binary mask
return : binary mask with 1 for blue 0
*/
cv::Mat bin_mask_blue(Mat &frame){

  Mat hsv_image, bin_mask;
  cvtColor(frame,hsv_image,COLOR_BGR2HSV);

  //GaussianBlur( hsv_image, hsv_image, Size( 5, 5 ), 0, 0 );

  inRange(hsv_image,Scalar(MIN_HUE_BLUE,100,100),Scalar(MAX_HUE_BLUE,255,255),bin_mask);

  return bin_mask;
}


/*
@params Mat : frame to calculate the binary mask
return : binary mask of the frame with green pixel
*/
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



/*
@params : frame to calculate player contourns
return: return a frame with black pixels except for the areas (those greater than 20) and its content
*/
Mat find_n_draw_closed_areas(Mat &frame){

  Mat gray,mask,res;
  if( frame.channels() > 1 )  cvtColor(frame,gray,CV_BGR2GRAY);
  else gray=frame.clone();
  mask=Mat(frame.rows,frame.cols,CV_8UC3,Scalar(0,0,0));
  res=Mat(frame.rows,frame.cols,CV_8UC3,Scalar(0,0,0));

  //detect edge
  Canny( gray, gray, 50, 150, 3);
  //find contours
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;
  Moments moms;
  double area;
  //double centroidex;
  //double centroidey;

  int cy, cx;
  Vec3b whitePX={255,255,255};
  findContours( gray, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
  for (size_t i = 0; i < contours.size(); i++) {

    moms=moments(Mat(contours[i]));
    area=moms.m00;

    //centroidex =moms.m01;
    //centroidey =moms.m10;
    //cx=centroidex/area;
    //cy=centroidey/area;
    if ( area > 20 ){
      approxPolyDP(contours[i] , contours[i] ,1 , true );
      drawContours( mask, contours, i, Scalar(255,255,255), CV_FILLED, 8, hierarchy, 0, Point() );
      //circle(frame,Point(cx,cy),7,Scalar(0,0,0),8);
    }


  }
  for (size_t i = 0; i < frame.rows; i++) {
    for (size_t j = 0; j < frame.cols; j++) {

      if( mask.at<Vec3b>(i,j) == whitePX ) res.at<Vec3b>(i,j)=frame.at<Vec3b>(i,j);

    }
  }
  //imshow("mask", mask);
  //imshow("res",res);
  return res;
}


/*
metodo ausiliare per mantere chiuso il main LAZINESS OVER NINE THOUSAND!!
*/
void do_job(Mat &frame){


  if(new_detector) detect_and_display_players(frame);
  else detectAndDisplay(frame);
  /*
  frame=find_n_draw_closed_areas(frame);
  imshow("fra",frame);
  waitKey(0);
  */
}
