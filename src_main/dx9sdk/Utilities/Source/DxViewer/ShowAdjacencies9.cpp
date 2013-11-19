//--------------------------------------------------------------------------------------
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include "d3d9.h"
#include "d3dx9.h"
#include "sas.h"

#include "resource.h"
#include "dxut.h"
#include "render9.h"
#include "App.h"
#include "Camera.h"
#include "ShowAdjacencies9.h"


int CShowAdjacencies9::Stride = 1*sizeof(D3DXVECTOR3);
D3DVERTEXELEMENT9 CShowAdjacencies9::Elements[2] = 
{
    {0, 0*sizeof(D3DXVECTOR3), D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
    D3DDECL_END()
};


void CShowAdjacencies9::SetMesh(LPD3DXMESH pNewMesh, DWORD* pNewAdj, LPD3DXSKININFO pNewSkin)
{
    HRESULT hr = S_OK;

    NumLines = 0;
    NumPoints = 0;
    

    UnskinnedVB.resize(0);
    SkinnedVB.resize(0);
    FaceIB.resize(0);
    SAFE_RELEASE(FaceVB);
    SAFE_RELEASE(LinesIB);
    SAFE_RELEASE(Skin);

    if(pNewMesh == NULL)
        return;

    IDirect3DDevice9* device = DXUTGetD3D9Device();

    {//EFFECT
     V( device->CreateVertexDeclaration( Elements, &Declaration ) );
     ID3DXBuffer* pErrors = NULL;
     V( SASCreateEffectFromResource(
             device, 
             NULL, MAKEINTRESOURCE(IDR_SHOWLINES9FX), MAKEINTRESOURCE(RT_RCDATA),
             NULL, 
             NULL, 
             0,
             NULL, 
             &Effect, 
             &pErrors));
    if(pErrors)
         DXVGetApp().OutputA( (const char*)pErrors->GetBufferPointer() );
    SAFE_RELEASE(pErrors);
    }//EFFECT

    D3DVERTEXELEMENT9 declIn[ MAX_FVF_DECL_SIZE ];
    V( pNewMesh->GetDeclaration(declIn) );

    int iPos= -1;
    int iNorm= -1;
    for( int i = 0 ; 
        declIn[i].Stream != 255 && i < MAX_FVF_DECL_SIZE;
        i++)
    {
        if(declIn[i].Usage == D3DDECLUSAGE_POSITION && declIn[i].UsageIndex == 0)
            iPos = i;
        if(declIn[i].Usage == D3DDECLUSAGE_NORMAL && declIn[i].UsageIndex == 0)
            iNorm = i;
    }

    if(iPos == -1 || iNorm == -1)
        return;

    if( (( declIn[iPos].Type & (D3DDECLTYPE_FLOAT3|D3DDECLTYPE_FLOAT4)) == 0  ) ||
        (( declIn[iNorm].Type & (D3DDECLTYPE_FLOAT3|D3DDECLTYPE_FLOAT4)) == 0  ) )
        return;

    NumPoints = pNewMesh->GetNumFaces()*4;
    NumLines = pNewMesh->GetNumFaces()*3;



    if(pNewSkin)
    {
        V( pNewSkin->Clone( &Skin ) ); 
        V( Skin->SetDeclaration(Elements) );
    }


    //GET VERTEX DATA
    CreateVertexBuffers(pNewMesh, declIn, iPos);

    //GET INDEX DATA
    CreateFaceIndexBuffer(pNewMesh);


    //Create Render IB
    CreateFaceLines(pNewMesh, pNewAdj);

    V( device->CreateVertexBuffer( NumPoints*Stride , D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &FaceVB, NULL) );


    AverageVertices();

}

void CShowAdjacencies9::CreateVertexBuffers(LPD3DXMESH pNewMesh, D3DVERTEXELEMENT9* declIn, int iPos)
{
    HRESULT hr = S_OK;

    int MeshStride = pNewMesh->GetNumBytesPerVertex();

    UnskinnedVB.resize(pNewMesh->GetNumVertices());
    SkinnedVB.resize(pNewMesh->GetNumVertices());
    BYTE* pSrcVB= NULL;
    V(  pNewMesh->LockVertexBuffer( D3DLOCK_READONLY, (LPVOID*)&pSrcVB ) );
    for( DWORD iVert = 0; iVert < pNewMesh->GetNumVertices(); iVert++)
    {
        VertexSrc v0;

        v0.Position = *(D3DXVECTOR3*) (pSrcVB+(MeshStride*iVert)+declIn[iPos].Offset);

        UnskinnedVB[iVert] = v0;
        SkinnedVB[iVert] = v0;
    }
    V( pNewMesh->UnlockVertexBuffer() );
}

void CShowAdjacencies9::CreateFaceIndexBuffer(LPD3DXMESH pNewMesh)
{
    HRESULT hr = S_OK;
    FaceIB.resize(pNewMesh->GetNumFaces());
    bool b32Bit = pNewMesh->GetOptions() & D3DXMESH_32BIT;
    BYTE* pSrcIB= NULL;
    V(  pNewMesh->LockIndexBuffer( D3DLOCK_READONLY, (LPVOID*)&pSrcIB ) );
    for( DWORD iFace = 0; iFace < pNewMesh->GetNumFaces(); iFace++)
    {
        FaceIB[iFace] = FaceIndices::GetFace(pSrcIB, b32Bit, iFace);
    }
    V( pNewMesh->UnlockIndexBuffer() );
}


void CShowAdjacencies9::CreateFaceLines(LPD3DXMESH pNewMesh, DWORD* pNewAdj)
{
    IDirect3DDevice9* device = DXUTGetD3D9Device();

    HRESULT hr = S_OK;
    V( device->CreateIndexBuffer( sizeof(FaceLines3)*pNewMesh->GetNumFaces() , D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_MANAGED, &LinesIB, NULL) );
    FaceLines3* pDstIB= NULL;
    V(  LinesIB->Lock( 0, 0, (LPVOID*)&pDstIB,  0) );
    for( DWORD iFace = 0; iFace < pNewMesh->GetNumFaces(); iFace++)
    {
        //FaceIndices* face = &FaceIB[iFace];
        FaceLines3* faceLines = &pDstIB[iFace];
        FaceAdjacencies* adj = &((FaceAdjacencies*)pNewAdj)[iFace];

        //fill everything with face-center point
        faceLines->L0[0] = faceLines->L0[1] = 4*iFace;
        faceLines->L1[0] = faceLines->L1[1] = 4*iFace;
        faceLines->L2[0] = faceLines->L2[1] = 4*iFace;

        //conditionally fill line ends with mid-edge points.
        if(adj->E0 != ((DWORD)-1))
            faceLines->L0[1]= (4*iFace+1)+0;
        if(adj->E1 != ((DWORD)-1))
            faceLines->L1[1]= (4*iFace+1)+1;
        if(adj->E2 != ((DWORD)-1))
            faceLines->L2[1]= (4*iFace+1)+2;


    }
    V( LinesIB->Unlock() );
}

void CShowAdjacencies9::AverageVertices()
{
    HRESULT hr = S_OK;

    VertexDst* pDstVB= NULL;
    V( FaceVB->Lock( 0, 0, (LPVOID*)&pDstVB,  0) );
    for( UINT iFace = 0; iFace < FaceIB.size(); iFace++)
    {
        FaceIndices& face = FaceIB[iFace];

        pDstVB[iFace].Position = (SkinnedVB[face.I0].Position+SkinnedVB[face.I1].Position+SkinnedVB[face.I2].Position)/3.0f;
        pDstVB[iFace].A0 = (SkinnedVB[face.I0].Position+SkinnedVB[face.I1].Position)/2.0f;
        pDstVB[iFace].A1 = (SkinnedVB[face.I1].Position+SkinnedVB[face.I2].Position)/2.0f;
        pDstVB[iFace].A2 = (SkinnedVB[face.I2].Position+SkinnedVB[face.I0].Position)/2.0f;

    }
    V( FaceVB->Unlock() );

}

void CShowAdjacencies9::Move(D3DXMATRIX* pBoneTranforms, D3DXMATRIX* pBoneInvTransposes)
{
    HRESULT hr = S_OK;

    if(Skin && pBoneTranforms)
    {    
        BYTE* pSrc=(BYTE*)(void*)&UnskinnedVB.front();
        BYTE* pDst=(BYTE*)(void*)&SkinnedVB.front();
        V( Skin->UpdateSkinnedMesh( pBoneTranforms, pBoneInvTransposes, pSrc, pDst ) );

        AverageVertices();
    }

}

void CShowAdjacencies9::Draw(const D3DXMATRIX& WorldMatrix)
{
    HRESULT hr = S_OK;

    if(!FaceVB)
        return;

    IDirect3DDevice9* device = DXUTGetD3D9Device();

    V( Effect->SetMatrix( "World", &WorldMatrix) );
    V( Effect->SetMatrix( "View", DXVGetCamera().GetViewMatrix() ));
    V( Effect->SetMatrix( "Projection", DXVGetCamera().GetProjMatrix() ));
    V( Effect->SetVector( "Color", &D3DXVECTOR4(0.5f,0.5f,1.0f,1.0f)) );

    V( Effect->SetTechnique( Effect->GetTechnique(0)) );

    UINT numPasses;
    V( Effect->Begin( &numPasses, 0 ));

    for( UINT iPass = 0; iPass < numPasses; iPass++ )
    {
        V( Effect->BeginPass( iPass ) );

        V( device->SetVertexDeclaration( Declaration ) );
        V( device->SetStreamSource(0, FaceVB, 0 , Stride ) );
        V( device->SetIndices(LinesIB) );

        V( device->DrawIndexedPrimitive( D3DPT_LINELIST, 0, 0, NumPoints, 0, NumLines) );

        V( Effect->EndPass() );
    }
    V( Effect->End() );
}
