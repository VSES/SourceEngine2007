
#ifndef _MFnVolumeAxisField
#define _MFnVolumeAxisField
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
// CLASS:    MFnVolumeAxisField
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnVolumeAxisField)
//
// Function set for VolumeAxis Fields
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MFnField.h>
#include <maya/MVector.h>

// *****************************************************************************

// DECLARATIONS

// *****************************************************************************

// CLASS DECLARATION (MFnVolumeAxisField)

/// Function set for VolumeAxis Fields (OpenMayaFX)
/**

*/

#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAFX_EXPORT MFnVolumeAxisField : public MFnField
{

    declareDagMFn(MFnVolumeAxisField, MFnField);

public:
    ///
    bool         invertAttenuation  ( MStatus *ReturnStatus = NULL ) const;
    ///
    MVector      direction          ( MStatus *ReturnStatus = NULL ) const;
    ///
    double       speedAlongAxis     ( MStatus *ReturnStatus = NULL ) const;
    ///
    double       speedAroundAxis    ( MStatus *ReturnStatus = NULL ) const;
    ///
    double       speedAwayFromAxis  ( MStatus *ReturnStatus = NULL ) const;
    ///
    double       speedAwayFromCenter( MStatus *ReturnStatus = NULL ) const;
    ///
    double       directionalSpeed   ( MStatus *ReturnStatus = NULL ) const;
    ///
    double       turbulence         ( MStatus *ReturnStatus = NULL ) const;
    ///
    double       turbulenceSpeed    ( MStatus *ReturnStatus = NULL ) const;
    ///
    MVector      turbulenceFrequency ( MStatus *ReturnStatus = NULL ) const;
    ///
    MVector      turbulenceOffset   ( MStatus *ReturnStatus = NULL ) const;
    ///
    double       detailTurbulence   ( MStatus *ReturnStatus = NULL ) const;
    ///
    MStatus      setInvertAttenuation  ( bool enable );
    ///
    MStatus      setDirection       ( const MVector & direction );
    ///
    MStatus      setSpeedAlongAxis  ( double value );
    ///
    MStatus      setSpeedAroundAxis  ( double value );
    ///
    MStatus      setSpeedAwayFromAxis  ( double value );
    ///
    MStatus      setSpeedAwayFromCenter  ( double value );
    ///
    MStatus      setDirectionalSpeed ( double value );
    ///
    MStatus      setTurbulence       ( double value );
    ///
    MStatus      setTurbulenceSpeed  ( double value );
    ///
    MStatus      setTurbulenceFrequency ( const MVector &value );
    ///
    MStatus      setTurbulenceOffset ( const MVector &value );
    ///
    MStatus      detailTurbulence    ( double value );

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
#endif /* _MFnVolumeAxisField */
