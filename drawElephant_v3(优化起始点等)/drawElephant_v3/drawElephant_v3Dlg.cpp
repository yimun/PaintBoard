
// drawElephant_v3Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "drawElephant_v3.h"
#include "drawElephant_v3Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CdrawElephant_v3Dlg �Ի���




CdrawElephant_v3Dlg::CdrawElephant_v3Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CdrawElephant_v3Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CdrawElephant_v3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CdrawElephant_v3Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON5, &CdrawElephant_v3Dlg::OnBnClickedAnalyse)
	ON_BN_CLICKED(IDC_BUTTON1, &CdrawElephant_v3Dlg::OnBnClickedBegin)
	ON_BN_CLICKED(IDC_BUTTON2, &CdrawElephant_v3Dlg::OnBnClickedPause)
	ON_BN_CLICKED(IDC_BUTTON6, &CdrawElephant_v3Dlg::OnBnClickedChangeInterval)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_CHOOSE, &CdrawElephant_v3Dlg::OnBnClickedButtonChooseFilePath)
	ON_BN_CLICKED(IDC_BUTTON4, &CdrawElephant_v3Dlg::OnBnClickedReset)
END_MESSAGE_MAP()


// CdrawElephant_v3Dlg ��Ϣ�������

BOOL CdrawElephant_v3Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	(CButton *)GetDlgItem(IDC_BUTTON5)->EnableWindow(false);		
	(CButton *)GetDlgItem(IDC_BUTTON1)->EnableWindow(false);		//��ʼ
	(CButton *)GetDlgItem(IDC_BUTTON2)->EnableWindow(false);	//��ͣ

	chessboard.setHDC(m_hWnd);
	imageboard.setHDC(m_hWnd);

	isInited = false;
	isRun = false;
	SetDlgItemText(IDC_EDIT1,_T("1"));
	interval = 1;

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CdrawElephant_v3Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CdrawElephant_v3Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CdrawElephant_v3Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CdrawElephant_v3Dlg::OnBnClickedAnalyse()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	char image_path[MAX_PATH];
	strFile.Replace(_T("\\"), _T("\\\\"));
	int iLength ;  
	//��ȡ�ֽڳ���   
	iLength = WideCharToMultiByte(CP_ACP, 0, strFile, -1, NULL, 0, NULL, NULL);  
	//��tcharֵ����_char    
	WideCharToMultiByte(CP_ACP, 0, strFile, -1, image_path, iLength, NULL, NULL);

	image.image2Arr(image_path); // ͼ���ȡ��ϸ��
	image.arr2Graph(); // �ұ�
	image.find_v2();
	image.getPointsInfo();
	isInited = true;
	//���Կ�ʼ��
	(CButton *)GetDlgItem(IDC_BUTTON1)->EnableWindow(true);
	(CButton *)GetDlgItem(IDC_BUTTON2)->EnableWindow(true);
	//���ÿ��
	chessboard.setWidth(image.getWidth());
	chessboard.setHeight(image.getHeight());
	chessboard.initBoard();
	//�õ�����ͼ������
	head = image.getList();
	p = head->front;

	imageboard.setWidth(image.getWidth());
	imageboard.setHeight(image.getHeight());
	imageboard.showBoard(head);

}


void CdrawElephant_v3Dlg::OnBnClickedBegin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(!isInited){
		MessageBox(_T("���ȷ���ͼ��"));
		return;
	}
	CString str;
	GetDlgItemText(IDC_EDIT1,str);
	interval = _tstoi(str);
	if(interval<=0 || interval > 3000)
	{
		interval = 1000;
		SetDlgItemText(IDC_EDIT1,_T("1000"));
	}
	SetTimer(1,interval,NULL);
	isRun = true;
}


void CdrawElephant_v3Dlg::OnBnClickedPause()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(isRun){
		isRun = false;
		SetDlgItemText(IDC_BUTTON2,_T("����"));
		((CButton *)GetDlgItem(IDC_BUTTON2))->EnableWindow(true);//��ͣ
	}
	else{
		isRun = true;
		SetDlgItemText(IDC_BUTTON2,_T("��ͣ"));
		((CButton *)GetDlgItem(IDC_BUTTON2))->EnableWindow(true);//��ͣ
	}
}


void CdrawElephant_v3Dlg::OnBnClickedChangeInterval()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str;
	GetDlgItemText(IDC_EDIT1,str);
	interval = _tstoi(str);
	if(interval<=0 || interval > 3000)
	{
		interval = 1000;
		SetDlgItemText(IDC_EDIT1,_T("1000"));
	}
	SetTimer(1,interval,NULL);
}

void CdrawElephant_v3Dlg::OnTimer(UINT_PTR nIDEvent){
	if(isRun){
		chessboard.cover(p->x,p->y);
		CString x,y;
		x.Format(_T("%d"),p->x);
		y.Format(_T("%d"),p->y);
		SetDlgItemText(IDC_EDITX,x);
		SetDlgItemText(IDC_EDITY,y);
		p = p->next;
		if(p==NULL){
			isRun = false;
			isInited = false;
			KillTimer(1);
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CdrawElephant_v3Dlg::OnBnClickedButtonChooseFilePath()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	strFile = _T("");

    CFileDialog    dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("Describe Files (All Files (*.*)|*.*||"), NULL);

    if (dlgFile.DoModal())
    {
        strFile = dlgFile.GetPathName();
		GetDlgItem(IDC_BUTTON5)->EnableWindow(TRUE);
		AfxMessageBox(strFile); 
    }
}


void CdrawElephant_v3Dlg::OnBnClickedReset()
{
	OnInitDialog(); 

}
