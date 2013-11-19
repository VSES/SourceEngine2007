//--------------------------------------------------------------------------------------
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

int GlobalParameter : SasGlobal
<
    int3 SasVersion = {1, 1, 0};
	
	string SasEffectDescription = "This is DirectX Viewer's vector rendering effect.  It adds the normal to the position so it expects a linelist with the first position with zero'ed vectors";
	string SasEffectCompany = "Microsoft Corporation";
>;

float4x4 World <string SasBindAddress = "Sas.Skeleton.MeshToJointToWorld[0]";>;			
float4x4 View <string SasBindAddress = "Sas.Camera.WorldToView";>;
float4x4 Projection <string SasBindAddress = "Sas.Camera.Projection";>;
float Scale = 0.25;
float3 Color = float3(0,0,0);

#define D3DDECLUSAGE_NORMAL 3
#define D3DDECLUSAGE_TANGENT 6
#define D3DDECLUSAGE_BINORMAL 7

int RenderUsage = D3DDECLUSAGE_NORMAL;

//--------------------------------------------------------------------------------------
// VertexShader I/O
//--------------------------------------------------------------------------------------
struct VSInput
{
	float3 Position : POSITION; 
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float3 Binormal : BINORMAL;
};

struct VSOutput
{
	float4 Position : POSITION;
};

VSOutput VS( VSInput input )
{
    VSOutput output;
    
    float3 worldPosition = mul(float4(input.Position, 1.0f), World);
    
    float3 worldNormal = float3(0,0,0);
    if(RenderUsage == D3DDECLUSAGE_NORMAL)
    {
      worldNormal= mul(input.Normal, (float3x3)World);
    }

	if(RenderUsage == D3DDECLUSAGE_TANGENT)
	{
		worldNormal= mul(input.Tangent, (float3x3)World);
	}

	if(RenderUsage == D3DDECLUSAGE_BINORMAL)
	{
		//worldNormal= mul(cross(input.Normal, input.Tangent), (float3x3)World);//input.Binormal
		worldNormal= mul(input.Binormal, (float3x3)World);
	}

    worldPosition += worldNormal*Scale;
  
    
    output.Position = mul(mul(float4(worldPosition, 1.0f), View), Projection);
  
          
    output.Position.z -= 1.0e-4;
    return output;    
}

//--------------------------------------------------------------------------------------
// PixelShader I/O
//--------------------------------------------------------------------------------------
struct PSOutput
{
    float4 Color : COLOR0;  // Pixel color    
};


PSOutput PS( ) 
{ 
    PSOutput output;
    
	output.Color = float4(Color,1);
	
    if(RenderUsage == D3DDECLUSAGE_NORMAL)
    {
		output.Color = float4(1,0,0,1);
    }
    
    if(RenderUsage == D3DDECLUSAGE_TANGENT)
	{
		output.Color = float4(0,0,1,1);
	}
	
	if(RenderUsage == D3DDECLUSAGE_BINORMAL)
	{
		output.Color = float4(0,1,0,1);
	}

    return output;
}


//--------------------------------------------------------------------------------------
// Techniques
//--------------------------------------------------------------------------------------
technique Standard
{
    pass P0
    {   
		CullMode=None;
 		ZWriteEnable=false;
 		        
        VertexShader = compile vs_2_0 VS();
        PixelShader  = compile ps_2_0 PS(); 
    }
}


