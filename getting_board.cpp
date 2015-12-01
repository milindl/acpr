#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
using namespace std;
using namespace cv;

Mat src,hsv, holded, holded1; 
Mat final1, final2; 
int flag=0, flag2=0; 

int main() {
	VideoCapture cam(1); 
	char key; 
	int maxArea=0, minArea=0; 
	namedWindow("Value and Saturation"); 
	namedWindow("Value and Saturation1"); 
	int vL=0, vH=0, sL=0, sH=0; 
	createTrackbar("sH", "Value and Saturation", &sH, 255); 
	createTrackbar("sL", "Value and Saturation", &sL, 255); 
	createTrackbar("vH", "Value and Saturation", &vH, 255); 
	createTrackbar("vL", "Value and Saturation", &vL, 255); 
	int vL1=0, vH1=0, sL1=0, sH1=0; 
	createTrackbar("sH1", "Value and Saturation1", &sH1, 255); 
	createTrackbar("sL1", "Value and Saturation1", &sL1, 255); 
	createTrackbar("vH1", "Value and Saturation1", &vH1, 255); 
	createTrackbar("vL1", "Value and Saturation1", &vL1, 255); 
	createTrackbar("minArea", "Value and Saturation", &minArea, 10000); 
	createTrackbar("maxArea", "Value and Saturation", &maxArea, 10000); 
	while(key!=27) {
		cam.read(src);
		cvtColor(src,hsv, CV_BGR2HSV); 
		inRange(hsv, Scalar(0, sL, vL), Scalar(179, sH,vH),holded); 
		inRange(hsv, Scalar(0, sL1, vL1), Scalar(179, sH1,vH1),holded1); 
		//Canny(		
		medianBlur(holded, holded, 15); 
		medianBlur(holded1, holded1, 15); 		
		vector<vector<Point> > contours; 		
		vector<Vec4i> hierarchy;	
		vector<vector<Point> > contours1; 		
		vector<Vec4i> hierarchy1;
		findContours(holded, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );	
		vector<Moments> mu(contours.size() );
		findContours(holded1, contours1, hierarchy1, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );	
		vector<Moments> mu1(contours1.size() );
		for( int i = 0; i < contours.size(); i++ ) {
			mu[i] = moments( contours[i], false );
		}
		for( int i = 0; i < contours1.size(); i++ ) {
			mu1[i] = moments( contours1[i], false );
		}
				
		vector<Point2f> mc( contours.size() );
		vector<Point2f> mc1( contours1.size() );
		for( int i = 0; i < contours.size(); i++ ) {
			mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 ); 
		}
		for( int i = 0; i < contours1.size(); i++ ) {
			mc1[i] = Point2f( mu1[i].m10/mu1[i].m00 , mu1[i].m01/mu1[i].m00 ); 
		}
		Mat drawing = Mat::zeros( holded.size(), CV_8UC3 );
		vector<Rect> boundRect( contours.size() );
		int k=0; 
		for( int i = 0; i< contours.size(); i++ )
     		{

			if(mu[i].m00<minArea || mu[i].m00>maxArea) continue; 
			if(flag==1) continue; 
			//Rect r = boundingRect(contours[i]); 
			RotatedRect r1 = minAreaRect( Mat(contours[i])); 	
			float modAngle = r1.angle>0?r1.angle:-r1.angle; 
			float ht = r1.size.height; 
			float wt = r1.size.width; 
			float ratio = ht/wt;
			if(modAngle<75 && modAngle>15) continue; 	
			if(ratio<0.8 || ratio >1.2) continue;
			Point2f r[4]; r1.points(r);
			for( int j = 0; j < 4; j++ ){ 
          			line( drawing, r[j], r[(j+1)%4], Scalar(255,255,0), 2, 8 ); 
			
     			}
			string s;
			stringstream out;
			out << k;
			s = out.str();
			putText(drawing, s, mc[i], 1,1, Scalar(255,255,255)); 
			k++;  			
			if(k==32) {
				flag=1; 
				drawing.copyTo(final1); 
				}
			//rectangle(drawing, r, Scalar(0,255,255)); 
	//		drawContours( drawing, contours, i, Scalar(0,0,255), 2, 8, hierarchy, 0, Point());       			
			//circle( drawing, mc[i], 4, Scalar(0,255,255), -1, 8, 0 );
     		}	
		k=0;	
		for( int i = 0; i< contours1.size(); i++ )
     		{

			if(mu1[i].m00<minArea || mu1[i].m00>maxArea) continue; 
			if(flag2==1) continue; 
			//Rect r = boundingRect(contours[i]); 
			RotatedRect r1 = minAreaRect( Mat(contours1[i])); 
			float modAngle = r1.angle>0?r1.angle:-r1.angle; 
			float ht = r1.size.height; 
			float wt = r1.size.width; 
			float ratio = ht/wt;
			if(modAngle<75 && modAngle>15) continue; 
			if(ratio<0.8 || ratio>1.2) continue;	
			Point2f r[4]; r1.points(r);
			for( int j = 0; j < 4; j++ ){ 
          			line( drawing, r[j], r[(j+1)%4], Scalar(0,0,255), 2, 8 );
     			} 
			string s;
			stringstream out;
			out << k;
			s = out.str();
			putText(drawing, s, mc1[i], 1,1, Scalar(255,0,255)); 
			k++;  		
			if(k==32) {
			flag2=1;
			drawing.copyTo(final2);
			}	
			//rectangle(drawing, r, Scalar(0,255,255)); 
	//		drawContours( drawing, contours, i, Scalar(0,0,255), 2, 8, hierarchy, 0, Point());       			
			//circle( drawing, mc[i], 4, Scalar(0,255,255), -1, 8, 0 );
     		}
		
		imshow("COM", src+drawing); 
		//imshow("Thresh", holded); 
		key=waitKey(30); 
	}
	imshow("final", src+final2 + final1); 
	waitKey(0); 
}
