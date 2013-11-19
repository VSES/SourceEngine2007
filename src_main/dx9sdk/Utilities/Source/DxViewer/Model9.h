#pragma once

#include "Model.h"
#include "macros.h"

#include "ShowNormals9.h"
#include "ShowPoints9.h"
#include "ShowEdges9.h"
#include "ShowCreases9.h"
#include "ShowAdjacencies9.h"
#include "ShowBounds9.h"
//--------------------------------------------------------------------------------------
// Name: struct ModelSubset9
// Desc: Structure derived from D3DXMESHCONTAINER so we can add some app-specific
//       info that will be stored with each mesh
//--------------------------------------------------------------------------------------
struct ModelSubset9: public ModelSubset
{
    LPD3DXMESH pUnskinnedMesh;
    
    // SkinMesh info      
    CShowNormals9         ShowNormals;
    CShowAdjacencies9     ShowAdjacencies;
    CShowEdges9           ShowEdges;
    CShowCreases9         ShowCreases;
    CShowPoints9          ShowPoints;

    ModelSubset9() : ModelSubset()
    {
        pUnskinnedMesh = NULL;
    }

    virtual ~ModelSubset9();

    virtual bool HasSkin() { return AsMeshContainer()->pSkinInfo != NULL; }
    virtual UINT GetBoneCount() { return AsMeshContainer()->pSkinInfo->GetNumBones(); }
    virtual LPCSTR GetBoneName(UINT index)  { return AsMeshContainer()->pSkinInfo->GetBoneName(index); }
    virtual const D3DXMATRIX* GetBoneOffset(UINT index)  { return AsMeshContainer()->pSkinInfo->GetBoneOffsetMatrix(index); }
    
    virtual HRESULT UpdateSkins(D3DXMATRIX* BonePalette, D3DXMATRIX* BoneInverseTransposePalette);
    virtual HRESULT CalculateBoundingBox( D3DXVECTOR3& minVec, D3DXVECTOR3& maxVec);

};

//--------------------------------------------------------------------------------------
// Name: class ModelLoader9
// Desc: Custom version of ID3DXAllocateHierarchy with custom methods to create
//       frames and meshcontainers.
//--------------------------------------------------------------------------------------
class ModelLoader9: public ModelLoader
{
public:
    FRIEND_SINGLETON_DIR_H( ModelLoader9 );
protected:  
    virtual ModelSubset* CreateModelSubset(){ return new ModelSubset9; }
    virtual void GetDefaultShader( Sas::SourceInfo& si );
    virtual Material* CreateMaterial(ModelSubset* pModelSubset, Sas::SourceInfo& si);
    virtual HRESULT SetMesh(ModelSubset* pModelSubset, LPD3DXMESH pMesh9, const DWORD* pAdjacency, bool skinned);
    virtual HRESULT SetSkin(ModelSubset* pModelSubset, LPD3DXSKININFO pSkin9);
    virtual void PostCreateModelSubset( ModelSubset* pModelSubset );

    void MakeMacroArray( const std::list<Define>& defineList, std::vector<D3DXMACRO>& macroArray );

};

//--------------------------------------------------------------------------------------
class Model9 : public Model
{
public:
    FRIEND_SINGLETON_H( Model9 );

    Model9() : Model() { } 

    HRESULT Load();
    
    HRESULT OnD3D9DeviceReset( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc );
    void    OnD3D9DeviceLost();
    void    OnD3D9DeviceDestroyed();

    CShowBounds9          ShowBounds;


protected:
    virtual void Destroy();
    virtual HRESULT ExtractFromMemory( void* pInMemory, DWORD InMemorySize);
    virtual HRESULT ExtractFromFile( const Sas::SourceInfo& sf );
};

//--------------------------------------------------------------------------------------

DECLARE_SINGLETON_DIR_H( ModelLoader9 );
DECLARE_SINGLETON_H( Model9 );