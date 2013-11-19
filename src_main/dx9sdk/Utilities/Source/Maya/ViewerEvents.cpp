#include "pch.h"

HRESULT CViewerEventsTrigger::RegisterEvents(CViewerEvents* pCall)
{
	UnregisterEvents(pCall);
	Events.push_back(pCall);
	return S_OK;
}

HRESULT CViewerEventsTrigger::UnregisterEvents(CViewerEvents* pCall)
{
	for(UINT i= 0; 
		i < Events.size(); 
		i++)
	{
		const CViewerEvents* event= Events.at(i);
		if(event==pCall)
		{
			Events.erase( Events.begin( ) + i);
			return S_OK;
		}
	}
	
	return E_INVALIDARG;
}



HRESULT CViewerEventsTrigger::OnFrameChildAdded(IDXCCFrame* pParent, IDXCCFrame* pChild)
{
	HRESULT hr= S_OK;
	for(UINT i= 0; 
		i < Events.size() 
		&& hr == S_OK; 
		i++)
	{
		CViewerEvents* &event= Events.at(i);
		hr= event->OnFrameChildAdded(pParent, pChild);
	}
	return hr;
};

HRESULT CViewerEventsTrigger::OnFrameChildRemoved(IDXCCFrame* pParent, IDXCCFrame* pChild) 
{
	HRESULT hr= S_OK;
	for(UINT i= 0; 
		i < Events.size() 
		&& hr == S_OK; 
		i++)
	{
		CViewerEvents* &event= Events.at(i);
		hr= event->OnFrameChildRemoved(pParent, pChild);
	}
	return hr;
};

HRESULT CViewerEventsTrigger::OnFrameMemberAdded(IDXCCFrame* pParent, IUnknown* pChild)
{
	HRESULT hr= S_OK;
	for(UINT i= 0; 
		i < Events.size() 
		&& hr == S_OK; 
		i++)
	{
		CViewerEvents* &event= Events.at(i);
		hr= event->OnFrameMemberAdded(pParent, pChild);
	}
	return hr;
};

HRESULT CViewerEventsTrigger::OnFrameMemberRemoved(IDXCCFrame* pParent, IUnknown* pChild)
{
	HRESULT hr= S_OK;
	for(UINT i= 0; 
		i < Events.size() 
		&& hr == S_OK; 
		i++)
	{
		CViewerEvents* &event= Events.at(i);
		hr= event->OnFrameMemberRemoved(pParent, pChild);
	}
	return hr;
};

HRESULT CViewerEventsTrigger::OnMeshChange(IDXCCMesh* pMesh)
{
	HRESULT hr= S_OK;
	for(UINT i= 0; 
		i < Events.size() 
		&& hr == S_OK; 
		i++)
	{
		CViewerEvents* &event= Events.at(i);
		hr= event->OnMeshChange(pMesh);
	}
	return hr;
};

HRESULT CViewerEventsTrigger::OnMeshDeclarationChange(IDXCCMesh* pMesh, IDXCCMesh* pOldMesh)
{
	HRESULT hr= S_OK;
	for(UINT i= 0; 
		i < Events.size() 
		&& hr == S_OK; 
		i++)
	{
		CViewerEvents* &event= Events.at(i);
		hr= event->OnMeshDeclarationChange(pMesh, pOldMesh);
	}
	return hr;
};

HRESULT CViewerEventsTrigger::OnMeshTopologyChange(IDXCCMesh* pMesh, IDXCCMesh* pOldMesh)
{
	HRESULT hr= S_OK;
	for(UINT i= 0; 
		i < Events.size() 
		&& hr == S_OK; 
		i++)
	{
		CViewerEvents* &event= Events.at(i);
		hr= event->OnMeshTopologyChange(pMesh, pOldMesh);
	}
	return hr;
};

HRESULT CViewerEventsTrigger::OnMeshVertexChange(IDXCCMesh* pMesh, UINT vertMin, UINT vertMax)
{
	HRESULT hr= S_OK;
	for(UINT i= 0; 
		i < Events.size() 
		&& hr == S_OK; 
		i++)
	{
		CViewerEvents* &event= Events.at(i);
		hr= event->OnMeshVertexChange(pMesh, vertMin, vertMax);
	}
	return hr;
};

HRESULT CViewerEventsTrigger::OnMeshSubVertexChange(IDXCCMesh* pMesh, D3DDECLUSAGE Usage, UINT UsageIndex, UINT vertMin, UINT vertMax)
{
	HRESULT hr= S_OK;
	for(UINT i= 0; 
		i < Events.size() 
		&& hr == S_OK; 
		i++)
	{
		CViewerEvents* &event= Events.at(i);
		hr= event->OnMeshSubVertexChange(pMesh, Usage, UsageIndex, vertMin, vertMax);
	}
	return hr;
};
HRESULT CViewerEventsTrigger::OnMeshAttributeChange(IDXCCMesh* pMesh, UINT faceMin, UINT faceMax)
{
	HRESULT hr= S_OK;
	for(UINT i= 0; 
		i < Events.size() 
		&& hr == S_OK; 
		i++)
	{
		CViewerEvents* &event= Events.at(i);
		hr= event->OnMeshAttributeChange(pMesh, faceMin, faceMax);
	}
	return hr;
};

HRESULT CViewerEventsTrigger::OnMeshMaterialChange(IDXCCMesh* pMesh, DWORD Attrib, ID3DXEffect* pOldMaterial)
{
	HRESULT hr= S_OK;
	for(UINT i= 0; 
		i < Events.size() 
		&& hr == S_OK; 
		i++)
	{
		CViewerEvents* &event= Events.at(i);
		hr= event->OnMeshMaterialChange(pMesh, Attrib, pOldMaterial);
	}
	return hr;
};

HRESULT CViewerEventsTrigger::OnMaterialEffectChange(ID3DXEffect* pMaterial, LPD3DXEFFECT pOldEffect)
{
	HRESULT hr= S_OK;
	for(UINT i= 0; 
		i < Events.size() 
		&& hr == S_OK; 
		i++)
	{
		CViewerEvents* &event= Events.at(i);
		hr= event->OnMaterialEffectChange(pMaterial, pOldEffect);
	}
	return hr;
};

HRESULT CViewerEventsTrigger::OnMaterialParameterChange(ID3DXEffect* pMaterial, D3DXHANDLE hParameter)
{
	HRESULT hr= S_OK;
	for(UINT i= 0; 
		i < Events.size() 
		&& hr == S_OK; 
		i++)
	{
		CViewerEvents* &event= Events.at(i);
		hr= event->OnMaterialParameterChange(pMaterial, hParameter);
	}
	return hr;
};

