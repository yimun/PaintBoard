#ifndef _SERIAL_MANAGER_H
#define _SERIAL_MANAGER_H

#include "../Include.h"
#include "../stdafx.h"

#include "../utils/CriticalSectionEx.h"
#include "../utils/DequeEx.h"
using namespace Utils;

#ifdef UNDER_CE
	#include "CESerial.h"
#else
	#include "PCCommEngine.h"
#endif

namespace Device
{
	class CDequeExSearch : public CDequeEx
	{
	public:
		explicit CDequeExSearch(DWORD dwPreLen);
		~CDequeExSearch(void);

	public:
		class CDequeIndex
		{
		public:
			WORD wStartIndex;
		//	WORD wLength;
		};

		static const DWORD HEAD_FLAG;
	private:
		CDequeIndex m_DequeIndex[ 72 ];

	public:
		virtual LPBYTE GetValidPacket(LPDWORD dwPacketSize);
		LPBYTE GetValidPacketEx(LPDWORD dwPacketSize);
	};

	typedef DWORD (WINAPI *PSERIAL_START_ROUTINE)(
		LPVOID lpThreadParameter,
		LPBYTE pszBuffer, 
		const DWORD dwLen
		);
	typedef PSERIAL_START_ROUTINE LPSERIAL_START_ROUTINE;

#ifdef UNDER_CE
	using namespace CECommEngine;
#else
	using namespace PCCommEngine;
#endif

#ifdef UNDER_CE
	class CSerialManager : public CCECommEngineCallBack
#else
	class CSerialManager : public CPCCommEngineCallBack
#endif
	{
	public:
		CSerialManager();
		~CSerialManager();

	private:
		CCriticalSectionEx m_CriticalSection;
#ifdef UNDER_CE
		CCESerialCtrl* pCommEngineCtrl;
#else
		CPCCommEngineCtrl* pCommEngineCtrl;
#endif
		CDequeExSearch m_DequeEx;
		LPSERIAL_START_ROUTINE VecDispatch[256];
		LPVOID VecDispatchParam[256];
		HANDLE m_hSerial;
		BOOL m_bUsePacketDispatch;
		static BYTE pbPacket[4096 * 10];

	public:
		//! Read Call Back
		__forceinline virtual VOID OnReadCommCompeleted(LPBYTE pszBuffer, const DWORD dwLen);

		//! Write Call Back
		__forceinline virtual VOID OnWriteCommCompeleted(LPCVOID lpBuffer, const DWORD dwLen);

	public:
		BOOL Initialize(LPCTSTR pszPort, DWORD BaudRate = CBR_115200);
		VOID Close();
		//void WriteCom(DWORD aLen, LPCVOID aData, WORD hour, WORD min, WORD sed, WORD ms);
		void WriteCom(DWORD aLen, LPCVOID aData);
		void DelayUs(__int64 Us);
		BOOL IsConnected();
		VOID ClearCache();

		CFile file;
		

#ifdef UNDER_CE
		CCESerialCtrl* GetCommEngine();
#else
		CPCCommEngineCtrl* GetCommEngine();
#endif

		void AddDispatch(BYTE bDispatch, LPSERIAL_START_ROUTINE pDispatchRoutine, LPVOID pDispatchParam);
		void RemoveDispatch(BYTE bDispatch);

		BOOL GetPacketDispatch();
		VOID SetPacketDispatch(BOOL bDispatch);
	};
}

#endif
