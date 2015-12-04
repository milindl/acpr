#include <iostream>
#include <stdlib.h>

using namespace std;
/* Returns 0 if it is a yellow-yellow move
 * Returns 1 if it is a blue-blue move
 * Returns 2 if it is a yellow->blue kill
 * Returns 3 if it is a blue->yellow kill
 */
 int K = 10000, Q = 79, R = 50, B = 33, N = 31, P = 10;
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
int **convert_to_board(int **board, int *oldb, int *newb) { //modifies the old board itself! 
	int *difference = (int *) calloc(64, sizeof(int)); 
	for(int i=0; i!=64; i++) {
			difference[i]=oldb[i] - newb[i]; 
		}
		int move_r = recog(oldb, newb); 
		//Deal with cases separately

			//Case: yellow moves
		if(move_r==0) {
			int which_moved;
			for(int i=0; i!=64; i++) {
				if(difference[i]==3) {
						int x = getX(i); 
						int y = getY(i); 
						which_moved = board[x][y] ;
						board[x][y] = 0; 
					}
				}
			for(int i=0; i!=64; i++) {
				if(difference[i]==-3) {
						int x = getX(i); 
						int y = getY(i); 
						board[x][y] = which_moved; 
					}
					
				}
			}	
			//Case: blue moves
			if(move_r==1) {
				int which_moved=99; 
				for(int i=0; i!=64; i++) {
					if(difference[i]==1) {
							int x = getX(i); 
							int y = getY(i); 
							which_moved = board[x][y] ;
							board[x][y]=0; 
						}
					}
				for(int i=0; i!=64; i++) {
					if(difference[i]==-1) {
							int x = getX(i); 
							int y = getY(i); 
							board[x][y] = which_moved; 
						}
					
					}
			}
			//Case yellow kill blue: 
			if(move_r==2) {
					int which_moved = 99; 
					for(int i=0; i!=64; i++) {
							if(difference[i]==3) {
								int x = getX(i); 
								int y = getY(i); 
								which_moved = board[x][y]; 
								board[x][y] = 0;
							}
						}
					for(int i=0; i!=64; i++) {
							if(difference[i]==-2) {
								int x = getX(i); 
								int y = getY(i); 
								board[x][y] = which_moved; 
							}
						}
				}
			//Case blue kill yellow: 
			if(move_r==3) {
					int which_moved = 99; 
					for(int i=0; i!=64; i++) {
							if(difference[i]==1) {
								int x = getX(i); 
								int y = getY(i); 
								which_moved = board[x][y]; 
								board[x][y] = 0;
							}
						}
					for(int i=0; i!=64; i++) {
							if(difference[i]==2) {
								int x = getX(i); 
								int y = getY(i); 
								board[x][y] = which_moved; 
							}
						}
				} 
			
			 return board; 
	}
int mod(int a) {
		return a>0?a:-a; 
	}

void
PrintBoard (int **a)
{
  int i, j;
  for (i = 1; i < 9; i++)
    cout<<"   "<<i<<"   ";
  cout<<"\n";
  for (i = 0; i < 8; i++)
    {
      cout<<" ";
      if (i & 1)
	for (j = 0; j < 4; j++)
	  cout<<"******      ";
      else
	for (j = 0; j < 4; j++)
	  cout<<"      ******";
      cout<<"\n"<< i + 1;
      for (j = 0; j < 8; j++)
	{
	  if ((i + j) & 1)
	    cout<<"**";
	  else
	    cout<<"  ";
	  if (mod (a[i][j]) == K)
	    {
	      if (a[i][j] > 0)
		cout<<"WK";
	      else
		cout<<"BK";
	    }
	  else if (mod (a[i][j]) == Q)
	    {
	      if (a[i][j] > 0)
		cout<<"WQ";
	      else
		cout<<"BQ";
	    }
	  else if (mod (a[i][j]) == R)
	    {
	      if (a[i][j] > 0)
		cout<<"WR";
	      else
		cout<<"BR";
	    }
	  else if (mod (a[i][j]) == B)
	    {
	      if (a[i][j] > 0)
		cout<<"WB";
	      else
		cout<<"BB";
	    }
	  else if (mod (a[i][j]) == N)
	    {
	      if (a[i][j] > 0)
		cout<<"WN";
	      else
		cout<<"BN";
	    }
	  else if (mod (a[i][j]) == P)
	    {
	      if (a[i][j] > 0)
		cout<<"WP";
	      else
		cout<<"BP";
	    }
	  else if (a[i][j] == 0)
	    {
	      if ((j + i) & 1)
		cout<<"**";
	      else
		cout<<"  ";
	    }
	  if ((i + j) & 1)
	    cout<<"**";
	  else
	    cout<<"  ";
	}
      cout<<"\n ";
      if (i & 1)
	for (j = 0; j < 4; j++)
	  cout<<"******      ";
      else
	for (j = 0; j < 4; j++)
	  cout<<"      ******";
      cout<<"\n";
    }
}

int main() {
		int o_board[] ={3,3,0,0,0,0,1,1,3,3,0,0,0,0,1,1,3,3,0,0,0,0,1,1,3,3,0,0,0,0,1,1,3,3,0,0,0,0,1,1,3,3,0,0,0,0,1,1,3,3,0,0,0,0,0,1,3,3,1,0,0,0,1,1}; 
		int f_board[] = {3,3,0,0,0,0,1,1,3,3,0,0,0,0,1,1,3,3,0,0,0,0,1,1,3,3,0,0,0,0,1,1,3,3,0,0,0,0,1,1,3,3,0,0,0,0,1,1,3,1,0,0,0,0,0,1,3,3,0,0,0,0,1,1}; 
		int **board_d = (int **) malloc(sizeof(int *) * 8); 
		int re=recog(o_board, f_board); 
		cout<<re<<endl; 
		for(int i = 0; i!=8; i++) {
				board_d[i] = (int *) malloc(sizeof(int) * 8); 
			}
		int board[8][8] = {{R,N,B,Q,K,B,N,R},
							{P,P,P,P,P,P,P,P},
							{0,0,0,0,0,0,0,-P},
							{0,0,0,0,0,0,0,0},
							{0,0,0,0,0,0,0,0},
							{0,0,0,0,0,0,0,0},
							{-P,-P,-P,-P,-P,-P,0,-P},
							{-R,-N,-B,-Q,-K,-B,-N,-R}
							
							}; 
							
		for(int i=0; i!=8; i++) {
				for(int j=0; j!=8; j++) board_d[i][j] = board[i][j]; 
			}
		
		PrintBoard(board_d); 
		cout<<"\n\n\n\n"<<endl;
		int **bdx = convert_to_board(board_d, o_board, f_board); 
		PrintBoard(board_d); 
		return 0; 
	}
