
#ifndef _MFnAnisotropyShader
#define _MFnAnisotropyShader
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
// CLASS:    MFnAnisotropyShader
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnAnisotropyShader)
//
// MFnAnisotropyShader facilitates creation and manipulation of dependency graph
// nodes representing anisotropy shaders.
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

// CLASS DECLARATION (MFnAnisotropyShader)

/// Manage anisotropic shaders
/**
  Facilitate the creation and manipulation of anisotropy shaders.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MFnAnisotropyShader : public MFnDependencyNode
{

	declareMFn( MFnAnisotropyShader, MFnDependencyNode );

public:
	///
	MObject     create( bool UIvisible = true, MStatus * ReturnStatus = NULL );
	///
	MColor      tangentUCamera( MStatus * ReturnStatus = NULL ) const;
	///
	MStatus     setTangentUCamera( const MColor & tangu_camera );
	///
	MColor      tangentVCamera( MStatus * ReturnStatus = NULL ) const;
	///
	MStatus     setTangentVCamera( const MColor & tangv_camera );
	///
	float      correlationX( MStatus * ReturnStatus = NULL ) const;
	///
	MStatus     setCorrelationX( const float &correl_x );
	///
	float      correlationY( MStatus * ReturnStatus = NULL ) const;
	///
	MStatus     setCorrelationY( const float &correl_y );
	///
	float       roughness( MStatus * ReturnStatus = NULL ) const;
	///
	MStatus     setRoughness( const float &roughness );
	///
	float       rotateAngle( MStatus * ReturnStatus = NULL ) const;
	///
	MStatus     setRotateAngle( const float& rotate_angle );
	///
	float       refractiveIndex( MStatus * ReturnStatus = NULL ) const;
	///
	MStatus     setRefractiveIndex( const float& refractive_index );
	///
	bool       anisotropicReflectivity( MStatus * ReturnStatus = NULL ) const;
	///
	MStatus     setAnisotropicReflectivity( const bool& anisotropic_reflectivity );

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
#endif /* _MFnAnisotropyShader */
