//***************************************************************************************
//
// File supervisor: Softimage 3D Games & 3D Bridge team
//
// (c) Copyright 2001-2002 Avid Technology, Inc. . All rights reserved.
//
//***************************************************************************************

/****************************************************************************************
THIS CODE IS PUBLISHED AS A SAMPLE ONLY AND IS PROVIDED "AS IS".
IN NO EVENT SHALL SOFTIMAGE, AVID TECHNOLOGY, INC. AND/OR THEIR RESPECTIVE
SUPPLIERS BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY
DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
CONNECTION WITH THE USE OR PERFORMANCE OF THIS CODE .

COPYRIGHT NOTICE. Copyright © 1999-2002 Avid Technology Inc. . All rights reserved. 

SOFTIMAGE is a registered trademark of Avid Technology Inc. or its subsidiaries 
or divisions. Windows NT is a registered trademark of Microsoft Corp. All other
trademarks contained herein are the property of their respective owners. 
****************************************************************************************/


#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

//*********************************************************************************************
//
//
//*********************************************************************************************

#ifndef __DSSTLMFC_H__
#define __DSSTLMFC_H__

// resolve min/max conflict between windef.h and STL
#ifdef min
	#undef min
	#define MIN_WAS_DEFINED
#endif
#ifdef max
	#undef max
	#define MAX_WAS_DEFINED
#endif

// To avoid including <bool.h>
#ifndef _BOOL
#define _BOOL
#endif

// C++ header files
#include <new.h>
#include <iostream.h>
#include <fstream.h>


//******************************************************************************
//
// Use the SGI Adapted Port STL Implementation
//
//******************************************************************************

#include <algorithm>
#include <deque>
#include <functional>
#include <iterator>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <vector>

// some backwards compatibility
#define __BEGIN_STL_NAMESPACE	__STL_BEGIN_NAMESPACE 
#define __END_STL_NAMESPACE		__STL_END_NAMESPACE 

// use only with the unix STL
#ifdef sgi
// A more user friendly define for the namespace name
# define _STL             std
// They changed the namespace define from std to __STD
# define __STL            __STD
# define std  __STD
#endif

// warning, an "#include <math.h>" unref the macros min and max
#define max(a,b)		(((a) > (b)) ? (a) : (b))
#define min(a,b)		(((a) < (b)) ? (a) : (b))

// Done include STL headers
#define __STL_OUTERMOST_HEADER_ID 0

#endif /* __DSSTLMFC_H__ */
