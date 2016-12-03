//
// C++ Interface: macro
//
// Description: 
//
//
// Author: gaoliang User,,, <gaoliang@gaoliang-desktop>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef __MACRO_H__
#define __MACRO_H__

//The macro descript whether working in debug mode
#define UTILS_DEBUG
#ifdef UTILS_DEBUG
	#ifdef UNDER_CE
	#define MESSAGEBOX_DEBUG// 调试对话框
	#endif 


// #if !defined(UNALIGNED)
// #if defined(_M_IA64) || defined(_M_AMD64)
// #define UNALIGNED __unaligned
// #else
// #define UNALIGNED
// #endif
// #endif
// //数组长度
// #if !defined(_countof)
// #if !defined(__cplusplus)
// #define _countof(_Array) (sizeof(_Array) / sizeof(_Array[0]))
// #else
// extern "C++"
// {
// 	template <typename _CountofType, size_t _SizeOfArray>
// 	char (*__countof_helper(UNALIGNED _CountofType (&_Array)[_SizeOfArray]))[_SizeOfArray];
// #define _countof(_Array) sizeof(*__countof_helper(_Array))
// }
// #endif
// #endif

#endif

//#define UTILS_MUTITHREAD
#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#if defined(WIN32) || defined(UNDER_CE)
	#define CommonType float
#else
	#define CommonType double
#endif

#define dim(x) (sizeof(x) / sizeof(x[0])) 

#if 0
//dump all macros 
#ifdef WIN32
#pragma message("Defined WIN32")
#endif

#ifdef UNDER_CE
#pragma message("Defined UNDER_CE")
#endif

#ifdef UTILS_DEBUG
#pragma message("Defined UTILS_DEBUG")
#endif

#endif // #if 0

#endif // #ifndef __MACRO_H__
