#include "stdafx.h"
#include "SerialManager.h"
#include <atlconv.h>
#include "../utils/debug.h"
using namespace Utils::EasyDebug;
#include <vector>
using namespace std;
#ifdef UNDER_CE
	#include "CESerial.h"
	#include "SerialPortEnum.h"
	using namespace Device::CECommEngine;
#else
	#include "PCCommEngine.h"
	#include "PCEnumSerial.h"
	using namespace Device::PCCommEngine;
#endif
#include "../utils/CheckUtils.h"

namespace Device
{
	//
	#define PACKET_CHECKSUM

	//
	//const DWORD CDequeExSearch::HEAD_FLAG = 0xFCA12BD6;
	const DWORD CDequeExSearch::HEAD_FLAG = 0xFFFFFFFF;
	
	//
	CDequeExSearch::CDequeExSearch(DWORD dwPreLen)
		:CDequeEx(dwPreLen)
	{
		//
		// Packet CheckSum Test
		//
		/*
		LPSTR pPointer = m_pBuffer;
		BYTE bData = 1;
		memcpy(pPointer, &bData, sizeof(BYTE));
		pPointer += sizeof(BYTE);

		bData = 8;
		memcpy(pPointer, &bData, sizeof(BYTE));
		pPointer += sizeof(BYTE);

		DWORD dwFlag = HEAD_FLAG;
		memcpy(pPointer, &dwFlag, sizeof(DWORD));
		pPointer += sizeof(DWORD);

		WORD wLen = 2;
		memcpy(pPointer, &wLen, sizeof(WORD));
		pPointer += sizeof(WORD);

		bData = 5;
		memcpy(pPointer, &bData, sizeof(BYTE));
		pPointer += sizeof(BYTE);
		bData = 6;
		memcpy(pPointer, &bData, sizeof(BYTE));
		pPointer += sizeof(BYTE);

		//bData = 0xDD;
		bData = CCheckUtils::GetLRC((LPBYTE)( m_pBuffer + 2 + sizeof(DWORD) ), 4);
		memcpy(pPointer, &bData, sizeof(BYTE));
		pPointer += sizeof(BYTE);

		bData = 0xEE;
		memcpy(pPointer, &bData, sizeof(BYTE));
		pPointer += sizeof(BYTE);

		m_dwEnd += (pPointer - m_pBuffer);
		//DWORD dwSize = size();
		//dwSize = 0;

		DWORD dwSize = size();
		LPSTR pPacket = GetValidPacket(&dwSize);
		SAFE_DELETE_AR(pPacket);

		bData = 0;
		dwSize = size();
		bData = operator[] (0);
		dwSize = 0;
		*/
	}

	CDequeExSearch::~CDequeExSearch()
	{
	}

	LPBYTE CDequeExSearch::GetValidPacket(LPDWORD dwPacketSize)
	{
		return GetValidPacketEx(dwPacketSize);

#ifndef PACKET_CHECKSUM
		return CDequeEx::GetValidPacket(dwPacketSize);
#else
		//
		// Head Flag(4 Bytes)	: 0xFCA12BD6
		// Len(2 Byte)			: n
		// Data(n Bytes)		:
		// CheckSum(1 Byte)		: CheckSum
		//
				
		// Check Size
		DWORD dwSize = size();
		const WORD wMinPacketLen = sizeof(DWORD) + sizeof(WORD) + sizeof(BYTE) + 1;
		if(dwSize < wMinPacketLen)
		{
			return NULL;
		}

		//
		DWORD dwPopSize = (DWORD)-1;
		DWORD i;
		for(i = 0; i < dwSize - sizeof(DWORD); i++)
		{
			LPBYTE pPakcet = Get(i, sizeof(DWORD));
			DWORD dwFlag = 0;
			memcpy(&dwFlag, pPakcet, sizeof(DWORD));
			SAFE_DELETE_AR(pPakcet);
			if(dwFlag == HEAD_FLAG)
			{
				//
				dwPopSize = i;

				//
				LPBYTE pLen = Get(i + sizeof(DWORD), sizeof(WORD));
				WORD wLen = 0;
				memcpy(&wLen, pLen, sizeof(WORD));
				SAFE_DELETE_AR(pLen);

				DWORD dwCheckPakcetSize = sizeof(DWORD) + sizeof(WORD) + wLen;
				if(i + dwCheckPakcetSize < dwSize)
				{
					// Transport CheckSum
					LPBYTE pCheckSum = Get(i + dwCheckPakcetSize, sizeof(BYTE));
					BYTE bCheckSum = 0;
					memcpy(&bCheckSum, pCheckSum, sizeof(BYTE));
					SAFE_DELETE_AR(pCheckSum);

					// Calculate CheckSum
					LPBYTE pCalCheckSum = Get(i + sizeof(DWORD), sizeof(WORD) + wLen);
					BYTE bCalCheckSum = CCheckUtils::GetLRC((LPBYTE)( pCalCheckSum ), sizeof(WORD) + wLen);
					SAFE_DELETE_AR(pCalCheckSum);

					// Check
					if(bCheckSum == bCalCheckSum)
					{
						*dwPacketSize = wLen;
						LPBYTE pRet = Get(i + sizeof(DWORD) + sizeof(WORD), wLen);
						
						Pop(i + dwCheckPakcetSize + sizeof(BYTE));
						dwSize = size();
						return pRet;
					}
				}
				else
				{
					break;
				}
			}
		}
		if(dwPopSize != (DWORD)-1 && dwPopSize != 0)
		{
			Pop(dwPopSize);
		}
		return NULL;
#endif
	}

	LPBYTE CDequeExSearch::GetValidPacketEx(LPDWORD dwPacketSize)
	{
#ifndef PACKET_CHECKSUM
		return CDequeEx::GetValidPacket(dwPacketSize);
#else
		//
		// Head Flag(4 Bytes)	: 0xFCA12BD6
		// Len(2 Byte)			: n
		// Data(n Bytes)		:
		// CheckSum(1 Byte)		: CheckSum
		//
		DWORD i;
		DWORD j;

		// Check Size
		DWORD dwSize = size();
		const WORD wMinPacketLen = sizeof(DWORD) + sizeof(WORD) + sizeof(BYTE) + 1;
		if(dwSize < wMinPacketLen)
		{
			return NULL;
		}

		// Try Possible Head
		DWORD dwTempPacketCount = 0;
		for(i = 0; i < dwSize - sizeof(DWORD); i++)
		{
			LPBYTE pPakcet = Get(i, sizeof(DWORD));
			DWORD dwFlag = 0;
			memcpy(&dwFlag, pPakcet, sizeof(DWORD));
			SAFE_DELETE_AR(pPakcet);
			if(dwFlag == HEAD_FLAG)
			{
				m_DequeIndex[ dwTempPacketCount ].wStartIndex = (WORD)i;
				dwTempPacketCount++;
			}
		}
		if(dwTempPacketCount == 0)
		{
			return NULL;
		}

		// Escape No Data Head Packet
		int iEscapeIndex = 0;
		for(i = 0; i < dwTempPacketCount - 1; i++)
		{
			if(m_DequeIndex[i].wStartIndex + wMinPacketLen <= m_DequeIndex[i + 1].wStartIndex)
			{
				memcpy(m_DequeIndex + iEscapeIndex, m_DequeIndex + i, sizeof(CDequeIndex));
				iEscapeIndex++;
			}
		}
		memcpy(m_DequeIndex + iEscapeIndex, m_DequeIndex + i, sizeof(CDequeIndex));
		iEscapeIndex++;
		dwTempPacketCount = iEscapeIndex;

		// Check Every Head Packet
		DWORD dwPopSize = (DWORD)-1;
		for(j = 0; j < dwTempPacketCount; j++)
		{
			// Get Every Start Index
			i = m_DequeIndex[j].wStartIndex;
			
			// Check Min Size
			if(i + sizeof(DWORD) + sizeof(WORD) >= dwSize)
			{
				break;
			}

			// Get Packet Len Information
			LPBYTE pLen = Get(i + sizeof(DWORD), sizeof(WORD));
			WORD wLen = 0;
			memcpy(&wLen, pLen, sizeof(WORD));
			SAFE_DELETE_AR(pLen);

			DWORD dwCheckPakcetSize = sizeof(DWORD) + sizeof(WORD) + wLen;
			if(i + dwCheckPakcetSize < dwSize)
			{
				//
				dwPopSize = i + dwCheckPakcetSize + sizeof(BYTE);

				// Transport CheckSum
				LPBYTE pCheckSum = Get(i + dwCheckPakcetSize, sizeof(BYTE));
				BYTE bCheckSum = 0;
				memcpy(&bCheckSum, pCheckSum, sizeof(BYTE));
				SAFE_DELETE_AR(pCheckSum);

				// Calculate CheckSum
				LPBYTE pCalCheckSum = Get(i + sizeof(DWORD), sizeof(WORD) + wLen);
				BYTE bCalCheckSum = CCheckUtils::GetLRC((LPBYTE)( pCalCheckSum ), sizeof(WORD) + wLen);
				SAFE_DELETE_AR(pCalCheckSum);

				// Check
				if(bCheckSum == bCalCheckSum)
				{
					*dwPacketSize = wLen;
					LPBYTE pRet = Get(i + sizeof(DWORD) + sizeof(WORD), wLen);

					Pop(dwPopSize);
					dwSize = size();
					return pRet;
				}
			}
			else
			{
				if(j == dwTempPacketCount - 1)
				{
					break;
				}
				else
				{
					// Get Pop Size
					dwPopSize = i + sizeof(DWORD) + sizeof(WORD);
				}
			}
		}

		//
		if(dwPopSize != (DWORD)-1 && dwPopSize != 0)
		{
			Pop(dwPopSize);
		}
		return NULL;
#endif
	}

	//////////////////////////////////////////////////////////////////////////

	#define PRE_SERIAL_LEN	(sizeof(WORD))
	BYTE CSerialManager::pbPacket[4096 * 10];

	CSerialManager::CSerialManager()
		:pCommEngineCtrl(NULL)
		,m_hSerial(INVALID_HANDLE_VALUE)
		,m_DequeEx(PRE_SERIAL_LEN)
		,m_bUsePacketDispatch(TRUE)
	{
#ifdef UNDER_CE
		pCommEngineCtrl = new CCESerialCtrl;
#else
		pCommEngineCtrl = new CPCCommEngineCtrl;
#endif
		
		int iCount = _countof(VecDispatch);
		for(int i = 0; i < iCount; i++)
		{
			VecDispatch[i] = NULL;
			VecDispatchParam[i] = NULL;
		}

		//
		memset(pbPacket, 0, sizeof(pbPacket));

		file.Open(_T("write.txt"),CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite);
	}

	CSerialManager::~CSerialManager()
	{
		if(pCommEngineCtrl)
		{
			delete pCommEngineCtrl;
			pCommEngineCtrl = NULL;
		}
		file.Close();
	}

#ifdef UNDER_CE
	CCESerialCtrl* CSerialManager::GetCommEngine()
	{
		return pCommEngineCtrl;
	}
#else
	CPCCommEngineCtrl* CSerialManager::GetCommEngine()
	{
		return pCommEngineCtrl;
	}
#endif

	BOOL CSerialManager::Initialize(LPCTSTR pszPort, DWORD BaudRate /*= CBR_115200*/)
	{
		//
		// Check Exist
		//
#ifdef UNDER_CE
		CSerialPortEnum Enum;
		size_t iCount = Enum.EnumeratePorts();
		size_t i;
		for(i = 0; i < iCount; i++)
		{
			LPCTSTR pPortName = Enum[i]->GetPortName();
			//LPTSTR pSkipped = _tcsrchr(pPortName, _T(':'));
			//if(pSkipped)	
			//{
			//	*pSkipped = 0; 
			//}

			if(lstrcmp(pPortName, pszPort) == 0)
			{
				break;
			}
		}
		if(i == iCount)
		{
			return FALSE;
		}
#else
		vector<SPCSerInfo> vecSerInfo;
		EnumPCSerialPorts(vecSerInfo, FALSE);
		vector<SPCSerInfo>::iterator pIterator = vecSerInfo.begin();
		for(; pIterator != vecSerInfo.end(); pIterator++)
		{
			if(pIterator->strPortName == pszPort)
			{
				break;
			}
		}
		if(pIterator == vecSerInfo.end())
		{
			return FALSE;
		}
#endif
		
		//
		// Initialize
		//
	
		//
		BOOL bOK = pCommEngineCtrl->Initialize(this, pszPort);
		if(bOK)
		{
			m_hSerial = pCommEngineCtrl->GetSerialEngine()->GetCommFile();

			// Set BaudRate
			pCommEngineCtrl->GetSerialEngine()->SetDCB(BaudRate);
		}
		return bOK;
	}

	VOID CSerialManager::Close()
	{
		pCommEngineCtrl->Close();
	}

	BOOL CSerialManager::GetPacketDispatch()
	{
		return m_bUsePacketDispatch;
	}

	VOID CSerialManager::SetPacketDispatch( BOOL bDispatch )
	{
		m_bUsePacketDispatch = bDispatch;
	}

	__forceinline VOID CSerialManager::OnWriteCommCompeleted( LPCVOID lpBuffer, const DWORD dwLen )
	{
	}

	void CSerialManager::DelayUs( __int64 Us )
	{
		LARGE_INTEGER CurrTicks, TicksCount;   

		QueryPerformanceFrequency(&TicksCount);
		QueryPerformanceCounter(&CurrTicks);

		TicksCount.QuadPart  = TicksCount.QuadPart * Us / 1000000;
		TicksCount.QuadPart += CurrTicks.QuadPart;

		while(CurrTicks.QuadPart < TicksCount.QuadPart)
		{
			QueryPerformanceCounter(&CurrTicks);
		}
	}

	void CSerialManager::WriteCom(DWORD dwDataLen, LPCVOID pData)
	{
		assert(m_hSerial != NULL);

//		PrintEx(_T("Before WriteCom Lock\n"));
		m_CriticalSection.Lock();
//		PrintEx(_T("After WriteCom Lock\n"));

		/*
		LPSTR pData = (LPSTR)aData;
		for(DWORD i = 0; i < aLen ; i++)
		{
			pCommEngineCtrl->WriteComm(1, pData + i);
			//
			// DSP Use FIFO, So Don't Need Delay
			//DelayUs(300);
		}
		*/

#ifndef PACKET_CHECKSUM
		pCommEngineCtrl->WriteComm(dwDataLen, pData);
#else
		LPBYTE pPointer = pbPacket;
		// Head
		DWORD dwHeadFlag = CDequeExSearch::HEAD_FLAG;
		memcpy(pPointer, &dwHeadFlag, sizeof(DWORD));
		pPointer += sizeof(DWORD);
		// Data
		memcpy(pPointer, pData, dwDataLen);
		pPointer += dwDataLen;
		// CheckSum
		BYTE bCheckSum = CCheckUtils::GetLRC((LPBYTE)pData, dwDataLen);
		memcpy(pPointer, &bCheckSum, sizeof(BYTE));
		pPointer += sizeof(BYTE);

		//fwrite(pbPacket,dwDataLen+5,1,fp);
		
		file.Write(pbPacket,dwDataLen+5);
	//	fprintf(fp, "  %d-%d-%d-%d\r\n", hour, min, sed, ms);

		// Send
//		PrintEx(_T("Before WriteCom\n"));
		pCommEngineCtrl->WriteComm((DWORD)(pPointer - pbPacket), pbPacket);
//		PrintEx(_T("After WriteCom\n"));

		/*
		// Head
		DWORD dwHeadFlag = HEAD_FLAG;
		PrintEx(_T("Before WriteCom 1\n"));
		pCommEngineCtrl->WriteComm(sizeof(DWORD), &dwHeadFlag);
		PrintEx(_T("After WriteCom 1\n"));

		// Data
		PrintEx(_T("Before WriteCom 2\n"));
		pCommEngineCtrl->WriteComm(dwDataLen, pData);
		PrintEx(_T("After WriteCom 2\n"));

		// CheckSum
		PrintEx(_T("Before LRC\n"));
		DWORD bCheckSum = CCheckUtils::GetLRC((LPBYTE)pData, dwDataLen);
		PrintEx(_T("After LRC\n"));

		PrintEx(_T("Before WriteCom 3\n"));
		pCommEngineCtrl->WriteComm(sizeof(BYTE), &bCheckSum);
		PrintEx(_T("After WriteCom 3\n"));
		*/
#endif

//		PrintEx(_T("Before WriteCom UnLock\n"));
		m_CriticalSection.UnLock();
//		PrintEx(_T("After WriteCom UnLock\n"));
	}

	void CSerialManager::AddDispatch(BYTE bDispatch, LPSERIAL_START_ROUTINE pDispatchRoutine, LPVOID pDispatchParam)
	{
		assert(bDispatch >= 0 && bDispatch < 256);
		if(m_bUsePacketDispatch == FALSE)
		{
			_tprintf(_T("Warning: Dispatch Disable\n"));
		}

		if(VecDispatch[bDispatch])
		{
			_tprintf(_T("Dispatch Already Exist, Replace: %d\n"), bDispatch);
		}
		VecDispatch[bDispatch] = pDispatchRoutine;
		VecDispatchParam[bDispatch] = pDispatchParam;
	}

	void CSerialManager::RemoveDispatch(BYTE bDispatch)
	{
		assert(bDispatch >= 0 && bDispatch < 256);
		if(m_bUsePacketDispatch == FALSE)
		{
			_tprintf(_T("Warning: Dispatch Disable\n"));
		}

		if(VecDispatch[bDispatch] == NULL)
		{
			_tprintf(_T("Dispatch Don't Exist, Ignore: %d\n"), bDispatch);
		}
		VecDispatch[bDispatch] = NULL;
		VecDispatchParam[bDispatch] = NULL;
	}

	__forceinline VOID CSerialManager::OnReadCommCompeleted( LPBYTE pszBuffer, const DWORD dwLen )
	{
		if( m_bUsePacketDispatch )
		{
			//
			//PrintEx(_T("Receive Buffer, Len: %d\n"), dwLen);

			//
			m_DequeEx.Push(pszBuffer, dwLen);
			while(1)
			{
				DWORD dwPacketSize = 0;
				LPBYTE pbPacket = m_DequeEx.GetValidPacket(&dwPacketSize);
				if(pbPacket == NULL)
				{
					break;
				}

//				PrintEx(_T("Receive Buffer After GetValidPacket, %d\n\n"), (int)*pbPacket);

				//
				//static int iTimes = 0;
				//PrintEx(_T("Receive Packet(%04d), Len: %d\n"), iTimes++, dwPacketSize);
				//PrintEx(_T("Receive Packet Len: %d\n"), dwPacketSize);

				BYTE bInstruction = *pbPacket;
				if(bInstruction >= 0 && bInstruction < 256)
				{
					LPSERIAL_START_ROUTINE pDispatch = VecDispatch[bInstruction];
					if(pDispatch != NULL)
					{
						pDispatch(
							VecDispatchParam[bInstruction],
							pbPacket,
							dwPacketSize);
					}
					else
					{
						USES_CONVERSION;
						PrintEx(_T("Packet Dispatch Error: %s, Instruction: %d\n"), 
							A2T(__FUNCTION__), bInstruction);
					}
				}
				else
				{
					USES_CONVERSION;
					PrintEx(_T("Packet Instruction Error: %s, Instruction: %d\n"), 
						A2T(__FUNCTION__), bInstruction);
				}

				//
				delete[] pbPacket;
				pbPacket = NULL;
			}
		}
		else
		{
			BYTE bInstruction = 0;
			LPSERIAL_START_ROUTINE pDispatch = VecDispatch[bInstruction];
			if(pDispatch != NULL)
			{
				pDispatch(
					VecDispatchParam[bInstruction],
					pszBuffer,
					dwLen);
			}
			else
			{
				USES_CONVERSION;
				PrintEx(_T("Packet Dispatch Extension Error: %s\n"), 
					A2T(__FUNCTION__));
			}
		}
	}

	BOOL CSerialManager::IsConnected()
	{
		return GetCommEngine()->GetSerialEngine()->GetCommFile() != INVALID_HANDLE_VALUE;
	}

	VOID CSerialManager::ClearCache()
	{
		m_DequeEx.Init();
	}
}
