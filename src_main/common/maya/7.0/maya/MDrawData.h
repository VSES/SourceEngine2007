
#ifndef _MDrawData
#define _MDrawData
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
// CLASS:    MDrawData
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MDrawData)
//
//    The MDrawData class holds geometry specific information for user
//    defined shapes which maya does not intrinsicly know about.
//
//    This class is used in the draw methods of MPxSurfaceShapeUI
//    For each draw request you must create and add a draw data object
//    which will contain geometry specific information that you will need
//    in the subsequent call to MPxSurfaceShapeUI::draw.
//
//    MDrawData contains one void* member which is a pointer to an
//    object that you define. This object is the geometry needed to draw
//    your shape.
//
//    To create draw data use the function MPxSurfaceShapeUI::getDrawData.
//    This function takes two arguments, the first is a pointer to your
//    geometry object, the second is the draw data being created.
//    To add the data to a request use MDrawRequest::setDrawData.
//
//    Draw data is also used to carry texture information to your draw method.
//    For materials with texture you must call MMaterial::evaluateTexture
//    from your MPxSurfaceShapeUI::getDrawRequests method.
//    Then in your draw method use MMaterial::applyTexture to set up the
//    viewport to draw using the textured material.
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

// *****************************************************************************

// DECLARATIONS

class MDagPath;
class MVector;

// *****************************************************************************

// CLASS DECLARATION (MDrawData)

/// Draw data used in the draw methods of MPxSurfaceShapeUI (OpenMayaUI)
/**
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAUI_EXPORT MDrawData
{
public:
	///
	MDrawData();
	///
	MDrawData( const MDrawData& in );
	///
	~MDrawData();

public:
	///
	void *		geometry();

protected:
// No protected members

private:
	const char*	 className() const;
    friend class MMaterial;
    friend class MPxSurfaceShapeUI;
	friend class MDrawRequest;

    MDrawData( void* in );
	void*	 fDrawData;
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MDrawData */
