#include <vector>
#include "sas.h"
#include "DXUT.h"

#include "Model10.h"
#include "Render10.h"
#include "Camera.h"
#include "StateBlock.h"
#include "App.h"
#include "resource.h"

#include "ShowShared10.h"


DECLARE_SINGLETON_CPP( ShowShared10 );

D3D10_INPUT_ELEMENT_DESC ShowShared10::DECL[] =
{
    { "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
    { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },
    { "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0 },
    { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36, D3D10_INPUT_PER_VERTEX_DATA, 0 },
};



void ShowShared10::Startup()
{
    HRESULT hr = S_OK;
    Shutdown();

    ID3D10Device* pDevice = DXUTGetD3D10Device();




    ID3D10Blob *pErrors=NULL;
    V( D3DX10CreateEffectFromResourceW(
        NULL, 
        MAKEINTRESOURCE(IDR_SHOW10FX),
        L"Show10.Fx", 
        NULL, 
        NULL,
        "fx_4_0",
        0,
        0, 
        pDevice, 
        NULL, 
        NULL, 
        &Effect,
        &pErrors,
        NULL ) );

    if(pErrors)
        DXVGetApp().OutputA( (const char*)pErrors->GetBufferPointer() );
    SAFE_RELEASE(pErrors);


    Adjacencies = Effect->GetTechniqueByName("Adjacencies")->GetPassByIndex(0);
    Bounds = Effect->GetTechniqueByName("Bounds")->GetPassByIndex(0);
    Creases = Effect->GetTechniqueByName("Creases")->GetPassByIndex(0);
    Edges = Effect->GetTechniqueByName("Edges")->GetPassByIndex(0);
    Normals = Effect->GetTechniqueByName("Normals")->GetPassByIndex(0);
    Binormals = Effect->GetTechniqueByName("Binormals")->GetPassByIndex(0);
    Tangents = Effect->GetTechniqueByName("Tangents")->GetPassByIndex(0);
    Points = Effect->GetTechniqueByName("Points")->GetPassByIndex(0);

    Min = Effect->GetVariableByName("MinPosition")->AsVector();
    Max = Effect->GetVariableByName("MaxPosition")->AsVector();

    World = Effect->GetVariableByName("World")->AsMatrix();
    View = Effect->GetVariableByName("View")->AsMatrix();
    Projection = Effect->GetVariableByName("Projection")->AsMatrix();

    LineSize = Effect->GetVariableByName("LineSize")->AsScalar();


    D3D10_BUFFER_DESC vbDesc =
    {
        sizeof(D3DXVECTOR3)*4,
        D3D10_USAGE_DEFAULT,
        D3D10_BIND_VERTEX_BUFFER,
        0,
        0
    };

    V( pDevice->CreateBuffer( &vbDesc, NULL, &BoundsBuffer ) ); 

    D3D10_PASS_DESC passDesc;
    Effect->GetTechniqueByIndex(0)->GetPassByIndex(0)->GetDesc( &passDesc );
    V( pDevice->CreateInputLayout( DECL, sizeof(DECL)/sizeof(D3D10_INPUT_ELEMENT_DESC), passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &BoundsLayout) ); 
}

void ShowShared10::Shutdown()
{
    SAFE_RELEASE(BoundsLayout);
    SAFE_RELEASE(BoundsBuffer);
    SAFE_RELEASE(Effect);
}


HRESULT ShowShared10::CreateInputLayout( ID3DX10Mesh* pMesh10, ID3D10InputLayout** layout)
{
    D3D10_PASS_DESC passDesc;
    Effect->GetTechniqueByIndex(0)->GetPassByIndex(0)->GetDesc( &passDesc );

    const D3D10_INPUT_ELEMENT_DESC* decl;
    UINT declLength;
    pMesh10->GetVertexDescription( &decl, &declLength);

    return DXUTGetD3D10Device()->CreateInputLayout(decl, declLength, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, layout);
}

void ShowShared10::Move()
{
    Min->SetFloatVector( (FLOAT*)DXVGetModel10().GetMin() );
    Max->SetFloatVector( (FLOAT*)DXVGetModel10().GetMax() );

    View->SetMatrix( (FLOAT*)DXVGetCamera().GetViewMatrix() );
    Projection->SetMatrix( (FLOAT*)DXVGetCamera().GetProjMatrix() );

    LineSize->SetFloat( DXVGetRender10().GetModelRadius()*0.05f );
}

void ShowShared10::DrawBounds()
{
    ID3D10Device* pDevice = DXUTGetD3D10Device();


    ID3D10Buffer* Buffers[] = { BoundsBuffer };
    UINT Strides[] = { sizeof(D3DXVECTOR3) };
    UINT Offsets[] = { 0 };

    pDevice->IASetInputLayout( BoundsLayout );
    pDevice->IASetIndexBuffer(NULL, DXGI_FORMAT_R32_UINT, 0);
    pDevice->IASetVertexBuffers( 0, 1, Buffers, Strides, Offsets );
    pDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_POINTLIST );

    World->SetMatrix( (FLOAT*)DXVGetCamera().GetWorldMatrix() );
    Bounds->Apply(0);
    pDevice->Draw(1, 0);
}

void ShowShared10::DrawAdjacencies(const D3DXMATRIX& WorldMatrix, ID3DX10Mesh* pMesh10, UINT AttribCount, ID3D10InputLayout* layout)
{
    ID3D10Device* pDevice = DXUTGetD3D10Device();
    pDevice->IASetInputLayout( BoundsLayout );
    
    World->SetMatrix( (float*)(void*)&WorldMatrix );
    Adjacencies->Apply(0);
    for( UINT i = 0; i < AttribCount; i++ )
        pMesh10->DrawSubset( i );
}

void ShowShared10::DrawCreases(const D3DXMATRIX& WorldMatrix, ID3DX10Mesh* pMesh10, UINT AttribCount, ID3D10InputLayout* layout)
{
    ID3D10Device* pDevice = DXUTGetD3D10Device();
    pDevice->IASetInputLayout( BoundsLayout );
    
    World->SetMatrix( (float*)(void*)&WorldMatrix );
    Creases->Apply(0);
    for( UINT i = 0; i < AttribCount; i++ )
        pMesh10->DrawSubset( i );
}

void ShowShared10::DrawEdges(const D3DXMATRIX& WorldMatrix, ID3DX10Mesh* pMesh10, UINT AttribCount, ID3D10InputLayout* layout)
{
    ID3D10Device* pDevice = DXUTGetD3D10Device();
    pDevice->IASetInputLayout( BoundsLayout );
    
    World->SetMatrix( (float*)(void*)&WorldMatrix );
    Edges->Apply(0);
    for( UINT i = 0; i < AttribCount; i++ )
        pMesh10->DrawSubset( i );
}

void ShowShared10::DrawNormals(const D3DXMATRIX& WorldMatrix, ID3DX10Mesh* pMesh10, UINT AttribCount, ID3D10InputLayout* layout)
{
    ID3D10Device* pDevice = DXUTGetD3D10Device();
    pDevice->IASetInputLayout( BoundsLayout );
    
    World->SetMatrix( (float*)(void*)&WorldMatrix );
    Normals->Apply(0);
    for( UINT i = 0; i < AttribCount; i++ )
        pMesh10->DrawSubset( i );
}

void ShowShared10::DrawBinormals(const D3DXMATRIX& WorldMatrix, ID3DX10Mesh* pMesh10, UINT AttribCount, ID3D10InputLayout* layout)
{
    ID3D10Device* pDevice = DXUTGetD3D10Device();
    pDevice->IASetInputLayout( BoundsLayout );
    
    World->SetMatrix( (float*)(void*)&WorldMatrix );
    Binormals->Apply(0);
    for( UINT i = 0; i < AttribCount; i++ )
        pMesh10->DrawSubset( i );
}

void ShowShared10::DrawTangents(const D3DXMATRIX& WorldMatrix, ID3DX10Mesh* pMesh10, UINT AttribCount, ID3D10InputLayout* layout)
{
    ID3D10Device* pDevice = DXUTGetD3D10Device();
    pDevice->IASetInputLayout( BoundsLayout );
    
    World->SetMatrix( (float*)(void*)&WorldMatrix );
    Tangents->Apply(0);
    for( UINT i = 0; i < AttribCount; i++ )
        pMesh10->DrawSubset( i );
}

void ShowShared10::DrawPoints(const D3DXMATRIX& WorldMatrix, ID3DX10Mesh* pMesh10, UINT AttribCount, ID3D10InputLayout* layout)
{
    ID3D10Device* pDevice = DXUTGetD3D10Device();
    pDevice->IASetInputLayout( BoundsLayout );
    
    World->SetMatrix( (float*)(void*)&WorldMatrix );
    Points->Apply(0);
    for( UINT i = 0; i < AttribCount; i++ )
        pMesh10->DrawSubset( i );
}
