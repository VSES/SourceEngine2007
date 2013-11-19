//====== Copyright © 1996-2005, Valve Corporation, All rights reserved. =======
//
// Purpose: 
//
//=============================================================================

#include "cbase.h"
#include <vgui_controls/Controls.h>
#include <vgui_controls/Panel.h>
#include <vgui/ISurface.h>
#include "vgui_avatarimage.h"
#if defined( _X360 )
#include "xbox/xbox_win32stubs.h"
#endif

DECLARE_BUILD_FACTORY( CAvatarImagePanel );

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CAvatarImage::CAvatarImage( void )
{
	ClearAvatarSteamID();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CAvatarImage::SetAvatarSteamID( CSteamID steamIDUser )
{
	ClearAvatarSteamID();
#ifndef NO_STEAM
	if ( SteamFriends() && SteamUtils() )
	{
		int iAvatar = SteamFriends()->GetFriendAvatar( steamIDUser );

		/*
		// See if it's in our list already
		*/

		uint32 wide, tall;
		if ( SteamUtils()->GetImageSize( iAvatar, &wide, &tall ) )
		{
			int cubImage = wide * tall * 4;
			byte *rgubDest = (byte*)_alloca( cubImage );
			SteamUtils()->GetImageRGBA( iAvatar, rgubDest, cubImage );
			InitFromRGBA( rgubDest, wide, tall );

			/*
			// put it in the list
			RGBAImage *pRGBAImage = new RGBAImage( rgubDest, wide, tall );
			int iImageList = m_pImageList->AddImage( pRGBAImage );
			m_mapAvatarToIImageList.Insert( iAvatar, iImageList );
			*/
		}
	}
#endif
	return m_bValid;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CAvatarImage::InitFromRGBA( const byte *rgba, int width, int height )
{
	m_iTextureID = vgui::surface()->CreateNewTextureID( true );
	vgui::surface()->DrawSetTextureRGBA( m_iTextureID, rgba, width, height, false, false );
	m_nWide = XRES(width);
	m_nTall = YRES(height);
	m_Color = Color( 255, 255, 255, 255 );

	m_bValid = true;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CAvatarImage::Paint( void )
{
	if ( m_bValid )
	{
		vgui::surface()->DrawSetColor( m_Color );
		vgui::surface()->DrawSetTexture( m_iTextureID );
		vgui::surface()->DrawTexturedRect( m_nX, m_nY, m_nX + m_nWide, m_nY + m_nTall );
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CAvatarImagePanel::CAvatarImagePanel( vgui::Panel *parent, const char *name ) : vgui::ImagePanel( parent, name )
{
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CAvatarImagePanel::SetPlayer( C_BasePlayer *pPlayer )
{
	if ( GetImage() )
	{
		((CAvatarImage*)GetImage())->ClearAvatarSteamID();
	}
#ifndef NO_STEAM
	if ( pPlayer && SteamUtils() )
	{
		int iIndex = pPlayer->entindex();
		player_info_t pi;
		if ( engine->GetPlayerInfo(iIndex, &pi) )
		{
			if ( pi.friendsID )
			{
				CSteamID steamIDForPlayer( pi.friendsID, 1, SteamUtils()->GetConnectedUniverse(), k_EAccountTypeIndividual );

				if ( !GetImage() )
				{
					CAvatarImage *pImage = new CAvatarImage();
					SetImage( pImage );
				}

				((CAvatarImage*)GetImage())->SetAvatarSteamID( steamIDForPlayer );

				// Indent the image. These are deliberately non-resolution-scaling.
				int iIndent = 2;
				GetImage()->SetPos( iIndent, iIndent );
				GetImage()->SetSize( GetWide()-(iIndent*2), GetTall()-(iIndent*2) );
			}
		}
	}
#endif
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CAvatarImagePanel::PaintBackground( void )
{
	// Paint the outline
	vgui::surface()->DrawSetColor( m_clrOutline );
	vgui::surface()->DrawFilledRect( 0, 0, GetWide(), GetTall() );

	if ( GetImage() )
	{
		GetImage()->Paint();
	}
}
