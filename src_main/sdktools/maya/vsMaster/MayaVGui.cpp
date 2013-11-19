//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: Class for dealing with vgui focus issues across all plugins
//
// $NoKeywords: $
//===========================================================================//

#include "imayavgui.h"

#include "tier1/utldict.h"
#include "tier3/tier3.h"
#include <vgui/IVGui.h>
#include <vgui/ISystem.h>
#include <vgui/ILocalize.h>
#include "vgui_controls/AnimationController.h"
#include <VGuiMatSurface/IMatSystemSurface.h>
#include "inputsystem/iinputsystem.h"
#include "VsMayaMPxFactory.h"
#include "ValveMaya.h"
#include <maya/MArgParser.h>


//-----------------------------------------------------------------------------
// Implementation class
//-----------------------------------------------------------------------------
class CMayaVGui : public CTier3AppSystem< IMayaVGui >
{
	typedef CTier3AppSystem< IMayaVGui > BaseClass;

public:
	CMayaVGui(void);
	virtual ~CMayaVGui(void);

	// Inherited from IAppSystem
	virtual bool Connect( );
	virtual InitReturnVal_t Init( );
	virtual void Shutdown();

	// Inherited from IMayaVGui
	virtual void InstallVguiWindowFactory( const char *pWindowTypeName, IMayaVguiWindowFactory *pFactory );
	virtual void RemoveVguiWindowFactory( const char *pWindowTypeName, IMayaVguiWindowFactory *pFactory );
	virtual bool HasFocus( void *hWnd );
	virtual void SetFocus( void *hWnd, int hVGuiContext );
	virtual void SetModalMode( bool bEnable );
	virtual bool IsInModalMode( ) const ;

	// Other public methods
	bool CreateVGuiWindow( const char *pWindowTypeName, const char *pPanelName );
	void DestroyVGuiWindow( const char *pWindowTypeName, const char *pPanelName );

private:
	CUtlDict< IMayaVguiWindowFactory*, unsigned short > m_WindowFactories;
	void *m_pActiveWindow;	// the VGUI window that has the focus
	void *m_pQueuedActiveWindow;	// the VGUI window that has the focus
	int m_hQueuedVguiContext;
	bool m_bIsInitialized : 1;
	bool m_bInModalMode : 1;
};



//-----------------------------------------------------------------------------
// Purpose: singleton accessor
//-----------------------------------------------------------------------------
static CMayaVGui s_MayaVGui;
IMayaVGui *g_pMayaVGui = &s_MayaVGui;
EXPOSE_SINGLE_INTERFACE_GLOBALVAR( CMayaVGui, IMayaVGui, MAYA_VGUI_INTERFACE_VERSION, s_MayaVGui );


//-----------------------------------------------------------------------------
// Constructor, destructor
//-----------------------------------------------------------------------------
CMayaVGui::CMayaVGui(void) : BaseClass( false )
{
	m_pActiveWindow = NULL;
	m_pQueuedActiveWindow = NULL;
	m_hQueuedVguiContext = 0;
	m_bIsInitialized = false;
	m_bInModalMode = false;
}

CMayaVGui::~CMayaVGui(void)
{
}


//-----------------------------------------------------------------------------
// Connection
//-----------------------------------------------------------------------------
bool CMayaVGui::Connect( )
{
	return true;
}


//-----------------------------------------------------------------------------
// Init, shutdown
//-----------------------------------------------------------------------------
InitReturnVal_t CMayaVGui::Init( )
{
	// configuration settings
	vgui::system()->SetUserConfigFile("mayaPlugins.vdf", "EXECUTABLE_PATH");

	// Are we trapping input?
	g_pMatSystemSurface->EnableWindowsMessages( true );

	// Need to be able to play sounds through vgui
	// g_pMatSystemSurface->InstallPlaySoundFunc( VGui_PlaySound );

	// load scheme
	if ( !vgui::scheme()->LoadSchemeFromFile("Resource/BoxRocket.res", "MayaPlugins" ) )
		return INIT_FAILED;

	g_pVGuiLocalize->AddFile( "Resource/valve_%language%.txt" );
	g_pVGuiLocalize->AddFile( "Resource/vgui_%language%.txt");
	g_pVGuiLocalize->AddFile( "Resource/dmecontrols_%language%.txt");

	// Start the App running
	vgui::ivgui()->Start();
	vgui::ivgui()->SetSleep(false);

	vgui::VPANEL root = g_pVGuiSurface->GetEmbeddedPanel();
	g_pVGuiSurface->Invalidate( root );

	m_bIsInitialized = true;
	return INIT_OK;
}

void CMayaVGui::Shutdown()
{
	// Give panels a chance to settle so things
	//  Marked for deletion will actually get deleted
	if ( !m_bIsInitialized )
		return;

	g_pInputSystem->PollInputState();
	vgui::ivgui()->RunFrame();

	// stop the App running
	vgui::ivgui()->Stop();

	m_bIsInitialized = false;
}


//-----------------------------------------------------------------------------
// Installs/removes a vgui window factory 
//-----------------------------------------------------------------------------
void CMayaVGui::InstallVguiWindowFactory( const char *pWindowTypeName, IMayaVguiWindowFactory *pFactory )
{
	if ( m_WindowFactories.Find( pWindowTypeName ) != m_WindowFactories.InvalidIndex() )
	{
		Warning( "Attempted to install vgui window factory %s twice!\n", pWindowTypeName );
		return;
	}

	m_WindowFactories.Insert( pWindowTypeName, pFactory );
}

void CMayaVGui::RemoveVguiWindowFactory( const char *pWindowTypeName, IMayaVguiWindowFactory *pFactory )
{
	if ( m_WindowFactories.Find( pWindowTypeName ) == m_WindowFactories.InvalidIndex() )
	{
		Warning( "Attempted to remove unknown vgui window factory %s!\n", pWindowTypeName );
		return;
	}

	unsigned short hFactory = m_WindowFactories.Find( pWindowTypeName );
	Assert( m_WindowFactories[hFactory] == pFactory );
	m_WindowFactories.RemoveAt( hFactory );
}


//-----------------------------------------------------------------------------
// Modal mode
//-----------------------------------------------------------------------------
void CMayaVGui::SetModalMode( bool bEnable )
{
	if ( bEnable != m_bInModalMode )
	{
		m_bInModalMode = bEnable;
		if ( !bEnable )
		{
			SetFocus( m_pQueuedActiveWindow, m_hQueuedVguiContext );
			g_pMatSystemSurface->EnableWindowsMessages( true );
		}
		else
		{
			g_pMatSystemSurface->EnableWindowsMessages( false );
		}
	}
}

bool CMayaVGui::IsInModalMode( ) const 
{
	return m_bInModalMode;
}


//-----------------------------------------------------------------------------
// Other public methods
//-----------------------------------------------------------------------------
bool CMayaVGui::CreateVGuiWindow( const char *pWindowTypeName, const char *pPanelName )
{
	unsigned short hFactory = m_WindowFactories.Find( pWindowTypeName );
	if ( hFactory != m_WindowFactories.InvalidIndex() )
	{
		m_WindowFactories[hFactory]->CreateVguiWindow( pPanelName );
		return true;
	}
	return false;
}

void CMayaVGui::DestroyVGuiWindow( const char *pWindowTypeName, const char *pPanelName )
{
	// Have to iterate over all factories to see if that factory is the one that created it
	unsigned short hFactory = m_WindowFactories.Find( pWindowTypeName );
	if ( hFactory != m_WindowFactories.InvalidIndex() )
	{
		m_WindowFactories[hFactory]->DestroyVguiWindow( pPanelName );
	}
}


//-----------------------------------------------------------------------------
// Sets the focus to be on the specified window
//-----------------------------------------------------------------------------
void CMayaVGui::SetFocus( void *hWnd, int hVGuiContext )
{
	m_pQueuedActiveWindow = hWnd;
	m_hQueuedVguiContext = hVGuiContext;

	// Prevent focus messages from doing anything in modal mode
	// Calling ActivateContext will cause the vgui message pump to fire
	// which we can't do in this mode
	if ( m_bInModalMode )
		return;

	if ( hWnd == m_pActiveWindow )
		return;

	g_pInputSystem->PollInputState();
	vgui::ivgui()->RunFrame(); 

	g_pMatSystemSurface->AttachToWindow( NULL, false );
	g_pInputSystem->DetachFromWindow( );

	if ( hWnd )
	{
		m_pActiveWindow = hWnd;
		g_pInputSystem->AttachToWindow( hWnd );
		g_pMatSystemSurface->AttachToWindow( hWnd, false );
		vgui::ivgui()->ActivateContext( hVGuiContext );
	}
	else
	{
		m_pActiveWindow = NULL;
		vgui::ivgui()->ActivateContext( vgui::DEFAULT_VGUI_CONTEXT );
	}
}


//-----------------------------------------------------------------------------
// Returns true if the window has focus
//-----------------------------------------------------------------------------
bool CMayaVGui::HasFocus( void *pWnd )
{
	return m_pActiveWindow == pWnd;
}


//-----------------------------------------------------------------------------
//
// Command to create any type of vgui window
//
//-----------------------------------------------------------------------------
class CVsVguiWindow : public CVsMayaMPxCommand
{
public:
	virtual MStatus doIt( const MArgList& args );
};


//-----------------------------------------------------------------------------
// Hook the command so we can see it in Maya
//-----------------------------------------------------------------------------
INSTALL_MAYA_MPXCOMMAND(
	CVsVguiWindow,
	vsVGuiWindow,
	"Attaches and detaches vgui windows to and from previously configured Maya scripted panels" );


//-----------------------------------------------------------------------------
// Main entry point from MEL
//-----------------------------------------------------------------------------
MStatus CVsVguiWindow::doIt( const MArgList& args )
{
	MSyntax syntax;

	// parse the command line arguments
	syntax.addFlag( "a", "attach", MSyntax::kString, MSyntax::kString );
	syntax.addFlag( "r", "remove", MSyntax::kString, MSyntax::kString );
	MArgParser parser(syntax, args);

	// ATTACH COMMAND
	if( parser.isFlagSet( "attach" ) )
	{
		MString vguiWindowType, panelName;
		parser.getFlagArgument( "attach", 0, vguiWindowType );
		parser.getFlagArgument( "attach", 1, panelName );
		if ( s_MayaVGui.CreateVGuiWindow( vguiWindowType.asChar(), panelName.asChar() ) )
		{
			setResult( "vsVguiWindow creation successful!\n" );
			return MS::kSuccess;
		}
		setResult( "vsVguiWindow - Unknown window type!\n" );
		return MS::kFailure;
	}

	if ( parser.isFlagSet( "remove" ) ) 
	{
		MString vguiWindowType, panelName;
		parser.getFlagArgument( "remove", 0, vguiWindowType );
		parser.getFlagArgument( "remove", 1, panelName );
		s_MayaVGui.DestroyVGuiWindow( vguiWindowType.asChar(), panelName.asChar() );
	}

	return MS::kSuccess;
}
