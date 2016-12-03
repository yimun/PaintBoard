#ifndef _INCLUDE_H
#define _INCLUDE_H

// Windows
//WIN32_LEAN_AND_MEAN prevent windows.h includ the <winsock.h> so that it can be avoid conflict with <WinSock2.h>
#define WIN32_LEAN_AND_MEAN
#include <WinSock2.h>
#include <Windows.h>

#include <WindowsX.h>
#include <winioctl.h>				// Needed for CTLCODE macro
#include <WinBase.h>
#include <tchar.h>
#include <assert.h>
#include <time.h>
#include <Shlwapi.h>
#include <ShellAPI.h>
#include <WTypes.h>
#include <math.h>
#include <CommCtrl.h>
#ifndef UNDER_CE
	#pragma comment(lib, "comctl32.lib")
#endif

#define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA
//Skip Fallow Warning
//_CE_ACTIVEX was not defined because this Windows CE SDK does not have DCOM.
//_CE_ACTIVEX could be caused to be defined by defining _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA, but it is recommended that this be done only for single-threaded apps.
//_ATL_NO_HOSTING was defined because _CE_ACTIVEX was not defined.
#include <atldef.h>

#ifdef WIN32
	#ifdef UNDER_CE
		#pragma comment(lib, "ws2.lib")

		#include <aygshell.h>
		#pragma comment(lib, "aygshell.lib")
	#else
		#include <WtsApi32.h>
		#pragma comment(lib, "WtsApi32.lib")
		#pragma comment(lib, "WS2_32.LIB")
		#pragma comment(lib, "setupapi.lib")
		#pragma comment(lib, "shlwapi.lib")
	#endif
#endif
//
typedef const LPBYTE LPCBYTE;

// RTTI Support
#define DECLARE_DYNAMIC_EX()\
public:\
	static const LPTSTR _className;

#define IMPLEMENT_DYNAMIC_EX(class_name) \
	const LPTSTR class_name::_className = { \
	_T(#class_name)\
};

//
#include "utils/utils.h"

#ifdef UNDER_CE
	#define DTAG        TEXT ("Camera: ")
#endif

#define GLOBAL_DEBUG
#ifdef GLOBAL_DEBUG
	//设备宏
	//#define DEVICE_DEBUG
	//用户界面调试宏
	#define UI_DEBUG
#endif

#ifdef UNDER_CE
	#ifdef _UNICODE
		double _tstof_Custom(const TCHAR* _Str);

		#define _tstoi _wtoi
		#define _tstof _tstof_Custom
		//double _tstof(_In_z_ const TCHAR* _Str);
	#else
		#define _tstoi atoi
		#define _tstof atof
	#endif
#endif

// Global Error Process
#ifdef UNDER_CE
	VOID GlobalErrorProcessInternal(LPCTSTR lpszFormat, ...);
	#define GlobalErrorProcess(lpszFormat, ...) GlobalErrorProcessInternal(lpszFormat, __VA_ARGS__)
#else
	VOID Output(LPCTSTR lpszFormat, ...);
	#define GlobalErrorProcess(lpszFormat, ...) Output(lpszFormat, __VA_ARGS__)
#endif

#endif
