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

float4x4 World <string SasBindAddress = "Sas.Skeleton.MeshToJointToWorld[0]";> = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};  
float4x4 View <string SasBindAddress = "Sas.Camera.WorldToView";> = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};  
float4x4 Projection <string SasBindAddress = "Sas.Camera.Projection";> = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};  


float4 MaterialDiffuse <string SasBindAddress = "Microsoft.Material.Diffuse";>;
float4 MaterialAmbient <string SasBindAddress = "Microsoft.Material.Ambient";>;
float4 MaterialEmissive <string SasBindAddress = "Microsoft.Material.Emissive";>;


bool IsMaterialTextured <string SasBindAddress = "Microsoft.Material.IsTextured";>;
Texture2D MaterialTexture <string SasBindAddress = "Microsoft.Material.Texture";>;

SamplerState SampleLinear
{ 
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};


DepthStencilState Depth
{
    DepthEnable = TRUE;
    DepthWriteMask = ALL;
};

RasterizerState Culling
{
	FillMode = SOLID;
	CullMode = BACK;
	FrontCounterClockwise = false;
	
};

BlendState Blending
{
    AlphaToCoverageEnable = FALSE;
    BlendEnable[0] = FALSE;
    RenderTargetWriteMask[0] = 0xF;
};

//--------------------------------------------------------------------------------------
// VertexShader I/O
//--------------------------------------------------------------------------------------
struct VSInput
{
    float4 Position : SV_Position;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD;
};

struct VSOutput
{
    float4 Position : SV_Position;
    float3 Diffuse : COLOR0;    
    float2 TexCoord : TEXCOORD; 
};


//--------------------------------------------------------------------------------------
// This shader computes standard transform and lighting
//--------------------------------------------------------------------------------------
VSOutput VS( VSInput input )
{
    VSOutput output;
    
    float4 worldPosition = mul(input.Position, (float4x4)World);
    float3 worldNormal = mul(input.Normal, (float3x3)World);
    
    output.Position = mul(worldPosition, mul(View, Projection));
    //output.Position = float4( input.Position.xyz, 1.0f);

    output.TexCoord = input.TexCoord;
    output.Diffuse = MaterialEmissive + (MaterialAmbient * AmbientLight.Color); 
    output.Diffuse += (MaterialDiffuse * DirectionalLight.Color *  max(0, dot(worldNormal, -DirectionalLight.Direction)));
    
    return output;    
}



//--------------------------------------------------------------------------------------
// PixelShader I/O
//--------------------------------------------------------------------------------------

float4 PS( VSOutput input ) : SV_Target
{ 

    float3 output = input.Diffuse;

    if(IsMaterialTextured)
    {
        output *= MaterialTexture.Sample( SampleLinear, input.TexCoord ).rgb; 
    }   

   
    return float4(output, 1);

    //return float4(1, 0, 0, 1);
}


//--------------------------------------------------------------------------------------
// Techniques
//--------------------------------------------------------------------------------------
technique10 Standard
{
    pass P0
    {   

	SetBlendState( Blending, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
        SetDepthStencilState( Depth, 0 );
        SetRasterizerState( Culling );
 
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
        
   }
}



