
#ifndef _MFnLightDataAttribute
#define _MFnLightDataAttribute
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
// CLASS:    MFnLightDataAttribute
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnLightDataAttribute)
//
// Function object for LightData attributes.  A LightData attribute describes
// a single light source, giving its direction and intensity, as well as
// specifying whether or not it contributes to the ambient, diffuse, and
// specular components of the shading model.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MFnAttribute.h>

// *****************************************************************************

// DECLARATIONS

// *****************************************************************************

class MString;

// CLASS DECLARATION (MFnLightDataAttribute)

/// Light data attribute function set
/**
  Function set for light data attributes.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MFnLightDataAttribute : public MFnAttribute 
{

	declareMFn(MFnLightDataAttribute, MFnAttribute);

public:
	///
	MObject    create( const MString& fullName,
					   const MString& briefName,
					   const MObject & direction,
					   const MObject & intensity,
					   const MObject & ambient,
					   const MObject & diffuse,
					   const MObject & specular,
					   const MObject & shadowFraciton,
					   const MObject & preShadowIntensity,
					   const MObject & blindData,
					   MStatus* ReturnStatus = NULL );

	///
	MStatus     getDefault( float & defDirectionX, 
							float & defDirectionY, 
							float & defDirectionZ,
							float & defIntensityR,
							float & defIntensityG, 
							float & defIntensityB,
							bool  & defAmbient, 
							bool  & defDiffuse, 
							bool  & defSpecular,
							float & defShadowFraction,
							float & defPreShadowIntensity,
							MUintPtrSz & defBlindData);

	///
	MStatus     setDefault( float defDirectionX, 
							float defDirectionY, 
							float defDirectionZ,
							float defIntensityR,
							float defIntensityG, 
							float defIntensityB,
							bool  defAmbient, 
							bool  defDiffuse, 
							bool  defSpecular,
							float defShadowFraction,
							float defPreShadowIntensity,
							MUintPtrSz defBlindData);

	///
	MObject     child( unsigned index, MStatus* returnStatus );

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
#endif /* _MFnLightDataAttribute */



