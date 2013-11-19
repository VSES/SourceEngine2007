#ifndef DXCCFILEIO_H
#define DXCCFILEIO_H

//all basic windows sub-includes
//#include "windows.h"
#include "d3d9.h"
#include "d3dx9.h"

typedef interface	IDXCCHierarchyCallbacks		IDXCCHierarchyCallbacks			,*LPDXCCHIERARCHYCALLBACKS;

typedef struct		DXCCSaveSceneArgs			DXCCSaveSceneArgs				,*LPDXCCSAVESCENEARGS;

//BEGIN PROTOTYPES
typedef interface IDXCCManager IDXCCManager, *LPDXCCMANAGER;
typedef interface IDXCCResource IDXCCResource, *LPDXCCRESOURCE;
typedef interface IDXCCFrame IDXCCFrame, *LPDXCCFRAME;
typedef interface IDXCCMesh IDXCCMesh, *LPDXCCMESH;
//END PROTOTYPES

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

/*
HRESULT 
	DXCCPropertyCollectionLoadFromFile( 
		LPDXCCPROPERTYCOLLECTION* ppCollection,
		LPCSTR szPath,
		LPCSTR type, 
		INT version );

HRESULT 
	DXCCPropertyCollectionSaveToFile(
		LPDXCCPROPERTYCOLLECTION pCollection,
		LPCSTR szPath, 
		LPCSTR type, 
		INT version );

HRESULT 
	DXCCPropertyCollectionDialog(
		LPDXCCPROPERTYCOLLECTION pCollection,
		HWND hParent, 
		BOOL bModal);
*/


HRESULT WINAPI
	DXCCSaveScene(
 	LPCSTR Filename, 
 	DWORD XFormat, 
 	LPDXCCMANAGER pManager,
 	LPDXCCFRAME pRoot,
 	LPD3DXANIMATIONCONTROLLER pAnimation,		//LPD3DXKEYFRAMEDANIMIONSET or LPD3DXANIMATIONCONTROLLER
 	LPDXCCSAVESCENEARGS pSaveOptions);


LPSTR 
DXCCConvertStringToXFileObjectName(
	LPCSTR szString, 
	LPSTR szOutput, 
	UINT cOutputLength);

LPSTR 
DXCCConvertStringToXFilePath(
	LPCSTR szString, 
	LPSTR szOutput,
	UINT cOutputLength);


BOOL WINAPI
DXCCFindFile(
	const char* a_szFilepath, 
	const char* a_SecondaryDirectory, 
	DWORD a_FoundLength,
	char* a_FoundFilepath);

HRESULT WINAPI
DXCCLoadEffectFromFile(
	LPDIRECT3DDEVICE9 a_pDevice,
	LPD3DXINCLUDE pInclude,
	LPDXCCMANAGER a_pManager,  
	const char* a_FxFilename,
	const char* a_SecondaryEffectDirectory, 	
	const char* a_SecondaryTextureDirectory, 	
	LPDXCCRESOURCE* a_ppOutResource,//[out]
	LPD3DXEFFECT* a_ppOutEffect,//[out]
    LPD3DXBUFFER* a_ppOutErrors);//[out]


HRESULT WINAPI
DXCCLoadTextureFromFile(
	LPDIRECT3DDEVICE9 a_pDevice, 
	LPDXCCMANAGER a_pManager,  
	const char* a_TexFilename,
	const char* a_SecondaryDirectory, 	
	LPDXCCRESOURCE* a_ppOutResource,//[out]
	LPDIRECT3DBASETEXTURE9* a_ppOutTexture);//[out]

#ifdef __cplusplus
}
#endif //__cplusplus


struct DXCCSaveSceneArgs
{
	BOOL 							NoAddRefs;	
	BOOL 							NoAnimation;	
	BOOL 							NoFrameNames;		
	BOOL 							NoMeshNames;		
	BOOL 							NoMeshes;		
	BOOL 							NoMeshAdjacencies;	
	BOOL 							NoMeshSkins;	
	BOOL 							NoMeshMaterials;	
	BOOL							ReplaceEffectPaths;
	CHAR							ReplacementEffectPath[MAX_PATH];
	BOOL							ReplaceTexturePaths;
	CHAR							ReplacementTexturePath[MAX_PATH];
	BOOL 							RelativePaths;
	CHAR							RelativeToPath[MAX_PATH];
	//BOOL 							ResaveTexturesAsDDS;

	//BOOL							SkipVertexPositions;
	//BOOL							SkipVertexNormals;
	//BOOL							SkipVertexTangents;
	//BOOL							SkipVertexBinormals;
	//BOOL							SkipVertexColors;
	//BOOL							SkipVertexTextures;
	//BOOL							SkipVertexPointSizes;
	//BOOL							SkipVertexBlendWeights;
	//BOOL							SkipVertexBlendIndices;
};


#undef INTERFACE
#define INTERFACE IDXCCHIERARCHYCALLBACKS
DECLARE_INTERFACE(IDXCCHierarchyCallbacks)
{
	STDMETHOD(OnCreateD3DXFrame)(THIS_ LPDXCCFRAME pThis, LPD3DXFRAME pFrame) PURE;
	STDMETHOD(OnCreateD3DXMeshContainer)(THIS_ LPDXCCMESH pThis, LPD3DXMESHCONTAINER pMesh) PURE;
};





#endif