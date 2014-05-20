#include "stdafx.h"

#include "chessBoard.h"


chessBoard::chessBoard(){
	boardStart_x = 200;
	boardStart_y = 0;
	gridInnerSize = 1;
	color=RGB(255,0,0);
}

void chessBoard::setHDC(HWND hWnd){
	hdc = ::GetDC(hWnd);
}

void chessBoard::setHeight(int height){
	chessBoard::height = height;
}

void chessBoard::setWidth(int width){
	chessBoard::width = width;
}

void chessBoard::initBoard(){
	boardStart_x = 200 + width + 20;
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
}

void chessBoard::cover(int x, int y){
	int _x = x + boardStart_x;
	int _y = y + boardStart_y;
	::SetPixelV(hdc,_x,_y,color);
}