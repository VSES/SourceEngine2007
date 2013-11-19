#ifndef DXCCEFFECTHELPERS_H
#define DXCCEFFECTHELPERS_H

//all basic windows sub-includes
//#include "windows.h"
//defines INITGUID and includes guiddef.h
#include "initguid.h"
#include "d3d9.h"
#include "d3dx9.h"

//BEGIN PROTOTYPES
interface IDXCCManager;
interface IDXCCResource;
interface IDXCCShaderProperties;
//END PROTOTYPES


typedef interface	IDXCCEnumEffectParameters	IDXCCEnumEffectParameters		,*LPDXCCENUMEFFECTPARAMETERS;
typedef interface	IDXCCEffectDlgCallbacks		IDXCCEffectDlgCallbacks			,*LPDXCCFXDLGCALLBACKS;

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

/*
VOID WINAPI
DXCCCreatePropertyBagFromEffect(LPD3DXEFFECT pEffect, LPDXCCPROPERTYBAG* ppBag);
*/

void
DXCCResolvePath(
	char* outFinalPath, 
	UINT inFinalPathLength, 
	const char* inOriginalPath,
	const char* inReplacementPath, 
	const char* inRelativePath);

D3DXHANDLE WINAPI
DXCCFindEffectAnnotation(
	LPD3DXEFFECT pEffect, 
	D3DXHANDLE hParameter, 
	LPCSTR szAnnotation);

HRESULT WINAPI
DXCCEffectRemoveResources(
        LPD3DXEFFECT pEffect,
        IDXCCManager* a_pManager);

HRESULT WINAPI
DXCCLoadEffectFromShaderProperties(
        IDXCCShaderProperties* a_Shader,
  		LPDIRECT3DDEVICE9 a_pDevice,
		LPD3DXINCLUDE pInclude,
		IDXCCManager* a_pManager,
		const char* a_SecondaryEffectDirectory, //load directory if the path is not complete
		const char* a_SecondaryTextureDirectory, //load directory if the path is not complete
    	LPDXCCRESOURCE* a_pOutResource,
        LPD3DXEFFECT* a_pOutEffect,
		LPD3DXBUFFER* a_errors);



HRESULT WINAPI
DXSASLoadEffectDefaults(
		LPDIRECT3DDEVICE9 a_pDevice,
		IDXCCManager* a_pManager,//for retrieval of effect and textures path
		LPD3DXEFFECT a_pEffect,
		IDXCCResource* a_pEffectResource,
		const char* a_SecondaryDirectory);




HRESULT WINAPI
DXCCEnumEffectParameters(
	LPD3DXEFFECT pEffect,
	LPDXCCENUMEFFECTPARAMETERS pEnum);


HRESULT WINAPI
DXCCShowEffectDialog(
 	HWND hParent,
 	LPDIRECT3DDEVICE9 pDevice,
	IDXCCManager* pManager,
 	IDXCCResource* pResource,
	LPDXCCFXDLGCALLBACKS pCallbacks,
 	BOOL bChangableEffect,
	BOOL bModal);

#ifdef __cplusplus
}
#endif //__cplusplus


enum DXCCEffectElementType
{
	DXCCEET_ROOT= 0,
	DXCCEET_ARRAYELEMENT= 1,
	DXCCEET_STRUCTMEMBER= 2,
};

struct EXPORTDEF DXCCEffectElement
{
	D3DXHANDLE				Handle;
	D3DXPARAMETER_DESC		Description;
	LPCSTR					ShortPathName; //Description.Name or [%d] used to construct LongPathName
	LPCSTR					LongPathName; //complete path string to reach this D3DXHANDLE
	UINT					Index; //MemberIndex or ElementIndex

	//FUNCTIONS
	UINT					NumChildren();
	DXCCEffectElementType	GetChildrenType();
	D3DXHANDLE				GetChild( LPD3DXEFFECT pEffect, UINT index);
};

struct EXPORTDEF DXCCEffectPath
{
	UINT					Length;
	DXCCEffectElement*		Root;//indexable array of 'Length' //NEVER NULL
	DXCCEffectElement*		Parent; //2nd to end of path//MAY BE NULL
	DXCCEffectElement*		End; //end of path //NEVER NULL
	
	//FUNCTIONS
	BOOL	IsArrayElement(UINT index);
	BOOL	EndIsArrayElement();
};


#undef INTERFACE
//THIS US A USER DEFINED CLASS USED AS A PARAMETER FOR DXCCEnumEffectParameters
#define INTERFACE IDXCCEnumEffectParameters
DECLARE_INTERFACE(IDXCCEnumEffectParameters)
{
	//Once at Start
	STDMETHOD(PreEnumeration)(THIS_  LPD3DXEFFECT pEffect, BOOL& GoDeeper) { return S_OK; };

	//Called before any children (member or element) are iterated 
	STDMETHOD(PreEnumParameter)(THIS_  LPD3DXEFFECT pEffect, DXCCEffectPath& path, BOOL& GoDeeper, BOOL& DoEnum, BOOL& DoPost ) { return S_OK; };

	//Called for every parameter
	STDMETHOD(EnumParameter)(THIS_  LPD3DXEFFECT pEffect, DXCCEffectPath& path, BOOL& GoDeeper, BOOL& DoPost) { return S_OK; };

	//Called after all children (member or element) are iterated 
	STDMETHOD(PostEnumParameter)(THIS_  LPD3DXEFFECT pEffect, DXCCEffectPath& path) { return S_OK; };

	//Once at End
	STDMETHOD(PostEnumeration)(THIS_  LPD3DXEFFECT pEffect) { return S_OK; };

};


#undef INTERFACE
#define INTERFACE IDXCCEFFECTDIALOGCALLBACKS
//all functions return E_NOTIMPL for default processing
DECLARE_INTERFACE(IDXCCEffectDlgCallbacks)
{
	//called after standard show procedures are performed
	//manipulate the dialog here such as embedding.
	//!!!!  OnShow !!!! must return TRUE for HandleOnShow to be called
	STDMETHOD_(BOOL, HandleOnShow)(THIS) { return FALSE; };
	STDMETHOD(OnShow)(THIS_  
 		LPDIRECT3DDEVICE9 pDevice,
 		IDXCCManager* pManager,
 		IDXCCResource* pResource) { return E_NOTIMPL; };


	//called immediately after the load button is pressed
	//!!!!  HandleOnPreLoad !!!! must return TRUE for OnPreLoad to be called
	STDMETHOD_(BOOL, HandleOnPreLoad)(THIS) { return FALSE; };
	STDMETHOD(OnPreLoad)(THIS_  
	 	HWND hDialog, 
 		LPDIRECT3DDEVICE9 pDevice,
 		IDXCCManager* pManager,
 		IDXCCResource* pResource,
 		LPCSTR szNewEffectFilename) { return E_NOTIMPL; };//return E_NOTIMPL for default processing


	//called as a replacement of the standard load...return E_NOTIMPL for default behavior
	//load the effect.
	//recycle the resource with the effect
	//add the appropriate resource path string to the resource
	//change the title of the hDialog
	//!!!!  HandleOnLoad !!!! must return TRUE for OnLoad to be called
	STDMETHOD_(BOOL, HandleOnLoad)(THIS) { return FALSE; }; //MUST OVERRIDE ME
	STDMETHOD(OnLoad)(THIS_  
	 	HWND hDialog, 
 		LPDIRECT3DDEVICE9 pDevice,
 		IDXCCManager* pManager,
 		IDXCCResource* pResource,
 		LPCSTR szNewEffectFilename) { return E_NOTIMPL; };//return E_NOTIMPL for default processing

	//called after load is performed
	//!!!!  HandleOnPostLoad !!!! must return TRUE for OnPostLoad to be called
	STDMETHOD_(BOOL, HandleOnPostLoad)(THIS) { return FALSE; };
	STDMETHOD(OnPostLoad)(THIS_  
	 	HWND hDialog, 
 		LPDIRECT3DDEVICE9 pDevice,
 		IDXCCManager* pManager,
 		IDXCCResource* pResource,//recycle me
 		LPCSTR szNewEffectFilename) { return E_NOTIMPL; };//return E_NOTIMPL for default processing

	//when the Parameter Data is applied
	//!!!!  HandleOnParameterApply !!!! must return TRUE for OnParameterApply to be called
	STDMETHOD_(BOOL, HandleOnParameterApply)(THIS) { return FALSE; };
	STDMETHOD(OnParameterApply)(THIS_  
	 	HWND hDialog, 
 		LPDIRECT3DDEVICE9 pDevice,
 		IDXCCManager* pManager,
 		IDXCCResource* pResource,
 		LPD3DXEFFECT pEffect,
 		D3DXHANDLE hParameter) { return E_NOTIMPL; }; //return E_NOTIMPL for default processing


	//when the Parameter is selected
	//!!!!  HandleOnParameterSelect !!!! must return TRUE for OnParameterSelect to be called
	STDMETHOD_(BOOL, HandleOnParameterSelect)(THIS) { return FALSE; };
	STDMETHOD(OnParameterSelect)(THIS_  
	 	HWND hDialog, 
 		LPDIRECT3DDEVICE9 pDevice,
 		IDXCCManager* pManager,
		IDXCCResource* pResource,
 		LPD3DXEFFECT pEffect,
 		D3DXHANDLE hNewParameter,
		HWND hParentWindow,
		HWND* hOutWindow) { return E_NOTIMPL; }; //return E_NOTIMPL for default processing


	//called before standard close procedures are performed
	//!!!!  HandleOnClose !!!! must return TRUE for OnClose to be called
	STDMETHOD_(BOOL, HandleOnClose)(THIS) { return FALSE; };
	STDMETHOD(OnClose)(THIS_  
	 	HWND hDialog, 
 		LPDIRECT3DDEVICE9 pDevice,
 		IDXCCManager* pManager,
 		IDXCCResource* pResource) { return E_NOTIMPL; };


};




#endif