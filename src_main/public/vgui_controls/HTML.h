//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: Creates a HTML control
//
// $NoKeywords: $
//=============================================================================//

#ifndef HTML_H
#define HTML_H

#ifdef _WIN32
#pragma once
#endif

#include <vgui/VGUI.h>
#include <vgui/IHTML.h>
#include <vgui/IImage.h>
#include <vgui_controls/Panel.h>
#include <vgui_controls/PHandle.h>

namespace vgui
{

//-----------------------------------------------------------------------------
// Purpose: Control to display HTML content
//			This control utilises a hidden IE window to render a HTML page for you.
//			It can load any valid URL (i.e local files or web pages), you cannot dynamically change the
//			content however (internally to the control that is).
//-----------------------------------------------------------------------------
class HTML: public Panel, public IHTMLEvents
{
	DECLARE_CLASS_SIMPLE( HTML, Panel );

public:

	HTML(Panel *parent,const char *name, bool allowJavaScript = false);
	~HTML();

	// start and stop the HTML control repainting itself periodically
	void StartAnimate(int time);
	void StopAnimate();

	// IHTML pass through functions
	virtual void OpenURL(const char *URL, bool force = false); // force means ignore the offline mod override
	virtual bool StopLoading();
	virtual bool Refresh();
	virtual void Clear();
	virtual void AddText(const char *text);
	virtual void SetVisible( bool state );
	virtual void OnMove();

	// configuration
	virtual void SetScrollbarsEnabled(bool state);
	virtual void SetContextMenuEnabled(bool state);
	virtual void NewWindowsOnly( bool state );

	// events callbacks
	// Called when the control wishes to go to a new URL (either through the OpenURL() method or internally triggered).
	// Return TRUE to allow it to continue and FALSE to stop it loading the URL. Target is the frame this page will load in
	virtual bool OnStartURL(const char *url, const char *target, bool first);
	// Called when a page has finished loading an is being render. If you derive from this make SURE to
	// call this method also (i.e call Baseclass::OnFinishURL(url) in your derived version).
	virtual void OnFinishURL(const char *url);
	// Called as a page is loading. Maximum is the total number of "tick" events in the progress count, 
	// current is the current amount you are into the progress. This can be used to update another 
	// on screen control to show that the browser is active.
	virtual void OnProgressURL(long current, long maximum);
	// Called when IE has some status text it wishes to display. You can use this to provide status information
	// in another control.
	virtual void OnSetStatusText(const char *text);
	virtual void OnUpdate();
	virtual void OnLink();
	virtual void OffLink();

	// url handlers, lets you have web page links fire vgui events
	// use to have custom web page links, eg. "steam://open/subscriptionpage"
	// everything after the "://" is sent to the watcher in a message "CustomURL", "url", "protocol"
	virtual void AddCustomURLHandler(const char *customProtocolName, vgui::Panel *target);

	// overridden to paint our special web browser texture
	virtual void PaintBackground();
	
	// pass messages to the texture component to tell it about resizes
	virtual void OnSizeChanged(int wide,int tall);
	// for animation task
	virtual void OnTick();

	// pass mouse clicks through
	virtual void OnMousePressed(MouseCode code);
	virtual void OnMouseReleased(MouseCode code);
	virtual void OnCursorMoved(int x,int y);
	virtual void OnMouseDoublePressed(MouseCode code);
	virtual void OnKeyTyped(wchar_t unichar);
	virtual void OnKeyCodePressed(KeyCode code);
	virtual void PerformLayout();
	virtual void OnMouseWheeled(int delta);

protected:
	virtual void ApplySchemeSettings(IScheme *pScheme);

private:
	virtual void BrowserResize();
	virtual void CalcScrollBars(int w,int h);
	MESSAGE_FUNC( OnSliderMoved, "ScrollBarSliderMoved" );

	IHTML *browser; // the interface to the browser itself

	vgui::Label *loading;
	IImage *picture;
	vgui::ScrollBar *_hbar,*_vbar;

	int m_iMouseX,m_iMouseY; // where the mouse is on the control
	long m_iNextFrameTime; // next time (in milliseconds) to repaint
	int m_iAnimTime; // the time between repaints (in milliseconds)
	int m_iScrollX,m_iScrollY;
	int m_iScrollBorderX,m_iScrollBorderY;
	bool m_bRegenerateHTMLBitmap;

	bool m_bScrollBarEnabled;
	bool m_bContextMenuEnabled;
	int	m_iScrollbarSize;
	bool m_bNewWindowsOnly;
	bool m_bSetVisibleOnPerformLayout;

	struct CustomURLHandler_t
	{
		PHandle hPanel;
		char url[32];
	};
	CUtlVector<CustomURLHandler_t> m_CustomURLHandlers;
};

} // namespace vgui

#endif // HTML_H
