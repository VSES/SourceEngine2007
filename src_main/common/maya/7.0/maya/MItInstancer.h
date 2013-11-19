#ifndef _MItInstancer
#define _MItInstancer
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
// CLASS:    MItInstancer
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MItInstancer)
//
// Use the particle instancer node iterator to traverse all instancer nodes in 
// Maya's Dependency Graph, and optionally 
// 
// With filtering enabled, the iterator checks to see if the node is
// compatible with the type specified by the filter.  See MFn::Type for a
// list of all valid types.
// 
// Since MObjects may be compatible with more than one type (nodes are
// organised in a hierarchy) the MObject::hasFn() method can be used to
// further check for compatible types.
// 
// Any compatible Function Set can be attached to the retrieved object to
// query or or edit it.  Often you will want to use the dependency node
// function set (MFnDependencyNode), which is compatible with all
// dependency nodes, to perform queries on each node as the iterator
// Traverses the Dependency Graph.
// 
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES

#include <maya/MObject.h>
#include <maya/MStatus.h>
#include <maya/MDagPath.h>
#include <maya/MMatrix.h>

// *****************************************************************************

// DECLARATIONS


// *****************************************************************************

// CLASS DECLARATION (MItInstancer)

/// Particle iterator.
/**

Iterate over all nodes in the dependency graph.

*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32


class OPENMAYA_EXPORT MItInstancer  
{
public:
    ///
				MItInstancer();
	///
	virtual		~MItInstancer();

    ///
	MStatus		reset();
	
	///
    MStatus		next();

	///
    MStatus   	nextParticle();

    ///
	MStatus		nextInstancer();

	///
    bool 		isDone() const;

	///
    MDagPath	path();

    ///
	MMatrix		matrix();

    ///
	MUintPtrSz instancerId() const;

    ///
	unsigned int particleId() const;

    ///
	unsigned int pathId() const;

protected:
// No protected members

private:
	void *data;
	static const char* className();
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MItInstancer */
