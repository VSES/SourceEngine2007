//--------------------------------------------------------------------------------------
// File: IntermediateMesh.h
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#pragma once
#ifndef _INTERMEDIATEMESH_
#define _INTERMEDIATEMESH_

#include "SDKMesh.h"

struct FRAME_NAME
{
    char Name[MAX_FRAME_NAME];
};

struct INTERMEDIATE_VERTEX_BUFFER : public SDKMESH_VERTEX_BUFFER_HEADER
{
    void* pVertices;
};

struct INTERMEDIATE_INDEX_BUFFER : public SDKMESH_INDEX_BUFFER_HEADER
{
    void* pIndices;
};

struct INTERMEDIATE_MESH : public SDKMESH_MESH
{
    CStack<UINT> SubsetIDArray;
    CStack<FRAME_NAME> FrameInfluenceArray;
};

struct INTERMEDIATE_SUBSET : public SDKMESH_SUBSET
{
};

struct INTERMEDIATE_FRAME : public SDKMESH_FRAME
{
    UINT id;
};

struct INTERMEDIATE_MATERIAL : public SDKMESH_MATERIAL
{
};

struct INTERMEDIATE_ANIMATION_FRAME_DATA : public SDKANIMATION_FRAME_DATA
{
    SDKANIMATION_DATA* pIntermediateData;
    D3DXVECTOR3* pPositions;
    D3DXVECTOR4* pQuats;
    D3DXVECTOR3* pScales;
    bool* bPosKeySet;
    bool* bOrientKeySet;
    bool* bScaleKeySet;
    bool bHasKey;
};

//--------------------------------------------------------------------------------------
// CIntermediateMesh class
//--------------------------------------------------------------------------------------
class CIntermediateMesh
{
public:
    CIntermediateMesh( UINT iTPS, WCHAR* szName ) : m_bBigEndian(false), m_NumTicks(0), m_AnimDuration(0)
    { 
        m_TPS = iTPS; 
        StringCchCopy( m_szName, MAX_PATH, szName );
    }
    ~CIntermediateMesh() {}

    // adding data
    INTERMEDIATE_VERTEX_BUFFER* AddVertexBuffer(   INTERMEDIATE_MESH* pMesh,
                                                   UINT StreamID, 
                                                   UINT64 NumVertices,
                                                   UINT64 SizeBytes,
                                                   UINT64 StrideBytes,
                                                   D3DVERTEXELEMENT9* pDecl,
                                                   UINT NumDeclElements,
                                                   void* pBinaryData );

    INTERMEDIATE_INDEX_BUFFER* AddIndexBuffer(	 INTERMEDIATE_MESH* pMesh,
                                                 UINT64 NumIndices,
                                                 UINT64 SizeBytes,
                                                 UINT IndexType,
                                                 void* pBinaryData );

    INTERMEDIATE_MESH* AddMesh( char* pszName );

    void AddFrameInfluence( INTERMEDIATE_MESH* pMesh,
                            char* pszName );
                
    INTERMEDIATE_SUBSET* AddSubset( INTERMEDIATE_MESH* pMesh,
                                    char* pszName,
                                    UINT MaterialID,
                                    UINT PrimitiveType,
                                    UINT64 IndexStart,
                                    UINT64 IndexCount,
                                    UINT64 VertexStart,
                                    UINT64 VertexCount );

    INTERMEDIATE_FRAME* AddFrame(  char* pszName,
                                   INTERMEDIATE_FRAME* pParent,
                                   D3DXMATRIX Matrix,
                                   UINT MeshID );

    INTERMEDIATE_MATERIAL* AddMaterial(  char* pszMaterialName,
                                          char* pszMaterialInstancePath,
                                          char* pszDiffuseTexture,
                                          char* pszNormalTexture,
                                          char* pszSpecularTexture,
                                          D3DXVECTOR4 Diffuse,
                                          D3DXVECTOR4 Ambient,
                                          D3DXVECTOR4 Specular,
                                          D3DXVECTOR4 Emissive,
                                          FLOAT Power );

    INTERMEDIATE_ANIMATION_FRAME_DATA* AddAnimationFrame( char* pszFrameName );

    // Get/Find functions
    void SetBigEndian( bool bBigEndian );
    bool IsBigEndian();
    UINT FrameHeirarchyStackGetCount();
    void FrameHeirarchyStackPush( INTERMEDIATE_FRAME* pFrame );
    INTERMEDIATE_FRAME* FrameHeirarchyStackPop();
    INTERMEDIATE_FRAME* FrameHeirarchyStackGetTop();
    INTERMEDIATE_VERTEX_BUFFER* GetVertexBuffer( UINT i );
    INTERMEDIATE_INDEX_BUFFER* GetIndexBuffer( UINT i );
    INTERMEDIATE_MESH* FindMesh( char* pszName );
    UINT FindMeshIndex( char* pszName );
    INTERMEDIATE_FRAME* FindFrame( char* pszName );
    UINT FindFrameIndex( char* pszName );
    INTERMEDIATE_SUBSET* FindSubset( char* pszName );
    void FindAllSubsets( char* pszName, CStack<INTERMEDIATE_SUBSET*>* pStack );
    INTERMEDIATE_MATERIAL* FindMaterial( char* pszName );
    UINT FindMaterialIndex( char* pszName );
    INTERMEDIATE_ANIMATION_FRAME_DATA* FindAnimationFrame( char* pszName );

    // Animation functions
    bool AllocTransformFrameAnimationData( INTERMEDIATE_ANIMATION_FRAME_DATA* pFrame );
    bool AllocFrameAnimationData( float Duration );
    bool AddPositionKey( INTERMEDIATE_ANIMATION_FRAME_DATA* pFrame, float Time, float x, float y, float z );
    bool AddOrientationKey( INTERMEDIATE_ANIMATION_FRAME_DATA* pFrame, float Time, float w, float x, float y, float z );
    bool AddScaleKey( INTERMEDIATE_ANIMATION_FRAME_DATA* pFrame, float Time, float x, float y, float z );

    // Mesh fixup functions
    bool FixMesh();
    bool EndianSwap();
    void ConvertToLittleEndian();
    void ConvertToBigEndian();
    bool GenerateTangents( IDirect3DDevice9* pd3dDevice );
    bool ConvertVertexData( IDirect3DDevice9* pd3dDevice, D3DVERTEXELEMENT9* pDecl, UINT NumElements );

    // Save functions
    bool SaveAsSDKMesh( WCHAR* szFileName );
    bool SaveAsXFile( IDirect3DDevice9* pd3dDevice, WCHAR* szFileName, bool bBinary );  
    bool SaveAnimationData( WCHAR* szFileName );

protected:
    bool FixFrameTicks( INTERMEDIATE_ANIMATION_FRAME_DATA* pBone );
    bool SolidifyFrameAnimationData(INTERMEDIATE_ANIMATION_FRAME_DATA* pFrame);
    bool ConvertVertexData( IDirect3DDevice9* pd3dDevice, void* pInVertices, UINT64 NumInVertices, D3DVERTEXELEMENT9* pInDecl, UINT NumInElements, 
                        void** ppOutVertices, UINT64* pOutputStride, D3DVERTEXELEMENT9* pOutDecl, UINT NumOutElements );

private:
    CStack<INTERMEDIATE_FRAME*>			m_FrameHeirarchyStack;
    WCHAR							    m_szName[MAX_PATH];
    bool							    m_bBigEndian;

    // Data
    CStack<INTERMEDIATE_VERTEX_BUFFER*>	m_VertexBufferArray;
    CStack<INTERMEDIATE_INDEX_BUFFER*>  m_IndexBufferArray;
    CStack<INTERMEDIATE_MESH*>			m_MeshArray;
    CStack<INTERMEDIATE_SUBSET*>		m_SubsetArray;
    CStack<INTERMEDIATE_FRAME*>			m_FrameArray;
    CStack<INTERMEDIATE_MATERIAL*>      m_MaterialArray;

    // animation
    UINT				m_TPS;
    float				m_AnimDuration;
    UINT				m_NumTicks;
    CStack<INTERMEDIATE_ANIMATION_FRAME_DATA*> m_AnimationFrameData;
};

#endif