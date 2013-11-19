//--------------------------------------------------------------------------------------
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

int GlobalParameter : SasGlobal
<
    int3 SasVersion = {1, 1, 0};
	
	string SasEffectDescription = "This is DirectX Viewer's line rendering effect";
	string SasEffectCompany = "Microsoft Corporation";
>;

float4x4 World <string SasBindAddress = "Sas.Skeleton.MeshToJointToWorld[0]";>;			
float4x4 View <string SasBindAddress = "Sas.Camera.WorldToView";>;
float4x4 Projection <string SasBindAddress = "Sas.Camera.Projection";>;
float Scale = 0.25;
float3 Color = float3(0,0,0);

//--------------------------------------------------------------------------------------
// VertexShader I/O
//--------------------------------------------------------------------------------------
struct VSInput
{
	float3 Position : POSITION; 
};

struct VSOutput
{
	float4 Position : POSITION;
	float Size : PSIZE;
};

VSOutput VS( VSInput input )
{
    VSOutput output;
    
    float4 worldPosition = mul(float4(input.Position, 1.0f), World);
    float4 viewPosition = mul(worldPosition, View);
    output.Position = mul(viewPosition, Projection);
              
    output.Position.z -= 1.0e-4;   
    
    output.Size = 4.0f; 

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
    
	output.Color = float4(Color,1.0f);

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
        PointScaleEnable=false;
        VertexShader = compile vs_2_0 VS();
        PixelShader  = compile ps_2_0 PS(); 
    }
}


