
// tdmDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "tdm.h"
#include "tdmDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define STR_CONNECT		_T("����(&C)")
#define STR_DISCONNECT  _T("�ر�(&X)")

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


// CtdmDlg �Ի���




CtdmDlg::CtdmDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CtdmDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CtdmDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_COM, m_ComboxCom);
	DDX_Control(pDX, IDC_COMBO_SPEED, m_ComboxSpeed);
}

BEGIN_MESSAGE_MAP(CtdmDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(BtnSend, &CtdmDlg::OnBnClickedBtnsend)
	ON_BN_CLICKED(IDC_BTN_CONNECT, &CtdmDlg::OnBnClickedBtnconnect)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, &CtdmDlg::OnBnClickedButtonRefresh)
	ON_BN_CLICKED(IDC_BUTTON_CHOOSE, &CtdmDlg::OnBnClickedButtonChooseFilePath)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_BRUSH, &CtdmDlg::OnBnClickedButtonBrush)
	ON_BN_CLICKED(IDC_BTN_OUTPUTTHIN, &CtdmDlg::OnBnClickedOutputThin)
	ON_BN_CLICKED(IDC_BTN_PNTFILE, &CtdmDlg::OnBnClickedPntFile)
	ON_BN_CLICKED(IDC_BTN_SIMULATE, &CtdmDlg::OnBnClickedSimulate)
	ON_BN_CLICKED(IDC_BTN_RESET, &CtdmDlg::OnBnClickedReset)
	ON_WM_SETFOCUS()
	ON_BN_CLICKED(IDC_BUTTON7, &CtdmDlg::OnBnClickedThresh)
	ON_BN_CLICKED(IDC_BUTTON8, &CtdmDlg::OnBnClickedThin)
	ON_BN_CLICKED(IDC_BUTTON9, &CtdmDlg::OnBnClickedResize)
END_MESSAGE_MAP()


// CtdmDlg ��Ϣ�������

BOOL CtdmDlg::OnInitDialog()
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
	// COM
	EnumPCSerialPorts(m_ComArray, FALSE);
	INT_PTR iCount = (INT_PTR)m_ComArray.size();
	m_ComboxCom.ResetContent();
	for(INT_PTR i = 0; i < iCount; i++)
	{
		m_ComboxCom.AddString(m_ComArray[i].strFriendlyName.c_str());
	}
	m_ComboxCom.SetCurSel(0);

	// SPEED
	m_ComboxSpeed.ResetContent();
	m_ComboxSpeed.AddString(_T("9600"));
	m_ComboxSpeed.AddString(_T("115200"));
	m_ComboxSpeed.SetCurSel(1);

	SetDlgItemText(IDC_BTN_CONNECT, STR_CONNECT);
	GetDlgItem(IDC_COMBO_COM)->EnableWindow(TRUE);
	GetDlgItem(IDC_COMBO_SPEED)->EnableWindow(TRUE);
//	GetDlgItem(BtnSend)->EnableWindow(FALSE);

	replyOK = true;
	chess.setHDC(m_hWnd);
	SetDlgItemText(IDC_EDIT_COUNT, _T("0"));
	falseCount = 0;
	count = 0;
	canbeSend = true;

	//remove("C:\\Users\\Administrator\\Documents\\Visual Studio 2010\\Projects\\jianianhua\\tdm\\recv_1.txt");
	//remove("C:\\Users\\Administrator\\Documents\\Visual Studio 2010\\Projects\\jianianhua\\tdm\\write_2.txt");
	//remove("C:\\Users\\Administrator\\Documents\\Visual Studio 2010\\Projects\\jianianhua\\tdm\\test.txt");
	remove("recv_1.txt");
	remove("test.txt");

	pre_ok_x = pre_ok_z = pre_ok_y = 0;
//	recieve_ok = true;
	allReady = false;
	analyseOk = false; // �ж��Ƿ�ͼƬ������ɣ���������־�������
	isSimulate = false;
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CtdmDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CtdmDlg::OnPaint()
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
HCURSOR CtdmDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CtdmDlg::OnBnClickedBtnsend()
{
	/*char image_path[MAX_PATH];
	strFile.Replace(_T("\\"), _T("\\\\"));
	int iLength ;  
	//��ȡ�ֽڳ���   
	iLength = WideCharToMultiByte(CP_ACP, 0, strFile, -1, NULL, 0, NULL, NULL);  
	//��tcharֵ����_char    
	WideCharToMultiByte(CP_ACP, 0, strFile, -1, image_path, iLength, NULL, NULL);

	image.image2Arr(image_path);
	image.arr2Graph();
	image.find_v2();
	image.getPointsInfo();

	chess.setHeight(image.getHeight());
	chess.setWidth(image.getWidth());
	chess.initBoard();*/
	
	TCHAR szBuf[10]={0x00};
	//for(int i=0;i<3;i++){
		image.getPacket(1);
		image.copyPacket(szBuf);

		int senfbuflen = image.getSendBufLen();
		WriteCom(SerialType_String,szBuf,senfbuflen);
	//}

	SetTimer(1,1000,NULL);
}


void CtdmDlg::OnBnClickedBtnconnect()
{
	if(!allReady)
	{
		AfxMessageBox(_T("ͼ��δ׼����")); 
		return;
	}
	chess.coverOld();
	if(!m_SerialManager.IsConnected())
	{
		// Com 
		INT iComCur = m_ComboxCom.GetCurSel();
		CString strPort = m_ComArray[iComCur].strPortName.c_str();
		
		// Speed
		DWORD dwRate;
		INT iComSpeed = m_ComboxSpeed.GetCurSel();
		if(iComSpeed == 0)
		{
			dwRate = CBR_9600;
		}
		else if(iComSpeed == 1)
		{
			dwRate = CBR_115200;
		}
		
		// Init
		if(m_SerialManager.Initialize(strPort, dwRate))
		{
			SetDlgItemText(IDC_BTN_CONNECT, STR_DISCONNECT);
			GetDlgItem(IDC_COMBO_COM)->EnableWindow(FALSE);
			GetDlgItem(IDC_COMBO_SPEED)->EnableWindow(FALSE);

			//
			m_SerialManager.SetPacketDispatch(TRUE);
			//m_SerialManager.AddDispatch(SerialType_String, _RoutineSerial, this);
			//m_SerialManager.AddDispatch(SerialType_Reply, _RoutineSerial, this);
			m_SerialManager.AddDispatch(Packet_Ok, _RoutineSerial, this);
			m_SerialManager.AddDispatch(CheckSum_Err, _RoutineSerial, this);
			m_SerialManager.AddDispatch(Buffer_Full, _RoutineSerial, this);
			m_SerialManager.AddDispatch(Buffer_NotFull, _RoutineSerial, this);
			m_SerialManager.AddDispatch(Head_Err, _RoutineSerial, this);
		}
		else
		{
			AfxMessageBox(_T("Init Serial Failed"));
		}
	}
	else
	{
		m_SerialManager.Close();

		SetDlgItemText(IDC_BTN_CONNECT, STR_CONNECT);
		GetDlgItem(IDC_COMBO_COM)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_SPEED)->EnableWindow(TRUE);
	}
}
DWORD WINAPI CtdmDlg::_RoutineSerial(LPVOID lpThreadParameter, LPBYTE pszBuffer, const DWORD dwLen)
{
	CtdmDlg* pManager = (CtdmDlg*)lpThreadParameter;
	return pManager->RoutineSerial((SerialType)pszBuffer[0], pszBuffer + 1, dwLen - 1);

	return 0;
}

VOID CtdmDlg::WriteCom( SerialType eType, PVOID pData, DWORD dwDataLen)
{
	/*CTime time = GetCurrentTime();
	SYSTEMTIME t1;
	GetSystemTime(&t1);
	CTime curTime(t1);
	WORD ms = t1.wMilliseconds; 

	WORD hour = time.GetHour();
	WORD min = time.GetMinute();
	WORD sed = time.GetSecond();*/

	TCHAR sdBuf[10]={0x00};
	memcpy(sdBuf,pData,dwDataLen);

//	fp2 = fopen("write_2.txt", "ab+");
//	fprintf(fp2,"%d %d : %d %d \r\n",dwDataLen, eType, sdBuf[0],sdBuf[1]);
//	fclose(fp2);

	LPBYTE pRealData = new BYTE[dwDataLen + 3];
	LPBYTE pPointer = pRealData;
	*(PWORD)pPointer = (WORD)dwDataLen + 1;
	pPointer += sizeof(WORD);
	*pPointer = eType;
	pPointer += sizeof(BYTE);
	memcpy(pPointer, pData, dwDataLen);
	//m_SerialManager.WriteCom(dwDataLen + 3, pRealData, hour, min, sed, ms);
	m_SerialManager.WriteCom(dwDataLen + 3, pRealData);
	SAFE_DELETE_AR(pRealData);
	delete[] pRealData;
}

DWORD CtdmDlg::RoutineSerial( SerialType eType, LPBYTE pszBuffer, const DWORD dwLen )
{
	LPBYTE pText = (LPBYTE)pszBuffer;
	TCHAR sdBuf[10]={0x00};
	TCHAR recv[10]={0x00};
	short x1,y1,z1;
	int senfbuflen;
	switch (eType){
	case Packet_Ok: //1
		falseCount = 0;
		KillTimer(1);

		memcpy(recv,pText,6);
		fp1 = fopen("recv_1.txt", "ab+");
		fprintf(fp1,"%d : %d %d %d\r\n", eType, recv[0],recv[1],recv[2]);
		fclose(fp1);
		fp3 = fopen("test.txt", "ab+");
		fprintf(fp3,"%d : %d %d %d\r\n", eType,recv[0],recv[1],recv[2]);
		fclose(fp3);
		memcpy(&x1,recv,2);
		memcpy(&y1,&recv[1],2);
		memcpy(&z1,&recv[2],2);
		chess.cover(x1,y1);
		showCurrPnt(x1,y1,z1);
		/***************************/
		pre_ok_x = recv[0];
		pre_ok_y = recv[1];
		pre_ok_z = recv[2];
		/***************************/
		//SetTimer(1,1000,NULL);
		////////////////
//		recieve_ok = true;
		////////////////
		break;

	case Head_Err://5
	case CheckSum_Err://2
		KillTimer(1);
		image.copyPacket(sdBuf);
		fp3 = fopen("test.txt", "ab+");
		fprintf(fp3,"%d: %d %d %d\r\n", eType,sdBuf[0],sdBuf[1],sdBuf[2]);
		fclose(fp3);

		falseCount++;
		if(falseCount==10){
			if(!image.isEnd)
				image.getPacket(1);
			falseCount = 0;
			image.copyPacket(sdBuf);
		}
		
		senfbuflen = image.getSendBufLen();
		WriteCom(SerialType_String,sdBuf,senfbuflen);
		////////////////
//		recieve_ok = false;
		////////////////
		SetTimer(1,1000,NULL);
		break;
	case Buffer_Full://3
		KillTimer(1);
		image.copyPacket(sdBuf);
		fp3 = fopen("test.txt", "ab+");
		fprintf(fp3,"%d: %d %d %d\r\n", eType,sdBuf[0],sdBuf[1],sdBuf[2]);
		fclose(fp3);
		////////////////
//		recieve_ok = false;
		////////////////
		break;
	case Buffer_NotFull://4
//		if(!image.isEnd)
//			image.getPacket(1);
//		image.copyPacket(sdBuf);
		/**************NEW ADD**************/
		image.copyPacket(sdBuf);
		if(pre_ok_x==sdBuf[0] && pre_ok_y==sdBuf[1] && pre_ok_z==sdBuf[2]){ 
			if(!image.isEnd){
				image.getPacket(1);
			}
		}
		image.copyPacket(sdBuf);
/*		if(recieve_ok){
			if(!image.isEnd)
				image.getPacket(1);
		}
		image.copyPacket(sdBuf);*/
		//////////////////////////////////////////
		fp3 = fopen("test.txt", "ab+");
		fprintf(fp3,"%d: %d %d %d\r\n", eType,sdBuf[0],sdBuf[1],sdBuf[2]);
		fclose(fp3);

		senfbuflen = image.getSendBufLen();
		WriteCom(SerialType_String,sdBuf,senfbuflen);
		SetTimer(1,1000,NULL);
		////////////////
//		recieve_ok = false;
		////////////////
		break;
	default:
		fp3 = fopen("test.txt", "ab+");
		fprintf(fp3,"unknow_err \r\n");
		fclose(fp3);
		////////////////
//		recieve_ok = false;
		////////////////
		break;
	}

	return 0;
}

void CtdmDlg::OnTimer(UINT_PTR nIDEvent){
	switch(nIDEvent){
	case 1: // ͨ�ŵȴ���ʱ��
		{
		TCHAR sdBuf[10]={0x00};
		image.copyPacket(sdBuf);
		int senfbuflen = image.getSendBufLen();
		WriteCom(SerialType_String,sdBuf,senfbuflen);
		break;
		}
	case 2: // ģ�����ж�ʱ��
		{
		if(isSimulate && p != NULL){
			chess.cover(p->x,p->y);
			showCurrPnt(p->x,p->y,p->z);
			p = p->next;
		}else{
			KillTimer(2);
			isSimulate = false;
		}
		break;
		}
	default:
		CDialogEx::OnTimer(nIDEvent);
	}
	
}


void CtdmDlg::OnBnClickedButtonRefresh()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// COM
	//
	m_ComboxCom.ResetContent();
	EnumPCSerialPorts(m_ComArray, FALSE);
	INT_PTR iCount = (INT_PTR)m_ComArray.size();
	for(INT_PTR i = 0; i < iCount; i++)
	{
		m_ComboxCom.AddString(m_ComArray[i].strFriendlyName.c_str());
	}
	m_ComboxCom.SetCurSel(0);
}


void CtdmDlg::OnBnClickedButtonChooseFilePath()
{
	strFile = _T("");

    CFileDialog    dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("Describe Files (All Files (*.*)|*.*||"), NULL);

    if (dlgFile.DoModal())
    {
        strFile = dlgFile.GetPathName();
		GetDlgItem(BtnSend)->EnableWindow(TRUE);
		
		char image_path[MAX_PATH];
		strFile.Replace(_T("\\"), _T("\\\\"));
		int iLength ;  
		
		//��ȡ�ֽڳ���   
		iLength = WideCharToMultiByte(CP_ACP, 0, strFile, -1, NULL, 0, NULL, NULL);  
		if(iLength < 2)
		{
			AfxMessageBox(_T("ͼƬѡ�����"));
			return;
		}
		AfxMessageBox(strFile); 
		//��tcharֵ����_char    
		WideCharToMultiByte(CP_ACP, 0, strFile, -1, image_path, iLength, NULL, NULL);

		image.image2Arr(image_path);
		image.arr2Graph();
		image.find_v2();
		image.getPointsInfo();
		analyseOk = true;

		chess.initBoard(image.getList());
		allReady = true;
		//SetTimer(1,1000,NULL);
    }

}


void CtdmDlg::OnBnClickedButtonBrush()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	TCHAR sdBuf[1] = {0x00};
	WriteCom(Use_Brush,sdBuf,sizeof(TCHAR));
}



void CtdmDlg::OnBnClickedPntFile()
{

	strFile = _T("");

    CFileDialog    dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("Describe Files (All Files (*.*)|*.*||"), NULL);

    if (!dlgFile.DoModal())
    {
		AfxMessageBox(_T("δѡ���ļ�")); 
		return;
    }
	strFile = dlgFile.GetPathName();
	GetDlgItem(IDC_BUTTON5)->EnableWindow(TRUE);
	AfxMessageBox(strFile); 
	
	
	const char *path = image.CString2char(strFile);

	if(!image.getFilePoint(path))
	{
		AfxMessageBox(_T("���ļ���ȡ����")); 
		return;
	}
	//analyseOk = true;
	chess.initBoard(image.getList());
	allReady = true;
}


void CtdmDlg::OnBnClickedSimulate()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(!allReady)
	{
		AfxMessageBox(_T("ͼ��δ׼����")); 
		return;
	}
	p = image.getList()->front;
	chess.coverOld();
	if(!isSimulate)
	{
		SetTimer(2,10,NULL);
		isSimulate = true;
	}
}

void CtdmDlg::OnBnClickedReset()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnInitDialog();
	KillTimer(1);
	Invalidate();
	chess.clearBuff();
	image.init();
	

}


void CtdmDlg::OnSetFocus(CWnd* pOldWnd)
{
	CDialogEx::OnSetFocus(pOldWnd);
	// TODO: �ڴ˴������Ϣ����������
	if(allReady)
	{
		chess.rePaint();
	}

}


void CtdmDlg::OnBnClickedOutputThin()
{
	if(!analyseOk){
		AfxMessageBox(_T("δ����ͼƬ��"));
		return;
	}
	CString suffix;
	CString path;
	AfxExtractSubString(path,strFile,0,'.');
	AfxExtractSubString(suffix,strFile,1,'.');
	CString newPath = path + _T("_AfterThin.") + suffix;
	if(cvSaveImage(image.CString2char(newPath),image.thinImg))
		AfxMessageBox(_T("���浽 ")+newPath);
	else
		AfxMessageBox(_T("�洢ʧ��"));
}


void CtdmDlg::OnBnClickedThresh()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(!analyseOk){
		AfxMessageBox(_T("δ����ͼƬ��"));
		return;
	}
	cvNamedWindow("ImShow", CV_WINDOW_NORMAL ); // ��ͬһ�������ܹ�������ͬ�����ű���
	cvShowImage("ImShow" , image.threshImg);
	cvWaitKey(0);
	cvDestroyWindow("ImShow");

}


void CtdmDlg::OnBnClickedThin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(!analyseOk){
		AfxMessageBox(_T("δ����ͼƬ��"));
		return;
	}
	cvNamedWindow("ImShow", CV_WINDOW_NORMAL );
	cvShowImage("ImShow" , image.thinImg);
	cvWaitKey(0);
	cvDestroyWindow("ImShow");
}


void CtdmDlg::OnBnClickedResize()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(!analyseOk){
		AfxMessageBox(_T("δ����ͼƬ��"));
		return;
	}
	cvNamedWindow("ImShow", CV_WINDOW_NORMAL );
	cvShowImage("ImShow" , image.resizeImg);
	cvWaitKey(0);
	cvDestroyWindow("ImShow");
}


void CtdmDlg::showCurrPnt(int x, int y, int z)
{
	CString sx,sy,sz;
	sx.Format(_T("%d"),x);
	sy.Format(_T("%d"),y);
	sz.Format(_T("%d"),z);
	SetDlgItemText(IDC_EDIT_X,sx);
	SetDlgItemText(IDC_EDIT_Y,sy);
	SetDlgItemText(IDC_EDIT_Z,sz);

}
