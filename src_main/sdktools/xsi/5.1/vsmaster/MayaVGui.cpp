//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: Class for dealing with vgui focus issues across all plugins
//
// $NoKeywords: $
//===========================================================================//

#include <windows.h>
#include "imayavgui.h"

#include "xsi_pluginregistrar.h"
#include "xsi_decl.h"
#include "xsi_command.h"
#include "xsi_viewcontext.h"
#include "xsi_argument.h"
#include "xsi_application.h"
#include "xsi_layout.h"
#include "xsi_view.h"
#include "xsi_desktop.h"
#include "inputsystem/iinputsystem.h"
#include "tier1/utldict.h"
#include "tier3/tier3.h"
#include <vgui/IVGui.h>
#include <vgui/ISystem.h>
#include <vgui/ILocalize.h>
#include "vgui_controls/AnimationController.h"
#include <VGuiMatSurface/IMatSystemSurface.h>
#include "VsMayaCommand.h"

#define PLUGIN_NAME			L"ValveViewer"

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

	virtual bool CreateVGuiWindow ( HWND in_hParent, const char *pWindowTypeName, const char *pPanelName);
	virtual void DestroyVGuiWindow( const char *pWindowTypeName, const char *pPanelName );

private:
	static CUtlDict< IMayaVguiWindowFactory*, unsigned short > m_WindowFactories;
	void *m_pActiveWindow;	// the VGUI window that has the focus
	bool m_bIsInitialized;
};


//-----------------------------------------------------------------------------
// Purpose: singleton accessor
//-----------------------------------------------------------------------------
static CMayaVGui s_MayaVGui;
EXPOSE_SINGLE_INTERFACE_GLOBALVAR( CMayaVGui, IMayaVGui, MAYA_VGUI_INTERFACE_VERSION, s_MayaVGui );

// static
CUtlDict< IMayaVguiWindowFactory*, unsigned short > CMayaVGui::m_WindowFactories;


//-----------------------------------------------------------------------------
// Constructor, destructor
//-----------------------------------------------------------------------------
CMayaVGui::CMayaVGui(void) : BaseClass( false )
{
	m_pActiveWindow = NULL;
	m_bIsInitialized = false;
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

	// Start the App running
	vgui::ivgui()->Start();
	vgui::ivgui()->SetSleep(false);

	// load the base localization file
	g_pVGuiLocalize->AddFile( "Resource/valve_%language%.txt" );
	g_pVGuiLocalize->AddFile( "Resource/vgui_%language%.txt");
	g_pVGuiLocalize->AddFile( "Resource/dmecontrols_%language%.txt");

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
// Other public methods
//-----------------------------------------------------------------------------
bool CMayaVGui::CreateVGuiWindow( HWND in_hParent, const char *pWindowTypeName, const char *pPanelName )
{
	unsigned short hFactory = m_WindowFactories.Find( pWindowTypeName );
	if ( hFactory != m_WindowFactories.InvalidIndex() )
	{
		m_WindowFactories[hFactory]->CreateVguiWindow( in_hParent, pPanelName );
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
// These methods are called as a result of creating the valve viewer
//-----------------------------------------------------------------------------
XSIPLUGINCALLBACK void ValveViewer_Init( XSI::CRef in_pViewCtx)
{
	XSI::ViewContext l_vViewContext =  in_pViewCtx;

	l_vViewContext.SetFlags(XSI::siWindowNotifications);
	return ;
}

XSIPLUGINCALLBACK void ValveViewer_Term( XSI::CRef in_pViewCtx)
{
}

XSIPLUGINCALLBACK void ValveViewer_Notify( XSI::CRef in_pViewCtx )
{
}

XSIPLUGINCALLBACK void ValveViewer_SetAttributeValue(XSI::CRef in_pViewCtx, XSI::CString in_sAttribName, XSI::CValue in_vValue)
{

	XSI::ViewContext l_vViewContext = in_pViewCtx;

	if ( in_sAttribName.IsEqualNoCase(L"attach"))
	{
		XSI::CString l_cViewName(in_vValue);

		s_MayaVGui.CreateVGuiWindow((HWND)l_vViewContext.GetParentWindowHandle(),
			l_cViewName.GetAsciiString(), 
			l_cViewName.GetAsciiString());

		// save pointer to VGui as user data of the new XSI window
// FIXME		l_vViewContext.PutUserData ( (void*)l_pVGui );
	}

}

XSIPLUGINCALLBACK XSI::CValue ValveViewer_GetAttributeValue(XSI::CRef in_pViewCtx, XSI::CString in_sAttribName)
{

	return XSI::CStatus::OK;
}


//-----------------------------------------------------------------------------
//
// Command to create any type of vgui window
//
//-----------------------------------------------------------------------------
class CVsVguiWindow : public CVsMayaCommand
{
public:
	virtual void SpecifySyntax( const XSI::Context &ctxt );
	virtual XSI::CStatus doIt( XSI::Context &ctxt );
	virtual void OnRegister( XSI::PluginRegistrar& in_reg );
};


//-----------------------------------------------------------------------------
// Hook the command so we can see it in Maya
//-----------------------------------------------------------------------------
INSTALL_MAYA_COMMAND( CVsVguiWindow, vsVGuiWindow );


//-----------------------------------------------------------------------------
// Called when we register
//-----------------------------------------------------------------------------
void CVsVguiWindow::OnRegister( XSI::PluginRegistrar& in_reg )
{
	in_reg.RegisterCustomDisplay( PLUGIN_NAME );
}


//-----------------------------------------------------------------------------
// Specify syntax for the vgui window command
//-----------------------------------------------------------------------------
void CVsVguiWindow::SpecifySyntax( const XSI::Context &ctx )
{
	XSI::Command cmd( ctx.GetSource() );
	XSI::ArgumentArray args = cmd.GetArguments();
	args.Add( L"viewname", L"MDLPreview" );
	args.Add( L"targetwindow", L"floating" );
	
	//cmd.EnableReturnValue(true);
}


//-----------------------------------------------------------------------------
// Main entry point from scripts
//-----------------------------------------------------------------------------
XSI::CStatus CVsVguiWindow::doIt( XSI::Context &ctxt )
{
	XSI::Application app;

	XSI::CValueArray args = ctxt.GetAttribute( L"Arguments" );

	XSI::CString l_cViewname (args[0].GetAsText() );			
	XSI::CString l_cTargetWindow (args[1].GetAsText() );			
	XSI::View retval;

	if ( l_cTargetWindow.IsEqualNoCase(L"floating") )
	{
		XSI::Layout  l_cActiveLayout = app.GetDesktop().GetActiveLayout();
		XSI::View	view;
		l_cActiveLayout.CreateView(L"Custom Display Host", PLUGIN_NAME, view);
		view.PutAttributeValue(L"targetcontent", PLUGIN_NAME);
		view.PutAttributeValue(L"attach", l_cViewname);
		//oCDH.Resize( 50, 500 );
		retval = view;
	} else {

		XSI::View l_vVM;
		XSI::Layout  l_cActiveLayout = app.GetDesktop().GetActiveLayout();
	
		XSI::CRefArray array = l_cActiveLayout.GetViews();


		for (int a=0;a<array.GetCount();a++)
		{
			XSI::View l_vView(array[a]);
			XSI::CString name = l_vView.GetName();

			if (name.IsEqualNoCase(L"vm"))
			{
				XSI::CString l_viewportName;
				char viewport[1024];
				sprintf ( viewport, "viewport:%s", l_cTargetWindow.GetAsciiString());
				l_viewportName.PutAsciiString(viewport);

				l_vView.PutAttributeValue(l_viewportName,L"Custom Display Host");
				XSI::CRefArray viewports = l_vView.GetViews();
				for (int n=0;n<viewports.GetCount();n++)
				{
					XSI::View l_vViewport(viewports[n]);
					if ( l_cTargetWindow.IsEqualNoCase(l_vViewport.GetName()))
					{
						l_vViewport.PutAttributeValue(L"targetcontent", PLUGIN_NAME);
						l_vViewport.PutAttributeValue(L"attach", l_cViewname);
						retval = l_vViewport;
					}
				}

			}
		}
	}

	return  XSI::CStatus::OK ;
}




