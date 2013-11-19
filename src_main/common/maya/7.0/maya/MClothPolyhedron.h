#ifndef _MClothPolyhedron
#define _MClothPolyhedron
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
// CLASS:    MClothPolyhedron
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MClothPolyhedron)
//
//   Interface for cloth solver collision rigid body. 
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


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
class MPointArray;
class MObject;
class MIntArray; 

// *****************************************************************************

// CLASS DECLARATION (MClothPolyhedron)

/// Interface for cloth solver collision rigid body.
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class MAYACLOTH_EXPORT MClothPolyhedron
{
public:
	///
	MClothPolyhedron();
	///
	virtual ~MClothPolyhedron();

	///
	virtual MStatus create( int nVertices,
							int indices[][3], 
							int nTriangles,
							double triDepthMap[],
							double triCollisionOffsetMap[],
							char *name = NULL,
							unsigned numRampSamples = 0, 
							float* offsetVelMultiplier = NULL,
							float* depthVelMultiplier = NULL,
							double dynFrictionMultiplier = 1.0,
							double staticFrictionMultiplier = 1.0 ) = 0;

	///
	virtual unsigned numVertices( MStatus* ReturnStatus = NULL ) const = 0;
	///
	virtual unsigned numTriangles( MStatus* ReturnStatus = NULL ) const = 0;

	///
	virtual MStatus setPosition( const MPointArray& postions ) = 0;
	///
	virtual MStatus setPosition( int index,
							  const MPoint& position,
							  int cacheNumber = 0 ) = 0;
	///
	virtual MStatus getPosition( MPointArray& postions ) = 0;

	///
	virtual MStatus updateNormals( bool init = false ) = 0;
	///
	virtual MStatus updateFromMesh( double currTime,
									double scale,
									const MObject& newMesh ) = 0;
	///
	virtual MStatus updateVelocityMultiplier( unsigned numRampSamples,
											 float offsetVelMultiplier[],
											 float depthVelMultiplier[] ) = 0;

	///
	virtual	void updateDynFrictionMultiplier( double val ) = 0;
	///
	virtual void updateStaticFrictionMultiplier( double val ) = 0;
	///
	virtual void disableCollisions( const MIntArray &faces ); 
	
public:
	// Internal use for CpClothSolver plugin.
	///
	void  setUserdata( void* userPtr );
	///
	void* getUserdata();

protected:
	// No protected members

private:
	void* fUserData;
};

typedef MClothPolyhedron * MClothPolyhedronPtr;

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MClothPolyhedron */

