//===== Copyright © 1996-2006, Valve Corporation, All rights reserved. ======//
//
// Base class for windows that draw vgui in Maya
//
//===========================================================================//

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows NT 4 or later.
#define _WIN32_WINNT 0x0501	// This means target Windows XP or later. (Subclassing windows requires XP)
#endif						

#include "vsvguiwindow.h"
#include "valvemaya.h"

#include <windows.h>
#include <commctrl.h>
#include "tier0/dbg.h"
#include "tier3/tier3.h"
#include "materialsystem/IMaterialSystem.h"
#include "vgui/IVGui.h"
#include "vgui/ISurface.h"
#include "vgui/ISystem.h"
#include "vgui_controls/phandle.h"
#include "vgui_controls/EditablePanel.h"
#include "vgui_controls/AnimationController.h"
#include "vgui_controls/phandle.h"
#include "VGuiMatSurface/IMatSystemSurface.h"
#include "imayavgui.h"
#include "inputsystem/iinputsystem.h"
#include "maya/mtimermessage.h"


//-----------------------------------------------------------------------------
// Name to look for when attaching to a window
//-----------------------------------------------------------------------------
#define VGUI_WINDOW_NAME	"vsVGuiWindow"


//-----------------------------------------------------------------------------
// Window class for windows that draw vgui panels in Maya
//-----------------------------------------------------------------------------
class CVsVGuiWindow
{
public:
	CVsVGuiWindow();
	virtual ~CVsVGuiWindow();

	// Sets the main panel for the window
	void SetMainPanel( vgui::EditablePanel *pPanel );
	vgui::EditablePanel *GetMainPanel();

	// Attaches to/detaches from a particular window
	void Attach( );
	void Detach();

protected:
	// Paints the window
	void PaintWindow();

private:
	// Windows messages callback
	LRESULT WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	LRESULT ParentWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	// Locate window to attach to 
	HWND GetMayaWindow( const char *pWindowName );

	// Locate window to key focus control to 
	HWND GetKeyFocusWindow( HWND hRenderWindow );

	void Tick( float flElapsedTime );

	static LRESULT CALLBACK SubclassCallback( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData );
	static LRESULT CALLBACK KeyFocusSubclassCallback( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData );
	static BOOL CALLBACK EnumChildProc( HWND hwnd, LPARAM lParam );
	static BOOL CALLBACK EnumKeyFocusChildProc( HWND hWnd, LPARAM lParam );
	static void MayaTimerFunc( float flElapsedTime, float flLastTime, void* clientData );

	HWND m_hWnd;
	HWND m_hKeyFocusWnd;
	int m_hVGuiContext;
	float m_flRenderDelayTime;
	int m_nCurrentTick;
	int m_nLastRenderedTick;
	MCallbackId m_hTimerCallbackId;
	vgui::DHANDLE<vgui::EditablePanel> m_hMainPanel;

	static const UINT_PTR s_subclassId;
};


//-----------------------------------------------------------------------------
// Some random number, that and the callback pointer identify things
//-----------------------------------------------------------------------------
const UINT_PTR CVsVGuiWindow::s_subclassId(0xab01265);


//-----------------------------------------------------------------------------
// Constructor, destructor
//-----------------------------------------------------------------------------
CVsVGuiWindow::CVsVGuiWindow()
{
	m_nLastRenderedTick = -1;
	m_nCurrentTick = 0;
	m_hTimerCallbackId = 0;
	m_flRenderDelayTime = 0.0f;
	m_hWnd = NULL;
	m_hKeyFocusWnd = NULL;
	m_hVGuiContext = vgui::DEFAULT_VGUI_CONTEXT;
}

CVsVGuiWindow::~CVsVGuiWindow()
{
	Detach();
}


//-----------------------------------------------------------------------------
// Subclassed Parent Callback
//-----------------------------------------------------------------------------
LRESULT CVsVGuiWindow::ParentWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// This is necessary because keyboard input is controlled by the 'focus hole' window
	switch ( uMsg )
	{
	case WM_CHAR:
	case WM_SYSCHAR:
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		if ( hWnd == m_hKeyFocusWnd )
			return SendMessage( m_hWnd, uMsg, wParam, lParam );
		break;

	case WM_GETDLGCODE:
		return DLGC_WANTALLKEYS | DLGC_WANTCHARS | DLGC_WANTARROWS | DLGC_WANTTAB; // forward all keyboard into to vgui panel	

	case WM_SETFOCUS:
		if ( hWnd == m_hKeyFocusWnd )
		{
			g_pMayaVGui->SetFocus( m_hWnd, m_hVGuiContext );
		}
		break;

	case WM_KILLFOCUS:
		if ( hWnd == m_hKeyFocusWnd )
		{
			if ( g_pMayaVGui->HasFocus( m_hWnd ) )
			{
				g_pMayaVGui->SetFocus( NULL, 0 );
			}
		}
		break;
	}

	return ::DefSubclassProc( hWnd, uMsg, wParam, lParam );
}


//-----------------------------------------------------------------------------
// Subclassed Window Callback
//-----------------------------------------------------------------------------
LRESULT CVsVGuiWindow::WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch ( uMsg )
	{
	case WM_NCDESTROY:
		// WM_NCDESTROY is the last message, detach the object from the window handle
		if ( hWnd == m_hWnd )
		{
			Detach();
			LRESULT lr = ::DefSubclassProc( hWnd, uMsg, wParam, lParam );
			delete this;
			return lr;
		}
		break;
	 
	case WM_GETDLGCODE:
		return DLGC_WANTALLKEYS | DLGC_WANTCHARS; // forward all keyboard into to vgui panel	

	case WM_SETCURSOR:
		return 1;	// don't pass WM_SETCURSOR
	  
	case WM_MOUSEACTIVATE:
		if ( hWnd == m_hWnd )
		{
			HWND hFocus = GetFocus();
			if ( hFocus != m_hKeyFocusWnd )
			{
				SetFocus( m_hKeyFocusWnd );
			}
			return MA_ACTIVATE;
		}
		break;

	case WM_SETFOCUS:
		if ( hWnd == m_hWnd )
		{
			g_pMayaVGui->SetFocus( m_hWnd, m_hVGuiContext );
			return 0;
		}
		break;

	case WM_KILLFOCUS:
		if ( hWnd == m_hWnd )
		{
			if ( g_pMayaVGui->HasFocus( m_hWnd ) )
			{
				g_pMayaVGui->SetFocus( NULL, 0 );
			}
			return 0;
		}
		break;
		 
	case WM_PAINT:
		if ( hWnd == m_hWnd )
		{
			PaintWindow();
			return 0;
		}
		break;
	}
	 
	return ::DefSubclassProc( hWnd, uMsg, wParam, lParam );
}


//-----------------------------------------------------------------------------
// Subclassed Window Callback
//-----------------------------------------------------------------------------
LRESULT CALLBACK CVsVGuiWindow::SubclassCallback( HWND hWnd, UINT uMsg, 
	WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData )
{
	// Call the SubclassedWindow object that is attached to this window
	CVsVGuiWindow *pCVsVGuiWindow( (CVsVGuiWindow *)dwRefData );
	if ( pCVsVGuiWindow )
		return pCVsVGuiWindow->WndProc( hWnd, uMsg, wParam, lParam );

	return 0;
}


//-----------------------------------------------------------------------------
// Subclassed Window Callback
//-----------------------------------------------------------------------------
LRESULT CALLBACK CVsVGuiWindow::KeyFocusSubclassCallback( HWND hWnd, UINT uMsg, 
												 WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData )
{
	// Call the SubclassedWindow object that is attached to this window
	CVsVGuiWindow *pCVsVGuiWindow( (CVsVGuiWindow *)dwRefData );
	if ( pCVsVGuiWindow )
		return pCVsVGuiWindow->ParentWndProc( hWnd, uMsg, wParam, lParam );

	return 0;
}


//-----------------------------------------------------------------------------
// Callback for enumerating windows via EnumChildWindows to search for the
// proper window pane in Maya with the content "Vst ModelBrowser"
//-----------------------------------------------------------------------------
static const char *s_pWindowName;
BOOL CALLBACK CVsVGuiWindow::EnumChildProc( HWND hWnd, LPARAM lParam )
{
	char str[1024];

	// Make sure the window text matches 
	::GetWindowText( hWnd, str, sizeof(str) );
	if ( Q_strcmp( str, s_pWindowName ) )
		return TRUE;

	DWORD_PTR ptr;
	if ( GetWindowSubclass( hWnd, SubclassCallback, s_subclassId, &ptr ) )
		return TRUE;

	*(HWND*)lParam = hWnd;
	return FALSE;
}

BOOL CALLBACK CVsVGuiWindow::EnumKeyFocusChildProc( HWND hWnd, LPARAM lParam )
{
	// Make sure the window text matches 
	char str[1024];
	::GetWindowText( hWnd, str, sizeof(str) );
	if ( Q_stricmp( str, "focushole" ) )
		return TRUE;

	*(HWND*)lParam = hWnd;
	return FALSE;
}


//-----------------------------------------------------------------------------
// Locate window to attach to 
//-----------------------------------------------------------------------------
HWND CVsVGuiWindow::GetMayaWindow( const char *pWindowName )
{
	// Find the window to attach to. 
	// The window has a child window with a particular name
	HWND subclassHWnd = NULL;
	s_pWindowName = pWindowName;
	EnumChildWindows( ::GetDesktopWindow(), EnumChildProc, ( long )&subclassHWnd );

	if ( !subclassHWnd )
	{
		Error( "Can't find window \"%s\"\n", pWindowName );
		return NULL;
	}

	return subclassHWnd;
}


//-----------------------------------------------------------------------------
// Locate window to key focus control to 
//-----------------------------------------------------------------------------
HWND CVsVGuiWindow::GetKeyFocusWindow( HWND hRenderWindow )
{
	// The parent window is the one that has keyboard focus, and deals with input
	HWND hParentWnd = GetParent( hRenderWindow );
	if ( !hParentWnd )
		return NULL;

	HWND hGrandparentWnd = GetParent( hParentWnd );
	if ( !hGrandparentWnd )
		return hParentWnd;

	HWND kKeyFocusWnd = 0;
	EnumChildWindows( hGrandparentWnd, EnumKeyFocusChildProc, ( long )&kKeyFocusWnd );
	return kKeyFocusWnd ? kKeyFocusWnd : hParentWnd;
}


//-----------------------------------------------------------------------------
// Sets a panel to be the main panel
//-----------------------------------------------------------------------------
void CVsVGuiWindow::SetMainPanel( vgui::EditablePanel * pPanel )
{
	Assert( m_hMainPanel.Get() == NULL );
	Assert( m_hVGuiContext == vgui::DEFAULT_VGUI_CONTEXT );

	m_hMainPanel = pPanel;

	m_hMainPanel->SetParent( vgui::surface()->GetEmbeddedPanel() );
	m_hMainPanel->SetVisible( true );
	m_hMainPanel->SetCursor( vgui::dc_arrow );

	m_hVGuiContext = vgui::ivgui()->CreateContext();
	vgui::ivgui()->AssociatePanelWithContext( m_hVGuiContext, m_hMainPanel->GetVPanel() );

	pPanel->InvalidateLayout();

	EnableWindow( m_hWnd, true );
	SetFocus( m_hWnd );
}

vgui::EditablePanel *CVsVGuiWindow::GetMainPanel()
{
	return m_hMainPanel.Get();
}


//-----------------------------------------------------------------------------
// Called by maya to update the window
//-----------------------------------------------------------------------------
void CVsVGuiWindow::MayaTimerFunc( float flElapsedTime, float flLastTime, void* clientData )
{
	CVsVGuiWindow* pVGuiWindow = (CVsVGuiWindow*)clientData;
	pVGuiWindow->Tick( flElapsedTime );
}

void CVsVGuiWindow::Tick( float flElapsedTime )
{
	m_flRenderDelayTime -= flElapsedTime;

	g_pInputSystem->PollInputState();
	vgui::ivgui()->RunFrame(); 

	// run vgui animations
	vgui::GetAnimationController()->UpdateAnimations( vgui::system()->GetCurrentTime() );

	// Ensures we give time to the other windows to render
	if ( m_flRenderDelayTime > 0.0f )
		return;

	++m_nCurrentTick;
			 
	InvalidateRect( m_hWnd, NULL, TRUE );
}


//-----------------------------------------------------------------------------
// Attaches to/detaches from a particular window
//-----------------------------------------------------------------------------
void CVsVGuiWindow::Attach( )
{
	// Find the window to attach to. It's got a specific name, so we can use that 
	// to find the window
	m_hWnd = GetMayaWindow( VGUI_WINDOW_NAME );
	if ( !m_hWnd )
		return;

	// Maya appears to have a special window we must hook to in order to deal with keyboard focus
	m_hKeyFocusWnd = GetKeyFocusWindow( m_hWnd );

	// Subclass The Window
	if ( !::SetWindowSubclass( m_hWnd, SubclassCallback, s_subclassId, (DWORD_PTR)this ) )
	{
		Warning( "Unable to subclass window!\n" );
		return;
	}

	// Subclass The Window
	if ( m_hKeyFocusWnd )
	{
		if ( !::SetWindowSubclass( m_hKeyFocusWnd, KeyFocusSubclassCallback, s_subclassId, (DWORD_PTR)this ) )
		{
			Warning( "Unable to subclass key focus window!\n" );
			return;
		}
	}

	g_pMaterialSystem->AddView( m_hWnd );

	m_hTimerCallbackId = MTimerMessage::addTimerCallback( 0.015f, MayaTimerFunc, this );
}


//-----------------------------------------------------------------------------
// main application
//-----------------------------------------------------------------------------
void CVsVGuiWindow::Detach()
{
	if ( m_hTimerCallbackId != 0 )
	{
		MTimerMessage::removeCallback( m_hTimerCallbackId );
		m_hTimerCallbackId = 0;
	}

	if ( m_hMainPanel.Get() )
	{
		m_hMainPanel->MarkForDeletion();
		m_hMainPanel = NULL;
	}

	if ( m_hVGuiContext != vgui::DEFAULT_VGUI_CONTEXT )
	{
		vgui::ivgui()->DestroyContext( m_hVGuiContext );
		m_hVGuiContext = vgui::DEFAULT_VGUI_CONTEXT;
	}

	// detach
	if ( m_hWnd )
	{
		// kill the timer if any
		g_pMaterialSystem->RemoveView( m_hWnd );
		RemoveWindowSubclass( m_hWnd, SubclassCallback, s_subclassId );
		m_hWnd = NULL;
	}

	if ( m_hKeyFocusWnd )
	{
		RemoveWindowSubclass( m_hKeyFocusWnd, KeyFocusSubclassCallback, s_subclassId );
		m_hKeyFocusWnd = NULL;
	}
}

 
//-----------------------------------------------------------------------------
// main application
//-----------------------------------------------------------------------------
void CVsVGuiWindow::PaintWindow()
{
	if ( m_nCurrentTick == m_nLastRenderedTick || !g_pVGui->IsRunning() )
	{
		ValidateRect( m_hWnd, NULL );
		return;
	}

	m_nCurrentTick = m_nLastRenderedTick;

	vgui::VPANEL root = g_pVGuiSurface->GetEmbeddedPanel();
	g_pVGuiSurface->Invalidate( root );

	RECT windowRect;
	CMatRenderContextPtr pRenderContext( g_pMaterialSystem );
	::GetClientRect( m_hWnd, &windowRect );
	g_pMaterialSystem->SetView( m_hWnd );
	pRenderContext->Viewport( 0, 0, windowRect.right, windowRect.bottom );

	float flStartTime = Plat_FloatTime();

	pRenderContext->ClearColor4ub( 76, 88, 68, 255 ); 
	pRenderContext->ClearBuffers( true, true );

	g_pMaterialSystem->BeginFrame( 0 );

	// draw from the main panel down
	if ( m_hMainPanel.Get() )
	{
		int w, h;
		m_hMainPanel->GetSize( w, h );

		if ( w != windowRect.right || h != windowRect.bottom )
		{
			m_hMainPanel->SetBounds( 0, 0, windowRect.right, windowRect.bottom );
			m_hMainPanel->Repaint();
		}

		g_pVGuiSurface->RestrictPaintToSinglePanel( m_hMainPanel->GetVPanel() );
		g_pVGuiSurface->PaintTraverseEx( root, true );
		g_pVGuiSurface->RestrictPaintToSinglePanel( 0 );
	}

	g_pMaterialSystem->EndFrame();
	g_pMaterialSystem->SwapBuffers();
	g_pMaterialSystem->SetView( NULL );
	ValidateRect( m_hWnd, NULL );

	m_flRenderDelayTime = Plat_FloatTime() - flStartTime;
	m_flRenderDelayTime = max( m_flRenderDelayTime, 0.015f );
}


//-----------------------------------------------------------------------------
// Creates, destroys a maya vgui window
//-----------------------------------------------------------------------------
void CreateMayaVGuiWindow( vgui::EditablePanel *pRootPanel, const char *pPanelName )
{
	CVsVGuiWindow *pVGuiWindow = new CVsVGuiWindow;
	pVGuiWindow->Attach( );
	pVGuiWindow->SetMainPanel( pRootPanel );
}

void DestroyMayaVGuiWindow( const char *pPanelName )
{
}



