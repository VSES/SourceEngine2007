
#ifndef _MDrawRequestQueue
#define _MDrawRequestQueue
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
// CLASS:    MDrawRequestQueue
//
//    This class defines a simple interface for a collection of
//    MDrawRequest objects.
//
//    An MDrawRequestQueue object is passed to the getDrawRequests method
//    of a user defined shape's UI class (MPxSurfaceShapeUI).
//    This queue keeps track of all the things that need to get draw
//    when a refresh of the view occurs.
//
//    Maya will call the getDrawRequest methods of all the visible DAG objects
//    before a refresh happens. Then as the refresh happens, each draw request
//    will be processed and the corresponding draw method for each DAG
//    object will get called. For user defined shapes MPxSurfaceShapeUI::draw
//    will get called.
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MDrawRequestQueue)
//
// *****************************************************************************
//
#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTypes.h>
#include <maya/MObject.h>
#include <maya/MDrawRequest.h>

// *****************************************************************************

// DECLARATIONS

// *****************************************************************************

// CLASS DECLARATION (MDrawRequestQueue)
/// Drawing queue used in MPxSurfaceShapeUI::getDrawRequests (OpenMayaUI)
/**
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAUI_EXPORT MDrawRequestQueue
{
public:
	///
	MDrawRequestQueue();
	///
	~MDrawRequestQueue();

public:
	///
	bool			isEmpty() const;
	///
	void			add( MDrawRequest & );
	///
	MDrawRequest	remove();


protected:
// No protected members

private:
	const char*	 className() const;
    MDrawRequestQueue( void* in );
	void*	 fDrawRequestQueue;
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MDrawRequestQueue */
