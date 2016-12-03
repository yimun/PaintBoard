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

#ifndef _THREADEX_H_
#define _THREADEX_H_

//
#include "../Include.h"

//
#ifdef UNDER_CE
	#define BeginThread CreateThread
#else
	typedef unsigned (__stdcall *PTHREAD_START)(void *);
	#include <process.h>

	/*
	#define BeginThread(lpThreadAttributes, dwStackSize,			\
		lpStartAddress,	lpParameter, dwCreationFlags, lpThreadId)	\
		((HANDLE)_beginthreadex(									\
			(void*)(lpThreadAttributes),							\
			(unsigned)(dwStackSize),								\
			(PTHREAD_START)(lpStartAddress),						\
			(void*)(lpParameter),									\
			(unsigned)(dwCreationFlags),							\
			(unsigned*)(lpThreadId)))
	*/

	//
	#define BeginThread CreateThread
#endif

namespace Utils
{
	//
	#define THREAD_EVENT_INIT		0
	#define THREAD_EVENT_ROUTINE	1
	#define THREAD_EVENT_NOTIFY		2
	#define THREAD_EVENT_EXIT		3

	/*
	//
	class CThreadExCallBack
	{
	public:
		// Read Call Back
		virtual BOOL OnRoutine(DWORD dwFlags) = 0;
	};
	*/

	typedef VOID (WINAPI *PTHREAD_CALLBACK_ROUTINE)(
		LPVOID lpThreadParameter,
		DWORD  dwThreadEventCode,
		LPVOID lpRet
		);
	typedef PTHREAD_CALLBACK_ROUTINE LPTHREAD_PCALLBACK_ROUTINE;

	//
	class CThreadEx
	{
	public:
		CThreadEx();
		// Don't Forget Before Destroy it, call Close();
		~CThreadEx();

	public:
		//
		static DWORD WINAPI RoutineThread(LPVOID lpParam);
		//
		BOOL Initialize(PTHREAD_CALLBACK_ROUTINE pCallBackRoutine, LPVOID pParam, DWORD dwThreadFlags);
		//
		void Close();
		//
		DWORD GetThreadId();
		HANDLE GetThread();
	private:
		//
		DWORD		m_dwThreadId;
		//
		HANDLE		m_hThread;
		// 
		HANDLE		m_hWantCloseThread;
		//
		BOOL		m_bThreadAlive;
		// 
		HANDLE		m_hThreadClosed;
		//
		PTHREAD_CALLBACK_ROUTINE m_pCallBackRoutine;
		LPVOID		m_pParam;
		DWORD		m_dwThreadFlags;
		//
		HANDLE		m_hEventArray[2];

	};

}
#endif