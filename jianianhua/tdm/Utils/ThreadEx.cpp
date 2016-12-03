//
// C++ Interface: ThreadEx
//
// Description: 
//
// Author:    xltyt, <xltytlive@hotmail.com>, (C) 2009
// Created:   2009-3-6
//
// Copyright: See COPYING file that comes with this distribution
//
#include "stdafx.h"
#include "ThreadEx.h"
#include "Debug.h"
#include "GlobalUtility.h"
using namespace Utils;
#include "debug.h"
using namespace Utils::EasyDebug;

//
namespace Utils
{
	CThreadEx::CThreadEx()
	{
		this->m_dwThreadId			= 0;
		this->m_hThread				= NULL;
		this->m_hWantCloseThread	= CGlobalUtility::CreateEvent();
		this->m_hThreadClosed		= CGlobalUtility::CreateEvent();
		this->m_bThreadAlive		= FALSE;

		this->m_pCallBackRoutine	= NULL;
		this->m_pParam				= NULL;
		this->m_dwThreadFlags		= 0;

		//
		this->m_hEventArray[0]		= this->m_hWantCloseThread;
	}

	CThreadEx::~CThreadEx()
	{
		//
		this->Close();
		//
		//SAFE_CLOSE(this->m_hThreadClosed);
		CloseHandle(this->m_hThreadClosed);

		//
		//SAFE_CLOSE(this->m_hWantCloseThread);
		CloseHandle(this->m_hWantCloseThread);

		//
		PrintEx(_T("[Thread Object Deleted]\n"));
	}

	void CThreadEx::Close()
	{
		if(m_dwThreadId != 0)
		{
			//
			SetEvent(this->m_hWantCloseThread);
			SetEvent(this->m_hEventArray[1]);
			
			//
			while(1)
			{
				DWORD dwRet = WaitForSingleObject(this->m_hThreadClosed, 0);
				if(dwRet == WAIT_OBJECT_0)
				{
					break;
				}
			}
						
			this->m_dwThreadId			= 0;
			this->m_hThread				= 0;
			this->m_bThreadAlive		= FALSE;
			this->m_dwThreadFlags		= 0;
		}
	}

	BOOL CThreadEx::Initialize(PTHREAD_CALLBACK_ROUTINE pCallBackRoutine, LPVOID pParam, DWORD dwThreadFlags)
	{
		//
		if(pCallBackRoutine == NULL || dwThreadFlags == 0)
		{
			return FALSE;
		}
		//
		this->m_pParam = pParam;
		this->m_pCallBackRoutine = pCallBackRoutine;
		this->m_dwThreadFlags = dwThreadFlags;

		this->m_hThread = BeginThread(
			NULL, 0,
			CThreadEx::RoutineThread,
			(LPVOID)this, 0, &this->m_dwThreadId);
		if(this->m_hThread == NULL)
		{
			return FALSE;			
		}
		CloseHandle(this->m_hThread);
		//
		return TRUE;
	}

	//
	DWORD WINAPI CThreadEx::RoutineThread(LPVOID lpParam)
	{
		DWORD dwEvent;
		CThreadEx* pThread = (CThreadEx*)lpParam;
		//
		pThread->m_bThreadAlive = TRUE;
		HANDLE hEvent = NULL;
		pThread->m_pCallBackRoutine(pThread->m_pParam, THREAD_EVENT_INIT, &hEvent);
		// Wait Handle		
		pThread->m_hEventArray[1] = hEvent;
		//
		BOOL bExit = FALSE;
		ResetEvent(pThread->m_hThreadClosed);
		for(;;)
		{
			//
			Sleep(1);
			//
			BOOL bSucceed = FALSE;
			
			//PrintEx(_T("Thread Begin Routine\n"));
			pThread->m_pCallBackRoutine(pThread->m_pParam, THREAD_EVENT_ROUTINE, &bSucceed);
			//PrintEx(_T("Thread End Routine\n"));
			if(bSucceed == FALSE)
			{
				break;
			}

			//
			// Wait For Multiple Objects
			//
			//PrintEx(_T("Thread Begin Wait\n"));
			dwEvent = WaitForMultipleObjects(2, pThread->m_hEventArray, FALSE, INFINITE);
			//PrintEx(_T("Thread End Wait\n"));
			switch (dwEvent)
			{
			case 0:
				{
					bExit = TRUE;
				}
				break;
			case 1:
				{
					//PrintEx(_T("Thread Begin Notify\n"));
					pThread->m_pCallBackRoutine(pThread->m_pParam, THREAD_EVENT_NOTIFY, NULL);
					//PrintEx(_T("Thread End Notify\n"));
				}
				break;
			case WAIT_FAILED:
				{
					//DWORD dwError = GetLastError();
					//DebugBreak();
					bExit = TRUE;
				}
				break;
			default:
				bExit = TRUE;
				break;
			}

			if(bExit)	
			{
				break;
			}
		}
		//
		//PrintEx(_T("Thread Begin Exit\n"));
		pThread->m_pCallBackRoutine(pThread->m_pParam, THREAD_EVENT_EXIT, NULL);
		//PrintEx(_T("Thread End Exit\n"));

		//
		//LPCSTR pMacroFile = __FILE__;
		//NGlobalUtils::CDebug::Print(_T("Thread \'%S\' Exit, Code %d(0x%X)\n"), 
		//	__FUNCTION__, pThread->m_dwThreadFlags, pThread->m_dwThreadFlags);

		//
		pThread->m_bThreadAlive = FALSE;
		//
		DWORD dwRet = pThread->m_dwThreadFlags;
		SetEvent(pThread->m_hThreadClosed);
		//
		return dwRet;
	}
	
	DWORD CThreadEx::GetThreadId()
	{
		return m_dwThreadId;
	}

	HANDLE CThreadEx::GetThread()
	{
		return m_hThread;
	}
}

