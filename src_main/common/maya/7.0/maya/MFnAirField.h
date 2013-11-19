
#ifndef _MFnAirField
#define _MFnAirField
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
// CLASS:    MFnAirField
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnAirField)
//
// Function set for Air Fields
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

// CLASS DECLARATION (MFnAirField)

/// Function set for Air Fields (OpenMayaFX)
/**

*/

#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAFX_EXPORT MFnAirField : public MFnField
{

    declareDagMFn(MFnAirField, MFnField);

public:
    ///
    MVector      direction          ( MStatus *ReturnStatus = NULL ) const;
    ///
    MStatus      setDirection       ( const MVector & airDirection );
    ///
    double       speed              ( MStatus *ReturnStatus = NULL ) const;
    ///
    MStatus      setSpeed           ( double value );
    ///
    double       inheritVelocity    ( MStatus *ReturnStatus = NULL ) const;
    ///
    MStatus      setInheritVelocity ( double velocity );
    ///
    bool         inheritRotation    ( MStatus *ReturnStatus = NULL ) const;
    ///
    MStatus      setInheritRotation ( bool enable );
    ///
    bool         componentOnly      ( MStatus *ReturnStatus = NULL ) const;
    ///
    MStatus      setComponentOnly   ( bool enable );
    ///
    double       spread             ( MStatus *ReturnStatus = NULL ) const;
    ///
    MStatus      setSpread          ( double value );
    ///
    bool         enableSpread       ( MStatus *ReturnStatus = NULL ) const;
    ///
    MStatus      setEnableSpread    ( bool enable );


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
#endif /* _MFnAirField */
