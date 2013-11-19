//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef CSTRIKECLIENTSCOREBOARDDIALOG_H
#define CSTRIKECLIENTSCOREBOARDDIALOG_H
#ifdef _WIN32
#pragma once
#endif

#include <clientscoreboarddialog.h>
#include "cs_shareddefs.h"

//-----------------------------------------------------------------------------
// Purpose: Game ScoreBoard
//-----------------------------------------------------------------------------
class CCSClientScoreBoardDialog : public CClientScoreBoardDialog
{
private:
	DECLARE_CLASS_SIMPLE( CCSClientScoreBoardDialog, CClientScoreBoardDialog );
	
public:
	CCSClientScoreBoardDialog( IViewPort *pViewPort );
	~CCSClientScoreBoardDialog();

	virtual void Reset();
	virtual void Update();

	// vgui overrides for rounded corner background
	virtual void PaintBackground();
	virtual void PaintBorder();
	virtual void ApplySchemeSettings( vgui::IScheme *pScheme );

private:
	void InitPlayerList( vgui::SectionedListPanel *pPlayerList, int teamNumber );
	void UpdateTeamInfo();
	void UpdatePlayerList();
	void UpdateSpectatorList();
	bool GetPlayerScoreInfo( int playerIndex, KeyValues *outPlayerInfo );
	
	bool ShouldShowAsSpectator( int iPlayerIndex );
	void FireGameEvent( IGameEvent *event );

	static bool CSPlayerSortFunc( vgui::SectionedListPanel *list, int itemID1, int itemID2 );

	// rounded corners
	Color					 m_bgColor;
	Color					 m_borderColor;

	// player lists
	vgui::SectionedListPanel *m_pPlayerListT;
	vgui::SectionedListPanel *m_pPlayerListCT;

	vgui::Label	*m_pPlayerCountLabel_T;
	vgui::Label	*m_pScoreLabel_T;
	vgui::Label	*m_pPingLabel_T;
	vgui::Label	*m_pPlayerCountLabel_CT;
	vgui::Label	*m_pScoreLabel_CT;
	vgui::Label	*m_pPingLabel_CT;
};


#endif // CSTRIKECLIENTSCOREBOARDDIALOG_H
