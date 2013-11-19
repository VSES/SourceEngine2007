//========= Copyright © 1996-2001, Valve LLC, All rights reserved. ============
//
// Purpose:		
//
// $NoKeywords: $
//=============================================================================

#include "cbase.h"
#include "mp_shareddefs.h"
#include "basemultiplayerplayer.h"

CBaseMultiplayerPlayer::CBaseMultiplayerPlayer()
{
	m_iCurrentConcept = MP_CONCEPT_NONE;
	m_flLastForcedChangeTeamTime = -1;
	m_iBalanceScore = 0;
	m_flConnectionTime = gpGlobals->curtime;
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CAI_Expresser *CBaseMultiplayerPlayer::CreateExpresser( void )
{
	m_pExpresser = new CMultiplayer_Expresser(this);
	if ( !m_pExpresser)
		return NULL;

	m_pExpresser->Connect(this);
	return m_pExpresser;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CBaseMultiplayerPlayer::PostConstructor( const char *szClassname )
{
	BaseClass::PostConstructor( szClassname );
	CreateExpresser();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CBaseMultiplayerPlayer::ModifyOrAppendCriteria( AI_CriteriaSet& criteriaSet )
{
	BaseClass::ModifyOrAppendCriteria( criteriaSet );

	ModifyOrAppendPlayerCriteria( criteriaSet );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CBaseMultiplayerPlayer::SpeakIfAllowed( AIConcept_t concept, const char *modifiers, char *pszOutResponseChosen, size_t bufsize, IRecipientFilter *filter ) 
{ 
	if ( !IsAlive() )
		return false;

	//if ( IsAllowedToSpeak( concept, bRespondingToPlayer ) )
	return Speak( concept, modifiers, pszOutResponseChosen, bufsize, filter );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
IResponseSystem *CBaseMultiplayerPlayer::GetResponseSystem()
{
	return BaseClass::GetResponseSystem();
	// NOTE: This is where you would hook your custom responses.
//	return <*>GameRules()->m_ResponseRules[iIndex].m_ResponseSystems[m_iCurrentConcept];
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
AI_Response *CBaseMultiplayerPlayer::SpeakConcept( int iConcept )
{
	m_iCurrentConcept = iConcept;
	return SpeakFindResponse( g_pszMPConcepts[iConcept] );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CBaseMultiplayerPlayer::SpeakConceptIfAllowed( int iConcept, const char *modifiers, char *pszOutResponseChosen, size_t bufsize, IRecipientFilter *filter )
{
	// Save the current concept.
	m_iCurrentConcept = iConcept;
	return SpeakIfAllowed( g_pszMPConcepts[iConcept], modifiers, pszOutResponseChosen, bufsize, filter );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CBaseMultiplayerPlayer::CanHearAndReadChatFrom( CBasePlayer *pPlayer )
{
	// can always hear the console unless we're ignoring all chat
	if ( !pPlayer )
		return m_iIgnoreGlobalChat != CHAT_IGNORE_ALL;

	// check if we're ignoring all chat
	if ( m_iIgnoreGlobalChat == CHAT_IGNORE_ALL )
		return false;

	// check if we're ignoring all but teammates
	if ( m_iIgnoreGlobalChat == CHAT_IGNORE_TEAM && g_pGameRules->PlayerRelationship( this, pPlayer ) != GR_TEAMMATE )
		return false;

	// can't hear dead players if we're alive
	if ( pPlayer->m_lifeState != LIFE_ALIVE && m_lifeState == LIFE_ALIVE )
		return false;

	return true;
}

bool CBaseMultiplayerPlayer::ClientCommand( const CCommand &args )
{
	const char *pcmd = args[0];

	if ( FStrEq( pcmd, "ignoremsg" ) )
	{
		m_iIgnoreGlobalChat = (m_iIgnoreGlobalChat + 1) % 3;
		switch( m_iIgnoreGlobalChat )
		{
		case CHAT_IGNORE_NONE:
			ClientPrint( this, HUD_PRINTTALK, "#Accept_All_Messages" );
			break;
		case CHAT_IGNORE_ALL:
			ClientPrint( this, HUD_PRINTTALK, "#Ignore_Broadcast_Messages" );
			break;
		case CHAT_IGNORE_TEAM:
			ClientPrint( this, HUD_PRINTTALK, "#Ignore_Broadcast_Team_Messages" );
			break;
		default:
			break;
		}
		return true;
	}

	return BaseClass::ClientCommand( args );
}

bool CBaseMultiplayerPlayer::ShouldShowVoiceSubtitleToEnemy( void )
{
	return false;
}

//-----------------------------------------------------------------------------
// calculate a score for this player. higher is more likely to be switched
//-----------------------------------------------------------------------------
int	CBaseMultiplayerPlayer::CalculateTeamBalanceScore( void )
{
	// base score is 0 - ( seconds on server )
	float flTimeConnected = gpGlobals->curtime - m_flConnectionTime;
	int iScore = 0 - (int)flTimeConnected;

	// if we were switched recently, score us way down
	float flLastSwitchedTime = GetLastForcedChangeTeamTime();
	if ( flLastSwitchedTime > 0 && ( gpGlobals->curtime - flLastSwitchedTime ) < 300 )
	{
		iScore -= 10000;
	}
	return iScore;
}

void CBaseMultiplayerPlayer::AwardAchievement( int iAchievement )
{
	Assert( iAchievement >= 0 && iAchievement < 255 );		// must fit in byte

	CSingleUserRecipientFilter filter( this );

	UserMessageBegin( filter, "AchievementEvent" );
		WRITE_BYTE( iAchievement );
	MessageEnd();
}