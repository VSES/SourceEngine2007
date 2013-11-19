//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: Xbox controller implementation for inputsystem.dll
//
//===========================================================================//

#include "inputsystem.h"

//-----------------------------------------------------------------------------
// Xbox helpers
//-----------------------------------------------------------------------------
#define XBX_MAX_MOTOR_SPEED			65535

#define XBX_MAX_BUTTONSAMPLE		32768
#define XBX_MAX_ANALOGSAMPLE		255
#define XBX_MAX_STICKSAMPLE_LEFT	32768
#define XBX_MAX_STICKSAMPLE_RIGHT	32767
#define XBX_MAX_STICKSAMPLE_DOWN	32768
#define XBX_MAX_STICKSAMPLE_UP		32767

#define XBX_STICK_SCALE_LEFT(x) 	( ( float )XBX_MAX_STICKSAMPLE_LEFT/( float )( XBX_MAX_STICKSAMPLE_LEFT-(x) ) )
#define XBX_STICK_SCALE_RIGHT(x) 	( ( float )XBX_MAX_STICKSAMPLE_RIGHT/( float )( XBX_MAX_STICKSAMPLE_RIGHT-(x) ) )
#define XBX_STICK_SCALE_DOWN(x) 	( ( float )XBX_MAX_STICKSAMPLE_DOWN/( float )( XBX_MAX_STICKSAMPLE_DOWN-(x) ) )
#define XBX_STICK_SCALE_UP(x)	 	( ( float )XBX_MAX_STICKSAMPLE_UP/( float )( XBX_MAX_STICKSAMPLE_UP-(x) ) )

#define XBX_STICK_SMALL_THRESHOLD	((int)( 0.20f * XBX_MAX_STICKSAMPLE_LEFT ))

// Threshold for counting analog movement as a button press
#define JOYSTICK_ANALOG_BUTTON_THRESHOLD	XBX_MAX_STICKSAMPLE_LEFT * 0.4f

// Xbox key translation
typedef struct
{
	int	xinput;
	int	xkey;
} xInputToXKey_t;

xInputToXKey_t g_digitalXKeyTable[] = 
{
	{XINPUT_GAMEPAD_DPAD_UP,		XK_BUTTON_UP}, 
	{XINPUT_GAMEPAD_DPAD_DOWN,		XK_BUTTON_DOWN}, 
	{XINPUT_GAMEPAD_DPAD_LEFT,		XK_BUTTON_LEFT}, 
	{XINPUT_GAMEPAD_DPAD_RIGHT,		XK_BUTTON_RIGHT}, 
	{XINPUT_GAMEPAD_START,			XK_BUTTON_START}, 
	{XINPUT_GAMEPAD_BACK,			XK_BUTTON_BACK}, 
	{XINPUT_GAMEPAD_LEFT_THUMB,		XK_BUTTON_STICK1}, 
	{XINPUT_GAMEPAD_RIGHT_THUMB,	XK_BUTTON_STICK2}, 
	{XINPUT_GAMEPAD_LEFT_SHOULDER,	XK_BUTTON_LEFT_SHOULDER}, 
	{XINPUT_GAMEPAD_RIGHT_SHOULDER,	XK_BUTTON_RIGHT_SHOULDER}, 
	{XINPUT_GAMEPAD_A,				XK_BUTTON_A}, 
	{XINPUT_GAMEPAD_B,				XK_BUTTON_B}, 
	{XINPUT_GAMEPAD_X,				XK_BUTTON_X}, 
	{XINPUT_GAMEPAD_Y,				XK_BUTTON_Y}, 
};

#if !defined( _X360 )
	typedef DWORD (WINAPI *XInputGetState_t)
		(
		DWORD         dwUserIndex,  // [in] Index of the gamer associated with the device
		XINPUT_STATE* pState        // [out] Receives the current state
		);

	typedef DWORD (WINAPI *XInputSetState_t)
		(
		DWORD             dwUserIndex,  // [in] Index of the gamer associated with the device
		XINPUT_VIBRATION* pVibration    // [in, out] The vibration information to send to the controller
		);

	typedef DWORD (WINAPI *XInputGetCapabilities_t)
		(
		DWORD                dwUserIndex,   // [in] Index of the gamer associated with the device
		DWORD                dwFlags,       // [in] Input flags that identify the device type
		XINPUT_CAPABILITIES* pCapabilities  // [out] Receives the capabilities
		);

	XInputGetState_t PC_XInputGetState;
	XInputSetState_t PC_XInputSetState;
	XInputGetCapabilities_t PC_XInputGetCapabilities;

	#define XINPUTGETSTATE			PC_XInputGetState
	#define XINPUTSETSTATE			PC_XInputSetState
	#define XINPUTGETCAPABILITIES	PC_XInputGetCapabilities
#else
	#define XINPUTGETSTATE			XInputGetState
	#define XINPUTSETSTATE			XInputSetState
	#define XINPUTGETCAPABILITIES	XInputGetCapabilities
#endif

//-----------------------------------------------------------------------------
//	Purpose: Initialize all Xbox controllers
//-----------------------------------------------------------------------------
void CInputSystem::InitializeXDevices( void )
{
	int					i;
	xdevice_t*			pXDevice;

 	// assume no joystick
	m_nJoystickCount = 0; 

#if !defined( _X360 )
	PC_XInputGetState = (XInputGetState_t)GetProcAddress( (HMODULE)m_pXInputDLL, "XInputGetState" );
	PC_XInputSetState = (XInputSetState_t)GetProcAddress( (HMODULE)m_pXInputDLL, "XInputSetState" );
	PC_XInputGetCapabilities = (XInputGetCapabilities_t)GetProcAddress( (HMODULE)m_pXInputDLL, "XInputGetCapabilities" );
	if ( !PC_XInputGetState || !PC_XInputSetState || !PC_XInputGetCapabilities )
		return;
#endif

	// query gamepads
	pXDevice = m_XDevices;
	for ( i = 0; i < XUSER_MAX_COUNT; ++i, ++pXDevice )
	{
		OpenXDevice( pXDevice, i );
		//Msg( "UserID %d: %s\n", i+1, pXDevice->userId != INVALID_USER_ID ? "GamePad" : "???" );
	}
}

//-----------------------------------------------------------------------------
//	Purpose: Open an Xbox controller
//-----------------------------------------------------------------------------
void CInputSystem::OpenXDevice( xdevice_t* pXDevice, int userId )
{
	XINPUT_CAPABILITIES	capabilities;

	// Invalidate device properties
	pXDevice->userId = INVALID_USER_ID;
	pXDevice->active = false;

	DWORD result = XINPUTGETCAPABILITIES( userId, XINPUT_FLAG_GAMEPAD, &capabilities );
	if ( result == ERROR_SUCCESS )
	{
		bool bIsSupported = false;
		if ( IsX360() )
		{
			// TCR says that we cannot restrict input based on subtype, so don't check it
			bIsSupported = ( capabilities.Type == XINPUT_DEVTYPE_GAMEPAD );
		}
		else
		{
			// Current version of XInput mistakenly returns 0 as the Type. Ignore it and ensure the subtype is a gamepad.
			bIsSupported = ( capabilities.SubType == XINPUT_DEVSUBTYPE_GAMEPAD );
		}
		if ( !bIsSupported )
		{
			// TBD: This may not be sufficient to not crash us later
			Assert( 0 && "Unsupported XDevice Type" );
			return;
		}

		// valid
		pXDevice->type		  = capabilities.Type;
		pXDevice->subtype	  = capabilities.SubType;
		pXDevice->flags		  = capabilities.Flags;
		pXDevice->userId      = userId;
		pXDevice->active	  = true;
		pXDevice->quitTimeout = 0;
		pXDevice->dpadLock    = 0;

		// left stick, default to narrow zone
		pXDevice->stickThreshold[STICK1_AXIS_LEFT]  = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
		pXDevice->stickScale[STICK1_AXIS_LEFT]      = XBX_STICK_SCALE_LEFT( XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE );
		pXDevice->stickThreshold[STICK1_AXIS_RIGHT] = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
		pXDevice->stickScale[STICK1_AXIS_RIGHT]     = XBX_STICK_SCALE_RIGHT( XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE );
		pXDevice->stickThreshold[STICK1_AXIS_DOWN]  = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
		pXDevice->stickScale[STICK1_AXIS_DOWN]      = XBX_STICK_SCALE_DOWN( XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE );
		pXDevice->stickThreshold[STICK1_AXIS_UP]    = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
		pXDevice->stickScale[STICK1_AXIS_UP]        = XBX_STICK_SCALE_UP( XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE );

		// right stick, default to narrow zone
		pXDevice->stickThreshold[STICK2_AXIS_LEFT]  = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
		pXDevice->stickScale[STICK2_AXIS_LEFT]      = XBX_STICK_SCALE_LEFT( XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE );
		pXDevice->stickThreshold[STICK2_AXIS_RIGHT] = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
		pXDevice->stickScale[STICK2_AXIS_RIGHT]     = XBX_STICK_SCALE_RIGHT( XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE );
		pXDevice->stickThreshold[STICK2_AXIS_DOWN]  = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
		pXDevice->stickScale[STICK2_AXIS_DOWN]      = XBX_STICK_SCALE_DOWN( XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE );
		pXDevice->stickThreshold[STICK2_AXIS_UP]    = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
		pXDevice->stickScale[STICK2_AXIS_UP]        = XBX_STICK_SCALE_UP( XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE );

		pXDevice->vibration.wLeftMotorSpeed = WORD( 0 );
		pXDevice->vibration.wRightMotorSpeed = WORD( 0 );
		pXDevice->pendingRumbleUpdate = false;
		++m_nJoystickCount;
	}
}


//-----------------------------------------------------------------------------
//	Purpose: Close an Xbox controller
//-----------------------------------------------------------------------------
void CInputSystem::CloseXDevice( xdevice_t* pXDevice )
{
	pXDevice->userId = INVALID_USER_ID;
	pXDevice->active = false;
	--m_nJoystickCount;
}


//-----------------------------------------------------------------------------
//	Purpose: Sample the Xbox controllers.
//-----------------------------------------------------------------------------
void CInputSystem::PollXDevices( void )
{
	xdevice_t* pXDevice = m_XDevices;

	for ( int userId = 0; userId < XUSER_MAX_COUNT; ++userId, ++pXDevice )
	{
		DWORD result = XINPUTGETSTATE( userId, &pXDevice->states[pXDevice->newState] );
		switch ( result )
		{
		case ERROR_SUCCESS:
			if ( !pXDevice->active )
			{
				// just inserted
				OpenXDevice( pXDevice, userId );
				PostEvent( IE_ControllerInserted, m_nLastSampleTick, userId );
			}

			// See if primary user has been set, and if so then block all other input
			if ( pXDevice->userId == m_PrimaryUserId ||  m_PrimaryUserId == INVALID_USER_ID )
			{
				ReadXDevice( pXDevice );
				WriteToXDevice( pXDevice );
			}

			break;

		case ERROR_DEVICE_NOT_CONNECTED:
			if ( pXDevice->active )
			{
				// just removed
				int closedPort = pXDevice->userId;
				CloseXDevice( pXDevice );
				
				if ( m_PrimaryUserId == INVALID_USER_ID || m_PrimaryUserId == userId )
				{
					// Controller unplugged - game needs to take action

					// TODO: Change this to release buttons of the specific joystick that unplugged
					ReleaseAllButtons( JOYSTICK_FIRST, JOYSTICK_LAST );
					ZeroAnalogState( JOYSTICK_FIRST_AXIS, JOYSTICK_LAST_AXIS );
					memset( m_appXKeys, 0, XUSER_MAX_COUNT * XK_MAX_KEYS * sizeof(appKey_t) );

					PostEvent( IE_ControllerUnplugged, m_nLastSampleTick, closedPort );
				}
			}
			break;
		}
	}
}


//-----------------------------------------------------------------------------
//	Purpose: Post Xbox events, ignoring key repeats
//-----------------------------------------------------------------------------
void CInputSystem::PostXKeyEvent( int userId, xKey_t xKey, int nSample )
{
	AnalogCode_t	code	= ANALOG_CODE_LAST;
	float			value	= 0.f;

	// Lock the user. 
	if ( m_PrimaryUserId == INVALID_USER_ID )
	{
		// Lock in the active user. Input is ignored from all other controllers
		SetPrimaryUserId( userId );
	}

	// FIXME: Uncomment userId when we support multiple controllers
	int nMsgUserId = 0; // userId;
	int nSampleThreshold = 0; 

	// Look for changes on the analog axes
	switch( xKey )
	{
	case XK_STICK1_LEFT:
	case XK_STICK1_RIGHT:
		{
			code = (AnalogCode_t)JOYSTICK_AXIS( nMsgUserId, JOY_AXIS_X );
			value = ( xKey == XK_STICK1_LEFT ) ? -nSample : nSample;
			nSampleThreshold = JOYSTICK_ANALOG_BUTTON_THRESHOLD;
		}
		break;

	case XK_STICK1_UP:
	case XK_STICK1_DOWN:
		{
			code = (AnalogCode_t)JOYSTICK_AXIS( nMsgUserId, JOY_AXIS_Y );
			if ( IsX360() )
			{
				value = ( xKey == XK_STICK1_DOWN ) ? -nSample : nSample;
			}
			else
			{
				value = ( xKey == XK_STICK1_UP ) ? -nSample : nSample;
			}
			nSampleThreshold = JOYSTICK_ANALOG_BUTTON_THRESHOLD;
		}
		break;

	case XK_STICK2_LEFT:
	case XK_STICK2_RIGHT:
		{
			code = (AnalogCode_t)JOYSTICK_AXIS( nMsgUserId, JOY_AXIS_U );
			value = ( xKey == XK_STICK2_LEFT ) ? -nSample : nSample;
			nSampleThreshold = JOYSTICK_ANALOG_BUTTON_THRESHOLD;
		}
		break;

	case XK_STICK2_UP:
	case XK_STICK2_DOWN:
		{
			code = (AnalogCode_t)JOYSTICK_AXIS( nMsgUserId, JOY_AXIS_R );
			if ( IsX360() )
			{
				value = ( xKey == XK_STICK2_DOWN ) ? -nSample : nSample;
			}
			else
			{
				value = ( xKey == XK_STICK2_UP ) ? -nSample : nSample;
			}
			nSampleThreshold = JOYSTICK_ANALOG_BUTTON_THRESHOLD;
		}
		break;
	}

	// Store the analog event
	if ( ANALOG_CODE_LAST != code )
	{
		InputState_t &state = m_InputState[ m_bIsPolling ];
		state.m_pAnalogDelta[ code ] = value - state.m_pAnalogValue[ code ];
		state.m_pAnalogValue[ code ] = value;
		if ( state.m_pAnalogDelta[ code ] != 0 )
		{
			PostEvent( IE_AnalogValueChanged, m_nLastSampleTick, code, value, 0 );
		}
	}

	// store the key
	m_appXKeys[userId][xKey].sample = nSample;
	if ( nSample > nSampleThreshold )
	{
		m_appXKeys[userId][xKey].repeats++;
	}
	else
	{
		m_appXKeys[userId][xKey].repeats = 0;
		nSample = 0;
	}

	if ( m_appXKeys[userId][xKey].repeats > 1 )
	{
		// application cannot handle streaming keys
		// first keypress is the only edge trigger
		return;
	}

	// package the key
	ButtonCode_t buttonCode = XKeyToButtonCode( nMsgUserId, xKey );
	if ( nSample )
	{
		PostButtonPressedEvent( IE_ButtonPressed, m_nLastSampleTick, buttonCode, buttonCode );
	}
	else
	{
		PostButtonReleasedEvent( IE_ButtonReleased, m_nLastSampleTick, buttonCode, buttonCode );
	}
}


//-----------------------------------------------------------------------------
//	Purpose: Send force feedback to an Xbox controller
//-----------------------------------------------------------------------------
void CInputSystem::WriteToXDevice( xdevice_t* pXDevice )
{
	if ( pXDevice->pendingRumbleUpdate )
	{
		XINPUTSETSTATE( pXDevice->userId, &pXDevice->vibration );
		pXDevice->pendingRumbleUpdate = false;
	}
}


//-----------------------------------------------------------------------------
//	Purpose: Queue input key events for a device
//-----------------------------------------------------------------------------
void CInputSystem::ReadXDevice( xdevice_t* pXDevice )
{
	XINPUT_STATE*	oldStatePtr;
	XINPUT_STATE*	newStatePtr;
	xKey_t*			lastStickKeys;
	xKey_t			key;
	int				x;
	int				y;
	int				mask;
	int				buttons;
	int				sample;

	oldStatePtr = &pXDevice->states[pXDevice->newState ^ 1];
	newStatePtr = &pXDevice->states[pXDevice->newState];
	
	// forceful exit
	if ( newStatePtr->Gamepad.wButtons == ( XINPUT_GAMEPAD_START|XINPUT_GAMEPAD_BACK ) )
	{
		if ( !pXDevice->quitTimeout )
		{
			pXDevice->quitTimeout = GetTickCount();
		}
		else if ( GetTickCount() - pXDevice->quitTimeout > 2*1000 )
		{
			// mandatory 2sec hold
			pXDevice->quitTimeout = 0;
			ProcessEvent( WM_XREMOTECOMMAND, 0, (LPARAM)"quit_x360" );
		}
	}
	else
	{
		// reset
		pXDevice->quitTimeout = 0;
	}

	// No changes if packet numbers match
	if ( oldStatePtr->dwPacketNumber == newStatePtr->dwPacketNumber )
		return;

	// digital events
	buttons = newStatePtr->Gamepad.wButtons ^ oldStatePtr->Gamepad.wButtons;
	if ( buttons )
	{
		// determine if dpad press is axial or diagonal combos
		bool bDpadIsAxial = IsPowerOfTwo( newStatePtr->Gamepad.wButtons & (XINPUT_GAMEPAD_DPAD_UP|XINPUT_GAMEPAD_DPAD_DOWN|XINPUT_GAMEPAD_DPAD_LEFT|XINPUT_GAMEPAD_DPAD_RIGHT) );

		// determine digital difference - up or down?
		for ( int i = 0; i < sizeof( g_digitalXKeyTable )/sizeof( g_digitalXKeyTable[0] ); ++i )
		{
			mask = buttons & g_digitalXKeyTable[i].xinput;
			if ( !mask )
				continue;

			if ( mask & newStatePtr->Gamepad.wButtons )
			{
				// down event
				sample = XBX_MAX_BUTTONSAMPLE;
			}
			else
			{
				// up event
				sample = 0;
			}

			// due to rocker mechanics, allow only 1 of 4 behavior
			// the last down axial event trumps, preventing diagonals causing multiple events
			if ( mask & (XINPUT_GAMEPAD_DPAD_UP|XINPUT_GAMEPAD_DPAD_DOWN|XINPUT_GAMEPAD_DPAD_LEFT|XINPUT_GAMEPAD_DPAD_RIGHT) )
			{
				if ( !bDpadIsAxial )
				{
					// diagonal dpad event, discard any dpad event that isn't the lock
					if ( pXDevice->dpadLock && pXDevice->dpadLock != mask )
						continue;
				}
				else if ( sample )
				{
					// axial dpad down event, set the lock
					pXDevice->dpadLock = mask;
				}
			}

			PostXKeyEvent( pXDevice->userId, (xKey_t)g_digitalXKeyTable[i].xkey, sample );
		}
	}

	// analog events
	// queue left trigger axis analog response
	if ( newStatePtr->Gamepad.bLeftTrigger <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD )
	{
		newStatePtr->Gamepad.bLeftTrigger = 0;	
	}
	sample = newStatePtr->Gamepad.bLeftTrigger;
	if ( sample != oldStatePtr->Gamepad.bLeftTrigger )
	{
		PostXKeyEvent( pXDevice->userId, XK_BUTTON_LTRIGGER, ( int )( sample * ( float )XBX_MAX_BUTTONSAMPLE/( float )XBX_MAX_ANALOGSAMPLE ) );
	}

	// queue right trigger axis analog response
	if ( newStatePtr->Gamepad.bRightTrigger <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD )
	{
		newStatePtr->Gamepad.bRightTrigger = 0;	
	}
	sample = newStatePtr->Gamepad.bRightTrigger;
	if ( sample != oldStatePtr->Gamepad.bRightTrigger )
	{
		PostXKeyEvent( pXDevice->userId, XK_BUTTON_RTRIGGER, ( int )( sample * ( float )XBX_MAX_BUTTONSAMPLE/( float )XBX_MAX_ANALOGSAMPLE ) );
	}

	lastStickKeys = pXDevice->lastStickKeys;

	// queue stick1 x-axis analog response
	x = newStatePtr->Gamepad.sThumbLX;
	if ( x < -pXDevice->stickThreshold[STICK1_AXIS_LEFT] )
	{
		x = ( int )( ( -x - pXDevice->stickThreshold[STICK1_AXIS_LEFT] ) * pXDevice->stickScale[STICK1_AXIS_LEFT] );
		PostXKeyEvent( pXDevice->userId, XK_STICK1_LEFT, x );
		key = XK_STICK1_LEFT;
	}
	else if ( x > pXDevice->stickThreshold[STICK1_AXIS_RIGHT] )
	{
		x = ( int )( ( x - pXDevice->stickThreshold[STICK1_AXIS_RIGHT] ) * pXDevice->stickScale[STICK1_AXIS_RIGHT] );
		PostXKeyEvent( pXDevice->userId, XK_STICK1_RIGHT, x );
		key = XK_STICK1_RIGHT;
	}
	else 
		key = XK_NULL;

	if ( lastStickKeys[LASTKEY_STICK1_X] && lastStickKeys[LASTKEY_STICK1_X] != key )
	{
		// queue stick1 x-axis release response
		PostXKeyEvent( pXDevice->userId, lastStickKeys[LASTKEY_STICK1_X], 0 );
	}
	lastStickKeys[LASTKEY_STICK1_X] = key;

	// queue stick1 y-axis analog response
	y = newStatePtr->Gamepad.sThumbLY;
	if ( y < -pXDevice->stickThreshold[STICK1_AXIS_DOWN] )
	{
		y = ( int )( ( -y - pXDevice->stickThreshold[STICK1_AXIS_DOWN] ) * pXDevice->stickScale[STICK1_AXIS_DOWN] );
		PostXKeyEvent( pXDevice->userId, XK_STICK1_DOWN, y );
		key = XK_STICK1_DOWN;
	}
	else if ( y > pXDevice->stickThreshold[STICK1_AXIS_UP] )
	{
		y = ( int )( ( y - pXDevice->stickThreshold[STICK1_AXIS_UP] ) * pXDevice->stickScale[STICK1_AXIS_UP] );
		PostXKeyEvent( pXDevice->userId, XK_STICK1_UP, y );
		key = XK_STICK1_UP;
	}
	else
		key = XK_NULL;

	if ( lastStickKeys[LASTKEY_STICK1_Y] && lastStickKeys[LASTKEY_STICK1_Y] != key )
	{
		// queue y-axis release response
		PostXKeyEvent( pXDevice->userId, lastStickKeys[LASTKEY_STICK1_Y], 0 );
	}
	lastStickKeys[LASTKEY_STICK1_Y] = key;
		
	// queue stick2 x-axis analog response
	x = newStatePtr->Gamepad.sThumbRX;
	if ( x < -pXDevice->stickThreshold[STICK2_AXIS_LEFT] )
	{
		x = ( int )( ( -x - pXDevice->stickThreshold[STICK2_AXIS_LEFT] ) * pXDevice->stickScale[STICK2_AXIS_LEFT] );
		PostXKeyEvent( pXDevice->userId, XK_STICK2_LEFT, x );
		key = XK_STICK2_LEFT;
	}
	else if ( x > pXDevice->stickThreshold[STICK2_AXIS_RIGHT] )
	{
		x = ( int )( ( x - pXDevice->stickThreshold[STICK2_AXIS_RIGHT] ) * pXDevice->stickScale[STICK2_AXIS_RIGHT] );
		PostXKeyEvent( pXDevice->userId, XK_STICK2_RIGHT, x );
		key = XK_STICK2_RIGHT;
	}
	else 
		key = XK_NULL;

	if ( lastStickKeys[LASTKEY_STICK2_X] && lastStickKeys[LASTKEY_STICK2_X] != key )
	{
		// queue stick1 x-axis release response
		PostXKeyEvent( pXDevice->userId, lastStickKeys[LASTKEY_STICK2_X], 0 );
	}
	lastStickKeys[LASTKEY_STICK2_X] = key;

	// queue stick2 y-axis analog response
	y = newStatePtr->Gamepad.sThumbRY;
	if ( y < -pXDevice->stickThreshold[STICK2_AXIS_DOWN] )
	{
		y = ( int )( ( -y - pXDevice->stickThreshold[STICK2_AXIS_DOWN] ) * pXDevice->stickScale[STICK2_AXIS_DOWN]  );
		PostXKeyEvent( pXDevice->userId, XK_STICK2_DOWN, y );
		key = XK_STICK2_DOWN;
	}
	else if ( y > pXDevice->stickThreshold[STICK2_AXIS_UP] )
	{
		y = ( int )( ( y - pXDevice->stickThreshold[STICK2_AXIS_UP] ) * pXDevice->stickScale[STICK2_AXIS_UP] );
		PostXKeyEvent( pXDevice->userId, XK_STICK2_UP, y );
		key = XK_STICK2_UP;
	}
	else
		key = XK_NULL;

	if ( lastStickKeys[LASTKEY_STICK2_Y] && lastStickKeys[LASTKEY_STICK2_Y] != key )
	{
		// queue y-axis release response
		PostXKeyEvent( pXDevice->userId, lastStickKeys[LASTKEY_STICK2_Y], 0 );
	}
	lastStickKeys[LASTKEY_STICK2_Y] = key;

	// toggle the states
	pXDevice->newState ^= 1;
}

//-----------------------------------------------------------------------------
//	Purpose: Queues a left and right motor value for the Xbox controller
//-----------------------------------------------------------------------------
void CInputSystem::SetXDeviceRumble( float fLeftMotor, float fRightMotor, int userId )
{
	WORD wOldLeft, wOldRight;// Last values we sent
	WORD wNewLeft, wNewRight;// Values we're about to send.
	xdevice_t* pXDevice;

	if ( userId == INVALID_USER_ID )
	{
		userId = m_PrimaryUserId;
		if ( userId == INVALID_USER_ID )
		{
			return;
		}
	}

	pXDevice = &m_XDevices[userId];

	// can only set rumble on active controllers
	if ( pXDevice->userId == INVALID_USER_ID )
		return;

	wNewLeft = XBX_MAX_MOTOR_SPEED * fLeftMotor;
	wNewRight = XBX_MAX_MOTOR_SPEED * fRightMotor;

	if ( wNewLeft > XBX_MAX_MOTOR_SPEED )
		wNewLeft = XBX_MAX_MOTOR_SPEED;

	if ( wNewRight > XBX_MAX_MOTOR_SPEED )
		wNewRight = XBX_MAX_MOTOR_SPEED;

	wOldLeft = pXDevice->vibration.wLeftMotorSpeed;
	wOldRight = pXDevice->vibration.wRightMotorSpeed;

	if ( wNewLeft != wOldLeft || wNewRight != wOldRight )
	{
		pXDevice->vibration.wLeftMotorSpeed = wNewLeft;
		pXDevice->vibration.wRightMotorSpeed = wNewRight;
		pXDevice->pendingRumbleUpdate = true;
	}
	else
	{
		pXDevice->pendingRumbleUpdate = false;
	}
}
