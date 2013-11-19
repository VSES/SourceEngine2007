#ifndef DXCCFRAME_H
#define DXCCFRAME_H

//all basic windows sub-includes
//#include "windows.h"
//defines INITGUID and includes guiddef.h
#include "initguid.h"
#include "d3d9.h"
#include "d3dx9.h"

#include "DXCCglobals.h"

//BEGIN PROTOTYPES
interface IDXCCManager;
interface IDXCCResource;
interface IDXCCHeap;
interface IDXCCAnimationStream;
struct DXCCSaveSceneArgs;
interface IDXCCHierarchyCallbacks;
//END PROTOTYPES




interface __declspec( uuid("f9acf835-d202-4ccb-a42c-6bb79cafdc48") ) IDXCCFrame;
DEFINE_GUID( IID_IDXCCFrame, 
			0xf9acf835, 
			0xd202, 
			0x4ccb, 
			0xa4, 0x2c, 0x6b, 0xb7, 0x9c, 0xaf, 0xdc, 0x48);


typedef interface	IDXCCFrame					IDXCCFrame						,*LPDXCCFRAME;
typedef interface	IDXCCEnumFrames				IDXCCEnumFrames					,*LPDXCCENUMFRAMES;

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

HRESULT WINAPI
	DXCCCreateFrame(
	LPDXCCFRAME* ppFrame);

HRESULT WINAPI
DXCCEnumFrames(
	LPDXCCFRAME pRoot,
	LPDXCCENUMFRAMES pEnum);

#ifdef __cplusplus
}
#endif //__cplusplus

#undef INTERFACE
#define INTERFACE IDXCCFrame
DECLARE_INTERFACE_(IDXCCFrame, IUnknown)
{
	STDMETHOD(QueryInterface)(THIS_  REFIID riid,  void** ppvObj) PURE;
	STDMETHOD_(ULONG,AddRef)(THIS) PURE;
	STDMETHOD_(ULONG,Release)(THIS) PURE;

	//am I the root of a tree? 
	STDMETHOD_(BOOL, Root)(THIS) PURE;

	//[IMPLIMENTATION INFO]this should cascade and call RemoveChild on old parent and AddChild on new parent if child is not already retrievable
	//[IMPLIMENTATION INFO]this should NOT perform AddRef/Release on pParent to avoid circular referencing
	STDMETHOD(SetParent)(THIS_ 
		 LPDXCCFRAME pParent, 
		 bool DelayRecalcs) PURE;

	STDMETHOD(GetParent)(THIS_
		 LPDXCCFRAME* ppParent) PURE;

	STDMETHOD_(UINT, NumChildren)(THIS) PURE;

	//[IMPLIMENTATION INFO]this should cascade and call SetParent(this) on child if parent is not set properly
	STDMETHOD(AddChild)(THIS_ 
		 LPDXCCFRAME pChild, 
		 bool DelayRecalcs) PURE;

	//[IMPLIMENTATION INFO]this should cascade and call SetParent(NULL) on child if parent is not set properly
	STDMETHOD(RemoveChild)(THIS_ 
		 UINT index, 
		 bool DelayRecalcs) PURE;

	STDMETHOD(GetChild)(THIS_ 
		 UINT index, 
		 LPDXCCFRAME* ppChild) PURE;

	//find an object within the space 
	STDMETHOD(FindChildByHandle)(THIS_ 
		 IDXCCManager* pManager, //required if finding by handle
		 DXCCHANDLE Handle,
		 UINT *pIndex,
		 LPDXCCFRAME* ppChild) PURE;

	//find an object within the space 
	STDMETHOD(FindChildByPointer)(THIS_ 
		 LPDXCCFRAME pFind, //required if finding by handle
		 UINT *pIndex) PURE;

	//objects within this space
	STDMETHOD_(UINT, NumMembers)(THIS) PURE;

	//add an object to this space
	STDMETHOD(AddMember)(THIS_ 
		 LPUNKNOWN pMember) PURE;

	//remove an object from this space
	STDMETHOD(RemoveMember)(THIS_ 
		 UINT index) PURE;

	//get an object within the space by index
	STDMETHOD(GetMember)(THIS_ 
		 UINT index,  
		 LPUNKNOWN* ppMember) PURE;

	//get an object within the space by index
	STDMETHOD(QueryMember)(THIS_ 
		 UINT index, 
		 REFIID riid, 
		 void** ppMember) PURE;

	//find an object within the space 
	STDMETHOD(FindMemberByHandle)(THIS_ 
		 IDXCCManager* pManager, //required if finding by handle
		 DXCCHANDLE Handle,
		 UINT *pIndex,
		 LPUNKNOWN* ppMember) PURE;

	//find an object within the space 
	STDMETHOD(FindMemberByPointer)(THIS_ 
		 LPUNKNOWN pUnknown, //required if finding by handle
		 UINT *pIndex) PURE;

	//find an object within the space 
	STDMETHOD(FindMemberByQuery)(THIS_ 
		 REFIID riid, //required if finding by handle
		 UINT *pIndex,
		 void** ppMember) PURE;

	//get/set matrix in local space ...ie relative to parent
	STDMETHOD_(CONST LPD3DXMATRIX, GetLocalMatrix)(THIS) PURE;	

	STDMETHOD(SetLocalMatrix)(THIS_ 
		 CONST LPD3DXMATRIX pMatrix, bool DelayRecalcs) PURE;//this affects all m_Children
	STDMETHOD(RecalcLocalsFromWorlds)(THIS) PURE;


	//get/set matrix in world space
	STDMETHOD_(CONST LPD3DXMATRIX, GetWorldMatrix)(THIS) PURE;

	STDMETHOD(SetWorldMatrix)(THIS_ 
		 CONST LPD3DXMATRIX pMatrix, bool DelayRecalcs) PURE;//this affects all m_Children
	STDMETHOD(RecalcWorldsFromLocals)(THIS) PURE;


	//STDMETHOD(SetLocalAnimation)(THIS_ 
	//	 IDXCCAnimationStream* pAnim) PURE;

	STDMETHOD(GetLocalAnimation)(THIS_ 
		 IDXCCAnimationStream** ppAnim) PURE;

	//pManager is to fetch Name and ResourcePath
	//flags are DXCCTOD3DX_FLAGS
	STDMETHOD(CreateD3DXFrame)(THIS_ IDXCCManager* pManager, DXCCSaveSceneArgs* pSaveOptions, DWORD ExtensionSize, IDXCCHierarchyCallbacks* MeshCallback, IDXCCHeap* pHeap) PURE;

	//flags are DXCCTOD3DX_FLAGS
	STDMETHOD(CreateD3DXHierarchy)(THIS_
		IDXCCManager* pManager,
		DXCCSaveSceneArgs* pSaveOptions, 
		DWORD FrameExtensionSize, 
		DWORD MeshExtensionSize, 
		IDXCCHierarchyCallbacks* pCallbacks,
		IDXCCHeap* pHeap) PURE;


};





struct DXCCFramePath
{
	UINT Length;
	LPDXCCFRAME* Root;//array root
	LPDXCCFRAME Parent;
	LPDXCCFRAME End;
};

#undef INTERFACE
//THIS US A USER DEFINED CLASS USED AS A PARAMETER FOR DXCCEnumEffectParameters
#define INTERFACE IDXCCEnumFrames
DECLARE_INTERFACE(IDXCCEnumFrames)
{
	//Once at Start
	STDMETHOD(PreEnumeration)(THIS_  LPDXCCFRAME Root, BOOL& GoDeeper) { return S_OK; };

	//Called before any children (member or element) are iterated 
	STDMETHOD(PreEnumFrame)(THIS_  DXCCFramePath& path, BOOL& GoDeeper, BOOL& DoEnum, BOOL& DoPost ) { return S_OK; };

	//Called for every parameter
	STDMETHOD(EnumFrame)(THIS_  DXCCFramePath& path, BOOL& GoDeeper, BOOL& DoPost) { return S_OK; };

	//Called after all children (member or element) are iterated 
	STDMETHOD(PostEnumFrame)(THIS_  DXCCFramePath& path) { return S_OK; };

	//Once at End
	STDMETHOD(PostEnumeration)(THIS_  LPDXCCFRAME Root) { return S_OK; };

};

#endif