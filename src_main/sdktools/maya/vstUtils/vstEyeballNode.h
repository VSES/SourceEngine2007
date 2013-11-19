//======= Copyright © 1996-2006, Valve Corporation, All rights reserved. ======
//
// Purpose: Valve Maya Volume Skinning Node
//
//=============================================================================

#ifndef VSTEYEBALLNODE_H
#define VSTEYEBALLNODE_H
#if defined( _WIN32 )
#pragma once
#endif

// Maya includes
#include <maya/MDagModifier.h>
#include <maya/MFloatArray.h>


// OpenGL
#include <gl/GL.h>


// Valve includes
#include "VsMayaMPxFactory.h"
#include "ValveMaya/Undo.h"
#include "tier1/utlvector.h"


//=============================================================================
//
//=============================================================================
class CUnitSphere
{
public:
	CUnitSphere();

	~CUnitSphere();

	void Draw( M3dView &m3dView );

	void Draw( M3dView &m3dView ) const { const_cast< CUnitSphere * >( this )->Draw( m3dView ); }

	double GetRadius() const { return m_dRadius; }
	void SetRadius( double val ) { if ( m_dRadius != val ) { m_dRadius = val; m_bRebuildSphere = true; } }

	int GetSlices() const { return m_nSlices; }
	void SetSlices( int val ) { if ( m_nSlices != val ) { m_nSlices = val; m_bRebuildSphere = true; } }

	int GetStacks() const { return m_nStacks; }
	void SetStacks( int val ) { if ( m_nStacks != val ) { m_nStacks = val;  m_bRebuildSphere = true; } }

	double GetLatBegin() const { return m_dLatBegin; }
	void SetLatBegin(double val) { if ( m_dLatBegin != val ) { m_dLatBegin = clamp( val, 0.0, m_dLatEnd );  m_bRebuildSphere = true; } }

	double GetLatEnd() const { return m_dLatEnd; }
	void SetLatEnd(double val) { if ( m_dLatEnd != val ) { m_dLatEnd = clamp( val, m_dLatBegin, 1.0 );  m_bRebuildSphere = true; } }

protected:
	bool m_bRebuildSphere;

	double m_dRadius;
	int m_nSlices;				// Longitude Spans
	int m_nStacks;				// Latitude Spans

	HDC m_hdc;					// Handle to device context
	HGLRC m_hglrc;				// Handle to OpenGL rendering context
	GLuint m_displayList;		// OpenGL Display List

	double m_dLatBegin;			// Latitude Begin [ 0, 1 ]
	double m_dLatEnd;			// Latitude End [ 0, 1 ]
};


//=============================================================================
//
//=============================================================================
class CVstEyeballNode : public CVsMayaMPxShapeNode
{
public:
	CVstEyeballNode();

	virtual ~CVstEyeballNode();

	virtual void postConstructor();

	static MStatus Initialize();

	virtual MStatus compute(
		const MPlug &mPlug,
		MDataBlock &mDataBlock );

	virtual bool isBounded() const { return true; }

	virtual MBoundingBox boundingBox() const;

	virtual bool match(
		const MSelectionMask &mask,
		const MObjectArray &cList ) const;

	virtual MatchResult matchComponent(
		const MSelectionList &item,
		const MAttributeSpecArray &specArray,
		MSelectionList &list );

public:
	// New functions

public:
	// Attributes

	static MObject m_iaDiameter;	// Diameter of the ball of the eye
	static MObject m_iaAngle;		// Yaw Angle
	static MObject m_iaIrisScale;	// Iris Scale
	static MObject m_iaPupilScale;	// Pupil Scale

	static MObject m_iaStacks;		// Sphere Stacks
	static MObject m_iaSlices;		// Sphere Slices

	static MObject m_iaDrawEyeball;	// Draw the eyeball
	static MObject m_iaDrawIris;	// Draw the iris
	static MObject m_iaDrawPupil;	// Draw the pupil

	static MObject m_caSyncAttrs;	// Compute attribute to syncronize attributes

protected:
	friend class CVstEyeballUI;

	void SyncAttrs() const;

	CUnitSphere m_eyeball;
	CUnitSphere m_iris;
	CUnitSphere m_pupil;
};

#endif // VSTEYEBALLNODE_H
