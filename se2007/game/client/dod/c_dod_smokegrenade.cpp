//====== Copyright © 1996-2003, Valve Corporation, All rights reserved. =======
//
// Purpose: 
//
//=============================================================================

#include "cbase.h"
#include "c_dod_smokegrenade.h"
#include "dod_shareddefs.h"
#include "tier1/keyvalues.h"
#include "toolframework_client.h"
#include "particles_simple.h"
#include "particles_new.h"
#include "fx.h"

IMPLEMENT_NETWORKCLASS_ALIASED( DODSmokeGrenade, DT_DODSmokeGrenade )

BEGIN_RECV_TABLE(C_DODSmokeGrenade, DT_DODSmokeGrenade )
	RecvPropBool( RECVINFO( m_bEmitSmoke ) )
END_NETWORK_TABLE()

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
C_DODSmokeGrenade::C_DODSmokeGrenade( void )
{
	m_bEmitSmoke = false;
	m_pEmitter = NULL;
}

C_DODSmokeGrenade::~C_DODSmokeGrenade()
{
	if ( clienttools->IsInRecordingMode() && m_pEmitter.IsValid() && m_pEmitter->GetToolParticleEffectId() != TOOLPARTICLESYSTEMID_INVALID )
	{
		KeyValues *msg = new KeyValues( "OldParticleSystem_ActivateEmitter" );
		msg->SetInt( "id", m_pEmitter->GetToolParticleEffectId() );
		msg->SetInt( "emitter", 0 );
		msg->SetInt( "active", false );
		msg->SetFloat( "time", gpGlobals->curtime );
		ToolFramework_PostToolMessage( HTOOLHANDLE_INVALID, msg );
		msg->deleteThis();
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : bnewentity - 
//-----------------------------------------------------------------------------
void C_DODSmokeGrenade::OnDataChanged( DataUpdateType_t updateType )
{
	C_BaseEntity::OnDataChanged(updateType);

	if( updateType == DATA_UPDATE_CREATED )
	{
		Start();
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void C_DODSmokeGrenade::Start( void )
{
	Assert( m_pEmitter == NULL );

	m_ParticleSpawn.Init( 15 ); //Events per second

	//Create the basic emitter
	m_pEmitter = CSimpleEmitter::Create( "C_DODSmokeGrenade::m_pEmitter" );

	Assert( m_pEmitter.IsValid() );
	if ( m_pEmitter.IsValid() )
	{
		m_MaterialHandle = g_Mat_DustPuff[0];
		m_pEmitter->SetSortOrigin( GetAbsOrigin() );
	}

	ParticleProp()->Create( "dod_smoke_grenade_cloud", PATTACH_ABSORIGIN_FOLLOW );

}


//-----------------------------------------------------------------------------
// This is called after sending this entity's recording state
//-----------------------------------------------------------------------------
void C_DODSmokeGrenade::CleanupToolRecordingState( KeyValues *msg )
{
	BaseClass::CleanupToolRecordingState( msg );

	// Generally, this is used to allow the entity to clean up
	// allocated state it put into the message, but here we're going
	// to use it to send particle system messages because we
	// know the grenade has been recorded at this point
	if ( !clienttools->IsInRecordingMode() || !m_pEmitter.IsValid() )
		return;
	
	// NOTE: Particle system destruction message will be sent by the particle effect itself.
	if ( m_pEmitter->GetToolParticleEffectId() == TOOLPARTICLESYSTEMID_INVALID )
	{
		int nId = m_pEmitter->AllocateToolParticleEffectId();

		KeyValues *msg = new KeyValues( "OldParticleSystem_Create" );
		msg->SetString( "name", "C_DODSmokeGrenade" );
		msg->SetInt( "id", nId );
		msg->SetFloat( "time", gpGlobals->curtime );

		KeyValues *pEmitter = msg->FindKey( "DmeSpriteEmitter", true );
		pEmitter->SetInt( "count", 15 );	// particles per second, when duration is < 0
		pEmitter->SetFloat( "duration", -1 );
		pEmitter->SetString( "material", "particle/particle_smokegrenade1" );
		pEmitter->SetInt( "active", m_bEmitSmoke );

		KeyValues *pInitializers = pEmitter->FindKey( "initializers", true );

		KeyValues *pPosition = pInitializers->FindKey( "DmePositionEntityInitializer", true );
		pPosition->SetPtr( "entindex", (void*)entindex() );
 		pPosition->SetInt( "attachmentIndex", 0 );

		KeyValues *pLifetime = pInitializers->FindKey( "DmeRandomLifetimeInitializer", true );
		pLifetime->SetFloat( "minLifetime", 0.3f );
 		pLifetime->SetFloat( "maxLifetime", 0.3f );

		KeyValues *pVelocity = pInitializers->FindKey( "DmeAttachmentVelocityInitializer", true );
		pVelocity->SetPtr( "entindex", (void*)entindex() );
 		pVelocity->SetInt( "attachmentIndex", 0 );
		pVelocity->SetFloat( "minAttachmentSpeed", 150.0f );
 		pVelocity->SetFloat( "maxAttachmentSpeed", 180.0f );
 		pVelocity->SetFloat( "fixedVelZ", 50.0f );

		// NOTE: This produces a roughly 8 degree random velocity as below;
		// tan 8 degreees = 25 / 165 (165 = average of min + max attachment vel)
 		pVelocity->SetFloat( "minRandomSpeed", -25.0f );
 		pVelocity->SetFloat( "maxRandomSpeed", 25.0f );

		KeyValues *pRoll = pInitializers->FindKey( "DmeRandomRollInitializer", true );
		pRoll->SetFloat( "minRoll", 0.0f );
 		pRoll->SetFloat( "maxRoll", 360.0f );

		KeyValues *pRollSpeed = pInitializers->FindKey( "DmeRandomRollSpeedInitializer", true );
		pRollSpeed->SetFloat( "minRollSpeed", -4.0f );
 		pRollSpeed->SetFloat( "maxRollSpeed", 4.0f );

		KeyValues *pColor = pInitializers->FindKey( "DmeRandomInterpolatedColorInitializer", true );
		pColor->SetColor( "color1", Color( 80, 80, 80, 255 ) );
 		pColor->SetColor( "color2", Color( 100, 100, 100, 255 ) );

		KeyValues *pAlpha = pInitializers->FindKey( "DmeRandomAlphaInitializer", true );
		pAlpha->SetInt( "minStartAlpha", 250 );
		pAlpha->SetInt( "maxStartAlpha", 255 );
		pAlpha->SetInt( "minEndAlpha", 0 );
		pAlpha->SetInt( "maxEndAlpha", 0 );

		KeyValues *pSize = pInitializers->FindKey( "DmeRandomSizeInitializer", true );
		pSize->SetFloat( "minStartSize", 1 );
		pSize->SetFloat( "maxStartSize", 1 );
		pSize->SetFloat( "minEndSize", 15 );
		pSize->SetFloat( "maxEndSize", 20 );

		KeyValues *pUpdaters = pEmitter->FindKey( "updaters", true );

		pUpdaters->FindKey( "DmePositionVelocityUpdater", true );
		pUpdaters->FindKey( "DmeRollUpdater", true );
		pUpdaters->FindKey( "DmeAlphaLinearUpdater", true );
		pUpdaters->FindKey( "DmeSizeUpdater", true );

		ToolFramework_PostToolMessage( HTOOLHANDLE_INVALID, msg );
		msg->deleteThis();
	}
	else 
	{
		KeyValues *msg = new KeyValues( "OldParticleSystem_ActivateEmitter" );
		msg->SetInt( "id", m_pEmitter->GetToolParticleEffectId() );
		msg->SetInt( "emitter", 0 );
		msg->SetInt( "active", m_bEmitSmoke );
		msg->SetFloat( "time", gpGlobals->curtime );
		ToolFramework_PostToolMessage( HTOOLHANDLE_INVALID, msg );
		msg->deleteThis();
	}
}

	
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
int C_DODSmokeGrenade::DrawModel( int flags )
{
	if ( m_bEmitSmoke )
	{
		Update( Helper_GetFrameTime() );
	}

	return BaseClass::DrawModel( flags );
}

void C_DODSmokeGrenade::Update( float flTimeDelta )
{
	if ( !m_pEmitter.IsValid() )
		return;

	SimpleParticle	*pParticle;

	m_pEmitter->SetSortOrigin( GetAbsOrigin() );

	Vector vecEmitOrigin;
	QAngle vecEmitAngles;

	// Will set to GetAbsOrigin, GetAbsAngles if attachment is not found
	if ( GetAttachment( 0, vecEmitOrigin, vecEmitAngles ) == false )
	{
		//vecEmitOrigin = GetAbsOrigin();	//redundant, GetAttachment does this
		vecEmitAngles = vec3_angle;
	}

	while( m_ParticleSpawn.NextEvent( flTimeDelta ) )
	{
		pParticle = (SimpleParticle *) m_pEmitter->AddParticle( sizeof(SimpleParticle), m_MaterialHandle, vecEmitOrigin );

		if ( pParticle )
		{
			pParticle->m_flDieTime	= 0.3f;
			pParticle->m_flLifetime	= 0.0f;

			pParticle->m_flRoll		= random->RandomInt( 0, 360 );
			pParticle->m_flRollDelta= random->RandomFloat( -4.0f, 4.0f );

			pParticle->m_uchStartSize	= 1;
			pParticle->m_uchEndSize		= random->RandomInt( 15, 20 );
			pParticle->m_uchStartAlpha	= random->RandomInt( 250, 255 );
			pParticle->m_uchEndAlpha	= 0;

			int	cScale = random->RandomInt( 80, 100 );
			pParticle->m_uchColor[0]	= cScale;
			pParticle->m_uchColor[1]	= cScale;
			pParticle->m_uchColor[2]	= cScale;

			Vector	dir;
			QAngle  ofsAngles;

			ofsAngles.Random( -8.0f, 8.0f );
			ofsAngles += vecEmitAngles;

			AngleVectors( ofsAngles, &dir );

			pParticle->m_vecVelocity	= dir * random->RandomInt( 150, 180 );

			pParticle->m_vecVelocity.z += 50;
		}
	}
}

const char *C_DODSmokeGrenade::GetOverviewSpriteName( void )
{
	const char *pszSprite = "";

	switch( GetTeamNumber() )
	{
	case TEAM_ALLIES:
		pszSprite = "sprites/minimap_icons/minimap_smoke_us";
		break;
	case TEAM_AXIS:
		pszSprite = "sprites/minimap_icons/minimap_smoke_ger";
		break;
	default:
		break;
	}

	return pszSprite;
}
