#ifndef MANAGER_H
#define MANAGER_H

#include "windows.h"
#include "dxcc.h"

#include "Engine.h"
#include "EngineEvents.h"
#include "Viewer.h"
#include "ViewerEvents.h"
#include "ManagerLock.h"
#include "ManagerEvents.h"


#define GetSingleObject(WaitForLock, obj) (WAIT_OBJECT_0 == ((WaitForLock) ? WaitForSingleObject(obj, INFINITE) :   WaitForSingleObject(obj, 0)))


class CManager
{
	friend CManagerLock;
public:

	virtual HRESULT Create();
	virtual HRESULT Destroy();

	virtual bool IsValid();

	virtual CEngine* AccessEngine();
	virtual LPDXCCMANAGER AccessManager();
	virtual LPDXCCFRAME AccessRoot();

    virtual float GetTime();
    virtual void SetTime(float time);


    virtual HRESULT SetEngine( CEngine* pPreviewEngine );


    virtual bool SceneWriteLock(BOOL WaitForLock, CManagerLock& Lock); //returns true if you got the lock
	virtual void SceneWriteUnlock(CManagerLock& Lock);

	virtual bool SceneReadLock(BOOL WaitForLock, CManagerLock& Lock); //returns true if you got the lock
	virtual void SceneReadUnlock(CManagerLock& Lock);

	CEngineEventsTrigger		TriggerDeviceEvents;
	CManagerEventsTrigger		TriggerManagerEvents;
	CViewerEventsTrigger		TriggerViewerEvents;

private:


	//everyone must gain the Access to play fair
	//Write not release until unlock so that no reads come though
	//Read releases immediately to support multiple writes
	//Read incriments ReadCount on gain of AccessMutex and decriment on finishing
	//Read sets ZeroEvent when  ReadCount is zero and resets when non-zero
	//write waits on ZeroEvent after it has gained AccessMutex to ensure that reads are finished
	HANDLE	ReadWriteMutex;
	CRITICAL_SECTION ZeroSceneReadersSection; 
	UINT	SceneReadersCount;
	HANDLE	ZeroSceneReadersEvent;	
	
	CEngine* pPreviewEngine;
	LPDXCCMANAGER pDXCCManager;	
	LPDXCCFRAME pDXCCRoot;
	float fTime;



};



#endif