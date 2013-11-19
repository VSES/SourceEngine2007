#ifndef DXCCMESH_H
#define DXCCMESH_H

//all basic windows sub-includes
//#include "windows.h"
//defines INITGUID and includes guiddef.h
#include "initguid.h"
#include "d3d9.h"
#include "d3dx9.h"


//BEGIN PROTOTYPES
interface IDXCCManager;
interface IDXCCResource;
interface IDXCCHeap;
interface IDXCCFrame;
interface IDXCCVertexBundler;
struct DXCCSaveSceneArgs;
interface IDXCCHierarchyCallbacks;
//END PROTOTYPES


interface __declspec( uuid("640ed315-dec3-477e-84ce-17d7ce2f11da") ) IDXCCMesh;
DEFINE_GUID( IID_IDXCCMesh, 
			0x640ed315, 
			0xdec3, 
			0x477e, 
			0x84, 0xce, 0x17, 0xd7, 0xce, 0x2f, 0x11, 0xda);


typedef interface	IDXCCMesh					IDXCCMesh						,*LPDXCCMESH;
typedef const		ID3DXMesh*					LPCD3DXMESH;
typedef const		ID3DXSkinInfo*				LPCD3DXSKININFO;

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

HRESULT WINAPI
	DXCCCreateMesh(
 	DWORD NumFaces,
 	DWORD NumVertices,
 	LPDIRECT3DDEVICE9 pDevice,
 	LPD3DVERTEXELEMENT9 pDeclaration,
	LPDXCCMESH* ppMesh);

#ifdef __cplusplus
}
#endif //__cplusplus


#undef INTERFACE
#define INTERFACE IDXCCMesh
//This is a derivative of d3dx mesh container.  
//it provides simple access to all vertex, face, material, bone/skin information
//which makes up the concept of a renderable mesh
//It is a hierarchy component which lives under IDXCCFrame
DECLARE_INTERFACE_(IDXCCMesh, IUnknown)
{
    /*** IUnknown methods ***/
	STDMETHOD(QueryInterface)(THIS_  REFIID riid,  void** ppvObj) PURE;
	STDMETHOD_(ULONG,AddRef)(THIS) PURE;
	STDMETHOD_(ULONG,Release)(THIS) PURE;

	//E_ACCESSDENIED may be returned when the structure is embedded
	STDMETHOD(Recycle)(THIS_ 
 		DWORD NumFaces,
 		DWORD NumVertices,
 		LPDIRECT3DDEVICE9 pDevice,
 		LPD3DVERTEXELEMENT9 pDeclaration) PURE;

	STDMETHOD_(DWORD, NumVertices)(THIS) PURE;
	STDMETHOD_(DWORD, NumFaces)(THIS) PURE;

	//access to declaration and vertex buffers
	STDMETHOD(GetVertexBundler)(THIS_ 
		 IDXCCVertexBundler** ppVBundle) PURE;

	//locks and unlocks index buffer (everything else is nonvideo) (attribute and adjacency)
    STDMETHOD(LockFaces)(THIS) PURE; 

	//locks and unlocks index buffer (everything else is nonvideo) (attribute and adjacency)
    STDMETHOD(UnlockFaces)(THIS) PURE;

	//must  call LockFaces to use 
	//must  call UnlockFaces to submit geometry to vid card
	//associate a triangle's local vertex to a vertex  GetVertexBundler
	STDMETHOD(SetFaceVertex)( 
		 UINT iFace, //0 to NumFaces()-1
		 UINT iLocalVertex, //0 to 2
		 DWORD iVertex) PURE; //0 to NumVertices()-1

	//must  call LockFaces to use 
	STDMETHOD(GetFaceVertex)(UINT 
		 iFace, //0 to NumFaces()-1
		 UINT iLocalVertex,//0 to 2
		 DWORD* piVertex) PURE;

	///////////////////////////////////////
	//ADJACENCY
	///////////////////////////////////////

	//adjacency information is for topology based decisions and optimizations
	STDMETHOD(SetFaceAdjacency)(
		 UINT iFace, //0 to NumFaces()-1
		 UINT iLocalAdjacency, //0 to 2
		 DWORD iNewFace) PURE; //0 to NumFaces()-1

	STDMETHOD(GetFaceAdjacency)(
		 UINT iFace, //0 to NumFaces()-1
		 UINT iLocalAdjacency, //0 to 2
		 DWORD* piNewFace) PURE;

	//if you do not wish to provide topology information then it may be generated 
    STDMETHOD(GenerateAdjacency)(THIS_ 
		 FLOAT Epsilon) PURE; //where epsilon is the max distance between vertices for them to be considered the same point

	//users may choose to use pointreps at times for convenience. these functions provide conversion.
	STDMETHOD(ConvertPointRepsToAdjacency)(THIS_ 
		 CONST DWORD* pPRep) PURE;//take pointreps to fix internal adjacency

	STDMETHOD(ConvertAdjacencyToPointReps)(THIS_ 
		 LPD3DXBUFFER* ppPRep) PURE;//get pointreps instead of adjacency


	///////////////////////////////////////
	//ATTRIBUTES
	///////////////////////////////////////

	//set an ID per face that can be associated with materials though Get/SetAttributedMaterial
	STDMETHOD(SetFaceAttribute)(
		 UINT iFace, //0 to NumFaces()-1
		 DWORD Attribute) PURE; //any value you would like to group your polygons by

	STDMETHOD(GetFaceAttribute)(
		 UINT iFace, //0 to NumFaces()-1
		 DWORD* pAttribute) PURE;

	//get a list of all DWORD attributes that have had materials assigned to them
	STDMETHOD(GetAttributeBindingList)(THIS_ 
		 LPD3DXBUFFER* ppbAttribute, //a buffer full of DWORDs.  one for each material assignment
		 DWORD* pNumAttributedMaterials) PURE;

	//assign a material to a DWORD attributes
	STDMETHOD(SetAttributeBoundShader)(THIS_ 
		 DWORD dwAttribute, 
		 LPDXCCSHADERPROPERTIES pShader) PURE;

	STDMETHOD(SetAttributeBoundMaterial)(THIS_ 
		 DWORD dwAttribute, 
		 D3DXMATERIAL* pShader) PURE;

	//retrieve a material assigned to a DWORD attributes
	//Assignment ID ... may be used for hashing
	//static shared (material and shader) incrimented on assignments
	STDMETHOD(GetAttributeBoundShader)(THIS_ 
		 DWORD dwAttribute, 
		 LPDXCCSHADERPROPERTIES* ppShader, 
		 DWORD* AssignmentID) PURE;
	
	//retrieve a material assigned to a DWORD attributes
	//Assignment ID ... may be used for hashing
	//static shared (material and shader) incrimented on assignments
	STDMETHOD(GetAttributeBoundMaterial)(THIS_ 
		 DWORD dwAttribute, 
		 LPD3DXMATERIAL* ppShader, 
		 DWORD* AssignmentID) PURE;


	STDMETHOD(RemoveAttributeBinding)(THIS_ 
		DWORD dwAttribute) PURE;

	STDMETHOD(RemoveAllAttributeBindings)(THIS) PURE;

	STDMETHOD(GetAttributeTable)(THIS_ 
		 LPD3DXBUFFER* ppAttribTable,  //d3dxattribute structures
		 DWORD* pAttribTableSize) PURE;

	///////////////////////////////////////
	//SKIN
	///////////////////////////////////////

	//Creates Bones - Frame bindings, OffsetMatrices, and Influences
	//Creates BindPose Stream and modifies existing VertexBundler
	//multiple calls to this function will result in loss of previous skin data
	//NumBones == 0 is same as calling RemoveSkin
	STDMETHOD(CreateSkin)(THIS_ 
		 DWORD NumBones) PURE; 
	//Removes Bones
	//Remove BindPose Stream
	STDMETHOD(RemoveSkin)(THIS) PURE; 
	
	STDMETHOD(SetSkinParent)(THIS_ 
		 IDXCCFrame* pParent) PURE;

	STDMETHOD(GetSkinParent)(THIS_
		 IDXCCFrame** ppParent) PURE;

	STDMETHOD(DoSkinning)(THIS) PURE; 
	
	STDMETHOD_(BOOL, IsSkinned)(THIS) PURE; 

	STDMETHOD(UndoSkinning)(THIS) PURE; 

	STDMETHOD(RecalculateUnskinnedMesh)(THIS) PURE; 

	STDMETHOD_(DWORD, NumBones)(THIS) PURE;		

	STDMETHOD(SetBoneFrame)(THIS_ 
		 DWORD Bone, //0 to NumBones()-1
		 IDXCCFrame* pBoneFrame, //the frame of the bone you are associating
		 BOOL CalcBoneOffset) PURE;

	STDMETHOD(GetBoneFrame)(THIS_ 
		 DWORD Bone, //0 to NumBones()-1
		 IDXCCFrame** ppBoneFrame) PURE;	

	//matrix that brings a vertex from the mesh's space to the bone's space
	STDMETHOD(SetBoneOffsetMatrix)(THIS_ 
		 DWORD Bone, //0 to NumBones()-1
		 LPD3DXMATRIX pBone) PURE;	

	//matrix that brings a vertex from the mesh's space to the bone's space
	STDMETHOD(GetBoneOffsetMatrix)(THIS_ 
		 DWORD Bone, //0 to NumBones()-1
		 LPD3DXMATRIX* ppBone) PURE;	

	STDMETHOD(SetBoneInfluence)(THIS_ 
		 DWORD bone, //0 to NumBones()-1
		 UINT iVertex, //0 to NumVertices()-1
		 FLOAT weight) PURE;	//% of total that that bone effects the vertex

	STDMETHOD(GetBoneInfluence)(THIS_ 
		 DWORD bone, //0 to NumBones()-1
		 UINT iVertex, //0 to NumVertices()-1
		 FLOAT* pWeight) PURE;
 
	STDMETHOD(RemoveDuplicateVertices)(THIS_ 
		D3DXWELDEPSILONS* epsilon,
		LPD3DXBUFFER* ppFaceRemap, 
		LPD3DXBUFFER* ppVertexRemap) PURE;

	//inplace only
    STDMETHOD(Optimize)(THIS_ 
		 DWORD OptimizeFlags, //use one or more D3DXMESHOPT flags
		 LPD3DXBUFFER* ppFaceRemap, 
		 LPD3DXBUFFER *ppVertexRemap) PURE;  

	STDMETHOD(DrawSubset)(THIS_ 
		 DWORD AttribId) PURE;

    //get the index buffer for private rendering
	STDMETHOD(GetIndexBuffer)(THIS_ 
		 LPDIRECT3DINDEXBUFFER9* ppIB) PURE;

	STDMETHOD(GetAdjacencyBuffer)(THIS_ 
		 LPD3DXBUFFER* ppBuf) PURE;
	
	//see D3DXComputeTangent for docs
	STDMETHOD(ComputeTangent)( THIS_ 
		DWORD TexStageIndex, 
		DWORD TangentIndex, 
		DWORD BinormIndex, 
		DWORD Wrap ) PURE;


	//pManager is to fetch Name and ResourcePath
	//flags are DXCCTOD3DX_FLAGS && DXCCTOD3DXMESH_FLAGS
	STDMETHOD(CreateD3DXMeshContainer)(THIS_ IDXCCManager* pManager, DXCCSaveSceneArgs* pSaveOptions, DWORD ExtensionSize, IDXCCHierarchyCallbacks* MeshCallback, IDXCCHeap* pHeap) PURE;
};

#endif