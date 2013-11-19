
#ifndef _MDrawRequest
#define _MDrawRequest
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
// CLASS:    MDrawRequest
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MDrawRequest)
//
//		This class encapsulates all the information needed to fulfill
//		a request to draw an object or part of an object.
//      This class is used by the draw methods of MPxSurfaceShapeUI
//      derived objects.
//
//      The draw request should be created in the overridden
//      MPxSurfaceShapeUI::getDrawRequests method. Once created the
//      appropriate "set" methods of this class should be used to define
//      what is being requested. Then the request should be placed on the
//      draw reqeust queue using MDrawRequestQueue::add.
//
//      When your request gets processed by maya, your overriden
//      MPxSurfaceShape::draw method will get called with your request.
//      Use the query methods of this class to determine what to draw.
//
//      You create a draw request using the method MDrawInfo::getPrototype.
//      A draw request automatically picks up certain information (listed
//      below) upon it's creation. So you don't have to set any of this
//      information unless you want to change it.
//
//      Information automatically set by MDrawInfo::getPrototype :
//         path                - path to object to be drawn
//         view                - view to draw in
//         matrix              - world matrix for object
//         display appearance  - how object should be drawn
//         display status      - active, dormant etc.
//
//      The draw token is an integer value which you can use to specify
//      what is to be drawn. This is object specific and so you should
//      define an enum with the information you require to decide what
//      is being drawn in your MPxSurfaceShapeUI::draw method.
//
//      Here is an example of draw token values for a polygonal mesh
//      object as defined in an MPxSurfaceShapeUI derived class.
//
//          // Draw Tokens
//          //
//          enum {
//              kDrawVertices, // component token
//              kDrawWireframe,
//              kDrawWireframeOnShaded,
//              kDrawSmoothShaded,
//              kDrawFlatShaded,
//              kLastToken
//          };
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

class MSelectionMask;
class MPoint;
class MPointArray;
class MVector;
class MSelectionList;
class MMatrix;
class MPxSurfaceShapeUI;
class MMaterial;
class MDrawData;

// *****************************************************************************

// CLASS DECLARATION (MDrawRequest)

/// A draw reqeust used in the draw methods of MPxSurfaceShapeUI (OpenMayaUI)
/**
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAUI_EXPORT MDrawRequest
{
public:
	///
	MDrawRequest();
	///
	MDrawRequest( const MDrawRequest& in );
	///
	~MDrawRequest();

public:

	// the view to draw to
	///
	M3dView					view() const;
	///
	void					setView( M3dView & );
	///
	const MDagPath			multiPath() const;
	///
	void					setMultiPath( const MDagPath & );
	///
	MObject 				component() const;
	///
	void					setComponent( MObject & );
	///
	MDrawData 				drawData() const;
	///
	void					setDrawData( MDrawData & );
	///
	M3dView::DisplayStatus	displayStatus() const;
	///
	void					setDisplayStatus( M3dView::DisplayStatus );
	///
	bool					displayCulling() const;
	///
	void					setDisplayCulling( bool );
	///
	bool					displayCullOpposite() const;
	///
	void					setDisplayCullOpposite( bool );
	///
	M3dView::DisplayStyle	displayStyle() const;
	///
	void					setDisplayStyle( M3dView::DisplayStyle );
	///
	int						color( M3dView::ColorTable table ) const;
	///
	void					setColor( int, M3dView::ColorTable table );
	///
	MMaterial 				material() const;
	///
	void					setMaterial( MMaterial& );
	///
	bool					isTransparent() const;
	///
	void					setIsTransparent( bool );
	///
	bool					drawLast() const;
	///
	void					setDrawLast( bool );
	///
	int						token() const;
	///
	void					setToken( int );

	///
	MDrawRequest&	operator = ( const MDrawRequest& other );

protected:
// No protected members

private:
	const char*	 className() const;
    MDrawRequest( void* in, bool own );
	void*	fDrawRequest;
	bool    fOwn;
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MDrawRequest */
