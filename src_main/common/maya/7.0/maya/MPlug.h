#ifndef _MPlug
#define _MPlug
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
// CLASS:    MPlug
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MPlug)
//
//	MPlug provides methods for creating plugs, and accessing the plugs'
//	dependency node and attributes.
//
//	A plug is a point on a dependency node where a particular attribute can be
//	connected.  In simple cases the plug and attribute are equivalent.
//	When you have array-attributes,  however, the plug is more specific
//	in that it indicates which of the array-elements is to be connected.
//
//	There are two main types of plugs; networked plugs and non-networked plugs.
//	Non-networked plugs can be considered user plugs as they are
//	created by users and belong to users. Networked plugs can be considered
//	dependency node plugs as they are part of the dependency node and can
//	only be referenced by users.
//
//  In every dependency node there is a network or "tree" of plugs indicating
//  connections that have been made to attributes of the node. The plugs in 
//	this tree are known as {\bf networked} plugs as they belong to the
//	dependency node's network.
//
//  {\bf Non-networked} plugs are plugs that you can create in order to
//	establish a new connection to an attribute, or to set or get a value
//  on an attribute.  When a connection is made using a non-networked plug, a
//  networked version of the plug is created and added to the dependency
//  nodes network. 
//
//  A {\bf networked} plug cannot be explicitly created. They are created when
//  a connection is established for some attribute of the node.
//
//  All a {\bf non-networked} plug knows is how to uniquely describe the
//	attribute that it references, in fact the purpose of a non-networked
//	plug is to specify, without amibiguity, an attribute of a dependency node.
//	A non-networked plug contains an array of array indices that plot the
//	path from the root plug to this plug.
//
//  For simple attributes the plug and attribute are equivalent. Compound
//  attributes are also unambiguous. A plug that refers to an array
//  attribute, however, is more complex as it can refer to the {\bf array
//  plug} or an {\bf element plug} in the array.
//
//	Several methods are provided for navigating the plug tree. The child method
//  can be used to retrieve the {\bf child plugs} of a {\bf compound plug}. The
//  elementByLogicalIndex and elementByPhysicalIndex methods can be used to
//  retrieve the element plugs of an array plug. The parent and array methods
//  traverse the tree in the opposite direction: parent retrieves a compound
//  plug from a child plug, and array retrieves the array plug from an
//  element plug.
//
//	Since connections to arrays are sparse, element plugs have both logical
//  and physical indexes. The logical indexes are used by MEL, and are sparse.
//  Physical indexes, on the other hand, are not sparse. It is guaranteed that
//  the physical indexes will range from 0 to numElements() - 1. Using the
//  physical indexes, iterating over the element plugs in an array is easy:
//
//      for (i = 0; i < arrayPlug.numElements (); i++)
//      {
//          MPlug elementPlug = arrayPlug [i];
// 		    unsigned logicalIndex = elementPlug.logicalIndex();
//          // ...
//      }
//
//	This is equivalent to calling elementByPhysicalIndex since the bracket
//  operator uses physical indexes.
//
//	The ancestry of a plug is the tree above the plug: any parents or arrays
//	which this plug belongs to. The {\em selectAncestorLogicalIndex} method
//  provides quick access to element plugs without walking the plug tree.
//
// *****************************************************************************
//
#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTypes.h>
#include <maya/MDGContext.h>
#include <maya/MObject.h>
#include <maya/MIntArray.h>
#include <maya/MArrayDataBuilder.h>

// *****************************************************************************

// DECLARATIONS

class MString;
class MStringArray;
class MPlugArray;
class MTime;
class MPxData;
class MAngle;
class MDistance;
class MDataHandle;
class MDataBlock;

// *****************************************************************************

// CLASS DECLARATION (MPlug)

/// Create and Access dependency node plugs.
/**
  Methods for creating and accessing plugs and attributes.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MPlug  
{
public:
	///
	MPlug();
	///
	MPlug( const MPlug& in );
	///
	MPlug( const MObject & node, const MObject & attribute );
	///
	~MPlug();

	///
	MStatus     setAttribute (MObject &attribute);
	///
	MObject		attribute( MStatus* ReturnStatus = NULL ) const;
	///
	MObject 	node( MStatus* ReturnStatus = NULL ) const;
	///
	MString		name( MStatus* ReturnStatus = NULL ) const;
	///
	MString		partialName(
					bool			includeNodeName = false,
					bool			includeNonMandatoryIndices = false,
					bool			includeInstancedIndices = false,
					bool			useAlias = false,
					bool			useFullAttributePath = false,
					bool			useLongNames = false,
					MStatus*		ReturnStatus = NULL
				) const;
	//
	// Which values to generate setAttr commands for.
	//
	///
	enum MValueSelector {
		///
		kAll,
		///
		kNonDefault,
		///
		kChanged,
		///
		kLastAttrSelector
	};

	///
	MStatus		getSetAttrCmds(
					MStringArray&	cmds,
					MValueSelector	valueSelector = kAll,
					bool			useLongNames = false
				);

	///
	bool		isNetworked( MStatus* ReturnStatus = NULL ) const;
	///
	bool        isArray( MStatus* ReturnStatus = NULL ) const;
	///
	bool        isElement( MStatus* ReturnStatus = NULL) const;
	///
	bool        isCompound( MStatus* ReturnStatus = NULL ) const;
	///
	bool        isChild( MStatus* ReturnStatus = NULL ) const;
	///
	bool        isProcedural( MStatus* ReturnStatus = NULL ) const;
	///
	unsigned	logicalIndex( MStatus* ReturnStatus = NULL ) const;
	///
	MStatus	    selectAncestorLogicalIndex( unsigned index,
											const MObject &attribute =
											MObject::kNullObj);

	///
	unsigned    getExistingArrayAttributeIndices( MIntArray& indices, 
												  MStatus* ReturnStatus = NULL);

	///
	unsigned    numElements( MStatus* ReturnStatus = NULL ) const;
	///
	unsigned    evaluateNumElements( MStatus* ReturnStatus = NULL );
	///
	unsigned    numChildren( MStatus* ReturnStatus = NULL ) const;
	///
	unsigned    numConnectedElements( MStatus* ReturnStatus = NULL ) const;
	///
	unsigned    numConnectedChildren( MStatus* ReturnStatus = NULL ) const;
	///
	MPlug		child(	MObject& attr, MStatus* ReturnStatus = NULL ) const;
	///
	MPlug		child(	unsigned index, MStatus* ReturnStatus = NULL ) const;
	///
	MPlug		parent( MStatus* ReturnStatus = NULL ) const;
	///
	MPlug       array( MStatus* ReturnStatus = NULL ) const;
	///
	MPlug       elementByLogicalIndex( unsigned logicalIndex,
									   MStatus* ReturnStatus = NULL)
                const;
	///
	MPlug       elementByPhysicalIndex( unsigned physicalIndex,
										MStatus* ReturnStatus = NULL)
                const;
	///
	MPlug       connectionByPhysicalIndex( unsigned physicalIndex,
										MStatus* ReturnStatus = NULL)
                const;
	///
	bool		connectedTo( MPlugArray & array, bool asDst, bool asSrc,
							 MStatus* ReturnStatus = NULL ) const;
	///
	bool		isConnected( MStatus* ReturnStatus = NULL ) const;
	///
	bool        isKeyable( MStatus* ReturnStatus = NULL ) const;
	///
	MStatus     setKeyable( bool keyable );
	///
	bool        isLocked( MStatus* ReturnStatus = NULL ) const;
	///
	MStatus     setLocked( bool locked );
	///
	bool        isChannelBoxFlagSet( MStatus* ReturnStatus = NULL ) const;
	///
	MStatus     setChannelBox( bool channelBox );
	///
	bool        isCachingFlagSet( MStatus* ReturnStatus = NULL ) const;
	///
	MStatus     setCaching( bool caching );
	///
	bool		isNull( MStatus* ReturnStatus = NULL ) const;
	///
	MString 	info( MStatus* ReturnStatus = NULL ) const;
	///
	bool		isFromReferencedFile( MStatus* ReturnStatus = NULL ) const;
	///
	bool		isDynamic( MStatus* ReturnStatus = NULL ) const;

	//
	//	Enums for isFreeToChange
	//

	///
	enum FreeToChangeState {
		///	All tested plugs are free to change
		kFreeToChange = 0,
		/// Some of the tested plugs are not free to change
		kNotFreeToChange,
		///	Some of the children are not free to change
		kChildrenNotFreeToChange
	};
	///
	MPlug::FreeToChangeState isFreeToChange(bool checkParents = true,
									   bool checkChildren = true,
									   MStatus* ReturnStatus = NULL ) const;
	///
	MDataHandle     constructHandle(MDataBlock&) const;
	///
	void            destructHandle(MDataHandle&) const;
	
	// functions to get and set attribute values 
	///
	MStatus		getValue( MObject &val, MDGContext& ctx=MDGContext::fsNormal )
				const; 
	///
	MStatus		getValue( double&, MDGContext& ctx=MDGContext::fsNormal ) const;
	///
	MStatus		getValue( float&, MDGContext& ctx=MDGContext::fsNormal ) const;
	///
	MStatus		getValue( int&, MDGContext& ctx=MDGContext::fsNormal ) const;
	///
	MStatus		getValue( short&, MDGContext& ctx=MDGContext::fsNormal ) const;
	///
	MStatus		getValue( bool&, MDGContext& ctx=MDGContext::fsNormal ) const;
	///
	MStatus		getValue( MDistance&,
						  MDGContext& ctx=MDGContext::fsNormal ) const;
	///
	MStatus		getValue( MAngle&,
						  MDGContext& ctx=MDGContext::fsNormal ) const;
	///
	MStatus		getValue( MTime&, MDGContext& ctx=MDGContext::fsNormal ) const;
	///
	MStatus		getValue( char&, MDGContext& ctx=MDGContext::fsNormal ) const;
	///
	MStatus     getValue( MString&,
						  MDGContext& ctx=MDGContext::fsNormal ) const;
	///
	MStatus		setValue( MObject & val );
	///
	MStatus		setValue( MPxData * data );
	///
	MStatus		setValue( MDataHandle & handle );
	///
	MStatus		setValue( double );
	///
	MStatus		setValue( float );
	///
	MStatus		setValue( int );
	///
	MStatus		setValue( short );
	///
	MStatus		setValue( bool );
	///
	MStatus		setValue( MDistance& ); 
	///
	MStatus		setValue( MAngle& );
	///
	MStatus		setValue( MTime& );
	///
	MStatus		setValue( char );
	///
	MStatus     setValue( MString& );
	///
	MStatus     setValue( const char* );


	// Operators
	///
	MPlug&		operator =( const MPlug& other );
	///
	MPlug		operator[] ( MObject& attr ) const; // child(attr)
	///
	MPlug		operator[] ( unsigned physicalIndex ) const;	// index(index)
	///
	bool		operator!() const;						// false if valid
	///
	bool		operator ==( const MPlug &other ) const;
	///
	bool        operator ==( const MObject &other ) const;
	///
	bool        operator !=( const MPlug &other ) const;
	///
	bool        operator !=( const MObject &other ) const;
	///
	operator	MObject() const;					// attribute()
	///
	MStatus		setNumElements( unsigned );

protected:
// No protected members

private:
	const char*	 className() const;

	MPlug( const void*, bool );
	const void*	 fPlug;
	bool         ownPlug;
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MPlug */
