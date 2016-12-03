//
// C++ Interface: PCCommEngine
//
// Description: 
//
// Author:    xltyt, <xltytlive@hotmail.com>, (C) 2009
// Created:   2009-3-6
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef _COMM_ENGINE_H
#define _COMM_ENGINE_H

#ifndef UNDER_CE

#include "../Include.h"

namespace Device
{
	namespace PCCommEngine
	{
		//定义最大数据块
		#define MAXBLOCK	4096

		class CPCCommEngine
		{
		public:
			CPCCommEngine(void);
			~CPCCommEngine(void);

		public:
			BOOL	IsConnected();
			BOOL	OpenComm(LPCTSTR pszPort);
			VOID	SetDCBRate(const DWORD BaudRate);
			BOOL	SetDCB(DWORD dwBaudRate);
			BOOL	CloseComm();
			DWORD	ReadComm(LPVOID cpRXBuffer, OVERLAPPED* olRead);
			DWORD	WriteComm(LPCVOID lpBuffer, DWORD dwLen);

			BOOL	Config();
			HANDLE	GetCommFile()
			{
				return this->m_hCommFile;
			}
			LPCTSTR GetPort() const;

		private:
			BOOL	m_bConnected;
			DCB		m_dConfig;
			// The object that is a instance of port. 
			HANDLE 	m_hCommFile;
			//
			TCHAR	m_szPort[64];
		};

		class CPCCommEngineCallBack
		{
		public:
			// Read Call Back
			virtual VOID OnReadCommCompeleted(LPBYTE pszBuffer, const DWORD dwLen) = 0;

			// Write Call Back
			virtual VOID OnWriteCommCompeleted(LPCVOID lpBuffer, const DWORD dwLen) = 0;
		};


		class CPCCommEngineCtrl
		{
		public:
			CPCCommEngineCtrl();
			~CPCCommEngineCtrl();

		public:
			// Init the Thread and COM Engine
			BOOL Initialize(CPCCommEngineCallBack* notify, LPCTSTR pszPort);
			// Write Data To COM
			DWORD WriteComm(DWORD aLen, LPCVOID aData);
			//
			VOID Close();
			BOOL ClearBuffer();
			//
			CPCCommEngine* GetSerialEngine();

		private:
			// Static Method of Thread
			static DWORD WINAPI ComEngineReadThread(LPVOID lpParam);

		public:
			// CallBack
			CPCCommEngineCallBack*	m_pEngineCallback;
			// Comm Engine
			CPCCommEngine			m_CommEngine;
			// Active Exit Thread
			//BOOL					m_bExitThread;

			HANDLE					m_hThread;

			// Handle to Close COM Thread
			HANDLE					m_hEventCloseThread;
			BOOL					m_bThreadAlive;		//串口辅助线程活动标志
		};
	}
}
#endif // #ifndef UNDER_CE
#endif