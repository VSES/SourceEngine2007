#include "pch.h"

HRESULT CEngineEventsTrigger::RegisterEvents(CEngineEvents* pCall)
{
	UnregisterEvents(pCall);
	Events.push_back(pCall);
	return S_OK;
}

HRESULT CEngineEventsTrigger::UnregisterEvents(CEngineEvents* pCall) 
{
	for(UINT i= 0; 
		i < Events.size(); 
		i++)
	{
		const CEngineEvents* event= Events.at(i);
		if(event==pCall)
		{
			Events.erase( Events.begin( ) + i);
			return S_OK;
		}
	}
	
	return E_INVALIDARG;
}


HRESULT CEngineEventsTrigger::OnD3DDeviceCreate()
{
	HRESULT hr= S_OK;
	for(UINT i= 0; 
		i < Events.size() 
		&& hr == S_OK; 
		i++)
	{
		CEngineEvents* &event= Events.at(i);
		hr= event->OnD3DDeviceCreate();
	}
	return hr;
};

HRESULT CEngineEventsTrigger::OnD3DDeviceReset()
{
	HRESULT hr= S_OK;
	for(UINT i= 0; 
		i < Events.size() 
		&& hr == S_OK; 
		i++)
	{
		CEngineEvents* &event= Events.at(i);
		hr= event->OnD3DDeviceReset();
	}
	return hr;
};

HRESULT CEngineEventsTrigger::OnD3DDeviceLost()
{
	HRESULT hr= S_OK;
	for(UINT i= 0; 
		i < Events.size() 
		&& hr == S_OK; 
		i++)
	{
		CEngineEvents* &event= Events.at(i);
		hr= event->OnD3DDeviceLost();
	}
	return hr;
};

HRESULT CEngineEventsTrigger::OnD3DDeviceDestroy()
{
	HRESULT hr= S_OK;
	for(UINT i= 0;
		i < Events.size() 
		&& hr == S_OK; 
		i++)
	{
		CEngineEvents* &event= Events.at(i);
		hr= event->OnD3DDeviceDestroy();
	}
	return hr;
};
