#include <curses.h>
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include "snake.h"
#include <queue>

using namespace std;

#include "ai.cpp"

int maxX;
int maxY;
int targetX;
int targetY;
int score=0;
int round=0;
//vector<int> time;
bool isAutoMode=true;
vector<string> r ;//log vector

void GenerateNewTarget() {

	targetX = rand() % maxX;
	targetY = rand() % maxY;
        
	// Should check to make sure the new target does not overlap with the snake body
	//mvaddch(targetY, targetX, '$');
}

//const char* FILE_NAME = "snake.txt";

int main() {
	int c = 0;

	initscr();
	cbreak();				// Disable buffering, making input available to the program immediately
    timeout(500);
	keypad(stdscr, TRUE);	// Enable input from arrow keys
	curs_set(0); 			// Hide cursor
	getmaxyx(stdscr, maxY, maxX);	// Get max X and Y of the terminal

	//Snake* snake = Snake::Load(FILE_NAME);
    Snake* snake;
	
    snake = new Snake(maxX-1, maxY-1, UP);
    
	GenerateNewTarget();
	while(snake->SetTarget(targetX, targetY)==false)
        GenerateNewTarget();
        mvaddch(targetY, targetX, '$');
    
	while (true) {
        c = getch();
        if(c==32)
        {
            GenerateNewTarget();
            while(snake->SetTarget(targetX, targetY)==false)
                GenerateNewTarget();
            mvaddch(targetY, targetX, '$');
             
            
        }
        //運行AI
        if(isAutoMode==true)
        {
            autoMove(snake,snake->getTarX(),snake->getTarY(),maxX,maxY);
            c=moveQueue.front();
            moveQueue.pop();
        }
		switch (c) {
		case KEY_UP:    //259
			snake->SetDir(UP);
			break;
		case KEY_DOWN: //258
			snake->SetDir(DOWN);
			break;
		case KEY_LEFT: //260
			snake->SetDir(LEFT);
			break;
		case KEY_RIGHT://261
			snake->SetDir(RIGHT);
			break;
		default:
			break;
		}
		
		if (c == 'q' || c == 'Q') {
			break;
		}

		snake->Move();
		if (snake->CurX() < 0 || snake->CurX() >= maxX
            || snake->CurY() < 0 || snake->CurY() >= maxY)
        {
            printLog(log);
            //int i=0;
            //while(i==0)//停留在結束畫面
            //{
                //cout<<endl;
            //}
            break;
        }
        round=round+1;
		if (snake->CurX() == targetX && snake->CurY() == targetY) {
			GenerateNewTarget();
            while(snake->SetTarget(targetX, targetY)==false)
                GenerateNewTarget();
            mvaddch(targetY, targetX, '$');
			//snake->SetTarget(targetX, targetY);
            score+=1;
            //time.push_back(round);
            r.push_back("Round="+int2str(round)+" mean="+dbl2str(static_cast<double>(round)/score));
            //round=0;
		}
        printLog(r,"round");

	}
	
	//snake->Save(FILE_NAME);
	delete snake;
	endwin();	// Restores the terminal after Curses activity
    cout<<"你的得分是："<<score<<endl;
}
