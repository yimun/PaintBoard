
// drawElephant_v3.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CdrawElephant_v3App:
// �йش����ʵ�֣������ drawElephant_v3.cpp
//

class CdrawElephant_v3App : public CWinApp
{
public:
	CdrawElephant_v3App();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CdrawElephant_v3App theApp;