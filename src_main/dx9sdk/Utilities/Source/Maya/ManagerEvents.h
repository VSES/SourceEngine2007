#ifndef MANAGEREVENTS_H
#define MANAGEREVENTS_H

#include "windows.h"
#include "dxcc.h"
#include <vector>


class CManagerEvents
{
public:
	CManagerEvents();
	~CManagerEvents();

	virtual HRESULT OnReload(){ return S_OK; };//expect that everything is new
	virtual HRESULT OnResourceAdd(IDXCCResource* pRes){ return S_OK; };
	virtual HRESULT OnResourceRecycle(IDXCCResource* pRes){ return S_OK; };
	virtual HRESULT OnResourceUpdate(IDXCCResource* pRes){ return S_OK; };
	virtual HRESULT OnResourceRemove(IDXCCResource* pRes){ return S_OK; };

private:
	HANDLE ExclusiveModeMutex;
};



class CManagerEventsTrigger : public CManagerEvents
{
	std::vector<CManagerEvents*> Events;
public:

	HRESULT RegisterEvents(CManagerEvents* pCall);
	HRESULT UnregisterEvents(CManagerEvents* pCall);


	HRESULT OnReload(); 
	HRESULT OnResourceAdd(IDXCCResource* pRes);//EMITTED
	HRESULT OnResourceRecycle(IDXCCResource* pRes);//EMITTED
	HRESULT OnResourceRemove(IDXCCResource* pRes);
};

#endif
