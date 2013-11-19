//========= Copyright © 1996-2004, Valve LLC, All rights reserved. ============
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================


#if defined( STEAM ) || defined( HL1 )
#include "stdafx.h"
#else
#include <stdio.h>
#include "dbg.h"
#include "SteamCommon.h"
#include "steam/steamclientpublic.h"
#include "strtools.h"
#endif

#ifdef HL1
#include "steamcommon.h"
#include "steam/steamclientpublic.h"
#endif

//-----------------------------------------------------------------------------
// Purpose: Constructor
// Input  : pchSteamID -		text representation of a Steam ID
//-----------------------------------------------------------------------------
CSteamID::CSteamID( const char *pchSteamID, EUniverse eDefaultUniverse /* = k_EUniverseInvalid */  )
{
	SetFromString( pchSteamID, eDefaultUniverse );
}


//-----------------------------------------------------------------------------
// Purpose: Initializes a steam ID from a string
// Input  : pchSteamID -		text representation of a Steam ID
//-----------------------------------------------------------------------------
void CSteamID::SetFromString( const char *pchSteamID, EUniverse eDefaultUniverse )
{
	int nAccountID = -1;
	int nInstance = 1;
	EUniverse eUniverse = eDefaultUniverse;
	EAccountType eAccountType = k_EAccountTypeIndividual;
	// BUGBUG Rich use the Q_ functions
	if (*pchSteamID == 'A')
	{
		// This is test only
		pchSteamID++; // skip the A
		eAccountType = k_EAccountTypeAnonGameServer;
		if (*pchSteamID == '-' || *pchSteamID == ':')
			pchSteamID++; // skip the optional - or :

		if ( strchr( pchSteamID, '(' ) )
			sscanf( strchr( pchSteamID, '(' ), "(%d)", &nInstance );
		if ( strchr( pchSteamID, ':' ) )
		{
			sscanf( pchSteamID, "%d:%d", &eUniverse, &nAccountID );
		}
		else
		{
			sscanf( pchSteamID, "%d", &nAccountID );
		}

		if ( nAccountID == 0 )
		{
			// i dont care what number you entered
			CreateBlankAnonLogon(eUniverse);
		}
		else
		{
			InstancedSet( nAccountID, nInstance, eUniverse, eAccountType );
		}
		return;
	}
	else if (*pchSteamID == 'G')
	{
		pchSteamID++; // skip the G
		eAccountType = k_EAccountTypeGameServer;
		if (*pchSteamID == '-' || *pchSteamID == ':')
			pchSteamID++; // skip the optional - or :
	}
	else if (*pchSteamID == 'C')
	{
		pchSteamID++; // skip the C
		eAccountType = k_EAccountTypeContentServer;
		if (*pchSteamID == '-' || *pchSteamID == ':')
			pchSteamID++; // skip the optional - or :
	}

	if ( strchr( pchSteamID, ':' ) )
	{
		if (*pchSteamID == '[')
			pchSteamID++; // skip the optional [
		sscanf( pchSteamID, "%d:%d", &eUniverse, &nAccountID );
		if ( eUniverse == k_EUniverseInvalid )
			eUniverse = eDefaultUniverse; 
	}
	else
	{
		sscanf( pchSteamID, "%d", &nAccountID );
	}	
	
	Assert( (eUniverse > k_EUniverseInvalid) && (eUniverse < k_EUniverseMax) );

	Set( nAccountID, eUniverse, eAccountType );
}


#if defined( INCLUDED_STEAM_COMMON_STEAMCOMMON_H ) 
//-----------------------------------------------------------------------------
// Purpose: Initializes a steam ID from a Steam2 ID string
// Input:	pchSteam2ID -	Steam2 ID (as a string #:#:#) to convert
//			eUniverse -		universe this ID belongs to
// Output:	true if successful, false otherwise
//-----------------------------------------------------------------------------
bool CSteamID::SetFromSteam2String( const char *pchSteam2ID, EUniverse eUniverse )
{
	Assert( pchSteam2ID );

	// Convert the Steam2 ID string to a Steam2 ID structure
	TSteamGlobalUserID steam2ID;
	steam2ID.m_SteamInstanceID = 0;
	steam2ID.m_SteamLocalUserID.Split.High32bits = 0;
	steam2ID.m_SteamLocalUserID.Split.Low32bits	= 0;

	const char *pchTSteam2ID = pchSteam2ID;

	// Customer support is fond of entering steam IDs in the following form:  STEAM_n:x:y
	char *pchOptionalLeadString = "STEAM_";
	if ( Q_strnicmp( pchSteam2ID, pchOptionalLeadString, Q_strlen( pchOptionalLeadString ) ) == 0 )
		pchTSteam2ID = pchSteam2ID + Q_strlen( pchOptionalLeadString );

	char cExtraCharCheck = 0;

	int cFieldConverted = sscanf( pchTSteam2ID, "%hu:%u:%u%c", &steam2ID.m_SteamInstanceID, 
		&steam2ID.m_SteamLocalUserID.Split.High32bits, &steam2ID.m_SteamLocalUserID.Split.Low32bits, &cExtraCharCheck );

	// Validate the conversion ... a special case is steam2 instance ID 1 which is reserved for special DoD handling
	if ( cExtraCharCheck != 0 || cFieldConverted == EOF || cFieldConverted < 2 || ( cFieldConverted < 3 && steam2ID.m_SteamInstanceID != 1 ) )
		return false;

	// Now convert to steam ID from the Steam2 ID structure
	SetFromSteam2( &steam2ID, eUniverse );
	return true;
}
#endif

//-----------------------------------------------------------------------------
// Purpose: Renders the steam ID to a buffer.  NOTE: for convenience of calling
//			code, this code returns a pointer to a static buffer and is NOT thread-safe.
// Output:  buffer with rendered Steam ID
//-----------------------------------------------------------------------------
char * CSteamID::Render() const
{
	const int k_cBufLen = 255;
	const int k_cBufs = 4;	// # of static bufs to use (so people can compose output with multiple calls to Render() )
	static char rgchBuf[k_cBufs][k_cBufLen];
	static int nBuf = 0;
	char * pchBuf = rgchBuf[nBuf];	// get pointer to current static buf
	nBuf ++;	// use next buffer for next call to this method
	nBuf %= k_cBufs;

	if ( k_EAccountTypeAnonGameServer == m_EAccountType )
	{
		Q_snprintf( pchBuf, k_cBufLen, "[A-%u:%u(%u)]", m_EUniverse, m_unAccountID, m_unAccountInstance );
	}
	else if ( k_EAccountTypeGameServer == m_EAccountType )
	{
		Q_snprintf( pchBuf, k_cBufLen, "[G-%u:%u]", m_EUniverse, m_unAccountID );
	}
	else if ( k_EAccountTypeMultiseat == m_EAccountType )
	{
		Q_snprintf( pchBuf, k_cBufLen, "[%u:%u(%u%)]", m_EUniverse, m_unAccountID, m_unAccountInstance );
	} 
	else if ( k_EAccountTypePending == m_EAccountType )
	{
		Q_snprintf( pchBuf, k_cBufLen, "[%u:%u(pending)]", m_EUniverse, m_unAccountID );
	} 
	else if ( k_EAccountTypeContentServer == m_EAccountType )
	{
		Q_snprintf( pchBuf, k_cBufLen, "[C-%u:%u]", m_EUniverse, m_unAccountID );
	}
	else
	{
		Q_snprintf( pchBuf, k_cBufLen, "[%u:%u]", m_EUniverse, m_unAccountID );
	}
	return pchBuf;
}


//-----------------------------------------------------------------------------
// Purpose: Renders the passed-in steam ID to a buffer.  NOTE: for convenience of calling
//			code, this code returns a pointer to a static buffer and is NOT thread-safe.
// Input:	64-bit representation of Steam ID to render
// Output:  buffer with rendered Steam ID
//-----------------------------------------------------------------------------
char * CSteamID::Render( uint64 ulSteamID )
{
	CSteamID steamID( ulSteamID );
	return steamID.Render();
}


//-----------------------------------------------------------------------------
// Purpose: some steamIDs are for internal use only
// This is really debug code, but we run with asserts on in retail, so ...
//-----------------------------------------------------------------------------
bool CSteamID::BValidExternalSteamID() const
{
	if ( m_EAccountType == k_EAccountTypePending )
		return false;
	if ( m_EAccountType != k_EAccountTypeAnonGameServer && m_EAccountType != k_EAccountTypeContentServer )
	{
		if ( m_unAccountID == 0 && m_unAccountInstance == 0 )
			return false;
	}
	return true;
}
