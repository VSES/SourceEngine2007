#include "sas.h"
#include <vector>
#include "DXUT.h"

#include "resource.h"
#include "App.h"
#include "Camera.h"
#include "FileSystem.h"
#include "IncludeHandler9.h"
#include "Model9.h"
#include "Render10.h"


//--------------------------------------------------------------------------------------
HRESULT ModelLoader9::SetMesh( ModelSubset* pModelSubset, LPD3DXMESH pMesh9, const DWORD* pAdjacency, bool skinned )
{
    HRESULT hr = S_OK;
    pModelSubset->AsMeshContainer()->MeshData.Type = D3DXMESHTYPE_MESH;
    pModelSubset->AsMeshContainer()->MeshData.pMesh = pMesh9;
    pModelSubset->AsMeshContainer()->MeshData.pMesh->AddRef();

    pModelSubset->AsMeshContainer()->pAdjacency = new DWORD[pMesh9->GetNumFaces() * 3];
    memcpy(pModelSubset->AsMeshContainer()->pAdjacency, pAdjacency, pMesh9->GetNumFaces() * 3 * sizeof(DWORD));

    return hr;
}
ModelSubset9::~ModelSubset9()
{
    ModelSubset::~ModelSubset();
    SAFE_RELEASE(pUnskinnedMesh);
}

HRESULT Model9::OnD3D9DeviceReset( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc )
{
    ShowBounds.OnResetDevice();
    return S_OK;
}

void    Model9::OnD3D9DeviceLost()
{
    ShowBounds.OnLostDevice();
}

void    Model9::OnD3D9DeviceDestroyed()
{
    ShowBounds.OnDestroyDevice();
}
//--------------------------------------------------------------------------------------
HRESULT ModelLoader9::SetSkin(ModelSubset* pModelSubset, LPD3DXSKININFO pSkin9)
{
    HRESULT hr = S_OK;
    LPDIRECT3DDEVICE9 pDevice = NULL;

    pSkin9->AddRef();
    pModelSubset->AsMeshContainer()->pSkinInfo = pSkin9;

    V( pModelSubset->AsMeshContainer()->MeshData.pMesh->GetDevice(&pDevice) );

    V( pModelSubset->AsMeshContainer()->MeshData.pMesh->CloneMesh(
            pModelSubset->AsMeshContainer()->MeshData.pMesh->GetOptions(),
            NULL,
            pDevice,
            &((ModelSubset9*)pModelSubset)->pUnskinnedMesh) );

    SAFE_RELEASE(pDevice);
    return hr;
}

//--------------------------------------------------------------------------------------
void ModelLoader9::MakeMacroArray( const std::list<Define>& defineList, std::vector<D3DXMACRO>& macroArray )
{
    macroArray.resize( defineList.size() + 1 );

    UINT index = 0;
    for( std::list<Define>::const_iterator itr = defineList.begin();
        itr != defineList.end();
        itr++, index++)
    {
        macroArray[index].Name = itr->name.c_str();
        macroArray[index].Definition = itr->value.c_str();
    }

    macroArray[ defineList.size() ].Name = NULL;
    macroArray[ defineList.size() ].Definition = NULL;

}

//--------------------------------------------------------------------------------------
void ModelLoader9::GetDefaultShader( Sas::SourceInfo& si )
{
    si.Set( MAKEINTRESOURCE(IDR_DEFAULT9FX), MAKEINTRESOURCE(RT_RCDATA), NULL);
}


Material* ModelLoader9::CreateMaterial(ModelSubset* pModelSubset, Sas::SourceInfo& si)
{
    HRESULT hr = S_OK;
    std::vector<D3DXMACRO> macroArray;
    ModelLoader9::MakeMacroArray( STATE_GET( Defines ), macroArray );

    Material* pResult = new Material;

    bool dbgVS = STATE_GET( DebugVertexShaders );
    bool dbgPS = STATE_GET( DebugPixelShaders );
    DWORD flags = ((dbgVS|dbgPS)? D3DXSHADER_DEBUG : 0) |
        ((dbgVS)? D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT : 0) |
        ((dbgPS)? D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT : 0) |
        (STATE_GET(OptimizeShaders) ? 0 : D3DXSHADER_SKIPOPTIMIZATION) |
        (STATE_GET(ValidateShaders) ? 0 : D3DXSHADER_SKIPVALIDATION) |
        (STATE_GET(Preshaders) ? 0 : D3DXSHADER_NO_PRESHADER) |
        (STATE_GET(PartialPrecision) ? D3DXSHADER_PARTIALPRECISION : 0) |
        (STATE_GET(PreferDynamicFlow) ? D3DXSHADER_PREFER_FLOW_CONTROL : D3DXSHADER_AVOID_FLOW_CONTROL);

    wstring includes = si.GetDirectoryPath()? si.GetDirectoryPath() : L"";
    includes += L";";
    includes += STATE_GET(Includes);

    V( Sas::Effect9::FromSource( 
        DXUTGetD3D9Device(), 
        si, 
        &macroArray.front(), 
        &DXVGetIncludeHandler9(includes.c_str()), 
        flags,
        (Sas::Effect9**)&pResult->Effect ) );

    if(SUCCEEDED(hr))
    {
        std::vector<TechniqueInfo>& EffectInfo = DXVGetRender9().GetEffectInfo();
        if(!STATE_GET(FileFX).empty() && EffectInfo.empty() )
        {
            UINT iTec, iPass;
            ID3DXEffect* pEffect = ((Sas::Effect9*)pResult->Effect)->GetD3DXEffect();

            D3DXEFFECT_DESC effectDesc;
            V(pEffect->GetDesc( &effectDesc ));

            EffectInfo.resize( effectDesc.Techniques );
            for(iTec = 0 ; iTec < effectDesc.Techniques ; iTec++ )
            {
                TechniqueInfo& techInfo = EffectInfo[iTec];

                D3DXHANDLE technique = pEffect->GetTechnique(iTec); 

                D3DXTECHNIQUE_DESC techniqueDesc;
                V( pEffect->GetTechniqueDesc( technique, &techniqueDesc ) );

                CharToWString( techniqueDesc.Name, techInfo.Name);

                techInfo.Passes.resize(techniqueDesc.Passes);

                for( iPass = 0; iPass < techniqueDesc.Passes; iPass++ )
                {
                    D3DXHANDLE pass = pEffect->GetPass(technique, iPass);

                    D3DXPASS_DESC passDesc;
                    pEffect->GetPassDesc( pass, &passDesc );

                    CharToWString( passDesc.Name, techInfo.Passes[iPass] );
                }
            }
        }
    }

    if( FAILED(hr) )
        SAFE_DELETE( pResult);

    return pResult;
}


//--------------------------------------------------------------------------------------
HRESULT ModelSubset9::UpdateSkins(D3DXMATRIX* BonePalette, D3DXMATRIX* BoneInverseTransposePalette)
{
    HRESULT hr = S_OK;
    if ( HasSkin() )
    {
        //get skinning data
        PBYTE       pbVerticesSrc;
        PBYTE       pbVerticesDest;
        V( pUnskinnedMesh->LockVertexBuffer(D3DLOCK_READONLY, (LPVOID*)&pbVerticesSrc) );
        V( AsMeshContainer()->MeshData.pMesh->LockVertexBuffer(0, (LPVOID*)&pbVerticesDest) );

        // do skinning
        V( AsMeshContainer()->pSkinInfo->UpdateSkinnedMesh(BonePalette, BoneInverseTransposePalette, pbVerticesSrc, pbVerticesDest) );

        V( pUnskinnedMesh->UnlockVertexBuffer());
        V( AsMeshContainer()->MeshData.pMesh->UnlockVertexBuffer());



        if(STATE_GET(ShowNormals))
            ShowNormals.Move(BonePalette, BoneInverseTransposePalette);
        if(STATE_GET(ShowPoints))
            ShowPoints.Move(BonePalette, BoneInverseTransposePalette);
        if(STATE_GET(ShowEdges))
            ShowEdges.Move(BonePalette, BoneInverseTransposePalette);
        if(STATE_GET(ShowCreases))
            ShowCreases.Move(BonePalette, BoneInverseTransposePalette);
    }
    return hr;
}


//--------------------------------------------------------------------------------------
HRESULT Model9::ExtractFromMemory( void* pInMemory, DWORD InMemorySize)
{
    HRESULT hr = S_OK;
    hr =  D3DXLoadMeshHierarchyFromXInMemory(
                pInMemory, InMemorySize, 
                D3DXMESH_32BIT|D3DXMESH_MANAGED,
                DXUTGetD3D9Device(),
                &DXVGetModelLoader9(L""), 
                NULL, 
                (LPD3DXFRAME*)&m_pRoot, 
                &m_pAnimController );

    return hr;
}
//--------------------------------------------------------------------------------------
HRESULT Model9::ExtractFromFile( const Sas::SourceInfo& sf )
{
    if(sf.GetSourceType() != Sas::SOURCEINFO_FILE)
        return E_FAIL;

    HRESULT hr = S_OK;
    hr =   D3DXLoadMeshHierarchyFromX( 
                sf.GetFilePath(), 
                D3DXMESH_32BIT|D3DXMESH_MANAGED, 
                DXUTGetD3D9Device(),
                &DXVGetModelLoader9( sf.GetDirectoryPath() ), 
                NULL, 
                (LPD3DXFRAME*)&m_pRoot, 
                &m_pAnimController );

    return hr;

}

//--------------------------------------------------------------------------------------
HRESULT ModelSubset9::CalculateBoundingBox( D3DXVECTOR3& minVec, D3DXVECTOR3& maxVec )
{
    HRESULT hr = S_OK;
    BYTE* pData;
    V( AsMeshContainer()->MeshData.pMesh->LockVertexBuffer( D3DLOCK_READONLY, (void**)&pData) );

    UINT offset = 0;
    D3DVERTEXELEMENT9 decl9[MAXD3DDECLLENGTH+1];
    V( AsMeshContainer()->MeshData.pMesh->GetDeclaration(decl9) );
    for(int i = 0; i < MAXD3DDECLLENGTH; i++)
    {
        if(decl9[i].Usage == D3DDECLUSAGE_POSITION)
        {
            offset = decl9[i].Offset;
            break;
        }
    }

    V( D3DXComputeBoundingBox(
            (const D3DXVECTOR3*)(pData+offset),
            AsMeshContainer()->MeshData.pMesh->GetNumVertices(),
            AsMeshContainer()->MeshData.pMesh->GetNumBytesPerVertex(), 
            &minVec, 
            &maxVec));
    
    V( AsMeshContainer()->MeshData.pMesh->UnlockVertexBuffer() );

    return hr;
}

//--------------------------------------------------------------------------------------
void ModelLoader9::PostCreateModelSubset( ModelSubset* pModelSubset )
{
    ModelSubset9* pModelSubset9 = (ModelSubset9*)pModelSubset;

    pModelSubset9->ShowNormals.SetMesh(
            pModelSubset9->AsMeshContainer()->MeshData.pMesh,
            pModelSubset9->AsMeshContainer()->pSkinInfo);

    pModelSubset9->ShowPoints.SetMesh(
            pModelSubset9->AsMeshContainer()->MeshData.pMesh,
            pModelSubset9->AsMeshContainer()->pSkinInfo);

    pModelSubset9->ShowEdges.SetMesh(
            pModelSubset9->AsMeshContainer()->MeshData.pMesh,
            pModelSubset9->AsMeshContainer()->pSkinInfo);

    pModelSubset9->ShowCreases.SetMesh(
            pModelSubset9->AsMeshContainer()->MeshData.pMesh,
            pModelSubset9->AsMeshContainer()->pAdjacency,
            pModelSubset9->AsMeshContainer()->pSkinInfo);

    pModelSubset9->ShowAdjacencies.SetMesh(
            pModelSubset9->AsMeshContainer()->MeshData.pMesh,
            pModelSubset9->AsMeshContainer()->pAdjacency,
            pModelSubset9->AsMeshContainer()->pSkinInfo);

}
    
HRESULT Model9::Load()
{
    HRESULT hr = __super::Load();
    if( SUCCEEDED(hr) && m_pRoot)
        ShowBounds.SetBounds( m_Min, m_Max );

    return hr;
}

void Model9::Destroy() 
{ 
    __super::Destroy();
    ShowBounds.Clear();
}


//--------------------------------------------------------------------------------------
DECLARE_SINGLETON_CPP( Model9 );

DECLARE_SINGLETON_DIR_CPP( ModelLoader9, m_ContentDirectory );
