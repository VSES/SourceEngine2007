#ifndef DXCCPROPERTYCOLLECTION_H
#define DXCCPROPERTYCOLLECTION_H

//all basic windows sub-includes
//#include "windows.h"
//defines INITGUID and includes guiddef.h
#include "initguid.h"
#include "time.h"
#include "DXCCProperty.h"

typedef interface	IDXCCPropertyCollection			IDXCCPropertyCollection				,*LPDXCCPROPERTYCOLLECTION;
typedef interface	IDXCCEnumProperties				IDXCCEnumProperties					,*LPDXCCENUMPROPERTIES;

interface __declspec( uuid("8FF15262-18C2-4985-AA69-1E6A82983BB6") ) IDXCCPropertyCollection;
DEFINE_GUID(IID_IDXCCPropertyCollection, 
            0x8ff15262, 
            0x18c2, 
            0x4985, 
            0xaa, 0x69, 0x1e, 0x6a, 0x82, 0x98, 0x3b, 0xb6);


typedef UINT_PTR DXCCPROPERTY_KEY;

#define DXCCPROPERTY_KEY_NULL ((UINT_PTR)0)


#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

HRESULT WINAPI
DXCCEnumProperties(
	LPDXCCPROPERTYCOLLECTION pCollection,
	LPDXCCENUMPROPERTIES pEnum);

#ifdef __cplusplus
}
#endif //__cplusplus

#undef INTERFACE
#define INTERFACE IDXCCPropertyCollection
DECLARE_INTERFACE_(IDXCCPropertyCollection, IUnknown)
{
	STDMETHOD(QueryInterface)(THIS_  REFIID riid,  void** ppvObj) PURE;
	STDMETHOD_(ULONG,AddRef)(THIS) PURE;
	STDMETHOD_(ULONG,Release)(THIS) PURE;

	STDMETHOD_(UINT, NumProperties)(THIS) PURE;
	STDMETHOD_(DXCCPROPERTY_KEY, FirstKey)(THIS) PURE;
	STDMETHOD_(DXCCPROPERTY_KEY, EnumKeys)(THIS_ DXCCPROPERTY_KEY PreviousKey) PURE;
	
	STDMETHOD_(DXCCPROPERTY_DESC, GetPropertyDesc)(THIS_ DXCCPROPERTY_KEY Key) PURE;
	STDMETHOD(GetPropertyValueAsData)(THIS_ DXCCPROPERTY_KEY Key, LPCVOID* ppOut ) PURE;
	STDMETHOD(SetPropertyValueAsData)(THIS_ DXCCPROPERTY_KEY Key, DXCCPROPERTY_DESC desc, LPCVOID value ) PURE;
	STDMETHOD(GetPropertyValueAsInterface)(THIS_ DXCCPROPERTY_KEY Key, LPUNKNOWN* ppOut) PURE;
	STDMETHOD(SetPropertyValueAsInterface)(THIS_ DXCCPROPERTY_KEY Key, DXCCPROPERTY_DESC desc, LPUNKNOWN value) PURE;

	STDMETHOD_(void, GetRoot)(THIS_ LPDXCCPROPERTYCOLLECTION* ppCollection) PURE;	
	STDMETHOD_(void, GetParent)(THIS_ LPDXCCPROPERTYCOLLECTION* ppCollection) PURE;
	STDMETHOD_(DWORD, GetSchemaHash)(THIS) PURE;
	STDMETHOD_(clock_t, LastChanged)(THIS) PURE;

};

struct EXPORTDEF DXCCPropertyElement
{
	LPDXCCPROPERTYCOLLECTION	Collection;
	DXCCPROPERTY_KEY			Key;
	LPCSTR						ShortPathName; //Description.Name or [%d] used to construct LongPathName
	LPCSTR						LongPathName; //complete path string to reach this D3DXHANDLE

	UINT						Index; //MemberIndex or ElementIndex

    BOOL    IsLeaf();
};

struct EXPORTDEF DXCCPropertyPath
{
	UINT					Length;
	DXCCPropertyElement*	Root;//indexable array of 'Length' //NEVER NULL
	DXCCPropertyElement*	Parent; //2nd to end of path//MAY BE NULL
	DXCCPropertyElement*	End; //end of path //NEVER NULL

	//FUNCTIONS
	BOOL	IsArrayElement(UINT index);
	BOOL	EndIsArrayElement();
};


#undef INTERFACE
//THIS US A USER DEFINED CLASS USED AS A PARAMETER FOR DXCCEnumEffectParameters
#define INTERFACE IDXCCEnumProperties
DECLARE_INTERFACE(IDXCCEnumProperties)
{
	//Once at Start
	STDMETHOD(PreEnumeration)(THIS_  LPDXCCPROPERTYCOLLECTION pCollection, BOOL& GoDeeper) { return S_OK; };

	//Called before any children (member or element) are iterated 
	STDMETHOD(PreEnumProperty)(THIS_  DXCCPropertyPath& path, BOOL& GoDeeper, BOOL& DoEnum, BOOL& DoPost ) { return S_OK; };

	//Called for every parameter
	STDMETHOD(EnumProperty)(THIS_  DXCCPropertyPath& path, BOOL& GoDeeper, BOOL& DoPost) { return S_OK; };

	//Called after all children (member or element) are iterated 
	STDMETHOD(PostEnumProperty)(THIS_  DXCCPropertyPath& path) { return S_OK; };

	//Once at End
	STDMETHOD(PostEnumeration)(THIS_  LPDXCCPROPERTYCOLLECTION pCollection) { return S_OK; };

};


#endif