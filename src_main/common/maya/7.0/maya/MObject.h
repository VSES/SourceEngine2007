#ifndef _MObject
#define _MObject
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
// CLASS:    MObject
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MObject)
//
//  MObject is the generic class for accessing all Maya internal modelling,
//	animation and rendering Objects, collectively referred to as Model Objects,
//	through the API.  This includes all Dependency Graph (DG) Nodes, of which
//	Directed Acyclic Graph (DAG) Nodes are a subset.
//	
//	Each instance of an MObject represents a specific Node or Attribute on a
//	Node in the DG.  Under the C++ class scheme MObjects simply have type
//	MObject with no base or derived hierarchy.  However,  MObjects may be many
//	different types of Model objects, from Attributes to Xforms, including
//	components of geometry such as Control Vertices (CV), faces, edges and
//	vertices.  This allows many different types of objects to be moved accessed
//	through the API without undue concern on the part of plug-in developers for
//	the type of object being manipulated.
//
//	Since it is impractical to provide every applicable method on MObjects, the
//	methods applicable to Maya Objects are encapsulated in Function Sets.
//	Function Sets can be attached to compatible MObjects to provide an external
//	interface to methods appropriate to that MObject.
//
//  The link between an MObject and the role it performs as a Maya Model object,
//  as well as the compatibility between MObjects and Function Sets is managed
//  by the API Run-Time Type Identification (RTTI) system.  The two key concepts
//  of this system are the Maya Object type and the Function Set type (both in
//  MFn::Type).  All MObjects have one and only one Maya Object type.  All
//  Function Sets have one and only one Function Set type. MObjects may,
//  however, be compatible with many types of Function Sets.  This compatibility
//  follows the class hierarchy of the Function Sets.  Thus an MObject with Maya
//  Object type MFn::kNurbsSurface would be compatible with MFnNurbsSurface,
//  MFnDagNode, MFnDependencyNode, MFnNamedObject and MFnBase.
//
//	Any MObject can be queried with the MObject::hasFn() method to determine if
//	it is comaptible with a given Function Set.
//
//	The purpose of the Base Function Set (MFnBase) is to provide methods for
//	determining whether or not a Function Set is compatible with a given Maya
//	Object (MObject) type (MFn::Type) or a specific MObject.  See:
//					MFnBase::hasObj(MFn::Type)
//							and
//					MFnBase::hasObj(const MObject &)
//
//	As well, this Function Set provides virtual methods for determining the type
//	of a Function Set under the API RTTI system. 
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES

#include <maya/MFn.h>
#include <maya/MTypes.h>
#include <stdio.h>

// *****************************************************************************

// DECLARATIONS

class MPtrBase;
class MObjectHandle;

// *****************************************************************************

////////////////////////////////////////////////////////////////////////////////

// CLASS DECLARATION (MObject)

/// Generic Class for Accessing Internal Maya Objects
/**

Determine the exact type (MFn::Type) of an MObject within Maya.

Determine if an Object exists.

Determine if an Object is compatible with a specific Function Set.

*/

class FND_EXPORT  MObject  
{
public:


	///
	MObject();
	///
	MObject( const MObject &other ); 
	///
	~MObject();
	///
	bool			hasFn( MFn::Type fs ) const;
	///
	bool            isNull() const;
 
	///
    MFn::Type		apiType() const;
	///
	const char *    apiTypeStr() const;
 
	///
	bool			operator == (const MObject &) const;
	///
	bool			operator != (const MObject &) const;
	///
	MObject &		operator =  (const MObject &);

	/// Null Object for use as default argument.
	static MObject  kNullObj;

protected:
// No protected members

private:
	bool			isFlagSet( short ) const;
	void			setFlag( short, bool );
	short			allFlags() const;
	
	friend class	MObjectHandle;

	MPtrBase *		ptr;
	unsigned short	flags;
};

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MObject */
