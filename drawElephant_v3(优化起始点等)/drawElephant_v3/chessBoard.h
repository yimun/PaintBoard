#pragma once

#include<Windows.h>
#include "stdafx.h"

class chessBoard
{
protected:
	int boardStart_x ,boardStart_y;
	int gridInnerSize;
	int width;
	int height;
		
	HDC hdc;

	COLORREF color;
public:
	chessBoard();
	void setHeight(int height);
	void setWidth(int width);
	void cover(int x,int y);
	void setHDC(HWND hWnd);
	void initBoard();
	
};