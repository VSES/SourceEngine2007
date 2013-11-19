#pragma warning(disable: 4995)
#include <list>
#include <string>
#pragma warning(default: 4995)

#include "windows.h"
#include "strsafe.h"

#include "resource.h"

#include "state.h"
#include "StateBlock.h"

#include "DXUT.h"
#include "DXUTmisc.h"


DECLARE_SINGLETON_CPP( StateBlock );


void StateBlock::ResetState()
{
    AllowD3D10.Set(false);
    Initializing.Set(true);
    Render.Set(NULL);
    Version.Set(DXUT_D3D9_DEVICE);
    MergeMode.Set(false);
    Reset.Set(DXVRESET_NONE);
    ExitOnLoad.Set(false);

    ShowPoints.Set(false);
    ShowNormals.Set(false);
    ShowTangents.Set(false);
    ShowBinormals.Set(false);
    ShowAdjacencies.Set(false);
    ShowCreases.Set(false);
    ShowEdges.Set(false);
    ShowBounds.Set(false);
    ShowStatistics.Set(false);
    ManualRefreshMode.Set(false);

    FillMode.Set(DXVFILLMODE_AUTO);

    TechniqueMode.Set(DXVTECHNIQUEMODE_FIRSTVALID);
    PassMode.Set(DXVPASSMODE_ALL);

    
    STATE_LOCKV(Defines, std::list<Define> , defList);
    defList.push_back( Define(L"SASPRESENT") );
    defList.push_back( Define(L"__DXVIEWER") );
    STATE_UNLOCK(Defines);

    DebugPixelShaders.Set(false);
    DebugVertexShaders.Set(false);
    OptimizeShaders.Set(true);
    ValidateShaders.Set(true);
    Preshaders.Set(true);
    PartialPrecision.Set(false);
    PreferDynamicFlow.Set(false);

    PreviewShape.Set(IDR_SPHEREX);
}


Define::Define(const WCHAR* n, const WCHAR* v) 
{ 
    if(n)
    {
        int len = lstrlenW(n);
        CHAR* buf = new CHAR[len+1];
        StringCchPrintfA(buf, len+1, "%S", n);
        name = buf;
        SAFE_DELETE_ARRAY(buf);

    }

    if(v)
    {
        int len = lstrlenW(v);
        CHAR* buf = new CHAR[len+1];
        StringCchPrintfA(buf, len+1, "%S", v);
        value = buf;
        SAFE_DELETE_ARRAY(buf);
    }
}