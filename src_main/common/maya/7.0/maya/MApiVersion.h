
#ifndef _MApiVersion
#define _MApiVersion
//
//-
// ==========================================================================
// Copyright (C) 1995 - 2005 Alias Systems Corp. and/or its licensors.  All 
// rights reserved.
// 
// The coded instructions, statements, computer programs, and/or related 
// material (collectively the "Data") in these files contain unpublished 
// information proprietary to Alias Systems Corp. ("Alias") and/or its 
// licensors, which is protected by Canadian and US federal copyright law and 
// by international treaties.
// 
// The Data may not be disclosed or distributed to third parties or be copied 
// or duplicated, in whole or in part, without the prior written consent of 
// Alias.
// 
// THE DATA IS PROVIDED "AS IS". ALIAS HEREBY DISCLAIMS ALL WARRANTIES RELATING 
// TO THE DATA, INCLUDING, WITHOUT LIMITATION, ANY AND ALL EXPRESS OR IMPLIED 
// WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE. IN NO EVENT SHALL ALIAS BE LIABLE FOR ANY DAMAGES 
// WHATSOEVER, WHETHER DIRECT, INDIRECT, SPECIAL, OR PUNITIVE, WHETHER IN AN 
// ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, OR IN EQUITY, 
// ARISING OUT OF ACCESS TO, USE OF, OR RELIANCE UPON THE DATA.
// ==========================================================================
//+

#ifndef MNoVersionString

// Create 2 cpp macros which will allow use to get the value of
// the #define MAYA_API_VERSION into a string and assigned to
// the global variable.
#define _versionString1( _version ) _versionString2(_version)
#define _versionString2( _version ) PLUGIN_EXPORT char MApiVersion[] =  #_version

#ifdef __cplusplus
extern "C" {
#endif

_versionString1(MAYA_API_VERSION);

#ifdef __cplusplus
}
#endif

// Delete the temporary macros
#undef _versionString1
#undef _versionString2

#endif // MNoVersionString

#endif /* _MApiVersion */
