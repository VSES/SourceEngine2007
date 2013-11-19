#include "pch.h"

HRESULT CManagerEventsTrigger::OnReload()
{
	HRESULT hr= S_OK;
	for(UINT i= 0; 
		i < Events.size() 
		&& hr == S_OK; 
		i++)
	{
		CManagerEvents* &event= Events.at(i);
		hr= event->OnReload();
	}
	return hr;
};

HRESULT CManagerEventsTrigger::OnResourceAdd(IDXCCResource* pRes)
{
	HRESULT hr= S_OK;
	for(UINT i= 0; 
		i < Events.size() 
		&& hr == S_OK; 
		i++)
	{
		CManagerEvents* &event= Events.at(i);
		hr= event->OnResourceAdd(pRes);
	}
	return hr;
};


CManagerEvents::CManagerEvents()
{
	ExclusiveModeMutex= CreateMutex(NULL, false, NULL);
}

CManagerEvents::~CManagerEvents()
{
	CloseHandle(ExclusiveModeMutex);

}


HRESULT CManagerEventsTrigger::OnResourceRecycle(IDXCCResource* pRes)
{
	HRESULT hr= S_OK;
	for(UINT i= 0; 
		i < Events.size() 
		&& hr == S_OK; 
		i++)
	{
		CManagerEvents* &event= Events.at(i);
		hr= event->OnResourceRecycle(pRes);
	}
	return hr;
};

HRESULT CManagerEventsTrigger::OnResourceRemove(IDXCCResource* pRes)
{
	HRESULT hr= S_OK;
	for(UINT i= 0; 
		i < Events.size() 
		&& hr == S_OK; 
		i++)
	{
		CManagerEvents* &event= Events.at(i);
		hr=event->OnResourceRemove(pRes);
	}
	return hr;
};



HRESULT CManagerEventsTrigger::RegisterEvents(CManagerEvents* pCall)
{
	UnregisterEvents(pCall);
	Events.push_back(pCall);
	return S_OK;
}

HRESULT CManagerEventsTrigger::UnregisterEvents(CManagerEvents* pCall) 
{
	for(UINT i= 0; 
		i < Events.size(); 
		i++)
	{
		const CManagerEvents* event= Events.at(i);
		if(event==pCall)
		{
			Events.erase( Events.begin( ) + i);
			return S_OK;
		}
	}
	
	return E_INVALIDARG;
}
