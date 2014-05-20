
// drawElephant_v3Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "drawElephant_v3.h"
#include "drawElephant_v3Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CdrawElephant_v3Dlg 对话框




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


// CdrawElephant_v3Dlg 消息处理程序

BOOL CdrawElephant_v3Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	(CButton *)GetDlgItem(IDC_BUTTON5)->EnableWindow(false);		
	(CButton *)GetDlgItem(IDC_BUTTON1)->EnableWindow(false);		//开始
	(CButton *)GetDlgItem(IDC_BUTTON2)->EnableWindow(false);	//暂停

	chessboard.setHDC(m_hWnd);
	imageboard.setHDC(m_hWnd);

	isInited = false;
	isRun = false;
	SetDlgItemText(IDC_EDIT1,_T("1"));
	interval = 1;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CdrawElephant_v3Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CdrawElephant_v3Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CdrawElephant_v3Dlg::OnBnClickedAnalyse()
{
	// TODO: 在此添加控件通知处理程序代码
	char image_path[MAX_PATH];
	strFile.Replace(_T("\\"), _T("\\\\"));
	int iLength ;  
	//获取字节长度   
	iLength = WideCharToMultiByte(CP_ACP, 0, strFile, -1, NULL, 0, NULL, NULL);  
	//将tchar值赋给_char    
	WideCharToMultiByte(CP_ACP, 0, strFile, -1, image_path, iLength, NULL, NULL);

	image.image2Arr(image_path); // 图像读取，细化
	image.arr2Graph(); // 找边
	image.find_v2();
	image.getPointsInfo();
	isInited = true;
	//可以开始了
	(CButton *)GetDlgItem(IDC_BUTTON1)->EnableWindow(true);
	(CButton *)GetDlgItem(IDC_BUTTON2)->EnableWindow(true);
	//设置宽高
	chessboard.setWidth(image.getWidth());
	chessboard.setHeight(image.getHeight());
	chessboard.initBoard();
	//拿到分析图像数据
	head = image.getList();
	p = head->front;

	imageboard.setWidth(image.getWidth());
	imageboard.setHeight(image.getHeight());
	imageboard.showBoard(head);

}


void CdrawElephant_v3Dlg::OnBnClickedBegin()
{
	// TODO: 在此添加控件通知处理程序代码
	if(!isInited){
		MessageBox(_T("请先分析图像！"));
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
	// TODO: 在此添加控件通知处理程序代码
	if(isRun){
		isRun = false;
		SetDlgItemText(IDC_BUTTON2,_T("继续"));
		((CButton *)GetDlgItem(IDC_BUTTON2))->EnableWindow(true);//暂停
	}
	else{
		isRun = true;
		SetDlgItemText(IDC_BUTTON2,_T("暂停"));
		((CButton *)GetDlgItem(IDC_BUTTON2))->EnableWindow(true);//暂停
	}
}


void CdrawElephant_v3Dlg::OnBnClickedChangeInterval()
{
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
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
