//========= Copyright © 1996-2001, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

#include "vgui_controls/scrollableeditablepanel.h"
#include "vgui_controls/scrollbar.h"

using namespace vgui;

ScrollableEditablePanel::ScrollableEditablePanel( vgui::Panel *pParent, vgui::EditablePanel *pChild, const char *pName ) :
	BaseClass( pParent, pName )
{
	m_pChild = pChild;
	m_pChild->SetParent( this );

	m_pScrollBar = new vgui::ScrollBar( this, "VerticalScrollBar", true ); 
	m_pScrollBar->SetWide( 16 );
	m_pScrollBar->SetAutoResize( PIN_TOPRIGHT, AUTORESIZE_DOWN, 0, 0, -16, 0 );
	m_pScrollBar->AddActionSignalTarget( this );
}

void ScrollableEditablePanel::PerformLayout()
{
	BaseClass::PerformLayout();

	m_pChild->SetWide( GetWide() - 16 );
	m_pScrollBar->SetRange( 0, m_pChild->GetTall() );
	m_pScrollBar->SetRangeWindow( GetTall() );
}


//-----------------------------------------------------------------------------
// Called when the scroll bar moves
//-----------------------------------------------------------------------------
void ScrollableEditablePanel::OnScrollBarSliderMoved()
{
	InvalidateLayout();

	int nScrollAmount = m_pScrollBar->GetValue();
	m_pChild->SetPos( 0, -nScrollAmount ); 
}



