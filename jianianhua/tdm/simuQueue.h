#pragma once
#include "stdlib.h"
#include "stdafx.h"

#define SIMU_SIZE 360

typedef struct node{
	int x;
	int y;
}node;

class simuQueue{
private:
	node simuBuf[360];
	int front;
	int tail;
	int getMod(int index);
public:
	simuQueue();
	void add(int x,int y);
	node* getNode();
	bool isEmpty();
	int size();
};