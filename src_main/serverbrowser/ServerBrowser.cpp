//========= Copyright © 1996-2001, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

#include "pch_serverbrowser.h"

// expose the server browser interfaces
CServerBrowser g_ServerBrowserSingleton;
EXPOSE_SINGLE_INTERFACE_GLOBALVAR(CServerBrowser, IServerBrowser, SERVERBROWSER_INTERFACE_VERSION, g_ServerBrowserSingleton);
EXPOSE_SINGLE_INTERFACE_GLOBALVAR(CServerBrowser, IVGuiModule, "VGuiModuleServerBrowser001", g_ServerBrowserSingleton); // the interface loaded by PlatformMenu.vdf

// singleton accessor
CServerBrowser &ServerBrowser()
{
	return g_ServerBrowserSingleton;
}

IRunGameEngine *g_pRunGameEngine = NULL;
IAppInformation *g_pAppInformation = NULL;

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CServerBrowser::CServerBrowser()
{
}


//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CServerBrowser::~CServerBrowser()
{
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CServerBrowser::CreateDialog()
{
	if (!m_hInternetDlg.Get())
	{
		m_hInternetDlg = new CServerBrowserDialog(NULL); // SetParent() call below fills this in
		m_hInternetDlg->Initialize();
	}
}


//-----------------------------------------------------------------------------
// Purpose: links to vgui and engine interfaces
//-----------------------------------------------------------------------------
bool CServerBrowser::Initialize(CreateInterfaceFn *factorylist, int factoryCount)
{
	ConnectTier1Libraries( factorylist, factoryCount );
	ConVar_Register();
	ConnectTier2Libraries( factorylist, factoryCount );
	ConnectTier3Libraries( factorylist, factoryCount );
	g_pRunGameEngine = NULL;
	g_pAppInformation = NULL;
#ifndef NO_STEAM
	SteamAPI_Init();
#endif

	for (int i = 0; i < factoryCount; i++)
	{
		if (!g_pRunGameEngine)
		{
			g_pRunGameEngine = (IRunGameEngine *)(factorylist[i])(RUNGAMEENGINE_INTERFACE_VERSION, NULL);
		}
		if (!g_pAppInformation)
		{
			g_pAppInformation = (IAppInformation *)(factorylist[i])(APPINFORMATION_INTERFACE_VERSION, NULL);
		}
	}

	// load the vgui interfaces
#if defined( STEAM ) || defined( HL1 )
	if ( !vgui::VGuiControls_Init("ServerBrowser", factorylist, factoryCount) )
#else
	if ( !vgui::VGui_InitInterfacesList("ServerBrowser", factorylist, factoryCount) )
#endif
		return false;

	// load localization file
	g_pVGuiLocalize->AddFile( "servers/serverbrowser_%language%.txt" );
	return true;
}


//-----------------------------------------------------------------------------
// Purpose: links to other modules interfaces (tracker)
//-----------------------------------------------------------------------------
bool CServerBrowser::PostInitialize(CreateInterfaceFn *modules, int factoryCount)
{
	// find the interfaces we need
	for (int i = 0; i < factoryCount; i++)
	{
		if (!g_pRunGameEngine)
		{
			g_pRunGameEngine = (IRunGameEngine *)(modules[i])(RUNGAMEENGINE_INTERFACE_VERSION, NULL);
		}

		if (!g_pAppInformation)
		{
			g_pAppInformation = (IAppInformation *)(modules[i])(APPINFORMATION_INTERFACE_VERSION, NULL);
		}

	}

	CreateDialog();
	m_hInternetDlg->SetVisible(false);

	// g_pAppInformation ISN'T required
	return g_pRunGameEngine;
}


//-----------------------------------------------------------------------------
// Purpose: true if the user can't play a game due to VAC banning
//-----------------------------------------------------------------------------
bool CServerBrowser::IsVACBannedFromGame( int nAppID )
{
	return false;
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CServerBrowser::IsValid()
{
	return ( g_pRunGameEngine );
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CServerBrowser::Activate()
{
	static bool firstTimeOpening = true;
	if ( firstTimeOpening )
	{
		m_hInternetDlg->LoadUserData(); // reload the user data the first time the dialog is made visible, helps with the lag between module load and
										// steamui getting Deactivate() call
		firstTimeOpening = false;
	}

	Open();
	return true;
}


//-----------------------------------------------------------------------------
// Purpose: called when the server browser gets used in the game
//-----------------------------------------------------------------------------
void CServerBrowser::Deactivate()
{
	if (m_hInternetDlg.Get())
	{
		m_hInternetDlg->SaveUserData();
	}
}


//-----------------------------------------------------------------------------
// Purpose: called when the server browser is no longer being used in the game
//-----------------------------------------------------------------------------
void CServerBrowser::Reactivate()
{
	if (m_hInternetDlg.Get())
	{
		m_hInternetDlg->LoadUserData();
		if (m_hInternetDlg->IsVisible())
		{
			m_hInternetDlg->RefreshCurrentPage();
		}
	}
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CServerBrowser::Open()
{
	m_hInternetDlg->Open();
}


//-----------------------------------------------------------------------------
// Purpose: returns direct handle to main server browser dialog
//-----------------------------------------------------------------------------
vgui::VPANEL CServerBrowser::GetPanel()
{
	return m_hInternetDlg.Get() ? m_hInternetDlg->GetVPanel() : NULL;
}


//-----------------------------------------------------------------------------
// Purpose: sets the parent panel of the main module panel
//-----------------------------------------------------------------------------
void CServerBrowser::SetParent(vgui::VPANEL parent)
{
	if (m_hInternetDlg.Get())
	{
		m_hInternetDlg->SetParent(parent);
	}
}


//-----------------------------------------------------------------------------
// Purpose: Closes down the server browser for good
//-----------------------------------------------------------------------------
void CServerBrowser::Shutdown()
{
	if (m_hInternetDlg.Get())
	{
		m_hInternetDlg->Close();
		m_hInternetDlg->MarkForDeletion();
	}

#if defined( STEAM )
	vgui::VGuiControls_Shutdown();
#endif

	DisconnectTier3Libraries();
	DisconnectTier2Libraries();
	ConVar_Unregister();
	DisconnectTier1Libraries();
#ifndef NO_STEAM
	SteamAPI_Shutdown();
#endif
}


//-----------------------------------------------------------------------------
// Purpose: opens a game info dialog to watch the specified server; associated with the friend 'userName'
//-----------------------------------------------------------------------------
bool CServerBrowser::OpenGameInfoDialog( uint64 ulSteamIDFriend )
{
#if !defined( _X360 ) // X360TBD: SteamFriends()
	if ( m_hInternetDlg.Get() )
	{
		// activate an already-existing dialog
		CDialogGameInfo *pDialogGameInfo = m_hInternetDlg->GetDialogGameInfoForFriend( ulSteamIDFriend );
		if ( pDialogGameInfo )
		{
			pDialogGameInfo->Activate();
			return true;
		}

		// none yet, create a new dialog
		uint64 nGameID;
		uint32 unGameIP;
		uint16 usGamePort;
		uint16 usQueryPort;
#ifndef NO_STEAM
		if ( SteamFriends()->GetFriendGamePlayed( ulSteamIDFriend, &nGameID, &unGameIP, &usGamePort, &usQueryPort ) )
		{
			uint16 usConnPort = usGamePort;
			if ( usQueryPort < QUERY_PORT_ERROR )
				usConnPort = usGamePort;
			CDialogGameInfo *pDialogGameInfo = m_hInternetDlg->OpenGameInfoDialog( unGameIP, usGamePort, usConnPort );
			pDialogGameInfo->SetFriend( ulSteamIDFriend );
			return true;
		}
#endif
	}
#endif
	return false;
}


//-----------------------------------------------------------------------------
// Purpose: joins a specified game - game info dialog will only be opened if the server is fully or passworded
//-----------------------------------------------------------------------------
bool CServerBrowser::JoinGame( uint64 ulSteamIDFriend )
{
	if ( OpenGameInfoDialog( ulSteamIDFriend ) )
	{
		CDialogGameInfo *pDialogGameInfo = m_hInternetDlg->GetDialogGameInfoForFriend( ulSteamIDFriend );
		pDialogGameInfo->Connect();
	}

	return false;
}


//-----------------------------------------------------------------------------
// Purpose: joins a game by IP/Port
//-----------------------------------------------------------------------------
bool CServerBrowser::JoinGame( uint32 unGameIP, uint16 usGamePort )
{
    m_hInternetDlg->JoinGame( unGameIP, usGamePort );
	return true;
}


//-----------------------------------------------------------------------------
// Purpose: forces the game info dialog closed
//-----------------------------------------------------------------------------
void CServerBrowser::CloseGameInfoDialog( uint64 ulSteamIDFriend )
{
	CDialogGameInfo *pDialogGameInfo = m_hInternetDlg->GetDialogGameInfoForFriend( ulSteamIDFriend );
	if ( pDialogGameInfo )
	{
		pDialogGameInfo->Close();
	}
}


//-----------------------------------------------------------------------------
// Purpose: closes all the game info dialogs
//-----------------------------------------------------------------------------
void CServerBrowser::CloseAllGameInfoDialogs()
{
	if ( m_hInternetDlg.Get() )
	{
		m_hInternetDlg->CloseAllGameInfoDialogs();
	}
}
