//
// C++ Interface: utils
//
// Description: 
//
//
// Author: gaoliang User,,, <gaoliang@gaoliang-desktop>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef __UTILS_H__
#define __UTILS_H__

#include "macro.h"
#include "debug.h"
#include "utype.h"
#include "DequeEx.h"
using namespace Utils;


//-----------------------------------------------------------------------------
// Summary:
//     Frees dynamically allocated memory.
// Parameters:
//     ptr - Points to the memory to free.
// Remarks:
//     This macro will first check to see if the block of memory specified by
//     <i>ptr</i> is NULL. If <i>ptr</i> is not NULL, SAFE_DELETE will deallocate
//     the block of memory and set its value to NULL. The pointer argument
//     must refer to a block of memory previously allocated for an object
//     created with the new operator.
// Example:
// <code>
// CName* pName = new CName;          // Allocate memory for the object
// pName->SetName("Firstname", "Lastname");
// SAFE_DELETE(pName);                // Deallocate memory for the object
// ASSERT(pName == NULL);
// </code>
//-----------------------------------------------------------------------------
#define SAFE_DELETE(ptr) \
	if (ptr) { delete ptr; ptr = NULL; }

//-----------------------------------------------------------------------------
// Summary:
//     Frees dynamically allocated memory for an array.
// Parameters:
//     ptr - Points to the memory array to free.
// Remarks:
//     This macro will first check to see if the block of memory specified by
//     <i>ptr</i> is NULL. If <i>ptr</i> is not NULL, SAFE_DELETE_AR will deallocate
//     the block of memory and set its value to NULL. The pointer argument
//     must refer to a block of memory previously allocated for an object
//     created with the new operator.
// Example:
// <code>
// char* pCharArray = new char[256];  // Allocate memory for the array
// strcpy(pCharArray, "Array of characters");
// SAFE_DELETE_AR(pCharArray);        // Deallocate memory for the array
// ASSERT(pCharArray == NULL);
// </code>
//-----------------------------------------------------------------------------
#define SAFE_DELETE_AR(ptr) \
	if (ptr) { delete[] ptr; ptr = NULL; }

//
//
//
#define SAFE_CLOSE(ptr) \
	if (ptr) { CloseHandle(ptr); ptr = NULL; }

//
//
//
#define SAFE_CLOSE_FILE(ptr) \
	if (ptr != INVALID_HANDLE_VALUE) { CloseHandle(ptr); ptr = INVALID_HANDLE_VALUE; }


#endif
