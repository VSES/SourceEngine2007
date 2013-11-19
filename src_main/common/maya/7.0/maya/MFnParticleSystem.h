#ifndef _MFnParticleSystem
#define _MFnParticleSystem
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
// CLASS:    MFnParticleSystem
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnParticleSystem)
//
//      Particle object access class. The rendering related data in the particle 
//      object is a function of the particle render type. 
//
//	Safe to call these methods with any render type:
//
//	Method Name		Description
//	================================================================
//	isValid()		data is valid (predicate)
//	renderType()	        object render type
//	count()			particle count at current frame
//	----------------------------------------------------------------
//
//
//	Use this table to determine which methods to call based on the
//	render type of the particle object.
//
//	Render Type		Valid Methods		Description
//	================================================================
//	kCloud			position()		particle position
//				radius()		particle radius
//				surfaceShading()	object surface shading value
//				betterIllum()		invoke thick cloud sampling
//				threshold()		object threshold
//				disableCloudAxis()	for internal use; do not call
//	----------------------------------------------------------------
//	kTube			position0()		particle start position
//				position1()		particle end position
//				radius0()		particle start radius
//				radius1()		particle end radius
//				tailSize()		length scale factor
//	----------------------------------------------------------------
//	kBlobby			position()		particle position
//				radius()		particle radius
//				threshold()		object threshold
//	----------------------------------------------------------------
//	All hardware types	position()		particle position
//				radius()		particle radius
//	----------------------------------------------------------------
//	
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTypes.h>
#include <maya/MFnDagNode.h>


// *****************************************************************************

// DECLARATIONS

class MTime;
class MPoint;
class MVector;
class MFnDagNode;
class MIntArray;
class MPointArray;
class MDoubleArray;
class MVectorArray;
class MIntArray;

// *****************************************************************************

// CLASS DECLARATION (MFnParticleSystem)

/// 
/**
	Class for obtaining information about a particle system. (OpenMayaFX)
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAFX_EXPORT MFnParticleSystem : public MFnDagNode
{
    declareDagMFn(MFnParticleSystem, MFnDagNode);

public:

	///
	enum RenderType
	{
		///
		kCloud,
		///
		kTube,
		///
		kBlobby,
		///
		kMultiPoint,
		///
		kMultiStreak,
		///
		kNumeric,
		///
		kPoints,
		///
		kSpheres,
		///
		kSprites,
		///
		kStreak
	};

	///
	MObject			create                  ( MStatus* status = NULL );
	///
	MObject			create                  ( MObject parent, MStatus* status = NULL );
	///
	MStatus                 emit                    ( const MPoint& position );
	///
	MStatus                 emit                    ( const MPointArray& positionArray );
	///
	MStatus                 emit                    ( const MPoint& position, const MVector& velocity );
	///
	MStatus                 emit                    ( const MPointArray& positionArray, const MVectorArray& velocityArray );
	///
	MStatus                 saveInitialState        () const;
	///
	void			evaluateDynamics        ( MTime &to, bool runupFromStart );
	///
	bool			isValid			() const;
	///
	MString			particleName	        () const;
	///
	unsigned int	        count			() const;
	///
	RenderType		renderType		() const;
	///
	void			position		( MVectorArray& ) const;
	///
	void			velocity		( MVectorArray& ) const;
	///
	void			acceleration		( MVectorArray& ) const;
	///
	void			position0		( MVectorArray& ) const;
	///
	void			position1		( MVectorArray& ) const;
	///
	void			radius			( MDoubleArray& ) const;
	///
	void			radius0			( MDoubleArray& ) const;
	///
	void			radius1			( MDoubleArray& ) const;
	///
	double			surfaceShading	        () const;
	///
	double			threshold		() const;
	///
	bool			betterIllum		() const;
	///
	bool			disableCloudAxis        () const;
	///
	double			tailSize		() const;
	///
	void			particleIds		( MIntArray& ) const;
	///
	void			age			( MDoubleArray& ) const;
	///
	void			lifespan		( MDoubleArray& ) const;
	///
	void			rgb			( MVectorArray& ) const;
	///
	void			opacity			( MDoubleArray& ) const;
	///
	void			mass			( MDoubleArray& ) const;
	///
	void			emission		( MVectorArray& ) const;
	///
	bool			hasLifespan		() const;
	///
	bool			hasRgb			() const;
	///
	bool			hasOpacity		() const;
	///
	bool			hasEmission		() const;
	///
	bool			primaryVisibility	() const;
	///
	bool			visibleInReflections    () const;
	///
	bool			visibleInRefractions    () const;
	///
	bool			castsShadows		() const;
	///
	bool			receiveShadows		() const;
	///
	unsigned int            getPerParticleAttribute ( MString &attrName, MIntArray&, MStatus* status = NULL ) const;
	///
	unsigned int            getPerParticleAttribute ( MString &attrName, MVectorArray&, MStatus* status = NULL ) const;
	///
	unsigned int            getPerParticleAttribute ( MString &attrName, MDoubleArray&, MStatus* status = NULL ) const;
	///
	void                    setPerParticleAttribute ( MString &attrName, MVectorArray&, MStatus* status = NULL ) const;
	///
	void                    setPerParticleAttribute ( MString &attrName, MDoubleArray&, MStatus* status = NULL ) const;
	///
	bool                    isPerParticleIntAttribute ( const MString &attrName, MStatus* status = NULL ) const;
	///
	bool                    isPerParticleDoubleAttribute ( const MString &attrName, MStatus* status = NULL ) const;
	///
	bool                    isPerParticleVectorAttribute ( const MString &attrName, MStatus* status = NULL ) const;
	///
	bool                    isDeformedParticleShape ( MStatus* status = NULL ) const;
	///
	MObject                 deformedParticleShape   ( MStatus* status = NULL ) const;
	///
	MObject                 originalParticleShape   ( MStatus* status = NULL ) const;

protected:

	virtual bool	objectChanged( MFn::Type, MStatus * );

private:
// No private members
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MFnParticleSystem */
