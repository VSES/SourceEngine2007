//--------------------------------------------------------------------------------------
// File: Device.h
//
// Encapsulates a Direct3D device which exists in a separate thread
//
// Copyright (c) Microsoft Corporation. All rights reserved
//--------------------------------------------------------------------------------------

#ifndef ENGINEIMPL_H
#define ENGINEIMPL_H

class CManager;

class CMayaEngine : public CEngine, public CManagerEvents
{
public:
	virtual HRESULT Create( CManager* pPreviewPipeline );
	virtual HRESULT Destroy();

	virtual HRESULT GetD3DObject( IDirect3D9** ppObject );
	virtual HRESULT GetD3DDevice( IDirect3DDevice9** ppDevice );

private:
    UINT __stdcall RunThread();
    static UINT __stdcall StaticRunThread( void* pParam );

    static bool CALLBACK IsDeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, bool bWindowed, void* pUserContext );
    static void CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, const D3DCAPS9* pCaps );

private:
    bool m_bDestroyCalled;
    UINT m_nThreadID;
    uintptr_t m_hThread;
	HWND m_hwndFocus;

    IDirect3DDevice9* m_pd3dDevice;
    CManager* m_pPreviewPipeline;
};

#endif //ENGINE_H