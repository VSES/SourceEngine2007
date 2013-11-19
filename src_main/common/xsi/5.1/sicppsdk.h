//*****************************************************************************
/*
   \file sicppsdk.h 
   \brief Defines the initialization routines for the library.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights 
   reserved. This file contains confidential and proprietary information of 
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI 
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif 


#ifndef __SICPPSDK_H__
#define __SICPPSDK_H__

//********************************************************************
//
// Includes
//
//********************************************************************
#ifdef unix
#include <values.h>
#else
#include <FLOAT.H>
#pragma warning( disable : 4251 )
#pragma warning( disable : 4275 )
#endif

#include <limits.h>

#if defined(unix) && (defined(WIN64) || defined(_WIN64))
typedef unsigned int ULONG;
typedef int LONG;
#else
typedef unsigned long ULONG;
typedef long LONG;
#endif

typedef int INT;
typedef unsigned int UINT;

typedef short SHORT;
typedef unsigned short USHORT;

typedef USHORT WORD;
typedef ULONG  DWORD;

//typedef int BOOL;

			 
//********************************************************************
//
// Forward declarations
//
//********************************************************************

//********************************************************************
//
// Typedefs 
//
//********************************************************************

//********************************************************************
//
// Defines
//
//********************************************************************
#ifndef SICPPSDKDECL
	#ifndef unix
		#ifdef SICPPSDKIMPL
			#define SICPPSDKDECL __declspec(dllexport)
		#else
			#define SICPPSDKDECL __declspec(dllimport) 
		#endif
	#else
		#define SICPPSDKDECL
	#endif
#endif

#ifndef XSIPLUGINCALLBACK
	// Use this define to properly export C++ API callbacks
	// in your dll, for example XSILoadPlugin()
	// Note for windows developers: Using this define means that you do not need to use a .def file 
	
	#ifdef unix
		#define XSIPLUGINCALLBACK extern "C"
	#else
		// XSI C++ API uses C-linkage.  Disable the warning
		// that reminds us that CStatus is a C++ object
		#pragma warning( disable : 4190 ) 
		#define XSIPLUGINCALLBACK extern "C" __declspec(dllexport)
	#endif
#endif

#endif // __SICPPSDK_H__
