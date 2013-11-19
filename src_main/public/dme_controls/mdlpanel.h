//====== Copyright © 1996-2005, Valve Corporation, All rights reserved. =======
//
// Purpose: 
//
//=============================================================================

#ifndef MDLPANEL_H
#define MDLPANEL_H

#ifdef _WIN32
#pragma once
#endif


#include "vgui_controls/Panel.h"
#include "datacache/imdlcache.h"
#include "materialsystem/materialsystemutil.h"
#include "dme_controls/simplepotterywheelpanel.h"
#include "datamodel/dmehandle.h"


//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------
class IManipulator;
class CPotteryWheelManip;
class CTranslationManip;
class CDmeMDL;
class CDmeCamera;
class CDmeLight;
class CDmeDag;
class CDmeMDLMakefile;

namespace vgui
{
	class IScheme;
}


//-----------------------------------------------------------------------------
// Material Viewer Panel
//-----------------------------------------------------------------------------
class CMDLPanel : public CSimplePotteryWheelPanel
{
	DECLARE_CLASS_SIMPLE( CMDLPanel, CSimplePotteryWheelPanel );

public:
	// constructor, destructor
	CMDLPanel( vgui::Panel *pParent, const char *pName );
	virtual ~CMDLPanel();

	// Overriden methods of vgui::Panel
	virtual void ApplySchemeSettings( vgui::IScheme *pScheme );

	virtual void OnTick();

	// Sets the current mdl
	void SetMDL( MDLHandle_t handle );

	// Sets the camera to look at the model
	void LookAtMDL( );

	// Sets the current sequence
	void SetSequence( int nSequence );

	void SetCollsionModel( bool bVisible );
	void SetGroundGrid( bool bVisible );
	void SetWireFrame( bool bVisible );
	void SetLockView( bool bLocked );
	void SetSkin( int nSkin );
	void SetLookAtCamera( bool bLookAtCamera );

	// DMEPanel..
	void SetDmeElement( CDmeMDLMakefile *pMDLMakefile );

private:
	// paint it!
	void OnPaint3D();
	void OnMouseDoublePressed( vgui::MouseCode code );

	void DrawGrid();
	void DrawCollisionModel();
	void UpdateStudioRenderConfig( void );

	CMaterialReference m_Wireframe;
	CTextureReference m_DefaultEnvCubemap;
	CTextureReference m_DefaultHDREnvCubemap;
	CDmeHandle< CDmeMDL > m_hMDL;
	bool	m_bDrawCollisionModel : 1;
	bool	m_bGroundGrid : 1;
	bool	m_bLockView : 1;
	bool	m_bWireFrame : 1;
	bool	m_bIsExternalMDL : 1;
	bool	m_bLookAtCamera : 1;
};


#endif // MDLPANEL_H
