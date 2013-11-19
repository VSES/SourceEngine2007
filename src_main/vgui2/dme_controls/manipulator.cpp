//========= Copyright © 1996-2001, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

#include "dme_controls/manipulator.h"

#include "movieobjects/dmetransform.h"
#include "movieobjects/dmecamera.h"

#include "datamodel/dmattribute.h"

#include "materialsystem/imaterialsystem.h"

#include "vgui/ivgui.h"
#include "vgui/iinput.h"
#include "vgui/isystem.h"
#include "vgui/MouseCode.h"

#include "mathlib/vector.h"
#include "mathlib/vmatrix.h"
#include "mathlib/mathlib.h"

#include <float.h>

using namespace vgui;


///////////////////////////////////////////////////////////////////////////////
//
//	local helper functions
//
///////////////////////////////////////////////////////////////////////////////

float UpdateTime( float &flLastTime )
{
	float flTime = vgui::system()->GetFrameTime();
	float dt = flTime - flLastTime;
	flLastTime = flTime;
	return dt;
}

///////////////////////////////////////////////////////////////////////////////
//
//	CFloatAttributeManipulator float attribute manipulator class
//
///////////////////////////////////////////////////////////////////////////////

CFloatAttributeManipulator::CFloatAttributeManipulator( CDmAttribute *pAttr ) :
	m_pAttr( pAttr ), m_startingValue( 0.0f ),
	m_lastx( -1 ), m_lasty( -1 )
{
	if ( m_pAttr )
	{
		m_startingValue = m_pAttr->GetValue<float>();
	}
}

void CFloatAttributeManipulator::SetAttribute( CDmAttribute *pAttr )
{
	m_pAttr = pAttr;
	if ( m_pAttr )
	{
		m_startingValue = m_pAttr->GetValue<float>();
	}
}

CDmAttribute *CFloatAttributeManipulator::GetAttribute()
{
	return m_pAttr;
}

void CFloatAttributeManipulator::OnBeginManipulation( void )
{
	if ( m_pAttr )
	{
		m_startingValue = m_pAttr->GetValue<float>();
	}
}

void CFloatAttributeManipulator::OnAcceptManipulation( void )
{
}

void CFloatAttributeManipulator::OnCancelManipulation( void )
{
	if ( m_pAttr )
	{
		m_pAttr->SetValue( m_startingValue );
	}
}

void CFloatAttributeManipulator::OnTick( void )
{
}

void CFloatAttributeManipulator::OnCursorMoved( int x, int y )
{
//	int dy = y - m_lasty;
	int dx = x - m_lastx;

	if ( m_pAttr )
	{
		float value = m_pAttr->GetValue<float>();
		value = max( 0.0f, min( 1.0f, value + 0.0005f * dx ) );
		m_pAttr->SetValue( value );
	}

	m_lastx = x;
	m_lasty = y;
}

void CFloatAttributeManipulator::OnMousePressed( vgui::MouseCode code, int x, int y )
{
	m_lastx = x;
	m_lasty = y;
}

void CFloatAttributeManipulator::OnMouseReleased( vgui::MouseCode code, int x, int y )
{
	m_lastx = x;
	m_lasty = y;
}

void CFloatAttributeManipulator::OnMouseWheeled( int delta )
{
	if ( m_pAttr )
	{
		float value = m_pAttr->GetValue<float>();
		value = max( 0.0f, min( 1.0f, value + 0.1f * delta ) );
		m_pAttr->SetValue( value );
	}
}


///////////////////////////////////////////////////////////////////////////////
//
//	CTransformManipulator transform manipulator base class
//
///////////////////////////////////////////////////////////////////////////////

CTransformManipulator::CTransformManipulator( CDmeTransform *transform ) :
	m_pTransform( transform )
{
}

void CTransformManipulator::SetTransform( CDmeTransform *transform )
{
	m_pTransform = transform;
}

CDmeTransform *CTransformManipulator::GetTransform( void )
{
	return m_pTransform;
}


///////////////////////////////////////////////////////////////////////////////
//
//	CPotteryWheelManip - nendo-style camera manipulator
//
///////////////////////////////////////////////////////////////////////////////

CPotteryWheelManip::CPotteryWheelManip( CDmeTransform *transform ) :
	CTransformManipulator( transform ),
	m_lastx( -1 ), m_lasty( -1 ),
	m_zoom( 100.0f ), m_altitude( 0.0f ), m_azimuth( 0.0f ),
	m_prevZoom( 100.0f ), m_prevAltitude( 0.0f ), m_prevAzimuth( 0.0f ),
	m_flLastMouseTime( 0.0f ), m_flLastTickTime( 0.0f ),
	m_flSpin( 0.0f ), m_bSpin( false )
{
}

void CPotteryWheelManip::OnBeginManipulation( void )
{
	m_prevZoom = m_zoom;
	m_prevAltitude = m_altitude;
	m_prevAzimuth = m_azimuth;
	m_flLastMouseTime = m_flLastTickTime = vgui::system()->GetFrameTime();
	m_flSpin = 0.0f;
	m_bSpin = false;
}

// Sets the zoom level
void CPotteryWheelManip::SetZoom( float flZoom )
{
	m_prevZoom = m_zoom = flZoom;
}


void CPotteryWheelManip::OnAcceptManipulation( void )
{
	m_flSpin = 0.0f;
	m_bSpin = false;
}

void CPotteryWheelManip::OnCancelManipulation( void )
{
	m_zoom = m_prevZoom;
	m_altitude = m_prevAltitude;
	m_azimuth = m_prevAzimuth;
	m_flSpin = 0.0f;
	m_bSpin = false;
	UpdateTransform();
}


void CPotteryWheelManip::OnTick( void )
{
	float dt = UpdateTime( m_flLastTickTime );

	if ( m_bSpin )
	{
		m_azimuth += dt * m_flSpin;
		UpdateTransform();
	}
}

void CPotteryWheelManip::OnCursorMoved( int x, int y )
{
	float dt = UpdateTime( m_flLastMouseTime );

	if ( m_bSpin )
	{
		m_lastx = x;
		m_lasty = y;
		return;
	}

	if ( input()->IsMouseDown( MOUSE_MIDDLE ) )
	{
		int dy = y - m_lasty;
		int dx = x - m_lastx;

		if ( abs( dx ) < 2 * abs( dy ) )
		{
			UpdateZoom( 0.2f * dy );
		}
		else
		{
			m_flSpin = (dt != 0.0f) ? 0.002f * dx / dt : 0.0f;
			m_azimuth  += 0.002f * dx;
		}
	}
	else
	{
		m_azimuth  += 0.002f * ( x - m_lastx );
		m_altitude -= 0.002f * ( y - m_lasty );
		m_altitude = max( -M_PI/2, min( M_PI/2, m_altitude ) );
	}
	m_lastx = x;
	m_lasty = y;

	UpdateTransform();
}

void CPotteryWheelManip::OnMousePressed( vgui::MouseCode code, int x, int y )
{
	UpdateTime( m_flLastMouseTime );
	m_lastx = x;
	m_lasty = y;
	m_bSpin = false;
	m_flSpin = 0.0f;
}

void CPotteryWheelManip::OnMouseReleased( vgui::MouseCode code, int x, int y )
{
	UpdateTime( m_flLastMouseTime );

	if ( code == MOUSE_MIDDLE )
	{
		m_bSpin = ( fabs( m_flSpin ) > 1.0f );
	}

	m_lastx = x;
	m_lasty = y;
}

void CPotteryWheelManip::OnMouseWheeled( int delta )
{
	UpdateTime( m_flLastMouseTime );

	UpdateZoom( -10.0f * delta );
	UpdateTransform();
}

void CPotteryWheelManip::UpdateTransform()
{
	if ( !m_pTransform )
		return;

	float y = m_zoom * sin( m_altitude );
	float xz = m_zoom * cos( m_altitude );
	float x = xz * sin( m_azimuth );
	float z = xz * cos( m_azimuth );

	Vector position(x, y, z);

	CDisableUndoScopeGuard guard;
	Quaternion orientation( RadianEuler( -m_altitude, m_azimuth, 0 ) );
	m_pTransform->SetValue( "position", position );
	m_pTransform->SetValue( "orientation", orientation );
}


void CPotteryWheelManip::UpdateZoom( float delta )
{
	m_zoom *= pow( 1.01f, delta );
}


///////////////////////////////////////////////////////////////////////////////
//
//	CTranslationManip - implicit (no UI) translation manipulator
//
///////////////////////////////////////////////////////////////////////////////

CTranslationManip::CTranslationManip( CDmeTransform *transform, CDmeCamera *camera ) :
	CTransformManipulator( transform ), m_pCamera( camera ),
	m_lastx( -1 ), m_lasty( -1 ),
	m_offset( vec3_origin ), m_prevOffset( vec3_origin )
{
}

void CTranslationManip::SetCamera( CDmeCamera *camera )
{
	m_pCamera = camera;
}

CDmeCamera *CTranslationManip::GetCamera( void )
{
	return m_pCamera;
}

void CTranslationManip::OnBeginManipulation( void )
{
	if ( !m_pTransform )
		return;

	m_prevOffset = m_pTransform->GetValue<Vector>( "position" );
	m_offset = m_prevOffset;
}

void CTranslationManip::OnAcceptManipulation( void )
{
}

void CTranslationManip::OnCancelManipulation( void )
{
	m_offset = m_prevOffset;
	UpdateTransform();
}

void CTranslationManip::OnTick( void )
{
}

void CTranslationManip::OnCursorMoved( int x, int y )
{
	int dy = y - m_lasty;
	int dx = x - m_lastx;

	VMatrix viewprojinv;
	m_pCamera->GetViewProjectionInverse( viewprojinv, m_nViewport[ 0 ], m_nViewport[ 1 ] );

	Vector right, up, forward;
	MatrixGetColumn( viewprojinv, 0, &right );
	MatrixGetColumn( viewprojinv, 1, &up );
	MatrixGetColumn( viewprojinv, 2, &forward );
	Vector zoom( viewprojinv[0][3], viewprojinv[1][3], viewprojinv[2][3] );
	float speed = zoom.Length() / viewprojinv[3][3];

	float invWidth = 1.0f / 1024;
	float invHeight = 1.0f / 768;

	if ( input()->IsMouseDown( MOUSE_MIDDLE ) )
	{
		m_offset -= ( 2 * dy * invHeight ) * forward;
	}
	else
	{
		m_offset += speed * ( 2 * dx * invWidth ) * right;
		m_offset -= speed * ( 2 * dy * invHeight ) * up;
	}
	m_lastx = x;
	m_lasty = y;

	UpdateTransform();
}

void CTranslationManip::OnMousePressed( vgui::MouseCode code, int x, int y )
{
	m_lastx = x;
	m_lasty = y;
}

void CTranslationManip::OnMouseReleased( vgui::MouseCode code, int x, int y )
{
	m_lastx = x;
	m_lasty = y;
}

void CTranslationManip::OnMouseWheeled( int delta )
{
	VMatrix viewprojinv;
	m_pCamera->GetViewProjectionInverse( viewprojinv, m_nViewport[ 0 ], m_nViewport[ 1 ] );

	Vector forward;
	MatrixGetColumn( viewprojinv, 2, &forward );
	Vector zoom( viewprojinv[0][3], viewprojinv[1][3], viewprojinv[2][3] );
	float speed = zoom.Length() / viewprojinv[3][3];

	m_offset += 0.001f * speed * delta * forward;

	UpdateTransform();
}

void CTranslationManip::UpdateTransform()
{
	if ( !m_pTransform )
		return;

	m_pTransform->SetValue( "position", m_offset );
}
