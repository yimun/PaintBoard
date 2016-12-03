#include "StdAfx.h"
#include "DequeEx.h"
#include <assert.h>
#include "utils.h"
#include <new>
using namespace std;
namespace Utils
{
	const DWORD MAX_DATA_BUFFER	= 1024 * 1024;

	CDequeEx::CDequeEx(DWORD dwPreLen)
		:m_dwPreLen(dwPreLen)
	{
		m_pBuffer = new BYTE[MAX_DATA_BUFFER];
		Init();
	}

	CDequeEx::~CDequeEx(void)
	{
		SAFE_DELETE_AR(m_pBuffer);
	}

	DWORD CDequeEx::GetMod(DWORD dwMod)
	{
		return dwMod - dwMod / MAX_DATA_BUFFER * MAX_DATA_BUFFER;
	}

	void CDequeEx::push_back(const BYTE& _Val)
	{
//		assert(size() < MAX_DATA_BUFFER - 1);
// 		DWORD dwReal = GetMod(MAX_DATA_BUFFER + m_dwEnd + 1);
// 		m_pBuffer[dwReal] = _Val;
// 		m_dwEnd = GetMod(m_dwEnd + 1);

		//
		if(size() >= MAX_DATA_BUFFER - 1)
		{
			pop_front();
		}

		//
		m_dwEnd = GetMod(MAX_DATA_BUFFER + m_dwEnd + 1);
		m_pBuffer[m_dwEnd] = _Val;
	}

	void CDequeEx::pop_front()
	{
		if(size() > 0)
		{
			m_dwBegin = GetMod(MAX_DATA_BUFFER + m_dwBegin + 1);
		}
	}

	DWORD CDequeEx::size()
	{
		return GetMod(MAX_DATA_BUFFER + m_dwEnd - m_dwBegin + 1);
	}

	BYTE& CDequeEx::operator[](DWORD _Pos)
	{
		assert(_Pos < size());
		DWORD dwReal = GetMod(m_dwBegin + _Pos);
		return m_pBuffer[dwReal];
	}

	//////////////////////////////////////////////////////////////////////////

	VOID CDequeEx::Init()
	{
		m_dwBegin = 0;
		m_dwEnd = -1;
	}

	BOOL CDequeEx::Push(LPBYTE pDataBuf, DWORD dwDataLen)
	{
		for(DWORD i = 0; i < dwDataLen; i++)
		{
			push_back(pDataBuf[i]);
		}
		return TRUE;
	}

	BOOL CDequeEx::Pop(DWORD dwDataLen)
	{
		if(dwDataLen > size())
		{
			return FALSE;
		}
		for(DWORD i = 0; i < dwDataLen; i++)
		{
			pop_front();
		}
		return TRUE;
	}

	LPBYTE CDequeEx::Get(DWORD dwIndex, DWORD dwDataLen,
		LPBYTE lpRevBuf /*= NULL*/, DWORD dwRevBufLen /*= 0*/)
	{
		/*
		if(dwIndex + dwDataLen > size())
		{
			return NULL;
		}
		*/
		LPBYTE pBuf = NULL;
		if(lpRevBuf == NULL)
		{
			pBuf = new BYTE[dwDataLen + 1];
			memset(pBuf, 0, dwDataLen + 1);
		}
		else
		{
			assert(dwDataLen <= dwRevBufLen);
			pBuf = lpRevBuf;
			memset(pBuf, 0, dwRevBufLen);
		}
		DWORD i;
		for(i = dwIndex; i < dwIndex + dwDataLen; i++)
		{
			pBuf[i - dwIndex] = operator[](i);
		}
		pBuf[i - dwIndex] = 0;
		return pBuf;
	}

	LPBYTE CDequeEx::GetValidPacket(LPDWORD dwPacketSize)
	{
		if(size() < m_dwPreLen)
		{
			return FALSE;
		}
		LPBYTE pBegin = Get(0, m_dwPreLen);
		DWORD dwLen = 0;
		memcpy(&dwLen, pBegin, m_dwPreLen);
		SAFE_DELETE_AR(pBegin);
		if(dwLen + m_dwPreLen > size())
		{
			return FALSE;
		}
		//
		pBegin = Get(m_dwPreLen, dwLen);
		if(pBegin)
		{
			Pop(dwLen + m_dwPreLen);
			*dwPacketSize = dwLen;
		}
		return pBegin;
	}

}

