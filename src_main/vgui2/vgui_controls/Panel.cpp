//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//


#include <stdio.h>
#include <assert.h>
#include <UtlVector.h>
#include <vstdlib/IKeyValuesSystem.h>

#include <vgui/IBorder.h>
#include <vgui/IInput.h>
#include <vgui/IPanel.h>
#include <vgui/IScheme.h>
#include <vgui/ISurface.h>
#include <vgui/ISystem.h>
#include <vgui/ILocalize.h>
#include <vgui/IVGui.h>
#include <KeyValues.h>
#include <vgui/MouseCode.h>

#include <vgui_controls/Panel.h>
#include <vgui_controls/BuildGroup.h>
#include <vgui_controls/Tooltip.h>
#include <vgui_controls/PHandle.h>
#include <vgui_controls/Controls.h>
#include "vgui_controls/Menu.h"
#include "vgui_controls/MenuItem.h"

#include "UtlDict.h"
#include "UtlBuffer.h"
#include "MemPool.h"
#include "FileSystem.h"
#include "tier0/icommandline.h"

#include "tier0/vprof.h"

// memdbgon must be the last include file in a .cpp file!!!
#include <tier0/memdbgon.h>

using namespace vgui;

#define TRIPLE_PRESS_MSEC	300

static char *CopyString( const char *in )
{
	if ( !in )
		return NULL;

	int len = strlen( in );
	char *n = new char[ len + 1 ];
	Q_strncpy( n, in, len  + 1 );
	return n;
}

#if defined( VGUI_USEDRAGDROP )
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
struct vgui::DragDrop_t
{
	vgui::DragDrop_t() :
		m_bDragEnabled( false ),
		m_bDropEnabled( false ),
		m_bDragStarted( false ),
		m_nDragStartTolerance( 8 ),
		m_bDragging( false ),
		m_lDropHoverTime( 0 ),
		m_bDropMenuShown( false ),
		m_bPreventChaining( false )
	{
		m_nStartPos[ 0 ] = m_nStartPos[ 1 ] = 0;
		m_nLastPos[ 0 ] = m_nLastPos[ 1 ] = 0;
	}

	// Drag related data
	bool		m_bDragEnabled;
	bool		m_bDragging;
	bool		m_bDragStarted;
	// How many pixels the dragged box must move before showing the outline rect...
	int			m_nDragStartTolerance;
	int			m_nStartPos[ 2 ];
	int			m_nLastPos[ 2 ];
	CUtlVector< KeyValues * >	m_DragData;
	CUtlVector< PHandle >		m_DragPanels;

	// Drop related data
	bool		m_bDropEnabled;
	// A droppable panel can have a hover context menu, which will show up after m_flHoverContextTime of hovering
	float		m_flHoverContextTime;

	PHandle			m_hCurrentDrop;
	// Amount of time hovering over current drop target
	long			m_lDropHoverTime;
	bool			m_bDropMenuShown;
	DHANDLE< Menu >	m_hDropContextMenu;

	// Misc data
	bool			m_bPreventChaining;
};

//-----------------------------------------------------------------------------
// Purpose: Helper for painting to the full screen...
//-----------------------------------------------------------------------------
class CDragDropHelperPanel : public Panel
{
	DECLARE_CLASS_SIMPLE( CDragDropHelperPanel, Panel );
public:
	CDragDropHelperPanel();

	virtual VPANEL IsWithinTraverse(int x, int y, bool traversePopups);
	virtual void PostChildPaint();

	void AddPanel( Panel *current );

	void RemovePanel( Panel *search );

private:
	struct DragHelperPanel_t
	{
		PHandle		m_hPanel;
	};

	CUtlVector< DragHelperPanel_t >	m_PaintList;
};

vgui::DHANDLE< CDragDropHelperPanel >	s_DragDropHelper;
#endif

#if defined( VGUI_USEKEYBINDINGMAPS )

BoundKey_t::BoundKey_t():
	isbuiltin( true ),
	bindingname( 0 ),
	keycode( KEY_NONE ),
	modifiers( 0 )
{
}

BoundKey_t::BoundKey_t( const BoundKey_t& src )
{
	isbuiltin			= src.isbuiltin;
	bindingname			= isbuiltin ? src.bindingname : CopyString( src.bindingname );
	keycode				= src.keycode;
	modifiers			= src.modifiers;
}

BoundKey_t& BoundKey_t::operator =( const BoundKey_t& src )
{
	if ( this == &src )
		return *this;
	isbuiltin			= src.isbuiltin;
	bindingname			= isbuiltin ? src.bindingname : CopyString( src.bindingname );
	keycode				= src.keycode;
	modifiers			= src.modifiers;
	return *this;
}


BoundKey_t::~BoundKey_t()
{
	if ( !isbuiltin )
	{
		delete[] bindingname;
	}
}

KeyBindingMap_t::KeyBindingMap_t() :
	bindingname( 0 ),
	func( 0 ),
	helpstring( 0 ),
	docstring( 0 ),
	passive( false )
{
}

KeyBindingMap_t::KeyBindingMap_t( const KeyBindingMap_t& src )
{
	bindingname			= src.bindingname;
	helpstring			= src.helpstring;
	docstring			= src.docstring;

	func				= src.func;
	passive				= src.passive;
}

KeyBindingMap_t::~KeyBindingMap_t()
{
}

class CKeyBindingsMgr
{
public:
	
	CKeyBindingsMgr() :
		m_Bindings( 0, 0, KeyBindingContextHandleLessFunc ),
		m_nKeyBindingContexts( 0 )
	{
	}

	struct KBContext_t
	{
		KBContext_t() :
			m_KeyBindingsFile( UTL_INVAL_SYMBOL ),
			m_KeyBindingsPathID( UTL_INVAL_SYMBOL )
		{
			m_Handle = INVALID_KEYBINDINGCONTEXT_HANDLE;
		}

		KBContext_t( const KBContext_t& src )
		{
			m_Handle = src.m_Handle;
			m_KeyBindingsFile = src.m_KeyBindingsFile;
			m_KeyBindingsPathID = src.m_KeyBindingsPathID;
			int c = src.m_Panels.Count();
			for ( int i = 0; i < c; ++i )
			{
				m_Panels.AddToTail( src.m_Panels[ i ] );
			}
		}

		KeyBindingContextHandle_t	m_Handle;
		CUtlSymbol					m_KeyBindingsFile;
		CUtlSymbol					m_KeyBindingsPathID;
		CUtlVector< Panel * >		m_Panels;
	};

	static bool KeyBindingContextHandleLessFunc( const KBContext_t& lhs, const KBContext_t& rhs )
	{
		return lhs.m_Handle < rhs.m_Handle;
	}

	KeyBindingContextHandle_t CreateContext( char const *filename, char const *pathID )
	{
		KBContext_t entry;

		entry.m_Handle = (KeyBindingContextHandle_t)++m_nKeyBindingContexts;
		entry.m_KeyBindingsFile = filename;
		if ( pathID )
		{
			entry.m_KeyBindingsPathID = pathID;
		}
		else
		{
			entry.m_KeyBindingsPathID = UTL_INVAL_SYMBOL;
		}

		m_Bindings.Insert( entry );

		return entry.m_Handle;
	}

	void AddPanelToContext( KeyBindingContextHandle_t handle, Panel *panel )
	{
		if ( !panel->GetName() || !panel->GetName()[ 0 ] )
		{
			Warning( "Can't add Keybindings Context for unnamed panels\n" );
			return;
		}

		KBContext_t *entry = Find( handle );
		Assert( entry );
		if ( entry )
		{
			int idx = entry->m_Panels.Find( panel );
			if ( idx == entry->m_Panels.InvalidIndex() )
			{
				entry->m_Panels.AddToTail( panel );
			}
		}
	}

	void OnPanelDeleted( KeyBindingContextHandle_t handle, Panel *panel )
	{
		KBContext_t *kb = Find( handle );
		if ( kb )
		{
			kb->m_Panels.FindAndRemove( panel );
		}
	}
	
	KBContext_t *Find( KeyBindingContextHandle_t handle )
	{
		KBContext_t search;
		search.m_Handle = handle;
		int idx = m_Bindings.Find( search );
		if ( idx == m_Bindings.InvalidIndex() )
		{
			return NULL;
		}
		return &m_Bindings[ idx ];
	}

	char const *GetKeyBindingsFile( KeyBindingContextHandle_t handle )
	{
		KBContext_t *kb = Find( handle );
		if ( kb )
		{
			return kb->m_KeyBindingsFile.String();
		}
		Assert( 0 );
		return "";
	}

	char const *GetKeyBindingsFilePathID( KeyBindingContextHandle_t handle )
	{
		KBContext_t *kb = Find( handle );
		if ( kb )
		{
			return kb->m_KeyBindingsPathID.String();
		}
		Assert( 0 );
		return NULL;
	}

	int GetPanelsWithKeyBindingsCount( KeyBindingContextHandle_t handle )
	{
		KBContext_t *kb = Find( handle );
		if ( kb )
		{
			return kb->m_Panels.Count();
		}
		Assert( 0 );
		return 0;
	}

	//-----------------------------------------------------------------------------
	// Purpose: static method
	// Input  : index - 
	// Output : Panel
	//-----------------------------------------------------------------------------
	Panel *GetPanelWithKeyBindings( KeyBindingContextHandle_t handle, int index )
	{		
		KBContext_t *kb = Find( handle );
		if ( kb )
		{
			Assert( index >= 0 && index < kb->m_Panels.Count() );
			return kb->m_Panels[ index ];
		}
		Assert( 0 );
		return 0;
	}

	CUtlRBTree< KBContext_t, int >	m_Bindings;
	int m_nKeyBindingContexts;
};

static CKeyBindingsMgr g_KBMgr;

//-----------------------------------------------------------------------------
// Purpose: Static method to allocate a context
// Input  :  - 
// Output : KeyBindingContextHandle_t
//-----------------------------------------------------------------------------
KeyBindingContextHandle_t Panel::CreateKeyBindingsContext( char const *filename, char const *pathID /*=0*/ )
{
	return g_KBMgr.CreateContext( filename, pathID );
}


//-----------------------------------------------------------------------------
// Purpose: static method
// Input  :  - 
// Output : int
//-----------------------------------------------------------------------------
int Panel::GetPanelsWithKeyBindingsCount( KeyBindingContextHandle_t handle )
{
	return g_KBMgr.GetPanelsWithKeyBindingsCount( handle );
}

//-----------------------------------------------------------------------------
// Purpose: static method
// Input  : index - 
// Output : Panel
//-----------------------------------------------------------------------------
Panel *Panel::GetPanelWithKeyBindings( KeyBindingContextHandle_t handle, int index )
{
	return g_KBMgr.GetPanelWithKeyBindings( handle, index );
}


//-----------------------------------------------------------------------------
// Returns the number of keybindings
//-----------------------------------------------------------------------------
int Panel::GetKeyMappingCount( )
{
	int nCount = 0;
	PanelKeyBindingMap *map = GetKBMap();
	while ( map )
	{
		nCount += map->entries.Count();
		map = map->baseMap;
	}
	return nCount;
}


//-----------------------------------------------------------------------------
// Purpose: static method.  Reverts key bindings for all registered panels (panels with keybindings actually
//  loaded from file
// Input  :  - 
//-----------------------------------------------------------------------------
void Panel::RevertKeyBindings( KeyBindingContextHandle_t handle )
{
	int c = GetPanelsWithKeyBindingsCount( handle );
	for ( int i = 0; i < c; ++i )
	{
		Panel *kbPanel = GetPanelWithKeyBindings( handle, i );
		Assert( kbPanel );
		kbPanel->RevertKeyBindingsToDefault();
	}
}

static void BufPrint( CUtlBuffer& buf, int level, char const *fmt, ... )
{
	char string[ 2048 ];
	va_list argptr;
	va_start( argptr, fmt );
	_vsnprintf( string, sizeof( string ) - 1, fmt, argptr );
	va_end( argptr );
	string[ sizeof( string ) - 1 ] = 0;

	while ( --level >= 0 )
	{
		buf.Printf( "    " );
	}
	buf.Printf( "%s", string );
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : handle - 
//-----------------------------------------------------------------------------
void Panel::SaveKeyBindings( KeyBindingContextHandle_t handle )
{
	char const *filename = g_KBMgr.GetKeyBindingsFile( handle );
	char const *pathID = g_KBMgr.GetKeyBindingsFilePathID( handle );

	SaveKeyBindingsToFile( handle, filename, pathID );
}

//-----------------------------------------------------------------------------
// Purpose: static method.  Saves key binding files out for all keybindings
// Input  :  - 
//-----------------------------------------------------------------------------
void Panel::SaveKeyBindingsToFile( KeyBindingContextHandle_t handle, char const *filename, char const *pathID /*= 0*/ )
{
	CUtlBuffer buf( 0, 0, CUtlBuffer::TEXT_BUFFER );

	BufPrint( buf, 0, "keybindings\n" );
	BufPrint( buf, 0, "{\n" );

	int c = GetPanelsWithKeyBindingsCount( handle );
	for ( int i = 0; i < c; ++i )
	{
		Panel *kbPanel = GetPanelWithKeyBindings( handle, i );
		Assert( kbPanel );
		if ( !kbPanel )
			continue;

		Assert( kbPanel->GetName() );
		Assert( kbPanel->GetName()[ 0 ] );

		if ( !kbPanel->GetName() || !kbPanel->GetName()[ 0 ] )
			continue;
	
		BufPrint( buf, 1, "\"%s\"\n", kbPanel->GetName() );
		BufPrint( buf, 1, "{\n" );

		kbPanel->SaveKeyBindingsToBuffer( 2, buf );

		BufPrint( buf, 1, "}\n" );
	}

	BufPrint( buf, 0, "}\n" );

	if ( g_pFullFileSystem->FileExists( filename, pathID ) &&
		!g_pFullFileSystem->IsFileWritable( filename, pathID ) )
	{
		Warning( "Panel::SaveKeyBindings '%s' is read-only!!!\n", filename );
	}

	FileHandle_t h = g_pFullFileSystem->Open( filename, "wb", pathID );
	if ( FILESYSTEM_INVALID_HANDLE != h )
	{
		g_pFullFileSystem->Write( buf.Base(), buf.TellPut(), h );
		g_pFullFileSystem->Close( h );
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : handle - 
//			*panelOfInterest - 
//-----------------------------------------------------------------------------
void Panel::LoadKeyBindingsForOnePanel( KeyBindingContextHandle_t handle, Panel *panelOfInterest )
{
	if ( !panelOfInterest )
		return;
	if ( !panelOfInterest->GetName() )
		return;
	if ( !panelOfInterest->GetName()[ 0 ] )
		return;

	char const *filename = g_KBMgr.GetKeyBindingsFile( handle );
	char const *pathID = g_KBMgr.GetKeyBindingsFilePathID( handle );

	KeyValues *kv = new KeyValues( "keybindings" );
	if ( kv->LoadFromFile( g_pFullFileSystem, filename, pathID ) )
	{
		int c = GetPanelsWithKeyBindingsCount( handle );
		for ( int i = 0; i < c; ++i )
		{
			Panel *kbPanel = GetPanelWithKeyBindings( handle, i );
			Assert( kbPanel );
		
			char const *panelName = kbPanel->GetName();
			if ( !panelName )
			{
				continue;
			}

			if ( Q_stricmp( panelOfInterest->GetName(), panelName ) )
				continue;

			KeyValues *subKey = kv->FindKey( panelName, false );
			if ( !subKey )
			{
				Warning( "Panel::ReloadKeyBindings:  Can't find entry for panel '%s'\n", panelName );
				continue;
			}
			
            kbPanel->ParseKeyBindings( subKey );
		}
	}
	kv->deleteThis();
}

//-----------------------------------------------------------------------------
// Purpose: static method.  Loads all key bindings again
// Input  :  - 
//-----------------------------------------------------------------------------

void Panel::ReloadKeyBindings( KeyBindingContextHandle_t handle )
{
	char const *filename = g_KBMgr.GetKeyBindingsFile( handle );
	char const *pathID = g_KBMgr.GetKeyBindingsFilePathID( handle );

	KeyValues *kv = new KeyValues( "keybindings" );
	if ( kv->LoadFromFile( g_pFullFileSystem, filename, pathID ) )
	{
		int c = GetPanelsWithKeyBindingsCount( handle );
		for ( int i = 0; i < c; ++i )
		{
			Panel *kbPanel = GetPanelWithKeyBindings( handle, i );
			Assert( kbPanel );
		
			char const *panelName = kbPanel->GetName();
			if ( !panelName )
			{
				continue;
			}

			KeyValues *subKey = kv->FindKey( panelName, false );
			if ( !subKey )
			{
				Warning( "Panel::ReloadKeyBindings:  Can't find entry for panel '%s'\n", panelName );
				continue;
			}
			
            kbPanel->ParseKeyBindings( subKey );
		}
	}
	kv->deleteThis();
}
#endif // VGUI_USEKEYBINDINGMAPS

DECLARE_BUILD_FACTORY( Panel );

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
Panel::Panel()
{
	Init(0, 0, 64, 24);
}

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
Panel::Panel(Panel *parent)
{
	Init(0, 0, 64, 24);
	SetParent(parent);
}

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
Panel::Panel(Panel *parent, const char *panelName)
{
	Init(0, 0, 64, 24);
	SetName(panelName);
	SetParent(parent);
	SetBuildModeEditable(true);
}

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
Panel::Panel( Panel *parent, const char *panelName, HScheme scheme )
{
	Init(0, 0, 64, 24);
	SetName(panelName);
	SetParent(parent);
	SetBuildModeEditable(true);
	SetScheme( scheme );
}

//-----------------------------------------------------------------------------
// Purpose: Setup
//-----------------------------------------------------------------------------
void Panel::Init( int x, int y, int wide, int tall )
{
	_panelName = NULL;

	// get ourselves an internal panel
	_vpanel = ivgui()->AllocPanel();
	ipanel()->Init(_vpanel, this);

	SetPos(x, y);
	SetSize(wide, tall);
	_flags.SetFlag( NEEDS_LAYOUT | NEEDS_SCHEME_UPDATE | NEEDS_DEFAULT_SETTINGS_APPLIED );
	_flags.SetFlag( AUTODELETE_ENABLED | PAINT_BORDER_ENABLED | PAINT_BACKGROUND_ENABLED | PAINT_ENABLED );
#if defined( VGUI_USEKEYBINDINGMAPS )
	_flags.SetFlag( ALLOW_CHAIN_KEYBINDING_TO_PARENT );
#endif
	m_nPinDeltaX = m_nPinDeltaY = 0;
	m_nResizeDeltaX = m_nResizeDeltaY = 0;
	_autoResizeDirection = AUTORESIZE_NO;
	_pinCorner = PIN_TOPLEFT;
	_cursor = dc_arrow;
	_border = NULL;
	_buildGroup = UTLHANDLE_INVALID;
	_tabPosition = 0;
	m_iScheme = 0;
	m_bIsSilent = false;

	_buildModeFlags = 0; // not editable or deletable in buildmode dialog by default

	m_pTooltips = NULL;

	m_flAlpha = 255.0f;
	m_nPaintBackgroundType = 0;
	m_nBgTextureId1 = -1;
	m_nBgTextureId2 = -1;
	m_nBgTextureId3 = -1;
	m_nBgTextureId4 = -1;
#if defined( VGUI_USEDRAGDROP )
	m_pDragDrop = new DragDrop_t;

#endif

	m_lLastDoublePressTime = 0L;

#if defined( VGUI_USEKEYBINDINGMAPS )
	m_hKeyBindingsContext = INVALID_KEYBINDINGCONTEXT_HANDLE;
#endif
}


//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
Panel::~Panel()
{
#if defined( VGUI_USEKEYBINDINGMAPS )
	if ( IsValidKeyBindingsContext() )
	{
		g_KBMgr.OnPanelDeleted( m_hKeyBindingsContext, this );
	}
#endif // VGUI_USEKEYBINDINGMAPS
#if defined( VGUI_USEDRAGDROP )
	if ( m_pDragDrop->m_bDragging )
	{
		OnFinishDragging( false, (MouseCode)-1 );
	}
#endif // VGUI_USEDRAGDROP

	_flags.ClearFlag( AUTODELETE_ENABLED );
	_flags.SetFlag( MARKED_FOR_DELETION );

	// remove panel from any list
	SetParent((VPANEL)NULL);

	// Stop our children from pointing at us, and delete them if possible
	while (ipanel()->GetChildCount(GetVPanel()))
	{
		VPANEL child = ipanel()->GetChild(GetVPanel(), 0);
		if (ipanel()->IsAutoDeleteSet(child))
		{
			ipanel()->DeletePanel(child);
		}
		else
		{
			ipanel()->SetParent(child, NULL);
		}
	}

	// delete VPanel
	ivgui()->FreePanel(_vpanel);
	// free our name
	delete [] _panelName;

	_vpanel = NULL;
#if defined( VGUI_USEDRAGDROP )
	delete m_pDragDrop;
#endif // VGUI_USEDRAGDROP
}

//-----------------------------------------------------------------------------
// Purpose: fully construct this panel so its ready for use right now (i.e fonts loaded, colors set, default label text set, ...)
//-----------------------------------------------------------------------------
void Panel::MakeReadyForUse()
{
//	PerformApplySchemeSettings();
	surface()->SolveTraverse( GetVPanel(), true );
}

	
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void Panel::SetName( const char *panelName )
{
	// No change?
	if ( _panelName && 
		panelName && 
		!Q_strcmp( _panelName, panelName ) )
	{
		return;
	}

	if (_panelName)
	{
		delete [] _panelName;
		_panelName = NULL;
	}

	if (panelName)
	{
		int len = Q_strlen(panelName) + 1;
		_panelName = new char[ len ];
		Q_strncpy( _panelName, panelName, len );
	}
}

//-----------------------------------------------------------------------------
// Purpose: returns the given name of the panel
//-----------------------------------------------------------------------------
const char *Panel::GetName()
{
	if (_panelName)
		return _panelName;

	return "";
}

//-----------------------------------------------------------------------------
// Purpose: returns the name of the module that this instance of panel was compiled into
//-----------------------------------------------------------------------------
const char *Panel::GetModuleName()
{
	return vgui::GetControlsModuleName();
}

//-----------------------------------------------------------------------------
// Purpose: returns the classname of the panel (as specified in the panelmaps)
//-----------------------------------------------------------------------------
const char *Panel::GetClassName()
{
	// loop up the panel map name
	PanelMessageMap *panelMap = GetMessageMap();
	if ( panelMap )
	{
		return panelMap->pfnClassName();
	}

	return "Panel";
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void Panel::SetPos(int x, int y)
{
	Assert( abs(x) < 32768 && abs(y) < 32768 );
	ipanel()->SetPos(GetVPanel(), x, y);
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void Panel::GetPos(int &x, int &y)
{
	ipanel()->GetPos(GetVPanel(), x, y);
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void Panel::SetSize(int wide, int tall)
{
	Assert( abs(wide) < 32768 && abs(tall) < 32768 );
	ipanel()->SetSize(GetVPanel(), wide, tall);
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void Panel::GetSize(int &wide, int &tall)
{
	ipanel()->GetSize(GetVPanel(), wide, tall);
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void Panel::SetBounds(int x, int y, int wide, int tall)
{
	SetPos(x,y);
	SetSize(wide,tall);
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void Panel::GetBounds(int &x, int &y, int &wide, int &tall)
{
	GetPos(x, y);
	GetSize(wide, tall);
}

//-----------------------------------------------------------------------------
// Purpose: returns safe handle to parent
//-----------------------------------------------------------------------------
VPANEL Panel::GetVParent()
{
	return ipanel()->GetParent(GetVPanel());
}

//-----------------------------------------------------------------------------
// Purpose: returns a pointer to a controls version of a Panel pointer
//-----------------------------------------------------------------------------
Panel *Panel::GetParent()
{
	// get the parent and convert it to a Panel *
	// this is OK, the hierarchy is guaranteed to be all from the same module, except for the root node
	// the root node always returns NULL when a GetParent() is done so everything is OK
	VPANEL parent = ipanel()->GetParent(GetVPanel());
	if (parent)
	{
		Panel *pParent = ipanel()->GetPanel(parent, GetControlsModuleName());
		Assert(!pParent || !strcmp(pParent->GetModuleName(), GetControlsModuleName()));
		return pParent;
	}

	return NULL;
}

//-----------------------------------------------------------------------------
// Purpose: Screen size change notification handler
//-----------------------------------------------------------------------------
void Panel::OnScreenSizeChanged(int nOldWide, int nOldTall)
{
	// post to all children
	for (int i = 0; i < ipanel()->GetChildCount(GetVPanel()); i++)
	{
		VPANEL child = ipanel()->GetChild(GetVPanel(), i);
		PostMessage(child, new KeyValues("OnScreenSizeChanged", "oldwide", nOldWide, "oldtall", nOldTall), NULL);
	}

	// make any currently fullsize window stay fullsize
	int x, y, wide, tall;
	GetBounds(x, y, wide, tall);
	int screenWide, screenTall;
	surface()->GetScreenSize(screenWide, screenTall);
	if (x == 0 && y == 0 && nOldWide == wide && tall == nOldTall)
	{
		// fullsize
		surface()->GetScreenSize(wide, tall);
		SetBounds(0, 0, wide, tall);
	}

	// panel needs to re-get it's scheme settings
	_flags.SetFlag( NEEDS_SCHEME_UPDATE );

	// invalidate our settings
	InvalidateLayout();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void Panel::SetVisible(bool state)
{
	ipanel()->SetVisible(GetVPanel(), state);
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool Panel::IsVisible()
{
	return ipanel()->IsVisible(GetVPanel());
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void Panel::SetEnabled(bool state)
{
	if (state != ipanel()->IsEnabled( GetVPanel()))
	{
		ipanel()->SetEnabled(GetVPanel(), state);
		InvalidateLayout(false);
		Repaint();
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool Panel::IsEnabled()
{
	return ipanel()->IsEnabled(GetVPanel());
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool Panel::IsPopup()
{
	return ipanel()->IsPopup(GetVPanel());
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void Panel::Repaint()
{
	_flags.SetFlag( NEEDS_REPAINT );
	surface()->Invalidate(GetVPanel());
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void Panel::Think()
{
	if (IsVisible())
	{	
		// update any tooltips
		if (m_pTooltips)
		{
			m_pTooltips->PerformLayout();
		}
		if ( _flags.IsFlagSet( NEEDS_LAYOUT ) )
		{
			InternalPerformLayout();
		}
	}

	OnThink();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void Panel::PaintTraverse( bool repaint, bool allowForce )
{
	if ( !IsVisible() )
	{
		return;
	}

	float oldAlphaMultiplier = surface()->DrawGetAlphaMultiplier();
	float newAlphaMultiplier = oldAlphaMultiplier * m_flAlpha * 1.0f/255.0f;

	if ( IsXbox() && !newAlphaMultiplier )
	{
		// xbox optimization not suitable for pc
		// xbox panels are compliant and can early out and not traverse their children
		// when they have no opacity
		return;
	}

	if( _flags.IsFlagSet( NEEDS_REPAINT ) && allowForce )
	{
		repaint = true;
	}

	if ( allowForce )
	{
		_flags.ClearFlag( NEEDS_REPAINT );
	}

	VPANEL vpanel = GetVPanel();


	int clipRect[4];
	ipanel()->GetClipRect( vpanel, clipRect[0], clipRect[1], clipRect[2], clipRect[3] );
	if ( ( clipRect[2] <= clipRect[0] ) || ( clipRect[3] <= clipRect[1] ) )
	{
		repaint = false;
	}

	// set global alpha
	surface()->DrawSetAlphaMultiplier( newAlphaMultiplier );
	if ( repaint && _flags.IsFlagSet( PAINT_BACKGROUND_ENABLED | PAINT_ENABLED ) )
	{
		// draw the background with no inset
		if ( _flags.IsFlagSet( PAINT_BACKGROUND_ENABLED ) )
		{
			surface()->PushMakeCurrent( vpanel, false );
			PaintBackground();
			surface()->PopMakeCurrent( vpanel );
		}

		// draw the front of the panel with the inset
		if ( _flags.IsFlagSet( PAINT_ENABLED ) )
		{
			surface()->PushMakeCurrent( vpanel, true );
			Paint();
			surface()->PopMakeCurrent( vpanel );
		}
	}

	// traverse and paint all our children
	int childCount = ipanel()->GetChildCount( vpanel );
	for (int i = 0; i < childCount; i++)
	{
		VPANEL child = ipanel()->GetChild( vpanel, i );
		if ( surface()->ShouldPaintChildPanel( child ) )
		{
			ipanel()->PaintTraverse( child, repaint, allowForce );
		}
		else
		{
			// Invalidate the child panel so that it gets redrawn
			surface()->Invalidate( child );

			// keep traversing the tree, just don't allow anyone to paint after here
			ipanel()->PaintTraverse( child, false, false );
		}
	}

	// draw the border last
	if ( repaint && _flags.IsFlagSet( PAINT_BORDER_ENABLED ) && ( _border != null ) )
	{
		// Paint the border over the background with no inset
		surface()->PushMakeCurrent( vpanel, false );
		PaintBorder();
		surface()->PopMakeCurrent( vpanel );
	}

	if ( repaint && IsBuildGroupEnabled() ) //&& HasFocus() )
	{
		// outline all selected panels 
		CUtlVector<PHandle> *controlGroup = _buildGroup->GetControlGroup();
		for (int i=0; i < controlGroup->Size(); ++i)
		{
			surface()->PushMakeCurrent( ((*controlGroup)[i].Get())->GetVPanel(), false );
			((*controlGroup)[i].Get())->PaintBuildOverlay();
			surface()->PopMakeCurrent( ((*controlGroup)[i].Get())->GetVPanel() );
		}	
		
		_buildGroup->DrawRulers();						
	}

	// All of our children have painted, etc, now allow painting in top of them
	if ( repaint && _flags.IsFlagSet( POST_CHILD_PAINT_ENABLED ) )
	{
		surface()->PushMakeCurrent( vpanel, false );
		PostChildPaint();
		surface()->PopMakeCurrent( vpanel );
	}

	surface()->DrawSetAlphaMultiplier( oldAlphaMultiplier );

	surface()->SwapBuffers( vpanel );
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void Panel::PaintBorder()
{
	_border->Paint(GetVPanel());
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void Panel::PaintBackground()
{ 
	int wide, tall;
	GetSize( wide, tall );
	if ( m_SkipChild.Get() && m_SkipChild->IsVisible() )
	{
		if ( GetPaintBackgroundType() == 2 )
		{
			int cornerWide, cornerTall;
			GetCornerTextureSize( cornerWide, cornerTall );

			Color col = GetBgColor();
			DrawHollowBox( 0, 0, wide, tall, col, 1.0f );

			wide -= 2 * cornerWide;
			tall -= 2 * cornerTall;

			FillRectSkippingPanel( GetBgColor(), cornerWide, cornerTall, wide, tall, m_SkipChild.Get() );
		}
		else
		{
			FillRectSkippingPanel( GetBgColor(), 0, 0, wide, tall, m_SkipChild.Get() );
		}
	}
	else
	{
		Color col = GetBgColor();

		switch ( m_nPaintBackgroundType )
		{
		default:
		case 0:
			{
				surface()->DrawSetColor(col);
				surface()->DrawFilledRect(0, 0, wide, tall);
			}
			break;
		case 1:
			{
				DrawTexturedBox( 0, 0, wide, tall, col, 1.0f );
			}
			break;
		case 2:
			{
				DrawBox( 0, 0, wide, tall, col, 1.0f );
			}
			break;
		case 3:
			{
				DrawBoxFade( 0, 0, wide, tall, col, 1.0f, 255, 0, true );
			}
			break;
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void Panel::Paint()
{
	// empty on purpose
	// PaintBackground is painted and default behavior is for Paint to do nothing
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void Panel::PostChildPaint()
{
	// Empty on purpose
	// This is called if _postChildPaintEnabled is true and allows painting to
	//  continue on the surface after all of the panel's children have painted 
	//  themselves.  Allows drawing an overlay on top of the children, etc.
}

//-----------------------------------------------------------------------------
// Purpose: Draws a black rectangle around the panel.
//-----------------------------------------------------------------------------
void Panel::PaintBuildOverlay()
{
	int wide,tall;
	GetSize(wide,tall);
	surface()->DrawSetColor(0, 0, 0, 255);

	surface()->DrawFilledRect(0,0,wide,2);           //top
	surface()->DrawFilledRect(0,tall-2,wide,tall);   //bottom
	surface()->DrawFilledRect(0,2,2,tall-2);         //left
	surface()->DrawFilledRect(wide-2,2,wide,tall-2); //right
}

//-----------------------------------------------------------------------------
// Purpose: Returns true if the panel's draw code will fully cover it's area
//-----------------------------------------------------------------------------
bool Panel::IsOpaque()
{
	// FIXME: Add code to account for the 'SkipChild' functionality in Frame
	if ( IsVisible() && _flags.IsFlagSet( PAINT_BACKGROUND_ENABLED ) && ( _bgColor[3] == 255 ) )
		return true;

	return false;
}

//-----------------------------------------------------------------------------
// Purpose: returns true if the settings are aligned to the right of the screen
//-----------------------------------------------------------------------------
bool Panel::IsRightAligned()
{
	return (_buildModeFlags & BUILDMODE_SAVE_XPOS_RIGHTALIGNED);
}

//-----------------------------------------------------------------------------
// Purpose: returns true if the settings are aligned to the bottom of the screen
//-----------------------------------------------------------------------------
bool Panel::IsBottomAligned()
{
	return (_buildModeFlags & BUILDMODE_SAVE_YPOS_BOTTOMALIGNED);
}

//-----------------------------------------------------------------------------
// Purpose: sets the parent
//-----------------------------------------------------------------------------
void Panel::SetParent(Panel *newParent)
{
	// Assert that the parent is from the same module as the child
	// FIXME: !!! work out how to handle this properly!
//	Assert(!newParent || !strcmp(newParent->GetModuleName(), GetControlsModuleName()));

	if (newParent)
	{
		SetParent(newParent->GetVPanel());
	}
	else
	{
		SetParent((VPANEL)NULL);
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void Panel::SetParent(VPANEL newParent)
{
	if (newParent)
	{
		ipanel()->SetParent(GetVPanel(), newParent);
	}
	else
	{
		ipanel()->SetParent(GetVPanel(), NULL);
	}

	if (GetVParent() && !IsPopup())
	{
		SetProportional(ipanel()->IsProportional(GetVParent()));

		// most of the time KBInput == parents kbinput
		if (ipanel()->IsKeyBoardInputEnabled(GetVParent()) != IsKeyBoardInputEnabled())
		{
			SetKeyBoardInputEnabled(ipanel()->IsKeyBoardInputEnabled(GetVParent()));
		}

		if (ipanel()->IsMouseInputEnabled(GetVParent()) != IsMouseInputEnabled())
		{
			SetMouseInputEnabled(ipanel()->IsMouseInputEnabled(GetVParent()));
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void Panel::OnChildAdded(VPANEL child)
{
	Assert( !_flags.IsFlagSet( IN_PERFORM_LAYOUT ) );
}

//-----------------------------------------------------------------------------
// Purpose: default message handler
//-----------------------------------------------------------------------------
void Panel::OnSizeChanged(int newWide, int newTall)
{
	InvalidateLayout(); // our size changed so force us to layout again
}

//-----------------------------------------------------------------------------
// Purpose: sets Z ordering - lower numbers are always behind higher z's
//-----------------------------------------------------------------------------
void Panel::SetZPos(int z)
{
	ipanel()->SetZPos(GetVPanel(), z);
}

//-----------------------------------------------------------------------------
// Purpose: sets alpha modifier for panel and all child panels [0..255]
//-----------------------------------------------------------------------------
void Panel::SetAlpha(int alpha)
{
	m_flAlpha = alpha;
}

//-----------------------------------------------------------------------------
// Purpose: data accessor
//-----------------------------------------------------------------------------
int Panel::GetAlpha()
{
	return (int)m_flAlpha;
}

//-----------------------------------------------------------------------------
// Purpose: Moves the panel to the front of the z-order
//-----------------------------------------------------------------------------
void Panel::MoveToFront(void)
{
	// FIXME: only use ipanel() as per src branch?
	if (IsPopup())
	{
		surface()->BringToFront(GetVPanel());
	}
	else
	{
		ipanel()->MoveToFront(GetVPanel());
	}
}

//-----------------------------------------------------------------------------
// Purpose: Iterates up the hierarchy looking for a particular parent
//-----------------------------------------------------------------------------
bool Panel::HasParent(VPANEL potentialParent)
{
	if (!potentialParent)
		return false;

	return ipanel()->HasParent(GetVPanel(), potentialParent);
}

//-----------------------------------------------------------------------------
// Purpose: Finds the index of a child panel by string name
// Output : int - -1 if no panel of that name is found
//-----------------------------------------------------------------------------
int Panel::FindChildIndexByName(const char *childName)
{
	for (int i = 0; i < GetChildCount(); i++)
	{
		Panel *pChild = GetChild(i);
		if (!pChild)
			continue;

		if (!stricmp(pChild->GetName(), childName))
		{
			return i;
		}
	}

	return -1;
}

//-----------------------------------------------------------------------------
// Purpose: Finds a child panel by string name
// Output : Panel * - NULL if no panel of that name is found
//-----------------------------------------------------------------------------
Panel *Panel::FindChildByName(const char *childName, bool recurseDown)
{
	for (int i = 0; i < GetChildCount(); i++)
	{
		Panel *pChild = GetChild(i);
		if (!pChild)
			continue;

		if (!stricmp(pChild->GetName(), childName))
		{
			return pChild;
		}

		if (recurseDown)
		{
			Panel *panel = pChild->FindChildByName(childName, recurseDown);
			if ( panel )
			{
				return panel;
			}
		}
	}

	return NULL;
}

//-----------------------------------------------------------------------------
// Purpose: Finds a sibling panel by name
//-----------------------------------------------------------------------------
Panel *Panel::FindSiblingByName(const char *siblingName)
{
	int siblingCount = ipanel()->GetChildCount(GetVParent());
	for (int i = 0; i < siblingCount; i++)
	{
		VPANEL sibling = ipanel()->GetChild(GetVParent(), i);
		Panel *panel = ipanel()->GetPanel(sibling, GetControlsModuleName());
		if (!stricmp(panel->GetName(), siblingName))
		{
			return panel;
		}
	}

	return NULL;
}

//-----------------------------------------------------------------------------
// Purpose: Dispatches immediately a message to the parent
//-----------------------------------------------------------------------------
void Panel::CallParentFunction(KeyValues *message)
{
	if (GetVParent())
	{
		ipanel()->SendMessage(GetVParent(), message, GetVPanel());
	}
	if (message)
	{
		message->deleteThis();
	}
}


//-----------------------------------------------------------------------------
// Purpose: if set to true, panel automatically frees itself when parent is deleted
//-----------------------------------------------------------------------------
void Panel::SetAutoDelete( bool state )
{
	_flags.SetFlag( AUTODELETE_ENABLED, state );
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool Panel::IsAutoDeleteSet()
{
	return _flags.IsFlagSet( AUTODELETE_ENABLED );
}


//-----------------------------------------------------------------------------
// Purpose: Just calls 'delete this'
//-----------------------------------------------------------------------------
void Panel::DeletePanel()
{
	// Avoid re-entrancy
	_flags.SetFlag( MARKED_FOR_DELETION );
	_flags.ClearFlag( AUTODELETE_ENABLED );
	delete this;
}

//-----------------------------------------------------------------------------
// Purpose: data accessor
//-----------------------------------------------------------------------------
HScheme Panel::GetScheme()
{
	if (m_iScheme)
	{
		return m_iScheme; // return our internal scheme
	}
	
	if (GetVParent()) // recurse down the heirarchy 
	{
		return ipanel()->GetScheme(GetVParent());
	}

	return scheme()->GetDefaultScheme();
}

//-----------------------------------------------------------------------------
// Purpose: set the scheme to render this panel with by name
//-----------------------------------------------------------------------------
void Panel::SetScheme(const char *tag)
{
	if (strlen(tag) > 0 && scheme()->GetScheme(tag)) // check the scheme exists
	{
		SetScheme(scheme()->GetScheme(tag));
	}
}

//-----------------------------------------------------------------------------
// Purpose: set the scheme to render this panel with 
//-----------------------------------------------------------------------------
void Panel::SetScheme(HScheme scheme)
{
	if (scheme != m_iScheme)
	{
		m_iScheme = scheme;

		// This will cause the new scheme to be applied at a later point
//		InvalidateLayout( false, true );
	}
}


//-----------------------------------------------------------------------------
// Purpose: returns the char of this panels hotkey
//-----------------------------------------------------------------------------
Panel *Panel::HasHotkey(wchar_t key)
{
	return NULL;
}

#if defined( VGUI_USEDRAGDROP )
static vgui::PHandle	g_DragDropCapture;
#endif // VGUI_USEDRAGDROP

void Panel::InternalCursorMoved(int x, int y)
{
#if defined( VGUI_USEDRAGDROP )
	if ( g_DragDropCapture.Get() )
	{
		bool started = g_DragDropCapture->GetDragDropInfo()->m_bDragStarted;

		g_DragDropCapture->OnContinueDragging();

		if ( started )
		{
			bool isEscapeKeyDown = input()->IsKeyDown( KEY_ESCAPE );
			if ( isEscapeKeyDown )
			{
				g_DragDropCapture->OnFinishDragging( true, (MouseCode)-1, true );
			}
			return;
		}
	}
#endif // VGUI_USEDRAGDROP

	if ( !ShouldHandleInputMessage() )
		return;

	if ( IsCursorNone() )
		return;
	
	if ( !IsMouseInputEnabled() )
	{
		return;
	}

	if (IsBuildGroupEnabled())
	{
		if ( _buildGroup->CursorMoved(x, y, this) )
		{
			return;
		}
	}

	if (m_pTooltips)
	{
		m_pTooltips->ShowTooltip(this);
	}

	ScreenToLocal(x, y);

	OnCursorMoved(x, y);
}

void Panel::InternalCursorEntered()
{
	if (IsCursorNone() || !IsMouseInputEnabled())
		return;
	
	if (IsBuildGroupEnabled())
		return;

	if (m_pTooltips)
	{
		m_pTooltips->ResetDelay();
		m_pTooltips->ShowTooltip(this);
	}

	OnCursorEntered();
}

void Panel::InternalCursorExited()
{
	if (IsCursorNone() || !IsMouseInputEnabled())
		return;
	
	if (IsBuildGroupEnabled())
		return;

	if (m_pTooltips)
	{
		m_pTooltips->HideTooltip();
	}

	OnCursorExited();
}

bool Panel::IsChildOfSurfaceModalPanel()
{
	VPANEL appModalPanel = input()->GetAppModalSurface();
	if ( !appModalPanel )
		return true;

	if ( ipanel()->HasParent( GetVPanel(), appModalPanel ) )
		return true;

	return false;
}


//-----------------------------------------------------------------------------
// Purpose: 
// Input  :  - 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool Panel::IsChildOfModalSubTree()
{
	VPANEL subTree = input()->GetModalSubTree();
	if ( !subTree )
		return true;

	if ( HasParent( subTree ) )
		return true;

	return false;
}

//-----------------------------------------------------------------------------
// Purpose: Checks to see if message is being subverted due to modal subtree logic
// Input  :  - 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
static bool ShouldHandleInputMessage( VPANEL p )
{
	// If there is not modal subtree, then always handle the msg
	if ( !input()->GetModalSubTree() )
		return true;

	// What state are we in?
	bool bChildOfModal = false;
	VPANEL subTree = input()->GetModalSubTree();
	if ( !subTree )
	{
		bChildOfModal = true;
	}
	else if ( ipanel()->HasParent( p, subTree ) )
	{
		bChildOfModal = true;
	}

	if ( input()->ShouldModalSubTreeReceiveMessages() )
		return bChildOfModal;

	return !bChildOfModal;
}

bool Panel::ShouldHandleInputMessage()
{
	return ::ShouldHandleInputMessage( GetVPanel() );
}

void Panel::InternalMousePressed(int code)
{
	long curtime = system()->GetTimeMillis();
	if ( IsTriplePressAllowed() )
	{
		long elapsed = curtime - m_lLastDoublePressTime;
		if ( elapsed < TRIPLE_PRESS_MSEC )
		{
			InternalMouseTriplePressed( code );
			return;
		}
	}

	// The menu system passively watches for mouse released messages so it 
	// can clear any open menus if the release is somewhere other than on a menu
	Menu::OnInternalMousePressed( this, (MouseCode)code );

	if ( !ShouldHandleInputMessage() )
		return;

	if ( IsCursorNone() )
		return;
	
	if ( !IsMouseInputEnabled())
	{
#if defined( VGUI_USEDRAGDROP )
		DragDropStartDragging();
#endif
		return;
	}
	
	if (IsBuildGroupEnabled())
	{
		if ( _buildGroup->MousePressed((MouseCode)code, this) )
		{
			return;
		}
	}
	
	OnMousePressed((MouseCode)code);
#if defined( VGUI_USEDRAGDROP )
	DragDropStartDragging();
#endif
}

void Panel::InternalMouseDoublePressed(int code)
{
	m_lLastDoublePressTime = system()->GetTimeMillis();

	if ( !ShouldHandleInputMessage() )
		return;

	if ( IsCursorNone() )
		return;
	
	if ( !IsMouseInputEnabled())
	{
#if defined( VGUI_USEDRAGDROP )
		DragDropStartDragging();
#endif
		return;
	}
	
	if (IsBuildGroupEnabled())
	{
		if ( _buildGroup->MouseDoublePressed((MouseCode)code, this) )
		{
			return;
		}
	}

	OnMouseDoublePressed((MouseCode)code);
#if defined( VGUI_USEDRAGDROP )
	DragDropStartDragging();
#endif
}

#if defined( VGUI_USEDRAGDROP )
void Panel::SetStartDragWhenMouseExitsPanel( bool state )
{
	_flags.SetFlag( DRAG_REQUIRES_PANEL_EXIT, state );
}

bool Panel::IsStartDragWhenMouseExitsPanel() const
{
	return 	_flags.IsFlagSet( DRAG_REQUIRES_PANEL_EXIT );
}
#endif // VGUI_USEDRAGDROP

void Panel::SetTriplePressAllowed( bool state )
{
	_flags.SetFlag( TRIPLE_PRESS_ALLOWED, state );
}

bool Panel::IsTriplePressAllowed() const
{
	return 	_flags.IsFlagSet( TRIPLE_PRESS_ALLOWED );
}

void Panel::InternalMouseTriplePressed( int code )
{
	Assert( IsTriplePressAllowed() );
	m_lLastDoublePressTime = 0L;

	if ( !ShouldHandleInputMessage() )
		return;

	if ( IsCursorNone() )
		return;
	
	if ( !IsMouseInputEnabled())
	{
#if defined( VGUI_USEDRAGDROP )
		DragDropStartDragging();
#endif
		return;
	}
	
	if (IsBuildGroupEnabled())
	{
		return;
	}

	OnMouseTriplePressed((MouseCode)code);
#if defined( VGUI_USEDRAGDROP )
	DragDropStartDragging();
#endif
}

void Panel::InternalMouseReleased(int code)
{
#if defined( VGUI_USEDRAGDROP )
	if ( g_DragDropCapture.Get() )
	{
		bool started = g_DragDropCapture->GetDragDropInfo()->m_bDragStarted;
		g_DragDropCapture->OnFinishDragging( true, (MouseCode)code );
		if ( started )
		{
			return;
		}
	}
#endif

	if ( !ShouldHandleInputMessage() )
		return;

	if ( IsCursorNone() )
		return;
	
	if ( !IsMouseInputEnabled())
	{
		return;
	}
	
	if (IsBuildGroupEnabled())
	{
		if ( _buildGroup->MouseReleased((MouseCode)code, this) )
		{
			return;
		}
	}

	OnMouseReleased((MouseCode)code);
}

void Panel::InternalMouseWheeled(int delta)
{
	if (IsBuildGroupEnabled() || !IsMouseInputEnabled())
	{
		return;
	}

	if ( !ShouldHandleInputMessage() )
		return;

	OnMouseWheeled(delta);
}

void Panel::InternalKeyCodePressed(int code)
{
	if ( !ShouldHandleInputMessage() )
		return;

	if (IsKeyBoardInputEnabled()) 
	{
		OnKeyCodePressed((KeyCode)code);
	}
	else
	{
		CallParentFunction(new KeyValues("KeyCodePressed", "code", code));
	}
}

#if defined( VGUI_USEKEYBINDINGMAPS )
//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *bindingName - 
//			keycode - 
//			modifiers - 
//-----------------------------------------------------------------------------
void Panel::AddKeyBinding( char const *bindingName, int keycode, int modifiers )
{
	PanelKeyBindingMap *map = LookupMapForBinding( bindingName );
	if ( !map )
	{
		Assert( 0 );
		return;
	}		

	BoundKey_t kb;																	
	kb.isbuiltin = false;															
	kb.bindingname = CopyString( bindingName );												
	kb.keycode = keycode;															
	kb.modifiers = modifiers;														

	map->boundkeys.AddToTail( kb );													
}

KeyBindingMap_t *Panel::LookupBinding( char const *bindingName )
{
	PanelKeyBindingMap *map = GetKBMap();
	while( map )
	{
		int c = map->entries.Count();
		for( int i = 0; i < c ; ++i )
		{
			KeyBindingMap_t *binding = &map->entries[ i ];
			if ( !Q_stricmp( binding->bindingname, bindingName ) )
				return binding;
		}

		map = map->baseMap;
	}

	return NULL;
}

PanelKeyBindingMap *Panel::LookupMapForBinding( char const *bindingName )
{
	PanelKeyBindingMap *map = GetKBMap();
	while( map )
	{
		int c = map->entries.Count();
		for( int i = 0; i < c ; ++i )
		{
			KeyBindingMap_t *binding = &map->entries[ i ];
			if ( !Q_stricmp( binding->bindingname, bindingName ) )
				return map;
		}

		map = map->baseMap;
	}

	return NULL;
}

KeyBindingMap_t *Panel::LookupBindingByKeyCode( KeyCode code, int modifiers )
{
	PanelKeyBindingMap *map = GetKBMap();
	while( map )
	{
		int c = map->boundkeys.Count();
		for( int i = 0; i < c ; ++i )
		{
			BoundKey_t *kb = &map->boundkeys[ i ];
			if ( kb->keycode == code && kb->modifiers == modifiers )
			{
				KeyBindingMap_t *binding = LookupBinding( kb->bindingname );
				Assert( binding );
				if ( binding )
				{
					return binding;
				}
			}
		}

		map = map->baseMap;
	}

	return NULL;
}

BoundKey_t *Panel::LookupDefaultKey( char const *bindingName )
{
	PanelKeyBindingMap *map = GetKBMap();
	while( map )
	{
		int c = map->defaultkeys.Count();
		for( int i = 0; i < c ; ++i )
		{
			BoundKey_t *kb = &map->defaultkeys[ i ];
			if ( !Q_stricmp( kb->bindingname, bindingName ) )
			{
				return kb;
			}
		}

		map = map->baseMap;
	}
	return NULL;
}

void Panel::LookupBoundKeys( char const *bindingName, CUtlVector< BoundKey_t * >& list )
{
	PanelKeyBindingMap *map = GetKBMap();
	while( map )
	{
		int c = map->boundkeys.Count();
		for( int i = 0; i < c ; ++i )
		{
			BoundKey_t *kb = &map->boundkeys[ i ];
			if ( !Q_stricmp( kb->bindingname, bindingName ) )
			{
				list.AddToTail( kb );
			}
		}

		map = map->baseMap;
	}
}

void Panel::RevertKeyBindingsToDefault()
{
	PanelKeyBindingMap *map = GetKBMap();
	while( map )
	{
		map->boundkeys.RemoveAll();
		map->boundkeys = map->defaultkeys;

		map = map->baseMap;
	}
}

void Panel::RemoveAllKeyBindings()
{
	PanelKeyBindingMap *map = GetKBMap();
	while( map )
	{
		map->boundkeys.RemoveAll();
		map = map->baseMap;
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  :  - 
//-----------------------------------------------------------------------------
void Panel::ReloadKeyBindings()
{
	RevertKeyBindingsToDefault();
	LoadKeyBindingsForOnePanel( GetKeyBindingsContext(), this );
}

#define MAKE_STRING( x )	#x
#define KEY_NAME( str, disp )	{ KEY_##str, MAKE_STRING( KEY_##str ), disp }

struct KeyNames_t
{
	KeyCode		code;
	char const	*string;
	char const	*displaystring;
};

static KeyNames_t g_KeyNames[] =
{
KEY_NAME( NONE, "None" ),
KEY_NAME( 0, "0" ),
KEY_NAME( 1, "1" ),
KEY_NAME( 2, "2" ),
KEY_NAME( 3, "3" ),
KEY_NAME( 4, "4" ),
KEY_NAME( 5, "5" ),
KEY_NAME( 6, "6" ),
KEY_NAME( 7, "7" ),
KEY_NAME( 8, "8" ),
KEY_NAME( 9, "9" ),
KEY_NAME( A, "A" ),
KEY_NAME( B, "B" ),
KEY_NAME( C, "C" ),
KEY_NAME( D, "D" ),
KEY_NAME( E, "E" ),
KEY_NAME( F, "F" ),
KEY_NAME( G, "G" ),
KEY_NAME( H, "H" ),
KEY_NAME( I, "I" ),
KEY_NAME( J, "J" ),
KEY_NAME( K, "K" ),
KEY_NAME( L, "L" ),
KEY_NAME( M, "M" ),
KEY_NAME( N, "N" ),
KEY_NAME( O, "O" ),
KEY_NAME( P, "P" ),
KEY_NAME( Q, "Q" ),
KEY_NAME( R, "R" ),
KEY_NAME( S, "S" ),
KEY_NAME( T, "T" ),
KEY_NAME( U, "U" ),
KEY_NAME( V, "V" ),
KEY_NAME( W, "W" ),
KEY_NAME( X, "X" ),
KEY_NAME( Y, "Y" ),
KEY_NAME( Z, "Z" ),
KEY_NAME( PAD_0, "Key Pad 0" ),
KEY_NAME( PAD_1, "Key Pad 1" ),
KEY_NAME( PAD_2, "Key Pad 2" ),
KEY_NAME( PAD_3, "Key Pad 3" ),
KEY_NAME( PAD_4, "Key Pad 4" ),
KEY_NAME( PAD_5, "Key Pad 5" ),
KEY_NAME( PAD_6, "Key Pad 6" ),
KEY_NAME( PAD_7, "Key Pad 7" ),
KEY_NAME( PAD_8, "Key Pad 8" ),
KEY_NAME( PAD_9, "Key Pad 9" ),
KEY_NAME( PAD_DIVIDE, "Key Pad /" ),
KEY_NAME( PAD_MULTIPLY, "Key Pad *" ),
KEY_NAME( PAD_MINUS, "Key Pad -" ),
KEY_NAME( PAD_PLUS, "Key Pad +" ),
KEY_NAME( PAD_ENTER, "Key Pad Enter" ),
KEY_NAME( PAD_DECIMAL, "Key Pad ." ),
KEY_NAME( LBRACKET, "[" ),
KEY_NAME( RBRACKET, "]" ),
KEY_NAME( SEMICOLON, "," ),
KEY_NAME( APOSTROPHE, "'" ),
KEY_NAME( BACKQUOTE, "`" ),
KEY_NAME( COMMA, "," ),
KEY_NAME( PERIOD, "." ),
KEY_NAME( SLASH, "/" ),
KEY_NAME( BACKSLASH, "\\" ),
KEY_NAME( MINUS, "-" ),
KEY_NAME( EQUAL, "=" ),
KEY_NAME( ENTER, "Enter" ),
KEY_NAME( SPACE, "Space" ),
KEY_NAME( BACKSPACE, "Backspace" ),
KEY_NAME( TAB, "Tab" ),
KEY_NAME( CAPSLOCK, "Caps Lock" ),
KEY_NAME( NUMLOCK, "Num Lock" ),
KEY_NAME( ESCAPE, "Escape" ),
KEY_NAME( SCROLLLOCK, "Scroll Lock" ),
KEY_NAME( INSERT, "Ins" ),
KEY_NAME( DELETE, "Del" ),
KEY_NAME( HOME, "Home" ),
KEY_NAME( END, "End" ),
KEY_NAME( PAGEUP, "PgUp" ),
KEY_NAME( PAGEDOWN, "PdDn" ),
KEY_NAME( BREAK, "Break" ),
KEY_NAME( LSHIFT, "Shift" ),
KEY_NAME( RSHIFT, "Shift" ),
KEY_NAME( LALT, "Alt" ),
KEY_NAME( RALT, "Alt" ),
KEY_NAME( LCONTROL, "Ctrl" ),
KEY_NAME( RCONTROL, "Ctrl" ),
KEY_NAME( LWIN, "Windows" ),
KEY_NAME( RWIN, "Windows" ),
KEY_NAME( APP, "App" ),
KEY_NAME( UP, "Up" ),
KEY_NAME( LEFT, "Left" ),
KEY_NAME( DOWN, "Down" ),
KEY_NAME( RIGHT, "Right" ),
KEY_NAME( F1, "F1" ),
KEY_NAME( F2, "F2" ),
KEY_NAME( F3, "F3" ),
KEY_NAME( F4, "F4" ),
KEY_NAME( F5, "F5" ),
KEY_NAME( F6, "F6" ),
KEY_NAME( F7, "F7" ),
KEY_NAME( F8, "F8" ),
KEY_NAME( F9, "F9" ),
KEY_NAME( F10, "F10" ),
KEY_NAME( F11, "F11" ),
KEY_NAME( F12, "F12" ),
KEY_NAME( CAPSLOCKTOGGLE, "Caps Lock Toggle" ),
KEY_NAME( NUMLOCKTOGGLE, "Num Lock Toggle" ),
KEY_NAME( SCROLLLOCKTOGGLE, "Scroll Lock Toggle" ),
};

char const *Panel::KeyCodeToString( KeyCode code )
{
	int c = ARRAYSIZE( g_KeyNames );
	for ( int i = 0; i < c ; ++i )
	{
		if ( g_KeyNames[ i ].code == code )
			return g_KeyNames[ i ].string;
	}

	return "";
}

wchar_t const *Panel::KeyCodeToDisplayString( KeyCode code )
{
	int c = ARRAYSIZE( g_KeyNames );
	for ( int i = 0; i < c ; ++i )
	{
		if ( g_KeyNames[ i ].code == code )
		{
			char const *str = g_KeyNames[ i ].displaystring;
			wchar_t *wstr = g_pVGuiLocalize->Find( str );
			if ( wstr )
			{
				return wstr;
			}

			static wchar_t buf[ 64 ];
			g_pVGuiLocalize->ConvertANSIToUnicode( str, buf, sizeof( buf ) );
			return buf;
		}
	}

	return L"";
}

static void AddModifierToString( char const *modifiername, char *buf, size_t bufsize )
{
	char add[ 32 ];
	if ( Q_strlen( buf ) > 0 )
	{
		Q_snprintf( add, sizeof( add ), "+%s", modifiername );
	}
	else
	{
		Q_strncpy( add, modifiername, sizeof( add ) );
	}

	Q_strncat( buf, add, bufsize, COPY_ALL_CHARACTERS );
		
}

wchar_t const *Panel::KeyCodeModifiersToDisplayString( KeyCode code, int modifiers )
{
	char sz[ 256 ];
	sz[ 0 ] = 0;

	if ( modifiers & MODIFIER_SHIFT )
	{
		AddModifierToString( "Shift", sz, sizeof( sz ) );
	}
	if ( modifiers & MODIFIER_CONTROL )
	{
		AddModifierToString( "Ctrl", sz, sizeof( sz ) );
	}
	if ( modifiers & MODIFIER_ALT )
	{
		AddModifierToString( "Alt", sz, sizeof( sz ) );
	}

	if ( Q_strlen( sz ) > 0 )
	{
		Q_strncat( sz, "+", sizeof( sz ), COPY_ALL_CHARACTERS );
	}

	static wchar_t unicode[ 256 ];
	_snwprintf( unicode, 255, L"%S%s", sz, Panel::KeyCodeToDisplayString( (KeyCode)code ) );
	return unicode;
}

KeyCode Panel::StringToKeyCode( char const *str )
{
	int c = ARRAYSIZE( g_KeyNames );
	for ( int i = 0; i < c ; ++i )
	{
		if ( !Q_stricmp( str, g_KeyNames[ i ].string ) )
			return g_KeyNames[ i ].code;
	}

	return KEY_NONE;
}

static void WriteKeyBindingToBuffer( CUtlBuffer& buf, int level, const BoundKey_t& binding )
{
	BufPrint( buf, level, "\"keycode\"\t\"%s\"\n", Panel::KeyCodeToString( (KeyCode)binding.keycode ) );
	if ( binding.modifiers & MODIFIER_SHIFT )
	{
		BufPrint( buf, level, "\"shift\"\t\"1\"\n" );
	}
	if ( binding.modifiers & MODIFIER_CONTROL )
	{
		BufPrint( buf, level, "\"ctrl\"\t\"1\"\n" );
	}
	if ( binding.modifiers & MODIFIER_ALT )
	{
		BufPrint( buf, level, "\"alt\"\t\"1\"\n" );
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *filename - 
//			*pathID - 
//-----------------------------------------------------------------------------
void Panel::SaveKeyBindingsToBuffer( int level, CUtlBuffer& buf  )
{
	Assert( IsValidKeyBindingsContext() );

	Assert( buf.IsText() );

	PanelKeyBindingMap *map = GetKBMap();
	while( map )
	{
		int c = map->boundkeys.Count();
		for( int i = 0; i < c ; ++i )
		{
			const BoundKey_t& binding = map->boundkeys[ i ];

			// Spew to file
			BufPrint( buf, level, "\"%s\"\n", binding.bindingname );
			BufPrint( buf, level, "{\n" );
	
			WriteKeyBindingToBuffer( buf, level + 1, binding );

			BufPrint( buf, level, "}\n" );
		}

		map = map->baseMap;
	}
}

bool Panel::ParseKeyBindings( KeyValues *kv )
{
	Assert( IsValidKeyBindingsContext() );
	if ( !IsValidKeyBindingsContext() )
		return false;

	// To have KB the panel must have a name
	Assert( GetName() && GetName()[ 0 ] );
	if ( !GetName() || !GetName()[ 0 ] )
		return false;

	bool success = false;

	g_KBMgr.AddPanelToContext( GetKeyBindingsContext(), this );

	RemoveAllKeyBindings();

	// Walk through bindings
	for ( KeyValues *binding = kv->GetFirstSubKey(); binding != NULL; binding = binding->GetNextKey() )
	{
		char const *bindingName = binding->GetName();
		if ( !bindingName || !bindingName[ 0 ] )
			continue;

		KeyBindingMap_t *b = LookupBinding( bindingName );
		if ( b )
		{
			success = true;
			const char *keycode = binding->GetString( "keycode", "" );
			int modifiers = 0;
			if ( binding->GetInt( "shift", 0 ) != 0 )
			{
				modifiers |= MODIFIER_SHIFT;
			}
			if ( binding->GetInt( "ctrl", 0 ) != 0 )
			{
				modifiers |= MODIFIER_CONTROL;
			}
			if ( binding->GetInt( "alt", 0 ) != 0 )
			{
				modifiers |= MODIFIER_ALT;
			}

			KeyBindingMap_t *bound = LookupBindingByKeyCode( StringToKeyCode( keycode ), modifiers );
			if ( !bound )
			{
				AddKeyBinding( bindingName, StringToKeyCode( keycode ), modifiers );
			}
		}
		else
		{
			Warning( "KeyBinding for panel '%s' contained unknown binding '%s'\n", GetName() ? GetName() : "???", bindingName );
		}
	}

	// Now for each binding which is currently "unbound" to any key, use the default binding
	PanelKeyBindingMap *map = GetKBMap();
	while( map )
	{
		int c = map->entries.Count();
		for( int i = 0; i < c ; ++i )
		{
			KeyBindingMap_t *binding = &map->entries[ i ];
			
			// See if there is a bound key
			CUtlVector< BoundKey_t * > list;
			LookupBoundKeys( binding->bindingname, list );
			if ( list.Count() == 0 )
			{
				// Assign the default binding to this key
				BoundKey_t *defaultKey = LookupDefaultKey( binding->bindingname );
				if ( defaultKey )
				{
					KeyBindingMap_t *alreadyBound = LookupBindingByKeyCode( (KeyCode)defaultKey->keycode, defaultKey->modifiers );
					if ( alreadyBound )
					{
						Warning( "No binding for '%s', defautl key already bound to '%s'\n", binding->bindingname, alreadyBound->bindingname );
					}
					else
					{
						AddKeyBinding( defaultKey->bindingname, defaultKey->keycode, defaultKey->modifiers );
					}
				}
			}
		}

		map = map->baseMap;
	}

	return success;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : handle - 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
void Panel::SetKeyBindingsContext( KeyBindingContextHandle_t handle )
{
	Assert( !IsValidKeyBindingsContext() || handle == GetKeyBindingsContext() );
	g_KBMgr.AddPanelToContext( handle, this );
	m_hKeyBindingsContext = handle;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  :  - 
// Output : KeyBindingContextHandle_t
//-----------------------------------------------------------------------------
KeyBindingContextHandle_t Panel::GetKeyBindingsContext() const
{
	return m_hKeyBindingsContext;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  :  - 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool Panel::IsValidKeyBindingsContext() const
{
	return GetKeyBindingsContext() != INVALID_KEYBINDINGCONTEXT_HANDLE;
}

char const *Panel::GetKeyBindingsFile() const
{
	Assert( IsValidKeyBindingsContext() );
	return g_KBMgr.GetKeyBindingsFile( GetKeyBindingsContext() );
}

char const *Panel::GetKeyBindingsFilePathID() const
{
	Assert( IsValidKeyBindingsContext() );
	return g_KBMgr.GetKeyBindingsFilePathID( GetKeyBindingsContext() );
}

void Panel::EditKeyBindings()
{
	Assert( 0 );
}


//-----------------------------------------------------------------------------
// Purpose: Set this to false to disallow IsKeyRebound chaining to GetParent() Panels...
// Input  : state - 
//-----------------------------------------------------------------------------
void Panel::SetAllowKeyBindingChainToParent( bool state )
{
	_flags.SetFlag( ALLOW_CHAIN_KEYBINDING_TO_PARENT, state );
}


//-----------------------------------------------------------------------------
// Purpose: 
// Input  :  - 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool Panel::IsKeyBindingChainToParentAllowed() const
{
	return _flags.IsFlagSet( ALLOW_CHAIN_KEYBINDING_TO_PARENT );
}

bool Panel::IsKeyOverridden( KeyCode code, int modifiers )
{
	// By default assume all keys should pass through binding system
	return false;
}

bool Panel::IsKeyRebound( KeyCode code, int modifiers )
{
	if ( IsKeyBoardInputEnabled() )
	{ 
		KeyBindingMap_t* binding = LookupBindingByKeyCode( code, modifiers );
		// Only dispatch if we're part of the current modal subtree
		if ( binding && IsChildOfSurfaceModalPanel() )
		{
			// Found match, post message to panel
			if ( binding->func )
			{
				// dispatch the func
				(this->*binding->func)();
			}
			else
			{
				Assert( 0 );
			}

			if ( !binding->passive )
			{
				// Exit this function...
				return true;
			}
		}
	}

	// Chain to parent
	Panel* pParent = GetParent();
	if ( IsKeyBindingChainToParentAllowed() && pParent && !IsKeyOverridden( code, modifiers ) )
		return pParent->IsKeyRebound( code, modifiers );

	// No suitable binding found
	return false;
}

static bool s_bSuppressRebindChecks = false;
#endif // VGUI_USEKEYBINDINGMAPS

void Panel::InternalKeyCodeTyped( int code )
{
	if ( !ShouldHandleInputMessage() )
	{
		input()->OnKeyCodeUnhandled( code );
		return;
	}

	if (IsKeyBoardInputEnabled()) 
	{
		bool shift = (input()->IsKeyDown(KEY_LSHIFT) || input()->IsKeyDown(KEY_RSHIFT));
		bool ctrl = (input()->IsKeyDown(KEY_LCONTROL) || input()->IsKeyDown(KEY_RCONTROL));
		bool alt = (input()->IsKeyDown(KEY_LALT) || input()->IsKeyDown(KEY_RALT));

		int modifiers = 0;
		if ( shift )
		{
			modifiers |= MODIFIER_SHIFT;
		}
		if ( ctrl )
		{
			modifiers |= MODIFIER_CONTROL;
		}
		if ( alt )
		{
			modifiers |= MODIFIER_ALT;
		}

		// Things in build mode don't have accelerators
		if (IsBuildGroupEnabled())
		{
			 _buildGroup->KeyCodeTyped((KeyCode)code, this);
			return;
		}

		if ( !s_bSuppressRebindChecks && IsKeyRebound( (KeyCode)code, modifiers ) )
		{
			return;
		}

		bool oldVal = s_bSuppressRebindChecks;
		s_bSuppressRebindChecks = true;
		OnKeyCodeTyped((KeyCode)code);
		s_bSuppressRebindChecks = oldVal;
	}
	else
	{
		if ( GetVPanel() == surface()->GetEmbeddedPanel() )
		{
			input()->OnKeyCodeUnhandled( code );
		}
		CallParentFunction(new KeyValues("KeyCodeTyped", "code", code));
	}
}

void Panel::InternalKeyTyped(int unichar)
{
	if ( !ShouldHandleInputMessage() )
		return;

	if (IsKeyBoardInputEnabled())
	{
		if ( IsBuildGroupEnabled() )
		{
			if ( _buildGroup->KeyTyped( (wchar_t)unichar, this ) )
			{
				return;
			}
		}

		OnKeyTyped((wchar_t)unichar);
	}
	else
	{
		CallParentFunction(new KeyValues("KeyTyped", "unichar", unichar));
	}
}

void Panel::InternalKeyCodeReleased(int code)
{
	if ( !ShouldHandleInputMessage() )
		return;

	if (IsKeyBoardInputEnabled()) 
	{
		if (IsBuildGroupEnabled())
		{
			if ( _buildGroup->KeyCodeReleased((KeyCode)code, this) )
			{
				return;
			}
		}

		OnKeyCodeReleased((KeyCode)code);
	}
	else
	{
		CallParentFunction(new KeyValues("KeyCodeReleased", "code", code));
	}
}

void Panel::InternalKeyFocusTicked()
{
	if (IsBuildGroupEnabled())
		return;
	
	OnKeyFocusTicked();
}

void Panel::InternalMouseFocusTicked()
{
	if (IsBuildGroupEnabled())
	{
		// must repaint so the numbers will be accurate
		if (_buildGroup->HasRulersOn())
		{
			PaintTraverse(true);
		}
		return;
	}

	// update cursor
	InternalSetCursor();
	OnMouseFocusTicked();
}


void Panel::InternalSetCursor()
{
	bool visible = IsVisible();

	if (visible)
	{
#if defined( VGUI_USEDRAGDROP )
		// Drag drop is overriding cursor?
		if ( m_pDragDrop->m_bDragging ||
			g_DragDropCapture.Get() != NULL )
			return;
#endif
		// chain up and make sure all our parents are also visible
		VPANEL p = GetVParent();
		while (p)
		{
			visible &= ipanel()->IsVisible(p);
			p = ipanel()->GetParent(p);
		}
	
		// only change the cursor if this panel is visible, and if its part of the main VGUI tree
		if (visible && HasParent(surface()->GetEmbeddedPanel())) 
		{	
			HCursor cursor = GetCursor();
			
			if (IsBuildGroupEnabled())
			{
				cursor = _buildGroup->GetCursor(this);
			}
			
			if (input()->GetCursorOveride())
			{
				cursor = input()->GetCursorOveride();
			}

			surface()->SetCursor(cursor);
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: Called every frame the panel is visible, designed to be overridden
//-----------------------------------------------------------------------------
void Panel::OnThink()
{
#if defined( VGUI_USEDRAGDROP )
	if ( IsPC() && 
		m_pDragDrop->m_bDragEnabled &&
		m_pDragDrop->m_bDragging &&
		m_pDragDrop->m_bDragStarted )
	{
		bool isEscapeKeyDown = input()->IsKeyDown( KEY_ESCAPE );
		if ( isEscapeKeyDown )
		{
			OnContinueDragging();
			OnFinishDragging( true, (MouseCode)-1, true );
			return;
		}

		if ( m_pDragDrop->m_hCurrentDrop != NULL )
		{
			if ( !input()->IsMouseDown( MOUSE_LEFT ) )
			{
				OnContinueDragging();
				OnFinishDragging( true, (MouseCode)-1 );
				return;
			}

			// allow the cursor to change based upon things like changing keystate, etc.
			surface()->SetCursor( m_pDragDrop->m_hCurrentDrop->GetDropCursor( m_pDragDrop->m_DragData ) );

			if ( !m_pDragDrop->m_bDropMenuShown )
			{
				// See if the hover time has gotten larger
				float hoverSeconds = ( system()->GetTimeMillis() - m_pDragDrop->m_lDropHoverTime ) * 0.001f;
				DragDrop_t *dropInfo = m_pDragDrop->m_hCurrentDrop->GetDragDropInfo();

				if ( dropInfo->m_flHoverContextTime != 0.0f )
				{
					if ( hoverSeconds >= dropInfo->m_flHoverContextTime )
					{
						m_pDragDrop->m_bDropMenuShown = true;

						CUtlVector< KeyValues * > data;
						
						GetDragData( data );

						int x, y;
						input()->GetCursorPos( x, y );

						if ( m_pDragDrop->m_hDropContextMenu.Get() )
						{
							delete m_pDragDrop->m_hDropContextMenu.Get();
						}

						Menu *menu = new Menu( m_pDragDrop->m_hCurrentDrop.Get(), "DropContext" );
							
						bool useMenu = m_pDragDrop->m_hCurrentDrop->GetDropContextMenu( menu, data );
						if ( useMenu )
						{
							m_pDragDrop->m_hDropContextMenu = menu;

							menu->SetPos( x, y );
							menu->SetVisible( true );
							menu->MakePopup();
							surface()->MovePopupToFront( menu->GetVPanel() );
							if ( menu->GetItemCount() > 0 )
							{
								int id = menu->GetMenuID( 0 );
								menu->SetCurrentlyHighlightedItem( id );
								MenuItem *item = menu->GetMenuItem( id );
								item->SetArmed( true );
							}
						}
						else
						{
							delete menu;
						}

						m_pDragDrop->m_hCurrentDrop->OnDropContextHoverShow( data );
					}
				}
			}
		}
	}
#endif
}

// input messages handlers (designed for override)
void Panel::OnCursorMoved(int x, int y)
{
}

void Panel::OnCursorEntered()
{
}

void Panel::OnCursorExited()
{
}

void Panel::OnMousePressed(MouseCode code)
{
}

void Panel::OnMouseDoublePressed(MouseCode code)
{
}

void Panel::OnMouseTriplePressed(MouseCode code)
{
}

void Panel::OnMouseReleased(MouseCode code)
{
}

void Panel::OnMouseWheeled(int delta)
{
	CallParentFunction(new KeyValues("MouseWheeled", "delta", delta));
}

// base implementation forwards Key messages to the Panel's parent - override to 'swallow' the input
void Panel::OnKeyCodePressed(KeyCode code)
{
	CallParentFunction(new KeyValues("KeyCodePressed", "code", code));
}

void Panel::OnKeyCodeTyped(KeyCode code)
{
	// handle focus change
	if ( IsX360() )
	{
		if ( code == KEY_XSTICK1_RIGHT || code == KEY_XBUTTON_RIGHT )
		{
			RequestFocusNext();
			return;
		}
		else if ( code == KEY_XSTICK1_LEFT || code == KEY_XBUTTON_LEFT )
		{
			RequestFocusPrev();
			return;
		}
	}
	
	if (code == KEY_TAB)
	{
		// if shift is down goto previous tab position, otherwise goto next
		if (input()->IsKeyDown(KEY_LSHIFT) || input()->IsKeyDown(KEY_RSHIFT))
		{
			RequestFocusPrev();
		}
		else
		{
			RequestFocusNext();
		}
	}
	else
	{
		// forward up
		if ( GetVPanel() == surface()->GetEmbeddedPanel() )
		{
			input()->OnKeyCodeUnhandled( code );
		}
		CallParentFunction(new KeyValues("KeyCodeTyped", "code", code));
	}
}

void Panel::OnKeyTyped(wchar_t unichar)
{
	CallParentFunction(new KeyValues("KeyTyped", "unichar", unichar));
}

void Panel::OnKeyCodeReleased(KeyCode code)
{
	CallParentFunction(new KeyValues("KeyCodeReleased", "code", code));
}

void Panel::OnKeyFocusTicked()
{
	CallParentFunction(new KeyValues("KeyFocusTicked"));
}

void Panel::OnMouseFocusTicked()
{
	CallParentFunction(new KeyValues("OnMouseFocusTicked"));
}

bool Panel::IsWithin(int x,int y)
{
	// check against our clip rect
	int clipRect[4];
	ipanel()->GetClipRect(GetVPanel(), clipRect[0], clipRect[1], clipRect[2], clipRect[3]);

	if (x < clipRect[0])
	{
		return false;
	}
	
	if (y < clipRect[1])
	{
		return false;
	}

	if (x >= clipRect[2])
	{
		return false;
	}
	
	if (y >= clipRect[3])
	{
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: determines which is the topmost panel under the coordinates (x, y)
//-----------------------------------------------------------------------------
VPANEL Panel::IsWithinTraverse(int x, int y, bool traversePopups)
{
	// if this one is not visible, its children won't be either
	// also if it doesn't want mouse input its children can't get it either
	if (!IsVisible() || !IsMouseInputEnabled())
		return NULL;

	if (traversePopups)
	{
		// check popups first
		int i;
		for (i = GetChildCount() - 1; i >= 0; i--)
		{
			VPANEL panel = ipanel()->GetChild(GetVPanel(), i);
			if (ipanel()->IsPopup(panel))
			{
				panel = ipanel()->IsWithinTraverse(panel, x, y, true);
				if (panel != null)
				{
					return panel;
				}
			}
		}

		// check children recursive, if you find one, just return first one
		// this checks in backwards order so the last child drawn for this panel is chosen which
		// coincides to how it would be visibly displayed
		for (i = GetChildCount() - 1; i >= 0; i--)
		{
			VPANEL panel = ipanel()->GetChild(GetVPanel(), i);
			// we've already checked popups so ignore
			if (!ipanel()->IsPopup(panel))
			{
				panel = ipanel()->IsWithinTraverse(panel, x, y, true);
				if (panel != NULL)
				{
					return panel;
				}
			}
		}
		
		// check ourself
		if ( !IsMouseInputDisabledForThisPanel() && IsWithin(x, y) )
		{
			return GetVPanel();
		}
	}
	else
	{
		// since we're not checking popups, it must be within us, so we can check ourself first
		if (IsWithin(x, y))
		{
			// check children recursive, if you find one, just return first one
			// this checks in backwards order so the last child drawn for this panel is chosen which
			// coincides to how it would be visibly displayed
			for (int i = GetChildCount() - 1; i >= 0; i--)
			{
				VPANEL panel = ipanel()->GetChild(GetVPanel(), i);
				// ignore popups
				if (!ipanel()->IsPopup(panel))
				{
					panel = ipanel()->IsWithinTraverse(panel, x, y, false);
					if (panel != NULL)
					{
						return panel;
					}
				}
			}
	
			// not a child, must be us
			if ( !IsMouseInputDisabledForThisPanel() )
				return GetVPanel();
		}
	}

	return NULL;
}

void Panel::LocalToScreen(int& x,int& y)
{
	int px, py;
	ipanel()->GetAbsPos(GetVPanel(), px, py);

	x = x + px;
	y = y + py;
}

void Panel::ScreenToLocal(int& x,int& y)
{
	int px, py;
	ipanel()->GetAbsPos(GetVPanel(), px, py);

	x = x - px;
	y = y - py;
}

void Panel::ParentLocalToScreen(int &x, int &y)
{
	int px, py;
	ipanel()->GetAbsPos(GetVParent(), px, py);

	x = x + px;
	y = y + py;
}

void Panel::MakePopup(bool showTaskbarIcon,bool disabled)
{
	surface()->CreatePopup(GetVPanel(), false, showTaskbarIcon,disabled);
}

void Panel::SetCursor(HCursor cursor)
{
	_cursor = cursor;
}

HCursor Panel::GetCursor()
{
	return _cursor;
}

void Panel::SetMinimumSize(int wide,int tall)
{
	ipanel()->SetMinimumSize(GetVPanel(), wide, tall);
}

void Panel::GetMinimumSize(int& wide,int &tall)
{
	ipanel()->GetMinimumSize(GetVPanel(), wide, tall);
}

bool Panel::IsBuildModeEditable()
{
   return true;
}

void Panel::SetBuildModeEditable(bool state)
{
	if (state)
	{
		_buildModeFlags |= BUILDMODE_EDITABLE;
	}
	else
	{
		_buildModeFlags &= ~BUILDMODE_EDITABLE;
	}
}

//-----------------------------------------------------------------------------
// Purpose: data accessor
//-----------------------------------------------------------------------------
bool Panel::IsBuildModeDeletable()
{
	return (_buildModeFlags & BUILDMODE_DELETABLE);
}

//-----------------------------------------------------------------------------
// Purpose: data accessor
//-----------------------------------------------------------------------------
void Panel::SetBuildModeDeletable(bool state)
{
	if (state)
	{
		_buildModeFlags |= BUILDMODE_DELETABLE;
	}
	else
	{
		_buildModeFlags &= ~BUILDMODE_DELETABLE;
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool Panel::IsBuildModeActive()
{
	return _buildGroup ? _buildGroup->IsEnabled() : false;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void Panel::GetClipRect(int& x0,int& y0,int& x1,int& y1)
{
	ipanel()->GetClipRect(GetVPanel(), x0, y0, x1, y1);
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
int Panel::GetChildCount()
{
	return ipanel()->GetChildCount(GetVPanel());
}

//-----------------------------------------------------------------------------
// Purpose: returns a child by the specified index
//-----------------------------------------------------------------------------
Panel *Panel::GetChild(int index)
{
	// get the child and cast it to a panel
	// this assumes that the child is from the same module as the this (precondition)
	return ipanel()->GetPanel(ipanel()->GetChild(GetVPanel(), index), GetControlsModuleName());
}

//-----------------------------------------------------------------------------
// Purpose: moves the key focus back
//-----------------------------------------------------------------------------
bool Panel::RequestFocusPrev(VPANEL panel)
{
	// chain to parent
	if (GetVParent())
	{
		return ipanel()->RequestFocusPrev(GetVParent(), GetVPanel());
	}
	return false;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool Panel::RequestFocusNext(VPANEL panel)
{
	// chain to parent
	if (GetVParent())
	{
		return ipanel()->RequestFocusNext(GetVParent(), GetVPanel());
	}
	return false;
}

//-----------------------------------------------------------------------------
// Purpose: Sets the panel to have the current sub focus
// Input  : direction - the direction in which focus travelled to arrive at this panel; forward = 1, back = -1
//-----------------------------------------------------------------------------
void Panel::RequestFocus(int direction)
{
	// NOTE: This doesn't make any sense if we don't have keyboard input enabled
	Assert( IsX360() || IsKeyBoardInputEnabled() );
//	ivgui()->DPrintf2("RequestFocus(%s, %s)\n", GetName(), GetClassName());
	OnRequestFocus(GetVPanel(), NULL);
}

//-----------------------------------------------------------------------------
// Purpose: Called after a panel requests focus to fix up the whole chain
//-----------------------------------------------------------------------------
void Panel::OnRequestFocus(VPANEL subFocus, VPANEL defaultPanel)
{
	CallParentFunction(new KeyValues("OnRequestFocus", "subFocus", subFocus, "defaultPanel", defaultPanel));
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
VPANEL Panel::GetCurrentKeyFocus()
{
	return NULL;
}

//-----------------------------------------------------------------------------
// Purpose: returns true if the panel has focus
//-----------------------------------------------------------------------------
bool Panel::HasFocus()
{
	if (input()->GetFocus() == GetVPanel())
	{
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void Panel::SetTabPosition(int position)
{
	_tabPosition = position;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
int Panel::GetTabPosition()
{
	return _tabPosition;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void Panel::InternalFocusChanged(bool lost)
{
/*
	//if focus is gained tell the focusNavGroup about it so its current can be correct
	if( (!lost) && (_focusNavGroup!=null) )
	{
		_focusNavGroup->setCurrentPanel(this);
	}
*/
}

//-----------------------------------------------------------------------------
// Purpose: Called when a panel loses it's mouse capture
//-----------------------------------------------------------------------------
void Panel::OnMouseCaptureLost()
{
	if (m_pTooltips)
	{
		m_pTooltips->ResetDelay();
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void Panel::AddActionSignalTarget(Panel *messageTarget)
{
	HPanel target = ivgui()->PanelToHandle(messageTarget->GetVPanel());
	if (!_actionSignalTargetDar.HasElement(target))
	{
		_actionSignalTargetDar.AddElement(target);
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void Panel::AddActionSignalTarget(VPANEL messageTarget)
{
	HPanel target = ivgui()->PanelToHandle(messageTarget);
	if (!_actionSignalTargetDar.HasElement(target))
	{
		_actionSignalTargetDar.AddElement(target);
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void Panel::RemoveActionSignalTarget(Panel *oldTarget)
{
	_actionSignalTargetDar.RemoveElement(ivgui()->PanelToHandle(oldTarget->GetVPanel()));
}

//-----------------------------------------------------------------------------
// Purpose: Sends a message to all the panels that have requested action signals
//-----------------------------------------------------------------------------
void Panel::PostActionSignal( KeyValues *message )
{
	if ( m_bIsSilent != true )
	{	
		// add who it was from the message
		message->SetPtr("panel", this);
		int i;
		for (i = _actionSignalTargetDar.GetCount() - 1; i > 0; i--)
		{
			VPANEL panel = ivgui()->HandleToPanel(_actionSignalTargetDar[i]);
			if (panel)
			{
				ivgui()->PostMessage(panel, message->MakeCopy(), GetVPanel());
			}
		}

		// do this so we can save on one MakeCopy() call
		if (i == 0)
		{
			VPANEL panel = ivgui()->HandleToPanel(_actionSignalTargetDar[i]);
			if (panel)
			{
				ivgui()->PostMessage(panel, message, GetVPanel());
				return;
			}
		}
	}
	message->deleteThis();
}

void Panel::SetBorder(IBorder *border)
{
	_border = border;

	if (border)
	{
		int x, y, x2, y2;
		border->GetInset(x, y, x2, y2);
		ipanel()->SetInset(GetVPanel(), x, y, x2, y2);

		// update our background type based on the bord
		SetPaintBackgroundType(border->GetBackgroundType());
	}
	else
	{
		ipanel()->SetInset(GetVPanel(), 0, 0, 0, 0);
	}
}

IBorder *Panel::GetBorder()
{
	return _border;
}


void Panel::SetPaintBorderEnabled(bool state)
{
	_flags.SetFlag( PAINT_BORDER_ENABLED, state );
}

void Panel::SetPaintBackgroundEnabled(bool state)
{
	_flags.SetFlag( PAINT_BACKGROUND_ENABLED, state );
}

void Panel::SetPaintBackgroundType( int type )
{
	// HACK only 0 through 2 supported for now
	m_nPaintBackgroundType = clamp( type, 0, 2 );
}

void Panel::SetPaintEnabled(bool state)
{
	_flags.SetFlag( PAINT_ENABLED, state );
}

void Panel::SetPostChildPaintEnabled(bool state)
{
	_flags.SetFlag( POST_CHILD_PAINT_ENABLED, state );
}

void Panel::GetInset(int& left,int& top,int& right,int& bottom)
{
	ipanel()->GetInset(GetVPanel(), left, top, right, bottom);
}

void Panel::GetPaintSize(int& wide,int& tall)
{
	GetSize(wide, tall);
	if (_border != null)
	{
		int left,top,right,bottom;
		_border->GetInset(left,top,right,bottom);

		wide -= (left+right);
		tall -= (top+bottom);
	}
}

int Panel::GetWide()
{
	int wide, tall;
	ipanel()->GetSize(GetVPanel(), wide, tall);
	return wide;
}

void Panel::SetWide(int wide)
{
	ipanel()->SetSize(GetVPanel(), wide, GetTall());
}

int Panel::GetTall()
{
	int wide, tall;
	ipanel()->GetSize(GetVPanel(), wide, tall);
	return tall;
}

void Panel::SetTall(int tall)
{
	ipanel()->SetSize(GetVPanel(), GetWide(), tall);
}

void Panel::SetBuildGroup(BuildGroup* buildGroup)
{
	//TODO: remove from old group

	Assert(buildGroup != NULL);
	
	_buildGroup = buildGroup;

	_buildGroup->PanelAdded(this);
}

bool Panel::IsBuildGroupEnabled()
{
	if ( !_buildGroup.IsValid() )
		return false;

	bool enabled = _buildGroup->IsEnabled();
	if ( enabled )
		return enabled;

	if ( GetParent() && GetParent()->IsBuildGroupEnabled() )
		return true;

	return false;
}

void Panel::SetBgColor(Color color)
{
	_bgColor = color;
}

void Panel::SetFgColor(Color color)
{
	_fgColor = color;
}

Color Panel::GetBgColor()
{
	return _bgColor;
}

Color Panel::GetFgColor()
{
	return _fgColor;
}

void Panel::InternalPerformLayout()
{
	_flags.SetFlag( IN_PERFORM_LAYOUT );
	// make sure the scheme has been applied
	_flags.ClearFlag( NEEDS_LAYOUT );
	PerformLayout();
	_flags.ClearFlag( IN_PERFORM_LAYOUT );
}

void Panel::PerformLayout()
{
	// this should be overridden to relayout controls
}

void Panel::InvalidateLayout( bool layoutNow, bool reloadScheme )
{
	_flags.SetFlag( NEEDS_LAYOUT );
	
	if (reloadScheme)
	{
		// make all our children reload the scheme
		_flags.SetFlag( NEEDS_SCHEME_UPDATE );
	
		for (int i = 0; i < GetChildCount(); i++)
		{
			GetChild(i)->InvalidateLayout(layoutNow, true);
		}
		
		PerformApplySchemeSettings();
	}
	
	if (layoutNow)
	{
		InternalPerformLayout();
		Repaint();
	}
}

bool Panel::IsCursorNone()
{
	HCursor cursor = GetCursor();

	if (!cursor)
	{
		return true;
	}
	
	return false;
}

//-----------------------------------------------------------------------------
// Purpose: returns true if the cursor is currently over the panel
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool Panel::IsCursorOver(void)
{
	int x, y;
	input()->GetCursorPos(x, y);
	return IsWithin(x, y);
}

//-----------------------------------------------------------------------------
// Purpose: Called when a panel receives a command message from another panel
//-----------------------------------------------------------------------------
void Panel::OnCommand(const char *command)
{
	// if noone else caught this, pass along to the listeners
	// (this is useful for generic dialogs - otherwise, commands just get ignored)
	KeyValues *msg = new KeyValues( command );
	PostActionSignal( msg );
}

//-----------------------------------------------------------------------------
// Purpose: panel gained focus message
//-----------------------------------------------------------------------------
void Panel::OnSetFocus()
{
	Repaint();
}

//-----------------------------------------------------------------------------
// Purpose: panel lost focus message
//-----------------------------------------------------------------------------
void Panel::OnKillFocus()
{
	Repaint();
}

//-----------------------------------------------------------------------------
// Purpose: Sets the object up to be deleted next frame
//-----------------------------------------------------------------------------
void Panel::MarkForDeletion()
{
	if ( _flags.IsFlagSet( MARKED_FOR_DELETION ) )
		return;

	_flags.SetFlag( MARKED_FOR_DELETION );
	_flags.ClearFlag( AUTODELETE_ENABLED );

	if (ivgui()->IsRunning())
	{
		ivgui()->MarkPanelForDeletion(GetVPanel());
	}
	else
	{
		delete this;
	}
}

//-----------------------------------------------------------------------------
// Purpose: return true if this object require a perform layout
//-----------------------------------------------------------------------------
bool Panel::IsLayoutInvalid()
{
	return _flags.IsFlagSet( NEEDS_LAYOUT );
}


//-----------------------------------------------------------------------------
// Sets the pin corner + resize mode for resizing panels
//-----------------------------------------------------------------------------
void Panel::SetAutoResize( PinCorner_e pinCorner, AutoResize_e resizeDir, 
	int nPinOffsetX, int nPinOffsetY, int nUnpinnedCornerOffsetX, int nUnpinnedCornerOffsetY )
{
	_pinCorner = pinCorner;
	_autoResizeDirection = resizeDir;
	m_nPinDeltaX = nPinOffsetX;
	m_nPinDeltaY = nPinOffsetY;
	m_nResizeDeltaX = nUnpinnedCornerOffsetX;
	m_nResizeDeltaY = nUnpinnedCornerOffsetY;
}


//-----------------------------------------------------------------------------
// Sets the pin corner for non-resizing panels
//-----------------------------------------------------------------------------
void Panel::SetPinCorner( PinCorner_e pinCorner, int nOffsetX, int nOffsetY )
{
	_pinCorner = pinCorner;
	_autoResizeDirection = AUTORESIZE_NO;
	m_nPinDeltaX = nOffsetX;
	m_nPinDeltaY = nOffsetY;
	m_nResizeDeltaX = 0;
	m_nResizeDeltaY = 0;
}

	
//-----------------------------------------------------------------------------
// Purpose: data accessor
//-----------------------------------------------------------------------------
Panel::PinCorner_e Panel::GetPinCorner()
{
	return (PinCorner_e)_pinCorner;
}


//-----------------------------------------------------------------------------
// Gets the relative offset of the control from the pin corner
//-----------------------------------------------------------------------------
void Panel::GetPinOffset( int &dx, int &dy )
{
	dx = m_nPinDeltaX;
	dy = m_nPinDeltaY;
}


//-----------------------------------------------------------------------------
// Purpose: data accessor
//-----------------------------------------------------------------------------
Panel::AutoResize_e Panel::GetAutoResize()
{
	return (AutoResize_e)_autoResizeDirection;
}


//-----------------------------------------------------------------------------
// Gets the relative offset of the control from the pin corner
//-----------------------------------------------------------------------------
void Panel::GetResizeOffset( int &dx, int &dy )
{
	dx = m_nResizeDeltaX;
	dy = m_nResizeDeltaY;
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void Panel::ApplySchemeSettings(IScheme *pScheme)
{
	// get colors
	SetFgColor(GetSchemeColor("Panel.FgColor", pScheme));
	SetBgColor(GetSchemeColor("Panel.BgColor", pScheme));

#if defined( VGUI_USEDRAGDROP )
    m_clrDragFrame = pScheme->GetColor("DragDrop.DragFrame", Color(255, 255, 255, 192));
	m_clrDropFrame = pScheme->GetColor("DragDrop.DropFrame", Color(150, 255, 150, 255));

	m_infoFont = pScheme->GetFont( "DefaultVerySmall" );
#endif
	// mark us as no longer needing scheme settings applied
	_flags.ClearFlag( NEEDS_SCHEME_UPDATE );

	if ( IsBuildGroupEnabled() )
	{
		_buildGroup->ApplySchemeSettings(pScheme);
		return;
	}
}

//-----------------------------------------------------------------------------
// Purpose: Checks to see if the panel needs it's scheme info setup
//-----------------------------------------------------------------------------
void Panel::PerformApplySchemeSettings()
{
	if ( _flags.IsFlagSet( NEEDS_DEFAULT_SETTINGS_APPLIED ) )
	{
		InternalInitDefaultValues( GetAnimMap() );
	}

	if ( _flags.IsFlagSet( NEEDS_SCHEME_UPDATE ) )
	{
		VPROF( "ApplySchemeSettings" );
		IScheme *pScheme = scheme()->GetIScheme( GetScheme() );
		AssertOnce( pScheme );
		if ( pScheme ) // this should NEVER be null, but if it is bad things would happen in ApplySchemeSettings...
		{
			ApplySchemeSettings( pScheme );
			//_needsSchemeUpdate = false;	
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: Loads panel details related to autoresize from the resource info
//-----------------------------------------------------------------------------
#if defined( _DEBUG )
static Panel *lastWarningParent = 0;
#endif

void Panel::ApplyAutoResizeSettings(KeyValues *inResourceData)
{
	int x, y;
	GetPos(x, y);

	int wide, tall;
	GetSize( wide, tall );

	AutoResize_e autoResize = (AutoResize_e)inResourceData->GetInt( "AutoResize", AUTORESIZE_NO );
	PinCorner_e pinCorner = (PinCorner_e)inResourceData->GetInt( "PinCorner", PIN_TOPLEFT );

	// By default, measure unpinned corner for the offset
	int pw = wide, pt = tall;
	if ( GetParent() )
	{
		GetParent()->GetSize( pw, pt );
#if defined( _DEBUG )
		if ( pw == 64 && pt == 24 )
		{
			if ( GetParent() != lastWarningParent )
			{
				lastWarningParent = GetParent();
				Warning( "Resize parent (panel(%s) -> parent(%s)) not sized yet!!!\n", GetName(), GetParent()->GetName() );
			}
		}
#endif
	}
				    
	int nPinnedCornerOffsetX = 0, nPinnedCornerOffsetY = 0;
	int nUnpinnedCornerOffsetX = 0, nUnpinnedCornerOffsetY = 0;
	switch( pinCorner )
	{
	case PIN_TOPLEFT:
		nPinnedCornerOffsetX = x;
		nPinnedCornerOffsetY = y;
		nUnpinnedCornerOffsetX = (x + wide) - pw;
		nUnpinnedCornerOffsetY = (y + tall) - pt;
		break;

	case PIN_TOPRIGHT:
		nPinnedCornerOffsetX = (x + wide) - pw;
		nPinnedCornerOffsetY = y;
		nUnpinnedCornerOffsetX = x;
		nUnpinnedCornerOffsetY = (y + tall) - pt;
		break;

	case PIN_BOTTOMLEFT:
		nPinnedCornerOffsetX = x;
		nPinnedCornerOffsetY = (y + tall) - pt;
		nUnpinnedCornerOffsetX = (x + wide) - pw;
		nUnpinnedCornerOffsetY = y;
		break;

	case PIN_BOTTOMRIGHT:
		nPinnedCornerOffsetX = (x + wide) - pw;
		nPinnedCornerOffsetY = (y + tall) - pt;
		nUnpinnedCornerOffsetX = x;
		nUnpinnedCornerOffsetY = y;
		break;
	}

	// Allow specific overrides in the resource file
	if ( IsProportional() )
	{
		if ( inResourceData->FindKey( "PinnedCornerOffsetX" ) )
		{
            nPinnedCornerOffsetX = scheme()->GetProportionalScaledValueEx( GetScheme(), inResourceData->GetInt( "PinnedCornerOffsetX" ) );
		}
		if ( inResourceData->FindKey( "PinnedCornerOffsetY" ) )
		{
            nPinnedCornerOffsetY =	scheme()->GetProportionalScaledValueEx( GetScheme(), inResourceData->GetInt( "PinnedCornerOffsetY" ) );
		}
		if ( inResourceData->FindKey( "UnpinnedCornerOffsetX" ) )
		{
            nUnpinnedCornerOffsetX = scheme()->GetProportionalScaledValueEx( GetScheme(), inResourceData->GetInt( "UnpinnedCornerOffsetX" ) );
		}
		if ( inResourceData->FindKey( "UnpinnedCornerOffsetY" ) )
		{
            nUnpinnedCornerOffsetY = scheme()->GetProportionalScaledValueEx( GetScheme(), inResourceData->GetInt( "UnpinnedCornerOffsetY" ) );
		}
	}
	else
	{
		nPinnedCornerOffsetX = inResourceData->GetInt( "PinnedCornerOffsetX", nPinnedCornerOffsetX );
		nPinnedCornerOffsetY = inResourceData->GetInt( "PinnedCornerOffsetY", nPinnedCornerOffsetY );
		nUnpinnedCornerOffsetX = inResourceData->GetInt( "UnpinnedCornerOffsetX", nUnpinnedCornerOffsetX );
		nUnpinnedCornerOffsetY = inResourceData->GetInt( "UnpinnedCornerOffsetY", nUnpinnedCornerOffsetY );
	}

	if ( autoResize == AUTORESIZE_NO )
	{
		nUnpinnedCornerOffsetX = nUnpinnedCornerOffsetY = 0;
	}

	SetAutoResize( pinCorner, autoResize, nPinnedCornerOffsetX, nPinnedCornerOffsetY, nUnpinnedCornerOffsetX, nUnpinnedCornerOffsetY );
}


//-----------------------------------------------------------------------------
// Purpose: Loads panel details from the resource info
//-----------------------------------------------------------------------------
void Panel::ApplySettings(KeyValues *inResourceData)
{
	// First restore to default values
	if ( _flags.IsFlagSet( NEEDS_DEFAULT_SETTINGS_APPLIED ) )
	{
		InternalInitDefaultValues( GetAnimMap() );
	}

	// Let PanelAnimationVars auto-retrieve settings (we restore defaults above
	//  since a script might be missing certain values)
	InternalApplySettings( GetAnimMap(), inResourceData );

	// clear any alignment flags
	_buildModeFlags &= ~(BUILDMODE_SAVE_XPOS_RIGHTALIGNED | BUILDMODE_SAVE_XPOS_CENTERALIGNED | BUILDMODE_SAVE_YPOS_BOTTOMALIGNED | BUILDMODE_SAVE_YPOS_CENTERALIGNED | BUILDMODE_SAVE_WIDE_FULL);

	// get the position
	int screenWide, screenTall;
	surface()->GetScreenSize(screenWide, screenTall);
	int x, y;
	GetPos(x, y);
	const char *xstr = inResourceData->GetString( "xpos", NULL );
	const char *ystr = inResourceData->GetString( "ypos", NULL );
	if (xstr)
	{
		// look for alignment flags
		if (xstr[0] == 'r' || xstr[0] == 'R')
		{
			_buildModeFlags |= BUILDMODE_SAVE_XPOS_RIGHTALIGNED;
			xstr++;
		}
		else if (xstr[0] == 'c' || xstr[0] == 'C')
		{
			_buildModeFlags |= BUILDMODE_SAVE_XPOS_CENTERALIGNED;
			xstr++;
		}

		// get the value
		x = atoi(xstr);

		// scale the x up to our screen co-ords
		if ( IsProportional() )
		{
			x = scheme()->GetProportionalScaledValueEx(GetScheme(), x);
		}

		// now correct the alignment
		if (_buildModeFlags & BUILDMODE_SAVE_XPOS_RIGHTALIGNED)
		{
			x = screenWide - x; 
		}
		else if (_buildModeFlags & BUILDMODE_SAVE_XPOS_CENTERALIGNED)
		{
			x = (screenWide / 2) + x;
		}
	}

	if (ystr)
	{
		// look for alignment flags
		if (ystr[0] == 'r' || ystr[0] == 'R')
		{
			_buildModeFlags |= BUILDMODE_SAVE_YPOS_BOTTOMALIGNED;
			ystr++;
		}
		else if (ystr[0] == 'c' || ystr[0] == 'C')
		{
			_buildModeFlags |= BUILDMODE_SAVE_YPOS_CENTERALIGNED;
			ystr++;
		}
		y = atoi(ystr);
		if (IsProportional())
		{
			// scale the y up to our screen co-ords
			y = scheme()->GetProportionalScaledValueEx(GetScheme(), y);
		}
		// now correct the alignment
		if (_buildModeFlags & BUILDMODE_SAVE_YPOS_BOTTOMALIGNED)
		{
			y = screenTall - y; 
		}
		else if (_buildModeFlags & BUILDMODE_SAVE_YPOS_CENTERALIGNED)
		{
			y = (screenTall / 2) + y;
		}
	}

	SetPos(x, y);

	if (inResourceData->FindKey( "zpos" ))
	{
		SetZPos( inResourceData->GetInt( "zpos" ) );
	}

	// size
	int wide, tall;
	GetSize( wide, tall );

	const char *wstr = inResourceData->GetString( "wide", NULL );
	if ( wstr )
	{
		if (wstr[0] == 'f' || wstr[0] == 'F')
		{
			_buildModeFlags |= BUILDMODE_SAVE_WIDE_FULL;
			wstr++;
		}
		wide = atoi(wstr);
		if ( IsProportional() )
		{
			// scale the x and y up to our screen co-ords
			wide = scheme()->GetProportionalScaledValueEx(GetScheme(), wide);
		}
		// now correct the alignment
		if (_buildModeFlags & BUILDMODE_SAVE_WIDE_FULL)
		{
			wide = screenWide - wide; 
		}
	}

	tall = inResourceData->GetInt( "tall", tall );
	if ( IsProportional() )
	{
		// scale the x and y up to our screen co-ords
		tall = scheme()->GetProportionalScaledValueEx(GetScheme(), tall);
	}
	
	SetSize( wide, tall );

	// NOTE: This has to happen after pos + size is set
	ApplyAutoResizeSettings( inResourceData );

	// only get colors if we're ignoring the scheme
	if (inResourceData->GetInt("IgnoreScheme", 0))
	{
		PerformApplySchemeSettings();
	}

	// state
	int state = inResourceData->GetInt("visible", 1);
	if (state == 0)
	{
		SetVisible(false);
	}
	else if (state == 1)
	{
		SetVisible(true);
	}

	SetEnabled( inResourceData->GetInt("enabled", true) );

	// tab order
	SetTabPosition(inResourceData->GetInt("tabPosition", 0));

	const char *tooltip = inResourceData->GetString("tooltiptext", NULL);
	if (tooltip && *tooltip)
	{
		GetTooltip()->SetText(tooltip);
	}

	// paint background?
	int nPaintBackground = inResourceData->GetInt("paintbackground", -1);
	if (nPaintBackground >= 0)
	{
		SetPaintBackgroundEnabled( nPaintBackground != 0 );
	}

	// paint border?
	int nPaintBorder = inResourceData->GetInt("paintborder", -1);
	if (nPaintBorder >= 0)
	{
		SetPaintBorderEnabled( nPaintBorder != 0 );
	}

	// check to see if we have a new name assigned
	const char *newName = inResourceData->GetString("fieldName", NULL);
	if ( newName )
	{
		// Only slam the name if the new one differs...
		SetName(newName);
	}
}

//-----------------------------------------------------------------------------
// Purpose: Saves out a resource description of this panel
//-----------------------------------------------------------------------------
void Panel::GetSettings( KeyValues *outResourceData )
{
	// control class name (so it can be recreated later if needed)
	outResourceData->SetString( "ControlName", GetClassName() );

	// name
	outResourceData->SetString( "fieldName", _panelName );
	
	// positioning
	int screenWide, screenTall;
	surface()->GetScreenSize(screenWide, screenTall);
	int x, y;
	GetPos( x, y );
	if ( IsProportional() )
	{
		x = scheme()->GetProportionalNormalizedValueEx( GetScheme(), x );
		y = scheme()->GetProportionalNormalizedValueEx( GetScheme(), y );
	}
	// correct for alignment
	if (_buildModeFlags & BUILDMODE_SAVE_XPOS_RIGHTALIGNED)
	{
		x = screenWide - x;
		char xstr[32];
		Q_snprintf(xstr, sizeof( xstr ), "r%d", x);
		outResourceData->SetString( "xpos", xstr );
	}
	else if (_buildModeFlags & BUILDMODE_SAVE_XPOS_CENTERALIGNED)
	{
		x = (screenWide / 2) + x;
		char xstr[32];
		Q_snprintf(xstr, sizeof( xstr ), "c%d", x);
		outResourceData->SetString( "xpos", xstr );
	}
	else
	{
		outResourceData->SetInt( "xpos", x );
	}
	if (_buildModeFlags & BUILDMODE_SAVE_YPOS_BOTTOMALIGNED)
	{
		y = screenTall - y;
		char ystr[32];
		Q_snprintf(ystr, sizeof( ystr ), "r%d", y);
		outResourceData->SetString( "ypos", ystr );
	}
	else if (_buildModeFlags & BUILDMODE_SAVE_YPOS_CENTERALIGNED)
	{
		y = (screenTall / 2) + y;
		char ystr[32];
		Q_snprintf(ystr, sizeof( ystr ), "c%d", y);
		outResourceData->SetString( "ypos", ystr );
	}
	else
	{
		outResourceData->SetInt( "ypos", y );
	}
	if (m_pTooltips)
	{
		if (strlen(m_pTooltips->GetText()) > 0)
		{
			outResourceData->SetString("tooltiptext", m_pTooltips->GetText());
		}
	}
	int wide, tall;
	GetSize( wide, tall );
	if ( IsProportional() )
	{
		wide = scheme()->GetProportionalNormalizedValueEx( GetScheme(), wide );
		tall = scheme()->GetProportionalNormalizedValueEx( GetScheme(), tall );
	}

	int z = ipanel()->GetZPos(GetVPanel());
	if (z)
	{
		outResourceData->SetInt("zpos", z);
	}

	// Correct for alignment
	if (_buildModeFlags & BUILDMODE_SAVE_WIDE_FULL )
	{
		wide = screenWide - wide;
		char wstr[32];
		Q_snprintf(wstr, sizeof( wstr ), "f%d", wide);
		outResourceData->SetString( "wide", wstr );
	}
	else
	{
		outResourceData->SetInt( "wide", wide );
	}
	outResourceData->SetInt( "tall", tall );

	outResourceData->SetInt("AutoResize", GetAutoResize());
	outResourceData->SetInt("PinCorner", GetPinCorner());

	// state
	outResourceData->SetInt( "visible", IsVisible() );
	outResourceData->SetInt( "enabled", IsEnabled() );

	outResourceData->SetInt( "tabPosition", GetTabPosition() );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
Color Panel::GetSchemeColor(const char *keyName, IScheme *pScheme)
{
	return pScheme->GetColor(keyName, Color(255, 255, 255, 255));
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
Color Panel::GetSchemeColor(const char *keyName, Color defaultColor, IScheme *pScheme)
{
	return pScheme->GetColor(keyName, defaultColor);
}

//-----------------------------------------------------------------------------
// Purpose: Returns a string description of the panel fields for use in the UI
//-----------------------------------------------------------------------------
const char *Panel::GetDescription( void )
{
	static const char *panelDescription = "string fieldName, int xpos, int ypos, int wide, int tall, bool visible, bool enabled, int tabPosition, corner pinCorner, autoresize autoResize, string tooltiptext";
	return panelDescription;
}

//-----------------------------------------------------------------------------
// Purpose: user configuration settings
//			this is used for any control details the user wants saved between sessions
//			eg. dialog positions, last directory opened, list column width
//-----------------------------------------------------------------------------
void Panel::ApplyUserConfigSettings(KeyValues *userConfig)
{
}

//-----------------------------------------------------------------------------
// Purpose: returns user config settings for this control
//-----------------------------------------------------------------------------
void Panel::GetUserConfigSettings(KeyValues *userConfig)
{
}

//-----------------------------------------------------------------------------
// Purpose: optimization, return true if this control has any user config settings
//-----------------------------------------------------------------------------
bool Panel::HasUserConfigSettings()
{
	return false;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void Panel::InternalInvalidateLayout()
{
	InvalidateLayout(false, false);
}

//-----------------------------------------------------------------------------
// Purpose: called whenever the panel moves
//-----------------------------------------------------------------------------
void Panel::OnMove()
{
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void Panel::InternalMove()
{
	OnMove();
	for(int i=0;i<GetChildCount();i++)
	{
		// recursively apply to all children
		GetChild(i)->OnMove();
	}
}

//-----------------------------------------------------------------------------
// Purpose: empty function
//-----------------------------------------------------------------------------
void Panel::OnTick()
{
}

//-----------------------------------------------------------------------------
// Purpose: versioning
//-----------------------------------------------------------------------------
void *Panel::QueryInterface(EInterfaceID id)
{
	if (id == ICLIENTPANEL_STANDARD_INTERFACE)
	{
		return this;
	}

	return NULL;
}


//-----------------------------------------------------------------------------
// Purpose: Map all the base messages to functions
//			ordering from most -> least used improves speed
//-----------------------------------------------------------------------------
MessageMapItem_t Panel::m_MessageMap[] =
{
	MAP_MESSAGE_INT( Panel, "RequestFocus", RequestFocus, "direction" )
};

// IMPLEMENT_PANELMAP( Panel, NULL )
PanelMap_t Panel::m_PanelMap = { Panel::m_MessageMap, ARRAYSIZE(Panel::m_MessageMap), "Panel", NULL };
PanelMap_t *Panel::GetPanelMap( void ) { return &m_PanelMap; }

//-----------------------------------------------------------------------------
// Purpose: !! Soon to replace existing prepare panel map
//-----------------------------------------------------------------------------
void PreparePanelMessageMap(PanelMessageMap *panelMap)
{
	// iterate through the class hierarchy message maps
	while ( panelMap != NULL && !panelMap->processed )
	{
		// hash message map strings into symbols
		for (int i = 0; i < panelMap->entries.Count(); i++)
		{
			MessageMapItem_t *item = &panelMap->entries[i];

			if (item->name)
			{
				item->nameSymbol = KeyValuesSystem()->GetSymbolForString(item->name);
			}
			else
			{
				item->nameSymbol = INVALID_KEY_SYMBOL;
			}
			if (item->firstParamName)
			{
				item->firstParamSymbol = KeyValuesSystem()->GetSymbolForString(item->firstParamName);
			}
			else
			{
				item->firstParamSymbol = INVALID_KEY_SYMBOL;
			}
			if (item->secondParamName)
			{
				item->secondParamSymbol = KeyValuesSystem()->GetSymbolForString(item->secondParamName);
			}
			else
			{
				item->secondParamSymbol = INVALID_KEY_SYMBOL;
			}
		}
		
		panelMap->processed = true;
		panelMap = panelMap->baseMap;
	}
}



//-----------------------------------------------------------------------------
// Purpose: Handles a message
//			Dispatches the message to a set of message maps
//-----------------------------------------------------------------------------
void Panel::OnMessage(const KeyValues *params, VPANEL ifromPanel)
{
	PanelMessageMap *panelMap = GetMessageMap();
	bool bFound = false;
	int iMessageName = params->GetNameSymbol();

	if ( !panelMap->processed )
	{
		PreparePanelMessageMap( panelMap );
	}

	// iterate through the class hierarchy message maps
	for ( ; panelMap != NULL && !bFound; panelMap = panelMap->baseMap )
	{
#if defined( _DEBUG )
//		char const *className = panelMap->pfnClassName();
//		NOTE_UNUSED( className );
#endif

		// iterate all the entries in the panel map
		for ( int i = 0; i < panelMap->entries.Count(); i++ )
		{
			MessageMapItem_t *pMap = &panelMap->entries[i];

			if (iMessageName == pMap->nameSymbol)
			{
				bFound = true;

				switch (pMap->numParams)
				{
				case 0:
				{
					(this->*(pMap->func))();
					break;
				}
		
				case 1:
				{
					KeyValues *param1 = params->FindKey(pMap->firstParamSymbol);
					if (!param1)
					{
						param1 = const_cast<KeyValues *>(params);
					}

					switch ( pMap->firstParamType )
					{
						case DATATYPE_INT:
							typedef void (Panel::*MessageFunc_Int_t)(int);
							(this->*((MessageFunc_Int_t)pMap->func))( param1->GetInt() );
							break;

						case DATATYPE_UINT64:
							typedef void (Panel::*MessageFunc_Uin64_t)(uint64);
							(this->*((MessageFunc_Uin64_t)pMap->func))( param1->GetUint64() );
							break;

						case DATATYPE_PTR:
							typedef void (Panel::*MessageFunc_Ptr_t)( void * );
							(this->*((MessageFunc_Ptr_t)pMap->func))( param1->GetPtr() );
							break;

						case DATATYPE_FLOAT:
							typedef void (Panel::*MessageFunc_Float_t)( float );
							(this->*((MessageFunc_Float_t)pMap->func))( param1->GetFloat() );
							break;

						case DATATYPE_CONSTCHARPTR:
							typedef void (Panel::*MessageFunc_CharPtr_t)( const char * );
							(this->*((MessageFunc_CharPtr_t)pMap->func))( param1->GetString() );
							break;

						case DATATYPE_CONSTWCHARPTR:
							typedef void (Panel::*MessageFunc_WCharPtr_t)( const wchar_t * );
							(this->*((MessageFunc_WCharPtr_t)pMap->func))( param1->GetWString() );
							break;

						case DATATYPE_KEYVALUES:
							typedef void (Panel::*MessageFunc_KeyValues_t)(KeyValues *);
							if ( pMap->firstParamName )
							{
								(this->*((MessageFunc_KeyValues_t)pMap->func))( (KeyValues *)param1->GetPtr() );
							}
							else
							{
								// no param set, so pass in the whole thing
								(this->*((MessageFunc_KeyValues_t)pMap->func))( const_cast<KeyValues *>(params) );
							}
							break;

						default:
							Assert(!("No handler for vgui message function"));
							break;
					}
					break;
				}

				case 2:
				{
					KeyValues *param1 = params->FindKey(pMap->firstParamSymbol);
					if (!param1)
					{
						param1 = const_cast<KeyValues *>(params);
					}
					KeyValues *param2 = params->FindKey(pMap->secondParamSymbol);
					if (!param2)
					{
						param2 = const_cast<KeyValues *>(params);
					}

					if ( (DATATYPE_INT == pMap->firstParamType) && (DATATYPE_INT == pMap->secondParamType) )
					{
						typedef void (Panel::*MessageFunc_IntInt_t)(int, int);
						(this->*((MessageFunc_IntInt_t)pMap->func))( param1->GetInt(), param2->GetInt() );
					}
					else if ( (DATATYPE_PTR == pMap->firstParamType) && (DATATYPE_INT == pMap->secondParamType) )
					{
						typedef void (Panel::*MessageFunc_PtrInt_t)(void *, int);
						(this->*((MessageFunc_PtrInt_t)pMap->func))( param1->GetPtr(), param2->GetInt() );
					}
					else if ( (DATATYPE_CONSTCHARPTR == pMap->firstParamType) && (DATATYPE_INT == pMap->secondParamType) )
					{
						typedef void (Panel::*MessageFunc_ConstCharPtrInt_t)(const char *, int);
						(this->*((MessageFunc_ConstCharPtrInt_t)pMap->func))( param1->GetString(), param2->GetInt() );
					}
					else if ( (DATATYPE_CONSTCHARPTR == pMap->firstParamType) && (DATATYPE_CONSTCHARPTR == pMap->secondParamType) )
					{
						typedef void (Panel::*MessageFunc_ConstCharPtrConstCharPtr_t)(const char *, const char *);
						(this->*((MessageFunc_ConstCharPtrConstCharPtr_t)pMap->func))( param1->GetString(), param2->GetString() );
					}
					else if ( (DATATYPE_INT == pMap->firstParamType) && (DATATYPE_CONSTCHARPTR == pMap->secondParamType) )
					{
						typedef void (Panel::*MessageFunc_IntConstCharPtr_t)(int, const char *);
						(this->*((MessageFunc_IntConstCharPtr_t)pMap->func))( param1->GetInt(), param2->GetString() );
					}
					else if ( (DATATYPE_PTR == pMap->firstParamType) && (DATATYPE_CONSTCHARPTR == pMap->secondParamType) )
					{
						typedef void (Panel::*MessageFunc_PtrConstCharPtr_t)(void *, const char *);
						(this->*((MessageFunc_PtrConstCharPtr_t)pMap->func))( param1->GetPtr(), param2->GetString() );
					}
					else if ( (DATATYPE_PTR == pMap->firstParamType) && (DATATYPE_CONSTWCHARPTR == pMap->secondParamType) )
					{
						typedef void (Panel::*MessageFunc_PtrConstCharPtr_t)(void *, const wchar_t *);
						(this->*((MessageFunc_PtrConstCharPtr_t)pMap->func))( param1->GetPtr(), param2->GetWString() );
					}
					else
					{
						// the message isn't handled
						ivgui()->DPrintf( "Message '%s', sent to '%s', has invalid parameter types\n", params->GetName(), GetName() );
					}
					break;
				}

				default:
					Assert(!("Invalid number of parameters"));
					break;
				}

				// break the loop
				bFound = true;
				break;
			}
		}
	}

	if (!bFound)
	{
		OnOldMessage(const_cast<KeyValues *>(params), ifromPanel);
	}
}

void Panel::OnOldMessage(KeyValues *params, VPANEL ifromPanel)
{
	bool bFound = false;
	// message map dispatch
	int iMessageName = params->GetNameSymbol();

	PanelMap_t *panelMap = GetPanelMap();
	if ( !panelMap->processed )
	{
		PreparePanelMap( panelMap );
	}

	// iterate through the class hierarchy message maps
	for ( ; panelMap != NULL && !bFound; panelMap = panelMap->baseMap )
	{
		MessageMapItem_t *pMessageMap = panelMap->dataDesc;

		for ( int i = 0; i < panelMap->dataNumFields; i++ )
		{
			if (iMessageName == pMessageMap[i].nameSymbol)
			{
				// call the mapped function
				switch ( pMessageMap[i].numParams )
				{
				case 2:
					if ( (DATATYPE_INT == pMessageMap[i].firstParamType) && (DATATYPE_INT == pMessageMap[i].secondParamType) )
					{
						typedef void (Panel::*MessageFunc_IntInt_t)(int, int);
						(this->*((MessageFunc_IntInt_t)pMessageMap[i].func))( params->GetInt(pMessageMap[i].firstParamName), params->GetInt(pMessageMap[i].secondParamName) );
					}
					else if ( (DATATYPE_PTR == pMessageMap[i].firstParamType) && (DATATYPE_INT == pMessageMap[i].secondParamType) )
					{
						typedef void (Panel::*MessageFunc_PtrInt_t)(void *, int);
						(this->*((MessageFunc_PtrInt_t)pMessageMap[i].func))( params->GetPtr(pMessageMap[i].firstParamName), params->GetInt(pMessageMap[i].secondParamName) );
					}
					else if ( (DATATYPE_CONSTCHARPTR == pMessageMap[i].firstParamType) && (DATATYPE_INT == pMessageMap[i].secondParamType) )
					{
						typedef void (Panel::*MessageFunc_ConstCharPtrInt_t)(const char *, int);
						(this->*((MessageFunc_ConstCharPtrInt_t)pMessageMap[i].func))( params->GetString(pMessageMap[i].firstParamName), params->GetInt(pMessageMap[i].secondParamName) );
					}
					else if ( (DATATYPE_CONSTCHARPTR == pMessageMap[i].firstParamType) && (DATATYPE_CONSTCHARPTR == pMessageMap[i].secondParamType) )
					{
						typedef void (Panel::*MessageFunc_ConstCharPtrConstCharPtr_t)(const char *, const char *);
						(this->*((MessageFunc_ConstCharPtrConstCharPtr_t)pMessageMap[i].func))( params->GetString(pMessageMap[i].firstParamName), params->GetString(pMessageMap[i].secondParamName) );
					}
					else if ( (DATATYPE_INT == pMessageMap[i].firstParamType) && (DATATYPE_CONSTCHARPTR == pMessageMap[i].secondParamType) )
					{
						typedef void (Panel::*MessageFunc_IntConstCharPtr_t)(int, const char *);
						(this->*((MessageFunc_IntConstCharPtr_t)pMessageMap[i].func))( params->GetInt(pMessageMap[i].firstParamName), params->GetString(pMessageMap[i].secondParamName) );
					}
					else if ( (DATATYPE_PTR == pMessageMap[i].firstParamType) && (DATATYPE_CONSTCHARPTR == pMessageMap[i].secondParamType) )
					{
						typedef void (Panel::*MessageFunc_PtrConstCharPtr_t)(void *, const char *);
						(this->*((MessageFunc_PtrConstCharPtr_t)pMessageMap[i].func))( params->GetPtr(pMessageMap[i].firstParamName), params->GetString(pMessageMap[i].secondParamName) );
					}
					else if ( (DATATYPE_PTR == pMessageMap[i].firstParamType) && (DATATYPE_CONSTWCHARPTR == pMessageMap[i].secondParamType) )
					{
						typedef void (Panel::*MessageFunc_PtrConstCharPtr_t)(void *, const wchar_t *);
						(this->*((MessageFunc_PtrConstCharPtr_t)pMessageMap[i].func))( params->GetPtr(pMessageMap[i].firstParamName), params->GetWString(pMessageMap[i].secondParamName) );
					}
					else
					{
						// the message isn't handled
						ivgui()->DPrintf( "Message '%s', sent to '%s', has invalid parameter types\n", params->GetName(), GetName() );
					}
					break;

				case 1:
					switch ( pMessageMap[i].firstParamType )
					{
					case DATATYPE_BOOL:
						typedef void (Panel::*MessageFunc_Bool_t)(bool);
						(this->*((MessageFunc_Bool_t)pMessageMap[i].func))( (bool)params->GetInt(pMessageMap[i].firstParamName) );
						break;

					case DATATYPE_CONSTCHARPTR:
						typedef void (Panel::*MessageFunc_ConstCharPtr_t)(const char *);
						(this->*((MessageFunc_ConstCharPtr_t)pMessageMap[i].func))( (const char *)params->GetString(pMessageMap[i].firstParamName) );
						break;

					case DATATYPE_CONSTWCHARPTR:
						typedef void (Panel::*MessageFunc_ConstCharPtr_t)(const char *);
						(this->*((MessageFunc_ConstCharPtr_t)pMessageMap[i].func))( (const char *)params->GetWString(pMessageMap[i].firstParamName) );
						break;

					case DATATYPE_INT:
						typedef void (Panel::*MessageFunc_Int_t)(int);
						(this->*((MessageFunc_Int_t)pMessageMap[i].func))( params->GetInt(pMessageMap[i].firstParamName) );
						break;

					case DATATYPE_FLOAT:
						typedef void (Panel::*MessageFunc_Float_t)(float);
						(this->*((MessageFunc_Float_t)pMessageMap[i].func))( params->GetFloat(pMessageMap[i].firstParamName) );
						break;

					case DATATYPE_PTR:
						typedef void (Panel::*MessageFunc_Ptr_t)(void *);
						(this->*((MessageFunc_Ptr_t)pMessageMap[i].func))( (void *)params->GetPtr(pMessageMap[i].firstParamName) );
						break;

					case DATATYPE_KEYVALUES:
						typedef void (Panel::*MessageFunc_KeyValues_t)(KeyValues *);
						if ( pMessageMap[i].firstParamName )
						{
							(this->*((MessageFunc_KeyValues_t)pMessageMap[i].func))( (KeyValues *)params->GetPtr(pMessageMap[i].firstParamName) );
						}
						else
						{
							(this->*((MessageFunc_KeyValues_t)pMessageMap[i].func))( params );
						}
						break;

					default:
						// the message isn't handled
						ivgui()->DPrintf( "Message '%s', sent to '%s', has an invalid parameter type\n", params->GetName(), GetName() );
						break;
					}

					break;

				default:
					(this->*(pMessageMap[i].func))();
					break;
				};

				// break the loop
				bFound = true;
				break;
			}
		}
	}

	// message not handled
	// debug code
	if ( !bFound )
	{
		static int s_bDebugMessages = -1;
		if ( s_bDebugMessages == -1 )
		{
			s_bDebugMessages = CommandLine()->FindParm( "-vguimessages" ) ? 1 : 0;
		}
		if ( s_bDebugMessages == 1 )
		{
			ivgui()->DPrintf( "Message '%s' not handled by panel '%s'\n", params->GetName(), GetName() );
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: Safe call to get info from child panel by name
//-----------------------------------------------------------------------------
bool Panel::RequestInfoFromChild(const char *childName, KeyValues *outputData)
{
	Panel *panel = FindChildByName(childName);
	if (panel)
	{
		return panel->RequestInfo(outputData);
	}
	return false;
}

//-----------------------------------------------------------------------------
// Purpose: Posts a message
//-----------------------------------------------------------------------------
void Panel::PostMessage(Panel *target, KeyValues *message, float delay)
{
	ivgui()->PostMessage(target->GetVPanel(), message, GetVPanel(), delay);
}

void Panel::PostMessage(VPANEL target, KeyValues *message, float delaySeconds)
{
	ivgui()->PostMessage(target, message, GetVPanel(), delaySeconds);
}

//-----------------------------------------------------------------------------
// Purpose: Safe call to post a message to a child by name
//-----------------------------------------------------------------------------
void Panel::PostMessageToChild(const char *childName, KeyValues *message)
{
	Panel *panel = FindChildByName(childName);
	if (panel)
	{
		ivgui()->PostMessage(panel->GetVPanel(), message, GetVPanel());
	}
	else
	{
		message->deleteThis();
	}
}

//-----------------------------------------------------------------------------
// Purpose: Requests some information from the panel
//			Look through the message map for the handler
//-----------------------------------------------------------------------------
bool Panel::RequestInfo( KeyValues *outputData )
{
	if ( InternalRequestInfo( GetAnimMap(), outputData ) )
	{
		return true;
	}

	if (GetVParent())
	{
		return ipanel()->RequestInfo(GetVParent(), outputData);
	}

	return false;
}

//-----------------------------------------------------------------------------
// Purpose: sets a specified value in the control - inverse of RequestInfo
//-----------------------------------------------------------------------------
bool Panel::SetInfo(KeyValues *inputData)
{
	if ( InternalSetInfo( GetAnimMap(), inputData ) )
	{
		return true;
	}

	// doesn't chain to parent
	return false;
}

//-----------------------------------------------------------------------------
// Purpose: change the panel's silent mode; if silent, the panel will not post
//			any action signals
//-----------------------------------------------------------------------------

void Panel::SetSilentMode( bool bSilent )
{
	m_bIsSilent = bSilent;
}

//-----------------------------------------------------------------------------
// Purpose: Prepares the hierarchy panel maps for use (with message maps etc)
//-----------------------------------------------------------------------------
void Panel::PreparePanelMap( PanelMap_t *panelMap )
{
	// iterate through the class hierarchy message maps
	while ( panelMap != NULL && !panelMap->processed )
	{
		// fixup cross-dll boundary panel maps
		if ( panelMap->baseMap == (PanelMap_t*)0x00000001 )
		{
			panelMap->baseMap = &Panel::m_PanelMap;
		}

		// hash message map strings into symbols
		for (int i = 0; i < panelMap->dataNumFields; i++)
		{
			MessageMapItem_t *item = &panelMap->dataDesc[i];

			if (item->name)
			{
				item->nameSymbol = KeyValuesSystem()->GetSymbolForString(item->name);
			}
			else
			{
				item->nameSymbol = INVALID_KEY_SYMBOL;
			}
			if (item->firstParamName)
			{
				item->firstParamSymbol = KeyValuesSystem()->GetSymbolForString(item->firstParamName);
			}
			else
			{
				item->firstParamSymbol = INVALID_KEY_SYMBOL;
			}
			if (item->secondParamName)
			{
				item->secondParamSymbol = KeyValuesSystem()->GetSymbolForString(item->secondParamName);
			}
			else
			{
				item->secondParamSymbol = INVALID_KEY_SYMBOL;
			}
		}
		
		panelMap->processed = true;
		panelMap = panelMap->baseMap;
	}
}

//-----------------------------------------------------------------------------
// Purpose: Called to delete the panel
//-----------------------------------------------------------------------------
void Panel::OnDelete()
{
	Assert( IsX360() || ( IsPC() && _heapchk() == _HEAPOK ) );

	delete this;

	Assert( IsX360() || ( IsPC() && _heapchk() == _HEAPOK ) );
}

//-----------------------------------------------------------------------------
// Purpose: Panel handle implementation
//			Returns a pointer to a valid panel, NULL if the panel has been deleted
//-----------------------------------------------------------------------------
Panel *PHandle::Get() 
{
	if (m_iPanelID != INVALID_PANEL)
	{
		VPANEL panel = ivgui()->HandleToPanel(m_iPanelID);
		if (panel)
		{
			Panel *vguiPanel = ipanel()->GetPanel(panel, GetControlsModuleName());
			return vguiPanel;
		}
	}
	return NULL;
}

//-----------------------------------------------------------------------------
// Purpose: sets the smart pointer
//-----------------------------------------------------------------------------
Panel *PHandle::Set(Panel *pent)
{
	if (pent)
	{
		m_iPanelID = ivgui()->PanelToHandle(pent->GetVPanel());
	}
	else
	{
		m_iPanelID = INVALID_PANEL;
	}
	return pent; 
}

Panel *PHandle::Set( HPanel hPanel )
{
	m_iPanelID = hPanel;
	return Get();
}


//-----------------------------------------------------------------------------
// Purpose: Returns a handle to a valid panel, NULL if the panel has been deleted
//-----------------------------------------------------------------------------
VPANEL VPanelHandle::Get()
{
	if (m_iPanelID != INVALID_PANEL)
	{
		return ivgui()->HandleToPanel(m_iPanelID);
	}
	return NULL;
}

//-----------------------------------------------------------------------------
// Purpose: sets the smart pointer
//-----------------------------------------------------------------------------
VPANEL VPanelHandle::Set(VPANEL pent)
{
	if (pent)
	{
		m_iPanelID = ivgui()->PanelToHandle(pent);
	}
	else
	{
		m_iPanelID = INVALID_PANEL;
	}
	return pent; 
}

//-----------------------------------------------------------------------------
// Purpose: returns a pointer to the tooltip object associated with the panel
//-----------------------------------------------------------------------------
Tooltip *Panel::GetTooltip()
{
	if (!m_pTooltips)
	{
		m_pTooltips = new Tooltip(this, NULL);
	}

	return m_pTooltips;
}

//-----------------------------------------------------------------------------
// Purpose: sets the proportional flag on this panel and all it's children
//-----------------------------------------------------------------------------
void Panel::SetProportional(bool state)
{ 
	// only do something if the state changes
	if( state != _flags.IsFlagSet( IS_PROPORTIONAL ) )
	{
		_flags.SetFlag( IS_PROPORTIONAL, state );	

		for(int i=0;i<GetChildCount();i++)
		{
			// recursively apply to all children
			GetChild(i)->SetProportional( IsProportional() );
		}
	}
	InvalidateLayout();
}


void Panel::SetKeyBoardInputEnabled( bool state )
{
	ipanel()->SetKeyBoardInputEnabled( GetVPanel(), state );
	for ( int i = 0; i < GetChildCount(); i++ )
	{
		GetChild(i)->SetKeyBoardInputEnabled( state );
	}

	// If turning off keyboard input enable, then make sure
	// this panel is not the current key focus of a parent panel
	if ( !state )
	{
		Panel *pParent = GetParent();
		if ( pParent )
		{
			if ( pParent->GetCurrentKeyFocus() == GetVPanel() )
			{
				pParent->RequestFocusNext();
			}
		}
	}
}

void Panel::SetMouseInputEnabled( bool state )
{
	ipanel()->SetMouseInputEnabled( GetVPanel(), state );
/*	for(int i=0;i<GetChildCount();i++)
	{
		GetChild(i)->SetMouseInput(state);
	}*/
	vgui::surface()->CalculateMouseVisible();
}

bool Panel::IsKeyBoardInputEnabled()
{
	return ipanel()->IsKeyBoardInputEnabled( GetVPanel() );
}

bool Panel::IsMouseInputEnabled()
{
	return ipanel()->IsMouseInputEnabled( GetVPanel() );
}

class CFloatProperty : public vgui::IPanelAnimationPropertyConverter
{
public:
	virtual void GetData( Panel *panel, KeyValues *kv, PanelAnimationMapEntry *entry )
	{
		void *data = ( void * )( (*entry->m_pfnLookup)( panel ) );
		kv->SetFloat( entry->name(), *(float *)data );
	}
	
	virtual void SetData( Panel *panel, KeyValues *kv, PanelAnimationMapEntry *entry )
	{
		void *data = ( void * )( (*entry->m_pfnLookup)( panel ) );
		*(float *)data = kv->GetFloat( entry->name() );
	}

	virtual void InitFromDefault( Panel *panel, PanelAnimationMapEntry *entry )
	{
		void *data = ( void * )( (*entry->m_pfnLookup)( panel ) );
		*(float *)data = atof( entry->defaultvalue() );
	}
};

class CProportionalFloatProperty : public vgui::IPanelAnimationPropertyConverter
{
public:
	virtual void GetData( Panel *panel, KeyValues *kv, PanelAnimationMapEntry *entry )
	{
		void *data = ( void * )( (*entry->m_pfnLookup)( panel ) );
		float f = *(float *)data;
		f = scheme()->GetProportionalNormalizedValueEx( panel->GetScheme(), f );
		kv->SetFloat( entry->name(), f );
	}
	
	virtual void SetData( Panel *panel, KeyValues *kv, PanelAnimationMapEntry *entry )
	{
		void *data = ( void * )( (*entry->m_pfnLookup)( panel ) );
		float f = kv->GetFloat( entry->name() );
		f = scheme()->GetProportionalScaledValueEx( panel->GetScheme(), f );
		*(float *)data = f;
	}

	virtual void InitFromDefault( Panel *panel, PanelAnimationMapEntry *entry )
	{
		void *data = ( void * )( (*entry->m_pfnLookup)( panel ) );
		float f = atof( entry->defaultvalue() );
		f = scheme()->GetProportionalScaledValueEx( panel->GetScheme(), f );
		*(float *)data = f;
	}
};

class CIntProperty : public vgui::IPanelAnimationPropertyConverter
{
public:
	virtual void GetData( Panel *panel, KeyValues *kv, PanelAnimationMapEntry *entry )
	{
		void *data = ( void * )( (*entry->m_pfnLookup)( panel ) );
		kv->SetInt( entry->name(), *(int *)data );
	}
	
	virtual void SetData( Panel *panel, KeyValues *kv, PanelAnimationMapEntry *entry )
	{
		void *data = ( void * )( (*entry->m_pfnLookup)( panel ) );
		*(int *)data = kv->GetInt( entry->name() );
	}

	virtual void InitFromDefault( Panel *panel, PanelAnimationMapEntry *entry )
	{
		void *data = ( void * )( (*entry->m_pfnLookup)( panel ) );
		*(int *)data = atoi( entry->defaultvalue() );
	}
};

class CProportionalIntProperty : public vgui::IPanelAnimationPropertyConverter
{
public:
	virtual void GetData( Panel *panel, KeyValues *kv, PanelAnimationMapEntry *entry )
	{
		void *data = ( void * )( (*entry->m_pfnLookup)( panel ) );
		int i = *(int *)data;
		i = scheme()->GetProportionalNormalizedValueEx( panel->GetScheme(), i );
		kv->SetInt( entry->name(), i );
	}
	
	virtual void SetData( Panel *panel, KeyValues *kv, PanelAnimationMapEntry *entry )
	{
		void *data = ( void * )( (*entry->m_pfnLookup)( panel ) );
		int i = kv->GetInt( entry->name() );
		i = scheme()->GetProportionalScaledValueEx( panel->GetScheme(), i );
		*(int *)data = i;
	}
	virtual void InitFromDefault( Panel *panel, PanelAnimationMapEntry *entry )
	{
		void *data = ( void * )( (*entry->m_pfnLookup)( panel ) );
		int i = atoi( entry->defaultvalue() );
		i = scheme()->GetProportionalScaledValueEx( panel->GetScheme(), i );
		*(int *)data = i;
	}
};

class CColorProperty : public vgui::IPanelAnimationPropertyConverter
{
public:
	virtual void GetData( Panel *panel, KeyValues *kv, PanelAnimationMapEntry *entry )
	{
		void *data = ( void * )( (*entry->m_pfnLookup)( panel ) );
		kv->SetColor( entry->name(), *(Color *)data );
	}
	
	virtual void SetData( Panel *panel, KeyValues *kv, PanelAnimationMapEntry *entry )
	{
		vgui::IScheme *scheme = vgui::scheme()->GetIScheme( panel->GetScheme() );
		Assert( scheme );
		if ( scheme )
		{
			void *data = ( void * )( (*entry->m_pfnLookup)( panel ) );

			char const *colorName = kv->GetString( entry->name() );
			if ( !colorName || !colorName[0] )
			{
				*(Color *)data = kv->GetColor( entry->name() );
			}
			else
			{
				*(Color *)data = scheme->GetColor( colorName, Color( 0, 0, 0, 0 ) );
			}
		}
	}

	virtual void InitFromDefault( Panel *panel, PanelAnimationMapEntry *entry )
	{
		vgui::IScheme *scheme = vgui::scheme()->GetIScheme( panel->GetScheme() );
		Assert( scheme );
		if ( scheme )
		{
			void *data = ( void * )( (*entry->m_pfnLookup)( panel ) );
			*(Color *)data = scheme->GetColor( entry->defaultvalue(), Color( 0, 0, 0, 0 ) );
		}
	}
};

class CBoolProperty : public vgui::IPanelAnimationPropertyConverter
{
public:
	virtual void GetData( Panel *panel, KeyValues *kv, PanelAnimationMapEntry *entry )
	{
		void *data = ( void * )( (*entry->m_pfnLookup)( panel ) );
		kv->SetInt( entry->name(), *(bool *)data ? 1 : 0 );
	}
	
	virtual void SetData( Panel *panel, KeyValues *kv, PanelAnimationMapEntry *entry )
	{
		void *data = ( void * )( (*entry->m_pfnLookup)( panel ) );
		*(bool *)data = kv->GetInt( entry->name() ) ? true : false;
	}

	virtual void InitFromDefault( Panel *panel, PanelAnimationMapEntry *entry )
	{
		void *data = ( void * )( (*entry->m_pfnLookup)( panel ) );
		bool b = false;
		if ( !stricmp( entry->defaultvalue(), "true" )||
			atoi( entry->defaultvalue() )!= 0 )
		{
			b = true;
		}

		*(bool *)data = b;
	}
};

class CStringProperty : public vgui::IPanelAnimationPropertyConverter
{
public:
	virtual void GetData( Panel *panel, KeyValues *kv, PanelAnimationMapEntry *entry )
	{
		void *data = ( void * )( (*entry->m_pfnLookup)( panel ) );
		kv->SetString( entry->name(), (char *)data );
	}
	
	virtual void SetData( Panel *panel, KeyValues *kv, PanelAnimationMapEntry *entry )
	{
		void *data = ( void * )( (*entry->m_pfnLookup)( panel ) );
		strcpy( (char *)data, kv->GetString( entry->name() ) );
	}

	virtual void InitFromDefault( Panel *panel, PanelAnimationMapEntry *entry )
	{
		void *data = ( void * )( (*entry->m_pfnLookup)( panel ) );
		strcpy( ( char * )data, entry->defaultvalue() );
	}
};

class CHFontProperty : public vgui::IPanelAnimationPropertyConverter
{
public:
	virtual void GetData( Panel *panel, KeyValues *kv, PanelAnimationMapEntry *entry )
	{
		vgui::IScheme *scheme = vgui::scheme()->GetIScheme( panel->GetScheme() );
		Assert( scheme );
		if ( scheme )
		{
			void *data = ( void * )( (*entry->m_pfnLookup)( panel ) );
			char const *fontName = scheme->GetFontName( *(HFont *)data );
			kv->SetString( entry->name(), fontName );
		}
	}
	
	virtual void SetData( Panel *panel, KeyValues *kv, PanelAnimationMapEntry *entry )
	{
		vgui::IScheme *scheme = vgui::scheme()->GetIScheme( panel->GetScheme() );
		Assert( scheme );
		if ( scheme )
		{
			void *data = ( void * )( (*entry->m_pfnLookup)( panel ) );
			char const *fontName = kv->GetString( entry->name() );
			*(HFont *)data = scheme->GetFont( fontName, panel->IsProportional() );
		}
	}

	virtual void InitFromDefault( Panel *panel, PanelAnimationMapEntry *entry )
	{
		vgui::IScheme *scheme = vgui::scheme()->GetIScheme( panel->GetScheme() );
		Assert( scheme );
		if ( scheme )
		{
			void *data = ( void * )( (*entry->m_pfnLookup)( panel ) );
			*(HFont *)data = scheme->GetFont( entry->defaultvalue(), panel->IsProportional() );
		}
	}
};

class CTextureIdProperty : public vgui::IPanelAnimationPropertyConverter
{
public:
	virtual void GetData( Panel *panel, KeyValues *kv, PanelAnimationMapEntry *entry )
	{
		void *data = ( void * )( (*entry->m_pfnLookup)( panel ) );
		int currentId = *(int *)data;

		// lookup texture name for id
		char texturename[ 512 ];
		if ( currentId != -1 &&
			surface()->DrawGetTextureFile( currentId, texturename, sizeof( texturename ) ) )
		{
			kv->SetString( entry->name(), texturename );
		}
		else
		{
			kv->SetString( entry->name(), "" );
		}
	}
	
	virtual void SetData( Panel *panel, KeyValues *kv, PanelAnimationMapEntry *entry )
	{
		void *data = ( void * )( (*entry->m_pfnLookup)( panel ) );

		int currentId = -1;

		char const *texturename = kv->GetString( entry->name() );
		if ( texturename && texturename[ 0 ] )
		{
			currentId = surface()->DrawGetTextureId( texturename );
			if ( currentId == -1 )
			{
				currentId = surface()->CreateNewTextureID();
			}
			surface()->DrawSetTextureFile( currentId, texturename, false, true );
		}

		*(int *)data = currentId;
	}

	virtual void InitFromDefault( Panel *panel, PanelAnimationMapEntry *entry )
	{
		void *data = ( void * )( (*entry->m_pfnLookup)( panel ) );

		int currentId = -1;

		char const *texturename = entry->defaultvalue();
		if ( texturename && texturename[ 0 ] )
		{
			currentId = surface()->DrawGetTextureId( texturename );
			if ( currentId == -1 )
			{
				currentId = surface()->CreateNewTextureID();
			}
			surface()->DrawSetTextureFile( currentId, texturename, false, true );
		}

		*(int *)data = currentId;
	}
};

static CFloatProperty floatconverter;
static CProportionalFloatProperty p_floatconverter;
static CIntProperty intconverter;
static CProportionalIntProperty p_intconverter;
static CColorProperty colorconverter;
static CBoolProperty boolconverter;
static CStringProperty stringconverter;
static CHFontProperty fontconverter;
static CTextureIdProperty textureidconverter;

static CUtlDict< IPanelAnimationPropertyConverter *, int > g_AnimationPropertyConverters;

static IPanelAnimationPropertyConverter *FindConverter( char const *typeName )
{
	int lookup = g_AnimationPropertyConverters.Find( typeName );
	if ( lookup == g_AnimationPropertyConverters.InvalidIndex() )
		return NULL;

	IPanelAnimationPropertyConverter *converter = g_AnimationPropertyConverters[ lookup ];
	return converter;
}

void Panel::AddPropertyConverter( char const *typeName, IPanelAnimationPropertyConverter *converter )
{
	int lookup = g_AnimationPropertyConverters.Find( typeName );
	if ( lookup != g_AnimationPropertyConverters.InvalidIndex() )
	{
		Msg( "Already have converter for type %s, ignoring...\n", typeName );
		return;
	}

	g_AnimationPropertyConverters.Insert( typeName, converter );
}

//-----------------------------------------------------------------------------
// Purpose: Static method to initialize all needed converters
//-----------------------------------------------------------------------------
void Panel::InitPropertyConverters( void )
{
	static bool initialized = false;
	if ( initialized )
		return;
	initialized = true;

	AddPropertyConverter( "float", &floatconverter );
	AddPropertyConverter( "int", &intconverter );
	AddPropertyConverter( "Color", &colorconverter );
//	AddPropertyConverter( "vgui::Color", &colorconverter );
	AddPropertyConverter( "bool", &boolconverter );
	AddPropertyConverter( "char", &stringconverter );
	AddPropertyConverter( "string", &stringconverter );
	AddPropertyConverter( "HFont", &fontconverter );
	AddPropertyConverter( "vgui::HFont", &fontconverter );

	// This is an aliased type for proportional float
	AddPropertyConverter( "proportional_float", &p_floatconverter );
	AddPropertyConverter( "proportional_int", &p_intconverter );

	AddPropertyConverter( "textureid", &textureidconverter );
}

bool Panel::InternalRequestInfo( PanelAnimationMap *map, KeyValues *outputData )
{
	if ( !map )
		return false;

	Assert( outputData );

	char const *name = outputData->GetName();

	PanelAnimationMapEntry *e = FindPanelAnimationEntry( name, map );
	if ( e )
	{
		IPanelAnimationPropertyConverter *converter = FindConverter( e->type() );
		if ( converter )
		{
			converter->GetData( this, outputData, e );
			return true;
		}
	}

	return false;
}

bool Panel::InternalSetInfo( PanelAnimationMap *map, KeyValues *inputData )
{
	if ( !map )
		return false;

	Assert( inputData );

	char const *name = inputData->GetName();

	PanelAnimationMapEntry *e = FindPanelAnimationEntry( name, map );
	if ( e )
	{
		IPanelAnimationPropertyConverter *converter = FindConverter( e->type() );
		if ( converter )
		{
			converter->SetData( this, inputData, e );
			return true;
		}
	}

	return false;
}

PanelAnimationMapEntry *Panel::FindPanelAnimationEntry( char const *scriptname, PanelAnimationMap *map )
{
	if ( !map )
		return NULL;

	Assert( scriptname );

	// Look through mapping for entry
	int c = map->entries.Count();
	for ( int i = 0; i < c; i++ )
	{
		PanelAnimationMapEntry *e = &map->entries[ i ];

		if ( !stricmp( e->name(), scriptname ) )
		{
			return e;
		}
	}

	// Recurse
	if ( map->baseMap )
	{
		return FindPanelAnimationEntry( scriptname, map->baseMap );
	}

	return NULL;
}

// Recursively invoke settings for PanelAnimationVars
void Panel::InternalApplySettings( PanelAnimationMap *map, KeyValues *inResourceData)
{
	// Loop through keys
	KeyValues *kv;
	
	for ( kv = inResourceData->GetFirstSubKey(); kv; kv = kv->GetNextKey() )
	{
		char const *varname = kv->GetName();

		PanelAnimationMapEntry *entry = FindPanelAnimationEntry( varname, GetAnimMap() );
		if ( entry )
		{
			// Set value to value from script
			IPanelAnimationPropertyConverter *converter = FindConverter( entry->type() );
			if ( converter )
			{
				converter->SetData( this, inResourceData, entry );
			}
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: sets the default values of all CPanelAnimationVars
//-----------------------------------------------------------------------------
void  Panel::InternalInitDefaultValues( PanelAnimationMap *map )
{
	_flags.ClearFlag( NEEDS_DEFAULT_SETTINGS_APPLIED );

	// Look through mapping for entry
	int c = map->entries.Count();
	for ( int i = 0; i < c; i++ )
	{
		PanelAnimationMapEntry *e = &map->entries[ i ];
		Assert( e );
		IPanelAnimationPropertyConverter *converter = FindConverter( e->type() );
		if ( !converter )
			continue;

		converter->InitFromDefault( this, e );
	}

	if ( map->baseMap )
	{	
		InternalInitDefaultValues( map->baseMap );
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  :  - 
//-----------------------------------------------------------------------------
int	Panel::GetPaintBackgroundType()
{
	return m_nPaintBackgroundType;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : w - 
//			h - 
//-----------------------------------------------------------------------------
void Panel::GetCornerTextureSize( int& w, int& h )
{
	if ( m_nBgTextureId1 == -1 )
	{
		w = h = 0;
		return;
	}
	surface()->DrawGetTextureSize(m_nBgTextureId1, w, h);
}

//-----------------------------------------------------------------------------
// Purpose: draws a selection box
//-----------------------------------------------------------------------------
void Panel::DrawBox(int x, int y, int wide, int tall, Color color, float normalizedAlpha, bool hollow /*=false*/ )
{
	if ( m_nBgTextureId1 == -1 ||
		 m_nBgTextureId2 == -1 ||
		 m_nBgTextureId3 == -1 ||
		 m_nBgTextureId4 == -1 )
	{
		return;
	}

	color[3] *= normalizedAlpha;

	// work out our bounds
	int cornerWide, cornerTall;
	GetCornerTextureSize( cornerWide, cornerTall );

	// draw the background in the areas not occupied by the corners
	// draw it in three horizontal strips
	surface()->DrawSetColor(color);
	surface()->DrawFilledRect(x + cornerWide, y, x + wide - cornerWide,	y + cornerTall);
	if ( !hollow )
	{
		surface()->DrawFilledRect(x, y + cornerTall, x + wide, y + tall - cornerTall);
	}
	else
	{
		surface()->DrawFilledRect(x, y + cornerTall, x + cornerWide, y + tall - cornerTall);
		surface()->DrawFilledRect(x + wide - cornerWide, y + cornerTall, x + wide, y + tall - cornerTall);
	}
	surface()->DrawFilledRect(x + cornerWide, y + tall - cornerTall, x + wide - cornerWide, y + tall);

	// draw the corners
	surface()->DrawSetTexture(m_nBgTextureId1);
	surface()->DrawTexturedRect(x, y, x + cornerWide, y + cornerTall);
	surface()->DrawSetTexture(m_nBgTextureId2);
	surface()->DrawTexturedRect(x + wide - cornerWide, y, x + wide, y + cornerTall);
	surface()->DrawSetTexture(m_nBgTextureId3);
	surface()->DrawTexturedRect(x + wide - cornerWide, y + tall - cornerTall, x + wide, y + tall);
	surface()->DrawSetTexture(m_nBgTextureId4);
	surface()->DrawTexturedRect(x + 0, y + tall - cornerTall, x + cornerWide, y + tall);
}

void Panel::DrawBoxFade(int x, int y, int wide, int tall, Color color, float normalizedAlpha, unsigned int alpha0, unsigned int alpha1, bool bHorizontal, bool hollow /*=false*/ )
{
	if ( m_nBgTextureId1 == -1 ||
		m_nBgTextureId2 == -1 ||
		m_nBgTextureId3 == -1 ||
		m_nBgTextureId4 == -1 ||
		surface()->DrawGetAlphaMultiplier() == 0 )
	{
		return;
	}

	color[3] *= normalizedAlpha;

	// work out our bounds
	int cornerWide, cornerTall;
	GetCornerTextureSize( cornerWide, cornerTall );

	if ( !bHorizontal )
	{
		// draw the background in the areas not occupied by the corners
		// draw it in three horizontal strips
		surface()->DrawSetColor(color);
		surface()->DrawFilledRectFade(x + cornerWide, y, x + wide - cornerWide,	y + cornerTall, alpha0, alpha0, bHorizontal );
		if ( !hollow )
		{
			surface()->DrawFilledRectFade(x, y + cornerTall, x + wide, y + tall - cornerTall, alpha0, alpha1, bHorizontal);
		}
		else
		{
			surface()->DrawFilledRectFade(x, y + cornerTall, x + cornerWide, y + tall - cornerTall, alpha0, alpha1, bHorizontal);
			surface()->DrawFilledRectFade(x + wide - cornerWide, y + cornerTall, x + wide, y + tall - cornerTall, alpha0, alpha1, bHorizontal);
		}
		surface()->DrawFilledRectFade(x + cornerWide, y + tall - cornerTall, x + wide - cornerWide, y + tall, alpha1, alpha1, bHorizontal);
	}
	else
	{
		// draw the background in the areas not occupied by the corners
		// draw it in three horizontal strips
		surface()->DrawSetColor(color);
		surface()->DrawFilledRectFade(x, y + cornerTall, x + cornerWide, y + tall - cornerTall, alpha0, alpha0, bHorizontal );
		if ( !hollow )
		{
			surface()->DrawFilledRectFade(x + cornerWide, y, x + wide - cornerWide, y + tall, alpha0, alpha1, bHorizontal);
		}
		else
		{
			// FIXME: Hollow horz version not implemented
			//surface()->DrawFilledRectFade(x, y + cornerTall, x + cornerWide, y + tall - cornerTall, alpha0, alpha1, bHorizontal);
			//surface()->DrawFilledRectFade(x + wide - cornerWide, y + cornerTall, x + wide, y + tall - cornerTall, alpha0, alpha1, bHorizontal);
		}
		surface()->DrawFilledRectFade(x + wide - cornerWide, y + cornerTall, x + wide, y + tall - cornerTall, alpha1, alpha1, bHorizontal);
	}

	float fOldAlpha = color[ 3 ];
	int iAlpha0 = fOldAlpha * ( static_cast<float>( alpha0 ) / 255.0f );
	int iAlpha1 = fOldAlpha * ( static_cast<float>( alpha1 ) / 255.0f );

	// draw the corners
	if ( !bHorizontal )
	{
		color[ 3 ] = iAlpha0;
		surface()->DrawSetColor( color );
		surface()->DrawSetTexture(m_nBgTextureId1);
		surface()->DrawTexturedRect(x, y, x + cornerWide, y + cornerTall);
		surface()->DrawSetTexture(m_nBgTextureId2);
		surface()->DrawTexturedRect(x + wide - cornerWide, y, x + wide, y + cornerTall);

		color[ 3 ] = iAlpha1;
		surface()->DrawSetColor( color );
		surface()->DrawSetTexture(m_nBgTextureId3);
		surface()->DrawTexturedRect(x + wide - cornerWide, y + tall - cornerTall, x + wide, y + tall);
		surface()->DrawSetTexture(m_nBgTextureId4);
		surface()->DrawTexturedRect(x + 0, y + tall - cornerTall, x + cornerWide, y + tall);
	}
	else
	{
		color[ 3 ] = iAlpha0;
		surface()->DrawSetColor( color );
		surface()->DrawSetTexture(m_nBgTextureId1);
		surface()->DrawTexturedRect(x, y, x + cornerWide, y + cornerTall);
		surface()->DrawSetTexture(m_nBgTextureId4);
		surface()->DrawTexturedRect(x + 0, y + tall - cornerTall, x + cornerWide, y + tall);

		color[ 3 ] = iAlpha1;
		surface()->DrawSetColor( color );
		surface()->DrawSetTexture(m_nBgTextureId2);
		surface()->DrawTexturedRect(x + wide - cornerWide, y, x + wide, y + cornerTall);
		surface()->DrawSetTexture(m_nBgTextureId3);
		surface()->DrawTexturedRect(x + wide - cornerWide, y + tall - cornerTall, x + wide, y + tall);
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : x - 
//			y - 
//			wide - 
//			tall - 
//			color - 
//			normalizedAlpha - 
//-----------------------------------------------------------------------------
void Panel::DrawHollowBox(int x, int y, int wide, int tall, Color color, float normalizedAlpha )
{
	DrawBox( x, y, wide, tall, color, normalizedAlpha, true );
}

//-----------------------------------------------------------------------------
// Purpose: draws a selection box
//-----------------------------------------------------------------------------
void Panel::DrawTexturedBox(int x, int y, int wide, int tall, Color color, float normalizedAlpha )
{
	if ( m_nBgTextureId1 == -1 )
		return;

	color[3] *= normalizedAlpha;

	surface()->DrawSetColor( color );
	surface()->DrawSetTexture(m_nBgTextureId1);
	surface()->DrawTexturedRect(x, y, x + wide, y + tall);
}

//-----------------------------------------------------------------------------
// Purpose: Marks this panel as draggable (note that children will chain to their parents to see if any parent is draggable)
// Input  : enabled - 
//-----------------------------------------------------------------------------
void Panel::SetDragEnabled( bool enabled )
{
#if defined( VGUI_USEDRAGDROP )
	// If turning it off, quit dragging if mid-drag
	if ( !enabled && 
		m_pDragDrop->m_bDragging )
	{
		OnFinishDragging( false, (MouseCode)-1 );
	}
	m_pDragDrop->m_bDragEnabled = enabled;
#endif
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  :  - 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool Panel::IsDragEnabled() const
{
#if defined( VGUI_USEDRAGDROP )
	return m_pDragDrop->m_bDragEnabled;
#endif
	return false;
}

// Use this to prevent chaining up from a parent which can mess with mouse functionality if you don't want to chain up from a child panel to the best
//  draggable parent.
void Panel::SetBlockDragChaining( bool block )
{
#if defined( VGUI_USEDRAGDROP )
	m_pDragDrop->m_bPreventChaining = block;
#endif
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  :  - 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool Panel::IsBlockingDragChaining() const
{
#if defined( VGUI_USEDRAGDROP )
	return m_pDragDrop->m_bPreventChaining;
#endif
	return true;
}


//-----------------------------------------------------------------------------
// accessors for m_nDragStartTolerance
//-----------------------------------------------------------------------------
int Panel::GetDragStartTolerance() const
{
#if defined( VGUI_USEDRAGDROP )
	return m_pDragDrop->m_nDragStartTolerance;
#endif
	return 0;
}

void Panel::SetDragSTartTolerance( int nTolerance )
{
#if defined( VGUI_USEDRAGDROP )
	m_pDragDrop->m_nDragStartTolerance = nTolerance;
#endif
}


//-----------------------------------------------------------------------------
// Purpose: Marks this panel as droppable ( note that children will chain to their parents to see if any parent is droppable)
// Input  : enabled - 
//-----------------------------------------------------------------------------
void Panel::SetDropEnabled( bool enabled, float flHoverContextTime /* = 0.0f */ )
{
#if defined( VGUI_USEDRAGDROP )
	m_pDragDrop->m_bDropEnabled = enabled;
	m_pDragDrop->m_flHoverContextTime = flHoverContextTime;
#endif
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  :  - 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool Panel::IsDropEnabled() const
{
#if defined( VGUI_USEDRAGDROP )
	return m_pDragDrop->m_bDropEnabled;
#endif
	return false;
}

//-----------------------------------------------------------------------------
// Purpose: Chains up to any parent 
// 1) marked DropEnabled; and 
// 2) willing to accept the drop payload
// Input  :  - 
// Output : Panel
//-----------------------------------------------------------------------------
Panel *Panel::GetDropTarget( CUtlVector< KeyValues * >& msglist )
{
#if defined( VGUI_USEDRAGDROP )
	// Found one
	if ( m_pDragDrop->m_bDropEnabled && 
		IsDroppable( msglist ) )
	{
		return this;
	}

	// Chain up
	if ( GetParent() )
	{
		return GetParent()->GetDropTarget( msglist );
	}
#endif
	// No luck
	return NULL;
}

//-----------------------------------------------------------------------------
// Purpose: Chains up to first parent marked DragEnabled
// Input  :  - 
// Output : Panel
//-----------------------------------------------------------------------------
Panel *Panel::GetDragPanel()
{
#if defined( VGUI_USEDRAGDROP )
	// If we encounter a blocker, stop chaining
	if ( m_pDragDrop->m_bPreventChaining )
		return NULL;

	if ( m_pDragDrop->m_bDragEnabled )
		return this;

	// Chain up
	if ( GetParent() )
	{
		return GetParent()->GetDragPanel();
	}
#endif
	// No luck
	return NULL;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  :  - 
//-----------------------------------------------------------------------------
void Panel::OnStartDragging()
{
#if defined( VGUI_USEDRAGDROP )
	// Only left mouse initiates drag/drop.
	// FIXME: Revisit?
	if ( !input()->IsMouseDown( MOUSE_LEFT ) )
		return;

	if ( !m_pDragDrop->m_bDragEnabled )
		return;

	if ( m_pDragDrop->m_bDragging )
		return;

	g_DragDropCapture = this;

	m_pDragDrop->m_bDragStarted = false;
	m_pDragDrop->m_bDragging = true;
	input()->GetCursorPos( m_pDragDrop->m_nStartPos[ 0 ], m_pDragDrop->m_nStartPos[ 1 ] );
	m_pDragDrop->m_nLastPos[ 0 ] = m_pDragDrop->m_nStartPos[ 0 ];
	m_pDragDrop->m_nLastPos[ 1 ] = m_pDragDrop->m_nStartPos[ 1 ];

	OnContinueDragging();
#endif
}

//-----------------------------------------------------------------------------
// Purpose: Called if drag drop is started but not dropped on top of droppable panel...
// Input  :  - 
//-----------------------------------------------------------------------------
void Panel::OnDragFailed( CUtlVector< KeyValues * >& msglist )
{
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  :  - 
//-----------------------------------------------------------------------------
void Panel::OnFinishDragging( bool mousereleased, MouseCode code, bool abort /*= false*/ )
{
#if defined( VGUI_USEDRAGDROP )
	g_DragDropCapture = NULL;

	if ( !m_pDragDrop->m_bDragEnabled )
		return;

	Assert( m_pDragDrop->m_bDragging );

	if ( !m_pDragDrop->m_bDragging )
		return;

	int x, y;
	input()->GetCursorPos( x, y );

    m_pDragDrop->m_nLastPos[ 0 ] = x;
	m_pDragDrop->m_nLastPos[ 1 ] = y;

	if ( s_DragDropHelper.Get() )
	{
		s_DragDropHelper->RemovePanel( this );
	}

	m_pDragDrop->m_bDragging = false;

	CUtlVector< KeyValues * >& data = m_pDragDrop->m_DragData;
	int c = data.Count();

	Panel *target = NULL;
	bool shouldDrop = false;

	if ( m_pDragDrop->m_bDragStarted )
	{
		char cmd[ 256 ];
		Q_strncpy( cmd, "default", sizeof( cmd ) );

		if ( mousereleased &&
			m_pDragDrop->m_hCurrentDrop != NULL &&
			m_pDragDrop->m_hDropContextMenu.Get() )
		{
			Menu *menu = m_pDragDrop->m_hDropContextMenu;

			VPANEL hover = menu->IsWithinTraverse( x, y, false );
			if ( hover )
			{
				Panel *pHover = ipanel()->GetPanel( hover, GetModuleName() );
				if ( pHover )
				{
					// Figure out if it's a menu item...
					int c = menu->GetItemCount();
					for ( int i = 0; i < c; ++i )
					{
						int id = menu->GetMenuID( i );
						MenuItem *item = menu->GetMenuItem( id );
						if ( item == pHover )
						{
							KeyValues *command = item->GetCommand();
							if ( command )
							{
								char const *p = command->GetString( "command", "" );
								if ( p && p[ 0 ] )
								{
									Q_strncpy( cmd, p, sizeof( cmd ) );
								}
							}
						}
					}
				}
			}

			delete menu;
			m_pDragDrop->m_hDropContextMenu = NULL;
		}

		for ( int i = 0 ; i < c; ++i )
		{
			KeyValues *msg = data[ i ];

			msg->SetString( "command", cmd );

			msg->SetInt( "screenx", x );
			msg->SetInt( "screeny", y );
		}

		target = m_pDragDrop->m_hCurrentDrop.Get();
		if ( target && !abort )
		{
			int localmousex = x, localmousey = y;
			// Convert screen space coordintes to coordinates relative to drop window
			target->ScreenToLocal( localmousex, localmousey );

			for ( int i = 0 ; i < c; ++i )
			{
				KeyValues *msg = data[ i ];

				msg->SetInt( "x", localmousex );
				msg->SetInt( "y", localmousey );
			}

			shouldDrop = true;
		}

		if ( !shouldDrop )
		{
			OnDragFailed( data );
		}
	}

	m_pDragDrop->m_bDragStarted = false;
	m_pDragDrop->m_DragPanels.RemoveAll();
	m_pDragDrop->m_hCurrentDrop = NULL;

	// Copy data ptrs out of data because OnPanelDropped might cause this panel to be deleted
	// and our this ptr will be hosed...
	CUtlVector< KeyValues * > temp;
	for ( int i = 0 ; i < c; ++i )
	{
		temp.AddToTail( data[ i ] );
	}
	data.RemoveAll();

	if ( shouldDrop && target )
	{
		target->OnPanelDropped( temp );
	}
	for ( int i = 0 ; i < c ; ++i )
	{
        temp[ i ]->deleteThis();
	}
#endif
}

void Panel::OnDropContextHoverShow( CUtlVector< KeyValues * >& msglist )
{
}

void Panel::OnDropContextHoverHide( CUtlVector< KeyValues * >& msglist )
{
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *msg - 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool Panel::IsDroppable( CUtlVector< KeyValues * >& msglist )
{
	return false;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : startx - 
//			starty - 
//			mx - 
//			my - 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool Panel::CanStartDragging( int startx, int starty, int mx, int my )
{
#if defined( VGUI_USEDRAGDROP )
	if ( IsStartDragWhenMouseExitsPanel() )
	{
		ScreenToLocal( mx, my );
		if ( mx < 0 || my < 0 )
			return true;
		if ( mx > GetWide() || my > GetTall() )
			return true;

		return false;
	}

	int deltax = abs( mx - startx );
	int deltay = abs( my - starty );
	if ( deltax > m_pDragDrop->m_nDragStartTolerance ||
		 deltay > m_pDragDrop->m_nDragStartTolerance )
	{
		return true;
	}
#endif
	return false;
}

HCursor Panel::GetDropCursor( CUtlVector< KeyValues * >& msglist )
{
	return dc_arrow;
}

bool IsSelfDroppable( CUtlVector< KeyValues * > &dragData )
{
	if ( dragData.Count() == 0 )
		return false;

	KeyValues *pKeyValues( dragData[ 0 ] );
	if ( !pKeyValues )
		return false;

	return pKeyValues->GetInt( "selfDroppable" ) != 0;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  :  - 
//-----------------------------------------------------------------------------
void Panel::OnContinueDragging()
{
#if defined( VGUI_USEDRAGDROP )
	if ( !m_pDragDrop->m_bDragEnabled )
		return;

	if ( !m_pDragDrop->m_bDragging )
		return;

	int x, y;
	input()->GetCursorPos( x, y );

	// Update last position
	m_pDragDrop->m_nLastPos[ 0 ] = x;
	m_pDragDrop->m_nLastPos[ 1 ] = y;

	if ( !m_pDragDrop->m_bDragStarted )
	{
		if ( CanStartDragging( m_pDragDrop->m_nStartPos[ 0 ], m_pDragDrop->m_nStartPos[ 1 ], x, y ) )
		{
			m_pDragDrop->m_bDragStarted = true;
			CreateDragData();
		}
		else
		{
			return;
		}
	}

	if ( !s_DragDropHelper.Get() )
	{
		s_DragDropHelper = new CDragDropHelperPanel();
		s_DragDropHelper->SetKeyBoardInputEnabled( false );
		s_DragDropHelper->SetMouseInputEnabled( false );
	}

	Assert( s_DragDropHelper.Get() );
	if ( !s_DragDropHelper.Get() )
		return;

	s_DragDropHelper->AddPanel( this );

	Assert( m_pDragDrop->m_DragData.Count() );

	vgui::PHandle oldDrop = m_pDragDrop->m_hCurrentDrop;

	// See what's under that
	m_pDragDrop->m_hCurrentDrop = NULL;

	// Search under mouse pos...
	Panel *dropTarget = FindDropTargetPanel();
	if ( dropTarget )
	{
		dropTarget = dropTarget->GetDropTarget( m_pDragDrop->m_DragData );
	}

	// it's not okay until we find a droppable panel
	surface()->SetCursor( dc_no );

	if ( dropTarget )
	{
		if ( dropTarget != this || IsSelfDroppable( m_pDragDrop->m_DragData ) )
		{
			m_pDragDrop->m_hCurrentDrop = dropTarget;
			surface()->SetCursor( dropTarget->GetDropCursor( m_pDragDrop->m_DragData ) );
		}
	}

	if ( m_pDragDrop->m_hCurrentDrop.Get() != oldDrop.Get() )
	{
		if ( oldDrop.Get() )
		{
			oldDrop->OnPanelExitedDroppablePanel( m_pDragDrop->m_DragData );
		}

		if ( m_pDragDrop->m_hCurrentDrop.Get() )
		{
			m_pDragDrop->m_hCurrentDrop->OnPanelEnteredDroppablePanel( m_pDragDrop->m_DragData );
			m_pDragDrop->m_hCurrentDrop->OnDropContextHoverHide( m_pDragDrop->m_DragData );

			// Reset hover time
			m_pDragDrop->m_lDropHoverTime = system()->GetTimeMillis();
			m_pDragDrop->m_bDropMenuShown = false;
		}

		// Discard any stale context menu...
		if ( m_pDragDrop->m_hDropContextMenu.Get() )
		{
			delete m_pDragDrop->m_hDropContextMenu.Get();
		}
	}

	if ( m_pDragDrop->m_hCurrentDrop != NULL &&
		m_pDragDrop->m_hDropContextMenu.Get() )
	{
		Menu *menu = m_pDragDrop->m_hDropContextMenu;

		VPANEL hover = menu->IsWithinTraverse( x, y, false );
		if ( hover )
		{
			Panel *pHover = ipanel()->GetPanel( hover, GetModuleName() );
			if ( pHover )
			{
				// Figure out if it's a menu item...
				int c = menu->GetItemCount();
				for ( int i = 0; i < c; ++i )
				{
					int id = menu->GetMenuID( i );
					MenuItem *item = menu->GetMenuItem( id );
					if ( item == pHover )
					{
						menu->SetCurrentlyHighlightedItem( id );
					}
				}
			}
		}
		else
		{
			menu->ClearCurrentlyHighlightedItem();
		}
	}
#endif
}

#if defined( VGUI_USEDRAGDROP )
//-----------------------------------------------------------------------------
// Purpose: 
// Input  :  - 
// Output : DragDrop_t
//-----------------------------------------------------------------------------
DragDrop_t *Panel::GetDragDropInfo()
{
	Assert( m_pDragDrop );
	return m_pDragDrop;
}
#endif

void Panel::OnGetAdditionalDragPanels( CUtlVector< Panel * >& dragabbles )
{
	// Nothing here
}

//-----------------------------------------------------------------------------
// Purpose: Virtual method to allow panels to add to the default values
// Input  : *msg - 
//-----------------------------------------------------------------------------
void Panel::OnCreateDragData( KeyValues *msg )
{
	// These values are filled in for you:
	// "panel"	ptr to panel being dropped
	// "screenx", "screeny" - drop cursor pos in screen space
	// "x", "y" - drop coordinates relative to this window (the window being dropped upon)
}

// Called if m_flHoverContextTime was non-zero, allows droppee to preview the drop data and show an appropriate menu
bool Panel::GetDropContextMenu( Menu *menu, CUtlVector< KeyValues * >& msglist )
{
	return false;
}

void Panel::CreateDragData()
{
#if defined( VGUI_USEDRAGDROP )
	int i, c;

	if ( m_pDragDrop->m_DragData.Count() )
	{
		return;
	}

	PHandle h;
	h = this;
	m_pDragDrop->m_DragPanels.AddToTail( h );

	CUtlVector< Panel * > temp;
	OnGetAdditionalDragPanels( temp );
	c = temp.Count();
	for ( i = 0; i < c; ++i )
	{
		h = temp[ i ];
		m_pDragDrop->m_DragPanels.AddToTail( h );
	}

	c = m_pDragDrop->m_DragPanels.Count();
	for ( i = 0 ; i < c; ++i )
	{
		Panel *sibling = m_pDragDrop->m_DragPanels[ i ].Get();
		if ( !sibling )
		{
			continue;
		}

		KeyValues *msg = new KeyValues( "DragDrop" );
		msg->SetPtr( "panel", sibling );

		sibling->OnCreateDragData( msg );

		m_pDragDrop->m_DragData.AddToTail( msg );
	}
#endif
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  :  - 
// Output : KeyValues
//-----------------------------------------------------------------------------
void Panel::GetDragData( CUtlVector< KeyValues * >& list )
{
#if defined( VGUI_USEDRAGDROP )
	int i, c;

	list.RemoveAll();

	c = m_pDragDrop->m_DragData.Count();
	for ( i = 0 ; i < c; ++i )
	{
		list.AddToTail( m_pDragDrop->m_DragData[ i ] );
	}
#endif
}

#if defined( VGUI_USEDRAGDROP )
CDragDropHelperPanel::CDragDropHelperPanel() : BaseClass( NULL, "DragDropHelper" )
{
	SetVisible( true );
	SetPaintEnabled( false );
	SetPaintBackgroundEnabled( false );
	SetMouseInputEnabled( false );
	SetKeyBoardInputEnabled( false );
	// SetCursor( dc_none );
	ipanel()->SetTopmostPopup( GetVPanel(), true );
	int w, h;
	surface()->GetScreenSize( w, h );
	SetBounds( 0, 0, w, h );

	SetPostChildPaintEnabled( true );

	MakePopup( false );
}

VPANEL CDragDropHelperPanel::IsWithinTraverse(int x, int y, bool traversePopups)
{
	return (VPANEL)0;
}

void CDragDropHelperPanel::PostChildPaint()
{
	int c = m_PaintList.Count();
	for ( int i = c - 1; i >= 0 ; --i )
	{
		DragHelperPanel_t& data = m_PaintList[ i ];

		Panel *panel = data.m_hPanel.Get();
		if ( !panel )
		{
			m_PaintList.Remove( i );
			continue;
		}

		Panel *dropPanel = panel->GetDragDropInfo()->m_hCurrentDrop.Get();
		if ( panel )
		{
			if ( !dropPanel )
			{
				panel->OnDraggablePanelPaint();
			}
			else
			{
				CUtlVector< Panel * > temp;
				CUtlVector< PHandle >& data = panel->GetDragDropInfo()->m_DragPanels;
				CUtlVector< KeyValues * >& msglist = panel->GetDragDropInfo()->m_DragData;
				int i, c;
				c = data.Count();
				for ( i = 0; i < c ; ++i )
				{
					Panel *pPanel = data[ i ].Get();
					if ( pPanel )
					{
						temp.AddToTail( pPanel );
					}
				}

				dropPanel->OnDroppablePanelPaint( msglist, temp );
			}
		}
	}

	if ( c == 0 )
	{
		MarkForDeletion();
	}
}

void CDragDropHelperPanel::AddPanel( Panel *current )
{
	if ( !current )
		return;

	Menu *hover = current->GetDragDropInfo()->m_hDropContextMenu.Get();

	surface()->MovePopupToFront( GetVPanel() );
	if ( hover && hover->IsPopup() )
	{
		surface()->MovePopupToFront( hover->GetVPanel() );
	}

	int c = m_PaintList.Count();
	for ( int i = 0; i < c; ++i )
	{
		if ( m_PaintList[ i ].m_hPanel.Get() == current )
			return;
	}

	DragHelperPanel_t data;
	data.m_hPanel				= current;
	m_PaintList.AddToTail( data );
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *search - 
//-----------------------------------------------------------------------------
void CDragDropHelperPanel::RemovePanel( Panel *search )
{
	int c = m_PaintList.Count();
	for ( int i = c - 1 ; i >= 0; --i )
	{
		if ( m_PaintList[ i ].m_hPanel.Get() == search )
		{
			m_PaintList.Remove( i );
			return;
		}
	}
}
#endif
//-----------------------------------------------------------------------------
// Purpose: Enumerates panels under mouse x,y
// Input  : panelList - 
//			x - 
//			y - 
//			check - 
//-----------------------------------------------------------------------------
void Panel::FindDropTargetPanel_R( CUtlVector< VPANEL >& panelList, int x, int y, VPANEL check )
{
#if defined( VGUI_USEDRAGDROP )
	if ( !ipanel()->IsFullyVisible( check ) )
		return;

	if ( ::ShouldHandleInputMessage( check ) && ipanel()->IsWithinTraverse( check, x, y, false ) )
	{
		panelList.AddToTail( check );
	}

	int childcount = ipanel()->GetChildCount( check );
	for ( int i = 0; i < childcount; i++ )
	{
		VPANEL child = ipanel()->GetChild( check, i );
		FindDropTargetPanel_R( panelList, x, y, child );
	}
#endif
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  :  - 
// Output : Panel
//-----------------------------------------------------------------------------
Panel *Panel::FindDropTargetPanel()
{
#if defined( VGUI_USEDRAGDROP )
	if ( !s_DragDropHelper.Get() )
		return NULL;

	CUtlVector< VPANEL > hits;

	int x, y;
	input()->GetCursorPos( x, y );

	VPANEL embedded = surface()->GetEmbeddedPanel();
	VPANEL helper = s_DragDropHelper.Get()->GetVPanel();

	if ( surface()->IsCursorVisible() && surface()->IsWithin(x, y) )
	{
		// faster version of code below
		// checks through each popup in order, top to bottom windows
		int c = surface()->GetPopupCount();
		for (int i = c - 1; i >= 0 && hits.Count() == 0; i--)
		{
			VPANEL popup = surface()->GetPopup(i);
			if ( !popup )
				continue;

			if ( popup == embedded )
				continue;

			// Don't return helper panel!!!
			if ( popup == helper )
				continue;

			if ( !ipanel()->IsFullyVisible( popup ) )
				continue;

			FindDropTargetPanel_R( hits, x, y, popup );
		}

		// Check embedded
		if ( !hits.Count() )
		{
			FindDropTargetPanel_R( hits, x, y, embedded );
		}
	}

	// Nothing under mouse...
	if ( !hits.Count() )
		return NULL;

	// Return topmost panel under mouse, if it's visible to this .dll
	Panel *panel = NULL;
	int nCount = hits.Count();
	while ( --nCount >= 0 )
	{
		panel = ipanel()->GetPanel( hits[ nCount ], GetModuleName() );
		if ( panel )
			return panel;
	}
#endif
	return NULL;
}

//-----------------------------------------------------------------------------
// Purpose: Mouse is on draggable panel and has started moving, but is not over a droppable panel yet
// Input  :  - 
//-----------------------------------------------------------------------------
void Panel::OnDraggablePanelPaint()
{
#if defined( VGUI_USEDRAGDROP )
	int sw, sh;
	GetSize( sw, sh );

	int x, y;
	input()->GetCursorPos( x, y );
	int w, h;

	w = min( sw, 80 );
	h = min( sh, 80 );
	x -= ( w >> 1 );
	y -= ( h >> 1 );

	surface()->DrawSetColor( m_clrDragFrame );
	surface()->DrawOutlinedRect( x, y, x + w, y + h );

	if ( m_pDragDrop->m_DragPanels.Count() > 1 )
	{
		surface()->DrawSetTextColor( m_clrDragFrame );
		surface()->DrawSetTextFont( m_infoFont );
		surface()->DrawSetTextPos( x + 5, y + 2 );

		wchar_t sz[ 64 ];
		_snwprintf( sz, 64, L"[ %i ]", m_pDragDrop->m_DragPanels.Count() );

		surface()->DrawPrintText( sz, wcslen( sz ) );
	}
#endif
}

//-----------------------------------------------------------------------------
// Purpose: Mouse is now over a droppable panel
// Input  : *dragPanel - 
//-----------------------------------------------------------------------------
void Panel::OnDroppablePanelPaint( CUtlVector< KeyValues * >& msglist, CUtlVector< Panel * >& dragPanels )
{
#if defined( VGUI_USEDRAGDROP )
	if ( !dragPanels.Count() )
		return;

	// Convert this panel's bounds to screen space
	int w, h;
	GetSize( w, h );

	int x, y;
	x = y = 0;
	LocalToScreen( x, y );

	surface()->DrawSetColor( m_clrDropFrame );
	// Draw 2 pixel frame
	surface()->DrawOutlinedRect( x, y, x + w, y + h );
	surface()->DrawOutlinedRect( x+1, y+1, x + w-1, y + h-1 );
#endif
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  :  - 
// Output : Color
//-----------------------------------------------------------------------------
Color Panel::GetDropFrameColor()
{
#if defined( VGUI_USEDRAGDROP )
	return m_clrDropFrame;
#endif
	return Color(0, 0, 0, 0);
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  :  - 
// Output : Color
//-----------------------------------------------------------------------------
Color Panel::GetDragFrameColor()
{
#if defined( VGUI_USEDRAGDROP )
	return m_clrDragFrame;
#endif
	return Color(0, 0, 0, 0);
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *data - 
//-----------------------------------------------------------------------------
void Panel::OnPanelDropped( CUtlVector< KeyValues * >& data )
{
	// Empty.  Derived classes would implement handlers here
}

//-----------------------------------------------------------------------------
// called on droptarget when draggable panel enters droptarget
//-----------------------------------------------------------------------------
void Panel::OnPanelEnteredDroppablePanel( CUtlVector< KeyValues * >& msglist )
{
	// Empty.  Derived classes would implement handlers here
}

//-----------------------------------------------------------------------------
// called on droptarget when draggable panel exits droptarget
//-----------------------------------------------------------------------------
void Panel::OnPanelExitedDroppablePanel ( CUtlVector< KeyValues * >& msglist )
{
	// Empty.  Derived classes would implement handlers here
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  :  - 
//-----------------------------------------------------------------------------
void Panel::DragDropStartDragging()
{
#if defined( VGUI_USEDRAGDROP )
	// We somehow missed a mouse release, cancel the previous drag
	if ( g_DragDropCapture.Get() )
	{
		if ( HasParent( g_DragDropCapture.Get()->GetVPanel() ) )
			return;

		bool started = g_DragDropCapture->GetDragDropInfo()->m_bDragStarted;
		g_DragDropCapture->OnFinishDragging( true, (MouseCode)-1 );
		if ( started )
		{
			return;
		}
	}

	// Find actual target panel
	Panel *panel = GetDragPanel();
	if ( !panel )
		return;

	DragDrop_t *data = panel->GetDragDropInfo();
	if ( !data )
		return;

	if ( !panel->IsDragEnabled() )
		return;

	if ( data->m_bDragging )
		return;

	panel->OnStartDragging();
#endif
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  :  - 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool Panel::IsBeingDragged()
{
#if defined( VGUI_USEDRAGDROP )
	if ( !g_DragDropCapture.Get() )
		return false;

	if ( g_DragDropCapture.Get() == this )
		return true;

	// If we encounter a blocker, stop chaining
	if ( m_pDragDrop->m_bPreventChaining )
		return false;

	// Chain up
	if ( GetParent() )
	{
		return GetParent()->IsBeingDragged();
	}
#endif
	// No luck
	return false;
}

struct srect_t
{
	int x0, y0;
	int x1, y1;

	bool IsDegenerate()
	{
		if ( x1 - x0 <= 0 )
			return true;
		if ( y1 - y0 <= 0 )
			return true;
		return false;
	}
};

// Draws a filled rect of specified bounds, but omits the bounds of the skip panel from those bounds
void Panel::FillRectSkippingPanel( Color& clr, int x, int y, int w, int h, Panel *skipPanel )
{
	int sx = 0, sy = 0, sw, sh;
	skipPanel->GetSize( sw, sh );
	skipPanel->LocalToScreen( sx, sy );
	ScreenToLocal( sx, sy );

	surface()->DrawSetColor( clr );

	srect_t r1;
	r1.x0 = x;
	r1.y0 = y;
	r1.x1 = x + w;
	r1.y1 = y + h;

	srect_t r2;
	r2.x0 = sx;
	r2.y0 = sy;
	r2.x1 = sx + sw;
	r2.y1 = sy + sh;

	int topy = r1.y0;
	int bottomy = r1.y1;

	// We'll descend vertically and draw:
	// 1 a possible bar across the top
	// 2 a possible bar across the bottom
	// 3 possible left bar
	// 4 possible right bar

	// Room at top?
	if ( r2.y0 > r1.y0 )
	{
		topy = r2.y0;

		surface()->DrawFilledRect( r1.x0, r1.y0, r1.x1, topy );
	}

	// Room at bottom?
	if ( r2.y1 < r1.y1 )
	{
		bottomy = r2.y1;

		surface()->DrawFilledRect( r1.x0, bottomy, r1.x1, r1.y1 );
	}

	// Room on left side?
	if ( r2.x0 > r1.x0 )
	{
		int left = r2.x0;

		surface()->DrawFilledRect( r1.x0, topy, left, bottomy );
	}

	// Room on right side
	if ( r2.x1 < r1.x1 )
	{
		int right = r2.x1;

		surface()->DrawFilledRect( right, topy, r1.x1, bottomy );
	}
}


//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *child - 
//-----------------------------------------------------------------------------
void Panel::SetSkipChildDuringPainting( Panel *child )
{
	m_SkipChild = child;
}

//-----------------------------------------------------------------------------
// Purpose: Utility class for handling message map allocation
//-----------------------------------------------------------------------------
class CPanelMessageMapDictionary
{
public:
	CPanelMessageMapDictionary() : m_PanelMessageMapPool( sizeof(PanelMessageMap), 32, CMemoryPool::GROW_FAST, "CPanelMessageMapDictionary::m_PanelMessageMapPool" )
	{
		m_MessageMaps.RemoveAll();
	}

	PanelMessageMap	*FindOrAddPanelMessageMap( char const *className );
	PanelMessageMap	*FindPanelMessageMap( char const *className );
private:

	struct PanelMessageMapDictionaryEntry
	{
		PanelMessageMap *map;
	};

	char const *StripNamespace( char const *className );
	
	CUtlDict< PanelMessageMapDictionaryEntry, int > m_MessageMaps;
	CMemoryPool m_PanelMessageMapPool;
};


char const *CPanelMessageMapDictionary::StripNamespace( char const *className )
{
	if ( !strnicmp( className, "vgui::", 6 ) )
	{
		return className + 6;
	}
	return className;
}

//-----------------------------------------------------------------------------
// Purpose: Find but don't add mapping
//-----------------------------------------------------------------------------
PanelMessageMap *CPanelMessageMapDictionary::FindPanelMessageMap( char const *className )
{
	int lookup = m_MessageMaps.Find( StripNamespace( className ) );
	if ( lookup != m_MessageMaps.InvalidIndex() )
	{
		return m_MessageMaps[ lookup ].map;
	}
	return NULL;
}

#include <tier0/memdbgoff.h>
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
PanelMessageMap *CPanelMessageMapDictionary::FindOrAddPanelMessageMap( char const *className )
{
	PanelMessageMap *map = FindPanelMessageMap( className );
	if ( map )
		return map;

	PanelMessageMapDictionaryEntry entry;
	// use the alloc in place method of new
	entry.map = new (m_PanelMessageMapPool.Alloc(sizeof(PanelMessageMap))) PanelMessageMap;
	Construct(entry.map);
	m_MessageMaps.Insert( StripNamespace( className ), entry );
	return entry.map;
}
#include <tier0/memdbgon.h>

#if defined( VGUI_USEKEYBINDINGMAPS )
//-----------------------------------------------------------------------------
// Purpose: Utility class for handling keybinding map allocation
//-----------------------------------------------------------------------------
class CPanelKeyBindingMapDictionary
{
public:
	CPanelKeyBindingMapDictionary() : m_PanelKeyBindingMapPool( sizeof(PanelKeyBindingMap), 32, CMemoryPool::GROW_FAST, "CPanelKeyBindingMapDictionary::m_PanelKeyBindingMapPool" )
	{
		m_MessageMaps.RemoveAll();
	}

	PanelKeyBindingMap	*FindOrAddPanelKeyBindingMap( char const *className );
	PanelKeyBindingMap	*FindPanelKeyBindingMap( char const *className );
private:

	struct PanelKeyBindingMapDictionaryEntry
	{
		PanelKeyBindingMap *map;
	};

	char const *StripNamespace( char const *className );
	
	CUtlDict< PanelKeyBindingMapDictionaryEntry, int > m_MessageMaps;
	CMemoryPool m_PanelKeyBindingMapPool;
};


char const *CPanelKeyBindingMapDictionary::StripNamespace( char const *className )
{
	if ( !strnicmp( className, "vgui::", 6 ) )
	{
		return className + 6;
	}
	return className;
}

//-----------------------------------------------------------------------------
// Purpose: Find but don't add mapping
//-----------------------------------------------------------------------------
PanelKeyBindingMap *CPanelKeyBindingMapDictionary::FindPanelKeyBindingMap( char const *className )
{
	int lookup = m_MessageMaps.Find( StripNamespace( className ) );
	if ( lookup != m_MessageMaps.InvalidIndex() )
	{
		return m_MessageMaps[ lookup ].map;
	}
	return NULL;
}

#include <tier0/memdbgoff.h>
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
PanelKeyBindingMap *CPanelKeyBindingMapDictionary::FindOrAddPanelKeyBindingMap( char const *className )
{
	PanelKeyBindingMap *map = FindPanelKeyBindingMap( className );
	if ( map )
		return map;

	PanelKeyBindingMapDictionaryEntry entry;
	// use the alloc in place method of new
	entry.map = new (m_PanelKeyBindingMapPool.Alloc(sizeof(PanelKeyBindingMap))) PanelKeyBindingMap;
	Construct(entry.map);
	m_MessageMaps.Insert( StripNamespace( className ), entry );
	return entry.map;
}

#include <tier0/memdbgon.h>

CPanelKeyBindingMapDictionary& GetPanelKeyBindingMapDictionary()
{
	static CPanelKeyBindingMapDictionary dictionary;
	return dictionary;
}

#endif // VGUI_USEKEYBINDINGMAPS

CPanelMessageMapDictionary& GetPanelMessageMapDictionary()
{
	static CPanelMessageMapDictionary dictionary;
	return dictionary;
}

namespace vgui
{

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
PanelMessageMap *FindOrAddPanelMessageMap( char const *className )
{
	return GetPanelMessageMapDictionary().FindOrAddPanelMessageMap( className );
}

//-----------------------------------------------------------------------------
// Purpose: Find but don't add mapping
//-----------------------------------------------------------------------------
PanelMessageMap *FindPanelMessageMap( char const *className )
{
	return GetPanelMessageMapDictionary().FindPanelMessageMap( className );
}

#if defined( VGUI_USEKEYBINDINGMAPS )
CPanelKeyBindingMapDictionary& GetPanelKeyBindingMapDictionary()
{
	static CPanelKeyBindingMapDictionary dictionary;
	return dictionary;
}
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
PanelKeyBindingMap *FindOrAddPanelKeyBindingMap( char const *className )
{
	return GetPanelKeyBindingMapDictionary().FindOrAddPanelKeyBindingMap( className );
}

//-----------------------------------------------------------------------------
// Purpose: Find but don't add mapping
//-----------------------------------------------------------------------------
PanelKeyBindingMap *FindPanelKeyBindingMap( char const *className )
{
	return GetPanelKeyBindingMapDictionary().FindPanelKeyBindingMap( className );
}
#endif // VGUI_USEKEYBINDINGMAPS

}
