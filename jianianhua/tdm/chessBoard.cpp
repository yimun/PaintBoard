#include "stdafx.h"

#include "chessBoard.h"


chessBoard::chessBoard(){

	gridInnerSize = 1;
	color=RGB(255,0,0);
	size = I_PANEL_WIDTH;

	buffQueue = (p_queue*)malloc(sizeof(p_queue));
	buffQueue->next = NULL;
}

void chessBoard::setHDC(HWND hWnd){
	hdc = ::GetDC(hWnd);
	GetWindowRect(hWnd,&windowRect);
}

void chessBoard::initBoard(header* head){

	queue = head;
	int W_width = windowRect.Width();//宽度
	int W_height = windowRect.Height();//高度
	boardStart_x = (W_width - size*2) / 4 * 3 + size;
	previewStart_x = (W_width - size*2) / 4;
	previewStart_y = boardStart_y = (W_height - size) / 2;
	int i=0;
	int length_x = size*gridInnerSize;
	int length_y = size*gridInnerSize;
	

	// 绘图边框
	Rectangle(hdc,boardStart_x,boardStart_y,boardStart_x +size,boardStart_y+size);

	// 预览边框
	Rectangle(hdc,previewStart_x,previewStart_y,previewStart_x +size,previewStart_y+size);

	// 预览图
	p_queue* p = queue->front;
	int x,y;
	while(p){
		x = I_PANEL_WIDTH - p->x + previewStart_x;
		y = p->y + previewStart_y;
		SetPixelV(hdc,x,y,RGB(0,0,0));
		p = p->next;
	}
}


// 填充动态画板上的点,只限绘图区
void chessBoard::cover(int x, int y){
	int _x = I_PANEL_WIDTH - x + boardStart_x;
	int _y = y + boardStart_y;
	::SetPixelV(hdc,_x,_y,color);

	p_queue *p = (p_queue*)malloc(sizeof(p_queue));
	p->x = x;
	p->y = y;
	p->next = buffQueue->next;
	buffQueue->next = p;
}

// 开始通信前清除模拟时的图像
void chessBoard::coverOld()
{
	clearBuff();
	rePaint();
}

// 窗口重获焦点时重绘
void chessBoard::rePaint(void)
{
	initBoard(queue);
	p_queue* p = buffQueue->next;
	int x,y;
	while(p){
		int _x = I_PANEL_WIDTH - p->x + boardStart_x;
		int _y = p->y + boardStart_y;
		::SetPixelV(hdc,_x,_y,color);
		p = p->next;
	}

}


void chessBoard::clearBuff(void)
{
	p_queue *temp,*p = buffQueue->next;
	while(p){
		temp = p->next;
		free(p);
		p = temp;
	}
	buffQueue->next = NULL;
}
