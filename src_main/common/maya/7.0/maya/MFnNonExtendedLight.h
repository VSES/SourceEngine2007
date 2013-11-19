
#ifndef _MFnNonExtendedLight
#define _MFnNonExtendedLight
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
// CLASS:    MFnNonExtendedLight
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnNonExtendedLight)
//
// MFnNonExtendedLight facilitates manipulation of dependency graph nodes
// representing non-extended lights.  This includes point lights, directional
// lights, and spotlights.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MFnDagNode.h>
#include <maya/MObject.h>
#include <maya/MStatus.h>
#include <maya/MFnNonAmbientLight.h>

// *****************************************************************************

// DECLARATIONS

// *****************************************************************************

// CLASS DECLARATION (MFnNonExtendedLight)

/// Manage Non-extended Light dependency Nodes
/**
  Facilitate the creation and manipulation of non-extended light nodes.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MFnNonExtendedLight : public MFnNonAmbientLight 
{

	declareDagMFn(MFnNonExtendedLight,MFnNonAmbientLight);
public:
	///
	float         shadowRadius( MStatus * ReturnStatus = NULL ) const;
	///
	MStatus       setShadowRadius( const float& shadow_radius );
	///
	bool          castSoftShadows( MStatus * ReturnStatus = NULL ) const;
	///
	MStatus       setCastSoftShadows( const bool& cast_soft_shadows );
	///
	bool          useDepthMapShadows( MStatus * ReturnStatus = NULL ) const;
	///
	MStatus       setUseDepthMapShadows( const bool& use_depth_map );
	///
	short         depthMapFilterSize( MStatus * ReturnStatus ) const;
	///
	MStatus       setDepthMapFilterSize( const short& depth_map_filter_size );
	///
	short         depthMapResolution( MStatus * ReturnStatus ) const;
	///
	MStatus       setDepthMapResolution( const short& depth_map_resolution );
	///
	float         depthMapBias( MStatus * ReturnStatus ) const;
	///
	MStatus       setDepthMapBias( const float& depth_map_bias );

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
#endif /* _MFnNonExtendedLight */



