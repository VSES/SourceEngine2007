#ifndef DXCCPROPERTYARRAY_H
#define DXCCPROPERTYARRAY_H

//all basic windows sub-includes
//#include "windows.h"
//defines INITGUID and includes guiddef.h
#include "initguid.h"


#include "DXCCPropertyCollection.h"

typedef interface	IDXCCPropertyArray			IDXCCPropertyArray				,*LPDXCCPROPERTYARRAY;

interface __declspec( uuid("52C7366C-BB6A-4385-9088-665F94566C70") ) IDXCCPropertyArray;
DEFINE_GUID(IID_IDXCCPropertyArray, 
            0x52c7366c, 
            0xbb6a, 
            0x4385, 
            0x90, 0x88, 0x66, 0x5f, 0x94, 0x56, 0x6c, 0x70);

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

HRESULT WINAPI 
DXCCCreatePropertyArray(
	LPDXCCPROPERTYARRAY* ppArray );

#ifdef __cplusplus
}
#endif //__cplusplus


#undef INTERFACE
#define INTERFACE IDXCCPropertyArray
DECLARE_INTERFACE_(IDXCCPropertyArray, IDXCCPropertyCollection)
{
	//IUnknown - BEGIN
	STDMETHOD(QueryInterface)(THIS_  REFIID riid,  void** ppvObj) PURE;
	STDMETHOD_(ULONG,AddRef)(THIS) PURE;
	STDMETHOD_(ULONG,Release)(THIS) PURE;
	//IUnknown - END

	//IDXCCPropertyCollection - BEGIN
	STDMETHOD_(UINT, NumProperties)(THIS) PURE;
	STDMETHOD_(DXCCPROPERTY_KEY, FirstKey)() PURE;
	STDMETHOD_(DXCCPROPERTY_KEY, EnumKeys)(DXCCPROPERTY_KEY PreviousKey) PURE;
	
	STDMETHOD_(DXCCPROPERTY_DESC, GetPropertyDesc)(THIS_ DXCCPROPERTY_KEY Key) PURE;
	STDMETHOD(GetPropertyValueAsData)(THIS_ DXCCPROPERTY_KEY Key, LPCVOID* ppOut ) PURE;
	STDMETHOD(SetPropertyValueAsData)(THIS_ DXCCPROPERTY_KEY Key, DXCCPROPERTY_DESC desc, LPCVOID value ) PURE;
	STDMETHOD(GetPropertyValueAsInterface)(THIS_ DXCCPROPERTY_KEY Key, LPUNKNOWN* ppOut) PURE;
	STDMETHOD(SetPropertyValueAsInterface)(THIS_ DXCCPROPERTY_KEY Key, DXCCPROPERTY_DESC desc, LPUNKNOWN value) PURE;
	//IDXCCPropertyCollection - END

	STDMETHOD_(UINT, GetPropertyIndex)(THIS_ DXCCPROPERTY_KEY Key) PURE;
	STDMETHOD_(DXCCPROPERTY_KEY, GetKey)(THIS_ UINT Index) PURE;

	STDMETHOD(AddPropertyAtEnd)(THIS_ DXCCPROPERTY_KEY* pOutKey ) PURE;
	STDMETHOD(AddPropertyAtIndex)(THIS_ UINT InsertBefore, DXCCPROPERTY_KEY* pOutKey ) PURE;
	STDMETHOD(RemoveProperty)(THIS_ DXCCPROPERTY_KEY Key) PURE;
	STDMETHOD(RemoveAll)(THIS) PURE;

	STDMETHOD(GetPath)(THIS_ LPCSTR a_path, LPDXCCPROPERTYCOLLECTION* ppCollection, DXCCPROPERTY_KEY* pOutKey) PURE;
	STDMETHOD(GetOrMakePath)(THIS_ LPCSTR a_path, LPDXCCPROPERTYCOLLECTION* ppCollection, DXCCPROPERTY_KEY* pOutKey) PURE;
	STDMETHOD_(void, GetRoot)(THIS_ LPDXCCPROPERTYCOLLECTION* ppCollection) PURE;	
	STDMETHOD_(void, GetParent)(THIS_ LPDXCCPROPERTYCOLLECTION* ppCollection) PURE;

	STDMETHOD_(DWORD, GetSchemaHash)(THIS) PURE;
	STDMETHOD_(clock_t, LastChanged)(THIS) PURE;
};




#endif