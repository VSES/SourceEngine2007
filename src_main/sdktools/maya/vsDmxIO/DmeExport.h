//======= Copyright © 1996-2006, Valve Corporation, All rights reserved. ======
//
// Purpose: Implementation of ValveMaya::CDmeExport
//
//=============================================================================

// Maya includes

#include <maya/MFnDagNode.h>
#include <maya/MDagPath.h>
#include <maya/MSelectionList.h>
#include <maya/MString.h>

// Valve includes

#include "mathlib/mathlib.h"
#include "utlmap.h"
#include "utlstack.h"
#include "utlstring.h"
#include "utlvector.h"
#include "datamodel/idatamodel.h"

class CDmeDag;
class CDmeCombinationOperator;
class CDmElement;
class CDmeFaceSet;
class CDmeMayaMakefile;
class CDmeMesh;
class CDmeModel;
class CDmeSourceMayaFile;
class CDmeTransform;
class CDmeVertexData;


//-----------------------------------------------------------------------------
// Class to store information about a mesh's normals
//-----------------------------------------------------------------------------

class CNormalData
{
public:
	CNormalData();

	void GetNormalData(
		MObject &meshObj,
		CUtlVector< Vector > &normals ) const;

	void GetNormalData(
		const MDagPath &baseMeshPath,
		bool allShared );

	int AddNormalData(
		CDmeVertexData *pDmeVertexData ) const;

	int NormalDataIndex(
		int faceIndex,
		int faceVertexIndex ) const;

private:
	void MapSoftVertexNormals(
		const MDagPath &baseMeshPath,
		bool allShared );

	CUtlVector< Vector > m_normals;
	CUtlVector< int > m_faceIndexToFaceVaryingIndexMap;
	CUtlVector< int > m_faceVaryingIndexToDmeNormalIndexMap;
	CUtlMap< int, int > m_softVertexNormalMap;
};


//-----------------------------------------------------------------------------
// Class to store information about a mesh's normals
//-----------------------------------------------------------------------------

class CDmeExport
{
public:
	CDmeExport(
		uint type,
		const MString &pathname,
		const MString &dmObjectIdStr );

	~CDmeExport();

	DmFileId_t Adopt();

	CDmeDag *FindDmeDag(
		const MDagPath &mDagPath ) const;

	void MayaAttrToDmeAttr(
		const MPlug &mPlug,
		CDmElement *pDmeElement );

	void MayaAttrsToDme(
		MDagPath &mDagPath,
		CDmElement *pDmeElement );

	CDmeModel *CreateDmeModel(
		const char *pDmeModelName,
		const char *pDmeModelTransformName,
		CDmeDag *pDmeDag = NULL );

	uint ConvertDag(
		MDagPath &mDagPath,
		CDmeDag *pDmeDagParent,
		CDmeDag **ppDmeDagRet,
		bool force );

	uint ConvertShape(
		const MDagPath &mDagPath,
		CDmeDag *pDmeDagParent,
		CDmeDag **ppDmeDagRet );

	uint ConvertTransform(
		const MDagPath &mDagPath,
		CDmeDag *pDmeDagParent,
		CDmeDag **ppDmeDagRet,
		bool force );

	uint ConvertTransformChildren(
		const MDagPath &mDagPath,
		CDmeDag *pDmeDag,
		CDmeDag **ppDmeDagRet,
		bool force );

	void ConvertTransformData(
		const MDagPath &mDagPath,
		MFn::Type apiType,
		CDmeDag *pDmeDag );

	uint ConvertAttachment(
		const MDagPath &mDagPath,
		const MFnDagNode &dagFn,
		CDmeDag *pDmeDagParent ) const;

	uint ConvertMesh(
		const MDagPath &mDagPath,
		CDmeDag *pDmeDagParent,
		CDmeDag **ppDmeDagRet );

	void MayaFaceSetsToDmeFaceSets(
		const MDagPath &meshPath,
		const MDagPath &baseMeshPath,
		const CNormalData &normalData,
		CDmeMesh *pDmeMesh,
		CUtlVector< int > &positionIndices,
		CUtlVector< int > &normalIndices,
		CUtlVector< int > &uvIndices,
		CUtlVector< Vector2D > &uvData,
		CUtlVector< int > &colorIndices ) const;

	void MayaFaceSetToDmeFaceSet(
		const MDagPath &meshPath,
		const MDagPath &baseMeshPath,
		const CNormalData &normalData,
		const MObject &mayaFaceSetObject,
		const MString &uvSet,
		CDmeFaceSet *pFaceSet,
		CUtlVector< int > &positionIndices,
		CUtlVector< int > &normalIndices,
		CUtlVector< int > &uvIndices,
		const CUtlVector< int > &uvIndexMap,
		int uvOffset,
		CUtlVector< int > &colorIndices ) const;

	void SaveBindPose(
		const MFnDagNode &dagFn,
		CDmeDag *pDmeDag,
		CDmeTransform *pDmeTransform ) const;

	void GetMaterialInfo(
		const MObject &shadingGroupObj,
		MString &meshName,
		MString &faceSetName,
		MString &materialName,
		MString &materialPath,
		MString &uvSet,
		MString &debugWhy ) const;

	void AddTags(
		const MString &cmdLine ) const;

	DmFileId_t FileId() const { return m_dmFileId; }

	void SetFileName(
		const char *filename );

	void Bind(
		CDmeModel *pDmeModelRoot );

	CDmElement *GetRoot() const { return m_pRoot; }

	typedef CUtlMap<CUtlString, CDmeDag *>::IndexType_t Index_t;	// Index into the map

	enum Type	// NOTE: Bitmask flags that can be or'd together
	{
		kModel				= 1 << 0,
		kSkeletalAnimation	= 1 << 1,
		kPhysicsModel		= 1 << 2
	};

protected:
	const uint m_type;								// Types of data to export ( check with & and Type's )
	DmFileId_t m_dmFileId;							// Data Model FileId, so everything gets the same FileId
	CDmElement *m_pRoot;							// The root node for the DMX scene
	bool m_ownDmeData;								// Should this class clear the Dme data when it is destructed?

public:
	CUtlStack<CDmeModel *> m_dmeModelStack;			// Currently active DME model (should only be 1)
	CUtlMap<CUtlString, CDmeDag *> m_mayaToDmeDag;	// Maps MDagPath::fullPathName to a DmeDag node
	MSelectionList m_bindList;						// A list of MDagPath's which have to be bound
	MSelectionList m_exportedList;					// A list of MDagPath's which have been exported
	bool m_exportInvisible;							// Should invisible nodes be exported?
	CDmeMayaMakefile *m_pMayaMakefile;				// The Dme Makefile for re-running this conversion
	CDmeSourceMayaFile *m_pMayaMakefileSource;		// The Maya source information for re-running this conversion
	bool m_scratchVmt;								// Should scratch vmt files be created?
	bool m_optMergeUvs;								// Should UV coordinates be merged?
	bool m_optTriangulateBad;						// Should bad polygons be triangulated
	bool m_optWeightMap;							// Create a weight map
	bool m_optBindCurrent;							// Use the current pose as the bind pose

private:
	void HandleDeformers(
		const MDagPath &mDagPath,
		MDagPath &baseDagPath,
		MObject &skinObj,
		MObject &blendObj ) const;

	static int AddPositionData(
		const MDagPath &meshDagPath,
		CDmeVertexData *pDmeVertexData );

	int AddUVData(
		const MDagPath &meshDagPath,
		const MString &uvSet,
		CUtlVector< Vector2D > &uvData,
		CUtlVector< int > &uvIndexMap ) const;

	static int AddColorData(
		const MDagPath &meshDagPath,
		CDmeVertexData *pDmeVertexData );

	static bool IsInfluenceObject(
		const MDagPath &mDagPath );

	CDmeCombinationOperator *FindOrCreateDmeComboOp(
		const MString &name ) const;

	CDmeCombinationOperator *DoBlendShape(
		const MDagPath &meshPath,
		const MDagPath &baseMeshPath,
		const MObject &blendShapeObj,
		const CNormalData &normalData,
		CDmeMesh *pDmeMesh ) const;

	static void GetDeltaNormals(
		MFnMesh &meshFn,
		MObject &meshObj,
		const CNormalData &normalData,
		CDmeMesh *pDmeMesh,
		int nDeltaIndex,
		const CUtlVector< int > &dependentDeltaStates );

	static void GetDeltaNormals(
		const MDagPath &baseMeshPath,
		const CNormalData &normalData,
		CDmeMesh *pDmeMesh,
		CDmeCombinationOperator *pDmeComboOp );

	static MString GetNodeName(
		const MDagPath &baseMeshPath );

	bool DoType( uint type ) const { return ( m_type & type ) != 0; }

	bool IsDagOkToConvert(
		MDagPath &mDagPath,
		bool force,
		uint &retVal ) const;
};