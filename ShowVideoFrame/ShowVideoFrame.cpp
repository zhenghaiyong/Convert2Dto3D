#include "cv.h"
#include "highgui.h"
#include <iostream>
using namespace std;
using namespace cv;

int main( int argc, char** argv )
{
    CvCapture *pCapture = NULL;
    IplImage *pFrame = NULL;

    int frameH,frameW,numFrames;
    float fps;
    int nFrmNum = 0;
    
    if( argc != 2 )
      {
	cout << "USAGE: "<< argv[0] <<" video" << endl;
	return -1;
      }
    else
      pCapture = cvCaptureFromAVI( argv[1] );

    if( !pCapture )
    {
      cout << "Could not initialize capturing..." << endl;
      return -1;
    }
    
    fps = (float) cvGetCaptureProperty(pCapture, CV_CAP_PROP_FPS);
    numFrames = (int) cvGetCaptureProperty(pCapture, CV_CAP_PROP_FRAME_COUNT);
    frameH = (int) cvGetCaptureProperty(pCapture, CV_CAP_PROP_FRAME_HEIGHT);
    frameW = (int) cvGetCaptureProperty(pCapture, CV_CAP_PROP_FRAME_WIDTH);

    cout << "numFrames:" << numFrames << endl;
    cvNamedWindow("VideoFrame",1);
    while(pFrame = cvQueryFrame( pCapture ))
      {
	nFrmNum ++;
	//	cout << "nFrmNum:" << nFrmNum << endl;
	
	cvShowImage("VideoFrame",pFrame);
	cvWaitKey(0);
      }
    cvDestroyWindow("VideoFrame");
    cvReleaseCapture(&pCapture);

    cout << "nFrmNum:" << nFrmNum << endl;

    return 0;
}
