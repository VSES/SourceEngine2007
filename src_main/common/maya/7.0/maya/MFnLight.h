
#ifndef _MFnLight
#define _MFnLight
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
// CLASS:    MFnLight
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnLight)
// 
// MFnLight allows the creation and manipulation of dependency graph nodes 
// representing lights.  This is the top level of a hierarchy of light 
// node function sets.  It permits manipulation of the attributes common to
// all types of lights.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MFnDagNode.h>
#include <maya/MObject.h>
#include <maya/MStatus.h>

// *****************************************************************************

// DECLARATIONS

class MPoint;
class MFloatVector;
class MColor;
class MDagPath;

// *****************************************************************************

// CLASS DECLARATION (MFnLight)

/// Manage dependency graph nodes representing lights
/**
  MFnLight allows manipulation of dependency graph nodes representing lights.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MFnLight : public MFnDagNode 
{

	declareDagMFn(MFnLight,MFnDagNode);
public:
	///
	MColor       color( MStatus * ReturnStatus = NULL ) const;
	///
	MStatus      setColor( const MColor &col );
	///
	float        intensity( MStatus * ReturnStatus = NULL ) const;
	///
	MStatus      setIntensity( const float& intens );
	///
	bool         useRayTraceShadows( MStatus * ReturnStatus = NULL ) const;
	///
	MStatus      setUseRayTraceShadows( const bool& useRayTraceShadows );
	///
	MColor       shadowColor( MStatus * ReturnStatus = NULL ) const;
	///
    MStatus      setShadowColor( const MColor& shadow_color );
	///
	double       centerOfIllumination( MStatus * ReturnStatus = NULL ) const;
	///
	MStatus      setCenterOfIllumination( const double& dist );
	///
	short        numShadowSamples( MStatus * ReturnStatus = NULL ) const;
	///
	MStatus      setNumShadowSamples( const short& num_shadow_samples );
	///
	short        rayDepthLimit( MStatus * ReturnStatus = NULL ) const;
	///
	MStatus      setRayDepthLimit( const short& rayDepthLimit );
	///
    MColor       opticalFXvisibility( MStatus * ReturnStatus = NULL ) const;
	///
	MStatus      setOpticalFXvisibility( const MColor& visibility );
	///
	MColor       lightIntensity( MStatus * ReturnStatus = NULL ) const;
	///
	MFloatVector lightDirection( int instance, MSpace::Space space = MSpace::kWorld, MStatus * ReturnStatus = NULL ) const;
	///
	MFloatVector lightDirection( MStatus * ReturnStatus = NULL ) const;
	///
	bool         lightAmbient( MStatus * ReturnStatus = NULL ) const;
	///
	bool         lightDiffuse( MStatus * ReturnStatus = NULL ) const;
	///
	bool         lightSpecular( MStatus * ReturnStatus = NULL ) const;



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
#endif /* _MFnLight */
