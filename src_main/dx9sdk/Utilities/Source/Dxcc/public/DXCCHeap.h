#ifndef DXCCHEAP_H
#define DXCCHEAP_H

//all basic windows sub-includes
//#include "windows.h"
//defines INITGUID and includes guiddef.h
#include "initguid.h"
#include "d3d9.h"
#include "d3dx9.h"

typedef interface	IDXCCHeap					IDXCCHeap						,*LPDXCCHEAP;

interface __declspec( uuid("8dd12056-b4e3-4195-976a-5fc56c2d8ae0") ) IDXCCHeap;
DEFINE_GUID( IID_IDXCCHeap, 
	0x8dd12056, 
	0xb4e3, 
	0x4195, 
	0x97, 0x6a, 0x5f, 0xc5, 0x6c, 0x2d, 0x8a, 0xe0);

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

HRESULT WINAPI
	DXCCCreateHeap(
	LPDXCCHEAP* ppHeap);

#ifdef __cplusplus
}
#endif //__cplusplus

#undef INTERFACE
#define INTERFACE IDXCCHeap
DECLARE_INTERFACE_(IDXCCHeap, ID3DXBuffer)
{
	STDMETHOD(QueryInterface)(THIS_  REFIID riid,  void** ppvObj) PURE;
	STDMETHOD_(ULONG,AddRef)(THIS) PURE;
	STDMETHOD_(ULONG,Release)(THIS) PURE;

	//this is the primary memory
    STDMETHOD_(LPVOID, GetBufferPointer)(THIS) PURE;
    STDMETHOD_(DWORD, GetBufferSize)(THIS) PURE;

	STDMETHOD_(LPVOID,CreateMemory)(THIS_ DWORD SizeInBytes) PURE;
	STDMETHOD(DeleteMemory)(THIS_ void* Memory) PURE;
	STDMETHOD(SetPrimaryMemory)(THIS_ void* memory) PURE;

};


#endif