#ifndef _MFnPfxGeometry
#define _MFnPfxGeometry
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
// CLASS:    MFnPfxGeometry
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnPfxGeometry)
//
// This is the function set for PfxGeometry objects.
// 
// A pfxGeometry object is a node in the dependency graph that contains a grid which 
// is made up of cells.  Each cell has a variety of values assigned to it such
// as density, velocity, temperature, and color.  The grid may be 2D or 3D.  
// See the methods below for full details.
// 
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES

#include <maya/MFnDagNode.h>

// *****************************************************************************

// DECLARATIONS

class MPointArray;
class MVectorArray;
class MDoubleArray;
class MRenderLineArray;

// *****************************************************************************
                                                 
// CLASS DECLARATION (MFnPfxGeometry)

/// PfxGeometry node function set
/**

*/

#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAFX_EXPORT MFnPfxGeometry : public MFnDagNode 
{
	declareDagMFn(MFnPfxGeometry, MFnDagNode);

	///
	MStatus getLineData(
			MRenderLineArray &mainLines, 
			MRenderLineArray &leafLines, 
			MRenderLineArray &flowerLines,
			bool doLines, bool doTwist,
			bool doWidth, bool doFlatness,
			bool doParameter, bool doColor,
			bool doIncandescence, bool doTransparency,
			bool worldSpace );

	///
	MStatus getBoundingBox( double* min, double* max );

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
#endif /* _MFnPfxGeometry */
