
#ifndef _MFnHikEffector
#define _MFnHikEffector
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
// CLASS:    MFnHikEffector
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnHikEffector)
//
//
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MFnTransform.h>
#include <maya/MObject.h>

// *****************************************************************************

// DECLARATIONS

// *****************************************************************************

// CLASS DECLARATION (MFnHikEffector)

/// Full Body IK end effector function set (OpenMayaAnim)
/**
  Function set for full body inverse kinematics end effectors
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class MColor;
class MObjectArray;

class OPENMAYAANIM_EXPORT MFnHikEffector : public MFnTransform
{
	declareDagMFn( MFnHikEffector, MFn::kHikEffector );

public:
    ///
	MObject create( MObject parent = MObject::kNullObj,
					MStatus * ReturnStatus = NULL );
	///
	MVector	getPivotOffset( MStatus* ReturnStatus = NULL ) const;
	///
	MStatus setPivotOffset( const MVector& vector );
	///
	MColor	getEffColor ( MStatus* ReturnStatus = NULL ) const;
	///
	MStatus setEffColor( const MColor& color );
	///
	MStatus	getAuxiliaryEffectors( MObjectArray& effs ) const;
	
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
#endif /* _MFnHikEffector */
