#ifndef _MClothControl
#define _MClothControl
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
// *****************************************************************************
//
// CLASS:    MClothControl
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MClothControl)
//
//  This class provides access to various cloth data. This includes UVs per
//	per cloth, UVs per panel, stitcher node of a cloth, solver node of a cloth,
//	face membership of a panel, etc.
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

#include <maya/MClothTypes.h> 

class MObject;
class MIntArray;
class MDoubleArray;
class MClothPolyhedron;
class MClothSystem; 
class MDagPath;

// *****************************************************************************

// CLASS DECLARATION (MClothControl)

/// Access to cloth data
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class MAYACLOTH_EXPORT MClothControl
{
public:
	///
								MClothControl();
	///
	virtual						~MClothControl();
	///
	static MStatus				getUVs( const MString& cloth,
										const MString& panel,
										MDoubleArray& uvs );
	///
	static int					panelId( const MString& panel,
										 MStatus* ReturnStatus = NULL );
	///
	static MStatus				panelFaces( const MString& panel,
											MIntArray& faceIds );
	///
	static MObject				solverNode( const MString& cloth,
											MStatus* ReturnStatus = NULL );
	///
	static MObject				stitcherNode( const MString& cloth,
											  MStatus* ReturnStatus = NULL );

	///
	static bool					isClothMesh( MDagPath &path, MObject &solver,
											 int &index ); 

	///
	static bool					isSolverCloth( const MDagPath &cloth, 
											   const MString &solverName,
											   MClothSystem*& system,
											   MClothHandle& handle );

	///
	static double				getPrecedingFrame(	MClothSystem &system,
													double currentTime,
													double& precedingTime,
													MStatus* ReturnStatus = NULL);
	///
	static double				getSucceedingFrame( MClothSystem &system,
													double currentTime, 
													double& succeedingTime,
													MStatus* ReturnStatus = NULL);
	///
	static void					loadFrame( MClothPolyhedron &rigidBody,
										   double frame, 
										   int cacheNumber,
										   MStatus* ReturnStatus = NULL);
	///
	static double				dgTimeGivenClothFrame( MClothPolyhedron &rigidBody,
													   double frame,
													   MStatus* ReturnStatus = NULL);
	///
	static int					pinch(	MClothSystem &system, 
										MClothPolyhedron *poly1, 
										unsigned triIndex1, 
										MClothPolyhedron *poly2, 
										unsigned triIndex2,
										MStatus* ReturnStatus = NULL);
	///
	static void					externalForces( MClothSystem &system,
												MStatus* ReturnStatus = NULL);
	///
	static bool					solveCB( MClothSystem &system,
										 double cutoff, 
										 double cur, 
										 int istep);
	///
	static bool					collisionCB( MClothSystem &system,
											 int ipass,
											 double maxp);
	///
	static bool					stepCB( MClothSystem &system,
										double h);
	///
	static bool					interruptCheckCB( MClothSystem &system );

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
#endif /* _MClothControl */
