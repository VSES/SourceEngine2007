
#ifndef _MToolsInfo
#define _MToolsInfo
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
//
// CLASS:    MToolsInfo
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MToolsInfo)
//
// MToolsInfo is a caretaker class used to keep track of
// the dirty state of the current tool property sheet.
// The tool writer should make sure to call the
// setDirtyFlag method when any of the values are
// modified.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MTypes.h>

// *****************************************************************************

// DECLARATIONS

class MStatus;
class MPxContext;

// CLASS DECLARATION (MToolsInfo)

/// Tool information (OpenMayaUI)
/**
MToolsInfo provides methods for keeping track of the
state of the current tool property sheet.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32


class OPENMAYAUI_EXPORT MToolsInfo {
public:
	///
	static void		setDirtyFlag(const MPxContext &context);
	///
	static void		resetDirtyFlag();
	///
	static bool		isDirty();

private:
	static const char *className();
};


#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MToolsInfo */
