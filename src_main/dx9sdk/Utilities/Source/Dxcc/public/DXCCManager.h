#ifndef DXCCMANAGER_H
#define DXCCMANAGER_H

//all basic windows sub-includes
//#include "windows.h"
//defines INITGUID and includes guiddef.h
#include "initguid.h"
#include "d3d9.h"
#include "d3dx9.h"

interface __declspec( uuid("435d9fb7-e9df-4d5e-ab69-d7cf54332d4e") ) IDXCCManager;
DEFINE_GUID( IID_IDXCCManager,
			0x435d9fb7, 
			0xe9df, 
			0x4d5e, 
			0xab, 0x69, 0xd7, 0xcf, 0x54, 0x33, 0x2d, 0x4e);
typedef interface	IDXCCManager				IDXCCManager					,*LPDXCCMANAGER;


#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

HRESULT WINAPI 
	DXCCCreateManager(          
	LPDXCCMANAGER* ppManager);

#ifdef __cplusplus
}
#endif //__cplusplus


#undef INTERFACE
#define INTERFACE IDXCCManager
DECLARE_INTERFACE_(IDXCCManager, IUnknown)
{
	STDMETHOD(QueryInterface)(THIS_  REFIID riid,  void** ppvObj) PURE;
	STDMETHOD_(ULONG,AddRef)(THIS) PURE;
	STDMETHOD_(ULONG,Release)(THIS) PURE;


	//case sensitive
	STDMETHOD(FindResourceByName)(THIS_ 
		 LPCSTR szName, 
		 DXCCHANDLE hAfter, 
		 LPDXCCRESOURCE* ppResource) PURE;

	//non case sensitive
	STDMETHOD(FindResourceByResourcePath)(THIS_ 
		 LPCSTR szResourcePath, 
		 DXCCHANDLE hAfter, //if 0 then it starts from the beginning otherwise if it's a valid handle it searches after
		 LPDXCCRESOURCE* ppResource) PURE;

	//find a specific interface or derivative interface though QueryForInterface variable
	//ex:good for searching for all texture object since they derive from basetexture
	STDMETHOD(FindResourceByIID)(THIS_ 
		 REFIID iid, 
		 DXCCHANDLE hAfter, //if 0 then it starts from the beginning otherwise if it's a valid handle it searches after
		 LPDXCCRESOURCE* ppResource) PURE;

	STDMETHOD(FindResourceByQuery)(THIS_ 
		 REFIID iid, 
		 DXCCHANDLE hAfter, //if 0 then it starts from the beginning otherwise if it's a valid handle it searches after
		 LPDXCCRESOURCE* ppResource) PURE;

	//should be the fastest find method
	STDMETHOD(FindResourceByHandle)(THIS_ 
		 DXCCHANDLE Handle,
		 LPDXCCRESOURCE* ppResource) PURE;

	//find an object within the space 
	STDMETHOD(FindResourceByPointer)(THIS_ 
		 LPUNKNOWN pFind, //required if finding by handle
		 DXCCHANDLE hAfter, //if 0 then it starts from the beginning otherwise if it's a valid handle it searches after
		 LPDXCCRESOURCE* ppResource) PURE;

	//create a resource
	STDMETHOD(CreateResource)(THIS_ 
		 LPUNKNOWN pUnknown, 
		 REFIID riid,
		 BOOL Attached,
		 LPDXCCRESOURCE* ppResource) PURE;

	//remove a resource from the manager. 
	//The resource may remain alive if it being referenced by something 
	//but it will not be retrievable though the mananger.
	STDMETHOD(RemoveResource)(THIS_ DXCCHANDLE handle) PURE;

	STDMETHOD(RecycleResource)(THIS_ 
		DXCCHANDLE hResource,
		LPUNKNOWN pUnknown, 
		REFIID riid,
		BOOL Attached,
		BOOL EraseMetadata) PURE;//name, path. userdata 

	//STDMETHOD_(void*, CreateMemory)(THIS_ DWORD SizeInBytes) PURE;
	//STDMETHOD_(DWORD, FindMemorySize)(THIS_ void* memory) PURE;
	//STDMETHOD_(UINT, AddRefMemory)(THIS_ void* memory) PURE;
	//STDMETHOD_(UINT, ReleaseMemory)(THIS_ void* memory) PURE;
	//translator is another set of memory, set of DWORDs, each indicating an offset of a pointer.
	//STDMETHOD_(SetMemoryTranslator)(THIS_ void* memory, void* translator) PURE;
	//STDMETHOD(ShadowMemory)(THIS_ DWORD SizeInBytes, void* memory) PURE;

};


#endif
