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
#include "ShowPoints9.h"


int CShowPoints9::Stride = 1*sizeof(D3DXVECTOR3);
D3DVERTEXELEMENT9 CShowPoints9::Elements[2] = 
{
    {0, 0*sizeof(D3DXVECTOR3), D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
    D3DDECL_END()
};


void CShowPoints9::SetMesh(LPD3DXMESH pNewMesh, LPD3DXSKININFO pNewSkin)
{
    HRESULT hr = S_OK;

    NumPoints = 0;
    
    UnskinnedVB.resize(0);
    SAFE_RELEASE(SkinnedVB);
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

    NumPoints = pNewMesh->GetNumVertices();

    int MeshStride = pNewMesh->GetNumBytesPerVertex();


    if(pNewSkin)
    {
        V( pNewSkin->Clone( &Skin ) ); 
        V( Skin->SetDeclaration(Elements) );
    }


    //GET VERTEX DATA

    BYTE* pSrcVB= NULL;
    V(  pNewMesh->LockVertexBuffer( D3DLOCK_READONLY, (LPVOID*)&pSrcVB ) );
    UnskinnedVB.resize(pNewMesh->GetNumVertices());
    for( DWORD iVert = 0; iVert < pNewMesh->GetNumVertices(); iVert++)
    {
        Vertex& v0 = UnskinnedVB[iVert];

        v0.Position = *(D3DXVECTOR3*) (pSrcVB+(MeshStride*iVert)+declIn[iPos].Offset);
    }
    V( pNewMesh->UnlockVertexBuffer() );

    V( device->CreateVertexBuffer( NumPoints*Stride , D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &SkinnedVB, NULL) );
    //Fill in with initial values so unskinned meshes do not have to do this every render.
    pSrcVB=(BYTE*)(void*)&UnskinnedVB.front();
    BYTE* pDstVB=NULL;
    V( SkinnedVB->Lock(0, 0, (void**)&pDstVB, 0 ) );
    {
        memcpy( pDstVB, pSrcVB, Stride*pNewMesh->GetNumVertices() );
    }
    V( SkinnedVB->Unlock() );
}

void CShowPoints9::Move(D3DXMATRIX* pBoneTranforms, D3DXMATRIX* pBoneInvTransposes)
{
    HRESULT hr = S_OK;
    if(Skin && pBoneTranforms)
    {    
        BYTE* pSrc=(BYTE*)(void*)&UnskinnedVB.front();
        BYTE* pDst=NULL;
        V( SkinnedVB->Lock(0, 0, (void**)&pDst, 0 ) );
        V( Skin->UpdateSkinnedMesh( pBoneTranforms, pBoneInvTransposes, pSrc, pDst ) );
        V( SkinnedVB->Unlock() );
    }

}

void CShowPoints9::Draw(const D3DXMATRIX& WorldMatrix)
{
    HRESULT hr = S_OK;

    if(!SkinnedVB)
        return;

    IDirect3DDevice9* device = DXUTGetD3D9Device();


    V( Effect->SetMatrix( "World", &WorldMatrix) );
    V( Effect->SetMatrix( "View", DXVGetCamera().GetViewMatrix() ));
    V( Effect->SetMatrix( "Projection", DXVGetCamera().GetProjMatrix() ));
    V( Effect->SetVector( "Color", &D3DXVECTOR4(1.0f,1.0f,0.0f,1.0f)) );

    V( Effect->SetTechnique( Effect->GetTechnique(0)) );

    UINT numPasses;
    V( Effect->Begin( &numPasses, 0 ));

    for( UINT iPass = 0; iPass < numPasses; iPass++ )
    {
        V( Effect->BeginPass( iPass ) );

        V( device->SetVertexDeclaration( Declaration ) );
        V( device->SetStreamSource(0, SkinnedVB, 0 , Stride ) );

        V( device->DrawPrimitive( D3DPT_POINTLIST, 0, NumPoints) );

        V( Effect->EndPass() );
    }
    V( Effect->End() );
}



void CShowPoints9::OnLostDevice()
{ 
    if(Effect)Effect->OnLostDevice(); 
}

void CShowPoints9::OnResetDevice()
{ 
    if(Effect)Effect->OnResetDevice(); 
}
