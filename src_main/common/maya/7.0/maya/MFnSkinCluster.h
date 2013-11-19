#ifndef _MFnSkinCluster
#define _MFnSkinCluster
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
// CLASS:    MFnSkinCluster
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnSkinCluster)
//
// MFnSkinCluster is the function set for skinClusters.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MFnGeometryFilter.h>
#include <maya/MObject.h>
// *****************************************************************************

// DECLARATIONS
class MObject;
class MDagPath;
class MIntArray;
class MFloatArray;
class MDoubleArray;
class MObjectArray;
class MSelectionList;
class MString;
class MDagPathArray;

// *****************************************************************************

// CLASS DECLARATION (MFnSkinCluster)

/// skinCluster function set (OpenMayaAnim)
/**
MFnSkinCluster is the function set for editing the weights of
skinClusters.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAANIM_EXPORT MFnSkinCluster : public MFnGeometryFilter
{
	declareMFn(MFnSkinCluster, MFnGeometryFilter);

public:
	///
	MStatus		getWeights(const MDagPath& path,
						   const MObject& components,
						   unsigned influenceIndex,
						   MDoubleArray& weights) const;
	///
	MStatus		getWeights(const MDagPath& path,
						   const MObject& components,
						   MDoubleArray& weights,
						   unsigned& influenceCount) const;
	///
	MStatus		setWeights(const MDagPath& path,
						   const MObject& components,
						   unsigned jointIndex,
						   double value,
						   bool normalize = true,
						   MDoubleArray *oldValues = NULL);
	///
	MStatus		setWeights(const MDagPath& path,
						   const MObject& components,
						   MIntArray& influenceIndices,
						   MDoubleArray& values,
						   bool normalize = true,
						   MDoubleArray *oldValues = NULL);
	///
	MStatus		getPointsAffectedByInfluence(const MDagPath& path,
											 MSelectionList& result,
											 MDoubleArray& weights);
	///
	unsigned 	indexForInfluenceObject(const MDagPath& path,
										MStatus *ReturnStatus) const;
	///
	unsigned 	influenceObjects(MDagPathArray& paths,
								 MStatus *ReturnStatus) const;
	///
	MStatus		getWeights(const MDagPath& path,
						   const MObject& components,
						   unsigned influenceIndex,
						   MFloatArray& weights) const;
	///
	MStatus		getWeights(const MDagPath& path,
						   const MObject& components,
						   MFloatArray& weights,
						   unsigned& influenceCount) const;
	///
	MStatus		setWeights(const MDagPath& path,
						   const MObject& components,
						   unsigned jointIndex,
						   float value,
						   bool normalize = true,
						   MFloatArray *oldValues = NULL);
	///
	MStatus		setWeights(const MDagPath& path,
						   const MObject& components,
						   MIntArray& influenceIndices,
						   MFloatArray& values,
						   bool normalize = true,
						   MFloatArray *oldValues = NULL);
	///
	MStatus		getPointsAffectedByInfluence(const MDagPath& path,
											 MSelectionList& result,
											 MFloatArray& weights);

protected:
// No protected members

private:
	MStatus		getWeightsHelper(bool useInfluenceIndex,
								 unsigned influenceIndex,
								 const MDagPath& path,
								 const MObject& components,
								 unsigned& influenceCount,
								 MDoubleArray& weights) const;

	MStatus		setWeightsHelper(const MDagPath& path,
								 const MObject& components,
								 bool useInfluenceIndex,
								 unsigned influenceIndex,
								 const MIntArray& infArray,
								 const MDoubleArray& wtArray,
								 bool normalize /* = true */,
								 MDoubleArray *oldValues /* = NULL */);

};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MFnSkinCluster */
