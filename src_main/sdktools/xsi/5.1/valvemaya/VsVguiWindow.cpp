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
#include "inputsystem/iinputsystem.h"
#include "imayavgui.h"


//-----------------------------------------------------------------------------
// Name to look for when attaching to a window
//-----------------------------------------------------------------------------
#define VGUI_WINDOW_NAME	"vsVGuiWindow"
#define REPAINT_TIMER_ID	1042 //random value, hopfully no collisions	


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
	void Attach(HWND in_hParent  );
	void Detach();

protected:
	// Paints the window
	void PaintWindow();

private:
	// Windows messages callback
	LRESULT WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	// Locate window to attach to 
	HWND GetMayaWindow( const char *pWindowName );

	static LRESULT CALLBACK SubclassCallback( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData );
	static BOOL CALLBACK EnumChildProc( HWND hwnd, LPARAM lParam );

	HWND m_hWnd;
	int m_hVGuiContext;
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
	m_hWnd = NULL;
	m_hVGuiContext = vgui::DEFAULT_VGUI_CONTEXT;
}

CVsVGuiWindow::~CVsVGuiWindow()
{
	Detach();
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

	case WM_TIMER:
		if ( hWnd == m_hWnd && wParam == REPAINT_TIMER_ID )
		{
			InvalidateRect( m_hWnd, NULL, TRUE );
		}
		break;

	case WM_SETCURSOR:
		return 1;	// don't pass WM_SETCURSOR

	case WM_MOUSEACTIVATE:
	case WM_SETFOCUS:
		if ( hWnd == m_hWnd )
		{
			g_pMayaVGui->SetFocus( m_hWnd, m_hVGuiContext );
		}
		return 0;

	case WM_KILLFOCUS:
		if ( hWnd == m_hWnd )
		{
			if ( g_pMayaVGui->HasFocus( m_hWnd ) )
			{
				g_pMayaVGui->SetFocus( NULL, 0 );
			}
		}
		return 0;
	 
	case WM_NCPAINT:
	case WM_PAINT:
		PaintWindow();
		return 0;
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
// Attaches to/detaches from a particular window
//-----------------------------------------------------------------------------
void CVsVGuiWindow::Attach( HWND in_hParent )
{
	// Find the window to attach to. It's got a specific name, so we can use that 
	// to find the window
	m_hWnd = in_hParent;
	if ( !m_hWnd )
		return;

	// Note, it's hidden
	DWORD style = WS_POPUP | WS_CLIPSIBLINGS | WS_OVERLAPPEDWINDOW;
	style &= ~WS_THICKFRAME;
	style &= ~WS_MAXIMIZEBOX;

	RECT windowRect;
	GetWindowRect( m_hWnd, &windowRect );
	AdjustWindowRectEx( &windowRect, style, FALSE, 0 );

	// Subclass The Window
	if ( !::SetWindowSubclass( m_hWnd, SubclassCallback, s_subclassId, (DWORD_PTR)this ) )
	{
		Warning( "Unable to subclass window!\n" );
	}

	g_pMaterialSystem->AddView( m_hWnd );

	::SetTimer( m_hWnd, REPAINT_TIMER_ID, 10, NULL );
}


//-----------------------------------------------------------------------------
// main application
//-----------------------------------------------------------------------------
void CVsVGuiWindow::Detach()
{
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
		::KillTimer( m_hWnd, REPAINT_TIMER_ID );
		g_pMaterialSystem->RemoveView( m_hWnd );
		RemoveWindowSubclass( m_hWnd, SubclassCallback, s_subclassId );
		m_hWnd = NULL;
	}
}


//-----------------------------------------------------------------------------
// main application
//-----------------------------------------------------------------------------
void CVsVGuiWindow::PaintWindow()
{
	// run app frame loop
	if ( !g_pVGui->IsRunning() )
		return;

	vgui::VPANEL root = g_pVGuiSurface->GetEmbeddedPanel();
	g_pVGuiSurface->Invalidate( root );

	RECT windowRect;
	::GetClientRect( m_hWnd, &windowRect );
	g_pMaterialSystem->SetView( m_hWnd );
	g_pMaterialSystem->Viewport( 0, 0, windowRect.right, windowRect.bottom );
	
	g_pInputSystem->PollInputState();
	vgui::ivgui()->RunFrame(); 

	// run vgui animations
	vgui::GetAnimationController()->UpdateAnimations( vgui::system()->GetCurrentTime() );

	g_pMaterialSystem->ClearColor4ub( 76, 88, 68, 255 ); 
	g_pMaterialSystem->ClearBuffers( true, true );

	g_pMaterialSystem->BeginFrame();

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
}


//-----------------------------------------------------------------------------
// Creates, destroys a maya vgui window
//-----------------------------------------------------------------------------
void CreateMayaVGuiWindow( HWND in_hParent, vgui::EditablePanel *pRootPanel, const char *pPanelName )
{
	CVsVGuiWindow *pVGuiWindow = new CVsVGuiWindow;
	pVGuiWindow->Attach( in_hParent);
	pVGuiWindow->SetMainPanel( pRootPanel );
}

void DestroyMayaVGuiWindow( const char *pPanelName )
{
}



