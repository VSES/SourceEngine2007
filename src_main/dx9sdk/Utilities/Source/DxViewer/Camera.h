#pragma once
#include "macros.h"

#include "d3d9.h"
#include "d3dx9.h"
#include "d3d10.h"
#include "d3dx10.h"
#include "dxut.h"
#include "dxutcamera.h"


class Camera : public CModelViewerCamera
{
public:
    FRIEND_SINGLETON_H( Camera );
    
    Camera(){}
    void Place( D3DXVECTOR3& pEye);
    void Place( D3DCUBEMAP_FACES face);
    void Home();
    void Resize();
    //bool OnKeyboardMsg( UINT nChar, bool bKeyDown, bool bAltDown );
    virtual LRESULT HandleMessages( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

protected:
};

DECLARE_SINGLETON_H( Camera );
