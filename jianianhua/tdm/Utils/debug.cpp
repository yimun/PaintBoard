//
// C++ Implementation: debug
//
// Description: 
//
//
// Author: gaoliang User,,, <gaoliang@gaoliang-desktop>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//

//
// Author:    xltyt, <xltytlive@hotmail.com>, (C) 2009
// Created:   2009-3-6
//
// Copyright: See COPYING file that comes with this distribution
//
//#define WIN32_LEAN_AND_MEAN
//#include <Windows.h>

#include "stdafx.h"
#include "debug.h"

#define WIN32_LEAN_AND_MEAN
#include <WinSock2.h>
#include <Windows.h>

#include <WinBase.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdlib.h>
#include <tchar.h>
#include "macro.h"
#pragma warning(push)
// 4996: “vswprintf”被声明为否决的
#pragma warning(disable:4996)

namespace Utils
{
	namespace EasyDebug
	{
		void TerminalPrint(TCHAR* szFmt,...)
		{
			va_list ap;
			va_start(ap,szFmt);
			//vfprintf(stdout,szFmt,ap);
			_vftprintf(stdout,szFmt,ap);
			va_end(ap);
			_ftprintf(stdout,_T("\n"));
			fflush(stdout);
		}
#ifdef MESSAGEBOX_DEBUG
		void MBDebug(void* msg)
		{
			LPVOID lpMsgBuf; 
			FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | 
				FORMAT_MESSAGE_IGNORE_INSERTS,0,
				GetLastError(), 
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
				(LPTSTR) &lpMsgBuf,
				NULL,
				NULL );
			MessageBox( NULL, (LPCTSTR)lpMsgBuf, (LPCTSTR)msg, MB_OK | MB_ICONINFORMATION ); 
			LocalFree( lpMsgBuf ); 
		}
#endif
		
 		TCHAR m_szDebug[8192];
		LPTSTR PrintInternal( LPCTSTR lpszFormat, ... )
		{
			va_list args;
			va_start(args, lpszFormat);
			_vstprintf(m_szDebug, lpszFormat, args); 
		//	nBuf = _vsprintf(m_szDebug, lpszFormat, args);
		//	_vsnprintf(m_szDebug, _countof(m_szDebug), lpszFormat, args);
			va_end(args);

			//
			return m_szDebug;
		}

 		void PrintEx(LPCTSTR lpszFormat, ...)
 		{
			va_list args;
			va_start(args, lpszFormat);
			_vstprintf(m_szDebug, lpszFormat, args); 
		//	nBuf = _vsprintf(m_szDebug, lpszFormat, args);
		//	_vsnprintf(m_szDebug, _countof(m_szDebug), lpszFormat, args);
			va_end(args);

#ifdef UNDER_CE
			_tprintf(m_szDebug);
#else
			OutputDebugString(m_szDebug);
#endif
 		}
	}
}

#pragma warning(pop)
