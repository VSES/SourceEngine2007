
#ifndef _MPxDeformerNode
#define _MPxDeformerNode
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
// CLASS:    MPxDeformerNode
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MPxDeformerNode)
//
// MPxDeformerNode allows the creation of user-defined deformers.  A deformer
// is a node which takes any number of input geometries, deforms them and
// places the output into the output geometry attribute.
//
// For more information, please see the built-in documentation
// in MPxDeformerNode.cpp.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES



#include <maya/MStatus.h>
#include <maya/MTypes.h>
#include <maya/MObject.h>
#include <maya/MPxNode.h>
#include <maya/MSelectionList.h>


// *****************************************************************************

// DECLARATIONS

// *****************************************************************************

// CLASS DECLARATION (MPxDeformerNode)

class MItGeometry;
class MDagModifier;

/// Base class for user defined Deformers (OpenMayaAnim)
/**
  Create user defined Deformers.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAANIM_EXPORT MPxDeformerNode : public MPxNode
{
public:
    ///
	MPxDeformerNode();

    ///
	virtual ~MPxDeformerNode();

	///
	virtual MPxNode::Type type() const;

	// Methods to overload

	// deform is called by computePlug when an output geometry
	// value is evaluated
	//

	///
    virtual MStatus        deform(MDataBlock& block,
								  MItGeometry& iter,
								  const MMatrix& mat,
								  unsigned int multiIndex);

	// return the attribute that gets connected to the
	// deformer tool shape
	//
	///
	virtual MObject&    	accessoryAttribute() const;

	// called at creation time so that the node can create and attach any
	// that it needs in order to function
	//
	///
	virtual MStatus			accessoryNodeSetup(MDagModifier& cmd);

	// return the weight value for the given index pair
	//
	///
	float						weightValue( MDataBlock& mblock,
											 unsigned int multiIndex,
											 unsigned int wtIndex);
	
	///
	void					setUseExistingConnectionWhenSetEditing(bool state);

	// called when the set being deformed by this deformer has been modified to
	// add/remove this selection list.
	//
	///
	virtual void			setModifiedCallback( MSelectionList& list,
												 bool listAdded );
	
	// Inherited attributes
	/// input attribute, multi
	static MObject input;
	/// input geometry attribute
		static MObject inputGeom;
	/// input group id attribute
		static MObject groupId;
	/// geometry output attribute
	static MObject outputGeom;
	/// weight list attribute, multi
	static MObject weightList;
	/// weight attribute, multi
		static MObject weights;
	/// envelope attribute
	static MObject envelope;
protected:
// No protected members

private:
	static void				initialSetup();
	static const char*	    className();
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MPxNode */
