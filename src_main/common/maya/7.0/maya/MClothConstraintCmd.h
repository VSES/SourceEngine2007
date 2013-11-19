#ifndef _MClothConstraintCmd
#define _MClothConstraintCmd
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
// CLASS:    MClothConstraintCmd
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MClothConstraintCmd)
//
//   Interface for cloth solver constraint command.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES
#include <maya/MStatus.h>
#include <maya/MClothConstraint.h>
#include <maya/MClothConstraintBridge.h>



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

// CLASS DECLARATION (MClothConstraintCmd)

/// Interface for cloth solver constraint command.

#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class MAYACLOTH_EXPORT MClothConstraintCmd : public MClothConstraint
{
public:
	///
	MClothConstraintCmd( MClothParticle* particle );
	///
	MClothConstraintCmd( MClothParticle* particle, 
						 double shear, 
						 double damp );
	///
	virtual ~MClothConstraintCmd();

	///
	virtual MPoint	desired_position( double frame ) = 0;

	///
	void	setSoft(bool soft);
	///
	bool	isSoft() const;

	///
	void	setStrength( double shear, double damp );
	///
	double	getShear() const;
	///
	double	getDamp() const;

	///
	bool	userDefined();

public:
	// This must be provided by MClothConstraintBridge.
	///
	MStatus	execute();

	///
	void setCustomConstraintBehavior(MClothConstraintBridge* impl);
	///
	MClothConstraintBridge* getCustomConstraintBehavior() const;

protected:
	MClothConstraintBridge* fCustomConstraint; 
	double		fShear, fDamp;
	bool		fSoft;

private:
	// No private members
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MClothConstraintCmd */
