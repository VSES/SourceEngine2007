#ifndef __PipelineViewerEvents_h__
#define __PipelineViewerEvents_h__

class CPipelineViewerEvents
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
	virtual HRESULT OnMeshMaterialChange(IDXCCMesh* pMesh, DWORD Attrib, LPDXCCSHADERPROPERTIES pOldMaterial/*, ID3DXEffect* pOldMaterial*/){ return S_OK; };
	//The material's Effect interface has changed (commonly used to unbind from Standard Semantics detabase)
	virtual HRESULT OnMaterialShaderChange(LPDXCCSHADERPROPERTIES /*LPD3DXEFFECT*/ pMaterial, LPDXCCSHADERPROPERTIES pOldShader/*, LPD3DXEFFECT pOldEffect*/){ return S_OK; };
	//The material's Effect interface has changed (commonly used to unbind from Standard Semantics detabase)
	virtual HRESULT OnMaterialParameterChange(LPDXCCSHADERPROPERTIES /*LPD3DXEFFECT*/ pMaterial, D3DXHANDLE hParameter){ return S_OK; };
};


class CPipelineViewerEventsTrigger : public CPipelineViewerEvents
{
	std::vector<CPipelineViewerEvents*> Events;
public:
	
	HRESULT RegisterEvents(CPipelineViewerEvents* pCall);
	HRESULT UnregisterEvents(CPipelineViewerEvents* pCall);


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
	HRESULT OnMeshMaterialChange(IDXCCMesh* pMesh, DWORD Attrib, LPDXCCSHADERPROPERTIES pOldShader/*, LPD3DXEFFECT pOldMaterial*/);
	HRESULT OnMaterialShaderChange(LPDXCCSHADERPROPERTIES /*LPD3DXEFFECT*/ pMaterial, LPDXCCSHADERPROPERTIES pOldShader /*LPD3DXEFFECT pOldEffect*/);
	HRESULT OnMaterialParameterChange(LPDXCCSHADERPROPERTIES /*LPD3DXEFFECT*/ pMaterial, D3DXHANDLE hParameter);
};

#endif

