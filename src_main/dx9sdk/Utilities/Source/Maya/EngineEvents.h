#ifndef ENGINEEVENTS_H
#define ENGINEEVENTS_H

#include "windows.h"
#include "d3d9.h"
#include <vector>

class CEngineEvents
{
public:

	virtual HRESULT OnD3DDeviceCreate(){ return S_OK; };//on Adapter Change recovery
	virtual HRESULT OnD3DDeviceReset(){ return S_OK; };//on Device Lost recovery
	virtual HRESULT OnD3DDeviceLost(){ return S_OK; };//on Device Lost
	virtual HRESULT OnD3DDeviceDestroy(){ return S_OK; };//on Adapter Change
};


class CEngineEventsTrigger : public CEngineEvents
{
	std::vector<CEngineEvents*> Events;
public:

	HRESULT RegisterEvents(CEngineEvents* pCall);
	HRESULT UnregisterEvents(CEngineEvents* pCall);


	HRESULT OnD3DDeviceCreate();
	HRESULT OnD3DDeviceReset();
	HRESULT OnD3DDeviceLost();
	HRESULT OnD3DDeviceDestroy();
};

#endif