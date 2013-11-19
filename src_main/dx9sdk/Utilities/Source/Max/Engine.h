
#ifndef __Engine_h__
#define __Engine_h__

#include "PipelineEngine.h"
#include "PipelineManagerEvents.h"

class CPipeline;

class CEngine : public CPipelineEngine, public CPipelineManagerEvents
{
public:
    CEngine();

	virtual HRESULT Create(CPipeline* pipeline);
	virtual HRESULT Destroy();

	virtual HRESULT GetD3DObject(IDirect3D9** ppObject);
	virtual HRESULT GetD3DDevice(IDirect3DDevice9** ppDevice);

private:

    UINT __stdcall RunThread();
    static UINT __stdcall StaticRunThread(void* pParam);

    static bool CALLBACK IsDeviceAcceptable(D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, bool bWindowed);
    static void CALLBACK ModifyDeviceSettings(struct DXUTDeviceSettings* pDeviceSettings, const D3DCAPS9* pCaps);

private:

    bool DestroyCalled;
    UINT ThreadId;
    uintptr_t Thread;
	HWND FocusWindow;

    ComPtr<IDirect3D9> D3d;
    ComPtr<IDirect3DDevice9> D3dDevice;
    CPipeline* Pipeline;
};

#endif //ENGINE_H