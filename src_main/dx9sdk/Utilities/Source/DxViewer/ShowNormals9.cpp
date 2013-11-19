//--------------------------------------------------------------------------------------
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include "d3d9.h"
#include "d3dx9.h"
#include "sas.h"

#include "resource.h"
#include "dxut.h"
#include "render9.h"
#include "Camera.h"
#include "App.h"
#include "ShowNormals9.h"



int CShowNormals9::Stride = sizeof(CShowNormals9::Vertex);

D3DVERTEXELEMENT9 CShowNormals9::Elements[5] = 
{
    {0, 0*sizeof(D3DXVECTOR3), D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
    {0, 1*sizeof(D3DXVECTOR3), D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
    {0, 2*sizeof(D3DXVECTOR3), D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0},
    {0, 3*sizeof(D3DXVECTOR3), D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0},
    D3DDECL_END()
};



void CShowNormals9::SetMesh(LPD3DXMESH pNewMesh, LPD3DXSKININFO pNewSkin)
{
    HRESULT hr = S_OK;

    NumLines = 0;
    
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
             NULL, MAKEINTRESOURCE(IDR_SHOWVECTORS9FX), MAKEINTRESOURCE(RT_RCDATA),
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
    int iTan= -1;
    int iBi= -1;
    for( int i = 0 ; 
        declIn[i].Stream != 255 && i < MAX_FVF_DECL_SIZE;
        i++)
    {
        if(declIn[i].Usage == D3DDECLUSAGE_POSITION && declIn[i].UsageIndex == 0)
            iPos = i;
        if(declIn[i].Usage == D3DDECLUSAGE_NORMAL && declIn[i].UsageIndex == 0)
            iNorm = i;
        if(declIn[i].Usage == D3DDECLUSAGE_TANGENT && declIn[i].UsageIndex == 0)
            iTan = i;
        if(declIn[i].Usage == D3DDECLUSAGE_BINORMAL && declIn[i].UsageIndex == 0)
            iBi = i;
    }

    if(iPos == -1 || iNorm == -1 || iTan == -1 || iBi == -1)
        return;

    if( (( declIn[iPos].Type & (D3DDECLTYPE_FLOAT3|D3DDECLTYPE_FLOAT4)) == 0 )|| 
        (( declIn[iNorm].Type & (D3DDECLTYPE_FLOAT3|D3DDECLTYPE_FLOAT4)) == 0 ) ||
        (( declIn[iTan].Type & (D3DDECLTYPE_FLOAT3|D3DDECLTYPE_FLOAT4)) == 0 ) ||
        (( declIn[iBi].Type & (D3DDECLTYPE_FLOAT3|D3DDECLTYPE_FLOAT4)) == 0 ) )
        return;

    NumLines = pNewMesh->GetNumVertices();

    int NormalsSizeInBytes = Stride*NumLines*2;
    int MeshStride = pNewMesh->GetNumBytesPerVertex();



    V( device->CreateVertexBuffer( NormalsSizeInBytes , 0, 0, D3DPOOL_MANAGED, &SkinnedVB, NULL) );

    BYTE* pSrc= NULL;
    V(  pNewMesh->LockVertexBuffer( D3DLOCK_READONLY, (LPVOID*)&pSrc ) );

    D3DXVECTOR3 zero(0,0,0);
    UnskinnedVB.resize( NumLines*2 );
    for( DWORD iVert = 0; iVert < pNewMesh->GetNumVertices(); iVert++)
    {
        Vertex& v0 = UnskinnedVB[(iVert*2)];
        Vertex& v1 = UnskinnedVB[(iVert*2)+1];

        v0.Position = *(D3DXVECTOR3*) (pSrc+(MeshStride*iVert)+declIn[iPos].Offset);
        v0.Normal = zero;
        v0.Tangent = zero;
        v0.Binormal = zero;

        v1 = v0;
        v1.Normal = *(D3DXVECTOR3*) (pSrc+(MeshStride*iVert)+declIn[iNorm].Offset);
        v1.Tangent = *(D3DXVECTOR3*) (pSrc+(MeshStride*iVert)+declIn[iTan].Offset);
        v1.Binormal = *(D3DXVECTOR3*) (pSrc+(MeshStride*iVert)+declIn[iBi].Offset);
    }
    V( pNewMesh->UnlockVertexBuffer() );

    //Fill in with initial values so unskinned meshes do not have to do this every render.
    pSrc=(BYTE*)(void*)&UnskinnedVB.front();
    BYTE* pDst=NULL;
    V( SkinnedVB->Lock(0, 0, (void**)&pDst, 0 ) );
    {
        memcpy( pDst, pSrc, Stride*NumLines*2 );
    }
    V( SkinnedVB->Unlock() );


    if(pNewSkin)
    {
        V( D3DXCreateSkinInfo( NumLines*2, Elements, pNewSkin->GetNumBones(), &Skin) );
        for( UINT iBone = 0; iBone < pNewSkin->GetNumBones(); iBone++)
        {
            DWORD numInfs = pNewSkin->GetNumBoneInfluences(iBone);

            std::vector<DWORD> srcVertices( numInfs );
            std::vector<FLOAT> srcWeights( numInfs );
            std::vector<DWORD> dstVertices( numInfs * 2);
            std::vector<FLOAT> dstWeights( numInfs * 2);

            V( pNewSkin->GetBoneInfluence( iBone, &(*srcVertices.begin()) , &(*srcWeights.begin()) ) );

        
            for( UINT iInf = 0; iInf < numInfs; iInf++ )
            {
                DWORD vert = srcVertices[iInf];
                FLOAT weight = srcWeights[iInf];
                
                dstVertices[iInf*2+0] = vert*2+0;
                dstVertices[iInf*2+1] = vert*2+1;
                dstWeights[iInf*2+0] = weight;
                dstWeights[iInf*2+1] = weight;
            }

            V( Skin->SetBoneOffsetMatrix( iBone, pNewSkin->GetBoneOffsetMatrix(iBone)) );
            V( Skin->SetBoneInfluence(iBone, numInfs*2 , &(*dstVertices.begin()), &(*dstWeights.begin()) ) );
        }
    }
}

void CShowNormals9::Move(D3DXMATRIX* pBoneTranforms, D3DXMATRIX* pBoneInvTransposes)
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

void CShowNormals9::Draw(D3DDECLUSAGE usage, const D3DXMATRIX& WorldMatrix)
{
    HRESULT hr = S_OK;

    if(!SkinnedVB)
        return;

    IDirect3DDevice9* device = DXUTGetD3D9Device();

    V( Effect->SetMatrix( "World", &WorldMatrix) );
    V( Effect->SetMatrix( "View", DXVGetCamera().GetViewMatrix() ));
    V( Effect->SetMatrix( "Projection", DXVGetCamera().GetProjMatrix() ));

    if(usage == D3DDECLUSAGE_NORMAL)
    {
        V( Effect->SetVector( "Color", &D3DXVECTOR4(1,0,0,1)) );
    }
    else if(usage == D3DDECLUSAGE_TANGENT)
    {
        V( Effect->SetVector( "Color", &D3DXVECTOR4(0,0,1,1)) );
    }
    else if(usage == D3DDECLUSAGE_BINORMAL)
    {
        V( Effect->SetVector( "Color", &D3DXVECTOR4(0,1,0,1)) );
    }



    V( Effect->SetInt( "RenderUsage", usage) );
    V( Effect->SetFloat( "Scale", DXVGetRender9().GetModelRadius()*0.05f) );



    V( Effect->SetTechnique( Effect->GetTechnique(0)) );

    UINT numPasses;
    V( Effect->Begin( &numPasses, 0 ));

    for( UINT iPass = 0; iPass < numPasses; iPass++ )
    {
        V( Effect->BeginPass( iPass ) );

        V( device->SetVertexDeclaration( Declaration ) );
        V( device->SetStreamSource(0, SkinnedVB, 0 , Stride ) );

        V( device->DrawPrimitive( D3DPT_LINELIST, 0, NumLines) );

        V( Effect->EndPass() );
    }
    V( Effect->End() );
}




void CShowNormals9::OnLostDevice()
{ 
    if(Effect)Effect->OnLostDevice(); 
}

void CShowNormals9::OnResetDevice()
{ 
    if(Effect)Effect->OnResetDevice(); 
}
