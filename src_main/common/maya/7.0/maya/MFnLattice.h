
#ifndef _MFnLattice
#define _MFnLattice
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
// CLASS:    MFnLattice
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnLattice)
//
//	MFnLattice is the function set for lattice shapes and lattice geometry.
//	It can be used on lattices in the DAG or on lattice geometry from a
//	dependency node attribute.
//
//	Lattices are most commonly used for specifying FFDs (free-form
//	deformations).  See MFnLatticeDeformer for more information on those.
//
//	MFnLatticeData can be used to create new lattice data objects for use with
//	dependency node attributes.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MFnDagNode.h>
#include <maya/MPoint.h>
#include <maya/MVector.h>
#include <maya/MObject.h>
#include <maya/MTransformationMatrix.h>

// *****************************************************************************

// DECLARATIONS

class MPointArray;
class MDoubleArray;
class MPoint;
class MVector;
class MDagPath;
class MPtrBase;

// *****************************************************************************

// CLASS DECLARATION (MFnLattice)

/// Lattice function set (OpenMayaAnim)
/**
  Function set for lattice shapes and lattice geometry
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAANIM_EXPORT MFnLattice : public MFnDagNode
{
	declareDagMFn(MFnLattice, MFnDagNode);

public:

    ///
	MObject 	create( unsigned xDiv, unsigned yDiv, unsigned zDiv,
						MObject parentOrOwner = MObject::kNullObj,
						MStatus* ReturnStatus = NULL );
	///
	MStatus     reset( double sSize = 1.0 , double tSize = 1.0,
						double uSize = 1.0 );
	///
	MStatus     getDivisions( unsigned & s, unsigned & t, unsigned & u );
	///
	MStatus     setDivisions( unsigned   s, unsigned   t, unsigned   u );

	///
	MPoint &    point( unsigned s, unsigned t, unsigned u,
					   MStatus* ReturnStatus = NULL );

protected:
	virtual bool objectChanged( MFn::Type, MStatus * );
private:
// No private members

};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MFnLattice */
