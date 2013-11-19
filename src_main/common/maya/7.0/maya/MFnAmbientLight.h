
#ifndef _MFnAmbientLight
#define _MFnAmbientLight
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
// CLASS:    MFnAmbientLight
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnAmbientLight)
//
// MFnAmbientLight facilitates creation and manipulation of ambient light 
// dependency graph nodes.  
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MFnDagNode.h>
#include <maya/MObject.h>
#include <maya/MStatus.h>
#include <maya/MFnLight.h>

// *****************************************************************************

// DECLARATIONS

class MPoint;
class MColor;
class MDagPath;

// *****************************************************************************

// CLASS DECLARATION (MFnAmbientLight)

/// Manage Ambient Light dependency Nodes
/**
  Facilitate the creation and manipulation of ambient light nodes.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MFnAmbientLight : public MFnLight 
{

	declareDagMFn(MFnAmbientLight,MFnLight);
public:
	///
	MObject     create( bool UIvisible = true, MStatus * ReturnStatus = NULL );
	///
	MObject     create( const MObject& parent, bool UIvisible = true,
											   MStatus * ReturnStatus = NULL );
	///
	MObject     create( const MObject& parent, bool UIvisible = true,
				  bool wantTransform = false, MStatus * ReturnStatus = NULL );
	///
	float       ambientShade( MStatus * ReturnStatus = NULL ) const;
	///
	MStatus     setAmbientShade( const float& ambient_shade );
	///
	bool        castSoftShadows( MStatus * ReturnStatus = NULL ) const;
	///
	MStatus     setCastSoftShadows( const bool& cast_soft_shadows );
	///
	float       shadowRadius( MStatus * ReturnStatus = NULL ) const;
	///
	MStatus     setShadowRadius( const float& shadow_radius );

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
#endif /* _MFnAmbientLight */
