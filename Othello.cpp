#include <iostream>
#include <cstdlib> 
#include <conio.h>
using namespace std;

#define BOARD_WIDTH 8
#define BOARD_HEIGHT 8

int cursorX,cursorY;
int cells[BOARD_WIDTH][BOARD_HEIGHT];
int cellsBackup[BOARD_WIDTH][BOARD_HEIGHT];

int turn;		//Whose turn

char colorNames[][5 + 1]{
	"BLACK",
	"WHITE"
};

bool cantPut = false;
bool canBack = false;
bool debug = false;

int directions[][2]{
	//[x,y]
	{0,-1},		//DIRECTION_UP
	{1,-1},		//DIRECTION_UP_RIGHT
	{-1,-1},	//DIRECTION_UP_LEFT
	{1,0},		//DIRECTION_RIGHT
	{-1,0},		//DIRECTION_LEFT
	{0,1},		//DIRECTION_DOWN
	{1,1},		//DIRECTION_DOWN_RIGHT
	{-1,1}		//DIRECTION_DOWN_LEFT
};

enum{
	COLOR_NONE = 0,
	COLOR_BLACK = 1,
	COLOR_WHITE = -1
};

enum{
	DIRECTION_UP,
	DIRECTION_UP_RIGHT,
	DIRECTION_UP_LEFT,
	DIRECTION_RIGHT,
	DIRECTION_LEFT,
	DIRECTION_DOWN,
	DIRECTION_DOWN_RIGHT,
	DIRECTION_DOWN_LEFT,
	DIRECTION_MAX,
};

int reverseSide(int ourSide){
	if(ourSide == COLOR_WHITE){
		return COLOR_BLACK;
	}
	else{
		return COLOR_WHITE;
	}	
}

bool chkCanPut(int _x,int _y,int _turn,bool reverse){
	if(cells[_y][_x] != COLOR_NONE){return false;}
	for(int i=0;i<DIRECTION_MAX;i++){			
		int x = _x,y = _y;
		x += directions[i][0];
		y += directions[i][1];
		if(cells[y][x] != reverseSide(_turn)){
			continue;
		}
		while(true){
			x += directions[i][0];
			y += directions[i][1];
			
			if(x < 0 || x >= BOARD_WIDTH || y < 0 || y >= BOARD_HEIGHT) break;
			if(cells[y][x] == COLOR_NONE) break;
			if(cells[y][x] == _turn){
				//cout << "target:" << x << ',' << y << endl;
				//system("PAUSE");
				if(!reverse){
					return true;
				}
				
				//cout << "direction=" << directions[i][0] << ',' << directions[i][1] << endl;
				//system("PAUSE");
				int x2 = _x,y2 = _y;
				while(true){
					cells[y2][x2] = _turn;
					if(debug){
						cout << x2+1 << "," << y2+1 << endl;	
					}
					x2 += directions[i][0];
					y2 += directions[i][1];
					
					if(x2 == x && y2 == y){
						if(debug){
							system("PAUSE");
						}
						break;	
					}
				}
				break;
			}
		}
	}
	return false;
};

bool chkAllCanPut(int _turn){
	for(int y=0;y<BOARD_HEIGHT;y++){
		for(int x=0;x<BOARD_WIDTH;x++){
			if(cells[y][x] == COLOR_NONE){
				if(chkCanPut(x,y,_turn,false)){
					return true;
				}
			}
		}
	}
	return false;
}

struct score{
	int black,white;
};

void countScore(struct score *ptr){
	int black = 0,white = 0;
	for(int y = 0;y < BOARD_HEIGHT;y++){
		for(int x = 0;x < BOARD_HEIGHT;x++){
			if(cells[y][x] ==  COLOR_BLACK)
				black++;
			if(cells[y][x] ==  COLOR_WHITE)
				white++;
		}
	}
	cout << "Black : White" << endl;
	cout << black << ":" << white << endl;
	
	ptr->black = black;
	ptr->white = white;
}

void InitBoard(void){
	for(int y = 0;y < BOARD_HEIGHT;y++){
		for(int x = 0;x < BOARD_HEIGHT;x++){
			cells[y][x] = COLOR_NONE;
		}
	}
	cells[3][3] = cells[4][4] = COLOR_WHITE;
	cells[3][4] = cells[4][3] = COLOR_BLACK;
	
	turn = COLOR_BLACK;		//BLACK turn first
};

int main(void){
	//Initial Board    
	InitBoard();
	score score;
	
	while(true){
		system("cls");		//Clear Window
		
		//print BOARD
		for(int y=0;y<BOARD_HEIGHT;y++){
			for(int x=0;x<BOARD_WIDTH;x++){
				if(x == cursorX && y == cursorY){
					cout << "¡·";
				}
				else{
					switch(cells[y][x]){			
						case COLOR_NONE:
							cout << "¡D";
							break;
						case COLOR_BLACK:
							cout << "¡´";
							break;
						case COLOR_WHITE:
							cout << "¡³";
							break;
					}
				}
			}
			cout << endl;
		}
		countScore(&score);
		
		if(cantPut){
			cout << "You can't put here." << endl;
		}
		
		if(chkAllCanPut(turn) == false){
			cout << (turn ==  COLOR_BLACK ? colorNames[0] : colorNames[1]) << " can't put anywhere. Change turn." << endl;
			turn = reverseSide(turn);
			system("PAUSE");
		}
		cout << (turn ==  COLOR_BLACK ? colorNames[0] : colorNames[1]) << " turn." << endl;
		
		//Operating
		switch(getch()){
			case 'w': 
				if(cursorY != 0)
					cursorY--;
				break;
			case 's': 
				if(cursorY < BOARD_HEIGHT - 1)
					cursorY++;
				break;
			case 'a': 
				if(cursorX != 0)
					cursorX--;
				break;
			case 'd': 
				if(cursorX < BOARD_HEIGHT - 1)
					cursorX++;
				break;
			case 13:	//KB Enter Code
				if(chkCanPut(cursorX,cursorY,turn,false)){
					for(int y=0;y<BOARD_HEIGHT;y++){
						for(int x=0;x<BOARD_WIDTH;x++){
							cellsBackup[y][x] = cells[y][x];
						}
					}
					chkCanPut(cursorX,cursorY,turn,true);		//Reverse Othello
					
					cantPut = false;
					turn = reverseSide(turn);
				}
				else{
					cantPut = true;
				}
				canBack = true;
				break;
			case 8:		//Back
				if(canBack){
					for(int y=0;y<BOARD_HEIGHT;y++){
						for(int x=0;x<BOARD_WIDTH;x++){
							cells[y][x] = cellsBackup[y][x];
						}
					}
					turn = reverseSide(turn);
					canBack = false;
				}
				break;
			case 96:		//Debug
				debug = debug ? false :true;
				break;
		}
		
		if(chkAllCanPut(COLOR_BLACK) == false && chkAllCanPut(COLOR_WHITE) == false){
			cout << "You can't put anywhere.'" << endl;
			system("PAUSE");
			system("cls");		//Clear Window
			
			for(int y=0;y<BOARD_HEIGHT;y++){
				for(int x=0;x<BOARD_WIDTH;x++){
					switch(cells[y][x]){
						case COLOR_NONE:
							cout << "¡D";
							break;
						case COLOR_BLACK:
							cout << "¡´";
							break;
						case COLOR_WHITE:
							cout << "¡³";
							break;
					}
				}
				cout << endl;
			}
			
			cout << "GAME SET" << endl;
			cout << "Black : White" << endl;
			cout << "  " << score.black << " :   " << score.white << endl;
			system("PAUSE");
			
			if(score.black == score.white){
				cout << "¥­¤â" << endl;
			}
			else{
				if(score.black > score.white)
					cout << "BLACK WIN" << endl;
				else
					cout << "WHITE WIN" << endl;
			}
			system("PAUSE");
			break;
		}
	}
	cout << "GAME OVER!" << endl;
	return 0;
}
