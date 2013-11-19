//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: Creates a Message box with a question in it and yes/no buttons
//
// $NoKeywords: $
//=============================================================================//

#ifndef TOOLTIP_H
#define TOOLTIP_H

#ifdef _WIN32
#pragma once
#endif

#include <vgui/VGUI.h>
#include <vgui_controls/Controls.h>
#include <utlvector.h>

namespace vgui
{

//-----------------------------------------------------------------------------
// Purpose: Tooltip for a panel - shows text when cursor hovers over a panel
//-----------------------------------------------------------------------------
class Tooltip 
{
public:
	Tooltip(Panel *parent, const char *text = NULL);
	~Tooltip();

	void SetText(const char *text);
	const char *GetText();
	void ShowTooltip(Panel *currentPanel);
	void HideTooltip();
	void SizeTextWindow();
	void ResetDelay();
	void PerformLayout();

	void SetTooltipFormatToSingleLine();
	void SetTooltipFormatToMultiLine();
	void SetTooltipDelay(int tooltipDelayMilliseconds);
	int GetTooltipDelay();

private:
	virtual void ApplySchemeSettings(IScheme *pScheme);
	CUtlVector<char> m_Text;
	int _delay;			// delay that counts down
	int _tooltipDelay;	// delay before tooltip comes up.
	bool _makeVisible : 1;
	bool _displayOnOneLine : 1;
	bool _isDirty : 1;
};

};

#endif // TOOLTIP_H
