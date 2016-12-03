#pragma once

#include<Windows.h>
#include "stdafx.h"
#include "imageAnalyse.h"

class chessBoard
{
private:
	int boardStart_x ,boardStart_y;
	int previewStart_x,previewStart_y;
	int gridInnerSize;
	int size;
	header *queue;
	HDC hdc;
	CRect windowRect;

	COLORREF color;
public:
	chessBoard();
	void cover(int x,int y);
	void setHDC(HWND hWnd);
	void initBoard(header* head);
	void simulate();
	void coverOld();
	// 窗口重获焦点时重绘
	void rePaint(void);
private:
	p_queue *buffQueue;
public:
	void clearBuff(void);
};