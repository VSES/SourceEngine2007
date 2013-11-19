//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
// This class is a message box that has two buttons, ok and cancel instead of
// just the ok button of a message box. We use a message box class for the ok button
// and implement another button here.
//
// $NoKeywords: $
//=============================================================================//

#include "OfflineMode.h"
#include "vgui/Cursor.h"
#include "vgui/IScheme.h"
#include "vgui/ISystem.h"
#include "vgui/ISurface.h"
#include "vgui/IVGUI.h"
#include "vgui/IBorder.h"
#include "filesystem.h"

#include "vgui_controls/HTML.h"
#include "vgui_controls/Controls.h"
#include "vgui_controls/Label.h"
#include "vgui_controls/Image.h"
#include "vgui_controls/ScrollBar.h"
#include "KeyValues.h"

#include <stdio.h>

#if defined( _X360 )
#include "xbox/xbox_win32stubs.h"
#endif

#include "tier0/memdbgon.h"

using namespace vgui;

enum 
{
	WINDOW_BORDER_WIDTH=1
};

vgui::Panel *HTML_NoJavascript_Factory()
{
	return new HTML( NULL, NULL, false );
}

vgui::Panel *HTML_Javascript_Factory()
{
	return new HTML( NULL, NULL, true );
}

//DECLARE_BUILD_FACTORY_CUSTOM_ALIAS( HTML, HTML_NoJavascript, HTML_NoJavascript_Factory );
//DECLARE_BUILD_FACTORY_CUSTOM_ALIAS( HTML, HTML_Javascript, HTML_Javascript_Factory );

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
HTML::HTML(Panel *parent, const char *name, bool allowJavaScript) : Panel(parent, name)
{	
	browser = surface()->CreateHTMLWindow(this, GetVPanel());
	Assert(browser != NULL);
	m_iNextFrameTime=0;
	m_iAnimTime=0;
	loading=NULL;
	picture=NULL;
	m_iScrollBorderX=m_iScrollBorderY=0;
	m_iScrollX=m_iScrollY=0;
	m_bScrollBarEnabled = true;
	m_bContextMenuEnabled = true;
	m_bNewWindowsOnly = false;
	m_bSetVisibleOnPerformLayout = false;
	if ( surface()->SupportsFeature( ISurface::DIRECT_HWND_RENDER ) )
	{
		SetCursor( dc_blank );
	}

	_hbar = new ScrollBar(this, "HorizScrollBar", false);
	_hbar->SetVisible(false);
	_hbar->AddActionSignalTarget(this);

	_vbar = new ScrollBar(this, "VertScrollBar", true);
	_vbar->SetVisible(false);
	_vbar->AddActionSignalTarget(this);

	m_bRegenerateHTMLBitmap = true;
	SetEnabled(true);
	SetVisible(true);
}

//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
HTML::~HTML()
{
	surface()->DeleteHTMLWindow(browser);
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void HTML::ApplySchemeSettings(IScheme *pScheme)
{
    BaseClass::ApplySchemeSettings(pScheme);
	SetBgColor(pScheme->GetColor("HTML.BgColor", GetBgColor()));
	SetBorder(pScheme->GetBorder( "BrowserBorder"));
	BrowserResize();
}

//-----------------------------------------------------------------------------
// Purpose: Causes the HTML window to repaint itself every 100ms, to allow animaited gifs and the like
//-----------------------------------------------------------------------------
void HTML::StartAnimate(int time)
{
	// a tick signal to let the web page re-render itself, in case of animated images
	//ivgui()->AddTickSignal(GetVPanel());
	m_iAnimTime=time;

}

//-----------------------------------------------------------------------------
// Purpose: stops the repainting
//-----------------------------------------------------------------------------
void HTML::StopAnimate()
{
	m_iNextFrameTime=0xffffffff; // next update is at infinity :)
	m_iAnimTime=0;

}

//-----------------------------------------------------------------------------
// Purpose: overrides panel class, paints a texture of the HTML window as a background
//-----------------------------------------------------------------------------
void HTML::PaintBackground()
{
	BaseClass::PaintBackground();

	if (m_bRegenerateHTMLBitmap)
	{
		if ( !surface()->SupportsFeature( ISurface::DIRECT_HWND_RENDER ) )
		{	
			surface()->PaintHTMLWindow(browser);
		}
		m_bRegenerateHTMLBitmap = false;
		int w, h;
		GetSize(w, h);
		CalcScrollBars(w, h);
	}

	if ( surface()->SupportsFeature( ISurface::DIRECT_HWND_RENDER ) )
	{
	}
	else
	{
		// the window is a textured background
		picture = browser->GetBitmap();	
		if (picture)
		{
			surface()->DrawSetColor(GetBgColor());

			picture->SetPos(0,0);
			picture->Paint();
		}

		// If we have scrollbars, we need to draw the bg color under them, since the browser
		// bitmap is a checkerboard under them, and they are transparent in the in-game client
		if ( m_iScrollBorderX > 0 || m_iScrollBorderY > 0 )
		{
			int w, h;
			GetSize( w, h );
			IBorder *border = GetBorder();
			int left = 0, top = 0, right = 0, bottom = 0;
			if ( border )
			{
				border->GetInset( left, top, right, bottom );
			}

			surface()->DrawSetColor( GetBgColor() );
			if ( m_iScrollBorderX )
			{
				surface()->DrawFilledRect( w-m_iScrollBorderX - right, top, w - right, h - bottom );
			}
			if ( m_iScrollBorderY )
			{
				surface()->DrawFilledRect( left, h-m_iScrollBorderY - bottom, w-m_iScrollBorderX - right, h - bottom );
			}
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: causes a repaint when the layout changes
//-----------------------------------------------------------------------------
void HTML::PerformLayout()
{
	BaseClass::PerformLayout();
	Repaint();
	if ( m_bSetVisibleOnPerformLayout )
	{
		browser->SetVisible( true );
		m_bSetVisibleOnPerformLayout= false;
	}
}

//-----------------------------------------------------------------------------
// Purpose: passthru to the HTML surface widget
//-----------------------------------------------------------------------------
void HTML::SetVisible( bool state )
{
	BaseClass::SetVisible( state );
	int w, h;
	GetSize(w, h);
	CalcScrollBars(w, h);
	BrowserResize();

	if ( !state ) // allow the visibleonlayout flag to be turned off
	{
		m_bSetVisibleOnPerformLayout = false;
	}

	if ( IsLayoutInvalid() && state ) // only set visible on performlayout IF you are setting it visible
	{
		m_bSetVisibleOnPerformLayout = true;
	}
	else
	{
		browser->SetVisible( state );
	}
}

//-----------------------------------------------------------------------------
// Purpose: updates the underlying HTML surface widgets position
//-----------------------------------------------------------------------------
void HTML::OnMove()
{
	BaseClass::OnMove();
	BrowserResize();
}

//-----------------------------------------------------------------------------
// Purpose: calculates the need for and position of both horizontal and vertical scroll bars
//-----------------------------------------------------------------------------
void HTML::CalcScrollBars(int w, int h)
{
	int img_w, img_h;
	if (m_bScrollBarEnabled )
	{
		browser->GetHTMLSize(img_w, img_h);

		if ( img_w < 0 || img_h < 0 )
		{
			m_iScrollBorderX = 0;
			m_iScrollBorderY=0;
			_vbar->SetVisible(false);
			_hbar->SetVisible(false);
			BrowserResize();
			return;
		}

		if (img_h > h)
		{
			if (!_vbar->IsVisible())
			{
				_vbar->SetVisible(true);
				// displayable area has changed, need to force an update
				PostMessage(this, new KeyValues("ScrollBarSliderMoved"), 0.02f);
			}

			_vbar->SetEnabled(false);
			_vbar->SetRangeWindow( h/2-5 );
			_vbar->SetRange( 0, img_h);	
			_vbar->SetButtonPressedScrollValue( 5 );

			_vbar->SetPos(w - (_vbar->GetWide()+WINDOW_BORDER_WIDTH), WINDOW_BORDER_WIDTH);
			if(img_w>w)
			{
				_vbar->SetSize(_vbar->GetWide(), h-_vbar->GetWide()-1-WINDOW_BORDER_WIDTH);
			}
			else
			{
				_vbar->SetSize(_vbar->GetWide(), h-1-WINDOW_BORDER_WIDTH);
			}
			m_iScrollBorderX=_vbar->GetWide()+WINDOW_BORDER_WIDTH;
		}
		else
		{
			m_iScrollBorderX=0;
			_vbar->SetVisible(false);
			BrowserResize();
		}

		if (img_w > w)
		{
			_hbar->SetVisible(true);
			_hbar->SetEnabled(false);
			_hbar->SetRangeWindow( w/2-5 );
			_hbar->SetRange( 0, img_w);	
			_hbar->SetButtonPressedScrollValue( 5 );

			_hbar->SetPos(WINDOW_BORDER_WIDTH,h-(_vbar->GetWide()+WINDOW_BORDER_WIDTH));
			if(img_h>h)
			{
				_hbar->SetSize(w-_vbar->GetWide()-WINDOW_BORDER_WIDTH,_vbar->GetWide());
			}
			else
			{
				_hbar->SetSize(w-WINDOW_BORDER_WIDTH,_vbar->GetWide());	
			}
			
			m_iScrollBorderY=_vbar->GetWide()+WINDOW_BORDER_WIDTH+1;

		}
		else
		{
			m_iScrollBorderY=0;
			_hbar->SetVisible(false);
			BrowserResize();
		}
	}
	else
	{
		_vbar->SetVisible(false);
		_hbar->SetVisible(false);
		BrowserResize();
	}
}

//-----------------------------------------------------------------------------
// Purpose: opens the URL, will accept any URL that IE accepts
//-----------------------------------------------------------------------------
void HTML::OpenURL(const char *URL, bool force)
{
	if ( IsSteamInOfflineMode() && !force )
	{
		const char *baseDir = getenv("HTML_OFFLINE_DIR");
		if ( baseDir )
		{
			// get the app we need to run
			char htmlLocation[_MAX_PATH];
			char otherName[128];
			char fileLocation[_MAX_PATH];

			if ( ! g_pFullFileSystem->FileExists( baseDir ) ) 
			{
				_snprintf( otherName, sizeof(otherName), "%senglish.html", OFFLINE_FILE );
				baseDir = otherName;
			}
			g_pFullFileSystem->GetLocalCopy( baseDir ); // put this file on disk for IE to load

			g_pFullFileSystem->GetLocalPath( baseDir, fileLocation, sizeof(fileLocation) );
			_snprintf(htmlLocation, sizeof(htmlLocation), "file://%s", fileLocation);
			browser->OpenURL( htmlLocation );
		}
		else
		{
			browser->OpenURL(URL);
		}
	}
	else
	{
		browser->OpenURL(URL);
	}
}

//-----------------------------------------------------------------------------
// Purpose: opens the URL, will accept any URL that IE accepts
//-----------------------------------------------------------------------------
bool HTML::StopLoading()
{
	return browser->StopLoading();
}

//-----------------------------------------------------------------------------
// Purpose: refreshes the current page
//-----------------------------------------------------------------------------
bool HTML::Refresh()
{
	return browser->Refresh();
}

//-----------------------------------------------------------------------------
// Purpose: empties the current HTML container of any HTML text (used in conjunction with AddText)
//-----------------------------------------------------------------------------
void HTML::Clear()
{
	browser->Clear();
}

//-----------------------------------------------------------------------------
// Purpose: appends "text" to the end of the current page. "text" should be a HTML formatted string
//-----------------------------------------------------------------------------
void HTML::AddText(const char *text)
{
	browser->AddText(text);
}

//-----------------------------------------------------------------------------
// Purpose: handle resizing
//-----------------------------------------------------------------------------
void HTML::OnSizeChanged(int wide,int tall)
{
	BaseClass::OnSizeChanged(wide,tall);
	CalcScrollBars(wide,tall);

	BrowserResize();
	m_bRegenerateHTMLBitmap = true;
	if ( !surface()->SupportsFeature( ISurface::DIRECT_HWND_RENDER ) )
	{
		Repaint();
	}
}


//-----------------------------------------------------------------------------
// Purpose: used for the animation calls above, to repaint the screen
//			periodically. ( NOT USED !!!)
//-----------------------------------------------------------------------------
void HTML::OnTick()
{
	if (IsVisible() && m_iAnimTime && system()->GetTimeMillis() >= m_iNextFrameTime)
	{
		m_iNextFrameTime = system()->GetTimeMillis() + 	m_iAnimTime;
		m_bRegenerateHTMLBitmap = true;
		if ( !surface()->SupportsFeature( ISurface::DIRECT_HWND_RENDER ) )
		{
			Repaint();
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: passes mouse clicks to the control
//-----------------------------------------------------------------------------
void HTML::OnMousePressed(MouseCode code)
{
	// ask for the focus to come to this window
	RequestFocus();

	// now tell the browser about the click
	// ignore right clicks if context menu has been disabled
	if (code != MOUSE_RIGHT || m_bContextMenuEnabled)
	{
		if (browser) 
		{
			browser->OnMouse(code,IHTML::DOWN,m_iMouseX,m_iMouseY);
		}
	}
	m_bRegenerateHTMLBitmap = true;
	if ( !surface()->SupportsFeature( ISurface::DIRECT_HWND_RENDER ) )
	{
		Repaint();
	}
}

//-----------------------------------------------------------------------------
// Purpose: passes mouse up events
//-----------------------------------------------------------------------------
void HTML::OnMouseReleased(MouseCode code)
{
	if (browser) 
	{
		browser->OnMouse(code, IHTML::UP, m_iMouseX, m_iMouseY);
	}
	m_bRegenerateHTMLBitmap = true;
	if ( !surface()->SupportsFeature( ISurface::DIRECT_HWND_RENDER ) )
	{
		Repaint();
	}
}

//-----------------------------------------------------------------------------
// Purpose: keeps track of where the cursor is
//-----------------------------------------------------------------------------
void HTML::OnCursorMoved(int x,int y)
{
	MouseCode code=MOUSE_LEFT;
	m_iMouseX=x;
	m_iMouseY=y;
	if(browser) browser->OnMouse(code,IHTML::MOVE,x,y);
}

//-----------------------------------------------------------------------------
// Purpose: passes double click events to the browser
//-----------------------------------------------------------------------------
void HTML::OnMouseDoublePressed(MouseCode code)
{
	if (browser)
	{
		browser->OnMouse(code, IHTML::DOWN, m_iMouseX, m_iMouseY);
	}
	m_bRegenerateHTMLBitmap = true;
	if ( !surface()->SupportsFeature( ISurface::DIRECT_HWND_RENDER ) )
	{
		Repaint();
	}
}

//-----------------------------------------------------------------------------
// Purpose: passes key presses to the browser (we don't current do this)
//-----------------------------------------------------------------------------
void HTML::OnKeyTyped(wchar_t unichar)
{
	// the OnKeyCodeDown member handles this
}

//-----------------------------------------------------------------------------
// Purpose: passes key presses to the browser 
//-----------------------------------------------------------------------------
void HTML::OnKeyCodePressed(KeyCode code)
{
	RequestFocus();
	if( code == KEY_PAGEDOWN || code == KEY_SPACE)
	{
		int val = _vbar->GetValue();
		val += 200;
		_vbar->SetValue(val);
	}
	else if ( code == KEY_PAGEUP )
	{
		int val = _vbar->GetValue();
		val -= 200;
		_vbar->SetValue(val);
		
	}

	if(browser) browser->OnKeyDown(code);
	m_bRegenerateHTMLBitmap = true;
	Repaint();
}

//-----------------------------------------------------------------------------
// Purpose: scrolls the vertical scroll bar on a web page
//-----------------------------------------------------------------------------
void HTML::OnMouseWheeled(int delta)
{	
	if (_vbar)
	{
		int val = _vbar->GetValue();
		val -= (delta * 25);
		_vbar->SetValue(val);
	}
}

//-----------------------------------------------------------------------------
// Purpose: Inserts a custom URL handler
//-----------------------------------------------------------------------------
void HTML::AddCustomURLHandler(const char *customProtocolName, vgui::Panel *target)
{
	int index = m_CustomURLHandlers.AddToTail();
	m_CustomURLHandlers[index].hPanel = target;
	strncpy(m_CustomURLHandlers[index].url, customProtocolName, sizeof(m_CustomURLHandlers[index].url));
}

//-----------------------------------------------------------------------------
// Purpose: gets called when a URL is first being loaded
// Return: return TRUE to continue loading, FALSE to stop this URL loading.
//-----------------------------------------------------------------------------
bool HTML::OnStartURL(const char *url, const char *target, bool first)
{
	if ( IsVisible() && !surface()->SupportsFeature( ISurface::DIRECT_HWND_RENDER ) )
	{
		SetCursor( dc_arrow );
	}

	// see if we have a custom handler for this
	bool bURLHandled = false;
	for (int i = 0; i < m_CustomURLHandlers.Count(); i++)
	{
		if (!strnicmp(m_CustomURLHandlers[i].url, url, strlen(m_CustomURLHandlers[i].url)))
		{
			// we have a custom handler
			Panel *target = m_CustomURLHandlers[i].hPanel;
			if (target)
			{
				PostMessage(target, new KeyValues("CustomURL", "url", url + strlen(m_CustomURLHandlers[i].url) + 3, "protocol", m_CustomURLHandlers[i].url));
			}

			bURLHandled = true;
		}
	}

	if (bURLHandled)
		return false;

	if ( m_bNewWindowsOnly )
	{
		if ( target && ( !stricmp( target, "_blank" ) || !stricmp( target, "_new" ) )  ) // only allow NEW windows (_blank ones)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: shared code for sizing the HTML surface window
//-----------------------------------------------------------------------------
void HTML::BrowserResize()
{
	int w,h;
	GetSize( w, h );
	IBorder *border;
	border = scheme()->GetIScheme(GetScheme())->GetBorder( "BrowserBorder");
	int left = 0, top = 0, right = 0, bottom = 0;
	if ( border )
	{
		border->GetInset( left, top, right, bottom );
	}
	// TODO: does the win32 surface still need this offset when direct rendering?
	//left += 1;
	//top += 1;
	//right += 1;
	//bottom += 1;

	if(browser) 
	{
		browser->OnSize(m_iScrollX + left, m_iScrollY + top, w-m_iScrollBorderX - right, h-m_iScrollBorderY - bottom);
	}
}

//-----------------------------------------------------------------------------
// Purpose: gets called when a URL is finished loading
//-----------------------------------------------------------------------------
void HTML::OnFinishURL(const char *url)
{
	// reset the scroll bar positions
	_vbar->SetValue(0);
	_hbar->SetValue(0);
	m_iScrollX = m_iScrollY = 0;
	int w, h;
	GetSize(w, h);
	CalcScrollBars(w, h);
	BrowserResize();

	m_bRegenerateHTMLBitmap = true; // repaint the window, as we have a new picture to show
	if ( !surface()->SupportsFeature( ISurface::DIRECT_HWND_RENDER ) )
	{
		Repaint();
	}
}

//-----------------------------------------------------------------------------
// Purpose: gets called while a URL is loading
//-----------------------------------------------------------------------------
void HTML::OnProgressURL(long current, long maximum)
{

}

//-----------------------------------------------------------------------------
// Purpose: gets called with status text from IE as the page loads
//-----------------------------------------------------------------------------
void HTML::OnSetStatusText(const char *text)
{

}

//-----------------------------------------------------------------------------
// Purpose: get called when IE wants us to redraw
//-----------------------------------------------------------------------------
void HTML::OnUpdate()
{
	// only let it redraw every m_iAnimTime milliseconds, so stop it sucking up all the CPU time
	if (m_iAnimTime && system()->GetTimeMillis() >= m_iNextFrameTime)
	{
		m_iNextFrameTime = system()->GetTimeMillis() + m_iAnimTime;
		m_bRegenerateHTMLBitmap = true;
		if ( !surface()->SupportsFeature( ISurface::DIRECT_HWND_RENDER ) )
		{
			Repaint();
		}
	}

	int w, h;
	GetSize(w, h);
	CalcScrollBars(w, h);
	BrowserResize();
}

//-----------------------------------------------------------------------------
// Purpose: get called when the cursor moved over a valid URL on the page
//-----------------------------------------------------------------------------
void HTML::OnLink()
{
	if( IsVisible() && !surface()->SupportsFeature( ISurface::DIRECT_HWND_RENDER ))
	{
		SetCursor(dc_hand);
	}
}

//-----------------------------------------------------------------------------
// Purpose: get called when the cursor leaves a valid URL
//-----------------------------------------------------------------------------
void HTML::OffLink()
{
	if( IsVisible() && !surface()->SupportsFeature( ISurface::DIRECT_HWND_RENDER ))
	{
		SetCursor(dc_arrow);
	}
}

//-----------------------------------------------------------------------------
// Purpose: when a slider moves causes the IE images to re-render itself
//-----------------------------------------------------------------------------
void HTML::OnSliderMoved()
{
	if(_hbar->IsVisible())
	{
		m_iScrollX=_hbar->GetValue();
	}
	else
	{
		m_iScrollX=0;
	}

	if(_vbar->IsVisible())
	{
		m_iScrollY=_vbar->GetValue();
	}
	else
	{
		m_iScrollY=0;
	}
	BrowserResize();

	
	m_bRegenerateHTMLBitmap = true;
	if ( !surface()->SupportsFeature( ISurface::DIRECT_HWND_RENDER ) )
	{
		Repaint();
	}
}

//-----------------------------------------------------------------------------
// Purpose: data accessor
//-----------------------------------------------------------------------------
void HTML::SetScrollbarsEnabled(bool state)
{
	m_bScrollBarEnabled = state;
}

//-----------------------------------------------------------------------------
// Purpose: data accessor
//-----------------------------------------------------------------------------
void HTML::SetContextMenuEnabled(bool state)
{
	m_bContextMenuEnabled = state;
}

//-----------------------------------------------------------------------------
// Purpose: data accessor
//-----------------------------------------------------------------------------
void HTML::NewWindowsOnly( bool state )
{
	m_bNewWindowsOnly = state;
}
