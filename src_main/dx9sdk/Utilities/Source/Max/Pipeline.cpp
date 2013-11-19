
#include "StdAfx.h"
#include "Pipeline.h"
#include "PipelineLock.h"

HRESULT CPipeline::Create() 
{ 
	PreviewEngine = NULL;
	//fTime= 0;
	
    CreateRoot();

	ReadWriteMutex = CreateMutexA(NULL, FALSE, "DXCCPipeline_SceneReadWriteLock");
	InitializeCriticalSection(&ZeroSceneReadersSection);
	SceneReadersCount = 0;
	ZeroSceneReadersEvent = CreateEventA(NULL, TRUE, TRUE, "DXCCPipeline_ZeroReaders");

	return S_OK; 
}

HRESULT CPipeline::Destroy() 
{ 
	ReleaseRoot();
	
	CloseHandle(ReadWriteMutex);
	ReadWriteMutex = NULL;
	CloseHandle(ZeroSceneReadersEvent);
	ZeroSceneReadersEvent = NULL;
	DeleteCriticalSection(&ZeroSceneReadersSection);
		
	return S_OK; 
}

void CPipeline::CreateRoot()
{
    ReleaseRoot();

	DXCCCreateManager(DxccManager);
    DXCCCreateFrame(DxccRoot);

    ComPtr<IDXCCResource> res;
	if (SUCCEEDED(DxccManager->CreateResource(DxccRoot, IID_IDXCCFrame, TRUE, res)))
    {
    	res->SetName("DXCC_ROOT");
    }
}

void CPipeline::ReleaseRoot()
{
    if (DxccManager.IsNull())
    {
        return;
    }

    ComPtr<IDXCCResource> res;
    if (SUCCEEDED(DxccManager->FindResourceByPointer(DxccRoot, 0, res)))
    {
        DxccManager->RemoveResource(res->GetHandle());
    }

    DxccRoot.Reset();

	DxccManager.Reset();
}

HRESULT CPipeline::SetEngine(CPipelineEngine* previewEngine)  
{ 
	TriggerDeviceEvents.OnD3DDeviceDestroy();
	PreviewEngine = previewEngine; 
	TriggerDeviceEvents.OnD3DDeviceCreate();
	return S_OK;
}

//void CPipeline::SetTime(float time) 
//{
//	fTime= time;
//}
//
//float CPipeline::GetTime() 
//{
//	return fTime;
//}

CPipelineEngine* CPipeline::AccessEngine()
{
	return PreviewEngine;
}

LPDXCCMANAGER CPipeline::AccessManager()
{
	return DxccManager;
}

LPDXCCFRAME CPipeline::AccessRoot()
{
	return DxccRoot;
}

bool CPipeline::SceneWriteLock(BOOL WaitForLock, CPipelineLock& Lock) 
{
	if(!Lock.isLocked())
	{
		if(GetSingleObject(WaitForLock, ReadWriteMutex))
		{
			if(GetSingleObject(WaitForLock, ZeroSceneReadersEvent))
			{
				Lock.Locked = true;
				Lock.Type = CPipelineLock::TYPE_SCENE_WRITE;
				Lock.Pipeline = this;
				return true;
			}
			else
			{
				ReleaseMutex(ReadWriteMutex);
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}
void CPipeline::SceneWriteUnlock(CPipelineLock& Lock) 
{
	if(Lock.isLocked() && Lock.ofType() == CPipelineLock::TYPE_SCENE_WRITE)
	{
		Lock.Locked = false;
		Lock.Pipeline=NULL;

		ReleaseMutex(ReadWriteMutex);
	}
}


bool CPipeline::SceneReadLock(BOOL WaitForLock, CPipelineLock& Lock)
{
	if(!Lock.isLocked())
	{
		if(GetSingleObject(WaitForLock, ReadWriteMutex))
		{
			if(0==SceneReadersCount)
				ResetEvent(ZeroSceneReadersEvent);
			++SceneReadersCount; 

			ReleaseMutex(ReadWriteMutex);

			Lock.Locked = true;
			Lock.Type = CPipelineLock::TYPE_SCENE_READ;
			Lock.Pipeline=this;

			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}

}

void CPipeline::SceneReadUnlock(CPipelineLock& Lock)
{
	if(Lock.isLocked() && Lock.ofType()==CPipelineLock::TYPE_SCENE_READ)
	{
		EnterCriticalSection(&ZeroSceneReadersSection); 

		Lock.Locked = false;
		Lock.Pipeline = NULL;
		
		if(SceneReadersCount > 0 && 0 == --SceneReadersCount)
		{
			SetEvent(ZeroSceneReadersEvent);
		}
		
		LeaveCriticalSection(&ZeroSceneReadersSection);
	}
}

bool CPipeline::IsValid()
{
	return (PreviewEngine && !DxccManager.IsNull() && !DxccRoot.IsNull());
}

