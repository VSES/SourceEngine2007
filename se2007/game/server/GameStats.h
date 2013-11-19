//====== Copyright © 1996-2005, Valve Corporation, All rights reserved. =======
//
// Purpose: 
//
//=============================================================================

#ifndef GAMESTATS_H
#define GAMESTATS_H
#ifdef _WIN32
#pragma once
#endif

#include "tier1/utldict.h"
#include "tier1/utlbuffer.h"

#define GAMESTATS_STANDARD_NOT_SAVED 0xFEEDBEEF

enum GameStatsVersions_t
{
	GAMESTATS_FILE_VERSION_OLD = 001,
	GAMESTATS_FILE_VERSION_OLD2,
	GAMESTATS_FILE_VERSION_OLD3,
	GAMESTATS_FILE_VERSION_OLD4,
	GAMESTATS_FILE_VERSION_OLD5,
	GAMESTATS_FILE_VERSION
};

struct BasicGameStatsRecord_t
{
public:
	BasicGameStatsRecord_t() :
	  m_nCount( 0 ),
		  m_nSeconds( 0 ),
		  m_nCommentary( 0 ),
		  m_nHDR( 0 ),
		  m_nCaptions( 0 ),
		  m_bSteam( true ),
		  m_bCyberCafe( false ),
		  m_nDeaths( 0 )
	  {
		  Q_memset( m_nSkill, 0, sizeof( m_nSkill ) );
	  }

	  void		Clear();

	  void		SaveToBuffer( CUtlBuffer& buf );
	  bool		ParseFromBuffer( CUtlBuffer& buf, int iBufferStatsVersion );

	  // Data
public:
	int			m_nCount;
	int			m_nSeconds;

	int			m_nCommentary;
	int			m_nHDR;
	int			m_nCaptions;
	int			m_nSkill[ 3 ];
	bool		m_bSteam;
	bool		m_bCyberCafe;
	int			m_nDeaths;
};

struct BasicGameStats_t
{
public:
	BasicGameStats_t() :
		  m_nSecondsToCompleteGame( 0 ),
		  m_nHL2ChaptureUnlocked( 0 ),
		  m_bSteam( true ),
		  m_bCyberCafe( false ),
		  m_nDXLevel( 0 )
	  {
	  }

	  void						Clear();

	  void						SaveToBuffer( CUtlBuffer& buf );
	  bool						ParseFromBuffer( CUtlBuffer& buf, int iBufferStatsVersion );

	  BasicGameStatsRecord_t	*FindOrAddRecordForMap( char const *mapname );

	  // Data
public:
	int							m_nSecondsToCompleteGame; // 0 means they haven't finished playing yet

	BasicGameStatsRecord_t		m_Summary;			// Summary record
	CUtlDict< BasicGameStatsRecord_t, unsigned short > m_MapTotals;
	bool						m_bSteam;
	bool						m_bCyberCafe;
	int							m_nHL2ChaptureUnlocked;
	int							m_nDXLevel;
};

class CBasePlayer;
class CPropVehicleDriveable;
class CTakeDamageInfo;

class CBaseGameStats
{
public:
	CBaseGameStats();

	void StatsLog( char const *fmt, ... );
	
	// This is the first call made, so that we can "subclass" the CBaseGameStats based on gamedir as needed (e.g., ep2 vs. episodic)
	virtual CBaseGameStats *OnInit( CBaseGameStats *pCurrentGameStats, char const *gamedir ) { return pCurrentGameStats; }

	// Frees up data from gamestats and resets it to a clean state.
	virtual void Clear( void );

	virtual bool StatTrackingEnabledForMod( void ) { return false; } //Override this to turn on the system. Stat tracking is disabled by default and will always be disabled at the user's request
	static bool StatTrackingAllowed( void ); //query whether stat tracking is possible and warranted by the user
	
	//not implemented yet
	//virtual bool VerboseEventTracking( void ) { return false; } //writes every standard event to a stream with a timestamp, probably only a good idea during internal tests
	//virtual bool ClearVerboseEventTrackingOnUpload( void ) { return true; } //no point in sending a stream of events, then resending the same stream later.... or is there?

	virtual bool ShouldTrackStandardStats( void ) { return true; } //exactly what was tracked for EP1 release
	
	//Get mod specific strings used for tracking, defaults should work fine for most cases
	virtual const char *GetStatSaveFileName( void );
	virtual const char *GetStatUploadRegistryKeyName( void );
	const char *GetUserPseudoUniqueID( void );

	virtual bool UserPlayedAllTheMaps( void ) { return false; } //be sure to override this to determine user completion time

	//default save behavior is to save on level shutdown, and game shutdown
	virtual bool AutoSave_OnInit( void ) { return false; }
	virtual bool AutoSave_OnShutdown( void ) { return true; }
	virtual bool AutoSave_OnMapChange( void ) { return false; }
	virtual bool AutoSave_OnLevelInit( void ) { return false; }
	virtual bool AutoSave_OnLevelShutdown( void ) { return true; }

	//default upload behavior is to upload on game shutdown
	virtual bool AutoUpload_OnInit( void ) { return false; }
	virtual bool AutoUpload_OnShutdown( void ) { return true; }
	virtual bool AutoUpload_OnMapChange( void ) { return false; }
	virtual bool AutoUpload_OnLevelInit( void ) { return false; }
	virtual bool AutoUpload_OnLevelShutdown( void ) { return false; }

	virtual void Event_Init( void );
	virtual void Event_Shutdown( void );
	virtual void Event_MapChange( const char *szOldMapName, const char *szNewMapName );
	virtual void Event_LevelInit( void );
	virtual void Event_LevelShutdown( float flElapsed );
	virtual void Event_SaveGame( void );
	virtual void Event_LoadGame( void );

	virtual void Event_PlayerKilled( CBasePlayer *pPlayer, const CTakeDamageInfo &info );
	
	virtual void Event_PlayerConnected( CBasePlayer *pBasePlayer );
	virtual void Event_PlayerDisconnected( CBasePlayer *pBasePlayer );
	virtual void Event_PlayerDamage( CBasePlayer *pBasePlayer, const CTakeDamageInfo &info );
	virtual void Event_PlayerKilledOther( CBasePlayer *pAttacker, CBaseEntity *pVictim, const CTakeDamageInfo &info );

	virtual void Event_Credits();
	virtual void Event_Commentary();
	virtual void Event_CrateSmashed();
	virtual void Event_Punted( CBaseEntity *pObject );
	virtual void Event_PlayerTraveled( CBasePlayer *pBasePlayer, float distanceInInches, bool bInVehicle, bool bSprinting );
	virtual void Event_WeaponFired( CBasePlayer *pShooter, bool bPrimary, char const *pchWeaponName );
	virtual void Event_WeaponHit( CBasePlayer *pShooter, bool bPrimary, char const *pchWeaponName, const CTakeDamageInfo &info );
	virtual void Event_FlippedVehicle( CBasePlayer *pDriver, CPropVehicleDriveable *pVehicle );
	// Called before .sav file is actually loaded (player should still be in previous level, if any)
	virtual void Event_PreSaveGameLoaded( char const *pSaveName, bool bInGame );
	virtual void Event_PlayerEnteredGodMode( CBasePlayer *pBasePlayer );
	virtual void Event_PlayerEnteredNoClip( CBasePlayer *pBasePlayer );
	virtual void Event_DecrementPlayerEnteredNoClip( CBasePlayer *pBasePlayer );

	// Generic statistics lump
	virtual void Event_IncrementCountedStatistic( const Vector& vecAbsOrigin, char const *pchStatisticName, float flIncrementAmount );

	//not implemented yet
	//virtual void Event_AwayFromKeyboard( void );
	//virtual void Event_ReturnedToKeyboard( void );	
	
	//custom data to tack onto existing stats if you're not doing a complete overhaul
	virtual void AppendCustomDataToSaveBuffer( CUtlBuffer &SaveBuffer ) { } //custom data you want thrown into the default save and upload path
	virtual void LoadCustomDataFromBuffer( CUtlBuffer &LoadBuffer ) { }; //when loading the saved stats file, this will point to where you started saving data to the save buffer

	virtual void LoadingEvent_PlayerIDDifferentThanLoadedStats( void ); //Only called if you use the base SaveToFileNOW() and LoadFromFile() functions. Used in case you want to keep/invalidate data that was just loaded. 

	virtual bool LoadFromFile( void ); //called just before Event_Init()
	virtual bool SaveToFileNOW( void ); //saves buffers to their respective files now, returns success or failure
	virtual bool UploadStatsFileNOW( void ); //uploads data to the CSER now, returns success or failure

	static bool AppendLump( int nMaxLumpCount, CUtlBuffer &SaveBuffer, unsigned short iLump, unsigned short iLumpCount, size_t nSize, void *pData );
	static bool GetLumpHeader( int nMaxLumpCount, CUtlBuffer &LoadBuffer, unsigned short &iLump, unsigned short &iLumpCount, bool bPermissive = false );
	static void LoadLump( CUtlBuffer &LoadBuffer, unsigned short iLumpCount, size_t nSize, void *pData );

	// Helper for builtin stuff
	void SetSteamStatistic( bool bUsingSteam );
	void SetCyberCafeStatistic( bool bIsCyberCafeUser );
	void SetHDRStatistic( bool bHDREnabled );
	void SetCaptionsStatistic( bool bClosedCaptionsEnabled );
	void SetSkillStatistic( int iSkillSetting );
	void SetDXLevelStatistic( int iDXLevel );
	void SetHL2UnlockedChapterStatistic( void );

public:
	BasicGameStats_t m_BasicStats; //exposed in case you do a complete overhaul and still want to save it
	bool			m_bLogging : 1;
	bool			m_bLoggingToFile : 1;
};

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : &SaveBuffer - 
//			iLump - 
//			iLumpCount - 
//-----------------------------------------------------------------------------
inline bool CBaseGameStats::AppendLump( int nMaxLumpCount, CUtlBuffer &SaveBuffer, unsigned short iLump, unsigned short iLumpCount, size_t nSize, void *pData )
{
	// Verify the lump index.
	Assert( ( iLump > 0 ) && ( iLump < nMaxLumpCount ) );

	if ( !( ( iLump > 0 ) && ( iLump < nMaxLumpCount ) ) )
		return false;

	// Check to see if we have any elements to save.
	if ( iLumpCount <= 0 )
		return false;

	// Write the lump id and element count.
	SaveBuffer.PutUnsignedShort( iLump );
	SaveBuffer.PutUnsignedShort( iLumpCount );

	size_t nTotalSize = iLumpCount * nSize;
	SaveBuffer.Put( pData, nTotalSize );

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : &LoadBuffer - 
//			&iLump - 
//			&iLumpCount - 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
inline bool CBaseGameStats::GetLumpHeader( int nMaxLumpCount, CUtlBuffer &LoadBuffer, unsigned short &iLump, unsigned short &iLumpCount, bool bPermissive /*= false*/ )
{
	// Get the lump id and element count.
	iLump = LoadBuffer.GetUnsignedShort();
	if ( !LoadBuffer.IsValid() )
	{
		// check for EOF
		return false;
	}
	iLumpCount = LoadBuffer.GetUnsignedShort();

	if ( bPermissive )
		return true;

	// Verify the lump index.
	Assert( ( iLump > 0 ) && ( iLump < nMaxLumpCount ) );
	if ( !( ( iLump > 0 ) && ( iLump < nMaxLumpCount ) ) )
	{
		return false;
	}

	// Check to see if we have any elements to save.
	if ( iLumpCount <= 0 )
		return false;

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: Loads 1 or more lumps of raw data
// Input  : &LoadBuffer - buffer to be read from
//			iLumpCount - # of lumps to read
//			nSize - size of each lump
//			pData - where to store the data
//-----------------------------------------------------------------------------
inline void CBaseGameStats::LoadLump( CUtlBuffer &LoadBuffer, unsigned short iLumpCount, size_t nSize, void *pData )
{
	LoadBuffer.Get( pData, iLumpCount * nSize );
}

extern CBaseGameStats *gamestats; //starts out pointing at a singleton of the class above, overriding this in any constructor should work for replacing it

#endif // GAMESTATS_H
