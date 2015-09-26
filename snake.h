#ifndef SNAKE_H
#define SNAKE_H

#include "node.h"
#include <queue>
using namespace std;

enum Direction{
		UP,DOWN,LEFT,RIGHT
};

class Snake {
public:
    Node* head;
    Node* tail;
    Direction dir;// Direction of the snake
    //Snake();
    Snake(int ,int );
    //Snake(int ,int ,Direction);
    Snake(int x = 1, int y = 1, Direction direction = RIGHT, int targetX = 0, int targetY = 0);
	~Snake();

	void SetDir(Direction new_dir);
    bool SetTarget(int x, int y) ;
	void Move();// Move the snake one step further toward the dir
    void ghostMove(queue<int>*);
    bool ghostMove(int,int,int);
    void Draw();
	void Save(const char* file_name);
	static Snake* Load(const char* file_name);

	int CurX() { return head->x; }
	int CurY() { return head->y; }
    void calSurvTime();
    int getTarX();
    int getTarY();
    int getLength();
    void operator=(Snake*);
    bool checkSelf();
    void RemoveAtTail();
    void InsertAtHead(Node* newHead);
private:
	

	
	
	int targetX;
	int targetY;
};
#endif
