#ifndef NODE_H
#define NODE_H

#include "snake.h"

class Node {
	friend class Snake;
public:
    int x;
    int y;
    int survTime;
    void operator=(Node*);
    Node* prevNode;
private:
	//int x;
	//int y;
    
	
};

#endif
