//====== Copyright © 1996-2005, Valve Corporation, All rights reserved. =======
//
// Purpose: 
//
//=============================================================================

#include "dme_controls/simplepotterywheelpanel.h"
#include "dme_controls/manipulator.h"
#include "vgui/isystem.h"
#include "vgui/cursor.h"
#include "vgui/ivgui.h"
#include "vgui/isurface.h"
#include "vgui/iinput.h"
#include "vguimatsurface/IMatSystemSurface.h"
#include "movieobjects/dmecamera.h"
#include "movieobjects/dmelight.h"
#include "movieobjects/dmetransform.h"
#include "datamodel/dmehandle.h"
#include "movieobjects/dmedag.h"
#include "vgui_controls/frame.h"
#include "convar.h"
#include "tier0/dbg.h"
#include "matsys_controls/matsyscontrols.h"
#include "materialsystem/imaterial.h"
#include "materialsystem/imaterialsystem.h"
#include "istudiorender.h"
#include "materialsystem/imaterialsystemhardwareconfig.h"
#include "tier2/renderutils.h"
#include "tier1/keyvalues.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


using namespace vgui;

extern float UpdateTime( float &flLastTime );

class CTranslationManipulator : public CTransformManipulator
{
public:
	CTranslationManipulator( CDmeTransform *transform, CDmeCamera *pCamera ) : CTransformManipulator( transform )
	{
		m_lastx = m_lasty = 0;
		m_hCamera = pCamera;
		m_bIsUsingEngineCoordinateSystem = false;
	}

	virtual void OnMousePressed( vgui::MouseCode code, int x, int y )
	{
		m_lasty = y;
		m_lastx = x;
	}

	virtual void OnCursorMoved( int x, int y )
	{
		if ( !m_pTransform )
			return;

		Vector vPosition = m_pTransform->GetValue<Vector>( "position" );
		Quaternion qOrientation = m_pTransform->GetValue<Quaternion>( "orientation" );
		QAngle quakeEuler( RadianEuler( qOrientation ).ToQAngle() );
		
		Vector forward, right, up;
		AngleVectors( quakeEuler, &forward, &right, &up );
		
		int dy = y - m_lasty;
		int dx = x - m_lastx;
		
		if ( m_bIsUsingEngineCoordinateSystem )
		{
			right *= -0.2f * dx;
			up *= 0.2f * dy;
			vPosition += up + right;
		}
		else
		{
			forward *= -0.2f * dx;
			right *= -0.2f * dy;
			vPosition += forward + right;
		}

		m_lastx = x;
		m_lasty = y;

		CDisableUndoScopeGuard guard;
		m_pTransform->SetValue( "position", vPosition );
	}

	// set coordinate system mode
	void SetUsingEngineCoordinateSystem( bool bUseEngineCoordinateSystem )
	{
		m_bIsUsingEngineCoordinateSystem = bUseEngineCoordinateSystem;
	}
	bool IsUsingEngineCoordinateSystem() const
	{
		return m_bIsUsingEngineCoordinateSystem;
	}

protected:
	CDmeHandle< CDmeCamera > m_hCamera;
	int m_lastx, m_lasty;
	bool m_bIsUsingEngineCoordinateSystem;
};

class CZoomManipulator : public CBaseManipulator
{
public:
	CZoomManipulator( float *pDistance )
	{
		m_lasty = 0;
		m_pDistance = pDistance;
	}

	virtual void OnMousePressed( vgui::MouseCode code, int x, int y )
	{
		m_lasty = y;
	}

	virtual void OnCursorMoved( int x, int y )
	{
		float delta  = 0.2f * ( y - m_lasty );
		m_lasty = y;
		*m_pDistance *= pow( 1.01f, delta );
	}

protected:
	int m_lasty;
	float *m_pDistance;
};

class CRotationManipulator : public CTransformManipulator
{
public:
	CRotationManipulator( CDmeTransform *transform, CDmeCamera *pCamera ) : CTransformManipulator( transform )
	{
		m_hCamera = pCamera;
		SetUsingEngineCoordinateSystem( false );
		UpdateTransform();
	}

	virtual void OnMousePressed( vgui::MouseCode code, int x, int y )
	{
		m_lasty = y;
		m_lastx = x;
	}

	virtual void OnCursorMoved( int x, int y )
	{
		m_azimuth  += 0.002f * ( m_lastx - x );
		m_altitude -= 0.002f * ( m_lasty - y );
		m_altitude = max( -M_PI/2, min( M_PI/2, m_altitude ) );
	
		m_lastx = x;
		m_lasty = y;

		UpdateTransform();
	}

	virtual void UpdateTransform()
	{
		if ( !m_pTransform )
			return;

		CDisableUndoScopeGuard guard;
		QAngle angles;
		if ( m_bIsUsingEngineCoordinateSystem )
		{
			angles.Init( RAD2DEG( m_altitude ), RAD2DEG( m_azimuth ), 0 );
		}
		else
		{
			angles.Init( RAD2DEG( m_azimuth ), 0, RAD2DEG( -m_altitude ) );
		}
		Quaternion qOrientation( angles );
		m_pTransform->SetValue( "orientation", qOrientation );
	}

	// set coordinate system mode
	void SetUsingEngineCoordinateSystem( bool bUseEngineCoordinateSystem )
	{
		m_bIsUsingEngineCoordinateSystem = bUseEngineCoordinateSystem;
		m_lastx = m_lasty = 0;
		m_altitude = M_PI/6;
		if ( m_bIsUsingEngineCoordinateSystem )
		{
			m_azimuth = -3*M_PI/4;
		}
		else
		{
			m_azimuth = M_PI/4;
		}
		UpdateTransform();
	}
	bool IsUsingEngineCoordinateSystem() const
	{
		return m_bIsUsingEngineCoordinateSystem;
	}

	CDmeHandle< CDmeCamera > m_hCamera;
	int m_lastx, m_lasty;
	float m_altitude, m_azimuth;
	bool m_bIsUsingEngineCoordinateSystem;
};


//-----------------------------------------------------------------------------
// Constructor, destructor
//-----------------------------------------------------------------------------
CSimplePotteryWheelPanel::CSimplePotteryWheelPanel( vgui::Panel *pParent, const char *pName ) :
	BaseClass( pParent, pName ), 
	m_pCameraRotate( NULL ), 
	m_pCameraTranslate( NULL ),
	m_pCameraZoom( NULL ),
	m_pLightManip( NULL ),
	m_pCurrentManip( NULL ),
	m_nCaptureMouseCode( vgui::MouseCode( -1 ) ),
	m_xoffset( 0 ), m_yoffset( 0 )
{
	m_bHasLightProbe = false;

	SetPaintBackgroundEnabled( false );
	SetPaintBorderEnabled( false );
	m_ClearColor.SetColor( 76, 88, 68, 255 );

	m_pCamera = CreateElement< CDmeCamera >( "camera", DMFILEID_INVALID );

	CreateDefaultLights();

	m_pDag = NULL;
	m_pCameraRootDag = CreateElement< CDmeDag >( "root", DMFILEID_INVALID );
	m_pCameraRootDag->AddChild( m_pCamera );
	m_nManipStartX = m_nManipStartY = 0;

	m_flDistance = 100.0f;

	m_pCameraRotate = new CRotationManipulator( m_pCameraRootDag->GetTransform(), m_pCamera );
	m_pCameraTranslate = new CTranslationManipulator( m_pCameraRootDag->GetTransform(), m_pCamera );
	m_pCameraZoom = new CZoomManipulator( &m_flDistance );

	SetKeyBoardInputEnabled( true );

	// Used to poll input
	vgui::ivgui()->AddTickSignal( GetVPanel() );
}

CSimplePotteryWheelPanel::~CSimplePotteryWheelPanel()
{
	delete m_pCameraRotate;
	delete m_pCameraZoom;
	delete m_pCameraTranslate;

	CDisableUndoScopeGuard sg;

	g_pDataModel->DestroyElement( m_pCamera->GetHandle() );
	DestroyLights();
	g_pDataModel->DestroyElement( m_pCameraRootDag->GetHandle() );
}

void CSimplePotteryWheelPanel::CreateDefaultLights()
{
	CDisableUndoScopeGuard sg;

	CDmeDirectionalLight *pLight = CreateElement< CDmeDirectionalLight >( "dir light", DMFILEID_INVALID );
	m_Lights.AddToTail( pLight );
	for ( int i = 0; i < 6; ++i )
	{
		m_vecAmbientCube[i].Init( 0.4f, 0.4f, 0.4f, 1.0f );
	}
	m_pLightManip = new CPotteryWheelManip( pLight->GetTransform() );
}


void CSimplePotteryWheelPanel::DestroyLights()
{
	CDisableUndoScopeGuard sg;

	int nLightCount = m_Lights.Count();
	for ( int i = 0; i < nLightCount; ++i )
	{
		g_pDataModel->DestroyElement( m_Lights[i]->GetHandle() );
	}
	m_Lights.RemoveAll();

	if ( m_pLightManip )
	{
		delete m_pLightManip;
		m_pLightManip = NULL;
	}
}


//-----------------------------------------------------------------------------
// Sets the background color
//-----------------------------------------------------------------------------
void CSimplePotteryWheelPanel::SetBackgroundColor( int r, int g, int b )
{
	m_ClearColor.SetColor( r, g, b, 255 );
}

void CSimplePotteryWheelPanel::SetBackgroundColor( const Color& c )
{
	m_ClearColor = c;
}

const Color& CSimplePotteryWheelPanel::GetBackgroundColor() const
{
	return m_ClearColor;
}


//-----------------------------------------------------------------------------
// Light probe
//-----------------------------------------------------------------------------
void CSimplePotteryWheelPanel::SetLightProbe( CDmElement *pLightProbe )
{
	m_LightProbeBackground.Shutdown();
	m_LightProbeHDRBackground.Shutdown();
	m_LightProbeCubemap.Shutdown();
	m_LightProbeHDRCubemap.Shutdown();
	DestroyLights();

	m_bHasLightProbe = ( pLightProbe != NULL );
	if ( !m_bHasLightProbe )
	{
		CreateDefaultLights();
		return;
	}

	const char *pCubemap = pLightProbe->GetValueString( "cubemap" );
	m_LightProbeCubemap.Init( pCubemap, TEXTURE_GROUP_OTHER );

	const char *pCubemapHDR = pLightProbe->HasAttribute( "cubemapHdr" ) ? pLightProbe->GetValueString( "cubemapHdr" ) : pCubemap;
	m_LightProbeHDRCubemap.Init( pCubemapHDR, TEXTURE_GROUP_OTHER );

	KeyValues *pVMTKeyValues = new KeyValues( "UnlitGeneric" );
	pVMTKeyValues->SetInt( "$ignorez", 1 );
	pVMTKeyValues->SetString( "$envmap", pCubemap );
	pVMTKeyValues->SetInt( "$no_fullbright", 1 );
	pVMTKeyValues->SetInt( "$nocull", 1 );
	m_LightProbeBackground.Init( "SPWP_LightProbeBackground", pVMTKeyValues );
	m_LightProbeBackground->Refresh();

	pVMTKeyValues = new KeyValues( "UnlitGeneric" );
	pVMTKeyValues->SetInt( "$ignorez", 1 );
	pVMTKeyValues->SetString( "$envmap", pCubemapHDR );
	pVMTKeyValues->SetInt( "$no_fullbright", 1 );
	pVMTKeyValues->SetInt( "$nocull", 1 );
	m_LightProbeHDRBackground.Init( "SPWP_LightProbeBackground", pVMTKeyValues );
	m_LightProbeHDRBackground->Refresh();

	const CDmrArray< Vector > ambientCube( pLightProbe, "ambientCube" );
	if ( ambientCube.Count() == 6 )
	{
		for ( int i = 0; i < 6; ++i )
		{
			m_vecAmbientCube[i].Init( ambientCube[i].x, ambientCube[i].y, ambientCube[i].z, 0.0f );
		}
	}

	const CDmrElementArray<> localLights( pLightProbe, "localLights" );
	int nLightCount = localLights.Count();
	for ( int i = 0; i < nLightCount; ++i )
	{
		CDmElement *pLocalLight = localLights[ i ];
		const char *pType = pLocalLight->GetValueString( "name" );

		const Vector& vecColor = pLocalLight->GetValue<Vector>( "color" );
		float flIntensity = max( vecColor.x, vecColor.y );
		flIntensity = max( flIntensity, vecColor.z );
		if ( flIntensity == 0.0f )
		{
			flIntensity = 1.0f;
		}

		Color c( 255 * vecColor.x / flIntensity + 0.5f, 255 * vecColor.y / flIntensity + 0.5f, 255 * vecColor.z / flIntensity + 0.5f, 255 );
		if ( !Q_stricmp( pType, "directional" ) )
		{
			CDmeDirectionalLight *pLight = CreateElement< CDmeDirectionalLight >( "light" );
			pLight->SetDirection( pLocalLight->GetValue<Vector>( "direction" ) );
			pLight->SetColor( c );
			pLight->SetIntensity( flIntensity );
			m_Lights.AddToTail( pLight );
			continue;
		}

		if ( !Q_stricmp( pType, "point" ) )
		{
			const Vector& vecAtten = pLocalLight->GetValue<Vector>( "attenuation" );
			CDmePointLight *pLight = CreateElement< CDmePointLight >( "light" );
			pLight->SetPosition( pLocalLight->GetValue<Vector>( "origin" ) );
			pLight->SetColor( c );
			pLight->SetIntensity( flIntensity );
			pLight->SetAttenuation( vecAtten.x, vecAtten.y, vecAtten.z );
			pLight->SetMaxDistance( pLocalLight->GetValue<float>( "maxDistance" ) );
			m_Lights.AddToTail( pLight );
			continue;
		}

		if ( !Q_stricmp( pType, "spot" ) )
		{
			const Vector& vecAtten = pLocalLight->GetValue<Vector>( "attenuation" );
			CDmeSpotLight *pLight = CreateElement< CDmeSpotLight >( "light" );
			pLight->SetPosition( pLocalLight->GetValue<Vector>( "origin" ) );
			pLight->SetDirection( pLocalLight->GetValue<Vector>( "direction" ) );
			pLight->SetColor( c );
			pLight->SetIntensity( flIntensity );
			pLight->SetAttenuation( vecAtten.x, vecAtten.y, vecAtten.z );
			pLight->SetMaxDistance( pLocalLight->GetValue<float>( "maxDistance" ) );
			pLight->SetAngles( 
				RAD2DEG ( pLocalLight->GetValue<float>( "theta" ) ), 
				RAD2DEG ( pLocalLight->GetValue<float>( "phi" ) ), 
				pLocalLight->GetValue<float>( "exponent" ) );
			m_Lights.AddToTail( pLight );
			continue;
		}
	}

	if ( nLightCount > 0 )
	{
		m_pLightManip = new CPotteryWheelManip( m_Lights[0]->GetTransform() );
	}
}

bool CSimplePotteryWheelPanel::HasLightProbe() const
{
	return m_bHasLightProbe;
}

ITexture *CSimplePotteryWheelPanel::GetLightProbeCubemap( bool bHDR )
{
	if ( !m_bHasLightProbe )
		return NULL;

	return bHDR ? m_LightProbeHDRCubemap : m_LightProbeCubemap;
}


//-----------------------------------------------------------------------------
// Makes the panel use the engine coordinate system (z-up)
//-----------------------------------------------------------------------------
void CSimplePotteryWheelPanel::UseEngineCoordinateSystem( bool bEnable )
{
	static_cast< CRotationManipulator* >( m_pCameraRotate )->SetUsingEngineCoordinateSystem( bEnable );
	static_cast< CTranslationManipulator* >( m_pCameraTranslate )->SetUsingEngineCoordinateSystem( bEnable );
}

bool CSimplePotteryWheelPanel::IsUsingEngineCoordinateSystem() const
{
	return static_cast< CRotationManipulator* >( m_pCameraRotate )->IsUsingEngineCoordinateSystem();
}


//-----------------------------------------------------------------------------
// Sets the camera to look at the the thing we're spinning around
//-----------------------------------------------------------------------------
void CSimplePotteryWheelPanel::LookAt( float flRadius )
{
	// Compute the distance to the camera for the object based on its
	// radius and fov.

	// since tan( fov/2 ) = f/d
	// cos( fov/2 ) = r / r' where r = sphere radius, r' = perp distance from sphere center to max extent of camera
	// d/f = r'/d' where d' is distance of camera to sphere
	// d' = r' / tan( fov/2 ) * r' = r / ( cos (fov/2) * tan( fov/2 ) ) = r / sin( fov/2 )
	float flFOVx = m_pCamera->GetFOVx();

	// Compute fov/2 in radians
	flFOVx *= M_PI / 360.0f;

	// Compute an effective fov	based on the aspect ratio 
	// if the height is smaller than the width
	int w, h;
	GetSize( w, h );
	if ( h < w )
	{
		flFOVx = atan( h * tan( flFOVx ) / w );
	}

	m_flDistance = flRadius / sin( flFOVx );
}


void CSimplePotteryWheelPanel::LookAt( const Vector &vecCenter, float flRadius )
{
	CDisableUndoScopeGuard guard;

	matrix3x4_t mat;
	m_pCameraRootDag->GetTransform()->GetTransform( mat );
	MatrixSetColumn( vecCenter, 3, mat );
	m_pCameraRootDag->GetTransform()->SetTransform( mat );

	LookAt( flRadius );
}

	
//-----------------------------------------------------------------------------
// Compute the camera world position
//-----------------------------------------------------------------------------
void CSimplePotteryWheelPanel::ComputeCameraTransform( matrix3x4_t *pWorldToCamera )
{
	// Set up the render state for the camera + light
	matrix3x4_t parent, offset;
	m_pCameraRootDag->GetTransform()->GetTransform( parent );
	SetIdentityMatrix( offset );
	if ( IsUsingEngineCoordinateSystem() )
	{
		MatrixSetColumn( Vector( -m_flDistance,0,0 ), 3, offset );
	}
	else
	{
		MatrixSetColumn( Vector( 0,0,m_flDistance ), 3, offset );
	}
	ConcatTransforms( parent, offset, *pWorldToCamera );
}


//-----------------------------------------------------------------------------
// Computes the position in the panel of a particular 3D point
//-----------------------------------------------------------------------------
void CSimplePotteryWheelPanel::ComputePanelPosition( const Vector &vecPosition, Vector2D *pPanelPos )
{ 
	int w, h;
	GetSize( w, h );
	m_pCamera->ComputeScreenSpacePosition( vecPosition, w, h, pPanelPos );
}



//-----------------------------------------------------------------------------
// paint it!
//-----------------------------------------------------------------------------
void CSimplePotteryWheelPanel::Paint()
{
	int iWidth, iHeight;
	GetSize( iWidth, iHeight );

	int screenw, screenh;
	vgui::surface()->GetScreenSize( screenw, screenh );

	int windowposx = 0, windowposy = 0;
	GetPos( windowposx, windowposy );

	int windowposright = windowposx + iWidth;
	int windowposbottom = windowposy + iHeight;

	if ( windowposright >= screenw )
	{
		iWidth -= ( windowposright - screenw );
	}
	if ( windowposbottom >= screenh )
	{
		iHeight -= ( windowposbottom - screenh );
	}

	int startx = 0, starty = 0;
	if( windowposx < 0 )
	{
		startx = -windowposx;
		iWidth -= startx;
	}
	if ( windowposy < 0 )
	{
		starty = -windowposy;
		iHeight -= starty;
	}

	int w, h;
	GetSize( w, h );
	vgui::MatSystemSurface()->Begin3DPaint( 0, 0, w, h );

	if ( m_pCurrentManip )
	{
		m_pCurrentManip->SetViewportSize( iWidth, iHeight );
	}

	// Set up the render state for the camera + light
	matrix3x4_t total;
	ComputeCameraTransform( &total );

	{
		CDisableUndoScopeGuard scope;
		m_pCamera->GetTransform()->SetTransform( total );
		m_pCamera->SetupRenderState( iWidth, iHeight, IsUsingEngineCoordinateSystem() );

		CMatRenderContextPtr pRenderContext( g_pMaterialSystem );
		int nLightCount = m_Lights.Count();
		LightDesc_t *pDesc = (LightDesc_t*)stackalloc( nLightCount * sizeof(LightDesc_t) );
		for ( int i = 0; i < nLightCount; ++i )
		{
			if ( m_Lights[i]->GetLightDesc( &pDesc[i] ) )
			{
				pRenderContext->SetLight( i, pDesc[i] );
			}
		}

		pRenderContext->SetAmbientLightCube( m_vecAmbientCube );
		g_pStudioRender->SetLocalLights( nLightCount, pDesc );
		g_pStudioRender->SetAmbientLightColors( m_vecAmbientCube );
	}

	CMatRenderContextPtr pRenderContext( vgui::MaterialSystem() );
	pRenderContext->ClearColor4ub( m_ClearColor.r(), m_ClearColor.g(), m_ClearColor.b(), m_ClearColor.a() ); 
	pRenderContext->ClearBuffers( true, true );

	if ( IsUsingEngineCoordinateSystem() )
	{
		pRenderContext->CullMode( MATERIAL_CULLMODE_CCW );
	}

	if ( HasLightProbe() )
	{
		const Vector &vecCameraOrigin = m_pCamera->GetTransform()->GetPosition();
		IMaterial *pMaterial = ( vgui::MaterialSystemHardwareConfig()->GetHDRType() == HDR_TYPE_NONE ) ?
			m_LightProbeBackground : m_LightProbeHDRBackground;

		RenderBox( vecCameraOrigin, vec3_angle, Vector( -100, -100, -100 ), Vector( 100, 100, 100 ),
			Color( 255, 255, 255, 255 ), pMaterial, true );
	}

	OnPaint3D();

	if ( IsUsingEngineCoordinateSystem() )
	{
		pRenderContext->CullMode( MATERIAL_CULLMODE_CW );
	}

	vgui::MatSystemSurface()->End3DPaint( );
}


//-----------------------------------------------------------------------------
// called when we're ticked...
//-----------------------------------------------------------------------------
void CSimplePotteryWheelPanel::OnTick()
{
	BaseClass::OnTick();
	if ( m_pCurrentManip )
	{
		m_pCurrentManip->OnTick();
	}
}


//-----------------------------------------------------------------------------
// input
//-----------------------------------------------------------------------------
void CSimplePotteryWheelPanel::OnKeyCodePressed(KeyCode code)
{
	if ( m_pCurrentManip )
	{
		switch( code )
		{
		case KEY_RSHIFT:
		case KEY_LSHIFT:
			// start translate mode
			AcceptManipulation( false );
			EnterManipulationMode( CAMERA_TRANSLATE, false );
			break;

		case KEY_RCONTROL:
		case KEY_LCONTROL:
			// start light mode
			AcceptManipulation( false );
			EnterManipulationMode( LIGHT_MODE, false );
			break;
		}
	}

	BaseClass::OnKeyCodePressed( code );
}



//-----------------------------------------------------------------------------
// Purpose: soaks up any remaining messages
//-----------------------------------------------------------------------------
void CSimplePotteryWheelPanel::OnKeyCodeReleased(KeyCode code)
{
	// disable translation mode
	if ( m_pCurrentManip )
	{
		switch( code )
		{
		case KEY_RSHIFT:
		case KEY_LSHIFT:
		case KEY_RCONTROL:
		case KEY_LCONTROL:
			// stop manipulation mode
			AcceptManipulation( false );
			EnterManipulationMode( CAMERA_ROTATE, false );
			break;
		}
	}
	BaseClass::OnKeyCodeReleased( code );
}

void CSimplePotteryWheelPanel::OnMousePressed( vgui::MouseCode code )
{
	if ( m_pCurrentManip )
		return;

	RequestFocus();

	switch ( code )
	{
		case MOUSE_LEFT:
			EnterManipulationMode( CAMERA_ROTATE, true, code );
			break;

		case MOUSE_MIDDLE:
			EnterManipulationMode( CAMERA_TRANSLATE, true, code );
			break;

		case MOUSE_RIGHT:
			EnterManipulationMode( CAMERA_ZOOM, true, code );
			break;
	}

	if ( m_pCurrentManip )
	{
		// Warp the mouse to the center of the screen
		int width, height;
		GetSize( width, height );
		int x = width / 2;
		int y = height / 2;

		int xpos = x;
		int ypos = y;
		LocalToScreen( xpos, ypos );
		input()->SetCursorPos( xpos, ypos );

		m_pCurrentManip->OnMousePressed( code, x, y );
	}

	BaseClass::OnMousePressed( code );
}

void CSimplePotteryWheelPanel::OnMouseReleased( vgui::MouseCode code )
{
	int x, y;
	input()->GetCursorPos( x, y );
	ScreenToLocal( x, y );

	AcceptManipulation();
	
	BaseClass::OnMouseReleased( code );
}

void CSimplePotteryWheelPanel::OnCursorMoved( int x, int y )
{
	if ( m_pCurrentManip )
	{
		if ( WarpMouse( x, y ) )
		{
			m_pCurrentManip->OnCursorMoved( x, y );
		}
	}

	BaseClass::OnCursorMoved( x, y );
}

void CSimplePotteryWheelPanel::OnMouseWheeled( int delta )
{
	if ( m_pCurrentManip )
	{
		m_pCurrentManip->OnMouseWheeled( delta );
	}

	BaseClass::OnMouseWheeled( delta );
}


void CSimplePotteryWheelPanel::EnterManipulationMode( ManipulationMode_t manipMode, bool bMouseCapture, vgui::MouseCode mouseCode /* = -1 */ )
{
	switch ( manipMode )
	{
	case CAMERA_ROTATE:
		m_pCurrentManip = m_pCameraRotate;
		break;

	case CAMERA_TRANSLATE:
		m_pCurrentManip = m_pCameraTranslate;
		break;

	case CAMERA_ZOOM:
		m_pCurrentManip = m_pCameraZoom;
		break;

	case LIGHT_MODE:
		m_pCurrentManip = m_pLightManip;
		break;
	}

	if ( m_pCurrentManip )
	{
		m_pCurrentManip->OnBeginManipulation();

		m_xoffset = m_yoffset = 0;

		if ( bMouseCapture )
		{
			input()->GetCursorPos( m_nManipStartX, m_nManipStartY );
			EnableMouseCapture( true, mouseCode );
		}
	}
}

void CSimplePotteryWheelPanel::AcceptManipulation( bool bReleaseMouseCapture )
{
	if ( m_pCurrentManip )
	{
		m_pCurrentManip->OnAcceptManipulation();

		if ( bReleaseMouseCapture )
		{
			EnableMouseCapture( false );
			input()->SetCursorPos( m_nManipStartX, m_nManipStartY );
		}

		m_pCurrentManip = NULL;
	}
}

void CSimplePotteryWheelPanel::CancelManipulation()
{
	if ( m_pCurrentManip )
	{
		m_pCurrentManip->OnCancelManipulation();

		EnableMouseCapture( false );
		input()->SetCursorPos( m_nManipStartX, m_nManipStartY );

		m_pCurrentManip = NULL;
	}
	
}

void CSimplePotteryWheelPanel::OnMouseCaptureLost()
{
	SetCursor( vgui::dc_arrow );
	m_nCaptureMouseCode = vgui::MouseCode( -1 );
}

void CSimplePotteryWheelPanel::EnableMouseCapture( bool enable, vgui::MouseCode mouseCode /* = -1 */ )
{
	if ( enable )
	{
		m_nCaptureMouseCode = mouseCode;
		SetCursor( vgui::dc_none );
		input()->SetMouseCaptureEx( GetVPanel(), m_nCaptureMouseCode );
	}
	else
	{
		m_nCaptureMouseCode = vgui::MouseCode( -1 );
		input()->SetMouseCapture( NULL );
		SetCursor( vgui::dc_arrow );
	}
}

bool CSimplePotteryWheelPanel::WarpMouse( int &x, int &y )
{
	// Re-force capture if it was lost...
	if ( input()->GetMouseCapture() != GetVPanel() )
	{
		input()->GetCursorPos( m_nManipStartX, m_nManipStartY );
		EnableMouseCapture( true, m_nCaptureMouseCode );
	}

	int width, height;
	GetSize( width, height );

	int centerx = width / 2;
	int centery = height / 2;

	// skip this event
	if ( x == centerx && y == centery )
		return false; 

	int xpos = centerx;
	int ypos = centery;
	LocalToScreen( xpos, ypos );
	input()->SetCursorPos( xpos, ypos );

	int dx = x - centerx;
	int dy = y - centery;

	x += m_xoffset;
	y += m_yoffset;

	m_xoffset += dx;
	m_yoffset += dy;

	return true;
}
