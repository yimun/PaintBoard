#include "stdafx.h"
#include "simuQueue.h"

simuQueue::simuQueue(){
	front = 0;
	tail = 0;
}

void simuQueue::add(int x, int y){
	simuBuf[front].x = x;
	simuBuf[front].y = y;
	front = (front++)%SIMU_SIZE;
}

node* simuQueue::getNode(){
	int index = tail;
	tail = (tail++)%SIMU_SIZE;
	return &simuBuf[index];
}

bool simuQueue::isEmpty(){
	if(front==tail)
		return true;
	return false;
}

int simuQueue::size(){
	return (front-tail+SIMU_SIZE)%SIMU_SIZE;
}

int simuQueue::getMod(int index){
	return 0;
}