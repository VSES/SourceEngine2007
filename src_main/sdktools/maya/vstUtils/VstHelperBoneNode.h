//======= Copyright © 1996-2006, Valve Corporation, All rights reserved. ======
//
// Purpose: VstHelperBone declaration
//
//=============================================================================

#ifndef VSTHELPERBONENODE_H
#define VSTHELPERBONENODE_H
#if defined( _WIN32 )
#pragma once
#endif


// Maya includes
#include <maya/MBoundingBox.h>
#include <maya/MFloatVector.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MMatrix.h>
#include <maya/MPlug.h>
#include <maya/MQuaternion.h>
#include <maya/MString.h>


// Valve includes
#include "VsMayaMPxFactory.h"
#include "tier1/utlvector.h"
#include "mathlib/mathlib.h"


//=============================================================================
//
//=============================================================================
class CHelperBoneTrigger
{
public:
	bool m_draw;
	float m_tolerance;
	float m_invTolerance;	// 1.0 / tolerance
	MQuaternion m_trigger;	// Angle to match
	MFloatVector m_pos;
	MQuaternion m_quat;
	double m_weight;
	MString m_name;
};


//=============================================================================
//
//=============================================================================
class CVstHelperBoneNode : public CVsMayaMPxShapeNode
{
public:
	CVstHelperBoneNode();

	virtual ~CVstHelperBoneNode();

	virtual void postConstructor();

	static MStatus Initialize();

	virtual MStatus compute(
		const MPlug &mPlug,
		MDataBlock &mDataBlock );

	bool isBounded() const { return true; }

	MBoundingBox boundingBox() const;

	MStatus DoExport(
		std::ostream &os,
		const MString &addPrefix,
		const MString &delPrefix );

public:
	static MObject m_iaControlRotateAxis;

	static MObject m_iaControlJointOrient;

	static MObject m_iaControlParentWorldSpace;

	static MObject m_iaControlWorldSpace;

	static MObject m_iaTriggerDrawAxis;

	static MObject m_iaBaseTranslate;
	static MObject m_iaBaseRotate;

	static MObject m_caMatrices;

	static MObject m_iaMaxRadius;

	static MObject m_iaTrigger;
	static MObject m_iaTriggerDraw;
	static MObject m_iaTriggerTolerance;
	static MObject m_iaTriggerAxis;
	static MObject m_iaTriggerTranslate;
	static MObject m_iaTriggerRotate;
	static MObject m_iaTriggerName;

	static MObject m_caTriggerData;

	static MObject m_oaTranslate;
	static MObject m_oaTranslateX;
	static MObject m_oaTranslateY;
	static MObject m_oaTranslateZ;

	static MObject m_oaRotate;
	static MObject m_oaRotateX;
	static MObject m_oaRotateY;
	static MObject m_oaRotateZ;

	static MObject m_iaSubdivisons;

	static MObject m_oaNumTriangles;

	static MObject m_iaDrawWireframe;
	static MObject m_iaWireframeOnShaded;

	enum DrawWireframe
	{
		kNone = 0,
		kBack = 1,
		kFront = 2,
		kBoth
	};

protected:
	friend class CVstHelperBoneUI;

	float ComputeTriggerWeights( const Quaternion &controlQuat );

	void DrawBoundingBox() const;

	MStatus ComputeConeData(
		MDataBlock &mDataBlock );

	void DrawCone(
		float halfConeAngle,
		float sphereRadius,
		float weight ) const;

	bool WireframeOnShaded() const;

	MFloatVectorArray m_unitCirclePoints;
	MFloatVectorArray m_unitNormalPoints;

	CUtlVector< CHelperBoneTrigger > m_triggers;

	double m_maxRadius;

	MMatrix m_parentRotateMatrix;
	MMatrix m_triggerWorldMatrix;
	MMatrix m_triggerWorldTranslateMatrix;
	MMatrix m_triggerLocalMatrix;
	Quaternion m_triggerLocalMatrixQuat;
	MVector m_baseTranslate;
	MVector m_baseRotate;
	MQuaternion m_baseRotateQuat;
	MVector m_triggerDrawAxis;

	MBoundingBox m_wbbox;	// World Space Bounding Box
};

#endif // VSTHELPERBONENODE_H
