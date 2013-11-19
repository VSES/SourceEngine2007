
#ifndef _MFnIkHandle
#define _MFnIkHandle
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
// CLASS:    MFnIkHandle
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnIkHandle)
//
//  This is the function set for inverse kinematics (IK) handles.
//  An ik handle specifies the joints in a skeleton that are effected
//  by an attached ik solver.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MFnTransform.h>
#include <maya/MObject.h>

// *****************************************************************************

// DECLARATIONS

class MObjectArray;

// *****************************************************************************

// CLASS DECLARATION (MFnIkHandle)

/// Function set for inverse kinematics (IK) handles (OpenMayaAnim)
/**

*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAANIM_EXPORT MFnIkHandle : public MFnTransform
{
	declareDagMFn( MFnIkHandle, MFn::kHandle );

public:
    ///
	enum Stickiness {
        ///
		kStickyOff,
        ///
		kStickyOn,
        ///
		kSuperSticky
    };
    ///
	MObject   create( MDagPath& startJoint,
	                  MDagPath& effector,
					  MStatus * ReturnStatus = NULL );
    ///
	MStatus   getStartJoint( MDagPath &jointPath );
    ///
	MStatus   setStartJoint( const MDagPath &jointPath );
    ///
	MStatus   getEffector( MDagPath &effectorPath );
    ///
	MStatus   setEffector( const MDagPath &effectorPath );
    ///
	MStatus   setStartJointAndEffector( const MDagPath &jointPath,
										const MDagPath &effectorPath );
    ///
	unsigned  priority( MStatus * ReturnStatus = NULL );
    ///
	MStatus   setPriority( unsigned priority );
    ///
	Stickiness stickiness( MStatus * ReturnStatus = NULL );
    ///
	MStatus   setStickiness( Stickiness stickiness );
    ///
	double    weight( MStatus * ReturnStatus = NULL );
    ///
	MStatus   setWeight( double weight );
    ///
	double    poWeight( MStatus * ReturnStatus = NULL );
    ///
	MStatus   setPOWeight( double weight );
    ///
	MObject   solver( MStatus * ReturnStatus = NULL );
    ///
	MStatus   setSolver( const MObject &solver );
    ///
	MStatus   setSolver( const MString & solverName );

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
#endif /* _MFnIkHandle */
