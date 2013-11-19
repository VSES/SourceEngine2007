
#ifndef _MFnLayeredShader
#define _MFnLayeredShader
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
// CLASS:    MFnLayeredShader
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnLayeredShader)
//
// MFnLayeredShader facilitates creation and manipulation of dependency graph
// nodes representing layered shaders.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MObject.h>
#include <maya/MStatus.h>
#include <maya/MFnDependencyNode.h>
// *****************************************************************************

// DECLARATIONS

class MFnDependencyNode;
class MFltVector;
class MColor;

// *****************************************************************************

// CLASS DECLARATION (MFnLayeredShader)

/// Manage Layered shaders
/**
  Facilitate the creation and manipulation of Layered shaders.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MFnLayeredShader : public MFnDependencyNode
{

	declareMFn( MFnLayeredShader, MFnDependencyNode );

public:
	///
	MObject     create( bool UIvisible = true, MStatus * ReturnStatus = NULL );
	///
	short		compositingFlag( MStatus * ReturnStatus = NULL ) const;
	///
	MStatus     setCompositingFlag( const short & cflag );
	///
	MColor      color( unsigned lIndex = 0, MStatus * ReturnStatus = NULL ) const;
	///
	MStatus     setColor( const MColor & col, unsigned lIndex = 0 );
	///
	MColor      transparency( unsigned lIndex = 0, MStatus * ReturnStatus = NULL ) const;
	///
	MStatus     setTransparency( const MColor & transp, unsigned lIndex = 0 );
	///
	MColor      glowColor( unsigned lIndex = 0, MStatus * ReturnStatus = NULL ) const;
	///
	MStatus     setGlowColor( const MColor & glow_color, unsigned lIndex = 0 );
	///
	MColor      hardwareColor( MStatus * ReturnStatus = NULL ) const;
	///
	MStatus     setHardwareColor( const MColor & hardware_color );
	///
	MColor       hardwareShader( MStatus * ReturnStatus = NULL ) const;
	///
	MStatus     setHardwareShader( const MColor& hardware_shader );

protected:
// No protected members

private:
// No private members

};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MFnLayeredShader */
