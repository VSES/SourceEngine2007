#ifndef DXCCSHADER_H
#define DXCCSHADER_H

//all basic windows sub-includes
//#include "windows.h"
//defines INITGUID and includes guiddef.h
#include "initguid.h"
#include "d3d9.h"
#include "d3dx9.h"

#include "DXCCglobals.h"

interface IDXCCPropertyBag;

interface __declspec( uuid("15CDFC91-6C87-4bde-A699-E828CA96AC6E") ) IDXCCShaderProperties;
DEFINE_GUID(IID_IDXCCShaderProperties, 
			0x15cdfc91, 
			0x6c87, 
			0x4bde, 
			0xa6, 0x99, 0xe8, 0x28, 0xca, 0x96, 0xac, 0x6e);

typedef interface	IDXCCShaderProperties			IDXCCShaderProperties		,*LPDXCCSHADERPROPERTIES;

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

HRESULT WINAPI
	DXCCCreateShaderProperties(
	LPDXCCSHADERPROPERTIES* ppShader);

#ifdef __cplusplus
}
#endif //__cplusplus

#undef INTERFACE
#define INTERFACE IDXCCShaderProperties
DECLARE_INTERFACE_(IDXCCShaderProperties, IUnknown)
{
	STDMETHOD(QueryInterface)(THIS_  REFIID riid,  void** ppvObj) PURE;
	STDMETHOD_(ULONG,AddRef)(THIS) PURE;
	STDMETHOD_(ULONG,Release)(THIS) PURE;

	STDMETHOD_(void, SetType)(THIS_ LPCSTR pType) PURE;
	STDMETHOD_(LPCSTR, GetType)(THIS) PURE;

	STDMETHOD(GetProperties)(THIS_ LPDXCCPROPERTYBAG* pPropertyBag) PURE;
};

#endif