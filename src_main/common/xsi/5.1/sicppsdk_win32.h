//*****************************************************************************
/*!
   \file sicppsdk_win32.h
   \brief Win32 utilities.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#include <sicppsdk.h>
#include <xsi_value.h>

#ifndef __SICPPSDK_WIN32_H__
#define __SICPPSDK_WIN32_H__

/*! Helper to convert from the Win32 VARIANT structure to a CValue. This function is useful for plug-ins
which mix the COM and C++ versions of the object model in their implementation.

The VARIANT is the data type used internally by scripting languages and, like CValue, it can represent many
different types, including strings, objects, integers, doubles and arrays.  A VARIANT containing a BSTR is
converted to a CValue containing a CString. A VARIANT referring to an %XSI COM object is converted to a %CValue
containing the equivalent XSI::CRef object.  A VARIANT referencing a SAFEARRAY is converted to a CValue
containing a XSI::CValueArray. Some uncommon types of VARIANT are not supported, in which case E_INVALIDARG
is returned.

\note The XSI::CComAPIHandler object permits access to the COM version of the object model from C++, yet
	shields the caller from VARIANTs and other Win32-specific structures.  Similarly, script code can call
	custom commands written in C++ and the arguments are automatically transformed from VARIANT to CValue
	without the need to call this routine explicitly.

\param in_Variant Variant to convert.  It is treated as const.
\retval out_val CValue that is equivalent to the Variant.
\returns HRESULT: A LONG representing success (S_OK) or error (E_INVALIDARG, S_FALSE).
\sa XSI::CValue, XSI::CStatus, XSI::CComAPIHandler
*/

SICPPSDKDECL HRESULT XSIVariantToCValue( /*const*/ VARIANT* in_Variant, XSI::CValue& out_val );

/*! Helper to convert from CValue to the Win32 VARIANT structure.  Normally this routine is not needed because
XSI::CComAPIHandler or automatic conversions shield the C++ API user from needing to deal with VARIANTs.
\param in_value CValue to convert
\retval out_var Address of a VARIANT to fill in based on the CValue's contents. The caller is reponsible for
	freeing the VARIANT with ::VariantClear().
\returns HRESULT: A LONG representing success (S_OK) or error (E_INVALIDARG, S_FALSE).
*/
SICPPSDKDECL HRESULT XSIVariantFromCValue( const XSI::CValue& in_value, VARIANT* out_var );

#endif // __SICPPSDK_WIN32_H__
