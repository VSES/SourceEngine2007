#ifndef _MPxObjectSet
#define _MPxObjectSet
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
// CLASS:    MPxObjectSet
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MPxObjectSet)
// 
//  MPxObjectSet is the parent class of all user defined object sets.
//  User defined sets are dependency nodes.
//
//  This class can be used to implement new kinds of sets within maya that
//  can have selectable/manipulatable components and behave in a similar
//  manner to the objectSet node included in maya.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MTypes.h>
#include <maya/MPxNode.h>


// *****************************************************************************

// CLASS DECLARATION (MPxObjectSet)

/// Parent class of all user defined object sets
/**
	Create user defined sets.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MPxObjectSet : public MPxNode  
{
public:

	///
	MPxObjectSet();
	///
	virtual ~MPxObjectSet();
    ///               
	virtual MPxNode::Type type() const;
	///
	virtual bool canBeDeleted( bool isSrcNode );

	//
    // Inherited attributes
	//

	/// Connections to this attribute specify the dagNodes or
	/// parts (components) of the same that are members of this set.
	static MObject dagSetMembers;

	/// References to dependNode attributes which defines set membership.
	static MObject DNSetMembers;

	/// The index of a user defined color in which the dag object component
	/// members should appear. A value of -1 disables use of the color.
	/// Values outside the range [-1,7] may give unpredictable results.
	static MObject memberWireframeColor;

	/// A text string to annotate the set.
	static MObject annotation;

	/// This attribute is obsolete and should no longer be used
	static MObject isLayer;

	/// Is set membership restricted to objects with vertices?
	/// This attribute should not be explicitly changed by the user.
	static MObject verticesOnlySet;

	/// Is set membership restricted to objects with edges?
	/// This attribute should not be explicitly changed by the user.
	static MObject edgesOnlySet;

	/// Is set membership restricted to objects with facets?
	/// This attribute should not be explicitly changed by the user.
	static MObject facetsOnlySet;

	/// Is set membership restricted to objects with edit points?
	/// This attribute should not be explicitly changed by the user.
	static MObject editPointsOnlySet;

	/// Is set membership restricted to renderable objects only?
	/// This attribute should not be explicitly changed by the user.
	static MObject renderableOnlySet;

	/// References to the partition nodes this set is a member of.
	static MObject partition;

	/// When parts (components) of dagNodes are in the set, connections
	/// are made to this attribute to hold references to groupId nodes,
	/// with the id uniquely identifying the group in the dagNode.
	static MObject groupNodes;

	/// References to nodes that operate upon this set, such as deformers.
	static MObject usedByNodes;

private:
	static void				initialSetup();
	static const char*	    className();

};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MPxObjectSet */
