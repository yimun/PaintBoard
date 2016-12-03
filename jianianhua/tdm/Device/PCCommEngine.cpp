//
// C++ Interface: PCCommEngine
//
// Description: 
//
// Author:    xltyt, <xltytlive@hotmail.com>, (C) 2009
// Created:   2009-3-6
//
// Copyright: See COPYING file that comes with this distribution
//

#include "stdafx.h"
#include "PCCommEngine.h"
#include "../utils/ThreadEx.h"
using namespace Utils;

namespace Device
{
	namespace PCCommEngine
	{
		//	SetTimer(hWnd, 1, 100, &CCommEngineCtrl::ComEngineReadThread);
		// 	VOID CALLBACK CCommEngineCtrl::TimeProc(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
		// 	{
		// 		
		// 	}	

		CPCCommEngine::CPCCommEngine(void)
		{
			//! Default parameter.
			this->m_dConfig.ByteSize	= 8;			//! Byte of the Data.
			this->m_dConfig.StopBits	= ONESTOPBIT;	//! Use one bit for stopbit.
			this->m_dConfig.Parity		= NOPARITY;		//! No parity bit
			this->m_dConfig.BaudRate	= CBR_115200;	//! Baudrate 9600 bit/sec

			this->m_bConnected			= FALSE;
			this->m_hCommFile			= INVALID_HANDLE_VALUE;
		}

		CPCCommEngine::~CPCCommEngine(void)
		{
			this->CloseComm();
		}

		BOOL CPCCommEngine::OpenComm(LPCTSTR pszPort)
		{
			if(!this->m_bConnected)
			{
				TCHAR szRealPort[256];
				wsprintf(szRealPort, _T("\\\\.\\%s"), pszPort);

				this->m_hCommFile = CreateFile(
					szRealPort,							//! Specify port device: default "COM1"
					GENERIC_READ|GENERIC_WRITE,         //! Specify mode that open device.
					0,                                  //! the devide isn't shared.
					NULL,                               //! the object gets a default security.
					OPEN_EXISTING,                      //! Specify which action to take on file. 
					FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,
					NULL);                              //! default.
				if(this->m_hCommFile == INVALID_HANDLE_VALUE)
				{
					return FALSE;
				}

				if(!this->Config())
				{
					return FALSE;
				}

				lstrcpy(m_szPort, pszPort);
				this->m_bConnected = TRUE;
			}
			return TRUE;
		}

		BOOL CPCCommEngine::Config()
		{
			if(!SetupComm(this->m_hCommFile, MAXBLOCK, MAXBLOCK))
			{
				return FALSE;
			}
			if(!SetCommMask(this->m_hCommFile, EV_RXCHAR))
			{
				return FALSE;
			}

			//! 把间隔超时设为最大，把总超时设为0将导致ReadFile立即返回并完成操作
			//! instance an object of COMMTIMEOUTS.
			COMMTIMEOUTS CommTimeouts;
			if(!GetCommTimeouts(this->m_hCommFile, &CommTimeouts))
			{
				return FALSE;
			}
			
			/*
			CommTimeouts.ReadIntervalTimeout = 20;			//! Specify time-out between charactor for receiving.
			CommTimeouts.ReadTotalTimeoutMultiplier = 0;	//! Specify value that is multiplied
			//! by the requested number of bytes to be read.
			CommTimeouts.ReadTotalTimeoutConstant = 0;
			//! Specify value is added to the product of the
			//! ReadTotalTimeoutMultiplier member
			CommTimeouts.WriteTotalTimeoutMultiplier = 0;	//! Specify value that is multiplied
			//! by the requested number of bytes to be sent.
			CommTimeouts.WriteTotalTimeoutConstant = 0;
			*/

			CommTimeouts.ReadIntervalTimeout			= MAXDWORD;
			CommTimeouts.ReadTotalTimeoutMultiplier		= 0;
			CommTimeouts.ReadTotalTimeoutConstant		= 0;
//			CommTimeouts.WriteTotalTimeoutMultiplier	= 5;
//			CommTimeouts.WriteTotalTimeoutConstant		= 100;
			CommTimeouts.WriteTotalTimeoutMultiplier	= 0;
			CommTimeouts.WriteTotalTimeoutConstant		= 500;
			
			/*
			CommTimeouts.ReadIntervalTimeout			= 0;
			CommTimeouts.ReadTotalTimeoutMultiplier		= 0;
			CommTimeouts.ReadTotalTimeoutConstant		= 1000;
			CommTimeouts.WriteTotalTimeoutMultiplier	= 0;
			CommTimeouts.WriteTotalTimeoutConstant		= 500;
			*/

			//! Specify value is added to the product of the
			//! WriteTotalTimeoutMultiplier member
			if(!SetCommTimeouts(this->m_hCommFile,&CommTimeouts))	//! set the time-out parameter into device control.
			{
				return FALSE;
			}

			return SetDCB(this->m_dConfig.BaudRate);
		}

		BOOL CPCCommEngine::SetDCB(DWORD dwBaudRate)
		{
			DCB dcbTemp;
			//! Get current configuration of serial communication port.
			if (!GetCommState(this->m_hCommFile, &dcbTemp))
			{
				return FALSE;
			}
			//! Assign user parameter.
			dcbTemp.BaudRate = dwBaudRate;					//! Specify baud rate of communication.
			dcbTemp.StopBits = this->m_dConfig.StopBits;    //! Specify stop-bit of communication.
			dcbTemp.Parity	 = this->m_dConfig.Parity;      //! Specify parity of communication.
			dcbTemp.ByteSize = this->m_dConfig.ByteSize;    //! Specify  byte of size of communication.

			//! Set current configuration of serial communication port.
			if(!SetCommState(this->m_hCommFile, &dcbTemp))
			{
				return FALSE;
			}
			return TRUE;
		}

		BOOL CPCCommEngine::IsConnected()
		{
			return this->m_bConnected;
		}

		VOID CPCCommEngine::SetDCBRate( const DWORD BaudRate )
		{
			this->m_dConfig.BaudRate = BaudRate;
		}

		BOOL CPCCommEngine::CloseComm()
		{
			if(this->m_bConnected)
			{
				if(!CloseHandle(this->m_hCommFile))
				{
					return FALSE;
				}
				this->m_bConnected = FALSE;
				this->m_hCommFile = INVALID_HANDLE_VALUE;
			}
			return TRUE;
		}

		DWORD CPCCommEngine::ReadComm(LPVOID cpRXBuffer, OVERLAPPED* olRead)
		{
			if(this->m_bConnected)
			{
				DWORD dwErrorFlags;
				COMSTAT ComStat;
				//Sleep(5);
				ClearCommError(this->m_hCommFile, &dwErrorFlags, &ComStat);
				if(!ComStat.cbInQue)
				{
					return 0;
				}

				DWORD dwLen  = 0;
				BOOL  bResult= ReadFile(this->m_hCommFile, cpRXBuffer, ComStat.cbInQue, &dwLen, olRead);
				if(!bResult)
				{
					if(GetLastError()==ERROR_IO_PENDING)
					{
						GetOverlappedResult(this->m_hCommFile, olRead, &dwLen, 1);
					}
					else	
					{
						return 0;
					}
				}
				::ZeroMemory(&ComStat, sizeof(COMSTAT));
				return dwLen;
			}
			return 0;
		}

		DWORD CPCCommEngine::WriteComm(LPCVOID lpBuffer, DWORD dwLen )
		{
			if(this->m_bConnected && dwLen > 0)
			{
				OVERLAPPED olWrite;
				::ZeroMemory(&olWrite, sizeof(OVERLAPPED));
				olWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
				if(olWrite.hEvent == NULL)
				{
					return 0;
				}

				COMSTAT ComStat;
				DWORD dwErrorFlags;
				ClearCommError(this->m_hCommFile, &dwErrorFlags, &ComStat);

				DWORD length = dwLen;
				BOOL bResult = WriteFile(this->m_hCommFile, lpBuffer, length, &length, &olWrite);
				if(!bResult)
				{
					if(GetLastError() == ERROR_IO_PENDING)
					{
						//!Wait For End
						GetOverlappedResult(this->m_hCommFile, &olWrite, &length, TRUE);
					}
					else	
					{
						length = 0;
					}
				}
				::ZeroMemory(&ComStat, sizeof(COMSTAT));
				::CloseHandle (olWrite.hEvent);
				return length;
			}
			return 0;
		}

		LPCTSTR CPCCommEngine::GetPort() const
		{
			return m_szPort;
		}

		//////////////////////////////////////////////////////////////////////////
		//
		//
		//
		//////////////////////////////////////////////////////////////////////////

		CPCCommEngineCtrl::CPCCommEngineCtrl()
		{
			this->m_pEngineCallback		= NULL;
			this->m_hThread				= NULL;
			this->m_hEventCloseThread	= NULL;
			this->m_bThreadAlive		= FALSE;
		}

		VOID CPCCommEngineCtrl::Close()
		{
			DWORD dwR = 0;
			for(;;)
			{
				if(this->m_CommEngine.GetCommFile() && m_bThreadAlive)
				{
					dwR = ResumeThread(this->m_hThread);
					if((dwR > 1) && (dwR != 0xFFFFFFFF))	
					{
						continue;
					}
				}
				SetEvent(m_hEventCloseThread);
				if(!m_bThreadAlive)	
				{
					break;
				}
			}

			::CloseHandle(m_hEventCloseThread);
			m_hEventCloseThread = NULL;

			//
			GetSerialEngine()->CloseComm();

			//
			OutputDebugString(_T("[COM Object Deleted]\n"));
		}

		BOOL CPCCommEngineCtrl::ClearBuffer()
		{
			HANDLE hFile = m_CommEngine.GetCommFile();
			BOOL bOK = PurgeComm(hFile, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
			return bOK;
		}

		CPCCommEngine* CPCCommEngineCtrl::GetSerialEngine()
		{
			return &m_CommEngine;
		}

		CPCCommEngineCtrl::~CPCCommEngineCtrl()
		{
			Close();	
		}

		//! Init the Thread and COM Engine
		//! Set the CallBack Point
		BOOL CPCCommEngineCtrl::Initialize(CPCCommEngineCallBack* notify, LPCTSTR pszPort /* = _T */)
		{
			assert(notify != NULL);
			this->m_pEngineCallback = notify;

			if(!this->m_CommEngine.OpenComm(pszPort))
			{
				return FALSE;
			}

			//! Set Close Event
			if (m_hEventCloseThread != NULL)	
			{
				ResetEvent(m_hEventCloseThread);
			}
			m_hEventCloseThread = CreateEvent(NULL, TRUE, FALSE, NULL);
			if (m_hEventCloseThread == NULL)	
			{
				::CloseHandle(this->m_CommEngine.GetCommFile());
				return FALSE;
			}

			//
			DWORD dwThreadID;
			this->m_hThread = BeginThread(NULL, 0, CPCCommEngineCtrl::ComEngineReadThread, 
				(LPVOID)this, 0, &dwThreadID);
			if( this->m_hThread == NULL)
			{
				return FALSE;			
			}
			CloseHandle(this->m_hThread);

			//
			return TRUE;
		}

		DWORD CPCCommEngineCtrl::WriteComm( DWORD aLen, LPCVOID aData )
		{
			DWORD aRetLen = this->m_CommEngine.WriteComm(aData, aLen);
			this->m_pEngineCallback->OnWriteCommCompeleted(aData, aRetLen);
			//!
			return aRetLen;
		}

		DWORD WINAPI CPCCommEngineCtrl::ComEngineReadThread( LPVOID lpParam )
		{
			CPCCommEngineCtrl* Engine = (CPCCommEngineCtrl*)lpParam;
			CRITICAL_SECTION Section;
			InitializeCriticalSection(&Section);

			//! OverLapped
			OVERLAPPED olRead;
			::ZeroMemory(&olRead, sizeof(OVERLAPPED));
			olRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
			if(olRead.hEvent == NULL)	
			{
				return (DWORD)-1;
			}

			//! Set Alive Flag
			Engine->m_bThreadAlive = TRUE;

			//! Wait Handle		
			HANDLE hEventArray[2];
			hEventArray[0] = Engine->m_hEventCloseThread;
			hEventArray[1] = olRead.hEvent;	

			HANDLE hFile = Engine->m_CommEngine.GetCommFile();
			if(hFile)
			{
				//! check if the port is opened
				PurgeComm(hFile, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
			}

			DWORD Event = 0;
			DWORD CommEvent = 0;
			COMSTAT comstat;
			LPBYTE pRXBuffer = new BYTE[MAXBLOCK];
			for(;;)
			{
				//! Wait For Comm Event
				BOOL bError  = 0;
				BOOL bResult = WaitCommEvent(hFile, &Event, &olRead);
				if(!bResult)
				{
					if(GetLastError() != ERROR_IO_PENDING)
					{
						bError = 1;
					}
				}
				else
				{
					DWORD dwError = 0;
					bResult = ClearCommError(hFile, &dwError, &comstat);
					if (comstat.cbInQue == 0)
					{
						continue;
					}
				}
				if(bError)	
				{
					break;
				}

				//! Wait For Multiple Objects
				Event = WaitForMultipleObjects(2, hEventArray, FALSE, INFINITE);
				switch (Event)
				{
				case 0:
					{
						//Engine->m_bThreadAlive = FALSE;
						//bError = 1;
						//delete[] pRXBuffer;
						//pRXBuffer = NULL;

						bError = 1;
					}
					break;
				case 1:
					{
						GetCommMask(hFile, &CommEvent);				
						if (CommEvent & EV_RXCHAR)	
						{
							memset(pRXBuffer, 0, MAXBLOCK);
							DWORD dwReadLen = Engine->m_CommEngine.ReadComm(pRXBuffer, &olRead);
							if(dwReadLen)
							{
								EnterCriticalSection(&Section);
								Engine->m_pEngineCallback->OnReadCommCompeleted(pRXBuffer, dwReadLen);
								LeaveCriticalSection(&Section);

							}
						}
					}
					break;
				default:
					bError = 1;
					break;
				}
				if(bError)
				{
					break;
				}
			}

			//! Clear Alive Flag
			delete[] pRXBuffer;
			pRXBuffer = NULL;

			::ZeroMemory(&comstat, sizeof(COMSTAT));
			CloseHandle(olRead.hEvent);
			DeleteCriticalSection(&Section);

			//
			OutputDebugString(_T("[COM Thread End]\n"));
			Engine->m_bThreadAlive = FALSE;
			return TRUE;
		}
	}
}
