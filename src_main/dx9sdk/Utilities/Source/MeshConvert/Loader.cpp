//--------------------------------------------------------------------------------------
// File: Loader.cpp
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include "stdafx.h"


//-----------------------------------------------------------------------------
CLoader::CLoader() : m_pIntermediateMesh(NULL)
{
}


//-----------------------------------------------------------------------------
CLoader::~CLoader()
{
}


//-----------------------------------------------------------------------------
HRESULT CLoader::Load( WCHAR* szFileName, FRAME_TRANSFORM_TYPE requestedBHT )
{
    HRESULT hr = S_OK;

    size_t nLength = 0;
    WCHAR* pszExt = wcsrchr( szFileName, L'.' );
    if( pszExt ) StringCchLength( pszExt, 32, &nLength );

    if( nLength == 2 && pszExt && wcscmp( pszExt, L".x" ) == 0 )
    {
        CLoaderXFile loader;
        hr = loader.Load( szFileName, requestedBHT );
        m_pIntermediateMesh = loader.GetMesh();
    }
    
    if( SUCCEEDED(hr) && !m_pIntermediateMesh )
        hr = E_FAIL;

    return hr;
}


//-----------------------------------------------------------------------------
HRESULT CLoader::Save( IDirect3DDevice9* pd3dDevice, WCHAR* szFileName, MESH_TYPE meshType )
{
    HRESULT hr = S_OK;

    if( !m_pIntermediateMesh )
        return E_FAIL;

    switch( meshType )
    {
        case MESH_TYPE_X_TEXT: 
            if( !m_pIntermediateMesh->SaveAsXFile( pd3dDevice, szFileName, false ) )
                hr = E_FAIL;
            break;
        case MESH_TYPE_X_BINARY: 
            if( !m_pIntermediateMesh->SaveAsXFile( pd3dDevice, szFileName, true ) )
                hr = E_FAIL;
            break;
        case MESH_TYPE_SDKMESH: 
            if( !m_pIntermediateMesh->SaveAsSDKMesh( szFileName ) )
                hr = E_FAIL;
            break;
    }

    return hr;
}

//-----------------------------------------------------------------------------
HRESULT CLoader::SaveAnimationData( WCHAR* szFileName )
{
    if( !m_pIntermediateMesh )
        return E_FAIL;

    if( !m_pIntermediateMesh->SaveAnimationData( szFileName ) )
        return E_FAIL;

    return S_OK;
}
