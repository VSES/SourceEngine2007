//========= Copyright © 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

#include "pch_serverbrowser.h"

#if defined( _X360 )
#include "xbox/xbox_win32stubs.h"
#endif

using namespace vgui;

#define FILTER_ALLSERVERS			0
#define FILTER_SECURESERVERSONLY	1
#define FILTER_INSECURESERVERSONLY	2

#define UNIVERSE_OFFICIAL			0
#define UNIVERSE_CUSTOMGAMES		1

#undef wcscat

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CGameListPanel::CGameListPanel( CBaseGamesPage *pOuter, const char *pName ) :
	BaseClass( pOuter, pName )
{
	m_pOuter = pOuter;
}

//-----------------------------------------------------------------------------
// Purpose: Forward KEY_ENTER to the CBaseGamesPage.
//-----------------------------------------------------------------------------
void CGameListPanel::OnKeyCodeTyped(vgui::KeyCode code)
{
	// Let the outer class handle it.
	if ( code == KEY_ENTER && m_pOuter->OnGameListEnterPressed() )
		return;
	
	BaseClass::OnKeyCodeTyped( code );
}


//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CBaseGamesPage::CBaseGamesPage( vgui::Panel *parent, const char *name, EMatchMakingType eType, const char *pCustomResFilename) 
	: PropertyPage(parent, name), m_pCustomResFilename( pCustomResFilename )
#ifndef NO_STEAM
	,
	m_CallbackFavoritesMsg( this, &CBaseGamesPage::OnFavoritesMsg )
#endif
{
	SetSize( 624, 278 );
	m_szGameFilter[0] = 0;
	m_szMapFilter[0]  = 0;
	m_iPingFilter = 0;
	m_iServerRefreshCount = 0;
	m_bFilterNoFullServers = false;
	m_bFilterNoEmptyServers = false;
	m_bFilterNoPasswordedServers = false;
	m_iSecureFilter = FILTER_ALLSERVERS;
	m_iLimitToAppID = 0;
	m_hFont = NULL;
	m_eMatchMakingType = eType;
	SetDefLessFunc( m_mapServers );
	SetDefLessFunc( m_mapServerIP );

	// get the 'all' text
	wchar_t *all = g_pVGuiLocalize->Find("ServerBrowser_All");
	Q_UnicodeToUTF8(all, m_szComboAllText, sizeof(m_szComboAllText));

	// Init UI
	m_pConnect = new Button(this, "ConnectButton", "#ServerBrowser_Connect");
	m_pConnect->SetEnabled(false);
	m_pRefreshAll = new Button(this, "RefreshButton", "#ServerBrowser_Refresh");
	m_pRefreshQuick = new Button(this, "RefreshQuickButton", "#ServerBrowser_RefreshQuick");
	m_pAddServer = new Button(this, "AddServerButton", "#ServerBrowser_AddServer");
	m_pAddCurrentServer = new Button(this, "AddCurrentServerButton", "#ServerBrowser_AddCurrentServer");
	m_pGameList = new CGameListPanel(this, "gamelist");
	m_pGameList->SetAllowUserModificationOfColumns(true);

	m_pAddToFavoritesButton = new vgui::Button( this, "AddToFavoritesButton", "" );
	m_pAddToFavoritesButton->SetEnabled( false );
	m_pAddToFavoritesButton->SetVisible( false );

	// Add the column headers
	m_pGameList->AddColumnHeader(0, "Password", "#ServerBrowser_Password", 16, ListPanel::COLUMN_FIXEDSIZE | ListPanel::COLUMN_IMAGE);
	m_pGameList->AddColumnHeader(1, "Bots", "#ServerBrowser_Bots", 16, ListPanel::COLUMN_FIXEDSIZE | ListPanel::COLUMN_HIDDEN);
	m_pGameList->AddColumnHeader(2, "Secure", "#ServerBrowser_Secure", 16, ListPanel::COLUMN_FIXEDSIZE | ListPanel::COLUMN_IMAGE);
	m_pGameList->AddColumnHeader(3, "Name", "#ServerBrowser_Servers", 50, ListPanel::COLUMN_RESIZEWITHWINDOW | ListPanel::COLUMN_UNHIDABLE);
	m_pGameList->AddColumnHeader(4, "IPAddr", "#ServerBrowser_IPAddress", 64, ListPanel::COLUMN_HIDDEN);
	m_pGameList->AddColumnHeader(5, "GameDesc", "#ServerBrowser_Game", 112,
		112,	// minwidth
		300,	// maxwidth
		0		// flags
		);
	m_pGameList->AddColumnHeader(6, "Players", "#ServerBrowser_Players", 55, ListPanel::COLUMN_FIXEDSIZE);
	m_pGameList->AddColumnHeader(7, "Map", "#ServerBrowser_Map", 90, 
		90,		// minwidth
		300,	// maxwidth
		0		// flags
		);
	m_pGameList->AddColumnHeader(8, "Ping", "#ServerBrowser_Latency", 55, ListPanel::COLUMN_FIXEDSIZE);

	m_pGameList->SetColumnHeaderTooltip(0, "#ServerBrowser_PasswordColumn_Tooltip");
	m_pGameList->SetColumnHeaderTooltip(1, "#ServerBrowser_BotColumn_Tooltip");
	m_pGameList->SetColumnHeaderTooltip(2, "#ServerBrowser_SecureColumn_Tooltip");

	// setup fast sort functions
	m_pGameList->SetSortFunc(0, PasswordCompare);
	m_pGameList->SetSortFunc(1, BotsCompare);
	m_pGameList->SetSortFunc(2, SecureCompare);
	m_pGameList->SetSortFunc(3, ServerNameCompare);
	m_pGameList->SetSortFunc(4, IPAddressCompare);
	m_pGameList->SetSortFunc(5, GameCompare);
	m_pGameList->SetSortFunc(6, PlayersCompare);
	m_pGameList->SetSortFunc(7, MapCompare);
	m_pGameList->SetSortFunc(8, PingCompare);

	// Sort by ping time by default
	m_pGameList->SetSortColumn(8);

 	CreateFilters();
	LoadFilterSettings();

	m_bAutoSelectFirstItemInGameList = false;
}

//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CBaseGamesPage::~CBaseGamesPage()
{
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
int CBaseGamesPage::GetInvalidServerListID()
{
	return m_pGameList->InvalidItemID();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CBaseGamesPage::PerformLayout()
{
	BaseClass::PerformLayout();

	if ( m_pGameList->GetSelectedItemsCount() < 1 )
	{
		m_pConnect->SetEnabled(false);
	}
	else
	{
		m_pConnect->SetEnabled(true);
	}


	if (SupportsItem(IGameList::GETNEWLIST))
	{
		m_pRefreshQuick->SetVisible(true);
		m_pRefreshAll->SetText("#ServerBrowser_RefreshAll");
	}
	else
	{
		m_pRefreshQuick->SetVisible(false);
		m_pRefreshAll->SetText("#ServerBrowser_Refresh");
	}

	if ( SupportsItem(IGameList::ADDSERVER) )
	{
		m_pFilterString->SetWide( 90 ); // shrink the filter label to fix the add current server button
		m_pAddServer->SetVisible(true);
	}
	else
	{
		m_pAddServer->SetVisible(false);
	}

	if ( SupportsItem(IGameList::ADDCURRENTSERVER) )
	{
		m_pAddCurrentServer->SetVisible(true);
	}
	else
	{
		m_pAddCurrentServer->SetVisible(false);
	}

	if ( IsRefreshing() )
	{
		m_pRefreshAll->SetText( "#ServerBrowser_StopRefreshingList" );
	}

	if (m_pGameList->GetItemCount() > 0)
	{
		m_pRefreshQuick->SetEnabled(true);
	}
	else
	{
		m_pRefreshQuick->SetEnabled(false);
	}
#ifndef NO_STEAM
	if ( !SteamMatchmakingServers() || !SteamMatchmaking() )
	{
		m_pAddCurrentServer->SetVisible( false );
		m_pRefreshQuick->SetEnabled( false );
		m_pAddServer->SetEnabled( false );
		m_pConnect->SetEnabled( false );
		m_pRefreshAll->SetEnabled( false );
		m_pAddToFavoritesButton->SetEnabled( false );
		m_pGameList->SetEmptyListText( "#ServerBrowser_SteamRunning" );
	}
#endif
	Repaint();
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CBaseGamesPage::ApplySchemeSettings(IScheme *pScheme)
{
	BaseClass::ApplySchemeSettings(pScheme);

	OnButtonToggled( m_pFilter, false );

	// load the password icon
	ImageList *imageList = new ImageList(false);
	imageList->AddImage(scheme()->GetImage("servers/icon_password", false));
	imageList->AddImage(scheme()->GetImage("servers/icon_bots", false));
	imageList->AddImage(scheme()->GetImage("servers/icon_robotron", false));
	imageList->AddImage(scheme()->GetImage("servers/icon_secure_deny", false));

	int passwordColumnImage = imageList->AddImage(scheme()->GetImage("servers/icon_password_column", false));
	int botColumnImage = imageList->AddImage(scheme()->GetImage("servers/icon_bots_column", false));
	int secureColumnImage = imageList->AddImage(scheme()->GetImage("servers/icon_robotron_column", false));

	m_pGameList->SetImageList(imageList, true);
	m_hFont = pScheme->GetFont( "ListSmall", IsProportional() );
	if ( !m_hFont )
		m_hFont = pScheme->GetFont( "DefaultSmall", IsProportional() );

	m_pGameList->SetFont( m_hFont );
	m_pGameList->SetColumnHeaderImage(0, passwordColumnImage);
	m_pGameList->SetColumnHeaderImage(1, botColumnImage);
	m_pGameList->SetColumnHeaderImage(2, secureColumnImage);
}

//-----------------------------------------------------------------------------
// Purpose: gets information about specified server
//-----------------------------------------------------------------------------
gameserveritem_t *CBaseGamesPage::GetServer( unsigned int serverID )
{
#ifndef NO_STEAM
	if ( !SteamMatchmakingServers() )
		return NULL;

	if ( serverID >= 0 )
	{
		return SteamMatchmakingServers()->GetServerDetails( m_eMatchMakingType, serverID );
	}
	else
	{
		Assert( !"Unable to return a useful entry" );
		return NULL; // bugbug Alfred: temp Favorites/History objects won't return a good value here...
	}
#else
	return NULL;
#endif
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CBaseGamesPage::CreateFilters()
{
	m_bFiltersVisible = false;
	m_pFilter = new ToggleButton(this, "Filter", "#ServerBrowser_Filter");
	m_pFilterString = new Label(this, "FilterString", "");

	// filter controls
	m_pGameFilter = new ComboBox(this, "GameFilter", 6, false);

	KeyValues *pkv = new KeyValues("mod", "gamedir", "", "appid", 0);
	m_pGameFilter->AddItem("#ServerBrowser_All", pkv);
	pkv->deleteThis();

	m_pLocationFilter = new ComboBox(this, "LocationFilter", 6, false);
	m_pLocationFilter->AddItem("", NULL);

	m_pMapFilter = new TextEntry(this, "MapFilter");
	m_pPingFilter = new ComboBox(this, "PingFilter", 6, false);
	m_pPingFilter->AddItem("#ServerBrowser_All", NULL);
	m_pPingFilter->AddItem("#ServerBrowser_LessThan50", NULL);
	m_pPingFilter->AddItem("#ServerBrowser_LessThan100", NULL);
	m_pPingFilter->AddItem("#ServerBrowser_LessThan150", NULL);
	m_pPingFilter->AddItem("#ServerBrowser_LessThan250", NULL);
	m_pPingFilter->AddItem("#ServerBrowser_LessThan350", NULL);
	m_pPingFilter->AddItem("#ServerBrowser_LessThan600", NULL);

	m_pSecureFilter = new ComboBox(this, "SecureFilter", 3, false);
	m_pSecureFilter->AddItem("#ServerBrowser_All", NULL);
	m_pSecureFilter->AddItem("#ServerBrowser_SecureOnly", NULL);
	m_pSecureFilter->AddItem("#ServerBrowser_InsecureOnly", NULL);

	m_pNoEmptyServersFilterCheck = new CheckButton(this, "ServerEmptyFilterCheck", "");
	m_pNoFullServersFilterCheck = new CheckButton(this, "ServerFullFilterCheck", "");
	m_pNoPasswordFilterCheck = new CheckButton(this, "NoPasswordFilterCheck", "");

	pkv = new KeyValues( "mod" );
	for (int i = 0; i < ModList().ModCount(); i++)
	{
		pkv->SetString("gamedir", ModList().GetModDir(i));
		pkv->SetInt("appid", ModList().GetAppID(i) );
		m_pGameFilter->AddItem(ModList().GetModName(i), pkv);
	}
	pkv->deleteThis();
}

//-----------------------------------------------------------------------------
// Purpose: loads filter settings from the keyvalues
//-----------------------------------------------------------------------------
void CBaseGamesPage::LoadFilterSettings()
{
	KeyValues *filter = ServerBrowserDialog().GetFilterSaveData(GetName());

	if (ServerBrowserDialog().GetActiveModName())
	{
		Q_strncpy(m_szGameFilter, ServerBrowserDialog().GetActiveModName(), sizeof(m_szGameFilter));
		m_iLimitToAppID = ServerBrowserDialog().GetActiveAppID();
	}
	else
	{
		Q_strncpy(m_szGameFilter, filter->GetString("game"), sizeof(m_szGameFilter));
		m_iLimitToAppID = filter->GetInt( "appid", 0 );
	}
	Q_strncpy(m_szMapFilter, filter->GetString("map"), sizeof(m_szMapFilter));
	m_iPingFilter = filter->GetInt("ping");
	m_bFilterNoFullServers = filter->GetInt("NoFull");
	m_bFilterNoEmptyServers = filter->GetInt("NoEmpty");
	m_bFilterNoPasswordedServers = filter->GetInt("NoPassword");

	int secureFilter = filter->GetInt("Secure");
	m_pSecureFilter->ActivateItem(secureFilter);

	// apply to the controls
	UpdateGameFilter();
	m_pMapFilter->SetText(m_szMapFilter);
	m_pLocationFilter->ActivateItem(filter->GetInt("location"));

	if (m_iPingFilter)
	{
		char buf[32];
		Q_snprintf(buf, sizeof(buf), "< %d", m_iPingFilter);
		m_pPingFilter->SetText(buf);
	}

	m_pNoFullServersFilterCheck->SetSelected(m_bFilterNoFullServers);
	m_pNoEmptyServersFilterCheck->SetSelected(m_bFilterNoEmptyServers);
	m_pNoPasswordFilterCheck->SetSelected(m_bFilterNoPasswordedServers);

	OnLoadFilter( filter );
	UpdateFilterSettings();
}

//-----------------------------------------------------------------------------
// Purpose: Sets the game filter combo box to be the saved setting
//-----------------------------------------------------------------------------
void CBaseGamesPage::UpdateGameFilter()
{
	bool bFound = false;
	for (int i = 0; i < m_pGameFilter->GetItemCount(); i++)
	{
		KeyValues *kv = m_pGameFilter->GetItemUserData(i);
		int iAppID = kv->GetInt( "appID", 0 );
		const char *pchGameDir = kv->GetString( "gamedir" );
		if ( iAppID == m_iLimitToAppID && ( !m_szGameFilter[0] || !Q_strncmp( pchGameDir, m_szGameFilter, Q_strlen( pchGameDir ) ) ) )
		{
			if ( i != m_pGameFilter->GetActiveItem() )
			{
				m_pGameFilter->ActivateItem(i);
			}
			bFound = true;
			break;
		}
	}
	if (!bFound)
	{
		// default to empty
		if ( 0 != m_pGameFilter->GetActiveItem() )
		{
			m_pGameFilter->ActivateItem(0);
		}
	}

	// only one mod is allowed in the game
	if ( ServerBrowserDialog().GetActiveModName() )
	{
		m_pGameFilter->SetEnabled( false );
		m_pGameFilter->SetText( ServerBrowserDialog().GetActiveGameName() );
	}
}

//-----------------------------------------------------------------------------
// Purpose: Handles incoming server refresh data
//			updates the server browser with the refreshed information from the server itself
//-----------------------------------------------------------------------------
void CBaseGamesPage::ServerResponded( gameserveritem_t &server )
{
	int nIndex = -1; // start at -1 and work backwards to find the next free slot for this adhoc query
	while ( m_mapServers.Find( nIndex ) != m_mapServers.InvalidIndex() )
		nIndex--;
	ServerResponded( nIndex, &server );
}


//-----------------------------------------------------------------------------
// Purpose: Callback for ISteamMatchmakingServerListResponse
//-----------------------------------------------------------------------------
void CBaseGamesPage::ServerResponded( int iServer )
{
#ifndef NO_STEAM
	gameserveritem_t *pServerItem = SteamMatchmakingServers()->GetServerDetails( m_eMatchMakingType, iServer );
	if ( !pServerItem )
	{
		Assert( !"Missing server response" );
		return;
	}
	ServerResponded( iServer, pServerItem );
#endif
}


//-----------------------------------------------------------------------------
// Purpose: Handles incoming server refresh data
//			updates the server browser with the refreshed information from the server itself
//-----------------------------------------------------------------------------
void CBaseGamesPage::ServerResponded( int iServer, gameserveritem_t *pServerItem )
{
	int iServerMap = m_mapServers.Find( iServer );
	if ( iServerMap == m_mapServers.InvalidIndex() )
	{
		netadr_t netAdr( pServerItem->m_NetAdr.GetIP(), pServerItem->m_NetAdr.GetQueryPort() );
		int iServerIP = m_mapServerIP.Find( netAdr );
		if ( iServerIP != m_mapServerIP.InvalidIndex() )
		{
			// if we already had this entry under another index remove the old entry
			int iServerMap = m_mapServers.Find( m_mapServerIP[ iServerIP ] );
			if ( iServerMap != m_mapServers.InvalidIndex() )
			{
				serverdisplay_t &server = m_mapServers[ iServerMap ];
				if ( m_pGameList->IsValidItemID( server.m_iListID ) )
					m_pGameList->RemoveItem( server.m_iListID );
				m_mapServers.RemoveAt( iServerMap );
			}
			m_mapServerIP.RemoveAt( iServerIP );
		}

		serverdisplay_t serverFind;
		serverFind.m_iListID = -1;
		serverFind.m_bDoNotRefresh = false;
		iServerMap = m_mapServers.Insert( iServer, serverFind );
		m_mapServerIP.Insert( netAdr, iServer );
	}

	serverdisplay_t *pServer = &m_mapServers[ iServerMap ];
	pServer->m_iServerID = iServer;
	Assert( pServerItem->m_NetAdr.GetIP() != 0 );

	// check filters
	bool removeItem = false;
	if ( !CheckPrimaryFilters( *pServerItem ) )
	{
		// server has been filtered at a primary level
		// remove from lists
		pServer->m_bDoNotRefresh = true;

		// remove from UI list
		removeItem = true;
	}
	else if (!CheckSecondaryFilters(  *pServerItem ))
	{
		// we still ping this server in the future; however it is removed from UI list
		removeItem = true;
	}

	if (removeItem)
	{
		if ( m_pGameList->IsValidItemID( pServer->m_iListID ) )
		{
			m_pGameList->RemoveItem( pServer->m_iListID );
			pServer->m_iListID = GetInvalidServerListID();
		}
		return;
	}

	// update UI
	KeyValues *kv;
	if ( m_pGameList->IsValidItemID( pServer->m_iListID ) )
	{
		// we're updating an existing entry
		kv = m_pGameList->GetItem( pServer->m_iListID );
		m_pGameList->SetUserData( pServer->m_iListID, pServer->m_iServerID );
	}
	else
	{
		// new entry
		kv = new KeyValues("Server");
	}

	kv->SetString("name", pServerItem->GetName());
	kv->SetString("map", pServerItem->m_szMap);
	kv->SetString("GameDir", pServerItem->m_szGameDir);
	kv->SetString("GameDesc", pServerItem->m_szGameDescription);
	kv->SetInt("password", pServerItem->m_bPassword ? 1 : 0);
	
	if ( pServerItem->m_nBotPlayers > 0 )
		kv->SetInt("bots", pServerItem->m_nBotPlayers);
	else
		kv->SetString("bots", "");
	
	if ( pServerItem->m_bSecure )
	{
		// show the denied icon if banned from secure servers, the secure icon otherwise
		kv->SetInt("secure", ServerBrowser().IsVACBannedFromGame( pServerItem->m_nAppID ) ?  4 : 3);
	}
	else
	{
		kv->SetInt("secure", 0);
	}

	kv->SetString( "IPAddr", pServerItem->m_NetAdr.GetConnectionAddressString() );

	int nAdjustedForBotsPlayers = max( 0, pServerItem->m_nPlayers - pServerItem->m_nBotPlayers );
	int nAdjustedForBotsMaxPlayers = max( 0, pServerItem->m_nMaxPlayers - pServerItem->m_nBotPlayers );

	char buf[32];
	Q_snprintf(buf, sizeof(buf), "%d / %d", nAdjustedForBotsPlayers, nAdjustedForBotsMaxPlayers);
	kv->SetString("Players", buf);
	
	kv->SetInt("Ping", pServerItem->m_nPing);

	kv->SetString("Tags", pServerItem->m_szGameTags );

	if ( pServerItem->m_ulTimeLastPlayed )
	{
		// construct a time string for last played time
		struct tm *now;
		now = localtime( (time_t*)&pServerItem->m_ulTimeLastPlayed );

		if ( now ) 
		{
			char buf[64];
			strftime(buf, sizeof(buf), "%a %d %b %I:%M%p", now);
			Q_strlower(buf + strlen(buf) - 4);
			kv->SetString("LastPlayed", buf);
		}
	}

	if ( pServer->m_bDoNotRefresh )
	{
		// clear out the vars
		kv->SetString("Ping", "");
		kv->SetWString("GameDesc", g_pVGuiLocalize->Find("#ServerBrowser_NotResponding"));
		kv->SetString("Players", "");
		kv->SetString("map", "");
	}

	if ( !m_pGameList->IsValidItemID( pServer->m_iListID ) )
	{
		// new server, add to list
		pServer->m_iListID = m_pGameList->AddItem(kv, pServer->m_iServerID, false, false);
		if ( m_bAutoSelectFirstItemInGameList && m_pGameList->GetItemCount() == 1 )
		{
			m_pGameList->AddSelectedItem( pServer->m_iListID );
		}
		
		kv->deleteThis();
	}
	else
	{
		// tell the list that we've changed the data
		m_pGameList->ApplyItemChanges( pServer->m_iListID );
		m_pGameList->SetItemVisible( pServer->m_iListID, true );
	}

	UpdateStatus();
	m_iServerRefreshCount++;
}

//-----------------------------------------------------------------------------
// Purpose: Handles filter dropdown being toggled
//-----------------------------------------------------------------------------
void CBaseGamesPage::OnButtonToggled(Panel *panel, int state)
{
	if (panel == m_pFilter)
	{
		int wide, tall;
		GetSize( wide, tall );
		SetSize( 624, 278 );

		if ( m_pCustomResFilename )
		{
			m_bFiltersVisible = false;
		}
		else
		{
			if ( m_pFilter->IsSelected() )
			{
				// drop down
				m_bFiltersVisible = true;
			}
			else
			{
				// hide filter area
				m_bFiltersVisible = false;
			}
		}

		UpdateDerivedLayouts();
		m_pFilter->SetSelected( m_bFiltersVisible );
		UpdateGameFilter();

		if ( m_hFont )
		{
			SETUP_PANEL( m_pGameList );
			m_pGameList->SetFont( m_hFont );
		}
		
		SetSize( wide, tall );

		InvalidateLayout();
	}
	else if (panel == m_pNoFullServersFilterCheck || panel == m_pNoEmptyServersFilterCheck || panel == m_pNoPasswordFilterCheck)
	{
		// treat changing these buttons like any other filter has changed
		OnTextChanged(panel, "");
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CBaseGamesPage::UpdateDerivedLayouts( void )
{
	char rgchControlSettings[MAX_PATH];
	if ( m_pCustomResFilename )
	{
		Q_snprintf( rgchControlSettings, sizeof( rgchControlSettings ), "%s", m_pCustomResFilename );
	}
	else
	{
		if ( m_pFilter->IsSelected() )
		{
			// drop down
			Q_snprintf( rgchControlSettings, sizeof( rgchControlSettings ), "servers/%sPage_Filters.res", "InternetGames" );
		}
		else
		{
			// hide filter area
			Q_snprintf( rgchControlSettings, sizeof( rgchControlSettings ), "servers/%sPage.res", "InternetGames" );
		}
	}

	LoadControlSettings( rgchControlSettings );
}

//-----------------------------------------------------------------------------
// Purpose: Called when the game dir combo box is changed
//-----------------------------------------------------------------------------
void CBaseGamesPage::OnTextChanged(Panel *panel, const char *text)
{
	if (!Q_stricmp(text, m_szComboAllText))
	{
		ComboBox *box = dynamic_cast<ComboBox *>(panel);
		if (box)
		{
			box->SetText("");
			text = "";
		}
	}

	// get filter settings from controls
	UpdateFilterSettings();

	// apply settings
	ApplyGameFilters();

	if ( m_bFiltersVisible && ( panel == m_pGameFilter || panel == m_pLocationFilter ) )
	{
		// if they changed games and/or region then cancel the refresh because the old list they are getting
		// will be for the wrong game, so stop and start a refresh
		StopRefresh(); 
		GetNewServerList(); 
	}
}

//-----------------------------------------------------------------------------
// Purpose: applies only the game filter to the current list
//-----------------------------------------------------------------------------
void CBaseGamesPage::ApplyGameFilters()
{
#ifndef NO_STEAM
	if ( !SteamMatchmakingServers() )
		return;
	// loop through all the servers checking filters
	FOR_EACH_MAP_FAST( m_mapServers, i )
	{
		serverdisplay_t &server = m_mapServers[ i ];
		gameserveritem_t *pServer = SteamMatchmakingServers()->GetServerDetails( m_eMatchMakingType, server.m_iServerID );
		if ( !pServer ) 
			continue;

		if (!CheckPrimaryFilters( *pServer ) || !CheckSecondaryFilters( *pServer ))
		{
			// server failed filtering, remove it
			server.m_bDoNotRefresh = true;
			if ( m_pGameList->IsValidItemID( server.m_iListID) )
			{
				// don't remove the server from list, just hide since this is a lot faster
				m_pGameList->SetItemVisible( server.m_iListID, false );
			}
		}
		else if ( BShowServer( server ) )
		{
			// server passed filters, so it can be refreshed again
			server.m_bDoNotRefresh = false;
			gameserveritem_t *pServer = SteamMatchmakingServers()->GetServerDetails( m_eMatchMakingType, server.m_iServerID );

			// re-add item to list
			if ( !m_pGameList->IsValidItemID( server.m_iListID ) )
			{
				KeyValues *kv = new KeyValues("Server");
				kv->SetString("name", pServer->GetName());
				kv->SetString("map", pServer->m_szMap);
				kv->SetString("GameDir", pServer->m_szGameDir);
				if ( pServer->m_szGameDescription[0] )
				{
					kv->SetString("GameDesc", pServer->m_szGameDescription );
				}
				else
				{
					kv->SetWString("GameDesc", g_pVGuiLocalize->Find("#ServerBrowser_PendingPing"));
				}

				int nAdjustedForBotsPlayers = max( 0, pServer->m_nPlayers - pServer->m_nBotPlayers );
				int nAdjustedForBotsMaxPlayers = max( 0, pServer->m_nMaxPlayers - pServer->m_nBotPlayers );

				char buf[256];
				Q_snprintf(buf, sizeof(buf), "%d / %d", nAdjustedForBotsPlayers, nAdjustedForBotsMaxPlayers);
				kv->SetString( "Players", buf);
				kv->SetInt( "Ping", pServer->m_nPing );
				kv->SetInt( "password", pServer->m_bPassword ? 1 : 0);
				if ( pServer->m_nBotPlayers > 0 )
					kv->SetInt("bots", pServer->m_nBotPlayers);
				else
					kv->SetString("bots", "");

				server.m_iListID = m_pGameList->AddItem(kv, server.m_iServerID, false, false);
				kv->deleteThis();
			}
			
			// make sure the server is visible
			m_pGameList->SetItemVisible( server.m_iListID, true );
		}
	}
#endif
	UpdateStatus();
	m_pGameList->SortList();
	InvalidateLayout();
	Repaint();
}

//-----------------------------------------------------------------------------
// Purpose: Resets UI server count
//-----------------------------------------------------------------------------
void CBaseGamesPage::UpdateStatus()
{
	if (m_pGameList->GetItemCount() > 1)
	{
		wchar_t header[256];
		wchar_t count[128];

		_itow( m_pGameList->GetItemCount(), count, 10 );
		g_pVGuiLocalize->ConstructString( header, sizeof( header ), g_pVGuiLocalize->Find( "#ServerBrowser_ServersCount"), 1, count );
		m_pGameList->SetColumnHeaderText(3, header);
	}
	else
	{
		m_pGameList->SetColumnHeaderText(3, g_pVGuiLocalize->Find("#ServerBrowser_Servers"));
	}
}

//-----------------------------------------------------------------------------
// Purpose: gets filter settings from controls
//-----------------------------------------------------------------------------
void CBaseGamesPage::UpdateFilterSettings()
{
	// game
	if ( ServerBrowserDialog().GetActiveModName() )
	{
		// overriding the game filter
		Q_strncpy(m_szGameFilter, ServerBrowserDialog().GetActiveModName(), sizeof(m_szGameFilter));
		m_iLimitToAppID = ServerBrowserDialog().GetActiveAppID();
		RecalculateFilterString();
		UpdateGameFilter();
	}
	else
	{
		KeyValues *data = m_pGameFilter->GetActiveItemUserData();
		if (data)
		{
			Q_strncpy( m_szGameFilter, data->GetString( "gamedir" ), sizeof( m_szGameFilter ) );
			if ( Q_strlen( m_szGameFilter ) > 0 ) // if there is a gamedir
			{
				m_iLimitToAppID = data->GetInt( "appid", 0 );
			}
			else
			{
				m_iLimitToAppID = 0;
			}
		}
		else
		{
			m_iLimitToAppID = 0;
		}
		m_pGameFilter->SetEnabled(true);
	}
	Q_strlower(m_szGameFilter);

	// map
	m_pMapFilter->GetText(m_szMapFilter, sizeof(m_szMapFilter) - 1);
	Q_strlower(m_szMapFilter);

	// ping
	char buf[256];
	m_pPingFilter->GetText(buf, sizeof(buf));
	if (buf[0])
	{
		m_iPingFilter = atoi(buf + 2);
	}
	else
	{
		m_iPingFilter = 0;
	}

	// players
	m_bFilterNoFullServers = m_pNoFullServersFilterCheck->IsSelected();
	m_bFilterNoEmptyServers = m_pNoEmptyServersFilterCheck->IsSelected();
	m_bFilterNoPasswordedServers = m_pNoPasswordFilterCheck->IsSelected();
	m_iSecureFilter = m_pSecureFilter->GetActiveItem();

	m_vecServerFilters.RemoveAll();

	// update master filter string text
	if (m_szGameFilter[0] && m_iLimitToAppID != 1002 ) // HACKHACK: Alfred - don't use a dir filter for RDKF
	{
		m_vecServerFilters.AddToTail( MatchMakingKeyValuePair_t( "gamedir", m_szGameFilter ) );
	}
	if (m_bFilterNoEmptyServers)
	{
		m_vecServerFilters.AddToTail( MatchMakingKeyValuePair_t( "empty", "1" ) );
	}
	if (m_bFilterNoFullServers)
	{
		m_vecServerFilters.AddToTail( MatchMakingKeyValuePair_t( "full", "1" ) );
	}
	if (m_iSecureFilter == FILTER_SECURESERVERSONLY)
	{
		m_vecServerFilters.AddToTail( MatchMakingKeyValuePair_t( "secure", "1" ) );
	}
	int regCode = GetRegionCodeToFilter();
	if ( regCode > 0 )
	{
		char szRegCode[ 32 ];
		Q_snprintf( szRegCode, sizeof(szRegCode), "%i", regCode );
		m_vecServerFilters.AddToTail( MatchMakingKeyValuePair_t( "region", szRegCode ) );		
	}

	// copy filter settings into filter file
	KeyValues *filter = ServerBrowserDialog().GetFilterSaveData(GetName());

	// only save the game filter if we're not overriding it
	if (!ServerBrowserDialog().GetActiveModName())
	{
		filter->SetString("game", m_szGameFilter);
		filter->SetInt( "appid", m_iLimitToAppID );
	}
	filter->SetString("map", m_szMapFilter);

	filter->SetInt("ping", m_iPingFilter);

	if ( m_pLocationFilter->GetItemCount() > 1 )
	{ 
		// only save this if there are options to choose from
		filter->SetInt("location", m_pLocationFilter->GetActiveItem());
	}
	
	filter->SetInt("NoFull", m_bFilterNoFullServers);
	filter->SetInt("NoEmpty", m_bFilterNoEmptyServers);
	filter->SetInt("NoPassword", m_bFilterNoPasswordedServers);
	filter->SetInt("Secure", m_iSecureFilter);

	filter->SetString("gametype", "notags" );
	m_vecServerFilters.AddToTail( MatchMakingKeyValuePair_t( "gametype", "notags" ) );

	OnSaveFilter(filter);

	RecalculateFilterString();
}


//-----------------------------------------------------------------------------
// Purpose: allow derived classes access to the saved filter string
//-----------------------------------------------------------------------------
void CBaseGamesPage::OnSaveFilter(KeyValues *filter)
{
}

//-----------------------------------------------------------------------------
// Purpose: allow derived classes access to the saved filter string
//-----------------------------------------------------------------------------
void CBaseGamesPage::OnLoadFilter(KeyValues *filter)
{
}

//-----------------------------------------------------------------------------
// Purpose: reconstructs the filter description string from the current filter settings
//-----------------------------------------------------------------------------
void CBaseGamesPage::RecalculateFilterString()
{
	wchar_t unicode[2048], tempUnicode[128], spacerUnicode[8];
	unicode[0] = 0;
	int iTempUnicodeSize = sizeof( tempUnicode );

	Q_UTF8ToUnicode( "; ", spacerUnicode, sizeof( spacerUnicode ) );

	if (m_szGameFilter[0])
	{
		Q_UTF8ToUnicode( ModList().GetModNameForModDir( m_szGameFilter, m_iLimitToAppID ), tempUnicode, iTempUnicodeSize );
		wcscat( unicode, tempUnicode );
		wcscat( unicode, spacerUnicode );
	}

	if (m_iSecureFilter == FILTER_SECURESERVERSONLY)
	{
		wcscat( unicode, g_pVGuiLocalize->Find( "#ServerBrowser_FilterDescSecureOnly" ) );
		wcscat( unicode, spacerUnicode );
	}
	else if (m_iSecureFilter == FILTER_INSECURESERVERSONLY)
	{
		wcscat( unicode, g_pVGuiLocalize->Find( "#ServerBrowser_FilterDescInsecureOnly" ) );
		wcscat( unicode, spacerUnicode );
	}

	if (m_pLocationFilter->GetActiveItem() > 0)
	{
		m_pLocationFilter->GetText(tempUnicode, sizeof(tempUnicode));
		wcscat( unicode, tempUnicode );
		wcscat( unicode, spacerUnicode );
	}

	if (m_iPingFilter)
	{
		char tmpBuf[16];
		_itoa( m_iPingFilter, tmpBuf, 10 );

		wcscat( unicode, g_pVGuiLocalize->Find( "#ServerBrowser_FilterDescLatency" ) );
		Q_UTF8ToUnicode( " < ", tempUnicode, iTempUnicodeSize );
		wcscat( unicode, tempUnicode );
		Q_UTF8ToUnicode(tmpBuf, tempUnicode, iTempUnicodeSize );
		wcscat( unicode, tempUnicode );	
		wcscat( unicode, spacerUnicode );
	}

	if (m_bFilterNoFullServers)
	{
		wcscat( unicode, g_pVGuiLocalize->Find( "#ServerBrowser_FilterDescNotFull" ) );
		wcscat( unicode, spacerUnicode );
	}

	if (m_bFilterNoEmptyServers)
	{
		wcscat( unicode, g_pVGuiLocalize->Find( "#ServerBrowser_FilterDescNotEmpty" ) );
		wcscat( unicode, spacerUnicode );
	}

	if (m_bFilterNoPasswordedServers)
	{
		wcscat( unicode, g_pVGuiLocalize->Find( "#ServerBrowser_FilterDescNoPassword" ) );
		wcscat( unicode, spacerUnicode );
	}

	if (m_szMapFilter[0])
	{
		Q_UTF8ToUnicode( m_szMapFilter, tempUnicode, iTempUnicodeSize );
		wcscat( unicode, tempUnicode );
	}

	m_pFilterString->SetText(unicode);
}

//-----------------------------------------------------------------------------
// Purpose: Checks to see if the server passes the primary filters
//			if the server fails the filters, it will not be refreshed again
//-----------------------------------------------------------------------------
bool CBaseGamesPage::CheckPrimaryFilters( gameserveritem_t &server )
{
	if (m_szGameFilter[0] && ( server.m_szGameDir[0] || server.m_nPing ) && Q_stricmp(m_szGameFilter, server.m_szGameDir ) && server.m_nAppID != 1002 ) // HACKHACK Alfred - don't apply gamedir filter to rdkf
	{
		return false;
	}
		
	if ( server.m_nAppID < 200 || ( server.m_nAppID > 900 && server.m_nAppID != 4000 ) )
		return false;

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: Checks to see if a server passes the secondary set of filters
//			server will be continued to be pinged if it fails the filter, since
//			the relvent server data is dynamic
//-----------------------------------------------------------------------------
bool CBaseGamesPage::CheckSecondaryFilters( gameserveritem_t &server )
{
	if ( m_bFilterNoEmptyServers && (server.m_nPlayers - server.m_nBotPlayers) < 1 )
	{
		return false;
	}

	if ( m_bFilterNoFullServers && server.m_nPlayers >= server.m_nMaxPlayers )
	{
		return false;
	}

	if ( m_iPingFilter && server.m_nPing > m_iPingFilter )
	{
		return false;
	}

	if ( m_bFilterNoPasswordedServers && server.m_bPassword )
	{
		return false;
	}

	if ( m_iSecureFilter == FILTER_SECURESERVERSONLY && !server.m_bSecure )
	{
		return false;
	}

	if ( m_iSecureFilter == FILTER_INSECURESERVERSONLY && server.m_bSecure )
	{
		return false;
	}

	// compare the first few characters of the filter name
	int count = Q_strlen( m_szMapFilter );
	if ( count && Q_strnicmp( server.m_szMap, m_szMapFilter, count ) )
	{
		return false;
	}

	return CheckTagFilter( server );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
uint32 CBaseGamesPage::GetServerFilters( MatchMakingKeyValuePair_t **pFilters )
{
	*pFilters = m_vecServerFilters.Base();
	return m_vecServerFilters.Count();
}


//-----------------------------------------------------------------------------
// Purpose: call to let the UI now whether the game list is currently refreshing
//-----------------------------------------------------------------------------
void CBaseGamesPage::SetRefreshing(bool state)
{
	if (state)
	{
		ServerBrowserDialog().UpdateStatusText("#ServerBrowser_RefreshingServerList");

		// clear message in panel
		m_pGameList->SetEmptyListText("");
		m_pRefreshAll->SetText("#ServerBrowser_StopRefreshingList");
		m_pRefreshAll->SetCommand("stoprefresh");
		m_pRefreshQuick->SetEnabled(false);
	}
	else
	{
		ServerBrowserDialog().UpdateStatusText("");
		if (SupportsItem(IGameList::GETNEWLIST))
		{
			m_pRefreshAll->SetText("#ServerBrowser_RefreshAll");
		}
		else
		{
			m_pRefreshAll->SetText("#ServerBrowser_Refresh");
		}
		m_pRefreshAll->SetCommand("GetNewList");

		// 'refresh quick' button is only enabled if there are servers in the list
		if (m_pGameList->GetItemCount() > 0)
		{
			m_pRefreshQuick->SetEnabled(true);
		}
		else
		{
			m_pRefreshQuick->SetEnabled(false);
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CBaseGamesPage::OnCommand(const char *command)
{
	if (!Q_stricmp(command, "Connect"))
	{
		OnBeginConnect();
	}
	else if (!Q_stricmp(command, "stoprefresh"))
	{
		// cancel the existing refresh
		StopRefresh();
	}
	else if ( !Q_stricmp(command, "refresh") )
	{
#ifndef NO_STEAM
		if ( SteamMatchmakingServers() )
			SteamMatchmakingServers()->RefreshQuery( m_eMatchMakingType );
		SetRefreshing( true );
		m_iServerRefreshCount = 0;
#endif
	}
	else if (!Q_stricmp(command, "GetNewList"))
	{
		GetNewServerList();
	}
	else
	{
		BaseClass::OnCommand(command);
	}
}

//-----------------------------------------------------------------------------
// Purpose: called when a row gets selected in the list
//-----------------------------------------------------------------------------
void CBaseGamesPage::OnItemSelected()
{
	if (m_pGameList->GetSelectedItemsCount() < 1)
	{
		m_pConnect->SetEnabled(false);
	}
	else
	{
		m_pConnect->SetEnabled(true);
	}
}

//-----------------------------------------------------------------------------
// Purpose: refreshes server list on F5
//-----------------------------------------------------------------------------
void CBaseGamesPage::OnKeyCodePressed(vgui::KeyCode code)
{
	if (code == KEY_F5)
	{
		StartRefresh();
	}
	else
	{
		BaseClass::OnKeyCodePressed(code);
	}
}


//-----------------------------------------------------------------------------
// Purpose: Handle enter pressed in the games list page. Return true
// to intercept the message instead of passing it on through vgui.
//-----------------------------------------------------------------------------
bool CBaseGamesPage::OnGameListEnterPressed()
{
	return false;
}


//-----------------------------------------------------------------------------
// Purpose: Get the # items selected in the game list.
//-----------------------------------------------------------------------------
int CBaseGamesPage::GetSelectedItemsCount()
{
	return m_pGameList->GetSelectedItemsCount();
}


//-----------------------------------------------------------------------------
// Purpose: adds a server to the favorites
//-----------------------------------------------------------------------------
void CBaseGamesPage::OnAddToFavorites()
{
#ifndef NO_STEAM
	if ( !SteamMatchmakingServers() )
		return;

	// loop through all the selected favorites
	for (int i = 0; i < m_pGameList->GetSelectedItemsCount(); i++)
	{
		int serverID = m_pGameList->GetItemUserData(m_pGameList->GetSelectedItem(i));

		gameserveritem_t *pServer = SteamMatchmakingServers()->GetServerDetails( m_eMatchMakingType, serverID );
		if ( pServer )
		{
			// add to favorites list
			ServerBrowserDialog().AddServerToFavorites(*pServer);
		}
	}
#endif
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CBaseGamesPage::ServerFailedToRespond( int iServer )
{
	ServerResponded( iServer );
}


//-----------------------------------------------------------------------------
// Purpose: removes the server from the UI list
//-----------------------------------------------------------------------------
void CBaseGamesPage::RemoveServer( serverdisplay_t &server )
{
	if ( m_pGameList->IsValidItemID( server.m_iListID ) )
	{
		// don't remove the server from list, just hide since this is a lot faster
		m_pGameList->SetItemVisible( server.m_iListID, false );

		// find the row in the list and kill
		//	m_pGameList->RemoveItem(server.listEntryID);
		//	server.listEntryID = GetInvalidServerListID();
	}

	UpdateStatus();
}


//-----------------------------------------------------------------------------
// Purpose: refreshes a single server
//-----------------------------------------------------------------------------
void CBaseGamesPage::OnRefreshServer( int serverID )
{
#ifndef NO_STEAM
	if ( !SteamMatchmakingServers() )
		return;

	// walk the list of selected servers refreshing them
	for (int i = 0; i < m_pGameList->GetSelectedItemsCount(); i++)
	{
		int serverID = m_pGameList->GetItemUserData(m_pGameList->GetSelectedItem(i));

		// refresh this server
		SteamMatchmakingServers()->RefreshServer( m_eMatchMakingType, serverID );
	}

	SetRefreshing(IsRefreshing());
#endif
}


//-----------------------------------------------------------------------------
// Purpose: starts the servers refreshing
//-----------------------------------------------------------------------------
void CBaseGamesPage::StartRefresh()
{
#ifndef NO_STEAM
	if ( !SteamMatchmakingServers() )
		return;

	ClearServerList();
	MatchMakingKeyValuePair_t *pFilters;
	int nFilters = GetServerFilters( &pFilters );

	switch ( m_eMatchMakingType )
	{
	case eFavoritesServer:
		SteamMatchmakingServers()->RequestFavoritesServerList( GetFilterAppID(), &pFilters, nFilters, this );
		break;
	case eHistoryServer:
		SteamMatchmakingServers()->RequestHistoryServerList( GetFilterAppID(), &pFilters, nFilters, this );
		break;
	case eInternetServer:
		SteamMatchmakingServers()->RequestInternetServerList( GetFilterAppID(), &pFilters, nFilters, this );
		break;
	case eSpectatorServer:
		SteamMatchmakingServers()->RequestSpectatorServerList( GetFilterAppID(), &pFilters, nFilters, this );
		break;
	case eFriendsServer:
		SteamMatchmakingServers()->RequestFriendsServerList( GetFilterAppID(), &pFilters, nFilters, this );
		break;
	case eLANServer:
		SteamMatchmakingServers()->RequestLANServerList( GetFilterAppID(), this );
		break;
	default:
		Assert( !"Unknown server type" );
		break;
	}

	SetRefreshing( true );
#endif
	m_iServerRefreshCount = 0;
}


//-----------------------------------------------------------------------------
// Purpose: Remove all the servers we currently have
//-----------------------------------------------------------------------------
void CBaseGamesPage::ClearServerList()
{ 
	m_mapServers.RemoveAll(); 
	m_mapServerIP.RemoveAll();
	m_pGameList->RemoveAll();
}


//-----------------------------------------------------------------------------
// Purpose: get a new list of servers from the backend
//-----------------------------------------------------------------------------
void CBaseGamesPage::GetNewServerList()
{
	StartRefresh();
}


//-----------------------------------------------------------------------------
// Purpose: stops current refresh/GetNewServerList()
//-----------------------------------------------------------------------------
void CBaseGamesPage::StopRefresh()
{
	// clear update states
	m_iServerRefreshCount = 0;
#ifndef NO_STEAM
	// Stop the server list refreshing
	if ( SteamMatchmakingServers() )
		SteamMatchmakingServers()->CancelQuery( m_eMatchMakingType );
#endif
	// update UI
	RefreshComplete( eServerResponded );
}

//-----------------------------------------------------------------------------
// Purpose: returns true if the list is currently refreshing servers
//-----------------------------------------------------------------------------
bool CBaseGamesPage::IsRefreshing()
{
#ifndef NO_STEAM
	return SteamMatchmakingServers() && SteamMatchmakingServers()->IsRefreshing( m_eMatchMakingType );
#else
	return false;
#endif
}

//-----------------------------------------------------------------------------
// Purpose: Activates the page, starts refresh
//-----------------------------------------------------------------------------
void CBaseGamesPage::OnPageShow()
{
	StartRefresh();
}

//-----------------------------------------------------------------------------
// Purpose: Called on page hide, stops any refresh
//-----------------------------------------------------------------------------
void CBaseGamesPage::OnPageHide()
{
	StopRefresh();
}

//-----------------------------------------------------------------------------
// Purpose: initiates server connection
//-----------------------------------------------------------------------------
void CBaseGamesPage::OnBeginConnect()
{
	if (!m_pGameList->GetSelectedItemsCount())
		return;

	// get the server
	int serverID = m_pGameList->GetItemUserData( m_pGameList->GetSelectedItem(0) );

	// Stop the current refresh
	StopRefresh();

	// join the game
	ServerBrowserDialog().JoinGame(this, serverID);
}

//-----------------------------------------------------------------------------
// Purpose: Displays the current game info without connecting
//-----------------------------------------------------------------------------
void CBaseGamesPage::OnViewGameInfo()
{
	if (!m_pGameList->GetSelectedItemsCount())
		return;

	// get the server
	int serverID = m_pGameList->GetItemUserData( m_pGameList->GetSelectedItem(0) );

	// Stop the current refresh
	StopRefresh();

	// join the game
	ServerBrowserDialog().OpenGameInfoDialog(this, serverID);
}


//-----------------------------------------------------------------------------
// Purpose: Refresh if our favorites list changed
//-----------------------------------------------------------------------------
#ifndef NO_STEAM
void CBaseGamesPage::OnFavoritesMsg( FavoritesListChanged_t *pFavListChanged )
{
	switch ( m_eMatchMakingType )
	{
	case eInternetServer:
	case eLANServer:
	case eSpectatorServer:
	case eFriendsServer:
		break;
	case eFavoritesServer:
	case eHistoryServer:
		{
		int iIPServer = m_mapServerIP.Find( netadr_t( pFavListChanged->m_nIP, pFavListChanged->m_nQueryPort ) );
		if ( iIPServer == m_mapServerIP.InvalidIndex() )
		{
			if ( pFavListChanged->m_bAdd )	
			{
#ifndef NO_STEAM
				if ( SteamMatchmakingServers() )
					SteamMatchmakingServers()->PingServer( pFavListChanged->m_nIP, pFavListChanged->m_nQueryPort, this );
#endif
			}
			// ignore deletes of fav's we didn't have
		}
		else
		{
			if ( pFavListChanged->m_bAdd )	
			{
				if ( m_mapServerIP[ iIPServer ] > 0 )
					ServerResponded( m_mapServerIP[ iIPServer ] );
			}
			else
			{
				int iServer = m_mapServers.Find( m_mapServerIP[ iIPServer ] );
				serverdisplay_t &server = m_mapServers[ iServer ];
				RemoveServer( server );
			}
		}
		}
		break;
	default:
		Assert( !"unknown matchmaking type" );
	};
}
#endif
