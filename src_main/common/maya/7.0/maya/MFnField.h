
#ifndef _MFnField
#define _MFnField
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
// CLASS:    MFnField
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnField)
//
// Function set for Dynamic Fields
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MFnDagNode.h>

// *****************************************************************************

// DECLARATIONS

class MPointArray;
class MVectorArray;
class MDoubleArray;

// *****************************************************************************

// CLASS DECLARATION (MFnField)

/// Function set for Dynamic Fields (OpenMayaFX)
/**

*/

#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAFX_EXPORT MFnField : public MFnDagNode
{

    declareDagMFn(MFnField, MFnDagNode);

public:
    ///
    MStatus getForceAtPoint(const MPointArray&   point,
                            const MVectorArray&  velocity,
                            const MDoubleArray&  mass,
                            MVectorArray&        force,
							double deltaTime = 1.0 / 24.0 );
    ///
    MStatus getForceAtPoint(const MVectorArray&  point,
                            const MVectorArray&  velocity,
                            const MDoubleArray&  mass,
                            MVectorArray&        force,
							double deltaTime = 1.0 / 24.0 );
    ///
    double       magnitude          ( MStatus *ReturnStatus = NULL ) const;
    ///
    MStatus      setMagnitude       ( double mag );
    ///
    double       attenuation        ( MStatus *ReturnStatus = NULL ) const;
    ///
    MStatus      setAttenuation     ( double atten );
    ///
    double       maxDistance        ( MStatus *ReturnStatus = NULL ) const;
    ///
    MStatus      setMaxDistance     ( double maxDist );
    ///
    bool         perVertex          ( MStatus *ReturnStatus = NULL ) const;
    ///
    MStatus      setPerVertex       ( bool enable );
    ///
    bool         useMaxDistance     ( MStatus *ReturnStatus = NULL ) const;
    ///
    MStatus      setUseMaxDistance  ( bool enable );
    ///
    double       falloffCurve       ( const double param, MStatus *ReturnStatus = NULL );
    ///
    bool         isFalloffCurveConstantOne       (MStatus *ReturnStatus = NULL);



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
#endif /* _MFnField */
