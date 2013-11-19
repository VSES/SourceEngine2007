//--------------------------------------------------------------------------------------
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#pragma once
#include <vector>
#include "d3d9.h"
#include "d3dx9.h"



namespace Sas
{
    class Effect;
}



class CShowBounds9
{
public:
    CShowBounds9()
    {
        VB = NULL;
        IB = NULL;
        Declaration = NULL;
        Effect = NULL;
    }

    ~CShowBounds9()
    {
        OnDestroyDevice();
    }

    void OnLostDevice();
    void OnResetDevice();
    void OnDestroyDevice();

    void Draw();
    void Clear() { OnDestroyDevice(); }

    void SetBounds(const D3DXVECTOR3& minVec, const D3DXVECTOR3& maxVec);

private:
    static void GetBox(D3DXVECTOR3* v0, UINT stride,
                         const D3DXVECTOR3& minVec, 
                         const D3DXVECTOR3& maxVec);

    struct Vertex
    {
        D3DXVECTOR3 Position;
    };

    LPDIRECT3DVERTEXBUFFER9 VB;
    LPDIRECT3DINDEXBUFFER9 IB;

    LPDIRECT3DVERTEXDECLARATION9 Declaration;
    ID3DXEffect* Effect;

    static int Stride; 
    static D3DVERTEXELEMENT9 Elements[2]; 

};
