//======= Copyright © 1996-2006, Valve Corporation, All rights reserved. ======
//
// Implementation of CSmdExport
//
//=============================================================================

#ifndef SMDEXPORT_H
#define SMDEXPORT_H
#ifdef _WIN32
#pragma once
#if _MSC_VER >= 1300
#pragma warning(disable : 4511)	// warning C4511: copy constructor could not be generated
#pragma warning(disable : 4512)	// warning C4512: assignment operator could not be generated
								// When using std::map
#pragma warning(disable : 4702)	// c:\program files\microsoft visual studio .net 2003\vc7\include\xtree(1116): warning C4702: unreachable code
#endif // _MSC_VER >= 1300
#endif // WIN32

// Standard includes
#include <fstream>
#include <list>
#include <map>
#include <string>
#include <vector>

// Maya includes
#include <maya/MDagPath.h>
#include <maya/MDagPathArray.h>
#include <maya/MFnMesh.h>
#include <maya/MFnSkinCluster.h>
#include <maya/MFnSingleIndexedComponent.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MItMeshVertex.h>
#include <maya/MObjectArray.h>
#include <maya/MStatus.h>
#include <maya/MString.h>
#include <maya/MStringArray.h>
#include <maya/MSelectionList.h>
#include <maya/MMatrix.h>


// Forward declarations
class CVProjectUtil;

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
class CMaterial
{
public:
	CMaterial()
		: m_transparent( false )
	{}

	CMaterial(
		const std::string &path,
		const std::string &name,
		bool transparent )
	: m_path( path )
	, m_name( name )
	, m_transparent( transparent )
	{}

	std::string m_path;
	std::string m_name;
	bool m_transparent;
};


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
class CDagData
{
public:
	CDagData(
		int index,
		const MDagPath &mDagPath,
		const CDagData *pDagParent,
		const bool bBindCurrent);

	int Index() const
	{
		return m_index;
	}

	MString Name() const
	{
		return MFnDagNode( m_mDagPath ).name();
	}

	bool HasParent() const
	{
		return( m_pDagParent != NULL );
	}

	int ParentIndex() const
	{
		return m_pDagParent ? m_pDagParent->Index() : -1;
	}

	MMatrix LocalMatrix() const
	{
		return MFnDagNode( m_mDagPath ).transformationMatrix();
	}

	MMatrix InclusiveMatrix() const
	{
		return m_mDagPath.inclusiveMatrix();
	}

	const MMatrix &BindMatrix() const
	{
		return m_bindMatrix;
	}

	const MMatrix &WorldBindMatrix() const
	{
		return m_worldBindMatrix;
	}

protected:
	const int m_index;					// SMD Index
	const MDagPath m_mDagPath;			// Maya Dag path to node
	const CDagData *m_pDagParent;		// Pointer to parent node

	const MMatrix m_bindMatrix;			// Local space bind matrix
	const MMatrix m_worldBindMatrix;	// World space bind matrix
	MDagPathArray m_bboxes;				// Bounding boxes of things to stay near the centre of
};


class CMeshData
{
public:
	CMeshData(
		const MDagPath &mDagPath,
		const MDagPath &baseDagPath,
		const MObject &skinObj,
		const MObject &blendObj,
		const bool bBindCurrent );

	const MDagPath &DagPath() const
	{
		return m_mDagPath;
	}

	const MDagPath &BaseDagPath() const
	{
		return m_baseDagPath;
	}

	const MMatrix &WorldBindMatrix() const
	{
		return m_worldBindMatrix;
	}

	const MObject &SkinObj() const
	{
		return m_skinObj;
	}

protected:
	const MDagPath m_mDagPath;			// Maya Dag path to node
	const MDagPath m_baseDagPath;		// The mesh before skin/blend deformers

	const MMatrix m_worldBindMatrix;	// World space bind matrix

	const MObject m_skinObj;			// Skin deformer
	const MObject m_blendObj;			// Blend shape deformer
	const MObject m_latticeObj;			// Lattice deformer
};


//-----------------------------------------------------------------------------
// Class to do exporting of Maya data
//-----------------------------------------------------------------------------
class CSmdExport
{
public:
	CSmdExport(
		uint type,
		uint options,
		uint version );

	~CSmdExport();

	MStatus DoIt(
		const MString &filename,
		const MSelectionList &exportList,
		MStringArray &result );

	enum Type	// NOTE: Bitmask flags that can be or'd together
	{
		kReference				= 1 << 0,
		kModel					= 1 << 0,

		kPhysModel				= 1 << 1,

		kAnimation				= 1 << 2,
		kSkeletalAnimation		= 1 << 2,

		kVTA					= 1 << 3,
		kVertexAnimation		= 1 << 3
	};

	enum Flags
	{
		kOptExportInvisible			= 1 << 0,
		kOptXUp						= 1 << 1,
		kOptYUp						= 1 << 2,
		kOptZUp						= 1 << 3,
		kOptPrettyPrint				= 1 << 4,
		kOptWorld					= 1 << 5,
		kOptDoubleSided				= 1 << 6,
		kOptMaterialSrc				= 1 << 7,
		kOptMaterialVtf				= 1 << 8,
		kOptMaterialVmt				= 1 << 9,
		kOptMaterials				= 1 << 7 | 1 << 8 | 1 << 9,	// All three!
		kOptQc						= 1 << 10,
		kOptConfirm					= 1 << 11,
		kOptRelativeMaterials		= 1 << 12,
		kOptParents					= 1 << 13,
		kOptQcCollapseBones			= 1 << 14,
		kOptQcCenterBonesOnVerts	= 1 << 15,
		kOptQcSkipBoneInBBox		= 1 << 16,
		kOptRunStudiomdl			= 1 << 17,
		kOptExportAttachments		= 1 << 18,
		kOptBindCurrent				= 1 << 19
	};

	void SetNodeDelPrefix(
		const MString &nodePrefix );

	void SetNodeAddPrefix(
		const MString &nodePrefix );

	void SetFrameRange(
		double fs,
		double fe,
		double fi );

protected:
	void OutputVmts(
		CVProjectUtil &vp,
		MStringArray &result );

	void CreateExportLists(
		const MSelectionList &masterExportList );

	uint PreprocessDag(
		const MDagPath &mDagPath,
		bool force );

	uint AddPreprocessedDag(
		const MDagPath &mDagPath );

	void PreprocessMesh(
		const MDagPath &mDagPath );

	uint AddPreprocessedMesh(
		const MDagPath &mDagPath,
		const MObject &skinObj,
		const MObject &blendObj,
		const MObject &latticeObj );

	void PreprocessSkinCluster(
		const MObject &skinObj );

	void OutputExportData();

	void OutputSkeletalAnimation();

	void OutputBindSkeleton();

	void OutputDag(
		const CDagData *pDagData );

	const CDagData *GetParent(
		const MDagPath &mDagPath ) const;

	int GetParentIndex(
		const MDagPath &mDagPath ) const;

	void OutputMesh(
		const CMeshData *pMeshData );

	void OutputSkinnedFaceSet(
		const MDagPath &mDagPath,
		const MFnMesh &meshFn,
		const MMatrix &xformMat,
		bool doubleSided,
		const MObject &fileObj,
		const MObject &place2dTextureObj,
		bool transparent,
		const CMeshData *pMeshData,
		int parentIndex,
		const MFnSkinCluster &skinFn,
		size_t weightsPerVertex,
		size_t validInfluenceCount,
		const std::vector< uint > &validInfluenceIndices,
		const std::vector< int > &influenceNodeIndices,
		MObject &faceSetObj,
		const std::string &targaName );

	void OutputFaceSet(
		const MFnMesh &meshFn,
		const MMatrix &xformMat,
		bool doubleSided,
		const MObject &fileObj,
		const MObject &place2dTextureObj,
		bool transparent,
		const CMeshData *pMeshData,
		int parentIndex,
		MObject &faceSetObj,
		const std::string &targaName );

	std::ostream &PrintVertex(
		int parentIndex,
		const MPoint &p,
		const MVector &n,
		const float2 &uv );

	std::ostream &PrintWeights(
		float wTotal,
		size_t weightsPerVertex,
		int nWeights,
		const int3 &wIndex,
		const float3 &wValue );

	void GetNormal(
		const MFnMesh &meshFn,
		MItMeshVertex &vIt,
		int meshVertexIndex,	// Mesh relative vertex index
		MItMeshPolygon &pIt,
		int faceVertexIndex,	// Face relative vertex index
		MVector &n ) const;

	static void GetHistory(
		const MDagPath &mDagPath,
		MSelectionList &historyList );

	static bool PathFromProj(
		const MStringArray &p,
		uint pLen,
		const MStringArray &v,
		uint vLen,
		MString &path );

	static MString &StripExtension(
		MString &path );

	static uint SplitPath(
		const MString &path,
		MStringArray &splitPath );

	std::string GetTargaName(
		std::string &targaPath,
		const MObject &shadingGroupObj,
		MObject &fileObj,
		MObject &place2dTextureObj,
		bool &transparent ) const;

	static MObject FindInputNodeOfType(
		const MObject &dstObj,
		MString dstPlugName,
		MString typeName );

	static MObject FindInputNode(
		const MObject &dstObj,
		MString dstPlugName );

	void GetFaceSets( 
		const MDagPath &mDagPath,
		const MFnMesh &meshFn,
		MObjectArray &shaders,
		std::vector< MFnSingleIndexedComponent * > &faceSetFns );

	void OutputDagInDependentOrder(
		const MDagPath &mDagPath,
		MSelectionList &done );

	void OutputHeader();

	std::ostream &OutputTransform(
		int index,
		const MMatrix &m );

	std::ostream &OutputPrettyTransform(
		int index,
		const MMatrix &m );

	std::ostream &OutputPlainTransform(
		int index,
		const MMatrix &m );

	static std::string PrettyPrint(
		const double &d );

	bool IsNodeVisible( const MDagPath &mDagPath, bool bTemplateAsInvisible = true ) const;

	bool IsPathVisible( MDagPath mDagPath, bool bTemplateAsInvisible = true ) const;

	inline bool OptDoubleSided() const { return ( ( m_options & kOptDoubleSided ) != 0 ); }

	inline bool OptExportInvisible() const { return ( ( m_options & kOptExportInvisible ) != 0 ); }

	inline bool OptBindCurrent() const { return ( ( m_options & kOptBindCurrent ) != 0 ); }

	const uint m_version;
	const std::string m_debugMaterial;
	const uint m_type;
	const uint m_options;
	uint m_optUpAxis;
	MMatrix m_optRotMat;
	MString m_nodeDelPrefix;
	MString m_nodeAddPrefix;
	const bool m_optPrettyPrint;
	const bool m_optWorld;
	uint m_maxNameLen;
	double m_fs;
	double m_fe;
	double m_fi;

	std::ofstream m_os;

	MSelectionList m_nodeList;
	MSelectionList m_meshList;

	typedef std::vector< const CDagData * > DagDataList_t;
	DagDataList_t m_orderedDagList;

	typedef std::map< std::string, CDagData * > DagMap_t;
	DagMap_t m_dagMap;

	typedef std::list< CMeshData * > MeshList_t;
	MeshList_t m_meshDataList;

	std::map< std::string, CMaterial > m_materialList;
};

#endif // SMDEXPORT_H