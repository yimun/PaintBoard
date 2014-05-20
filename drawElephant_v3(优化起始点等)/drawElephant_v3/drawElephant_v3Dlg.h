
// drawElephant_v3Dlg.h : ͷ�ļ�
//
#include "imageAnalyse.h"
#include "chessBoard.h"
#include "imageBoard.h"

#pragma once


// CdrawElephant_v3Dlg �Ի���
class CdrawElephant_v3Dlg : public CDialogEx
{
// ����
public:
	CdrawElephant_v3Dlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_DRAWELEPHANT_V3_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧�� 


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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

	CString strFile;     //���ѡ���Ŀ¼·��

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
