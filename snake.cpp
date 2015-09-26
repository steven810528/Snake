#include "snake.h"
#include <curses.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <queue>

using namespace std;
Snake::Snake(int x, int y)
{
    head = new Node;
    head->x = x;
    head->y = y;
    head->prevNode = NULL;
    tail = head;
}
Snake::Snake(int x, int y, Direction direction, int targetX, int targetY) {
	// Create snake's head at the specified position
	head = new Node;
	head->x = x;
	head->y = y;
	head->prevNode = NULL;
	tail = head;
	dir = direction;
}/*
Snake::Snake(int x, int y, Direction direction) {
    // Create snake's head at the specified position
    head = new Node;
    head->x = x;
    head->y = y;
    head->prevNode = NULL;
    tail = head;
    dir = direction;
}*/
Snake::~Snake() {
	while(tail != NULL) {
		Node* temp = tail;
		tail = tail->prevNode;
		delete temp;
	}
}
void Node::operator=(Node* n)
{
    //Node* tmp=new Node;
    this->x=n->x;
    this->y=n->y;
    this->survTime=n->survTime;
    //return tmp;
    
}
void Snake::operator=(Snake* s)
{
    dir=s->dir;
    Node* index = s->tail;
    Node* t=new Node;
    t->operator=(index);
    t->prevNode=NULL;
    this->InsertAtHead(t);
    this->tail=t;
    this->head=t;
    
    while(index->prevNode!=NULL)
    {
        Node* n = new Node;
        n->operator=(index);
        n->prevNode=NULL;
        this->InsertAtHead(n);
        index=index->prevNode;
    }
    Node* n= new Node;
    n->operator=(index);
    this->InsertAtHead(n);
    
}
void Snake::Draw() {
	Node* temp = tail;
	
	while(temp != NULL) {
		mvaddch(temp->y, temp->x, '*');
		temp = temp->prevNode;
	}
    //mvaddch(temp->y, temp->x, '@');
}
void Snake::RemoveAtTail() {
    if(this->tail->prevNode!=NULL)
    {
        Node* temp = this->tail;
        this->tail = this->tail->prevNode;
        //delete temp;
    }
}
void Snake::InsertAtHead(Node* newHead) {
	head->prevNode = newHead;
	head = newHead;
}
void Snake::SetDir(Direction new_dir) {
	if (!(new_dir == UP && dir == DOWN) && !(new_dir == DOWN && dir == UP) &&
		!(new_dir == LEFT && dir == RIGHT) && !(new_dir == RIGHT && dir == LEFT)) {
		dir = new_dir;
	}
}
bool Snake::SetTarget(int x, int y){
    targetX=x;
    targetY=y;
    Node* n = new Node;
    n=this->tail;
    while(n->prevNode!=NULL)
    {
        if(n->x==x && n->y==y)
            return false;
        n=n->prevNode;
    }
    if(n->x==x && n->y==y)
        return false;
    else return true;
}
void Snake::Move() {
	Node* new_head = new Node;
	
	*new_head = *head; // copy the current head

	// Compute position of the new head
	switch(dir) {
	case UP:
		new_head->y--;
		break;
	case DOWN:
		new_head->y++;
		break;
	case LEFT:
		new_head->x--;
		break;
	case RIGHT:
		new_head->x++;
		break;
	}
	
	// Draw the new head
	InsertAtHead(new_head);
	mvaddch(new_head->y, new_head->x, '*');
	
	// Clear the tail if we haven't reached the target
	if (targetX != new_head->x || targetY != new_head->y ) {
		mvaddch(tail->y, tail->x, ' ');
		RemoveAtTail();
	}
    
}
void Snake::ghostMove(queue<int>* sim) {
    int dir;
    queue<int>* s=sim;
    for(int i=0;i<s->size();i++)
    {
        dir=s->front();
        Node* new_head = new Node;
        new_head->x=head->x;
        new_head->y=head->y;
        //new_head->operator=(this->head);
        switch(dir) {
            case 259://UP
                this->SetDir(UP);
                new_head->y--;
                break;
            case 258://DOWN
                this->SetDir(DOWN);
                new_head->y++;
                break;
            case 260://LEFT
                this->SetDir(LEFT);
                new_head->x--;
                break;
            case 261://RIGHT
                this->SetDir(RIGHT);
                new_head->x++;
                break;
        }
        this->InsertAtHead(new_head);
        this->RemoveAtTail();
        s->pop();
    }
    

}
bool Snake::ghostMove(int dir ,int maxX,int maxY) {
    //Snake* tmp = new Snake;
    //tmp->operator=(this);
    
    //避免回頭問題
    if(this->dir==UP && dir==258){
        return false;
    }
    else if(this->dir==DOWN && dir==259){
        return false;
    }
    else if(this->dir==RIGHT && dir==260){
        return false;
    }
    else if(this->dir==LEFT && dir==261){
        return false;
    }
    else{
        Node* new_head = new Node;
        new_head->x=head->x;
        new_head->y=head->y;
        new_head->prevNode=NULL;
        //new_head->operator=(this->head);
        switch(dir) {
            case 259://UP
                this->SetDir(UP);
                new_head->y--;
                break;
            case 258://DOWN
                this->SetDir(DOWN);
                new_head->y++;
                break;
            case 260://LEFT
                this->SetDir(LEFT);
                new_head->x--;
                break;
            case 261://RIGHT
                this->SetDir(RIGHT);
                new_head->x++;
                break;
        }
        
        this->InsertAtHead(new_head);
        this->RemoveAtTail();
        if(this->CurX()<0||this->CurY()<0||this->CurY()>=maxY||this->CurX()>=maxX|| this->checkSelf()==false)
        {
            //this->operator=(tmp);
            return false;
        }
        else
            return true;
        
        /*
        if(new_head->x<0||new_head->y<0||new_head->y>=maxY||new_head->x>=maxX|| this->checkSelf()==false)
        {
            //this->operator=(tmp);
            return false;
        }
        else
            return true;*/
    }
}
void Snake::calSurvTime(){
    int a=1;
    Node* temp = this->tail;
    while(temp->prevNode!=NULL)
    {
        temp->survTime=a;
        a++;
    }
}
int Snake::getTarX(){
    return this->targetX;
}
int Snake::getTarY(){
    return this->targetY;
}
int Snake::getLength(){
    int x=1;
    Node* n =this->tail;
    while(n->prevNode!=NULL)
    {
        x=x+1;
        n=n->prevNode;
    }
    return x;
}
void Snake::Save(const char* file_name) {
	ofstream file(file_name);

	if (!file) return;
       	
	// Save moving direction
	file << dir << endl;

	// Save snake body locations
	Node* temp = tail;
	while(temp != NULL) {
		file << temp->x << ' ' << temp->y << endl;
		temp = temp->prevNode;
	}

	file.close();
}
Snake* Snake::Load(const char* file_name) {
	Snake* snake = NULL;

	ifstream file(file_name);

	if (!file) return NULL;
	
	int dir, x, y;

	// Create a new snake
	snake = new Snake;
	snake->head = snake->tail = NULL;

	// Load moving direction
	file >> dir;
	snake->dir = (Direction) dir;

	// Load the body
	while(file >> x >> y) {
		Node* node = new Node;
		node->x = x;
		node->y = y;
		node->prevNode = NULL;
		mvaddch(node->y, node->x, '*');
		if (snake->head == NULL) {
			snake->head = node;
			snake->tail = node;
		} else {
			snake->InsertAtHead(node);
		}
	}

	return snake;
}
/*bool Snake::isInBody(Node* n)
{
    Node* index=this->tail;
    while(index->prevNode!=NULL)
    {
        
    }
}*/
bool Snake::checkSelf()
{
    if(this->head!=this->tail){
        this->head->prevNode=NULL;
        int x =this->head->x;
        int y =this->head->y;
        Node*n =this->tail;
        while(n->prevNode!=NULL)
        {
            if(x==n->x && y==n->y)
                return false;
            n=n->prevNode;
        }
        
    }
    else if (this->head->x==this->tail->x && this->head->y==this->tail->y&& this->getLength()>3)
        return false;
    return true;
}


