//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: 
//
// $NoKeywords: $
//
//===========================================================================//
#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H
#ifdef _WIN32
#pragma once
#endif


#if !defined( _X360 )
#include <windows.h>
#include <zmouse.h>
#include "xbox/xboxstubs.h"
#include "../../dx9sdk/include/XInput.h"
#endif

#include "inputsystem/iinputsystem.h"
#include "tier2/tier2.h"

#include "inputsystem/buttoncode.h"
#include "inputsystem/analogcode.h"
#include "bitvec.h"
#include "tier1/utlvector.h"
#include "tier1/utlflags.h"

#if defined( _X360 )
#include "xbox/xbox_win32stubs.h"
#include "xbox/xbox_console.h"
#endif

//-----------------------------------------------------------------------------
// Implementation of the input system
//-----------------------------------------------------------------------------
class CInputSystem : public CTier2AppSystem< IInputSystem >
{
	typedef CTier2AppSystem< IInputSystem > BaseClass;

public:
	// Constructor, destructor
	CInputSystem();
	virtual ~CInputSystem();

	// Inherited from IAppSystem
	virtual	InitReturnVal_t Init();
	virtual void Shutdown();

	// Inherited from IInputSystem
	virtual void AttachToWindow( void* hWnd );
	virtual void DetachFromWindow( );
	virtual void EnableInput( bool bEnable );
	virtual void EnableMessagePump( bool bEnable );
	virtual int GetPollTick() const;
	virtual void PollInputState();
	virtual bool IsButtonDown( ButtonCode_t code ) const;
	virtual int GetButtonPressedTick( ButtonCode_t code ) const;
	virtual int GetButtonReleasedTick( ButtonCode_t code ) const;
	virtual int GetAnalogValue( AnalogCode_t code ) const;
	virtual int GetAnalogDelta( AnalogCode_t code ) const;
	virtual int GetEventCount() const;
	virtual const InputEvent_t* GetEventData( ) const;
	virtual void PostUserEvent( const InputEvent_t &event );
	virtual int GetJoystickCount() const;
	virtual void EnableJoystickInput( int nJoystick, bool bEnable );
	virtual void EnableJoystickDiagonalPOV( int nJoystick, bool bEnable );
	virtual void SampleDevices( void );
	virtual void SetRumble( float fLeftMotor, float fRightMotor, int userId );
	virtual void StopRumble( void );
	virtual void ResetInputState( void );
	virtual void SetPrimaryUserId( int userId );
	virtual const char *ButtonCodeToString( ButtonCode_t code ) const;
	virtual const char *AnalogCodeToString( AnalogCode_t code ) const;
	virtual ButtonCode_t StringToButtonCode( const char *pString ) const;
	virtual AnalogCode_t StringToAnalogCode( const char *pString ) const;
	virtual ButtonCode_t VirtualKeyToButtonCode( int nVirtualKey ) const;
	virtual int ButtonCodeToVirtualKey( ButtonCode_t code ) const;
	virtual ButtonCode_t ScanCodeToButtonCode( int lParam ) const;
	virtual void SleepUntilInput( int nMaxSleepTimeMS );
	virtual int GetPollCount() const;
	virtual void SetCursorPosition( int x, int y );

	// Windows proc
	LRESULT WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

private:
	enum
	{
		STICK1_AXIS_LEFT,
		STICK1_AXIS_RIGHT,
		STICK1_AXIS_DOWN,
		STICK1_AXIS_UP,
		STICK2_AXIS_LEFT,
		STICK2_AXIS_RIGHT,
		STICK2_AXIS_DOWN,
		STICK2_AXIS_UP,
		MAX_STICKAXIS
	};

	// track Xbox stick keys from previous frame
	enum
	{
		LASTKEY_STICK1_X, 
		LASTKEY_STICK1_Y, 
		LASTKEY_STICK2_X, 
		LASTKEY_STICK2_Y, 
		MAX_LASTKEY, 
	};

	enum
	{
		INPUT_STATE_QUEUED = 0,
		INPUT_STATE_CURRENT,

		INPUT_STATE_COUNT,
	};

	struct JoystickInfo_t
	{
		JOYINFOEX m_JoyInfoEx;
		int m_nButtonCount;
		int m_nAxisFlags;
		int m_nDeviceId;
		bool m_bHasPOVControl;
		bool m_bDiagonalPOVControlEnabled;
		unsigned int m_nFlags;
		unsigned int m_nLastPolledButtons;
		unsigned int m_nLastPolledAxisButtons;
		unsigned int m_nLastPolledPOVState;
		unsigned long m_pLastPolledAxes[MAX_JOYSTICK_AXES];
	};

	struct xdevice_t
	{
		int					userId;
		byte				type;			
		byte				subtype;
		word				flags;
		bool				active;
		XINPUT_STATE		states[2];
		int					newState;
		xKey_t				lastStickKeys[MAX_LASTKEY];
		int					stickThreshold[MAX_STICKAXIS];
		float				stickScale[MAX_STICKAXIS];
		int					quitTimeout;
		int					dpadLock;
		// rumble
		XINPUT_VIBRATION	vibration;
		bool				pendingRumbleUpdate;
	};

	struct appKey_t
	{
		int repeats;
		int	sample;
	};

	struct InputState_t
	{
		// Analog states
		CBitVec<BUTTON_CODE_LAST> m_ButtonState;
		int m_ButtonPressedTick[ BUTTON_CODE_LAST ];
		int m_ButtonReleasedTick[ BUTTON_CODE_LAST ];
		int m_pAnalogDelta[ ANALOG_CODE_LAST ];
		int m_pAnalogValue[ ANALOG_CODE_LAST ];
		CUtlVector< InputEvent_t > m_Events;
		bool m_bDirty;
	};

	// Initializes all Xbox controllers
	void InitializeXDevices( void );

	// Opens an Xbox controller
	void OpenXDevice( xdevice_t* pXDevice, int userId );

	// Closes an Xbox controller
	void CloseXDevice( xdevice_t* pXDevice );

	// Samples the Xbox controllers
	void PollXDevices( void );

	// Samples an Xbox controller and queues input events
	void ReadXDevice( xdevice_t* pXDevice );

	// Submits force feedback data to an Xbox controller
	void WriteToXDevice( xdevice_t* pXDevice );

	// Sets rumble values for an Xbox controller
	void SetXDeviceRumble( float fLeftMotor, float fRightMotor, int userId );

	// Posts an Xbox key event, ignoring key repeats 
	void PostXKeyEvent( int nUserId, xKey_t xKey, int nSample );

	// Dispatches all joystick button events through the game's window procs
	void ProcessEvent( UINT uMsg, WPARAM wParam, LPARAM lParam );

	// Initializes joysticks
	void InitializeJoysticks( void );

	// Samples the joystick
	void PollJoystick( void );

	// Update the joystick button state
	void UpdateJoystickButtonState( int nJoystick );

	// Update the joystick POV control
	void UpdateJoystickPOVControl( int nJoystick );

	// Record button state and post the event
	void JoystickButtonEvent( ButtonCode_t button, int sample );

	// Purpose: Get raw joystick sample along axis
	unsigned int AxisValue( JoystickAxis_t axis, JOYINFOEX& ji );

	// Chains the window message to the previous wndproc
	LRESULT ChainWindowMessage( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	// Post an event to the queue
	void PostEvent( int nType, int nTick, int nData = 0, int nData2 = 0, int nData3 = 0 );

	// Deals with app deactivation (sends a bunch of button released messages)
	void ActivateInputSystem( bool bActivated );

	// Determines all mouse button presses
	int ButtonMaskFromMouseWParam( WPARAM wParam, ButtonCode_t code = BUTTON_CODE_INVALID, bool bDown = false ) const;

	// Updates the state of all mouse buttons
	void UpdateMouseButtonState( int nButtonMask, ButtonCode_t dblClickCode = BUTTON_CODE_INVALID );

	// Copies the input state record over
	void CopyInputState( InputState_t *pDest, const InputState_t &src, bool bCopyEvents );

	// Post an button press/release event to the queue
	void PostButtonPressedEvent( InputEventType_t nType, int nTick, ButtonCode_t scanCode, ButtonCode_t virtualCode );
	void PostButtonReleasedEvent( InputEventType_t nType, int nTick, ButtonCode_t scanCode, ButtonCode_t virtualCode );

	// Release all buttons
	void ReleaseAllButtons( int nFirstButton = 0, int nLastButton = BUTTON_CODE_LAST - 1 );

	// Zero analog state
	void ZeroAnalogState( int nFirstState, int nLastState );

	// Converts xbox keys to button codes
	ButtonCode_t XKeyToButtonCode( int nUserId, int nXKey ) const;
	
	// Computes the sample tick
	int ComputeSampleTick();

	// Clears the input state, doesn't generate key-up messages
	void ClearInputState();

	WNDPROC m_ChainedWndProc;
	HWND m_hAttachedHWnd;
	bool m_bEnabled;
	bool m_bPumpEnabled;
	bool m_bIsPolling;

	// Current button state
	InputState_t m_InputState[INPUT_STATE_COUNT];

	DWORD m_StartupTimeTick;
	int m_nLastPollTick;
	int m_nLastSampleTick;
	int m_nPollCount;

	// Mouse wheel hack
	UINT m_uiMouseWheel;

	// Joystick info
	CUtlFlags<unsigned short> m_JoysticksEnabled;
	int m_nJoystickCount;
	bool m_bXController;
	JoystickInfo_t m_pJoystickInfo[ MAX_JOYSTICKS ];

	// Xbox controller info
	appKey_t	m_appXKeys[ XUSER_MAX_COUNT ][ XK_MAX_KEYS ];
	xdevice_t	m_XDevices[ XUSER_MAX_COUNT ];
	int			m_PrimaryUserId;

	// For the 'SleepUntilInput' feature
	HANDLE m_hEvent;

	CSysModule   *m_pXInputDLL;
};

#endif // INPUTSYSTEM_H