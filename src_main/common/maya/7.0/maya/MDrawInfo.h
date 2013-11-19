
#ifndef _MDrawInfo
#define _MDrawInfo
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
// CLASS:    MDrawInfo
//
//    This class is used by the getDrawRequests method of MPxSurfaceShapeUI
//    to specify the current object drawing state for a user defined shape.
//
//    The getPrototype method is used to construct a draw request object
//    based on the current draw state for the object.
//
//    See MDrawRequest for more information.
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MDrawInfo)
//
// *****************************************************************************
//
#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTypes.h>
#include <maya/MObject.h>
#include <maya/M3dView.h>
#include <maya/MDrawRequest.h>

// *****************************************************************************

// DECLARATIONS

class MSelectionMask;
class MSelectionTypeSet;
class MPoint;
class MPointArray;
class MVector;
class MSelectionList;
class MMatrix;

// *****************************************************************************

// CLASS DECLARATION (MDrawInfo)

/// Drawing state used in the draw methods of MPxSurfaceShapeUI (OpenMayaUI)
/**
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAUI_EXPORT MDrawInfo
{
public:
	///
	MDrawInfo();
	///
	MDrawInfo( const MDrawInfo& in );
	///
	virtual ~MDrawInfo();


	// This method returns a partially filled in MDrawRequest
	///
	MDrawRequest			getPrototype(
								const MPxSurfaceShapeUI& drawHandler ) const;

	// Only really need camera path here
	///
	M3dView  				view() const;

	// path to object to draw
	///
	const MDagPath 			multiPath () const;

	// projection (camera) matrix
	///
	const MMatrix 			projectionMatrix() const;

	// world space inclusive matrix
	///
	const MMatrix 			inclusiveMatrix() const;

	// display appearance (this may change during traversal)
	///
	M3dView::DisplayStyle	displayStyle() const;

	// display status of object to draw
	///
	M3dView::DisplayStatus	displayStatus() const;

	// display objects
	///
	bool objectDisplayStatus( M3dView::DisplayObjects ) const;

	///
	bool					inSelect() const;
	///
	bool					completelyInside() const;

	// convenience to test if component can be drawn
	///
	bool					canDrawComponent( bool isDisplayOn,
									const MSelectionMask & compMask ) const;

public:

	// Public accessor methods

protected:
// No protected members
	void*	 fData;

private:
	const char*	 className() const;
    MDrawInfo( void* in );
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MDrawInfo */
