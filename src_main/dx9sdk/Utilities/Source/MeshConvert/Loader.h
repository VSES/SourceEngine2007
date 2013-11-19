//--------------------------------------------------------------------------------------
// File: Loader.h
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#pragma once
#ifndef _LOADER_
#define _LOADER_

enum MESH_TYPE
{
    MESH_TYPE_SDKMESH = 0,
    MESH_TYPE_X_BINARY,    
    MESH_TYPE_X_TEXT,   
};

class CLoader
{
public:
    CLoader();
    ~CLoader();

    HRESULT Load( WCHAR* szFileName, 
				  FRAME_TRANSFORM_TYPE requestedBHT );
    HRESULT Save( IDirect3DDevice9* pd3dDevice,
				  WCHAR* szFileName, 
				  MESH_TYPE meshType );
	HRESULT SaveAnimationData( WCHAR* szFileName );

    D3DVERTEXELEMENT9* GetInputVertexDecl();
	CIntermediateMesh*  GetMesh() { return m_pIntermediateMesh; }

protected:
    CIntermediateMesh*  m_pIntermediateMesh;
};


#endif