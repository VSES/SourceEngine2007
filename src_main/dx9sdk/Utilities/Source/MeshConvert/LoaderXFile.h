//--------------------------------------------------------------------------------------
// File: LoaderXFile.h
//
// Copyright (c) Microsoft Corp. All rights reserved.
//--------------------------------------------------------------------------------------
#pragma once
#ifndef _LOADERXFILE_
#define _LOADERXFILE_
#include "Loader.h"

class CLoaderXFile : public CLoader
{
public:
    CLoaderXFile();
    ~CLoaderXFile();

    HRESULT Load( WCHAR* szFileName, FRAME_TRANSFORM_TYPE requestedFHT );
    CIntermediateMesh* GetMesh();

private:
    HRESULT CreateIntermediateMesh( D3DVERTEXELEMENT9* pDecl,
									UINT NumElements );

    D3DXMATERIAL*       m_pMats;
    DWORD               m_dwNumMaterials;
    D3DXATTRIBUTERANGE* m_pAttr;
    DWORD               m_dwNumAttr;
    ID3DXMesh*          m_pMesh;
};

#endif