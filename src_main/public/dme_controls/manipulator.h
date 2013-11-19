//====== Copyright © 1996-2004, Valve Corporation, All rights reserved. =======
//
// Purpose: 
//
//=============================================================================

#ifndef MANIPULATOR_H
#define MANIPULATOR_H
#ifdef _WIN32
#pragma once
#endif

#include "vgui_controls/Panel.h"
#include "mathlib/Vector.h"

class CDmAttribute;
class CDmeTransform;
class CDmeCamera;

class IManipulator
{
public:
	virtual void OnBeginManipulation( void ) = 0;
	virtual void OnAcceptManipulation( void ) = 0;
	virtual void OnCancelManipulation( void ) = 0;

	virtual void OnTick( void ) = 0;

	virtual void OnCursorMoved( int x, int y ) = 0;
	virtual void OnMousePressed( vgui::MouseCode code, int x, int y ) = 0;
	virtual void OnMouseReleased( vgui::MouseCode code, int x, int y ) = 0;
	virtual void OnMouseWheeled( int delta ) = 0;

	virtual void SetViewportSize( int w, int h ) = 0;
};

class CBaseManipulator : public IManipulator
{
public:
	CBaseManipulator()
	{
		m_nViewport[ 0 ] = m_nViewport[ 1 ]  = 0;
	}

	virtual void OnTick( void ) {};

	virtual void OnBeginManipulation( void ) {}
	virtual void OnAcceptManipulation( void ) {};
	virtual void OnCancelManipulation( void ) {};

	virtual void OnCursorMoved( int x, int y ) {};
	virtual void OnMousePressed( vgui::MouseCode code, int x, int y ) {};
	virtual void OnMouseReleased( vgui::MouseCode code, int x, int y ) {};
	virtual void OnMouseWheeled( int delta ) {};

	virtual void SetViewportSize( int w, int h )
	{
		m_nViewport[ 0 ] = w;
        m_nViewport[ 1 ] = h;
	}

protected:

	int			m_nViewport[ 2 ];
};

class CFloatAttributeManipulator : public CBaseManipulator
{
public:
	CFloatAttributeManipulator( CDmAttribute *pAttr );

	void SetAttribute( CDmAttribute *pAttr );
	CDmAttribute *GetAttribute();

	virtual void OnBeginManipulation( void );
	virtual void OnAcceptManipulation( void );
	virtual void OnCancelManipulation( void );

	virtual void OnTick( void );

	virtual void OnCursorMoved( int x, int y );
	virtual void OnMousePressed( vgui::MouseCode code, int x, int y );
	virtual void OnMouseReleased( vgui::MouseCode code, int x, int y );
	virtual void OnMouseWheeled( int delta );

	
protected:
	CDmAttribute *m_pAttr;

	float m_startingValue;

	int m_lastx, m_lasty;
};

class CTransformManipulator : public CBaseManipulator
{
public:
	CTransformManipulator( CDmeTransform *transform );

	void SetTransform( CDmeTransform *transform );
	CDmeTransform *GetTransform();

protected:
	CDmeTransform *m_pTransform;
};

class CPotteryWheelManip : public CTransformManipulator
{
public:
	CPotteryWheelManip( CDmeTransform *transform );

	virtual void OnBeginManipulation( void );
	virtual void OnAcceptManipulation( void );
	virtual void OnCancelManipulation( void );

	virtual void OnTick( void );

	virtual void OnCursorMoved( int x, int y );
	virtual void OnMousePressed( vgui::MouseCode code, int x, int y );
	virtual void OnMouseReleased( vgui::MouseCode code, int x, int y );
	virtual void OnMouseWheeled( int delta );

	// Sets the zoom level
	void SetZoom( float flZoom );

protected:
	int m_lastx, m_lasty;

	float m_zoom;
	float m_altitude, m_azimuth;
	//vec3 m_lookat

	float m_prevZoom;
	float m_prevAltitude, m_prevAzimuth;

	float m_flLastMouseTime;
	float m_flLastTickTime;
	float m_flSpin;
	bool m_bSpin;

	void UpdateTransform( void );
	void UpdateZoom( float delta );
};

class CTranslationManip : public CTransformManipulator
{
public:
	CTranslationManip( CDmeTransform *transform, CDmeCamera *camera );

	void SetCamera( CDmeCamera *camera );
	CDmeCamera *GetCamera( void );

	virtual void OnBeginManipulation( void );
	virtual void OnAcceptManipulation( void );
	virtual void OnCancelManipulation( void );

	virtual void OnTick( void );

	virtual void OnCursorMoved( int x, int y );
	virtual void OnMousePressed( vgui::MouseCode code, int x, int y );
	virtual void OnMouseReleased( vgui::MouseCode code, int x, int y );
	virtual void OnMouseWheeled( int delta );

protected:
	CDmeCamera *m_pCamera;

	int m_lastx, m_lasty;

	Vector m_offset;
	Vector m_prevOffset;

	void UpdateTransform( void );
};

#endif // MANIPULATOR_H
