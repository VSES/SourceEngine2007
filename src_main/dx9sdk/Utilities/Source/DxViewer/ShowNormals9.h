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



class CShowNormals9
{
public:
    CShowNormals9()
    {
        SkinnedVB = NULL;
        Skin = NULL;

        Declaration = NULL;
        Effect = NULL;
    }

    ~CShowNormals9()
    {
        SAFE_RELEASE(SkinnedVB);
        SAFE_RELEASE(Skin);

        SAFE_RELEASE(Declaration);
        SAFE_RELEASE(Effect);
    }

    void OnLostDevice();
    void OnResetDevice();
    
    void Move(D3DXMATRIX* pBoneTranforms, D3DXMATRIX* pBoneInvTransposes);
    void Draw(D3DDECLUSAGE usage, const D3DXMATRIX& WorldMatrix);

    void SetMesh(LPD3DXMESH pNewMesh, LPD3DXSKININFO pNewSkin);
private:


    struct Vertex
    {
        D3DXVECTOR3 Position;
        D3DXVECTOR3 Normal;
        D3DXVECTOR3 Tangent;
        D3DXVECTOR3 Binormal;
    };


    int NumLines;
    std::vector<Vertex> UnskinnedVB;
    LPD3DXSKININFO Skin;
    LPDIRECT3DVERTEXBUFFER9 SkinnedVB;
    
    LPDIRECT3DVERTEXDECLARATION9 Declaration;
    ID3DXEffect* Effect;

    static int Stride; 
    static D3DVERTEXELEMENT9 Elements[5]; 
};
