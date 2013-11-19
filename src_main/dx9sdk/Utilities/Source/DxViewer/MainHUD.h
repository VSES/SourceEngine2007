#pragma once

#include "macros.h"
#include "HUD.h"

class MainHUD : public HUD
{
public:
    FRIEND_SINGLETON_H( MainHUD );
 
    MainHUD() : HUD() {}

    virtual void Refresh();

protected:
    virtual void CreateControls();
    virtual void OnEvent( UINT nEvent, int nControlID, CDXUTControl* pControl );

    HUD_STATIC_ON_EVENT
};

DECLARE_SINGLETON_H( MainHUD );