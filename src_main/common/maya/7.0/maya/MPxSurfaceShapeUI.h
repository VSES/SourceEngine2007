
#ifndef _MPxSurfaceShapeUI
#define _MPxSurfaceShapeUI
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
// CLASS:    MPxSurfaceShapeUI
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MPxNode)
//
//
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES



#include <maya/MStatus.h>
#include <maya/MTypes.h>
#include <maya/MObject.h>
#include <maya/MPxNode.h>
#include <maya/M3dView.h>
#include <maya/MBoundingBox.h>
#include <maya/M3dView.h>
#include <maya/MSelectInfo.h>
#include <maya/MDrawRequest.h>
#include <maya/MDrawRequestQueue.h>

// *****************************************************************************

// DECLARATIONS

class MSelectionList;
class MPointArray;
class MPxSurfaceShape;
class MDrawData;
class MMaterial;

// *****************************************************************************

// CLASS DECLARATION (MPxNode)

/// drawing and selection for user defined shapes (OpenMayaUI)
/**
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAUI_EXPORT MPxSurfaceShapeUI
{
public:
    ///
	MPxSurfaceShapeUI();
    ///
	virtual ~MPxSurfaceShapeUI();

	///
	void					getDrawData( void * geom, MDrawData & );

	/////////////////////////////////////////////////////////////////////////
	// Methods to overload

	///
	virtual void			getDrawRequests( const MDrawInfo &,
											 bool objectAndActiveOnly,
											 MDrawRequestQueue & requests );
	///
	virtual void		    draw( const MDrawRequest &, M3dView & view ) const;
	///
	virtual bool		    select( MSelectInfo &selectInfo,
							    	MSelectionList &selectionList,
							    	MPointArray &worldSpaceSelectPts ) const;

	/////////////////////////////////////////////////////////////////////////

	///
	MPxSurfaceShape*		surfaceShape() const;
	///
	MMaterial 				material( MDagPath & path ) const;

protected:

private:
	static const char*	    className();
	friend class MDrawRequest;
	friend class MDrawInfo;
	void * instance;
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MPxSurfaceShapeUI */
