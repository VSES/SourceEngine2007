//===== Copyright © 1996-2006, Valve Corporation, All rights reserved. ======//
//
// Shows the model browser in a window
//
//===========================================================================//

#include "vgui_controls/frame.h"
#include "dme_controls/mdlpicker.h"
#include "vsvguiwindow.h"


//-----------------------------------------------------------------------------
// Sequence picker frame
//-----------------------------------------------------------------------------
class CMDLPreviewFrame : public vgui::Frame
{
	DECLARE_CLASS_SIMPLE( CMDLPreviewFrame, vgui::Frame );
public:
	CMDLPreviewFrame() : BaseClass( NULL, "MDLPreviewFrame" ) 
	{
		m_pMDLPicker = new CMDLPicker( this );
		SetTitle( "Model Browser", true );
		SetSizeable( false );
		SetCloseButtonVisible( false );
		SetMoveable( false );
		Activate();
		m_pMDLPicker->Activate();
	}

	virtual ~CMDLPreviewFrame() 
	{
		delete m_pMDLPicker;
	}

	virtual void PerformLayout()
	{
		BaseClass::PerformLayout();

		int x, y, w, h;
		GetClientArea( x, y, w, h );
		m_pMDLPicker->SetBounds( x, y, w, h );
	}

private:
	CMDLPicker *m_pMDLPicker;
};


//-----------------------------------------------------------------------------
// Hook the vgui window into the class factory
//-----------------------------------------------------------------------------
INSTALL_MAYA_VGUI_WINDOW( CMDLPreviewFrame, "MDLPreview" );

