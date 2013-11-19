#ifndef VIEWEREVENTS_H
#define VIEWEREVENTS_H

#include "windows.h"
#include "d3dx9.h"
#include "dxcc.h"
#include <vector>

class CViewerEvents
{
public:

	virtual HRESULT OnFrameChildAdded(IDXCCFrame* pParent, IDXCCFrame* pChild) { return S_OK; };
	virtual HRESULT OnFrameChildRemoved(IDXCCFrame* pParent, IDXCCFrame* pChild) { return S_OK; };

	virtual HRESULT OnFrameMemberAdded(IDXCCFrame* pParent, IUnknown* pChild) { return S_OK; };
	virtual HRESULT OnFrameMemberRemoved(IDXCCFrame* pParent, IUnknown* pChild) { return S_OK; };

	virtual HRESULT OnMeshChange(IDXCCMesh* pMesh){ return S_OK; };

	virtual HRESULT OnMeshDeclarationChange(IDXCCMesh* pMesh, IDXCCMesh* pOldMesh){ return S_OK; };
	//topology has change, regenerate mesh
	virtual HRESULT OnMeshTopologyChange(IDXCCMesh* pMesh, IDXCCMesh* pOldMesh){ return S_OK; };
	//vertices has changed so updates that range with the given Usage info
	virtual HRESULT OnMeshVertexChange(IDXCCMesh* pMesh, UINT vertMin, UINT vertMax){ return S_OK; };
	virtual HRESULT OnMeshSubVertexChange(IDXCCMesh* pMesh, D3DDECLUSAGE Usage, UINT UsageIndex, UINT vertMin, UINT vertMax){ return S_OK; };
	//the attributes used to associate a material to the mesh have changed
	virtual HRESULT OnMeshAttributeChange(IDXCCMesh* pMesh, UINT faceMin, UINT faceMax){ return S_OK; };
	//the materials associated to the attributes have changed
	virtual HRESULT OnMeshMaterialChange(IDXCCMesh* pMesh, DWORD Attrib, ID3DXEffect* pOldMaterial){ return S_OK; };
	//The material's Effect interface has changed (commonly used to unbind from Standard Semantics detabase)
	virtual HRESULT OnMaterialEffectChange(LPD3DXEFFECT pMaterial, LPD3DXEFFECT pOldEffect){ return S_OK; };
	//The material's Effect interface has changed (commonly used to unbind from Standard Semantics detabase)
	virtual HRESULT OnMaterialParameterChange(LPD3DXEFFECT pMaterial, D3DXHANDLE hParameter){ return S_OK; };
};


class CViewerEventsTrigger : public CViewerEvents
{
	std::vector<CViewerEvents*> Events;
public:
	
	HRESULT RegisterEvents(CViewerEvents* pCall);
	HRESULT UnregisterEvents(CViewerEvents* pCall);


	HRESULT OnFrameChildAdded(IDXCCFrame* pParent, IDXCCFrame* pChild);//EMITTED
	HRESULT OnFrameChildRemoved(IDXCCFrame* pParent, IDXCCFrame* pChild) ;//EMITTED
	HRESULT OnFrameMemberAdded(IDXCCFrame* pParent, IUnknown* pChild);//EMITTED
	HRESULT OnFrameMemberRemoved(IDXCCFrame* pParent, IUnknown* pChild);//EMITTED
	HRESULT OnMeshChange(IDXCCMesh* pMesh);//EMITTED
	HRESULT OnMeshDeclarationChange(IDXCCMesh* pMesh, IDXCCMesh* pOldMesh);
	HRESULT OnMeshTopologyChange(IDXCCMesh* pMesh, IDXCCMesh* pOldMesh);
	HRESULT OnMeshVertexChange(IDXCCMesh* pMesh, UINT vertMin, UINT vertMax);
	HRESULT OnMeshSubVertexChange(IDXCCMesh* pMesh, D3DDECLUSAGE Usage, UINT UsageIndex, UINT vertMin, UINT vertMax);
	HRESULT OnMeshAttributeChange(IDXCCMesh* pMesh, UINT faceMin, UINT faceMax);
	HRESULT OnMeshMaterialChange(IDXCCMesh* pMesh, DWORD Attrib, LPD3DXEFFECT pOldMaterial);
	HRESULT OnMaterialEffectChange(LPD3DXEFFECT pMaterial, LPD3DXEFFECT pOldEffect);
	HRESULT OnMaterialParameterChange(LPD3DXEFFECT pMaterial, D3DXHANDLE hParameter);
};

#endif

