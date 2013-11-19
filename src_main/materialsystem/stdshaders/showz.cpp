//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: Visualize shadow z buffers.  Designed to be used when drawing a screen-aligned
//          quad with a floating-point z-buffer so that the large z-range is divided down
//          into visual range of grayscale colors.
//
// $NoKeywords: $
//=============================================================================//

#include "convar.h"
#include "BaseVSShader.h"

#include "showz_vs11.inc"
#include "showz_ps20.inc"
#include "showz_ps20b.inc"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

static ConVar r_showz_power( "r_showz_power", "1.0f", FCVAR_CHEAT );


BEGIN_VS_SHADER_FLAGS( showz, "Help for ShowZ", SHADER_NOT_EDITABLE )
	BEGIN_SHADER_PARAMS
		SHADER_PARAM( ALPHADEPTH, SHADER_PARAM_TYPE_INTEGER, "0", "Depth is stored in alpha channel" )
	END_SHADER_PARAMS

	SHADER_INIT_PARAMS()
	{
		if ( !params[ALPHADEPTH]->IsDefined() )
		{
			params[ALPHADEPTH]->SetIntValue( 0 );
		}
	}

	SHADER_FALLBACK
	{
		return 0;
	}

	SHADER_INIT
	{
	}

	SHADER_DRAW
	{
		SHADOW_STATE
		{
			pShaderShadow->EnableTexture( SHADER_SAMPLER0, true );

			showz_vs11_Static_Index vshIndex;
			pShaderShadow->SetVertexShader( "showz_vs11", vshIndex.GetIndex() );

			int nShadowFilterMode = g_pHardwareConfig->GetShadowFilterMode();	// Based upon vendor and device dependent formats

			if( g_pHardwareConfig->SupportsPixelShaders_2_b() )
			{
				DECLARE_STATIC_PIXEL_SHADER( showz_ps20b );
				SET_STATIC_PIXEL_SHADER_COMBO( FLASHLIGHTDEPTHFILTERMODE, nShadowFilterMode );
				SET_STATIC_PIXEL_SHADER_COMBO( DEPTH_IN_ALPHA, params[ALPHADEPTH]->GetIntValue() );
				SET_STATIC_PIXEL_SHADER( showz_ps20b );
			}
			else
			{
				DECLARE_STATIC_PIXEL_SHADER( showz_ps20 );
				SET_STATIC_PIXEL_SHADER_COMBO( DEPTH_IN_ALPHA, params[ALPHADEPTH]->GetIntValue() );
				SET_STATIC_PIXEL_SHADER( showz_ps20 );
			}

			pShaderShadow->VertexShaderVertexFormat( VERTEX_POSITION, 1, 0, 0 );

			pShaderShadow->EnableSRGBWrite( false );
		}
		DYNAMIC_STATE
		{
			BindTexture( SHADER_SAMPLER0, BASETEXTURE, FRAME );	// Bind shadow depth map

			showz_vs11_Dynamic_Index vshIndex;
			pShaderAPI->SetVertexShaderIndex( vshIndex.GetIndex() );

			if( g_pHardwareConfig->SupportsPixelShaders_2_b() )
			{
				DECLARE_DYNAMIC_PIXEL_SHADER( showz_ps20b );
				SET_DYNAMIC_PIXEL_SHADER( showz_ps20b );
			}
			else
			{
				DECLARE_DYNAMIC_PIXEL_SHADER( showz_ps20 );
				SET_DYNAMIC_PIXEL_SHADER( showz_ps20 );
			}

			Vector4D C0;
			C0.x = r_showz_power.GetFloat();

			pShaderAPI->SetPixelShaderConstant( 0, C0.Base(), 1 );
		}
		Draw();
	}
END_SHADER

