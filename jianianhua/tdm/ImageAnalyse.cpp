#include "stdafx.h"

#include <algorithm>
#include <math.h>
#include "ImageAnalyse.h"

imageAnalyse::imageAnalyse(){
	
	init();
}

void imageAnalyse::init()
{
	sendQueue = initQueue();
	canCount = 0;
	//sendBufLen = 0;
	sendCount = 0;
	panel_val_thresh = panel_line*panel_line/2;
	memset(sendBuf, 0, sizeof(TCHAR)*SEND_BUF_SIZE);
	data = NULL;
	origin_node_vec.clear();
	node_list_vec3d.clear();
}

bool imageAnalyse::isEmpty(header* head){
	if(!head->front || !head->tail)
		return true;

	return false;
}

header* imageAnalyse::initQueue(){
	header *head = (header*)malloc(sizeof(header));
	head->front =NULL;
	head->tail = NULL;
	return head;
}

void imageAnalyse::add2Queue(int x, int y, int z, header* head){
	//第一个
	if(head->front==NULL){
		head->front = (struct p_queue*)malloc(sizeof(struct p_queue));
		head->tail = head->front;
		head->front->next=NULL;
		head->tail->next=NULL;

		head->front->x = x;
		head->front->y = y;
		head->front->z = z;
		return;
	}
	p_queue *temp = (struct p_queue*)malloc(sizeof(struct p_queue));

	if(head == sendQueue && x != 0 && y != 0) // 如果添加的点为发送队列的点，加上偏移量  并进行镜像翻转
	{
		if(is_width_short){       // 如果x边比较短
			temp->x = I_PANEL_WIDTH - (x + image_offset);
			temp->y = y;
		}else{				// 如果y边比较短
			temp->y = y + image_offset;
			temp->x = I_PANEL_WIDTH - x;
		}
	} // +end
	else{ // 其余的队列
		temp->x = x;
		temp->y = y;
	}

	
	temp->z = z;
	temp->next = NULL;

	//把新来的放后面，并改队尾
	head->tail->next = temp;
	head->tail = temp;
	//printf("x:%d,y:%d\n",x,y);
	//cvSet2D(copy,x,y,cvScalar(0,255,0));
}

p_queue* imageAnalyse::getQueue(header* head){
	if(isEmpty(head))
		return NULL;
	p_queue* p = head->front;
	head->front = p->next;

	if(NULL==head->front){
		head->tail = NULL;
	}
	return p;
}

DLheader* imageAnalyse::initDblList(){
	DLheader *head = (DLheader*)malloc(sizeof(DLheader));
	head->front = NULL;
	head->tail = NULL;
	return head;
}

void imageAnalyse::add2List(int x,int y,DLheader* head){
	if(head->front==NULL){
		head->front = (DblList*)malloc(sizeof(DblList));
		head->tail = head->front;
		head->front->next = NULL;
		head->front->pre = NULL;

		head->front->x = x;
		head->front->y = y;
		return;
	}
	DblList* temp = (DblList*)malloc(sizeof(DblList));
	temp->x = x;
	temp->y = y;
	temp->next = NULL;
	temp->pre = head->tail;
	head->tail->next = temp;
	head->tail = temp;
}

void imageAnalyse::thin(){
	int neighbor[8]={0};
	bool loop = true;
	bool mark = false;

	while(loop){
		loop = false;

		for(int y=1;y<height-1;y++){
			for(int x=1;x<width-1;x++){
				if(data[y][x].val==0)
					continue;
				//4
				neighbor[2]=data[y][x+1].val;
				//3
				neighbor[1]=data[y-1][x+1].val;
				//2
				neighbor[0]=data[y-1][x].val;
				//9
				neighbor[7]=data[y-1][x-1].val;
				//8
				neighbor[6]=data[y][x-1].val;
				//7
				neighbor[5]=data[y+1][x-1].val;
				//6
				neighbor[4]=data[y+1][x].val;
				//5
				neighbor[3]=data[y+1][x+1].val;

				int np = 0;
				for(int i=0;i<8;i++)
					if(neighbor[i]==1)
						np++;
				if(np<2 || np>6)
					continue;

				int sp = 0;
				for(int i=1;i<8;i++){
					if(neighbor[i] > neighbor[i-1])
						sp++;
				}
				if(neighbor[0]>neighbor[7])
					sp++;
				if(sp!=1)
					continue;

				if(neighbor[2]*neighbor[0]*neighbor[4]!=0)
					continue;
				if(neighbor[2]*neighbor[6]*neighbor[4]!=0)
					continue;

				data[y][x].flag=1;
				mark = true;
				loop = true;
			}
		}

		if(mark){
			for(int y=0;y<height;y++){
				for(int x=0;x<width;x++){
					if(data[y][x].flag==1){
						data[y][x].val=0;
					}
				}
			}
		}

		mark = false;

		for(int y=1;y<height-1;y++){
			for(int x=1;x<width-1;x++){
				if(data[y][x].val==0)
					continue;
				//4
				neighbor[2]=data[y][x+1].val;
				//3
				neighbor[1]=data[y-1][x+1].val;
				//2
				neighbor[0]=data[y-1][x].val;
				//9
				neighbor[7]=data[y-1][x-1].val;
				//8
				neighbor[6]=data[y][x-1].val;
				//7
				neighbor[5]=data[y+1][x-1].val;
				//6
				neighbor[4]=data[y+1][x].val;
				//5
				neighbor[3]=data[y+1][x+1].val;

				int np = 0;
				for(int i=0;i<8;i++)
					if(neighbor[i]==1)
						np++;
				if(np<2 || np>6)
					continue;

				int sp = 0;
				for(int i=1;i<8;i++){
					if(neighbor[i] > neighbor[i-1])
						sp++;
				}
				if(neighbor[0]>neighbor[7])
					sp++;
				if(sp!=1)
					continue;

				if(neighbor[2]*neighbor[0]*neighbor[6]!=0)
					continue;
				if(neighbor[0]*neighbor[6]*neighbor[4]!=0)
					continue;

				data[y][x].flag=1;
				mark = true;
				loop = true;
			}
		}

		if(mark){
			for(int y=0;y<height;y++){
				for(int x=0;x<width;x++){
					if(data[y][x].flag==1){
						data[y][x].val=0;
					}
				}
			}
		}
	}
}

CvSize imageAnalyse::changeSize(int w, int h){
	CvSize size;
	printf("%d==%d",w,h);
	if(h>=w){
		height = I_PANEL_WIDTH; 
		width = I_PANEL_WIDTH*w/h;
		// LW+
		is_width_short = true;
		image_offset = (I_PANEL_WIDTH-width)/2;
		// +end
	}
	else{
		width = I_PANEL_WIDTH;
		height = I_PANEL_WIDTH*h/w;
		// LW+
		is_width_short = false;
		image_offset = (I_PANEL_WIDTH-height)/2;
		// +end
	}
	size = cvSize(width,height);
	return size;
}

int imageAnalyse::getLineWidth(IplImage* img){
	uchar *ptr;
	int candidate_width[10] = {0};
	bool is_bg = true;
	bool throughed = false;
	int count = 0;
	for(int y=0;y<height;y++){
		ptr = (uchar *)(img->imageData + y*img->widthStep);
		for(int x=0;x<width;x++){
			//到达目标区域
			if(ptr[x]==0){
				count++;
				is_bg = false;
				throughed = true;
			}
			//进入背景区域
			else{
				is_bg = true;
				//刚从目标区域粗来
				if(throughed){
					if(count>0 && count<=10)
						candidate_width[count-1]++;
					count = 0;
					throughed = false;
				}
			}
		}
	}
	int max = 0,target_width = 0,best_width=-1;
	for(int i=0;i<10;i++){
		if(candidate_width[i]>max){
			target_width = i+1;
			max = candidate_width[i];
		}
		if(candidate_width[i]>=0.8*max){
			best_width = i+1;
		}
	}
	if(best_width==-1)
		best_width = target_width;
	return best_width;
}

void imageAnalyse::image2Arr(char* path){
	IplImage *srcc_2,*src_2,*thresh_2,*panel_2;
	IplImage *srcc,*src_1,*no_panel;
	IplImage *canImg,*panel;
	if((srcc=cvLoadImage(path,CV_LOAD_IMAGE_GRAYSCALE))==NULL){
		return;
	}
	if((srcc_2=cvLoadImage(path,CV_LOAD_IMAGE_GRAYSCALE))==NULL){
		return;
	}

	CvSize size = changeSize(srcc->width,srcc->height);
	height = size.height;
	width = size.width;
	//CvSize size = cvGetSize(srcc);

	thresh_2 = cvCreateImage(size,srcc_2->depth,srcc_2->nChannels);
	src_2 = cvCreateImage(size,srcc_2->depth,srcc_2->nChannels);
	panel_2 = cvCreateImage(size,srcc_2->depth,srcc_2->nChannels);
	panel = cvCreateImage(size,srcc->depth,srcc->nChannels);
	no_panel = cvCreateImage(size,srcc->depth,srcc->nChannels);
	canImg = cvCreateImage(size,srcc->depth,srcc->nChannels);
	threshImg = cvCreateImage(size,srcc->depth,srcc->nChannels);
	src_1 = cvCreateImage(size,srcc->depth,srcc->nChannels);

	// 缩放大小
	cvResize(srcc_2,src_2,1);
	cvResize(srcc,src_1,1);
	resizeImg = src_1;

	// 阈值化操作
	thresh(src_1,threshImg);
	thresh(src_2,thresh_2);

	panel_h = height/panel_line +1;
	panel_w = width/panel_line +1;

	data = new point *[height];
	for(int i=0;i<height;i++){
		data[i] = new point[width];
	}

	panel_data = new point *[panel_h];
	for(int i=0;i<panel_h;i++){
		panel_data[i] = new point[panel_w];
		for(int j=0;j<panel_w;j++){
			panel_data[i][j].val = 0;
		}
	}

	int line_width = 0; 
	line_width = getLineWidth(threshImg);

	cvDilate(thresh_2,panel_2,0,line_width);

	uchar *p_thr,*p_pane;
	for(int y=0;y<height;y++){
		p_thr = (uchar *)(threshImg->imageData + y*threshImg->widthStep);
		p_pane = (uchar *)(panel_2->imageData + y*panel_2->widthStep);
		for(int x=0;x<width;x++){
			if(p_thr[x]==0 && p_pane[x]==255){
				cvSetReal2D(no_panel,y,x,0);
			}
			else {
				cvSetReal2D(no_panel,y,x,255);
			}

			if(p_pane[x]==0)
				panel_data[y/panel_line][x/panel_line].val++;
		}
	}
	for(int y=0;y<panel_h;y++){
		for(int x=0;x<panel_w;x++){
			panel_data[y][x].type = 0;
		}
	}
	int count = 1;
	for(int y=0;y<panel_h;y++){
		for(int x=0;x<panel_w;x++){
			if(panel_data[y][x].val>=panel_val_thresh){
				//panel_data[y][x].val = 1;
				if(bfs(panel_data,x,y,panel_val_thresh,panel_w,panel_h,count)){
					count++;
				}
			}
			else{
				panel_data[y][x].val = 0;
			}
			panel_data[y][x].flag = 0;
			panel_data[y][x].x = x;
			panel_data[y][x].y = y;
		}
	}
	uchar* ptr;
	IplImage* temp = cvCreateImage(size,8,1);
	for(int y=0;y<height;y++){
		ptr = (uchar *)(no_panel->imageData + y*no_panel->widthStep);
		for(int x=0;x<width;x++){
			data[y][x].x = x;
			data[y][x].y = y;
			data[y][x].z = 0;

			data[y][x].val = (int)ptr[x];
			if((int)ptr[x]==0){
				data[y][x].val = 1;
			}
			else{
				data[y][x].val = 0;
			}
			data[y][x].flag = 0;
		}
	}

	//细化
	thin();
	bool first=true;
	for(int y=0;y<height;y++){
		for(int x=0;x<width;x++){
			if(data[y][x].val==1 && first){
				startX = x;
				startY = y;
				first = false;
			}
			data[y][x].flag = 0;
		}
			
	}
	
	// 生成细化图片
	thinImg =  cvCreateImage(cvGetSize(threshImg),threshImg->depth,1);
	for(int y=0;y<height;y++){
		for(int x=0;x<width;x++){
			if(data[y][x].val==1)
				cvSetReal2D(thinImg,y,x,0);
			else
				cvSetReal2D(thinImg,y,x,255);
		}
			
	}
}

/////////////////////////////访问过的点不管是否为边缘，都应修改访问标位//////////////////////////////
/****八连通区域中，若只有一个邻接块，或两个挨着的邻接块，就不分支****/
/*****不分支，若有两个挨着的邻接块，取离得近的***/
/********/
vector<point*> imageAnalyse::findNeighbor(int x, int y, bool special, int* branch_count=NULL)
{
	int coor_x[4]={-1},coor_y[4]={-1};
	int coor_x2[4]={-1},coor_y2[4]={-1};
	int p_count = 0,p_count2 = 0;
	vector<point*> branch_vec;
	int index = 0;
	int b_count = 0;
	//y-1 x
	if(y>0 &&x>=0)
	{

		if(data[y-1][x].val==1){
			coor_x[p_count] = x;
			coor_y[p_count] = y-1;
			p_count++;
		}
		else
			data[y-1][x].flag = 1;
	}
	//y x-1
	if(y>=0 &&x>0)
	{
		if(data[y][x-1].val==1){
			coor_x[p_count] = x-1;
			coor_y[p_count] = y;
			p_count++;
		}
		else
			data[y][x-1].flag = 1;
	}
	//y+1 x
	if(y<height-1 &&x>=0)
	{
		if(data[y+1][x].val==1){
			coor_x[p_count] = x;
			coor_y[p_count] = y+1;
			p_count++;
		}
		else
			data[y+1][x].flag = 1;
	}
	//y x+1
	if(y>=0 &&x<width-1)
	{
		if(data[y][x+1].val==1){
			coor_x[p_count] = x+1;
			coor_y[p_count] = y;
			p_count++;
		}
		else
			data[y][x+1].flag = 1;
	}

	////////////////////////////////////////////////////////////

	//y-1 x-1
	if(y>0 &&x>0)
	{
		if(data[y-1][x-1].val==1 ){
			coor_x2[p_count2] = x-1;
			coor_y2[p_count2] = y-1;
			p_count2++;
		}
		else
			data[y-1][x-1].flag = 1;
	}
	//y+1 x-1
	if(y<height-1 &&x>0)
	{
		if(data[y+1][x-1].val==1){
			coor_x2[p_count2] = x-1;
			coor_y2[p_count2] = y+1;
			p_count2++;
		}
		else
			data[y+1][x-1].flag = 1;
	}
	//y+1 x+1
	if(y<height-1 &&x<width-1)
	{
		if(data[y+1][x+1].val==1){
			coor_x2[p_count2] = x+1;
			coor_y2[p_count2] = y+1;
			p_count2++;
		}
		else
			data[y+1][x+1].flag = 1;
	}
	//y-1 x+1
	if(y>0 &&x<width-1)
	{

		if(data[y-1][x+1].val==1){
			coor_x2[p_count2] = x+1;
			coor_y2[p_count2] = y-1;
			p_count2++;
		}
		else
			data[y-1][x+1].flag = 1;
	}

	for(int i=0;i<p_count;i++){
		if(branch_count!=NULL){
			b_count++;
		}
		if(special){
			if(data[coor_y[i]][coor_x[i]].flag==2||data[coor_y[i]][coor_x[i]].flag==3)
				continue;
		}
		if(data[coor_y[i]][coor_x[i]].flag!=1){
			branch_vec.push_back(&data[coor_y[i]][coor_x[i]]);
		}
	}

	for(int i=0;i<p_count2;i++){
		int j;
		for(j=0;j<p_count;j++){
			if((abs(coor_x[j]-coor_x2[i])+abs(coor_y[j]-coor_y2[i]))==1){
				break;
			}
		}
		if(j==p_count){

			if(branch_count!=NULL){
				b_count++;
			}
			if(special){
				if(data[coor_y2[i]][coor_x2[i]].flag==2||data[coor_y2[i]][coor_x2[i]].flag==3)
					continue;
			}
			if(data[coor_y2[i]][coor_x2[i]].flag!=1){
				branch_vec.push_back(&data[coor_y2[i]][coor_x2[i]]);
			}
		}
	}

	if(branch_count!=NULL)
		memcpy(branch_count,&b_count,sizeof(int));

	return branch_vec;
}

double imageAnalyse::calAngle(int x1,int x2,int y1,int y2){
	if(x1==x2){
		if(y2>=y1)
			return 90.0;
		else
			return -90.0;
	}
	else{
		double angle = (double)(y2-y1)/(double)(x2-x1);
		return atan(angle);
	}
}

point* imageAnalyse::findNext(){
	for(int y=startY;y<height;y++){
		for(int x=0;x<width;x++){
			//边缘~
			if(data[y][x].val==1&&data[y][x].flag==0){
				//
				startX = x;
				startY = y;
				return &data[y][x];
			}
		}
	}
	return NULL;
}

class vec_finder{
public:
	vec_finder(const int x,const int y):p_x(x),p_y(y){}
	bool operator()(vector<struct Node*>::value_type &n){
		if(n->x==p_x && n->y==p_y)
			return true;
		else
			return false;
	}
private:
	int p_x;
	int p_y;
};

void imageAnalyse::arr2Graph(){
	int x=startX,y=startY;
	//分支节点所有分支起始点集合
	vector<point*> branch_vec;
	bool haveOrigin = false;
	//下一个连通区域
	vector<Node*> node_vec;
	//node_vec中的元素
	Node* node = NULL;
	//findNext
	point* start_p = NULL;
	Node* start_node;

	//“邻接点”
	vector<point*> p_vec;

	Edge* edge;
	//边上的点
	DLheader* head=NULL;

	vector<Node*>::iterator exist_it;

	int branch_count = 0;

	while(true){
		start_p = findNext();
		if(start_p==NULL)
			break;
		x = start_p->x;
		y = start_p->y;
		//if(x==120&&y==23)
		//{int iii=9;}
		start_node = (Node*)malloc(sizeof(Node));
		start_node->x = x;
		start_node->y = y;
		data[y][x].flag = 1;
		start_node->edge_count = 0;
		start_node->flag = 0;
		start_node->unclose = 0;
		start_node->origin = 0;
		node_vec.push_back(start_node);

		while(!node_vec.empty()){
			//
			node = node_vec[node_vec.size()-1];
			node_vec.pop_back();
			branch_vec = findNeighbor(node->x,node->y, false);
			if(node->flag==0){
				if(branch_vec.size()==1)
					node->flag = 2;
				else if(branch_vec.size()>2)
					node->flag = 1;
			}
			//特殊访问标志
	/*		for(int j=0;j<branch_vec.size();j++){
				data[branch_vec[j]->y][branch_vec[j]->x].flag = 2;
			}*/
			data[node->y][node->x].flag = 3;
			vector<point*>::iterator it=branch_vec.begin();
			for(int i=node->edge_count;it!=branch_vec.end();it++,i++){
				point* p = (point*)*it;
				if(data[p->y][p->x].flag==1)
					continue;
				//对应一条边
				edge = (Edge*)malloc(sizeof(Edge));
				edge->flag = 0;
				head = initDblList();
				//下一个节点
				Node* n=NULL;

				int count = 1;
				//用来前一段计算角度
				int x1=-1,x2=-1,y1=-1,y2=-1;
				//后一段角度
				header* angle_head=NULL;
				int angle_list_size = 0;
				angle_head = initQueue();
				add2Queue(node->x,node->y,0,angle_head);

				add2List(p->x,p->y,head);
				x1 = p->x;
				y1 = p->y;

				p_vec = findNeighbor(p->x, p->y, true, &branch_count);
				//被访问
				data[p->y][p->x].flag = 1;
				while(true){
					add2Queue(p->x,p->y,0,angle_head);
					if(angle_list_size>10){
						getQueue(angle_head);
					}
					else{
						angle_list_size++;
					}
					if(branch_count==0)
						break;
					if(branch_count==1){
						haveOrigin = true;
						//以端点节点结尾
						if(p_vec.size()==0){
							n = (Node*)malloc(sizeof(Node));
							n->flag = 2;
							n->unclose = 0;
							n->origin=1;
							n->x = p->x;
							n->y = p->y;
							edge->n2 = n;
							edge->head = head;
							edge->n1 = node;
							n->arc[0] = edge;
							node->arc[i] = edge;
							n->edge_count =1;
							p_queue* q1 = angle_head->front;
							p_queue* q2 = angle_head->tail;
							n->angle[0] = calAngle(q1->x,q2->x,q1->y,q2->y);

							origin_node_vec.push_back(n);
							break;
						}
						else{
							p = p_vec[0];
							//已访问
							data[p->y][p->x].flag = 1;
							add2List(p->x,p->y,head);
							p_vec = findNeighbor(p->x, p->y,false, &branch_count);
						}
					}

					/////////!!!!!				
					else if(branch_count==2){
						p = p_vec[0];
						if(data[p->y][p->x].flag==3){
							p_queue* q1 = angle_head->front;
							p_queue* q2 = angle_head->tail;
							edge->n1 = node;
							edge->n2 = node;
							node->arc[node->edge_count] = edge;
							node->angle[node->edge_count] = calAngle(q1->x,q2->x,q1->y,q2->y);
							node->edge_count--;
							edge->head = head;
							break;
						}
						//已访问
						data[p->y][p->x].flag = 1;
						add2List(p->x,p->y,head);
						if(count==1)
							p_vec = findNeighbor(p->x, p->y,true, &branch_count);
						else
							p_vec = findNeighbor(p->x, p->y,false, &branch_count);
					}
					else{
						data[p->y][p->x].flag = 0;
						//查找该点是否已在
						exist_it = find_if(node_vec.begin(),node_vec.end(),vec_finder(p->x,p->y));
						p_queue* q1 = angle_head->front;
						p_queue* q2 = angle_head->tail;

						if(p->x==node->x && p->y==node->y){
							edge->n2 = node;
							node->arc[node->edge_count] = edge;
							node->angle[node->edge_count] = calAngle(q1->x,q2->x,q1->y,q2->y);
							node->edge_count--;
						}
						//新的节点
						if(exist_it==node_vec.end()){
							n = (Node*)malloc(sizeof(Node));
							n->flag = 1;
							n->unclose = 0;
							n->x = p->x;
							n->y = p->y;
							n->edge_count = 0;
							edge->n2 = n;
							n->arc[n->edge_count] = edge;
							node->arc[i] = edge;
							n->angle[n->edge_count] = calAngle(q1->x,q2->x,q1->y,q2->y);
							n->edge_count++;
							n->origin=0;
							node_vec.push_back(n);
						}
						//已有节点
						else{
							n = (Node*)exist_it[0];
							edge->n2 = n;
							n->arc[n->edge_count] = edge;
							node->arc[i] = edge;
							n->angle[n->edge_count] = calAngle(q1->x,q2->x,q1->y,q2->y);
							n->edge_count++;
						}

						edge->head = head;
						edge->n1 = node;

						break;
					}
					count++;
					if(count==10){
						x2 = p->x;
						y2 = p->y;
					}
				}//while
				if(count<10){
					x2 = p->x;
					y2 = p->y;
				}
				node->angle[i] = calAngle(x1,x2,y1,y2);

			}//for iterator
			//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
			node->edge_count += branch_vec.size();
			data[node->y][node->x].flag = 1;
			for(int j=0;j<branch_vec.size();j++){
				exist_it = find_if(node_vec.begin(),node_vec.end(),vec_finder(branch_vec[j]->x,branch_vec[j]->y));
				if(exist_it==node_vec.end())
					data[branch_vec[j]->y][branch_vec[j]->x].flag = 1;
			}
		}//while empty
		if(!haveOrigin){
			start_node->origin=1;
			origin_node_vec.push_back(start_node);
		}
		haveOrigin = false;

	}//while canCount
}

//pos 目前没有用
Node* imageAnalyse::getNearstNode(int x,int y, int* pos){
	Node* node;
	Node* result=NULL;
	int min = width+height;
	vector<Node*>::iterator it = origin_node_vec.begin();
	for(int i=0;it!=origin_node_vec.end();i++,it++){
		node = *it;
		if(abs(node->x - x)+abs(node->y -y)<min){
			min = abs(node->x - x)+abs(node->y -y);
			result = node;
			*pos = i;
		}

	}//for origin itrator
	return result;
}

int imageAnalyse::getEdgeIndex(Node*node, Edge* edge){
	for(int i=0;i<node->edge_count;i++){
		if(node->arc[i]==edge){
			return i;
		}
	}
	return -1;
}

int imageAnalyse::getBestPath_index(vector<int> index_vec, vector<double> angle_vec, double angle){
	double min_da=360.0;
	double da;
	int result_index=-1;
	int index;
	for(int i=0;i<index_vec.size();i++){
		index = index_vec[i];
		da = abs(angle_vec[i]-angle);
		if(da<min_da){
			min_da = da;
			result_index = index;
		}

		for(int j=1;j<index_vec.size();j++){
			if(j==i)continue;
			index = index_vec[j];
			da = abs(angle_vec[i]-angle_vec[j]);
			if(da<min_da){
				min_da = da;
				result_index = -1;
			}
		}
	}
	return result_index;
}

void imageAnalyse::deleteNode(vector<Node*> &vec, vector<Node*> &closed_circuit_vec, Node* node){
	//移除该节点
	vector<Node*>::iterator temp_it;
	Node* temp_n;
	for(temp_it=vec.begin();temp_it!=vec.end();temp_it++){
		temp_n = *temp_it;
		if(temp_n->x==node->x && temp_n->y==node->y){
			temp_it = vec.erase(temp_it);
		}
		if(temp_it == vec.end()){
			break;
		}
	}

	for(temp_it=closed_circuit_vec.begin();temp_it!=closed_circuit_vec.end();temp_it++){
		temp_n = *temp_it;
		if(temp_n->x==node->x && temp_n->y==node->y){
			temp_it = closed_circuit_vec.erase(temp_it);
		}
		if(temp_it == closed_circuit_vec.end()){
			break;
		}
	}
}

void imageAnalyse::add2NodeListVec(vector<LIST_NODE_VEC> node_list_vec, bool first){
	if(first){
		vector<LIST_NODE_VEC>::iterator list_it;
		Node* it_node;
		vector<Node*> result_vec;
		LIST_NODE_VEC it_list_node_vec;
		for(list_it=node_list_vec.begin();list_it!=node_list_vec.end();list_it++){
			it_list_node_vec = *list_it;
			node_list_vec3d.push_back(it_list_node_vec);
		}
	}
	else{
		LIST_NODE_VEC local_it_list_node_vec;
		LIST_NODE_VEC globel_it_list_node_vec;
		LIST_NODE* list_node;
		Node* node;
		vector<LIST_NODE_VEC>::iterator local_list_it=node_list_vec.begin();
		vector<LIST_NODE_VEC>::iterator globel_list_it;

		bool ok = false;
		vector<LIST_NODE*>::iterator list_it;
		LIST_NODE* list_it_node;

		//遍历当前得到所有的节点链
		for(;local_list_it!=node_list_vec.end();local_list_it++){
			local_it_list_node_vec = *local_list_it;
			//起始节点
			node = local_it_list_node_vec[0]->node;
			//
			ok = false;
			for(globel_list_it=node_list_vec3d.begin();globel_list_it!=node_list_vec3d.end();globel_list_it++){
				globel_it_list_node_vec = *globel_list_it;
				for(list_it=globel_it_list_node_vec.begin();list_it!=globel_it_list_node_vec.end();list_it++){
					list_it_node = *list_it;
					if(list_it_node->node == node){
						node_list_vec3d.insert(globel_list_it+1, local_it_list_node_vec);
						ok = true;
						break;
					}
				}
				if(ok)
					break;
			}
		}
	}

}

void imageAnalyse::find_v2(){
	vector<Node*> draw_vec;
	Node* node;
	//vector<Node*>::iterator branch_it;
	Edge* pre_edge = NULL;

	vector<int> index_vec;
	vector<double> angle_vec;

	int node_pos = -1;
	int start_find_x=0,start_find_y=0;
	int edge_visited_count;
	double cur_angle = 0.0;

	vector<Node*> closed_circuit_vec;
	vector<LIST_NODE_VEC> temp_list_node_vecs;

	bool isfirst = true;
	while(true){
		while(!origin_node_vec.empty()){
			node = getNearstNode(start_find_x, start_find_y, &node_pos);
			LIST_NODE_VEC node_list_vec;

			/*if(!isfirst){
			vector<LIST_NODE_VEC>::iterator globel_list_it;
			LIST_NODE_VEC globel_it_list_node_vec;

			int index=0;
			bool ok = false;
			vector<LIST_NODE*>::iterator list_it;
			LIST_NODE* list_it_node;
			for(globel_list_it=node_list_vec3d.begin();globel_list_it!=node_list_vec3d.end();globel_list_it++,index++){
			globel_it_list_node_vec = *globel_list_it;
			for(list_it=globel_it_list_node_vec.begin();list_it!=globel_it_list_node_vec.end();list_it++){
			list_it_node = *list_it;
			if(list_it_node->node == node){
			adjion_list_index_vec.push_back(index);
			ok = true;
			break;
			}
			}
			if(ok)
			break;
			}
			}*/

			while(true){

				index_vec.clear();
				angle_vec.clear();
				edge_visited_count=0;
				for(int i=0;i<node->edge_count;i++){
					//边界节点
					//if(node->flag==2){
					//	pre_edge = node->arc[0];
					//}

					//该边已访问
					if(node->arc[i]->flag==1){
						edge_visited_count++;
						continue;
					}
					if(node->origin==1){
						for(int j=0;j<node->edge_count;j++){
							if(node->arc[j]->flag==0){
								pre_edge = node->arc[j];
								//pre_edge->flag = 1;
								edge_visited_count++;
								break;
							}
						}
					}

					index_vec.push_back(i);
					angle_vec.push_back(node->angle[i]);
				}

				int cur_edge_index = getEdgeIndex(node, pre_edge);
				cur_angle = node->angle[cur_edge_index];
				int next_edge_index = getBestPath_index(index_vec, angle_vec, cur_angle);
				//换别的路走
				if(next_edge_index==-1){
					if(node->flag!=2){
						node->unclose = 1;
						closed_circuit_vec.push_back(node);
					}

					LIST_NODE* list_node;
					list_node = (LIST_NODE*)malloc(sizeof(LIST_NODE));
					list_node->arc = NULL;
					list_node->node = node;
					node_list_vec.push_back(list_node);

					start_find_x = node->x;
					start_find_y = node->y;
					if(edge_visited_count==node->edge_count){
						//移除该节点
						deleteNode(origin_node_vec, closed_circuit_vec, node);
					}
					break;
				}
				pre_edge = node->arc[next_edge_index];
				//将被访问（肯定要被访问）
				pre_edge->flag = 1;
				LIST_NODE* list_node;
				list_node = (LIST_NODE*)malloc(sizeof(LIST_NODE));
				list_node->arc = pre_edge;
				list_node->node = node;
				node_list_vec.push_back(list_node);

				if(node->unclose==1){
					node->unclose = 0;
				}

				//add2SendQueue(node,pre_edge);
				int no_visited_count = 0;
				for(int i=0;i<node->edge_count;i++){
					if(node->arc[i]->flag==0){
						no_visited_count++;
					}
				}
				if(no_visited_count==0){
					//移除该节点
					deleteNode(origin_node_vec, closed_circuit_vec, node);
				}
				else if(no_visited_count==1){
					//"边界"节点
					node->flag = 2;
					//
					origin_node_vec.push_back(node);
					node->origin = 1;
				}
				else if(no_visited_count==2){
					//node->edge_count = 4
					node->unclose = 1;
					closed_circuit_vec.push_back(node);
				}
				if(pre_edge->n1==pre_edge->n2){
					break;
				}
				if(pre_edge->n1==node)
					node = pre_edge->n2;
				else
					node = pre_edge->n1;
			}//while true
			//将这条路线加进去
			temp_list_node_vecs.push_back(node_list_vec);
		}//while !empty

		vector<Node*>::iterator node_it;
		Node* it_node;
		for(node_it=closed_circuit_vec.begin();node_it!=closed_circuit_vec.end();node_it++){
			it_node = *node_it;
			if(it_node->unclose==1){
				it_node->origin = 1;
				origin_node_vec.push_back(it_node);
			}
		}

		if(isfirst){
			add2NodeListVec(temp_list_node_vecs, true);
			isfirst = false;
		}
		else{
			add2NodeListVec(temp_list_node_vecs, false);
		}
		temp_list_node_vecs.clear();
		closed_circuit_vec.clear();
		if(origin_node_vec.empty())
			break;
	}

}

void imageAnalyse::add2SendQueue(Node* node, Edge* edge, int head_or_tail){
	DLheader* dlheader = edge->head;
	DblList* list=NULL;
	DblList* front = dlheader->front;
	DblList* tail = dlheader->tail;
	if(edge->n1 == node){
		list = dlheader->front;
		if(head_or_tail==1)
			add2Queue(list->x,list->y,1,sendQueue);
		else
			add2Queue(list->x,list->y,0,sendQueue);
		list = list->next;
		while(list!=NULL){
			if(list->next!=NULL)
				add2Queue(list->x,list->y,0,sendQueue);
			else{
				if(head_or_tail==2)
					add2Queue(list->x,list->y,1,sendQueue);
				else
					add2Queue(list->x,list->y,0,sendQueue);
			}
			list = list->next;
		}
	}
	else{
		list = dlheader->tail;
		if(head_or_tail==1)
			add2Queue(list->x,list->y,1,sendQueue);
		else
			add2Queue(list->x,list->y,0,sendQueue);
		list = list->pre;
		while(list!=NULL){
			if(list->pre!=NULL)
				add2Queue(list->x,list->y,0,sendQueue);
			else{
				if(head_or_tail==2)
					add2Queue(list->x,list->y,1,sendQueue);
				else
					add2Queue(list->x,list->y,0,sendQueue);
			}
			list = list->pre;
		}
	}
}

void imageAnalyse::getPointsInfo(){
	addInitPoint();
	/**遍历node_list_vec3d
	*/
	vector<LIST_NODE_VEC>::iterator node_list_vec3d_it;
	LIST_NODE_VEC it_list_vec3d;

	vector<LIST_NODE*>::iterator list_node_vec_it;
	LIST_NODE* it_list_vec;
	for(node_list_vec3d_it=node_list_vec3d.begin();node_list_vec3d_it!=node_list_vec3d.end();node_list_vec3d_it++){
		//一条链
		it_list_vec3d = *node_list_vec3d_it;
		for(list_node_vec_it=it_list_vec3d.begin();list_node_vec_it!=it_list_vec3d.end();list_node_vec_it++){
			it_list_vec = *list_node_vec_it;
			if(it_list_vec->arc==NULL)
				break;
			if(list_node_vec_it==it_list_vec3d.begin())
				add2SendQueue(it_list_vec->node,it_list_vec->arc,1);
			//else if(list_node_vec_it==it_list_vec3d.end())
			//	add2SendQueue(it_list_vec->node,it_list_vec->arc,2);
			else{
				if(node_list_vec3d_it==node_list_vec3d.end() && list_node_vec_it==it_list_vec3d.end()){
///last change2->0
					add2SendQueue(it_list_vec->node,it_list_vec->arc,0);
				}
				else
					add2SendQueue(it_list_vec->node,it_list_vec->arc,0);
			}
			/*if(node_list_vec3d_it==node_list_vec3d.end()){
				if(list_node_vec_it==it_list_vec3d.end()){
					add2SendQueue(it_list_vec->node,it_list_vec->arc,1);
				}
			}*/
		}
	}
	getPanelTract();
	addEndPoint();
	isEnd = false;
}

void imageAnalyse::addInitPoint(){
	add2Queue(0,0,0,sendQueue);
	//add2Queue(0,0,1,sendQueue);
	//add2Queue(startX,startY,1,sendQueue);
}
void imageAnalyse::addEndPoint(){
	add2Queue(0,0,1,sendQueue);
	add2Queue(0,0,1,sendQueue);
	add2Queue(0,0,1,sendQueue);
	//add2Queue(0,0,0,sendQueue);
}

header* imageAnalyse::getList(){
	return sendQueue;
}

int imageAnalyse::getWidth(){
	return width;
}

int imageAnalyse::getHeight(){
	return height;
}

void imageAnalyse::getPacket(int length){
	int count = 0;
	p_queue* q;
	//CString p_data;
	TCHAR* szBuf = sendBuf;

	sendBufLen = 0;

	memset(sendBuf, 0, sizeof(TCHAR)*SEND_BUF_SIZE);

	while(count!=length){
		//p_data.Format(_T("%d%d%d"),q->x,q->y,q->z);
		//memcpy(szBuf, &p_data, sizeof(CString));
		
	//	memcpy(szBuf,&sendCount,2);
	//	sendCount++;
	//	szBuf++;
		q = getQueue(sendQueue);
		if(q==NULL)
		{


		}
		short temp = (short)q->x;
		memcpy(szBuf,&temp,sizeof(TCHAR));
		szBuf++;
		temp = (short)q->y;
		memcpy(szBuf,&temp,sizeof(TCHAR));
		szBuf++;
		temp = (short)q->z;
		memcpy(szBuf,&temp,sizeof(TCHAR));
		szBuf++;
		BYTE t;
		if(q->next == NULL)
			t = 1;
		else
			t = 0;
		//memcpy(szBuf,&t,sizeof(TCHAR));
		memcpy(szBuf,&t,1);

		//q = q->next;

		sendBufLen = sendBufLen + 6;

		count++;
		if(q->next == NULL){
			isEnd = true;
			break;
		}
	}
	sendBufLen++;
}

void imageAnalyse::copyPacket(TCHAR* szBuf){
	memset(szBuf, 0, sizeof(TCHAR)*SEND_BUF_SIZE);
	memcpy(szBuf, sendBuf, sizeof(TCHAR)*SEND_BUF_SIZE);
}

int imageAnalyse::getSendBufLen(){
	return sendBufLen;
}

void imageAnalyse::add2SendQueue(int x,int y, int z){
	add2Queue(x,y,z,sendQueue);
}

point* imageAnalyse::findNextPanel(int cur_type){
	for(int y=0;y<panel_h;y++){
		for(int x=0;x<panel_w;x++){
			//边缘~
			if(panel_data[y][x].val>=panel_val_thresh && panel_data[y][x].flag==0){
				if(panel_data[y][x].type==cur_type)
					return &panel_data[y][x];
			}
		}
	}
	return NULL;
}

point* imageAnalyse::getNextLine(int x,int y,direct dir,int next_x){
	int _y = y+1, _x = x;
	point* p=NULL;
	if(_y>=panel_h)
		return NULL;
	//正下方是有效像素点
	if(panel_data[_y][_x].val>=panel_val_thresh){
		if(dir==direct::right){
			while(_x+1<panel_w){
				if(panel_data[_y][_x+1].val>=panel_val_thresh && panel_data[_y][_x+1].flag==0)
					_x++;
				else
					break;
			}
		}
		else{
			while(_x-1>=0){
				if(panel_data[_y][_x-1].val>=panel_val_thresh && panel_data[_y][_x-1].flag==0)
					_x--;
				else
					break;
			}
		}
		p = &panel_data[_y][_x];
	}
	else{
		if(next_x!=-1)
			p = &panel_data[_y+1][next_x];
		else{
			if(_y+1<panel_h && x-1>=0 && panel_data[_y+1][x-1].val>=panel_val_thresh && panel_data[_y+1][x-1].flag==0)
				p = &panel_data[_y+1][x-1];
			if(_y+1<panel_h && x+1<panel_w && panel_data[_y+1][x+1].val>=panel_val_thresh && panel_data[_y+1][x+1].flag==0)
				p = &panel_data[_y+1][x+1];
		}
	}
	return p;
}

void imageAnalyse::getPanelTract(){
	int cur_type = 1;
	direct dir = direct::right;
	point *p=NULL;
	int x,y;
	int board_x,board_y;
	int next_x = -1;

	while(true){
		p = findNextPanel(cur_type);
		if(!p){
			cur_type++;
			p = findNextPanel(cur_type);
			if(!p)
				break;
		}
		x = p->x;
		y = p->y;
		add2SendQueue(x*panel_line , y*panel_line , 1);
		dir = direct::right;
		while(true){
			next_x = -1;
			y = p->y;
			if(dir==direct::right){
				for(x=p->x;x<panel_w;x++){
					if(panel_data[y][x].val>=panel_val_thresh && panel_data[y][x].flag==0){
						//add2SendQueue((x-1)*panel_line + panel_line/2, (y-1)*panel_line + panel_line/2, 0);
						add2SendQueue(x*panel_line , y*panel_line , 0);
						panel_data[y][x].flag = 1;

						if(y+1<panel_h && panel_data[y+1][x].val>=panel_val_thresh && panel_data[y+1][x].flag==0)
							next_x = x;
					}
					else{
						//不再是有效像素点，break
						p = getNextLine(x-1,y,dir,next_x);
						//change direct
						dir = direct::left;
						board_x = (x-1)*panel_line;
						board_y = y*panel_line;
						break;
					}
				}
				
			}
			else{
				for(x=p->x;x>=0;x--){
					if(panel_data[y][x].val>=panel_val_thresh && panel_data[y][x].flag==0){
						//add2SendQueue((x-1)*panel_line + panel_line/2, (y-1)*panel_line + panel_line/2, 0);
						add2SendQueue(x*panel_line , y*panel_line , 0);
						panel_data[y][x].flag = 1;

						if(y+1<panel_h && panel_data[y+1][x].val>=panel_val_thresh && panel_data[y+1][x].flag==0)
							next_x = x;
					}
					else{
						p = getNextLine(x+1,y,dir,next_x);
						//change direct
						dir = direct::right;
						board_x = (x+1)*panel_line;
						board_y = y*panel_line;
						break;
					}
				}
				
			}
			//no next line point
			if(!p){
//				add2SendQueue(board_x , board_y , 1);
				break;
			}
		}
	}	
}

bool imageAnalyse::bfs(point** arr,int x,int y,int threshold,int width, int height,int count){
	if(arr[y][x].type>0)
		return false;
	if(arr[y][x].val<panel_val_thresh)
		return false;
	int _x,_y;
	queue<search_node> q;
	search_node n(x,y);
	q.push(n);
	while(!q.empty()){
		n = q.front();
		q.pop();
		for(_x=n.x-1;_x<=n.x+1;_x++){
			for(_y=n.y-1;_y<=n.y+1;_y++){
				if(_x==x&&_y==y)
					continue;
				if(_x>=0 && _x<width && _y>=0 && _y<height){
					if(arr[_y][_x].val>=threshold && arr[_y][_x].type==0){
						search_node n1(_x,_y);
						q.push(n1);
						arr[_y][_x].type = count;
					}
				}
			}
		}
	}
	return true;
}


const char* imageAnalyse::CString2char(CString cstr)
{
	// 此处用到了宽字节的转换
	DWORD size; 
	size = WideCharToMultiByte(CP_OEMCP,NULL,cstr,-1,NULL,0,NULL,FALSE);
	char *newstr = new char[size];
	WideCharToMultiByte(CP_OEMCP,NULL,cstr,-1,newstr,size,NULL,FALSE);
	return newstr;
}

bool imageAnalyse::getFilePoint(const char* path)
{

	is_width_short = true;
	image_offset = 0;
	FILE *fd;
	int arr[4];
    fd = fopen(path,"rt");
	int cnt = 0;
    if(NULL == fd)
    {
      return false;  
    }
	int i;
    while(!feof(fd))
    {
		fscanf(fd,"%d : %d %d %d\n",&arr[0],&arr[1],&arr[2],&arr[3]);
		if(arr[0] == 1)
		{
			add2Queue(I_PANEL_WIDTH - arr[1],arr[2],arr[3],sendQueue); // 由于add2Queue中已进行了镜像翻转
			cnt++;
		}
    }
    fclose(fd);
	isEnd = false;
	printf("cnt=%d\n",cnt);
	return true;

}


void imageAnalyse::thresh(IplImage *src,IplImage *dst)
{
	// double cvThreshold( const void* srcarr, void* dstarr, double thresh, double maxval, int type );
	cvThreshold(src,dst,150,255,CV_THRESH_BINARY);
	//void cvAdaptiveThreshold( const void *srcIm, void *dstIm, double maxValue, int method, int type, int blockSize, double delta );
	//cvAdaptiveThreshold(src,dst,255,CV_ADAPTIVE_THRESH_GAUSSIAN_C  ,CV_THRESH_BINARY,10,15);
}
