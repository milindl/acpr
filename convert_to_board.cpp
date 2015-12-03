#include <iostream>
#include <stdlib.h>

using namespace std;
/* Returns 0 if it is a yellow-yellow move
 * Returns 1 if it is a blue-blue move
 * Returns 2 if it is a yellow->blue kill
 * Returns 3 if it is a blue->yellow kill
 */
 
int sum(int *mat, int n) {
		int sum=0; 
		for(int i = 0; i!=n; i++) sum+=mat[i]; 
		return sum; 
	}
 
int recog(int *board_original, int *board_final) {
	int *difference = (int *) calloc(64, sizeof(int)); 
	int flag_is_three_found=0; 
	for(int i=0; i!=64; i++) {
			difference[i]=board_original[i] - board_final[i]; 
			if(difference[i]==3) flag_is_three_found=1; 
		}
		int s = sum(difference, 64); 
		if(s==1) return 2 ; 
		if(s==3) return 3;  
		if(flag_is_three_found==1) return 0 ; 
		return 1; 
	}
int getY(int N) {
		return N/8; 
	}
int getX(int N) {
		return N%8; 
	}
void convert_to_board(int **board, int *oldb, int *newb) { //modifies the old board itself! 
	int *difference = (int *) calloc(64, sizeof(int)); 
	for(int i=0; i!=64; i++) {
			difference[i]=oldb[i] - newb[i]; 
		}
		int move_r = recog(oldb, newb); 
		//Deal with cases separately
		//Case 1: Yellow kills blue
		if(move_r==2) {
				for(int i=0; i!=64; i++) {
					if(difference[i]==3) {
							int x = getX(i); 
							int y = getY(i); 
							board[y][x] = 0; 
						}
					if(difference[i]==-2) {
							int x = getX(i); 
							int y = getY(i); 
							board[y][x] = 3; 
						}
					}
			}
		//Case 2: Blue kills yellow
		if(move_r==3) {
				for(int i=0; i!=64; i++) {
					if(difference[i]==1) {
							int x = getX(i); 
							int y = getY(i); 
							board[y][x] = 0; 
						}
					if(difference[i]==2) {
							int x = getX(i); 
							int y = getY(i); 
							board[y][x] = 1; 
						}
					}
			}	
			//Case: yellow moves
			if(move_r==0) {
				for(int i=0; i!=64; i++) {
					if(difference[i]==-3) {
							int x = getX(i); 
							int y = getY(i); 
							board[y][x] = 3; 
						}
					if(difference[i]==3) {
							int x = getX(i); 
							int y = getY(i); 
							board[y][x] = 0 ;
						}
					}
			}	
			//Case: blue moves
			if(move_r==1) {
				for(int i=0; i!=64; i++) {
					if(difference[i]==-1) {
							int x = getX(i); 
							int y = getY(i); 
							board[y][x] = 1; 
						}
					if(difference[i]==1) {
							int x = getX(i); 
							int y = getY(i); 
							board[y][x] = 0 ;
						}
					}
			}
	}

int main() {
		int *o_board = (int *) calloc(64, sizeof(int));  
		int *f_board = (int *) calloc(64, sizeof(int));  
		o_board[0] = 3; o_board[1] = 0; 
		f_board[0]=0; f_board[1] = 3; 
		cout<<recog(o_board, f_board);
		return 0; 
	}
