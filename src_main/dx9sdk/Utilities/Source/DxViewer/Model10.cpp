#include "D3D10.h"
#include "D3DX10.h"
#include <vector>
#include "sas.h"
#include "DXUT.h"
#include "DXUTmisc.h"

#include "resource.h"
#include "App.h"
#include "Camera.h"
#include "FileSystem.h"
#include "Model10.h"
#include "Render10.h"
#include "IncludeHandler10.h"
#include "ShowShared10.h"


#define ENABLE_DX10_SKINNING

struct Face9
{
    union
    {
        UINT i[3];
        struct 
        {
            UINT i0, i1, i2;
        };
    };
};

struct AdjacentFace9
{
    union
    {
        UINT i[3];
        struct 
        {
            UINT i0, i1, i2;
        };
    };
};
struct Edge
{
    union
    {
        UINT e[2];
        struct 
        {
            UINT e0, e1;
        };
    };
};

struct Face10
{
    union
    {
        UINT i[6];
        struct 
        {
            UINT i0;
            UINT a0;
            UINT i1;
            UINT a1;
            UINT i2;
            UINT a2;
        };
    };
};

ModelSubset10::ModelSubset10() : ModelSubset()
{
    pMesh10 = NULL;
    pUnskinnedMesh = NULL;
    pSkinInfo10 = NULL;
    ShowLayout = NULL;
}

//--------------------------------------------------------------------------------------
ModelSubset10::~ModelSubset10()
{
    ModelSubset::~ModelSubset();

    SAFE_RELEASE(pMesh10);
    SAFE_RELEASE(pUnskinnedMesh);
    SAFE_RELEASE(pSkinInfo10);
    SAFE_RELEASE(ShowLayout);
}

HRESULT Model10::Load()
{
    HRESULT hr = S_OK;

    NullRef = DXUTCreateRefDevice9( STATE_GET( Dialog ), true );

    hr = __super::Load();

    SAFE_RELEASE(NullRef);

    return hr;
}
    
//--------------------------------------------------------------------------------------
HRESULT ModelLoader10::SetMesh( ModelSubset* pModelSubset, LPD3DXMESH pMesh9, const DWORD* pAdjacency, bool skinned )
{
    HRESULT hr = S_OK;
    ModelSubset10* pModelSubset10 = (ModelSubset10*)pModelSubset;

    V( ConvertMesh9ToMesh10(pMesh9, pAdjacency, pModelSubset10->pMesh10) );

    D3DVERTEXELEMENT9 decl9[MAXD3DDECLLENGTH+1];
    V( pMesh9->GetDeclaration(decl9) );
    pModelSubset10->nVertexStride = D3DXGetDeclVertexSize( decl9, 0);

    DXVGetShowShared10().CreateInputLayout( pModelSubset10->pMesh10, &pModelSubset10->ShowLayout );

    return hr;
}

//--------------------------------------------------------------------------------------
HRESULT ModelLoader10::SetSkin(ModelSubset* pModelSubset, LPD3DXSKININFO pSkin9)
{

#ifndef ENABLE_DX10_SKINNING
    DXVGetApp().OutputW( L"WARNING: Skinning is not supported in Direct3D 10 Mode." );
    return S_OK;
#else
    HRESULT hr = S_OK;

    ModelSubset10* pModelSubset10 = (ModelSubset10*) pModelSubset;

    V( ConvertSkin9ToSkin10(
        pModelSubset10->pMesh10->GetVertexCount(),
        pSkin9, 
        pModelSubset10->pSkinInfo10, 
        pModelSubset10->SkinBoneNames, 
        pModelSubset10->SkinBoneOffsets, 
        pModelSubset10->SkinChannels) );

    CONST D3D10_INPUT_ELEMENT_DESC * pDesc;
    UINT DeclCount;
    pModelSubset10->pMesh10->GetVertexDescription( &pDesc, &DeclCount );

    V( pModelSubset10->pMesh10->CloneMesh(
            pModelSubset10->pMesh10->GetFlags(),
            "SV_Position",
            pDesc,
            DeclCount,
            &pModelSubset10->pUnskinnedMesh) );
    return hr;
#endif
}
//--------------------------------------------------------------------------------------
HRESULT ModelLoader10::ConvertSkin9ToSkin10( 
    UINT numVertices,
    LPD3DXSKININFO pSkin9, 
    ID3DX10SkinInfo*& pSkin10, 
    std::vector<std::string>& Skin10_BoneNames, 
    std::vector<D3DXMATRIX>& Skin10_BoneOffsets,
    std::vector<D3DX10_SKINNING_CHANNEL>& Skin10_Channels)
{
    HRESULT hr = S_OK;
    UINT i;

    V( D3DX10CreateSkinInfo(&pSkin10) );

    V( pSkin10->AddVertices( numVertices ) );
    V( pSkin10->AddBones( pSkin9->GetNumBones() ) );
    Skin10_BoneNames.resize(pSkin9->GetNumBones());
    Skin10_BoneOffsets.resize(pSkin9->GetNumBones());

    for(  i = 0; i < pSkin9->GetNumBones(); i++ )
    {
        DWORD* vertices = (DWORD*) alloca( sizeof(DWORD) * pSkin9->GetNumBoneInfluences(i) );
        FLOAT* weights = (FLOAT*) alloca( sizeof(FLOAT) * pSkin9->GetNumBoneInfluences(i) );
        
        V( pSkin9->GetBoneInfluence(i, vertices, weights) );

        V( pSkin10->AddBoneInfluences(i, pSkin9->GetNumBoneInfluences(i), (UINT*)vertices, weights) );
    
        Skin10_BoneNames[i] = pSkin9->GetBoneName(i);
        Skin10_BoneOffsets[i] = *pSkin9->GetBoneOffsetMatrix(i);
    }

    Skin10_Channels.resize(0);
    D3DVERTEXELEMENT9 decl9[MAXD3DDECLLENGTH+1];
    V( pSkin9->GetDeclaration(decl9) );
    UINT32 decl9Length = D3DXGetDeclLength(decl9);
    for( i = 0; i < decl9Length; i++)
    {
        D3DVERTEXELEMENT9& ve9 = decl9[i];
        switch( ve9.Usage )
        {
        case D3DDECLUSAGE_POSITION:
            {
                D3DX10_SKINNING_CHANNEL channel = { ve9.Offset, ve9.Offset, false };
                Skin10_Channels.push_back(channel);
            }
            break;
        case D3DDECLUSAGE_NORMAL:
        case D3DDECLUSAGE_BINORMAL:
        case D3DDECLUSAGE_TANGENT:
            {
                D3DX10_SKINNING_CHANNEL channel = { ve9.Offset, ve9.Offset, true };
                Skin10_Channels.push_back(channel);
            }
            break;
        };
    }

    return hr;
}

//--------------------------------------------------------------------------------------
HRESULT ModelLoader10::ConvertMesh9ToMesh10( LPD3DXMESH pMesh9, const DWORD* pAdjacency9, ID3DX10Mesh*& pMesh10)
{
    //pAdjacency9 = NULL;

    HRESULT hr = S_OK;
    UINT32 i;

    bool is32bit = ((pMesh9->GetOptions()&D3DXMESH_32BIT) != 0);


    D3DVERTEXELEMENT9 decl9[MAXD3DDECLLENGTH+1];
    V( pMesh9->GetDeclaration(decl9) );
    UINT32 decl9Length = D3DXGetDeclLength(decl9);

    D3D10_INPUT_ELEMENT_DESC* decl10 = (D3D10_INPUT_ELEMENT_DESC*) alloca( decl9Length * sizeof(D3D10_INPUT_ELEMENT_DESC) );

    static char* SemanticMap[] = 
    {
        "SV_Position",
        "BLENDWEIGHT",
        "BLENDINDICES",
        "NORMAL",
        "PSIZE",
        "TEXCOORD",
        "TANGENT",
        "BINORMAL",
        "TESSFACTOR",
        "POSITIONT",
        "COLOR",
        "FOG",
        "DEPTH",
        "SAMPLE",
    };

    static DXGI_FORMAT FormatMap[] = 
    {
        DXGI_FORMAT_R32_FLOAT,//D3DDECLTYPE_FLOAT1 = 0,
        DXGI_FORMAT_R32G32_FLOAT,//D3DDECLTYPE_FLOAT2 = 1,
        DXGI_FORMAT_R32G32B32_FLOAT,//D3DDECLTYPE_FLOAT3 = 2,
        DXGI_FORMAT_R32G32B32A32_FLOAT,//D3DDECLTYPE_FLOAT4 = 3,
        DXGI_FORMAT_R8G8B8A8_UNORM,//D3DDECLTYPE_D3DCOLOR = 4,
        DXGI_FORMAT_R8G8B8A8_UINT,//D3DDECLTYPE_UBYTE4 = 5,
        DXGI_FORMAT_R16G16_UINT,//D3DDECLTYPE_SHORT2 = 6,
        DXGI_FORMAT_R16G16B16A16_UINT,//D3DDECLTYPE_SHORT4 = 7,
        DXGI_FORMAT_R8G8B8A8_UNORM,//D3DDECLTYPE_UBYTE4N = 8,
        DXGI_FORMAT_R16G16_SNORM,//D3DDECLTYPE_SHORT2N = 9,
        DXGI_FORMAT_R16G16B16A16_SNORM,//D3DDECLTYPE_SHORT4N = 10,
        DXGI_FORMAT_R16G16_UNORM,//D3DDECLTYPE_USHORT2N = 11,
        DXGI_FORMAT_R16G16B16A16_UNORM,//D3DDECLTYPE_USHORT4N = 12,
        DXGI_FORMAT_R10G10B10A2_UINT,//D3DDECLTYPE_UDEC3 = 13,
        DXGI_FORMAT_R10G10B10A2_UNORM,//D3DDECLTYPE_DEC3N = 14,
        DXGI_FORMAT_R16G16_FLOAT,//D3DDECLTYPE_FLOAT16_2 = 15,
        DXGI_FORMAT_R16G16B16A16_FLOAT,//D3DDECLTYPE_FLOAT16_4 = 16,
        DXGI_FORMAT_UNKNOWN,//D3DDECLTYPE_UNUSED = 17,
    };

    //translate the decl
    for(i = 0; i < decl9Length; i++)
    {
        D3DVERTEXELEMENT9& ve9 = decl9[i];
        D3D10_INPUT_ELEMENT_DESC& ve10 = decl10[i];

        ve10.SemanticName = SemanticMap[ ve9.Usage ];
        ve10.SemanticIndex = ve9.UsageIndex;
        ve10.Format = FormatMap[ ve9.Type ];
        ve10.InputSlot = 0;
        ve10.AlignedByteOffset = ve9.Offset;
        ve10.InputSlotClass = D3D10_INPUT_PER_VERTEX_DATA;
        ve10.InstanceDataStepRate = 0;
    }


    void* pData9 = NULL;
    void* pData10 = NULL;
    SIZE_T Data10Size = 0;

    V( D3DX10CreateMesh( 
        DXUTGetD3D10Device(), 
        decl10, 
        decl9Length, 
        SemanticMap[D3DDECLUSAGE_POSITION], 
        pMesh9->GetNumVertices(),
        pMesh9->GetNumFaces(),
        ((pMesh9->GetOptions()&D3DXMESH_32BIT) ? D3DX10_MESH_32_BIT : 0),// | ( (pAdjacency9) ? D3DX10_MESH_GS_ADJACENCY : 0 ), 
        &pMesh10) );

    V( pMesh9->LockVertexBuffer(D3DLOCK_READONLY, &pData9));

    ID3DX10MeshBuffer* pVb10 = NULL;
    V( pMesh10->GetVertexBuffer(0, &pVb10) );
    V( pVb10->Map(&pData10, &Data10Size ) );
    memcpy(pData10, pData9, Data10Size);
    V( pVb10->Unmap() );
    SAFE_RELEASE(pVb10);
    //V( pMesh10->SetVertexData(0, pData9) );

    V( pMesh9->UnlockVertexBuffer() );

    V( pMesh9->LockIndexBuffer(D3DLOCK_READONLY, &pData9) );
    
    
    ID3DX10MeshBuffer* pIb10 = NULL;
    V( pMesh10->GetIndexBuffer(&pIb10) );
    V( pIb10->Map((void**)&pData10, &Data10Size) );
    /*if(pAdjacency9)
    {
        DWORD* pPointRep9 = (DWORD*) alloca( sizeof(DWORD)*pMesh9->GetNumVertices() );
        V( pMesh9->ConvertAdjacencyToPointReps(pAdjacency9, pPointRep9 ) );


        UINT cFace = pMesh9->GetNumFaces();
        for( UINT iFace = 0; iFace < cFace; iFace++ )
        {
            Face10& face10 = ((Face10*)pData10)[iFace];
            Face9& face9 = ((Face9*)pData9)[iFace];

            Face9 repFace9 = 
            {
                    pPointRep9[ face9.i[0] ],
                    pPointRep9[ face9.i[1] ],
                    pPointRep9[ face9.i[2] ],
            };

            Edge EdgeList[3] =     
            { 
                {face9.i0, face9.i1},
                {face9.i1, face9.i2},
                {face9.i2, face9.i0},
            };

            AdjacentFace9& AdjFace = ((AdjacentFace9*)pAdjacency9)[iFace];
            Face9* AdjFaceList[3] = 
            {
                (AdjFace.i0 == ((DWORD)-1) ?  NULL : &(((Face9*)pData9)[AdjFace.i0])),
                (AdjFace.i1 == ((DWORD)-1) ?  NULL : &(((Face9*)pData9)[AdjFace.i1])),
                (AdjFace.i2 == ((DWORD)-1) ?  NULL : &(((Face9*)pData9)[AdjFace.i2])),
            };


            for( UINT iEdge = 0; iEdge < 3; iEdge++ )
            {
                bool foundVert = false;
                //set vertex
                face10.i[iEdge*2] = face9.i[iEdge];

                //set default adjacency
                face10.i[(iEdge*2)+1] = face9.i[iEdge]; //encase we cant find the proper adjacency


                if( AdjFaceList[iEdge] != NULL )
                {
                    Face9& realAdjFace = *(AdjFaceList[iEdge]);
                    Face9 repAdjFace = 
                    {
                            pPointRep9[ realAdjFace.i[0] ],
                            pPointRep9[ realAdjFace.i[1] ],
                            pPointRep9[ realAdjFace.i[2] ],
                    };

                    Edge& realSharedEdge = EdgeList[iEdge];
                    Edge repSharedEdge = 
                    {
                        pPointRep9[ realSharedEdge.e[0] ],
                        pPointRep9[ realSharedEdge.e[1] ],
                    };

                    for( UINT iVert = 0; iVert < 3; iVert++ )
                    {
                        if(repAdjFace.i[iVert] != repSharedEdge.e0
                            && repAdjFace.i[iVert] != repSharedEdge.e1)
                        {
                            face10.i[(iEdge*2)+1] = realAdjFace.i[iVert];
                            foundVert = true;
                            break;
                        }
                    }
                }
            }
        }
    }
    else*/
    {
        memcpy( pData10, pData9, Data10Size);
    }
    V( pIb10->Unmap() );
    SAFE_RELEASE(pIb10);

    V( pMesh9->UnlockIndexBuffer() );
    

    ID3DX10MeshBuffer *pAttributeBuffer10 = NULL;
    V( pMesh10->GetAttributeBuffer(&pAttributeBuffer10) );
    V( pAttributeBuffer10->Map(&pData10, &Data10Size) );
    V( pMesh9->LockAttributeBuffer(D3DLOCK_READONLY, (DWORD**)&pData9) )

    memcpy( pData10, pData9, Data10Size);

    V( pMesh9->UnlockAttributeBuffer() );
    V( pAttributeBuffer10->Unmap() );
    SAFE_RELEASE(pAttributeBuffer10);

    pMesh10->GenerateAdjacencyAndPointReps( 0.001f );
    pMesh10->Optimize(D3DX10_MESHOPT_ATTR_SORT, NULL, NULL );
    pMesh10->CommitToDevice();

    return hr;
}



//--------------------------------------------------------------------------------------
void ModelLoader10::MakeMacroArray( const std::list<Define>& defineList, std::vector<D3D10_SHADER_MACRO>& macroArray )
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
void ModelLoader10::GetDefaultShader( Sas::SourceInfo& si )
{
    si.Set( MAKEINTRESOURCE(IDR_DEFAULT10FX), MAKEINTRESOURCE(RT_RCDATA), NULL);
}

Material* ModelLoader10::CreateMaterial(ModelSubset* pModelSubset, Sas::SourceInfo& si)
{
    HRESULT hr = S_OK;
    std::vector<D3D10_SHADER_MACRO> macroArray;
    ModelLoader10::MakeMacroArray( STATE_GET( Defines ), macroArray );

    Material10* pResult = new Material10;

    bool dbgVS = STATE_GET( DebugVertexShaders );
    bool dbgPS = STATE_GET( DebugPixelShaders );
    DWORD HlslFlags = D3D10_SHADER_ENABLE_BACKWARDS_COMPATIBILITY |
            ((dbgVS|dbgPS)? D3D10_SHADER_DEBUG : 0) |
            ((dbgVS)? D3D10_SHADER_FORCE_VS_SOFTWARE_NO_OPT : 0) |
            ((dbgPS)? D3D10_SHADER_FORCE_PS_SOFTWARE_NO_OPT : 0) |
            (STATE_GET(OptimizeShaders) ? 0 : D3D10_SHADER_SKIP_OPTIMIZATION) |
            (STATE_GET(ValidateShaders) ? 0 : D3D10_SHADER_SKIP_VALIDATION) |
            (STATE_GET(Preshaders) ? 0 : D3D10_SHADER_NO_PRESHADER) |
            (STATE_GET(PartialPrecision) ? D3D10_SHADER_PARTIAL_PRECISION : 0) |
            (STATE_GET(PreferDynamicFlow) ? D3D10_SHADER_PREFER_FLOW_CONTROL : D3D10_SHADER_AVOID_FLOW_CONTROL);


    wstring includes = si.GetDirectoryPath()? si.GetDirectoryPath() : L"";
    includes += L";";
    includes += STATE_GET(Includes);

    hr= Sas::Effect10::FromSource( 
            DXUTGetD3D10Device(), 
            si, 
            &macroArray.front(), 
            &DXVGetIncludeHandler10(includes.c_str()), 
            HlslFlags, 
            0, 
            (Sas::Effect10**)&pResult->Effect );

    if( SUCCEEDED(hr) )
    {
        UINT iTec, iPass;
        const D3D10_INPUT_ELEMENT_DESC* vertDesc;
        UINT vertDescLength;
        ((ModelSubset10*)pModelSubset)->pMesh10->GetVertexDescription( &vertDesc, &vertDescLength);

        ID3D10Effect* effect = ((Sas::Effect10*)pResult->Effect)->GetD3DEffect();
        
        D3D10_EFFECT_DESC effectDesc;
        effect->GetDesc( &effectDesc );

        pResult->Techniques.resize( effectDesc.Techniques );

        for(iTec = 0 ; iTec < effectDesc.Techniques ; iTec++ )
        {

            ID3D10EffectTechnique* technique = effect->GetTechniqueByIndex(iTec);

            D3D10_TECHNIQUE_DESC techniqueDesc;
            technique->GetDesc( &techniqueDesc );

            pResult->Techniques[iTec].Passes.resize( techniqueDesc.Passes );

            for( iPass = 0; iPass < techniqueDesc.Passes; iPass++ )
            {

                ID3D10EffectPass* pass = technique->GetPassByIndex(iPass);

                D3D10_PASS_DESC passDesc;
                pass->GetDesc( &passDesc );
              
                hr= DXUTGetD3D10Device()->CreateInputLayout(
                        vertDesc, 
                        vertDescLength, 
                        passDesc.pIAInputSignature, 
                        passDesc.IAInputSignatureSize,
                        &pResult->Techniques[iTec].Passes[iPass].Layout);
                if(FAILED(hr))
                {
                    DXVGetApp().OutputW( L"Could not fully match shader to mesh declaration for technique %d pass %d", iTec, iPass );
                    goto e_Exit;
                }
            }
        }

        std::vector<TechniqueInfo>& EffectInfo = DXVGetRender10().GetEffectInfo();
        if(!STATE_GET(FileFX).empty() && EffectInfo.empty() )
        {
            EffectInfo.resize( effectDesc.Techniques );
            for(iTec = 0 ; iTec < effectDesc.Techniques ; iTec++ )
            {
                TechniqueInfo& techInfo = EffectInfo[iTec];

                ID3D10EffectTechnique* technique = effect->GetTechniqueByIndex(iTec);

                D3D10_TECHNIQUE_DESC techniqueDesc;
                technique->GetDesc( &techniqueDesc );

                CharToWString( techniqueDesc.Name, techInfo.Name);

                techInfo.Passes.resize(techniqueDesc.Passes);

                for( iPass = 0; iPass < techniqueDesc.Passes; iPass++ )
                {
                    ID3D10EffectPass* pass = technique->GetPassByIndex(iPass);

                    D3D10_PASS_DESC passDesc;
                    pass->GetDesc( &passDesc );

                    CharToWString( passDesc.Name, techInfo.Passes[iPass] );
                }
            }
        }
    }

e_Exit:
    if( FAILED(hr) )
        SAFE_DELETE( pResult);

    return pResult;
}


//--------------------------------------------------------------------------------------
HRESULT ModelSubset10::UpdateSkins(D3DXMATRIX* BonePalette, D3DXMATRIX* BoneInverseTransposePalette)
{
    HRESULT hr = S_OK;

    // if there is a skinmesh, then setup the bone matrices
    if (HasSkin())
    {
        //get skinning data
        ID3DX10MeshBuffer* pSrcVb=NULL;
        void* pSrcData=NULL;
        SIZE_T nSrcLength=0;

        V( pUnskinnedMesh->GetVertexBuffer( 0, &pSrcVb) );
        V( pSrcVb->Map( &pSrcData, &nSrcLength ) );

        ID3DX10MeshBuffer* pDstVb;
        void* pDstData=NULL;
        SIZE_T nDstLength=0;

        V( pMesh10->GetVertexBuffer(0, &pDstVb) );
        V( pDstVb->Map( &pDstData, &nDstLength ) );

        // do skinning
        V( pSkinInfo10->DoSoftwareSkinning(
            0, 
            pMesh10->GetVertexCount(),
            pSrcData,
            nVertexStride, 
            pDstData,
            nVertexStride,
            BonePalette, 
            BoneInverseTransposePalette, 
            &SkinChannels.front(),
            (UINT)SkinChannels.size() ) );
        
        V( pDstVb->Unmap( ) );
        SAFE_RELEASE(pDstVb);

        V( pSrcVb->Unmap( ) );
        SAFE_RELEASE(pSrcVb);

        pMesh10->CommitToDevice();
    }

    return hr;
}

//--------------------------------------------------------------------------------------
HRESULT Model10::ExtractFromMemory( void* pInMemory, DWORD InMemorySize)
{
    HRESULT hr = S_OK;
    hr= D3DXLoadMeshHierarchyFromXInMemory(
                pInMemory, InMemorySize, 
                D3DXMESH_32BIT|D3DXMESH_MANAGED,
                NullRef,
                &DXVGetModelLoader10(L""), 
                NULL, 
                (LPD3DXFRAME*)&m_pRoot, 
                &m_pAnimController );

    return hr;
}
//--------------------------------------------------------------------------------------
HRESULT Model10::ExtractFromFile( const Sas::SourceInfo& sf )
{
    if(sf.GetSourceType() != Sas::SOURCEINFO_FILE)
        return E_FAIL;

    HRESULT hr = S_OK;
    hr= D3DXLoadMeshHierarchyFromX( 
                sf.GetFilePath(), 
                D3DXMESH_32BIT|D3DXMESH_MANAGED, 
                NullRef,
                &DXVGetModelLoader10( sf.GetDirectoryPath() ), 
                NULL, 
                (LPD3DXFRAME*)&m_pRoot, 
                &m_pAnimController );

    return hr;
}

//--------------------------------------------------------------------------------------
HRESULT ModelSubset10::CalculateBoundingBox( D3DXVECTOR3& minVec, D3DXVECTOR3& maxVec)
{
    HRESULT hr = S_OK;


    ID3DX10MeshBuffer* pVB = NULL;
    V( pMesh10->GetVertexBuffer(0, &pVB) );

    BYTE* pData;
    SIZE_T DataSize;
    V( pVB->Map((void**)&pData, &DataSize ) );
//    V( pMesh10->GetVertexData( 0, (void**)&pData, &DataSize ) );

    const D3D10_INPUT_ELEMENT_DESC* pDesc;
    UINT descLength;
    pMesh10->GetVertexDescription( &pDesc, &descLength);

    UINT offset = 0;
    for(UINT i = 0; i < descLength; i++)
    {
        if(0 == _stricmp( pDesc[i].SemanticName, "SV_Position") )
            offset = pDesc[i].AlignedByteOffset;
    }

    V( D3DXComputeBoundingBox(
            (const D3DXVECTOR3*)(pData+offset),
            pMesh10->GetVertexCount(),
            nVertexStride, 
            &minVec, 
            &maxVec));

    V( pVB->Unmap() );
    SAFE_RELEASE(pVB);


    return hr;
}


//--------------------------------------------------------------------------------------
void ModelLoader10::PostCreateModelSubset( ModelSubset* pModelSubset )
{

}

//--------------------------------------------------------------------------------------
DECLARE_SINGLETON_CPP( Model10 );

DECLARE_SINGLETON_DIR_CPP( ModelLoader10, m_ContentDirectory );
