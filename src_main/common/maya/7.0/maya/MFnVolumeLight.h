
#ifndef _MFnVolumeLight
#define _MFnVolumeLight
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
// CLASS:    MFnVolumeLight
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnVolumeLight)
//
// MFnVolumeLight facilitates creation and manipulation of dependency graph nodes
// representing volume lights.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MObject.h>
#include <maya/MStatus.h>
#include <maya/MFnPointLight.h>


// *****************************************************************************
 
// DECLARATIONS
 
class MRampAttribute;

// *****************************************************************************

// CLASS DECLARATION (MFnVolumeLight)

/// Manage volume Light dependency Nodes
/**
  Facilitate the creation and manipulation of volume light nodes
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MFnVolumeLight : public MFnPointLight
{

	declareDagMFn(MFnVolumeLight, MFnPointLight);
public:
	///
	enum MLightShape 
	{
	///
		kBoxVolume,
	///
		kSphereVolume,
	///
		kCylinderVolume,
	///
		kConeVolume
	};
	///
	enum MLightDirection
	{
	///
		kOutward,
	///
		kInward,
	///
		kDownAxis
	};
			
	///
	MObject     create( bool UIvisible = true, MStatus * ReturnStatus = NULL );
	///
	MObject     create( const MObject& parent, bool UIvisible = true,
						MStatus * ReturnStatus = NULL );
	///
	MObject     create( const MObject& parent, bool UIvisible = true,
				  bool wantTransform = false, MStatus * ReturnStatus = NULL );
	///
	MFnVolumeLight::MLightShape  lightShape ( MStatus * ReturnStatus ) const;
	///
	MStatus     setLightShape ( const MFnVolumeLight::MLightShape& light_shape);
	///
	MFnVolumeLight::MLightDirection volumeLightDirection (MStatus * ReturnStatus) const;
	///
	MStatus		setVolumeLightDirection (const MFnVolumeLight::MLightDirection& volume_light_direction);
	///
	float		arc ( MStatus * ReturnStatus ) const;
	///
	MStatus		setArc (const float& arc);
	///
	float		coneEndRadius ( MStatus * ReturnStatus ) const;
	///
	MStatus		setConeEndRadius (const float& cone_end_radius);
	///
	bool		emitAmbient ( MStatus * ReturnStatus ) const;
	///
	MStatus		setEmitAmbient (const bool& emit_ambient);
	///
	MRampAttribute colorRamp (MStatus * ReturnStatus) const ;
	///
	MRampAttribute penumbraRamp (MStatus * ReturnStatus) const;

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
#endif /* _MFnVolumeLight */



