//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#include "cbase.h"
#include "hudelement.h"
#include <vgui_controls/Panel.h>
#include <vgui/isurface.h>
#include <vgui/ILocalize.h>
#include "c_baseplayer.h"
#include "voice_status.h"
#include "clientmode_shared.h"
#include "c_playerresource.h"
#include "voice_common.h"


ConVar *sv_alltalk = NULL;

//=============================================================================
// Icon for the local player using voice
//=============================================================================
class CHudVoiceSelfStatus : public CHudElement, public vgui::Panel
{
public:
	DECLARE_CLASS_SIMPLE( CHudVoiceSelfStatus, vgui::Panel );

	CHudVoiceSelfStatus( const char *name );

	virtual bool ShouldDraw();	
	virtual void Paint();
	virtual void VidInit();
	virtual void ApplySchemeSettings(vgui::IScheme *pScheme);

private:
	CHudTexture *m_pVoiceIcon;


	Color	m_clrIcon;
};


DECLARE_HUDELEMENT( CHudVoiceSelfStatus );


CHudVoiceSelfStatus::CHudVoiceSelfStatus( const char *pName ) :
	vgui::Panel( NULL, "HudVoiceSelfStatus" ), CHudElement( pName )
{
	SetParent( g_pClientMode->GetViewport() );

	m_pVoiceIcon = NULL;

	SetHiddenBits( 0 );

	m_clrIcon = Color(255,255,255,255);
}

	
void CHudVoiceSelfStatus::ApplySchemeSettings(vgui::IScheme *pScheme)
{
	BaseClass::ApplySchemeSettings( pScheme );

#ifdef HL2MP
	SetBgColor( Color( 0, 0, 0, 0 ) );
#endif
}

void CHudVoiceSelfStatus::VidInit( void )
{
	m_pVoiceIcon = gHUD.GetIcon( "voice_self" );
}

bool CHudVoiceSelfStatus::ShouldDraw()
{
	return GetClientVoiceMgr()->IsLocalPlayerSpeaking();
}

void CHudVoiceSelfStatus::Paint()
{
   if( !m_pVoiceIcon )
		return;
	
	int x, y, w, h;
	GetBounds( x, y, w, h );

	m_pVoiceIcon->DrawSelf( 0, 0, w, h, m_clrIcon );
}


//=============================================================================
// Icons for other players using voice
//=============================================================================
class CHudVoiceStatus : public CHudElement, public vgui::Panel
{
public:
	DECLARE_CLASS_SIMPLE( CHudVoiceStatus, vgui::Panel );

	CHudVoiceStatus( const char *name );

	virtual bool ShouldDraw();	
	virtual void Paint();
	virtual void VidInit();
	virtual void Init();
	virtual void OnThink();
	virtual void ApplySchemeSettings(vgui::IScheme *pScheme);

private:
	CHudTexture *m_pVoiceIcon;
	int m_iDeadImageID;

	Color	m_clrIcon;

	CUtlLinkedList< int > m_SpeakingList;
	
	CPanelAnimationVar( vgui::HFont, m_NameFont, "Default", "Default" );

	CPanelAnimationVarAliasType( float, item_tall, "item_tall", "32", "proportional_float" );
	CPanelAnimationVarAliasType( float, item_wide, "item_wide", "100", "proportional_float" );

	CPanelAnimationVarAliasType( float, item_spacing, "item_spacing", "2", "proportional_float" );

	CPanelAnimationVarAliasType( float, icon_ypos, "icon_ypos", "0", "proportional_float" );
	CPanelAnimationVarAliasType( float, icon_xpos, "icon_xpos", "0", "proportional_float" );
	CPanelAnimationVarAliasType( float, icon_tall, "icon_tall", "32", "proportional_float" );
	CPanelAnimationVarAliasType( float, icon_wide, "icon_wide", "32", "proportional_float" );

	CPanelAnimationVarAliasType( float, text_ypos, "text_ypos", "4", "proportional_float" );
	CPanelAnimationVarAliasType( float, text_xpos, "text_xpos", "32", "proportional_float" );
};


DECLARE_HUDELEMENT( CHudVoiceStatus );


CHudVoiceStatus::CHudVoiceStatus( const char *pName ) :
	vgui::Panel( NULL, "HudVoiceStatus" ), CHudElement( pName )
{
	SetParent( g_pClientMode->GetViewport() );

	m_pVoiceIcon = NULL;

	SetHiddenBits( 0 );

	m_clrIcon = Color(255,255,255,255);

	m_iDeadImageID = surface()->DrawGetTextureId( "hud/leaderboard_dead" );
	if ( m_iDeadImageID == -1 ) // we didn't find it, so create a new one
	{
		m_iDeadImageID = surface()->CreateNewTextureID();	
	}

	surface()->DrawSetTextureFile( m_iDeadImageID, "hud/leaderboard_dead", true, false );
}

void CHudVoiceStatus::ApplySchemeSettings(vgui::IScheme *pScheme)
{
	BaseClass::ApplySchemeSettings( pScheme );

#ifdef HL2MP
	SetBgColor( Color( 0, 0, 0, 0 ) );
#endif
}

void CHudVoiceStatus::Init( void )
{
	m_SpeakingList.RemoveAll();
}

void CHudVoiceStatus::VidInit( void )
{
	m_pVoiceIcon = gHUD.GetIcon( "voice_player" );
}

void CHudVoiceStatus::OnThink( void )
{
	for( int i=1;i<=gpGlobals->maxClients;i++ )
	{
		bool bTalking = GetClientVoiceMgr()->IsPlayerSpeaking(i);

		// if they are in the list and not talking, remove them
		if( !bTalking )
		{
			// remove them if they are in the list
			m_SpeakingList.FindAndRemove(i);
		}
		else
		{
			// if they are talking and not in the list, add them to the end
			if( m_SpeakingList.Find(i) == m_SpeakingList.InvalidIndex() )
			{
				m_SpeakingList.AddToTail(i);
			}
		}
	}
}

bool CHudVoiceStatus::ShouldDraw()
{
	if ( IsInFreezeCam() == true )
		return false;

	return true;
}

void CHudVoiceStatus::Paint()
{
   	if( !m_pVoiceIcon )
		return;
	
	int x, y, w, h;
	GetBounds( x, y, w, h );

	// Heights to draw the current voice item at
	int xpos = 0;
	int ypos = h - item_tall;


	int i;
	int length = m_SpeakingList.Count();

	int iFontHeight = 0;

	if( length > 0 )
	{
		surface()->DrawSetTextFont( m_NameFont );
		surface()->DrawSetTextColor( Color(255,255,255,255) );
		iFontHeight = surface()->GetFontTall( m_NameFont );
	}

	if ( !sv_alltalk )
		sv_alltalk = cvar->FindVar( "sv_alltalk" );

	//draw everyone in the list!
	for( i = m_SpeakingList.Head(); i != m_SpeakingList.InvalidIndex(); i = m_SpeakingList.Next(i) )
	{
		int playerIndex = m_SpeakingList.Element(i);
		bool bIsAlive = g_PR->IsAlive( playerIndex );

		Color c = g_PR->GetTeamColor( g_PR ? g_PR->GetTeam(playerIndex) : TEAM_UNASSIGNED );

		c[3] = 128;
	
		const char *pName = g_PR ? g_PR->GetPlayerName(playerIndex) : "unknown";
		wchar_t szconverted[ 64 ];

		// Add the location, if any
		bool usedLocation = false;
		if ( sv_alltalk && !sv_alltalk->GetBool() )
		{
			C_BasePlayer *pPlayer = UTIL_PlayerByIndex( playerIndex );
			if ( pPlayer )
			{
				const char *asciiLocation = pPlayer->GetLastKnownPlaceName();
				if ( asciiLocation && *asciiLocation )
				{
					const wchar_t *unicodeLocation = g_pVGuiLocalize->Find( asciiLocation );
					if ( unicodeLocation && *unicodeLocation )
					{
						wchar_t *formatStr = g_pVGuiLocalize->Find( "#Voice_UseLocation" );
						if ( formatStr )
						{
							wchar_t unicodeName[ 64 ];
							g_pVGuiLocalize->ConvertANSIToUnicode( pName, unicodeName, sizeof( unicodeName ) );

							g_pVGuiLocalize->ConstructString( szconverted, sizeof( szconverted ),
								formatStr, 2, unicodeName, unicodeLocation );

							usedLocation = true;
						}
					}
				}
			}
		}

		if ( !usedLocation )
		{
			g_pVGuiLocalize->ConvertANSIToUnicode( pName, szconverted, sizeof(szconverted)  );
		}

		// Draw the item background
		surface()->DrawSetColor( c );
		surface()->DrawFilledRect( xpos, ypos, xpos + item_wide, ypos + item_tall );
	
		int iDeathIconWidth = 0;

		if ( bIsAlive == false && m_iDeadImageID != -1 )
		{
			Vertex_t vert[4];	
			float uv1 = 0.0f;
			float uv2 = 1.0f;

			// Draw the dead material
			surface()->DrawSetTexture( m_iDeadImageID );

			vert[0].Init( Vector2D( xpos, ypos ), Vector2D( uv1, uv1 ) );
			vert[1].Init( Vector2D( xpos + icon_wide, ypos ), Vector2D( uv2, uv1 ) );
			vert[2].Init( Vector2D( xpos + icon_wide, ypos + icon_tall ), Vector2D( uv2, uv2 ) );				
			vert[3].Init( Vector2D( xpos, ypos + icon_tall ), Vector2D( uv1, uv2 ) );

			surface()->DrawSetColor( Color(255,255,255,255) );

			surface()->DrawTexturedPolygon( 4, vert );

			iDeathIconWidth = icon_wide;
		}

		// Draw the voice icon
		m_pVoiceIcon->DrawSelf( xpos + icon_xpos + iDeathIconWidth, ypos + icon_ypos, icon_wide, icon_tall, m_clrIcon );

		// Draw the player's name
		surface()->DrawSetTextPos( xpos + text_xpos + iDeathIconWidth, ypos + ( item_tall / 2 ) - ( iFontHeight / 2 ) );

		int iTextSpace = item_wide - text_xpos;

		// write as much of the name as will fit, truncate the rest and add ellipses
		int iNameLength = wcslen(szconverted);
		const wchar_t *pszconverted = szconverted;
		int iTextWidthCounter = 0;
		for( int j=0;j<iNameLength;j++ )
		{
			iTextWidthCounter += surface()->GetCharacterWidth( m_NameFont, pszconverted[j] );

			if( iTextWidthCounter > iTextSpace )
			{	
				if( j > 3 )
				{
					szconverted[j-2] = '.';
					szconverted[j-1] = '.';
					szconverted[j] = '\0';
				}
				break;
			}
		}

		surface()->DrawPrintText( szconverted, wcslen(szconverted) );
			
		ypos -= ( item_spacing + item_tall );
	}
}
