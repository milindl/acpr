#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include "sort_board_as_per_location.h"
using namespace std;
using namespace cv;


/*global variables to be used thru the function */ 

int min_area=0, max_area=0; 
int min_angle = 15, max_angle=75; 
float min_ratio = 0.8, max_ratio=1.2;
//To be used for proc
Mat src,hsv, holded, holded1; 

//To be used for printing
Mat final1, final2; 
int flag=0, flag2=0;
 
RotatedRect *label(VideoCapture); 

int main() { 
	VideoCapture cam(1);
	Mat src1; 
	cam.read(src1); 
	Mat draw1 = Mat::zeros(src1.size(), CV_8UC3); 
	RotatedRect *r = label(cam); 
	for(int i= 0; i!=64; i++) {
		string s;
		stringstream out;
		out << i;
		s = out.str();
		putText(draw1, s, r[i].center, 1,1, Scalar(0,0,255)); 
		cerr<<"Rectangle at number "<< i<<" has width = "<<r[i].size.width<<endl; 
	}
	imshow("..", src1+draw1); 
	waitKey(0);
}

/* This function checks if a contour is a valid chess block */
bool checkContour(vector<Point> cont) {
	Moments m = moments(cont, false); 
	//Area: 
	if(m.m00>max_area || m.m00<min_area) return false; 
	//Angle
	RotatedRect r = minAreaRect( Mat(cont));
	float modAngle = r.angle>0?r.angle:-r.angle; 	
	if(modAngle>min_angle && modAngle<max_angle) return false; 
	//Sides Ratio
	float ht = r.size.height; 
	float wt = r.size.width; 
	float ratio = ht/wt;
	if(ratio<min_ratio || ratio>max_ratio) return false; 
	//It is alright
	return true;
}
/* This sorts the array of rotated rects */ 
/* new task: to totally remove white and base it on black(completely) */
RotatedRect *sorting(RotatedRect *original, int N) {
	int k = N/2; 
	int flag = 0; 
	int k2=0; 
	RotatedRect *sorted = (RotatedRect *) malloc(sizeof(RotatedRect) * N); 
	for(int i=0; i!=k; i++) {
		if(flag==1) {
		sorted[2*i] = original[32+i]; 
		sorted[2*i+1] = original[i]; 
		k2+=2;  
		} else{
		sorted[2*i] = original[i]; 
		sorted[2*i+1] = original[32+i]; 
		k2+=2;  
		}
		if(k2==8) {
			flag=!flag; 
			k2=0;
		}	
	}
	return sorted; 
}

/* This function returns an array of RotatedRects containing the chess blocks */ 
RotatedRect *label(VideoCapture cam) {
	char key; //to quit
	RotatedRect *rects = (RotatedRect *)malloc(64 * sizeof(RotatedRect)); 

	/* These are configuration trackbars. Will be removed once the chess board is final. */ 
	namedWindow("Value and Saturation1"); 
	/*namedWindow("Value and Saturation1"); 
	int vL=0, vH=0, sL=0, sH=0; 
	createTrackbar("sH", "Value and Saturation", &sH, 255); 
	createTrackbar("sL", "Value and Saturation", &sL, 255); 
	createTrackbar("vH", "Value and Saturation", &vH, 255); 
	createTrackbar("vL", "Value and Saturation", &vL, 255); 
	*/
	int vL1=0, vH1=0, sL1=0, sH1=0; 
	createTrackbar("sH1", "Value and Saturation1", &sH1, 255); 
	createTrackbar("sL1", "Value and Saturation1", &sL1, 255); 
	createTrackbar("vH1", "Value and Saturation1", &vH1, 255); 
	createTrackbar("vL1", "Value and Saturation1", &vL1, 255); 
	createTrackbar("minArea", "Value and Saturation1", &min_area, 10000); 
	createTrackbar("maxArea", "Value and Saturation1", &max_area, 10000); 
	/* end config */
	//Start main loop: 
	while(key!=27) {
		//Read and threshold image. Clean up. 
		
		cam.read(src);
		cvtColor(src,hsv, CV_BGR2HSV); 
	//	inRange(hsv, Scalar(0, sL, vL), Scalar(179, sH,vH),holded); 
		//threshold(holded, holded, 15, 255, THRESH_BINARY); 
		inRange(hsv, Scalar(0, sL1, vL1), Scalar(179, sH1,vH1),holded1); 
		//threshold(holded1, holded1, 12, 255, THRESH_BINARY); 
		//medianBlur(holded, holded, 15); 
		medianBlur(holded1, holded1, 15);
		//imshow("holded_for_white", holded); 
		//imshow("holded_for_black", holded1);
		waitKey(33); 
		//destroyAllWindows();  
		//Fiond contours for black and white , find moments		
	//	vector<vector<Point> > contours; 		
	//	vector<Vec4i> hierarchy;	
		vector<vector<Point> > contours1; 		
		vector<Vec4i> hierarchy1;
	//	findContours(holded, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );	
	//	vector<Moments> mu(contours.size() );
		findContours(holded1, contours1, hierarchy1, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );	
		vector<Moments> mu1(contours1.size() );
		/*for( int i = 0; i < contours.size(); i++ ) {
			mu[i] = moments( contours[i], false );
		}*/
		for( int i = 0; i < contours1.size(); i++ ) {
			mu1[i] = moments( contours1[i], false );
		}
				
		//vector<Point2f> mc( contours.size() );
		vector<Point2f> mc1( contours1.size() );

/*
		for( int i = 0; i < contours.size(); i++ ) {
			mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 ); 
		} */
		for( int i = 0; i < contours1.size(); i++ ) {
			mc1[i] = Point2f( mu1[i].m10/mu1[i].m00 , mu1[i].m01/mu1[i].m00 ); 
		}

		
		Mat drawing = Mat::zeros( holded1.size(), CV_8UC3 );//for drawing

		
		int k=0; //k will count number of white/black boxes which are valid. 
		/*for( int i = 0; i< contours.size(); i++ )
     		{

			
			if(flag==1) continue; 
			if(checkContour(contours[i])==false) continue; 
			RotatedRect r1 = minAreaRect( Mat(contours[i])); 	
			waitKey(1); 
			/* start drawing functions */ /*
			Point2f r[4]; r1.points(r);
			for( int j = 0; j < 4; j++ ){ 
          			line( drawing, r[j], r[(j+1)%4], Scalar(255,255,0), 2, 8 ); 
			
     			}
			string s;
			stringstream out;
			out << k;
			s = out.str();
			putText(drawing, s, mc[i], 1,1, Scalar(255,255,255)); 
			rects[k] = r1; 
			
			/* end drawing functions */  /*
			k++;  			
			if(k==32) { //k=32 implies we are finished with the chess blocks
				flag=1; 
				drawing.copyTo(final1); 
			}
	
     		}	
		sort_array_y(rects, 32); 
		for(int m=0; m!=8; m++) {
			sort_array_x(rects+(4*m), 4); 
		}
		print_array(rects, 32); */ 
		k=0;	//reset white/black counter for black 
		for( int i = 0; i< contours1.size(); i++ )
     		{

			if(flag2==1) continue;			
			if(checkContour(contours1[i])==false) continue; 
			RotatedRect r1 = minAreaRect( Mat(contours1[i])); 
			waitKey(1); 
			/* start drawing functions */	
			Point2f r[4]; r1.points(r);
			for( int j = 0; j < 4; j++ ){ 
          			line( drawing, r[j], r[(j+1)%4], Scalar(0,0,255), 2, 8 );
     			} 
			string s;
			stringstream out;
			out << k;
			s = out.str();
			putText(drawing, s, mc1[i], 1,1, Scalar(255,0,255)); 
			rects[k+32] = r1; 
			/* end drawing function */ 

			k++;  		
			if(k==32) {
				flag2=1;
				drawing.copyTo(final2);
			}	
     		}
     	sort_array_y(rects+32, 32); 
			for(int m=0; m!=8; m++) {
				sort_array_x(32+rects+(4*m), 4); 
			}
		print_array(rects+32, 32); 
		
		imshow("COM", src+drawing); //Display current drawn imgs
		key=waitKey(30); 
	}
imshow("final", src+final2); //display the final contours 
	//Here I need to populate the white array ie the 0-31 of rects. 32-63 is already populated. 
	int flg = 1; 
	int ctr=0; //count ofthe rectangles
	float widths[8]={0,0,0,0,0,0,0,0}; 
	for(int i=0; i!=8; i++) {
		for(int j=0; j!=4; j++) {
			widths[i]+=rects[32+4*i+j].size.width; 
			//This average is the problem. 
			//Messes up everything! 
			cerr<<"Width of "<<32+4*i+j<<"th width is equal to "<<rects[32+4*i+j].size.width<<endl; 
			}
			cerr<<"Sum of widths in this case is equal to"<<widths[i]<<endl; 
			widths[i]/=4; 
			cerr<<"Average of widths in this case is equal to"<<widths[i]<<endl; 
		}
	for(int j = 32; j!=64; j++) {
			int i = j-32; 
			float centerX = rects[j].center.x, centerY = rects[j].center.y;
			Point2f p; 
			p.y = centerY; 	
			if(flg==1) {
					p.x = centerX - widths[i%4]; 
				}  else {
					p.x = centerX + widths[i%4]; 
				}
			RotatedRect rn(p, rects[j].size, rects[j].angle); 
			rects[i] = rn; 
			ctr++;
			cerr<<"Rectangle created at centerX, centerY, Width "<<rects[i].center.x<<" "<<rects[i].center.y<<" "<<endl; 
			if(ctr%4==0) flg=!flg; 
			
		}
	waitKey(0); 
	return sorting(rects, 64);
}
