#pragma once
#include "D3D9.H"
#include "D3DX9.H"
#include <vector>
#include "sas.h"

struct DXVMESHCONTAINER;
struct Transform;
struct ModelSubset;


struct DXVMESHCONTAINER : public D3DXMESHCONTAINER
{
    ModelSubset* Owner;

    DXVMESHCONTAINER(ModelSubset* owner);
    ~DXVMESHCONTAINER();
};

struct Material
{
    Material() { Effect = NULL; }
    virtual ~Material();

    Sas::Effect* Effect;

};

struct ModelSubset
{
    DXVMESHCONTAINER* Container;

    static ModelSubset* CastMeshContainerToModelSubset( const D3DXMESHCONTAINER* a );
    static DXVMESHCONTAINER* CastModelSubsetToMeshContainer( const ModelSubset* a );

    Sas::BoundValueTable* pBoundValueTable;
    std::vector< Material* > Materials;
    
    ModelSubset();
    virtual ~ModelSubset();

    HRESULT UpdateSkins(Transform& root, const D3DXMATRIX& worldMatrix);

    virtual bool HasSkin() = 0;
    virtual UINT GetBoneCount() = 0;
    virtual LPCSTR GetBoneName(UINT index) = 0;
    virtual const D3DXMATRIX* GetBoneOffset(UINT index) = 0;
    virtual HRESULT UpdateSkins(D3DXMATRIX* BonePalette, D3DXMATRIX* BoneInverseTransposePalette) = 0; //local not world
    virtual HRESULT CalculateBoundingBox( D3DXVECTOR3& minVec, D3DXVECTOR3& maxVec) = 0;

    ModelSubset* GetNextModelSubset();
    DXVMESHCONTAINER* AsMeshContainer();
};

struct Transform : public D3DXFRAME
{
    Transform();
    ~Transform();

    D3DXMATRIX        WorldMatrix;

    HRESULT CalculateBoundingBox( D3DXVECTOR3& minVec, D3DXVECTOR3& maxVec);
    void    UpdateMatrices( const D3DXMATRIX* pParentMatrix );
    HRESULT UpdateSkins(Transform* root = NULL);

    Transform* GetFirstChild();
    Transform* GetNextSibling();
    ModelSubset* GetModelSubset();
    D3DXFRAME* AsFrame();

protected:
    void IncreaseBox(const D3DXMATRIX& WorldMatrix, const D3DXVECTOR3& pushMin, const D3DXVECTOR3& pushMax, D3DXVECTOR3& minVec, D3DXVECTOR3& maxVec);
    void IncreaseBox(const D3DXVECTOR3& pushVec, D3DXVECTOR3& minVec, D3DXVECTOR3& maxVec);


};



class ModelLoader: public ID3DXAllocateHierarchy
{
public:
    STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame);
    STDMETHOD(CreateMeshContainer)(THIS_ 
        LPCSTR Name, 
        CONST D3DXMESHDATA *pMeshData,
        CONST D3DXMATERIAL *pMaterials, 
        CONST D3DXEFFECTINSTANCE *pEffectInstances, 
        DWORD NumMaterials, 
        CONST DWORD *pAdjacency, 
        LPD3DXSKININFO pSkinInfo, 
        LPD3DXMESHCONTAINER *ppNewMeshContainer);
    STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrame);
    STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerBase);


protected:  

    virtual ModelSubset* CreateModelSubset() = 0;
    virtual bool GetAlternateShader( Sas::SourceInfo& si );
    virtual void GetDefaultShader( Sas::SourceInfo& si ) = 0;
    virtual Material* CreateMaterial(ModelSubset* pModelSubset, Sas::SourceInfo& si) = 0;
    virtual HRESULT SetMesh(ModelSubset* pModelSubset, LPD3DXMESH pMesh9, const DWORD* pAdjacency, bool skinned) = 0;
    virtual HRESULT SetSkin(ModelSubset* pModelSubset, LPD3DXSKININFO pSkin9) = 0;
    virtual void PostCreateModelSubset( ModelSubset* pModelSubset ){};


    static HRESULT HomogenizeMesh9(LPD3DXMESH* ppMesh, const DWORD* pAdjacency, LPD3DXSKININFO pSkinInfo);
    static char* DuplicateString( LPCSTR Name );
    std::wstring m_ContentDirectory;
protected:
    Transform* CreateTransform() { return new Transform; }
    Material* CreateMaterial(ModelSubset* pModelSubset, int materialIndex, const D3DXMATERIAL& MaterialInst, const D3DXEFFECTINSTANCE& EffectInst);
};

class Model
{
public:
    Model() { Initialize(); } 
    virtual ~Model(){ Destroy(); }


    virtual HRESULT Load();
    virtual void Clear() { Destroy(); }
   
    const Sas::SourceInfo& GetSourceInfo() const { return m_SourceInfo; }
       
    void FrameMove( double fElapsedTime );
    //void Render(LPD3DXEFFECT pOverrideEffect);

    Transform* GetRoot() { return m_pRoot; }

    const D3DXVECTOR3* GetCenter() const { return &m_Center; }
    float GetRadius() const { return m_Radius; }
    const D3DXVECTOR3* GetMin() const { return &m_Min; }
    const D3DXVECTOR3* GetMax() const { return &m_Max; }

protected:
    virtual void Initialize();
    virtual void Destroy();

    virtual HRESULT ExtractFromMemory( void* pInMemory, DWORD InMemorySize) = 0;
    virtual HRESULT ExtractFromFile( const Sas::SourceInfo& sf ) = 0;
    void CalculateBounds();
    
    Transform* m_pRoot;
    LPD3DXANIMATIONCONTROLLER m_pAnimController;


    float m_Radius;
    D3DXVECTOR3 m_Center;      
    D3DXVECTOR3 m_Min;      
    D3DXVECTOR3 m_Max;      
    

    Sas::SourceInfo m_SourceInfo;

};
