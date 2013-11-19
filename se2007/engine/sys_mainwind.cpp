//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: 
//
//===========================================================================//

#ifdef _LINUX
#include "sys_linuxwind.cpp"
#else

#if !defined( _X360 )
#include "winlite.h"
#include "xbox/xboxstubs.h"
#endif

#ifdef IS_WINDOWS_PC
#include <winsock.h>
#endif

#include "igame.h"
#include "cl_main.h"
#include "host.h"
#include "quakedef.h"
#include "tier0/vcrmode.h"
#include "tier0/icommandline.h"
#include "ivideomode.h"
#include "gl_matsysiface.h"
#include "cdll_engine_int.h"
#include "vgui_baseui_interface.h"
#include "iengine.h"
#include "avi/iavi.h"
#include "keys.h"
#include "vguimatsurface/imatsystemsurface.h"
#include "tier3/tier3.h"
#include "sound.h"
#include "vgui_controls/Controls.h"
#include "vgui_controls/messagedialog.h"
#include "sys_dll.h"
#include "inputsystem/iinputsystem.h"
#include "inputsystem/ButtonCode.h"
#include "unicode/unicode.h"
#include "gameui/igameui.h"
#include "matchmaking.h"
#include "sv_main.h"
#include "bink/bink.h"

#if defined( _X360 )
#include "xbox/xbox_win32stubs.h"
#include "hl2orange.spa.h"
#endif

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

void S_BlockSound (void);
void S_UnblockSound (void);
void ClearIOStates( void );

//-----------------------------------------------------------------------------
// Game input events
//-----------------------------------------------------------------------------
enum GameInputEventType_t
{
	IE_Close = IE_FirstAppEvent,
	IE_WindowMove,
	IE_AppActivated,
};



static 	IUnicodeWindows *unicode = NULL;

//-----------------------------------------------------------------------------
// Purpose: Main game interface, including message pump and window creation
//-----------------------------------------------------------------------------
class CGame : public IGame
{
public:
					CGame( void );
	virtual			~CGame( void );

	bool			Init( void *pvInstance );
	bool			Shutdown( void );

	bool			CreateGameWindow( void );
	void			DestroyGameWindow();
	void			SetGameWindow( void* hWnd );

	// This is used in edit mode to override the default wnd proc associated w/
	bool			InputAttachToGameWindow();
	void			InputDetachFromGameWindow();

	void			PlayStartupVideos( void );

	void*			GetMainWindow( void );
	void**			GetMainWindowAddress( void );
	void			GetDesktopInfo( int &width, int &height, int &refreshrate );


	void			SetWindowXY( int x, int y );
	void			SetWindowSize( int w, int h );
	void			GetWindowRect( int *x, int *y, int *w, int *h );

	bool			IsActiveApp( void );

public:
	void			SetMainWindow( HWND window );
	void			SetActiveApp( bool active );
	int				WindowProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool			LoadUnicode();
	void			UnloadUnicode();

// Message handlers.
public:
	void	HandleMsg_WindowMove( const InputEvent_t &event );
	void	HandleMsg_ActivateApp( const InputEvent_t &event );
	void	HandleMsg_Close( const InputEvent_t &event );

	// Call the appropriate HandleMsg_ function.
	void	DispatchInputEvent( const InputEvent_t &event );

	// Dispatch all the queued up messages.
	virtual void	DispatchAllStoredGameMessages();
private:
	void			AppActivate( bool fActive );
	void			PlayVideoAndWait(const char *filename); // plays a video file and waits till it's done to return. Can be interrupted by user.

private:
	void AttachToWindow();
	void DetachFromWindow();

#ifndef _X360
	static const wchar_t CLASSNAME[];
#else
	static const char CLASSNAME[];
#endif

	bool			m_bExternallySuppliedWindow;

	HWND			m_hWindow;
	HINSTANCE		m_hInstance;

	// Stores a wndproc to chain message calls to
	WNDPROC			m_ChainedWindowProc;

	int				m_x;
	int				m_y;
	int				m_width;
	int				m_height;
	bool			m_bActiveApp;
	CSysModule		*m_hUnicodeModule;

	int				m_iDesktopWidth, m_iDesktopHeight, m_iDesktopRefreshRate;
	void			UpdateDesktopInformation( HWND hWnd );
	void			UpdateDesktopInformation( WPARAM wParam, LPARAM lParam );

	RECT			m_rcLastRestoredClientRect;
};

static CGame g_Game;
IGame *game = ( IGame * )&g_Game;

#if !defined( _X360 )
const wchar_t CGame::CLASSNAME[] = L"Valve001";
#else
const char CGame::CLASSNAME[] = "Valve001";
#endif

// In VCR playback mode, it sleeps this amount each frame.
int g_iVCRPlaybackSleepInterval = 0;

// During VCR playback, if this is true, then it'll pause at the end of each frame.
bool g_bVCRSingleStep = false;

bool g_bWaitingForStepKeyUp = false;	// Used to prevent it from running frames while you hold the S key down.

bool g_bShowVCRPlaybackDisplay = true;

// These are all the windows messages that can change game state.
// See CGame::WindowProc for a description of how they work.
struct GameMessageHandler_t
{
	int	m_nEventType;
	void (CGame::*pFn)( const InputEvent_t &event );
};

GameMessageHandler_t g_GameMessageHandlers[] = 
{
	{ IE_AppActivated,			&CGame::HandleMsg_ActivateApp },
	{ IE_WindowMove,			&CGame::HandleMsg_WindowMove },
	{ IE_Close,					&CGame::HandleMsg_Close },
	{ IE_Quit,					&CGame::HandleMsg_Close },
};


void CGame::AppActivate( bool fActive )
{
	// If text mode, force it to be active.
	if ( g_bTextMode )
	{
		fActive = true;
	}

	// Don't bother if we're already in the correct state
	if ( IsActiveApp() == fActive )
		return;

#ifndef SWDS
	if ( host_initialized )
	{
		if ( fActive )
		{
			if ( videomode )
			{
				videomode->RestoreVideo();
			}

			// Clear keyboard states (should be cleared already but...)
			// VGui_ActivateMouse will reactivate the mouse soon.
			ClearIOStates();
			
			UpdateMaterialSystemConfig();
		}
		else
		{
			// Clear keyboard input and deactivate the mouse while we're away.
			ClearIOStates();

			if ( g_ClientDLL )
			{
				g_ClientDLL->IN_DeactivateMouse();
			}

			if ( videomode )
			{
				videomode->ReleaseVideo();
			}
		}
	}
#endif // SWDS
	SetActiveApp( fActive );

#ifdef _XBOX
	if ( host_initialized )
	{
		ClearIOStates();
		if ( fActive )
		{
			UpdateMaterialSystemConfig();
		}
	}
	SetActiveApp( fActive );
#endif
}

void CGame::HandleMsg_WindowMove( const InputEvent_t &event )
{
	game->SetWindowXY( event.m_nData, event.m_nData2 );
#ifndef SWDS
	videomode->UpdateWindowPosition();
#endif
}

void CGame::HandleMsg_ActivateApp( const InputEvent_t &event )
{
	AppActivate( event.m_nData ? true : false );
}

void CGame::HandleMsg_Close( const InputEvent_t &event )
{
	if ( eng->GetState() == IEngine::DLL_ACTIVE )
	{
		eng->SetQuitting( IEngine::QUIT_TODESKTOP );
	}
}

void CGame::DispatchInputEvent( const InputEvent_t &event )
{
	switch( event.m_nType )
	{
	// Handle button events specially, 
	// since we have all manner of crazy filtering going on	when dealing with them
	case IE_ButtonPressed:
	case IE_ButtonDoubleClicked:
	case IE_ButtonReleased:
		Key_Event( event );
		break;

	default:
		// Let vgui have the first whack at events
		if ( g_pMatSystemSurface && g_pMatSystemSurface->HandleInputEvent( event ) )
			break;

		for ( int i=0; i < ARRAYSIZE( g_GameMessageHandlers ); i++ )
		{
			if ( g_GameMessageHandlers[i].m_nEventType == event.m_nType )
			{
				(this->*g_GameMessageHandlers[i].pFn)( event );
				break;
			}
		}
		break;
	}
}


void CGame::DispatchAllStoredGameMessages()
{
#if !defined( NO_VCR )
	if ( VCRGetMode() == VCR_Playback )
	{
		InputEvent_t event;
		while ( VCRHook_PlaybackGameMsg( &event ) )
		{
			event.m_nTick = g_pInputSystem->GetPollTick();
			DispatchInputEvent( event );
		}
	}
	else
	{
		int nEventCount = g_pInputSystem->GetEventCount();
		const InputEvent_t* pEvents = g_pInputSystem->GetEventData( );
		for ( int i = 0; i < nEventCount; ++i )
		{
			VCRHook_RecordGameMsg( pEvents[i] );
			DispatchInputEvent( pEvents[i] );
		}
		VCRHook_RecordEndGameMsg();
	}
#else
	int nEventCount = g_pInputSystem->GetEventCount();
	const InputEvent_t* pEvents = g_pInputSystem->GetEventData( );
	for ( int i = 0; i < nEventCount; ++i )
	{
		DispatchInputEvent( pEvents[i] );
	}
#endif
}

void VCR_EnterPausedState()
{
	// Turn this off in case they're in single-step mode.
	g_bVCRSingleStep = false;

	// This is cheesy, but GetAsyncKeyState is blocked (in protected_things.h) 
	// from being accidentally used, so we get it through it by getting its pointer directly.
	static HINSTANCE hInst = LoadLibrary( "user32.dll" );
	if ( !hInst )
		return;

	typedef SHORT (WINAPI *GetAsyncKeyStateFn)( int vKey );
	static GetAsyncKeyStateFn pfn = (GetAsyncKeyStateFn)GetProcAddress( hInst, "GetAsyncKeyState" );
	if ( !pfn )
		return;

	// In this mode, we enter a wait state where we only pay attention to R and Q.
	while ( 1 )
	{
		if ( pfn( 'R' ) & 0x8000 )
			break;

		if ( pfn( 'Q' ) & 0x8000 )
			TerminateProcess( GetCurrentProcess(), 1 );

		if ( pfn( 'S' ) & 0x8000 )
		{
			if ( !g_bWaitingForStepKeyUp )
			{
				// Do a single step.
				g_bVCRSingleStep = true;
				g_bWaitingForStepKeyUp = true;	// Don't do another single step until they release the S key.
				break;
			}
		}
		else
		{
			// Ok, they released the S key, so we'll process it next time the key goes down.
			g_bWaitingForStepKeyUp = false;
		}
	
		Sleep( 2 );
	}
}

void VCR_HandlePlaybackMessages( 
	HWND hWnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam 
	)
{
	if ( uMsg == WM_KEYDOWN )
	{
		if ( wParam == VK_SUBTRACT || wParam == 0xbd )
		{
			g_iVCRPlaybackSleepInterval += 5;
		}
		else if ( wParam == VK_ADD || wParam == 0xbb )
		{
			g_iVCRPlaybackSleepInterval -= 5;
		}
		else if ( toupper( wParam ) == 'Q' )
		{
			TerminateProcess( GetCurrentProcess(), 1 );
		}
		else if ( toupper( wParam ) == 'P' )
		{
			VCR_EnterPausedState();
		}
		else if ( toupper( wParam ) == 'S' && !g_bVCRSingleStep )
		{
			g_bWaitingForStepKeyUp = true;
			VCR_EnterPausedState();
		}
		else if ( toupper( wParam ) == 'D' )
		{
			g_bShowVCRPlaybackDisplay = !g_bShowVCRPlaybackDisplay;
		}

		g_iVCRPlaybackSleepInterval = clamp( g_iVCRPlaybackSleepInterval, 0, 500 );
	}
}

//-----------------------------------------------------------------------------
// Calls the default window procedure
// FIXME: It would be nice to remove the need for this, which we can do
// if we can make unicode work when running inside hammer.
//-----------------------------------------------------------------------------
static LONG WINAPI CallDefaultWindowProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if ( unicode )
		return unicode->DefWindowProcW( hWnd, uMsg, wParam, lParam );
	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

//-----------------------------------------------------------------------------
// Purpose: The user has accepted an invitation to a game, we need to detect if 
//			it's TF2 and restart properly if it is
//-----------------------------------------------------------------------------
void XBX_HandleInvite( DWORD nUserId )
{
#ifdef _X360
	// Grab our invite info
	XINVITE_INFO inviteInfo;
	DWORD dwError = XInviteGetAcceptedInfo( nUserId, &inviteInfo );
	if ( dwError != ERROR_SUCCESS )
		return;

	// We only care if we're asked to join an Orange Box session
	if ( inviteInfo.dwTitleID != TITLEID_THE_ORANGE_BOX )
	{
		// Do the normal "we've been invited to a game" behavior
		return;
	}

	// Otherwise, launch depending on our current MOD
	if ( !Q_stricmp( GetCurrentMod(), "tf" ) )
	{
		// We're already running TF2, so just join the session
		if ( nUserId != XBX_GetPrimaryUserId() )
		{
			// Switch users, the other had the invite
			XBX_SetPrimaryUserId( nUserId );
		}

		// Kick off our join
		g_pMatchmaking->JoinInviteSession( &(inviteInfo.hostInfo) );
	}
	else
	{
		// Save off our session ID for later retrieval
		// NOTE: We may need to actually save off the inviter's XID and search for them later on if we took too long or the
		//		 session they were a part of went away
		
		XBX_SetInviteSessionId( inviteInfo.hostInfo.sessionID );
		XBX_SetInvitedUserId( nUserId );

		// Quit via the menu path "QuitNoConfirm"
		EngineVGui()->SystemNotification( SYSTEMNOTIFY_INVITE_SHUTDOWN );

	}
#endif //_X360
}

//-----------------------------------------------------------------------------
// Main windows procedure
//-----------------------------------------------------------------------------
int CGame::WindowProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)

{
	LONG			lRet = 0;
	HDC				hdc;
	PAINTSTRUCT		ps;

	//
	// NOTE: the way this function works is to handle all messages that just call through to
	// Windows or provide data to it.
	//
	// Any messages that change the engine's internal state (like key events) are stored in a list
	// and processed at the end of the frame. This is necessary for VCR mode to work correctly because
	// Windows likes to pump messages during some of its API calls like SetWindowPos, and unless we add
	// custom code around every Windows API call so VCR mode can trap the wndproc calls, VCR mode can't 
	// reproduce the calls to the wndproc.
	//

	if ( eng->GetQuitting() != IEngine::QUIT_NOTQUITTING )
		return CallWindowProc( m_ChainedWindowProc, hWnd, uMsg, wParam, lParam );

	// If we're playing back, listen to a couple input things used to drive VCR mode
	if ( VCRGetMode() == VCR_Playback )
	{
		VCR_HandlePlaybackMessages( hWnd, uMsg, wParam, lParam );
	}

	//
	// Note: NO engine state should be changed in here while in VCR record or playback. 
	// We can send whatever we want to Windows, but if we change its state in here instead of 
	// in DispatchAllStoredGameMessages, the playback may not work because Windows messages 
	// are not deterministic, so you might get different messages during playback than you did during record.
	//
	InputEvent_t event;
	memset( &event, 0, sizeof(event) );
	event.m_nTick = g_pInputSystem->GetPollTick();

	switch ( uMsg )
	{
	case WM_CREATE:
		::SetForegroundWindow( hWnd );
		break;

	case WM_ACTIVATEAPP:
		{
			bool bActivated = ( wParam == 1 );
			event.m_nType = IE_AppActivated;
			event.m_nData = bActivated;
			g_pInputSystem->PostUserEvent( event );
		}
		break;

	case WM_POWERBROADCAST:
		// Don't go into Sleep mode when running engine, we crash on resume for some reason (as
		//  do half of the apps I have running usually anyway...)
		if ( wParam == PBT_APMQUERYSUSPEND )
		{
			Msg( "OS requested hibernation, ignoring request.\n" );
			return BROADCAST_QUERY_DENY;
		}

		lRet = CallWindowProc( m_ChainedWindowProc, hWnd, uMsg, wParam, lParam );
		break;

	case WM_SYSCOMMAND:
		if ( ( wParam == SC_MONITORPOWER ) || ( wParam == SC_KEYMENU ) || ( wParam == SC_SCREENSAVE ) )
            return lRet;
    
		if ( wParam == SC_CLOSE ) 
		{
#if !defined( NO_VCR )
			// handle the close message, but make sure 
			// it's not because we accidently hit ALT-F4
			if ( HIBYTE(VCRHook_GetKeyState(VK_LMENU)) || HIBYTE(VCRHook_GetKeyState(VK_RMENU) ) )
				return lRet;
#endif
			Cbuf_Clear();
			Cbuf_AddText( "quit\n" );
		}

#ifndef SWDS
		if ( VCRGetMode() == VCR_Disabled )
		{
			S_BlockSound();
			S_ClearBuffer();
		}
#endif

		lRet = CallWindowProc( m_ChainedWindowProc, hWnd, uMsg, wParam, lParam );

#ifndef SWDS
		if ( VCRGetMode() == VCR_Disabled )
		{
			S_UnblockSound();
		}
#endif
		break;

	case WM_CLOSE:
		// Handle close messages
		event.m_nType = IE_Close;
		g_pInputSystem->PostUserEvent( event );
		return 0;

	case WM_MOVE:
		event.m_nType = IE_WindowMove;
		event.m_nData = (short)LOWORD(lParam);
		event.m_nData2 = (short)HIWORD(lParam);
		g_pInputSystem->PostUserEvent( event );
		break;

	case WM_SIZE:
		if ( wParam != SIZE_MINIMIZED )
		{
			// Update restored client rect
			::GetClientRect( hWnd, &m_rcLastRestoredClientRect );
		}
		else
		{
#ifndef _X360
			// Fix the window rect to have same client area as it used to have
			// before it got minimized
			RECT rcWindow;
			::GetWindowRect( hWnd, &rcWindow );

			rcWindow.right = rcWindow.left + m_rcLastRestoredClientRect.right;
			rcWindow.bottom = rcWindow.top + m_rcLastRestoredClientRect.bottom;

			::AdjustWindowRect( &rcWindow, ::GetWindowLong( hWnd, GWL_STYLE ), FALSE );
			::MoveWindow( hWnd, rcWindow.left, rcWindow.top,
				rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top, FALSE );
#endif
		}
		break;

	case WM_SYSCHAR:
		// keep Alt-Space from happening
		break;

	case WM_COPYDATA:
		// Hammer -> engine remote console command.
		// Return true to indicate that the message was handled.
		Cbuf_AddText( (const char *)(((COPYDATASTRUCT *)lParam)->lpData) );
		Cbuf_AddText( "\n" );
		lRet = 1;
		break;

#if defined( _X360 )
	case WM_XREMOTECOMMAND:
		Cbuf_AddText( (const char*)lParam );
		Cbuf_AddText( "\n" );
		break;

	case WM_SYS_STORAGEDEVICESCHANGED:
		if ( !EngineVGui()->IsGameUIVisible() )
		{
			EngineVGui()->ActivateGameUI();
		}
		EngineVGui()->SystemNotification( SYSTEMNOTIFY_STORAGEDEVICES_CHANGED );	
		break;

	case WM_XMP_PLAYBACKCONTROLLERCHANGED:
		S_EnableMusic( lParam != 0 );
		break;

	case WM_LIVE_INVITE_ACCEPTED:
		XBX_HandleInvite( LOWORD( lParam ) );
		break;

	case WM_SYS_SIGNINCHANGED:
		if ( XUserGetSigninState( XBX_GetPrimaryUserId() ) == eXUserSigninState_NotSignedIn )
		{
			// X360TBD: User signed out - pause the game?
		}
		EngineVGui()->SystemNotification( lParam ? SYSTEMNOTIFY_USER_SIGNEDIN : SYSTEMNOTIFY_USER_SIGNEDOUT );	
		break;

	case WM_SYS_UI:
		if ( lParam )
		{
			// Don't activate it if it's already active (a sub window may be active)
			// Multiplayer doesn't want the UI to appear, since it can't pause anyway
			if ( !EngineVGui()->IsGameUIVisible() && g_ServerGlobalVariables.maxClients == 1 )
			{
				Cbuf_AddText( "gameui_activate" );
			}
		}
		EngineVGui()->SystemNotification( lParam ? SYSTEMNOTIFY_XUIOPENING : SYSTEMNOTIFY_XUICLOSED );
		break;

	case WM_SYS_MUTELISTCHANGED:
		g_pMatchmaking->UpdateMuteList();
		break;

	case WM_SYS_INPUTDEVICESCHANGED:
		{
			XINPUT_CAPABILITIES caps;
			if ( XInputGetCapabilities( XBX_GetPrimaryUserId(), XINPUT_FLAG_GAMEPAD, &caps ) == ERROR_DEVICE_NOT_CONNECTED )
			{
				if ( EngineVGui()->IsGameUIVisible() == false )
				{
					EngineVGui()->ActivateGameUI();
				}
			}
		}
		break;

#endif

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		RECT rcClient;
		GetClientRect( hWnd, &rcClient );
#ifndef SWDS		
		// Only renders stuff if running -noshaderapi
		if ( videomode )
		{
			videomode->DrawNullBackground( hdc, rcClient.right, rcClient.bottom );
		}
#endif
		EndPaint(hWnd, &ps);
		break;

#ifndef _X360
	case WM_DISPLAYCHANGE:
		if ( !m_iDesktopHeight || !m_iDesktopWidth )
		{
			UpdateDesktopInformation( wParam, lParam );
		}
		break;
#endif

	case WM_IME_NOTIFY:
		switch ( wParam )
		{
		default:
			break;

#ifndef SWDS
		case 14:
            if ( !videomode->IsWindowedMode() )
				return 0;
			break;
#endif
		}
		return CallWindowProc( m_ChainedWindowProc, hWnd, uMsg, wParam, lParam);

	default:
		lRet = CallWindowProc( m_ChainedWindowProc, hWnd, uMsg, wParam, lParam );
	    break;
    }

    // return 0 if handled message, 1 if not
    return lRet;
}


//-----------------------------------------------------------------------------
// Creates the game window 
//-----------------------------------------------------------------------------
static LONG WINAPI HLEngineWindowProc( HWND hWnd, UINT uMsg, WPARAM  wParam, LPARAM  lParam )
{
	return g_Game.WindowProc( hWnd, uMsg, wParam, lParam );
}

#define DEFAULT_EXE_ICON 101

static void DoSomeSocketStuffInOrderToGetZoneAlarmToNoticeUs( void )
{
#ifdef IS_WINDOWS_PC
	WSAData wsaData;
	if ( ! WSAStartup( 0x0101, &wsaData ) )
	{
		SOCKET tmpSocket = socket( AF_INET, SOCK_DGRAM, 0 );
		if ( tmpSocket != INVALID_SOCKET )
		{
			char Options[]={ 1 };
			setsockopt( tmpSocket, SOL_SOCKET, SO_BROADCAST, Options, sizeof(Options));
			char pszHostName[256];
			gethostname( pszHostName, sizeof( pszHostName ) );
			hostent *hInfo = gethostbyname( pszHostName );
			if ( hInfo )
			{
				sockaddr_in myIpAddress;
				memset( &myIpAddress, 0, sizeof( myIpAddress ) );
				myIpAddress.sin_family = AF_INET;
				myIpAddress.sin_port = htons( 27015 );			// our normal server port
				myIpAddress.sin_addr.S_un.S_un_b.s_b1 = hInfo->h_addr_list[0][0];
				myIpAddress.sin_addr.S_un.S_un_b.s_b2 = hInfo->h_addr_list[0][1];
				myIpAddress.sin_addr.S_un.S_un_b.s_b3 = hInfo->h_addr_list[0][2];
				myIpAddress.sin_addr.S_un.S_un_b.s_b4 = hInfo->h_addr_list[0][3];
				if ( bind( tmpSocket, ( sockaddr * ) &myIpAddress, sizeof( myIpAddress ) ) != -1 )
				{
					if ( sendto( tmpSocket, pszHostName, 1, 0, ( sockaddr *) &myIpAddress, sizeof( myIpAddress ) ) == -1 )
					{
						// error?
					}

				}
			}
			closesocket( tmpSocket );
		}
		WSACleanup();
	}
	
#endif
}

bool CGame::CreateGameWindow( void )
{
#ifndef SWDS
	if ( IsPC() )
	{
		if ( !LoadUnicode() )
		{
			return false;
		}
	}

#if !defined( _X360 )
	WNDCLASSW wc;
#else
	WNDCLASS wc;
#endif
	memset( &wc, 0, sizeof( wc ) );

    wc.style         = CS_OWNDC | CS_DBLCLKS;
    wc.lpfnWndProc   = CallDefaultWindowProc;
    wc.hInstance     = m_hInstance;
    wc.lpszClassName = CLASSNAME;

	// find the icon file in the filesystem
	if ( IsPC() )
	{
		char localPath[ MAX_PATH ];
		if ( g_pFileSystem->GetLocalPath( "resource/game.ico", localPath, sizeof(localPath) ) )
		{
			g_pFileSystem->GetLocalCopy( localPath );
			wc.hIcon = (HICON)::LoadImage(NULL, localPath, IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE);
		}
		else
		{
			wc.hIcon = (HICON)LoadIcon( GetModuleHandle( 0 ), MAKEINTRESOURCE( DEFAULT_EXE_ICON ) );
		}
	}

	// get the window name
	char windowName[256];
	windowName[0] = 0;
	KeyValues *modinfo = new KeyValues("ModInfo");
	if (modinfo->LoadFromFile(g_pFileSystem, "gameinfo.txt"))
	{
		Q_strncpy( windowName, modinfo->GetString("game"), sizeof(windowName) );
	}
	

#ifndef SWDS
	char const *pszGameType = modinfo->GetString( "type" );
	if ( pszGameType && Q_stristr( pszGameType, "multiplayer" ) )
		DoSomeSocketStuffInOrderToGetZoneAlarmToNoticeUs();
#endif

	modinfo->deleteThis();
	if (!windowName[0])
	{
		Q_strncpy( windowName, "HALF-LIFE 2", sizeof(windowName) );
	}

	wchar_t uc[512];
	if ( IsPC() )
	{
		::MultiByteToWideChar(CP_UTF8, 0, windowName, -1, uc, sizeof( uc ) / sizeof(wchar_t));
	}

	// Oops, we didn't clean up the class registration from last cycle which
	// might mean that the wndproc pointer is bogus
#ifndef _X360
	unicode->UnregisterClassW( CLASSNAME, m_hInstance );
	// Register it again
    unicode->RegisterClassW( &wc );
#else
	RegisterClass( &wc );
#endif

	// Note, it's hidden
	DWORD style = WS_POPUP | WS_CLIPSIBLINGS;
	
	// Give it a frame
	if ( videomode->IsWindowedMode() )
	{
		style |= WS_OVERLAPPEDWINDOW;
		style &= ~WS_THICKFRAME;
	}

	// Never a max box
	style &= ~WS_MAXIMIZEBOX;

	int w, h;

	// Create a full screen size window by default, it'll get resized later anyway
	w = GetSystemMetrics( SM_CXSCREEN );
	h = GetSystemMetrics( SM_CYSCREEN );

	// Create the window
	DWORD exFlags = 0;
	if ( g_bTextMode )
	{
		style &= ~WS_VISIBLE;
		exFlags |= WS_EX_TOOLWINDOW; // So it doesn't show up in the taskbar.
	}

#if !defined( _X360 )
	HWND hwnd = unicode->CreateWindowExW( exFlags, CLASSNAME, uc, style, 
		0, 0, w, h, NULL, NULL, m_hInstance, NULL );
	// NOTE: On some cards, CreateWindowExW slams the FPU control word
	SetupFPUControlWord();
#else
	HWND hwnd = CreateWindowEx( exFlags, CLASSNAME, windowName, style, 
			0, 0, w, h, NULL, NULL, m_hInstance, NULL );
#endif

	if ( !hwnd )
	{
		Error( "Fatal Error:  Unable to create game window!" );
		return false;
	}

	SetMainWindow( hwnd );

	AttachToWindow( );
	return true;
#else
	return true;
#endif
}


//-----------------------------------------------------------------------------
// Destroys the game window 
//-----------------------------------------------------------------------------
void CGame::DestroyGameWindow()
{
#ifndef SWDS
	// Destroy all things created when the window was created
	if ( !m_bExternallySuppliedWindow )
	{
		DetachFromWindow( );

		if ( m_hWindow )
		{
			DestroyWindow( m_hWindow );
			m_hWindow = (HWND)0;
		}

#if !defined( _X360 )
		unicode->UnregisterClassW( CLASSNAME, m_hInstance );
		UnloadUnicode();
#else
		UnregisterClass( CLASSNAME, m_hInstance );
#endif
	}
	else
	{
		m_hWindow = (HWND)0;
		m_bExternallySuppliedWindow = false;
	}
#endif // !defined( SWDS )
}


//-----------------------------------------------------------------------------
// This is used in edit mode to specify a particular game window (created by hammer)
//-----------------------------------------------------------------------------
void CGame::SetGameWindow( void *hWnd )
{
	m_bExternallySuppliedWindow = true;
	SetMainWindow( (HWND)hWnd );
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CGame::AttachToWindow()
{
	if ( !m_hWindow )
		return;

	m_ChainedWindowProc = (WNDPROC)GetWindowLongPtr( m_hWindow, GWLP_WNDPROC );
	SetWindowLongPtr( m_hWindow, GWLP_WNDPROC, (LONG_PTR)HLEngineWindowProc );

	if ( g_pInputSystem )
	{
		// Attach the input system window proc
		g_pInputSystem->AttachToWindow( m_hWindow );
		g_pInputSystem->EnableInput( true );
		g_pInputSystem->EnableMessagePump( false );
	}

	if ( g_pMatSystemSurface )
	{
		// Attach the vgui matsurface window proc
		g_pMatSystemSurface->AttachToWindow( m_hWindow, true );
		g_pMatSystemSurface->EnableWindowsMessages( true );
	}
}

void CGame::DetachFromWindow()
{
	if ( !m_hWindow || !m_ChainedWindowProc )
	{
		m_ChainedWindowProc = NULL;
		return;
	}

	if ( g_pMatSystemSurface )
	{
		// Detach the vgui matsurface window proc
		g_pMatSystemSurface->AttachToWindow( NULL );
	}

	if ( g_pInputSystem )
	{
		// Detach the input system window proc
		g_pInputSystem->EnableInput( false );
		g_pInputSystem->DetachFromWindow( );
	}

	Assert( (WNDPROC)GetWindowLongPtr( m_hWindow, GWLP_WNDPROC ) == HLEngineWindowProc );
	SetWindowLongPtr( m_hWindow, GWLP_WNDPROC, (LONG_PTR)m_ChainedWindowProc );
}


//-----------------------------------------------------------------------------
// This is used in edit mode to override the default wnd proc associated w/
// the game window specified in SetGameWindow. 
//-----------------------------------------------------------------------------
bool CGame::InputAttachToGameWindow()
{
	// We can't use this feature unless we didn't control the creation of the window
	if ( !m_bExternallySuppliedWindow )
		return true;

	AttachToWindow();

#ifndef SWDS
	vgui::surface()->OnScreenSizeChanged( videomode->GetModeWidth(), videomode->GetModeHeight() );
#endif

	// We don't get WM_ACTIVATEAPP messages in this case; simulate one.
	AppActivate( true );

	// Capture + hide the mouse
	SetCapture( m_hWindow );

	return true;
}

void CGame::InputDetachFromGameWindow()
{
	// We can't use this feature unless we didn't control the creation of the window
	if ( !m_bExternallySuppliedWindow )
		return;

	if ( !m_ChainedWindowProc )
		return;

	// Release + show the mouse
	ReleaseCapture();

	// We don't get WM_ACTIVATEAPP messages in this case; simulate one.
	AppActivate( false );

	DetachFromWindow();
}

void CGame::PlayStartupVideos( void )
{
	if ( IsX360() )
		return;

#ifndef SWDS
	
	// Wait for the mode to change and stabilized
	// FIXME: There's really no way to know when this is completed, so we have to guess a time that will mostly be correct
	if ( videomode->IsWindowedMode() == false )
	{
		Sleep( 1000 );
	}

	bool bEndGame = CommandLine()->CheckParm("-endgamevid");
	bool bRecap = CommandLine()->CheckParm("-recapvid");	// FIXME: This is a temp addition until the movie playback is centralized -- jdw

	if (!bEndGame && !bRecap && (CommandLine()->CheckParm("-dev") || CommandLine()->CheckParm("-novid") || CommandLine()->CheckParm("-allowdebug")))
		return;

	char *pszFile = "media/StartupVids.txt";
	if ( bEndGame )
	{
		// Don't go back into the map that triggered this.
		CommandLine()->RemoveParm( "+map" );
		CommandLine()->RemoveParm( "+load" );
		
		pszFile = "media/EndGameVids.txt";
	}
	else if ( bRecap )
	{
		pszFile = "media/RecapVids.txt";
	}
	
	int vidFileLength;

	// have to use the malloc memory allocation option in COM_LoadFile since the memory system isn't set up at this point.
	const char *buffer = (char *)COM_LoadFile(pszFile, 5, &vidFileLength);
	
	if ((buffer == NULL) || (vidFileLength == 0))
	{
		return;
	}

	// hide cursor while playing videos
	::ShowCursor(FALSE);

	const char *start = buffer;

	while (1)
	{
		start = COM_Parse(start);
		if (Q_strlen(com_token) <= 0)
		{
			break;
		}

		// get the path to the avi file and play it.
		char localPath[MAX_PATH];
		g_pFileSystem->GetLocalPath( com_token, localPath, sizeof(localPath) );
		PlayVideoAndWait( localPath );
		localPath[0] = 0; // just to make sure we don't play the same avi file twice in the case that one movie is there but another isn't.
	}

	// show cursor again
	::ShowCursor(TRUE);

	// call free on the buffer since the buffer was malloc'd in COM_LoadFile
	free((void *)buffer);

#endif // SWDS
}

//-----------------------------------------------------------------------------
// Purpose: Plays a Bink video until the video completes or ESC is pressed
// Input  : *filename - Name of the file (relative to the filesystem)
//-----------------------------------------------------------------------------
void CGame::PlayVideoAndWait( const char *filename )
{

#if !defined(_X360) && defined(_WIN32)

	if ( !filename || !filename[0] )
		return;

	// Black out the back of the screen once at the beginning of each video (since we're not scaling to fit)
	HDC dc = ::GetDC( m_hWindow );

	RECT rect;
	rect.top = 0;
	rect.bottom = m_height;
	rect.left = 0;
	rect.right = m_width;

	HBRUSH hBlackBrush = (HBRUSH) ::GetStockObject( BLACK_BRUSH );
	::SetViewportOrgEx( dc, 0, 0, NULL );
	::FillRect( dc, &rect, hBlackBrush );
	::ReleaseDC( (HWND) GetMainWindow(), dc );
	
	// Supplying a NULL context will cause Bink to allocate its own
	BinkSoundUseDirectSound( NULL );

	// Open the bink file with audio
	HBINK hBINK = BinkOpen( filename, BINKSNDTRACK );
	if ( hBINK == 0 )
		return;
	
	// Create a buffer to decompress to
	// NOTE: The DIB version is the only one we can call on without DirectDraw
	HBINKBUFFER hBINKBuffer = BinkBufferOpen( GetMainWindow(), hBINK->Width, hBINK->Height, BINKBUFFERDIBSECTION | BINKBUFFERSTRETCHXINT | BINKBUFFERSTRETCHYINT | BINKBUFFERSHRINKXINT | BINKBUFFERSHRINKYINT );
	if ( hBINKBuffer == 0 )
	{
		BinkClose( hBINK );
		return;
	}

	// Integral scaling is much faster, so always scale the video as such
	int	nNewWidth = (int) hBINK->Width;
	int nNewHeight = (int) hBINK->Height;

	// Find if we need to scale up or down
	if ( (int) hBINK->Width < m_width && (int) hBINK->Height < m_height )
	{
		// Scaling up by powers of two
		int nScale = 0;
		while ( ( (int) hBINK->Width << (nScale+1) ) <= m_width && ( (int) hBINK->Height << (nScale+1) ) <= m_height )
			nScale++;
		
		nNewWidth = (int) hBINK->Width << nScale;
		nNewHeight = (int) hBINK->Height << nScale;
	}
	else if ( (int) hBINK->Width > m_width && (int) hBINK->Height > m_height )
	{
		// Scaling down by powers of two
		int nScale = 1;
		while ( ( (int) hBINK->Width >> nScale ) > m_width && ( (int) hBINK->Height >> nScale ) > m_height )
			nScale++;

		nNewWidth = (int) hBINK->Width >> nScale;
		nNewHeight = (int) hBINK->Height >> nScale;
	}

	// Scale if we need to
	BinkBufferSetScale( hBINKBuffer, nNewWidth, nNewHeight );
	int nXPos = ( m_width - nNewWidth ) / 2;
	int nYPos = ( m_height - nNewHeight ) / 2;

	// Offset to the middle of the screen
	BinkBufferSetOffset( hBINKBuffer, nXPos, nYPos );

	// We need to be able to poll the state of the input device, but we're not completely setup yet, so this spoofs the ability
	HINSTANCE hInst = LoadLibrary( "user32.dll" );
	if ( hInst == NULL )
		return;
	
	typedef SHORT (WINAPI *GetAsyncKeyStateFn)( int vKey );
	GetAsyncKeyStateFn pfnGetAsyncKeyState = (GetAsyncKeyStateFn)GetProcAddress( hInst, "GetAsyncKeyState" );
	if ( pfnGetAsyncKeyState )
	{	
		while ( 1 )
		{
			// Escape, return, or space stops the playback
			short nKeyState = (pfnGetAsyncKeyState(27)|pfnGetAsyncKeyState(32)|pfnGetAsyncKeyState(13));
			if ( nKeyState & 0x8000 )
				break;

			// Decompress this frame
			BinkDoFrame( hBINK );
			
			// Lock the buffer for writing
			if ( BinkBufferLock( hBINKBuffer ) )
			{		
				// Copy the decompressed frame into the BinkBuffer
				BinkCopyToBuffer( hBINK,
					hBINKBuffer->Buffer,
					hBINKBuffer->BufferPitch,
					hBINKBuffer->Height,
					0, 0,
					hBINKBuffer->SurfaceType );
				
				// Unlock the buffer
				BinkBufferUnlock( hBINKBuffer );
			}

			// Blit the pixels to the screen
			BinkBufferBlit( hBINKBuffer, hBINK->FrameRects, BinkGetRects( hBINK, hBINKBuffer->SurfaceType ) );
			
			// Wait until the next frame is ready
			while ( BinkWait( hBINK ) )
			{
				// FIXME: Bink recommends a higher precision than this
				Sleep( 1 );
			}
			
			// Check for video being complete
			if ( hBINK->FrameNum == hBINK->Frames )
				break;
			
			// Move on
			BinkNextFrame( hBINK );
		}
	}
	
	// Close it all down
	BinkBufferClose( hBINKBuffer );
	BinkClose( hBINK );

	// Free this as well
	FreeLibrary( hInst );

#endif // _X360
}


//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CGame::CGame()
{
	unicode = NULL;
	m_hUnicodeModule = NULL;
	m_hWindow = 0;
	m_x = m_y = 0;
	m_width = m_height = 0;
	m_bActiveApp = false;
	m_hInstance = 0;
	m_iDesktopWidth = 0;
	m_iDesktopHeight = 0;
	m_iDesktopRefreshRate = 0;
	m_ChainedWindowProc = NULL;
}

//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CGame::~CGame()
{
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CGame::Init( void *pvInstance )
{
	m_bExternallySuppliedWindow = false;

	OSVERSIONINFO	vinfo;
	vinfo.dwOSVersionInfoSize = sizeof(vinfo);

	if ( !GetVersionEx( &vinfo ) )
	{
		return false;
	}

	if ( vinfo.dwPlatformId == VER_PLATFORM_WIN32s )
	{
		return false;
	}

	m_hInstance = (HINSTANCE)pvInstance;
	return true;
}


bool CGame::Shutdown( void )
{
	m_hInstance = 0;
	return true;
}

bool CGame::LoadUnicode( void )
{
	m_hUnicodeModule = Sys_LoadModule( "unicode" );
	if ( !m_hUnicodeModule )
	{
		Error( "Unable to load unicode.dll" );
		return false;
	}

	CreateInterfaceFn factory = Sys_GetFactory( m_hUnicodeModule );
	if ( !factory )
	{
		Error( "Unable to get factory from unicode.dll" );
		return false;
	}

	unicode = ( IUnicodeWindows * )factory( VENGINE_UNICODEINTERFACE_VERSION, NULL );
	if ( !unicode )
	{
		Error( "Unable to load interface '%s' from unicode.dll", VENGINE_UNICODEINTERFACE_VERSION );
		return false;
	}

	return true;
}

void CGame::UnloadUnicode()
{
	unicode = NULL;

	if ( m_hUnicodeModule )
	{
		Sys_UnloadModule( m_hUnicodeModule );
		m_hUnicodeModule = NULL;
	}
}

void *CGame::GetMainWindow( void )
{
	return (void*)m_hWindow;
}

void** CGame::GetMainWindowAddress( void )
{
	return (void**)&m_hWindow;
}

void CGame::GetDesktopInfo( int &width, int &height, int &refreshrate )
{
	// order of initialization means that this might get called early.  In that case go ahead and grab the current
	// screen window and setup based on that.
	// we need to do this when initializing the base list of video modes, for example
	if ( m_iDesktopWidth == 0 )
	{
		HDC dc = ::GetDC( NULL );
		width = ::GetDeviceCaps(dc, HORZRES);
		height = ::GetDeviceCaps(dc, VERTRES);
		refreshrate = ::GetDeviceCaps(dc, VREFRESH);
		::ReleaseDC( NULL, dc );
		return;
	}
	width = m_iDesktopWidth;
	height = m_iDesktopHeight;
	refreshrate = m_iDesktopRefreshRate;
}

void CGame::UpdateDesktopInformation( HWND hWnd )
{
	HDC dc = ::GetDC( hWnd );
	m_iDesktopWidth = ::GetDeviceCaps(dc, HORZRES);
	m_iDesktopHeight = ::GetDeviceCaps(dc, VERTRES);
	m_iDesktopRefreshRate = ::GetDeviceCaps(dc, VREFRESH);
	::ReleaseDC( hWnd, dc );
}

void CGame::UpdateDesktopInformation( WPARAM wParam, LPARAM lParam )
{
	m_iDesktopWidth = LOWORD( lParam );
	m_iDesktopHeight = HIWORD( lParam );
}

void CGame::SetMainWindow( HWND window )
{
	m_hWindow = window;

	if ( IsPC() )
	{
		avi->SetMainWindow( (void*)window );
	}

	// update our desktop info (since the results will change if we are going to fullscreen mode)
	if ( !m_iDesktopWidth || !m_iDesktopHeight )
	{
		UpdateDesktopInformation( m_hWindow );
	}
}

void CGame::SetWindowXY( int x, int y )
{
	m_x = x;
	m_y = y;
}

void CGame::SetWindowSize( int w, int h )
{
	m_width = w;
	m_height = h;
}

void CGame::GetWindowRect( int *x, int *y, int *w, int *h )
{
	if ( x )
	{
		*x = m_x;
	}
	if ( y )
	{
		*y = m_y;
	}
	if ( w )
	{
		*w = m_width;
	}
	if ( h )
	{
		*h = m_height;
	}
}

bool CGame::IsActiveApp( void )
{
	return m_bActiveApp;
}

void CGame::SetActiveApp( bool active )
{
	m_bActiveApp = active;
}

#endif // _LINUX
