//
// C++ Interface: CriticalSectionEx
//
// Description: 
//
// Author:    xltyt, <xltytlive@hotmail.com>, (C) 2009
// Created:   2009-2-24
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef _CRITICAL_SECTIONEX_H
#define _CRITICAL_SECTIONEX_H

//
//#define WIN32_LEAN_AND_MEAN
//#include <windows.h>
#define WIN32_LEAN_AND_MEAN
#include <WinSock2.h>
#include <Windows.h>

namespace Utils
{
	class CCriticalSectionEx
	{
	public:
		CCriticalSectionEx(void);
		~CCriticalSectionEx(void);
	public:
		//
		void Init();
		void UnInit();
		void Lock();
		BOOL TryLock();
		void UnLock();

	private:
		CRITICAL_SECTION m_CriticalSection;
	};
inline void Step_In(CCriticalSectionEx& cs)
{
	cs.Lock();
}
inline void Step_Out(CCriticalSectionEx& cs)
{
	cs.UnLock();
}
}

#endif