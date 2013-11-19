#ifndef __Session_h__
#define __Session_h__
#pragma once

#include "PipelineEngineEvents.h"
#include "PipelineViewerEvents.h"
#include "PipelineManagerEvents.h"


#define GetSingleObject(WaitForLock, obj) (WAIT_OBJECT_0 == ((WaitForLock) ? WaitForSingleObject(obj, INFINITE) :   WaitForSingleObject(obj, 0)))

class CPipeline
{
	friend class CPipelineLock;
public:

	virtual HRESULT Create();
	virtual HRESULT Destroy();

	virtual bool IsValid();

	virtual class CPipelineEngine* AccessEngine();
	virtual LPDXCCMANAGER AccessManager();
	virtual LPDXCCFRAME AccessRoot();

    //virtual float GetTime();
    //virtual void SetTime(float time);

    virtual HRESULT SetEngine( class CPipelineEngine* pPreviewEngine );

    virtual bool SceneWriteLock(BOOL WaitForLock, class CPipelineLock& Lock); //returns true if you got the lock
	virtual void SceneWriteUnlock(class CPipelineLock& Lock);

	virtual bool SceneReadLock(BOOL WaitForLock, class CPipelineLock& Lock); //returns true if you got the lock
	virtual void SceneReadUnlock(class CPipelineLock& Lock);

	CPipelineEngineEventsTrigger  TriggerDeviceEvents;
	CPipelineManagerEventsTrigger TriggerManagerEvents;
	CPipelineViewerEventsTrigger  TriggerViewerEvents;

protected:

    void CreateRoot();
    void ReleaseRoot();

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
	
	class CPipelineEngine* PreviewEngine;
	ComPtr<IDXCCManager> DxccManager;	
	ComPtr<IDXCCFrame> DxccRoot;
	//float fTime;
};


#endif