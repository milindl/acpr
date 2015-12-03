#include <iostream>
#include <stdlib.h>
#include <sstream> 
#include <stdio.h> 
using namespace std;
int getbestmove (int **, int,int, int);
int X1=0,X2=0,Y1=0,Y2=0;
int K = 10000, Q = 79, R = 50, B = 33, N = 31, P = 10, Depth = 5;
int
getmin (int a, int b)
{
  if (a < b)
    return a;
  return b;
}

int
getmax (int a, int b)
{
  if (a > b)
    return a;
  return b;
}

int
mod (int a)
{
  if (a > 0)
    return a;
  return -a;
}

int getscore(int **);
void play (int **);
void PrintBoard (int **);
void cpy(int **,int **);

struct list
{
  int x;
  int y;
  list*next;
};

list *posmoves(int **,int ,int ,list *);
int **makeBoard(char *board_shit) {
		int **bd = (int**) malloc(sizeof(int *) * 8); 
		for(int i=0; i!=8; i++) {
				bd[i] = (int *) malloc(sizeof(int) * 8); 
				for(int j=0; j!=8; j++) {
						char key = board_shit[j*8+i]; 						
						//int K = 10000, Q = 79, R = 50, B = 33, N = 31, P = 10, Depth = 5;
						if(key=='K') bd[i][j] = K; 
						if(key=='k') bd[i][j] = -K; 
						if(key=='Q') bd[i][j] = Q; 
						if(key=='q') bd[i][j] = -Q; 
						if(key=='R') bd[i][j] = R; 
						if(key=='r') bd[i][j] = -R;	
						if(key=='B') bd[i][j] = B; 
						if(key=='b') bd[i][j] = -B;
						if(key=='N') bd[i][j] = N; 
						if(key=='n') bd[i][j] = -N; 
						if(key=='P') bd[i][j] = P; 
						if(key=='p') bd[i][j] = -P; 
						if(key=='0') bd[i][j] = 0; 
					}
			}
			return bd; 
	}
int main ()
{
 while(1) {
	 char board_state[64]; 
	 scanf("%s", board_state); 
	 int **bd = makeBoard(board_state); 
	 PrintBoard(bd); 
	 int mv = getbestmove(bd, 1,0,1); 
	 cout<<mv; 
	 }
  return 0;
}

int
King (int move, int **a)
{
  int x1, y1, x2, y2;
  y2 = move % 10;
  move /= 10;
  x2 = move % 10;
  move /= 10;
  y1 = move % 10;
  move /= 10;
  x1 = move % 10;
  if (mod (x1 - x2) < 2 && mod (y1 - y2) < 2)
    {
      if (a[x1][y1] * a[x2][y2] > 0)
	{
	  return 0;
	}
    }
  else
    return 0;
  //  if(inCheck())
  //return 0;
  return 1;
}

int
Rook (int move, int **a)
{
  int x1, y1, x2, y2;
  y2 = move % 10;
  move /= 10;
  x2 = move % 10;
  move /= 10;
  y1 = move % 10;
  move /= 10;
  x1 = move % 10;
  if (a[x1][y1] * a[x2][y2] > 1)
    return 0;
  if (x1 == x2 && mod (y1 - y2) > 0 || y1 == y2 && mod (x1 - x2) > 0);
  else
    return 0;
  if (x1 - x2 != 0)
    {
      int i, maX = getmax (x1, x2);
      for (i = getmin (x1, x2) + 1;; i++)
	{
	  if (i == maX)
	    return 1;
	  if (a[i][y1] != 0)
	    return 0;
	}
    }
  else
    {
      int i = getmin (y1, y2) + 1, maX = getmax (y1, y2);
      for (;; i++)
	{
	  if (i == maX)
	    return 1;
	  if (a[x1][i] != 0)
	    {
	      return 0;
	    }
	}
    }
  return 1;
}

int
Knight (int move, int **a)
{
  int x1, y1, x2, y2;
  y2 = move % 10;
  move /= 10;
  x2 = move % 10;
  move /= 10;
  y1 = move % 10;
  move /= 10;
  x1 = move % 10;
  if (a[x1][y1] * a[x2][y2] > 1)
    return 0;
  if (mod (x1 - x2) + mod (y1 - y2) == 3)
    if (mod (x1 - x2) == 1 || mod (x1 - x2) == 2)
      return 1;
  return 0;
}

int
Bishop (int move, int **a)
{
  int x1, y1, x2, y2;
  y2 = move % 10;
  move /= 10;
  x2 = move % 10;
  move /= 10;
  y1 = move % 10;
  move /= 10;
  x1 = move % 10;
  if (mod (x1 - x2) != mod (y1 - y2))
    return 0;
  if (a[x1][y1] * a[x2][y2] > 1)
    return 0;
  int i = 1, j = mod (x1 - x2);
  if (y1 < y2 && x1 < x2)
    {
      for (; i < j; i++)
	if (a[x1 + i][y1 + i] != 0)
	  return 0;
    }
  else if (y1 < y2 && x1 > x2)
    {
      for (; i < j; i++)
	if (a[x1 - i][y1 + i] != 0)
	  return 0;
    }
  else if (y1 > y2 && x1 > x2)
    {
      for (; i < j; i++)
	if (a[x1 - i][y1 - i] != 0)
	  return 0;
    }
  else
    {
      for (; i < j; i++)
	if (a[x1 + i][y1 - i] != 0)
	  return 0;
    }
  return 1;
}

int
Pawn (int move, int **a)
{
  int x1, y1, x2, y2;
  y2 = move % 10;
  move /= 10;
  x2 = move % 10;
  move /= 10;
  y1 = move % 10;
  move /= 10;
  x1 = move % 10;
  if (a[x1][y1] == -P)
    {
      if (x1 == 6 && x2 == 4 && a[x2][y2] == 0)
	return 1;
      if (!(x1 - x2 == 1))
	return 0;
      if (mod (y1 - y2) == 1)
	{
	  if (a[x1][y1] * a[x2][y2] >= 0)
	    return 0;
	  else
	    return 1;
	}
      if (y1 == y2)
	{
	  if (a[x1][y1] * a[x2][y2] == 0)
	    return 1;
	  else
	    return 0;
	}
    }
  else
    {
      if (x1 == 1 && x2 == 3 && a[x2][y2] == 0)
	return 1;
      if (!(x2 - x1 == 1))
	return 0;
      if (mod (y1 - y2) == 1)
	{
	  if (a[x1][y1] * a[x2][y2] >= 0)
	    return 0;
	  else
	    return 1;
	}
      if (y1 == y2)
	{
	  if (a[x1][y1] * a[x2][y2] == 0)
	    return 1;
	  else
	    return 0;
	}
    }
  return 0;
}

int
isValidMove (int move, int **a)	//return 0 if invalid   if move=1234 it means move piece from 1,2 to 3,4
{
  int x1, y1, x2, y2, tmpmove = move;
  y2 = move % 10;
  move /= 10;
  x2 = move % 10;
  move /= 10;
  y1 = move % 10;
  move /= 10;
  x1 = move % 10;
  cout<<"  "<<x1+1<<" "<<y1+1<<" "<<x2+1<<" "<<y2+1<<"\n"<<a[x1][y1]<<" "<<a[x2][y2]<<"\n";
  if (mod (a[x1][y1]) == Q)
    return Bishop (tmpmove, a) || Rook (tmpmove, a);
  if (mod (a[x1][y1]) == K)
    return King (tmpmove, a);
  if (mod (a[x1][y1]) == P)
    return Pawn (tmpmove, a);
  if (mod (a[x1][y1]) == R)
    return Rook (tmpmove, a);
  if (mod (a[x1][y1]) == N)
    return Knight (tmpmove, a);
  if (mod (a[x1][y1]) == B)
    return Bishop (tmpmove, a);
  return 0;
}

void
moveIt (int move, int **a)
{
  
  int x1, y1, x2, y2;
  y2 = move % 10;
  move /= 10;
  x2 = move % 10;
  move /= 10;
  y1 = move % 10;
  move /= 10;
  x1 = move % 10;
  a[x2][y2] = a[x1][y1];
  a[x1][y1] = 0;
  PrintBoard (a);
  
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
void play(int **a)
{
int m=1;
while(m!=-1111)
{
  cout<<"Your move :  ";
  cin>>m;
  m-=1111;
  if(m==-1111)
	break;
  if(m<0||m>7777)
  {
   cout<<"Invalid Move !!\n";
   continue;
  }
  if(isValidMove(m,a))
   moveIt(m,a);
  else
  {
   cout<<"Invalid Move !!\n";
   continue;
  }
  do {
  m = getbestmove (a, 1,0, 1);
} while(isValidMove(m,a)==0); 
      if (m == 0)
	{
	  PrintBoard (a);
	  cout << "\ncontinue\n";
	  continue;
	}
      moveIt (m, a);
      PrintBoard(a);
      cout<<"My Move "<<m+1111<<"\n";
  }
	cout<<m<<endl; 
	cout<<"Ending play"<<endl; 
}
 
int getbestmove(int **board,int turn,int score,int depth)
{
 int bestscore=-30000*turn;
 int t1,t2,bestt;
 list*move=NULL;
 if(depth==1)
 {
   for(int i=0 ; i<8 ; i++)
   {
	for(int j=0 ; j<8 ; j++)
	  {
  	   if(board[i][j]*turn>0)
	     {
		move=posmoves(board,i,j,move);
		list*first=move;
		int flag=0;
		while(move!=NULL)
		{
		  t1=board[i][j];
		  t2=board[move->x][move->y];
		  board[move->x][move->y]=board[i][j];
		  board[i][j]=0;
		//  bestt=bestscore;
		  if(turn>0)
		  bestt=getmax (bestscore, getscore(board) +getbestmove (board,-turn, bestscore, depth + 1));
		  if(turn<0)
		  bestt=getmin(bestscore,getscore(board)+getbestmove(board,-turn,bestscore,depth+1));
		  if((bestt>bestscore&&turn>0)||(bestt<bestscore&&turn<0))
			{
		  	 bestscore=bestt;
			  X1=i+1;
			 Y1=j+1;
			 X2=move->x+1;
			 Y2=move->y+1;
			}
		  board[i][j]=t1;
		  board[move->x][move->y]=t2;
		  first=move;
		  move=move->next;
		  free(first);
		}
	      }
	   }   
    }
 }
 else if(depth<Depth)
 {
  for(int i=0 ; i<8 ; i++)
  {
	for(int j=0 ; j<8 ; j++)
	{
	  if(board[i][j]*turn>0)
	   {
	    move=posmoves(board,i,j,move);
        list *first=move;
	    while(move!=NULL)
	    { 
		t1=board[i][j];
		t2=board[move->x][move->y];
		board[move->x][move->y]=board[i][j];
		board[i][j]=0;
		int newscore= (getscore(board)+getbestmove(board,-turn,bestscore,depth+1));
		if(newscore>bestscore&&turn==1)
		  bestscore=newscore;
		else if(newscore<bestscore&&turn==-1)
		  bestscore=newscore;
		board[i][j]=t1;
		board[move->x][move->y]=t2;
		first=move;
		move=move->next;
		free(first);
	    }
	   }
	}
  }
return bestscore;
}
else return 0;
return(1000*X1+100*Y1+10*X2+Y2);
}
void cpy(int **a,int **b)
{
  for(int i=0 ; i<8 ; i++)
  {
    for(int j=0 ; j<8 ; j++)
    {
	a[i][j]=b[i][j];
    }
  }
}



int getscore(int **a)
{
  int s=0 ;
  for(int i=0 ; i<8 ; i++)
  {
     for(int j=0 ; j<8 ; j++)
     {
	s+=a[i][j];
     }
  }
return s;
}

list * pop(list*start,int i,int j)
{
  if(start==NULL)
  {
    list*tmp=(list*)malloc(sizeof(list));
    tmp->x=i;
    tmp->y=j;
    tmp->next=NULL;
    return tmp;
  }
  
  list*tmp=(list*)malloc(sizeof(list));
  tmp->next=start;
  tmp->x=i;
  tmp->y=j;
  start=tmp;
  return start; 
}

int
Encode (int i, int j, int k, int l)
{
  return 1000 * i + 100 * j + 10 * k + l;
}

list *WhereToMoveKing (int **a, int i, int j, list * move)
{
  int x, y;
  for (x = -1; x < 2; x++)
    {
      for (y = -1; y < 2; y++)
	{
	  if (x + i > -1 && x + i < 8 && j + y > -1 && j + y < 8)
	    {
	      if (King (Encode (i, j, i + x, j + y), a))
		{
		  move = pop (move,i+x,j+y);
		}
	    }
	}
    }
  return move;
}

list *WhereToMoveRook (int **a, int i, int j, list * move)
{
  int x = 1, count = 0;
  while (i - x > -1)
    {
      if (Rook (Encode (i, j, i - x, j), a))
	{
	  move = pop (move, i - x, j);
	  count++;
	}
      x++;
    }
  x = 1;
  while (i + x < 8)
    {
      if (Rook (Encode (i, j, i + x, j), a))
	{
	  move = pop (move,i + x, j);
	  count++;
	}
      x++;
    }
  x = 1;
  while (j - x > -1)
    {
      if (Rook (Encode (i, j, i, j - x), a))
	{
	  move = pop (move, i, j - x);
	  count++;
	}
      x++;
    }
  x = 1;
  while (j + x < 8)
    {
      if (Rook (Encode (i, j, i, j + x), a))
	{
	  move = pop (move,i, j + x);
	  count++;
	}
      x++;
    }
  return move;
}

list *
WhereToMoveKnight (int **a, int i, int j, list * move)
{
  int x, y;
  if (i - 2 > -1)
    {
      if (j - 1 > -1)
	{
	  if (Knight (Encode (i, j, i - 2, j - 1), a))
	    {
	      move = pop (move, i - 2, j - 1);
	    }
	}
      if (j + 1 < 8)
	{
	  if (Knight (Encode (i, j, i - 2, j + 1), a))
	    {
	      move = pop (move,i - 2, j + 1);
	    }
	}
    }
  if (i + 2 < 8)
    {
      if (j - 1 > -1)
	{
	  if (Knight (Encode (i, j, i + 2, j - 1), a))
	    {
	      move = pop (move,i + 2, j - 1);
	    }
	}
      if (j + 1 < 8)
	{
	  if (Knight (Encode (i, j, i + 2, j + 1), a))
	    {
	      move = pop (move, i + 2, j + 1);
	    }
	}
    }
  if (i - 1 > -1)
    {
      if (j - 2 > -1)
	{
	  if (Knight (Encode (i, j, i - 1, j - 2), a))
	    {
	      move = pop (move, i - 1, j - 2);
	    }
	}
      if (j + 2 < 8)
	{
	  if (Knight (Encode (i, j, i - 1, j + 2), a))
	    {
	      move = pop (move, i - 1, j + 2);
	    }
	}
    }
  if (i + 1 < 8)
    {
      if (j - 2 > -1)
	{
	  if (Knight (Encode (i, j, i + 1, j - 2), a))
	    {
	      move = pop (move, i + 1, j - 2);
	    }
	}
      if (j + 2 < 8)
	{
	  if (Knight (Encode (i, j, i + 1, j + 2), a))
	    {
	      move = pop (move, i + 1, j + 2);
	    }
	}
    }
  return move;
}

list *
WhereToMoveBishop (int **a, int i, int j, list * move)
{
  int x;
  for (x = 1; x < 8; x++)
    {
      if (i - x > -1 && j - x > -1)
	{
	  if (Bishop (Encode (i, j, i - x, j - x), a))
	    {
	      move = pop (move,i - x, j - x);
	    }
	}
      if (i - x > -1 && j + x < 8)
	{
	  if (Bishop (Encode (i, j, i - x, j + x), a))
	    {
	      move = pop (move, i - x, j + x);
	    }
	}
      if (i + x < 8 && j - x > -1)
	{
	  if (Bishop (Encode (i, j, i + x, j - x), a))
	    {
	      move = pop (move, i + x, j - x);
	    }
	}
      if (i + x < 8 && j + x < 8)
	{
	  if (Bishop (Encode (i, j, i + x, j + x), a))
	    {
	      move = pop (move,i + x, j + x);
	    }
	}
    }
  return move;
}

list *
WhereToMovePawn (int **a, int i, int j, list * move)
{
  if (a[i][j] < 0)  {
      if (j > 0 && i > 0) {
		if (Pawn (Encode (i, j, i - 1, j - 1), a))    {
	      move = pop (move, i - 1, j - 1);
	    }
	}
      if (i > 0)
	{
	  if (Pawn (Encode (i, j, i - 1, j), a))
	    {
	      move = pop (move, i - 1, j);
	    }
	}
      if (i == 6)
	{
	  if (Pawn (Encode (i, j, i - 2, j), a))
	    {
	      move = pop (move, i - 2, j);
	    }
	}
      if (j < 7 && i > 0)
	{
	  if (Pawn (Encode (i, j, i - 1, j + 1), a))
	    {
	      move = pop (move, i - 1, j + 1);
	    }
	}
    }
  if (a[i][j] > 0)
    {
      if (j < 7 && i < 7)
	{
	  if (Pawn (Encode (i, j, i + 1, j + 1), a))
	    {
	      move = pop (move, i + 1, j + 1);
	    }
	}
      if (i == 1)
	{
	  if (Pawn (Encode (i, j, i + 2, j), a))
	    move = pop (move,i + 2, j);
	}
      if (i < 7)
	{
	  if (Pawn (Encode (i, j, i + 1, j), a))
	    move = pop (move, i + 1, j);
	}
      if (j > 0 && i < 7)
	{
	  if (Pawn (Encode (i, j, i + 1, j - 1), a))
	    move = pop (move, i + 1, j - 1);
	}
    }
  return move;
}

list *posmoves(int **a,int i,int j,list *move)
{
  if(a[i][j]==0)
   return move;
  else if(mod(a[i][j])==K)
   return(WhereToMoveKing(a,i,j,move));
  else if(mod(a[i][j])==P)
   return(WhereToMovePawn(a,i,j,move));
  else if(mod(a[i][j])==B)
   return(WhereToMoveBishop(a,i,j,move));
  else if(mod(a[i][j])==N)
   return(WhereToMoveKnight(a,i,j,move));
  else if(mod(a[i][j])==R)
   return(WhereToMoveRook(a,i,j,move));
  else if(mod(a[i][j])==Q)
  {
   move=WhereToMoveBishop(a,i,j,move);
   return(WhereToMoveRook(a,i,j,move));
  }
}
	 
	
