
#ifndef _MFnRenderLayer
#define _MFnRenderLayer
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
// CLASS:    MFnRenderLayer
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnRenderLayer)
//
//	This class is a function set for render layer objects.
//	This provides functionality for getting the objects present in the 
//	render layer, checking whether the given object is in the current 
//	layer, etc.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES



#include <maya/MStatus.h>
#include <maya/MTypes.h>
#include <maya/MObject.h>
#include <maya/MFnDependencyNode.h>

// *****************************************************************************

// DECLARATIONS

class MDagPath;
class MPlug;

// *****************************************************************************

// CLASS DECLARATION (MFnRenderLayer)

///  Function set for render layer.
/**

Provide functionalities for working with render layers.
*/

class MObjectArray;

#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYARENDER_EXPORT MFnRenderLayer : public MFnDependencyNode
{
	declareMFn(MFnRenderLayer, MFnDependencyNode); 

public:
	///
	bool inCurrentRenderLayer(const MDagPath& objectPath, MStatus *ReturnStatus = NULL);
	///
	MStatus listMembers(MObjectArray& objectsInLayer) const;
	///
	bool isPlugAdjusted(const MPlug& scenePlug, MStatus *ReturnStatus = NULL) const;

protected:
// No protected members

private:
	void *renderLayer;

};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MFnRenderLayer */
