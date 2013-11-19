
#ifndef _MSelectionMask
#define _MSelectionMask
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
// CLASS:    MSelectionMask
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MSelectionMask)
//
// Selection masks provide a way to control what is selectable in Maya.
// 
// Selection masks can be created by OR'ing several types together, or
// by using the addMask method to add a selection type to an MSelectionMask.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES



#include <maya/MObject.h>
#include <maya/MStatus.h>

// *****************************************************************************

// DECLARATIONS

class MPoint;
class MString;
class MTime;

// *****************************************************************************

// CLASS DECLARATION (MSelectionMask)

/// Manage what is selectable in Maya
/**
  Selection masks provide a way to control what is selectable in Maya.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MSelectionMask
{
public:
	///
	enum SelectionType {
		///
		kSelectHandles,
		///
		kSelectLocalAxis,
		
		///
		kSelectIkHandles,
		///
		kSelectIkEndEffectors,
		///
		kSelectJoints,
		
		///
		kSelectLights,
		///
		kSelectCameras,
		
		///
		kSelectLattices,
		///
		kSelectClusters,
		///
		kSelectSculpts,
		
		///
		kSelectNurbsCurves,
		///
		kSelectNurbsSurfaces,
		///
		kSelectMeshes,
		///
		kSelectSubdiv,
		///
		kSelectSketchPlanes,
		
		///
		kSelectParticleShapes,
		///
		kSelectEmitters,
		///
		kSelectFields,
		///
		kSelectSprings,
		///
		kSelectRigidBodies,
		///
		kSelectRigidConstraints,
		///
		kSelectCollisionModels,
		
		///
		kSelectXYZLocators,
		///
		kSelectOrientationLocators,
		///
		kSelectUVLocators,
		
		///
		kSelectTextures,

		///
		kSelectCurves,
		///
		kSelectSurfaces,
		///
		kSelectLocators,
		///
		kSelectObjectsMask,
		
		
		///
		kSelectCVs,
		///
		kSelectHulls,
		///
		kSelectEditPoints,
		
		///
		kSelectMeshVerts,
		///
		kSelectMeshEdges,
		///
		kSelectMeshFreeEdges,
		///
		kSelectMeshFaces,
		///
		kSelectSubdivMeshPoints,
		///
		kSelectSubdivMeshEdges,
		///
		kSelectSubdivMeshFaces,
		///
		kSelectMeshUVs,
		
		///
		kSelectVertices,
		///
		kSelectEdges,
		///
		kSelectFacets,
		///
		kSelectMeshLines,
		///
		kSelectMeshComponents,
		
		///
		kSelectCurveParmPoints,
		///
		kSelectCurveKnots,
		///
		kSelectSurfaceParmPoints,
		///
		kSelectSurfaceKnots,
		///
		kSelectSurfaceRange,
		///
		kSelectSurfaceEdge,
		///
		kSelectIsoparms,
		///
		kSelectCurvesOnSurfaces,
		///
		kSelectPPStrokes,
		
		///
		kSelectLatticePoints,
		
		///
		kSelectParticles,
		///

		kSelectJointPivots,
		///
		kSelectScalePivots,
		///
		kSelectRotatePivots,
		
		///
		kSelectPivots,
		
		///
		kSelectSelectHandles,
		
		///
		kSelectComponentsMask,
		
		///
		kSelectAnimCurves,
		///
		kSelectAnimKeyframes,
		///
		kSelectAnimInTangents,
		///
		kSelectAnimOutTangents,
		
		///
		kSelectAnimMask,
		///
		kSelectAnimAny,
		
		///
		kSelectTemplates,

		///
		kSelectManipulators,
		///
		kSelectGuideLines,
		///
		kSelectPointsForGravity,
		///
		kSelectPointsOnCurvesForGravity,
		///
		kSelectPointsOnSurfacesForGravity,
		///
		kSelectObjectGroups,
		///
		kSelectSubdivMeshMaps,
		///
		kSelectFluids,
		///
		kSelectHairSystems,
		///
		kSelectFollicles
	};

	///
	MSelectionMask();
	///
	MSelectionMask( SelectionType selType );
	///
	MSelectionMask( const MSelectionMask& in );

	///
	virtual ~MSelectionMask();

	///
	MStatus		setMask( SelectionType selType );
	///
	MStatus		setMask( MSelectionMask& mask );
	///
	MStatus		addMask( SelectionType selType );

	///
	bool		intersects( SelectionType selType,
							MStatus * ReturnStatus = NULL ) const;
	///
	bool		intersects( MSelectionMask& mask,
							MStatus * ReturnStatus = NULL ) const;

	///
	MSelectionMask	operator| ( SelectionType selType );
	///
	MSelectionMask	operator| ( MSelectionMask& mask );
	///
	MSelectionMask&	operator =( MSelectionMask& other );

protected:
// No protected members

private:
	MSelectionMask( const void *, bool );
	const void * data;
	bool fOwn;
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MSelectionMask */
