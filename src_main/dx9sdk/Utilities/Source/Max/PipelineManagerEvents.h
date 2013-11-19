#ifndef __PipelineManagerEvents_h__
#define __PipelineManagerEvents_h__

class CPipelineManagerEvents
{
public:
	CPipelineManagerEvents();
	~CPipelineManagerEvents();

	virtual HRESULT OnReload(){ return S_OK; };//expect that everything is new
	virtual HRESULT OnResourceAdd(IDXCCResource* pRes){ return S_OK; };
	virtual HRESULT OnResourceRecycle(IDXCCResource* pRes){ return S_OK; };
	virtual HRESULT OnResourceUpdate(IDXCCResource* pRes){ return S_OK; };
	virtual HRESULT OnResourceRemove(IDXCCResource* pRes){ return S_OK; };

private:
	HANDLE ExclusiveModeMutex;
};



class CPipelineManagerEventsTrigger : public CPipelineManagerEvents
{
	std::vector<CPipelineManagerEvents*> Events;
public:

	HRESULT RegisterEvents(CPipelineManagerEvents* pCall);
	HRESULT UnregisterEvents(CPipelineManagerEvents* pCall);


	HRESULT OnReload(); 
	HRESULT OnResourceAdd(IDXCCResource* pRes);//EMITTED
	HRESULT OnResourceRecycle(IDXCCResource* pRes);//EMITTED
	HRESULT OnResourceRemove(IDXCCResource* pRes);
};

#endif
