
#ifndef _MFnAttribute
#define _MFnAttribute
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
// CLASS:    MFnAttribute
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnAttribute)
//
//  MFnAttribute is the function set for dependency node attributes.  
//
//  An attribute of a dependency node describes a piece of data that belongs to 
//  nodes of that type.  For example, a node that makes a sphere might have a 
//  radius attribute.  A node's attributes describe connections sites on the
//  node that can be used in the dependency graph.
//
//  Attributes typically belong to a class of nodes, rather than to an
//  individual node.  For example, all sphere nodes have a radius.  Even though
//  a single attribute may be shared by all nodes of a given type, each node
//  will have  its own value for the attribute.
//
//  Attributes are heirarchical.  For example, the translate scale attribute
//  of a transform has x, y, and z child attributes.
//  
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MFnBase.h>
#include <maya/MFnData.h>

// *****************************************************************************

// DECLARATIONS

class MTypeId;
class MString;
class MStringArray;

// *****************************************************************************

// CLASS DECLARATION (MFnAttribute)

/// Dependency node attribute function set
/**
 Function set for attributes of dependency nodes.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MFnAttribute : public MFnBase 
{

	declareMFn(MFnAttribute, MFnBase);

public:

	///
	enum DisconnectBehavior {
		/// delete array element on disconnect (array attributes only)
		kDelete,
		/// reset the attribute to its default on disconnect
		kReset,
		/// do nothing to the attribute's value on disconnect
		kNothing
	};

	///
	bool		isReadable( MStatus* ReturnStatus=NULL ) const;
	///
	bool		isWritable( MStatus* ReturnStatus=NULL ) const;
	///
	bool		isConnectable( MStatus* ReturnStatus=NULL ) const;
	///
	bool		isStorable( MStatus* ReturnStatus=NULL ) const;
	///
	bool		isCached( MStatus* ReturnStatus=NULL ) const;
	///
	bool		isArray( MStatus* ReturnStatus=NULL ) const;
	///
	bool		indexMatters( MStatus* ReturnStatus=NULL ) const;
	///
	bool		isKeyable( MStatus* ReturnStatus=NULL ) const;
	///
	bool		isChannelBoxFlagSet( MStatus* ReturnStatus=NULL ) const;
	///
	bool		isHidden( MStatus* ReturnStatus=NULL ) const; 
	///
	bool		isUsedAsColor( MStatus* ReturnStatus=NULL ) const;
	///
	bool		isIndeterminant( MStatus* ReturnStatus=NULL ) const;
	///
	bool		isRenderSource( MStatus* ReturnStatus=NULL ) const;
	///
	bool		isDynamic( MStatus* ReturnStatus=NULL ) const;
	///
	bool		isWorldSpace( MStatus* ReturnStatus=NULL ) const;
	///
	bool		isAffectsWorldSpace( MStatus* ReturnStatus=NULL ) const;

	///
	DisconnectBehavior disconnectBehavior( MStatus* ReturnStatus=NULL ) const;
	///
	bool        usesArrayDataBuilder( MStatus* ReturnStatus=NULL ) const;
	///
	bool     	internal( MStatus* ReturnStatus=NULL ) const;

	///
	MStatus		setReadable( bool state );
	///
	MStatus		setWritable( bool state ); 
	///
	MStatus		setConnectable( bool state );
	///
	MStatus	 	setStorable( bool state );
	///
	MStatus		setCached( bool state );
	///
	MStatus	 	setArray( bool state );
	///
	MStatus	 	setIndexMatters( bool state );
	///
	MStatus	 	setKeyable( bool state );
	///
	MStatus	 	setChannelBox( bool state );
	///
	MStatus	 	setHidden( bool state );
	///
	MStatus     setUsedAsColor( bool state );
	///
	MStatus     setIndeterminant( bool state );
    ///
	MStatus		setRenderSource( bool state );
    ///
	MStatus		setWorldSpace( bool state );
    ///
	MStatus		setAffectsWorldSpace( bool state );

	///
	MStatus     setDisconnectBehavior( DisconnectBehavior behavior );
	///
	MStatus     setUsesArrayDataBuilder( bool state );
	///
	MStatus     setInternal( bool state );
	///
	bool		accepts( MFnData::Type type, MStatus* ReturnStatus=NULL ) const;
	///
	bool		accepts( const MTypeId& id, MStatus* ReturnStatus=NULL ) const;
	///
	MObject		parent(MStatus* ReturnStatus=NULL) const;
	///
	MStatus		setParent( const MObject & parent );
	///
    MString     name( MStatus* ReturnStatus=NULL ) const; 
	///
    MString     shortName( MStatus* ReturnStatus=NULL ) const; 
	///
	MString		getAddAttrCmd(
					bool			useLongName = false,
					MStatus*		status=NULL
				) const;

protected:
	void * ca[3];
	void setPtr( MPtrBase* );

private:
	virtual bool objectChanged( MFn::Type, MStatus * );
};
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MFnAttribute */
