#ifndef __UTYPE_H__
#define __UTYPE_H__
// #ifdef WIN32
// typedef unsigned TCHAR uchar;
// #else
// typedef unsigned TCHAR uchar;
// #endif
// typedef unsigned int uint;

#ifdef UNICODE
#define tstring wstring
#else
#define tstring string
#endif

#if defined(WIN32) || defined(UNDER_CE)
#include "../Include.h"
//#define WIN32_LEAN_AND_MEAN
//#include <Windows.h>

namespace Utils
{
	typedef struct _HANID
	{
		HANDLE handle;
		DWORD id;
	}
	HANID;
}

#endif

typedef float DEGREE;
typedef float RADIAN;

#endif