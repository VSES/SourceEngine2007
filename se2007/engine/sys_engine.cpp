//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: 
//
//===========================================================================//

#include "quakedef.h"
#include <assert.h>
#include "engine_launcher_api.h"
#include "iengine.h"
#include "ivideomode.h"
#include "igame.h"
#include "vmodes.h"
#include "modes.h"
#include "sys.h"
#include "host.h"
#include "keys.h"
#include "cdll_int.h"
#include "host_state.h"
#include "cdll_engine_int.h"
#include "sys_dll.h"
#include "tier0/vprof.h"
#include "profile.h"
#include "gl_matsysiface.h"
#include "vprof_engine.h"
#include "server.h"
#include "cl_demo.h"
#include "toolframework/itoolframework.h"
#include "toolframework/itoolsystem.h"
#include "inputsystem/iinputsystem.h"
#include "gl_cvars.h"
#include "filesystem_engine.h"
#ifndef SWDS
#include "vgui_baseui_interface.h"
#endif

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------
void Sys_ShutdownGame( void );
int Sys_InitGame( CreateInterfaceFn appSystemFactory, 
			char const* pBaseDir, void *pwnd, int bIsDedicated );

// sleep time when not focus
#define NOT_FOCUS_SLEEP	50				


//-----------------------------------------------------------------------------
// ConVars and ConCommands
//-----------------------------------------------------------------------------
static ConVar fps_max( "fps_max", ( IsX360() ) ? "30" : "300", 0, "Frame rate limiter" );
#ifndef _RETAIL
static ConVar async_serialize( "async_serialize", "0", 0, "Force async reads to serialize for profiling" );
#define ShouldSerializeAsync() async_serialize.GetBool()
#else
#define ShouldSerializeAsync() false
#endif



//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CEngine : public IEngine
{
public:
					CEngine( void );
	virtual			~CEngine( void );

	bool			Load( bool dedicated, const char *basedir );

	virtual void	Unload( void );
	virtual EngineState_t GetState( void );
	virtual void	SetNextState( EngineState_t iNextState );

	void			Frame( void );

	float			GetFrameTime( void );
	float			GetCurTime( void );
	
	bool			TrapKey_Event( ButtonCode_t key, bool down );
	void			TrapMouse_Event( int buttons, bool down );

	void			StartTrapMode( void );
	bool			IsTrapping( void );
	bool			CheckDoneTrapping( ButtonCode_t& key );

	int				GetQuitting( void );
	void			SetQuitting( int quittype );

private:
	bool			FilterTime( float t );

	int				m_nQuitting;

	EngineState_t	m_nDLLState;
	EngineState_t	m_nNextDLLState;

	double			m_flCurrentTime;
	float			m_flFrameTime;
	double			m_flPreviousTime;
	float			m_flFilteredTime;
};

static CEngine g_Engine;

IEngine *eng = ( IEngine * )&g_Engine;
//IEngineAPI *engine = NULL;


//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CEngine::CEngine( void )
{
	m_nDLLState			= DLL_INACTIVE;
	m_nNextDLLState		= DLL_INACTIVE;

	m_flCurrentTime		= 0.0;
	m_flFrameTime		= 0.0f;
	m_flPreviousTime	= 0.0;
	m_flFilteredTime	= 0.0f;

	m_nQuitting			= QUIT_NOTQUITTING;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CEngine::~CEngine( void )
{
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CEngine::Unload( void )
{
	Sys_ShutdownGame();

	m_nDLLState			= DLL_INACTIVE;
	m_nNextDLLState		= DLL_INACTIVE;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CEngine::Load( bool dedicated, const char *rootdir )
{
	bool success = false;

	// Activate engine
	// NOTE: We must bypass the 'next state' block here for initialization to work properly.
	m_nDLLState = m_nNextDLLState = InEditMode() ? DLL_PAUSED : DLL_ACTIVE;

	if ( Sys_InitGame( 
		g_AppSystemFactory,
		rootdir, 
		game->GetMainWindowAddress(), 
		dedicated ) )
	{
		success = true;

		UpdateMaterialSystemConfig();
	}

	return success;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : dt - 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CEngine::FilterTime( float dt )
{
	// Dedicated's tic_rate regulates server frame rate.  Don't apply fps filter here.
	// Only do this restriction on the client. Prevents clients from accomplishing certain
	// hacks by pausing their client for a period of time.
	if ( IsPC() && !sv.IsDedicated() && !CanCheat() && fps_max.GetFloat() < 30 )
	{
		// Don't do anything if fps_max=0 (which means it's unlimited).
		if ( fps_max.GetFloat() != 0.0f )
		{
			Warning( "sv_cheats is 0 and fps_max is being limited to a minimum of 30 (or set to 0).\n" );
			fps_max.SetValue( 30.0f );
		}
	}
	
	float fps = fps_max.GetFloat();
	if ( fps > 0.0f )
	{
		// Limit fps to withing tolerable range
//		fps = max( MIN_FPS, fps ); // red herring - since we're only checking if dt < 1/fps, clamping against MIN_FPS has no effect
		fps = min( MAX_FPS, fps );

		float minframetime = 1.0 / fps;

		if (
#if !defined(SWDS)
		    !demoplayer->IsPlayingTimeDemo() && 
#endif
			dt < minframetime )
		{
			// framerate is too high
			return false;		
		}
	}

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Output : int
//-----------------------------------------------------------------------------
void CEngine::Frame( void )
{
	// yield the CPU for a little while when paused, minimized, or not the focus
	// FIXME:  Move this to main windows message pump?
	if ( IsPC() && !game->IsActiveApp() && !sv.IsDedicated() )
	{
		g_pInputSystem->SleepUntilInput( NOT_FOCUS_SLEEP );
	}

	// Get current time
	m_flCurrentTime	= Sys_FloatTime();

	// Determine dt since we last checked
	float dt = m_flCurrentTime - m_flPreviousTime;

	// Remember old time
	m_flPreviousTime = m_flCurrentTime;

	// Accumulate current time delta into the true "frametime"
	m_flFrameTime += dt;

	// If the time is < 0, that means we've restarted. 
	// Set the new time high enough so the engine will run a frame
	if ( m_flFrameTime < 0.0f )
		return;

	// If the frametime is still too short, don't pass through
	if ( !FilterTime( m_flFrameTime ) )
	{
		m_flFilteredTime += dt;
		return;
	}

	if ( ShouldSerializeAsync() )
	{
		static ConVar *pSyncReportConVar = g_pCVar->FindVar( "fs_report_sync_opens" );
		bool bReportingSyncOpens = ( pSyncReportConVar && pSyncReportConVar->GetInt() );
		int reportLevel = 0;
		if ( bReportingSyncOpens )
		{
			reportLevel = pSyncReportConVar->GetInt();
			pSyncReportConVar->SetValue( 0 );
		}
		g_pFileSystem->AsyncFinishAll();
		if ( bReportingSyncOpens )
		{
			pSyncReportConVar->SetValue( reportLevel );
		}
	}

#ifdef VPROF_ENABLED
	PreUpdateProfile( m_flFilteredTime );
#endif
	
	// Reset swallowed time...
	m_flFilteredTime = 0.0f;

#ifndef SWDS
	if ( !sv.IsDedicated() )
	{
		ClientDLL_FrameStageNotify( FRAME_START );
	}
#endif

#ifdef VPROF_ENABLED
	PostUpdateProfile();
#endif

	{ // profile scope

	VPROF_BUDGET( "CEngine::Frame", VPROF_BUDGETGROUP_OTHER_UNACCOUNTED );


	switch( m_nDLLState )
	{
	case DLL_PAUSED:			// paused, in hammer
	case DLL_INACTIVE:			// no dll
		break;

	case DLL_ACTIVE:			// engine is focused
	case DLL_CLOSE:				// closing down dll
	case DLL_RESTART:			// engine is shutting down but will restart right away
		// Run the engine frame
		HostState_Frame( m_flFrameTime );
		break;
	}

	// Has the state changed?
	if ( m_nNextDLLState != m_nDLLState )
	{
		m_nDLLState = m_nNextDLLState;

		// Do special things if we change to particular states
		switch( m_nDLLState )
		{
		case DLL_CLOSE:
			SetQuitting( QUIT_TODESKTOP );
			break;
		case DLL_RESTART:
			SetQuitting( QUIT_RESTART );
			break;
		}
	}

	} // profile scope

	// Reset for next frame
	m_flFrameTime = 0.0f;

#if defined( VPROF_ENABLED ) && defined( _X360 )
	UpdateVXConsoleProfile();
#endif
}



//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CEngine::EngineState_t CEngine::GetState( void )
{
	return m_nDLLState;
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CEngine::SetNextState( EngineState_t iNextState )
{
	m_nNextDLLState = iNextState;
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
float CEngine::GetFrameTime( void )
{
	return m_flFrameTime;
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
float CEngine::GetCurTime( void )
{
	return m_flCurrentTime;
}


//-----------------------------------------------------------------------------
// Purpose: Flag that we are in the process of quiting
//-----------------------------------------------------------------------------
void CEngine::SetQuitting( int quittype )
{
	m_nQuitting = quittype;
}


//-----------------------------------------------------------------------------
// Purpose: Check whether we are ready to exit
//-----------------------------------------------------------------------------
int CEngine::GetQuitting( void )
{
	return m_nQuitting;
}
