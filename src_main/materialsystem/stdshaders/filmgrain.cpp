//========= Copyright © 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

#include "basevsshader.h"

#include "screenspaceeffect_vs11.inc"
#include "filmgrain_ps11.inc"

#include "..\materialsystem_global.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

BEGIN_VS_SHADER_FLAGS( FilmGrain, "Help for FilmGrain", SHADER_NOT_EDITABLE )
	BEGIN_SHADER_PARAMS
		SHADER_PARAM( GRAIN_TEXTURE,  SHADER_PARAM_TYPE_TEXTURE, "0", "Film grain texture" )
		SHADER_PARAM( NOISESCALE,     SHADER_PARAM_TYPE_VEC4, "", "Strength of film grain" )
	END_SHADER_PARAMS

	SHADER_INIT_PARAMS()
	{
		SET_FLAGS2( MATERIAL_VAR2_NEEDS_FULL_FRAME_BUFFER_TEXTURE );
	}

	SHADER_FALLBACK
	{
		// Requires DX9 + above
		if ( g_pHardwareConfig->GetDXSupportLevel() < 80)
		{
				return "FilmGrain_DX7";
		}
		return 0;
	}

	SHADER_INIT
	{
		LoadTexture( GRAIN_TEXTURE );
	}

	SHADER_DRAW
	{
		SHADOW_STATE
		{
			pShaderShadow->EnableBlending( true );
			pShaderShadow->BlendFunc( SHADER_BLEND_ONE, SHADER_BLEND_SRC_ALPHA );
			pShaderShadow->EnableTexture( SHADER_SAMPLER0, true );

			int fmt = VERTEX_POSITION;
			pShaderShadow->VertexShaderVertexFormat( fmt, 1, 0, 0 );

			DECLARE_STATIC_VERTEX_SHADER( screenspaceeffect_vs11 );
			SET_STATIC_VERTEX_SHADER( screenspaceeffect_vs11 );

			DECLARE_STATIC_PIXEL_SHADER( filmgrain_ps11 );
			SET_STATIC_PIXEL_SHADER( filmgrain_ps11 );
		}
		DYNAMIC_STATE
		{
			BindTexture( SHADER_SAMPLER0, GRAIN_TEXTURE, -1 );
						
			SetPixelShaderConstant( 0, NOISESCALE );

			DECLARE_DYNAMIC_VERTEX_SHADER( screenspaceeffect_vs11 );
			SET_DYNAMIC_VERTEX_SHADER( screenspaceeffect_vs11 );
		}
		Draw();
	}
END_SHADER
