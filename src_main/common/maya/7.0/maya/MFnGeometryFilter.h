
#ifndef _MFnGeometryFilter
#define _MFnGeometryFilter
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
// CLASS:    MFnGeometryFilter
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnGeometryFilter)
//
// MFnGeometryFilter is the function set for geometry filters (deformers)
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MFnDependencyNode.h>
#include <maya/MObject.h>
// *****************************************************************************

// DECLARATIONS
class MObject;
class MDagPath;
class MIntArray;
class MFloatArray;
class MObjectArray;
class MSelectionList;
class MString;

// *****************************************************************************

// CLASS DECLARATION (MFnGeometryFilter)

///  geometry filter function set (OpenMayaAnim)
/**
MFnGeometryFilter is the function set for deformers.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAANIM_EXPORT MFnGeometryFilter : public MFnDependencyNode
{
	declareMFn(MFnGeometryFilter, MFnDependencyNode);

public:
	///
	MStatus		getInputGeometry(MObjectArray &objects) const;
	///
	MStatus		getOutputGeometry(MObjectArray &objects) const;
	///
	MObject		inputShapeAtIndex(unsigned index,
								  MStatus *ReturnStatus = NULL) const;
	///
	MObject		outputShapeAtIndex(unsigned index,
								   MStatus *ReturnStatus = NULL) const;
	///
	unsigned	indexForOutputShape(const MObject &shape,
									MStatus *ReturnStatus = NULL) const;
	///
	MStatus		getPathAtIndex(unsigned index,
							   MDagPath &path) const;
	///
	unsigned	indexForGroupId(unsigned groupId,
								MStatus *ReturnStatus = NULL) const;
	///
	unsigned	groupIdAtIndex(unsigned index,
							   MStatus *ReturnStatus = NULL) const;
	///
	unsigned	numOutputConnections(MStatus *ReturnStatus = NULL) const;
	///
	unsigned	indexForOutputConnection(unsigned connectionIndex,
										 MStatus *ReturnStatus = NULL) const;
	///
	MObject		deformerSet(MStatus *ReturnStatus = NULL) const;
	///
	float		envelope(MStatus *ReturnStatus = NULL) const;
	///
	MStatus		setEnvelope(float envelope);

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
#endif /* _MFnGeometryFilter */
