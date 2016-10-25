//	+-----------------------------------------------------------------------------+
//	|                               Sebastien Charles                             |
//	|                        ECE 469 - Artificial Intelligence                    |
//	|                                    Prof. Sable                              |
//	+-----------------------------------------------------------------------------+

#include <iostream>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include <limits>
#include <ctime>
#include<vector>
#include <cstring>
#include <algorithm> 	
#include "colormod.h"
#include <fstream>
#include <string>

using namespace std;

#define ROWS 8
#define COLS 8

#define EMPTY 1
#define RED 2
#define BLACK 3
#define REDKING 4
#define BLACKKING 5

class move{
	public:
		int i;
		char j;
		int k;
		char l;
		move();
		move(int ii, char jj, int kk, char ll);
		void change(int ii, char jj, int kk, char ll);
		bool test;
};

move::move(int ii, char jj, int kk, char ll){
			i=ii;
			j=jj;
			k=kk;
			l=ll;
			test = false;
		};
		
void move::change(int ii, char jj, int kk, char ll){
			i=ii;
			j=jj;
			k=kk;
			l=ll;
		};

bool isRed(int player);
bool isBlack(int player);
bool isCPU(int player);
void swap(int d[ROWS][COLS], int i, int j, int k, int l);
char value2symbol(int i);
void printDisplayFancy(int d[ROWS][COLS]);
int Playersturn(int d[ROWS][COLS], int player,int i,int j,int k,int l);
int checkMove(int d[ROWS][COLS], int player,int i,int j,int k,int l);
int testMove(int d[ROWS][COLS], int player,int i,int j,int k,int l, clock_t start, double limit);
int legalMoves(int player, int d[ROWS][COLS]);
int checkJump(int d[ROWS][COLS],int i,int j);
int playerMove(int d[ROWS][COLS], int player);
int cpuMove(int d[ROWS][COLS], int player);
int testCPU(int d[ROWS][COLS], int player, clock_t start, double limit);
vector<move> cpuCheck(int d[ROWS][COLS], int player);
int heuristic(int b[8][8], int player);
move* minimax(int d[8][8], int player, clock_t start);
int maxMove(int d[8][8], move *bestMove, int player, double limit, clock_t start, int alpha, int beta, int bottom, int depth);
int minMove(int d[8][8], move *bestMove, int player, double limit, clock_t start, int alpha, int beta, int bottom, int depth);
int switchPlayer(int player);
int canJump = 0;
int tlimit=0;
int red;
int black;


bool isCPU(int player){
	if(player==RED){
    	if(red==0){
			return false;
		}
		else{
			return true;
		}
	}
	else{
		if(black==0){
			return false;
		}
		else{
			return true;
		}
	}
}


int switchPlayer(int player){
	if(player==2){
		return 3;
	}
	else{
		return 2;
	}
}


//Returns a zero sum evaluation of the board state
int heuristic(int b[8][8], int player){
	int i;
	int j;
	int result=0;
	int nred=0;
	int nblack=0;
	int redval=0;
	int blackval=0;
	int redking=0;
	int blackking=0;
	int redres=0;
	int blackres=0;
	
	for(i=0; i<8; i++){
		for(j=0; j<8; j++){
			if(b[i][j]==2){
				nred++;
				if(b[i-1][j-1]==2 || b[i-1][j-1]==4){
					redval++;
				}
			}

			if(b[i][j]==4){
				redking++ ;
				if(b[i-1][j-1]==2 || b[i-1][j-1]==4){
					redval++;
				}
			}
			
			if(b[i][j]==3){
				nblack++;
				if(b[i+1][j+1]==2 || b[i+1][j+1]==4){
					blackval++;
				}
			}
			
			if(b[i][j]==5){
				blackking++;
				if(b[i+1][j+1]==2 || b[i+1][j+1]==4){
					blackval++;
				}
			}
		}
	}
	
	if(player==2){
		for(i=2; i<=5; i++){
			for(j=3; j<=4; j++){
				if(b[i][j]==2 || b[i][j]==4){
					redval++;
				}
			}
		}
		
		for(j=0; j<8; j++){
			if(b[7][j]==2){
				redval++;
			}
		}
		
		if((nred + redking)<6){
			redking = redking + 2;
		}
	}

	if(player==3){
		for(i=2; i<=5; i++){
			for(j=3; j<=4; j++){
				if(b[i][j]==2 || b[i][j]==4){
					blackval++;
				}
			}
		}
		
		for(j=0; j<8; j++){
			if(b[7][j]==3){
				blackval++;
			}
		}
		
		if((nblack + blackking)<6){
			blackking = blackking + 2;
		}
	}
	
	redres = nred + redval + (2*redking);
	blackres = nblack + blackval+ (2*blackking);

	if(nred + redking ==0){
		blackres = numeric_limits<int>::max();
	}

	if(nblack + blackking ==0){
		redres = numeric_limits<int>::max();
	}


	

	if(player==2){
		return redres-blackres;
	}
	if(player==3){
		return blackres-redres;
	}
	return 0;
}

int dep = 0;

double tim=0;

move* minimax(int d[8][8], int player, clock_t start){

	int i;

	move *bestMove = new move(0,0,0,0);
	
	
	vector<move> moves = cpuCheck(d, player);
	
	if(moves.size()==1){
		dep = 1;
		tim = (clock() - start) / CLOCKS_PER_SEC;
		bestMove->change(moves[0].i, moves[0].j, moves[0].k, moves[0].l);

		return bestMove;
	}
	
	
	double limit = 0.5*tlimit;

	int bottom=0;
	
	double end;

	for(;;){

		end= (clock() - start) / CLOCKS_PER_SEC;

		if(end >= limit){
			break;
		}

		int depth = 0;

		bottom++;
		
		i = maxMove(d, bestMove, player, limit, start, numeric_limits<int>::min(), numeric_limits<int>::max(), bottom, depth);
		
	}
	
	dep = bottom;
	tim = (clock() - start) / CLOCKS_PER_SEC;
	return bestMove;
}

int maxMove(int d[8][8], move *bestMove, int player, double limit, clock_t start, int alpha, int beta, int bottom, int depth){
	
	int i=0;
	int res = numeric_limits<int>::min();
	int b[8][8];

	std::copy(&d[0][0], &d[0][0]+8*8,&b[0][0]);
	
	vector<move> moves = cpuCheck(b, player);
	
	double end= (clock() - start) / CLOCKS_PER_SEC;
	
	//cout<<"MaxEnd = "<<end<<endl;
	
	if(moves.size()==0 || end >= limit || depth >= bottom){

		return heuristic(b, player);
	}
	
	for(i=0; i<moves.size(); i++){
		moves = cpuCheck(b, player);
		testMove(b, player, moves[i].i-1, moves[i].j - 'a', moves[i].k-1, moves[i].l - 'a', start, limit);
		move *opp = new move(0,0,0,0);
		
		int curate;
		depth++;
		
		if(!isCPU(switchPlayer(player))){
			if(player==2){
				black = 1;
			}
			else{
				red = 1;
			}
			curate = minMove(b, opp, switchPlayer(player), limit, start, alpha, beta, bottom, depth);
			if(player==2){
				black = 0;
			}
			else{
				red = 0;
			}
		}
		else{
			curate = minMove(b, opp, switchPlayer(player), limit, start, alpha, beta, bottom, depth);
		}
		
		delete opp;
		

		
		

		//cout<<"curate: "<<curate<<endl;
		if(curate > res){
			res = curate;
			bestMove->change(moves[i].i, moves[i].j, moves[i].k, moves[i].l);
		}

		if(res > beta){

			return res;
		}
		
		alpha = max(alpha, res);
		
		std::copy(&d[0][0], &d[0][0]+8*8,&b[0][0]);
	}

	return res;
	
}

int minMove(int d[8][8], move *bestMove, int player, double limit, clock_t start, int alpha, int beta, int bottom, int depth){
	int i=0;
	int res= numeric_limits<int>::max();

	int b[8][8];
	std::copy(&d[0][0], &d[0][0]+8*8,&b[0][0]);
	vector<move> moves = cpuCheck(b, player);
	
	double end= (clock() - start) / CLOCKS_PER_SEC;
	
	//cout<<"MinEnd = "<<end<<endl;
	
	if(moves.size()==0 || end >= limit || depth >= bottom){

		return heuristic(b, player);
	}
	
	depth++;
	
	for(i=0; i<moves.size(); i++){
		moves = cpuCheck(b, player);
		testMove(b, player, moves[i].i-1, moves[i].j - 'a', moves[i].k-1, moves[i].l - 'a', start, limit);
		move *opp = new move(0,0,0,0);
		int curate = maxMove(b, opp, switchPlayer(player), limit, start, alpha, beta, bottom, depth);

		delete opp;

		if(curate < res){
			res = curate;
			bestMove->change(moves[i].i, moves[i].j, moves[i].k, moves[i].l);
		}
		std::copy(&d[0][0], &d[0][0]+8*8,&b[0][0]);
		
		
		
		if(res < alpha){

			return res;
		}
		
		beta = min(beta, res);
		
	}
	
	return res;
	
}





bool isRed(int player){
	if((player== RED) || (player==REDKING)){
		return true;
	}
	else{
		return false;
	}
};

bool isBlack(int player){
	if((player== BLACK) || (player==BLACKKING)){
		return true;
	}
	else{
		return false;
	}
};

int cpuMove(int d[ROWS][COLS], int player){
	int r,c;
	int pr, pb;
	int i, k;
	int rng;
	char j, l;
	
	cout<< "Computer is searching for move now...\n";
	
	vector<move> moves = cpuCheck(d, player);

	clock_t start = clock(); //Start timer
	move *best = minimax(d, player, start);

	cout<< "Computer finished searching at depth "<<dep<<" in "<<tim<<" seconds"<<endl;
	
	

	i = best->i;
	j = best->j;
	k = best->k;
	l = best->l;
	
	canJump=0;
	
	delete best;

	
	if(player==RED){
		printf("Green's turn: ");
	}
	else{
		printf("Blue's turn: ");
	}
	
	cout<<"( "<<i<<j<<","<<k<<l<<")\n";

	//swap(d,i-1,j - 'a',k-1,l - 'a');
	Playersturn(d, player, i-1,j - 'a',k-1,l - 'a');
}

int testCPU(int d[ROWS][COLS], int player, clock_t start, double limit){
	int r,c;
	int pr, pb;
	int i, k;
	int rng;
	char j, l;
	
	vector<move> moves = cpuCheck(d, player);

	move *best = minimax(d, player, start);

	//cout<< "test: "<<best->i<<endl;

	i = best->i;
	j = best->j;
	k = best->k;
	l = best->l;
	
	canJump=0;
	
	//delete best;
	

	
	//swap(d,i-1,j - 'a',k-1,l - 'a');
	testMove(d, player, i-1,j - 'a',k-1,l - 'a', start, limit);
}

int playerMove(int d[ROWS][COLS], int player){

	int r,c;
	int pr, pb;
	int i, k;
	char j, l;
    
    while(1){
    	if(player==RED){
        	printf("Green's turn: ");
        }
        else{
        	printf("Blue's turn: ");
        }
        scanf("%d%c%d%c",&i,&j,&k,&l);
        
        
        if(i>8 || i<1 || k>8 || k<1 || (j - 'a')<0 || (j - 'a')>7 || (j - 'a')<0 || (j - 'a')>7){
        	cout<< "Invald move\n";
        	while ( getchar() != '\n' );
        	continue;
        }
        
    	//swap(d,i-1,j - 'a',k-1,l - 'a');
    	if(Playersturn(d, player, i-1,j - 'a',k-1,l - 'a') == 0){
    	    break;
    	}
    	printf("Illegal move, try again\n");
    }
}

int checkJump(int d[ROWS][COLS], int player,int rr,int cc){
	if(checkMove(d, player, rr, cc, rr+2, cc-2) == 0){
		return 0;
	}
	if(checkMove(d, player, rr, cc, rr+2, cc+2) == 0){
		return 0;
	}
	if(checkMove(d, player, rr, cc, rr-2, cc-2) == 0){
		return 0;
	}
	if(checkMove(d, player, rr, cc, rr-2, cc+2) == 0){
		return 0;
	}
	
	return -1;
}



//Returns an array of the available moves.
vector<move> cpuCheck(int d[ROWS][COLS], int player){
	int rr, cc;
	int i=0;
	char ch;
	int num=0;
	vector<move> moveList;
	move current(rr,cc,rr,cc);
	canJump = 0;
	
	for(rr=0; rr<ROWS; ++rr){
		for(cc=0; cc<COLS; ++cc){
			ch = (char)cc+97;
			//Jumping moves
			if(checkMove(d, player, rr, cc, rr+2, cc-2) == 0){
				canJump = 1;
				current.change((rr+1), (ch), (rr+3), (ch-2));
				moveList.push_back(current);
			}
			if(checkMove(d, player, rr, cc, rr+2, cc+2) == 0){
				canJump = 1;
				current.change(rr+1, ch,rr+3,ch+2);
				moveList.push_back(current);
			}
			if(checkMove(d, player, rr, cc, rr-2, cc-2) ==0){
				canJump = 1;
				current.change(rr+1,ch,rr-1,ch-2);
				moveList.push_back(current);
			}
			if(checkMove(d, player, rr, cc, rr-2, cc+2) ==0){
				canJump = 1;
				current.change(rr+1,ch,rr-1,ch+2);
				moveList.push_back(current);
			}
		}
	}
	
	for(rr=0; rr<ROWS; ++rr){
		for(cc=0; cc<COLS; ++cc){
			ch = (char)cc+97;
			//Normal moves
			if(checkMove(d, player, rr, cc, rr+1, cc-1) == 0){
				current.change(rr+1,ch,rr+2,ch-1);
				moveList.push_back(current);
			}
			if(checkMove(d, player, rr, cc, rr+1, cc+1) == 0){
				current.change(rr+1,ch,rr+2,ch+1);
				moveList.push_back(current);
			}
			if(checkMove(d, player, rr, cc, rr-1, cc-1) == 0){
				current.change(rr+1,ch,rr,ch-1);
				moveList.push_back(current);
			}
			if(checkMove(d, player, rr, cc, rr-1, cc+1) == 0){
				current.change(rr+1,ch,rr,ch+1);
				moveList.push_back(current);
			}
		}
	}
	
	//cout<< "Moves: "<<moveList.size()<<endl;
	return moveList;
}

int legalMoves(int player, int d[ROWS][COLS]){
	int rr, cc;
	char ch;
	int num=0;
	int i;
	
	vector<move> moves = cpuCheck(d, player);
	
	if(moves.size()!=0){
		cout<< "\n\nLegal Moves: "<<endl;
		
		for(i=0; i<moves.size(); i++){
			//ch = (char)moves.j+97;
			printf("( %d%c, %d%c )\n",moves[i].i,moves[i].j,moves[i].k,moves[i].l);
			++num;
		}
	}
	
	return num;
}

void printDisplayFancy(int d[ROWS][COLS])
{
	Color::Modifier green(Color::FG_GREEN);
	Color::Modifier blue(Color::FG_BLUE);
    Color::Modifier def(Color::FG_DEFAULT);
    
    int rr, cc, pp;
    
    printf("  +-------+-------+-------+-------+-------+-------+-------+-------+\n");
    printf("  |       |       |       |       |       |       |       |       |\n");
    
    for (rr=0; rr<ROWS; ++rr)
    {
        printf("%d |", rr+1);
        for (cc=0; cc<COLS; ++cc)
        {
            //printf(" %c |", value2symbol(d[rr][cc]) );
            
            if(value2symbol(d[rr][cc])=='$'){
            	cout<< green << "   S   " << def << "|";
            }
            else if(value2symbol(d[rr][cc])=='O'){
            	cout<< blue << "   O   " << def << "|";
            }
            else if(value2symbol(d[rr][cc])=='&'){
            	cout<< green << "   &   " << def << "|";
            }
            else if(value2symbol(d[rr][cc])=='@'){
            	cout<< blue << "   @   " << def << "|";
            }
            else{
            	cout<<"   "<< value2symbol(d[rr][cc])<<"   |";
            };
            
        }
        printf("\n");
        printf("  |       |       |       |       |       |       |       |       |\n");
        printf("  +-------+-------+-------+-------+-------+-------+-------+-------+\n");
        if(rr!=7){
        	printf("  |       |       |       |       |       |       |       |       |\n");
        }
    }
    
    printf("     a       b       c       d       e       f       g       h\n");
}

void swap(int d[ROWS][COLS], int i, int j, int k, int l)
{
    int temp;
    
    //printf("SWAP: %d,%d to %d,%d\n", i, j, k, l);
    
    if(k==7 && (d[i][j]==2 || d[i][j]==4)){
    	d[i][j] = d[k][l];
    
    	d[k][l] = REDKING;
    }
    else if(k==0 && (d[i][j]==3 || d[i][j]==5)){
    	d[i][j] = d[k][l];
    
    	d[k][l] = BLACKKING;
    }
    else{
    temp = d[i][j];
    
    d[i][j] = d[k][l];
    
    d[k][l] = temp;
	};

}

char value2symbol(int i) 
{
	

    switch(i)
    {
	  	case 0:
		        return '#';
		case 1:
		        return ' ';  
		case 2:
		        return '$';
		case 3:
		        return 'O';
		case 4:
		        return '&';
		case 5:
		        return '@';
    }
    return ('?');
}


int testMove(int d[ROWS][COLS], int player,int i,int j,int k,int l, clock_t start, double limit)
{
    int jmp_r;
    int jmp_c;
    
    // check if it's a regular move
	if(i - k == -1 || i - k == 1){
		if(j - l == 1 || j - l == -1){
			if(canJump==0){
				swap(d,i,j,k,l);
				return 0;
			}
		}
	}

	if(i - k == -2 || i - k == 2){
	    if(j - l == -2 || j - l == 2){
	        // check if the jump position is enemy
	        if(i < k){ // move down
	            jmp_r = i + 1;
	        } else { // move up
	            jmp_r = i - 1;
	        }
	        if(j < l){ // move down
	            jmp_c = j + 1;
	        } else { // move up
	            jmp_c = j - 1;
	        }
	        
	        // we are sure the move is valid:
	        d[jmp_r][jmp_c] = 1;
	        swap(d,i,j,k,l);
	        
	        double end= (clock() - start) / CLOCKS_PER_SEC;

	        while(checkJump(d, player, k, l)==0 && end<limit){
	        	
	        	
	        	//printDisplayFancy(d);
			    testCPU(d, player, start, limit);
 
	        }

	        	
	        return 0;
	        
	    }
	}
    return -1;
    
}



int checkMove(int d[ROWS][COLS], int player,int i,int j,int k,int l)
{
    int jmp_r;
    int jmp_c;
    
    
    if(i < 0 && ROWS <= i){ // keeping in bounds
        return -1;
    }
    if(j < 0 && COLS <= j){
        return -1;
    }
        
    if(k < 0 && ROWS <= k){
        return -1;
        
    }
    if(l < 0 && COLS<= l){
        return -1;
    }
        
    // check player is moving his own piece.
    if(player == RED){
        if(d[i][j] != 2 && d[i][j] != 4){
            return -1;
        }
    } else { //BLACK
        if(d[i][j] != 3 && d[i][j] != 5){
            return -1;
        }
    }
    
    //make sure they are jumping to a empty loacation
    if(d[k][l] != EMPTY){
        return -1;
    }
    
    
    
    
    
    // make sure the vertical direction of the move is not illegaldef
    if(player == RED){
        //make sure he moves down
        if(i >= k &&  d[i][j]==2){
            return -1;
        }
    } else { //BLACK
        // make sure player moves up
        if(i <= k &&  d[i][j]==3){
            return -1;
        }
    }
    
    
    // check if it's a regular move
	if(i - k == -1 || i - k == 1){
	    if(j - l == 1 || j - l == -1){
	    	if(canJump==0){
	        	return 0;
	        }
	        else{
	        	return -1;
	        }
	    }
	}
    
    //check if it's a jump move
    if(i - k == -2 || i - k == 2){
        if(j - l == -2 || j - l == 2){
            // check if the jump position is enemy
            if(i < k){ // move down
                jmp_r = i + 1;
            } else { // move up
                jmp_r = i - 1;
            }
            if(j < l){ // move down
                jmp_c = j + 1;
            } else { // move up
                jmp_c = j - 1;
            }
            
            if(isRed(player) && !(isBlack(d[jmp_r][jmp_c])))
            {
                return -1;
            }
            if(isBlack(player) && !(isRed(d[jmp_r][jmp_c]))){
                return -1;
            }
            
            // we are sure the move is valid:
            return 0;
        }
    }
  
    return -1;
    
}

int Playersturn(int d[ROWS][COLS], int player,int i,int j,int k,int l)
{
    int jmp_r;
    int jmp_c;
    int m;
    
    int ii, kk;
    char jj, ll;
    
    if(player == RED){
        //printf("RED move from %d,%d to %d,%d\n", i, j, k, l);
    } else {
        //printf("BLACK move from %d,%d to %d,%d\n", i, j, k, l);
    }
    
    if(i < 0 && ROWS <= i){ // keeping in bounds
        printf("i is out of bounds\n");
        return -1;
    }
    if(j < 0 && COLS <= j){
        printf("j is out of bound\n");
        return -1;
    }
        
    if(k < 0 && ROWS <= k){
        printf("k is out of bounds\n");
        return -1;int r,c;
	int pr, pb;
	int i, k;
	char j, l;
        
    }
    if(l < 0 && COLS<= l){
        printf("l is out of bounds\n");
        return -1;
    }
        
    // check player is moving his own piece.
    if(player == RED){
        if(d[i][j] != 2 && d[i][j] != 4){
            printf("move your own piece!\n");
            return -1;
        }
    } else { //BLACK
        if(d[i][j] != 3 && d[i][j] != 5){
            printf("move your own piece!\n");
            return -1;
        }
    }
    
    //make sure they are umping to a empty loacation
    if(d[k][l] != EMPTY){
        printf("You must move to a empty location\n");
        return -1;
    }
    
    
    
    
    
    // make sure the vertical direction of the move is not illegal
    if(player == RED){
        //make sure he moves down
        if(i >= k &&  d[i][j]==2){
            printf("Green pawn must move down\n");
            return -1;
        }
    } else { //BLACK
        // make sure player moves up
        if(i <= k && d[i][j]==3){
            printf("Blue pawn must move up\n");   
            return -1;
        }
    }
    
    // check if it's a regular move
	if(i - k == -1 || i - k == 1){
		if(j - l == 1 || j - l == -1){
			if(canJump==0){
				swap(d,i,j,k,l);
				return 0;
			}
			else{
				cout<<"You have to capture\n"<<endl;
				return -1;
			}
		}
	}
    
    
    //check if it's a jump move
	if(i - k == -2 || i - k == 2){
	    if(j - l == -2 || j - l == 2){
	        // check if the jump position is enemy
	        if(i < k){ // move down
	            jmp_r = i + 1;
	        } else { // move up
	            jmp_r = i - 1;
	        }
	        if(j < l){ // move down
	            jmp_c = j + 1;
	        } else { // move up
	            jmp_c = j - 1;
	        }
	        
	        if(isRed(player) && !(isBlack(d[jmp_r][jmp_c])))
	        {
	            printf("you can only jump over an enemy player\n");
	            return -1;
	        }
	        if(isBlack(player) && !(isRed(d[jmp_r][jmp_c]))){
	            printf("you can only jump over an enemy player\n");
	            return -1;
	        }
	        
	        // we are sure the move is valid:
	        d[jmp_r][jmp_c] = 1;
	        swap(d,i,j,k,l);
	        
	        
	        while(checkJump(d, player, k, l)==0){
	        
	        	if(!isCPU(player)){
	        		cout<< "\n\nYou can still jump\n\n"<<endl;
			    	//printDisplayFancy(d);
			    	
			    	vector<move> moves = cpuCheck(d, player);
			    	
			    	cout<<"Legal Moves:\n";
			    	for(m = 0; m < moves.size(); m++){
			    		cout<<"( "<<moves[m].i<<moves[m].j<<", "<<moves[m].k<<moves[m].l<<" )\n";
			    	}
			    	
			    	cout<< "Play again: "<<endl;
			    	
			    	if(!isCPU(player)){
			    		playerMove(d, player);
			    	}
			    	else{
			    		cpuMove(d, player);
			    	}
			    }
			    else{
			    	cpuMove(d, player);
			    }
	        }
	        	
	        return 0;
	        
	    }
	}
    
    printf("You can only move diagnally\n");
    return -1;
    
}

int main()
{
	
	int r,c;
	int pr, pb;
	int i, k;
	char j, l;
	char answer;
	char load;
	string save;
	int currplay=0;
	int d[ROWS][COLS];
	
	cout<<"+-----------------------------------------------------------------------------+\n";
	cout<<"|                               Sebastien Charles                             |\n";
	cout<<"|                        ECE 469 - Artificial Intelligence                    |\n";
	cout<<"|                                    Prof. Sable                              |\n";
	cout<<"+-----------------------------------------------------------------------------+\n";
		
	
	while(1){	
		cout<<"Do you want to upload a previously saved game?(y/n)"<<endl;
		cin>>load;
	
		if(load=='y' || load=='Y'){
			cout<<"Enter the name of the save file"<<endl;
			cin>>save;
			int b[8][4];
			string line;
			int ii = 0;
			int jj = 0;
			int kk = 0;

			ifstream myfile (save.c_str());
			if (myfile.is_open()){
				while ( getline (myfile,line) ){
					if(ii<=8){
						kk=0;
						for(jj=0; jj<line.size(); jj++){
							if(line[jj]!=' ' && line[jj]!='\n' && line[jj]!='\r'){
								b[ii][kk] = atoi(&line[jj]) + 1;
								kk++;
							}
						}
					}
					if(ii==8){
						currplay = atoi(&line[0]) +1;
					}
					if(ii==9){
						tlimit = atoi(&line[0]);
					}
					ii++;
				}
				myfile.close();

			}
			int c[8][8]={
				{0,b[0][0],0,b[0][1],0,b[0][2],0,b[0][3]},

				{b[1][0],0,b[1][1],0,b[1][2],0,b[1][3],0}, 

				{0,b[2][0],0,b[2][1],0,b[2][2],0,b[2][3]}, 

				{b[3][0],0,b[3][1],0,b[3][2],0,b[3][3],0}, 

				{0,b[4][0],0,b[4][1],0,b[4][2],0,b[4][3]}, 

				{b[5][0],0,b[5][1],0,b[5][2],0,b[5][3],0},

				{0,b[6][0],0,b[6][1],0,b[6][2],0,b[6][3]},

				{b[7][0],0,b[7][1],0,b[7][2],0,b[7][3],0}};

			copy(&c[0][0], &c[0][0]+8*8,&d[0][0]);
			break;
		}
		else if(load=='n' || load=='N'){
			break;
		}
		else{
		cout<<"Invalid answer\n";
		cin.clear();
		while (cin.get() != '\n'); // clear buffer
		}
	};
	
	while(1){	
		cout<<"Is green going to be a cpu?(y/n)"<<endl;
		cin>>answer;
	
		if(answer=='y' || answer=='Y'){
			red = 1;
			break;
		}
		else if(answer=='n' || answer=='N'){
			red = 0;
			break;
		}
		else{
		cout<<"Invalid answer\n";
		cin.clear();
		while (cin.get() != '\n'); // clear buffer
		}
	};
	
	while(1){	
		cout<<"Is blue going to be a cpu?(y/n)"<<endl;
		cin>>answer;
	
		if(answer=='y' || answer=='Y'){
			black = 1;
			break;
		}
		else if(answer=='n' || answer=='N'){
			black = 0;
			break;
		}
		else{
		cout<<"Invalid answer\n";
		cin.clear();
		while (cin.get() != '\n'); // clear buffer
		}
	}
	
	if(red==1 || black==1){
		while(1){	
			cout<<"Enter the number of seconds per turn (between 5 and 60):"<<endl;
			cin>>tlimit;
		
			if(tlimit>60 || tlimit<1){
				cout<<"Invalid answer\n";
				cin.clear();
				while (cin.get() != '\n'); // clear buffer
			}
			else{
			break;
			}
		}
	}
		
	if(currplay==0){
		int c[8][8]={
			{0,2,0,2,0,2,0,2},
			{2,0,2,0,2,0,2,0}, 
			{0,2,0,2,0,2,0,2}, 
			{1,0,1,0,1,0,1,0}, 
			{0,1,0,1,0,1,0,1}, 
			{3,0,3,0,3,0,3,0},
			{0,3,0,3,0,3,0,3},
			{3,0,3,0,3,0,3,0}};
		copy(&c[0][0], &c[0][0]+8*8,&d[0][0]);
	}
	
	

    for(;;){
        // display board
        printDisplayFancy(d);
        // red turn
        canJump=0;
		if(currplay!=3){
			if(legalMoves(RED, d)==0){
				cout<< "Match is over, Blue won!!!"<<endl;
				break;
			}
			
			if(red==0||black==0){
				cout<<"\nEnter the coordinates of the starting position followed by the coordinates of the final position to move.\n";
			}
			
			if(red==0){
				playerMove(d, RED);
			}
			else{
				cpuMove(d, RED);
			}
			
			//check for win
			
			//display board
			printDisplayFancy(d);
			//black turn
        }
		currplay==0;
		
        canJump=0;
        if(legalMoves(BLACK, d)==0){
		    cout<< "Match is over, Green won!!!"<<endl;
		    break;
        }
        
        if(red==0||black==0){
			cout<<"\nEnter the coordinates of the starting position followed by the coordinates of the final position to move.\n";
		}
		
        if(black==0){
        	playerMove(d, BLACK);
        }
        else{
        	cpuMove(d, BLACK);
        }
        
        //check for win
        
    }
    return 0;
}
