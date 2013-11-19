//====== Copyright © 1996-2005, Valve Corporation, All rights reserved. =======
//
// Purpose: 
//
//=============================================================================

#ifndef SIMPLEPOTTERYWHEELPANEL_H
#define SIMPLEPOTTERYWHEELPANEL_H

#ifdef _WIN32
#pragma once
#endif


#include "vgui_controls/EditablePanel.h"
#include "materialsystem/materialsystemutil.h"


//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------
class IManipulator;
class CPotteryWheelManip;
class CBaseManipulator;
class CTransformManipulator;
class CDmeCamera;
class CDmeLight;
class CDmeDag;
class CDmElement;

namespace vgui
{
	class IScheme;
}


//-----------------------------------------------------------------------------
// Base pottery wheel Panel
//-----------------------------------------------------------------------------
class CSimplePotteryWheelPanel : public vgui::EditablePanel
{
	DECLARE_CLASS_SIMPLE( CSimplePotteryWheelPanel, vgui::EditablePanel );

public:
	// constructor, destructor
	CSimplePotteryWheelPanel( vgui::Panel *pParent, const char *pName );
	virtual ~CSimplePotteryWheelPanel();

	// Overriden methods of vgui::Panel
	virtual void Paint();

	virtual void OnKeyCodePressed ( vgui::KeyCode code );
	virtual void OnKeyCodeReleased( vgui::KeyCode code );
	virtual void OnMousePressed ( vgui::MouseCode code );
	virtual void OnMouseReleased( vgui::MouseCode code );
	virtual void OnCursorMoved( int x, int y );
	virtual void OnMouseWheeled( int delta );
	virtual void OnTick();

	virtual void OnMouseCaptureLost();

	// Sets the camera to look at the the thing we're spinning around
	void LookAt( const Vector &vecCenter, float flRadius );
	void LookAt( float flRadius );

	void ComputePanelPosition( const Vector &vecPosition, Vector2D *pPanelPos );

	void SetBackgroundColor( int r, int g, int b );
	void SetBackgroundColor( const Color& c );
	const Color& GetBackgroundColor() const;

	// Light probe
	void SetLightProbe( CDmElement *pLightProbe );

	// Compute the camera world position
	void ComputeCameraTransform( matrix3x4_t *pWorldToCamera );

private:
	// Inherited classes must implement this
	virtual void OnPaint3D() = 0;

private:
	
 	enum ManipulationMode_t 
	{
		CAMERA_ROTATE,
		CAMERA_TRANSLATE,
		CAMERA_ZOOM,
		LIGHT_MODE,
	};

	void Select();
	void EnterManipulationMode( ManipulationMode_t manipMode, bool bMouseCapture = true, vgui::MouseCode mouseCode = vgui::MouseCode( -1 ) );
	void AcceptManipulation( bool bReleaseMouseCapture = true );
	void CancelManipulation();
	void EnableMouseCapture( bool enable, vgui::MouseCode code = vgui::MouseCode( -1 ) );
	bool WarpMouse( int &x, int &y );

protected:
	void UseEngineCoordinateSystem( bool bEnable );
	bool IsUsingEngineCoordinateSystem() const;
	bool HasLightProbe() const;
	ITexture *GetLightProbeCubemap( bool bHDR );

	CDmeDag *m_pDag;

private:
	void CreateDefaultLights();
	void DestroyLights();

	CMaterialReference m_LightProbeBackground;
	CMaterialReference m_LightProbeHDRBackground;
	CTextureReference m_LightProbeCubemap;
	CTextureReference m_LightProbeHDRCubemap;

	CDmeCamera *m_pCamera;
	CDmeDag *m_pCameraRootDag;
	CUtlVector< CDmeLight * > m_Lights;

	Color m_ClearColor;
	CTransformManipulator	*m_pCameraRotate;
	CTransformManipulator	*m_pCameraTranslate;
	CBaseManipulator		*m_pCameraZoom;
	CPotteryWheelManip		*m_pLightManip;
	IManipulator			*m_pCurrentManip;
	vgui::MouseCode			m_nCaptureMouseCode;
	Vector4D m_vecAmbientCube[6];
	float m_flDistance;
	int m_xoffset, m_yoffset;
	int m_nManipStartX, m_nManipStartY;

	bool	m_bHasLightProbe : 1;
};


#endif // SIMPLEPOTTERYWHEELPANEL_H
