#ifndef VIEWER_H
#define VIEWER_H

#include "windows.h"
#include "d3d9.h"

class CManager;

//it is suggested that the viewers use StateBlocks to ensure that multiple viewers do not collide
class CViewer 
{
public:
	//get the device from the pPreviewPipeline and use CreateAdditionalSwapChain
	virtual HRESULT Create( CManager* pPreviewPipeline,	
							D3DPRESENT_PARAMETERS* pPresentationParameters,
							const WCHAR* strWindowTitle = L"Direct3D Preview Pipeline", 
							HINSTANCE hInstance = NULL, 
							HICON hIcon = NULL, 
							HMENU hMenu = NULL,
							int width = 640,
							int height = 480,
							int x = CW_USEDEFAULT, 
							int y = CW_USEDEFAULT) = 0;

    virtual HRESULT Destroy() = 0; //destroy thread

	virtual UINT Run() = 0; //allow core logic to start/continue
	virtual UINT GetPauseCount() = 0; //is the core logic running?
	virtual UINT Pause() = 0; //pause the core logic
	
    virtual HWND	GetRenderWindow() = 0; //get window this view will render to
    virtual HWND    GetShellWindow() = 0;//get the highest-level window you'd like docked inside a DCC
	virtual HRESULT	GetD3DSwapChain(IDirect3DSwapChain9** ppSwapChain) = 0;
};

#endif
