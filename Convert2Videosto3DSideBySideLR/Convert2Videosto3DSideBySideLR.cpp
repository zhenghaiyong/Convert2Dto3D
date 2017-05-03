#include "cv.h"
#include "highgui.h"
#include <iostream>
using namespace std;
using namespace cv;

int main( int argc, char** argv )
{
    CvCapture *pCaptureLeft = NULL;
    CvCapture *pCaptureRight = NULL;
    IplImage *pFrameLeft = NULL;
    IplImage *pFrameRight = NULL;

    int frameH,frameW,numFramesLeft,numFramesRight;
    float fps;
    int nFrmNum = 0;

    if( argc != 4 )
      {
	cout << "USAGE: "<< argv[0] <<" left_video right_video output_video" << endl;
	return -1;
      }
    else
      {
	pCaptureLeft = cvCaptureFromAVI( argv[1] );
	pCaptureRight = cvCaptureFromAVI( argv[2] );
      }

    if( !pCaptureLeft )
    {
      cout << "Could not initialize capturing left video ..." << endl;
      return -1;
    }
    if( !pCaptureRight )
    {
      cout << "Could not initialize capturing right video ..." << endl;
      return -1;
    }

    fps = (float) cvGetCaptureProperty(pCaptureLeft, CV_CAP_PROP_FPS);
    numFramesLeft = (int) cvGetCaptureProperty(pCaptureLeft, CV_CAP_PROP_FRAME_COUNT);
    numFramesRight = (int) cvGetCaptureProperty(pCaptureRight, CV_CAP_PROP_FRAME_COUNT);
    frameH = (int) cvGetCaptureProperty(pCaptureLeft, CV_CAP_PROP_FRAME_HEIGHT);
    frameW = (int) cvGetCaptureProperty(pCaptureLeft, CV_CAP_PROP_FRAME_WIDTH);

    cout << "numFramesLeft:" << numFramesLeft << endl;
    cout << "numFramesRight:" << numFramesRight << endl;

    CvVideoWriter *writer = cvCreateVideoWriter(argv[3],CV_FOURCC('D','I','V','X'),fps,cvSize(2*frameW,frameH),1);

    while(nFrmNum < numFramesLeft && nFrmNum < numFramesRight)
      {
	pFrameLeft = cvQueryFrame( pCaptureLeft );
	if (!pFrameLeft) break;
	pFrameRight = cvQueryFrame( pCaptureRight );
	if (!pFrameRight) break;

	IplImage *pFrameLR = NULL;
	
	pFrameLR = cvCreateImage(cvSize(2*frameW,frameH),8,3);
	CvRect rect = cvRect(0,0,frameW,frameH);
	cvSetImageROI(pFrameLR,rect);
	cvCopy(pFrameLeft,pFrameLR);
	cvResetImageROI(pFrameLR);
	rect = cvRect(frameW,0,frameW,frameH);
	cvSetImageROI(pFrameLR,rect);
	cvCopy(pFrameRight,pFrameLR);
	cvResetImageROI(pFrameLR);
	cvWriteFrame(writer,pFrameLR);

	cvReleaseImage(&pFrameLR);
      }
    cvReleaseVideoWriter(&writer);
    cvReleaseCapture(&pCaptureLeft);
    cvReleaseCapture(&pCaptureRight);

    return 0;
}
