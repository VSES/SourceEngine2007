#ifndef DXCCVERTEXDECLARATION_H
#define DXCCVERTEXDECLARATION_H

//all basic windows sub-includes
//#include "windows.h"
//defines INITGUID and includes guiddef.h
#include "initguid.h"
#include "d3d9.h"
#include "d3dx9.h"

typedef interface	IDXCCDeclaration			IDXCCDeclaration				,*LPDXCCDECLARATION;

interface __declspec( uuid("8d7815e8-3589-4938-bdab-366dbc57424d") ) IDXCCDeclaration;
DEFINE_GUID( IID_IDXCCDeclaration, 
	0x8d7815e8, 
	0x3589, 
	0x4938, 
	0xbd, 0xab, 0x36, 0x6d, 0xbc, 0x57, 0x42, 0x4d);

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

HRESULT WINAPI
	DXCCCreateDeclaration(
	LPDXCCDECLARATION* ppDeclaration);

#ifdef __cplusplus
}
#endif //__cplusplus

#undef INTERFACE
#define INTERFACE IDXCCDeclaration
//#define INTERFACE IDXCCDeclarationBuilder
//extends read only functionality of IDXCCDeclarationReadOnly to provide set/edit functionality.
//provides the ability to generate a declaration from scratch.
//DECLARE_INTERFACE_(IDXCCDeclarationBuilder, IDXCCDeclaration)
DECLARE_INTERFACE_(IDXCCDeclaration, IUnknown)
{
	STDMETHOD(QueryInterface)(THIS_  REFIID riid,  void** ppvObj) PURE;
	STDMETHOD_(ULONG,AddRef)(THIS) PURE;
	STDMETHOD_(ULONG,Release)(THIS) PURE;

	//STDMETHOD(Clone)(THIS_ D3DVERTEXELEMENT9* pDecl) PURE;
	STDMETHOD_(BOOL, Identical)(THIS_ 
		 CONST D3DVERTEXELEMENT9* pDecl) PURE;

	STDMETHOD_(UINT, NumElements)(THIS) PURE;

	//1to4 fields per element.
	STDMETHOD_(UINT, NumElementFields)(THIS_ 
		 UINT iElement) PURE;

	STDMETHOD_(CONST LPD3DVERTEXELEMENT9, GetElements)(THIS) PURE;

	STDMETHOD_(CONST LPD3DVERTEXELEMENT9, GetElement)(THIS_ 
		 UINT iElement) PURE;

	//a custom name can be attached per Element
	STDMETHOD_(LPCSTR, GetElementName)(THIS_ 
		 UINT iElement) PURE;

	//a custom name can be attached per Element
	STDMETHOD(FindElementByName)(THIS_ 
		 LPCSTR name, 
		 UINT* piElement, 
		 LPD3DVERTEXELEMENT9* ppElement) PURE;

	STDMETHOD(FindElementBySemantic)(THIS_ 
		 UINT _Usage, 
		 UINT _UsageIndex, 
		 UINT* piElement, 
		 LPD3DVERTEXELEMENT9* ppElement) PURE;

	STDMETHOD_(UINT, NumBlendWeightFields)(THIS) PURE;

	STDMETHOD_(UINT, NumBlendIndexFields)(THIS) PURE;

	STDMETHOD_(UINT, NumTexcoordFields)(THIS_ 
		 UINT UsageIndex) PURE;

	STDMETHOD(GetPositionElement)(THIS_ 
		 UINT* piElement, 
		 LPD3DVERTEXELEMENT9* ppElement) PURE;

	STDMETHOD(GetPointSizeElement)(THIS_ 
		 UINT* piElement,
		 LPD3DVERTEXELEMENT9* ppElement) PURE;

	STDMETHOD(GetNormalElement)(THIS_ 
		 UINT* piElement, 
		 LPD3DVERTEXELEMENT9* ppElement) PURE;

	STDMETHOD(GetTangentElement)(THIS_ 
		 UINT* piElement, 
		 LPD3DVERTEXELEMENT9* ppElement) PURE;

	STDMETHOD(GetBinormalElement)(THIS_ 
		 UINT* piElement, 
		 LPD3DVERTEXELEMENT9* ppElement) PURE;

	STDMETHOD(GetDiffuseElement)(THIS_ 
		 UINT* piElement, 
		 LPD3DVERTEXELEMENT9* ppElement) PURE;

	STDMETHOD(GetSpecularElement)(THIS_ 
		 UINT* piElement, 
		 LPD3DVERTEXELEMENT9* ppElement) PURE;

	STDMETHOD(GetTexcoordElement)(THIS_ 
		 BYTE UsageIndex, 
		 UINT* piElement, 
		 LPD3DVERTEXELEMENT9* ppElement) PURE;

	STDMETHOD(GetBlendWeightsElement)(THIS_ 
		 UINT* piElement, 
		 LPD3DVERTEXELEMENT9* ppElement) PURE;

	STDMETHOD(GetBlendIndicesElement)(THIS_ 
		 UINT* piElement, 
		 LPD3DVERTEXELEMENT9* ppElement) PURE;

	//E_ACCESSDENIED may be returned when the structure is embedded
	STDMETHOD(SetDeclaration)(THIS_ 
		 DWORD FVF) PURE;

	//E_ACCESSDENIED may be returned when the structure is embedded
	STDMETHOD(SetDeclaration)(THIS_ 
		 CONST D3DVERTEXELEMENT9* pDecl) PURE;

	//a custom name can be attached per Element
	STDMETHOD(SetElementName)(THIS_ 
		 UINT iElement, 
		 LPCSTR name) PURE;

	//a custom name can be attached per Element but this is the default name
	//E_ACCESSDENIED may be returned when the structure is embedded
	STDMETHOD(SetElementNameToDefault)(THIS_ 
		 UINT iElement) PURE;

	//E_ACCESSDENIED may be returned when the structure is embedded
	STDMETHOD(RemoveElement)(THIS_ 
		 UINT Index) PURE;

	//E_ACCESSDENIED may be returned when the structure is embedded
	STDMETHOD(InsertElement)(THIS_ 
		 UINT iBefore, //use NumElements() to hit the end
		 LPD3DVERTEXELEMENT9 pNew, 
		 LPCSTR _name) PURE;

	//E_ACCESSDENIED may be returned when the structure is embedded
	STDMETHOD(InsertPositionElement)(THIS_ 
		 UINT iBefore) PURE;//use NumElements() to hit the end

	//E_ACCESSDENIED may be returned when the structure is embedded
	STDMETHOD(InsertPointSizeElement)(THIS_ 
		 UINT iBefore) PURE;//use NumElements() to hit the end

	//E_ACCESSDENIED may be returned when the structure is embedded
	STDMETHOD(InsertNormalElement)(THIS_ 
		 UINT iBefore) PURE;//use NumElements() to hit the end

	//E_ACCESSDENIED may be returned when the structure is embedded
	STDMETHOD(InsertTangentElement)(THIS_ 
		 UINT iBefore) PURE;//use NumElements() to hit the end

	//E_ACCESSDENIED may be returned when the structure is embedded
	STDMETHOD(InsertBinormalElement)(THIS_ 
		 UINT iBefore) PURE;//use NumElements() to hit the end

	//E_ACCESSDENIED may be returned when the structure is embedded
	STDMETHOD(InsertDiffuseElement)(THIS_ 
		 UINT iBefore) PURE;//use NumElements() to hit the end

	//E_ACCESSDENIED may be returned when the structure is embedded
	STDMETHOD(InsertSpecularElement)(THIS_ 
		 UINT iBefore) PURE;//use NumElements() to hit the end

	//E_ACCESSDENIED may be returned when the structure is embedded
	STDMETHOD(InsertTexcoordElement)(THIS_ 
		 UINT iBefore, //use NumElements() to hit the end
		 UINT iTexIndex, 
		 UINT cCoordCount, 
		 LPCSTR name) PURE; //texcoords should be named for linkage reasons

	//E_ACCESSDENIED may be returned when the structure is embedded
	STDMETHOD(InsertBlendWeightsElement)(THIS_ 
		 UINT iBefore) PURE;//use NumElements() to hit the end

	//E_ACCESSDENIED may be returned when the structure is embedded
	STDMETHOD(InsertBlendIndicesElement)(THIS_ 
		 UINT iBefore) PURE;//use NumElements() to hit the end
};


#endif

