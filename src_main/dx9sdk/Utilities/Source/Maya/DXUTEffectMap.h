//-----------------------------------------------------------------------------
// File: DXUTEffectMap.h
//
// Desc: Maps the set of standard semantics and annotations to a collection
//       of ID3DXEffect objects. 
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------
#pragma once
#ifndef DXUT_EFFECTMAP_H
#define DXUT_EFFECTMAP_H

const int MAX_INDEX = 5;

enum SASBA_BINDADDRESS
{
    SASBA_Unknown= 0,

	SASBA_Skeleton_MeshToJointToWorld,
	SASBA_Skeleton_NumJoints,

	SASBA_Camera,
	SASBA_Camera_WorldToView,
	SASBA_Camera_Projection,
	SASBA_Camera_NearFarClipping,
	SASBA_Camera_Position,

 
	SASBA_Time,
	SASBA_Time_Now,
	SASBA_Time_Last,
	SASBA_Time_FrameNumber,

	//SASBA_EnvironmentMap,
  
	SASBA_AmbientLight,
	SASBA_AmbientLight_Color,
	SASBA_NumAmbientLights,

    SASBA_PointLight,
    SASBA_PointLight_Color,
    SASBA_PointLight_Position,
    SASBA_PointLight_Range,
	SASBA_NumPointLights,

    SASBA_DirectionalLight,
    SASBA_DirectionalLight_Color,
    SASBA_DirectionalLight_Direction,
    SASBA_NumDirectionalLights,

    SASBA_SpotLight,
    SASBA_SpotLight_Color,
    SASBA_SpotLight_Position,
    SASBA_SpotLight_Direction,
    SASBA_SpotLight_Range,
    SASBA_SpotLight_Theta,
    SASBA_SpotLight_Phi,
    SASBA_NumSpotLights,

    SASBA_Shadow_WorldToShadow,
    //SASBA_Shadow_ShadowMap,
    SASBA_NumShadows,

    SASBA_BINDADDRESS_COUNT
};

struct SasIncluder : public ID3DXInclude
{
    STDMETHOD(Open)(THIS_ D3DXINCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes);
    STDMETHOD(Close)(THIS_ LPCVOID pData);

	static const char* SasFxh;
};

extern SasIncluder g_SasIncluder;

struct ParamList
{
	ID3DXEffect* pEffect;
    CGrowableArray< D3DXHANDLE > ahParameters;

	void Reset();
};


class CDXUTEffectMap
{
public:
	CDXUTEffectMap() { Reset(); }
	~CDXUTEffectMap() { Reset(); }
	
	VOID    Reset();

	HRESULT AddEffect( ID3DXEffect* pEffect );
	HRESULT RemoveEffect( ID3DXEffect* pEffect );

    HRESULT SetStandardParameterAsValue( SASBA_BINDADDRESS eBind, LPCVOID pData, DWORD dwDataLen ); 
	HRESULT SetStandardParameterAsFloats( SASBA_BINDADDRESS eBind, float* pData, DWORD dwDataLen ); 
    HRESULT SetStandardParameterAsMatrix( SASBA_BINDADDRESS eBind, D3DXMATRIX* pData ); 

    
	static SASBA_BINDADDRESS StringToBindAddress( const char* strBind );
	static SASBA_BINDADDRESS StringToBindAddress( const WCHAR* strBind );

private:


	// Database of DXUTEffect object parameter handles which are indexed accoring to 
    // Semantic, Object annotation, index, and containing mesh pointer
    CGrowableArray<ParamList> m_Bindings[ SASBA_BINDADDRESS_COUNT ];
};





#endif //DXUT_EFFECTMAP_H