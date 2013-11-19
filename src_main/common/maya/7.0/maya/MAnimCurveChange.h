
#ifndef _MAnimCurveChange
#define _MAnimCurveChange
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
// CLASS:    MAnimCurveChange
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MAnimCurveChange)
//
//	Anim Curve Change caches are used as persistent storage for information
//	concerning changes to Anim Curve Nodes.
//
//	When invoked with a valid Anim Curve change parameter, the remove keyframe
//	and	add keyframe methods of the Anim Curve Function Set
//	(MFnAnimCurve::remove()	and MFnAnimCurve::addKeyframe(), respectively)
//	cache the change information in the Anim Curve Change cache so that the
//	caller can undo and redo the changes as required.
//
//	Caching of the undo and redo information in a Anim Curve Change cache is
//	required primarily to support interactive editing of Anim Curves in the UI.
//
//	Use Anim Curve Change caches in conjunction with the Anim Curve Function
//	Set (MFnAnimCurve) to perform undoable and redoable edits on Anim Curves.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES



#include <maya/MStatus.h>
#include <maya/MTypes.h>

// *****************************************************************************

// DECLARATIONS

// *****************************************************************************

// CLASS DECLARATION (MAnimCurveChange)

///  Anim Curve Change Cache (OpenMayaAnim)
/**

Create Anim Curve change caches.

Undo Anim Curve changes which have been cached.

Redo Anim Curve changes which were previously undone.

*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAANIM_EXPORT MAnimCurveChange
{

public:
	///
	MAnimCurveChange( MStatus * ReturnStatus = NULL );
	///
	~MAnimCurveChange();
	///
	MStatus undoIt();
	///
	MStatus redoIt();

protected:
// No protected members

private:
	static const char* className();
	void*		 data;
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MAnimCurveChange */
