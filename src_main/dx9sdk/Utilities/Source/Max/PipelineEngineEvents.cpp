#include "StdAfx.h"
#include "PipelineEngineEvents.h"

HRESULT CPipelineEngineEventsTrigger::RegisterEvents(CPipelineEngineEvents* pCall)
{
	UnregisterEvents(pCall);
	Events.push_back(pCall);
	return S_OK;
}

HRESULT CPipelineEngineEventsTrigger::UnregisterEvents(CPipelineEngineEvents* pCall) 
{
	for(UINT i= 0; 
		i < Events.size(); 
		i++)
	{
		const CPipelineEngineEvents* event= Events.at(i);
		if(event==pCall)
		{
			Events.erase( Events.begin( ) + i);
			return S_OK;
		}
	}
	
	return E_INVALIDARG;
}


HRESULT CPipelineEngineEventsTrigger::OnD3DDeviceCreate()
{
	HRESULT hr= S_OK;
	for(UINT i= 0; 
		i < Events.size() 
		&& hr == S_OK; 
		i++)
	{
		CPipelineEngineEvents* &event= Events.at(i);
		hr= event->OnD3DDeviceCreate();
	}
	return hr;
};

HRESULT CPipelineEngineEventsTrigger::OnD3DDeviceReset()
{
	HRESULT hr= S_OK;
	for(UINT i= 0; 
		i < Events.size() 
		&& hr == S_OK; 
		i++)
	{
		CPipelineEngineEvents* &event= Events.at(i);
		hr= event->OnD3DDeviceReset();
	}
	return hr;
};

HRESULT CPipelineEngineEventsTrigger::OnD3DDeviceLost()
{
	HRESULT hr= S_OK;
	for(UINT i= 0; 
		i < Events.size() 
		&& hr == S_OK; 
		i++)
	{
		CPipelineEngineEvents* &event= Events.at(i);
		hr= event->OnD3DDeviceLost();
	}
	return hr;
};

HRESULT CPipelineEngineEventsTrigger::OnD3DDeviceDestroy()
{
	HRESULT hr= S_OK;
	for(UINT i= 0;
		i < Events.size() 
		&& hr == S_OK; 
		i++)
	{
		CPipelineEngineEvents* &event= Events.at(i);
		hr= event->OnD3DDeviceDestroy();
	}
	return hr;
};
