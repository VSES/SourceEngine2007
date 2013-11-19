
#ifndef _MFnLatticeDeformer
#define _MFnLatticeDeformer
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
// CLASS:    MFnLatticeDeformer
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnLatticeDeformer)
//
//	MFnLatticeDeformer is the function set for lattice deformers.  Lattice
//	deformers use FFDs to deform geometry.
//
//	The lattice deformer is actually a small network of dependency nodes in
//	the dependency graph.  This function set is provided to make
//	manipulation of the network easier.  The main deformer node should be
//	given to this function set as its object.
//
//	There are two lattices associated with a lattice deformer.  There is a
//	base lattice that defines the start position for the lattice.  The
//	second lattice is the version that is modified to deform the geometry.
//	The difference between the two lattices defines the deformation that is
//	applied to the geometry.
//
//	The base lattice is a very simple shape that only defines a box in
//	space.  The base lattice should be modified by using the standard DAG
//	transformation support.  The second deformable lattice has geometry
//	that can be modified using the MFnLattice function set.
//
//	For a piece of geometry to be modified by this deformer, the geometry
//	must be attached to the deformer (use addGeometry method) and the
//	geometry must be contained within the base lattice.  The resetLattice
//	method can be used to make the lattice resize to the bounding box of
//	the attached geometry.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MFnDependencyNode.h>

// *****************************************************************************

// DECLARATIONS
class MDagPath;
class MObjectArray;

// *****************************************************************************

// CLASS DECLARATION (MFnLatticeDeformer)

/// FFD lattice deformer function set (OpenMayaAnim)
/**
 Function set for FFD lattice deformer
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAANIM_EXPORT MFnLatticeDeformer : public MFnDependencyNode
{

	declareMFn(MFnLatticeDeformer, MFnDependencyNode );

public:

	///
	MObject create( unsigned xDiv, unsigned yDiv, unsigned zDiv,
					MStatus * ReturnStatus = NULL );

	///
	MStatus addGeometry( const MObject & object );
	///
	MStatus removeGeometry( const MObject & object );

	///
	MStatus getAffectedGeometry( MObjectArray & objects );

	///
	MStatus getDivisions( unsigned & x, unsigned & y, unsigned & z );
	///
	MStatus setDivisions( unsigned   x, unsigned   y, unsigned   z );

	///
	MStatus resetLattice( bool centerLattice = false );

	///
	MObject deformLattice( MStatus * ReturnStatus = NULL );
	///
	MObject baseLattice( MStatus * ReturnStatus = NULL );

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
#endif /* _MFnLatticeDeformer */
