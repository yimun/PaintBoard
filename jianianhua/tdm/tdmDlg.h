
// tdmDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "Device/SerialManager.h"
#include "Device/PCEnumSerial.h"
#include "ImageAnalyse.h"
#include "chessBoard.h"
#include "afx.h"
using namespace Device::PCCommEngine;
using namespace Device;


enum SerialType
{
	SerialType_String = 0,

	Packet_Ok = 1,

	CheckSum_Err = 2,

	Buffer_Full = 3,

	Buffer_NotFull = 4,

	Head_Err = 5,

	Use_Brush = 6
};

// CtdmDlg �Ի���
class CtdmDlg : public CDialogEx
{
// ����
public:
	CtdmDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TDM_DIALOG };

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
	afx_msg void OnBnClickedBtnsend();
	afx_msg void OnBnClickedBtnconnect();
	afx_msg void OnBnClickedBAnalyse();

	afx_msg void OnTimer(UINT_PTR nIDEvent);

	VOID WriteCom(SerialType eType, PVOID pData, DWORD dwDataLen);
	CSerialManager m_SerialManager;
	vector<SPCSerInfo>	m_ComArray;
	static DWORD WINAPI _RoutineSerial(LPVOID lpThreadParameter, LPBYTE pszBuffer, const DWORD dwLen);
	DWORD RoutineSerial(SerialType eType, LPBYTE pszBuffer, const DWORD dwLen);
	CComboBox m_ComboxCom;
	CComboBox m_ComboxSpeed;
	imageAnalyse image;

	chessBoard chess;
	bool replyOK;
	int falseCount;
	int count;
	bool canbeSend;
	bool analyseOk; 
	bool isSimulate;
	bool allReady;
	//FILE *fp1,*fp2, *fp3;
	FILE *fp1, *fp3;

	CString strFile;     //���ѡ���Ŀ¼·��   
	p_queue *p;

	afx_msg void OnBnClickedButtonRefresh();
	afx_msg void OnBnClickedButtonChooseFilePath();
	afx_msg void OnBnClickedButtonBrush();

	//FILE *fp2;
	int pre_ok_x,pre_ok_y,pre_ok_z;
	//bool recieve_ok;
	afx_msg void OnBnClickedOutputThin();
	afx_msg void OnBnClickedPntFile();
	afx_msg void OnBnClickedSimulate();
	afx_msg void OnBnClickedReset();
	afx_msg void OnSetFocus(CWnd* pOldWnd); // �����ػ񽹵�
	afx_msg void OnBnClickedThresh();
	afx_msg void OnBnClickedThin();
	afx_msg void OnBnClickedResize();
	void showCurrPnt(int x, int y, int z);
};
