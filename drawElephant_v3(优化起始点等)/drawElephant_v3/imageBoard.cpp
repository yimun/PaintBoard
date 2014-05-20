#include "stdafx.h"
#include "imageBoard.h"


imageBoard::imageBoard(void):chessBoard()
{
}


imageBoard::~imageBoard(void)
{
}

void imageBoard::showBoard(header* head)
{
	color=RGB(0,0,0);
	boardStart_x = 200;
	int i=0;
	int length_x = width*gridInnerSize;
	int length_y = height*gridInnerSize;
	//¶¥²¿
	MoveToEx(hdc,boardStart_x,boardStart_y,NULL);
	LineTo(hdc,boardStart_x+length_x,boardStart_y);
	//µ×²¿
	MoveToEx(hdc,boardStart_x,boardStart_y+length_y,NULL);
	LineTo(hdc,boardStart_x+length_x,boardStart_y+length_y);
	
	//×ó±ß
	MoveToEx(hdc,boardStart_x,boardStart_y,NULL);
	LineTo(hdc,boardStart_x,boardStart_y+length_y);
	//ÓÒ±ß
	MoveToEx(hdc,boardStart_x+length_x,boardStart_y,NULL);
	LineTo(hdc,boardStart_x+length_x,boardStart_y+length_y);

	queue* p = head->front;
	while(p){
		cover(p->x,p->y);
		p = p->next;
	}

}

