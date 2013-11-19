//======= Copyright © 1996-2006, Valve Corporation, All rights reserved. ======
//
// Purpose: Declaration of CSmdImport
//
//=============================================================================


#ifndef SMDIMPORT_H
#define SMDIMPORT_H
#if defined( _WIN32 )
#pragma once
#endif

// Standard includes
#include <deque>
#include <map>
#include <list>
#include <string>
#include <set>


// Maya includes
#include <maya/MDagModifier.h>
#include <maya/MDagPath.h>
#include <maya/MFloatVector.h>
#include <maya/MString.h>
#include <maya/MTransformationMatrix.h>


// Valve includes
#include "valveMaya.h"
#include "valveMaya/Undo.h"
#include "tier1/utlvector.h"


//=============================================================================
//
//=============================================================================
class CQcData
{
public:
	CQcData()
	: m_upAxis( 2U )
	, m_scale( 1.0f )
	{}

	bool ParseQc( const std::string &smdPath, const std::string &qcPath );

	bool GetQcData( const std::string &smdPath );

	uint m_upAxis;
	float m_scale;
	std::list< std::string > m_cdmaterials;
};


//=============================================================================
//
//=============================================================================
class CUv
{
public:
	CUv()
	: u( 0.0f )
	, v( 0.0f )
	{}

	CUv(
		float i_u,
		float i_v )
	: u( i_u )
	, v( i_v )
	{}

	CUv(
		const CUv &rhs )
	: u( rhs.u )
	, v( rhs.v )
	{
	}

	inline CUv &operator=(
		const CUv &rhs )
	{
		u = rhs.u;
		v = rhs.v;
		return *this;
	}

	inline bool operator==(
		const CUv &rhs ) const
	{
		return( u == rhs.u && v == rhs.v );
	}

	inline bool operator!=(
		const CUv &rhs ) const
	{
		return( u != rhs.u || v != rhs.v );
	}

	float	u;
	float	v;
};


//=============================================================================
//
//=============================================================================
class CShadingGroup
{
public:
	std::string m_materialPath;
	std::map< int, std::deque< int > > m_componentListMap;
};


//=============================================================================
//
//=============================================================================
class CVertexWeights
{
public:
	enum { kMaxWeights = 3 };

	CVertexWeights()
	: m_nWeights( 0 )
	, m_weightTotal( 0.0 )
	{
	}

	int Count() const
	{
		return m_nWeights;
	}

	int InfluenceIndex(
		const int index ) const
	{
		Assert( index < kMaxWeights );
		return m_influenceIndices[ index ];
	}

	double NormalizedWeight(
		const int index ) const
	{
		Assert( index < kMaxWeights );
		Assert( m_weightTotal > 0.0 );
		return m_weights[ index ] / m_weightTotal;
	}

	bool AddWeight(
		int vertexIndex,
		int influenceIndex,
		double weight )
	{
		// Check to ensure the vertex isn't attempting to be weighted to the same bone with two different weights
		for ( int i( 0 ); i < m_nWeights; ++i )
		{
			if ( m_influenceIndices[ i ] == influenceIndex )
			{
				if ( m_weights[ i ] != weight )
				{
					merr << "Vertex " << vertexIndex << " is weighted to the same bone with two different weights, ignoring second weight" << std::endl;
					merr << " + Bone: " << influenceIndex << " Weight: " << m_weights[ i ] << " & Weight: " << weight << std::endl;
					return false;
				}
				return true;
			}
		}

		// Check to ensure the vertex isn't being given too many weights
		if ( m_nWeights >= kMaxWeights )
		{
			merr << "Too many weights on vertex, " << vertexIndex << ", ignoring Bone: " << influenceIndex << " Weight: " << weight << std::endl;
			return false;
		}

		m_influenceIndices[ m_nWeights ] = influenceIndex;
		m_weights[ m_nWeights ] = weight;
		m_weightTotal += weight;
		++m_nWeights;

		return true;
	}

protected:
	int m_nWeights;
	int m_influenceIndices[ kMaxWeights ];
	double m_weights[ kMaxWeights ];
	double m_weightTotal;
};


//=============================================================================
//
//=============================================================================
class CPolygonData
{
public:
	std::deque< int > m_polygonCounts;
	std::deque< int > m_polygonConnects;
	std::deque< int > m_uvIds;
	std::deque< MFloatVector > m_vertexArray;
	std::deque< MFloatVector > m_normalArray;
	std::deque< CUv > m_uvArray;
	MDagPath m_mDagPath;
	std::set< int > m_influenceObjects;
	CUtlVector< CVertexWeights > m_weightList;

	CPolygonData()
	{
	}

	CPolygonData( const CPolygonData &rhs )
	{
		m_polygonCounts = rhs.m_polygonCounts;
		m_polygonConnects = rhs.m_polygonConnects;
		m_uvIds = rhs.m_uvIds;
		m_vertexArray = rhs.m_vertexArray;
		m_normalArray = rhs.m_normalArray;
		m_uvArray = rhs.m_uvArray;
		m_mDagPath = rhs.m_mDagPath;
		m_influenceObjects = rhs.m_influenceObjects;
		m_weightList = rhs.m_weightList;
	}
};


//=============================================================================
//
//=============================================================================
class CSmdImport
{
public:
	CSmdImport(
		const std::string &game = "",
		const std::string &textureArchive = "" );

	MDagPath DoIt(
		const std::string &smdPath,
		CQcData &qcData,
		const MTransformationMatrix &topLevelMat,
		ValveMaya::CUndo &undo );

	void SetNodeDelPrefix(
		const MString &nodePrefix );

	void SetNodeAddPrefix(
		const MString &nodePrefix );

	MStatus ImportAnimation(
		const std::string &i_smdPath,
		const MDagPath &rootDagPath,
		CQcData &qcData,
		ValveMaya::CUndo &undo );

	bool m_optImportSkeleton;
	bool m_optAutoStripPrefix;
	bool m_optImportAnimation;

protected:

	MObject CreateMesh(
		const MObject &parentObj,
		const std::string &smdPath,
		const MString &name,
		CPolygonData &polygonData,
		MSelectionList &rootNodeList,
		MDagModifier &mDagModifier );

	class CNodeData
	{
	public:
		CNodeData()
		: m_parentIndex( -1 )
		, m_skinned( false )
		, m_influenceIndex( 0 )
		{
		}

		bool Valid() const
		{
			return m_dagPath.isValid() && m_dagPath.length();
		}

		void Reset()
		{
			m_dagPath = MDagPath();
		}

		int m_parentIndex;
		bool m_skinned;
		int m_influenceIndex;
		MDagPath m_dagPath;

		CUtlVector< MFloatVector > m_positions;
		CUtlVector< MFloatVector > m_orientations;
	};

	static void PurgeUnusedBones( 
		CUtlVector< CNodeData > &nodeList,
		MSelectionList &rootNodeList,
		MDagModifier &mDagModifier );

	static MObject SkinMesh(
		const MDagPath &origMeshDagPath,
		ValveMaya::CUndo &undo );

	static void SkinMeshes(
		CUtlVector< CNodeData > &nodeList,
		std::map< int, CPolygonData > &polygonDataMap,
		ValveMaya::CUndo &undo );

	void CreateShadingGroups(
		const CQcData &qcData,
		const std::string &smdPath,
		const std::map< int, CPolygonData > &polygonDataMap,
		const std::map< std::string, CShadingGroup > &shadingGroupMap,
		ValveMaya::CUndo &undo );

	std::string TexturePathFromMaterialPath(
		const CQcData &qcData,
		const std::string &smdPath,
		const std::string &materialPath,
		const std::string &textureType = "$basetexture" );

	std::string TargaPathFromTexturePath(
		const CQcData &qcData,
		const std::string &smdPath,
		const std::string &texturePath );

	std::string GetSmdPath(
		const std::string &i_smdPath ) const;

	std::string ArchiveTexture(
		const std::string &tgaPath );

	static bool ParserIsBlankLine( const char *pBuf );

	static bool ParserHandleVersion( const char *pBuf, int *version = NULL );

	void ParserGetNodeName( const char *pBuf, MString &name ) const;

	bool ParserHandleSkeletonLine( const char *pBuf, int &nId, MString &name, int &pId ) const;

	static bool ParserHandleSample( const char *pBuf, CUtlVector< CNodeData > &nodeList );

	static void CreateAnimationCurves( const CUtlVector< int > &times, const CUtlVector< CNodeData > &nodeList, ValveMaya::CUndo &undo );

	std::string m_vproject;
	std::list< std::string > m_gamePath;
	std::list< std::string > m_contentPath;

	std::string m_textureArchive;

	std::map< std::string, std::string > m_materialPathToTexturePathCache;
	std::map< std::string, std::string > m_texturePathToTargaPathCache;

	MString m_nodeAddPrefix;
	MString m_nodeDelPrefix;
};

#endif // SMDIMPORT_H
