
#ifndef _MPxNode
#define _MPxNode
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
// CLASS:    MPxNode
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MPxNode)
//
// MPxNode is the the parent class for user defined
// dependency nodes.  A dependency node is an object that resides in the 
// dependency graph.  It computes output attributes based on a set of input 
// attributes.  When an input changes, the compute method is called for each
// dependent output.
//
// The dependency graph is made up of nodes that have connections between  
// their attributes.  When an attribute changes, recomputation propagates
// through the graph until all affected values have been updated.
//
// When writing a dependency node, there is a very basic rule that should
// be observed.  The outputs should be calculated only using the values of
// the inputs.  All information about the world outside the node should 
// come from input attributes.  If this rule is not observed, then 
// the results may be unpredictable.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES



#include <maya/MStatus.h>
#include <maya/MTypes.h>
#include <maya/MObject.h>

#include <maya/MTypeId.h>
#include <maya/MDataHandle.h>
#include <maya/MDataBlock.h>
#include <maya/MPlug.h>
#include <maya/MString.h>
#include <maya/MDGContext.h>

// *****************************************************************************

// DECLARATIONS

// *****************************************************************************

// CLASS DECLARATION (MPxNode)

/// Base class for user defined dependency nodes
/**
  Create user defined dependency nodes.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class	MPlugArray;

class OPENMAYA_EXPORT MPxNode  
{
public:
    ///
	enum Type {
        ///
        kDependNode,
        ///
		kLocatorNode,
		///
		kDeformerNode,
		///
		kManipContainer,
		///
		kSurfaceShape,
		///
		kFieldNode,
		///
		kEmitterNode,
		///
		kSpringNode,
		///
		kIkSolverNode,
		///
		kHwShaderNode,
		///
		kTransformNode,
		///
		kObjectSet,
		///
		kFluidEmitterNode,
		///
		kLast
    }; 

    ///
	MPxNode();
    ///
	virtual ~MPxNode();
	///
	virtual void			postConstructor();
    ///
	virtual MStatus			compute( const MPlug& plug,
									 MDataBlock& dataBlock );

	///
	virtual bool			getInternalValueInContext( const MPlug&,
											  MDataHandle&,
											  MDGContext&);
	///
    virtual bool			setInternalValueInContext( const MPlug&,
											  const MDataHandle&,
											  MDGContext&);

	///
	virtual bool			getInternalValue( const MPlug&,
											  MDataHandle& );
	///
    virtual bool			setInternalValue( const MPlug&,
											  const MDataHandle&);
	
	///
    virtual void			copyInternalData( MPxNode* );
	
	///
	virtual MStatus			legalConnection( const MPlug& plug,
											 const MPlug& otherPlug,
											 bool asSrc,
											 bool& result ) const;
	///
	virtual MStatus			legalDisconnection( const MPlug& plug,
											 const MPlug& otherPlug,
											 bool asSrc,
											 bool& result ) const;
	///
	virtual MStatus			setDependentsDirty( const MPlug &, MPlugArray & );

	///
	virtual MStatus			connectionMade( const MPlug& plug,
											 const MPlug& otherPlug,
											 bool asSrc );
	///
	virtual MStatus			connectionBroken( const MPlug& plug,
											 const MPlug& otherPlug,
											 bool asSrc );
    ///
	virtual MStatus			shouldSave( const MPlug& plug, bool& result );

    ///
	MTypeId					typeId() const;
    ///
	MString					typeName() const;
    ///
	MString					name() const; 
	///                     
	virtual Type            type() const;
	///                     
	virtual bool			isAbstractClass  () const;
	///
	MObject                 thisMObject() const;

	///
	static MStatus          addAttribute( const MObject & attr );
    ///
	static MStatus		    inheritAttributesFrom(
											const MString & parentClassName );
    ///
	static MStatus          attributeAffects( const MObject & whenChanges,
											  const MObject & isAffected );

	///
	MStatus					setExistWithoutInConnections( bool flag );
	///
	bool					existWithoutInConnections(
								MStatus* ReturnStatus = NULL ) const;
	///
	MStatus					setExistWithoutOutConnections( bool flag );
	///
	bool					existWithoutOutConnections(
								MStatus* ReturnStatus = NULL ) const;


	/// message attribute
	static MObject          message;
	/// is historically interesting attribute
	static MObject          isHistoricallyInteresting;
	/// caching attribute
	static MObject          caching;
	/// state attribute
	static MObject          state;

protected:
	///
	MDataBlock				forceCache( MDGContext& ctx=MDGContext::fsNormal );

	///
	void					setMPSafe ( bool flag );

	void*					instance;
	  
private:
	static void				initialSetup();
	static const char*	    className();
	static void*            initClass;
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MPxNode */
