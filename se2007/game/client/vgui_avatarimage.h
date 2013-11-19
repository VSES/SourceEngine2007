//====== Copyright © 1996-2005, Valve Corporation, All rights reserved. =======
//
// Purpose: 
//
//=============================================================================

#ifndef VGUI_AVATARIMAGE_H
#define VGUI_AVATARIMAGE_H
#ifdef _WIN32
#pragma once
#endif

#include <vgui_controls/Image.h>
#include <vgui_controls/ImagePanel.h>
#include "steam\steam_api.h"
#include "c_baseplayer.h"

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CAvatarImage : public vgui::IImage
{
public:
	CAvatarImage( void );

	// Call this to set the steam ID associated with the avatar
	bool SetAvatarSteamID( CSteamID steamIDUser );
	void ClearAvatarSteamID( void ) { m_bValid = false; }

	// Call to Paint the image
	// Image will draw within the current panel context at the specified position
	virtual void Paint( void );

	// Set the position of the image
	virtual void SetPos(int x, int y)
	{
		m_nX = x;
		m_nY = y;
	}

	// Gets the size of the content
	virtual void GetContentSize(int &wide, int &tall)
	{
		wide = m_nWide;
		tall = m_nTall;
	}

	// Get the size the image will actually draw in (usually defaults to the content size)
	virtual void GetSize(int &wide, int &tall)
	{
		GetContentSize( wide, tall );
	}

	// Sets the size of the image
	virtual void SetSize(int wide, int tall)	
	{ 
		m_nWide = wide; 
		m_nTall = tall; 
	}

	// Set the draw color 
	virtual void SetColor(Color col)			
	{ 
		m_Color = col; 
	}

	bool	IsValid( void ) { return m_bValid; }

protected:
	void InitFromRGBA( const byte *rgba, int width, int height );

private:
	Color m_Color;
	int m_iTextureID;
	int m_nX, m_nY, m_nWide, m_nTall;
	bool m_bValid;
};

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CAvatarImagePanel : public vgui::ImagePanel
{
public:
	DECLARE_CLASS_SIMPLE( CAvatarImagePanel, vgui::ImagePanel );

	CAvatarImagePanel( vgui::Panel *parent, const char *name );

	// Set the player that this Avatar should display for
	void SetPlayer( C_BasePlayer *pPlayer );

	virtual void PaintBackground( void );

protected:
	CPanelAnimationVar( Color, m_clrOutline, "color_outline", "Black" );
};

#endif // VGUI_AVATARIMAGE_H
