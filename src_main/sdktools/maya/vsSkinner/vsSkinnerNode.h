//======= Copyright © 1996-2006, Valve Corporation, All rights reserved. ======
//
// Purpose: Valve Maya Volume Skinning Node
//
//=============================================================================

#ifndef VSSKINNERNODE_H
#define VSSKINNERNODE_H
#if defined( _WIN32 )
#pragma once
#endif

// Maya includes
#include <maya/MDagModifier.h>
#include <maya/MFloatArray.h>


// Valve includes
#include "VsMayaMPxFactory.h"
#include "ValveMaya/Undo.h"
#include "tier1/utlvector.h"

//=============================================================================
//
//=============================================================================
class CVsSkinnerNode : public CVsMayaMPxShapeNode
{
public:
	CVsSkinnerNode();

	virtual ~CVsSkinnerNode();

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

	void AttachTransformToVolume(
		const MPlug &volumePlug,
		const MDagPath &transformPath,
		const MObject &skinnerBoneObj,
		const MDagPath &editTransformPath,
		const MDagPath &editShapePath,
		ValveMaya::CUndo &undo ) const;

	MObject NewVolume(
		double strength,
		double falloff,
		const MDagPath &parent,
		ValveMaya::CUndo &undo ) const;

	MStatus ScaleVolumeToJoint(
		MPlug &vP,
		ValveMaya::CUndo &undo ) const;

	MStatus AttachMesh(
		const MDagPath &meshPath,
		MDagModifier &mDagModifier );

	MStatus DetachMesh(
		MDagModifier &mDagModifier );

public:
	static MObject m_iaVolume;

	static MObject m_iaVolumeParentBindWorldMatrix;
	static MObject m_iaVolumeBoneWorldMatrix;
	static MObject m_iaVolumeWorldMatrix;
	static MObject m_iaStrength;
	static MObject m_iaFalloff;

	static MObject m_iaVolumeParentWorldMatrix;

	static MObject m_iaGeometryWorldMatrix;
	static MObject m_iaGeometry;

	static MObject m_caVolumeData;

	static MObject m_caGeometryWeights;

	static MObject m_oaGeometry;

	static MObject m_oaWeightList;
	static MObject m_oaWeights;

	enum
	{
		kMaxWeightPerVertex = 3
	};

protected:
	friend class CVsSkinnerIterator;
	friend class CVsSkinnerUI;

	void SyncVolumeData() const;

	void SyncGeometryWeights() const;

	MStatus ComputeVolumeData(
		MDataBlock &mDataBlock );

	MStatus ComputeGeometryWeights(
		MDataBlock &mDataBlock );

	MStatus ComputeGeometry(
		MDataBlock &mDataBlock );

	MStatus ComputeWeightList(
		MDataBlock &mDataBlock ) const;

	void DrawBoundingBox() const;

	struct VolumeData_s
	{
		uint m_jointLogicalIndex;
		uint m_volumeLogicalIndex;
		MMatrix m_worldMatrix;
		MPoint m_volumePosition;				// Copy of translation part of m_worldMatrix
		MMatrix m_geometryToVolumeMatrix;		// geometryWorldMatrix * m_worldMatrix.inverse
		double m_strength;
		double m_falloff;
		MBoundingBox m_bbox;
	};

	CUtlVector< VolumeData_s > m_volumeData;		// Mirrors m_iaVolume by physical index

	class CJointWeight
	{
	public:
		uint m_volumeLogicalIndex;
		uint m_jointLogicalIndex;
		float m_weight;

		CJointWeight()
		: m_volumeLogicalIndex( 0 )
		, m_jointLogicalIndex( 0 )
		, m_weight( 0.0f )
		{}
	};

	class CSkinWeight
	{
	public:
		uint m_requestedCount;	// Number of weights attempted to be added
		uint m_count;			// Number of weights added in total
		CJointWeight m_weights[ kMaxWeightPerVertex ];

		CSkinWeight()
		: m_requestedCount( 0 )
		, m_count( 0 )
		{}

		void Reset()
		{
			m_requestedCount = 0;
			m_count = 0;
		}

		void AddWeight(
			uint volumeLogicalIndex,
			uint jointLogicalIndex,
			float weight );

		float TotalWeight() const
		{
			float totalWeight( 0.0f );
			for ( uint i( 0 ); i != m_count; ++i )
			{
				totalWeight += m_weights[ i ].m_weight;
			}

			return totalWeight;
		}
	};

	CUtlVector< CSkinWeight > m_skinWeights;

	MBoundingBox m_wbbox;	// World Space Bounding Box
};

#endif // VSSKINNERNODE_H
