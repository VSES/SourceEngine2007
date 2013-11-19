#ifndef _MPxComponentShape
#define _MPxComponentShape

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
// CLASS:    MPxComponentShape
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MPxComponentShape)
// 
//  MPxComponentShape is the parent class of user defined shapes that contain
//  an array of points/control vertices/components.
//
//  MPxComponentShape provides various basic support routines needed for any
//  custom shape that supports components. A copy of the data for those components is
//  stored locally to allow fast access, particularly for draw/pick routines.
//
//  User defined shapes are dependency nodes (and DAG nodes) which contain
//  overridable drawing, and selection methods.
//
//  The UI dependent aspects of the shape should be implemented in a class
//  derived from MPxSurfaceShapeUI. This includes the drawing and interactive
//  selection of the shape and any components that the shape implements.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MPxSurfaceShape.h>
#include <maya/MVectorArray.h>

// *****************************************************************************

// DECLARATIONS
 
class MSelectionList;
class MPointArray;
class MObjectArray;
class MSelectionMask;

// *****************************************************************************

// CLASS DECLARATION (MPxComponentShape)

/// Component helper class for surface shapes
/**
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MPxComponentShape : public MPxSurfaceShape  
{
public:

	///
	virtual void transformUsing( const MMatrix & matrix, const MObjectArray & componentList );

	///
	virtual void transformUsing( const MMatrix& mat, const MObjectArray& componentList, MVertexCachingMode cachingMode, MPointArray* pointCache);

	///
	virtual void componentToPlugs( MObject & component, MSelectionList & list ) const;

	///
	virtual bool match( const MSelectionMask & mask, const MObjectArray& componentList ) const;


	///
	virtual MObject createFullVertexGroup() const;

	///
	virtual MObject localShapeInAttr() const;

	///
	MVectorArray* getControlPoints();

	///
	MStatus setControlPoints( MVectorArray* cps );

protected:
	MVectorArray    myfCps;

private:
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MPxComponentShape */
