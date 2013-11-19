
#ifndef _MFnLambertShader
#define _MFnLambertShader
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
// CLASS:    MFnLambertShader
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnLambertShader)
//
// MFnLambertShader facilitates creation and manipulation of dependency graph
// nodes representing lambertian shaders.
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

// CLASS DECLARATION (MFnLambertShader)

/// Manage Lambert shaders
/**
  Facilitate the creation and manipulation of Lambert shaders.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MFnLambertShader : public MFnDependencyNode
{

	declareMFn( MFnLambertShader, MFnDependencyNode );

public:
	///
	MObject     create( bool UIvisible = true, MStatus * ReturnStatus = NULL );
	///
	short       refractedRayDepthLimit( MStatus * ReturnStatus = NULL ) const;
	///
	MStatus     setRefractedRayDepthLimit( const short& new_limit );
	///
	float       refractiveIndex( MStatus * ReturnStatus = NULL ) const;
	///
	MStatus     setRefractiveIndex( const float& refractive_index );
	///
	bool        rtRefractedColor( MStatus * ReturnStatus = NULL ) const;
	///
	MStatus     setRtRefractedColor( const bool& rt_refracted_color );
	///
	float       diffuseCoeff( MStatus * ReturnStatus = NULL ) const;
	///
	MStatus     setDiffuseCoeff( const float& diffuse_coeff );
	///
	MColor      color( MStatus * ReturnStatus = NULL ) const;
	///
	MStatus     setColor( const MColor & col );
	///
	MColor      transparency( MStatus * ReturnStatus = NULL ) const;
	///
	MStatus     setTransparency( const MColor & transp );
	///
	MColor      ambientColor( MStatus * ReturnStatus = NULL ) const;
	///
	MStatus     setAmbientColor( const MColor & ambient_color );
	///
	MColor      incandescence( MStatus * ReturnStatus = NULL ) const;
	///
	MStatus     setIncandescence( const MColor & incand );
	///
	float       translucenceCoeff( MStatus * ReturnStatus = NULL ) const;
	///
	MStatus     setTranslucenceCoeff( const float& translucence_coeff );
	///
	float       glowIntensity( MStatus * ReturnStatus = NULL ) const;
	///
	MStatus     setGlowIntensity( const float& glow_intensity );
	///
	bool        hideSource( MStatus * ReturnStatus = NULL ) const;
	///
	MStatus     setHideSource( const bool& hide_source );

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
#endif /* _MFnLambertShader */
