#pragma once

#include "macros.h"
#include "HUD.h"

class ViewHUD : public HUD
{
public:
    FRIEND_SINGLETON_H( ViewHUD );
 
    ViewHUD() : HUD() {}

    virtual void Refresh();
protected:
    virtual void CreateControls();
    virtual void OnEvent( UINT nEvent, int nControlID, CDXUTControl* pControl );

    HUD_STATIC_ON_EVENT
};

DECLARE_SINGLETON_H( ViewHUD );