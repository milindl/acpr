
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sstream> 
using namespace cv; 
using namespace std; 

bool inside_rect(RotatedRect, Point2f); 


int main() {
		RotatedRect r1(Point2f(0,0), Size2f(3,4), -55); 
		Point2f p1(0,0); 
		Point2f p2(5,5); 	
		cout<<inside_rect(r1,p1)<<endl; 
		cout<<inside_rect(r1,p2)<<endl; 
}

bool inside_rect(RotatedRect r, Point2f p) {
		//Check if point lies inside the boundaries of the rectangle. 
		Point2f temp_array[4]; 
		r.points(temp_array); 
		vector<Point2f> rect_points(temp_array, temp_array+4); 
		double result = pointPolygonTest(rect_points, p, false);
		return result>0?true:false; 
	}
