//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"
#include "hud.h"
#include "CstrikeClientScoreBoard.h"
#include "c_team.h"
#include "c_cs_playerresource.h"
#include "c_cs_player.h"
#include "backgroundpanel.h"

#include <KeyValues.h>

#include <vgui/IScheme.h>
#include <vgui/ILocalize.h>
#include <vgui/ISurface.h>
#include <vgui/IVgui.h>
#include <vgui_controls/SectionedListPanel.h>

#include "voice_status.h"
#include "vgui_avatarimage.h"

using namespace vgui;

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CCSClientScoreBoardDialog::CCSClientScoreBoardDialog( IViewPort *pViewPort ) : CClientScoreBoardDialog( pViewPort )
{
	m_pPlayerListT = new SectionedListPanel( this, "PlayerListT" );
	m_pPlayerListCT = new SectionedListPanel( this, "PlayerListCT" );

	m_pPlayerCountLabel_T = new Label( this, "T_PlayerCount", "" );
	m_pScoreLabel_T = new Label( this, "T_Score", "" );
	m_pPingLabel_T = new Label( this, "T_Latency", "" );
	m_pPlayerCountLabel_CT = new Label( this, "CT_PlayerCount", "" );
	m_pScoreLabel_CT = new Label( this, "CT_Score", "" );
	m_pPingLabel_CT = new Label( this, "CT_Latency", "" );

	ListenForGameEvent( "server_spawn" );
	SetDialogVariable( "server", "" );
	SetVisible( false );
}

//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CCSClientScoreBoardDialog::~CCSClientScoreBoardDialog()
{
}

//-----------------------------------------------------------------------------
// Purpose: Paint background for rounded corners
//-----------------------------------------------------------------------------
void CCSClientScoreBoardDialog::PaintBackground()
{
	int wide, tall;
	GetSize( wide, tall );

	DrawRoundedBackground( m_bgColor, wide, tall );
}

//-----------------------------------------------------------------------------
// Purpose: Paint border for rounded corners
//-----------------------------------------------------------------------------
void CCSClientScoreBoardDialog::PaintBorder()
{
	int wide, tall;
	GetSize( wide, tall );

	DrawRoundedBorder( m_borderColor, wide, tall );
}

//-----------------------------------------------------------------------------
// Purpose: Apply scheme settings
//-----------------------------------------------------------------------------
void CCSClientScoreBoardDialog::ApplySchemeSettings( vgui::IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );

	LoadControlSettings( "Resource/UI/scoreboard.res" );

	m_bgColor = GetSchemeColor( "SectionedListPanel.BgColor", GetBgColor(), pScheme );
	m_borderColor = pScheme->GetColor( "FgColor", Color( 0, 0, 0, 0 ) );

	SetBgColor( Color( 0, 0, 0, 0 ) );
	SetBorder( pScheme->GetBorder( "BaseBorder" ) );

	if ( m_pPlayerListT )
	{
		m_pPlayerListT->SetImageList( m_pImageList, false );
		m_pPlayerListT->SetBgColor( Color( 0, 0, 0, 0 ) );
		m_pPlayerListT->SetBorder( NULL );
		m_pPlayerListT->SetVisible( true );
	}

	if ( m_pPlayerListCT )
	{
		m_pPlayerListCT->SetImageList( m_pImageList, false );
		m_pPlayerListCT->SetBgColor( Color( 0, 0, 0, 0 ) );
		m_pPlayerListCT->SetBorder( NULL );
		m_pPlayerListCT->SetVisible( true );
	}

	// turn off the default player list since we have our own
	if ( m_pPlayerList )
	{
		m_pPlayerList->SetVisible( false );
	}

	if ( m_pPlayerCountLabel_T && m_pScoreLabel_T && m_pPingLabel_T )
	{
		m_pPlayerCountLabel_T->SetFgColor( COLOR_RED );
		m_pScoreLabel_T->SetFgColor( COLOR_RED );
		m_pPingLabel_T->SetFgColor( COLOR_RED );
	}

	if ( m_pPlayerCountLabel_CT && m_pScoreLabel_CT && m_pPingLabel_CT )
	{
		m_pPlayerCountLabel_CT->SetFgColor( COLOR_BLUE );
		m_pScoreLabel_CT->SetFgColor( COLOR_BLUE );
		m_pPingLabel_CT->SetFgColor( COLOR_BLUE );
	}

	SetVisible( false );
	Reset();
}

//-----------------------------------------------------------------------------
// Purpose: Resets the scoreboard panel
//-----------------------------------------------------------------------------
void CCSClientScoreBoardDialog::Reset()
{
	InitPlayerList( m_pPlayerListT, TEAM_TERRORIST );
	InitPlayerList( m_pPlayerListCT, TEAM_CT );
}

//-----------------------------------------------------------------------------
// Purpose: Used for sorting players
//-----------------------------------------------------------------------------
bool CCSClientScoreBoardDialog::CSPlayerSortFunc( vgui::SectionedListPanel *list, int itemID1, int itemID2 )
{
	KeyValues *it1 = list->GetItemData( itemID1 );
	KeyValues *it2 = list->GetItemData( itemID2 );
	Assert( it1 && it2 );

	// first compare score
	int v1 = it1->GetInt( "frags" );
	int v2 = it2->GetInt( "frags" );
	if ( v1 > v2 )
		return true;
	else if ( v1 < v2 )
		return false;

	// second compare deaths
	v1 = it1->GetInt( "deaths" );
	v2 = it2->GetInt( "deaths" );
	if ( v1 > v2 )
		return false;
	else if ( v1 < v2 )
		return true;

	// if score and deaths are the same, use player index to get deterministic sort
	int iPlayerIndex1 = it1->GetInt( "playerIndex" );
	int iPlayerIndex2 = it2->GetInt( "playerIndex" );
	return ( iPlayerIndex1 > iPlayerIndex2 );
}

//-----------------------------------------------------------------------------
// Purpose: Inits the player list in a list panel
//-----------------------------------------------------------------------------
void CCSClientScoreBoardDialog::InitPlayerList( SectionedListPanel *pPlayerList, int teamNumber )
{
	pPlayerList->SetVerticalScrollbar( false );
	pPlayerList->RemoveAll();
	pPlayerList->RemoveAllSections();
	pPlayerList->AddSection( 0, "Players", CSPlayerSortFunc );
	pPlayerList->SetSectionAlwaysVisible( 0, true );
	pPlayerList->SetSectionFgColor( 0, Color( 255, 255, 255, 255 ) );
	pPlayerList->SetBgColor( Color( 0, 0, 0, 0 ) );
	pPlayerList->SetBorder( NULL );

	// set the section to have the team color
	if ( teamNumber && GameResources() )
	{
		pPlayerList->SetSectionFgColor( 0, GameResources()->GetTeamColor( teamNumber ) );
	}

	// Avatars are always displayed at 32x32 regardless of resolution
	pPlayerList->AddColumnToSection( 0, "avatar", "", SectionedListPanel::COLUMN_IMAGE | SectionedListPanel::COLUMN_CENTER, m_iAvatarWidth );
	pPlayerList->AddColumnToSection( 0, "name", "", 0, m_iNameWidth );
	pPlayerList->AddColumnToSection( 0, "class", "" , 0, m_iClassWidth );
	pPlayerList->AddColumnToSection( 0, "frags", "", SectionedListPanel::COLUMN_RIGHT, m_iScoreWidth );
	pPlayerList->AddColumnToSection( 0, "deaths", "", SectionedListPanel::COLUMN_RIGHT, m_iDeathWidth );
	pPlayerList->AddColumnToSection( 0, "ping", "", SectionedListPanel::COLUMN_RIGHT, m_iPingWidth );
}

//-----------------------------------------------------------------------------
// Purpose: Updates the dialog
//-----------------------------------------------------------------------------
void CCSClientScoreBoardDialog::Update()
{
	UpdateTeamInfo();
	UpdatePlayerList();
	UpdateSpectatorList();
	MoveToCenterOfScreen();

	// update every second
	m_fNextUpdateTime = gpGlobals->curtime + 1.0f; 
}

//-----------------------------------------------------------------------------
// Purpose: Updates information about teams
//-----------------------------------------------------------------------------
void CCSClientScoreBoardDialog::UpdateTeamInfo()
{
	// update the team sections in the scoreboard
	for ( int teamIndex = TEAM_TERRORIST; teamIndex <= TEAM_CT; teamIndex++ )
	{
		wchar_t *teamName = NULL;
		C_Team *team = GetGlobalTeam( teamIndex );
		if ( team )
		{
			// choose dialog variables to set depending on team
			const char *pDialogVarTeamScore = NULL;
			const char *pDialogVarTeamPlayerCount = NULL;
			const char *pDialogVarTeamPing = NULL;
			switch ( teamIndex ) {
				case TEAM_TERRORIST:
					teamName = g_pVGuiLocalize->Find( "#Cstrike_ScoreBoard_Ter" );
					pDialogVarTeamScore = "t_teamscore";
					pDialogVarTeamPlayerCount = "t_teamplayercount";
					pDialogVarTeamPing = "t_teamping";
					break;
				case TEAM_CT:
					teamName = g_pVGuiLocalize->Find( "#Cstrike_ScoreBoard_CT" );
					pDialogVarTeamScore = "ct_teamscore";
					pDialogVarTeamPlayerCount = "ct_teamplayercount";
					pDialogVarTeamPing = "ct_teamping";
					break;
				default:
					Assert( false );
					break;
			}

			// update # of players on each team
			wchar_t name[64];
			wchar_t string1[1024];
			wchar_t wNumPlayers[6];
			_snwprintf( wNumPlayers, ARRAYSIZE( wNumPlayers ), L"%i", team->Get_Number_Players() );
			if ( !teamName && team )
			{
				g_pVGuiLocalize->ConvertANSIToUnicode( team->Get_Name(), name, sizeof( name ) );
				teamName = name;
			}
			if ( team->Get_Number_Players() == 1 )
			{
				g_pVGuiLocalize->ConstructString( string1, sizeof(string1), g_pVGuiLocalize->Find( "#Cstrike_ScoreBoard_Player" ), 2, teamName, wNumPlayers );
			}
			else
			{
				g_pVGuiLocalize->ConstructString( string1, sizeof(string1), g_pVGuiLocalize->Find( "#Cstrike_ScoreBoard_Players" ), 2, teamName, wNumPlayers );
			}

			// set # of players for team in dialog
			SetDialogVariable( pDialogVarTeamPlayerCount, string1 );

			// set team score in dialog
			SetDialogVariable( pDialogVarTeamScore, team->Get_Score() );			

			int pingsum = 0;
			int numcounted = 0;
			for( int playerIndex = 1 ; playerIndex <= MAX_PLAYERS; playerIndex++ )
			{
				if( g_PR->IsConnected( playerIndex ) && g_PR->GetTeam( playerIndex ) == teamIndex )
				{
					int ping = g_PR->GetPing( playerIndex );

					if ( ping >= 1 )
					{
						pingsum += ping;
						numcounted++;
					}
				}
			}

			if ( numcounted > 0 )
			{
				int ping = (int)( (float)pingsum / (float)numcounted );
				SetDialogVariable( pDialogVarTeamPing, ping );		
			}
			else
			{
				SetDialogVariable( pDialogVarTeamPing, "" );	
			}
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: Updates the player list
//-----------------------------------------------------------------------------
void CCSClientScoreBoardDialog::UpdatePlayerList()
{
	m_pPlayerListT->RemoveAll();
	m_pPlayerListCT->RemoveAll();

	C_CS_PlayerResource *cs_PR = dynamic_cast<C_CS_PlayerResource *>( g_PR );
	if ( !cs_PR )
		return;

	C_CSPlayer *pLocalPlayer = C_CSPlayer::GetLocalCSPlayer();
	if ( !pLocalPlayer )
		return;

	for( int playerIndex = 1 ; playerIndex <= MAX_PLAYERS; playerIndex++ )
	{
		if( g_PR->IsConnected( playerIndex ) )
		{
			SectionedListPanel *pPlayerList = NULL;
			switch ( g_PR->GetTeam( playerIndex ) )
			{
			case TEAM_TERRORIST:
				pPlayerList = m_pPlayerListT;
				break;
			case TEAM_CT:
				pPlayerList = m_pPlayerListCT;
				break;
			}

			if ( pPlayerList == NULL )
			{
				continue;			
			}

			KeyValues *pKeyValues = new KeyValues( "data" );
			GetPlayerScoreInfo( playerIndex, pKeyValues );

			int itemID = pPlayerList->AddItem( 0, pKeyValues );
			Color clr = g_PR->GetTeamColor( g_PR->GetTeam( playerIndex ) );
			pPlayerList->SetItemFgColor( itemID, clr );

			pKeyValues->deleteThis();
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: Updates the spectator list
//-----------------------------------------------------------------------------
void CCSClientScoreBoardDialog::UpdateSpectatorList()
{
	char szSpectatorList[512] = "" ;
	int nSpectators = 0;
	for( int playerIndex = 1 ; playerIndex <= MAX_PLAYERS; playerIndex++ )
	{
		if ( ShouldShowAsSpectator( playerIndex ) )
		{
			if ( nSpectators > 0 )
			{
				Q_strncat( szSpectatorList, ", ", ARRAYSIZE( szSpectatorList ) );
			}

			Q_strncat( szSpectatorList, g_PR->GetPlayerName( playerIndex ), ARRAYSIZE( szSpectatorList ) );
			nSpectators++;
		}
	}

	wchar_t wzSpectators[512] = L"";
	if ( nSpectators > 0 )
	{
		const char *pchFormat = ( 1 == nSpectators ? "#ScoreBoard_Spectator" : "#ScoreBoard_Spectators" );

		wchar_t wzSpectatorCount[16];
		wchar_t wzSpectatorList[1024];
		_snwprintf( wzSpectatorCount, ARRAYSIZE( wzSpectatorCount ), L"%i", nSpectators );
		g_pVGuiLocalize->ConvertANSIToUnicode( szSpectatorList, wzSpectatorList, sizeof( wzSpectatorList ) );
		g_pVGuiLocalize->ConstructString( wzSpectators, sizeof(wzSpectators), g_pVGuiLocalize->Find( pchFormat), 2, wzSpectatorCount, wzSpectatorList );
	}

	SetDialogVariable( "spectators", wzSpectators );
}

//-----------------------------------------------------------------------------
// Purpose: Returns whether the specified player index is a spectator
//-----------------------------------------------------------------------------
bool CCSClientScoreBoardDialog::ShouldShowAsSpectator( int iPlayerIndex )
{
	C_CS_PlayerResource *cs_PR = dynamic_cast<C_CS_PlayerResource *>( g_PR );
	if ( !cs_PR )
		return false;

	// see if player is connected
	if ( cs_PR->IsConnected( iPlayerIndex ) ) 
	{
		// either spectator or unassigned team should show in spectator list
		int iTeam = cs_PR->GetTeam( iPlayerIndex );
		if ( TEAM_SPECTATOR == iTeam || TEAM_UNASSIGNED == iTeam )
			return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
// Purpose: Event handler
//-----------------------------------------------------------------------------
void CCSClientScoreBoardDialog::FireGameEvent( IGameEvent *event )
{
	const char *type = event->GetName();

	if ( 0 == Q_strcmp( type, "server_spawn" ) )
	{		
		// set server name in scoreboard
		const char *hostname = event->GetString( "hostname" );
		wchar_t wzHostName[256];
		wchar_t wzServerLabel[256];
		g_pVGuiLocalize->ConvertANSIToUnicode( hostname, wzHostName, sizeof( wzHostName ) );
		g_pVGuiLocalize->ConstructString( wzServerLabel, sizeof(wzServerLabel), g_pVGuiLocalize->Find( "#Scoreboard_Server" ), 1, wzHostName );
		SetDialogVariable( "server", wzServerLabel );
	}

	if( IsVisible() )
	{
		Update();
	}
}

//-----------------------------------------------------------------------------
// Purpose: Adds a new row to the scoreboard, from the playerinfo structure
//-----------------------------------------------------------------------------
bool CCSClientScoreBoardDialog::GetPlayerScoreInfo( int playerIndex, KeyValues *kv )
{
	// Clean up the player name
	const char *oldName = g_PR->GetPlayerName( playerIndex );
	int bufsize = strlen( oldName ) * 2 + 1;
	char *newName = (char *)_alloca( bufsize );
	UTIL_MakeSafeName( oldName, newName, bufsize );
	kv->SetString( "name", newName );

	kv->SetInt( "playerIndex", playerIndex );
	kv->SetInt( "frags", g_PR->GetPlayerScore( playerIndex ) );
	kv->SetInt( "deaths", g_PR->GetDeaths( playerIndex ) );
	kv->SetString( "class", "" );

	UpdatePlayerAvatar( playerIndex, kv );
	
	if ( g_PR->GetPing( playerIndex ) < 1 )
	{
		if ( g_PR->IsFakePlayer( playerIndex ) )
		{
			kv->SetString( "ping", "BOT" );
		}
		else
		{
			kv->SetString( "ping", "" );
		}
	}
	else
	{
		kv->SetInt( "ping", g_PR->GetPing( playerIndex ) );
	}

	// get CS specific infos
	C_CS_PlayerResource *cs_PR = dynamic_cast<C_CS_PlayerResource *>( g_PR );

	C_CSPlayer *me = C_CSPlayer::GetLocalCSPlayer();
		
	if ( !cs_PR || !me )
		return true;

	bool bShowExtraInfo = 
			 ( me->GetTeamNumber() == TEAM_UNASSIGNED ) || // we're not spawned yet
			 ( me->GetTeamNumber() == TEAM_SPECTATOR ) || // we are a spectator
			 ( me->IsPlayerDead() ) ||					  // we are dead
			 ( me->GetTeamNumber() == g_PR->GetTeam( playerIndex ) ); // we're on the same team
	
	if ( g_PR->IsHLTV( playerIndex ) )
	{
		// show #spectators in class field, it's transmitted as player's score
		char numspecs[32];
		Q_snprintf( numspecs, sizeof( numspecs ), "%i Spectators", m_HLTVSpectators );
		kv->SetString( "class", numspecs );
	}
	else if ( !g_PR->IsAlive( playerIndex ) && g_PR->GetTeam( playerIndex ) > TEAM_SPECTATOR )
	{
		kv->SetString( "class", "#Cstrike_DEAD" );
	}
	else if ( cs_PR->HasC4( playerIndex ) &&  bShowExtraInfo )
	{
		kv->SetString( "class", "#Cstrike_BOMB" );
	}
	else if ( cs_PR->IsVIP( playerIndex ) &&  bShowExtraInfo )
	{
		kv->SetString( "class", "#Cstrike_VIP" );
	}
	
	return true;
}
