#pragma once
#include "D3D10.h"
#include "D3DX10.h"
#include "macros.h"

class ShowShared10
{
public:
    FRIEND_SINGLETON_H( ShowShared10 );
    
    ~ShowShared10()
    {
        Shutdown();
    }

    void Startup();
    void Shutdown();

    HRESULT CreateInputLayout( ID3DX10Mesh* pMesh10, ID3D10InputLayout** layout);

    void Move();

    void DrawBounds();//accesses the bounds info directly from the Model10
    void DrawAdjacencies(const D3DXMATRIX& WorldMatrix, ID3DX10Mesh* pMesh10, UINT AttribCount, ID3D10InputLayout* layout);
    void DrawCreases(const D3DXMATRIX& WorldMatrix, ID3DX10Mesh* pMesh10, UINT AttribCount, ID3D10InputLayout* layout);
    void DrawEdges(const D3DXMATRIX& WorldMatrix, ID3DX10Mesh* pMesh10, UINT AttribCount, ID3D10InputLayout* layout);
    void DrawNormals(const D3DXMATRIX& WorldMatrix, ID3DX10Mesh* pMesh10, UINT AttribCount, ID3D10InputLayout* layout);
    void DrawBinormals(const D3DXMATRIX& WorldMatrix, ID3DX10Mesh* pMesh10, UINT AttribCount, ID3D10InputLayout* layout);
    void DrawTangents(const D3DXMATRIX& WorldMatrix, ID3DX10Mesh* pMesh10, UINT AttribCount, ID3D10InputLayout* layout);
    void DrawPoints(const D3DXMATRIX& WorldMatrix, ID3DX10Mesh* pMesh10, UINT AttribCount, ID3D10InputLayout* layout);
private:
    ShowShared10()
    {
        Effect = NULL;
        BoundsLayout = NULL;
        BoundsBuffer = NULL;

        Min = NULL;
        Max = NULL;

        World = NULL;
        View = NULL;
        Projection = NULL;
        
        LineSize = NULL;

        Adjacencies = NULL;
        Bounds = NULL;
        Creases = NULL;
        Edges = NULL;
        Normals = NULL;
        Binormals = NULL;
        Tangents = NULL;
        Points = NULL;    
    }



    ID3D10Effect* Effect;
    ID3D10InputLayout* BoundsLayout;
    ID3D10Buffer* BoundsBuffer;

    ID3D10EffectVectorVariable* Min;
    ID3D10EffectVectorVariable* Max;

    ID3D10EffectMatrixVariable* World;
    ID3D10EffectMatrixVariable* View;
    ID3D10EffectMatrixVariable* Projection;
    
    ID3D10EffectScalarVariable* LineSize;

    ID3D10EffectPass* Adjacencies;
    ID3D10EffectPass* Bounds;
    ID3D10EffectPass* Creases;
    ID3D10EffectPass* Edges;
    ID3D10EffectPass* Normals;
    ID3D10EffectPass* Binormals;
    ID3D10EffectPass* Tangents;
    ID3D10EffectPass* Points;

    static D3D10_INPUT_ELEMENT_DESC DECL[];

};

DECLARE_SINGLETON_H( ShowShared10 );