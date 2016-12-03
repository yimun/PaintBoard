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
	int W_width = windowRect.Width();//���
	int W_height = windowRect.Height();//�߶�
	boardStart_x = (W_width - size*2) / 4 * 3 + size;
	previewStart_x = (W_width - size*2) / 4;
	previewStart_y = boardStart_y = (W_height - size) / 2;
	int i=0;
	int length_x = size*gridInnerSize;
	int length_y = size*gridInnerSize;
	

	// ��ͼ�߿�
	Rectangle(hdc,boardStart_x,boardStart_y,boardStart_x +size,boardStart_y+size);

	// Ԥ���߿�
	Rectangle(hdc,previewStart_x,previewStart_y,previewStart_x +size,previewStart_y+size);

	// Ԥ��ͼ
	p_queue* p = queue->front;
	int x,y;
	while(p){
		x = I_PANEL_WIDTH - p->x + previewStart_x;
		y = p->y + previewStart_y;
		SetPixelV(hdc,x,y,RGB(0,0,0));
		p = p->next;
	}
}


// ��䶯̬�����ϵĵ�,ֻ�޻�ͼ��
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

// ��ʼͨ��ǰ���ģ��ʱ��ͼ��
void chessBoard::coverOld()
{
	clearBuff();
	rePaint();
}

// �����ػ񽹵�ʱ�ػ�
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
