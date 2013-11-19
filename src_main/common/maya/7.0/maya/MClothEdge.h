#ifndef _MClothEdge
#define _MClothEdge
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
// CLASS:    MClothEdge
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MClothEdge)
//
//   Interface for cloth edge. 
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
class MClothParticle;
class MClothTriangle;
class MClothMaterial;

// *****************************************************************************

// CLASS DECLARATION (MClothEdge)

/// Interface for cloth edge.
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class MAYACLOTH_EXPORT MClothEdge
{
public:
	///
	MClothEdge();
	///
	virtual ~MClothEdge();

	///
	virtual MClothParticle* getParticle( unsigned int index, 
										 MStatus* ReturnStatus = NULL ) const = 0;
	///
	virtual MClothTriangle* getTriangle( unsigned int index,
										 MStatus* ReturnStatus = NULL ) const = 0;

	///
	virtual void			setCreaseAngle( const double theta,
											MStatus* ReturnStatus = NULL ) = 0;
	///
	virtual double			getCreaseAngle( MStatus* ReturnStatus = NULL ) const = 0;

	///
	virtual bool equals(MClothEdge *edge) const;

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
#endif /* _MClothEdge */
