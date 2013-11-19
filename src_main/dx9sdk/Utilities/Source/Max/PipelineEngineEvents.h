#ifndef __PipelineEngineEvents_h__
#define __PipelineEngineEvents_h__

class CPipelineEngineEvents
{
public:

	virtual HRESULT OnD3DDeviceCreate(){ return S_OK; };//on Adapter Change recovery
	virtual HRESULT OnD3DDeviceReset(){ return S_OK; };//on Device Lost recovery
	virtual HRESULT OnD3DDeviceLost(){ return S_OK; };//on Device Lost
	virtual HRESULT OnD3DDeviceDestroy(){ return S_OK; };//on Adapter Change
};


class CPipelineEngineEventsTrigger : public CPipelineEngineEvents
{
	std::vector<CPipelineEngineEvents*> Events;
public:

	HRESULT RegisterEvents(CPipelineEngineEvents* pCall);
	HRESULT UnregisterEvents(CPipelineEngineEvents* pCall);


	HRESULT OnD3DDeviceCreate();
	HRESULT OnD3DDeviceReset();
	HRESULT OnD3DDeviceLost();
	HRESULT OnD3DDeviceDestroy();
};

#endif