//--------------------------------------------------------------------------------------
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include <sas\sas.fxh>

int GlobalParameter : SasGlobal
<
    int3 SasVersion = {1, 1, 0};
    
    string SasEffectDescription = "This is DirectX Viewer's default effect for viewing meshes without embedded effect instances";
    string SasEffectCompany = "Microsoft Corporation";
>;


//--------------------------------------------------------------------------------------
// Variables
//--------------------------------------------------------------------------------------
SasAmbientLight AmbientLight <string SasBindAddress = "Sas.AmbientLight[0]";>;
SasDirectionalLight DirectionalLight <string SasBindAddress = "Sas.DirectionalLight[0]";>;

float4x4 World <string SasBindAddress = "Sas.Skeleton.MeshToJointToWorld[0]";>;         
float4x4 View <string SasBindAddress = "Sas.Camera.WorldToView";>;
float4x4 Projection <string SasBindAddress = "Sas.Camera.Projection";>;


float4 MaterialDiffuse <string SasBindAddress = "Microsoft.Material.Diffuse";>;
float4 MaterialAmbient <string SasBindAddress = "Microsoft.Material.Ambient";>;
float4 MaterialEmissive <string SasBindAddress = "Microsoft.Material.Emissive";>;
texture MaterialTexture <string SasBindAddress = "Microsoft.Material.Texture";>;
bool IsMaterialTextured <string SasBindAddress = "Microsoft.Material.IsTextured";>;


sampler MaterialTextureSampler = sampler_state
{ 
    Texture = (MaterialTexture);
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};


//--------------------------------------------------------------------------------------
// VertexShader I/O
//--------------------------------------------------------------------------------------
struct VSInput
{
    float4 Position : POSITION; 
    float3 Normal : NORMAL;
    float2 TexCoords : TEXCOORD0;
};

struct VSOutput
{
    float4 TransformedPosition : POSITION;
    float3 Diffuse : COLOR0;    
    float2 TexCoords : TEXCOORD0; 
};


//--------------------------------------------------------------------------------------
// This shader computes standard transform and lighting
//--------------------------------------------------------------------------------------
VSOutput VSStandard( VSInput input )
{
    VSOutput output;
    
    float4 worldPosition = mul(input.Position, (float4x4)World);
    float3 worldNormal = mul(input.Normal, (float3x3)World);
    
    output.TransformedPosition = mul(worldPosition, mul(View, Projection));
    output.TexCoords = input.TexCoords;
    output.Diffuse = MaterialEmissive + (MaterialAmbient * AmbientLight.Color); 
    output.Diffuse += (MaterialDiffuse * DirectionalLight.Color *  max(0, dot(worldNormal, -DirectionalLight.Direction)));
    
    return output;    
}



//--------------------------------------------------------------------------------------
// PixelShader I/O
//--------------------------------------------------------------------------------------
struct PSOutput
{
    float4 Color : COLOR0;  // Pixel color    
};


PSOutput PS( VSOutput input ) 
{ 
    PSOutput output;

    if(IsMaterialTextured)
        input.Diffuse *= tex2D(MaterialTextureSampler, input.TexCoords); 
   
    output.Color = float4(input.Diffuse, 1);
    
    return output;
}


//--------------------------------------------------------------------------------------
// Techniques
//--------------------------------------------------------------------------------------
technique Standard
{
    pass P0
    {   
        //CullMode = None;
        
        VertexShader = compile vs_1_1 VSStandard();
        PixelShader  = compile ps_1_1 PS(); 
    }
}



