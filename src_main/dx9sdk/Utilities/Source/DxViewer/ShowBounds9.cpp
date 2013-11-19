//--------------------------------------------------------------------------------------
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include "d3d9.h"
#include "d3dx9.h"
#include "sas.h"

#include "resource.h"
#include "dxut.h"
#include "render9.h"
#include "ShowBounds9.h"
#include "ShowShared9.h"
#include "camera.h"


int CShowBounds9::Stride = 1*sizeof(D3DXVECTOR3);
D3DVERTEXELEMENT9 CShowBounds9::Elements[2] = 
{
    {0, 0*sizeof(D3DXVECTOR3), D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
    D3DDECL_END()
};

void CShowBounds9::SetBounds(const D3DXVECTOR3& minVec, const D3DXVECTOR3& maxVec)
{
    HRESULT hr = S_OK;

    IDirect3DDevice9* device = DXUTGetD3D9Device();

    {//EFFECT
        LPD3DXBUFFER pErrorsAndWarnings= NULL;
        if(FAILED( D3DXCreateEffectFromResource( device, NULL, MAKEINTRESOURCE(IDR_SHOWLINES9FX), 
                                                NULL, NULL, 0, NULL, &Effect, &pErrorsAndWarnings )) )
        {
            MessageBoxA(NULL, (LPCSTR)(!pErrorsAndWarnings?"":pErrorsAndWarnings->GetBufferPointer()), "ShowBounds: Error loading Effect", 0 );
        }
        SAFE_RELEASE(pErrorsAndWarnings);
    }//EFFECT

    V( device->CreateVertexDeclaration( Elements, &Declaration ) );

    V( device->CreateIndexBuffer( 12*2*sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &IB, NULL) );

    WORD* pSrcIB=NULL;
    V( IB->Lock(0, 0, (void**)&pSrcIB, 0 ) );
    pSrcIB[0*2+0] = 0; pSrcIB[0*2+1] = 1;
    pSrcIB[1*2+0] = 1; pSrcIB[1*2+1] = 2;
    pSrcIB[2*2+0] = 2; pSrcIB[2*2+1] = 3;
    pSrcIB[3*2+0] = 3; pSrcIB[3*2+1] = 0;

    pSrcIB[4*2+0] = 4; pSrcIB[4*2+1] = 5;
    pSrcIB[5*2+0] = 5; pSrcIB[5*2+1] = 6;
    pSrcIB[6*2+0] = 6; pSrcIB[6*2+1] = 7;
    pSrcIB[7*2+0] = 7; pSrcIB[7*2+1] = 4;

    pSrcIB[8*2+0] = 0; pSrcIB[8*2+1] = 4;
    pSrcIB[9*2+0] = 1; pSrcIB[9*2+1] = 5;
    pSrcIB[10*2+0] = 2; pSrcIB[10*2+1] = 6;
    pSrcIB[11*2+0] = 3; pSrcIB[11*2+1] = 7;
    V( IB->Unlock() );

    V( device->CreateVertexBuffer( 8*Stride , D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &VB, NULL) );

    Vertex* pSrcVB=NULL;
    V( VB->Lock(0, 0, (void**)&pSrcVB, 0 ) );
    GetBox(&pSrcVB[0].Position, sizeof(Vertex), minVec, maxVec);
    V( VB->Unlock() );
}


void CShowBounds9::GetBox(D3DXVECTOR3* v0, UINT stride,
                         const D3DXVECTOR3& minVec, 
                         const D3DXVECTOR3& maxVec)
{
    *(D3DXVECTOR3*)(((BYTE*)v0)+(stride*0)) = D3DXVECTOR3(minVec.x, minVec.y, minVec.z);
    *(D3DXVECTOR3*)(((BYTE*)v0)+(stride*1)) = D3DXVECTOR3(minVec.x, maxVec.y, minVec.z);
    *(D3DXVECTOR3*)(((BYTE*)v0)+(stride*2)) = D3DXVECTOR3(maxVec.x, maxVec.y, minVec.z);
    *(D3DXVECTOR3*)(((BYTE*)v0)+(stride*3)) = D3DXVECTOR3(maxVec.x, minVec.y, minVec.z);
    *(D3DXVECTOR3*)(((BYTE*)v0)+(stride*4)) = D3DXVECTOR3(minVec.x, minVec.y, maxVec.z);
    *(D3DXVECTOR3*)(((BYTE*)v0)+(stride*5)) = D3DXVECTOR3(minVec.x, maxVec.y, maxVec.z);
    *(D3DXVECTOR3*)(((BYTE*)v0)+(stride*6)) = D3DXVECTOR3(maxVec.x, maxVec.y, maxVec.z);
    *(D3DXVECTOR3*)(((BYTE*)v0)+(stride*7)) = D3DXVECTOR3(maxVec.x, minVec.y, maxVec.z);
}



void CShowBounds9::Draw()
{
    HRESULT hr = S_OK;

    IDirect3DDevice9* device = DXUTGetD3D9Device();

    V( Effect->SetMatrix( "World", DXVGetCamera().GetWorldMatrix()) );
    V( Effect->SetMatrix( "View", DXVGetCamera().GetViewMatrix() ) );
    V( Effect->SetMatrix( "Projection", DXVGetCamera().GetProjMatrix() ) );

    V( Effect->SetVector( "Color", &D3DXVECTOR4(1.0f,1.0f,1.0f,1.0f)) );

    V( Effect->SetTechnique( Effect->GetTechnique(0)) );

    UINT numPasses;
    V( Effect->Begin( &numPasses, 0 ));

    for( UINT iPass = 0; iPass < numPasses; iPass++ )
    {
        V( Effect->BeginPass( iPass ) );

        V( device->SetVertexDeclaration( Declaration ) );
        V( device->SetStreamSource(0, VB, 0 , Stride ) );
        V( device->SetIndices(IB) );

        V( device->DrawIndexedPrimitive( D3DPT_LINELIST, 0, 0, 8, 0, 12) );

        V( Effect->EndPass() );
    }
    V( Effect->End() );
}

void CShowBounds9::OnLostDevice()
{ 
    if(Effect)Effect->OnLostDevice(); 
}

void CShowBounds9::OnResetDevice()
{ 
    if(Effect)Effect->OnResetDevice(); 
}

void CShowBounds9::OnDestroyDevice()
{ 
        SAFE_RELEASE(VB);
        SAFE_RELEASE(IB);
        SAFE_RELEASE(Declaration);
        SAFE_RELEASE( Effect );
}