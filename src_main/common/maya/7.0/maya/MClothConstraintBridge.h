#ifndef _MClothConstraintBridge
#define _MClothConstraintBridge
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
// CLASS:    MClothConstraintBridge
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MClothConstraintBridge)
//
//   Interface for cloth solver constraint command bridge.
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
class MClothParticle;

// *****************************************************************************

// CLASS DECLARATION (MClothConstraintBridge)

/// Interface for cloth solver constraint command bridge.
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

// Interface that must be implemented by solver writers to override 
// custom behavior for various constraints.
class MAYACLOTH_EXPORT MClothConstraintBridge
{
public:
	///
	MClothConstraintBridge();
	///
	virtual ~MClothConstraintBridge();

	///
	virtual MStatus execute( MClothParticle* particle, 
							 bool soft, 
							 double shear, 
							 double damp ) = 0;
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
#endif /* _MClothConstraintBridge */
