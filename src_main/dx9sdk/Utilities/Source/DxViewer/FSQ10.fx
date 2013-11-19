//--------------------------------------------------------------------------------------
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

Texture2D Image;

SamplerState SampleLinear
{ 
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};



DepthStencilState Depth
{
    DepthEnable = FALSE;
    DepthWriteMask = ALL;
};

RasterizerState Culling
{
	CullMode = NONE;
};

BlendState Blending
{
    AlphaToCoverageEnable = FALSE;
    BlendEnable[0] = FALSE;
    RenderTargetWriteMask[0] = 0x0F;
};


//--------------------------------------------------------------------------------------
// VertexShader I/O
//--------------------------------------------------------------------------------------
struct VSInput
{
    float3 Position : SV_Position;
    //float2 TexCoord : TEXCOORD;
};

struct VSOutput
{
    float4 Position  : SV_Position;
    float2 TexCoord : TEXCOORD; 
};


//--------------------------------------------------------------------------------------
// This shader computes standard transform and lighting
//--------------------------------------------------------------------------------------
VSOutput VS( VSInput input )
{
    VSOutput output;


    output.Position = float4(input.Position, 1.0f);
    output.TexCoord = (input.Position.xy+1.0)/2.0f;
    output.TexCoord.y = 1.0f-output.TexCoord.y;

    return output;    
}



//--------------------------------------------------------------------------------------
// PixelShader I/O
//--------------------------------------------------------------------------------------


float4 PS( VSOutput input ) : SV_Target
{ 
	return float4( Image.Sample( SampleLinear, input.TexCoord ).rgb , 1.0f); 
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



