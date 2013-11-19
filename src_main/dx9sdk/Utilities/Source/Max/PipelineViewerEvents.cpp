#include "StdAfx.h"
#include "PipelineViewerEvents.h"

HRESULT CPipelineViewerEventsTrigger::RegisterEvents(CPipelineViewerEvents* pCall)
{
	UnregisterEvents(pCall);
	Events.push_back(pCall);
	return S_OK;
}

HRESULT CPipelineViewerEventsTrigger::UnregisterEvents(CPipelineViewerEvents* pCall)
{
	for(UINT i= 0; 
		i < Events.size(); 
		i++)
	{
		const CPipelineViewerEvents* event= Events.at(i);
		if(event==pCall)
		{
			Events.erase( Events.begin( ) + i);
			return S_OK;
		}
	}
	
	return E_INVALIDARG;
}



HRESULT CPipelineViewerEventsTrigger::OnFrameChildAdded(IDXCCFrame* pParent, IDXCCFrame* pChild)
{
	HRESULT hr= S_OK;
	for(UINT i= 0; 
		i < Events.size() 
		&& hr == S_OK; 
		i++)
	{
		CPipelineViewerEvents* &event= Events.at(i);
		hr= event->OnFrameChildAdded(pParent, pChild);
	}
	return hr;
};

HRESULT CPipelineViewerEventsTrigger::OnFrameChildRemoved(IDXCCFrame* pParent, IDXCCFrame* pChild) 
{
	HRESULT hr= S_OK;
	for(UINT i= 0; 
		i < Events.size() 
		&& hr == S_OK; 
		i++)
	{
		CPipelineViewerEvents* &event= Events.at(i);
		hr= event->OnFrameChildRemoved(pParent, pChild);
	}
	return hr;
};

HRESULT CPipelineViewerEventsTrigger::OnFrameMemberAdded(IDXCCFrame* pParent, IUnknown* pChild)
{
	HRESULT hr= S_OK;
	for(UINT i= 0; 
		i < Events.size() 
		&& hr == S_OK; 
		i++)
	{
		CPipelineViewerEvents* &event= Events.at(i);
		hr= event->OnFrameMemberAdded(pParent, pChild);
	}
	return hr;
};

HRESULT CPipelineViewerEventsTrigger::OnFrameMemberRemoved(IDXCCFrame* pParent, IUnknown* pChild)
{
	HRESULT hr= S_OK;
	for(UINT i= 0; 
		i < Events.size() 
		&& hr == S_OK; 
		i++)
	{
		CPipelineViewerEvents* &event= Events.at(i);
		hr= event->OnFrameMemberRemoved(pParent, pChild);
	}
	return hr;
};

HRESULT CPipelineViewerEventsTrigger::OnMeshChange(IDXCCMesh* pMesh)
{
	HRESULT hr= S_OK;
	for(UINT i= 0; 
		i < Events.size() 
		&& hr == S_OK; 
		i++)
	{
		CPipelineViewerEvents* &event= Events.at(i);
		hr= event->OnMeshChange(pMesh);
	}
	return hr;
};

HRESULT CPipelineViewerEventsTrigger::OnMeshDeclarationChange(IDXCCMesh* pMesh, IDXCCMesh* pOldMesh)
{
	HRESULT hr= S_OK;
	for(UINT i= 0; 
		i < Events.size() 
		&& hr == S_OK; 
		i++)
	{
		CPipelineViewerEvents* &event= Events.at(i);
		hr= event->OnMeshDeclarationChange(pMesh, pOldMesh);
	}
	return hr;
};

HRESULT CPipelineViewerEventsTrigger::OnMeshTopologyChange(IDXCCMesh* pMesh, IDXCCMesh* pOldMesh)
{
	HRESULT hr= S_OK;
	for(UINT i= 0; 
		i < Events.size() 
		&& hr == S_OK; 
		i++)
	{
		CPipelineViewerEvents* &event= Events.at(i);
		hr= event->OnMeshTopologyChange(pMesh, pOldMesh);
	}
	return hr;
};

HRESULT CPipelineViewerEventsTrigger::OnMeshVertexChange(IDXCCMesh* pMesh, UINT vertMin, UINT vertMax)
{
	HRESULT hr= S_OK;
	for(UINT i= 0; 
		i < Events.size() 
		&& hr == S_OK; 
		i++)
	{
		CPipelineViewerEvents* &event= Events.at(i);
		hr= event->OnMeshVertexChange(pMesh, vertMin, vertMax);
	}
	return hr;
};

HRESULT CPipelineViewerEventsTrigger::OnMeshSubVertexChange(IDXCCMesh* pMesh, D3DDECLUSAGE Usage, UINT UsageIndex, UINT vertMin, UINT vertMax)
{
	HRESULT hr= S_OK;
	for(UINT i= 0; 
		i < Events.size() 
		&& hr == S_OK; 
		i++)
	{
		CPipelineViewerEvents* &event= Events.at(i);
		hr= event->OnMeshSubVertexChange(pMesh, Usage, UsageIndex, vertMin, vertMax);
	}
	return hr;
};
HRESULT CPipelineViewerEventsTrigger::OnMeshAttributeChange(IDXCCMesh* pMesh, UINT faceMin, UINT faceMax)
{
	HRESULT hr= S_OK;
	for(UINT i= 0; 
		i < Events.size() 
		&& hr == S_OK; 
		i++)
	{
		CPipelineViewerEvents* &event= Events.at(i);
		hr= event->OnMeshAttributeChange(pMesh, faceMin, faceMax);
	}
	return hr;
};

HRESULT CPipelineViewerEventsTrigger::OnMeshMaterialChange(IDXCCMesh* pMesh, DWORD Attrib, LPDXCCSHADERPROPERTIES pOldShader/*, ID3DXEffect* pOldMaterial*/)
{
	HRESULT hr= S_OK;
	for(UINT i= 0; 
		i < Events.size() 
		&& hr == S_OK; 
		i++)
	{
		CPipelineViewerEvents* &event= Events.at(i);
		hr= event->OnMeshMaterialChange(pMesh, Attrib, pOldShader);
	}
	return hr;
};

HRESULT CPipelineViewerEventsTrigger::OnMaterialShaderChange(LPDXCCSHADERPROPERTIES /*ID3DXEffect* */ pMaterial, LPDXCCSHADERPROPERTIES pOldShader/*, LPD3DXEFFECT pOldEffect*/)
{
	HRESULT hr= S_OK;
	for(UINT i= 0; 
		i < Events.size() 
		&& hr == S_OK; 
		i++)
	{
		CPipelineViewerEvents* &event= Events.at(i);
		hr= event->OnMaterialShaderChange(pMaterial, pOldShader);
	}
	return hr;
};

HRESULT CPipelineViewerEventsTrigger::OnMaterialParameterChange(LPDXCCSHADERPROPERTIES pMaterial/* ID3DXEffect* pMaterial*/, D3DXHANDLE hParameter)
{
	HRESULT hr= S_OK;
	for(UINT i= 0; 
		i < Events.size() 
		&& hr == S_OK; 
		i++)
	{
		CPipelineViewerEvents* &event= Events.at(i);
		hr= event->OnMaterialParameterChange(pMaterial, hParameter);
	}
	return hr;
};

