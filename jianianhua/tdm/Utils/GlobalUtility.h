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

#ifndef _GLOBAL_UTILITY_H
#define _GLOBAL_UTILITY_H
//
#include "../Include.h"
#include <vector>
using namespace std;

//
namespace Utils
{

	class CGlobalUtility
	{
	public:
		CGlobalUtility(void);
		~CGlobalUtility(void);

	public:
		static LPTSTR	GetDate(LPTSTR lpTime);
		static LPTSTR	GetTime(LPTSTR lpTime);
		static LPTSTR	GetDateTime(LPTSTR lpTime);
		static LPTSTR	GetDir();
		//
		static PTSTR	BigNumToString(LONGLONG lNum, PTSTR szBuf); 
		//
		static LPTSTR	UtfToChar(LPCSTR pInput);
		static LPSTR	CharToUtf(LPCTSTR pInput);
		//
		static LPTSTR	WCharToChar(LPCWSTR pInput, LPDWORD lpRetLen = NULL);
		//
		static LPTSTR	OemToChar(LPCSTR pInput, LPDWORD lpRetLen = NULL);
		static LPSTR	CharToOem(LPCTSTR pInput, LPDWORD lpRetLen = NULL);
		// 
		static LPTSTR	CustomToChar(LPCSTR pInput, UINT uCodePage, LPDWORD lpRetLen = NULL);
		static LPSTR	CharToCustom(LPCTSTR pInput, UINT uCodePage, LPDWORD lpRetLen = NULL);
		//
		static HANDLE CreateEvent(
			BOOL bManualReset = FALSE,
			BOOL bInitialState = FALSE);
		//
// 		static HANDLE CreateThread(
// 			LPTHREAD_START_ROUTINE lpStartAddress,
// 			LPVOID lpParameter = NULL,
// 			LPDWORD lpThreadId = NULL);
		//
// 		static BOOL		CreateNormalProcess(LPCTSTR lpCommandLine, BOOL bHide = TRUE, 
// 			LPPROCESS_INFORMATION lpProInfor = NULL);
		//
		static BOOL		FileTimeToSystemTime(	const LPFILETIME pFileTime, 
												LPSYSTEMTIME pSystemTime);
		
		//
		static BOOL		IsFileExist(LPCTSTR lpFilePath);
		static BOOL		IsHasFile(LPCTSTR lpDirPath, LPCTSTR lpFilePath, BOOL bUseDirectory = FALSE);

		//
		static BOOL		Extractfile(LPCTSTR lpType, LPCTSTR lpName, LPCTSTR lpPath, HMODULE hInstance);

		//
		class CProcessInfo
		{
		public:
			DWORD dwPID;
			TCHAR szProcess[MAX_PATH];
		};
		static DWORD	ListProcesses(vector<CProcessInfo>& vecProcess);
		static BOOL		TerminateProcess(DWORD dwProcessId);

		//
		static BOOL		ReadFileBytes(HANDLE hFile, LPVOID lpBuffer, DWORD dwSize);
//
// 		IMAGE_SUBSYSTEM_UNKNOWN              
// 		IMAGE_SUBSYSTEM_NATIVE               
// 		IMAGE_SUBSYSTEM_WINDOWS_GUI          
// 		IMAGE_SUBSYSTEM_WINDOWS_CUI          
// 		IMAGE_SUBSYSTEM_OS2_CUI              
// 		IMAGE_SUBSYSTEM_POSIX_CUI            
// 		IMAGE_SUBSYSTEM_NATIVE_WINDOWS       
// 		IMAGE_SUBSYSTEM_WINDOWS_CE_GUI       
// 		IMAGE_SUBSYSTEM_EFI_APPLICATION      
// 		IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER
// 		IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER   
// 		IMAGE_SUBSYSTEM_EFI_ROM              
// 		IMAGE_SUBSYSTEM_XBOX                 
// 		IMAGE_SUBSYSTEM_WINDOWS_BOOT_APPLICATION
//
		static WORD		GetPEFileType(LPCTSTR lpFilePath);

		//
		static BOOL		GetTemporaryFilePath(LPTSTR lpFilePath);

		//
		static char*	strtok(char* pString, const char* pControl, char** pContext);

		template<typename _Type>
		static _Type*	ArrayToken(_Type* pString, const _Type* pControl, _Type** pContext);

	private:
		static TCHAR	m_szTime[MAX_PATH];
		static TCHAR	m_szFileDir[MAX_PATH];
	};

	template<typename _Type>
	_Type* CGlobalUtility::ArrayToken(_Type* pString, const _Type* pControl, _Type** pContext)
	{
		const _Type* pSpanp;
		_Type c, sc;
		_Type* tok;
	
		if (pString == NULL && (pString = *pContext) == NULL)
		{
			return (NULL);
		}

		//
		// Skip (span) leading delimiters (s += strspn(s, delim), sort of).
		//
cont:
		c = *pString++;
		for (pSpanp = pControl; (sc = *pSpanp++) != 0;)
		{
			if (c == sc)
			{
				goto cont;
			}
		}

		/* no non-delimiter characters */
		if (c == 0)
		{
			*pContext = NULL;
			return (NULL);
		}
		tok = pString - 1;

		/*
		* Scan token (scan for delimiters: s += strcspn(s, delim), sort of).
		* Note that delim must have one NUL; we stop if we see that, too.
		*/
		for (;;) {
			c = *pString++;
			pSpanp = pControl;
			do {
				if ((sc = *pSpanp++) == c) {
					if (c == 0)
						pString = NULL;
					else
						pString[-1] = 0;
					*pContext = pString;
					return (tok);
				}
			} while (sc != 0);
		}
		return NULL;
	}
}

#endif
