//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: 
//
// $NoKeywords: $
//
//===========================================================================//
#include "cbase.h"
#include "hud.h"
#include "clientmode_csnormal.h"
#include "cdll_client_int.h"
#include "iinput.h"
#include "vgui/isurface.h"
#include "vgui/ipanel.h"
#include <vgui_controls/AnimationController.h>
#include "ivmodemanager.h"
#include "BuyMenu.h"
#include "filesystem.h"
#include "vgui/ivgui.h"
#include "hud_basechat.h"
#include "view_shared.h"
#include "view.h"
#include "ivrenderview.h"
#include "cstrikeclassmenu.h"
#include "model_types.h"
#include "iefx.h"
#include "dlight.h"
#include <imapoverview.h>
#include "c_playerresource.h"
#include "c_soundscape.h"
#include <keyvalues.h>
#include "text_message.h"
#include "panelmetaclassmgr.h"
#include "vguicenterprint.h"
#include "physpropclientside.h"
#include "c_weapon__stubs.h"
#include <engine/IEngineSound.h>
#include "c_cs_hostage.h"
#include "buy_presets/buy_presets.h"
#include "bitbuf.h"
#include "usermessages.h"
#include "prediction.h"
#include "datacache/imdlcache.h"

class CHudChat;

ConVar default_fov( "default_fov", "90", FCVAR_CHEAT );

IClientMode *g_pClientMode = NULL;

// This is a temporary entity used to render the player's model while drawing the class selection menu.
CHandle<C_BaseAnimatingOverlay> g_ClassImagePlayer;	// player
CHandle<C_BaseAnimating> g_ClassImageWeapon;	// weapon

STUB_WEAPON_CLASS( cycler_weapon,	WeaponCycler,	C_BaseCombatWeapon );
STUB_WEAPON_CLASS( weapon_cubemap,	WeaponCubemap,	C_BaseCombatWeapon );

//-----------------------------------------------------------------------------
// HACK: the detail sway convars are archive, and default to 0.  Existing CS:S players thus have no detail
// prop sway.  We'll force them to DoD's default values for now.  What we really need in the long run is
// a system to apply changes to archived convars' defaults to existing players.
extern ConVar cl_detail_max_sway;
extern ConVar cl_detail_avoid_radius;
extern ConVar cl_detail_avoid_force;
extern ConVar cl_detail_avoid_recover_speed;

//-----------------------------------------------------------------------------
ConVar cl_autobuy(
	"cl_autobuy",
	"",
	FCVAR_USERINFO,
	"The order in which autobuy will attempt to purchase items" );

//-----------------------------------------------------------------------------
ConVar cl_rebuy(
	"cl_rebuy",
	"",
	FCVAR_USERINFO,
	"The order in which rebuy will attempt to repurchase items" );

//-----------------------------------------------------------------------------
void SetBuyData( const ConVar &buyVar, const char *filename )
{
	// if we already have autobuy data, don't bother re-parsing the text file
	if ( *buyVar.GetString() )
		return;

	// read in the auto buy string file and construct the string we will send to the mp.dll.
	const char *pfile = (char*)UTIL_LoadFileForMe( filename, NULL );
	if (pfile == NULL)
	{
		return;
	}

	char token[256];
	char buystring[256];
	Q_snprintf( buystring, sizeof(buystring), "setinfo %s \"", buyVar.GetName() );

	pfile = engine->ParseFile( pfile, token, sizeof(token) );

	bool first = true;

	while (pfile != NULL)
	{
		if (first)
		{
			first = false;
		}
		else
		{
			Q_strncat(buystring, " ", sizeof(buystring), COPY_ALL_CHARACTERS);
		}

		Q_strncat(buystring, token, sizeof(buystring), COPY_ALL_CHARACTERS);

		pfile = engine->ParseFile( pfile, token, sizeof(token) );
	}

	UTIL_FreeFile((byte *)pfile);

	Q_strncat(buystring, "\"", sizeof(buystring), COPY_ALL_CHARACTERS);

	engine->ClientCmd(buystring);
}

void MsgFunc_KillCam(bf_read &msg) 
{
	C_CSPlayer *pPlayer = ToCSPlayer( C_BasePlayer::GetLocalPlayer() );

	if ( !pPlayer )
		return;

	int newMode = msg.ReadByte();

	if ( newMode != g_nKillCamMode )
	{
#if !defined( NO_ENTITY_PREDICTION )
		if ( g_nKillCamMode == OBS_MODE_NONE )
		{
			// kill cam is switch on, turn off prediction
			g_bForceCLPredictOff = true;
		}
		else if ( newMode == OBS_MODE_NONE )
		{
			// kill cam is switched off, restore old prediction setting is we switch back to normal mode
			g_bForceCLPredictOff = false;
		}
#endif
		g_nKillCamMode = newMode;
	}

	g_nKillCamTarget1	= msg.ReadByte();
	g_nKillCamTarget2	= msg.ReadByte();
}

// --------------------------------------------------------------------------------- //
// CCSModeManager.
// --------------------------------------------------------------------------------- //

class CCSModeManager : public IVModeManager
{
public:
	virtual void	Init();
	virtual void	SwitchMode( bool commander, bool force ) {}
	virtual void	LevelInit( const char *newmap );
	virtual void	LevelShutdown( void );
	virtual void	ActivateMouse( bool isactive ) {}
};

static CCSModeManager g_ModeManager;
IVModeManager *modemanager = ( IVModeManager * )&g_ModeManager;

// --------------------------------------------------------------------------------- //
// CCSModeManager implementation.
// --------------------------------------------------------------------------------- //

#define SCREEN_FILE		"scripts/vgui_screens.txt"

void CCSModeManager::Init()
{
	g_pClientMode = GetClientModeNormal();
	
	PanelMetaClassMgr()->LoadMetaClassDefinitionFile( SCREEN_FILE );
}

void CCSModeManager::LevelInit( const char *newmap )
{
	g_pClientMode->LevelInit( newmap );

	SetBuyData( cl_autobuy, "autobuy.txt" );
	SetBuyData( cl_rebuy, "rebuy.txt" );

#if !defined( NO_ENTITY_PREDICTION )
	if ( g_nKillCamMode > OBS_MODE_NONE )
	{
		g_bForceCLPredictOff = false;
	}
#endif

	g_nKillCamMode		= OBS_MODE_NONE;
	g_nKillCamTarget1	= 0;
	g_nKillCamTarget2	= 0;

	// HACK: the detail sway convars are archive, and default to 0.  Existing CS:S players thus have no detail
	// prop sway.  We'll force them to DoD's default values for now.
	if ( !cl_detail_max_sway.GetFloat() &&
		!cl_detail_avoid_radius.GetFloat() &&
		!cl_detail_avoid_force.GetFloat() &&
		!cl_detail_avoid_recover_speed.GetFloat() )
	{
		cl_detail_max_sway.SetValue( "5" );
		cl_detail_avoid_radius.SetValue( "64" );
		cl_detail_avoid_force.SetValue( "0.4" );
		cl_detail_avoid_recover_speed.SetValue( "0.25" );
	}
}

void CCSModeManager::LevelShutdown( void )
{
	g_pClientMode->LevelShutdown();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
ClientModeCSNormal::ClientModeCSNormal()
{
}

void ClientModeCSNormal::Init()
{
	BaseClass::Init();

	ListenForGameEvent( "round_end" );
	ListenForGameEvent( "round_start" );
	ListenForGameEvent( "player_team" );
	ListenForGameEvent( "player_death" );
	ListenForGameEvent( "bomb_planted" );
	ListenForGameEvent( "bomb_defused" );
	ListenForGameEvent( "hostage_killed" );
	ListenForGameEvent( "hostage_hurt" );
	ListenForGameEvent( "round_end_message" );

	usermessages->HookMessage( "KillCam", MsgFunc_KillCam );
}

void ClientModeCSNormal::InitViewport()
{
	BaseClass::InitViewport();

	m_pViewport = new CounterStrikeViewport();
	m_pViewport->Start( gameuifuncs, gameeventmanager );
}


void ClientModeCSNormal::Update()
{
	BaseClass::Update();

	// Override the hud's visibility if this is a logo (like E3 demo) map.
	if ( CSGameRules() && CSGameRules()->IsLogoMap() )
		m_pViewport->SetVisible( false );
}


/*
void ClientModeCSNormal::UpdateSpectatorMode( void )
{
	C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();

	if ( !pPlayer )
		return;

	IMapOverview * overviewmap = m_pViewport->GetMapOverviewInterface();

	if ( !overviewmap )
		return;

	overviewmap->SetTime( gpGlobals->curtime );

	int obs_mode = pPlayer->GetObserverMode();

	if ( obs_mode < OBS_MODE_IN_EYE )
		return;

	Vector worldpos = pPlayer->GetLocalOrigin();
	QAngle angles; engine->GetViewAngles( angles );

	C_BaseEntity *target = pPlayer->GetObserverTarget();

	if ( target && (obs_mode == OBS_MODE_IN_EYE || obs_mode == OBS_MODE_CHASE) )
	{
		worldpos = target->GetAbsOrigin();

		if ( obs_mode == OBS_MODE_IN_EYE )
		{
			angles = target->GetAbsAngles();
		}
	}

	Vector2D mappos = overviewmap->WorldToMap( worldpos );

	overviewmap->SetCenter( mappos );
	overviewmap->SetAngle( angles.y );	
	
	for ( int i = 1; i<= MAX_PLAYERS; i++)
	{
		C_BaseEntity *ent = ClientEntityList().GetEnt( i );

		if ( !ent || !ent->IsPlayer() )
			continue;

		C_BasePlayer *p = ToBasePlayer( ent );

		// update position of active players in our PVS
		Vector position = p->GetAbsOrigin();
		QAngle angle = p->GetAbsAngles();

		if ( p->IsDormant() )
		{
			// if player is not in PVS, use PlayerResources data
			position = g_PR->GetPosition( i );
			angles[1] = g_PR->GetViewAngle( i );
		}
		
		overviewmap->SetPlayerPositions( i-1, position, angles );
	}
} */

//-----------------------------------------------------------------------------
// Purpose: We've received a keypress from the engine. Return 1 if the engine is allowed to handle it.
//-----------------------------------------------------------------------------
int	ClientModeCSNormal::KeyInput( int down, ButtonCode_t keynum, const char *pszCurrentBinding )
{
	// don't process input in LogoMaps
	if( CSGameRules() && CSGameRules()->IsLogoMap() )
		return 1;
	
	return BaseClass::KeyInput( down, keynum, pszCurrentBinding );
}


ClientModeCSNormal g_ClientModeNormal;

IClientMode *GetClientModeNormal()
{
	return &g_ClientModeNormal;
}


ClientModeCSNormal* GetClientModeCSNormal()
{
	Assert( dynamic_cast< ClientModeCSNormal* >( GetClientModeNormal() ) );

	return static_cast< ClientModeCSNormal* >( GetClientModeNormal() );
}

float ClientModeCSNormal::GetViewModelFOV( void )
{
	return 74.0f;
}

int ClientModeCSNormal::GetDeathMessageStartHeight( void )
{
	return m_pViewport->GetDeathMessageStartHeight();
}

void ClientModeCSNormal::FireGameEvent( IGameEvent *event )
{
	CBaseHudChat *pHudChat = (CBaseHudChat *)GET_HUDELEMENT( CHudChat );
	C_BasePlayer *pLocalPlayer = C_BasePlayer::GetLocalPlayer();
	CLocalPlayerFilter filter;
	
	if ( !pLocalPlayer || !pHudChat )
		return;

	const char *eventname = event->GetName();

	if ( !eventname || !eventname[0] )
		return;

	if ( Q_strcmp( "round_start", eventname ) == 0 )
	{
		// recreate all client side physics props
		C_PhysPropClientside::RecreateAll();

		// remove hostage ragdolls
		for ( int i=0; i<g_HostageRagdolls.Count(); ++i )
		{
			// double-check that the EHANDLE is still valid
			if ( g_HostageRagdolls[i] )
			{
				g_HostageRagdolls[i]->Release();
			}
		}
		g_HostageRagdolls.RemoveAll();

		// Just tell engine to clear decals
		engine->ClientCmd( "r_cleardecals\n" );

		//stop any looping sounds
		enginesound->StopAllSounds( true );

		Soundscape_OnStopAllSounds();	// Tell the soundscape system.
	}


	else if ( Q_strcmp( "round_end", eventname ) == 0 )
	{
		int winningTeam = event->GetInt("winner");
		int reason = event->GetInt("reason");

		// play endround announcer sound
		if ( winningTeam == TEAM_CT )
		{
			if ( reason == Bomb_Defused )
			{
				C_BaseEntity::EmitSound( filter, SOUND_FROM_LOCAL_PLAYER, "Event.BombDefused");
			}
			else
			{
				C_BaseEntity::EmitSound( filter, SOUND_FROM_LOCAL_PLAYER, "Event.CTWin");
			}
		}
		else if ( winningTeam == TEAM_TERRORIST )
		{
			C_BaseEntity::EmitSound( filter, SOUND_FROM_LOCAL_PLAYER, "Event.TERWin");
		}
		else
		{
			C_BaseEntity::EmitSound( filter, SOUND_FROM_LOCAL_PLAYER, "Event.RoundDraw");
		}
		
		// show centerprint message 
		internalCenterPrint->Print( hudtextmessage->LookupString( event->GetString("message") ) );
	}

	else if ( Q_strcmp( "round_end_message", eventname ) == 0 )
	{
		int winningTeam = event->GetInt("winner");
		int reason = event->GetInt("reason");

		// play endround announcer sound
		if ( winningTeam == TEAM_CT )
		{
			if ( reason == Bomb_Defused )
			{
				C_BaseEntity::EmitSound( filter, SOUND_FROM_LOCAL_PLAYER, "Event.BombDefused");
			}
			else
			{
				C_BaseEntity::EmitSound( filter, SOUND_FROM_LOCAL_PLAYER, "Event.CTWin");
			}
		}
		else if ( winningTeam == TEAM_TERRORIST )
		{
			C_BaseEntity::EmitSound( filter, SOUND_FROM_LOCAL_PLAYER, "Event.TERWin");
		}
		else
		{
			C_BaseEntity::EmitSound( filter, SOUND_FROM_LOCAL_PLAYER, "Event.RoundDraw");
		}

		// show centerprint message 
		internalCenterPrint->Print( hudtextmessage->LookupString( event->GetString("message") ) );
	}

	else if ( Q_strcmp( "player_team", eventname ) == 0 )
	{
		CBaseHudChat *pHudChat = (CBaseHudChat *)GET_HUDELEMENT( CHudChat );
		C_BasePlayer *pPlayer = USERID2PLAYER( event->GetInt("userid") );
		
		if ( !pPlayer )
			return;

		bool bDisconnected = event->GetBool("disconnect");

		if ( bDisconnected )
			return;

		int iTeam = event->GetInt("team");

		if ( pPlayer->IsLocalPlayer() )
		{
			// that's me
			pPlayer->TeamChange( iTeam );
		}

		if ( iTeam == TEAM_SPECTATOR )
			pHudChat->Printf( CHAT_FILTER_NONE, hudtextmessage->LookupString( "#Game_join_spectators" ), pPlayer->GetPlayerName() );
		else if ( iTeam == TEAM_TERRORIST )
			pHudChat->Printf( CHAT_FILTER_NONE, hudtextmessage->LookupString( "#Game_join_terrorist" ), pPlayer->GetPlayerName() );
		else if ( iTeam == TEAM_CT )
			pHudChat->Printf( CHAT_FILTER_NONE, hudtextmessage->LookupString( "#Game_join_ct" ), pPlayer->GetPlayerName() );
	}

	else if ( Q_strcmp( "bomb_planted", eventname ) == 0 )
	{
		// C_BasePlayer *pPlayer = USERID2PLAYER( event->GetInt("userid") );

		// show centerprint message
		internalCenterPrint->Print( "#Cstrike_TitlesTXT_Bomb_Planted" );

		// play sound
		 C_BaseEntity::EmitSound( filter, SOUND_FROM_LOCAL_PLAYER, "Event.BombPlanted")  ;
	}

	else if ( Q_strcmp( "bomb_defused", eventname ) == 0 )
	{
		// C_BasePlayer *pPlayer = USERID2PLAYER( event->GetInt("userid") );
	}

	else if ( Q_strcmp( "hostage_killed", eventname ) == 0 )
	{
		// play sound for spectators and CTs
		if ( pLocalPlayer->IsObserver() || (pLocalPlayer->GetTeamNumber() == TEAM_CT) )
		{
			C_BaseEntity::EmitSound( filter, SOUND_FROM_LOCAL_PLAYER, "Event.HostageKilled")  ;
		}

		// Show warning to killer
		if ( pLocalPlayer->GetUserID() == event->GetInt("userid") )
		{
			internalCenterPrint->Print( "#Cstrike_TitlesTXT_Killed_Hostage" );
		}
	}

	else if ( Q_strcmp( "hostage_hurt", eventname ) == 0 )
	{
		// Let the loacl player know he harmed a hostage
		if ( pLocalPlayer->GetUserID() == event->GetInt("userid") )
		{
			internalCenterPrint->Print( "#Cstrike_TitlesTXT_Injured_Hostage" );
		}
	}

	else if ( Q_strcmp( "player_death", eventname ) == 0 )
	{
		C_BasePlayer *pPlayer = USERID2PLAYER( event->GetInt("userid") );
		
		if ( pPlayer == C_BasePlayer::GetLocalPlayer() )
		{
			// we just died, hide any buy panels
			gViewPortInterface->ShowPanel( PANEL_BUY, false );
			gViewPortInterface->ShowPanel( PANEL_BUY_CT, false );
			gViewPortInterface->ShowPanel( PANEL_BUY_TER, false );
			gViewPortInterface->ShowPanel( PANEL_BUY_EQUIP_CT, false );
			gViewPortInterface->ShowPanel( PANEL_BUY_EQUIP_TER, false );
		}
	}

	else if ( Q_strcmp( "player_changename", eventname ) == 0 )
	{
		return; // server sends a colorized text string for this
	}

	else
	{
		BaseClass::FireGameEvent( event );
	}
}


void RemoveClassImageEntity()
{
	C_BaseAnimating *pEnt = g_ClassImagePlayer.Get();
	if ( pEnt )
	{
		pEnt->Remove();
		g_ClassImagePlayer = NULL;
	}

	pEnt = g_ClassImageWeapon.Get();
	if ( pEnt )
	{
		pEnt->Remove();
		g_ClassImagePlayer = NULL;
	}
}


bool ShouldRecreateClassImageEntity( C_BaseAnimating *pEnt, const char *pNewModelName )
{
	if ( !pNewModelName || !pNewModelName[0] )
		return false;

	if ( !pEnt )
		return true;

	const model_t *pModel = pEnt->GetModel();

	if ( !pModel )
		return true;

	const char *pName = modelinfo->GetModelName( pModel );
	if ( !pName )
		return true;

	// reload only if names are different
	const char *pNameNoPath = V_UnqualifiedFileName( pName );
	const char *pNewModelNameNoPath = V_UnqualifiedFileName( pNewModelName );
	return( Q_stricmp( pNameNoPath, pNewModelNameNoPath ) != 0 );
}


void UpdateClassImageEntity( 
	const char *pModelName,
	int x, int y, int width, int height )
{
	C_BasePlayer *pLocalPlayer = C_BasePlayer::GetLocalPlayer();
	
	if ( !pLocalPlayer )
		return;

	MDLCACHE_CRITICAL_SECTION();

	const char *pWeaponName = "models/weapons/w_rif_ak47.mdl";
	const char *pWeaponSequence = "Walk_Upper_AK";

	int i;
	for ( i=0; i<CTPlayerModels.Count(); ++i )
	{
		if ( Q_strcasecmp( pModelName, CTPlayerModels[i] ) == 0 )
		{
			// give CTs a M4
			pWeaponName = "models/weapons/w_rif_m4a1.mdl";
			pWeaponSequence = "Walk_Upper_M4";
			break;
		}
	}

	if ( pLocalPlayer->IsAlive() && pLocalPlayer->GetActiveWeapon() )
	{
		C_WeaponCSBase *weapon = dynamic_cast< C_WeaponCSBase * >(pLocalPlayer->GetActiveWeapon());
		if ( weapon )
		{
			pWeaponName = weapon->GetWorldModel();
			pWeaponSequence = VarArgs("Walk_Upper_%s", weapon->GetCSWpnData().m_szAnimExtension);
		}
	}

	C_BaseAnimatingOverlay *pPlayerModel = g_ClassImagePlayer.Get();

	// Does the entity even exist yet?
	bool recreatePlayer = ShouldRecreateClassImageEntity( pPlayerModel, pModelName );
	if ( recreatePlayer )
	{
		if ( pPlayerModel )
			pPlayerModel->Remove();

		pPlayerModel = new C_BaseAnimatingOverlay;
		pPlayerModel->InitializeAsClientEntity( pModelName, RENDER_GROUP_OPAQUE_ENTITY );
		pPlayerModel->AddEffects( EF_NODRAW ); // don't let the renderer draw the model normally

		// let player walk ahead
		pPlayerModel->SetSequence( pPlayerModel->LookupSequence( "walk_lower" ) );
		pPlayerModel->SetPoseParameter( "move_yaw", 0.0f ); // move_yaw
		pPlayerModel->SetPoseParameter( "body_pitch", 10.0f ); // body_pitch, look down a bit
		pPlayerModel->SetPoseParameter( "body_yaw", 0.0f ); // body_yaw
		pPlayerModel->SetPoseParameter( "move_y", 0.0f ); // move_y
		pPlayerModel->SetPoseParameter( "move_x", 1.0f ); // move_x, walk forward
		pPlayerModel->m_flAnimTime = gpGlobals->curtime;

		g_ClassImagePlayer = pPlayerModel;
	}

	C_BaseAnimating *pWeaponModel = g_ClassImageWeapon.Get();

	// Does the entity even exist yet?
	if ( recreatePlayer || ShouldRecreateClassImageEntity( pWeaponModel, pWeaponName ) )
	{
		if ( pWeaponModel )
			pWeaponModel->Remove();

		pWeaponModel = new C_BaseAnimating;
		pWeaponModel->InitializeAsClientEntity( pWeaponName, RENDER_GROUP_OPAQUE_ENTITY );
		pWeaponModel->AddEffects( EF_NODRAW ); // don't let the renderer draw the model normally
		pWeaponModel->FollowEntity( pPlayerModel ); // attach to player model
		pWeaponModel->m_flAnimTime = gpGlobals->curtime;
		g_ClassImageWeapon = pWeaponModel;
	}

	Vector origin = pLocalPlayer->EyePosition();
	Vector lightOrigin = origin;

	// find a spot inside the world for the dlight's origin, or it won't illuminate the model
	Vector testPos( origin.x - 100, origin.y, origin.z + 100 );
	trace_t tr;
	UTIL_TraceLine( origin, testPos, MASK_OPAQUE, pLocalPlayer, COLLISION_GROUP_NONE, &tr );
	if ( tr.fraction == 1.0f )
	{
		lightOrigin = tr.endpos;
	}
	else
	{
		// Now move the model away so we get the correct illumination
		lightOrigin = tr.endpos + Vector( 1, 0, -1 );	// pull out from the solid
		Vector start = lightOrigin;
		Vector end = lightOrigin + Vector( 100, 0, -100 );
		UTIL_TraceLine( start, end, MASK_OPAQUE, pLocalPlayer, COLLISION_GROUP_NONE, &tr );
		origin = tr.endpos;
	}

	float ambient = engine->GetLightForPoint( origin, true ).Length();

	// Make a light so the model is well lit.
	dlight_t *dl = effects->CL_AllocDlight( LIGHT_INDEX_TE_DYNAMIC+1 );	// use a non-zero number so we cannibalize ourselves next frame

	dl->flags = DLIGHT_NO_WORLD_ILLUMINATION;
	dl->origin = lightOrigin;
	dl->die = gpGlobals->curtime + 0.1f; // Go away immediately so it doesn't light the world too.

	dl->color.r = dl->color.g = dl->color.b = 250;
	if ( ambient < 1.0f )
	{
        dl->color.exponent = 1 + (1 - ambient) * 2;
	}
	dl->radius	= 400;

	// move player model in front of our view
	pPlayerModel->SetAbsOrigin( origin );
	pPlayerModel->SetAbsAngles( QAngle( 0, 210, 0 ) );

	// wacky hacky, set upper body animation
	pPlayerModel->m_SequenceTransitioner.CheckForSequenceChange( 
		pPlayerModel->GetModelPtr(),
		pPlayerModel->LookupSequence( "walk_lower" ),
		false,
		true
	);
	pPlayerModel->m_SequenceTransitioner.UpdateCurrent( 
		pPlayerModel->GetModelPtr(),
		pPlayerModel->LookupSequence( "walk_lower" ),
		pPlayerModel->GetCycle(),
		pPlayerModel->GetPlaybackRate(),
		gpGlobals->realtime
	);

	// Now, blend the lower and upper (aim) anims together
	pPlayerModel->SetNumAnimOverlays( 2 );
	int numOverlays = pPlayerModel->GetNumAnimOverlays();
	for ( i=0; i < numOverlays; ++i )
	{
		C_AnimationLayer *layer = pPlayerModel->GetAnimOverlay( i );

		layer->m_flCycle = pPlayerModel->GetCycle();
		if ( i )
			layer->m_nSequence = pPlayerModel->LookupSequence( pWeaponSequence );
		else
			layer->m_nSequence = pPlayerModel->LookupSequence( "walk_lower" );

		layer->m_flPlaybackRate = 1.0;
		layer->m_flWeight = 1.0f;
		layer->SetOrder( i );
	}

	pPlayerModel->FrameAdvance( gpGlobals->frametime );

	// Now draw it.
	CViewSetup view;
	view.x = x;
	view.y = y;
	view.width = width;
	view.height = height;

	view.m_bOrtho = false;
	view.fov = 54;

	view.origin = origin + Vector( -110, -5, -5 );

	Vector vMins, vMaxs;
	pPlayerModel->C_BaseAnimating::GetRenderBounds( vMins, vMaxs );
	view.origin.z += ( vMins.z + vMaxs.z ) * 0.55f;

	view.angles.Init();
	view.zNear = VIEW_NEARZ;
	view.zFar = 1000;

	Frustum dummyFrustum;
	render->Push3DView( view, 0, NULL, dummyFrustum );

	pPlayerModel->DrawModel( STUDIO_RENDER );

	if ( pWeaponModel )
	{
		pWeaponModel->DrawModel( STUDIO_RENDER );
	}

	render->PopView( dummyFrustum );
}


bool WillPanelBeVisible( vgui::VPANEL hPanel )
{
	while ( hPanel )
	{
		if ( !vgui::ipanel()->IsVisible( hPanel ) )
			return false;

		hPanel = vgui::ipanel()->GetParent( hPanel );
	}
	return true;
}



void ClientModeCSNormal::PostRenderVGui()
{
	// If the team menu is up, then we will render the model of the character that is currently selected.
	for ( int i=0; i < g_ClassImagePanels.Count(); i++ )
	{
		CCSClassImagePanel *pPanel = g_ClassImagePanels[i];
		if ( WillPanelBeVisible( pPanel->GetVPanel() ) )
		{
			// Ok, we have a visible class image panel.
			int x, y, w, h;
			pPanel->GetBounds( x, y, w, h );
			pPanel->LocalToScreen( x, y );

			// Allow for the border.
			x += 3;
			y += 5;
			w -= 2;
			h -= 10;

			UpdateClassImageEntity( g_ClassImagePanels[i]->m_ModelName, x, y, w, h );
			return;
		}
	}
}

bool ClientModeCSNormal::ShouldDrawViewModel( void )
{
	C_CSPlayer *pPlayer = C_CSPlayer::GetLocalCSPlayer();
	
	if( pPlayer && pPlayer->GetFOV() != CSGameRules()->DefaultFOV() )
	{
		CWeaponCSBase *pWpn = pPlayer->GetActiveCSWeapon();

		if( pWpn && pWpn->HideViewModelWhenZoomed() )
		{
			return false;
		}
	}

	return BaseClass::ShouldDrawViewModel();
}


bool ClientModeCSNormal::CanRecordDemo( char *errorMsg, int length ) const
{
	C_CSPlayer *player = C_CSPlayer::GetLocalCSPlayer();
	if ( !player )
	{
		return true;
	}

	if ( !player->IsAlive() )
	{
		return true;
	}

	// don't start recording while flashed, as it would remove the flash
	if ( player->m_flFlashBangTime > gpGlobals->curtime )
	{
		Q_strncpy( errorMsg, "Cannot record demos while blind.", length );
		return false;
	}

	// don't start recording while smoke grenades are spewing smoke, as the existing smoke would be destroyed
	C_BaseEntityIterator it;
	C_BaseEntity *ent;
	while ( (ent = it.Next()) != NULL )
	{
		if ( Q_strcmp( ent->GetClassname(), "class C_ParticleSmokeGrenade" ) == 0 )
		{
			Q_strncpy( errorMsg, "Cannot record demos while a smoke grenade is active.", length );
			return false;
		}
	}

	return true;
}

