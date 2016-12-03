//
// C++ Interface: ErrorMessage
//
// Description: 
//
// Author:    xltyt, <xltytlive@hotmail.com>, (C) 2009
// Created:   2009-3-6
//
// Copyright: See COPYING file that comes with this distribution
//
#include "stdafx.h"
#include "ErrorMessage.h"

namespace Utils
{
#define LAN			LANG_CHINESE
#define SUB_LAN		SUBLANG_CHINESE_SIMPLIFIED

	CErrorMessage::CErrorMessage(void)
	{
	}

	CErrorMessage::~CErrorMessage(void)
	{
	}

	BOOL CErrorMessage::ShowErrorMessage(LPCTSTR szError)
	{
		// Get the error code
		DWORD dwError = GetLastError();

		//	  GetDlgItemText()
		HLOCAL hlocal = NULL;   // Buffer that gets the error message string

		// Get the error code's textual description
		BOOL fOk = FormatMessage(
			FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, 
			NULL, dwError, MAKELANGID( LAN,SUB_LAN ),
			(PTSTR) &hlocal, 0, NULL);

		/*
		if (!fOk) 
		{
			// Is it a network-related error?
			HMODULE hDll = LoadLibraryEx(TEXT("netmsg.dll"), NULL, 
				DONT_RESOLVE_DLL_REFERENCES);

			if (hDll != NULL) 
			{
				FormatMessage(
					FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_FROM_SYSTEM,
					hDll, dwError, MAKELANGID(LAN, SUB_LAN),
					(PTSTR) &hlocal, 0, NULL);
				FreeLibrary(hDll);
			}
		}
		*/

		if(fOk) 
		{
			PCTSTR	szResult = (PCTSTR)LocalLock(hlocal);
			TCHAR	szTemp[MAX_PATH];
			wsprintf(szTemp, _T("\r\n´íÎóÂë(%u):"), dwError);

			TCHAR* szBuffer = new TCHAR[lstrlen(szResult) + lstrlen(szError) + 20];
			lstrcpy(szBuffer, szError);
			lstrcat(szBuffer, szTemp);
			lstrcat(szBuffer, szResult);
			// Show Error Information
			MessageBox(NULL, szBuffer, _T("Error"), MB_ICONERROR);

			// Free Memory
			LocalFree(hlocal);
			delete [] szBuffer;
		}
		else
		{
			TCHAR	szTemp[MAX_PATH];
			wsprintf(szTemp, _T("\r\n´íÎóÂë(%u):"), dwError);

			TCHAR* szBuffer = new TCHAR[lstrlen(szError) + 20];
			lstrcpy(szBuffer, szError);
			lstrcat(szBuffer, szTemp);
			// Show Error Information
			MessageBox(NULL, szBuffer, _T("Error"), MB_ICONERROR);

			// Free Memory
			delete [] szBuffer;
		}
		return TRUE;
	}
}