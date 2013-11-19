
#ifndef _MFnWireDeformer
#define _MFnWireDeformer
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
// CLASS:    MFnWireDeformer
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnWireDeformer)
//
// MFnWireDeformer is the function set for wire deformers.  Wire deformers
// modify geometry based on its proximity to controlling wire curves.  As
// the wire curves are modified, the parts of the geometry close to the
// curve will follow.
//
// The wire deformer is actually a small network of dependency nodes in
// the dependency graph.  This function set is provided to make
// manipulation of the network easier.  The main deformer node should be
// given to this function set as its object.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MFnDependencyNode.h>
#include <maya/MPoint.h>
// *****************************************************************************

// DECLARATIONS
class MDagPath;
class MObjectArray;

// *****************************************************************************

// CLASS DECLARATION (MFnWireDeformer)

/// wire deformer function set (OpenMayaAnim)
/**
 Function set for wire deformer
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAANIM_EXPORT MFnWireDeformer : public MFnDependencyNode
{

	declareMFn(MFnWireDeformer, MFnDependencyNode );

public:

	///
	MObject  create( MStatus * ReturnStatus = NULL );


	///
	MStatus addGeometry( const MObject & object );
	///
	MStatus removeGeometry( const MObject & object );
	///
	MStatus getAffectedGeometry( MObjectArray & objects );

	///
	unsigned numWires( MStatus * ReturnStatus = NULL ) const;
	///
	MStatus  addWire( const MObject & object );
	///
	MObject  wire( unsigned wireIndex, MStatus * ReturnStatus = NULL );
	///
	float    wireDropOffDistance( unsigned wireIndex,
								  MStatus * ReturnStatus = NULL ) const;
	///
	MStatus  setWireDropOffDistance( unsigned wireIndex, float dropOff );
	///
	float    wireScale( unsigned wireIndex,
								  MStatus * ReturnStatus = NULL ) const;
	///
	MStatus  setWireScale( unsigned wireIndex, float scale );
	///
	MObject  holdingShape( unsigned wireIndex,
						          MStatus * ReturnStatus = NULL ) const;
	///
	MStatus  setHoldingShape( unsigned wireIndex, MObject holdingCurve );

	///
	float    envelope( MStatus * ReturnStatus = NULL ) const;
	///
	MStatus  setEnvelope( float envelope );
	///
	float    rotation( MStatus * ReturnStatus = NULL ) const;
	///
	MStatus  setRotation( float rotation );
	///
	float    localIntensity( MStatus * ReturnStatus = NULL ) const;
	///
	MStatus  setLocalIntensity( float localIntensity );
	///
	float    crossingEffect( MStatus * ReturnStatus = NULL ) const;
	///
	MStatus  setCrossingEffect( float crossingEffect );

	///
	unsigned numDropoffLocators( unsigned wireIndex,
								 MStatus * ReturnStatus = NULL ) const;
	///
	MStatus  setDropoffLocator( unsigned wireIndex, unsigned locatorIndex,
						         float param, float percentage );
	///
	MStatus  getDropoffLocator( unsigned wireIndex, unsigned locatorIndex,
						         float &param, float &percentage );
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
#endif /* _MFnWireDeformer */
