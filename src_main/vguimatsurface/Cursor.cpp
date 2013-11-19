//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: Methods associated with the cursor
//
// $Revision: $
// $NoKeywords: $
//===========================================================================//

#if !defined( _X360 )
#define OEMRESOURCE //for OCR_* cursor junk
#include <windows.h>
#endif
#include "tier0/dbg.h"
#include "tier0/vcrmode.h"
#include "tier1/UtlDict.h"
#include "cursor.h"
#include "vguimatsurface.h"
#include "FileSystem.h"
#if defined( _X360 )
#include "xbox/xbox_win32stubs.h"
#endif

#include "inputsystem/iinputsystem.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace vgui;
static HICON s_pDefaultCursor[20];
static HICON s_hCurrentCursor = NULL;
static bool s_bCursorLocked = false; 
static bool s_bCursorVisible = true;


//-----------------------------------------------------------------------------
// Initializes cursors
//-----------------------------------------------------------------------------
void InitCursors()
{
	// load up all default cursors
	s_pDefaultCursor[dc_none]     = NULL;
	s_pDefaultCursor[dc_arrow]    =(HICON)LoadCursor(NULL, (LPCTSTR)OCR_NORMAL);
	s_pDefaultCursor[dc_ibeam]    =(HICON)LoadCursor(NULL, (LPCTSTR)OCR_IBEAM);
	s_pDefaultCursor[dc_hourglass]=(HICON)LoadCursor(NULL, (LPCTSTR)OCR_WAIT);
	s_pDefaultCursor[dc_crosshair]=(HICON)LoadCursor(NULL, (LPCTSTR)OCR_CROSS);
	s_pDefaultCursor[dc_up]       =(HICON)LoadCursor(NULL, (LPCTSTR)OCR_UP);
	s_pDefaultCursor[dc_sizenwse] =(HICON)LoadCursor(NULL, (LPCTSTR)OCR_SIZENWSE);
	s_pDefaultCursor[dc_sizenesw] =(HICON)LoadCursor(NULL, (LPCTSTR)OCR_SIZENESW);
	s_pDefaultCursor[dc_sizewe]   =(HICON)LoadCursor(NULL, (LPCTSTR)OCR_SIZEWE);
	s_pDefaultCursor[dc_sizens]   =(HICON)LoadCursor(NULL, (LPCTSTR)OCR_SIZENS);
	s_pDefaultCursor[dc_sizeall]  =(HICON)LoadCursor(NULL, (LPCTSTR)OCR_SIZEALL);
	s_pDefaultCursor[dc_no]       =(HICON)LoadCursor(NULL, (LPCTSTR)OCR_NO);
	s_pDefaultCursor[dc_hand]     =(HICON)LoadCursor(NULL, (LPCTSTR)32649);

	s_bCursorLocked = false;
	s_bCursorVisible = true;
	s_hCurrentCursor = s_pDefaultCursor[dc_arrow];
}


#define USER_CURSOR_MASK 0x80000000

//-----------------------------------------------------------------------------
// Purpose: Simple manager for user loaded windows cursors in vgui
//-----------------------------------------------------------------------------
class CUserCursorManager
{
public:
	void Shutdown();
	vgui::HCursor CreateCursorFromFile( char const *curOrAniFile, char const *pPathID );
	bool LookupCursor( vgui::HCursor cursor, HCURSOR& handle );
private:
	CUtlDict< HCURSOR, int >	m_UserCursors;
};

void CUserCursorManager::Shutdown()
{
	for ( int i = m_UserCursors.First() ; i != m_UserCursors.InvalidIndex(); i = m_UserCursors.Next( i ) )
	{
		::DestroyCursor( m_UserCursors[ i ] );
	}
	m_UserCursors.RemoveAll();
}

vgui::HCursor CUserCursorManager::CreateCursorFromFile( char const *curOrAniFile, char const *pPathID )
{
	char fn[ 512 ];
	Q_strncpy( fn, curOrAniFile, sizeof( fn ) );
	Q_strlower( fn );
	Q_FixSlashes( fn );
	
	int cursorIndex = m_UserCursors.Find( fn );
	if ( cursorIndex != m_UserCursors.InvalidIndex() )
	{
		return cursorIndex | USER_CURSOR_MASK;
	}

	g_pFullFileSystem->GetLocalCopy( fn );

	char fullpath[ 512 ];
	g_pFullFileSystem->RelativePathToFullPath( fn, pPathID, fullpath, sizeof( fullpath ) );
	
	HCURSOR newCursor = (HCURSOR)LoadCursorFromFile( fullpath );
	cursorIndex = m_UserCursors.Insert( fn, newCursor );
	return cursorIndex | USER_CURSOR_MASK;
}

bool CUserCursorManager::LookupCursor( vgui::HCursor cursor, HCURSOR& handle )
{
	if ( !( (int)cursor & USER_CURSOR_MASK ) )
	{
		handle = 0;
		return false;
	}

	int cursorIndex = (int)cursor & ~USER_CURSOR_MASK;
	if ( !m_UserCursors.IsValidIndex( cursorIndex ) )
	{
		handle = 0;
		return false;
	}

	handle = m_UserCursors[ cursorIndex ];
	return true;
}

static CUserCursorManager g_UserCursors;

vgui::HCursor Cursor_CreateCursorFromFile( char const *curOrAniFile, char const *pPathID )
{
	return g_UserCursors.CreateCursorFromFile( curOrAniFile, pPathID );
}

void Cursor_ClearUserCursors()
{
	g_UserCursors.Shutdown();
}

//-----------------------------------------------------------------------------
// Selects a cursor
//-----------------------------------------------------------------------------
void CursorSelect(HCursor hCursor)
{
	if (s_bCursorLocked)
		return;

	s_bCursorVisible = true;
	switch (hCursor)
	{
	case dc_user:
	case dc_none:
	case dc_blank:
		s_bCursorVisible = false;
		break;

	case dc_arrow:
	case dc_waitarrow:
	case dc_ibeam:
	case dc_hourglass:
	case dc_crosshair:
	case dc_up:
	case dc_sizenwse:
	case dc_sizenesw:
	case dc_sizewe:
	case dc_sizens:
	case dc_sizeall:
	case dc_no:
	case dc_hand:
		s_hCurrentCursor = s_pDefaultCursor[hCursor];
		break;

	default:
		{
			HCURSOR custom = 0;
			if ( g_UserCursors.LookupCursor( hCursor, custom ) && custom != 0 )
			{
				s_hCurrentCursor = custom;
			}
			else
			{
				s_bCursorVisible = false;
				Assert(0);
			}
		}
		break;
	}

	ActivateCurrentCursor();
}


//-----------------------------------------------------------------------------
// Activates the current cursor
//-----------------------------------------------------------------------------
void ActivateCurrentCursor()
{
	if (s_bCursorVisible)
	{
		::SetCursor(s_hCurrentCursor);
	}
	else
	{
		::SetCursor(NULL);
	}
}


//-----------------------------------------------------------------------------
// Purpose: prevents vgui from changing the cursor
//-----------------------------------------------------------------------------
void LockCursor( bool bEnable )
{
	s_bCursorLocked = bEnable;
	ActivateCurrentCursor();
}


//-----------------------------------------------------------------------------
// Purpose: unlocks the cursor state
//-----------------------------------------------------------------------------
bool IsCursorLocked()
{
	return s_bCursorLocked;
}


//-----------------------------------------------------------------------------
// handles mouse movement
//-----------------------------------------------------------------------------
void CursorSetPos( void *hwnd, int x, int y )
{
	g_pInputSystem->SetCursorPosition( x, y );
}

void CursorGetPos(void *hwnd, int &x, int &y)
{
	POINT pt;

	// Default implementation
	VCRHook_GetCursorPos( &pt );
	VCRHook_ScreenToClient((HWND)hwnd, &pt);
	x = pt.x; y = pt.y;
}






