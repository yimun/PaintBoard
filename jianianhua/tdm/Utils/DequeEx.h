#ifndef _DEQUEEX_H
#define _DEQUEEX_H

#include "../Include.h"

namespace Utils
{
	class CDequeEx
	{
	public:
		explicit CDequeEx(DWORD dwPreLen);
		virtual ~CDequeEx(void);
	protected:
		LPBYTE m_pBuffer;
		DWORD m_dwBegin;
		DWORD m_dwEnd;
		DWORD GetMod(DWORD dwMod);
		// Protocol Head Size
		DWORD m_dwPreLen;
	protected:
		void push_back(const BYTE& _Val);
		void pop_front();
		BYTE& operator[](DWORD _Pos);

	public:
		//
		VOID Init();
		DWORD size();
		BOOL Pop(DWORD dwDataLen);
		LPBYTE Get(DWORD dwIndex, DWORD dwDataLen, LPBYTE lpBuf = NULL, DWORD dwBufLen = 0);
		
		BOOL Push(LPBYTE pDataBuf, DWORD dwDataLen);
		virtual LPBYTE GetValidPacket(LPDWORD dwPacketSize);
	};
}

#endif
