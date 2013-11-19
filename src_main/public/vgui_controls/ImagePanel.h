//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef IMAGEPANEL_H
#define IMAGEPANEL_H

#ifdef _WIN32
#pragma once
#endif

#include <vgui/VGUI.h>
#include <vgui_controls/Panel.h>

namespace vgui
{

class IImage;

//-----------------------------------------------------------------------------
// Purpose: Panel that holds a single image
//-----------------------------------------------------------------------------
class ImagePanel : public Panel
{
	DECLARE_CLASS_SIMPLE( ImagePanel, Panel );
public:
	ImagePanel(Panel *parent, const char *name);
	~ImagePanel();

	virtual void SetImage(IImage *image);
	virtual void SetImage(const char *imageName);
	virtual IImage *GetImage();
	char *GetImageName();

	// sets whether or not the image should scale to fit the size of the ImagePanel (defaults to false)
	void SetShouldScaleImage( bool state );
	void SetScaleAmount( float scale );
	float GetScaleAmount( void );

	// set the color to fill with, if no image is specified
	void SetFillColor( Color col );
	Color GetFillColor();

	virtual Color GetDrawColor( void );

protected:
	virtual void PaintBackground();
	virtual void GetSettings(KeyValues *outResourceData);
	virtual void ApplySettings(KeyValues *inResourceData);
	virtual const char *GetDescription();
	virtual void OnSizeChanged(int newWide, int newTall);
	virtual void ApplySchemeSettings( IScheme *pScheme );

private:
	IImage *m_pImage;
	char *m_pszImageName;
	char *m_pszColorName;
	bool m_bScaleImage;
	float m_fScaleAmount;
	Color m_FillColor;
};

} // namespace vgui

#endif // IMAGEPANEL_H
