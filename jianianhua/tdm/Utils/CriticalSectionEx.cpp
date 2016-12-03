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
#include "stdafx.h"
#include "CriticalSectionEx.h"

namespace Utils
{
	CCriticalSectionEx::CCriticalSectionEx(void)
	{
		Init();
	}

	CCriticalSectionEx::~CCriticalSectionEx(void)
	{
		UnInit();
	}

	void CCriticalSectionEx::Init()
	{
		InitializeCriticalSection(&m_CriticalSection);
	}

	void CCriticalSectionEx::UnInit()
	{
		DeleteCriticalSection(&m_CriticalSection);
	}

	void CCriticalSectionEx::Lock()
	{
		EnterCriticalSection(&m_CriticalSection);
	}

	BOOL CCriticalSectionEx::TryLock()
	{
#if _MSC_VER > 1200
		return TryEnterCriticalSection(&m_CriticalSection);
#else
		return FALSE;
#endif
	}

	void CCriticalSectionEx::UnLock()
	{
		LeaveCriticalSection(&m_CriticalSection);
	}

}