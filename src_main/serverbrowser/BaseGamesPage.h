//========= Copyright © 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

#ifndef BASEGAMESPAGE_H
#define BASEGAMESPAGE_H
#ifdef _WIN32
#pragma once
#endif


class CBaseGamesPage;

//-----------------------------------------------------------------------------
// Purpose: Acts like a regular ListPanel but forwards enter key presses
// to its outer control.
//-----------------------------------------------------------------------------
class CGameListPanel : public vgui::ListPanel
{
public:
	DECLARE_CLASS_SIMPLE( CGameListPanel, vgui::ListPanel );
	
	CGameListPanel( CBaseGamesPage *pOuter, const char *pName );
	
	virtual void OnKeyCodeTyped(vgui::KeyCode code);

private:
	CBaseGamesPage *m_pOuter;
};


//-----------------------------------------------------------------------------
// Purpose: Base property page for all the games lists (internet/favorites/lan/etc.)
//-----------------------------------------------------------------------------
class CBaseGamesPage : public vgui::PropertyPage, public IGameList, public ISteamMatchmakingServerListResponse, public ISteamMatchmakingPingResponse
{
	DECLARE_CLASS_SIMPLE( CBaseGamesPage, vgui::PropertyPage );

public:
	CBaseGamesPage( vgui::Panel *parent, const char *name, EMatchMakingType eType, const char *pCustomResFilename=NULL);
	~CBaseGamesPage();

	virtual void PerformLayout();
	virtual void ApplySchemeSettings(vgui::IScheme *pScheme);

	// gets information about specified server
	virtual gameserveritem_t *GetServer(unsigned int serverID);

	uint32 GetServerFilters( MatchMakingKeyValuePair_t **pFilters );

	virtual void SetRefreshing(bool state);

	// loads filter settings from disk
	virtual void LoadFilterSettings();

	// Called by CGameList when the enter key is pressed.
	// This is overridden in the add server dialog - since there is no Connect button, the message
	// never gets handled, but we want to add a server when they dbl-click or press enter.
	virtual bool OnGameListEnterPressed();
	
	int GetSelectedItemsCount();

	// adds a server to the favorites
	MESSAGE_FUNC( OnAddToFavorites, "AddToFavorites" );

	virtual void StartRefresh();

	virtual void UpdateDerivedLayouts( void );
#ifndef NO_STEAM
	STEAM_CALLBACK( CBaseGamesPage, OnFavoritesMsg, FavoritesListChanged_t, m_CallbackFavoritesMsg );
#endif
protected:
	virtual void OnCommand(const char *command);
	virtual void OnKeyCodePressed(vgui::KeyCode code);
	virtual int GetRegionCodeToFilter() { return -1; }

	MESSAGE_FUNC( OnItemSelected, "ItemSelected" );

	// applies games filters to current list
	void ApplyGameFilters();
	// updates server count UI
	void UpdateStatus();

	// ISteamMatchmakingServerListResponse callbacks
	virtual void ServerResponded( int iServer );
	virtual void ServerResponded( int iServer, gameserveritem_t *pServerItem );
	virtual void ServerFailedToRespond( int iServer );
	virtual void RefreshComplete( EMatchMakingServerResponse response ) = 0;

	// ISteamMatchmakingPingResponse callbacks
	virtual void ServerResponded( gameserveritem_t &server );
	virtual void ServerFailedToRespond() {}

	// Removes server from list
	void RemoveServer( serverdisplay_t &server );

	virtual bool BShowServer( serverdisplay_t &server ) { return server.m_bDoNotRefresh; } 
	void ClearServerList();

	// filtering methods
	// returns true if filters passed; false if failed
	virtual bool CheckPrimaryFilters( gameserveritem_t &server);
	virtual bool CheckSecondaryFilters( gameserveritem_t &server);
	virtual bool CheckTagFilter( gameserveritem_t &server ) { return true; }
	virtual int GetInvalidServerListID();

	virtual void OnSaveFilter(KeyValues *filter);
	virtual void OnLoadFilter(KeyValues *filter);
	virtual void UpdateFilterSettings();

	// whether filter settings limit which master server to query
	uint32 GetFilterAppID() { return m_iLimitToAppID; }
	
	virtual void GetNewServerList();
	virtual void StopRefresh();
	virtual bool IsRefreshing();
	virtual void OnPageShow();
	virtual void OnPageHide();

	// called when Connect button is pressed
	MESSAGE_FUNC( OnBeginConnect, "ConnectToServer" );
	// called to look at game info
	MESSAGE_FUNC( OnViewGameInfo, "ViewGameInfo" );
	// refreshes a single server
	MESSAGE_FUNC_INT( OnRefreshServer, "RefreshServer", serverID );

	// If true, then we automatically select the first item that comes into the games list.
	bool m_bAutoSelectFirstItemInGameList;

	CGameListPanel *m_pGameList;
	vgui::ComboBox *m_pLocationFilter;

	// command buttons
	vgui::Button *m_pConnect;
	vgui::Button *m_pRefreshAll;
	vgui::Button *m_pRefreshQuick;
	vgui::Button *m_pAddServer;
	vgui::Button *m_pAddCurrentServer;
	vgui::Button *m_pAddToFavoritesButton;
	vgui::ToggleButton *m_pFilter;

	CUtlMap<int, serverdisplay_t> m_mapServers;
	CUtlMap<netadr_t, int> m_mapServerIP;
	CUtlVector<MatchMakingKeyValuePair_t> m_vecServerFilters;
	int m_iServerRefreshCount;

	EMatchMakingType m_eMatchMakingType;

protected:
	virtual void CreateFilters();
	virtual void UpdateGameFilter();

	MESSAGE_FUNC_PTR_CHARPTR( OnTextChanged, "TextChanged", panel, text );
	MESSAGE_FUNC_PTR_INT( OnButtonToggled, "ButtonToggled", panel, state );

private:
	void RequestServersResponse( int iServer, EMatchMakingServerResponse response, bool bLastServer ); // callback for matchmaking interface

	void RecalculateFilterString();

	// If set, it uses the specified resfile name instead of its default one.
	const char *m_pCustomResFilename;

	// filter controls
	vgui::ComboBox *m_pGameFilter;
	vgui::TextEntry *m_pMapFilter;
	vgui::ComboBox *m_pPingFilter;
	vgui::ComboBox *m_pSecureFilter;
	vgui::CheckButton *m_pNoFullServersFilterCheck;
	vgui::CheckButton *m_pNoEmptyServersFilterCheck;
	vgui::CheckButton *m_pNoPasswordFilterCheck;
	vgui::Label *m_pFilterString;
	char m_szComboAllText[64];

	KeyValues *m_pFilters; // base filter data
	bool m_bFiltersVisible;	// true if filter section is currently visible
	vgui::HFont m_hFont;

	// filter data
	char m_szGameFilter[32];
	char m_szMapFilter[32];
	int	m_iPingFilter;
	bool m_bFilterNoFullServers;
	bool m_bFilterNoEmptyServers;
	bool m_bFilterNoPasswordedServers;
	int m_iSecureFilter;

	int m_iLimitToAppID;
};

#endif // BASEGAMESPAGE_H
