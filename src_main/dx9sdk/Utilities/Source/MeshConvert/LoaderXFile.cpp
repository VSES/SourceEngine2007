//--------------------------------------------------------------------------------------
// File: LoaderXFile.cpp
//
// Copyright (c) Microsoft Corp. All rights reserved.
//--------------------------------------------------------------------------------------
#include "stdafx.h"
#define COLOR_TO_VECTOR4( val ) D3DXVECTOR4( val.r, val.g, val.b, val.a )


//--------------------------------------------------------------------------------------
CLoaderXFile::CLoaderXFile() : m_pMats(NULL), m_pAttr(NULL), m_pMesh(NULL)
{
}

//--------------------------------------------------------------------------------------
CLoaderXFile::~CLoaderXFile()
{
    SAFE_DELETE_ARRAY( m_pMats );
    SAFE_DELETE_ARRAY( m_pAttr );
    SAFE_RELEASE( m_pMesh );
}

//--------------------------------------------------------------------------------------
HRESULT CLoaderXFile::Load( WCHAR* szFileName, FRAME_TRANSFORM_TYPE requestedBHT )
{
    HRESULT hr = E_FAIL;
    ID3DXBuffer *pMat = NULL;
    ID3DXMesh *pRawMesh = NULL;
    ID3DXMesh *pMesh = NULL;
    DWORD cMat;
    IDirect3DDevice9* pDev9 = NULL;
    DWORD* pAdjBuffer = NULL;

    // Create a d3d9 object
    IDirect3D9* pD3D9 = Direct3DCreate9( D3D_SDK_VERSION );

    if( pD3D9 == NULL )
        return E_FAIL;

    D3DPRESENT_PARAMETERS pp;
    pp.BackBufferWidth = 320;
    pp.BackBufferHeight = 240;
    pp.BackBufferFormat = D3DFMT_X8R8G8B8;
    pp.BackBufferCount = 1;
    pp.MultiSampleType = D3DMULTISAMPLE_NONE;
    pp.MultiSampleQuality = 0;
    pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    pp.hDeviceWindow = GetShellWindow();
    pp.Windowed = true;
    pp.Flags = 0;
    pp.FullScreen_RefreshRateInHz = 0;
    pp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
    pp.EnableAutoDepthStencil = false;

    hr = pD3D9->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_NULLREF, NULL, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &pp, &pDev9 );
    if(FAILED(hr))
        goto Error;

    if( szFileName )
    {
        hr = D3DXLoadMeshFromX( szFileName, 0, pDev9, NULL, &pMat, NULL, &cMat, &pRawMesh );
        if(FAILED(hr))
            goto Error;
    }

    D3DVERTEXELEMENT9 declTanBi[] = 
    {
        { 0, 0,  D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
        { 0, 12, D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0 },
        { 0, 24, D3DDECLTYPE_FLOAT2,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
        { 0, 32, D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT,  0 },
        { 0, 44, D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL,  0 },
        D3DDECL_END()
    };
    D3DVERTEXELEMENT9* pdecl = declTanBi;

    // Make a clone with the desired vertex format.
    if( SUCCEEDED( pRawMesh->CloneMesh( D3DXMESH_32BIT | D3DXMESH_DYNAMIC, pdecl, pDev9, &m_pMesh ) ) )
    {
        // Optimize
        pAdjBuffer = new DWORD[ 3*m_pMesh->GetNumFaces() ];
        if( !pAdjBuffer )
        {
            hr = E_OUTOFMEMORY;
            goto Error;
        }
        m_pMesh->GenerateAdjacency( 0.001f, pAdjBuffer );
        m_pMesh->OptimizeInplace( D3DXMESHOPT_ATTRSORT,
                                    pAdjBuffer,
                                    NULL,
                                    NULL,
                                    NULL );

        // Attributes
        m_pMesh->GetAttributeTable( NULL, &m_dwNumAttr );
        if( m_dwNumAttr > 0 )
        {
            m_pAttr = new D3DXATTRIBUTERANGE[m_dwNumAttr];
            m_pMesh->GetAttributeTable( m_pAttr, &m_dwNumAttr );
        }

        // Materials
        m_dwNumMaterials = cMat;
        if( m_dwNumMaterials > 0 )
        {
            D3DXMATERIAL* pMaterialBuffer = (D3DXMATERIAL*)pMat->GetBufferPointer();
            m_pMats = new D3DXMATERIAL[ m_dwNumMaterials ];
            if( !m_pMats )
            {
                hr = E_OUTOFMEMORY;
                goto Error;
            }

            for( DWORD m=0; m<m_dwNumMaterials; m++ )
            {
                CopyMemory( &m_pMats[m], &pMaterialBuffer[m], sizeof(D3DXMATERIAL) );
            }
        }

        // Create the intermediate mesh
        hr = CreateIntermediateMesh( declTanBi, 6 );
        if(FAILED(hr))
            goto Error;
    }

    hr = S_OK;
Error:
    SAFE_RELEASE( pMat );
    SAFE_RELEASE( pRawMesh );
    SAFE_RELEASE( pMesh );
    SAFE_RELEASE( pDev9 );
    SAFE_RELEASE( pD3D9 );
    SAFE_DELETE_ARRAY( pAdjBuffer );

    return hr;
}


//--------------------------------------------------------------------------------------
CIntermediateMesh* CLoaderXFile::GetMesh()
{
    return m_pIntermediateMesh;
}


//--------------------------------------------------------------------------------------
HRESULT CLoaderXFile::CreateIntermediateMesh( D3DVERTEXELEMENT9* pDecl,
                                              UINT NumElements )
{
    HRESULT hr = E_FAIL;
    D3DXMATRIX mIdentity;

    // Create the Mesh
    m_pIntermediateMesh = new CIntermediateMesh( 0, TEXT("XFileMesh") );
    INTERMEDIATE_MESH* pMesh = m_pIntermediateMesh->AddMesh( "" );

    // Get the Vertices into the Mesh
    DWORD dwNumVertices = m_pMesh->GetNumVertices();
    DWORD dwVertexStride = m_pMesh->GetNumBytesPerVertex();
    void* pVertices = NULL;
    hr = m_pMesh->LockVertexBuffer( 0, &pVertices );
    if(FAILED(hr))
        goto Error;
    m_pIntermediateMesh->AddVertexBuffer(pMesh, 0, dwNumVertices, dwNumVertices*dwVertexStride, dwVertexStride, pDecl, NumElements, pVertices );
    m_pMesh->UnlockVertexBuffer();

    // Get the Indices into the Mesh
    DWORD dwNumFaces = m_pMesh->GetNumFaces();
    void* pIndices = NULL;
    hr = m_pMesh->LockIndexBuffer( 0, &pIndices );
    if(FAILED(hr))
        goto Error;
    m_pIntermediateMesh->AddIndexBuffer( pMesh, dwNumFaces*3, dwNumFaces*3*sizeof(DWORD), IT_32BIT, pIndices );
    m_pMesh->UnlockIndexBuffer();

    // Get the Materials into the mesh
    for( DWORD m=0; m<m_dwNumMaterials; m++ )
    {
        m_pIntermediateMesh->AddMaterial( "", 
                                          "", 
                                          m_pMats[m].pTextureFilename, 
                                          NULL, 
                                          NULL, 
                                          COLOR_TO_VECTOR4( m_pMats[m].MatD3D.Diffuse ),
                                          COLOR_TO_VECTOR4( m_pMats[m].MatD3D.Ambient ),
                                          COLOR_TO_VECTOR4( m_pMats[m].MatD3D.Specular ),
                                          COLOR_TO_VECTOR4( m_pMats[m].MatD3D.Emissive ),
                                          m_pMats[m].MatD3D.Power );
    }

    // Get the subsets into the mesh
    for( DWORD s=0; s<m_dwNumAttr; s++ )
    {
        m_pIntermediateMesh->AddSubset( pMesh,
                                        "",
                                        m_pAttr[s].AttribId,
                                        PT_TRIANGLE_LIST,
                                        m_pAttr[s].FaceStart*3,
                                        m_pAttr[s].FaceCount*3,
                                        /*m_pAttr[s].VertexStart,*/0,
                                        m_pAttr[s].VertexCount );
    }

    // Add a default frame
    D3DXMatrixIdentity( &mIdentity );
    m_pIntermediateMesh->AddFrame( "", NULL, mIdentity, 0 );

    hr = S_OK;
Error:
    return hr;
}