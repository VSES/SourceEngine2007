#pragma once
#ifndef DXMNODEMESHADAPTER_H
#define DXMNODEMESHADAPTER_H

struct DCCVertexInfo;
struct DCCMeshInfo;
struct VertexD3DToMayaType; 
struct VertexMayaToD3DType; 	

class DXMNodeMeshAdapter : public DXMNodeAdapter
{
public:
	enum MESHSYNCFLAGS
	{
		MESHSYNC_ALL= 1,
	};
	DWORD SyncFlags;

	DXMNodeMeshAdapter();
	virtual ~DXMNodeMeshAdapter();

	virtual void Initialize(DXMNode* node);
	virtual void Destroy();

	virtual void SetCallbackState(bool DoCallbacks);
	
	virtual bool Synchronize();
	void SyncMesh();

	void DestroyMesh();

	//When a Route changes it's visibility it requests 
	//that the mesh updates it's visibility by calling this function.
	//when no routes are visible the visibility of this mesh is false.
	//when visibility is false all OnAttributeChanged calls are ignored.
	void UpdateVisibility();

	bool IsAnyRouteVisible() { return AnyRoutesVisible; }

	virtual void OnAttributeChanged( MNodeMessage::AttributeMessage msg, MPlug& plug, MPlug& otherPlug);
	static void DispatchAttributeChanged( MNodeMessage::AttributeMessage msg, MPlug& plug, MPlug& otherPlug, void* clientData );

	virtual const CStringA& Signature(){ return MySignature; }
	virtual const CStringA& OwnerSignature(){ return DXMGraphMeshAdapter::MySignature; }

	static CStringA			MySignature;
	IDXCCResource*			Resource;
	IDXCCMesh*				Mesh;

	static MObject		MPxMesh_outMesh;
protected:
	MCallbackId			AttributeChangedCID;
	bool				AnyRoutesVisible;


	HRESULT				ExtractMesh();
	void				GatherMesh(MFnMesh& mesh, MIntArray& ShaderAttributes, MFnSkinCluster& skinCluster, UINT boneCount);
	void				GatherMesh_Internal(MObject& obj, MIntArray& ShaderAttributes, UINT boneCount);


	bool				FindSkinMesh(const MFnMesh& mesh, MFnSkinCluster& skinCluster, MDagPathArray& boneArray);
	HRESULT				CreateMesh(MObject& obj, DCCMeshInfo &MayaMeshInfo);
	HRESULT 			GatherMeshInfo(MObject& obj, DCCMeshInfo& MayaMeshInfo);
	HRESULT 			GatherVertexInfo(MObject& obj, DCCVertexInfo& MayaVertexInfo);
	HRESULT				GatherSkin(MFnMesh& fnMesh, MFnSkinCluster& skinCluster, MDagPathArray& boneArray);
	HRESULT				GatherMaterials( MObjectArray& Shaders,  bool AddDefaults, LPDXCCMESH pMesh);


	static CAtlArray<DWORD>					g_D3DPointReps;
	static CAtlArray<VertexD3DToMayaType>	g_VertexD3DToMaya;
	static CAtlArray<VertexMayaToD3DType>	g_VertexMayaToD3D;
};

struct DCCVertexInfo
{
	MPoint			position;
	MVector			normal;
	MColor			color;
	UINT			cUVs;
	MStringArray	UVNames;
	MFloatArray		Ucoords;
	MFloatArray		Vcoords;

	DCCVertexInfo()
	{
		cUVs=0;
	}
};

struct DCCMeshInfo
{

	UINT 			cD3DTriangles; 
	UINT 			cD3DVertices;

	DCCVertexInfo	VertexInfo;

	DCCMeshInfo()
	{
		cD3DTriangles=0; 
		cD3DVertices=0;
	}
};


struct VertexD3DToMayaType
{
	VertexD3DToMayaType()
	{
		MayaMaterial= UNUSED32;
		MayaVertex= UNUSED32;
		D3DNextPointRep= UNUSED32;
	};
	DWORD MayaMaterial;
	DWORD MayaVertex;
	DWORD D3DNextPointRep;
}; 

struct VertexMayaToD3DType
{
	VertexMayaToD3DType()
	{
		D3DFirstPointRep= UNUSED32;
		D3DLastPointRep= UNUSED32;
	};

	DWORD D3DFirstPointRep;
	DWORD D3DLastPointRep;
}; 	

#endif