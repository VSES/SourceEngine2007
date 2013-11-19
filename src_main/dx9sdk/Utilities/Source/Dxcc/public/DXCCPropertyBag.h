#ifndef DXCCPROPERTYBAG_H
#define DXCCPROPERTYBAG_H

//all basic windows sub-includes
//#include "windows.h"
//defines INITGUID and includes guiddef.h
#include "initguid.h"

#include "DXCCPropertyCollection.h"


typedef interface	IDXCCPropertyBag			IDXCCPropertyBag				,*LPDXCCPROPERTYBAG;

interface __declspec( uuid("bb54ea69-4bf3-45ab-a8e5-f8149022912c") ) IDXCCPropertyBag;
DEFINE_GUID( IID_IDXCCPropertyBag,
			0xbb54ea69, 
			0x4bf3, 
			0x45ab, 
			0xa8, 0xe5, 0xf8, 0x14, 0x90, 0x22, 0x91, 0x2c);


#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

HRESULT WINAPI 
DXCCCreatePropertyBag(
	LPDXCCPROPERTYBAG* ppBag );

//ONLY SUPPORTS FLAT BAGS AT THIS TIME
//ONLY SUPPORTS SCALARS AND STRING
HRESULT WINAPI 
DXCCSavePropertyBagAsFile(
	LPDXCCPROPERTYBAG pBag,  
	LPCSTR szPath,
	LPCSTR typeName, 
	int version);

//ONLY SUPPORTS FLAT BAGS AT THIS TIME
//ONLY SUPPORTS SCALARS AND STRING
HRESULT WINAPI
DXCCLoadPropertyBagFromFile( 
	LPCSTR szPath, 
	LPCSTR verifyTypeName, 
	int verifyVersion,
	LPDXCCPROPERTYBAG* pBag);


//ONLY SUPPORTS FLAT BAGS AT THIS TIME
//ONLY SUPPORTS SCALARS AND STRING
HRESULT WINAPI
DXCCShowPropertyBagDialog( 
	LPDXCCPROPERTYBAG pBag, 
	LPCSTR title,
	HWND hParent, 
	BOOL bModal);

#ifdef __cplusplus
}
#endif //__cplusplus


#undef INTERFACE
#define INTERFACE IDXCCPropertyBag
DECLARE_INTERFACE_(IDXCCPropertyBag, IDXCCPropertyCollection)
{
	//IUnknown - BEGIN
	STDMETHOD(QueryInterface)(THIS_  REFIID riid,  void** ppvObj) PURE;
	STDMETHOD_(ULONG,AddRef)(THIS) PURE;
	STDMETHOD_(ULONG,Release)(THIS) PURE;
	//IUnknown - BEGIN

	//IDXCCPropertyCollection - BEGIN
	STDMETHOD_(UINT, NumProperties)(THIS) PURE;
	STDMETHOD_(DXCCPROPERTY_KEY, FirstKey)(THIS) PURE;
	STDMETHOD_(DXCCPROPERTY_KEY, EnumKeys)(THIS_ DXCCPROPERTY_KEY PreviousKey) PURE;
	
	STDMETHOD_(DXCCPROPERTY_DESC, GetPropertyDesc)(THIS_ DXCCPROPERTY_KEY Key) PURE;
	STDMETHOD(GetPropertyValueAsData)(THIS_ DXCCPROPERTY_KEY Key, LPCVOID* ppOut ) PURE;
	STDMETHOD(SetPropertyValueAsData)(THIS_ DXCCPROPERTY_KEY Key, DXCCPROPERTY_DESC desc, LPCVOID value ) PURE;
	STDMETHOD(GetPropertyValueAsInterface)(THIS_ DXCCPROPERTY_KEY Key, LPUNKNOWN* ppOut) PURE;
	STDMETHOD(SetPropertyValueAsInterface)(THIS_ DXCCPROPERTY_KEY Key, DXCCPROPERTY_DESC desc, LPUNKNOWN value) PURE;
	//IDXCCPropertyCollection - END


	STDMETHOD_(LPCSTR, GetPropertyName)(THIS_ DXCCPROPERTY_KEY Key) PURE;
	STDMETHOD_(DXCCPROPERTY_KEY, GetKey)(THIS_ LPCSTR name) PURE;

	STDMETHOD(AddProperty)(THIS_ LPCSTR pName, DXCCPROPERTY_KEY* pOutKey ) PURE;
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