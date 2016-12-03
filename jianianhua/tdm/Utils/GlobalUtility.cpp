//
// C++ Interface: GlobalUtility
//
// Description: 
//
// Author:    xltyt, <xltytlive@hotmail.com>, (C) 2009
// Created:   2009-3-6
//
// Copyright: See COPYING file that comes with this distribution
//
#include "stdafx.h"
#include "GlobalUtility.h"
#include <atlbase.h>
#include <ATLComTime.h>

#include <Tlhelp32.h>
#ifdef UNDER_CE
	#pragma comment(lib, "Toolhelp.lib")
#endif

namespace Utils
{
	TCHAR CGlobalUtility::m_szTime[MAX_PATH];
	TCHAR CGlobalUtility::m_szFileDir[MAX_PATH];

	CGlobalUtility::CGlobalUtility(void)
	{
	}

	CGlobalUtility::~CGlobalUtility(void)
	{
	}

	LPTSTR CGlobalUtility::GetDate(LPTSTR lpTime)
	{		
#if _MSC_VER > 1200
// 		COleDateTime DateTime = COleDateTime::GetCurrentTime();
// 		wsprintf(lpTime, _T("%04d-%02d-%02d"),
// 			DateTime.GetYear(), DateTime.GetMonth(), DateTime.GetDay()
// 			);
		
#else
		__int64 ltime;
		_time64( &ltime );
		SYSTEMTIME sysTime;
		::VariantTimeToSystemTime(ltime, &sysTime);
		wsprintf(lpTime, _T("%04d-%02d-%02d"),
			sysTime.wYear, sysTime.wMonth, sysTime.wDay
			);
#endif
		return lpTime;
	}

	LPTSTR CGlobalUtility::GetTime(LPTSTR lpTime)
	{
		COleDateTime DateTime = COleDateTime::GetCurrentTime();
		wsprintf(lpTime, _T("%02d.%02d.%02d"),
			DateTime.GetHour(), DateTime.GetMinute(), DateTime.GetSecond()
		);

// #if _MSC_VER > 1200
// 		COleDateTime DateTime = COleDateTime::GetCurrentTime();
// 		wsprintf(lpTime, _T("%02d.%02d.%02d"),
// 			DateTime.GetHour(), DateTime.GetMinute(), DateTime.GetSecond()
// 			);
// #else
		/*
		__int64 ltime;
		_time64( &ltime );
		SYSTEMTIME sysTime;
		::VariantTimeToSystemTime((double)ltime, &sysTime);
		wsprintf(lpTime, _T("%04d-%02d-%02d"),
			sysTime.wHour, sysTime.wMinute, sysTime.wSecond
			);
		*/
//#endif
		return lpTime;
	}

	LPTSTR CGlobalUtility::GetDateTime(LPTSTR lpTime)
	{
		COleDateTime DateTime = COleDateTime::GetCurrentTime();
		wsprintf(lpTime, _T("%04d-%02d-%02d %02d%02d%02d"),
			DateTime.GetYear(), DateTime.GetMonth(), DateTime.GetDay(),
			DateTime.GetHour(), DateTime.GetMinute(), DateTime.GetSecond()
			);

// #if _MSC_VER > 1200
// 		COleDateTime DateTime = COleDateTime::GetCurrentTime();
// 		wsprintf(lpTime, _T("%04d-%02d-%02d %02d:%02d:%02d"),
// 			DateTime.GetYear(), DateTime.GetMonth(), DateTime.GetDay(),
// 			DateTime.GetHour(), DateTime.GetMinute(), DateTime.GetSecond()
// 			);
// #else
		/*
		__int64 ltime;
		_time64( &ltime );
		SYSTEMTIME sysTime;
		::VariantTimeToSystemTime((double)ltime, &sysTime);
		wsprintf(lpTime, _T("%04d-%02d-%02d %02d:%02d:%02d"),
			sysTime.wYear, sysTime.wMonth, sysTime.wDay,
			sysTime.wHour, sysTime.wMinute, sysTime.wSecond
			);
		*/
//#endif
		return lpTime;
	}

	LPTSTR CGlobalUtility::GetDir()
	{
		memset(m_szFileDir, 0, sizeof(m_szFileDir));
		GetModuleFileName(NULL, m_szFileDir, MAX_PATH);
		*_tcsrchr(m_szFileDir,_T('\\')) = 0;
		return m_szFileDir;
	}

	PTSTR CGlobalUtility::BigNumToString(LONGLONG lNum, PTSTR szBuf) 
	{
		TCHAR szNum[100];
		wsprintf(szNum, TEXT("%d"), lNum);
		NUMBERFMT nf;
		nf.NumDigits		= 0;
		nf.LeadingZero		= FALSE;
		nf.Grouping			= 3;
		nf.lpDecimalSep		= TEXT(".");
		nf.lpThousandSep	= TEXT(",");
		nf.NegativeOrder	= 0;
		GetNumberFormat(LOCALE_USER_DEFAULT, 0, szNum, &nf, szBuf, 100);
		return(szBuf);
	}

	//////////////////////////////////////////////////////////////////////////

	LPTSTR CGlobalUtility::UtfToChar(LPCSTR pInput)
	{
		return CustomToChar(pInput, CP_UTF8, NULL);
	}

	LPSTR CGlobalUtility::CharToUtf(LPCTSTR pInput)
	{
		return CharToCustom(pInput, CP_UTF8, NULL);
	}

	//////////////////////////////////////////////////////////////////////////

	LPTSTR CGlobalUtility::WCharToChar(LPCWSTR pInput, LPDWORD lpRetLen /*= NULL*/)
	{
#ifdef UNICODE
		DWORD dwDataLen = lstrlenW(pInput);
		PWCHAR pText = new WCHAR[dwDataLen + 1];
		lstrcpyW(pText, pInput);
		if(lpRetLen) *lpRetLen = dwDataLen;
#else
		// Convert
		DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, pInput, -1, NULL, 0, NULL, FALSE);
		PCHAR pText = new CHAR[dwNum];
		WideCharToMultiByte(CP_OEMCP, NULL, pInput, -1, pText, dwNum, NULL, FALSE);
		if(lpRetLen) *lpRetLen = dwNum - 1;
#endif
		return pText;
	}

	//////////////////////////////////////////////////////////////////////////

	LPTSTR CGlobalUtility::OemToChar(LPCSTR pInput, LPDWORD lpRetLen /*= NULL*/)
	{
#ifdef UNICODE
		// Convert
		DWORD dwNum = MultiByteToWideChar(CP_ACP, 0, pInput, -1, NULL, 0);
		PWCHAR pText = new WCHAR[dwNum];
		MultiByteToWideChar (CP_ACP, 0, pInput, -1, pText, dwNum);
		if(lpRetLen) *lpRetLen = dwNum - 1;
		return pText;
#else
		DWORD dwDataLen = lstrlenA(pInput);
		PCHAR pText = new CHAR[dwDataLen + 1];
		lstrcpyA(pText, pInput);
		if(lpRetLen) *lpRetLen = dwDataLen;
#endif
		return pText;
	}

	LPSTR CGlobalUtility::CharToOem(LPCTSTR pInput, LPDWORD lpRetLen /*= NULL*/)
	{
#ifdef UNICODE
		// Convert
		DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, pInput, -1, NULL, 0, NULL, FALSE);
		PCHAR pText = new CHAR[dwNum];
		WideCharToMultiByte(CP_OEMCP, NULL, pInput, -1, pText, dwNum, NULL, FALSE);
		if(lpRetLen) *lpRetLen = dwNum - 1;
		return pText;
#else
		DWORD dwDataLen = lstrlenA(pInput);
		PCHAR pText = new CHAR[dwDataLen + 1];
		lstrcpyA(pText, pInput);
		if(lpRetLen) *lpRetLen = dwDataLen;
#endif
		return pText;	
	}

	//////////////////////////////////////////////////////////////////////////

	LPTSTR CGlobalUtility::CustomToChar( LPCSTR pInput, UINT uCodePage, LPDWORD lpRetLen /*= NULL*/ )
	{
		// Convert UTF to Unicode
		INT iUnicodeSize = MultiByteToWideChar(uCodePage, 0, pInput, -1, NULL, 0);
		PWCHAR pUnicode = new WCHAR[iUnicodeSize + 1];  
		memset(pUnicode, 0, (iUnicodeSize + 1) * sizeof(WCHAR)); 
		MultiByteToWideChar(uCodePage, 0, pInput, -1, (LPWSTR)pUnicode, iUnicodeSize);  
#ifndef UNICODE
		//
		INT iAnsiSize = WideCharToMultiByte(CP_OEMCP,NULL, pUnicode, -1, NULL, 0, NULL, FALSE);
		LPSTR lpszStr = new CHAR[iAnsiSize];
		WideCharToMultiByte(CP_OEMCP, NULL, pUnicode, -1, lpszStr, iAnsiSize, NULL, FALSE);
		delete[] pUnicode;
		pUnicode = NULL;
		return lpszStr;
#else
		//
		return pUnicode;
#endif
	}

	LPSTR CGlobalUtility::CharToCustom( LPCTSTR pInput, UINT uCodePage, LPDWORD lpRetLen /*= NULL*/ )
	{
#ifndef UNICODE
		// Convert
		DWORD dwNum = MultiByteToWideChar(CP_ACP, 0, pInput, -1, NULL, 0);
		PWCHAR pUnicode = new WCHAR[dwNum];
		MultiByteToWideChar (CP_ACP, 0, pInput, -1, pUnicode, dwNum);

		dwNum = WideCharToMultiByte(uCodePage, NULL, pUnicode, -1, NULL, 0, NULL, FALSE);
		PCHAR pText = new CHAR[dwNum];
		WideCharToMultiByte(uCodePage, NULL, pUnicode, -1, pText, dwNum, NULL, FALSE);
		delete[] pUnicode;
		pUnicode = NULL;
#else
		// Convert TCHAR to UTF
		DWORD dwNum = WideCharToMultiByte(uCodePage, NULL, pInput, -1, NULL, 0, NULL, FALSE);
		PCHAR pText = new CHAR[dwNum];
		WideCharToMultiByte(uCodePage, NULL, pInput, -1, pText, dwNum, NULL, FALSE);
#endif
		return pText;
	}

	HANDLE CGlobalUtility::CreateEvent(
		BOOL bManualReset /*= FALSE*/,
		BOOL bInitialState /*= FALSE*/)
	{
		return ::CreateEvent(NULL, bManualReset, bInitialState, NULL);
	}
// 
// 	HANDLE CGlobalUtility::BeginThread(
// 		LPTHREAD_START_ROUTINE lpStartAddress,
// 		LPVOID lpParameter /*= NULL*/,
// 		LPDWORD lpThreadId /*= NULL*/)
// 	{
// 		return ::CreateThread(NULL, 0, lpStartAddress, (LPVOID)lpParameter, 0, lpThreadId);
// 	}
// 
// 	BOOL CGlobalUtility::CreateNormalProcess(LPCTSTR lpCommandLine, BOOL bHide /*= TRUE*/, 
// 		LPPROCESS_INFORMATION lpProInfor /*= NULL*/)
// 	{
// 		// Create Process
// 		STARTUPINFO si;
// 		ZeroMemory(&si, sizeof(si));
// 		si.cb			= sizeof(si);
// 		if(bHide)
// 		{
// 			si.wShowWindow	= SW_HIDE;
// 			si.dwFlags		= STARTF_USESHOWWINDOW|STARTF_USESTDHANDLES;
// 		}
// 		else
// 		{
// 			si.wShowWindow	= SW_NORMAL;
// 			si.dwFlags		= STARTF_USEPOSITION|STARTF_USESIZE;
// 		}
// 		/*
// 		#ifndef _DEBUG
// 		si.lpDesktop	= _T("Winsta0\\Winlogon");
// 		#endif
// 		*/
// 
// 		PROCESS_INFORMATION pi;
// 		ZeroMemory(&pi, sizeof(pi));
// 
// 		// Start the child process.
// 		TCHAR lpCommand[512] = {0};
// 		wsprintf(lpCommand, _T("%s"), lpCommandLine);
// 		BOOL bOK = ::CreateProcess(
// 			NULL, lpCommand, NULL, NULL, FALSE, CREATE_NEW_CONSOLE,
// 			NULL, NULL, &si, &pi);
// 
// 		if(lpProInfor)
// 		{
// 			memcpy(lpProInfor, &pi, sizeof(PROCESS_INFORMATION));
// 		}
// 		return bOK;
// 	}

	BOOL CGlobalUtility::FileTimeToSystemTime(
		const LPFILETIME pFileTime, 
		LPSYSTEMTIME pSystemTime)
	{
		::FileTimeToSystemTime(pFileTime, pSystemTime);
		/*
		SYSTEMTIME stUTC;
		// Convert the last-write time to local time.
		::FileTimeToSystemTime(pFileTime, &stUTC);
		//
		TIME_ZONE_INFORMATION ZoneInformation;
		memset(&ZoneInformation, 0, sizeof(TIME_ZONE_INFORMATION));
		// In Minute
		// UTC = local time + bias
		// UTC = GMT+8 + (- 8 * 60)
		ZoneInformation.Bias = - 8 * 60;

		//GetTimeZoneInformation(&ZoneInformation);
		::SystemTimeToTzSpecificLocalTime(&ZoneInformation, &stUTC, pSystemTime);
		*/
		return TRUE;
	}

	BOOL CGlobalUtility::IsFileExist(LPCTSTR lpFilePath)
	{
		BOOL bFound = FALSE;
		WIN32_FIND_DATA FindData;
		HANDLE hContext = FindFirstFile(lpFilePath, &FindData);
		if(hContext != INVALID_HANDLE_VALUE)
		{
			bFound = TRUE;
			::FindClose(hContext);
		}
		return bFound;
	}

	BOOL CGlobalUtility::IsHasFile(LPCTSTR lpDirPath, LPCTSTR lpFilePath, BOOL bUseDirectory /*= FALSE*/)
	{
		//
		BOOL bExist = FALSE;
		//
		TCHAR szDirPath[MAX_PATH] = {0};
		lstrcpy(szDirPath, lpDirPath);
		if(szDirPath[lstrlen(szDirPath) - 1] != _T('\\'))
		{
			lstrcat(szDirPath, _T("\\"));
		}

		TCHAR szDirPathFilter[MAX_PATH] = {0};
		lstrcpy(szDirPathFilter, szDirPath);
		lstrcat(szDirPathFilter, _T("*.*"));

		BOOL bFound = TRUE;
		WIN32_FIND_DATA FindData;
		HANDLE m_hContext = FindFirstFile(szDirPathFilter, &FindData);
		if(m_hContext == INVALID_HANDLE_VALUE)
		{
			return FALSE;
		}

		while(bFound)
		{
			if(	lstrcmp(FindData.cFileName, _T(".")) != 0 &&
				lstrcmp(FindData.cFileName, _T(".."))!= 0)
			{
				//
				// Directory
				//
				BOOL bDirectory = FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
				if(bDirectory)
				{
					if(bUseDirectory == TRUE)
					{
						if(lstrcmpi(lpFilePath, FindData.cFileName) == 0)
						{
							bExist = TRUE;
							break;
						}
					}
				}
				//
				// File
				//
				else
				{
					if(bUseDirectory == FALSE)
					{
						if(lstrcmpi(lpFilePath, FindData.cFileName) == 0)
						{
							bExist = TRUE;
							break;
						}
					}
				}
			}
			bFound = FindNextFile(m_hContext, &FindData);
		}
		::FindClose(m_hContext);
		return bExist;
	}

	BOOL CGlobalUtility::Extractfile(LPCTSTR lpType, LPCTSTR lpName, LPCTSTR lpPath, HMODULE hInstance)
	{
		// Find and Load the resource containing file(s) data
		HRSRC hResLoad = FindResource(hInstance, lpType, lpName);
		HGLOBAL hResData = LoadResource(hInstance, hResLoad);
		LPCSTR data = (LPCSTR)LockResource(hResData);

		// We should have properly loaded the resource
		assert(data != NULL && hResData != NULL);

		DWORD dwFileLen = SizeofResource(hInstance, hResLoad);
		HANDLE hFileHandle = CreateFile(lpPath, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, 0);
		if(hFileHandle == INVALID_HANDLE_VALUE)
		{
			return FALSE;
		}

		SetFilePointer(hFileHandle, 0, 0, FILE_BEGIN);   
		DWORD dwBytesWrite;
		if(!WriteFile(hFileHandle, data, dwFileLen, &dwBytesWrite, 0))
		{
			CloseHandle(hFileHandle);
			return FALSE;
		}
		CloseHandle(hFileHandle);

		// Set Attributes
		//SetFileAttributes(lpPath, FILE_ATTRIBUTE_SYSTEM|FILE_ATTRIBUTE_HIDDEN);

#ifdef UNDER_CE
#else
		// Perform cleanup
		FreeResource(hResData);
#endif

		return TRUE;
	}

#pragma warning(push)
#pragma warning(disable:4996)
	DWORD CGlobalUtility::ListProcesses(vector<CProcessInfo>& vecProcess)
	{
		//
		vecProcess.clear();

		//
		HANDLE hSnapshot;
		hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if(hSnapshot == INVALID_HANDLE_VALUE)
		{
			return 0;
		}

		//
		PROCESSENTRY32 ProcessEntry;
		ProcessEntry.dwSize = sizeof(PROCESSENTRY32);
		
		BOOL bSuccess;
		bSuccess = Process32First(hSnapshot, &ProcessEntry);
		while(bSuccess)
		{
			CProcessInfo Info;
			Info.dwPID = ProcessEntry.th32ProcessID;
			_tcscpy(Info.szProcess, ProcessEntry.szExeFile);
			vecProcess.push_back(Info);
			
			//
			bSuccess = Process32Next(hSnapshot, &ProcessEntry);
		}
		CloseHandle(hSnapshot);

		//
		return vecProcess.size();
	}
#pragma warning(pop)

	BOOL CGlobalUtility::TerminateProcess(DWORD dwProcessId)
	{
		HANDLE hProcess = OpenProcess(0, FALSE, dwProcessId);
		if(hProcess == NULL)
		{
			return FALSE;
		}
		return ::TerminateProcess(hProcess, 0);
	}

	BOOL CGlobalUtility::ReadFileBytes(HANDLE hFile, LPVOID lpBuffer, DWORD dwSize)
	{
		DWORD dwBytes = 0;   
		if (!ReadFile (hFile, lpBuffer, dwSize, &dwBytes, NULL))
		{
			return FALSE;
		}

		if (dwSize != dwBytes)
		{
			return FALSE;
		}
		return TRUE; 
	}

	//
	/*
	#define IMAGE_SIZEOF_NT_OPTIONAL32_HEADER    224
	#define IMAGE_SIZEOF_NT_OPTIONAL64_HEADER    240
	#ifdef _WIN64
		#define IMAGE_SIZEOF_NT_OPTIONAL_HEADER IMAGE_SIZEOF_NT_OPTIONAL64_HEADER
	#else
		#define IMAGE_SIZEOF_NT_OPTIONAL_HEADER IMAGE_SIZEOF_NT_OPTIONAL32_HEADER
	#endif
	*/

	//
	WORD CGlobalUtility::GetPEFileType(LPCTSTR lpFilePath)
	{
		HANDLE hImage = INVALID_HANDLE_VALUE;
		WORD wType = -1;
		while(1)
		{
			// Open the application file. 
			hImage = CreateFile (lpFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL, NULL);
			if (hImage == INVALID_HANDLE_VALUE)
			{
				break;
			}

			// Read MS-Dos image header.
			IMAGE_DOS_HEADER dos_header;
			if(!ReadFileBytes (hImage, &dos_header, sizeof (IMAGE_DOS_HEADER)))
			{
				break;
			}
			if (dos_header.e_magic != IMAGE_DOS_SIGNATURE)
			{
				break;
			}

			// Read more MS-Dos header. 
			DWORD dwMoreDosHeader[16];
			if(!ReadFileBytes (hImage, dwMoreDosHeader, sizeof (dwMoreDosHeader)))
			{
				break;
			}

			// Move the file pointer to get the actual COFF header. 
			DWORD dwNewOffset;
			DWORD dwCoffHeaderOffset;
			dwNewOffset = SetFilePointer (hImage, dos_header.e_lfanew, NULL, FILE_BEGIN);
			dwCoffHeaderOffset = dwNewOffset + sizeof (ULONG);
			if (dwCoffHeaderOffset == 0xFFFFFFFF)
			{
				break;
			}

			// Read NT signature of the file.
			ULONG ulNTSignature;
			if (!ReadFileBytes (hImage, &ulNTSignature, sizeof (ULONG)))
			{
				break;
			}
			if (ulNTSignature != IMAGE_NT_SIGNATURE)
			{
				break;
			}

			//
			IMAGE_FILE_HEADER file_header;
			if (!ReadFileBytes (hImage, &file_header, IMAGE_SIZEOF_FILE_HEADER))
			{
				break;
			}

			// Read the optional header of file. 
			IMAGE_OPTIONAL_HEADER optional_header;
			if (!ReadFileBytes (hImage, &optional_header, sizeof(IMAGE_OPTIONAL_HEADER)))
				//if (!ReadFileBytes (hImage, &optional_header, IMAGE_SIZEOF_NT_OPTIONAL_HEADER))
			{
				break;
			}

			//
			wType = optional_header.Subsystem;

			//
			break;
		}
		if(hImage != INVALID_HANDLE_VALUE)
		{
			CloseHandle(hImage);
			hImage = INVALID_HANDLE_VALUE;
		}
		return wType;
	}

#pragma warning(push)
#pragma warning(disable:4996)
	BOOL CGlobalUtility::GetTemporaryFilePath(LPTSTR lpFilePath)
	{
		#define BUFSIZE 512
		TCHAR szPathBuffer[BUFSIZE];
		DWORD dwBufSize = BUFSIZE;

		// Get the temp path.
		DWORD dwRetVal = GetTempPath(
			dwBufSize,			// length of the buffer
			szPathBuffer);		// buffer for path 
		if (dwRetVal > dwBufSize || (dwRetVal == 0))
		{
			return FALSE;
		}

		// Create a temporary file.
		TCHAR szTempName[BUFSIZE];
		UINT uRetVal = GetTempFileName(
			szPathBuffer,		// directory for tmp files
			_T("NEW"),			// temp file name prefix 
			0,					// create unique name 
			szTempName);		// buffer for name 
		if (uRetVal == 0)
		{
			return FALSE;
		}
		#undef BUFSIZE
		_tcscpy(lpFilePath, szTempName);
		return TRUE;
	}
#pragma warning(pop)

	char* CGlobalUtility::strtok(char* pString, const char* pControl, char** pContext)
	{
		unsigned char *str;
		const unsigned char* ctrl = (const unsigned char*)pControl;
		unsigned char map[32];
		int count;

		// Clear control map
		for (count = 0; count < 32; count++)
		{
			map[count] = 0;
		}

		// Set bits in delimiter table
		do
		{
			map[*ctrl >> 3] |= (1 << (*ctrl & 7));
		}
		while (*ctrl++);

		// Initialize str

		// If string is NULL, set str to the saved
		// pointer (i.e., continue breaking tokens out of the string
		// from the last strtok call)
		if (pString)
		{
			str = (unsigned char*)pString;
		}
		else
		{
			str = (unsigned char*)*pContext;
		}

		// Find beginning of token (skip over leading delimiters). Note that
		// there is no token if this loop sets str to point to the terminal
		// null (*str == '\0')
		while ( (map[*str >> 3] & (1 << (*str & 7))) && *str )
		{
			str++;
		}

		//
		pString = (char*)str;

		// Find the end of the token. If it is not the end of the string,
		// put a null there.
		for ( ; *str ; str++ )
		{
			if ( map[*str >> 3] & (1 << (*str & 7)) )
			{
				*str++ = '\0';
				break;
			}
		}

		// Update next token (or the corresponding field in the per-thread data
		// structure
		*pContext = (char*)str;

		// Determine if a token has been found.
		if ( pString == (char*)str )
		{
			return NULL;
		}
		else
		{
			return pString;
		}
	}
}
