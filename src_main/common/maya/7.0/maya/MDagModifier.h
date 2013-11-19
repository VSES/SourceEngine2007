
#ifndef _MDagModifier
#define _MDagModifier
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
// CLASS:    MDagModifier
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MDagModifier)
//
// An MDagModifier is used to change the structure DAG.  This
// includes adding nodes, making new connections, and removing existing
// connections.  An MDagModifier keeps track of all operations that it is
// used for, so that they can be undone or redone.  Calling undo causes an
// MDagModifier to undo all of the dependency graph changes that it has
// done.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES



#include <maya/MStatus.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MDGModifier.h>
#include <maya/MObject.h>

// *****************************************************************************

// DECLARATIONS

// *****************************************************************************

// CLASS DECLARATION (MDagModifier)

/// Dependency graph modifier
/**
  A class that is used to modify the DAG and also supports undo 
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MDagModifier : public MDGModifier
{
public:
    ///
	MDagModifier();
    ///
	~MDagModifier();
    
    ///
    MObject     createNode(     const MTypeId &typeId, 
                                const MObject & parent =  MObject::kNullObj,
                                MStatus* ReturnStatus = NULL );
    ///
    MObject     createNode(     const MString &type,
                                const MObject & parent =  MObject::kNullObj,
                                MStatus* ReturnStatus = NULL );
    ///
    MStatus     reparentNode(   const MObject & node, 
                                const MObject & newParent = MObject::kNullObj );
 

protected:
	MDagModifier(void*);
	MDagModifier( const MDagModifier & other );
	MDagModifier&	operator =( const MDagModifier & rhs );

private:
	static const  char*	className();
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MDagModifier */
