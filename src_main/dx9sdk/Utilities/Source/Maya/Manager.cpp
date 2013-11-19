#include "pch.h"

HRESULT CManager::Create() 
{ 
	LPDXCCRESOURCE pResource= NULL;
	pPreviewEngine= NULL;
	fTime= 0;
	DXCCCreateManager(&pDXCCManager);
	DXCCCreateFrame(&pDXCCRoot);
	pDXCCManager->CreateResource(pDXCCRoot, IID_IDXCCFrame, TRUE, &pResource);
	pResource->SetName("DXCC_ROOT");
	DXCC_RELEASE(pResource);

	ReadWriteMutex= CreateMutexA(NULL, FALSE, "DXCCManager_SceneReadWriteLock");
	InitializeCriticalSection(&ZeroSceneReadersSection);
	SceneReadersCount= 0;
	ZeroSceneReadersEvent= CreateEventA(NULL, TRUE, TRUE, "DXCCManager_ZeroReaders");

	return S_OK; 
}

HRESULT CManager::Destroy() 
{ 

	DXCC_RELEASE(pDXCCRoot);
	DXCC_RELEASE(pDXCCManager);
	CloseHandle(ReadWriteMutex);
	ReadWriteMutex= NULL;
	CloseHandle(ZeroSceneReadersEvent);
	ZeroSceneReadersEvent= NULL;
	DeleteCriticalSection(&ZeroSceneReadersSection);
		
	return S_OK; 
}

HRESULT CManager::SetEngine( CEngine* pPreviewPipelineEngine )  
{ 
	TriggerDeviceEvents.OnD3DDeviceDestroy();
	pPreviewEngine= pPreviewPipelineEngine; 
	TriggerDeviceEvents.OnD3DDeviceCreate();
	return S_OK;
}



void CManager::SetTime(float time) 
{
	fTime= time;
}

float CManager::GetTime() 
{
	return fTime;
}

CEngine* 
CManager::AccessEngine()
{
	return pPreviewEngine;
}

LPDXCCMANAGER
CManager::AccessManager()
{
	return pDXCCManager;
}

LPDXCCFRAME 
CManager::AccessRoot()
{
	return pDXCCRoot;
}


bool CManager::SceneWriteLock(BOOL WaitForLock, CManagerLock& Lock) 
{
	if(Lock.isLocked()==false)
	{
		if(GetSingleObject(WaitForLock, ReadWriteMutex))
		{
			if(GetSingleObject(WaitForLock, ZeroSceneReadersEvent))
			{
				Lock.Locked=true;
				Lock.Type=CManagerLock::TYPE_SCENE_WRITE;
				Lock.pPreviewPipeline=this;
				return true;
			}
			else
			{
				ReleaseMutex(ReadWriteMutex);
				return false;
			}
		}
		else
			return false;
	}
	else
		return false;
}
	
void CManager::SceneWriteUnlock(CManagerLock& Lock) 
{
	if(Lock.isLocked()==true && Lock.ofType()==CManagerLock::TYPE_SCENE_WRITE)
	{
		Lock.Locked=false;
		Lock.pPreviewPipeline=NULL;

		ReleaseMutex(ReadWriteMutex);
	}
}


bool CManager::SceneReadLock(BOOL WaitForLock, CManagerLock& Lock)
{
	if(Lock.isLocked()==false)
	{
		if(GetSingleObject(WaitForLock, ReadWriteMutex))
		{
			//EnterCriticalSection(&ZeroSceneReadersSection); //using ReadWriteMutex instead
			if(0==SceneReadersCount)
				ResetEvent(ZeroSceneReadersEvent);
			++SceneReadersCount; //InterlockedIncrement(&SceneReadersCount);//using ReadWriteMutex instead
			//LeaveCriticalSection(&ZeroSceneReadersSection); //using ReadWriteMutex instead

			ReleaseMutex(ReadWriteMutex);

			Lock.Locked=true;
			Lock.Type=CManagerLock::TYPE_SCENE_READ;
			Lock.pPreviewPipeline=this;

			return true;
		}
		else
			return false;
	}
	else
		return false;

}


void CManager::SceneReadUnlock(CManagerLock& Lock)
{
	if(Lock.isLocked()==true && Lock.ofType()==CManagerLock::TYPE_SCENE_READ)
	{
		EnterCriticalSection(&ZeroSceneReadersSection); 

		Lock.Locked=false;
		Lock.pPreviewPipeline=NULL;
		
		if(SceneReadersCount > 0
			&& 0== --SceneReadersCount)
		{
			SetEvent(ZeroSceneReadersEvent);
		}
		
		LeaveCriticalSection(&ZeroSceneReadersSection);
	}
}

bool CManager::IsValid()
{
	return (pPreviewEngine && pDXCCManager && pDXCCRoot);
}

