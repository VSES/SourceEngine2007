#pragma once

#include "macros.h"
#include "HUD.h"

class EffectHUD : public HUD
{
public:
    FRIEND_SINGLETON_H( EffectHUD );
 
    EffectHUD() : HUD() { ReloadContent = false; ResetDevice = false; }

    virtual void Refresh();
protected:
    virtual void CreateControls();
    virtual void OnEvent( UINT nEvent, int nControlID, CDXUTControl* pControl );


    void BuildShaderDefines(std::wstring& output);


    bool ReloadContent;
    bool ResetDevice;

    HUD_STATIC_ON_EVENT
};

DECLARE_SINGLETON_H( EffectHUD );