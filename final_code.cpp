#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
using namespace std;
using namespace cv; 
//Need to import all headers...make headers first! 
RotatedRect *chess_board_blocks; 


Mat init(VideoCapture cam) { //will return original image of the chess board useable by populate_board
		chess_board_blocks = label(cam); 
		Mat original_board_image; 
		cam.read(original_board_image); 
		return original_board_image; 
}

void copy_array(int *b, int *a, int N) {
		for(int i=0;i!=N; i++) {
				b[i] = a[i];
			}
	}

int main() {
	VideoCapture c(1); 
	Mat original_board_image = init(c); 
	Mat old_board, new_board; 
	int *loc = (int*) calloc(64,sizeof(int)); 
	int *loc2 = (int*) malloc(64*sizeof(int)); 
	int **board_array = (int **) malloc(sizeof(int *) * 8); 
	for(int i=0; i!=16; i++) loc[i] = 3; 
	for(int i=48; i!=64; i++) loc[i]=1; 
	for(int i=0; i!=8; i++) {
			board_array[i] = (int *) calloc(8, sizeof(int)); 
		}
	board_array[0][0] = R;
	board_array[0][1] = N;
	board_array[0][2] = B;
	board_array[0][3] = Q;	
	board_array[0][4] = K;
	board_array[0][5] = B;
	board_array[0][6] = N;
	board_array[0][7] = R;
	board_array[7][0] =-R, 
	board_array[7][1] =-N,
	board_array[7][2] =-B, 
	board_array[7][3] =-Q, 
	board_array[7][4] =-K,
	board_array[7][5] =-B,
	board_array[7][6] =-N,
	board_array[7][7] =-R;
	cout<<"Calibration done. Place your gotis"<<endl; 
	//assuming gotis are placed
	c.read(old_board); 
	while(/*no win is detected in the chess code*/) {
			//Player moves: 
			new_board = makeMove(old_board, c); 
			loc2 = populate_board(original_board_image, new_board,chess_board_blocks); 
			convert_to_board(board_array, loc, loc2); 
			//Bot moves
			int best_move = getbestmove(board, 1,0,1); 
			int y1 = best_move%1000;
			best_move/=10; 
			int x1 = best_move%100;
			best_move/=10; 
			int y2 = best_move%10;
			best_move/=10; 
			int x2 = best_move;
			//Make best move using ACPR movement functions
			//Ask mech team
			//again, need to update chessboard representation
			copy_array(loc,loc2,64);
			//change loc[N1] to 0 and loc[N2] to 3
			int N1 = y1*8 + x1; 
			int N2 = y2*8 + x2; 
			loc2[N1] = 0; 
			loc2[N2] = 3;
			convert_to_board(board, loc, loc2);	
			copy_array(loc,loc2,64);
			new_board.copyTo(old_board); 
		}
	}
