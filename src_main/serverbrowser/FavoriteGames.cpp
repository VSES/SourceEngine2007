//========= Copyright © 1996-2001, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

#include "pch_serverbrowser.h"

using namespace vgui;

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CFavoriteGames::CFavoriteGames(vgui::Panel *parent) : 
	CBaseGamesPage(parent, "FavoriteGames", eFavoritesServer )
{
	m_bRefreshOnListReload = false;
}

//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CFavoriteGames::~CFavoriteGames()
{
}

//-----------------------------------------------------------------------------
// Purpose: loads favorites list from disk
//-----------------------------------------------------------------------------
void CFavoriteGames::LoadFavoritesList()
{
#ifndef NO_STEAM
	if ( SteamMatchmaking() && SteamMatchmaking()->GetFavoriteGameCount() == 0 )
	{
		// set empty message
		m_pGameList->SetEmptyListText("#ServerBrowser_NoFavoriteServers");
	}
	else
	{
		m_pGameList->SetEmptyListText("#ServerBrowser_NoInternetGamesResponded");

	}

	if ( m_bRefreshOnListReload )
	{
		m_bRefreshOnListReload = false;
		StartRefresh();
	}
#endif
}


//-----------------------------------------------------------------------------
// Purpose: returns true if the game list supports the specified ui elements
//-----------------------------------------------------------------------------
bool CFavoriteGames::SupportsItem(InterfaceItem_e item)
{
	switch (item)
	{
	case FILTERS:
	case ADDSERVER:
		return true;

	case ADDCURRENTSERVER:
		return !IsSteam();
	
	case GETNEWLIST:
	default:
		return false;
	}
}


//-----------------------------------------------------------------------------
// Purpose: called when the current refresh list is complete
//-----------------------------------------------------------------------------
void CFavoriteGames::RefreshComplete( EMatchMakingServerResponse response )
{
#ifndef NO_STEAM
	SetRefreshing(false);
	if ( SteamMatchmaking() && SteamMatchmaking()->GetFavoriteGameCount() == 0 )
	{
		// set empty message
		m_pGameList->SetEmptyListText("#ServerBrowser_NoFavoriteServers");
	}
	else
	{
		m_pGameList->SetEmptyListText("#ServerBrowser_NoInternetGamesResponded");

	}
	m_pGameList->SortList();
#endif
}

//-----------------------------------------------------------------------------
// Purpose: opens context menu (user right clicked on a server)
//-----------------------------------------------------------------------------
void CFavoriteGames::OnOpenContextMenu(int itemID)
{
	CServerContextMenu *menu = ServerBrowserDialog().GetContextMenu(m_pGameList);
	if (m_pGameList->GetSelectedItemsCount())
	{
		// get the server
		int serverID = m_pGameList->GetItemUserData(m_pGameList->GetSelectedItem(0));
		
		// Activate context menu
		menu->ShowMenu(this, serverID, true, true, true, false);
		menu->AddMenuItem("RemoveServer", "#ServerBrowser_RemoveServerFromFavorites", new KeyValues("RemoveFromFavorites"), this);
	}
	else
	{
		// no selected rows, so don't display default stuff in menu
		menu->ShowMenu( this,(uint32)-1, false, false, false, false );
	}
	
	menu->AddMenuItem("AddServerByName", "#ServerBrowser_AddServerByIP", new KeyValues("AddServerByName"), this);
}


//-----------------------------------------------------------------------------
// Purpose: removes a server from the favorites
//-----------------------------------------------------------------------------
void CFavoriteGames::OnRemoveFromFavorites()
{
#ifndef NO_STEAM
	if ( !SteamMatchmakingServers() || !SteamMatchmaking() )
		return;

	// iterate the selection
	for ( int iGame = 0; iGame < m_pGameList->GetSelectedItemsCount(); iGame++ )
	{
		int itemID = m_pGameList->GetSelectedItem( iGame );
		int serverID = m_pGameList->GetItemData(itemID)->userData;
		
		gameserveritem_t *pServer = SteamMatchmakingServers()->GetServerDetails( eFavoritesServer, serverID );
		
		if ( pServer )
		{
			SteamMatchmaking()->RemoveFavoriteGame2( pServer->m_nAppID, pServer->m_NetAdr.GetIP(), pServer->m_NetAdr.GetConnectionPort(), pServer->m_NetAdr.GetQueryPort(), k_unFavoriteFlagFavorite );
		}
	}
#endif
	UpdateStatus();	
	InvalidateLayout();
	Repaint();
}

//-----------------------------------------------------------------------------
// Purpose: Adds a server by IP address
//-----------------------------------------------------------------------------
void CFavoriteGames::OnAddServerByName()
{
	// open the add server dialog
	CDialogAddServer *dlg = new CDialogAddServer( &ServerBrowserDialog(), this );
	dlg->MoveToCenterOfScreen();
	dlg->DoModal();
}

//-----------------------------------------------------------------------------
// Purpose: Adds the currently connected server to the list
//-----------------------------------------------------------------------------
void CFavoriteGames::OnAddCurrentServer()
{
	gameserveritem_t *pConnected = ServerBrowserDialog().GetCurrentConnectedServer();
#ifndef NO_STEAM
	if ( pConnected && SteamMatchmaking() )
	{
		SteamMatchmaking()->AddFavoriteGame2( pConnected->m_nAppID, pConnected->m_NetAdr.GetIP(), pConnected->m_NetAdr.GetConnectionPort(), pConnected->m_NetAdr.GetQueryPort(), k_unFavoriteFlagFavorite, time( NULL ) );
		m_bRefreshOnListReload = true;
	}
#endif
}

//-----------------------------------------------------------------------------
// Purpose: Parse posted messages
//			 
//-----------------------------------------------------------------------------
void CFavoriteGames::OnCommand(const char *command)
{
	if (!Q_stricmp(command, "AddServerByName"))
	{
		OnAddServerByName();
	}
	else if (!Q_stricmp(command, "AddCurrentServer" ))
	{
		OnAddCurrentServer();
	}
	else
	{
		BaseClass::OnCommand(command);
	}
}

//-----------------------------------------------------------------------------
// Purpose: enables adding server
//-----------------------------------------------------------------------------
void CFavoriteGames::OnConnectToGame()
{
	m_pAddCurrentServer->SetEnabled( true );
}

//-----------------------------------------------------------------------------
// Purpose: disables adding current server
//-----------------------------------------------------------------------------
void CFavoriteGames::OnDisconnectFromGame( void )
{
	m_pAddCurrentServer->SetEnabled( false );
}
