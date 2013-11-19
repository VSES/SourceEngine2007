#pragma once

#include "d3d9.h"
#include "d3dx9.h"
#include "sas.h"

#include "StateBlock.h"

enum DXUTDeviceVersion;
struct DXUTDeviceSettings;

struct FSQ_VERTEX
{
    D3DVECTOR pos;
};

extern const SHORT IbFSQ[];
extern const FSQ_VERTEX VbFSQ[];

struct TechniqueInfo
{
    std::wstring Name;
    std::vector<std::wstring> Passes;
};


class Render
{
public:
    virtual DXUTDeviceVersion GetVersion() = 0;
    //Register device state changing callbacks (create, lost, destroy, reset, resize)
    //Register rendering callback
    virtual void Startup() = 0;
    virtual void Shutdown() = 0;

    virtual float GetModelRadius() { return 1.0f; }
    virtual const D3DXVECTOR3& GetModelCenter() { static D3DXVECTOR3 origin(0,0,0); return origin; }
    virtual const D3DXVECTOR3& GetMin() { static D3DXVECTOR3 origin(0,0,0); return origin; }
    virtual const D3DXVECTOR3& GetMax() { static D3DXVECTOR3 origin(0,0,0); return origin; }

    //clear the current scene contents
    virtual void Clear() = 0;
    //load new scene contents
    virtual void Load() = 0;

    virtual ~Render() {}

    //The app owns the FrameMove functions and calls the renderer for content specific updates
    virtual void OnFrameMove( double fTime, float fElapsedTime ) = 0;
    virtual void OnFrameRender( double fTime, float fElapsedTime ) = 0;
    virtual bool OnModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings ) = 0;


    std::vector<TechniqueInfo>& GetEffectInfo() { return EffectInfo; }

    static void UpdateBoundValueTable( Sas::BoundValueTable* table, D3DXMATRIX& WorldMatrix );
    static void UpdateRootBoundValueTable();
    static void PromoteSharedValues( Sas::BoundValueTable* table  );

    static D3DXEFFECTINSTANCE* GetDefaultEffectInstance();
    static D3DXMATERIAL* GetDefaultMaterial();

protected:
    Render(){}
    std::vector<TechniqueInfo> EffectInfo;


};


#include "Render9.h"