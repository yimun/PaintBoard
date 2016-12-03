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

#ifndef _ERROR_MESSAGE_H
#define _ERROR_MESSAGE_H
//
#include "../Include.h"
//
namespace Utils
{
	class CErrorMessage
	{
	public:
		CErrorMessage(void);
		~CErrorMessage(void);
	public:
		static BOOL ShowErrorMessage(LPCTSTR szError);
	};

}
#endif