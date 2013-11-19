//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#ifndef WEAPON_C4_H
#define WEAPON_C4_H
#ifdef _WIN32
#pragma once
#endif


#include "weapon_csbase.h"
#include "utlvector.h"

#define NUM_BEEPS 7

#if defined( CLIENT_DLL )

	#define CC4 C_C4

#else

	// ------------------------------------------------------------------------------------------ //
	// CPlantedC4 class.
	// ------------------------------------------------------------------------------------------ //

	class CPlantedC4 : public CBaseAnimating
	{
	public:
		DECLARE_CLASS( CPlantedC4, CBaseAnimating );
		DECLARE_DATADESC();
		DECLARE_SERVERCLASS();

		DECLARE_PREDICTABLE();

		CPlantedC4();
		virtual ~CPlantedC4();

		virtual int  UpdateTransmitState();
		virtual void SetTransmit( CCheckTransmitInfo *pInfo, bool bAlways );
		virtual int  ShouldTransmit( const CCheckTransmitInfo *pInfo );
		
		static CPlantedC4* ShootSatchelCharge( CCSPlayer *pevOwner, Vector vecStart, QAngle vecAngles );
		virtual void Precache();
		
		// Set these flags so CTs can use the C4 to disarm it.
		virtual int	ObjectCaps() { return BaseClass::ObjectCaps() | (FCAP_CONTINUOUS_USE | FCAP_USE_IN_RADIUS); }

		void SetBombSiteIndex( int iIndex ){ m_iBombSiteIndex = iIndex;	}

		inline bool IsBombActive( void ) { return m_bBombTicking; }
		
	public:

		CNetworkVar( bool, m_bBombTicking );
		CNetworkVar( float, m_flC4Blow );

	private:

		void Init( CCSPlayer *pevOwner, Vector vecStart, QAngle vecAngles );
		void C4Think();
		
		// This becomes the think function when the timer has expired and it is about to explode.
		void DetonateThink();
		void Explode( trace_t *pTrace, int bitsDamageType );

		void Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );

		// Replicate timer length to the client for effects
		CNetworkVar( float, m_flTimerLength );

		// Info for defusing.
		CHandle<CCSPlayer> m_pBombDefuser;
		float			m_flNextDefuse;
		bool			m_bStartDefuse;
		int				m_iBombSiteIndex;

		CNetworkVar( float, m_flDefuseLength );		//How long does the defuse take? Depends on if a defuser was used
		CNetworkVar( float, m_flDefuseCountDown );	//What time does the defuse complete?

		// Control panel
		void GetControlPanelInfo( int nPanelIndex, const char *&pPanelName );
		void GetControlPanelClassName( int nPanelIndex, const char *&pPanelName );
		void SpawnControlPanels();

		typedef CHandle<CVGuiScreen>	ScreenHandle_t;
		CUtlVector<ScreenHandle_t>	m_hScreens;

		int m_iProgressBarTime;
	};

	extern CUtlVector< CPlantedC4* > g_PlantedC4s;

#endif

#define WEAPON_C4_CLASSNAME "weapon_c4"
#define PLANTED_C4_CLASSNAME "planted_c4"

class CC4 : public CWeaponCSBase
{
public:
	DECLARE_CLASS( CC4, CWeaponCSBase );
	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();
	
	CC4();
	virtual ~CC4();
	
	virtual void Spawn();

	bool IsPistol() const;
	void ItemPostFrame();
	virtual void PrimaryAttack();
	virtual void WeaponIdle();
	virtual void UpdateShieldState( void );

	virtual CSWeaponID GetWeaponID( void ) const		{ return WEAPON_C4; }

	#ifdef CLIENT_DLL

		virtual bool OnFireEvent( C_BaseViewModel *pViewModel, const Vector& origin, const QAngle& angles, int event, const char *options );
		char *GetScreenText( void );
		char m_szScreenText[32];
		
	#else
		
		virtual void Precache();
		virtual void GetControlPanelInfo( int nPanelIndex, const char *&pPanelName );
		virtual unsigned int PhysicsSolidMaskForEntity( void ) const;

		virtual bool Holster( CBaseCombatWeapon *pSwitchingTo );
		virtual bool ShouldRemoveOnRoundRestart();
	
	#endif

	void PlayArmingBeeps( void );
	virtual void	OnPickedUp( CBaseCombatCharacter *pNewOwner );
	virtual void	Drop( const Vector &vecVelocity );

	CNetworkVar( bool, m_bStartedArming );
	CNetworkVar( float, m_fArmedTime );
	CNetworkVar( bool, m_bBombPlacedAnimation );

	virtual bool IsRemoveable( void ) { return false; }

private:	
	bool m_bPlayedArmingBeeps[NUM_BEEPS];
	bool m_bBombPlanted;

private:
	
	CC4( const CC4 & );
};


// All the currently-active C4 grenades.
extern CUtlVector< CC4* > g_C4s;


#endif // WEAPON_C4_H
