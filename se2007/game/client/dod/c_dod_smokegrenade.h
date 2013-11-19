//====== Copyright © 1996-2003, Valve Corporation, All rights reserved. =======
//
// Purpose: 
//
//=============================================================================

#ifndef C_DOD_SMOKEGRENADE_H
#define C_DOD_SMOKEGRENADE_H
#ifdef _WIN32
#pragma once
#endif

#include "c_dod_basegrenade.h"

class CNewParticleEffect;


class C_DODSmokeGrenade : public C_DODBaseGrenade
{
public:
	DECLARE_CLASS( C_DODSmokeGrenade, C_DODBaseGrenade );
	DECLARE_NETWORKCLASS(); 

	C_DODSmokeGrenade();
	virtual ~C_DODSmokeGrenade();

	virtual void OnDataChanged( DataUpdateType_t updateType );
	void Start( void );

	virtual int DrawModel( int flags );
	void Update( float flTimeDelta );

	virtual const char *GetOverviewSpriteName( void );
	virtual void CleanupToolRecordingState( KeyValues *msg );

	CNetworkVar( bool, m_bEmitSmoke );

private:
	TimedEvent		m_ParticleSpawn;
	CSmartPtr<CSimpleEmitter> m_pEmitter;
	PMaterialHandle	m_MaterialHandle;
};


#endif // C_DOD_SMOKEGRENADE_H
