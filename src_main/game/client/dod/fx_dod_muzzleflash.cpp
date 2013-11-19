//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//

#include "cbase.h"
#include "particles_simple.h"
#include "particles_localspace.h"
#include "c_te_effect_dispatch.h"
#include "ClientEffectPrecacheSystem.h"

#include "fx.h"
#include "r_efx.h"
#include "dlight.h"
#include "dod_shareddefs.h"

// Precache our effects
CLIENTEFFECT_REGISTER_BEGIN( PrecacheEffect_DOD_MuzzleFlash )
CLIENTEFFECT_MATERIAL( "sprites/effects/muzzleflash1" )
CLIENTEFFECT_MATERIAL( "sprites/effects/muzzleflash2" )
CLIENTEFFECT_REGISTER_END()

ConVar cl_muzzleflash_dlight_1st( "cl_muzzleflash_dlight_1st", "1" );

void TE_DynamicLight( IRecipientFilter& filter, float delay,
	const Vector* org, int r, int g, int b, int exponent, float radius, float time, float decay, int nLightIndex = LIGHT_INDEX_TE_DYNAMIC );

#define DOD_MUZZLEFLASH_MG 3

void DOD_3rdPersonMuzzleFlashCallback( const CEffectData &data )
{
	float scale = data.m_flMagnitude;
	int attachmentIndex = data.m_nAttachmentIndex;
	
	CSmartPtr<CLocalSpaceEmitter> pSimple = CLocalSpaceEmitter::Create( "MuzzleFlash", data.m_hEntity, attachmentIndex, 0 );
	
	SimpleParticle *pParticle;
	Vector			forward(1,0,0), offset, right(0,1,0);

	//
	// Flash
	//

	if ( data.m_nHitBox > 0 )	// >0 is a mg flash in script based muzzleflashes ..
	{
		offset = vec3_origin;

		//MATTTODO: billboard large muzzleflash for mgs
		//BUG is that the sprite always rotates to face you.
		/*
		// large shaped flash
		SimpleParticle *pParticle = (SimpleParticle *)pSimple->AddParticle( sizeof( SimpleParticle ),
															pSimple->GetPMaterial( VarArgs( "sprites/effects/muzzleflash1" ) ),
															offset );
		Assert( pParticle );
		if( pParticle )
		{
			pParticle->m_flLifetime		= 0.0f;
			pParticle->m_flDieTime		= 0.1f;

			pParticle->m_vecVelocity = Vector(0,0,0);

			pParticle->m_uchColor[0]	= 255;
			pParticle->m_uchColor[1]	= 255;
			pParticle->m_uchColor[2]	= 255;

			pParticle->m_uchStartAlpha	= 210.0f;
			pParticle->m_uchEndAlpha	= 0;

			pParticle->m_uchStartSize	= random->RandomFloat( 80, 100 ) * data.m_flMagnitude;
			pParticle->m_uchEndSize		= pParticle->m_uchStartSize;
			pParticle->m_flRoll			= 0;
			pParticle->m_flRollDelta	= 0.0f;
		}
		*/

		// small inner cone
		scale *= 4;
		float flScale = random->RandomFloat( scale-0.1f, scale+0.1f );

		int i;
		for ( i = 1; i < 9; i++ )
		{
			offset = (forward * (i*2.0f*scale));

			pParticle = (SimpleParticle *) pSimple->AddParticle( sizeof( SimpleParticle ), pSimple->GetPMaterial( VarArgs( "effects/muzzleflash%d", random->RandomInt(1,4) ) ), offset );
				
			if ( pParticle == NULL )
				return;

			pParticle->m_flLifetime		= 0.0f;
			pParticle->m_flDieTime		= 0.1f;

			pParticle->m_vecVelocity.Init();

			pParticle->m_uchColor[0]	= 255;
			pParticle->m_uchColor[1]	= 255;
			pParticle->m_uchColor[2]	= 255;

			pParticle->m_uchStartAlpha	= 255;
			pParticle->m_uchEndAlpha	= 128;

			pParticle->m_uchStartSize	= (random->RandomFloat( 6.0f, 9.0f ) * (12-(i))/9) * flScale;
			pParticle->m_uchEndSize		= pParticle->m_uchStartSize;
			pParticle->m_flRoll			= random->RandomInt( 0, 360 );
			pParticle->m_flRollDelta	= 0.0f;
		}
	}
	else
	{
		scale *= 4;
		float flScale = random->RandomFloat( scale-0.1f, scale+0.1f );
		
		int i;
		for ( i = 1; i < 9; i++ )
		{
			offset = (forward * (i*2.0f*scale));

			pParticle = (SimpleParticle *) pSimple->AddParticle( sizeof( SimpleParticle ), pSimple->GetPMaterial( VarArgs( "effects/muzzleflash%d", random->RandomInt(1,4) ) ), offset );
				
			if ( pParticle == NULL )
				return;

			pParticle->m_flLifetime		= 0.0f;
			pParticle->m_flDieTime		= 0.1f;

			pParticle->m_vecVelocity.Init();

			pParticle->m_uchColor[0]	= 255;
			pParticle->m_uchColor[1]	= 255;
			pParticle->m_uchColor[2]	= 255;

			pParticle->m_uchStartAlpha	= 128;
			pParticle->m_uchEndAlpha	= 64;

			pParticle->m_uchStartSize	= (random->RandomFloat( 6.0f, 9.0f ) * (12-(i))/9) * flScale;
			pParticle->m_uchEndSize		= pParticle->m_uchStartSize;
			pParticle->m_flRoll			= random->RandomInt( 0, 360 );
			pParticle->m_flRollDelta	= 0.0f;
		}
	}

	//Smoke
	int i;
	offset = vec3_origin;
	for( i=0;i<3;i++ )
	{
		SimpleParticle *pParticle = (SimpleParticle *) pSimple->AddParticle( sizeof( SimpleParticle ), g_Mat_DustPuff[0], offset );
			
		if ( pParticle )
		{
			pParticle->m_flLifetime		= 0.0f;
			pParticle->m_flDieTime		= 0.3f;

			pParticle->m_vecVelocity.Init();
			pParticle->m_vecVelocity = forward * ( random->RandomFloat( 80.0f, 100.0f ) + (2-i)*15 );

			int color = random->RandomInt( 200, 255 );
			pParticle->m_uchColor[0]	= color;
			pParticle->m_uchColor[1]	= color;
			pParticle->m_uchColor[2]	= color;

			pParticle->m_uchStartAlpha	= 32;
			pParticle->m_uchEndAlpha	= 0;

			pParticle->m_uchStartSize	= ( 4.0 + 3.0*(2-i) ) * data.m_flMagnitude;
			pParticle->m_uchEndSize		= pParticle->m_uchStartSize * 13.0f;
			pParticle->m_flRoll			= random->RandomInt( 0, 360 );
			pParticle->m_flRollDelta	= random->RandomFloat( -0.5f, 0.5f );
		}
	}	
}

void DOD_MuzzleFlashCallback( const CEffectData &data )
{
	CSmartPtr<CLocalSpaceEmitter> pEmitter = 
		CLocalSpaceEmitter::Create( "DOD_MuzzleFlash", data.m_hEntity, data.m_nAttachmentIndex, 0 );

	if ( !pEmitter )
		return;

	// SetBBox() manually on the particle system so it doesn't have to be recalculated more than once.
	Vector vCenter( 0.0f, 0.0f, 0.0f );
	C_BaseEntity *pEnt = data.GetEntity();
	if ( pEnt )
	{
		vCenter = pEnt->WorldSpaceCenter();
	}
	else
	{
		IClientRenderable *pRenderable = data.GetRenderable( );
		if ( pRenderable )
		{
			Vector vecMins, vecMaxs;
			pRenderable->GetRenderBoundsWorldspace( vecMins, vecMaxs );
			VectorAdd( vecMins, vecMaxs, vCenter );
			vCenter *= 0.5f;
		}
	}

	Assert( pEmitter );
	pEmitter->GetBinding().SetBBox( vCenter - Vector( 10, 10, 10 ), vCenter + Vector( 10, 10, 10 ) );

	// haxors - make the clip much shorter so the alpha is not 
	// changed based on large clip distances
	pEmitter->SetNearClip( 0, 5 );

	Vector vFlashOffset = vec3_origin;
	Vector vForward(1,0,0);
	int i;

	if( data.m_nHitBox == DOD_MUZZLEFLASH_MG )	//Machine gun
	{
		SimpleParticle *pParticle = (SimpleParticle *)pEmitter->AddParticle( sizeof( SimpleParticle ),
															g_Mat_SMG_Muzzleflash[0],
															vFlashOffset );
		Assert( pParticle );
		if( pParticle )
		{	
			pParticle->m_flLifetime		= 0.0f;
			pParticle->m_flDieTime		= 0.1f;

			pParticle->m_vecVelocity = Vector(0,0,0);

			pParticle->m_uchColor[0]	= 255;
			pParticle->m_uchColor[1]	= 255;
			pParticle->m_uchColor[2]	= 255;

			pParticle->m_uchStartAlpha	= 210.0f;
			pParticle->m_uchEndAlpha	= 0;

			pParticle->m_uchStartSize	= random->RandomFloat( 120, 130 ) * data.m_flMagnitude;
			pParticle->m_uchEndSize		= pParticle->m_uchStartSize;
			pParticle->m_flRoll			= 0;
			pParticle->m_flRollDelta	= 0.0f;
		}
	}
	else
	{
		for( i=0;i<3;i++ )
		{
			//move the three sprites around

			vFlashOffset = vForward * ( (2-i)*6 + 10 );

			SimpleParticle *pParticle = (SimpleParticle *)pEmitter->AddParticle( sizeof( SimpleParticle ),
																g_Mat_SMG_Muzzleflash[1],
																vFlashOffset );
			Assert( pParticle );
			if( pParticle )
			{
				pParticle->m_flLifetime		= 0.0f;
				pParticle->m_flDieTime		= 0.1f;

				pParticle->m_vecVelocity = vec3_origin;

				pParticle->m_uchColor[0]	= 255;
				pParticle->m_uchColor[1]	= 255;
				pParticle->m_uchColor[2]	= 255;

				pParticle->m_uchStartAlpha	= 100.0f;
				pParticle->m_uchEndAlpha	= 30;

				pParticle->m_uchStartSize	= ( 15.0 + 20.0*i ) * data.m_flMagnitude;
				pParticle->m_uchEndSize		= pParticle->m_uchStartSize;
				pParticle->m_flRoll			= random->RandomInt( 0, 360 );
				pParticle->m_flRollDelta	= 0.0f;
			}
		}
	}

	// dynamic light temporary entity for the muzzle flash
	if ( cl_muzzleflash_dlight_1st.GetBool() )
	{
		CPVSFilter filter(pEmitter->GetSortOrigin());
		TE_DynamicLight( filter, 0.0, &(pEmitter->GetSortOrigin()), 255, 192, 64, 5, 70, 0.05, 768 );
	}
 
	/*
	//Smoke
	for( i=0;i<3;i++ )
	{
		SimpleParticle *pParticle = (SimpleParticle *) pEmitter->AddParticle( sizeof( SimpleParticle ), pEmitter->GetPMaterial( "particle/particle_smokegrenade" ), vFlashOffset );
			
		if ( pParticle )
		{
			pParticle->m_flLifetime		= 0.0f;
			pParticle->m_flDieTime		= 0.4f;

			pParticle->m_vecVelocity.Init();
			pParticle->m_vecVelocity = vForward * ( random->RandomFloat( 150.0f, 170.0f ) + (2-i)*30 ); 

			int color = random->RandomInt( 200, 255 );
			pParticle->m_uchColor[0]	= color;
			pParticle->m_uchColor[1]	= color;
			pParticle->m_uchColor[2]	= color;

			pParticle->m_uchStartAlpha	= 32;
			pParticle->m_uchEndAlpha	= 0;

			pParticle->m_uchStartSize	= ( 3.0 + 3.0*(2-i) ) * data.m_flMagnitude;
			pParticle->m_uchEndSize		= pParticle->m_uchStartSize * 12.0f;
			pParticle->m_flRoll			= random->RandomInt( 0, 360 );
			pParticle->m_flRollDelta	= random->RandomFloat( -0.5f, 0.5f );
		}
	}	
	*/
}

DECLARE_CLIENT_EFFECT( "DOD_MuzzleFlash", DOD_MuzzleFlashCallback );
DECLARE_CLIENT_EFFECT( "DOD_3rdPersonMuzzleFlash", DOD_3rdPersonMuzzleFlashCallback );