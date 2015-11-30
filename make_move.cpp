#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std; 
using namespace cv; 

Mat makeMove(Mat, VideoCapture); 
int motionDetected(VideoCapture); 
int compareImage(Mat, Mat); 

int main(int argc, char **argv) {
	Mat capture; 
	VideoCapture c(1); 
	c.read(capture); 
	//waitKey(1000); 
	Mat newFrame = makeMove(capture, c); 
	
	imshow("Capture", capture); 
	imshow("Dif", newFrame); 
	waitKey(0); 
}

Mat makeMove(Mat original, VideoCapture cam) {
	Mat newImage, tmp;
	original.copyTo(newImage); 
	cvtColor(original, original, CV_BGR2GRAY); 
	cvtColor(newImage, newImage, CV_BGR2GRAY); 
	while(compareImage(newImage, original)) {
		cerr<<"In loop"<<endl; 
		motionDetected(cam); 
		cam.read(newImage); 
		cvtColor(newImage, newImage, CV_BGR2GRAY); 
	}
	
	return newImage; 
}

int compareImage(Mat a, Mat b) {
	Mat tmp; 
	
	//cvtColor(a,a,CV_BGR2GRAY);  
	//cvtColor(b,b,CV_BGR2GRAY);  
	absdiff(a,b,tmp);
	threshold(tmp, tmp, 100,255, THRESH_BINARY);
	medianBlur(tmp,tmp,9); 
	return !(countNonZero(tmp)>0); 
}


int motionDetected(VideoCapture cam) {
	Mat prevFrame, currFrame, tmp; 
	cam.read(currFrame);
	Mat x=Mat::zeros(currFrame.rows, currFrame.cols, CV_8UC1);
	cvtColor(currFrame, currFrame,CV_BGR2GRAY);  
	do {
		waitKey(50); 
		currFrame.copyTo(prevFrame); 
		cam.read(currFrame);
		cvtColor(currFrame, currFrame,CV_BGR2GRAY);  
		absdiff(currFrame,prevFrame,tmp); 
		threshold(tmp, tmp, 25,255, THRESH_BINARY);
		medianBlur(tmp,tmp,7); 
		//imshow("tmp", tmp); 
		
	} while(countNonZero(tmp)>0);
	return 0; 
}
