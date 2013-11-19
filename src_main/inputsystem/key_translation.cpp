//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: 
//
//===========================================================================//


#if !defined( _X360 )
#include <wtypes.h>
#include <winuser.h>
#include "xbox/xboxstubs.h"
#else
#include "xbox/xbox_win32stubs.h"
#endif

#include "key_translation.h"
#include "tier1/convar.h"
#include "tier1/strtools.h"
#include "tier0/dbg.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

static ButtonCode_t s_pVirtualKeyToButtonCode[256];
static ButtonCode_t s_pXKeyTrans[XK_MAX_KEYS];
static int s_pButtonCodeToVirtual[BUTTON_CODE_LAST];

static const char *s_pButtonCodeName[ ] =
{
	"",				// KEY_NONE
	"0",			// KEY_0,
	"1",			// KEY_1,
	"2",			// KEY_2,
	"3",			// KEY_3,
	"4",			// KEY_4,
	"5",			// KEY_5,
	"6",			// KEY_6,
	"7",			// KEY_7,
	"8",			// KEY_8,
	"9",			// KEY_9,
	"a",			// KEY_A,
	"b",			// KEY_B,
	"c",			// KEY_C,
	"d",			// KEY_D,
	"e",			// KEY_E,
	"f",			// KEY_F,
	"g",			// KEY_G,
	"h",			// KEY_H,
	"i",			// KEY_I,
	"j",			// KEY_J,
	"k",			// KEY_K,
	"l",			// KEY_L,
	"m",			// KEY_M,
	"n",			// KEY_N,
	"o",			// KEY_O,
	"p",			// KEY_P,
	"q",			// KEY_Q,
	"r",			// KEY_R,
	"s",			// KEY_S,
	"t",			// KEY_T,
	"u",			// KEY_U,
	"v",			// KEY_V,
	"w",			// KEY_W,
	"x",			// KEY_X,
	"y",			// KEY_Y,
	"z",			// KEY_Z,
	"KP_INS",		// KEY_PAD_0,
	"KP_END",		// KEY_PAD_1,
	"KP_DOWNARROW",	// KEY_PAD_2,
	"KP_PGDN",		// KEY_PAD_3,
	"KP_LEFTARROW",	// KEY_PAD_4,
	"KP_5",			// KEY_PAD_5,
	"KP_RIGHTARROW",// KEY_PAD_6,
	"KP_HOME",		// KEY_PAD_7,
	"KP_UPARROW",	// KEY_PAD_8,
	"KP_PGUP",		// KEY_PAD_9,
	"KP_SLASH",		// KEY_PAD_DIVIDE,
	"KP_MULTIPLY",	// KEY_PAD_MULTIPLY,
	"KP_MINUS",		// KEY_PAD_MINUS,
	"KP_PLUS",		// KEY_PAD_PLUS,
	"KP_ENTER",		// KEY_PAD_ENTER,
	"KP_DEL",		// KEY_PAD_DECIMAL,
	"[",			// KEY_LBRACKET,
	"]",			// KEY_RBRACKET,
	"SEMICOLON",	// KEY_SEMICOLON,
	"'",			// KEY_APOSTROPHE,
	"`",			// KEY_BACKQUOTE,
	",",			// KEY_COMMA,
	".",			// KEY_PERIOD,
	"/",			// KEY_SLASH,
	"\\",			// KEY_BACKSLASH,
	"-",			// KEY_MINUS,
	"=",			// KEY_EQUAL,
	"ENTER",		// KEY_ENTER,
	"SPACE",		// KEY_SPACE,
	"BACKSPACE",	// KEY_BACKSPACE,
	"TAB",			// KEY_TAB,
	"CAPSLOCK",		// KEY_CAPSLOCK,
	"NUMLOCK",		// KEY_NUMLOCK,
	"ESCAPE",		// KEY_ESCAPE,
	"SCROLLLOCK",	// KEY_SCROLLLOCK,
	"INS",			// KEY_INSERT,
	"DEL",			// KEY_DELETE,
	"HOME",			// KEY_HOME,
	"END",			// KEY_END,
	"PGUP",			// KEY_PAGEUP,
	"PGDN",			// KEY_PAGEDOWN,
	"PAUSE",		// KEY_BREAK,
	"SHIFT",		// KEY_LSHIFT,
	"RSHIFT",		// KEY_RSHIFT,
	"ALT",			// KEY_LALT,
	"RALT",			// KEY_RALT,
	"CTRL",			// KEY_LCONTROL,
	"RCTRL",		// KEY_RCONTROL,
	"LWIN",			// KEY_LWIN,
	"RWIN",			// KEY_RWIN,
	"APP",			// KEY_APP,
	"UPARROW",		// KEY_UP,
	"LEFTARROW",	// KEY_LEFT,
	"DOWNARROW",	// KEY_DOWN,
	"RIGHTARROW",	// KEY_RIGHT,
	"F1",			// KEY_F1,
	"F2",			// KEY_F2,
	"F3",			// KEY_F3,
	"F4",			// KEY_F4,
	"F5",			// KEY_F5,
	"F6",			// KEY_F6,
	"F7",			// KEY_F7,
	"F8",			// KEY_F8,
	"F9",			// KEY_F9,
	"F10",			// KEY_F10,
	"F11",			// KEY_F11,
	"F12",			// KEY_F12,

	// FIXME: CAPSLOCK/NUMLOCK/SCROLLLOCK all appear above. What are these for?!
	// They only appear in CInputWin32::UpdateToggleButtonState in vgui2
	"CAPSLOCKTOGGLE",	// KEY_CAPSLOCKTOGGLE,
	"NUMLOCKTOGGLE",	// KEY_NUMLOCKTOGGLE,
	"SCROLLLOCKTOGGLE", // KEY_SCROLLLOCKTOGGLE,

	// Mouse
	"MOUSE1",		// MOUSE_LEFT,
	"MOUSE2",		// MOUSE_RIGHT,
	"MOUSE3",		// MOUSE_MIDDLE,
	"MOUSE4",		// MOUSE_4,
	"MOUSE5",		// MOUSE_5,

	"MWHEELUP",		// MOUSE_WHEEL_UP
	"MWHEELDOWN",	// MOUSE_WHEEL_DOWN

#if defined ( _X360 )
	"A_BUTTON",		// JOYSTICK_FIRST_BUTTON		
	"B_BUTTON",		
	"X_BUTTON",		
	"Y_BUTTON",		
	"L_SHOULDER",		
	"R_SHOULDER",		
	"BACK",		
	"START",
	"STICK1",		
	"STICK2",		
	"JOY11",		
	"JOY12",		
	"JOY13",		
	"JOY14",		
	"JOY15",		
	"JOY16",		
	"JOY17",	
	"JOY18",		
	"JOY19",		
	"JOY20",	
	"JOY21",	
	"JOY22",	
	"JOY23",		
	"JOY24",		
	"JOY25",		
	"JOY26",		
	"JOY27",	
	"JOY28",	
	"JOY29",	
	"JOY30",		
	"JOY31",		
	"JOY32",		// JOYSTICK_LAST_BUTTON

	"UP",			// JOYSTICK_FIRST_POV_BUTTON
	"RIGHT",		
	"DOWN",		
	"LEFT",			// JOYSTICK_LAST_POV_BUTTON

	"S1_RIGHT",		// JOYSTICK_FIRST_AXIS_BUTTON
	"S1_LEFT",		
	"S1_DOWN",
	"S1_UP",		
	"L_TRIGGER",		
	"R_TRIGGER",
	"S2_RIGHT",
	"S2_LEFT",		
	"S2_DOWN",
	"S2_UP",		// JOYSTICK_LAST_AXIS_BUTTON
	"V AXIS POS",
	"V AXIS NEG",		
#else
	"JOY1",			// JOYSTICK_FIRST_BUTTON
	"JOY2",		
	"JOY3",		
	"JOY4",		
	"JOY5",		
	"JOY6",		
	"JOY7",		
	"JOY8",		
	"JOY9",		
	"JOY10",		
	"JOY11",		
	"JOY12",		
	"JOY13",		
	"JOY14",		
	"JOY15",		
	"JOY16",		
	"JOY17",	
	"JOY18",		
	"JOY19",		
	"JOY20",	
	"JOY21",	
	"JOY22",	
	"JOY23",		
	"JOY24",		
	"JOY25",		
	"JOY26",		
	"JOY27",	
	"JOY28",	
	"JOY29",	
	"JOY30",		
	"JOY31",		
	"JOY32",		// JOYSTICK_LAST_BUTTON

	"POV_UP",		// JOYSTICK_FIRST_POV_BUTTON
	"POV_RIGHT",		
	"POV_DOWN",		
	"POV_LEFT",		// JOYSTICK_LAST_POV_BUTTON

	"X AXIS POS",	// JOYSTICK_FIRST_AXIS_BUTTON
	"X AXIS NEG",		
	"Y AXIS POS",
	"Y AXIS NEG",		
	"Z AXIS POS",
	"Z AXIS NEG",		
	"R AXIS POS",
	"R AXIS NEG",		
	"U AXIS POS",
	"U AXIS NEG",		
	"V AXIS POS",
	"V AXIS NEG",	// JOYSTICK_LAST_AXIS_BUTTON
#endif
};

static const char *s_pAnalogCodeName[ ] =
{
	"MOUSE_X",		// MOUSE_X = 0,
	"MOUSE_Y",		// MOUSE_Y,
	"MOUSE_XY",		// MOUSE_XY,		// Invoked when either x or y changes
	"MOUSE_WHEEL",	// MOUSE_WHEEL,

	"X AXIS",		// JOY_AXIS_X
	"Y AXIS",		// JOY_AXIS_Y
	"Z AXIS",		// JOY_AXIS_Z
	"R AXIS",		// JOY_AXIS_R
	"U AXIS",		// JOY_AXIS_U
	"V AXIS",		// JOY_AXIS_V
};

#if !defined ( _X360 )
static const char *s_pXControllerButtonCodeNames[ ] =
{
	"A_BUTTON",		// JOYSTICK_FIRST_BUTTON		
	"B_BUTTON",		
	"X_BUTTON",		
	"Y_BUTTON",		
	"L_SHOULDER",		
	"R_SHOULDER",		
	"BACK",		
	"START",
	"STICK1",		
	"STICK2",		
	"JOY11",		
	"JOY12",		
	"JOY13",		
	"JOY14",		
	"JOY15",		
	"JOY16",		
	"JOY17",	
	"JOY18",		
	"JOY19",		
	"JOY20",	
	"JOY21",	
	"JOY22",	
	"JOY23",		
	"JOY24",		
	"JOY25",		
	"JOY26",		
	"JOY27",	
	"JOY28",	
	"JOY29",	
	"JOY30",		
	"JOY31",		
	"JOY32",		// JOYSTICK_LAST_BUTTON

	"UP",			// JOYSTICK_FIRST_POV_BUTTON
	"RIGHT",		
	"DOWN",		
	"LEFT",			// JOYSTICK_LAST_POV_BUTTON

	"S1_RIGHT",		// JOYSTICK_FIRST_AXIS_BUTTON
	"S1_LEFT",		
	"S1_DOWN",
	"S1_UP",		
	"L_TRIGGER",		
	"R_TRIGGER",
	"S2_RIGHT",
	"S2_LEFT",		
	"S2_DOWN",
	"S2_UP",		// JOYSTICK_LAST_AXIS_BUTTON
	"V AXIS POS",
	"V AXIS NEG",		
};
#endif

// this maps non-translated keyboard scan codes to engine key codes
// Google for 'Keyboard Scan Code Specification'
static ButtonCode_t s_pScanToButtonCode_QWERTY[128] = 
{ 
	//	0				1				2				3				4				5				6				7 
	//	8				9				A				B				C				D				E				F 
	KEY_NONE,		KEY_ESCAPE,		KEY_1,			KEY_2,			KEY_3,			KEY_4,			KEY_5,			KEY_6,			// 0
	KEY_7,			KEY_8,			KEY_9,			KEY_0,			KEY_MINUS,		KEY_EQUAL,		KEY_BACKSPACE,	KEY_TAB,		// 0 

	KEY_Q,			KEY_W,			KEY_E,			KEY_R,			KEY_T,			KEY_Y,			KEY_U,			KEY_I,			// 1
	KEY_O,			KEY_P,			KEY_LBRACKET,	KEY_RBRACKET,	KEY_ENTER,		KEY_LCONTROL,	KEY_A,			KEY_S,			// 1 

	KEY_D,			KEY_F,			KEY_G,			KEY_H,			KEY_J,			KEY_K,			KEY_L,			KEY_SEMICOLON,	// 2 
	KEY_APOSTROPHE,	KEY_BACKQUOTE,	KEY_LSHIFT,		KEY_BACKSLASH,	KEY_Z,			KEY_X,			KEY_C,			KEY_V,			// 2 

	KEY_B,			KEY_N,			KEY_M,			KEY_COMMA,		KEY_PERIOD,		KEY_SLASH,		KEY_RSHIFT,		KEY_PAD_MULTIPLY,// 3
	KEY_LALT,		KEY_SPACE,		KEY_CAPSLOCK,	KEY_F1,			KEY_F2,			KEY_F3,			KEY_F4,			KEY_F5,			// 3 

	KEY_F6,			KEY_F7,			KEY_F8,			KEY_F9,			KEY_F10,		KEY_NUMLOCK,	KEY_SCROLLLOCK,	KEY_HOME,		// 4
	KEY_UP,			KEY_PAGEUP,		KEY_PAD_MINUS,	KEY_LEFT,		KEY_PAD_5,		KEY_RIGHT,		KEY_PAD_PLUS,	KEY_END,		// 4 

	KEY_DOWN,		KEY_PAGEDOWN,	KEY_INSERT,		KEY_DELETE,		KEY_NONE,		KEY_NONE,		KEY_NONE,		KEY_F11,		// 5
	KEY_F12,		KEY_BREAK,		KEY_NONE,		KEY_NONE,		KEY_NONE,		KEY_NONE,		KEY_NONE,		KEY_NONE,		// 5

	KEY_NONE,		KEY_NONE,		KEY_NONE,		KEY_NONE,		KEY_NONE,		KEY_NONE,		KEY_NONE,		KEY_NONE,		// 6
	KEY_NONE,		KEY_NONE,		KEY_NONE,		KEY_NONE,		KEY_NONE,		KEY_NONE,		KEY_NONE,		KEY_NONE,		// 6 

	KEY_NONE,		KEY_NONE,		KEY_NONE,		KEY_NONE,		KEY_NONE,		KEY_NONE,		KEY_NONE,		KEY_NONE,		// 7
	KEY_NONE,		KEY_NONE,		KEY_NONE,		KEY_NONE,		KEY_NONE,		KEY_NONE,		KEY_NONE,		KEY_NONE		// 7 
};

static ButtonCode_t s_pScanToButtonCode[128];


void ButtonCode_InitKeyTranslationTable()
{
	COMPILE_TIME_ASSERT( sizeof(s_pButtonCodeName) / sizeof( const char * ) == BUTTON_CODE_LAST );
	COMPILE_TIME_ASSERT( sizeof(s_pAnalogCodeName) / sizeof( const char * ) == ANALOG_CODE_LAST );

	// set virtual key translation table
	memset( s_pVirtualKeyToButtonCode, KEY_NONE, sizeof(s_pVirtualKeyToButtonCode) );

	s_pVirtualKeyToButtonCode['0']			=KEY_0;
	s_pVirtualKeyToButtonCode['1']			=KEY_1;
	s_pVirtualKeyToButtonCode['2']			=KEY_2;
	s_pVirtualKeyToButtonCode['3']			=KEY_3;
	s_pVirtualKeyToButtonCode['4']			=KEY_4;
	s_pVirtualKeyToButtonCode['5']			=KEY_5;
	s_pVirtualKeyToButtonCode['6']			=KEY_6;
	s_pVirtualKeyToButtonCode['7']			=KEY_7;
	s_pVirtualKeyToButtonCode['8']			=KEY_8;
	s_pVirtualKeyToButtonCode['9']			=KEY_9;
	s_pVirtualKeyToButtonCode['A']			=KEY_A;
	s_pVirtualKeyToButtonCode['B'] 			=KEY_B;
	s_pVirtualKeyToButtonCode['C'] 			=KEY_C;
	s_pVirtualKeyToButtonCode['D'] 			=KEY_D;
	s_pVirtualKeyToButtonCode['E']			=KEY_E;
	s_pVirtualKeyToButtonCode['F']			=KEY_F;
	s_pVirtualKeyToButtonCode['G']			=KEY_G;
	s_pVirtualKeyToButtonCode['H'] 			=KEY_H;
	s_pVirtualKeyToButtonCode['I']			=KEY_I;
	s_pVirtualKeyToButtonCode['J']			=KEY_J;
	s_pVirtualKeyToButtonCode['K']			=KEY_K;
	s_pVirtualKeyToButtonCode['L']			=KEY_L;
	s_pVirtualKeyToButtonCode['M']			=KEY_M;
	s_pVirtualKeyToButtonCode['N']			=KEY_N;
	s_pVirtualKeyToButtonCode['O']			=KEY_O;
	s_pVirtualKeyToButtonCode['P']			=KEY_P;
	s_pVirtualKeyToButtonCode['Q']			=KEY_Q;
	s_pVirtualKeyToButtonCode['R']			=KEY_R;
	s_pVirtualKeyToButtonCode['S']			=KEY_S;
	s_pVirtualKeyToButtonCode['T']			=KEY_T;
	s_pVirtualKeyToButtonCode['U']			=KEY_U;
	s_pVirtualKeyToButtonCode['V']			=KEY_V;
	s_pVirtualKeyToButtonCode['W']			=KEY_W;
	s_pVirtualKeyToButtonCode['X']			=KEY_X;
	s_pVirtualKeyToButtonCode['Y']			=KEY_Y;
	s_pVirtualKeyToButtonCode['Z']			=KEY_Z;
	s_pVirtualKeyToButtonCode[VK_NUMPAD0]	=KEY_PAD_0;
	s_pVirtualKeyToButtonCode[VK_NUMPAD1]	=KEY_PAD_1;
	s_pVirtualKeyToButtonCode[VK_NUMPAD2]	=KEY_PAD_2;
	s_pVirtualKeyToButtonCode[VK_NUMPAD3]	=KEY_PAD_3;
	s_pVirtualKeyToButtonCode[VK_NUMPAD4]	=KEY_PAD_4;
	s_pVirtualKeyToButtonCode[VK_NUMPAD5]	=KEY_PAD_5;
	s_pVirtualKeyToButtonCode[VK_NUMPAD6]	=KEY_PAD_6;
	s_pVirtualKeyToButtonCode[VK_NUMPAD7]	=KEY_PAD_7;
	s_pVirtualKeyToButtonCode[VK_NUMPAD8]	=KEY_PAD_8;
	s_pVirtualKeyToButtonCode[VK_NUMPAD9]	=KEY_PAD_9;
	s_pVirtualKeyToButtonCode[VK_DIVIDE]	=KEY_PAD_DIVIDE;
	s_pVirtualKeyToButtonCode[VK_MULTIPLY]	=KEY_PAD_MULTIPLY;
	s_pVirtualKeyToButtonCode[VK_SUBTRACT]	=KEY_PAD_MINUS;
	s_pVirtualKeyToButtonCode[VK_ADD]		=KEY_PAD_PLUS;
	s_pVirtualKeyToButtonCode[VK_RETURN]	=KEY_PAD_ENTER;
	s_pVirtualKeyToButtonCode[VK_DECIMAL]	=KEY_PAD_DECIMAL;
	s_pVirtualKeyToButtonCode[0xdb]			=KEY_LBRACKET;
	s_pVirtualKeyToButtonCode[0xdd]			=KEY_RBRACKET;
	s_pVirtualKeyToButtonCode[0xba]			=KEY_SEMICOLON;
	s_pVirtualKeyToButtonCode[0xde]			=KEY_APOSTROPHE;
	s_pVirtualKeyToButtonCode[0xc0]			=KEY_BACKQUOTE;
	s_pVirtualKeyToButtonCode[0xbc]			=KEY_COMMA;
	s_pVirtualKeyToButtonCode[0xbe]			=KEY_PERIOD;
	s_pVirtualKeyToButtonCode[0xbf]			=KEY_SLASH;
	s_pVirtualKeyToButtonCode[0xdc]			=KEY_BACKSLASH;
	s_pVirtualKeyToButtonCode[0xbd]			=KEY_MINUS;
	s_pVirtualKeyToButtonCode[0xbb]			=KEY_EQUAL;
	s_pVirtualKeyToButtonCode[VK_RETURN]	=KEY_ENTER;
	s_pVirtualKeyToButtonCode[VK_SPACE]		=KEY_SPACE;
	s_pVirtualKeyToButtonCode[VK_BACK]		=KEY_BACKSPACE;
	s_pVirtualKeyToButtonCode[VK_TAB]		=KEY_TAB;
	s_pVirtualKeyToButtonCode[VK_CAPITAL]	=KEY_CAPSLOCK;
	s_pVirtualKeyToButtonCode[VK_NUMLOCK]	=KEY_NUMLOCK;
	s_pVirtualKeyToButtonCode[VK_ESCAPE]	=KEY_ESCAPE;
	s_pVirtualKeyToButtonCode[VK_SCROLL]	=KEY_SCROLLLOCK;
	s_pVirtualKeyToButtonCode[VK_INSERT]	=KEY_INSERT;
	s_pVirtualKeyToButtonCode[VK_DELETE]	=KEY_DELETE;
	s_pVirtualKeyToButtonCode[VK_HOME]		=KEY_HOME;
	s_pVirtualKeyToButtonCode[VK_END]		=KEY_END;
	s_pVirtualKeyToButtonCode[VK_PRIOR]		=KEY_PAGEUP;
	s_pVirtualKeyToButtonCode[VK_NEXT]		=KEY_PAGEDOWN;
	s_pVirtualKeyToButtonCode[VK_PAUSE]		=KEY_BREAK;
	s_pVirtualKeyToButtonCode[VK_SHIFT]		=KEY_RSHIFT;
	s_pVirtualKeyToButtonCode[VK_SHIFT]		=KEY_LSHIFT;	// SHIFT -> left SHIFT
	s_pVirtualKeyToButtonCode[VK_MENU]		=KEY_RALT;
	s_pVirtualKeyToButtonCode[VK_MENU]		=KEY_LALT;		// ALT -> left ALT
	s_pVirtualKeyToButtonCode[VK_CONTROL]	=KEY_RCONTROL;
	s_pVirtualKeyToButtonCode[VK_CONTROL]	=KEY_LCONTROL;	// CTRL -> left CTRL
	s_pVirtualKeyToButtonCode[VK_LWIN]		=KEY_LWIN;
	s_pVirtualKeyToButtonCode[VK_RWIN]		=KEY_RWIN;
	s_pVirtualKeyToButtonCode[VK_APPS]		=KEY_APP;
	s_pVirtualKeyToButtonCode[VK_UP]		=KEY_UP;
	s_pVirtualKeyToButtonCode[VK_LEFT]		=KEY_LEFT;
	s_pVirtualKeyToButtonCode[VK_DOWN]		=KEY_DOWN;
	s_pVirtualKeyToButtonCode[VK_RIGHT]		=KEY_RIGHT;	
	s_pVirtualKeyToButtonCode[VK_F1]		=KEY_F1;
	s_pVirtualKeyToButtonCode[VK_F2]		=KEY_F2;
	s_pVirtualKeyToButtonCode[VK_F3]		=KEY_F3;
	s_pVirtualKeyToButtonCode[VK_F4]		=KEY_F4;
	s_pVirtualKeyToButtonCode[VK_F5]		=KEY_F5;
	s_pVirtualKeyToButtonCode[VK_F6]		=KEY_F6;
	s_pVirtualKeyToButtonCode[VK_F7]		=KEY_F7;
	s_pVirtualKeyToButtonCode[VK_F8]		=KEY_F8;
	s_pVirtualKeyToButtonCode[VK_F9]		=KEY_F9;
	s_pVirtualKeyToButtonCode[VK_F10]		=KEY_F10;
	s_pVirtualKeyToButtonCode[VK_F11]		=KEY_F11;
	s_pVirtualKeyToButtonCode[VK_F12]		=KEY_F12;

	// init the xkey translation table
	s_pXKeyTrans[XK_NULL]					= KEY_NONE;
	s_pXKeyTrans[XK_BUTTON_UP]				= KEY_XBUTTON_UP;
	s_pXKeyTrans[XK_BUTTON_DOWN]			= KEY_XBUTTON_DOWN;
	s_pXKeyTrans[XK_BUTTON_LEFT]			= KEY_XBUTTON_LEFT;
	s_pXKeyTrans[XK_BUTTON_RIGHT]			= KEY_XBUTTON_RIGHT;
	s_pXKeyTrans[XK_BUTTON_START]			= KEY_XBUTTON_START;
	s_pXKeyTrans[XK_BUTTON_BACK]			= KEY_XBUTTON_BACK;
	s_pXKeyTrans[XK_BUTTON_STICK1]			= KEY_XBUTTON_STICK1;
	s_pXKeyTrans[XK_BUTTON_STICK2]			= KEY_XBUTTON_STICK2;
	s_pXKeyTrans[XK_BUTTON_A]				= KEY_XBUTTON_A;
	s_pXKeyTrans[XK_BUTTON_B]				= KEY_XBUTTON_B;
	s_pXKeyTrans[XK_BUTTON_X]				= KEY_XBUTTON_X;
	s_pXKeyTrans[XK_BUTTON_Y]				= KEY_XBUTTON_Y;
	s_pXKeyTrans[XK_BUTTON_LEFT_SHOULDER]   = KEY_XBUTTON_LEFT_SHOULDER;
	s_pXKeyTrans[XK_BUTTON_RIGHT_SHOULDER]	= KEY_XBUTTON_RIGHT_SHOULDER;
	s_pXKeyTrans[XK_BUTTON_LTRIGGER]		= KEY_XBUTTON_LTRIGGER;
	s_pXKeyTrans[XK_BUTTON_RTRIGGER]		= KEY_XBUTTON_RTRIGGER;
	s_pXKeyTrans[XK_STICK1_UP]				= KEY_XSTICK1_UP;
	s_pXKeyTrans[XK_STICK1_DOWN]			= KEY_XSTICK1_DOWN;
	s_pXKeyTrans[XK_STICK1_LEFT]			= KEY_XSTICK1_LEFT;
	s_pXKeyTrans[XK_STICK1_RIGHT]			= KEY_XSTICK1_RIGHT;
	s_pXKeyTrans[XK_STICK2_UP]				= KEY_XSTICK2_UP;
	s_pXKeyTrans[XK_STICK2_DOWN]			= KEY_XSTICK2_DOWN;
	s_pXKeyTrans[XK_STICK2_LEFT]			= KEY_XSTICK2_LEFT;
	s_pXKeyTrans[XK_STICK2_RIGHT]			= KEY_XSTICK2_RIGHT;

	// create reverse table engine to virtual
	for ( int i = 0; i < BUTTON_CODE_LAST; i++ )
	{
		s_pButtonCodeToVirtual[ s_pVirtualKeyToButtonCode[i] ] = i;
	}

	s_pButtonCodeToVirtual[0] = 0;
}

ButtonCode_t ButtonCode_VirtualKeyToButtonCode( int keyCode )
{
	if ( keyCode < 0 || keyCode >= sizeof( s_pVirtualKeyToButtonCode ) / sizeof( s_pVirtualKeyToButtonCode[0] ) )
	{
		Assert( false );
		return KEY_NONE;
	}
	return s_pVirtualKeyToButtonCode[keyCode];
}

int ButtonCode_ButtonCodeToVirtualKey( ButtonCode_t code )
{
	return s_pButtonCodeToVirtual[code];
}

ButtonCode_t ButtonCode_XKeyToButtonCode( int nPort, int keyCode )
{
	if ( keyCode < 0 || keyCode >= sizeof( s_pXKeyTrans ) / sizeof( s_pXKeyTrans[0] ) )
	{
		Assert( false );
		return KEY_NONE;
	}

	ButtonCode_t code = s_pXKeyTrans[keyCode];
	if ( IsJoystickButtonCode( code ) )
	{
		int nOffset = code - JOYSTICK_FIRST_BUTTON;
		return JOYSTICK_BUTTON( nPort, nOffset );
	}
	
	if ( IsJoystickPOVCode( code ) )
	{
		int nOffset = code - JOYSTICK_FIRST_POV_BUTTON;
		return JOYSTICK_POV_BUTTON( nPort, nOffset );
	}
	
	if ( IsJoystickAxisCode( code ) )
	{
		int nOffset = code - JOYSTICK_FIRST_AXIS_BUTTON;
		return JOYSTICK_AXIS_BUTTON( nPort, nOffset );
	}

	return code;
}

// Convert back + forth between ButtonCode/AnalogCode + strings
const char *ButtonCode_ButtonCodeToString( ButtonCode_t code, bool bXController )
{
#if !defined ( _X360 )
	if ( bXController && code >= JOYSTICK_FIRST_BUTTON && code <= JOYSTICK_LAST_AXIS_BUTTON )
		return s_pXControllerButtonCodeNames[ code - JOYSTICK_FIRST_BUTTON ];
#endif

	return s_pButtonCodeName[ code ];
}

const char *AnalogCode_AnalogCodeToString( AnalogCode_t code )
{
	return s_pAnalogCodeName[ code ];
}

ButtonCode_t ButtonCode_StringToButtonCode( const char *pString, bool bXController )
{
	if ( !pString || !pString[0] )
		return BUTTON_CODE_INVALID;

	// Backward compat for screwed up previous joystick button names
	if ( !Q_strnicmp( pString, "aux", 3 ) )
	{
		int nIndex = atoi( &pString[3] );
		if ( nIndex < 29 )
			return JOYSTICK_BUTTON( 0, nIndex );
		if ( ( nIndex >= 29 ) && ( nIndex <= 32 ) )
			return JOYSTICK_POV_BUTTON( 0, nIndex - 29 );
		return BUTTON_CODE_INVALID;
	}

	for ( int i = 0; i < BUTTON_CODE_LAST; ++i )
	{
		if ( !Q_stricmp( s_pButtonCodeName[i], pString ) )
			return (ButtonCode_t)i;
	}

#if !defined ( _X360 )
	if ( bXController )
	{
		for ( int i = 0; i < ARRAYSIZE(s_pXControllerButtonCodeNames); ++i )
		{
			if ( !Q_stricmp( s_pXControllerButtonCodeNames[i], pString ) )
				return (ButtonCode_t)(JOYSTICK_FIRST_BUTTON + i);
		}
	}
#endif

	return BUTTON_CODE_INVALID;
}

AnalogCode_t AnalogCode_StringToAnalogCode( const char *pString )
{
	if ( !pString || !pString[0] )
		return ANALOG_CODE_INVALID;

	for ( int i = 0; i < ANALOG_CODE_LAST; ++i )
	{
		if ( !Q_stricmp( s_pAnalogCodeName[i], pString ) )
			return (AnalogCode_t)i;
	}

	return ANALOG_CODE_INVALID;
}

ButtonCode_t ButtonCode_ScanCodeToButtonCode( int lParam )
{
	int nScanCode = ( lParam >> 16 ) & 0xFF;
	if ( nScanCode > 127 )
		return KEY_NONE;

	ButtonCode_t result = s_pScanToButtonCode[nScanCode];

	bool bIsExtended = ( lParam & ( 1 << 24 ) ) != 0;
	if ( !bIsExtended )
	{
		switch ( result )
		{
		case KEY_HOME:
			return KEY_PAD_7;
		case KEY_UP:
			return KEY_PAD_8;
		case KEY_PAGEUP:
			return KEY_PAD_9;
		case KEY_LEFT:
			return KEY_PAD_4;
		case KEY_RIGHT:
			return KEY_PAD_6;
		case KEY_END:
			return KEY_PAD_1;
		case KEY_DOWN:
			return KEY_PAD_2;
		case KEY_PAGEDOWN:
			return KEY_PAD_3;
		case KEY_INSERT:
			return KEY_PAD_0;
		case KEY_DELETE:
			return KEY_PAD_DECIMAL;
		default:
			break;
		}
	}
	else
	{
		switch ( result )
		{
		case KEY_ENTER:
			return KEY_PAD_ENTER;
		case KEY_LALT:
			return KEY_RALT;
		case KEY_LCONTROL:
			return KEY_RCONTROL;
		case KEY_SLASH:
			return KEY_PAD_DIVIDE;
		case KEY_CAPSLOCK:
			return KEY_PAD_PLUS;
		}
	}

	return result;
}


//-----------------------------------------------------------------------------
// Update scan codes for foreign keyboards
//-----------------------------------------------------------------------------
void ButtonCode_UpdateScanCodeLayout( )
{
	// reset the keyboard
	memcpy( s_pScanToButtonCode, s_pScanToButtonCode_QWERTY, sizeof(s_pScanToButtonCode) );

#if !defined( _X360 )
	// fix up keyboard layout for other languages
	HKL currentKb = ::GetKeyboardLayout( 0 );
	HKL englishKb = ::LoadKeyboardLayout("00000409", 0);

	if (englishKb && englishKb != currentKb)
	{
		for ( int i = 0; i < ARRAYSIZE(s_pScanToButtonCode); i++ )
		{
			// take the english/QWERTY
			ButtonCode_t code = s_pScanToButtonCode_QWERTY[ i ];

			// only remap printable keys
			if ( code != KEY_NONE && code != KEY_BACKQUOTE && ( IsAlphaNumeric( code ) || IsPunctuation( code ) ) )
			{
				// get it's virtual key based on the old layout
				int vk = ::MapVirtualKeyEx( i, 1, englishKb );

				// turn in into a scancode on the new layout
				int newScanCode = ::MapVirtualKeyEx( vk, 0, currentKb );

				// strip off any high bits
				newScanCode &= 0x0000007F;

				// set in the new layout
				s_pScanToButtonCode[newScanCode] = code;
			}
		}
	}

	s_pScanToButtonCode[0] = KEY_NONE;
#endif
}


//-----------------------------------------------------------------------------
// Purpose: Updates the current keyboard layout
//-----------------------------------------------------------------------------
CON_COMMAND( key_updatelayout, "Updates game keyboard layout to current windows keyboard setting." )
{
	ButtonCode_UpdateScanCodeLayout();
}
