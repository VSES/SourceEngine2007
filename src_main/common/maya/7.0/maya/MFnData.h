
#ifndef _MFnData
#define _MFnData
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
// CLASS:    MFnData
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnData)
// 
//   MFnData is the parent class for all dependency graph data function 
//   sets.  Conceptually, data objects are what flow through the connections
//   in the dependency graph.
//   
//   Each node in the dependency graph has a data block associated with it.
//   The data block holds the data objects for all of the node's attributes
//   (see MDataBlock).  The data block is only available during the compute
//   method of a node.  A data handle (MDataHandle) can be created to access
//   a particular attribute's data inside of the data block.
//   
//   Once a data handle is obtained, it is possible to get the data object
//   from it.  This is only necessary for heavier data structures such as
//   geometry or strings.  Simple numeric data can be manipulated straight
//   through the data handle.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MFnBase.h>

// *****************************************************************************

// DECLARATIONS

// *****************************************************************************

// CLASS DECLARATION (MFnData)

/// Parent Class for dependency graph data function sets.
/**
  Common methods for manipulating dependency graph data.
*/

#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MFnData : public MFnBase 
{

	declareMFnNoConstructor( MFnData, MFnBase );

public:
	///
	MFnData();
    ///
	enum Type {
        ///
		kInvalid,
        /// Numeric, use MFnNumericData extract the node data.
		kNumeric,
		/// Plugin Blind Data, use MFnPluginData to extract the node data.
		kPlugin,
        /// Plugin Geometry, use MFnGeometryData to extract the node data.
		kPluginGeometry,
        /// String, use MFnStringData to extract the node data.
		kString,
        /// Matrix, use MFnMatrixData to extract the node data.
		kMatrix,
        /// String Array, use MFnStringArrayData to extract the node data.
		kStringArray,
        /// Double Array, use MFnDoubleArrayData to extract the node data.
		kDoubleArray,
        /// Int Array, use MFnIntArrayData to extract the node data.
		kIntArray,
        /// Point Array, use MFnPointArrayData to extract the node data.
		kPointArray,
        /// Vector Array, use MFnVectorArrayData to extract the node data.
		kVectorArray,
        /// Component List, use MFnComponentListData to extract the node data.
		kComponentList,
        /// Mesh, use MFnMeshData to extract the node data.
		kMesh,
        /// Lattice, use MFnLatticeData to extract the node data.
		kLattice,
        /// Nurbs Curve, use MFnNurbsCurveData to extract the node data.
		kNurbsCurve,
        /// Nurbs Surface, use MFnNurbsSurfaceData to extract the node data.
		kNurbsSurface,
        /// Sphere, use MFnSphereData to extract the node data.
		kSphere,
        /// ArrayAttrs, use MFnArrayAttrsData to extract the node data.
		kDynArrayAttrs,
        /// SweptGeometry, use MFnDynSweptGeometryData to extract the node data.
        kDynSweptGeometry,
        /// Subdivision Surface, use MFnSubdData to extract the node data.
		kSubdSurface,
        ///
		kLast
	};

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
#endif /* _MFnData */
