//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $Header: $
// $NoKeywords: $
//=============================================================================//

#include "shaderlib/CShader.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

BEGIN_SHADER_FLAGS( Fillrate_DX6, "Help for Fillrate_DX6", SHADER_NOT_EDITABLE )
	BEGIN_SHADER_PARAMS
		SHADER_PARAM( PASSCOUNT, SHADER_PARAM_TYPE_INTEGER, "1", "Number of passes for this material" )
	END_SHADER_PARAMS

	SHADER_INIT_PARAMS()
	{
	}

	SHADER_INIT
	{
	}

	SHADER_DRAW
	{
		SHADOW_STATE
		{
			pShaderShadow->EnableDepthTest( false );
			pShaderShadow->EnableDepthWrites( false );
			pShaderShadow->EnableBlending( true );
			pShaderShadow->BlendFunc( SHADER_BLEND_ONE, SHADER_BLEND_ONE );
			pShaderShadow->EnableConstantColor( true );
			pShaderShadow->DrawFlags( SHADER_DRAW_POSITION );
		}
		DYNAMIC_STATE
		{
			int numPasses = params[PASSCOUNT]->GetIntValue();
			if (g_pConfig->bMeasureFillRate)
			{
				pShaderAPI->Color3ub( 1 * numPasses, 0, 255 );
			}
			else
			{
				pShaderAPI->Color3ub( 16 * numPasses, 0, 255 );
			}
		}
		Draw();

		SHADOW_STATE
		{
			pShaderShadow->EnableDepthTest( false );
			pShaderShadow->EnableConstantColor( true );
			pShaderShadow->PolyMode( SHADER_POLYMODEFACE_FRONT_AND_BACK, SHADER_POLYMODE_LINE );
			pShaderShadow->DrawFlags( SHADER_DRAW_POSITION );
		}
		DYNAMIC_STATE
		{
			pShaderAPI->Color3ub( 0, 255, 255 );
		}
		Draw();
	}
END_SHADER


