#ifndef DXCCVERTEXBUNDLER_H
#define DXCCVERTEXBUNDLER_H

//all basic windows sub-includes
//#include "windows.h"
//defines INITGUID and includes guiddef.h
#include "initguid.h"
#include "d3d9.h"
#include "d3dx9.h"

//BEGIN PROTOTYPES
interface IDXCCDeclaration;
struct DXCCFLOAT4;
//END PROTOTYPES

interface __declspec( uuid("c39daab3-5e71-43ba-9218-d25c947d746f") ) IDXCCVertexBundler;
DEFINE_GUID( IID_IDXCCVertexBundler, 
			0xc39daab3, 
			0x5e71, 
			0x43ba, 
			0x92, 0x18, 0xd2, 0x5c, 0x94, 0x7d, 0x74, 0x6f);

typedef interface	IDXCCVertexBundler			IDXCCVertexBundler				,*LPDXCCVERTEXBUNDLER;

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

HRESULT WINAPI
	DXCCCreateVertexBundler(
 	UINT NumVertices,
 	D3DVERTEXELEMENT9* pDecl, 
	LPDXCCVERTEXBUNDLER* ppVBundle);

#ifdef __cplusplus
}
#endif //__cplusplus

#undef INTERFACE
#define INTERFACE IDXCCVertexBundler
//simple encapsilation of vertex streams
//this is an interface to simply read/write all of the vertex data associated with a single declaration.
//the class holds all of the stream information and vertex buffers.
//but no functionality to set the streams....see derived classes
//you can read the declaration but not change it.
//then you can edit the data by element for each vertex of the appropriate vertex buffer.
DECLARE_INTERFACE_(IDXCCVertexBundler, IUnknown)
{
	STDMETHOD(QueryInterface)(THIS_  REFIID riid,  void** ppvObj) PURE;
	STDMETHOD_(ULONG,AddRef)(THIS) PURE;
	STDMETHOD_(ULONG,Release)(THIS) PURE;

	//E_ACCESSDENIED may be returned when the structure is embedded
	STDMETHOD(Recycle)(THIS_ 
 		UINT NumVertices,
 		LPD3DVERTEXELEMENT9 pDecl) PURE;

	STDMETHOD(GetVertexBuffer)(THIS_ 
		 UINT iStream, 
		 LPDIRECT3DVERTEXBUFFER9* ppVB) PURE;

	STDMETHOD(GetDeclaration)(THIS_ 
		 IDXCCDeclaration** ppDeclRead) PURE;
	
	STDMETHOD_(DWORD, NumVertices)(THIS) PURE;

	//Get data based on arbitrary vertex element
	STDMETHOD(GetData)(THIS_ 
		 UINT iVertex, 
		 UINT iElem, 
		 DXCCFLOAT4* pVED) PURE;

	//Get data based on standard vertex element	
	STDMETHOD(GetPosition)(THIS_ 
		 UINT iVertex, 
		 DXCCFLOAT4* pVED) PURE;

	STDMETHOD(GetPointSize)(THIS_ 
		 UINT iVertex, 
		 DXCCFLOAT4* pVED) PURE;

	STDMETHOD(GetNormal)(THIS_ 
		 UINT iVertex, 
		 DXCCFLOAT4* pVED) PURE;

	STDMETHOD(GetTangent)(THIS_ 
		 UINT iVertex, 
		 DXCCFLOAT4* pVED) PURE;

	STDMETHOD(GetBinormal)(THIS_ 
		 UINT iVertex, 
		 DXCCFLOAT4* pVED) PURE;

	STDMETHOD(GetDiffuse)(THIS_ 
		 UINT iVertex, 
		 DXCCFLOAT4* pVED) PURE;

	STDMETHOD(GetSpecular)(THIS_ 
		 UINT iVertex, 
		 DXCCFLOAT4* pVED) PURE;

	STDMETHOD(GetTexcoord)(THIS_ 
		 UINT iVertex, 
		 UINT iTexcoord,  
		 DXCCFLOAT4* pVED) PURE;

	STDMETHOD(GetBlendWeights)(THIS_ 
		 UINT iVertex, 
		 DXCCFLOAT4* pVED) PURE;

	STDMETHOD(GetBlendIndices)(THIS_ 
		 UINT iVertex, 
		 DXCCFLOAT4* pVED) PURE;

	//Set data based on arbitrary vertex element
	STDMETHOD(SetData)(THIS_ 
		 UINT iVertex, 
		 UINT iElem, 
		 DXCCFLOAT4* pVED) PURE;

	//Set data based on standard vertex element	
	STDMETHOD(SetPosition)(THIS_ 
		 UINT iVertex,  
		 DXCCFLOAT4* pVED) PURE;

	STDMETHOD(SetPointSize)(THIS_ 
		 UINT iVertex,  
		 DXCCFLOAT4* pVED) PURE;

	STDMETHOD(SetNormal)(THIS_ 
		 UINT iVertex,  
		 DXCCFLOAT4* pVED) PURE;

	STDMETHOD(SetTangent)(THIS_ 
		 UINT iVertex,  
		 DXCCFLOAT4* pVED) PURE;

	STDMETHOD(SetBinormal)(THIS_ 
		 UINT iVertex,  
		 DXCCFLOAT4* pVED) PURE;

	STDMETHOD(SetDiffuse)(THIS_ 
		 UINT iVertex,  
		 DXCCFLOAT4* pVED) PURE;

	STDMETHOD(SetSpecular)(THIS_ 
		 UINT iVertex,  
		 DXCCFLOAT4* pVED) PURE;

	STDMETHOD(SetTexcoord)(THIS_ 
		 UINT iVertex,  
		 UINT iTexcoord,   
		 DXCCFLOAT4* pVED) PURE;

	STDMETHOD(SetBlendWeights)(THIS_ 
		 UINT iVertex,  
		 DXCCFLOAT4* pVED) PURE;

	STDMETHOD(SetBlendIndices)(THIS_ 
		 UINT iVertex,  
		 DXCCFLOAT4* pVED) PURE;

	//Lock down the vertex buffer.  set up one of 16 vertex streams with full vertex stream data
	STDMETHOD(Lock)(THIS_  
		 UINT iStream) PURE;

	STDMETHOD(Unlock)(THIS_  
		 UINT iStream) PURE;

	STDMETHOD(LockAll)(THIS) PURE;

	STDMETHOD(UnlockAll)(THIS) PURE;

	STDMETHOD(SetStream)(THIS_  
		 UINT iStream,  //what stream index do you want? ...see vertex declaration elements
		 UINT OffsetInBytes,  //how far from the beginning of the vertex buffer does the data start
		 UINT Stride,  //how far between vertices
		 UINT iFrequencyDivider,	//instancing	(default is 1)
		 CONST LPDIRECT3DVERTEXBUFFER9 pVB) PURE;
};


#endif
