
// drawElephant_v3Dlg.h : 头文件
//
#include "imageAnalyse.h"
#include "chessBoard.h"
#include "imageBoard.h"

#pragma once


// CdrawElephant_v3Dlg 对话框
class CdrawElephant_v3Dlg : public CDialogEx
{
// 构造
public:
	CdrawElephant_v3Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_DRAWELEPHANT_V3_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持 


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedAnalyse();
	afx_msg void OnBnClickedBegin();
	afx_msg void OnBnClickedPause();
	afx_msg void OnBnClickedChangeInterval();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	bool isInited;
	bool isRun;
	int interval;

	CString strFile;     //存放选择的目录路径

	chessBoard chessboard;
	imageBoard imageboard;
	imageAnalyse image;
private:
	header* head;
	queue* p;
public:
	afx_msg void OnBnClickedButtonChooseFilePath();
	afx_msg void OnBnClickedReset();
};
