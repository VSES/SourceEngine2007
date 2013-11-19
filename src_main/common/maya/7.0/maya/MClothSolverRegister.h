
#ifndef _MClothSolverRegister
#define _MClothSolverRegister

//
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
//
// CLASS:    MClothSolverRegister
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MClothSolverRegister)
//
//   Register and unregister cloth solver plug-in services with CpClothPlugin. 
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTypes.h>

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

class MString ;
class MClothSystem ;

// *****************************************************************************

// CLASS DECLARATION (MClothSolverRegister)

// Register and unregister cloth solver plug-in services with CpClothPlugin. 
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

///
typedef MClothSystem* (*MClothSolverCreatorFnPtr) (void);

/// Manages cloth solvers 
/**
Provides an interface for plugins to register/unregister cloth solver classes with Maya.
Whenever a new solver needs to be generated, it checks the classification string of the node for the preferred solver.
If a match is found, it creates and manages the solver object (that is it deletes the solver object once the solver
is removed from scene). 
*/
class MAYACLOTH_EXPORT MClothSolverRegister {
public:
	/// registers a new cloth solver creation function by name. 
	static MStatus	registerClothSolver(MString solverTypeName, MClothSolverCreatorFnPtr fnPtr);

	/// removes the previously registered solver
	static MStatus	unregisterClothSolver(MString solverTypeName);

protected:
// No protected members

private:
// No private members
};


// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MClothSolverRegister */

