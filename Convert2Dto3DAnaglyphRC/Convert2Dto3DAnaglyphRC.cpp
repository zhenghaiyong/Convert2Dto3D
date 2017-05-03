#include "cv.h"
#include "highgui.h"
#include <iostream>
using namespace std;
using namespace cv;

int main( int argc, char** argv )
{
    CvCapture *pCapture = NULL;
    IplImage *pFrame = NULL;

    int frameH,frameW,ResW,CropW,numFrames;
    float fps;
    int dof;
    int nFrmNum = 0;
    
    if( argc != 4 )
      {
	cout << "USAGE: "<< argv[0] <<" input_video output_video DOF" << endl;
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
    dof = atoi(argv[3]);
    ResW = frameW + (frameW / dof);
    CropW = (ResW - frameW) / 2;

    cout << "numFrames:" << numFrames << endl;
    CvVideoWriter *writer = cvCreateVideoWriter(argv[2],CV_FOURCC('D','I','V','X'),fps,cvSize(frameW,frameH),1);
    //CvVideoWriter *writer = cvCreateVideoWriter(argv[2],CV_FOURCC('H','2','6','4'),fps,cvSize(frameW,frameH),1);
    while(pFrame = cvQueryFrame( pCapture ))
      {
	IplImage *frameL = NULL;
	IplImage *frameR = NULL;
	IplImage *pFrameL = NULL;
	IplImage *pFrameR = NULL;
	IplImage *pFrameLChannel[3] = {NULL,NULL,NULL};
	IplImage *pFrameRChannel[3] = {NULL,NULL,NULL};
	IplImage *pFrameRC = NULL;
	
	nFrmNum ++;
	//	cout << "nFrmNum:" << nFrmNum << endl;

	frameR = cvCreateImage(cvSize(ResW,frameH),8,3);
	cv::Mat frameRMat(frameR,0);
	//	cvResize(pFrame,frameR,CV_INTER_CUBIC);
	resize(pFrame,frameRMat,Size(ResW,frameH),0,0,INTER_LANCZOS4);
	*frameR = IplImage(frameRMat);
	cvSetImageROI(frameR,cvRect(0,0,frameW,frameH));
	pFrameR = cvCreateImage(cvGetSize(frameR),frameR->depth,frameR->nChannels);
	cvCopy(frameR,pFrameR,NULL);
	cvResetImageROI(frameR);

	frameL = cvCreateImage(cvSize(ResW,frameH),8,3);
	cv::Mat frameLMat(frameL,0);
	//	cvResize(pFrame,frameL,CV_INTER_CUBIC);
	resize(pFrame,frameLMat,Size(ResW,frameH),0,0,INTER_LANCZOS4);
	*frameL = IplImage(frameLMat);
	cvSetImageROI(frameL,cvRect(CropW,0,frameW,frameH));
	pFrameL = cvCreateImage(cvGetSize(frameL),frameL->depth,frameL->nChannels);
	cvCopy(frameL,pFrameL,NULL);
	cvResetImageROI(frameL);

	/*
	cout << "frameW:" << frameW << ", frameH:" << frameH << endl;
	cout << "ResW:" << ResW << ", CropW:" << CropW << endl;
	cout << "pFrame:" << pFrame->nChannels << "," << pFrame->depth << "," << pFrame->width << "," << pFrame->height << endl;
	cout << "frameR:" << frameR->nChannels << "," << frameR->depth << "," << frameR->width << "," << frameR->height << endl;
	cout << "frameL:" << frameL->nChannels << "," << frameL->depth << "," << frameL->width << "," << frameL->height << endl;
	cout << "pFrameR:" << pFrameR->nChannels << "," << pFrameR->depth << "," << pFrameR->width << "," << pFrameR->height << endl;
	cout << "pFrameL:" << pFrameL->nChannels << "," << pFrameL->depth << "," << pFrameL->width << "," << pFrameL->height << endl;
	*/

	CvSize pSize = cvGetSize(pFrameR);
	pFrameRChannel[0] = cvCreateImage(pSize,IPL_DEPTH_8U,1);
	pFrameRChannel[1] = cvCreateImage(pSize,IPL_DEPTH_8U,1);
	pFrameRChannel[2] = cvCreateImage(pSize,IPL_DEPTH_8U,1);
	cvSplit(pFrameR,pFrameRChannel[0],pFrameRChannel[1],pFrameRChannel[2],NULL); //BGR
	pSize = cvGetSize(pFrameL);
	pFrameLChannel[0] = cvCreateImage(pSize,IPL_DEPTH_8U,1);
	pFrameLChannel[1] = cvCreateImage(pSize,IPL_DEPTH_8U,1);
	pFrameLChannel[2] = cvCreateImage(pSize,IPL_DEPTH_8U,1);
	cvSplit(pFrameL,pFrameLChannel[0],pFrameLChannel[1],pFrameLChannel[2],NULL); //BGR
	pFrameRC = cvCreateImage(pSize,IPL_DEPTH_8U,3);
	cvMerge(pFrameRChannel[0],pFrameRChannel[1],pFrameLChannel[2],0,pFrameRC); //BGR
	
	cvWriteFrame(writer,pFrameRC);

	cvReleaseImage(&frameL);
	cvReleaseImage(&frameR);
	cvReleaseImage(&pFrameL);
	cvReleaseImage(&pFrameR);
	cvReleaseImage(&pFrameLChannel[0]);
	cvReleaseImage(&pFrameLChannel[1]);
	cvReleaseImage(&pFrameLChannel[2]);
	cvReleaseImage(&pFrameRChannel[0]);
	cvReleaseImage(&pFrameRChannel[1]);
	cvReleaseImage(&pFrameRChannel[2]);
	cvReleaseImage(&pFrameRC);
      }
    cvReleaseVideoWriter(&writer);
    cvReleaseCapture(&pCapture);

    cout << "nFrmNum:" << nFrmNum << endl;

    return 0;
}
