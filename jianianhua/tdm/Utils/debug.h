//
// C++ Interface: debug
//
// Description: 
//
//
// Author: gaoliang User,,, <gaoliang@gaoliang-desktop>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef __DEBUGE_H__
#define __DEBUGE_H__

#define WIN32_LEAN_AND_MEAN
#include <WinSock2.h>
#include <Windows.h>

#include "macro.h"
#include <stdlib.h>
#include <tchar.h>
#ifdef WIN32
	#include <tchar.h>
#endif //WIN32
#ifdef UTILS_DEBUG 

	#if !defined(UNDER_CE)
		#define DEGBUG_BREAK __asm{int 3}
		#define CONDITION_BREAK(x)\
		{\
			if (!(x)) DEGBUG_BREAK\
		}

	#endif//UNDER_CE
	
	#define	AssertMessage(x,...) \
	if (!(x))\
	{\
		Utils::EasyDebug::TerminalPrint(_T("%s:%d--%s"),__FILE__,__LINE__,#x);\
		Utils::EasyDebug::TerminalPrint(__VA_ARGS__);\
		exit(0);\
	}
	
	#define MBDEBUG(x,msg) \
	if ((x))\
	{\
		Utils::EasyDebug::MBDebug(msg); \
	}


	#ifdef WIN32
		#define	Print _tprintf
	#else
		#define	Print printf
	#endif

	
	//! 判断类型是否为整数型    
	/*!      
		再用模板编程时，要判断模板类型，此宏用于在编译时判断类型，如果不是int，long等类型时发出警告并退出
	*/
	#define ASSERT_CANT_INT(CommenType) \
	{\
		CommenType a = 1;\
		int b = 1;\
		if ((char&) b == (char&)a)\
		{\
			Print("CommenType Can't be Int");\
			exit(0);\
		}\
	}
	
#else
	#ifdef _MSV_VER
		#define AssertMessage(x,...)
		#define DEGBUG_BREAK
		#define ASSERT_CANT_INT(CommenType)
		#define CONDITION_BREAK(x)
		#define Print(x,...)
		#define MBDEBUG(x)
	#else
		#define AssertMessage(...)
		#define DEGBUG_BREAK
		#define ASSERT_CANT_INT(CommenType) 
		#define CONDITION_BREAK(x)
		#define Print(...) 
		#define MBDEBUG(...)
	#endif
#endif


namespace Utils
{
	namespace EasyDebug
	{
		//
		void TerminalPrint(TCHAR* szFmt,...);

		//
		void MBDebug(void* msg);

		//
		void PrintEx(LPCTSTR lpszFormat, ...);

		//
		LPTSTR PrintInternal(LPCTSTR lpszFormat, ...);
		
		//
		// Use 
		// #pragma chMSG("Fantasia")
		#define chSTR(x) #x
		#define chSTR2(x) chSTR(x)
		#define chMSG(desc) message(__FILE__ "(" chSTR2(__LINE__) "):" #desc)

		// Can Click "Go To Location"
		#define PrintSuper(_fmt, ...) PrintEx(_T("%s(%d): ") _fmt _T("\n"), _T(__FILE__), __LINE__, ##__VA_ARGS__)
	};
};

#endif
