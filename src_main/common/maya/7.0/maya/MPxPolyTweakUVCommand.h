#ifndef _MPxPolyTweakUVCommand
#define _MPxPolyTweakUVCommand
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
// CLASS:    MPxPolyTweakUVCommand
//
// ****************************************************************************
//
// CLASS DESCRIPTION (MPxPolyTweakUVCommand)
//
//  This is the command base class used for moving polygon UV's.
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************

// INCLUDED HEADER FILES

#include <maya/MTypes.h>
#include <maya/MStatus.h>
#include <maya/MPxCommand.h>

// ****************************************************************************

// DECLARATIONS

class MPxContext;
class MArgParser;
class MArgDatabase;
class MIntArray;
class MFloatArray;
class MFnMesh;

// ****************************************************************************

// CLASS DECLARATION (MPxPolyTweakUVCommand)

/// Base class used for moving polygon UV's (OpenMaya)

#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MPxPolyTweakUVCommand : public MPxCommand
{
public:
	///
							MPxPolyTweakUVCommand ();
	///
	virtual					~MPxPolyTweakUVCommand ();
	///
	virtual MStatus			parseSyntax (MArgDatabase &argData);
	///
	virtual MStatus			getTweakedUVs (const MFnMesh & mesh,
										   MIntArray & uvList,
										   MFloatArray & uPos,
										   MFloatArray & vPos);
	///
	static MSyntax			newSyntax ();

private:

	virtual MStatus			doIt(const class MArgList &);
	virtual MStatus			undoIt();
	virtual MStatus			redoIt();
	virtual bool			isUndoable() const;

	void * fData;
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// ****************************************************************************
#endif /* __cplusplus */
#endif /* _MPxPolyTweakUVCommand */
