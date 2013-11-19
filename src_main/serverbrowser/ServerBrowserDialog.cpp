//======== (C) Copyright 1999, 2000 Valve, L.L.C. All rights reserved. ========
//
// The copyright to the contents herein is the property of Valve, L.L.C.
// The contents may be used and/or copied only with the written permission of
// Valve, L.L.C., or in accordance with the terms and conditions stipulated in
// the agreement/contract under which the contents have been supplied.
//
// Purpose: 
//
// $Workfile:     $
// $Date:         $
//
//-----------------------------------------------------------------------------
// $Log: $
//
// $NoKeywords: $
//=============================================================================
#include "pch_serverbrowser.h"

#if defined( _X360 )
#include "xbox/xbox_win32stubs.h"
#endif

#if defined( _WIN32 ) && !defined( _X360 )
#define WIN32_LEAN_AND_MEAN
#include <winsock.h>
#endif

using namespace vgui;

static CServerBrowserDialog *s_InternetDlg = NULL;

CServerBrowserDialog &ServerBrowserDialog()
{
	return *CServerBrowserDialog::GetInstance();
}


// Returns a list of the ports that we hit when looking for 
void GetMostCommonQueryPorts( CUtlVector<uint16> &ports )
{
	for ( int i=0; i <= 5; i++ )
	{
		ports.AddToTail( 27015 + i );
		ports.AddToTail( 26900 + i );
	}

	ports.AddToTail(4242); //RDKF
}

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CServerBrowserDialog::CServerBrowserDialog(vgui::Panel *parent) : Frame(parent, "CServerBrowserDialog")
{
	s_InternetDlg = this;

	m_szGameName[0] = 0;
	m_szModDir[0] = 0;
	m_iLimitAppID = 0;
	m_pSavedData = NULL;
	m_pFilterData = NULL;
	m_pFavorites = NULL;
	m_pHistory = NULL;
	m_pCustomGames = NULL;

	LoadUserData();

	m_pInternetGames = new CInternetGames(this);
	m_pFavorites = new CFavoriteGames(this);
	m_pHistory = new CHistoryGames(this);
	m_pSpectateGames = new CSpectateGames(this);
	m_pLanGames = new CLanGames(this);
	m_pFriendsGames = new CFriendsGames(this);
	m_pCustomGames = new CCustomGames(this);

	SetMinimumSize( 640, 384 );
	SetSize( 640, 384 );

	m_pGameList = m_pInternetGames;

	m_pContextMenu =  new CServerContextMenu(this);;

	// property sheet
	m_pTabPanel = new PropertySheet(this, "GameTabs");
	m_pTabPanel->SetTabWidth(72);
	m_pTabPanel->AddPage(m_pInternetGames, "#ServerBrowser_InternetTab");
	m_pTabPanel->AddPage(m_pCustomGames, "#ServerBrowser_CustomTab");
	m_pTabPanel->AddPage(m_pFavorites, "#ServerBrowser_FavoritesTab");
	m_pTabPanel->AddPage(m_pHistory, "#ServerBrowser_HistoryTab");
	m_pTabPanel->AddPage(m_pSpectateGames, "#ServerBrowser_SpectateTab");
	m_pTabPanel->AddPage(m_pLanGames, "#ServerBrowser_LanTab");
	m_pTabPanel->AddPage(m_pFriendsGames, "#ServerBrowser_FriendsTab");
	m_pTabPanel->AddActionSignalTarget(this);

	m_pStatusLabel = new Label(this, "StatusLabel", "");

	LoadControlSettingsAndUserConfig("Servers/DialogServerBrowser.res");

	m_pStatusLabel->SetText("");

	// load current tab
	const char *gameList = m_pSavedData->GetString("GameList");

	if (!Q_stricmp(gameList, "spectate"))
	{
		m_pTabPanel->SetActivePage(m_pSpectateGames);
	}
	else if (!Q_stricmp(gameList, "favorites"))
	{
		m_pTabPanel->SetActivePage(m_pFavorites);
	}
	else if (!Q_stricmp(gameList, "history"))
	{
		m_pTabPanel->SetActivePage(m_pHistory);
	}
	else if (!Q_stricmp(gameList, "lan"))
	{
		m_pTabPanel->SetActivePage(m_pLanGames);
	}
	else if (!Q_stricmp(gameList, "friends"))
	{
		m_pTabPanel->SetActivePage(m_pFriendsGames);
	}
	else if (!Q_stricmp(gameList, "custom"))
	{
		m_pTabPanel->SetActivePage(m_pCustomGames);
	}
	else
	{
		m_pTabPanel->SetActivePage(m_pInternetGames);
	}

	ivgui()->AddTickSignal( GetVPanel() );
}

//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CServerBrowserDialog::~CServerBrowserDialog()
{
	delete m_pContextMenu;

	SaveUserData();

  	if (m_pSavedData)
  	{
  		m_pSavedData->deleteThis();
  	}
}


//-----------------------------------------------------------------------------
// Purpose: Called once to set up
//-----------------------------------------------------------------------------
void CServerBrowserDialog::Initialize()
{
	SetTitle("#ServerBrowser_Servers", true);
	SetVisible(false);
}


//-----------------------------------------------------------------------------
// Purpose: returns a server in the list
//-----------------------------------------------------------------------------
gameserveritem_t *CServerBrowserDialog::GetServer( unsigned int serverID )
{
	return m_pGameList->GetServer( serverID );
}


//-----------------------------------------------------------------------------
// Purpose: Activates and gives the tab focus
//-----------------------------------------------------------------------------
void CServerBrowserDialog::Open()
{	
	BaseClass::Activate();
	m_pTabPanel->RequestFocus();
}


//-----------------------------------------------------------------------------
// Purpose: Called every frame, updates animations for this module
//-----------------------------------------------------------------------------
void CServerBrowserDialog::OnTick()
{
	BaseClass::OnTick();
	vgui::GetAnimationController()->UpdateAnimations( system()->GetFrameTime() );
}


//-----------------------------------------------------------------------------
// Purpose: Loads filter settings from disk
//-----------------------------------------------------------------------------
void CServerBrowserDialog::LoadUserData()
{
  	// free any old filters
  	if (m_pSavedData)
  	{
  		m_pSavedData->deleteThis();
  	}

	m_pSavedData = new KeyValues("Filters");
	if (!m_pSavedData->LoadFromFile( g_pFullFileSystem, "ServerBrowser.vdf", "CONFIG"))
	{
		// doesn't matter if the file is not found, defaults will work successfully and file will be created on exit
	}

	KeyValues *filters = m_pSavedData->FindKey( "Filters", false );
	if ( filters )
	{
		m_pFilterData = filters->MakeCopy();
		m_pSavedData->RemoveSubKey( filters );
	}
	else
	{
		m_pFilterData = new KeyValues( "Filters" );
	}


	// reload all the page settings if necessary
	if (m_pHistory)
	{
		// history
		m_pHistory->LoadHistoryList();
		if ( m_pHistory->IsVisible() )
			m_pHistory->StartRefresh();
	}

	if (m_pFavorites)
	{
		// favorites
		m_pFavorites->LoadFavoritesList();

		// filters
		ReloadFilterSettings();

		if ( m_pFavorites->IsVisible() )
			m_pFavorites->StartRefresh();
	}

	InvalidateLayout();
	Repaint();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CServerBrowserDialog::SaveUserData()
{
	m_pSavedData->Clear();
	m_pSavedData->LoadFromFile( g_pFullFileSystem, "ServerBrowser.vdf", "CONFIG");

	// set the current tab
	if (m_pGameList == m_pSpectateGames)
	{
		m_pSavedData->SetString("GameList", "spectate");
	}
	else if (m_pGameList == m_pFavorites)
	{
		m_pSavedData->SetString("GameList", "favorites");
	}
	else if (m_pGameList == m_pLanGames)
	{
		m_pSavedData->SetString("GameList", "lan");
	}
	else if (m_pGameList == m_pFriendsGames)
	{
		m_pSavedData->SetString("GameList", "friends");
	}
	else if (m_pGameList == m_pHistory)
	{
		m_pSavedData->SetString("GameList", "history");
	}
	else if (m_pGameList == m_pCustomGames)
	{
		m_pSavedData->SetString("GameList", "custom");
	}
	else
	{
		m_pSavedData->SetString("GameList", "internet");
	}

	m_pSavedData->RemoveSubKey( m_pSavedData->FindKey( "Filters" ) ); // remove the saved subkey and add our subkey
	m_pSavedData->AddSubKey( m_pFilterData->MakeCopy() );
	m_pSavedData->SaveToFile( g_pFullFileSystem, "ServerBrowser.vdf", "CONFIG");

	// save per-page config
	SaveUserConfig();
}

//-----------------------------------------------------------------------------
// Purpose: refreshes the page currently visible
//-----------------------------------------------------------------------------
void CServerBrowserDialog::RefreshCurrentPage()
{
	if (m_pGameList)
	{
		m_pGameList->StartRefresh();
	}
}

//-----------------------------------------------------------------------------
// Purpose: Updates status test at bottom of window
//-----------------------------------------------------------------------------
void CServerBrowserDialog::UpdateStatusText(const char *fmt, ...)
{
	if ( !m_pStatusLabel )
		return;

	if ( fmt && strlen(fmt) > 0 )
	{
		char str[ 1024 ];
		va_list argptr;
		va_start( argptr, fmt );
		_vsnprintf( str, sizeof(str), fmt, argptr );
		va_end( argptr );

		m_pStatusLabel->SetText( str );
	}
	else
	{
		// clear
		m_pStatusLabel->SetText( "" );
	}
}

//-----------------------------------------------------------------------------
// Purpose: Updates status test at bottom of window
// Input  : wchar_t* (unicode string) - 
//-----------------------------------------------------------------------------
void CServerBrowserDialog::UpdateStatusText(wchar_t *unicode)
{
	if ( !m_pStatusLabel )
		return;

	if ( unicode && wcslen(unicode) > 0 )
	{
		m_pStatusLabel->SetText( unicode );
	}
	else
	{
		// clear
		m_pStatusLabel->SetText( "" );
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CServerBrowserDialog::OnGameListChanged()
{
	m_pGameList = dynamic_cast<IGameList *>(m_pTabPanel->GetActivePage());

	UpdateStatusText("");

	InvalidateLayout();
	Repaint();
}

//-----------------------------------------------------------------------------
// Purpose: returns a pointer to a static instance of this dialog
//-----------------------------------------------------------------------------
CServerBrowserDialog *CServerBrowserDialog::GetInstance()
{
	return s_InternetDlg;
}

//-----------------------------------------------------------------------------
// Purpose: Adds a server to the list of favorites
//-----------------------------------------------------------------------------
void CServerBrowserDialog::AddServerToFavorites(gameserveritem_t &server)
{
#ifndef NO_STEAM
	if ( SteamMatchmaking() )
	{
		SteamMatchmaking()->AddFavoriteGame2( 
			server.m_nAppID, 
			server.m_NetAdr.GetIP(), 
			server.m_NetAdr.GetConnectionPort(),		
			server.m_NetAdr.GetQueryPort(), 
			k_unFavoriteFlagFavorite, 
			time( NULL ) );
	}
#endif
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CServerContextMenu *CServerBrowserDialog::GetContextMenu(vgui::Panel *pPanel)
{
	// create a drop down for this object's states
	if (m_pContextMenu)
		delete m_pContextMenu;
	m_pContextMenu = new CServerContextMenu(this);
	m_pContextMenu->SetAutoDelete( false );

    if (!pPanel)
    {
	    m_pContextMenu->SetParent(this);
    }
    else
    {
        m_pContextMenu->SetParent(pPanel);
    }

	m_pContextMenu->SetVisible(false);
	return m_pContextMenu;
}

//-----------------------------------------------------------------------------
// Purpose: begins the process of joining a server from a game list
//			the game info dialog it opens will also update the game list
//-----------------------------------------------------------------------------
CDialogGameInfo *CServerBrowserDialog::JoinGame(IGameList *gameList, unsigned int serverIndex)
{
	// open the game info dialog, then mark it to attempt to connect right away
	CDialogGameInfo *gameDialog = OpenGameInfoDialog(gameList, serverIndex);

	// set the dialog name to be the server name
	gameDialog->Connect();

	return gameDialog;
}

//-----------------------------------------------------------------------------
// Purpose: joins a game by a specified IP, not attached to any game list
//-----------------------------------------------------------------------------
CDialogGameInfo *CServerBrowserDialog::JoinGame(int serverIP, int serverPort)
{
	// open the game info dialog, then mark it to attempt to connect right away
	CDialogGameInfo *gameDialog = OpenGameInfoDialog( serverIP, serverPort, serverPort );

	// set the dialog name to be the server name
	gameDialog->Connect();

	return gameDialog;
}

//-----------------------------------------------------------------------------
// Purpose: opens a game info dialog from a game list
//-----------------------------------------------------------------------------
CDialogGameInfo *CServerBrowserDialog::OpenGameInfoDialog( IGameList *gameList, unsigned int serverIndex )
{
	gameserveritem_t *pServer = gameList->GetServer( serverIndex );
	if ( !pServer )
		return NULL;

	CDialogGameInfo *gameDialog = new CDialogGameInfo( NULL, pServer->m_NetAdr.GetIP(), pServer->m_NetAdr.GetQueryPort(), pServer->m_NetAdr.GetConnectionPort() );
	gameDialog->SetParent(GetVParent());
	gameDialog->AddActionSignalTarget(this);
	gameDialog->Run( pServer->GetName() );
	int i = m_GameInfoDialogs.AddToTail();
	m_GameInfoDialogs[i] = gameDialog;
	return gameDialog;
}

//-----------------------------------------------------------------------------
// Purpose: opens a game info dialog by a specified IP, not attached to any game list
//-----------------------------------------------------------------------------
CDialogGameInfo *CServerBrowserDialog::OpenGameInfoDialog( int serverIP, uint16 connPort, uint16 queryPort )
{
	CDialogGameInfo *gameDialog = new CDialogGameInfo(NULL, serverIP, queryPort, connPort);
	gameDialog->AddActionSignalTarget(this);
	gameDialog->SetParent(GetVParent());
	gameDialog->Run("");
	int i = m_GameInfoDialogs.AddToTail();
	m_GameInfoDialogs[i] = gameDialog;
	return gameDialog;
}

//-----------------------------------------------------------------------------
// Purpose: closes all the game info dialogs
//-----------------------------------------------------------------------------
void CServerBrowserDialog::CloseAllGameInfoDialogs()
{
	for (int i = 0; i < m_GameInfoDialogs.Count(); i++)
	{
		vgui::Panel *dlg = m_GameInfoDialogs[i];
		if (dlg)
		{
			vgui::ivgui()->PostMessage(dlg->GetVPanel(), new KeyValues("Close"), NULL);
		}
	}
}


//-----------------------------------------------------------------------------
// Purpose: finds a dialog
//-----------------------------------------------------------------------------
CDialogGameInfo *CServerBrowserDialog::GetDialogGameInfoForFriend( uint64 ulSteamIDFriend )
{
	FOR_EACH_VEC( m_GameInfoDialogs, i )
	{
		CDialogGameInfo *pDlg = m_GameInfoDialogs[i];
		if ( pDlg && pDlg->GetAssociatedFriend() == ulSteamIDFriend )
		{
			return pDlg;
		}
	}
	return NULL;
}

//-----------------------------------------------------------------------------
// Purpose: accessor to the filter save data
//-----------------------------------------------------------------------------
KeyValues *CServerBrowserDialog::GetFilterSaveData(const char *filterSet)
{
	return m_pFilterData->FindKey(filterSet, true);
}

//-----------------------------------------------------------------------------
// Purpose: gets the name of the mod directory we're restricted to accessing, NULL if none
//-----------------------------------------------------------------------------
const char *CServerBrowserDialog::GetActiveModName()
{
	return m_szModDir[0] ? m_szModDir : NULL;
}


//-----------------------------------------------------------------------------
// Purpose: gets the name of the mod directory we're restricted to accessing, NULL if none
//-----------------------------------------------------------------------------
const char *CServerBrowserDialog::GetActiveGameName()
{
	return m_szGameName[0] ? m_szGameName : NULL;
}

//-----------------------------------------------------------------------------
// Purpose: return the app id to limit game queries to, set by Source/HL1 engines (NOT by filter settings, that is per page)
//-----------------------------------------------------------------------------
int CServerBrowserDialog::GetActiveAppID()
{
	return m_iLimitAppID;
}


//-----------------------------------------------------------------------------
// Purpose: receives a specified game is active, so no other game types can be displayed in server list
//-----------------------------------------------------------------------------
void CServerBrowserDialog::OnActiveGameName( KeyValues *pKV )
{
	Q_strncpy(m_szModDir, pKV->GetString( "name" ), sizeof(m_szModDir));
	Q_strncpy(m_szGameName, pKV->GetString( "game" ), sizeof(m_szGameName));
	m_iLimitAppID = pKV->GetInt( "appid", 0 );	
	// reload filter settings (since they are no forced to be game specific)
	ReloadFilterSettings();
}

//-----------------------------------------------------------------------------
// Purpose: resets all pages filter settings
//-----------------------------------------------------------------------------
void CServerBrowserDialog::ReloadFilterSettings()
{
	m_pInternetGames->LoadFilterSettings();
	m_pCustomGames->LoadFilterSettings();
	m_pSpectateGames->LoadFilterSettings();
	m_pFavorites->LoadFilterSettings();
	m_pLanGames->LoadFilterSettings();
	m_pFriendsGames->LoadFilterSettings();
	m_pHistory->LoadFilterSettings();
}

//-----------------------------------------------------------------------------
// Purpose: Adds server to the history, saves as currently connected server
//-----------------------------------------------------------------------------
void CServerBrowserDialog::OnConnectToGame( KeyValues *pMessageValues )
{
	int ip = pMessageValues->GetInt( "ip" );
	int connectionPort = pMessageValues->GetInt( "connectionport" );
	int queryPort = pMessageValues->GetInt( "queryport" );

	if ( !ip || !queryPort )
		return;

	memset( &m_CurrentConnection, 0, sizeof(gameserveritem_t) );
	m_CurrentConnection.m_NetAdr.SetIP( ip );
	m_CurrentConnection.m_NetAdr.SetQueryPort( queryPort );
	m_CurrentConnection.m_NetAdr.SetConnectionPort( (unsigned short)connectionPort );
#ifndef NO_STEAM
	if (m_pHistory && SteamMatchmaking() )
	{
		SteamMatchmaking()->AddFavoriteGame2( 0, ::htonl( ip ), connectionPort, queryPort, k_unFavoriteFlagHistory, time( NULL ) );
		m_pHistory->SetRefreshOnReload();
	}
#endif
	// tell the game info dialogs, so they can cancel if we have connected
	// to a server they were auto-retrying
	for (int i = 0; i < m_GameInfoDialogs.Count(); i++)
	{
		vgui::Panel *dlg = m_GameInfoDialogs[i];
		if (dlg)
		{
			KeyValues *kv = new KeyValues("ConnectedToGame", "ip", ip, "connectionport", connectionPort);
			kv->SetInt( "queryport", queryPort );
			vgui::ivgui()->PostMessage(dlg->GetVPanel(), kv, NULL);
		}
	}

	// forward to favorites
	m_pFavorites->OnConnectToGame();

	m_bCurrentlyConnected = true;
}

//-----------------------------------------------------------------------------
// Purpose: Clears currently connected server
//-----------------------------------------------------------------------------
void CServerBrowserDialog::OnDisconnectFromGame( void )
{
	m_bCurrentlyConnected = false;
	memset( &m_CurrentConnection, 0, sizeof(gameserveritem_t) );

	// forward to favorites
	m_pFavorites->OnDisconnectFromGame();
}

//-----------------------------------------------------------------------------
// Purpose: Passes build mode activation down into the pages
//-----------------------------------------------------------------------------
void CServerBrowserDialog::ActivateBuildMode()
{
	// no subpanel, no build mode
	EditablePanel *panel = dynamic_cast<EditablePanel *>(m_pTabPanel->GetActivePage());
	if (!panel)
		return;

	panel->ActivateBuildMode();
}

//-----------------------------------------------------------------------------
// Purpose: gets the default position and size on the screen to appear the first time
//-----------------------------------------------------------------------------
bool CServerBrowserDialog::GetDefaultScreenPosition(int &x, int &y, int &wide, int &tall)
{
	int wx, wy, ww, wt;
	surface()->GetWorkspaceBounds(wx, wy, ww, wt);
	x = wx + (int)(ww * 0.05);
	y = wy + (int)(wt * 0.4);
	wide = (int)(ww * 0.5);
	tall = (int)(wt * 0.55);
	return true;
}
