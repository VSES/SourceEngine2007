//--------------------------------------------------------------------------------------
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Variables
//--------------------------------------------------------------------------------------

float4x4 World <string SasBindAddress = "Sas.Skeleton.MeshToJointToWorld[0]";> = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};  
float4x4 View <string SasBindAddress = "Sas.Camera.WorldToView";> = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};  
float4x4 Projection <string SasBindAddress = "Sas.Camera.Projection";> = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};  

float3 MinPosition = {0,0,0};
float3 MaxPosition = {0,0,0};

float3 AdjacencyColor = {0.5f,0.5f,1.0f};
float3 BoundColor = {1.0f,1.0f,1.0f};
float3 CreaseColor = {1.0f,0.5f,0.0f};
float3 EdgeColor = {1.0f,0.0f,1.0f};
float3 NormalColor = {1.0f,0.0f,0.0f};
float3 BinormalColor = {0.0f,1.0f,0.0f};
float3 TangentColor = {0.0f,0.0f,1.0f};
float3 PointColor = {1.0f,1.0f,0.0f};

float LineSize = 0.05f;
float PointSize = 0.05f;
float Bias = 1.0e-3f;
DepthStencilState Depth
{
    DepthEnable = TRUE;
    DepthFunc = 4;//LESSEQUAL
    DepthWriteMask = ALL;
};

RasterizerState Culling
{
	FillMode = SOLID;
	CullMode = NONE;
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
    float3 Position : SV_Position;
    float3 Normal : NORMAL;
    float3 Binormal : BINORMAL;
    float3 Tangent : TANGENT;

};

struct VSOutput
{
    float4 Position : SV_Position;
    float3 Normal : NORMAL;
    float3 Binormal : BINORMAL;
    float3 Tangent : TANGENT;
    uint VertexId : VERTEXID;
};

struct GSOutput
{
    float4 Position : SV_Position;
    float3 Color : COLOR;
};

//--------------------------------------------------------------------------------------
// This shader computes standard transform and lighting
//--------------------------------------------------------------------------------------
VSOutput VS( VSInput input, uint VertexId : SV_VERTEXID  )
{
    VSOutput output;
     
    float4x4 WorldView = mul(World, View);
    
    output.Position = mul( float4(input.Position.xyz, 1.0f) , WorldView);
    output.Position.z -= Bias; //built in z bias  
    output.Normal =  normalize( mul( normalize(input.Normal), (float3x3)WorldView ) );
    output.Binormal = normalize( mul( normalize(input.Binormal), (float3x3)WorldView ) );
    output.Tangent = normalize( mul( normalize(input.Tangent), (float3x3)WorldView ) );
	output.VertexId = VertexId;
	
    return output;    
}

//--------------------------------------------------------------------------------------
// GeometryShader I/O for Vectors
//--------------------------------------------------------------------------------------

void GSVectorsPerVertex( VSOutput In, inout LineStream<GSOutput> outputStream, uniform bool showNormals, uniform bool showBinormals, uniform bool showTangents )
{
    GSOutput BasePoint;  
    GSOutput EndPoint;  
    BasePoint.Position = mul(In.Position, Projection);
        
	if(showNormals)
	{   
		BasePoint.Color = NormalColor;
		EndPoint.Color = NormalColor;
	    
		EndPoint.Position = mul( ( In.Position + float4(In.Normal.xyz * LineSize, 0.0f) ) , Projection);
		outputStream.Append(BasePoint);
		outputStream.Append(EndPoint);
		outputStream.RestartStrip();
	}  
	
	if(showBinormals)
	{   
		BasePoint.Color = BinormalColor;
		EndPoint.Color = BinormalColor;
	    
		EndPoint.Position = mul( ( In.Position + float4(In.Binormal.xyz * LineSize, 0.0f) ) , Projection);
		outputStream.Append(BasePoint);
		outputStream.Append(EndPoint);
		outputStream.RestartStrip();
	}  
	
	if(showTangents)
	{   
		BasePoint.Color = TangentColor;
		EndPoint.Color = TangentColor;
	    
		EndPoint.Position = mul( ( In.Position + float4(In.Tangent.xyz * LineSize, 0.0f) ) , Projection);
		outputStream.Append(BasePoint);
		outputStream.Append(EndPoint);
		outputStream.RestartStrip();
	}  
}

[maxvertexcount(6)]
void GSVectors( triangle VSOutput In[3], inout LineStream<GSOutput> outputStream, uniform bool showNormals, uniform bool showBinormals, uniform bool showTangents)
{
	GSVectorsPerVertex( In[0], outputStream, showNormals, showBinormals, showTangents );
	GSVectorsPerVertex( In[1], outputStream, showNormals, showBinormals, showTangents );
	GSVectorsPerVertex( In[2], outputStream, showNormals, showBinormals, showTangents );	
}

//--------------------------------------------------------------------------------------
// GeometryShader I/O for Edges
//--------------------------------------------------------------------------------------

[maxvertexcount(4)]
void GSEdges( triangle VSOutput In[3], inout LineStream<GSOutput> outputStream )
{
    GSOutput Next;  
    GSOutput First;
    
    First.Color = EdgeColor;
    Next.Color = EdgeColor;
    
    First.Position = mul(In[0].Position, Projection);
	outputStream.Append(First);
	        
    Next.Position = mul(In[1].Position, Projection);
	outputStream.Append(Next);

    Next.Position = mul(In[2].Position, Projection);
	outputStream.Append(Next);
	
	outputStream.Append(First);

	outputStream.RestartStrip();
}


//--------------------------------------------------------------------------------------
// GeometryShader I/O for Creases
//--------------------------------------------------------------------------------------

[maxvertexcount(6)]
void GSCreases( triangleadj VSOutput In[6], inout LineStream<GSOutput> outputStream )
{
    GSOutput StartPoint;  
    GSOutput EndPoint;  

    StartPoint.Color = CreaseColor;
    EndPoint.Color = CreaseColor;


	if(In[1].VertexId == In[0].VertexId)
	{
		StartPoint.Position = mul(In[0].Position, Projection);
		EndPoint.Position = mul(In[2].Position, Projection);
		
		outputStream.Append(StartPoint);	    
		outputStream.Append(EndPoint);
		outputStream.RestartStrip();
	}


	if(In[3].VertexId == In[2].VertexId)
	{
		StartPoint.Position = mul(In[2].Position, Projection);
		EndPoint.Position = mul(In[4].Position, Projection);
		
		outputStream.Append(StartPoint);	    
		outputStream.Append(EndPoint);
		outputStream.RestartStrip();
	}

	if(In[5].VertexId == In[4].VertexId)
	{
		StartPoint.Position = mul(In[4].Position, Projection);
		EndPoint.Position = mul(In[0].Position, Projection);
		
		outputStream.Append(StartPoint);	    
		outputStream.Append(EndPoint);
		outputStream.RestartStrip();
	}
}

//--------------------------------------------------------------------------------------
// GeometryShader I/O for Adjacencies
//--------------------------------------------------------------------------------------

[maxvertexcount(6)]
void GSAdjacencies( triangleadj VSOutput In[6], inout LineStream<GSOutput> outputStream )
{
    GSOutput Extent;  
    GSOutput Center;  
    
    Extent.Color = AdjacencyColor;
    Center.Color = AdjacencyColor;
    
    
	Center.Position = mul( (In[0].Position+In[2].Position+In[4].Position)/3.0f, Projection );

	if(In[1].VertexId != In[0].VertexId)
	{
		Extent.Position = mul( (In[0].Position+In[2].Position)/2.0f , Projection);

		outputStream.Append(Center);
		outputStream.Append(Extent);
		outputStream.RestartStrip();
	}


	if(In[3].VertexId != In[2].VertexId)
	{
		Extent.Position = mul( (In[2].Position+In[4].Position)/2.0f , Projection);

		outputStream.Append(Center);
		outputStream.Append(Extent);
		outputStream.RestartStrip();
	}

	if(In[5].VertexId != In[4].VertexId)
	{
		Extent.Position = mul( (In[4].Position+In[0].Position)/2.0f , Projection);

		outputStream.Append(Center);
		outputStream.Append(Extent);
		outputStream.RestartStrip();
	}
}

//--------------------------------------------------------------------------------------
// GeometryShader I/O for Bounds
//--------------------------------------------------------------------------------------

float4 GetP0(float4x4 wvp, float3 minpos, float3 maxpos) { float4 result = mul( float4( minpos.x, minpos.y, minpos.z, 1.0f), wvp ); result.z -= Bias; return result; }
float4 GetP1(float4x4 wvp, float3 minpos, float3 maxpos) { float4 result = mul( float4( minpos.x, maxpos.y, minpos.z, 1.0f), wvp ); result.z -= Bias; return result; }
float4 GetP2(float4x4 wvp, float3 minpos, float3 maxpos) { float4 result = mul( float4( maxpos.x, maxpos.y, minpos.z, 1.0f), wvp ); result.z -= Bias; return result; }
float4 GetP3(float4x4 wvp, float3 minpos, float3 maxpos) { float4 result = mul( float4( maxpos.x, minpos.y, minpos.z, 1.0f), wvp ); result.z -= Bias; return result; }
float4 GetP4(float4x4 wvp, float3 minpos, float3 maxpos) { float4 result = mul( float4( minpos.x, minpos.y, maxpos.z, 1.0f), wvp ); result.z -= Bias; return result; }
float4 GetP5(float4x4 wvp, float3 minpos, float3 maxpos) { float4 result = mul( float4( minpos.x, maxpos.y, maxpos.z, 1.0f), wvp ); result.z -= Bias; return result; }
float4 GetP6(float4x4 wvp, float3 minpos, float3 maxpos) { float4 result = mul( float4( maxpos.x, maxpos.y, maxpos.z, 1.0f), wvp ); result.z -= Bias; return result; }
float4 GetP7(float4x4 wvp, float3 minpos, float3 maxpos) { float4 result = mul( float4( maxpos.x, minpos.y, maxpos.z, 1.0f), wvp ); result.z -= Bias; return result; }

[maxvertexcount(16)]
void GSBounds( point VSOutput In[1], inout LineStream<GSOutput> outputStream )
{
    float4x4 WorldViewProj = mul( mul(World, View), Projection);

	GSOutput Out;
	Out.Color = BoundColor;
	
    
	//Zmin face square
	Out.Position = GetP0(WorldViewProj, MinPosition, MaxPosition);
	outputStream.Append(Out);
	Out.Position = GetP1(WorldViewProj, MinPosition, MaxPosition);
	outputStream.Append(Out);
	Out.Position = GetP2(WorldViewProj, MinPosition, MaxPosition);
	outputStream.Append(Out);
	Out.Position = GetP3(WorldViewProj, MinPosition, MaxPosition);
	outputStream.Append(Out);
	Out.Position = GetP0(WorldViewProj, MinPosition, MaxPosition);
	outputStream.Append(Out);
	
	//Zmax face
	Out.Position = GetP4(WorldViewProj, MinPosition, MaxPosition);
	outputStream.Append(Out);//SAVE 2 verts (one connector) by starting right behind end of Zmin loop
	Out.Position = GetP5(WorldViewProj, MinPosition, MaxPosition);
	outputStream.Append(Out);
	Out.Position = GetP6(WorldViewProj, MinPosition, MaxPosition);
	outputStream.Append(Out);
	Out.Position = GetP7(WorldViewProj, MinPosition, MaxPosition);
	outputStream.Append(Out);
	Out.Position = GetP4(WorldViewProj, MinPosition, MaxPosition);
	outputStream.Append(Out);

	outputStream.RestartStrip(); //finished 9 lines

	Out.Position = GetP1(WorldViewProj, MinPosition, MaxPosition);
	outputStream.Append(Out);
	Out.Position = GetP5(WorldViewProj, MinPosition, MaxPosition);
	outputStream.Append(Out);
	outputStream.RestartStrip(); //10 lines
	
	Out.Position = GetP2(WorldViewProj, MinPosition, MaxPosition);
	outputStream.Append(Out);
	Out.Position = GetP6(WorldViewProj, MinPosition, MaxPosition);
	outputStream.Append(Out);
	outputStream.RestartStrip(); //11 lines
	
	Out.Position = GetP3(WorldViewProj, MinPosition, MaxPosition);
	outputStream.Append(Out);
	Out.Position = GetP7(WorldViewProj, MinPosition, MaxPosition);
	outputStream.Append(Out);
	outputStream.RestartStrip(); //12 lines
	
}


//--------------------------------------------------------------------------------------
// GeometryShader I/O for Points
//--------------------------------------------------------------------------------------

void GSPointsPerVertex( VSOutput In, inout TriangleStream<GSOutput> outputStream )
{
    GSOutput Out;  
	Out.Color = PointColor;
    
    float4 center = mul(In.Position, Projection);


    Out.Position = float4( center.x - PointSize, center.y - PointSize, center.z, center.w );
	outputStream.Append(Out);

    Out.Position = float4( center.x + PointSize, center.y - PointSize, center.z, center.w );
	outputStream.Append(Out);

    Out.Position = float4( center.x - PointSize, center.y + PointSize, center.z, center.w );
	outputStream.Append(Out);

    Out.Position = float4( center.x + PointSize, center.y + PointSize, center.z, center.w );
	outputStream.Append(Out);

	outputStream.RestartStrip();
}

[maxvertexcount(12)]
void GSPoints( triangle VSOutput In[3], inout TriangleStream<GSOutput> outputStream )
{
	GSPointsPerVertex( In[0], outputStream );
	GSPointsPerVertex( In[1], outputStream );
	GSPointsPerVertex( In[2], outputStream );	
}

//--------------------------------------------------------------------------------------
// PixelShader I/O
//--------------------------------------------------------------------------------------

float4 PS( GSOutput input ) : SV_Target
{   
    return float4(input.Color.xyz, 1);
}


//--------------------------------------------------------------------------------------
// Techniques
//--------------------------------------------------------------------------------------

technique10 Points
{
    pass P0
    {   

		SetBlendState( Blending, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
        SetDepthStencilState( Depth, 0 );
        SetRasterizerState( Culling );
 
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( CompileShader( gs_4_0, GSPoints() ) );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
        
   }
}


technique10 Edges
{
    pass P0
    {   

		SetBlendState( Blending, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
        SetDepthStencilState( Depth, 0 );
        SetRasterizerState( Culling );
 
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( CompileShader( gs_4_0, GSEdges() ) );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
        
   }
}

technique10 Creases
{
    pass P0
    {   

		SetBlendState( Blending, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
        SetDepthStencilState( Depth, 0 );
        SetRasterizerState( Culling );
 
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( CompileShader( gs_4_0, GSCreases() ) );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
        
   }
}

technique10 Adjacencies
{
    pass P0
    {   

		SetBlendState( Blending, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
        SetDepthStencilState( Depth, 0 );
        SetRasterizerState( Culling );
 
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( CompileShader( gs_4_0, GSAdjacencies() ) );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
        
   }
}

technique10 Bounds
{
    pass P0
    {   

		SetBlendState( Blending, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
        SetDepthStencilState( Depth, 0 );
        SetRasterizerState( Culling );
 
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( CompileShader( gs_4_0, GSBounds() ) );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
        
   }
}

technique10 Normals
{
    pass P0
    {   

		SetBlendState( Blending, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
        SetDepthStencilState( Depth, 0 );
        SetRasterizerState( Culling );
 
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( CompileShader( gs_4_0, GSVectors(true, false, false) ) );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
        
   }
}


technique10 Binormals
{
    pass P0
    {   

		SetBlendState( Blending, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
        SetDepthStencilState( Depth, 0 );
        SetRasterizerState( Culling );
 
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( CompileShader( gs_4_0, GSVectors(false, true, false) ) );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
        
   }
}
technique10 Tangents
{
    pass P0
    {   

		SetBlendState( Blending, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
        SetDepthStencilState( Depth, 0 );
        SetRasterizerState( Culling );
 
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( CompileShader( gs_4_0, GSVectors(false, false, true) ) );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
        
   }
}
