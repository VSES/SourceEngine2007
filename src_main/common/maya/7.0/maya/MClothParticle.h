#ifndef _MClothParticle
#define _MClothParticle
//-
// ==========================================================================
// Copyright (C) Alias Systems Corp., and/or its licensors ("Alias").  
// All rights reserved.  These coded instructions, statements, computer  
// programs, and/or related material (collectively, the "Material")
// contain unpublished information proprietary to Alias, which is
// protected by Canadian and US federal copyright law and by international
// treaties. This Material may not be disclosed to third parties, or be copied
// or duplicated, in whole or in part, without the prior written consent of
// Alias.  ALIAS HEREBY DISCLAIMS ALL WARRANTIES RELATING TO THE MATERIAL,
// INCLUDING, WITHOUT LIMITATION, ANY AND ALL EXPRESS OR IMPLIED WARRANTIES OF
// NON-INFRINGEMENT, MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
// IN NO EVENT SHALL ALIAS BE LIABLE FOR ANY DAMAGES WHATSOEVER, WHETHER DIRECT,
// INDIRECT, SPECIAL, OR PUNITIVE, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
// OR OTHER TORTIOUS ACTION, OR IN EQUITY, ARISING OUT OF OR RELATED TO THE
// ACCESS TO, USE OF, OR RELIANCE UPON THE MATERIAL.
// ==========================================================================
//+
// *****************************************************************************
//
// CLASS:    MClothParticle
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MClothParticle)
//
//   Interface for cloth particle. 
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES
#include <maya/MStatus.h>



// MayaCloth Export
#ifdef _WIN32
#	ifndef MAYACLOTH_EXPORT
#		ifdef MAYACLOTH_DLL 
#			define MAYACLOTH_EXPORT _declspec( dllexport )
#		else
#			define MAYACLOTH_EXPORT _declspec( dllimport )
#		endif // MAYACLOTH_DLL
#	endif // MAYACLOTH_EXPORT
#else
#	ifndef MAYACLOTH_EXPORT
#		define MAYACLOTH_EXPORT
#	endif
#endif // _WIN32

// *****************************************************************************

// DECLARATIONS

class MPoint;
class MDoubleArray;
class MVector;
class MClothEdge;
class MClothTriangle;
class MClothMaterial;
class MClothPolyhedron; 

// *****************************************************************************

// CLASS DECLARATION (MClothParticle)

/// Interface for cloth particle.
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class MAYACLOTH_EXPORT MClothParticle
{
public:
	///
	MClothParticle();
	///
	virtual ~MClothParticle();

	///
	virtual MPoint getPosition( MStatus* ReturnStatus = NULL ) const = 0;
	///
	virtual void setPosition( const MPoint &position,
							  MStatus* ReturnStatus = NULL ) = 0;

	///
	virtual MVector getVelocity( MStatus* ReturnStatus = NULL ) const = 0;
	///
	virtual void setVelocity( const MVector &velocity,
							  MStatus* ReturnStatus = NULL ) = 0;

	///
	virtual MVector getAccelaration( MStatus* ReturnStatus = NULL ) const = 0;
	///
	virtual void setAccelaration( const MVector &acc,
								  MStatus* ReturnStatus = NULL ) = 0;

	///
	virtual MClothEdge* getEdge( unsigned int index,
								 MStatus* ReturnStatus = NULL ) const = 0;
	///
	virtual unsigned int numEdges( MStatus* ReturnStatus = NULL ) const = 0;

	///
	virtual bool ignoreSolid( MStatus* ReturnStatus = NULL ) const = 0;
	///
	virtual void ignoreSolid( bool isIgnored,
							  MStatus* ReturnStatus = NULL ) = 0;

	///
	virtual bool ignoreClothCollision( MStatus* ReturnStatus = NULL ) const = 0;
	///
	virtual void ignoreClothCollision( bool isIgnored,
									   MStatus* ReturnStatus = NULL ) = 0;

	///
	virtual double getMass( MStatus* ReturnStatus = NULL ) const = 0;
	///
	virtual void setMass( const double mass,
						  MStatus* ReturnStatus = NULL ) = 0;

	///
	virtual MClothMaterial* getMaterial( MStatus* ReturnStatus = NULL ) const = 0;

	///
	virtual unsigned getIndex() const = 0;

	///
	virtual bool equals(MClothParticle *other) const;

	///
	bool collideWithPolyhedron( MClothPolyhedron *poly = NULL ); 
	
protected:
	// No protected members

private:
	friend class CpHParticle; 
	void *fData; 
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MClothParticle */
