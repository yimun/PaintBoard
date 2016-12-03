#include "stdafx.h"
#include "CheckUtils.h"
#include "MD5/MD5Checksum.h"

namespace Utils
{
	using namespace MD5;

	CCheckUtils::CCheckUtils()
	{
	}

	CCheckUtils::~CCheckUtils()
	{
	}

	BYTE CCheckUtils::GetLRC(LPBYTE pBuffer, DWORD dwLen)
	{
		LPBYTE pPointer = pBuffer;
		BYTE bCheckSum = 0;
		for(DWORD i = 0; i < dwLen; i++)
		{
			bCheckSum += *pPointer++;
		}
		bCheckSum =~ bCheckSum;
		bCheckSum &= 0x00FF;

		return bCheckSum;
	}

	BOOL CCheckUtils::GetMD5(LPBYTE pBuffer, DWORD dwLen, LPTSTR lpRet)
	{
		return CMD5Checksum::GetMD5(pBuffer, dwLen, lpRet);
	}

	BOOL CCheckUtils::GetCRC(LPBYTE pBuffer, DWORD dwLen, LPTSTR lpRet)
	{
		return TRUE;
	}
}
