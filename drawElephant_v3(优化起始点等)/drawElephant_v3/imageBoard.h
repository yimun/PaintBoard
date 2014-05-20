#pragma once
#include "chessboard.h"
#include "imageAnalyse.h"

class imageBoard :
	public chessBoard
{
public:
	imageBoard(void);
	~imageBoard(void);
	void showBoard(header* head);
};

