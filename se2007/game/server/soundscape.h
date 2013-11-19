//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#ifndef SOUNDSCAPE_H
#define SOUNDSCAPE_H
#ifdef _WIN32
#pragma once
#endif


class CEnvSoundscape : public CPointEntity
{
public:
	DECLARE_CLASS( CEnvSoundscape, CPointEntity );
	DECLARE_DATADESC();

	CEnvSoundscape();
	~CEnvSoundscape();

	bool KeyValue( const char *szKeyName, const char *szValue );
	void Spawn( void );
	void Precache( void );
	void Update( void );
	void WriteAudioParamsTo( audioparams_t &audio );
	virtual int UpdateTransmitState();
	bool InRangeOfPlayer( CBasePlayer *pPlayer );
	void DrawDebugGeometryOverlays( void );

	void InputEnable( inputdata_t &inputdata );
	void InputDisable( inputdata_t &inputdata );
	void InputToggleEnabled( inputdata_t &inputdata );

	string_t GetSoundscapeName() const {return m_soundscapeName;}


private:

	bool IsEnabled( void ) const;
	void Disable( void );
	void Enable( void );

	bool UpdatePlayersInPVS();


public:
	COutputEvent	m_OnPlay;
	float	m_flRadius;
	string_t m_soundscapeName;
	int		m_soundscapeIndex;
	string_t m_positionNames[NUM_AUDIO_LOCAL_SOUNDS];
	
	// If this is set, then this soundscape ignores all its parameters and uses
	// those of this soundscape.
	CHandle<CEnvSoundscape> m_hProxySoundscape;


private:

	// Updated every couple seconds. Then we check against these players often.
	CUtlVector<CBasePlayerHandle> m_hPlayersInPVS;

	// Next time to update the m_hPlayersInPVS list.
	float m_flNextUpdatePlayersInPVS;
	
	bool	m_bDisabled;
};


class CEnvSoundscapeProxy : public CEnvSoundscape
{
public:
	DECLARE_CLASS( CEnvSoundscapeProxy, CEnvSoundscape );
	DECLARE_DATADESC();

	CEnvSoundscapeProxy();
	virtual void Activate();

	// Here just to stop it falling back to CEnvSoundscape's, and
	// printing bogus errors about missing soundscapes.
	virtual void Precache() { return; }

private:
	string_t m_MainSoundscapeName;
};


class CEnvSoundscapeTriggerable : public CEnvSoundscape
{
friend class CTriggerSoundscape;

public:
	DECLARE_CLASS( CEnvSoundscapeTriggerable, CEnvSoundscape );
	DECLARE_DATADESC();

	CEnvSoundscapeTriggerable();
	
	// Overrides the base class's think and prevents it from running at all.
	virtual void Think();


private:

	// Passed through from CTriggerSoundscape.
	void DelegateStartTouch( CBaseEntity *pEnt );
	void DelegateEndTouch( CBaseEntity *pEnt );
};


#endif // SOUNDSCAPE_H
