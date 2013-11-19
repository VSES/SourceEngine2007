#pragma once
#include "D3D10.h"
#include "D3DX10.h"
#include <vector>
#include <list>
#include "sas.h"
#include "macros.h"
#include "Model.h"

struct Define;

struct Pass10
{
    Pass10() 
    { 
        Layout = NULL; 
    }

    ~Pass10() 
    { 
        SAFE_RELEASE(Layout); 
    }

    ID3D10InputLayout* Layout;
};

struct Technique10
{
    ~Technique10() 
    { 
        Passes.resize(0); 
    }

    std::vector<Pass10> Passes;
};

struct Material10 : public Material
{
    virtual ~Material10() 
    { 
        Techniques.resize(0); 

        Material::~Material(); 
    }

    std::vector<Technique10> Techniques;
};

//--------------------------------------------------------------------------------------
// Name: struct ModelSubset10
// Desc: Structure derived from D3DXMESHCONTAINER so we can add some app-specific
//       info that will be stored with each mesh
//--------------------------------------------------------------------------------------
struct ModelSubset10 : public ModelSubset //dummy parent
{
    DWORD                    nVertexStride;
    ID3DX10Mesh*            pMesh10;
    ID3DX10Mesh*            pUnskinnedMesh;

    ID3DX10SkinInfo*        pSkinInfo10;
    std::vector<std::string> SkinBoneNames;
    std::vector<D3DXMATRIX> SkinBoneOffsets;
    std::vector<D3DX10_SKINNING_CHANNEL> SkinChannels;

    ID3D10InputLayout* ShowLayout;
    

    ModelSubset10();
    virtual ~ModelSubset10();

    virtual bool HasSkin() { return pSkinInfo10 != NULL; }
    virtual UINT GetBoneCount() { return pSkinInfo10->GetNumBones(); }
    virtual LPCSTR GetBoneName(UINT index)  { return SkinBoneNames[index].c_str(); }
    virtual const D3DXMATRIX* GetBoneOffset(UINT index)  { return &SkinBoneOffsets[index]; }
    
    virtual HRESULT UpdateSkins(D3DXMATRIX* BonePalette, D3DXMATRIX* BoneInverseTransposePalette);
    virtual HRESULT CalculateBoundingBox( D3DXVECTOR3& minVec, D3DXVECTOR3& maxVec);


};


//--------------------------------------------------------------------------------------
// Name: class ModelLoader10
// Desc: Custom version of ID3DXAllocateHierarchy with custom methods to create
//       frames and meshcontainers.
//--------------------------------------------------------------------------------------
class ModelLoader10 : public ModelLoader
{
public:
    FRIEND_SINGLETON_DIR_H( ModelLoader10 );
protected:  

    virtual ModelSubset* CreateModelSubset(){ return new ModelSubset10; }
    virtual void GetDefaultShader( Sas::SourceInfo& si );
    virtual Material* CreateMaterial(ModelSubset* pModelSubset, Sas::SourceInfo& si);
    virtual HRESULT SetMesh(ModelSubset* pModelSubset, LPD3DXMESH pMesh9, const DWORD* pAdjacency, bool skinned);
    virtual HRESULT SetSkin(ModelSubset* pModelSubset, LPD3DXSKININFO pSkin9);
    virtual void PostCreateModelSubset( ModelSubset* pModelSubset );

    void MakeMacroArray( const std::list<Define>& defineList, std::vector<D3D10_SHADER_MACRO>& macroArray );

    static HRESULT ConvertMesh9ToMesh10( LPD3DXMESH pMesh9, const DWORD* pAdjacency, ID3DX10Mesh*& pMesh10);
    static HRESULT ConvertSkin9ToSkin10(UINT numVertices, LPD3DXSKININFO pSkin9, ID3DX10SkinInfo*& pSkin10, std::vector<std::string>& Skin10_BoneNames, std::vector<D3DXMATRIX>& Skin10_BoneOffsets, std::vector<D3DX10_SKINNING_CHANNEL>& Skin10_Channels);
};



//--------------------------------------------------------------------------------------
// Name: class Model10
//--------------------------------------------------------------------------------------
class Model10 : public Model
{
public:
    FRIEND_SINGLETON_H( Model10 );

    Model10() : Model() { NullRef = NULL; }
 
    HRESULT Load();

protected:
    IDirect3DDevice9* NullRef; //created and killed during Load


    virtual HRESULT ExtractFromMemory( void* pInMemory, DWORD InMemorySize);
    virtual HRESULT ExtractFromFile( const Sas::SourceInfo& sf );
};

//--------------------------------------------------------------------------------------
DECLARE_SINGLETON_DIR_H( ModelLoader10 );
DECLARE_SINGLETON_H( Model10 );