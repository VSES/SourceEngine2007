
#ifndef _MPxGlBuffer
#define _MPxGlBuffer
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
// CLASS:    MPxGlBuffer
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MPxGlBuffer)
//
//  MPxGlBuffer allows the user to create OpenGL buffers that Maya
//	can draw into.  The base class as is defined will create a hardware
//	accellerated off-screen buffer for use on SGI hardware.
//
//  To create a custom buffer, derive from this class and override at
//	least the open, close, drawable, and context classes.  The customized
//	buffer may also require a constructor and destructor to initialize and
//	free its storage correctly.
//
// *****************************************************************************

#if defined __cplusplus

// ****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTypes.h>

#if defined(__unix) || defined(LINUX)
#include <GL/glx.h>
#endif

// ****************************************************************************

// DECLARATIONS

class MString;
class M3dView;

// ****************************************************************************

// CLASS DECLARATION (MPxGlBuffer)

/// Base class for user defined GL buffers (OpenMayaUI)

/**
  Create user defined (off-screen) GL buffers.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAUI_EXPORT MPxGlBuffer
{
public:
	///
	MPxGlBuffer();
	///
	MPxGlBuffer( M3dView &view );
	///
	virtual ~MPxGlBuffer();

#if defined(__unix) || defined(LINUX)
	///
	virtual	MStatus			open( short width, short height,
								  GLXContext shareCtx = NULL );
	///
	virtual GLXDrawable	    drawable( MStatus * ReturnStatus = NULL );
	///
	virtual	GLXContext      context( MStatus * ReturnStatus = NULL );
	///
	virtual	Display *       display( MStatus * ReturnStatus = NULL );
	///
	virtual XVisualInfo *   visual( MStatus * ReturnStatus = NULL );
	///
	virtual int *   		attributeList( MStatus * ReturnStatus = NULL );

	///
	virtual MStatus         setUseExternalDrawable( bool state );
	///
	virtual MStatus         setDoubleBuffer( bool state );
	///
	virtual MStatus         setDisplay( Display * disp );
	///
	virtual MStatus         setDrawable( GLXDrawable drawable );
	///
	virtual MStatus         setVisual( XVisualInfo * vis );
#endif // __unix

	///
	virtual	MStatus			close();

protected:
	///
	bool					hasColorIndex;
	///
	bool					hasAlphaBuffer;
	///
	bool					hasDepthBuffer;
	///
	bool					hasAccumulationBuffer;
	///
	bool					hasDoubleBuffer;

private:
	void   setData( void* );
	virtual const char*	className() const;
	void * 	data;

};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MPxGlBuffer */
