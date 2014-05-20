#pragma once

#include "stdafx.h"
#include <stdlib.h>
#include <vector>

#include "cv.hpp"
#include "cxcore.hpp"
#include "opencv2\highgui\highgui.hpp"

#define SEND_BUF_SIZE 10

using namespace std;

typedef struct point{
	//对于data数组，1是黑，0是白；对于border数组，1是边缘，0不是
	int val;
	//是否访问 1是
	int flag;
	int x;
	int y;
	int z;
}point;

typedef struct queue{
	int x;
	int y;
	int z;
	queue *next;
}queue;

typedef struct header{
	struct queue* front;
	struct queue* tail;
}header;

typedef struct DblList{
	int x;
	int y;
	DblList* next;
	DblList* pre;
}DblLsit;

typedef struct DLheader{
	DblList* front;
	DblList* tail;
}DLheader;

typedef struct Edge Edge;
typedef struct Node Node;

/*struct Edge{
	DLheader* head;
	Node* next;
};*/

struct Edge{
	DLheader* head;
	Node* n1;
	Node* n2;
	//访问位
	int flag;
};

struct Node{
	int x;
	int y;
	Edge* arc[4];
	double angle[4];
	int edge_count;
	//0不是节点，辅助使用; 1普通节点; 2边界节点
	int flag;
	int unclose;
	int origin;
};

typedef struct LIST_NODE{
	Node *node;
	Edge *arc;
}LIST_NODE;

typedef vector<LIST_NODE*> LIST_NODE_VEC;

class imageAnalyse{
private:
	int height;
	int width;

	int startX, startY;
	int initX, initY;

	point** data;
//	point** border;

	IplImage *copy;

	//轨迹点总数
	int canCount;

	//链表头
	header *sendQueue;
	//画图队列
	header *drawQueue;

	vector<Node*> origin_node_vec;

	vector<LIST_NODE_VEC> node_list_vec3d;

	//细化
	void thin();
	CvSize changeSize(int w,int h);

	//初始化队列
	header* initQueue();
	//向队列添加一个点
	void add2Queue(int x, int y, int z,header* head);
	queue* getQueue(header* head);

	DLheader* initDblList();
	void add2List(int x,int y, DLheader* head);

	void addInitPoint();
	//搜索3X3八连通区域
	vector<point*> findNeighbor(int x, int y, bool special, int* branch_count);
	//找下一个起始位置
	point* findNext();

	double calAngle(int x1,int y1,int x2,int y2);

	Node* getNearstNode(int x,int y, int* pos);
	int getEdgeIndex(Node* node, Edge* edge);
	int getBestPath_index(vector<int> index_vec, vector<double> angle_vec, double angle);

	void add2SendQueue(Node* node, Edge* edge, int head_or_tail);

	void deleteNode(vector<Node*> &vec, vector<Node*> &closed_circuit_vec, Node* node);
	void add2NodeListVec(vector<LIST_NODE_VEC> node_list_vec, bool first);

public:
	imageAnalyse();
	bool isEmpty(header* head);
	//载入图像预处理，并转换为数组矩阵
	void image2Arr(char* path);
	void arr2Graph();

	//总控函数
	void find_v2();
	void getPointsInfo();

	header* getList();
	void writeToFile();

	int getWidth();
	int getHeight();
	
	int sendBufLen;
	int getSendBufLen();
	TCHAR sendBuf[SEND_BUF_SIZE];
	void getPacket(int length);
	void send(TCHAR* szBuf);
	//void reply(bool ok);
	bool isEnd;
};