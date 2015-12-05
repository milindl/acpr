/* in this routine I need to sort the board in positional order. I will be supplied an array of rotated rects
 * I will need to first sort as per y and then sort each sub array of 8 in terms of x */
 
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
using namespace std;
using namespace cv;
//RotatedRectName.center.x; 
void swap(RotatedRect *a, RotatedRect *b) {
		RotatedRect temp = *a; 
		*a = *b; 
		*b=temp; 
	}

void sort_array_y(RotatedRect *arr, int N) { //sort as per y
		int i, j; 
		for(i=0; i!=N; i++) {
				for(j=0; j!=N; j++) {
						if(arr[j].center.y>arr[i].center.y) swap(arr[j], arr[i]); 
					}
			}
	}

void sort_array_x(RotatedRect *arr, int N) { //sort as per x
		int i, j; 
		for(i=0; i!=N; i++) {
				for(j=0; j!=N; j++) {
						if(arr[j].center.x>arr[i].center.x) swap(arr[j], arr[i]); 
					}
			}
	}

//Test function 
void print_array(RotatedRect *arr, int N) {
		for(int i=0; i!=N; i++) {
				cout<<"X,Y coordinate of point number "<<i<<" is "<<arr[i].center.x<<" , "<<arr[i].center.y<<endl; 
			}
	}
/*
int main() {
		RotatedRect r1[] = {RotatedRect(Point2f(1,2), Size2f(1,2), 90), 
							RotatedRect(Point2f(2,5), Size2f(1,2), 90), 
							RotatedRect(Point2f(6,9), Size2f(1,2), 90), 
							RotatedRect(Point2f(7,0), Size2f(1,2), 90)};
		//Treat as 3*2 array 
		print_array(r1, 4); 
		cout<<"Printed the normal shit"<<endl; 
		sort_array_y(r1, 4);
		print_array(r1, 4); 
		cout<<"Printed the sorted sorted shit"<<endl; 
		for(int m=0; m!=2; m++) {
				sort_array_x(r1+m*2, 2); 
			}
		print_array(r1, 4);  
		cout<<"Printed the y sorted shit"<<endl; 
		return 0; 
	}

*/
