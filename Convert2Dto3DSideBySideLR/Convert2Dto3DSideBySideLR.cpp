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
	cout << "USAGE: " << argv[0] <<" input_video output_video DOF" << endl;
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
    CvVideoWriter *writer = cvCreateVideoWriter(argv[2],CV_FOURCC('D','I','V','X'),fps,cvSize(2*frameW,frameH),1);
    //CvVideoWriter *writer = cvCreateVideoWriter(argv[2],CV_FOURCC('H','2','6','4'),fps,cvSize(frameW,frameH),1);
    while(pFrame = cvQueryFrame( pCapture ))
      {
	IplImage *frameL = NULL;
	IplImage *frameR = NULL;
	IplImage *pFrameL = NULL;
	IplImage *pFrameR = NULL;
	IplImage *pFrameLR = NULL;
	
	nFrmNum ++;
	//	cout << "nFrmNum:" << nFrmNum << endl;

	// frameR <=> f1 => right frame
	frameR = cvCreateImage(cvSize(ResW,frameH),8,3);
	cv::Mat frameRMat(frameR,0);
	//	cvResize(pFrame,frameR,CV_INTER_CUBIC);
	resize(pFrame,frameRMat,Size(ResW,frameH),0,0,INTER_LANCZOS4);
	*frameR = IplImage(frameRMat);
	cvSetImageROI(frameR,cvRect(0,0,frameW,frameH));
	pFrameR = cvCreateImage(cvGetSize(frameR),frameR->depth,frameR->nChannels);
	cvCopy(frameR,pFrameR,NULL);
	cvResetImageROI(frameR);

	// frameL <=> f2 => left frame
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
	
	pFrameLR = cvCreateImage(cvSize(2*frameW,frameH),8,3);
	CvRect rect = cvRect(0,0,frameW,frameH);
	cvSetImageROI(pFrameLR,rect);
	cvCopy(pFrameL,pFrameLR);
	cvResetImageROI(pFrameLR);
	rect = cvRect(frameW,0,frameW,frameH);
	cvSetImageROI(pFrameLR,rect);
	cvCopy(pFrameR,pFrameLR);
	cvResetImageROI(pFrameLR);
	cvWriteFrame(writer,pFrameLR);

	cvReleaseImage(&pFrameLR);
	cvReleaseImage(&frameL);
	cvReleaseImage(&frameR);
	cvReleaseImage(&pFrameL);
	cvReleaseImage(&pFrameR);
      }
    cvReleaseVideoWriter(&writer);
    cvReleaseCapture(&pCapture);

    cout << "nFrmNum:" << nFrmNum << endl;

    return 0;
}
