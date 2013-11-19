#include <vector>
#include "sas.h"
#include "DXUT.h"

#include "resource.h"
#include "App.h"
#include "Camera.h"
#include "FileSystem.h"
#include "Model.h"
#include "Render.h"

ModelSubset* ModelSubset::GetNextModelSubset()
{ 
    return CastMeshContainerToModelSubset( Container->pNextMeshContainer ); 
}

DXVMESHCONTAINER* ModelSubset::AsMeshContainer()
{ 
    return Container; 
}

Material::~Material() 
{ 
    SAFE_DELETE(Effect); 
};


ModelSubset* ModelSubset::CastMeshContainerToModelSubset( const D3DXMESHCONTAINER* a )
{
    return a ? ((DXVMESHCONTAINER*)a)->Owner : NULL;
}

DXVMESHCONTAINER* ModelSubset::CastModelSubsetToMeshContainer( const ModelSubset* a )
{
    return a ? a->Container : NULL;
}

Transform* Transform::GetFirstChild()
{ 
    return (Transform*)( pFrameFirstChild ); 
}

Transform* Transform::GetNextSibling()
{ 
    return (Transform*)( pFrameSibling ); 
}

ModelSubset* Transform::GetModelSubset()
{ 
    return pMeshContainer ? ((DXVMESHCONTAINER*)pMeshContainer)->Owner : NULL; 
}

D3DXFRAME* Transform::AsFrame()
{ 
    return this; 
}

DXVMESHCONTAINER::DXVMESHCONTAINER(ModelSubset* owner)
{
    Name = NULL;
    MeshData.pMesh = NULL;
    pMaterials = NULL;
    pEffects = NULL;
    pAdjacency = NULL;
    pSkinInfo = NULL;
    pNextMeshContainer = NULL;
    Owner = owner;
}

DXVMESHCONTAINER::~DXVMESHCONTAINER()
{
    SAFE_DELETE_ARRAY( Name );

    SAFE_RELEASE( MeshData.pMesh );

    if(pMaterials != Render::GetDefaultMaterial()
        && pEffects  != Render::GetDefaultEffectInstance())
    {
        for( UINT iMat=0; iMat < NumMaterials; iMat++ )
        {
            if(pMaterials != NULL)
                SAFE_DELETE_ARRAY( pMaterials[iMat].pTextureFilename );

            if(pEffects != NULL)
            {
                SAFE_DELETE_ARRAY( pEffects[iMat].pEffectFilename );
            
                for( UINT iDef=0; iDef < pEffects[iMat].NumDefaults; iDef++ )
                {
                    SAFE_DELETE_ARRAY( pEffects[iMat].pDefaults[iDef].pParamName );
                    SAFE_DELETE_ARRAY( pEffects[iMat].pDefaults[iDef].pValue );
                }
            }
        }
        
        SAFE_DELETE_ARRAY( pMaterials );
        SAFE_DELETE_ARRAY( pEffects );
    }
    SAFE_DELETE_ARRAY( pAdjacency );
    SAFE_RELEASE( pSkinInfo );


    LOOPSAFE_DELETE( pNextMeshContainer, DXVMESHCONTAINER );

    if(Owner)
    {
        Owner->Container = NULL;
        LOOPSAFE_DELETE(Owner, ModelSubset);
    }

}

Transform::Transform()
{
    Name=NULL;
    D3DXMatrixIdentity( &TransformationMatrix );
    pMeshContainer = NULL;
    pFrameFirstChild = NULL;
    pFrameSibling = NULL;

    D3DXMatrixIdentity( &WorldMatrix );
}
//--------------------------------------------------------------------------------------
Transform::~Transform()
{
    LOOPSAFE_DELETE( pFrameFirstChild,  Transform );
    LOOPSAFE_DELETE( pFrameSibling,  Transform );
    LOOPSAFE_DELETE( pMeshContainer,  DXVMESHCONTAINER );
    SAFE_DELETE_ARRAY( Name );
}

//--------------------------------------------------------------------------------------
ModelSubset::ModelSubset()
{
    pBoundValueTable = NULL;
    Container = new DXVMESHCONTAINER(this);
}

//--------------------------------------------------------------------------------------
ModelSubset::~ModelSubset()
{
    for( UINT i=0; i < Materials.size(); i++ )
    {
        SAFE_DELETE( Materials[i] );
    }
    Materials.clear();

    SAFE_DELETE( pBoundValueTable );

    if(Container)
    {
        Container->Owner = NULL;
        LOOPSAFE_DELETE(Container, DXVMESHCONTAINER);
    }
}
//--------------------------------------------------------------------------------------
void Transform::UpdateMatrices( const D3DXMATRIX* pParentMatrix )
{
    // Concatenate all matrices in the chain
    if (pParentMatrix != NULL)
        D3DXMatrixMultiply(&WorldMatrix, &TransformationMatrix, pParentMatrix);
    else
        WorldMatrix = TransformationMatrix;

    for( Transform* itr = GetFirstChild();
        (itr != NULL);
        itr = itr->GetNextSibling() )
    {
        itr->UpdateMatrices( &WorldMatrix );
    }
}


//--------------------------------------------------------------------------------------
HRESULT Transform::UpdateSkins(Transform* root)
{
    HRESULT hr = S_OK;

    if(root == NULL)
        root = this;

    for(ModelSubset* meshIterator = GetModelSubset() ;
        (meshIterator != NULL) && SUCCEEDED(hr);
        meshIterator = meshIterator->GetNextModelSubset() )
    {
        hr = meshIterator->UpdateSkins( *root, WorldMatrix );
        if(FAILED(hr)) return hr;
    }


    for( Transform* itr = GetFirstChild();
        (itr != NULL) && SUCCEEDED(hr);
        itr = itr->GetNextSibling() )
    {
        hr = itr->UpdateSkins( root );
    }


    return hr;
}
//--------------------------------------------------------------------------------------
HRESULT ModelSubset::UpdateSkins( Transform& root, const D3DXMATRIX& worldMatrix )
{
    HRESULT hr = S_OK;

    // if there is a skinmesh, then setup the bone matrices
    if ( HasSkin() )
    {

        static std::vector< D3DXMATRIX >   BonePalette;
        static std::vector< D3DXMATRIX >   BoneInverseTransposePalette;

        UINT cBones = GetBoneCount();

        if(BonePalette.size() < cBones )
            BonePalette.resize(cBones);
        if(BoneInverseTransposePalette.size() < cBones )
            BoneInverseTransposePalette.resize(cBones);


        D3DXMATRIX worldToLocal;
        D3DXMatrixInverse( &worldToLocal, NULL, &worldMatrix);

        //prepare palette
        for (UINT iBone = 0; iBone < cBones; iBone++)
        {
            D3DXMATRIX tmpMatrix;

            Transform* pTransform = (Transform*) D3DXFrameFind( root.AsFrame(), GetBoneName(iBone) );
            if (pTransform == NULL)
            {
                D3DXMatrixIdentity( &BonePalette[iBone] );
                D3DXMatrixIdentity( &BoneInverseTransposePalette[iBone] );
            }
            else
            {
                D3DXMATRIX meshToJointToWorld;

                D3DXMatrixMultiply
                (
                    &tmpMatrix, 
                    GetBoneOffset(iBone), 
                    &pTransform->WorldMatrix
                );

                D3DXMatrixMultiply
                (
                    &BonePalette[iBone],  
                    &tmpMatrix, 
                    &worldToLocal
                );

                D3DXMatrixInverse( &tmpMatrix, NULL, &BonePalette[iBone] );
                D3DXMatrixTranspose( &BoneInverseTransposePalette[iBone], &tmpMatrix );
            }
        }

        hr = UpdateSkins( &BonePalette.front(), &BoneInverseTransposePalette.front() );
    }

    return hr;
}

//--------------------------------------------------------------------------------------
char* ModelLoader::DuplicateString( LPCSTR Name )
{
    if( NULL == Name )
        return NULL;
    
    char* result = NULL;

    // No work to be done if name is NULL

    // Allocate a new buffer
    const UINT length = (UINT)strlen(Name) + 1;
    result = new CHAR[length];
    if( NULL == result )
        return NULL;
        

    // Copy the string and return
    if( S_OK != StringCchCopyA( result, length, Name ) )
    {
        SAFE_DELETE_ARRAY(result);
        return NULL;        
    }

    return result;
}

//--------------------------------------------------------------------------------------
HRESULT ModelLoader::CreateFrame( LPCSTR Name, D3DXFRAME** ppFrameOut )
{
    HRESULT hr = S_OK;
    
    // Start clean
    (*ppFrameOut) = NULL;
    
    // Create a new frame
    Transform* pNew = CreateTransform();
    if( NULL == pNew )
    {
        hr = E_OUTOFMEMORY;
        goto e_Exit;
    }

    // Duplicate the name string
    pNew->AsFrame()->Name = DuplicateString(Name);
    
    // Copy the frame to the output and return
    (*ppFrameOut) = pNew->AsFrame();
    
e_Exit:
    if(FAILED(hr))
        SAFE_DELETE(pNew);

    return hr;
}

//--------------------------------------------------------------------------------------
HRESULT ModelLoader::CreateMeshContainer( const char* a_Name, 
                                                 const D3DXMESHDATA* a_pMeshData, // use MeshData
                                                 const D3DXMATERIAL* a_pMaterials, 
                                                 const D3DXEFFECTINSTANCE* a_pEffectInstances, 
                                                 DWORD a_dwNumMaterials, 
                                                 const DWORD* a_pAdjacency, 
                                                 ID3DXSkinInfo* a_pSkinInfo, 
                                                 D3DXMESHCONTAINER** a_ppMeshContainerOut ) 
{

    HRESULT hr = S_OK;
    ModelSubset* pModelSubset = NULL;
    LPD3DXMESH pMesh9 = NULL;
    bool deleteAdjacencies = false;

   // Start clean
    (*a_ppMeshContainerOut) = NULL;

    // This sample does not handle patch meshes, so fail when one is found
    if( a_pMeshData == NULL || D3DXMESHTYPE_MESH != a_pMeshData->Type )
    {
        hr=E_FAIL;
        goto e_Exit;
    }


    // Allocate the overloaded structure to return as a D3DXMESHCONTAINER
    pModelSubset = CreateModelSubset();

   
    pMesh9 = a_pMeshData->pMesh;
    pMesh9->AddRef();//because HomogenizeMesh9 is going to swap this out...
    a_pMeshData = NULL;//dont let the user play with this value anymore//use pMesh9


    if(!a_pAdjacency)
    {
        deleteAdjacencies = true;
        UINT size = pMesh9->GetNumFaces() * 3;
        a_pAdjacency = new DWORD[size];
        if(!a_pAdjacency)
        { 
            hr = E_FAIL; 
            goto e_Exit; 
        }

        if(FAILED( pMesh9->GenerateAdjacency( 1e-6f, (DWORD*)(void*)a_pAdjacency ) ) )
            FillMemory((void*)a_pAdjacency, size, 0xFF );
    }

    {//CLEAN ADJACENCIES
        LPD3DXBUFFER pErrors = NULL;
        ID3DXMesh* pNewMesh = NULL;
        if(SUCCEEDED( D3DXCleanMesh(D3DXCLEAN_SKINNING, pMesh9, a_pAdjacency, &pNewMesh, (DWORD*)(void*)a_pAdjacency, &pErrors ) ) )
        {
            if(pMesh9 != pNewMesh)
            {
                SAFE_RELEASE(pMesh9);
                pMesh9 = pNewMesh;
                pNewMesh = NULL;

                V(E_FAIL /*SHOULD NOT BE IN HERE...IF SO THEN D3DXCleanMesh HAS CHANGED*/ );
            }
        }
        SAFE_RELEASE(pNewMesh);

        if(pErrors)
            DXVGetApp().OutputA( (char*)pErrors->GetBufferPointer() );
        SAFE_RELEASE(pErrors);
    }//END//CLEAN ADJACENCIES

    bool is32bit = ((pMesh9->GetOptions()&D3DXMESH_32BIT) != 0);


    V( ModelLoader::HomogenizeMesh9(&pMesh9, a_pAdjacency, a_pSkinInfo) );
    if(FAILED(hr)) 
        goto e_Exit;


    //---------------------------------
    // Name
    //---------------------------------
    pModelSubset->AsMeshContainer()->Name = ModelLoader::DuplicateString( a_Name ); 

    //---------------------------------
    // Mesh
    //---------------------------------
    
    V( SetMesh( pModelSubset, pMesh9,  a_pAdjacency , (a_pSkinInfo != NULL)) );
    if(FAILED(hr)) 
        goto e_Exit;

    //---------------------------------
    // SkinInfo
    //---------------------------------
    if(a_pSkinInfo)
    {
        V( SetSkin( pModelSubset, a_pSkinInfo ) );
        if( FAILED(hr) )
            goto e_Exit;
    }  

    //---------------------------------
    // Materials
    //---------------------------------
    pModelSubset->pBoundValueTable = Sas::Host::GetRootTable()->CreateChild(); // Sas::Host::GetRootTable(); //
    CharToWString(a_Name, pModelSubset->pBoundValueTable->Name);

    bool HasMaterials = (a_dwNumMaterials > 0);//so that we can avoid some messages

    //slip in a default material if none are present
    if( a_dwNumMaterials == 0 )
    {
        a_dwNumMaterials = 1;
        a_pMaterials = Render::GetDefaultMaterial();
        a_pEffectInstances = Render::GetDefaultEffectInstance();
    }
    pModelSubset->AsMeshContainer()->NumMaterials = a_dwNumMaterials;

    pModelSubset->Materials.resize(a_dwNumMaterials);
    
    for( UINT iMaterial=0; iMaterial < a_dwNumMaterials; iMaterial++ )
    {
        pModelSubset->Materials[iMaterial] = CreateMaterial(pModelSubset, iMaterial, a_pMaterials[iMaterial], a_pEffectInstances[iMaterial]);
    }


    Render::PromoteSharedValues(pModelSubset->pBoundValueTable);

    PostCreateModelSubset( pModelSubset );

    // Copy the mesh container and return
    *a_ppMeshContainerOut = pModelSubset->AsMeshContainer();

e_Exit:
    if( FAILED( hr ))
        SAFE_DELETE( pModelSubset );

    if(deleteAdjacencies)
        SAFE_DELETE_ARRAY( a_pAdjacency );

    SAFE_RELEASE(pMesh9);

    return hr;
}


//--------------------------------------------------------------------------------------
Material* ModelLoader::CreateMaterial(ModelSubset* pModelSubset, int materialIndex, const D3DXMATERIAL& MaterialInst, const D3DXEFFECTINSTANCE& EffectInst)
{
    HRESULT hr = S_OK;

    Material* pMaterial = NULL;
    Sas::SourceInfo si;


    if(GetAlternateShader(si))
    {
        pMaterial = CreateMaterial(pModelSubset, si ); 
    }
    else
    {
        //Attempt to resolve and load the effect file
        if( NULL == EffectInst.pEffectFilename || EffectInst.pEffectFilename[0] == '\0')
        {
            DXVGetApp().OutputA( "'%s' material %d - No effect instance specified. ", pModelSubset->AsMeshContainer()->Name, materialIndex );
        }
        else
        {
            if(EffectInst.pEffectFilename[0] == '#')
            {
                WCHAR resourceName[MAX_PATH];
                StringCchPrintfW( resourceName, MAX_PATH, L"%S", EffectInst.pEffectFilename );

                si.Set(resourceName, MAKEINTRESOURCE(RT_RCDATA), NULL);
                pMaterial = CreateMaterial(pModelSubset, si);             
                if( pMaterial == NULL )
                {
                    hr = E_FAIL;
                    DXVGetApp().OutputA( "'%s' material %d - There was an error creating the effect '%S'.", pModelSubset->AsMeshContainer()->Name, materialIndex , resourceName );  
                }
            }
            else
            {
                WCHAR unresolvedPath[MAX_PATH];
                StringCchPrintfW(unresolvedPath, MAX_PATH, L"%S", EffectInst.pEffectFilename );
                
                DXV_FIND_DATA fileData;
                if(!DXVGetFileFinder().FindFile( fileData, unresolvedPath, m_ContentDirectory.c_str() ))
                {
                    DXVGetApp().OutputA( "'%s' material %d - Could not find file '%S'.", pModelSubset->AsMeshContainer()->Name, materialIndex , unresolvedPath );
                }
                else
                {
                    si.Set(fileData.FullPath);
                    pMaterial = CreateMaterial(pModelSubset, si);     
                    if( pMaterial == NULL )
                    {
                        hr = E_FAIL;
                        DXVGetApp().OutputA( "'%s' material %d - There was an error creating the effect '%S'.", pModelSubset->AsMeshContainer()->Name, materialIndex , fileData.FullPath );  
                    }
                }
            }
        }

        //If nothing has been loaded, load the default from resource
        //THIS IS ONLY SUPPOSED TO HAPPEN WHEN NO OVERRIDE/ALTERNATIVE
        if(pMaterial)
        {
            // Set the embedded values
            for( UINT iDefault=0; iDefault < EffectInst.NumDefaults; iDefault++ )
            {
                D3DXEFFECTDEFAULT& effectValue = EffectInst.pDefaults[iDefault];

                Sas::Variable* pVariable = pMaterial->Effect->GetVariable( effectValue.pParamName );
                if( NULL == pVariable )
                    continue;

                if( D3DXEDT_STRING == effectValue.Type )
                {
                    switch( pVariable->Desc().Type )
                    {
                    case Sas::SASVT_STRING:
                        V( pVariable->SetValue( Sas::String((LPSTR)effectValue.pValue) ));
                        //RECOVER
                        if(FAILED(hr))
                            hr =S_OK;
                        break;
                    case Sas::SASVT_TEXTURE:
                    case Sas::SASVT_TEXTURE1D:
                    case Sas::SASVT_TEXTURE2D:
                    case Sas::SASVT_TEXTURE3D:
                    case Sas::SASVT_TEXTURECUBE:
                        {
                            WCHAR textureFilename[MAX_PATH];
                            StringCchPrintf(textureFilename, MAX_PATH, L"%S", (LPSTR)effectValue.pValue );

                            DXV_FIND_DATA fileData;
                            if( DXVGetFileFinder().FindFile(fileData, textureFilename, m_ContentDirectory.c_str() ) )
                            {
                                V( pVariable->SetValue( Sas::String( fileData.FullPath ) ) );
                                //RECOVER
                                if(FAILED(hr))
                                    hr =S_OK;
                            }

                        }
                        break;
                    }
                }
                else if( D3DXEDT_DWORD == effectValue.Type )
                {
                    V( pVariable->SetValue( Sas::Int( *((int*)(DWORD*)effectValue.pValue) ) ));
                    //RECOVER
                    if(FAILED(hr))
                        hr =S_OK;
                }
                else if( D3DXEDT_FLOATS == effectValue.Type )
                {
                    UINT numFloats = effectValue.NumBytes / sizeof(float);
                    Sas::Array floats(Sas::SASVT_FLOAT, numFloats);

                    floats.Set( (float*)effectValue.pValue );
                    
                    V( pVariable->SetValue( floats ));
                    //RECOVER
                    if(FAILED(hr))
                        hr =S_OK;
                }
            }
        }

    }

    //Load the default material if we dont have one by this point
    if( pMaterial == NULL )
    {    
        DXVGetApp().OutputA( "Reverting to the default effect." );  

        GetDefaultShader( si );
        pMaterial = CreateMaterial(pModelSubset, si );     //si already set to default
        if(!pMaterial || !pMaterial->Effect)
        {
            DXVGetApp().OutputW( L"Failed to load default material at mesh:%s material:%d", pModelSubset->AsMeshContainer()->Name, materialIndex);
#if defined(DEBUG) | defined (_DEBUG)
            assert(false);
#endif
        }
        else
        {
            Sas::Variable* pVariable = NULL;

            pVariable = pMaterial->Effect->GetVariable( L"MaterialAmbient");
            if( NULL != pVariable )
                V( pVariable->SetValue( Sas::Vector( MaterialInst.MatD3D.Ambient ) ) );
            //RECOVER
            if(FAILED(hr))
                hr =S_OK;

            pVariable = pMaterial->Effect->GetVariable( L"MaterialDiffuse");
            if( NULL != pVariable )
                V( pVariable->SetValue( Sas::Vector( MaterialInst.MatD3D.Diffuse ) ) );
            //RECOVER
            if(FAILED(hr))
                hr =S_OK;

            pVariable = pMaterial->Effect->GetVariable( L"MaterialEmissive");
            if( NULL != pVariable )
                V( pVariable->SetValue( Sas::Vector( MaterialInst.MatD3D.Emissive ) ) );
            //RECOVER
            if(FAILED(hr))
                hr =S_OK;

            pVariable = pMaterial->Effect->GetVariable( L"MaterialSpecular");
            if( NULL != pVariable )
                V( pVariable->SetValue( Sas::Vector( MaterialInst.MatD3D.Specular ) ) );
            //RECOVER
            if(FAILED(hr))
                hr =S_OK;

            pVariable = pMaterial->Effect->GetVariable( L"MaterialPower");
            if( NULL != pVariable )
                V( pVariable->SetValue( Sas::Float( MaterialInst.MatD3D.Power ) ) );
            //RECOVER
            if(FAILED(hr))
                hr =S_OK;
        
            WCHAR textureFilename[MAX_PATH] = L"";
            StringCchPrintf(textureFilename, MAX_PATH, L"%S", MaterialInst.pTextureFilename);

            DXV_FIND_DATA fileData;
            bool IsMaterialTextured = 
                textureFilename[0] != L'\0'
                && DXVGetFileFinder().FindFile(fileData, textureFilename, m_ContentDirectory.c_str() );
            
            pVariable = pMaterial->Effect->GetVariable( L"IsMaterialTextured");
            if( NULL != pVariable )
                V( pVariable->SetValue( Sas::Bool( IsMaterialTextured ) ) );
            //RECOVER
            if(FAILED(hr))
                hr =S_OK;

            if(IsMaterialTextured)
            {
                pVariable = pMaterial->Effect->GetVariable( L"MaterialTexture");
                if( NULL != pVariable )    
                {
                    if(SUCCEEDED( hr = pVariable->SetValue( Sas::String( fileData.FullPath ) ) ) )
                    {
                        if(si.GetSourceType() == Sas::SOURCEINFO_FILE)
                            DXVGetFileWatcher().Add( si.GetFilePath() );
                    }
                }
                //RECOVER
                if(FAILED(hr))
                    hr =S_OK;
            }
        }
    }

    if(SUCCEEDED(hr) )
    {
        if(pMaterial && pMaterial->Effect)
            pMaterial->Effect->Bind( pModelSubset->pBoundValueTable );

        if(si.GetSourceType() == Sas::SOURCEINFO_FILE)
            DXVGetFileWatcher().Add( si.GetFilePath() );
    }

//e_Exit:
    if( FAILED(hr) )
    {
        SAFE_DELETE(pMaterial);
    }

    return pMaterial;
}

//--------------------------------------------------------------------------------------
bool ModelLoader::GetAlternateShader( Sas::SourceInfo& si )
{
    const std::wstring& FileFX = STATE_GET( FileFX );
    const std::wstring& FileDDS = STATE_GET( FileDDS );
    if(!FileFX.empty())
    {
        si.Set( FileFX.c_str() );
        return true;
    }
    else if(!FileDDS.empty())
    {
        assert(false); //TODO ADD DDS SUPPORT //si.Set( MAKEINTRESOURCE(IDR_DDS9FX), MAKEINTRESOURCE(RT_RCDATA), NULL);
        return true;
    }
    else 
    {
        return false;
    }

}

//--------------------------------------------------------------------------------------
HRESULT ModelLoader::DestroyFrame(LPD3DXFRAME pFrame) 
{
    SAFE_CAST_DELETE(pFrame, Transform);

    return S_OK; 
}


//--------------------------------------------------------------------------------------
HRESULT ModelLoader::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainer)
{
    SAFE_CAST_DELETE(pMeshContainer, DXVMESHCONTAINER);
    return S_OK;
}




//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
void Model::Initialize()
{
    ZeroMemory( &m_Center, sizeof(m_Center) );
    m_Radius = 0;
    m_pRoot = NULL;
    m_pAnimController = NULL;
    m_SourceInfo = Sas::SourceInfo();
}
//--------------------------------------------------------------------------------------
void Model::Destroy()
{
    if( m_SourceInfo.GetSourceType() == Sas::SOURCEINFO_RESOURCE )
    {
       DXVGetFileWatcher().Remove( m_SourceInfo.GetFilePath() );
    }

    SAFE_DELETE(m_pRoot);
    SAFE_RELEASE(m_pAnimController);

    Initialize();
}

//--------------------------------------------------------------------------------------
HRESULT Model::Load()
{
    HRESULT hr = S_OK;
    HGLOBAL hGlobal = NULL;

    Destroy();
    const std::wstring& FileX = STATE_GET( FileX );
    const std::wstring& FileFX = STATE_GET( FileFX );
    const std::wstring& FileDDS = STATE_GET( FileDDS );

    Sas::SourceInfo si;
    if( !FileX.empty() )
    {
        si.Set( FileX.c_str() );
        DXVGetApp().OutputW( L"Loading model - %s", FileX.c_str() );

        if( !FileFX.empty() )
            DXVGetApp().OutputW( L"And effect - %s", FileFX.c_str() );
    }
    else if( !FileFX.empty() )
    {
        DWORD resource = STATE_GET( PreviewShape );
        si.Set( MAKEINTRESOURCE(resource), MAKEINTRESOURCE(RT_RCDATA), NULL);
        DXVGetApp().OutputW( L"Loading effect - %s", FileFX.c_str() );
    }
    else if( !FileDDS.empty() )
    {
        assert(false); //si.Set( MAKEINTRESOURCE(IDR_CUBEX), MAKEINTRESOURCE(RT_RCDATA), NULL);
        DXVGetApp().OutputW( L"Loading Texture - %s", FileDDS.c_str() );
    }

    m_SourceInfo = si;

   
    if(m_SourceInfo.GetSourceType() == Sas::SOURCEINFO_FILE)
    {
        V( ExtractFromFile( m_SourceInfo ) );
        if( FAILED(hr) )
            goto e_Exit;

        DXVGetFileWatcher().Add( m_SourceInfo.GetFilePath() );

    }
    else if(m_SourceInfo.GetSourceType() == Sas::SOURCEINFO_RESOURCE)
    {
        HRSRC hSrc= FindResource(m_SourceInfo.GetResourceModule() , m_SourceInfo.GetResourceName(), m_SourceInfo.GetResourceType());
        if(hSrc == NULL)
        {
            hr= E_FAIL;
            goto e_Exit;
        }
        else
        {
            hGlobal = LoadResource(m_SourceInfo.GetResourceModule() , hSrc);
            if(hGlobal == NULL)
            {
                hr= E_FAIL;
                goto e_Exit;
            }            
            else
            {
                DWORD InMemorySize = SizeofResource(m_SourceInfo.GetResourceModule() , hSrc);
                void* pInMemory = LockResource(hGlobal);
                if(pInMemory == NULL)
                {
                    hr= E_FAIL;
                    goto e_Exit;
                }
                else
                {
                    V( ExtractFromMemory( pInMemory, InMemorySize ) );
                    if( FAILED(hr) )
                        goto e_Exit;
                }
            }
        }
    }


    if(m_pRoot)
    {
        m_pRoot->UpdateMatrices( NULL );
        V( m_pRoot->UpdateSkins( NULL ) );
        CalculateBounds();
    }

    DXVGetCamera().Home();

    if( FAILED(hr) ) goto e_Exit;


e_Exit:
    if(hGlobal)    FreeResource(hGlobal);

    if( FAILED(hr) )
    {
        DXVGetApp().OutputW( L"Failed to load model - %s", m_SourceInfo.GetDescription() );
        Destroy();
    }

    return hr;
}



//--------------------------------------------------------------------------------------
void Model::FrameMove( double fElapsedTime )
{
    HRESULT hr = S_OK;
    if( m_pRoot != NULL )
    {
        if( m_pAnimController != NULL )
        {
            V( m_pAnimController->AdvanceTime( fElapsedTime, NULL ) );
        }        
        
        m_pRoot->UpdateMatrices( DXVGetCamera().GetWorldMatrix() );
        V( m_pRoot->UpdateSkins( NULL ) );
    }
}

//--------------------------------------------------------------------------------------
struct DeclarationElementSize
{
    DWORD Id;
    BYTE Size;
};
//--------------------------------------------------------------------------------------
D3DVERTEXELEMENT9 DeclarationEnd = D3DDECL_END();
//--------------------------------------------------------------------------------------
DeclarationElementSize DeclarationSizeTable[MAXD3DDECLTYPE+1] =
{
    {D3DDECLTYPE_FLOAT1, 4},     
    {D3DDECLTYPE_FLOAT2, 8},    
    {D3DDECLTYPE_FLOAT3, 12},    
    {D3DDECLTYPE_FLOAT4, 16},    
    {D3DDECLTYPE_D3DCOLOR, 4},  
    {D3DDECLTYPE_UBYTE4, 4},  
    {D3DDECLTYPE_SHORT2, 4},  
    {D3DDECLTYPE_SHORT4, 8},  
    {D3DDECLTYPE_UBYTE4N, 4},  
    {D3DDECLTYPE_SHORT2N, 4}, 
    {D3DDECLTYPE_SHORT4N, 8}, 
    {D3DDECLTYPE_USHORT2N , 4}, 
    {D3DDECLTYPE_USHORT4N, 8}, 
    {D3DDECLTYPE_UDEC3 , 4}, 
    {D3DDECLTYPE_DEC3N, 4}, 
    {D3DDECLTYPE_FLOAT16_2, 4}, 
    {D3DDECLTYPE_FLOAT16_4, 8},
    {D3DDECLTYPE_UNUSED, 0},
} ;
//--------------------------------------------------------------------------------------
void DeclarationElementInsert( D3DVERTEXELEMENT9* decl, UINT before, D3DDECLTYPE type, D3DDECLUSAGE usage, BYTE index)
{
    D3DVERTEXELEMENT9 element = { 0, 0, (BYTE)type, D3DDECLMETHOD_DEFAULT, (BYTE)usage, index };

    int length = before;
    if(decl[before].Stream == 255)
    {
        D3DVERTEXELEMENT9* last =  decl+(before-1);

        element.Offset = last->Offset + (BYTE)DeclarationSizeTable[last->Type].Size;
    }
    else
    {
        element.Offset = decl[before].Offset;

        for(int i = before; i < MAX_FVF_DECL_SIZE && decl[i].Stream != 255; i++)
        {
            decl[i].Offset = decl[i].Offset + (WORD)(DeclarationSizeTable[type].Size);

            length++;
        }
    }

    assert(decl[length].Stream == 255);

    memmove(&decl[before+1], &decl[before], (length+1-before)*sizeof(D3DVERTEXELEMENT9) );


    decl[before] = element;
}

//--------------------------------------------------------------------------------------
HRESULT ModelLoader::HomogenizeMesh9( LPD3DXMESH* ppMesh, const DWORD* pAdjacency, LPD3DXSKININFO pSkinInfo )
{
    HRESULT hr = S_OK;
    LPDIRECT3DDEVICE9 pDevice=NULL;
    
    bool bGenTangentFrame = false;

    D3DVERTEXELEMENT9 decl[MAX_FVF_DECL_SIZE];


    V( (*ppMesh)->GetDevice(&pDevice) );


    V( (*ppMesh)->GetDeclaration( decl ) );

    int length = 0;
    int iTex = -1;
    int iNorm = -1;
    int iTan = -1;
    int iBi = -1;
    int iTexTooSmall = -1;
    for(int iVE= 0; iVE < MAX_FVF_DECL_SIZE && decl[iVE].Stream!=255; iVE++ )
    {
        if( decl[iVE].Usage == D3DDECLUSAGE_TEXCOORD && decl[iVE].UsageIndex == 0)
        {
            if (decl[iVE].Type != D3DDECLTYPE_FLOAT1 && decl[iVE].Type != D3DDECLTYPE_UNUSED) // need 2-element or greater values
                iTex = iVE;
            else
                iTexTooSmall = iVE;
        }
        if( decl[iVE].Usage == D3DDECLUSAGE_NORMAL && decl[iVE].UsageIndex == 0)
            iNorm = iVE;
        if( decl[iVE].Usage == D3DDECLUSAGE_TANGENT && decl[iVE].UsageIndex == 0)
            iTan = iVE;
        if( decl[iVE].Usage == D3DDECLUSAGE_BINORMAL && decl[iVE].UsageIndex == 0)
            iBi = iVE;

        length++;
    }

    if(iTex == -1 && (iBi == -1 || iTan == -1))
    {
        DXVGetApp().OutputW( L"Tangents and Binormals cannot be generated without a 2-element or greater texture coordinate at TEXCOORD0. Please use DXOps to UV-Atlas your mesh." );
    }

    if( iNorm == -1 || iBi == -1 || iTan == -1 || iTex == -1)
    {
        int extraRoom = 0;
        if(iNorm != -1)
            extraRoom++;
        if(iTan != -1)
            extraRoom++;
        if(iBi != -1)
            extraRoom++;
        if(length+extraRoom>=MAX_FVF_DECL_SIZE)
        {
            DXVGetApp().OutputW( L"Not enough room to add missing vertex element (normals, tangents, and/or binormals)." );
            hr = E_FAIL;
            goto e_Exit;
        }


        bGenTangentFrame = (iTex != -1 && (iBi == -1 || iTan == -1));


        if(iNorm == -1)
        {
            DXVGetApp().OutputW( L"Normals do not exist on mesh.  Attempting to calculate." );
            
            DeclarationElementInsert(decl, 1, D3DDECLTYPE_FLOAT3, D3DDECLUSAGE_NORMAL, 0);
            length++;
        }

        if(iTex == -1 && iTexTooSmall == -1)
        {
            DeclarationElementInsert(decl, 2, D3DDECLTYPE_FLOAT2, D3DDECLUSAGE_TEXCOORD, 0);
            length++;
        
        }

        if(iBi == -1)
        {
            DXVGetApp().OutputW( L"Binormals do not exist on mesh.  Attempting to calculate." );

            DeclarationElementInsert(decl, length, D3DDECLTYPE_FLOAT3, D3DDECLUSAGE_BINORMAL, 0);
            length++;
        }



        if(iTan == -1)
        {
            DXVGetApp().OutputW( L"Tangents do not exist on mesh.  Attempting to calculate." );

            DeclarationElementInsert(decl, length, D3DDECLTYPE_FLOAT3, D3DDECLUSAGE_TANGENT, 0);
            length++;
        }
    }

    {// Clone mesh to the new vertex format
        ID3DXMesh* pNewMesh = NULL;
        V( (*ppMesh)->CloneMesh( D3DXMESH_32BIT | D3DXMESH_VB_MANAGED | (*ppMesh)->GetOptions(), decl, pDevice, &pNewMesh ));
        SAFE_RELEASE((*ppMesh));
        (*ppMesh) = pNewMesh;
        pNewMesh = NULL;
    }

    if( FAILED(hr) ) goto e_Exit;


    bool is32bit = (((*ppMesh)->GetOptions()&D3DXMESH_32BIT) != 0);


    if(pSkinInfo)
    {
        V( pSkinInfo->SetDeclaration(decl) );
        if( FAILED(hr) ) goto e_Exit;
    }


    if(bGenTangentFrame)
    {
        ID3DXMesh* pNewMesh = NULL;
        LPD3DXBUFFER pVertexRemap = NULL;

        DWORD dwFlags = (iNorm==-1)?D3DXTANGENT_CALCULATE_NORMALS:0;

        try
        {
            // Compute tangents, which are required for normal mapping
            hr = D3DXComputeTangentFrameEx( 
                    (*ppMesh), 
                    D3DDECLUSAGE_TEXCOORD, 0, 
                    ((iTan==-1)?D3DDECLUSAGE_TANGENT:D3DX_DEFAULT), 0,
                    ((iBi==-1)?D3DDECLUSAGE_BINORMAL:D3DX_DEFAULT), 0, 
                    ((iNorm==-1)?D3DDECLUSAGE_NORMAL:D3DX_DEFAULT), 0,
                    dwFlags, 
                    pAdjacency, 
                    -1.01f, -0.01f, -1.01f, 
                    &pNewMesh, &pVertexRemap );
        }
        catch( ... )
        {
            hr = E_FAIL;
        }

        if(SUCCEEDED(hr))
        {
            if(pSkinInfo)
            {
                V( pSkinInfo->Remap( pNewMesh->GetNumVertices(), (DWORD*) pVertexRemap->GetBufferPointer() ) );
            }
            SAFE_RELEASE(pVertexRemap);
            
            SAFE_RELEASE((*ppMesh));
            (*ppMesh) = pNewMesh;
            pNewMesh = NULL;           
        }
        else
        {
            SAFE_RELEASE(pVertexRemap);
            SAFE_RELEASE(pNewMesh);

            DXVGetApp().OutputW( L"Failed first attempt to compute TangentFrame.  Attempting less accurate calculations." );

            //Create a new vertex declaration to hold all the required data
            try
            {
                hr = D3DXComputeTangentFrame((*ppMesh), dwFlags|D3DXTANGENT_GENERATE_IN_PLACE|D3DXTANGENT_ORTHOGONALIZE_FROM_U );
            }
            catch( ... )
            {
                DXVGetApp().OutputW( L"D3DXComputeTangentFrame FAILED Try using DxOps to clean the mesh." );
                hr = E_FAIL;
            }

            if( FAILED(hr) )
                goto e_Exit;    
        }
    }

    hr = (*ppMesh)->OptimizeInplace(D3DXMESHOPT_ATTRSORT, NULL, NULL, NULL, NULL );

    hr = S_OK;


e_Exit:
    if(FAILED(hr))
    {
        SAFE_RELEASE( (*ppMesh) );
    }

    SAFE_RELEASE( pDevice );
    return hr;
}


HRESULT Transform::CalculateBoundingBox( D3DXVECTOR3& minPos, D3DXVECTOR3& maxPos)
{
    HRESULT hr = S_OK;

    D3DXMATRIX Identity;
    D3DXMatrixIdentity( &Identity );

    for( Transform* pChild = GetFirstChild();
        pChild != NULL;
        pChild = pChild->GetNextSibling() )
    {
        V( pChild->CalculateBoundingBox( minPos, maxPos ) );
    }

    for( ModelSubset* pSubMod = GetModelSubset();
        pSubMod != NULL;
        pSubMod = pSubMod->GetNextModelSubset())
    {
        D3DXVECTOR3 localMin = D3DXVECTOR3(FLT_MAX,FLT_MAX,FLT_MAX);
        D3DXVECTOR3 localMax = D3DXVECTOR3(-FLT_MAX,-FLT_MAX,-FLT_MAX);

        V( pSubMod->CalculateBoundingBox( localMin, localMax ) );

        IncreaseBox( WorldMatrix, localMin, localMax, minPos, maxPos);
    }

    return hr;
}

void Model::CalculateBounds()
{
    m_Min = D3DXVECTOR3(FLT_MAX,FLT_MAX,FLT_MAX);
    m_Max = D3DXVECTOR3(-FLT_MAX,-FLT_MAX,-FLT_MAX);

    m_pRoot->CalculateBoundingBox( m_Min, m_Max );

    m_Center = (m_Min + m_Max) / 2;

    D3DXVECTOR3 diff = m_Max - m_Min;
    m_Radius = D3DXVec3Length( &diff ) / 2.0f;


}

void Transform::IncreaseBox(const D3DXMATRIX& WorldMatrix, const D3DXVECTOR3& pushMin, const D3DXVECTOR3& pushMax, D3DXVECTOR3& minVec, D3DXVECTOR3& maxVec)
{
    D3DXVECTOR3 localPos[8] = 
    {
        D3DXVECTOR3(pushMin.x, pushMin.y, pushMin.z),
        D3DXVECTOR3(pushMin.x, pushMax.y, pushMin.z),
        D3DXVECTOR3(pushMax.x, pushMax.y, pushMin.z),
        D3DXVECTOR3(pushMax.x, pushMin.y, pushMin.z),
        D3DXVECTOR3(pushMin.x, pushMin.y, pushMax.z),
        D3DXVECTOR3(pushMin.x, pushMax.y, pushMax.z),
        D3DXVECTOR3(pushMax.x, pushMax.y, pushMax.z),
        D3DXVECTOR3(pushMax.x, pushMin.y, pushMax.z),
    };

    for( int i = 0; i < 8; i++ )
    {
        D3DXVECTOR4 worldPos;
        D3DXVec3Transform(&worldPos, localPos+i, &WorldMatrix);
        IncreaseBox( *((D3DXVECTOR3*)&worldPos), minVec, maxVec);
    }
}




void Transform::IncreaseBox(const D3DXVECTOR3& pushVec, D3DXVECTOR3& minVec, D3DXVECTOR3& maxVec)
{
        if(pushVec.x < minVec.x)
            minVec.x = pushVec.x;
        if(pushVec.y < minVec.y)
            minVec.y = pushVec.y;
        if(pushVec.z < minVec.z)
            minVec.z = pushVec.z;

        if(pushVec.x > maxVec.x)
            maxVec.x = pushVec.x;
        if(pushVec.y > maxVec.y)
            maxVec.y = pushVec.y;
        if(pushVec.z > maxVec.z)
            maxVec.z = pushVec.z;
}