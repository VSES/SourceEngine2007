#pragma once

#pragma warning(disable: 4995)
#include <string>
#pragma warning(default: 4995)
#include <vector>
#include <list>

//#include "DXUT.h"

#include "State.h"
#include "macros.h"

class Render; 

enum DXUTDeviceVersion;


enum DXVFILLMODE
{
    DXVFILLMODE_AUTO,
    DXVFILLMODE_SOLID,
    DXVFILLMODE_WIRE,
};

enum DXVPASSMODE
{
    DXVPASSMODE_ALL,
    DXVPASSMODE_UPTO,
};

enum DXVTECHNIQUEMODE
{
    DXVTECHNIQUEMODE_FIRSTVALID,
    DXVTECHNIQUEMODE_SELECTED,
};

enum DXVRESET
{
    DXVRESET_NONE,
    DXVRESET_RELOADCONTENT,
    DXVRESET_RESETDEVICE,
    DXVRESET_SWITCHDEVICE,
};

struct Define
{
    Define() {}
    Define(const WCHAR* n, const WCHAR* v = NULL);
        
    std::string name;//must not be wide for easy conversion to D3DXMACRO
    std::string value;//must not be wide for easy conversion to D3DXMACRO
};


typedef std::vector< std::wstring > wstringvector; 
typedef std::list< std::wstring > wstringlist; 
typedef std::list<Define> DefineList; 



class StateBlock
{
public:
    FRIEND_SINGLETON_H( StateBlock );

//APP BEGIN
    StateV<bool> AllowD3D10;
    StateV<bool> Initializing;
    StateV<HWND> Dialog;
    StateV<Render*> Render;
    StateV<DXUTDeviceVersion> Version;
    StateV<bool> MergeMode;
    StateV<DXVRESET> Reset;
    StateV<bool> ExitOnLoad;
//APP END

//VIEW-OPTIONS BEGIN
    StateV<bool> ShowPoints;

    StateV<bool> ShowNormals;
    StateV<bool> ShowTangents;
    StateV<bool> ShowBinormals;

    StateV<bool> ShowAdjacencies;
    StateV<bool> ShowCreases;
    StateV<bool> ShowEdges;

    StateV<bool> ShowBounds;

    StateV<bool> ShowStatistics;
    StateV<bool> ManualRefreshMode;
    StateV<bool> ManualRefreshNeeded;

    StateV<DXVFILLMODE> FillMode;

    StateV<std::wstring> Environment;
//VIEW-OPTIONS END

//EFFECT-OPTIONS BEGIN
    StateV<DXVTECHNIQUEMODE> TechniqueMode;
    StateV<int> TechniqueSelected;
    StateV<DXVPASSMODE> PassMode;
    StateV<int> PassSelected;

    StateR< std::list<Define> > Defines;
    StateV< std::wstring > Includes;

    StateV<bool> DebugPixelShaders;
    StateV<bool> DebugVertexShaders;
    StateV<bool> OptimizeShaders;
    StateV<bool> ValidateShaders;
    StateV<bool> Preshaders;
    StateV<bool> PartialPrecision;
    StateV<bool> PreferDynamicFlow;
//EFFECT-OPTIONS END

//CONTENT BEGIN
    StateV<std::wstring> FileX;
    StateV<std::wstring> FileFX;
    StateV<std::wstring> FileDDS;
    StateV<DWORD> PreviewShape;
//CONTENT END

    StateBlock()
    {
        ResetState();
    }

    void ResetState();
};

DECLARE_SINGLETON_H( StateBlock );
