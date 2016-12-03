#pragma once

#include "stdafx.h"
#include <stdlib.h>
#include <vector>
#include <queue>

#include "cv.hpp"
#include "cxcore.hpp"
#include "opencv2\highgui\highgui.hpp"

#define SEND_BUF_SIZE 10
#define panel_line 4

#define I_PANEL_WIDTH 300

using namespace std;

typedef enum direct{
	left = 1,
	right =2
}direct;

struct search_node{
	int x;
	int y;
	search_node(int x1,int y1):x(x1),y(y1){};
};

typedef struct point{
	int val;
	//是否访问 1是
	int flag;
	int x;
	int y;
	int z;
	//only use in panel_data
	int type;
}point;

typedef struct p_queue{
	int x;
	int y;
	int z;
	p_queue *next;
}p_queue;

typedef struct header{
	struct p_queue* front;
	struct p_queue* tail;
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


/// 端结点
struct Node{
	int x;
	int y;
	//连接节点的边，最多4条
	Edge* arc[4];
	//边对应的斜率角度值
	double angle[4];
	//边数量
	int edge_count;
	//0不是节点，辅助使用; 1普通节点; 2边界节点
	int flag;
	//是否为闭合曲线上的节点
	int unclose;
	//是否可作为起始节点
	int origin;
};

typedef struct LIST_NODE{
	Node *node;
	Edge *arc;
}LIST_NODE;

//
typedef vector<LIST_NODE*> LIST_NODE_VEC;

class imageAnalyse{
private:
	int height;
	int width;

	int startX, startY;
	int initX, initY;

	// 用来给最后的点加上偏移量，保证图片位于画板正中心
	int image_offset;
	bool is_width_short;

	point** data;
//	point** border;

	//轨迹点总数
	int canCount;

	//链表头
	header *sendQueue;
	//画图队列
	header *drawQueue;
	//find_v2中起始搜索点vector
	vector<Node*> origin_node_vec;

	//所有的LIST_NODE_VEC集合
	vector<LIST_NODE_VEC> node_list_vec3d;

	//细化
	void thin();
	//阈值化
	void thresh(IplImage *src,IplImage *dst);
	//更改大小
	CvSize changeSize(int w,int h);

	//初始化队列
	header* initQueue();
	//向队列尾添加一个点
	void add2Queue(int x, int y, int z,header* head);
	//从队列头取出一个点
	p_queue* getQueue(header* head);

	//初始化一个双向链表
	DLheader* initDblList();
	void add2List(int x,int y, DLheader* head);

	void addInitPoint();
	void addEndPoint();
	//搜索3X3八连通区域
	vector<point*> findNeighbor(int x, int y, bool special, int* branch_count);
	//找下一个起始位置
	point* findNext();

	double calAngle(int x1,int y1,int x2,int y2);

	Node* getNearstNode(int x,int y, int* pos);
	//根据edge指针得到它在arc数组里的下标
	int getEdgeIndex(Node* node, Edge* edge);
	//
	int getBestPath_index(vector<int> index_vec, vector<double> angle_vec, double angle);

	//head_or_tail->1head 2tail 0other
	void add2SendQueue(Node* node, Edge* edge, int head_or_tail);

	//从vec、closed_circut_vec中删除node
	void deleteNode(vector<Node*> &vec, vector<Node*> &closed_circuit_vec, Node* node);
	void add2NodeListVec(vector<LIST_NODE_VEC> node_list_vec, bool first);

public:
	imageAnalyse();
	bool isEmpty(header* head);
	//载入图像预处理，并转换为数组矩阵
	//总控函数
	void image2Arr(char* path); // 阈值化，细化，保存到数组
	void arr2Graph(); // 
	void find_v2();
	void getPointsInfo();
	///////////////////////////////////////
	IplImage *threshImg;
	IplImage *thinImg;
	IplImage *resizeImg;


	header* getList();
	void writeToFile();

	int getWidth();
	int getHeight();
	
	int sendBufLen;
	int getSendBufLen();
	TCHAR sendBuf[SEND_BUF_SIZE];
	void getPacket(int length);
	void copyPacket(TCHAR* szBuf);
	//void reply(bool ok);
	bool isEnd;
	short sendCount;

	const char* CString2char(CString cstr); // 宽字节转换
	bool getFilePoint(const char* path); // 从文件中获取点
	void init();
	

private:
	int panel_h,panel_w;
	point** panel_data;
	int panel_val_thresh;

	//一下为画面算法函数
	//估计线宽
	int getLineWidth(IplImage* img);
	//
	point* findNextPanel(int cur_type);
	
	point* getNextLine(int x,int y,direct dir,int next_x);
	void getPanelTract();
	bool  bfs(point** arr,int x,int y,int threshold,int width, int height,int count);

	void add2SendQueue(int x,int y,int z);


};