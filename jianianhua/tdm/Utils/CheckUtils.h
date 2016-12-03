#ifndef _CHECK_UTILS_H
#define _CHECK_UTILS_H

#include "../Include.h"

namespace Utils
{
	class CCheckUtils
	{
	public:
		CCheckUtils();
		~CCheckUtils();

	public:
		// LRC
		static BYTE	GetLRC(LPBYTE pBuffer, DWORD dwLen);

		// MD5
		static BOOL GetMD5(LPBYTE pBuffer, DWORD dwLen, LPTSTR lpRet);

		// MD5
		static BOOL GetCRC(LPBYTE pBuffer, DWORD dwLen, LPTSTR lpRet);
	};
}

#endif
