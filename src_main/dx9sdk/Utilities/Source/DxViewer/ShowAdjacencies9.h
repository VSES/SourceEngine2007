//--------------------------------------------------------------------------------------
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#pragma once
#include <vector>
#include "d3d9.h"
#include "d3dx9.h"
#include "ShowShared9.h"


namespace Sas
{
    class Effect;
}



class CShowAdjacencies9
{
public:
    CShowAdjacencies9()
    {
        FaceVB = NULL;
        LinesIB = NULL;
        Skin = NULL;

        Declaration = NULL;
        Effect = NULL;
    }

    ~CShowAdjacencies9()
    {
        SAFE_RELEASE(FaceVB);
        SAFE_RELEASE(LinesIB);
        SAFE_RELEASE(Skin);

        SAFE_RELEASE(Declaration);
        SAFE_RELEASE(Effect);
    }

    void OnLostDevice();
    void OnResetDevice();
    
    void Move(D3DXMATRIX* pBoneTranforms, D3DXMATRIX* pBoneInvTransposes);
    void Draw(const D3DXMATRIX& WorldMatrix);

    void SetMesh(LPD3DXMESH pNewMesh, DWORD* pNewAdj, LPD3DXSKININFO pNewSkin);


private:
    void CreateVertexBuffers(LPD3DXMESH pNewMesh, D3DVERTEXELEMENT9* declIn, int iPos);
    void CreateFaceIndexBuffer(LPD3DXMESH pNewMesh);
    void CreateFaceLines(LPD3DXMESH pNewMesh, DWORD* pNewAdj);
    void AverageVertices();

    struct VertexSrc
    {
        D3DXVECTOR3 Position;
    };

    struct VertexDst
    {
        D3DXVECTOR3 Position;
        D3DXVECTOR3 A0;
        D3DXVECTOR3 A1;
        D3DXVECTOR3 A2;
    };

    int NumLines;
    int NumPoints;

    LPD3DXSKININFO Skin;
    std::vector<VertexSrc> UnskinnedVB;
    std::vector<VertexSrc> SkinnedVB;
    std::vector<FaceIndices> FaceIB;

    LPDIRECT3DVERTEXBUFFER9 FaceVB;
    LPDIRECT3DINDEXBUFFER9 LinesIB;

    LPDIRECT3DVERTEXDECLARATION9 Declaration;
    ID3DXEffect* Effect;

    static int Stride; 
    static D3DVERTEXELEMENT9 Elements[2]; 
};
