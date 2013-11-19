
#ifndef _MItSubdFace
#define _MItSubdFace
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
// CLASS:    MItSubdFace
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MItSubdFace)
//
//	This class is the iterator for subdiv vertices.
//
//	The iterator iterates over all vertices for the subdiv.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MFnDagNode.h>
#include <maya/MObject.h>
#include <maya/MVector.h>
#include <maya/MPoint.h>
#include <maya/MColor.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MFloatArray.h>
#include <maya/MVectorArray.h>
#include <maya/MColorArray.h>
#include <maya/MString.h>

// *****************************************************************************

// DECLARATIONS

// *****************************************************************************

// CLASS DECLARATION (MItSubdFace)

/// Subdiv vertex iterator
/**
  Iterate of subdiv vertices
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MItSubdFace
{
public:   
	///
	MItSubdFace(MObject & subdObject, MStatus * ReturnStatus = NULL);
	///
	virtual ~MItSubdFace();
	///
	MStatus		reset();
	///
	MStatus		next(); 
	///
	bool		isDone(MStatus * ReturnStatus = NULL);
	///
	bool		isValid(MStatus * ReturnStatus = NULL) const;
	///
	unsigned int level(MStatus * ReturnStatus = NULL) const;
	///
	void setLevel(unsigned int level, MStatus * ReturnStatus = NULL);
	///
	MUint64		index(MStatus * ReturnStatus = NULL);

protected:

private:
	static const char* 	className();
	void     *       f_it;
}; 

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MItSubdFace */



