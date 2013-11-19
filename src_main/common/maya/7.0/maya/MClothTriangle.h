#ifndef _MClothTriangle
#define _MClothTriangle
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
// CLASS:    MClothTriangle
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MClothTriangle)
//
//   Interface for cloth triangle. 
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
class MClothParticle;
class MClothMaterial;

// *****************************************************************************

// CLASS DECLARATION (MClothTriangle)

/// Interface for cloth triangle.
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class MAYACLOTH_EXPORT MClothTriangle
{
public:
	///
	MClothTriangle();
	///
	virtual ~MClothTriangle();

	///
	virtual MClothEdge* getEdge( unsigned int index,
								 MStatus* ReturnStatus = NULL ) const = 0;
	///
	virtual MClothParticle* getParticle( unsigned int index,
										 MStatus* ReturnStatus = NULL ) const = 0;
	///
	virtual MClothMaterial* getMaterial( MStatus* ReturnStatus = NULL ) const = 0;

	///
	virtual MStatus getUV( int vertexIndex, 
						   double& u, 
						   double& v ) const = 0;
	///
	virtual MStatus getUVs( MDoubleArray& uArray, 
							MDoubleArray& vArray ) const = 0;

	///
	virtual bool contains( MClothParticle *n ) const;
	///
	virtual bool equals( MClothTriangle *triangle ) const;

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
#endif /* _MClothTriangle */
