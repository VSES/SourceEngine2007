//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: 
//
//===========================================================================//

#include "inputsystem.h"
#include "key_translation.h"
#include "inputsystem/buttoncode.h"
#include "inputsystem/analogcode.h"
#include "tier1/convar.h"

ConVar joy_xcontroller_found( "joy_xcontroller_found", "1", FCVAR_NONE, "Automatically set to 1 if an xcontroller has been detected." );

//-----------------------------------------------------------------------------
// Singleton instance
//-----------------------------------------------------------------------------
static CInputSystem g_InputSystem;
EXPOSE_SINGLE_INTERFACE_GLOBALVAR( CInputSystem, IInputSystem, 
						INPUTSYSTEM_INTERFACE_VERSION, g_InputSystem );


//-----------------------------------------------------------------------------
// Constructor, destructor
//-----------------------------------------------------------------------------
CInputSystem::CInputSystem()
{
	m_nLastPollTick = m_nLastSampleTick = m_StartupTimeTick = 0;
	m_ChainedWndProc = 0;
	m_hAttachedHWnd = 0;
	m_hEvent = NULL;
	m_bEnabled = true;
	m_bPumpEnabled = true;
	m_bIsPolling = false;
	m_JoysticksEnabled.ClearAllFlags();
	m_nJoystickCount = 0;
	m_nPollCount = 0;
	m_PrimaryUserId = INVALID_USER_ID;
	m_uiMouseWheel = 0;
	m_bXController = false;
	Assert( (MAX_JOYSTICKS + 7) >> 3 << sizeof(unsigned short) ); 

	m_pXInputDLL = NULL;
}

CInputSystem::~CInputSystem()
{
	if ( m_pXInputDLL )
	{
		Sys_UnloadModule( m_pXInputDLL );
		m_pXInputDLL = NULL;
	}
}


//-----------------------------------------------------------------------------
// Initialization
//-----------------------------------------------------------------------------
InitReturnVal_t CInputSystem::Init()
{
	InitReturnVal_t nRetVal = BaseClass::Init();
	if ( nRetVal != INIT_OK )
		return nRetVal;

	m_StartupTimeTick = GetTickCount();

	if ( IsPC() )
	{
		m_uiMouseWheel = RegisterWindowMessage( "MSWHEEL_ROLLMSG" );
	}

	m_hEvent = CreateEvent( NULL, FALSE, FALSE, NULL );
	if ( !m_hEvent )
		return INIT_FAILED;

	ButtonCode_InitKeyTranslationTable();
	ButtonCode_UpdateScanCodeLayout();

	joy_xcontroller_found.SetValue( 0 );
	if ( IsPC() )
	{
		m_pXInputDLL = Sys_LoadModule( "XInput1_3.dll" );
		if ( m_pXInputDLL )
		{
			InitializeXDevices();
		}

		if ( !m_nJoystickCount )
		{
			// Didn't find any XControllers. See if we can find other joysticks.
			InitializeJoysticks();
		}
		else
		{
			m_bXController = true;
			joy_xcontroller_found.SetValue( 1 );
		}
	}
	if ( IsX360() )
	{
		SetPrimaryUserId( XBX_GetPrimaryUserId() );
		InitializeXDevices();
		m_bXController = true;
	}
	return INIT_OK; 
}


//-----------------------------------------------------------------------------
// Shutdown
//-----------------------------------------------------------------------------
void CInputSystem::Shutdown()
{
	if ( m_hEvent != NULL )
	{
		CloseHandle( m_hEvent );
		m_hEvent = NULL;
	}

	BaseClass::Shutdown();
}


//-----------------------------------------------------------------------------
// Sleep until input
//-----------------------------------------------------------------------------
void CInputSystem::SleepUntilInput( int nMaxSleepTimeMS )
{
	if ( nMaxSleepTimeMS < 0 )
	{
		nMaxSleepTimeMS = INFINITE;
	}
	MsgWaitForMultipleObjects( 1, &m_hEvent, FALSE, nMaxSleepTimeMS, QS_ALLEVENTS );
}



//-----------------------------------------------------------------------------
// Callback to call into our class
//-----------------------------------------------------------------------------
static LRESULT CALLBACK InputSystemWindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return g_InputSystem.WindowProc( hwnd, uMsg, wParam, lParam );
}


//-----------------------------------------------------------------------------
// Hooks input listening up to a window
//-----------------------------------------------------------------------------
void CInputSystem::AttachToWindow( void* hWnd )
{
	Assert( m_hAttachedHWnd == 0 );
	if ( m_hAttachedHWnd )
	{
		Warning( "CInputSystem::AttachToWindow: Cannot attach to two windows at once!\n" );
		return;
	}

	m_ChainedWndProc = (WNDPROC)GetWindowLongPtr( (HWND)hWnd, GWLP_WNDPROC );
	SetWindowLongPtr( (HWND)hWnd, GWLP_WNDPROC, (LONG_PTR)InputSystemWindowProc );
	m_hAttachedHWnd = (HWND)hWnd;

	// New window, clear input state
	ClearInputState();
}


//-----------------------------------------------------------------------------
// Unhooks input listening from a window
//-----------------------------------------------------------------------------
void CInputSystem::DetachFromWindow( )
{
	if ( !m_hAttachedHWnd )
		return;

	ResetInputState();

	if ( m_ChainedWndProc )
	{
		SetWindowLongPtr( m_hAttachedHWnd, GWLP_WNDPROC, (LONG_PTR)m_ChainedWndProc );
		m_ChainedWndProc = 0;
	}

	m_hAttachedHWnd = 0;
}


//-----------------------------------------------------------------------------
// Enables/disables input
//-----------------------------------------------------------------------------
void CInputSystem::EnableInput( bool bEnable )
{
	m_bEnabled = bEnable;
}


//-----------------------------------------------------------------------------
// Enables/disables the inputsystem windows message pump
//-----------------------------------------------------------------------------
void CInputSystem::EnableMessagePump( bool bEnable )
{
	m_bPumpEnabled = bEnable;
}
	

//-----------------------------------------------------------------------------
// Clears the input state, doesn't generate key-up messages
//-----------------------------------------------------------------------------
void CInputSystem::ClearInputState()
{
	for ( int i = 0; i < INPUT_STATE_COUNT; ++i )
	{
		InputState_t& state = m_InputState[i];
		state.m_ButtonState.ClearAll();
		memset( state.m_pAnalogDelta, 0, ANALOG_CODE_LAST * sizeof(int) );
		memset( state.m_pAnalogValue, 0, ANALOG_CODE_LAST * sizeof(int) );
		memset( state.m_ButtonPressedTick, 0, BUTTON_CODE_LAST * sizeof(int) );
		memset( state.m_ButtonReleasedTick, 0, BUTTON_CODE_LAST * sizeof(int) );
		state.m_Events.Purge();
		state.m_bDirty = false;
	}
	memset( m_appXKeys, 0, XUSER_MAX_COUNT * XK_MAX_KEYS * sizeof(appKey_t) );
}

//-----------------------------------------------------------------------------
// Resets the input state
//-----------------------------------------------------------------------------
void CInputSystem::ResetInputState()
{
	ReleaseAllButtons();
	ZeroAnalogState( 0, ANALOG_CODE_LAST - 1 );
	memset( m_appXKeys, 0, XUSER_MAX_COUNT * XK_MAX_KEYS * sizeof(appKey_t) );
}


//-----------------------------------------------------------------------------
// Convert back + forth between ButtonCode/AnalogCode + strings
//-----------------------------------------------------------------------------
const char *CInputSystem::ButtonCodeToString( ButtonCode_t code ) const
{
	return ButtonCode_ButtonCodeToString( code, m_bXController );
}

const char *CInputSystem::AnalogCodeToString( AnalogCode_t code ) const
{
	return AnalogCode_AnalogCodeToString( code );
}

ButtonCode_t CInputSystem::StringToButtonCode( const char *pString ) const
{
	return ButtonCode_StringToButtonCode( pString, m_bXController );
}

AnalogCode_t CInputSystem::StringToAnalogCode( const char *pString ) const
{
	return AnalogCode_StringToAnalogCode( pString );
}


//-----------------------------------------------------------------------------
// Convert back + forth between virtual codes + button codes
// FIXME: This is a temporary piece of code
//-----------------------------------------------------------------------------
ButtonCode_t CInputSystem::VirtualKeyToButtonCode( int nVirtualKey ) const
{
	return ButtonCode_VirtualKeyToButtonCode( nVirtualKey );
}

int CInputSystem::ButtonCodeToVirtualKey( ButtonCode_t code ) const
{
	return ButtonCode_ButtonCodeToVirtualKey( code );
}

ButtonCode_t CInputSystem::XKeyToButtonCode( int nPort, int nXKey ) const
{
	if ( m_bXController )
		return ButtonCode_XKeyToButtonCode( nPort, nXKey );
	return KEY_NONE;
}

ButtonCode_t CInputSystem::ScanCodeToButtonCode( int lParam ) const
{
	return ButtonCode_ScanCodeToButtonCode( lParam );
}


//-----------------------------------------------------------------------------
// Post an event to the queue
//-----------------------------------------------------------------------------
void CInputSystem::PostEvent( int nType, int nTick, int nData, int nData2, int nData3 )
{
	InputState_t &state = m_InputState[ m_bIsPolling ];
	int i = state.m_Events.AddToTail();
	InputEvent_t &event = state.m_Events[i];
	event.m_nType = nType;
	event.m_nTick = nTick;
	event.m_nData = nData;
	event.m_nData2 = nData2;
	event.m_nData3 = nData3;
	state.m_bDirty = true;
}


//-----------------------------------------------------------------------------
// Post an button press event to the queue
//-----------------------------------------------------------------------------
void CInputSystem::PostButtonPressedEvent( InputEventType_t nType, int nTick, ButtonCode_t scanCode, ButtonCode_t virtualCode )
{
	InputState_t &state = m_InputState[ m_bIsPolling ];
	if ( !state.m_ButtonState.IsBitSet( scanCode ) )
	{
		// Update button state
		state.m_ButtonState.Set( scanCode ); 
		state.m_ButtonPressedTick[ scanCode ] = nTick;

		// Add this event to the app-visible event queue
		PostEvent( nType, nTick, scanCode, virtualCode );

		// FIXME: Remove! Fake a windows message for vguimatsurface's wndproc
		if ( IsX360() && IsJoystickCode( scanCode ) )
		{
			ProcessEvent( WM_XCONTROLLER_KEY, scanCode, 1 );
		}
	}
}


//-----------------------------------------------------------------------------
// Post an button release event to the queue
//-----------------------------------------------------------------------------
void CInputSystem::PostButtonReleasedEvent( InputEventType_t nType, int nTick, ButtonCode_t scanCode, ButtonCode_t virtualCode )
{
	InputState_t &state = m_InputState[ m_bIsPolling ];
	if ( state.m_ButtonState.IsBitSet( scanCode ) )
	{
		// Update button state
		state.m_ButtonState.Clear( scanCode ); 
		state.m_ButtonReleasedTick[ scanCode ] = nTick;

		// Add this event to the app-visible event queue
		PostEvent( nType, nTick, scanCode, virtualCode );

		// FIXME: Remove! Fake a windows message for vguimatsurface's input handler
		if ( IsX360() && IsJoystickCode( scanCode ) )
		{
			ProcessEvent( WM_XCONTROLLER_KEY, scanCode, 0 );
		}
	}
}


//-----------------------------------------------------------------------------
//	Purpose: Pass Joystick button events through the engine's window procs
//-----------------------------------------------------------------------------
void CInputSystem::ProcessEvent( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// To prevent subtle input timing bugs, all button events must be fed 
	// through the window proc once per frame, same as the keyboard and mouse.
	HWND hWnd = GetFocus();
	WNDPROC windowProc = (WNDPROC)GetWindowLongPtr(hWnd, GWLP_WNDPROC );
	if ( windowProc )
	{
		windowProc( hWnd, uMsg, wParam, lParam );
	}
}


//-----------------------------------------------------------------------------
// Copies the input state record over
//-----------------------------------------------------------------------------
void CInputSystem::CopyInputState( InputState_t *pDest, const InputState_t &src, bool bCopyEvents )
{
	pDest->m_Events.RemoveAll();
	pDest->m_bDirty = false;
	if ( src.m_bDirty )
	{
		pDest->m_ButtonState = src.m_ButtonState;
		memcpy( &pDest->m_ButtonPressedTick, &src.m_ButtonPressedTick, sizeof( BUTTON_CODE_LAST * sizeof(int) ) );
		memcpy( &pDest->m_ButtonReleasedTick, &src.m_ButtonReleasedTick, sizeof( BUTTON_CODE_LAST * sizeof(int) ) );
		memcpy( &pDest->m_pAnalogDelta, &src.m_pAnalogDelta, sizeof( ANALOG_CODE_LAST * sizeof(int) ) );
		memcpy( &pDest->m_pAnalogValue, &src.m_pAnalogValue, sizeof( ANALOG_CODE_LAST * sizeof(int) ) );
		if ( bCopyEvents )
		{
			if ( src.m_Events.Count() > 0 )
			{
				pDest->m_Events.EnsureCount( src.m_Events.Count() );
				memcpy( pDest->m_Events.Base(), src.m_Events.Base(), src.m_Events.Count() * sizeof(InputEvent_t) );
			}
		}
	}
}


//-----------------------------------------------------------------------------
// Polls the current input state
//-----------------------------------------------------------------------------
void CInputSystem::PollInputState()
{
	m_bIsPolling = true;
	++m_nPollCount;

	// Deals with polled input events
	InputState_t &queuedState = m_InputState[ INPUT_STATE_QUEUED ];
	CopyInputState( &m_InputState[ INPUT_STATE_CURRENT ], queuedState, true );

	// Sample the joystick
	SampleDevices();

	// NOTE: This happens after SampleDevices since that updates LastSampleTick
	// Also, I believe it's correct to post the joystick events with
	// the LastPollTick not updated (not 100% sure though)
	m_nLastPollTick = m_nLastSampleTick;

	if ( IsPC() && m_bPumpEnabled )
	{
		// Poll mouse + keyboard
		MSG msg;
		while ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			if ( msg.message == WM_QUIT )
			{
				PostEvent( IE_Quit, m_nLastSampleTick );
				break;
			}

			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}

		// NOTE: Under some implementations of Win9x, 
		// dispatching messages can cause the FPU control word to change
		SetupFPUControlWord();
	}

	// Leave the queued state up-to-date with the current
	CopyInputState( &queuedState, m_InputState[ INPUT_STATE_CURRENT ], false );

	m_bIsPolling = false;
}


//-----------------------------------------------------------------------------
// Computes the sample tick
//-----------------------------------------------------------------------------
int CInputSystem::ComputeSampleTick()
{
	// This logic will only fail if the app has been running for 49.7 days
	int nSampleTick;

	DWORD nCurrentTick = GetTickCount();
	if ( nCurrentTick >= m_StartupTimeTick )
	{
		nSampleTick = (int)( nCurrentTick - m_StartupTimeTick );
	}
	else
	{
		DWORD nDelta = (DWORD)0xFFFFFFFF - m_StartupTimeTick;
		nSampleTick = (int)( nCurrentTick + nDelta ) + 1;
	}
	return nSampleTick;
}


//-----------------------------------------------------------------------------
// How many times has poll been called?
//-----------------------------------------------------------------------------
int CInputSystem::GetPollCount() const
{
	return m_nPollCount;
}


//-----------------------------------------------------------------------------
// Samples attached devices and appends events to the input queue
//-----------------------------------------------------------------------------
void CInputSystem::SampleDevices( void )
{
	m_nLastSampleTick = ComputeSampleTick();

	if ( m_bXController )
	{
		PollXDevices();
	}
	else
	{
		PollJoystick();
	}
}


//-----------------------------------------------------------------------------
//	Purpose: Sets a player as the primary user - all other controllers will be ignored.
//-----------------------------------------------------------------------------
void CInputSystem::SetPrimaryUserId( int userId )
{
	if ( userId >= XUSER_MAX_COUNT || userId < 0 )
	{
		m_PrimaryUserId = INVALID_USER_ID;
	}
	else
	{
		m_PrimaryUserId = userId;
	}
	XBX_SetPrimaryUserId( m_PrimaryUserId );
}

//-----------------------------------------------------------------------------
//	Purpose: Forwards rumble info to attached devices
//-----------------------------------------------------------------------------
void CInputSystem::SetRumble( float fLeftMotor, float fRightMotor, int userId )
{
	// TODO: send force feedback to rumble-enabled joysticks
	if ( IsX360() )
	{
		SetXDeviceRumble( fLeftMotor, fRightMotor, userId );
	}
}


//-----------------------------------------------------------------------------
//	Purpose: Force an immediate stop, transmits immediately to all devices
//-----------------------------------------------------------------------------
void CInputSystem::StopRumble( void )
{
	if ( IsX360() )
	{
		xdevice_t* pXDevice = &m_XDevices[0];

		for ( int i = 0; i < XUSER_MAX_COUNT; ++i, ++pXDevice )
		{
			if ( pXDevice->active )
			{
				pXDevice->vibration.wLeftMotorSpeed = 0;
				pXDevice->vibration.wRightMotorSpeed = 0;
				pXDevice->pendingRumbleUpdate = true;
				WriteToXDevice( pXDevice );
			}
		}
	}
}


//-----------------------------------------------------------------------------
// Joystick interface
//-----------------------------------------------------------------------------
int CInputSystem::GetJoystickCount() const
{
	return m_nJoystickCount;
}

void CInputSystem::EnableJoystickInput( int nJoystick, bool bEnable )
{
	m_JoysticksEnabled.SetFlag( 1 << nJoystick, bEnable ); 
}

void CInputSystem::EnableJoystickDiagonalPOV( int nJoystick, bool bEnable )
{
	m_pJoystickInfo[ nJoystick ].m_bDiagonalPOVControlEnabled = bEnable;
}

//-----------------------------------------------------------------------------
// Poll current state
//-----------------------------------------------------------------------------
int CInputSystem::GetPollTick() const
{
	return m_nLastPollTick;
}
	
bool CInputSystem::IsButtonDown( ButtonCode_t code ) const
{
	return m_InputState[INPUT_STATE_CURRENT].m_ButtonState.IsBitSet( code );
}

int CInputSystem::GetAnalogValue( AnalogCode_t code ) const
{
	return m_InputState[INPUT_STATE_CURRENT].m_pAnalogValue[code];
}

int CInputSystem::GetAnalogDelta( AnalogCode_t code ) const
{
	return m_InputState[INPUT_STATE_CURRENT].m_pAnalogDelta[code];
}

int CInputSystem::GetButtonPressedTick( ButtonCode_t code ) const
{
	return m_InputState[INPUT_STATE_CURRENT].m_ButtonPressedTick[code];
}

int CInputSystem::GetButtonReleasedTick( ButtonCode_t code ) const
{
	return m_InputState[INPUT_STATE_CURRENT].m_ButtonReleasedTick[code];
}


//-----------------------------------------------------------------------------
// Returns the input events since the last poll
//-----------------------------------------------------------------------------
int CInputSystem::GetEventCount() const
{
	return m_InputState[INPUT_STATE_CURRENT].m_Events.Count();
}

const InputEvent_t* CInputSystem::GetEventData( ) const
{
	return m_InputState[INPUT_STATE_CURRENT].m_Events.Base();
}


//-----------------------------------------------------------------------------
// Posts a user-defined event into the event queue; this is expected
// to be called in overridden wndprocs connected to the root panel.
//-----------------------------------------------------------------------------
void CInputSystem::PostUserEvent( const InputEvent_t &event )
{
	InputState_t &state = m_InputState[ m_bIsPolling ];
	state.m_Events.AddToTail( event );
	state.m_bDirty = true;
}

	
//-----------------------------------------------------------------------------
// Chains the window message to the previous wndproc
//-----------------------------------------------------------------------------
inline LRESULT CInputSystem::ChainWindowMessage( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if ( m_ChainedWndProc )
		return CallWindowProc( m_ChainedWndProc, hwnd, uMsg, wParam, lParam );

	// FIXME: This comment is lifted from vguimatsurface; 
	// may not apply in future when the system is completed.

	// This means the application is driving the messages (calling our window procedure manually)
	// rather than us hooking their window procedure. The engine needs to do this in order for VCR 
	// mode to play back properly.
	return 0;	
}

	
//-----------------------------------------------------------------------------
// Release all buttons
//-----------------------------------------------------------------------------
void CInputSystem::ReleaseAllButtons( int nFirstButton, int nLastButton )
{
	// Force button up messages for all down buttons
	for ( int i = nFirstButton; i <= nLastButton; ++i )
	{
		PostButtonReleasedEvent( IE_ButtonReleased, m_nLastSampleTick, (ButtonCode_t)i, (ButtonCode_t)i );
	}
}


//-----------------------------------------------------------------------------
// Zero analog state
//-----------------------------------------------------------------------------
void CInputSystem::ZeroAnalogState( int nFirstState, int nLastState )
{
	InputState_t &state = m_InputState[ m_bIsPolling ];
	memset( &state.m_pAnalogDelta[nFirstState], 0, ( nLastState - nFirstState + 1 ) * sizeof(int) );
	memset( &state.m_pAnalogValue[nFirstState], 0, ( nLastState - nFirstState + 1 ) * sizeof(int) );
}


//-----------------------------------------------------------------------------
// Determines all mouse button presses
//-----------------------------------------------------------------------------
int CInputSystem::ButtonMaskFromMouseWParam( WPARAM wParam, ButtonCode_t code, bool bDown ) const
{
	int nButtonMask = 0;

	if ( wParam & MK_LBUTTON )
	{
		nButtonMask |= 1;
	}

	if ( wParam & MK_RBUTTON )
	{
		nButtonMask |= 2;
	}

	if ( wParam & MK_MBUTTON )
	{
		nButtonMask |= 4;
	}

	if ( wParam & MS_MK_BUTTON4 )
	{
		nButtonMask |= 8;
	}

	if ( wParam & MS_MK_BUTTON5 )
	{
		nButtonMask |= 16;
	}

#ifdef _DEBUG
	if ( code != BUTTON_CODE_INVALID )
	{
		int nMsgMask = 1 << ( code - MOUSE_FIRST );
		int nTestMask = bDown ? nMsgMask : 0;
		Assert( ( nButtonMask & nMsgMask ) == nTestMask );
	}
#endif

	return nButtonMask;
}


//-----------------------------------------------------------------------------
// Updates the state of all mouse buttons
//-----------------------------------------------------------------------------
void CInputSystem::UpdateMouseButtonState( int nButtonMask, ButtonCode_t dblClickCode )
{
	for ( int i = 0; i < 5; ++i )
	{
		ButtonCode_t code = (ButtonCode_t)( MOUSE_FIRST + i );
		bool bDown = ( nButtonMask & ( 1 << i ) ) != 0;
		if ( bDown )
		{
			InputEventType_t type = ( code != dblClickCode ) ? IE_ButtonPressed : IE_ButtonDoubleClicked; 
			PostButtonPressedEvent( type, m_nLastSampleTick, code, code );
		}
		else
		{
			PostButtonReleasedEvent( IE_ButtonReleased, m_nLastSampleTick, code, code );
		}
	}
}


//-----------------------------------------------------------------------------
// Handles input messages
//-----------------------------------------------------------------------------
void CInputSystem::SetCursorPosition( int x, int y )
{
	if ( !m_hAttachedHWnd )
		return;

	POINT pt;
	pt.x = x; pt.y = y;
	ClientToScreen( (HWND)m_hAttachedHWnd, &pt );
	SetCursorPos( pt.x, pt.y );

	InputState_t &state = m_InputState[ m_bIsPolling ];
	bool bXChanged = ( state.m_pAnalogValue[ MOUSE_X ] != x );
	bool bYChanged = ( state.m_pAnalogValue[ MOUSE_Y ] != y );

	state.m_pAnalogValue[ MOUSE_X ] = x;
	state.m_pAnalogValue[ MOUSE_Y ] = y;
	state.m_pAnalogDelta[ MOUSE_X ] = 0;
	state.m_pAnalogDelta[ MOUSE_Y ] = 0;

	if ( bXChanged )
	{
		PostEvent( IE_AnalogValueChanged, m_nLastSampleTick, MOUSE_X, state.m_pAnalogValue[ MOUSE_X ], state.m_pAnalogDelta[ MOUSE_X ] );
	}
	if ( bYChanged )
	{
		PostEvent( IE_AnalogValueChanged, m_nLastSampleTick, MOUSE_Y, state.m_pAnalogValue[ MOUSE_Y ], state.m_pAnalogDelta[ MOUSE_Y ] );
	}
	if ( bXChanged || bYChanged )
	{
		PostEvent( IE_AnalogValueChanged, m_nLastSampleTick, MOUSE_XY, state.m_pAnalogValue[ MOUSE_X ], state.m_pAnalogValue[ MOUSE_Y ] );
	}
}


//-----------------------------------------------------------------------------
// Handles input messages
//-----------------------------------------------------------------------------
LRESULT CInputSystem::WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if ( !m_bEnabled )
		return ChainWindowMessage( hwnd, uMsg, wParam, lParam );

	if ( hwnd != m_hAttachedHWnd )
		return ChainWindowMessage( hwnd, uMsg, wParam, lParam );

	InputState_t &state = m_InputState[ m_bIsPolling ];
	switch( uMsg )
	{
	case WM_ACTIVATEAPP:
		if ( hwnd == m_hAttachedHWnd )
		{
			bool bActivated = ( wParam == 1 );
			if ( !bActivated )
			{
				ResetInputState();
			}
		}
		break;

	case WM_LBUTTONDOWN:
		{
			int nButtonMask = ButtonMaskFromMouseWParam( wParam, MOUSE_LEFT, true );
			UpdateMouseButtonState( nButtonMask );
		}
		break;

	case WM_LBUTTONUP:
		{
			int nButtonMask = ButtonMaskFromMouseWParam( wParam, MOUSE_LEFT, false );
			UpdateMouseButtonState( nButtonMask );
		}
		break;

	case WM_RBUTTONDOWN:
		{
			int nButtonMask = ButtonMaskFromMouseWParam( wParam, MOUSE_RIGHT, true );
			UpdateMouseButtonState( nButtonMask );
		}
		break;

	case WM_RBUTTONUP:
		{
			int nButtonMask = ButtonMaskFromMouseWParam( wParam, MOUSE_RIGHT, false );
			UpdateMouseButtonState( nButtonMask );
		}
		break;

	case WM_MBUTTONDOWN:
		{
			int nButtonMask = ButtonMaskFromMouseWParam( wParam, MOUSE_MIDDLE, true );
			UpdateMouseButtonState( nButtonMask );
		}
		break;

	case WM_MBUTTONUP:
		{
			int nButtonMask = ButtonMaskFromMouseWParam( wParam, MOUSE_MIDDLE, false );
			UpdateMouseButtonState( nButtonMask );
		}
		break;

	case MS_WM_XBUTTONDOWN:
		{
			ButtonCode_t code = ( HIWORD( wParam ) == 1 ) ? MOUSE_4 : MOUSE_5;
			int nButtonMask = ButtonMaskFromMouseWParam( wParam, code, true );
			UpdateMouseButtonState( nButtonMask );

			// Windows docs say the XBUTTON messages we should return true from
			return TRUE;
		}
		break;

	case MS_WM_XBUTTONUP:
		{
			ButtonCode_t code = ( HIWORD( wParam ) == 1 ) ? MOUSE_4 : MOUSE_5;
			int nButtonMask = ButtonMaskFromMouseWParam( wParam, code, false );
			UpdateMouseButtonState( nButtonMask );

			// Windows docs say the XBUTTON messages we should return true from
			return TRUE;
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			int nButtonMask = ButtonMaskFromMouseWParam( wParam, MOUSE_LEFT, true );
			UpdateMouseButtonState( nButtonMask, MOUSE_LEFT );
		}
		break;

	case WM_RBUTTONDBLCLK:
		{
			int nButtonMask = ButtonMaskFromMouseWParam( wParam, MOUSE_RIGHT, true );
			UpdateMouseButtonState( nButtonMask, MOUSE_RIGHT );
		}
		break;

	case WM_MBUTTONDBLCLK:
		{
			int nButtonMask = ButtonMaskFromMouseWParam( wParam, MOUSE_MIDDLE, true );
			UpdateMouseButtonState( nButtonMask, MOUSE_MIDDLE );
		}
		break;

	case MS_WM_XBUTTONDBLCLK:
		{
			ButtonCode_t code = ( HIWORD( wParam ) == 1 ) ? MOUSE_4 : MOUSE_5;
			int nButtonMask = ButtonMaskFromMouseWParam( wParam, code, true );
			UpdateMouseButtonState( nButtonMask, code );

			// Windows docs say the XBUTTON messages we should return true from
			return TRUE;
		}
		break;

	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		{
			// Suppress key repeats
			if ( !( lParam & ( 1<<30 ) ) )
			{
				// NOTE: These two can be unequal! For example, keypad enter
				// which returns KEY_ENTER from virtual keys, and KEY_PAD_ENTER from scan codes
				// Since things like vgui care about virtual keys; we're going to
				// put both scan codes in the input message
				ButtonCode_t virtualCode = ButtonCode_VirtualKeyToButtonCode( wParam );
				ButtonCode_t scanCode = ButtonCode_ScanCodeToButtonCode( lParam );
				PostButtonPressedEvent( IE_ButtonPressed, m_nLastSampleTick, scanCode, virtualCode );

				// Deal with toggles
				if ( scanCode == KEY_CAPSLOCK || scanCode == KEY_SCROLLLOCK || scanCode == KEY_NUMLOCK )
				{
					int nVirtualKey;
					ButtonCode_t toggleCode;
					switch( scanCode )
					{
					default: case KEY_CAPSLOCK: nVirtualKey = VK_CAPITAL; toggleCode = KEY_CAPSLOCKTOGGLE; break;
					case KEY_SCROLLLOCK: nVirtualKey = VK_SCROLL; toggleCode = KEY_SCROLLLOCKTOGGLE; break;
					case KEY_NUMLOCK: nVirtualKey = VK_NUMLOCK; toggleCode = KEY_NUMLOCKTOGGLE; break;
					};

					SHORT wState = GetKeyState( nVirtualKey );
					bool bToggleState = ( wState & 0x1 ) != 0;
					PostButtonPressedEvent( bToggleState ? IE_ButtonPressed : IE_ButtonReleased, m_nLastSampleTick, toggleCode, toggleCode );
				}
			}
		}
		break;

	case WM_KEYUP:
	case WM_SYSKEYUP:
		{
			// Don't handle key ups if the key's already up. This can happen when we alt-tab back to the engine.
			ButtonCode_t virtualCode = ButtonCode_VirtualKeyToButtonCode( wParam );
			ButtonCode_t scanCode = ButtonCode_ScanCodeToButtonCode( lParam );
			PostButtonReleasedEvent( IE_ButtonReleased, m_nLastSampleTick, scanCode, virtualCode );
		}
		break;

	case WM_MOUSEWHEEL:
		{
			ButtonCode_t code = (short)HIWORD( wParam ) > 0 ? MOUSE_WHEEL_UP : MOUSE_WHEEL_DOWN;
			state.m_ButtonPressedTick[ code ] = state.m_ButtonReleasedTick[ code ] = m_nLastSampleTick;
			PostEvent( IE_ButtonPressed, m_nLastSampleTick, code, code );
			PostEvent( IE_ButtonReleased, m_nLastSampleTick, code, code );

			state.m_pAnalogDelta[ MOUSE_WHEEL ] = ( (short)HIWORD(wParam) ) / WHEEL_DELTA;
			state.m_pAnalogValue[ MOUSE_WHEEL ] += state.m_pAnalogDelta[ MOUSE_WHEEL ];
			PostEvent( IE_AnalogValueChanged, m_nLastSampleTick, MOUSE_WHEEL, state.m_pAnalogValue[ MOUSE_WHEEL ], state.m_pAnalogDelta[ MOUSE_WHEEL ] );
		}
		break;

	case WM_MOUSEMOVE:
		{
			int nOldX = state.m_pAnalogValue[ MOUSE_X ];
			int nOldY = state.m_pAnalogValue[ MOUSE_Y ];

			state.m_pAnalogValue[ MOUSE_X ] = (short)LOWORD(lParam);
			state.m_pAnalogValue[ MOUSE_Y ] = (short)HIWORD(lParam);
			state.m_pAnalogDelta[ MOUSE_X ] = state.m_pAnalogValue[ MOUSE_X ] - nOldX;
			state.m_pAnalogDelta[ MOUSE_Y ] = state.m_pAnalogValue[ MOUSE_Y ] - nOldY;

			if ( state.m_pAnalogDelta[ MOUSE_X ] != 0 )
			{
				PostEvent( IE_AnalogValueChanged, m_nLastSampleTick, MOUSE_X, state.m_pAnalogValue[ MOUSE_X ], state.m_pAnalogDelta[ MOUSE_X ] );
			}
			if ( state.m_pAnalogDelta[ MOUSE_Y ] != 0 )
			{
				PostEvent( IE_AnalogValueChanged, m_nLastSampleTick, MOUSE_Y, state.m_pAnalogValue[ MOUSE_Y ], state.m_pAnalogDelta[ MOUSE_Y ] );
			}
			if ( state.m_pAnalogDelta[ MOUSE_X ] != 0 || state.m_pAnalogDelta[ MOUSE_Y ] != 0 )
			{
				PostEvent( IE_AnalogValueChanged, m_nLastSampleTick, MOUSE_XY, state.m_pAnalogValue[ MOUSE_X ], state.m_pAnalogValue[ MOUSE_Y ] );
			}

			int nButtonMask = ButtonMaskFromMouseWParam( wParam );
			UpdateMouseButtonState( nButtonMask );
		}
 		break;
	}

	// Can't put this in the case statement, it's not constant
	if ( IsPC() && ( uMsg == m_uiMouseWheel ) )
	{
		ButtonCode_t code = ( ( int )wParam ) > 0 ? MOUSE_WHEEL_UP : MOUSE_WHEEL_DOWN;
		state.m_ButtonPressedTick[ code ] = state.m_ButtonReleasedTick[ code ] = m_nLastSampleTick;
		PostEvent( IE_ButtonPressed, m_nLastSampleTick, code, code );
		PostEvent( IE_ButtonReleased, m_nLastSampleTick, code, code );

		state.m_pAnalogDelta[ MOUSE_WHEEL ] = ( ( int )wParam ) / WHEEL_DELTA;
		state.m_pAnalogValue[ MOUSE_WHEEL ] += state.m_pAnalogDelta[ MOUSE_WHEEL ];
		PostEvent( IE_AnalogValueChanged, m_nLastSampleTick, MOUSE_WHEEL, state.m_pAnalogValue[ MOUSE_WHEEL ], state.m_pAnalogDelta[ MOUSE_WHEEL ] );
	}

	return ChainWindowMessage( hwnd, uMsg, wParam, lParam );
}
