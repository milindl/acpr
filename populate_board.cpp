 
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sstream> 
using namespace cv; 
using namespace std; 

int min_area=0, max_area=0; 
int min_angle = 15, max_angle=75; 
float min_ratio = 0.8, max_ratio=1.2;
//To be used for proc
Mat src,hsv, holded, holded1; 

//To be used for printing
Mat final1, final2; 
int flag=0, flag2=0;
 
RotatedRect *label(VideoCapture); 
bool inside_rect(RotatedRect, Point2f); 


int *populate_board(Mat original_board, Mat final_board, RotatedRect *rects) {
		
	int *loc = (int *) calloc(64,sizeof(int)); 
	/* subtraction */
	Mat subtracted; 
	absdiff(original_board, final_board, subtracted); 
	
	imshow("subtracted", subtracted); 
	waitKey(0); 
	cvtColor(subtracted, subtracted, CV_BGR2GRAY); 
	threshold(subtracted, subtracted, 15, 255, THRESH_BINARY); 
	medianBlur(subtracted, subtracted, 15); 	
	
	imshow("subtracted", subtracted); 
	waitKey(0); 
	/* end subtraction */ 

	/* find contours and centre of masses */ 
	vector<vector<Point> > contours; vector<Vec4i> hierarchy; 
	findContours(subtracted, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	Mat drawing= Mat::zeros(original_board.size(), CV_8UC3); 
	vector<Point2f> CoMs(64); 
	for(int i=0; i!=contours.size(); i++) {
			Moments m = moments(contours[i], false); 
			float CoM_x = m.m10/m.m00; 
			float CoM_y = m.m01/m.m00; 
			CoMs[i] = Point2f(CoM_x, CoM_y); 
			circle(drawing, Point(CoM_x, CoM_y), 2, Scalar(0,0,255), -1,8,0); 
		}	
	imshow("..", original_board+drawing); 
	
	
	waitKey(0); 
	for(int i = 0; i!=64; i++) { //Loop thru rects
			for(int j =0; j!=64; j++)  { //Loop thru all points
					bool result = inside_rect(rects[i], CoMs[j]); 
					if(result==true) loc[i] = 1; 
				}
		}
	return loc; 

}

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
RotatedRect *sorting(RotatedRect *original, int N) {
	int k = N/2; 
	int flag = 0; 
	int k2=0; 
	RotatedRect *sorted = (RotatedRect *) malloc(sizeof(RotatedRect) * N); 
	for(int i=0; i!=k; i++) {
		if(flag==0) {
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

bool inside_rect(RotatedRect r, Point2f p) {
		//Check if point lies inside the boundaries of the rectangle. 
		Point2f temp_array[4]; 
		r.points(temp_array); 
		vector<Point2f> rect_points(temp_array, temp_array+4); 
		double result = pointPolygonTest(rect_points, p, false);
		return result>0?true:false; 
	}


/* This function returns an array of RotatedRects containing the chess blocks */ 
RotatedRect *label(VideoCapture cam) {
	char key; //to quit
	RotatedRect *rects = (RotatedRect *)malloc(64 * sizeof(RotatedRect)); 

	/* These are configuration trackbars. Will be removed once the chess board is final. */ 
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
	createTrackbar("minArea", "Value and Saturation", &min_area, 10000); 
	createTrackbar("maxArea", "Value and Saturation", &max_area, 10000); 
	/* end config */
	//Start main loop: 
	while(key!=27) {
		//Read and threshold image. Clean up. 
		cam.read(src);
		cvtColor(src,hsv, CV_BGR2HSV); 
		inRange(hsv, Scalar(20, sL, vL), Scalar(50, sH,vH),holded); 
		inRange(hsv, Scalar(120, sL1, vL1), Scalar(150, sH1,vH1),holded1); 
		medianBlur(holded, holded, 15); 
		medianBlur(holded1, holded1, 15);

		//Fiond contours for black and white , find moments		
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

		
		Mat drawing = Mat::zeros( holded.size(), CV_8UC3 );//for drawing

		
		int k=0; //k will count number of white/black boxes which are valid. 
		for( int i = 0; i< contours.size(); i++ )
     		{

			
			if(flag==1) continue; 
			if(checkContour(contours[i])==false) continue; 
			RotatedRect r1 = minAreaRect( Mat(contours[i])); 	
			/* start drawing functions */
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
			/* end drawing functions */ 
			k++;  			
			if(k==32) { //k=32 implies we are finished with the chess blocks
				flag=1; 
				drawing.copyTo(final1); 
			}
	
     		}	

		k=0;	//reset white/black counter for black 
		for( int i = 0; i< contours1.size(); i++ )
     		{

			if(flag2==1) continue;			
			if(checkContour(contours1[i])==false) continue; 
			RotatedRect r1 = minAreaRect( Mat(contours1[i])); 

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
		
		imshow("COM", src+drawing); //Display current drawn imgs
		key=waitKey(30); 
	}
	imshow("final", src+final2 + final1); //display the final contours 
	
	waitKey(0); 
	return sorting(rects, 64);
}

int main() {
	Mat o,t; 
	
	VideoCapture c(1); 
	RotatedRect *rects = label(c); 
	while(1) {
	c.read(o);
	imshow("o", o); 
	char k = waitKey(30);
	if(k==27) break; 
	} 
	while(1) {
	c.read(t); 
	imshow("t", t); 
	char  k = waitKey(30);
	if(k==27) break;  
	}
	destroyAllWindows(); 
	int *loc = populate_board(o,t, rects);
	for(int i=0; i!=64; i++) {
			cerr<<loc[i]<<'\t'; 
			if((i+1)%8==0) cerr<<endl;  
		}
	}
