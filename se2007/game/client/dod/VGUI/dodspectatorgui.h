//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef DODSPECTATORGUI_H
#define DODSPECTATORGUI_H
#ifdef _WIN32
#pragma once
#endif

#include <spectatorgui.h>


//-----------------------------------------------------------------------------
// Purpose: Cstrike Spectator UI
//-----------------------------------------------------------------------------
class CDODSpectatorGUI : public CSpectatorGUI
{
private:
	DECLARE_CLASS_SIMPLE( CDODSpectatorGUI, CSpectatorGUI );

public:
	CDODSpectatorGUI( IViewPort *pViewPort );
		
	virtual void ApplySchemeSettings(vgui::IScheme *pScheme);
	virtual void Update( void );
	virtual bool NeedsUpdate( void );
	virtual Color GetClientColor(int index);

	virtual bool ShouldShowPlayerLabel( int specmode );
	
	//virtual bool HasInputElements( void ) { return true; }

protected:

	void UpdateTimer();
	void UpdateScores();

	int		m_nLastTime;
	int		m_nLastSpecMode;
	CBaseEntity	*m_nLastSpecTarget;

	vgui::Label *m_pReinforcementsLabel;
	vgui::Label *m_pTimerLabel;
	vgui::Label *m_pAlliesScoreLabel;
	vgui::Label *m_pAxisScoreLabel;
	vgui::Label *m_pExtraInfo;
};

#endif // DODSPECTATORGUI_H
