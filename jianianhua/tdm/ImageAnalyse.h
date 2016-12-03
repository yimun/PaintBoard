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
	//�Ƿ���� 1��
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
	//����λ
	int flag;
};


/// �˽��
struct Node{
	int x;
	int y;
	//���ӽڵ�ıߣ����4��
	Edge* arc[4];
	//�߶�Ӧ��б�ʽǶ�ֵ
	double angle[4];
	//������
	int edge_count;
	//0���ǽڵ㣬����ʹ��; 1��ͨ�ڵ�; 2�߽�ڵ�
	int flag;
	//�Ƿ�Ϊ�պ������ϵĽڵ�
	int unclose;
	//�Ƿ����Ϊ��ʼ�ڵ�
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

	// ���������ĵ����ƫ��������֤ͼƬλ�ڻ���������
	int image_offset;
	bool is_width_short;

	point** data;
//	point** border;

	//�켣������
	int canCount;

	//����ͷ
	header *sendQueue;
	//��ͼ����
	header *drawQueue;
	//find_v2����ʼ������vector
	vector<Node*> origin_node_vec;

	//���е�LIST_NODE_VEC����
	vector<LIST_NODE_VEC> node_list_vec3d;

	//ϸ��
	void thin();
	//��ֵ��
	void thresh(IplImage *src,IplImage *dst);
	//���Ĵ�С
	CvSize changeSize(int w,int h);

	//��ʼ������
	header* initQueue();
	//�����β���һ����
	void add2Queue(int x, int y, int z,header* head);
	//�Ӷ���ͷȡ��һ����
	p_queue* getQueue(header* head);

	//��ʼ��һ��˫������
	DLheader* initDblList();
	void add2List(int x,int y, DLheader* head);

	void addInitPoint();
	void addEndPoint();
	//����3X3����ͨ����
	vector<point*> findNeighbor(int x, int y, bool special, int* branch_count);
	//����һ����ʼλ��
	point* findNext();

	double calAngle(int x1,int y1,int x2,int y2);

	Node* getNearstNode(int x,int y, int* pos);
	//����edgeָ��õ�����arc��������±�
	int getEdgeIndex(Node* node, Edge* edge);
	//
	int getBestPath_index(vector<int> index_vec, vector<double> angle_vec, double angle);

	//head_or_tail->1head 2tail 0other
	void add2SendQueue(Node* node, Edge* edge, int head_or_tail);

	//��vec��closed_circut_vec��ɾ��node
	void deleteNode(vector<Node*> &vec, vector<Node*> &closed_circuit_vec, Node* node);
	void add2NodeListVec(vector<LIST_NODE_VEC> node_list_vec, bool first);

public:
	imageAnalyse();
	bool isEmpty(header* head);
	//����ͼ��Ԥ������ת��Ϊ�������
	//�ܿغ���
	void image2Arr(char* path); // ��ֵ����ϸ�������浽����
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

	const char* CString2char(CString cstr); // ���ֽ�ת��
	bool getFilePoint(const char* path); // ���ļ��л�ȡ��
	void init();
	

private:
	int panel_h,panel_w;
	point** panel_data;
	int panel_val_thresh;

	//һ��Ϊ�����㷨����
	//�����߿�
	int getLineWidth(IplImage* img);
	//
	point* findNextPanel(int cur_type);
	
	point* getNextLine(int x,int y,direct dir,int next_x);
	void getPanelTract();
	bool  bfs(point** arr,int x,int y,int threshold,int width, int height,int count);

	void add2SendQueue(int x,int y,int z);


};