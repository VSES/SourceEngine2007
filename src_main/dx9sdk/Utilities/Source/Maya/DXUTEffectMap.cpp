//-----------------------------------------------------------------------------
// File: DXUTEffectMap.cpp
//
// Desc: Maps the set of standard semantics and annotations to a collection
//       of ID3DXEffect objects. 
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------
#include "pch.h"

const char* SasIncluder::SasFxh= 
{
	"//-------------------------------------------------------------------------------------\n"
	"// Sas.fxh\n"
	"// Microsoft(R) Standard Annotations and Semantics\n"
	"// Version 1.0.0\n"
	"// Standard include file\n"
	"// Copyright (c) Microsoft Corporation. All rights reserved.\n"
	"//-------------------------------------------------------------------------------------\n"
	"\n"
	"//-------------------------------------------------------------------------------------\n"
	"// SasHostParameterValues\n"
	"// This structure is HLSL pseudocode and will not compile. It's provided here as a\n"
	"// reference for the values which an application should expose for parameter binding.\n"
	"//-------------------------------------------------------------------------------------\n"
	"\n"
	"/*\n"
	"\n"
	"struct SasHostParameterValues\n"
	"{\n"
	"struct\n"
	"{\n"
	"float Now;  // SasBindAddress = \"Sas.Time.Now\"\n"
	"float Last; // SasBindAddress = \"Sas.Time.Last\", etc...\n"
	"int FrameNumber;\n"
	"}\n"
	"Time;\n"
	"\n"
	"textureCUBE EnvironmentMap;\n"
	"\n"
	"SasCamera Camera;\n"
	"\n"
	"SasAmbientLight AmbientLight[ZeroOrMore];\n"
	"int NumAmbientLights;\n"
	"\n"
	"SasAmbientLight DirectionalLight[ZeroOrMore];\n"
	"int NumDirectionalLights;\n"
	"\n"
	"SasAmbientLight PointLight[ZeroOrMore];\n"
	"int NumPointLights;\n"
	"\n"
	"SasAmbientLight SpotLight[ZeroOrMore];\n"
	"int NumSpotLights;\n"
	"\n"
	"struct\n"
	"{\n"
	"float4x4 WorldToShadow;\n"
	"texture2D ShadowMap;\n"
	"}\n"
	"Shadow[ZeroOrMore];\n"
	"int NumShadows;\n"
	"\n"
	"struct\n"
	"{\n"
	"float4x4 MeshToJointToWorld[OneOrMore];\n"
	"int NumJoints;\n"
	"}\n"
	"Skeleton;\n"
	"}\n"
	"Sas;\n"
	"\n"
	"*/\n"
	"\n"
	"//-------------------------------------------------------------------------------------\n"
	"// Macros\n"
	"//-------------------------------------------------------------------------------------\n"
	"\n"
	"// The implementation’s shadow_sampler() macro must be\n"
	"// defined in the standard include as well.\n"
	"//\n"
	"// Host applications are permitted to change the\n"
	"// implementation of this macro as long as the macro\n"
	"// evaluates to a sampler_state constructor.\n"
	"//\n"
	"#define shadow_sampler(ShadowMap) \\n"
	"sampler_state \\n"
	"{ \\n"
	"Texture = (ShadowMap); \\n"
	"}\n"
	"\n"
	"\n"
	"//-------------------------------------------------------------------------------------\n"
	"// Structures\n"
	"//-------------------------------------------------------------------------------------\n"
	"\n"
	"struct SasCamera\n"
	"{\n"
	"float4x4 WorldToView;\n"
	"float4x4 Projection;\n"
	"float2 NearFarClipping;\n"
	"float3 Position;\n"
	"};\n"
	"\n"
	"struct SasAmbientLight\n"
	"{\n"
	"float3 Color;         // default: {0, 0, 0}\n"
	"};\n"
	"\n"
	"struct SasDirectionalLight\n"
	"{\n"
	"float3 Color;         // default: {0, 0, 0}\n"
	"float3 Direction;     // default: {0, 0, 0}\n"
	"};\n"
	"\n"
	"struct SasPointLight\n"
	"{\n"
	"float3 Color;         // default: {0, 0, 0}\n"
	"float3 Position;      // default: {0, 0, 0}\n"
	"float  Range;         // default: 0\n"
	"};\n"
	"\n"
	"struct SasSpotLight\n"
	"{\n"
	"float3 Color;         // default: {0, 0, 0}\n"
	"float3 Position;      // default: {0, 0, 0}\n"
	"float  Direction;     // default: {0, 0, 0}\n"
	"float  Range;         // default: 0\n"
	"float  Theta;         // default: 0\n"
	"float  Phi;           // default: 0\n"
	"};\n"
	"\n"
	"\n"
	"//-------------------------------------------------------------------------------------\n"
	"// Functions\n"
	"//-------------------------------------------------------------------------------------\n"
	"\n"
	"// The following provide generalized inverse of a matrix.\n"
	"// They cannot assume anything about the matrix.\n"
	"\n"
	"//-------------------------------------------------------------------------------------\n"
	"float3x3 inverse(float3x3 m)\n"
	"{\n"
	"float3x3 adj;\n"
	"\n"
	"// Calculate the adjoint matrix\n"
	"adj._11 = m._22*m._33 - m._23*m._32;\n"
	"adj._12 = m._13*m._32 - m._12*m._33;\n"
	"adj._13 = m._12*m._23 - m._13*m._22;\n"
	"\n"
	"adj._21 = m._23*m._31 - m._21*m._33;\n"
	"adj._22 = m._11*m._33 - m._13*m._31;\n"
	"adj._23 = m._13*m._21 - m._11*m._23;\n"
	"\n"
	"adj._31 = m._21*m._32 - m._22*m._31;\n"
	"adj._32 = m._12*m._31 - m._11*m._32;\n"
	"adj._33 = m._11*m._22 - m._12*m._21;\n"
	"\n"
	"// Calculate the determinant\n"
	"float det = determinant(m);\n"
	"\n"
	"float3x3 result = 0;\n"
	"if( 0.0f != det )\n"
	"result = 1.0f/det * adj;\n"
	"\n"
	"return result;\n"
	"}\n"
	"\n"
	"//-------------------------------------------------------------------------------------\n"
	"float4x4 inverse(float4x4 m)\n"
	"{\n"
	"float4x4 adj;\n"
	"\n"
	"// Calculate the adjoint matrix\n"
	"adj._11 = m._22*m._33*m._44 + m._23*m._34*m._42 + m._24*m._32*m._43 - m._22*m._34*m._43 - m._23*m._32*m._44 - m._24*m._33*m._42;\n"
	"adj._12 = m._12*m._34*m._43 + m._13*m._32*m._44 + m._14*m._33*m._42 - m._12*m._33*m._44 - m._13*m._34*m._42 - m._14*m._32*m._43;\n"
	"adj._13 = m._12*m._23*m._44 + m._13*m._24*m._42 + m._14*m._22*m._43 - m._12*m._24*m._43 - m._13*m._22*m._44 - m._14*m._23*m._42;\n"
	"adj._14 = m._12*m._24*m._33 + m._13*m._22*m._34 + m._14*m._23*m._32 - m._12*m._23*m._34 - m._13*m._24*m._32 - m._14*m._22*m._33;\n"
	"\n"
	"adj._21 = m._21*m._34*m._43 + m._23*m._31*m._44 + m._24*m._33*m._41 - m._21*m._33*m._44 - m._23*m._34*m._41 - m._24*m._31*m._43;\n"
	"adj._22 = m._11*m._33*m._44 + m._13*m._34*m._41 + m._14*m._31*m._43 - m._11*m._34*m._43 - m._13*m._31*m._44 - m._14*m._33*m._41;\n"
	"adj._23 = m._11*m._24*m._43 + m._13*m._21*m._44 + m._14*m._23*m._41 - m._11*m._23*m._44 - m._13*m._24*m._41 - m._14*m._21*m._43;\n"
	"adj._24 = m._11*m._23*m._34 + m._13*m._24*m._31 + m._14*m._21*m._33 - m._11*m._24*m._33 - m._13*m._21*m._34 - m._14*m._23*m._31;\n"
	"\n"
	"adj._31 = m._21*m._32*m._44 + m._22*m._34*m._41 + m._24*m._31*m._42 - m._21*m._34*m._42 - m._22*m._31*m._44 - m._24*m._32*m._41;\n"
	"adj._32 = m._11*m._34*m._42 + m._12*m._31*m._44 + m._14*m._32*m._41 - m._11*m._32*m._44 - m._12*m._34*m._41 - m._14*m._31*m._42;\n"
	"adj._33 = m._11*m._22*m._44 + m._12*m._24*m._41 + m._14*m._21*m._42 - m._11*m._24*m._42 - m._12*m._21*m._44 - m._14*m._22*m._41;\n"
	"adj._34 = m._11*m._24*m._32 + m._12*m._21*m._34 + m._14*m._22*m._31 - m._11*m._22*m._34 - m._12*m._24*m._31 - m._14*m._21*m._32;\n"
	"\n"
	"adj._41 = m._21*m._33*m._42 + m._22*m._31*m._43 + m._23*m._32*m._41 - m._21*m._32*m._43 - m._22*m._33*m._41 - m._23*m._31*m._42;\n"
	"adj._42 = m._11*m._32*m._43 + m._12*m._33*m._41 + m._13*m._31*m._42 - m._11*m._33*m._42 - m._12*m._31*m._43 - m._13*m._32*m._41;\n"
	"adj._43 = m._11*m._23*m._42 + m._12*m._21*m._43 + m._13*m._22*m._41 - m._11*m._22*m._43 - m._12*m._23*m._41 - m._13*m._21*m._42;\n"
	"adj._44 = m._11*m._22*m._33 + m._12*m._23*m._31 + m._13*m._21*m._32 - m._11*m._23*m._32 - m._12*m._21*m._33 - m._13*m._22*m._31;\n"
	"\n"
	"// Calculate the determinant\n"
	"float det = determinant(m);\n"
	"\n"
	"float4x4 result = 0;\n"
	"if( 0.0f != det )\n"
	"result = 1.0f/det * adj;\n"
	"\n"
	"return result;\n"
	"}\n"
	"\n"
	"//-------------------------------------------------------------------------------------\n"
	"float4x3 inverse(float4x3 m)\n"
	"{\n"
	"// Separate the rotation and translation components\n"
	"float3x3 R = (float3x3)m;\n"
	"float1x3 t = { m._41, m._42, m._43 };\n"
	"\n"
	"// Find the inverse matrix\n"
	"// A0 R0\n"
	"// v1 t1 = I\n"
	"\n"
	"float3x3 A = inverse( R );\n"
	"float1x3 v = -mul( A, t );\n"
	"\n"
	"return float4x3( A._11, A._12, A._13,\n"
	"A._21, A._22, A._23,\n"
	"A._31, A._32, A._33,\n"
	"v._11, v._12, v._13 );\n"
	"}\n"
	"\n"
	"//-------------------------------------------------------------------------------------\n"
	"float3x4 inverse(float3x4 m)\n"
	"{\n"
	"// Separate the rotation and translation components\n"
	"float3x3 R = (float3x3)m;\n"
	"float3x1 t = { m._14, m._24, m._34 };\n"
	"\n"
	"// Find the inverse matrix\n"
	"// Av Rt\n"
	"// 01 01 = I\n"
	"\n"
	"float3x3 A = inverse( R );\n"
	"float3x1 v = -mul( A, t );\n"
	"\n"
	"return float3x4( A._11, A._12, A._13, v._11,\n"
	"A._21, A._22, A._23, v._21,\n"
	"A._31, A._32, A._33, v._31 );\n"
	"}\n"
	"\n"
	"\n"
	"// The following provide faster versions of inverse that assume the matrix is orthogonal.\n"
	"\n"
	"//-------------------------------------------------------------------------------------\n"
	"float3x3 inverse_orthogonal(float3x3 m)\n"
	"{\n"
	"float length0 = length(m[0]);\n"
	"float length1 = length(m[1]);\n"
	"float length2 = length(m[2]);\n"
	"\n"
	"m[0] /= length0 * length0;\n"
	"m[1] /= length1 * length1;\n"
	"m[2] /= length2 * length2;\n"
	"\n"
	"return transpose(m);\n"
	"}\n"
	"\n"
	"//-------------------------------------------------------------------------------------\n"
	"float4x4 inverse_orthogonal(float4x4 m)\n"
	"{\n"
	"float length0 = length(m[0]);\n"
	"float length1 = length(m[1]);\n"
	"float length2 = length(m[2]);\n"
	"float length3 = length(m[3]);\n"
	"\n"
	"m[0] /= length0 * length0;\n"
	"m[1] /= length1 * length1;\n"
	"m[2] /= length2 * length2;\n"
	"m[3] /= length3 * length3;\n"
	"\n"
	"return transpose(m);\n"
	"}\n"
	"\n"
	"//-------------------------------------------------------------------------------------\n"
	"float4x3 inverse_orthogonal(float4x3 m)\n"
	"{\n"
	"// Separate the rotation and translation components\n"
	"float3x3 R = (float3x3)m;\n"
	"float1x3 t = { m._41, m._42, m._43 };\n"
	"\n"
	"// Find the inverse matrix\n"
	"// A0 R0\n"
	"// v1 t1 = I\n"
	"\n"
	"float3x3 A = inverse_orthogonal( R );\n"
	"float1x3 v = -mul( A, t );\n"
	"\n"
	"return float4x3( A._11, A._12, A._13,\n"
	"A._21, A._22, A._23,\n"
	"A._31, A._32, A._33,\n"
	"v._11, v._12, v._13 );\n"
	"}\n"
	"\n"
	"//-------------------------------------------------------------------------------------\n"
	"float3x4 inverse_orthogonal(float3x4 m)\n"
	"{\n"
	"// Separate the rotation and translation components\n"
	"float3x3 R = (float3x3)m;\n"
	"float3x1 t = { m._14, m._24, m._34 };\n"
	"\n"
	"// Find the inverse matrix\n"
	"// Av Rt\n"
	"// 01 01 = I\n"
	"\n"
	"float3x3 A = inverse_orthogonal( R );\n"
	"float3x1 v = -mul( A, t );\n"
	"\n"
	"return float3x4( A._11, A._12, A._13, v._11,\n"
	"A._21, A._22, A._23, v._21,\n"
	"A._31, A._32, A._33, v._31 );\n"
	"}\n"
	"\n"
	"\n"
	"// The following provide faster versions of inverse that assume the matrix is orthonormal.\n"
	"\n"
	"//-------------------------------------------------------------------------------------\n"
	"float3x3 inverse_orthonormal(float3x3 m)\n"
	"{\n"
	"return transpose(m);\n"
	"}\n"
	"\n"
	"//-------------------------------------------------------------------------------------\n"
	"float4x4 inverse_orthonormal(float4x4 m)\n"
	"{\n"
	"return transpose(m);\n"
	"}\n"
	"\n"
	"//-------------------------------------------------------------------------------------\n"
	"float4x3 inverse_orthonormal(float4x3 m)\n"
	"{\n"
	"// Separate the rotation and translation components\n"
	"float3x3 R = (float3x3)m;\n"
	"float1x3 t = { m._41, m._42, m._43 };\n"
	"\n"
	"// Find the inverse matrix\n"
	"// A0 R0\n"
	"// v1 t1 = I\n"
	"\n"
	"float3x3 A = inverse_orthonormal( R );\n"
	"float1x3 v = -mul( A, t );\n"
	"\n"
	"return float4x3( A._11, A._12, A._13,\n"
	"A._21, A._22, A._23,\n"
	"A._31, A._32, A._33,\n"
	"v._11, v._12, v._13 );\n"
	"}\n"
	"\n"
	"//-------------------------------------------------------------------------------------\n"
	"float3x4 inverse_orthonormal(float3x4 m)\n"
	"{\n"
	"// Separate the rotation and translation components\n"
	"float3x3 R = (float3x3)m;\n"
	"float3x1 t = { m._14, m._24, m._34 };\n"
	"\n"
	"// Find the inverse matrix\n"
	"// Av Rt\n"
	"// 01 01 = I\n"
	"\n"
	"float3x3 A = inverse_orthonormal( R );\n"
	"float3x1 v = -mul( A, t );\n"
	"\n"
	"return float3x4( A._11, A._12, A._13, v._11,\n"
	"A._21, A._22, A._23, v._21,\n"
	"A._31, A._32, A._33, v._31 );\n"
	"}\n"
	"\n"
	"\n"
	"// The implementation’s shadow() sampling function must\n"
	"// be defined in the standard include as well.\n"
	"//-------------------------------------------------------------------------------------\n"
	"float shadow( sampler ShadowSampler, float4x4 WorldToShadow, float3 WorldPoint)\n"
	"{\n"
	"return 0.0;\n"
	"}\n"
};

SasIncluder g_SasIncluder;

struct SASBA_BINDADDRESS_MAP
{
	SASBA_BINDADDRESS number;
	const char* string;
} 

SasBindAddressMap[SASBA_BINDADDRESS_COUNT] = 
{
	{SASBA_Unknown, ""},

	{SASBA_Skeleton_MeshToJointToWorld, "Sas.Skeleton.MeshToJointToWorld[]"},
	{SASBA_Skeleton_NumJoints, "Sas.Skeleton.NumJoints"},

	{SASBA_Camera, "Sas.Camera"},
	{SASBA_Camera_WorldToView, "Sas.Camera.WorldToView"},
	{SASBA_Camera_Projection, "Sas.Camera.Projection"},
	{SASBA_Camera_NearFarClipping, "Sas.Camera.NearFarClipping"},
	{SASBA_Camera_Position, "Sas.Camera.Position"},

	{SASBA_Time, "Sas.Time"},
	{SASBA_Time_Now, "Sas.Time.Now"},
	{SASBA_Time_Last, "Sas.Time.Last"},
	{SASBA_Time_FrameNumber, "Sas.Time.FrameNumber"},

	//{SASBA_EnvironmentMap, "Sas.EnvironmentMap"},

	{SASBA_AmbientLight, "Sas.AmbientLight[0]"},
	{SASBA_AmbientLight_Color, "Sas.AmbientLight[0].Color"},
	{SASBA_NumAmbientLights, "Sas.NumAmbientLights"},

	{SASBA_PointLight, "Sas.PointLight[0]"},
	{SASBA_PointLight_Color, "Sas.PointLight[0].Color"},
	{SASBA_PointLight_Position, "Sas.PointLight[0].Position"},
	{SASBA_PointLight_Range, "Sas.PointLight[0].Range"},
	{SASBA_NumPointLights, "Sas.NumPointLights"},

	{SASBA_DirectionalLight, "Sas.DirectionalLight[0]"},
	{SASBA_DirectionalLight_Color, "Sas.DirectionalLight[0].Color"},
	{SASBA_DirectionalLight_Direction, "Sas.DirectionalLight[0].Direction"},
	{SASBA_NumDirectionalLights, "Sas.NumDirectionalLights"},

	{SASBA_SpotLight, "Sas.SpotLight[0]"},
	{SASBA_SpotLight_Color, "Sas.SpotLight[0].Color"},
	{SASBA_SpotLight_Position, "Sas.SpotLight[0].Position"},
	{SASBA_SpotLight_Direction, "Sas.SpotLight[0].Direction"},
	{SASBA_SpotLight_Range, "Sas.SpotLight[0].Range"},
	{SASBA_SpotLight_Theta, "Sas.SpotLight[0].Theta"},
	{SASBA_SpotLight_Phi, "Sas.SpotLight[0].Phi"},
	{SASBA_NumSpotLights, "Sas.NumSpotLights"},

	{SASBA_Shadow_WorldToShadow, "Sas.Shadow[].WorldToShadow"},  
	//{SASBA_Shadow_ShadowMap, "Sas.Shadow[].ShadowMap"},   
	{SASBA_NumShadows, "Sas.NumShadows"}
};


DEFSTDMETHOD(SasIncluder::Open)(THIS_ 
	D3DXINCLUDE_TYPE IncludeType, 
	LPCSTR pFileName, 
	LPCVOID pParentData, 
	LPCVOID *ppData, 
	UINT *pBytes)
{
	HRESULT hr= S_OK;


	*ppData= 0;
	*pBytes= 0;

	if( 0 == lstrcmpiA( "sas/Sas.fxh", pFileName)
		|| 0 == lstrcmpiA( "sas\\Sas.fxh", pFileName)
		|| 0 == lstrcmpiA( "Sas.fxh", pFileName) )
	{
		DXCC_DPFA_REPORT("LOADING SAS/SAS.FXH");
		*ppData= SasFxh;
		*pBytes= lstrlenA(SasFxh);

	}
    else
    {
        // Attempt to read the given file
        WIN32_FIND_DATAA data;
        HANDLE hFind = NULL;
        HANDLE hFile = NULL;

        hFind = FindFirstFileA( pFileName, &data );
        if( INVALID_HANDLE_VALUE == hFind )
        {
            hr = E_FAIL;
            goto LFail;
        }

        hFile = CreateFileA( pFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL );
        if( INVALID_HANDLE_VALUE == hFile )
        {
            hr = E_FAIL;
            goto LFail;
        }

        *pBytes = data.nFileSizeLow;
        *ppData = (void*)new BYTE[*pBytes];
        if( *ppData == NULL )
        {
            hr = E_OUTOFMEMORY;
            goto LFail;
        }

        //TODO - this is failing
        DWORD bytesRead;
        if( 0 == ReadFile( hFile, (void*)*ppData, *pBytes, &bytesRead, NULL ) )
        {
            hr = E_FAIL;
            goto LFail;
        }

LFail:
		if(FAILED(hr))
		{
			if(*ppData)
				delete [] *ppData;
			*ppData= 0;
			*pBytes= 0;
		}
        CloseHandle(hFile);
        FindClose(hFind);
        return hr;
    }

	return S_OK;
}

DEFSTDMETHOD(SasIncluder::Close)(THIS_ 
	LPCVOID pData)
{
	if( pData != SasFxh)
		delete [] pData;
	return S_OK;
}


//-------------------------------------------------------------------------------------
bool StringBegins( const char* strTest, const char* strStartsWith ) 
{ 
    return ( 0 == _strnicmp( strTest, strStartsWith, strlen(strStartsWith) ) ); 
}


//-------------------------------------------------------------------------------------
bool StringEquals( const char* strTest, const char* strEquals ) 
{ 
    return( 0 == lstrcmpiA( strTest, strEquals ) ); 
}


//-------------------------------------------------------------------------------------
CStringA RemoveNumbers( const char* strWithoutNumber)//, DWORD dwBufLen, const char* str )
{
	CStringA result(strWithoutNumber);//lazy buffer resize....we're only going to get smaller
 
	const char *strRead = strWithoutNumber;
	char *strWrite = result.GetBuffer();
    while(*strRead != '\0')
	{
		if( ! isdigit( (BYTE)*strRead )  || (*strRead == '*'))
		{
			*strWrite = *strRead;
			strWrite++;
		}
		strRead++;
	}
	*strWrite = '\0';

	result.ReleaseBuffer();

	return result;
}

//-------------------------------------------------------------------------------------
SASBA_BINDADDRESS CDXUTEffectMap::StringToBindAddress( const char* str )
{
	//match full
    CStringA matcher(str);
	for(UINT i= 0; i < SASBA_BINDADDRESS_COUNT; i++)
	{
		if(matcher == SasBindAddressMap[i].string)
			return SasBindAddressMap[i].number;
	}

	//match stripped
    matcher= RemoveNumbers( str );
	for(UINT i= 0; i < SASBA_BINDADDRESS_COUNT; i++)
	{
		if(matcher == SasBindAddressMap[i].string)
			return SasBindAddressMap[i].number;
	}


	return SASBA_Unknown;
}


//-------------------------------------------------------------------------------------
SASBA_BINDADDRESS CDXUTEffectMap::StringToBindAddress( const WCHAR* strBind )
{
    char cstr[MAX_PATH+1] = {0};
    WideCharToMultiByte( CP_ACP, 0, strBind, -1, cstr, MAX_PATH, NULL, NULL );
    return StringToBindAddress( cstr );
}

//-------------------------------------------------------------------------------------
VOID CDXUTEffectMap::Reset()
{
	// Reset all the stored parameter lists
	for( UINT iBA = 0; iBA < SASBA_BINDADDRESS_COUNT; iBA++ )
	{
		CGrowableArray<ParamList>* pBinding = &m_Bindings[ iBA ];

		// Clear nested arrays first
		for( int iParamList = 0; iParamList < pBinding->GetSize(); iParamList++ )
		{
			pBinding->GetAt( iParamList ).Reset();
		}
		
		// Remove all the bound parameter lists
		pBinding->RemoveAll();
	}

}


//-------------------------------------------------------------------------------------
HRESULT CDXUTEffectMap::SetStandardParameterAsValue( SASBA_BINDADDRESS eBind, 
                                         LPCVOID pData, 
                                         DWORD dwDataLen
										 //, 
                                         //const WCHAR* strType, 
                                         //const WCHAR* strUnits, 
                                         //const WCHAR* strSpace 
										 )
{
    HRESULT hr;

    // TODO: handle unit and space conversions

    // Retrieve the interested handles
    CGrowableArray<ParamList>* pBindings = &m_Bindings[ eBind ];
        
    for( int iList=0; iList < pBindings->GetSize(); iList++ )
    {
        ParamList& paramList = pBindings->GetAt(iList);

        for( int iParam=0; iParam < paramList.ahParameters.GetSize(); iParam++ )
        {

            V_RETURN( paramList.pEffect->SetValue( paramList.ahParameters[iParam], pData, dwDataLen ) );
        }
    }

    return S_OK;
}
//-------------------------------------------------------------------------------------
HRESULT CDXUTEffectMap::SetStandardParameterAsFloats( SASBA_BINDADDRESS eBind, 
                                         float* pData, 
                                         DWORD dwDataLen
										 //, 
                                         //const WCHAR* strType, 
                                         //const WCHAR* strUnits, 
                                         //const WCHAR* strSpace 
										 )
{
    HRESULT hr;

    // TODO: handle unit and space conversions

    // Retrieve the interested handles
    CGrowableArray<ParamList>* pBindings = &m_Bindings[ eBind ];
        
    for( int iList=0; iList < pBindings->GetSize(); iList++ )
    {
        ParamList& paramList = pBindings->GetAt(iList);

        for( int iParam=0; iParam < paramList.ahParameters.GetSize(); iParam++ )
        {

            V_RETURN( paramList.pEffect->SetFloatArray( paramList.ahParameters[iParam], pData, dwDataLen ) );
        }
    }

    return S_OK;
}

HRESULT CDXUTEffectMap::SetStandardParameterAsMatrix( SASBA_BINDADDRESS eBind, 
                                         D3DXMATRIX* pData
										 //, 
                                         //const WCHAR* strType, 
                                         //const WCHAR* strUnits, 
                                         //const WCHAR* strSpace 
										 )
{
    HRESULT hr;

    // TODO: handle unit and space conversions

    // Retrieve the interested handles
    CGrowableArray<ParamList>* pBindings = &m_Bindings[ eBind ];
        
    for( int iList=0; iList < pBindings->GetSize(); iList++ )
    {
        ParamList& paramList = pBindings->GetAt(iList);

        for( int iParam=0; iParam < paramList.ahParameters.GetSize(); iParam++ )
        {

            V_RETURN( paramList.pEffect->SetMatrix( paramList.ahParameters[iParam], pData ) );
        }
    }

    return S_OK;
}

void ParamList::Reset() 
{ 
	SAFE_RELEASE(pEffect); 
	ahParameters.RemoveAll(); 
}


//-------------------------------------------------------------------------------------
// Investigates all the parameters, looking at semantics and annotations and placing 
// handles to these parameters within the internal database.
//-------------------------------------------------------------------------------------
HRESULT CDXUTEffectMap::AddEffect( ID3DXEffect* pEffect )
{
    HRESULT hr;

	if( pEffect == NULL )
		return E_INVALIDARG;

    // Get the number of parameters
    D3DXEFFECT_DESC descEffect;
    V_RETURN( pEffect->GetDesc( &descEffect ) );
    
    // Enumerate the parameters
    for( UINT iParam=0; iParam < descEffect.Parameters; iParam++ )
    {
        // Retrieve param
        D3DXHANDLE hParameter = pEffect->GetParameter( NULL, iParam );
        if( NULL == hParameter )
            return E_FAIL;

		D3DXHANDLE hBindAddy= pEffect->GetAnnotationByName( hParameter, "SasBindAddress");
        if( NULL == hBindAddy )
            continue;

		LPCSTR szBindAddress= NULL;
		pEffect->GetString(hBindAddy, &szBindAddress);

        if( szBindAddress == NULL || szBindAddress[0] == '\0' )
            continue;

        // Map the semantic to the standard set
        SASBA_BINDADDRESS eBind = StringToBindAddress( szBindAddress );
        if( eBind == SASBA_Unknown )
            continue;

        // Store the handle
        CGrowableArray<ParamList>* pBindings = &m_Bindings[ eBind ];
        
        bool bBound = false;
        for( int i=0; i < pBindings->GetSize(); i++ )
        {
            if( pBindings->GetAt(i).pEffect == pEffect )
            {
                // Found the containing effect for this parameter in the list, add the new handle
                pBindings->GetAt(i).ahParameters.Add( hParameter );
                bBound = true;
                break;
            }
        }

        if( !bBound )
        {
            // This parameter belongs to a new effect
            ParamList newParamList;
            newParamList.pEffect = pEffect;
			pEffect->AddRef();
            newParamList.ahParameters.Add( hParameter );
            pBindings->Add( newParamList );
        }
       
    }
    
    return S_OK;
}


//-------------------------------------------------------------------------------------
// Removes all instances of this effect from the binding list
//-------------------------------------------------------------------------------------
HRESULT CDXUTEffectMap::RemoveEffect( ID3DXEffect* pEffect )
{
	if( pEffect == NULL )
		return E_INVALIDARG;

	// Search through the list of registered semantics and remove all items
	// assigned to the given effect
	for( UINT iBA = 0; iBA < SASBA_BINDADDRESS_COUNT; iBA++ )
	{
		CGrowableArray<ParamList>* pBinding = &m_Bindings[ iBA ];

		// Clear nested arrays first
		for( int iParamList = 0; iParamList < pBinding->GetSize(); iParamList++ )
		{
			ParamList& rParamList = pBinding->GetAt( iParamList );

			if( rParamList.pEffect == pEffect )
				rParamList.Reset();
		}
	}

	return S_OK;
}




