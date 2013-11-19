#include "StdAfx.h"
#include "PipelineManagerEvents.h"

HRESULT CPipelineManagerEventsTrigger::OnReload()
{
	HRESULT hr= S_OK;
	for(UINT i= 0; 
		i < Events.size() 
		&& hr == S_OK; 
		i++)
	{
		CPipelineManagerEvents* &event= Events.at(i);
		hr= event->OnReload();
	}
	return hr;
};

HRESULT CPipelineManagerEventsTrigger::OnResourceAdd(IDXCCResource* pRes)
{
	HRESULT hr= S_OK;
	for(UINT i= 0; 
		i < Events.size() 
		&& hr == S_OK; 
		i++)
	{
		CPipelineManagerEvents* &event= Events.at(i);
		hr= event->OnResourceAdd(pRes);
	}
	return hr;
};


CPipelineManagerEvents::CPipelineManagerEvents()
{
	ExclusiveModeMutex= CreateMutex(NULL, false, NULL);
}

CPipelineManagerEvents::~CPipelineManagerEvents()
{
	CloseHandle(ExclusiveModeMutex);

}


HRESULT CPipelineManagerEventsTrigger::OnResourceRecycle(IDXCCResource* pRes)
{
	HRESULT hr= S_OK;
	for(UINT i= 0; 
		i < Events.size() 
		&& hr == S_OK; 
		i++)
	{
		CPipelineManagerEvents* &event= Events.at(i);
		hr= event->OnResourceRecycle(pRes);
	}
	return hr;
};

HRESULT CPipelineManagerEventsTrigger::OnResourceRemove(IDXCCResource* pRes)
{
	HRESULT hr= S_OK;
	for(UINT i= 0; 
		i < Events.size() 
		&& hr == S_OK; 
		i++)
	{
		CPipelineManagerEvents* &event= Events.at(i);
		hr=event->OnResourceRemove(pRes);
	}
	return hr;
};



HRESULT CPipelineManagerEventsTrigger::RegisterEvents(CPipelineManagerEvents* pCall)
{
	UnregisterEvents(pCall);
	Events.push_back(pCall);
	return S_OK;
}

HRESULT CPipelineManagerEventsTrigger::UnregisterEvents(CPipelineManagerEvents* pCall) 
{
	for(UINT i= 0; 
		i < Events.size(); 
		i++)
	{
		const CPipelineManagerEvents* event= Events.at(i);
		if(event==pCall)
		{
			Events.erase( Events.begin( ) + i);
			return S_OK;
		}
	}
	
	return E_INVALIDARG;
}
