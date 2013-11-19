//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
// This class is a message box that has two buttons, ok and cancel instead of
// just the ok button of a message box. We use a message box class for the ok button
// and implement another button here.
//=============================================================================//

#include <math.h>
#define PROTECTED_THINGS_DISABLE

#include <vgui/IInput.h>
#include <vgui/ISystem.h>
#include <vgui/ISurface.h>
#include <vgui/IScheme.h>
#include <vgui/IVGui.h>
#include <vgui/IPanel.h>

#include <vgui_controls/Tooltip.h>
#include <vgui_controls/TextEntry.h>
#include <vgui_controls/Controls.h>

// memdbgon must be the last include file in a .cpp file!!!
#include <tier0/memdbgon.h>

using namespace vgui;

static vgui::DHANDLE< TextEntry > s_TooltipWindow;
static int s_iTooltipWindowCount = 0;

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
Tooltip::Tooltip(Panel *parent, const char *text) 
{
	if (!s_TooltipWindow.Get())
	{
		s_TooltipWindow = new TextEntry(NULL, "tooltip");
	}
	s_iTooltipWindowCount++;

	// this line prevents the main window from losing focus
	// when a tooltip pops up
	s_TooltipWindow->MakePopup(false, true);
	s_TooltipWindow->SetKeyBoardInputEnabled( false );
	s_TooltipWindow->SetMouseInputEnabled( false );
	
	SetText(text);
	s_TooltipWindow->SetText(m_Text.Base());
	s_TooltipWindow->SetEditable(false);
	s_TooltipWindow->SetMultiline(true);
	s_TooltipWindow->SetVisible(false);

	_displayOnOneLine = false;
	_makeVisible = false;
	_isDirty = false;

	_tooltipDelay = 500; // default delay for opening tooltips
	_delay = 0;
}


//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
Tooltip::~Tooltip()
{
	if (--s_iTooltipWindowCount < 1)
	{
		if ( s_TooltipWindow.Get() )
		{
			s_TooltipWindow->MarkForDeletion();
		}
		s_TooltipWindow = NULL;
	}
}

//-----------------------------------------------------------------------------
// Purpose: Get the tooltip text
//-----------------------------------------------------------------------------
const char *Tooltip::GetText()
{
	return m_Text.Base();
}

//-----------------------------------------------------------------------------
// Purpose: Set the tooltip text
//-----------------------------------------------------------------------------
void Tooltip::SetText(const char *text)
{
	_isDirty = true;

	if (!text)
	{
		text = "";
	}

	if (m_Text.Size() > 0)
	{
		m_Text.RemoveAll();
	}

	for (unsigned int i = 0; i < strlen(text); i++)
	{
		m_Text.AddToTail(text[i]);
	}
	m_Text.AddToTail('\0');
	
	if (s_TooltipWindow.Get())
	{
		s_TooltipWindow->SetText(m_Text.Base());
	}
}

//-----------------------------------------------------------------------------
// Purpose: gets the font from the scheme
//-----------------------------------------------------------------------------
void Tooltip::ApplySchemeSettings(IScheme *pScheme)
{
	if ( s_TooltipWindow.Get() )
	{
		s_TooltipWindow->SetFont(pScheme->GetFont("DefaultSmall", s_TooltipWindow->IsProportional()));
	}
}

//-----------------------------------------------------------------------------
// Purpose: Reset the tooltip delay timer
//-----------------------------------------------------------------------------
void Tooltip::ResetDelay()
{
	_isDirty = true;
	_delay = system()->GetTimeMillis() + _tooltipDelay;
}

//-----------------------------------------------------------------------------
// Purpose: Set the tooltip delay before a tooltip comes up.
//-----------------------------------------------------------------------------
void Tooltip::SetTooltipDelay( int tooltipDelay )
{
	_tooltipDelay = tooltipDelay;
}

//-----------------------------------------------------------------------------
// Purpose: Get the tooltip delay before a tooltip comes up.
//-----------------------------------------------------------------------------
int Tooltip::GetTooltipDelay()
{
	return _tooltipDelay;
}

//-----------------------------------------------------------------------------
// Purpose: Display the tooltip
//-----------------------------------------------------------------------------
void Tooltip::ShowTooltip(Panel *currentPanel)
{
	if ( s_TooltipWindow.Get() )
	{
		int nLen = s_TooltipWindow->GetTextLength();
		char *pBuf = (char*)_alloca( nLen+1 );
		s_TooltipWindow->GetText( pBuf, nLen+1 );
		Panel *pCurrentParent = s_TooltipWindow->GetParent();

		_isDirty = _isDirty || ( pCurrentParent != currentPanel );
		s_TooltipWindow->SetText( m_Text.Base() );
		s_TooltipWindow->SetParent(currentPanel);
	}
	_makeVisible = true;

	PerformLayout();
}

//-----------------------------------------------------------------------------
// Purpose: Display the tooltip
//-----------------------------------------------------------------------------
void Tooltip::PerformLayout()
{
	if (!_makeVisible)
		return;
	if (_delay > system()->GetTimeMillis())
		return;	

	// we're ready, just make us visible
	if ( !s_TooltipWindow.Get() )
		return;

	// We only need to layout when we first become visible
	if ( !_isDirty )
		return;

	_isDirty = false;

	s_TooltipWindow->SetVisible(true);
	s_TooltipWindow->MakePopup( false, true );
	s_TooltipWindow->SetKeyBoardInputEnabled( false );
	s_TooltipWindow->SetMouseInputEnabled( false );

	IScheme *pScheme = scheme()->GetIScheme( s_TooltipWindow->GetScheme() );

	s_TooltipWindow->SetBgColor(s_TooltipWindow->GetSchemeColor("Tooltip.BgColor", s_TooltipWindow->GetBgColor(), pScheme));
	s_TooltipWindow->SetFgColor(s_TooltipWindow->GetSchemeColor("Tooltip.TextColor", s_TooltipWindow->GetFgColor(), pScheme));
	s_TooltipWindow->SetBorder(pScheme->GetBorder("ToolTipBorder"));

	// get cursor position
	int cursorX, cursorY;
	input()->GetCursorPos(cursorX, cursorY);
	
	// relayout the textwindow immediately so that we know it's size
	//m_pTextEntry->InvalidateLayout(true);
	SizeTextWindow();
	int menuWide, menuTall;
	s_TooltipWindow->GetSize(menuWide, menuTall);
	
	// work out where the cursor is and therefore the best place to put the menu
	int wide, tall;
	surface()->GetScreenSize(wide, tall);
	
	if (wide - menuWide > cursorX)
	{
		cursorY += 20;
		// menu hanging right
		if (tall - menuTall > cursorY)
		{
			// menu hanging down
			s_TooltipWindow->SetPos(cursorX, cursorY);
		}
		else
		{
			// menu hanging up
			s_TooltipWindow->SetPos(cursorX, cursorY - menuTall - 20);
		}
	}
	else
	{
		// menu hanging left
		if (tall - menuTall > cursorY)
		{
			// menu hanging down
			s_TooltipWindow->SetPos(cursorX - menuWide, cursorY);
		}
		else
		{
			// menu hanging up
			s_TooltipWindow->SetPos(cursorX - menuWide, cursorY - menuTall - 20 );
		}
	}	
}

//-----------------------------------------------------------------------------
// Purpose: Size the text window so all the text fits inside it.
//-----------------------------------------------------------------------------
void Tooltip::SizeTextWindow()
{
	if ( !s_TooltipWindow.Get() )
		return;

	if (_displayOnOneLine)
	{
		// We want the tool tip to be one line
		s_TooltipWindow->SetMultiline(false);
		s_TooltipWindow->SetToFullWidth();
	}
	else
	{
		// We want the tool tip to be one line
		s_TooltipWindow->SetMultiline(false);
		s_TooltipWindow->SetToFullWidth();
		int wide, tall;
		s_TooltipWindow->GetSize( wide, tall );
		double newWide = sqrt( (2.0/1) * wide * tall );
		double newTall = (1/2) * newWide;
		s_TooltipWindow->SetMultiline(true);
		s_TooltipWindow->SetSize((int)newWide, (int)newTall );
		s_TooltipWindow->SetToFullHeight();
		
		s_TooltipWindow->GetSize( wide, tall );
		
		if (( wide < 100 ) && ( s_TooltipWindow->GetNumLines() == 2) ) 
		{
			s_TooltipWindow->SetMultiline(false);
			s_TooltipWindow->SetToFullWidth();	
		}
		else
		{
			
			while ( (float)wide/(float)tall < 2 )
			{
				s_TooltipWindow->SetSize( wide + 1, tall );
				s_TooltipWindow->SetToFullHeight();
				s_TooltipWindow->GetSize( wide, tall );
			}
		}
		s_TooltipWindow->GetSize( wide, tall );
	//	ivgui()->DPrintf("End Ratio: %f\n", (float)wide/(float)tall);		
	}
}

//-----------------------------------------------------------------------------
// Purpose: Set the tool tip to display on one line only
//			Default is multiple lines.
//-----------------------------------------------------------------------------
void Tooltip::SetTooltipFormatToSingleLine()
{
	_displayOnOneLine = true;
	_isDirty = true;
}

//-----------------------------------------------------------------------------
// Purpose: Set the tool tip to display on multiple lines.
//-----------------------------------------------------------------------------
void Tooltip::SetTooltipFormatToMultiLine()
{
	_displayOnOneLine = false;
	_isDirty = true;
}

//-----------------------------------------------------------------------------
// Purpose: Display the tooltip
//-----------------------------------------------------------------------------
void Tooltip::HideTooltip()
{
	if ( s_TooltipWindow.Get() )
	{
		s_TooltipWindow->SetVisible(false);
	}
	_makeVisible = false;
	_isDirty = true;
}
