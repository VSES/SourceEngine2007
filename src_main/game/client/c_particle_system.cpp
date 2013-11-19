//====== Copyright © 1996-2005, Valve Corporation, All rights reserved. =======
//
// Purpose: 
//
//=============================================================================

#include "cbase.h"
#include "particles/particles.h"
#include "c_te_effect_dispatch.h"
#include "particles_new.h"
#include "networkstringtable_clientdll.h"
#include "tier0/vprof.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//-----------------------------------------------------------------------------
// Purpose: An entity that spawns and controls a particle system
//-----------------------------------------------------------------------------
class C_ParticleSystem : public C_BaseEntity
{
	DECLARE_CLASS( C_ParticleSystem, C_BaseEntity );
public:
	DECLARE_CLIENTCLASS();

	void PreDataUpdate( DataUpdateType_t updateType );
	void PostDataUpdate( DataUpdateType_t updateType );
	void ClientThink( void );

protected:
	int			m_iEffectIndex;
	bool		m_bActive;
	bool		m_bOldActive;
	float		m_flStartTime;	// Time at which the effect started

	enum { kMAXCONTROLPOINTS = 63 }; ///< actually one less than the total number of cpoints since 0 is assumed to be me

	
	EHANDLE		m_hControlPointEnts[kMAXCONTROLPOINTS];
	//	SendPropArray3( SENDINFO_ARRAY3(m_iControlPointParents), SendPropInt( SENDINFO_ARRAY(m_iControlPointParents), 3, SPROP_UNSIGNED ) ),
	unsigned char m_iControlPointParents[kMAXCONTROLPOINTS];
};

IMPLEMENT_CLIENTCLASS(C_ParticleSystem, DT_ParticleSystem, CParticleSystem);

BEGIN_RECV_TABLE_NOBASE( C_ParticleSystem, DT_ParticleSystem )
	RecvPropVector( RECVINFO_NAME( m_vecNetworkOrigin, m_vecOrigin ) ),
	RecvPropEHandle( RECVINFO(m_hOwnerEntity) ),
	RecvPropInt( RECVINFO_NAME(m_hNetworkMoveParent, moveparent), 0, RecvProxy_IntToMoveParent ),
	RecvPropInt( RECVINFO( m_iParentAttachment ) ),
	RecvPropQAngles( RECVINFO_NAME( m_angNetworkAngles, m_angRotation ) ),

	RecvPropInt( RECVINFO( m_iEffectIndex ) ),
	RecvPropBool( RECVINFO( m_bActive ) ),
	RecvPropFloat( RECVINFO( m_flStartTime ) ),

	RecvPropArray3( RECVINFO_ARRAY(m_hControlPointEnts), RecvPropEHandle( RECVINFO( m_hControlPointEnts[0] ) ) ),
	RecvPropArray3( RECVINFO_ARRAY(m_iControlPointParents), RecvPropInt( RECVINFO(m_iControlPointParents[0]))), 
END_RECV_TABLE();

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void C_ParticleSystem::PreDataUpdate( DataUpdateType_t updateType )
{
	m_bOldActive = m_bActive;

	BaseClass::PreDataUpdate( updateType );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void C_ParticleSystem::PostDataUpdate( DataUpdateType_t updateType )
{
	BaseClass::PostDataUpdate( updateType );

	// Always restart if just created and updated
	// FIXME: Does this play fairly with PVS?
	if ( updateType == DATA_UPDATE_CREATED )
	{
		if ( m_bActive )
		{
			// Delayed here so that we don't get invalid abs queries on level init with active particle systems
			SetNextClientThink( gpGlobals->curtime );
		}
	}
	else
	{
		if ( m_bOldActive != m_bActive )
		{
			if ( m_bActive )
			{
				// Delayed here so that we don't get invalid abs queries on level init with active particle systems
				SetNextClientThink( gpGlobals->curtime );
			}
			else
			{
				ParticleProp()->StopEmission();
			}
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void C_ParticleSystem::ClientThink( void )
{
	if ( m_bActive )
	{
		const char *pszName = GetParticleSystemNameFromIndex( m_iEffectIndex );
		if ( pszName && pszName[0] )
		{
			CNewParticleEffect *pEffect = ParticleProp()->Create( pszName, PATTACH_ABSORIGIN_FOLLOW );
			AssertMsg1( pEffect, "Particle system couldn't make %s", pszName );
			if (pEffect)
			{
				for ( int i = 0 ; i < kMAXCONTROLPOINTS ; ++i )
				{
					CBaseEntity *pOnEntity = m_hControlPointEnts[i].Get();
					if ( pOnEntity )
					{
						ParticleProp()->AddControlPoint( pEffect, i + 1, pOnEntity, PATTACH_ABSORIGIN_FOLLOW );
					}

					AssertMsg2( m_iControlPointParents[i] >= 0 && m_iControlPointParents[i] <= kMAXCONTROLPOINTS ,
						"Particle system specified bogus control point parent (%d) for point %d.",
						m_iControlPointParents[i], i );

					if (m_iControlPointParents[i] != 0)
					{
						pEffect->SetControlPointParent(i+1, m_iControlPointParents[i]);
					}
				}

				// NOTE: What we really want here is to compare our lifetime and that of our children and see if this delta is
				//		 already past the end of it, denoting that we're finished.  In that case, just destroy us and be done. -- jdw

				// TODO: This can go when the SkipToTime code below goes
				ParticleProp()->OnParticleSystemUpdated( pEffect, 0.0f );

				// Skip the effect ahead if we're restarting it
				float flTimeDelta = gpGlobals->curtime - m_flStartTime;
				if ( flTimeDelta > 0.01f )
				{
					VPROF_BUDGET( "C_ParticleSystem::ClientThink SkipToTime", "Particle Simulation" );
					pEffect->SkipToTime( flTimeDelta );
				}
			}
		}
	}
}


//======================================================================================================================
// PARTICLE SYSTEM DISPATCH EFFECT
//======================================================================================================================
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void ParticleEffectCallback( const CEffectData &data )
{
	if ( SuppressingParticleEffects() )
		return; // this needs to be before using data.m_nHitBox, since that may be a serialized value that's past the end of the current particle system string table

	const char *pszName = GetParticleSystemNameFromIndex( data.m_nHitBox );

	if ( data.m_fFlags & PARTICLE_DISPATCH_FROM_ENTITY )
	{
		if ( data.m_hEntity.Get() )
		{
			C_BaseEntity *pEnt = C_BaseEntity::Instance( data.m_hEntity );
			if ( pEnt && !pEnt->IsDormant() )
			{
				if ( data.m_fFlags & PARTICLE_DISPATCH_RESET_PARTICLES )
				{
					pEnt->ParticleProp()->StopEmission();
				}

				CSmartPtr<CNewParticleEffect> pEffect = pEnt->ParticleProp()->Create( pszName, (ParticleAttachment_t)data.m_nDamageType, data.m_nAttachmentIndex );
				AssertMsg2( pEffect.IsValid() && pEffect->IsValid(), "%s could not create particle effect %s",
					C_BaseEntity::Instance( data.m_hEntity )->GetDebugName(), pszName );
				if ( pEffect.IsValid() && pEffect->IsValid() )
				{
					if ( (ParticleAttachment_t)data.m_nDamageType == PATTACH_CUSTOMORIGIN )
					{
						pEffect->SetSortOrigin( data.m_vOrigin );
						pEffect->SetControlPoint( 0, data.m_vOrigin );
						pEffect->SetControlPoint( 1, data.m_vStart );
						Vector vecForward, vecRight, vecUp;
						AngleVectors( data.m_vAngles, &vecForward, &vecRight, &vecUp );
						pEffect->SetControlPointOrientation( 0, vecForward, vecRight, vecUp );
					}
				}
			}
		}
	}	
	else
	{
		CSmartPtr<CNewParticleEffect> pEffect = CNewParticleEffect::Create( NULL, pszName );
		if ( pEffect->IsValid() )
		{
			pEffect->SetSortOrigin( data.m_vOrigin );
			pEffect->SetControlPoint( 0, data.m_vOrigin );
			pEffect->SetControlPoint( 1, data.m_vStart );
			Vector vecForward, vecRight, vecUp;
			AngleVectors( data.m_vAngles, &vecForward, &vecRight, &vecUp );
			pEffect->SetControlPointOrientation( 0, vecForward, vecRight, vecUp );
		}
	}
}

DECLARE_CLIENT_EFFECT( "ParticleEffect", ParticleEffectCallback );


//======================================================================================================================
// PARTICLE SYSTEM STOP EFFECT
//======================================================================================================================
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void ParticleEffectStopCallback( const CEffectData &data )
{
	if ( data.m_hEntity.Get() )
	{
		C_BaseEntity *pEnt = C_BaseEntity::Instance( data.m_hEntity );
		if ( pEnt )
		{
			pEnt->ParticleProp()->StopEmission();
		}
	}
}

DECLARE_CLIENT_EFFECT( "ParticleEffectStop", ParticleEffectStopCallback );
