
#ifndef _MFnBlendShapeDeformer
#define _MFnBlendShapeDeformer
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
// CLASS:    MFnBlendShapeDeformer
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnBlendShapeDeformer)
//
// MFnBlendShapeDeformer is the function set for blend shape deformers.  A
// blend shape deformer takes a base shape (polygonal surface, curve,
// surface, or lattice) and blends it with other target shapes based on
// weight values.
//
// The blend shape deformer is actually a small network of dependency
// nodes in the dependency graph.  This function set is provided to make
// manipulation of the network easier.  The main deformer node should be
// given to this function set as its object.
//
// There are three parts to a blend shape deformer.  There are the base
// objects, the target objects, and the weight values.
//
// The base objects are the shapes that are to be deformed.  There must be
// at least one base object.  The base objects will change form as the
// targets and deformation parameters are modified.
//
// Each base object has a list of target objects that affect its shape.
// Each target is associated with one of the the deformer's weight
// values.  When the weight value increases, the target has more influence
// on the base shape.
//
// There is just one array of weight values between all of the base
// objects and targets.  So, it is possible for targets of different base
// objects to share the same weight index.  When the weight value changes,
// it will affect all of the base objects that have targets using that
// weight value.
//
// It is also possible to chain together target shapes so that a base
// object will deform through each shape one at a time as the weight value
// increases.  This is done by adding multiple targets to a base shape
// using the same weight index for all of them.  When each target is
// added, a weight value is specified at which that target will be in full
// effect.  Give each target a different full weight value.
//
// For example, one could take a sphere and make it blend into a cone and
// then into a cylinder.  One way to do this is to make sphere the base
// shape.  Then, add the cone as a target for the sphere at weight index 0
// with a full effect weight of 0.5.  Next, add the cylinder as a second
// target for the sphere also at weight index 0, but with a full effect
// weight of 1.0.  Now, as the weight goes from 0 to 1, the base shape
// will start as a sphere, morph into a cone, and then into a cylinder.
//
// It is not necessary for the base shape and it's targets to have
// identical geometry, but the blend will be more effective if they do.
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
class MIntArray;

// *****************************************************************************

// CLASS DECLARATION (MFnBlendShapeDeformer)

/// blend shape deformer function set (OpenMayaAnim)
/**
 Function set for blend shape deformer
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAANIM_EXPORT MFnBlendShapeDeformer : public MFnDependencyNode
{

	declareMFn(MFnBlendShapeDeformer, MFnDependencyNode );

public:

	///
	enum Origin {
		///
		kLocalOrigin,
		///
		kWorldOrigin
	};

	///
	enum HistoryLocation {
		///
		kFrontOfChain,
		///
		kNormal
	};
	///
	MObject  create(  MObject baseObject,
					  Origin originSpace = kLocalOrigin,
					  MStatus * ReturnStatus = NULL );

	///
	MObject  create(  MObjectArray baseObjects,
					  Origin originSpace = kLocalOrigin,
					  HistoryLocation = kNormal,
					  MStatus * ReturnStatus = NULL );

	///
	MStatus  addBaseObject( MObject & object );
	///
	MStatus  getBaseObjects( MObjectArray & objects ) const;

	///
	MStatus  addTarget( const MObject & baseObject, int weightIndex,
						const MObject & newTarget, double fullWeight );

	///
    MStatus  removeTarget( const MObject & baseObject,
						   int weightIndex,
						   const MObject & target,
						   double fullWeight );
	///
	MStatus  getTargets( MObject baseObject, int weightIndex,
						 MObjectArray & targetObjects ) const;

	///
	unsigned numWeights( MStatus * ReturnStatus = NULL ) const;

	///
	MStatus  weightIndexList( MIntArray& indexList ) const;

	///
    MStatus  targetItemIndexList( unsigned targetIndex,
								  MObject baseObject,
								  MIntArray& inbetweens ) const;

	///
	float    weight( unsigned index, MStatus * ReturnStatus = NULL ) const;
	///
	MStatus  setWeight( unsigned index, float weight );

	///
	float    envelope( MStatus * ReturnStatus = NULL ) const;
	///
	MStatus  setEnvelope( float envelope );

	///
	Origin   origin( MStatus * ReturnStatus = NULL ) const;
	///
	MStatus  setOrigin( Origin space );

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
#endif /* _MFnBlendShapeDeformer */
