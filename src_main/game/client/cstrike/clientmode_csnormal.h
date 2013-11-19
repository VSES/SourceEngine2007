//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#ifndef CS_CLIENTMODE_H
#define CS_CLIENTMODE_H
#ifdef _WIN32
#pragma once
#endif

#include "clientmode_shared.h"
#include "counterstrikeviewport.h"

class ClientModeCSNormal : public ClientModeShared 
{
DECLARE_CLASS( ClientModeCSNormal, ClientModeShared );

private:

// IClientMode overrides.
public:

					ClientModeCSNormal();

	virtual void	Init();
	virtual void	InitViewport();
	virtual void	Update();

	virtual int		KeyInput( int down, ButtonCode_t keynum, const char *pszCurrentBinding );

	virtual float	GetViewModelFOV( void );

	int				GetDeathMessageStartHeight( void );

	virtual void	FireGameEvent( IGameEvent *event );
	virtual void	PostRenderVGui();

	virtual bool	ShouldDrawViewModel( void );

	virtual bool	CanRecordDemo( char *errorMsg, int length ) const;

private:
	
	//	void	UpdateSpectatorMode( void );

};


extern IClientMode *GetClientModeNormal();
extern ClientModeCSNormal* GetClientModeCSNormal();


#endif // CS_CLIENTMODE_H
