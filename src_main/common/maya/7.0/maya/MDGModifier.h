#ifndef _MDGModifier
#define _MDGModifier
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
// CLASS:    MDGModifier
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MDGModifier)
//
// An MDGModifier is used to change the structure of the dependency graph.  This
// includes adding nodes, making new connections, and removing existing
// connections.  An MDGModifier keeps track of all operations that it is
// used for, so that they can be undone or redone.  Calling undo causes an
// MDGModifier to undo all of the dependency graph changes that it has
// done.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES



#include <maya/MStatus.h>
#include <maya/MFnDependencyNode.h>

// *****************************************************************************

// DECLARATIONS

class TdependNode;
class Tattribute;
class TattributeList;
class Tstring;
class MObject;
class MPlug;
class MTypeId;
class MString;

// *****************************************************************************

// CLASS DECLARATION (MDGModifier)

/// Dependency graph modifier
/**
  A class that is used to modify the dependency graph and also supports undo 
*/
#if defined(_WIN32)
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MDGModifier  
{
public:
    ///
	MDGModifier();
    ///
	virtual ~MDGModifier();    
    
    ///
    MObject     createNode( const MTypeId &typeId,
                            MStatus* ReturnStatus = NULL );
    ///
    MObject     createNode( const MString &type,
                            MStatus* ReturnStatus = NULL );
    /// 
    MStatus     deleteNode( const MObject & node );
    
    ///
    MStatus     renameNode( const MObject & node, const MString &newName );

	///
	MStatus		setNodeLockState( const MObject & node, bool newState ); 

    ///
	MStatus		connect(	const MObject & sourceNode,
							const MObject & sourceAttr,
							const MObject & destNode,	
							const MObject & destAttr );
    ///
	MStatus		disconnect(	const MObject & sourceNode,
							const MObject & sourceAttr,
							const MObject & destNode,
							const MObject & destAttr );
    ///
	MStatus		connect(	const MPlug& source, const MPlug& dest );
    ///
	MStatus		disconnect(	const MPlug& source, const MPlug& dest );

	///
    MStatus     addAttribute( const MObject& node, const MObject& attribute,
							  MFnDependencyNode::MAttrClass type 
									= MFnDependencyNode::kLocalDynamicAttr );
    ///
    MStatus     removeAttribute( const MObject& node, const MObject& attribute,
								 MFnDependencyNode::MAttrClass type 
									= MFnDependencyNode::kLocalDynamicAttr  );

    ///
	MStatus		commandToExecute( const MString& command );
    ///
	MStatus		doIt();
    ///
	MStatus		undoIt();


protected:
    MDGModifier( void * );
	MDGModifier( const MDGModifier & other );
	MDGModifier&	operator =( const MDGModifier & rhs );
	bool		checkCompoundAttrs(const TattributeList&, const Tstring&, const Tstring&);
	bool		attrExistsAtNode(TdependNode*, Tattribute*);
	void*		  data;
	bool		  fOwn;

private:
	static const  char*	className();
};

#if defined(_WIN32)
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MDGModifier */
