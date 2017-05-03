#include "cv.h"
#include "highgui.h"
#include <iostream>
using namespace std;
using namespace cv;

int main( int argc, char** argv )
{
    IplImage *pFrame = NULL;

    int frameH,frameW,ResW,CropW;
    int dof;
    
    if( argc != 3 )
      {
	cout << "USAGE: "<< argv[0] <<" input_image DOF" << endl;
	return -1;
      }
    else
      pFrame = cvLoadImage(argv[1],1);

    if( !pFrame )
    {
      cout << "Can not initializing Image ..." << endl;
      return -1;
    }

    string file=argv[1];
    size_t findext=file.find_last_of(".");
    string filename=file.substr(0,findext);
    string filedotext=file.substr(findext);
    string filepFrameR=filename+"-R-"+argv[2]+filedotext;
    string filepFrameL=filename+"-L-"+argv[2]+filedotext;
    
    frameH = pFrame->height;
    frameW = pFrame->width;
    dof = atoi(argv[2]);
    ResW = frameW + (frameW / dof);
    CropW = (ResW - frameW) / 2;

    IplImage *frameL = NULL;
    IplImage *frameR = NULL;
    IplImage *pFrameL = NULL;
    IplImage *pFrameR = NULL;
	
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
    
    cvSaveImage(strdup(filepFrameR.c_str()),pFrameR);
    cvSaveImage(strdup(filepFrameL.c_str()),pFrameL);
	
    cvReleaseImage(&frameL);
    cvReleaseImage(&frameR);
    cvReleaseImage(&pFrameL);
    cvReleaseImage(&pFrameR);
    cvReleaseImage(&pFrame);

    return 0;
}
