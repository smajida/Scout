/*
* Autor : Arnaud GROSJEAN (VIDE SARL)
* This implementation of VideoSource allows to use OpenCV as a source for the video input
* I did so because libCVD failed getting my V4L2 device
*
* INSTALLATION :
* - Copy the VideoSource_Linux_OpenCV.cc file in your PTAM directory
* - In the Makefile:
*	- set the linkflags to
	LINKFLAGS = -L MY_CUSTOM_LINK_PATH -lblas -llapack -lGVars3 -lcvd -lcv -lcxcore -lhighgui
*	- set the videosource to 
	VIDEOSOURCE = VideoSource_Linux_OpenCV.o
* - Compile the project
* - Enjoy !
* 
* Notice this code define two constants for the image width and height (OPENCV_VIDEO_W and OPENCV_VIDEO_H)
*/

#include "VideoSource.h"
#include <cvd/colourspace_convert.h>
#include <cvd/colourspaces.h>
#include <gvars3/instances.h>
#include <opencv/highgui.h>
#include <opencv/cxcore.h>
#include <opencv/cv.h>

#include <iostream>
#include <pthread.h>

using namespace CVD;
using namespace std;
using namespace GVars3;
using namespace cv;


#define OPENCV_VIDEO_W 640
#define OPENCV_VIDEO_H 360
// Original W H is 640 * 480
// AR Drone has a speical resolution size

/*
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* drainFrames(void *arg){
     Mat temp;
     VideoCapture *cap = (VideoCapture *)arg;
     while (true) {  
        pthread_mutex_lock(&mutex);
        //cout << "I am draining thread." << endl;
        *cap >> temp;
        pthread_mutex_unlock(&mutex);
    }
}
*/

int id = 0;

VideoSource::VideoSource()
{
  cout << "  VideoSource_Linux: Opening video source..." << endl;
  
  mptr = new VideoCapture("tcp://192.168.1.1:5555/");
  //mptr = new VideoCapture("tcp://192.168.1.1:5555/?dummy=param.mp4");
  //mptr = new VideoCapture(1);
  
  VideoCapture* cap = (VideoCapture*)mptr;
	if(!cap->isOpened()){
	cerr << "Unable to get the camera" << endl;
	exit(-1);
  }
  cout << "  ... got video source." << endl;
  mirSize = ImageRef(OPENCV_VIDEO_W, OPENCV_VIDEO_H);

  /*
  cout << " Launch Draining Thread" << endl;
  pthread_t drainingThread;
  int isFailed = pthread_create(&drainingThread, NULL, drainFrames, (void *)mptr);
  if (isFailed)
      cout << "Can not create draining thread." << endl;
  */
};

ImageRef VideoSource::Size()
{ 
  return mirSize;
};

void conversionNB(Mat frame, Image<byte> &imBW){
	Mat clone = frame.clone();
	Mat_<Vec3b>& frame_p = (Mat_<Vec3b>&)clone;
	for (int i = 0; i < frame.rows; i++){
		for (int j = 0; j < frame.cols; j++){	
		imBW[i][j] = (frame_p(i,j)[0] + frame_p(i,j)[1] + frame_p(i,j)[2]) / 3;
		}
	}
}

void conversionRGB(Mat frame, Image<Rgb<byte> > &imRGB){
	Mat clone = frame.clone();
	Mat_<Vec3b>& frame_p = (Mat_<Vec3b>&)clone;
	for (int i = 0; i < frame.rows; i++){
		for (int j = 0; j < frame.cols; j++){	
		imRGB[i][j].red = frame_p(i,j)[2];
		imRGB[i][j].green = frame_p(i,j)[1];
		imRGB[i][j].blue = frame_p(i,j)[0];
		}
	}
}

void VideoSource::GetAndFillFrameBWandRGB(Image<byte> &imBW, Image<Rgb<byte> > &imRGB)
{
Mat frame;
VideoCapture* cap = (VideoCapture*)mptr;
//pthread_mutex_lock(&mutex);

*cap >> frame;
cout << "Grab a frame"<< id++ << endl;
//framie = ((Mat *)mptr2)->clone();
//pthread_mutex_unlock(&mutex);
  conversionNB(frame, imBW);
  conversionRGB(frame, imRGB);
}


