//--------------------------------------------------------------------------------------
// File: IntermediateMesh.cpp
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include "stdafx.h"

#define COLOR_FROM_VEC4( color, vec4 ) { color.r = vec4.x; color.g = vec4.y; color.b = vec4.z; color.a = vec4.w; }
#define	EndianSwap16(type)  (((type >> 8) & 0x00ff) | ((type << 8) & 0xff00))
#define	EndianSwap32(type)  (((type >>24) & 0x000000ff) | ((type >> 8) & 0x0000ff00) | ((type << 8) & 0x00ff0000) | ((type <<24) & 0xff000000) )

//--------------------------------------------------------------------------------------
// Add a vertex buffer to the mesh
//--------------------------------------------------------------------------------------
INTERMEDIATE_VERTEX_BUFFER* CIntermediateMesh::AddVertexBuffer( INTERMEDIATE_MESH* pMesh,
                                                                UINT StreamID, 
                                                                UINT64 NumVertices,
                                                                UINT64 SizeBytes,
                                                                UINT64 StrideBytes,
                                                                D3DVERTEXELEMENT9* pDecl,
                                                                UINT NumDeclElements,
                                                                void* pBinaryData )
{
    if( StreamID >= MAX_VERTEX_STREAMS )
        return NULL;
    if( NumDeclElements > MAX_VERTEX_ELEMENTS )
        NumDeclElements = MAX_VERTEX_ELEMENTS;
    
    // Create a new VB structure
    INTERMEDIATE_VERTEX_BUFFER* pVertexBuffer = new INTERMEDIATE_VERTEX_BUFFER;
    if( !pVertexBuffer )
        return NULL;

    // Load it out
    CopyMemory( pVertexBuffer->Decl, pDecl, NumDeclElements*sizeof(D3DVERTEXELEMENT9) );
    pVertexBuffer->Decl[NumDeclElements].Stream = 0xFF;
    pVertexBuffer->Decl[NumDeclElements].Offset = 0;
    pVertexBuffer->Decl[NumDeclElements].Type = D3DDECLTYPE_UNUSED;
    pVertexBuffer->Decl[NumDeclElements].Method = D3DDECLMETHOD_DEFAULT;
    pVertexBuffer->Decl[NumDeclElements].Usage = 0;
    pVertexBuffer->Decl[NumDeclElements].UsageIndex = 0;

    pVertexBuffer->NumVertices = NumVertices;
    pVertexBuffer->SizeBytes = SizeBytes;
    pVertexBuffer->StrideBytes = StrideBytes;
    pVertexBuffer->DataOffset = 0;	//TBD later
    pVertexBuffer->pVertices = new BYTE[ (size_t)SizeBytes ];
    if( !pVertexBuffer->pVertices )
        return NULL;

    // Grab memory
    CopyMemory( pVertexBuffer->pVertices, (BYTE*)pBinaryData, (size_t)SizeBytes );

    // Update the mesh object
    pMesh->VertexBuffers[ StreamID ] = m_VertexBufferArray.GetCount();

    // Add the VB to the list
    m_VertexBufferArray.Push( pVertexBuffer );
    pMesh->NumVertexBuffers++;
    return pVertexBuffer;
}

//--------------------------------------------------------------------------------------
// Add an index buffer to the mesh
//--------------------------------------------------------------------------------------
INTERMEDIATE_INDEX_BUFFER* CIntermediateMesh::AddIndexBuffer( INTERMEDIATE_MESH* pMesh,
                                                              UINT64 NumIndices,
                                                              UINT64 SizeBytes,
                                                              UINT IndexType,
                                                              void* pBinaryData )
{
    // Create a new IB structure
    INTERMEDIATE_INDEX_BUFFER* pIndexBuffer = new INTERMEDIATE_INDEX_BUFFER;
    if( !pIndexBuffer )
        return NULL;

    // Load it out
    pIndexBuffer->NumIndices = NumIndices;
    pIndexBuffer->SizeBytes = SizeBytes;
    pIndexBuffer->IndexType = IndexType;
    pIndexBuffer->DataOffset = 0; //TBD later
    pIndexBuffer->pIndices = new BYTE[ (size_t)SizeBytes ];
    if( !pIndexBuffer->pIndices )
        return NULL;

    // Grab memory
    CopyMemory( pIndexBuffer->pIndices, (BYTE*)pBinaryData, (size_t)SizeBytes );
    
   // Update the mesh object
    pMesh->IndexBuffer = m_IndexBufferArray.GetCount();

    // Add the VB to the list
    m_IndexBufferArray.Push( pIndexBuffer );
    return pIndexBuffer;
}

//--------------------------------------------------------------------------------------
// Add a mesh
//--------------------------------------------------------------------------------------
INTERMEDIATE_MESH* CIntermediateMesh::AddMesh( char* pszName )
{
    INTERMEDIATE_MESH* pMesh = new INTERMEDIATE_MESH;
    if( !pMesh )
        return NULL;

    ZeroMemory( pMesh, sizeof(INTERMEDIATE_MESH) );

    pMesh->Name[0] = 0;
    if( pszName )
        StringCchCopyA( pMesh->Name, MAX_MESH_NAME, pszName );

    m_MeshArray.Push( pMesh );
    return pMesh;
}

//--------------------------------------------------------------------------------------
// Add a frame influence to a mesh
//--------------------------------------------------------------------------------------
void CIntermediateMesh::AddFrameInfluence( INTERMEDIATE_MESH* pMesh,
                                           char* pszName )
{
    FRAME_NAME FrameName;

    FrameName.Name[0] = 0;
    if( pszName )
        StringCchCopyA( FrameName.Name, MAX_FRAME_NAME, pszName );

    pMesh->FrameInfluenceArray.Push( FrameName );
    pMesh->NumFrameInfluences = pMesh->FrameInfluenceArray.GetCount();
}

//--------------------------------------------------------------------------------------
// Add a subset to a mesh
//--------------------------------------------------------------------------------------
INTERMEDIATE_SUBSET* CIntermediateMesh::AddSubset( INTERMEDIATE_MESH* pMesh,
                                                   char* pszName,
                                                   UINT MaterialID,
                                                   UINT PrimitiveType,
                                                   UINT64 IndexStart,
                                                   UINT64 IndexCount,
                                                   UINT64 VertexStart,
                                                   UINT64 VertexCount )
{
    INTERMEDIATE_SUBSET* pSubset = new INTERMEDIATE_SUBSET;
    if( !pSubset )
        return NULL;

    pSubset->Name[0] = 0;
    if( pszName )
        StringCchCopyA( pSubset->Name, MAX_SUBSET_NAME, pszName );
    pSubset->MaterialID = MaterialID;
    pSubset->PrimitiveType = PrimitiveType;
    pSubset->IndexStart = IndexStart;
    pSubset->IndexCount = IndexCount;
    pSubset->VertexStart = VertexStart;
    pSubset->VertexCount = VertexCount;

    pMesh->SubsetIDArray.Push( m_SubsetArray.GetCount() );
    m_SubsetArray.Push( pSubset );
    pMesh->NumSubsets = pMesh->SubsetIDArray.GetCount();
    return pSubset;
}

//--------------------------------------------------------------------------------------
// Add a frame
//--------------------------------------------------------------------------------------
INTERMEDIATE_FRAME* CIntermediateMesh::AddFrame( char* pszName,
                                                 INTERMEDIATE_FRAME* pParent,
                                                 D3DXMATRIX Matrix,
                                                 UINT MeshID )
{
    INTERMEDIATE_FRAME* pFrame = new INTERMEDIATE_FRAME;
    if( !pFrame )
        return NULL;

    pFrame->Name[0] = 0;
    if( pszName )
        StringCchCopyA( pFrame->Name, MAX_FRAME_NAME, pszName );
    pFrame->Matrix = Matrix;
    pFrame->ChildFrame = INVALID_FRAME;
    pFrame->ParentFrame = INVALID_FRAME;
    pFrame->SiblingFrame = INVALID_FRAME;
    pFrame->id = m_FrameArray.GetCount();
    pFrame->Mesh = MeshID;
    pFrame->AnimationDataIndex = INVALID_ANIMATION_DATA;

    // fixup the relationships
    if( pParent )
    {
        pFrame->ParentFrame = pParent->id;
        if( pParent->ChildFrame == INVALID_FRAME )
        {
            pParent->ChildFrame = pFrame->id;
        }
        else
        {
            INTERMEDIATE_FRAME* pTestFrame = m_FrameArray.GetAt( pParent->ChildFrame );
            while( pTestFrame->SiblingFrame != INVALID_FRAME )
            {
                pTestFrame = m_FrameArray.GetAt( pTestFrame->SiblingFrame );
            }
            pTestFrame->SiblingFrame = pFrame->id;
        }
    }

    m_FrameArray.Push( pFrame );
    return pFrame;
}

//--------------------------------------------------------------------------------------
// Add a material
//--------------------------------------------------------------------------------------
INTERMEDIATE_MATERIAL* CIntermediateMesh::AddMaterial( char* pszMaterialName,
                                                            char* pszMaterialInstancePath,
                                                            char* pszDiffuseTexture,
                                                            char* pszNormalTexture,
                                                            char* pszSpecularTexture,
                                                            D3DXVECTOR4 Diffuse,
                                                            D3DXVECTOR4 Ambient,
                                                            D3DXVECTOR4 Specular,
                                                            D3DXVECTOR4 Emissive,
                                                            FLOAT Power )
{
    INTERMEDIATE_MATERIAL* pMaterial = new INTERMEDIATE_MATERIAL;
    if( !pMaterial )
        return NULL;

    ZeroMemory( pMaterial, sizeof(INTERMEDIATE_MATERIAL) );

    if( pszMaterialName )
        StringCchCopyA( pMaterial->Name, MAX_MATERIAL_NAME, pszMaterialName );
    if( pszMaterialInstancePath )
        StringCchCopyA( pMaterial->MaterialInstancePath, MAX_MATERIAL_NAME, pszMaterialInstancePath );
    if( pszDiffuseTexture )
        StringCchCopyA( pMaterial->DiffuseTexture, MAX_MATERIAL_NAME, pszDiffuseTexture );
    if( pszNormalTexture )
        StringCchCopyA( pMaterial->NormalTexture, MAX_MATERIAL_NAME, pszNormalTexture );
    if( pszSpecularTexture )
        StringCchCopyA( pMaterial->SpecularTexture, MAX_MATERIAL_NAME, pszSpecularTexture );

    pMaterial->Diffuse = Diffuse;
    pMaterial->Ambient = Ambient;
    pMaterial->Specular = Specular;
    pMaterial->Emissive = Emissive;
    pMaterial->Power = Power;

    m_MaterialArray.Push( pMaterial );
    return pMaterial;
}

//--------------------------------------------------------------------------------------
// Add an animation frame
//--------------------------------------------------------------------------------------
INTERMEDIATE_ANIMATION_FRAME_DATA* CIntermediateMesh::AddAnimationFrame( char* pszFrameName )
{
    INTERMEDIATE_ANIMATION_FRAME_DATA* pAnimFrame = new INTERMEDIATE_ANIMATION_FRAME_DATA;
    ZeroMemory( pAnimFrame, sizeof(INTERMEDIATE_ANIMATION_FRAME_DATA) );
    StringCchCopyA( pAnimFrame->FrameName, MAX_FRAME_NAME, pszFrameName );

    m_AnimationFrameData.Push( pAnimFrame );
    return pAnimFrame;
}

//--------------------------------------------------------------------------------------
void CIntermediateMesh::SetBigEndian( bool bBigEndian )
{
	m_bBigEndian = bBigEndian;
}

//--------------------------------------------------------------------------------------
bool CIntermediateMesh::IsBigEndian()
{
	return m_bBigEndian;
}

//--------------------------------------------------------------------------------------
UINT CIntermediateMesh::FrameHeirarchyStackGetCount()
{
	return m_FrameHeirarchyStack.GetCount();
}

//--------------------------------------------------------------------------------------
void CIntermediateMesh::FrameHeirarchyStackPush( INTERMEDIATE_FRAME* pFrame )
{
	m_FrameHeirarchyStack.Push( pFrame );
}

//--------------------------------------------------------------------------------------
INTERMEDIATE_FRAME* CIntermediateMesh::FrameHeirarchyStackPop()
{
	return m_FrameHeirarchyStack.Pop();
}

//--------------------------------------------------------------------------------------
INTERMEDIATE_FRAME* CIntermediateMesh::FrameHeirarchyStackGetTop()
{
	return m_FrameHeirarchyStack.GetTop();
}

//--------------------------------------------------------------------------------------
INTERMEDIATE_VERTEX_BUFFER* CIntermediateMesh::GetVertexBuffer( UINT i )
{
    return m_VertexBufferArray.GetAt( i );
}

//--------------------------------------------------------------------------------------
INTERMEDIATE_INDEX_BUFFER* CIntermediateMesh::GetIndexBuffer( UINT i )
{
    return m_IndexBufferArray.GetAt( i );
}

//--------------------------------------------------------------------------------------
INTERMEDIATE_MESH* CIntermediateMesh::FindMesh( char* pszName )
{
    INTERMEDIATE_MESH* pData = NULL;
    for( UINT i=0; i<m_MeshArray.GetCount(); i++ )
    {
        pData = m_MeshArray.GetAt(i);
        if( _stricmp( pData->Name, pszName ) == 0 )
            return pData;
    }
    return NULL;
}

//--------------------------------------------------------------------------------------
UINT CIntermediateMesh::FindMeshIndex( char* pszName )
{
    INTERMEDIATE_MESH* pData = NULL;
    for( UINT i=0; i<m_MeshArray.GetCount(); i++ )
    {
        pData = m_MeshArray.GetAt(i);
        if( _stricmp( pData->Name, pszName ) == 0 )
            return i;
    }
    return INVALID_MESH;
}

//--------------------------------------------------------------------------------------
INTERMEDIATE_FRAME* CIntermediateMesh::FindFrame( char* pszName )
{
    INTERMEDIATE_FRAME* pData = NULL;
    for( UINT i=0; i<m_FrameArray.GetCount(); i++ )
    {
        pData = m_FrameArray.GetAt(i);
        if( _stricmp( pData->Name, pszName ) == 0 )
            return pData;
    }
    return NULL;
}

//--------------------------------------------------------------------------------------
UINT CIntermediateMesh::FindFrameIndex( char* pszName )
{
    INTERMEDIATE_FRAME* pData = NULL;
    for( UINT i=0; i<m_FrameArray.GetCount(); i++ )
    {
        pData = m_FrameArray.GetAt(i);
        if( _stricmp( pData->Name, pszName ) == 0 )
            return i;
    }
    return INVALID_FRAME;
}

//--------------------------------------------------------------------------------------
INTERMEDIATE_SUBSET* CIntermediateMesh::FindSubset( char* pszName )
{
    INTERMEDIATE_SUBSET* pData = NULL;
    for( UINT i=0; i<m_SubsetArray.GetCount(); i++ )
    {
        pData = m_SubsetArray.GetAt(i);
        if( _stricmp( pData->Name, pszName ) == 0 )
            return pData;
    }
    return NULL;
}

//--------------------------------------------------------------------------------------
void CIntermediateMesh::FindAllSubsets( char* pszName, CStack<INTERMEDIATE_SUBSET*>* pStack )
{
    INTERMEDIATE_SUBSET* pData = NULL;
    for( UINT i=0; i<m_SubsetArray.GetCount(); i++ )
    {
        pData = m_SubsetArray.GetAt(i);
        if( _stricmp( pData->Name, pszName ) == 0 )
			pStack->Push( pData );
    }
}

//--------------------------------------------------------------------------------------
INTERMEDIATE_MATERIAL* CIntermediateMesh::FindMaterial( char* pszName )
{
    INTERMEDIATE_MATERIAL* pData = NULL;
    for( UINT i=0; i<m_MaterialArray.GetCount(); i++ )
    {
        pData = m_MaterialArray.GetAt(i);
        if( _stricmp( pData->Name, pszName ) == 0 )
            return pData;
    }
    return NULL;
}

//--------------------------------------------------------------------------------------
UINT CIntermediateMesh::FindMaterialIndex( char* pszName )
{
    INTERMEDIATE_MATERIAL* pData = NULL;
    for( UINT i=0; i<m_MaterialArray.GetCount(); i++ )
    {
        pData = m_MaterialArray.GetAt(i);
        if( _stricmp( pData->Name, pszName ) == 0 )
            return i;
    }
    return INVALID_MATERIAL;
}

//--------------------------------------------------------------------------------------
INTERMEDIATE_ANIMATION_FRAME_DATA* CIntermediateMesh::FindAnimationFrame( char* pszName )
{
    INTERMEDIATE_ANIMATION_FRAME_DATA* pData = NULL;
    for( UINT i=0; i<m_AnimationFrameData.GetCount(); i++ )
    {
        pData = m_AnimationFrameData.GetAt(i);
        if( _stricmp( pData->FrameName, pszName ) == 0 )
            return pData;
    }
    return NULL;
}

//--------------------------------------------------------------------------------------
// Allocate all of the ticks for the bone animation data
//--------------------------------------------------------------------------------------
bool CIntermediateMesh::AllocFrameAnimationData( float Duration )
{
    m_AnimDuration = Duration;
    m_NumTicks = (UINT)(Duration*m_TPS);

    for( UINT i=0; i<m_AnimationFrameData.GetCount(); i++ )
    {
        INTERMEDIATE_ANIMATION_FRAME_DATA* pFrameData = m_AnimationFrameData.GetAt( i );		
        if(pFrameData)
        {
            AllocTransformFrameAnimationData( pFrameData );
        }
    }
    return true;
}


//--------------------------------------------------------------------------------------
// Late allocation of bone animation data for bones that only have the default transform
//--------------------------------------------------------------------------------------
bool CIntermediateMesh::AllocTransformFrameAnimationData( INTERMEDIATE_ANIMATION_FRAME_DATA* pFrameData )
{
    pFrameData->pPositions = new D3DXVECTOR3[ m_NumTicks ];
    pFrameData->pQuats = new D3DXVECTOR4[ m_NumTicks ];
    pFrameData->pScales = new D3DXVECTOR3[ m_NumTicks ];
    pFrameData->bPosKeySet = new bool[ m_NumTicks ];
    pFrameData->bOrientKeySet = new bool[ m_NumTicks ];
    pFrameData->bScaleKeySet = new bool[ m_NumTicks ];
    ZeroMemory(pFrameData->pPositions, (m_NumTicks*sizeof(D3DXVECTOR3)) );
    ZeroMemory(pFrameData->bPosKeySet, (m_NumTicks*sizeof(bool)) );
    ZeroMemory(pFrameData->bOrientKeySet, (m_NumTicks*sizeof(bool)) );
    ZeroMemory(pFrameData->bScaleKeySet, (m_NumTicks*sizeof(bool)) );

    for( UINT i=0; i<m_NumTicks; i++ )
    {
        pFrameData->pQuats[i].x = 0;
        pFrameData->pQuats[i].y = 0;
        pFrameData->pQuats[i].z = 0;
        pFrameData->pQuats[i].w = 1;

        pFrameData->pScales[i].x = 1;
        pFrameData->pScales[i].y = 1;
        pFrameData->pScales[i].z = 1;
    }
    return true;
}


//--------------------------------------------------------------------------------------
// Add a position key to the bone animation data
//--------------------------------------------------------------------------------------
bool CIntermediateMesh::AddPositionKey( INTERMEDIATE_ANIMATION_FRAME_DATA* pFrame, float Time, float x, float y, float z )
{
    UINT tick = (UINT)(Time*m_TPS );

    pFrame->bPosKeySet[tick] = true;
    pFrame->pPositions[tick] = D3DXVECTOR3(x,y,z);

    pFrame->bHasKey = true;
    return true;
}


//--------------------------------------------------------------------------------------
// Add an orientation key to the bone animation data
//--------------------------------------------------------------------------------------
bool CIntermediateMesh::AddOrientationKey( INTERMEDIATE_ANIMATION_FRAME_DATA* pFrame, float Time, float w, float x, float y, float z )
{
    UINT tick = (UINT)(Time*m_TPS );

    pFrame->bOrientKeySet[tick] = true;
    pFrame->pQuats[tick] = D3DXVECTOR4(x,y,z,w);

    pFrame->bHasKey = true;
    return true;
}

//--------------------------------------------------------------------------------------
// Add a scale key to the bone animation data
//--------------------------------------------------------------------------------------
bool CIntermediateMesh::AddScaleKey( INTERMEDIATE_ANIMATION_FRAME_DATA* pFrame, float Time, float x, float y, float z )
{
    UINT tick = (UINT)(Time*m_TPS );

    pFrame->bScaleKeySet[tick] = true;
    pFrame->pScales[tick] = D3DXVECTOR3(x,y,z);

    pFrame->bHasKey = true;
    return true;
}

//--------------------------------------------------------------------------------------
// Fix the mesh
//
// 1.  Slerp all bone tick information between key frames
// 2.  Transform the mesh verts into world space
//--------------------------------------------------------------------------------------
bool CIntermediateMesh::FixMesh()
{
    //make sure we have animation data
    if( m_NumTicks < 1 )
    {
        AllocFrameAnimationData( 0.5f );
    }

    //first fix all of the animation keys for bones
    for( UINT i=0; i<m_AnimationFrameData.GetCount(); i++ )
    {
        INTERMEDIATE_ANIMATION_FRAME_DATA* pFrame = m_AnimationFrameData.GetAt( i );
        FixFrameTicks( pFrame );
    }

	//find the bounding boxes for all meshes
	for( UINT i=0; i<m_MeshArray.GetCount(); i++ )
	{
		INTERMEDIATE_MESH* pMesh = m_MeshArray.GetAt(i);
		INTERMEDIATE_VERTEX_BUFFER* pVB = m_VertexBufferArray.GetAt( pMesh->VertexBuffers[0] );
		
		D3DXVECTOR3 Min,Max;
		if(SUCCEEDED(D3DXComputeBoundingBox( (D3DXVECTOR3*)pVB->pVertices, (DWORD)pVB->NumVertices, (DWORD)pVB->StrideBytes, &Min, &Max )))
		{
			pMesh->BoundingBoxCenter = (Min + Max)/2.0f;
			pMesh->BoundingBoxExtents = Max - pMesh->BoundingBoxCenter;
		}
	}
    
    return true;
}

//--------------------------------------------------------------------------------------
// Endian swap the vertex buffers for XML meshes
//--------------------------------------------------------------------------------------
bool CIntermediateMesh::EndianSwap()
{
    // Endian swap the VBs
    for( UINT s=0; s<m_VertexBufferArray.GetCount(); s++ )
    {
        INTERMEDIATE_VERTEX_BUFFER* pBuffer = m_VertexBufferArray.GetAt(s);

        //TODO:  This will break down is any of the vertex data is not 32bit
        DWORD* pdwords = (DWORD*)pBuffer->pVertices;
        for( UINT64 i=0; i<pBuffer->SizeBytes/4; i++ )
        {
            pdwords[i] = EndianSwap32( pdwords[i] );
        }
    }

    // Endian swap the IBs
    for( UINT s=0; s<m_IndexBufferArray.GetCount(); s++ )
    {
        INTERMEDIATE_INDEX_BUFFER* pBuffer = m_IndexBufferArray.GetAt(s);

        if( pBuffer->IndexType == IT_32BIT )
        {
            DWORD* pIndices = (DWORD*)pBuffer->pIndices;
            for( UINT64 i=0; i<pBuffer->NumIndices; i++ )
            {
                pIndices[i] = EndianSwap32( pIndices[i] );
            }
        }
        else if( pBuffer->IndexType == IT_16BIT )
        {
            WORD* pIndices = (WORD*)pBuffer->pIndices;
            for( UINT64 i=0; i<pBuffer->NumIndices; i++ )
            {
                pIndices[i] = EndianSwap16( pIndices[i] );
            }
        }
    }

    return true;
}

//--------------------------------------------------------------------------------------
void CIntermediateMesh::ConvertToLittleEndian()
{
    if( m_bBigEndian )
        EndianSwap();
    m_bBigEndian = false;
}

//--------------------------------------------------------------------------------------
void CIntermediateMesh::ConvertToBigEndian()
{
    if( !m_bBigEndian )
        EndianSwap();
    m_bBigEndian = true;
}

//--------------------------------------------------------------------------------------
bool IsSematicPresent( D3DDECLUSAGE usage, D3DVERTEXELEMENT9* pDecl, UINT NumDecls )
{
    for( UINT i=0; i<NumDecls; i++ )
    {
        if( pDecl[i].Stream == 0xff )
            return false;

        if( pDecl[i].Usage == usage )
            return true;

    }
    return false;
}

//--------------------------------------------------------------------------------------
bool CIntermediateMesh::GenerateTangents( IDirect3DDevice9* pd3dDevice )
{
    HRESULT hr = S_OK;
    bool bRetVal = false;
    ID3DXMesh* pD3DXMesh = NULL;
    INTERMEDIATE_MESH* pMesh = NULL;
    DWORD* pdwAdjacency = NULL;

    for( UINT i=0; i<m_MeshArray.GetCount(); i++ )
    {
        pMesh = m_MeshArray.GetAt(i);

        //only gen tangents on the first buffer
        INTERMEDIATE_VERTEX_BUFFER* pVB = m_VertexBufferArray.GetAt( pMesh->VertexBuffers[0] );
        INTERMEDIATE_INDEX_BUFFER* pIB = m_IndexBufferArray.GetAt( pMesh->IndexBuffer );
        
        DWORD dwOptions = D3DXMESH_SYSTEMMEM;
        if( pIB->IndexType == IT_32BIT )
            dwOptions |= D3DXMESH_32BIT;

        // create a mesh
        hr = D3DXCreateMesh( (DWORD)pIB->NumIndices/3,
                             (DWORD)pVB->NumVertices,
                             dwOptions,
                             pVB->Decl,
                             pd3dDevice,
                             &pD3DXMesh );
        if(FAILED(hr))
            goto Error;

        // Fill it up with vertices
        void* pLockedData = NULL;
        hr = pD3DXMesh->LockVertexBuffer( 0, &pLockedData );
        if(FAILED(hr))
            goto Error;
        CopyMemory( pLockedData, pVB->pVertices, (UINT)pVB->SizeBytes );
        pD3DXMesh->UnlockVertexBuffer();

        // Fill it up with indices
        hr = pD3DXMesh->LockIndexBuffer( 0, &pLockedData );
        if(FAILED(hr))
            goto Error;
        CopyMemory( pLockedData, pIB->pIndices, (UINT)pIB->SizeBytes );
        pD3DXMesh->UnlockIndexBuffer();

        // gen adjacency
        pdwAdjacency = new DWORD[ 3*pD3DXMesh->GetNumFaces() ];
        if( !pdwAdjacency )
            goto Error;
        hr = pD3DXMesh->GenerateAdjacency( 0.001f, pdwAdjacency );
        if(FAILED(hr))
            goto Error;

        D3DDECLUSAGE texcoord = (D3DDECLUSAGE)D3DX_DEFAULT;
        D3DDECLUSAGE binormal = (D3DDECLUSAGE)D3DX_DEFAULT;
        D3DDECLUSAGE tangent = (D3DDECLUSAGE)D3DX_DEFAULT;
        D3DDECLUSAGE normal = (D3DDECLUSAGE)D3DX_DEFAULT;

        if( IsSematicPresent( D3DDECLUSAGE_TEXCOORD, pVB->Decl, MAX_VERTEX_ELEMENTS ) )
            texcoord = D3DDECLUSAGE_TEXCOORD;
        if( IsSematicPresent( D3DDECLUSAGE_BINORMAL, pVB->Decl, MAX_VERTEX_ELEMENTS ) )
            binormal = D3DDECLUSAGE_BINORMAL;
        if( IsSematicPresent( D3DDECLUSAGE_TANGENT, pVB->Decl, MAX_VERTEX_ELEMENTS ) )
            tangent = D3DDECLUSAGE_TANGENT;
        if( IsSematicPresent( D3DDECLUSAGE_NORMAL, pVB->Decl, MAX_VERTEX_ELEMENTS ) )
            normal = D3DDECLUSAGE_NORMAL;

        // gen tangents
        DWORD dwFlags = D3DXTANGENT_GENERATE_IN_PLACE;
        hr = D3DXComputeTangentFrameEx(pD3DXMesh, 
                                        texcoord, 
                                        0,   
                                        binormal, 
                                        0, 
                                        tangent, 0, 
                                        normal, 0, 
                                        dwFlags,
                                        pdwAdjacency, -1.01f,-0.01f,-1.01f, NULL, NULL);
        if(FAILED(hr))
            goto Error;

        // Copy the new data back over
        hr = pD3DXMesh->LockVertexBuffer( 0, &pLockedData );
        if(FAILED(hr))
            goto Error;

        CopyMemory( pVB->pVertices, pLockedData, (size_t)pVB->SizeBytes );
        pD3DXMesh->UnlockVertexBuffer();
    }

    bRetVal = true;
Error:
    SAFE_DELETE_ARRAY( pdwAdjacency );
    SAFE_RELEASE( pD3DXMesh );
    return bRetVal;
}

//--------------------------------------------------------------------------------------
// ConvertVertexData
//--------------------------------------------------------------------------------------
bool CIntermediateMesh::ConvertVertexData( IDirect3DDevice9* pd3dDevice, D3DVERTEXELEMENT9* pDecl, UINT NumElements )
{
    INTERMEDIATE_VERTEX_BUFFER* pVB = m_VertexBufferArray.GetAt(0);
    void* pOutVerts = NULL;
    UINT64 OutStride = 0;
    if( !ConvertVertexData( pd3dDevice, pVB->pVertices, pVB->NumVertices, pVB->Decl, MAX_VERTEX_ELEMENTS,
                       &pOutVerts, &OutStride, pDecl, NumElements ) )
        return false;

    SAFE_DELETE_ARRAY( pVB->pVertices );
    pVB->pVertices = pOutVerts;
    pVB->StrideBytes = OutStride;
    pVB->SizeBytes = pVB->NumVertices*OutStride;
    for( UINT i=0; i<NumElements; i++ )
        pVB->Decl[i] = pDecl[i];
    return true;
}


//--------------------------------------------------------------------------------------
// Save the intermediate data out as an x-file
//--------------------------------------------------------------------------------------
bool CIntermediateMesh::SaveAsXFile( IDirect3DDevice9* pd3dDevice, WCHAR* szFileName, bool bBinary )
{
    bool bRetVal = false;
    HRESULT hr = S_OK;
    D3DXATTRIBUTERANGE* pAttributes = NULL;
    D3DXMATERIAL* pMaterials = NULL;
    ID3DXMesh* pD3DXMesh = NULL;

    INTERMEDIATE_MESH* pMesh = NULL;
    INTERMEDIATE_VERTEX_BUFFER* pVB = NULL;
    INTERMEDIATE_INDEX_BUFFER* pIB = NULL;

    // Only do the first mesh
    pMesh = m_MeshArray.GetAt(0);
    if( !pMesh )
        goto Error;

    // Only do the first VB
    pVB = m_VertexBufferArray.GetAt( pMesh->VertexBuffers[0] );
    if( !pVB )
        goto Error;

    pIB = m_IndexBufferArray.GetAt( pMesh->IndexBuffer );
    if( !pIB )
        goto Error;

    DWORD dwOptions = D3DXMESH_SYSTEMMEM;
    if( pIB->IndexType == IT_32BIT )
        dwOptions |= D3DXMESH_32BIT;

    // create a mesh
    hr = D3DXCreateMesh( (DWORD)pIB->NumIndices/3,
                         (DWORD)pVB->NumVertices,
                         dwOptions,
                         pVB->Decl,
                         pd3dDevice,
                         &pD3DXMesh );
    if(FAILED(hr))
        goto Error;

    // Fill verts
    void* pData;
    hr = pD3DXMesh->LockVertexBuffer( 0, &pData );
    if(FAILED(hr))
        goto Error;
    CopyMemory( pData, pVB->pVertices, (size_t)pVB->SizeBytes );
    pD3DXMesh->UnlockVertexBuffer();

    // Fill indices
    hr = pD3DXMesh->LockIndexBuffer( 0, &pData );
    if(FAILED(hr))
        goto Error;
    CopyMemory( pData, pIB->pIndices, (size_t)pIB->SizeBytes );
    pD3DXMesh->UnlockIndexBuffer();

    // LockAttributeBuffer
    DWORD* pdwAttributes;
    hr = pD3DXMesh->LockAttributeBuffer( 0, &pdwAttributes );
    if(FAILED(hr))
        goto Error;

    // Fill subsets
    pAttributes = new D3DXATTRIBUTERANGE[ pMesh->NumSubsets ];
    if( !pAttributes )
        goto Error;
    for( UINT i=0; i<pMesh->NumSubsets; i++ )
    {
        INTERMEDIATE_SUBSET* pSubset = m_SubsetArray.GetAt( pMesh->SubsetIDArray.GetAt(i) );

        for( UINT64 a=pSubset->IndexStart/3; a<(pSubset->IndexStart/3 + pSubset->IndexCount/3); a++ )
        {
            pdwAttributes[a] = pSubset->MaterialID;
        }

        pAttributes[i].AttribId = pSubset->MaterialID;
        pAttributes[i].FaceStart = (DWORD)pSubset->IndexStart/3;
        pAttributes[i].FaceCount = (DWORD)pSubset->IndexCount/3;
        pAttributes[i].VertexStart = (DWORD)pSubset->VertexStart;
        pAttributes[i].VertexCount = (DWORD)pSubset->VertexCount;
    }

    pD3DXMesh->UnlockAttributeBuffer();

    hr = pD3DXMesh->SetAttributeTable( pAttributes, pMesh->NumSubsets );
    if(FAILED(hr))
        goto Error;

    // Fill Materials
    if( m_MaterialArray.GetCount() > 0 )
    {
        pMaterials = new D3DXMATERIAL[ (size_t)m_MaterialArray.GetCount() ];
        if( !pMaterials )
            goto Error;
        ZeroMemory( pMaterials, (size_t)(m_MaterialArray.GetCount()*sizeof(D3DXMATERIAL)) );
        for( UINT i=0; i<m_MaterialArray.GetCount(); i++ )
        {
            INTERMEDIATE_MATERIAL* pInputMat = m_MaterialArray.GetAt( i );
            if( pInputMat )
            {
                pMaterials[i].pTextureFilename = pInputMat->DiffuseTexture;
                COLOR_FROM_VEC4( pMaterials[i].MatD3D.Diffuse, pInputMat->Diffuse );
                COLOR_FROM_VEC4( pMaterials[i].MatD3D.Ambient, pInputMat->Ambient );
                COLOR_FROM_VEC4( pMaterials[i].MatD3D.Specular, pInputMat->Specular );
                COLOR_FROM_VEC4( pMaterials[i].MatD3D.Emissive, pInputMat->Emissive );
                pMaterials[i].MatD3D.Power = pInputMat->Power;
            }
        }
    }

    // Save it all out
    DWORD dwFormat = D3DXF_FILEFORMAT_TEXT;
    if( bBinary )
        dwFormat = D3DXF_FILEFORMAT_BINARY;

    hr = D3DXSaveMeshToX( szFileName, 
                     pD3DXMesh,
                     NULL,
                     pMaterials,
                     NULL,
                     (DWORD)m_MaterialArray.GetCount(),
                     dwFormat );
    if(FAILED(hr))
        goto Error;

    bRetVal = true;
Error:
    SAFE_DELETE_ARRAY( pAttributes );
    SAFE_DELETE_ARRAY( pMaterials );
    SAFE_RELEASE( pD3DXMesh );
    return bRetVal;
}

//--------------------------------------------------------------------------------------
// Helper to align offsets to 4k
//--------------------------------------------------------------------------------------
UINT64 Align4k( UINT64 Offset )
{
    UINT64 floor = Offset / 4096;
    return (floor+1)*4096;

}

//--------------------------------------------------------------------------------------
// Output a binary sdkmesh file
//--------------------------------------------------------------------------------------
bool CIntermediateMesh::SaveAsSDKMesh( WCHAR* szFileName )
{
    FILE* fp = NULL;
    size_t retval = 0;
    bool bRetVal = false;

    fp = _wfopen( szFileName, L"wb" );
    if(!fp)
        goto Error;


    UINT64 HeaderSize = 0;
    HeaderSize += sizeof( SDKMESH_HEADER );
    HeaderSize += m_VertexBufferArray.GetCount()*sizeof(SDKMESH_VERTEX_BUFFER_HEADER);
    HeaderSize += m_IndexBufferArray.GetCount()*sizeof(SDKMESH_INDEX_BUFFER_HEADER);

    // Calculate the size for everything else
    UINT64 StaticDataSize = 0;
    StaticDataSize += m_MeshArray.GetCount()*sizeof(SDKMESH_MESH);
    StaticDataSize += m_SubsetArray.GetCount()*sizeof(SDKMESH_SUBSET);
    StaticDataSize += m_FrameArray.GetCount()*sizeof(SDKMESH_FRAME);
    StaticDataSize += m_MaterialArray.GetCount()*sizeof(SDKMESH_MATERIAL);
    UINT64 NonBufferDataSize = StaticDataSize;
    for( UINT i=0; i<m_MeshArray.GetCount(); i++ )
    {
        INTERMEDIATE_MESH* pMesh = m_MeshArray.GetAt(i);
        NonBufferDataSize += pMesh->NumSubsets * sizeof(UINT);
        NonBufferDataSize += pMesh->NumFrameInfluences * sizeof(UINT);
    }

    // Calculate all vertex and index data size
    UINT64 BufferDataSize = 0;
    for( UINT i=0; i<m_VertexBufferArray.GetCount(); i++ )
    {
        UINT64 iSize = m_VertexBufferArray.GetAt(i)->SizeBytes;
        BufferDataSize += Align4k( iSize );
    }
    for( UINT i=0; i<m_IndexBufferArray.GetCount(); i++ )
    {
        UINT64 iSize = m_IndexBufferArray.GetAt(i)->SizeBytes;
        BufferDataSize += Align4k( iSize );
    }

    SDKMESH_HEADER fileheader;
    fileheader.Version = SDKMESH_FILE_VERSION;
    fileheader.IsBigEndian = m_bBigEndian;
    fileheader.HeaderSize = HeaderSize;
    fileheader.NonBufferDataSize = NonBufferDataSize; 
    fileheader.BufferDataSize = BufferDataSize;
    fileheader.NumVertexBuffers = m_VertexBufferArray.GetCount();
    fileheader.NumIndexBuffers = m_IndexBufferArray.GetCount();
    fileheader.NumMeshes = m_MeshArray.GetCount();
    fileheader.NumTotalSubsets = m_SubsetArray.GetCount();
    fileheader.NumFrames = m_FrameArray.GetCount();
    fileheader.NumMaterials = m_MaterialArray.GetCount();
    fileheader.VertexStreamHeadersOffset = sizeof( SDKMESH_HEADER );
    fileheader.IndexStreamHeadersOffset = fileheader.VertexStreamHeadersOffset + m_VertexBufferArray.GetCount() * sizeof(SDKMESH_VERTEX_BUFFER_HEADER);
    fileheader.MeshDataOffset = fileheader.IndexStreamHeadersOffset + m_IndexBufferArray.GetCount() * sizeof(SDKMESH_INDEX_BUFFER_HEADER);
    fileheader.SubsetDataOffset = fileheader.MeshDataOffset + m_MeshArray.GetCount() * sizeof(SDKMESH_MESH);
    fileheader.FrameDataOffset = fileheader.SubsetDataOffset + m_SubsetArray.GetCount() * sizeof(SDKMESH_SUBSET);
    fileheader.MaterialDataOffset = fileheader.FrameDataOffset + m_FrameArray.GetCount() * sizeof(SDKMESH_FRAME);

    /////////////////////////////
    //Write out the header
    retval = fwrite( &fileheader, sizeof(SDKMESH_HEADER), 1, fp );
    if( retval != 1 )
        goto Error;

    /////////////////////////////
    //VB headers
    UINT64 BufferDataOffset = HeaderSize + NonBufferDataSize;
    for( UINT i=0; i<m_VertexBufferArray.GetCount(); i++ )
    {
        SDKMESH_VERTEX_BUFFER_HEADER* pVBH = m_VertexBufferArray.GetAt(i);
        pVBH->DataOffset = BufferDataOffset;
        retval = fwrite( pVBH, sizeof(SDKMESH_VERTEX_BUFFER_HEADER), 1, fp );
        if( retval != 1 )
            goto Error;

        BufferDataOffset += Align4k( pVBH->SizeBytes );
    }

    /////////////////////////////
    //IB headers
    for( UINT i=0; i<m_IndexBufferArray.GetCount(); i++ )
    {
        SDKMESH_INDEX_BUFFER_HEADER* pIBH = m_IndexBufferArray.GetAt(i);
        pIBH->DataOffset = BufferDataOffset;
        retval = fwrite( pIBH, sizeof(SDKMESH_INDEX_BUFFER_HEADER), 1, fp );
        if( retval != 1 )
            goto Error;

        BufferDataOffset += Align4k( pIBH->SizeBytes );
    }

    /////////////////////////////
    //Meshes
    UINT64 SubsetListDataOffset = HeaderSize + StaticDataSize;
    for( UINT i=0; i<m_MeshArray.GetCount(); i++ )
    {
        SDKMESH_MESH* pMesh = m_MeshArray.GetAt(i);
        pMesh->SubsetOffset = SubsetListDataOffset;
        SubsetListDataOffset += pMesh->NumSubsets*sizeof(UINT);
        pMesh->FrameInfluenceOffset = SubsetListDataOffset;
        SubsetListDataOffset += pMesh->NumFrameInfluences*sizeof(UINT);

        retval = fwrite( pMesh, sizeof(SDKMESH_MESH), 1, fp );
        if( retval != 1 )
            goto Error;
    }

    /////////////////////////////
    //Subsets
    for( UINT i=0; i<m_SubsetArray.GetCount(); i++ )
    {
        SDKMESH_SUBSET* pSubset = m_SubsetArray.GetAt(i);
        retval = fwrite( pSubset, sizeof(SDKMESH_SUBSET), 1, fp );
        if( retval != 1 )
            goto Error;
    }

    /////////////////////////////
    //Frames
    for( UINT i=0; i<m_FrameArray.GetCount(); i++ )
    {
        SDKMESH_FRAME* pFrame = m_FrameArray.GetAt(i);
        retval = fwrite( pFrame, sizeof(SDKMESH_FRAME), 1, fp );
        if( retval != 1 )
            goto Error;
    }

    /////////////////////////////
    //Materials
    for( UINT i=0; i<m_MaterialArray.GetCount(); i++ )
    {
        SDKMESH_MATERIAL* pMaterial = m_MaterialArray.GetAt(i);
        retval = fwrite( pMaterial, sizeof(SDKMESH_MATERIAL), 1, fp );
        if( retval != 1 )
            goto Error;
    }

    /////////////////////////////
    //Subset and FrameInfluence lists
    for( UINT i=0; i<m_MeshArray.GetCount(); i++ )
    {
        INTERMEDIATE_MESH* pMesh = m_MeshArray.GetAt(i);

        for( UINT s=0; s<pMesh->SubsetIDArray.GetCount(); s++ )
        {
            UINT subsetID = pMesh->SubsetIDArray.GetAt(s);
            retval = fwrite( &subsetID, sizeof(UINT), 1, fp );
            if( retval != 1 )
                goto Error;
        }

        for( UINT f=0; f<pMesh->FrameInfluenceArray.GetCount(); f++ )
        {
            FRAME_NAME FrameName = pMesh->FrameInfluenceArray.GetAt(f);
            UINT frameID = FindFrameIndex( FrameName.Name );
            retval = fwrite( &frameID, sizeof(UINT), 1, fp );
            if( retval != 1 )
                goto Error;
        }
    }

    /////////////////////////////
    //VertexData
    for( UINT i=0; i<m_VertexBufferArray.GetCount(); i++ )
    {
        INTERMEDIATE_VERTEX_BUFFER* pVBH = m_VertexBufferArray.GetAt(i);
        retval = fwrite( pVBH->pVertices, (size_t)pVBH->SizeBytes, 1, fp );
        if( retval != 1 )
            goto Error;

        //pad
        UINT64 alignsize = Align4k( pVBH->SizeBytes );
        BYTE nothing = 0;
        for( UINT64 b=0; b<alignsize - pVBH->SizeBytes; b++ )
        {
            retval = fwrite( &nothing, sizeof(BYTE), 1, fp );
            if( retval != 1 )
                goto Error;
        }
    }

    /////////////////////////////
    //IndexData
    for( UINT i=0; i<m_IndexBufferArray.GetCount(); i++ )
    {
        INTERMEDIATE_INDEX_BUFFER* pIBH = m_IndexBufferArray.GetAt(i);

        retval = fwrite( pIBH->pIndices, (size_t)pIBH->SizeBytes, 1, fp );
        if( retval != 1 )
            goto Error;

        //pad
        UINT64 alignsize = Align4k( pIBH->SizeBytes );
        BYTE nothing = 0;
        for( UINT64 b=0; b<alignsize - pIBH->SizeBytes; b++ )
        {
            retval = fwrite( &nothing, sizeof(BYTE), 1, fp );
            if( retval != 1 )
                goto Error;
        }
    }

    bRetVal = true;
Error:
    fclose(fp);
    return bRetVal;
}

//--------------------------------------------------------------------------------------
// Save out animation data to a separate file
//--------------------------------------------------------------------------------------
bool CIntermediateMesh::SaveAnimationData( WCHAR* szFileName )
{
    FILE* fp = NULL;
    size_t retval = 0;
    bool bRetVal = false;

    fp = _wfopen( szFileName, L"wb" );
    if(!fp)
        goto Error;

    UINT64 dataSize = m_AnimationFrameData.GetCount()*( sizeof(SDKANIMATION_FRAME_DATA) + m_NumTicks*sizeof(SDKANIMATION_DATA) );
    SDKANIMATION_FILE_HEADER fileheader;
    fileheader.Version = SDKMESH_FILE_VERSION;
    fileheader.IsBigEndian = 0;
    fileheader.FrameTransformType = FTT_RELATIVE;
    fileheader.NumFrames = m_AnimationFrameData.GetCount();
    fileheader.NumAnimationKeys = m_NumTicks;
    fileheader.AnimationFPS = m_TPS;
    fileheader.AnimationDataSize = dataSize;
    fileheader.AnimationDataOffset = sizeof(SDKANIMATION_FILE_HEADER);

    /////////////////////////////
    //Write out the header
    retval = fwrite( &fileheader, sizeof(SDKANIMATION_FILE_HEADER), 1, fp );

    /////////////////////////////
    //Write out the frame data
    for( UINT i=0; i<m_AnimationFrameData.GetCount(); i++ )
    {
        SDKANIMATION_FRAME_DATA* pFrameData = m_AnimationFrameData.GetAt(i);
        pFrameData->DataOffset = m_AnimationFrameData.GetCount()*sizeof(SDKANIMATION_FRAME_DATA) + i*m_NumTicks*sizeof(SDKANIMATION_DATA);

        retval = fwrite( pFrameData, sizeof(SDKANIMATION_FRAME_DATA), 1, fp );
    }

    /////////////////////////////
    //Write out the frame data
    for( UINT i=0; i<m_AnimationFrameData.GetCount(); i++ )
    {
        INTERMEDIATE_ANIMATION_FRAME_DATA* pFrameData = m_AnimationFrameData.GetAt(i);
        retval = fwrite( pFrameData->pIntermediateData, m_NumTicks*sizeof(SDKANIMATION_DATA), 1, fp );
    }

    bRetVal = true;
Error:
    fclose(fp);
    return bRetVal;
}

//--------------------------------------------------------------------------------------
// Helper to find the next used key in a list of ticks
//--------------------------------------------------------------------------------------
UINT FindNextUsedKey( bool* boolArray, UINT iStart, UINT iMax )
{
    for( UINT i = iStart; i<iMax; i++ )
    {
        if( boolArray[i] )
            return i;
    }

    if( iStart == 0 )
        return 0;

    return iMax;
}


//--------------------------------------------------------------------------------------
// FixFrameTicks
//
// 1.  Ensure the first tick is the bind pose
// 2.  Lerp positions ticks between key frames
// 3.  Slerp orientation ticks between key frames
//--------------------------------------------------------------------------------------
bool CIntermediateMesh::FixFrameTicks( INTERMEDIATE_ANIMATION_FRAME_DATA* pFrame )
{
    D3DXVECTOR3 v3Start,v3End,v3Delta;

    //Make sure the first key has at least the bind pose
    if( !pFrame->bHasKey )
    {
        INTERMEDIATE_FRAME* pFBind = FindFrame( pFrame->FrameName );
        if( pFBind )
        {
            pFrame->pPositions[0].x = pFBind->Matrix._41;
            pFrame->pPositions[0].y = pFBind->Matrix._42;
            pFrame->pPositions[0].z = pFBind->Matrix._43;
            D3DXQUATERNION quat;
            D3DXQuaternionRotationMatrix( &quat, &pFBind->Matrix );
            pFrame->pQuats[0].x = quat.x;
            pFrame->pQuats[0].y = quat.y;
            pFrame->pQuats[0].z = quat.z;
            pFrame->pQuats[0].w = quat.w;
            pFrame->pScales[0].x = 1;
            pFrame->pScales[0].y = 1;
            pFrame->pScales[0].z = 1;
            pFrame->bPosKeySet[0] = true;
            pFrame->bOrientKeySet[0] = true;
            pFrame->bScaleKeySet[0] = true;
            pFrame->bHasKey = true;
        }
    }

    //position
    for( UINT i=0; i<m_NumTicks; i++ )
    {
        //find the beginning
        UINT iStart = FindNextUsedKey( pFrame->bPosKeySet, i, m_NumTicks );
        UINT iEnd = FindNextUsedKey( pFrame->bPosKeySet, iStart+1, m_NumTicks );

        float fSpan = (float)(iEnd - iStart);
        v3Start = pFrame->pPositions[iStart];
        if( iEnd == m_NumTicks )
            v3End = v3Start;
        else
            v3End = pFrame->pPositions[iEnd];
        v3Delta = (v3End - v3Start) / fSpan;

        v3Start += v3Delta;
        for( UINT s=iStart+1; s<iEnd; s++ )
        {
            pFrame->pPositions[s] = v3Start;
            v3Start += v3Delta;
            pFrame->bPosKeySet[s] = true;
        }
    }

    D3DXVECTOR4 v4Start,v4End,v4Delta;
    D3DXQUATERNION quatStart;
    D3DXQUATERNION quatEnd;
    D3DXQUATERNION quatLerp;
    //orientation
    for( UINT i=0; i<m_NumTicks; i++ )
    {
        //find the beginning
        UINT iStart = FindNextUsedKey( pFrame->bOrientKeySet, i, m_NumTicks );
        UINT iEnd = FindNextUsedKey( pFrame->bOrientKeySet, iStart+1, m_NumTicks );

        UINT iSpan = (iEnd - iStart);
        v4Start = pFrame->pQuats[iStart];
        if( iEnd == m_NumTicks )
            v4End = v4Start;
        else
            v4End = pFrame->pQuats[iEnd];

        quatStart.x = v4Start.x;
        quatStart.y = v4Start.y;
        quatStart.z = v4Start.z;
        quatStart.w = v4Start.w;
        quatEnd.x = v4End.x;
        quatEnd.y = v4End.y;
        quatEnd.z = v4End.z;
        quatEnd.w = v4End.w;

        float fDelta = 1.0f/(float)iSpan;
        float fT = fDelta;
        for( UINT s=iStart+1; s<iEnd; s++ )
        {
            D3DXQuaternionSlerp( &quatLerp, &quatStart, &quatEnd, fT );
            D3DXQuaternionNormalize( &quatLerp, &quatLerp );
            pFrame->pQuats[s].x = quatLerp.x;
            pFrame->pQuats[s].y = quatLerp.y;
            pFrame->pQuats[s].z = quatLerp.z;
            pFrame->pQuats[s].w = quatLerp.w;
            
            fT += fDelta;

            pFrame->bOrientKeySet[s] = true;
        }
    }

    //scale
    for( UINT i=0; i<m_NumTicks; i++ )
    {
        //find the beginning
        UINT iStart = FindNextUsedKey( pFrame->bScaleKeySet, i, m_NumTicks );
        UINT iEnd = FindNextUsedKey( pFrame->bScaleKeySet, iStart+1, m_NumTicks );

        float fSpan = (float)(iEnd - iStart);
        v3Start = pFrame->pScales[iStart];
        if( iEnd == m_NumTicks )
            v3End = v3Start;
        else
            v3End = pFrame->pScales[iEnd];
        v3Delta = (v3End - v3Start) / fSpan;

        v3Start += v3Delta;
        for( UINT s=iStart+1; s<iEnd; s++ )
        {
            pFrame->pScales[s] = v3Start;
            v3Start += v3Delta;
            pFrame->bScaleKeySet[s] = true;
        }
    }

    return SolidifyFrameAnimationData(pFrame);	
}

bool CIntermediateMesh::SolidifyFrameAnimationData(INTERMEDIATE_ANIMATION_FRAME_DATA* pFrame)
{
    pFrame->pIntermediateData = new SDKANIMATION_DATA[ m_NumTicks ];
    if( !pFrame->pIntermediateData )
        return false;

    for( UINT i=0; i<m_NumTicks; i++ )
    {
        pFrame->pIntermediateData[i].Translation = pFrame->pPositions[i];
        pFrame->pIntermediateData[i].Orientation = pFrame->pQuats[i];
        pFrame->pIntermediateData[i].Scaling = pFrame->pScales[i];
    }
    return true;
}

//--------------------------------------------------------------------------------------
// Move all bone transformations into absolute space (aka, remove heirarchy necessity)
//--------------------------------------------------------------------------------------
/*bool CIntermediateMesh::CalculateBoneFrameData( INTERMEDIATE_ANIMATION_FRAME_DATA* pBone, BONE_FRAME* pFrame, UINT64 iTick )
{
    D3DXMATRIX mTotal;
    D3DXMatrixIdentity( &mTotal );

    XML_BONE* pParent = pBone->pBoneParent;
    while( pParent )
    {
        D3DXQUATERNION quat;
        D3DXVECTOR3 parentPos = pParent->pPositions[iTick];
        D3DXMATRIX mTranslate;
        D3DXMatrixTranslation( &mTranslate, parentPos.x, parentPos.y, parentPos.z );
        //D3DXMatrixIdentity( &mTranslate );
        
        D3DXMATRIX mQuat;
        quat.w = pParent->pQuats[iTick].w;
        quat.x = pParent->pQuats[iTick].x;
        quat.y = pParent->pQuats[iTick].y;
        quat.z = pParent->pQuats[iTick].z;
        if( quat.w == 0 && quat.x == 0 && quat.y == 0 && quat.z == 0 )
            D3DXQuaternionIdentity( &quat );
        D3DXQuaternionNormalize( &quat, &quat );
        D3DXMatrixRotationQuaternion( &mQuat, &quat );

        mTotal = mTotal * ( mQuat * mTranslate );
        pParent = pParent->pBoneParent;
    }

    //translation
    D3DXVECTOR4 pos;
    D3DXVec3Transform( &pos, &pBone->pPositions[iTick], &mTotal );

    if( 1 )
    {

        //rotation
        D3DXQUATERNION quatLocal;
        quatLocal.w = pBone->pQuats[iTick].w;
        quatLocal.x = pBone->pQuats[iTick].x;
        quatLocal.y = pBone->pQuats[iTick].y;
        quatLocal.z = pBone->pQuats[iTick].z;
        if( quatLocal.w == 0 && quatLocal.x == 0 && quatLocal.y == 0 && quatLocal.z == 0 )
            D3DXQuaternionIdentity( &quatLocal );

        //parent quaternion
        D3DXQUATERNION quatParent;
        D3DXMATRIX mTotalRot = mTotal;
        D3DXQuaternionRotationMatrix( &quatParent, &mTotalRot );
        D3DXQUATERNION quatRot = quatLocal*quatParent;
        D3DXQuaternionNormalize( &quatRot, &quatRot );

        //output
        pFrame->Data[3] = quatRot.x;
        pFrame->Data[4] = quatRot.y;
        pFrame->Data[5] = quatRot.z;
        pFrame->Data[6] = quatRot.w;
    }
    else
    {
        pFrame->Data[3] = 0;
        pFrame->Data[4] = 0;
        pFrame->Data[5] = 0;
        pFrame->Data[6] = 1;
    }

    pFrame->Data[0] = pos.x;
    pFrame->Data[1] = pos.y;
    pFrame->Data[2] = pos.z;

    return true;
}*/

//--------------------------------------------------------------------------------------
// ConvertVertexData
//--------------------------------------------------------------------------------------
bool CIntermediateMesh::ConvertVertexData( IDirect3DDevice9* pd3dDevice, 
                                           void* pInVertices, UINT64 NumInVertices, D3DVERTEXELEMENT9* pInDecl, UINT NumInElements, 
                                           void** ppOutVertices, UINT64* pOutputStride, D3DVERTEXELEMENT9* pOutDecl, UINT NumOutElements )
{
    HRESULT hr = S_OK;
    bool bRetVal = false;
    ID3DXMesh* pInMesh = NULL;
    ID3DXMesh* pOutMesh = NULL;

    // create a mesh
    hr = D3DXCreateMesh( (DWORD)NumInVertices/3,
                         (DWORD)NumInVertices,
                         D3DXMESH_SYSTEMMEM | D3DXMESH_32BIT,
                         pInDecl,
                         pd3dDevice,
                         &pInMesh );
    if(FAILED(hr))
        goto Error;

    UINT VBInSize = (UINT)(pInMesh->GetNumBytesPerVertex() * NumInVertices);

    void* pLockedVerts = NULL;
    hr = pInMesh->LockVertexBuffer( 0, &pLockedVerts );
    if(FAILED(hr))
        goto Error;

    CopyMemory( pLockedVerts, pInVertices, VBInSize );
    pInMesh->UnlockVertexBuffer();

    //clone it to the new specifications
    hr = pInMesh->CloneMesh( D3DXMESH_SYSTEMMEM | D3DXMESH_32BIT,
                        pOutDecl,
                        pd3dDevice,
                        &pOutMesh );
    if(FAILED(hr))
        goto Error;

    // allocate room for the new vertices
    *pOutputStride = pOutMesh->GetNumBytesPerVertex();
    UINT VBOutSize = (UINT)(pOutMesh->GetNumBytesPerVertex() * NumInVertices);
    (*ppOutVertices) = new BYTE[ VBOutSize ];
    if( !(*ppOutVertices) )
        goto Error;

    hr = pOutMesh->LockVertexBuffer( 0, &pLockedVerts );
    if(FAILED(hr))
        goto Error;
    CopyMemory( (*ppOutVertices), pLockedVerts, VBOutSize );
    pOutMesh->UnlockVertexBuffer();

    bRetVal = true;
Error:
    SAFE_RELEASE( pInMesh );
    SAFE_RELEASE( pOutMesh );
    return bRetVal;
}
