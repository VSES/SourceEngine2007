
#ifndef _MPxFieldNode
#define _MPxFieldNode
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
// CLASS:    MPxFieldNode
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MPxFieldNode)
//
// MPxFieldNode allows the creation and manipulation of dependency graph nodes
// representing fields.  This is the top level of a hierarchy of field
// node function sets.  It permits manipulation of the attributes common to
// all types of fields.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MObject.h>
#include <maya/MPxNode.h>
#include <maya/MPointArray.h>
#include <maya/MVectorArray.h>
#include <maya/MDoubleArray.h>
#include <maya/M3dView.h>
#if defined(OSMac_MachO_)
#	include <OpenGL/gl.h>
#else
#	include <GL/gl.h>
#endif

// *****************************************************************************

// DECLARATIONS

// *****************************************************************************

// CLASS DECLARATION (MPxFieldNode)

/// Base class for user defined fields (OpenMayaFX)
/**
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAFX_EXPORT MPxFieldNode : public MPxNode
{
public:
	///
	MPxFieldNode();
	///
	virtual ~MPxFieldNode();
	///
	virtual MPxNode::Type type() const;
	///
	virtual MStatus		compute( const MPlug& plug, MDataBlock& dataBlock );
	///
    virtual MStatus getForceAtPoint(const MVectorArray&  point,
                            const MVectorArray&  velocity,
                            const MDoubleArray&  mass,
                            MVectorArray&        force,
                            double deltaTime);
	///
	virtual MStatus iconSizeAndOrigin(	GLuint& width,
						GLuint& height,
						GLuint& xbo,
						GLuint& ybo   );
	///
	virtual MStatus iconBitmap(GLubyte* bitmap);

	virtual void    draw(  M3dView  & view, const  MDagPath  & path,  M3dView::DisplayStyle  style, M3dView:: DisplayStatus );

	virtual double  falloffCurve(const double param, MStatus *ReturnStatus = NULL);
	virtual bool    isFalloffCurveConstantOne(MStatus *ReturnStatus = NULL);

	/////////////////////////
	// Inherited attributes
	/////////////////////////

	// General attributes for every field.
	//
	/// magnitude attribute
	static MObject	mMagnitude;
	/// attenuation attribute
	static MObject	mAttenuation;
	/// maximum distance attribute
	static MObject	mMaxDistance;
	/// flag for maximum distance attribute
	static MObject	mUseMaxDistance;
	/// flag for per vertex attribute
	static MObject	mApplyPerVertex;

	// Attributes for input force data.
	//
	/// input attribute compound, multi
	static MObject	mInputData;
	/// child attribute, multi
	static MObject	mInputPositions;
	/// child attribute, multi
	static MObject	mInputVelocities;
	/// child attribute, multi
	static MObject	mInputMass;
	/// child attribute
	static MObject	mDeltaTime;

	// Attribute for input forces.
	//
	/// forces input attribute, multi
	static MObject	mInputForce;

	// Attribute for output forces.
	//
	/// forces output attribute, multi
	static MObject	mOutputForce;

	// Attributes for ownership.
	//
	/// X component of mOwnerCentroid
	static MObject	mOwnerCentroidX;
	/// Y component of mOwnerCentroid
	static MObject	mOwnerCentroidY;
	/// Z component of mOwnerCentroid
	static MObject	mOwnerCentroidZ;
	/// owner centroid attribute
	static MObject	mOwnerCentroid;
	/// owner position attribute, multi
	static MObject	mOwnerPosData;
	/// owner velocity attribute, multi
	static MObject	mOwnerVelData;

	// Attribute for world position.
	//
	/// world matrix attribute
	static MObject	mWorldMatrix;

	/// Attribute for input pp data from particleShape
	static MObject  mInputPPData;

	/// Attribute for input pp data from owner particleShape
	static MObject  mOwnerPPData;

protected:
// No protected members

private:
// No private members

	static void			initialSetup();
	static const char*	className();


};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32


// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MPxFieldNode */
