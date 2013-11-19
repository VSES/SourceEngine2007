#ifndef _MPxModelEditorCommand
#define _MPxModelEditorCommand
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
// CLASS:    MPxModelEditorCommand
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MPxModelEditorCommand)
//
//  This is the command class used for creating proxy editor.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES

#include <maya/MTypes.h>
#include <maya/MStatus.h>

// *****************************************************************************

// DECLARATIONS

class MArgParser;
class MPx3dModelView;
class MStringArray;

// *****************************************************************************

// CLASS DECLARATION (MPxModelEditorCommand)

/// Base class for editor creation commands (OpenMayaUI)
/**
  The base class for editor creation commands.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAUI_EXPORT MPxModelEditorCommand
{
public:
	///
							MPxModelEditorCommand	();
	///
	virtual					~MPxModelEditorCommand	();
	/// 
	virtual MStatus			doEditFlags			();
	///
	virtual MStatus			doQueryFlags		();
	///
	virtual MStatus			appendSyntax		();
	///
	virtual bool			skipFlagForCreate	(MString &longFlag);
	///
	MStatus					setResult			(bool result);
	///
	MStatus					setResult			(int result);
	///
	MStatus					setResult			(double result);
	///
	MStatus					setResult			(const MString &result);
	///
	MStatus					setResult			(const MStringArray &result);

	///
			MPx3dModelView	*modelView() const;

	///
	virtual	MString			editorMenuScriptName() const;
protected:
	///
	MSyntax	syntax(MStatus *ReturnStatus = NULL) const;
	///
	MArgParser parser(MStatus *ReturnStatus = NULL) const;

private:
	const char *className() const;
	void setData(void *ptr);

	void *instance;
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MPxModelEditorCommand */
