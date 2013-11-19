//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//
#include "cbase.h"
#include "dt_utlvector_recv.h"
#include "bone_setup.h"
#include "c_ai_basenpc.h"
#include "engine/IVDebugOverlay.h"

#include "IVRenderView.h"
#include "view_shared.h"
#include "iviewrender.h"

#include "tier0/vprof.h"
#include "soundinfo.h"

// TODO: These should be in public by the time the SDK ships
#if 1
	#include "../../common/blobulator/Implicit/ImpDefines.h"
	#include "../../common/blobulator/Implicit/ImpRenderer.h"
	#include "../../common/blobulator/Implicit/ImpTiler.h"
	#include "../../common/blobulator/Implicit/UserFunctions.h"
#else
	#include "../common/blobulator/Implicit/ImpDefines.h"
	#include "../../common/blobulator/Implicit/SweepRenderer2.h"
	#include "../../common/blobulator/Implicit/ImpTiler2.h"
	#include "../../common/blobulator/Implicit/UserFunctions2.h"
#endif
// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class C_NPC_Surface : public C_AI_BaseNPC
{
public:
	DECLARE_CLASS( C_NPC_Surface, C_AI_BaseNPC );
	DECLARE_CLIENTCLASS();
	DECLARE_INTERPOLATION();

					C_NPC_Surface();
	virtual			~C_NPC_Surface();

	// model specific
	virtual void GetRenderBounds( Vector& theMins, Vector& theMaxs );
	virtual bool IsTransparent( void );
	ShadowType_t	ShadowCastType() { return SHADOWS_NONE; }
	bool UsesPowerOfTwoFrameBufferTexture( void );
	bool UsesFullFrameBufferTexture( void );
	virtual int DrawModel( int flags );

	virtual bool	GetSoundSpatialization( SpatializationInfo_t& info );

#define	MAX_SURFACE_ELEMENTS 1000 //200

	IMaterial *m_pMaterial;

	CUtlVector< Vector	> m_vecSurfacePos;
	CUtlVector< CInterpolatedVar< Vector > > m_iv_vecSurfacePos;

	CUtlVector< float > m_flSurfaceV;
	CUtlVector< CInterpolatedVar< float > > m_iv_flSurfaceV;

	CUtlVector< float > m_flSurfaceR;
	CUtlVector< CInterpolatedVar< float > > m_iv_flSurfaceR;

	int				m_nActiveParticles;
	float			m_flRadius;

private:
	C_NPC_Surface( const C_NPC_Surface & ); // not defined, not accessible
};


//-----------------------------------------------------------------------------
// Purpose: setup network receive table
//-----------------------------------------------------------------------------

IMPLEMENT_CLIENTCLASS_DT(C_NPC_Surface, DT_NPC_Surface, CNPC_Surface)
	RecvPropFloat	( RECVINFO( m_flRadius ) ),
	RecvPropInt		( RECVINFO( m_nActiveParticles ) ),
	RecvPropUtlVector( 
		RECVINFO_UTLVECTOR( m_vecSurfacePos ), 
		MAX_SURFACE_ELEMENTS,
		RecvPropVector(NULL, 0, sizeof( Vector ))),
	RecvPropUtlVector( 
		RECVINFO_UTLVECTOR( m_flSurfaceV ), 
		MAX_SURFACE_ELEMENTS,
		RecvPropFloat(NULL, 0, sizeof( float ))),
	RecvPropUtlVector( 
		RECVINFO_UTLVECTOR( m_flSurfaceR ), 
		MAX_SURFACE_ELEMENTS,
		RecvPropFloat(NULL, 0, sizeof( float ))),
END_RECV_TABLE()

//-----------------------------------------------------------------------------
// Purpose: link networked elements to local data
//-----------------------------------------------------------------------------

C_NPC_Surface::C_NPC_Surface()
{
	m_pMaterial = NULL;

	m_vecSurfacePos.EnsureCount( MAX_SURFACE_ELEMENTS );
	m_iv_vecSurfacePos.EnsureCount( MAX_SURFACE_ELEMENTS );

	m_flSurfaceV.EnsureCount( MAX_SURFACE_ELEMENTS );
	m_iv_flSurfaceV.EnsureCount( MAX_SURFACE_ELEMENTS );

	m_flSurfaceR.EnsureCount( MAX_SURFACE_ELEMENTS );
	m_iv_flSurfaceR.EnsureCount( MAX_SURFACE_ELEMENTS );

	for (int i = 0; i < MAX_SURFACE_ELEMENTS; i++)
	{
		IInterpolatedVar *pWatcher = &m_iv_vecSurfacePos.Element( i );
		pWatcher->SetDebugName( "m_iv_vecSurfacePos" );
		AddVar( &m_vecSurfacePos.Element( i ), pWatcher, LATCH_ANIMATION_VAR );

		pWatcher = &m_iv_flSurfaceV.Element( i );
		pWatcher->SetDebugName( "m_iv_flSurfaceV" );
		AddVar( &m_flSurfaceV.Element( i ), pWatcher, LATCH_ANIMATION_VAR );

		pWatcher = &m_iv_flSurfaceR.Element( i );
		pWatcher->SetDebugName( "m_iv_flSurfaceR" );
		AddVar( &m_flSurfaceR.Element( i ), pWatcher, LATCH_ANIMATION_VAR );
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------

C_NPC_Surface::~C_NPC_Surface()
{
}


//-----------------------------------------------------------------------------
// Purpose: Draw a Sphere
//-----------------------------------------------------------------------------

float g_FastSpherePosData[51][8] = {
{  0.0000,  0.0000,  1.0000,  0.0000,  0.0000,  0.0000,  0.0000,  1.0000 },
{ -0.0000,  0.5000,  0.8660,  0.2500,  0.1667, -0.0000,  0.5000,  0.8660 },
{  0.5000,  0.0000,  0.8660,  0.0000,  0.1667,  0.5000,  0.0000,  0.8660 },
{  0.0000,  0.0000,  1.0000,  0.5000,  0.0000,  0.0000,  0.0000,  1.0000 },
{ -0.5000, -0.0000,  0.8660,  0.5000,  0.1667, -0.5000, -0.0000,  0.8660 },
{  0.0000,  0.0000, -1.0000,  0.0000,  1.0000,  0.0000,  0.0000, -1.0000 },
{  0.5000,  0.0000, -0.8660,  0.0000,  0.8333,  0.5000,  0.0000, -0.8660 },
{ -0.0000,  0.5000, -0.8660,  0.2500,  0.8333, -0.0000,  0.5000, -0.8660 },
{  0.0000,  0.0000, -1.0000,  0.5000,  1.0000,  0.0000,  0.0000, -1.0000 },
{ -0.5000, -0.0000, -0.8660,  0.5000,  0.8333, -0.5000, -0.0000, -0.8660 },
{  0.0000, -0.5000,  0.8660,  0.7500,  0.1667,  0.0000, -0.5000,  0.8660 },
{  0.0000,  0.0000,  1.0000,  1.0000,  0.0000,  0.0000,  0.0000,  1.0000 },
{  0.5000,  0.0000,  0.8660,  1.0000,  0.1667,  0.5000,  0.0000,  0.8660 },
{  0.0000, -0.5000, -0.8660,  0.7500,  0.8333,  0.0000, -0.5000, -0.8660 },
{  0.0000,  0.0000, -1.0000,  1.0000,  1.0000,  0.0000,  0.0000, -1.0000 },
{  0.5000,  0.0000, -0.8660,  1.0000,  0.8333,  0.5000,  0.0000, -0.8660 },
{  0.6124,  0.6124,  0.5000,  0.1250,  0.3333,  0.6124,  0.6124,  0.5000 },
{  0.8660,  0.0000,  0.5000,  0.0000,  0.3333,  0.8660,  0.0000,  0.5000 },
{ -0.0000,  0.8660,  0.5000,  0.2500,  0.3333, -0.0000,  0.8660,  0.5000 },
{  0.8660,  0.0000, -0.5000,  0.0000,  0.6667,  0.8660,  0.0000, -0.5000 },
{  0.6124,  0.6124, -0.5000,  0.1250,  0.6667,  0.6124,  0.6124, -0.5000 },
{ -0.0000,  0.8660, -0.5000,  0.2500,  0.6667, -0.0000,  0.8660, -0.5000 },
{ -0.6124,  0.6124,  0.5000,  0.3750,  0.3333, -0.6124,  0.6124,  0.5000 },
{ -0.8660, -0.0000,  0.5000,  0.5000,  0.3333, -0.8660, -0.0000,  0.5000 },
{ -0.6124,  0.6124, -0.5000,  0.3750,  0.6667, -0.6124,  0.6124, -0.5000 },
{ -0.8660, -0.0000, -0.5000,  0.5000,  0.6667, -0.8660, -0.0000, -0.5000 },
{ -0.6124, -0.6124,  0.5000,  0.6250,  0.3333, -0.6124, -0.6124,  0.5000 },
{  0.0000, -0.8660,  0.5000,  0.7500,  0.3333,  0.0000, -0.8660,  0.5000 },
{ -0.6124, -0.6124, -0.5000,  0.6250,  0.6667, -0.6124, -0.6124, -0.5000 },
{  0.0000, -0.8660, -0.5000,  0.7500,  0.6667,  0.0000, -0.8660, -0.5000 },
{  0.6124, -0.6124,  0.5000,  0.8750,  0.3333,  0.6124, -0.6124,  0.5000 },
{  0.8660,  0.0000,  0.5000,  1.0000,  0.3333,  0.8660,  0.0000,  0.5000 },
{  0.6124, -0.6124, -0.5000,  0.8750,  0.6667,  0.6124, -0.6124, -0.5000 },
{  0.8660,  0.0000, -0.5000,  1.0000,  0.6667,  0.8660,  0.0000, -0.5000 },
{  0.9239,  0.3827,  0.0000,  0.0625,  0.5000,  0.9239,  0.3827,  0.0000 },
{  1.0000,  0.0000,  0.0000,  0.0000,  0.5000,  1.0000,  0.0000,  0.0000 },
{  0.7071,  0.7071,  0.0000,  0.1250,  0.5000,  0.7071,  0.7071,  0.0000 },
{  0.3827,  0.9239,  0.0000,  0.1875,  0.5000,  0.3827,  0.9239,  0.0000 },
{ -0.0000,  1.0000,  0.0000,  0.2500,  0.5000, -0.0000,  1.0000,  0.0000 },
{ -0.3827,  0.9239,  0.0000,  0.3125,  0.5000, -0.3827,  0.9239,  0.0000 },
{ -0.7071,  0.7071,  0.0000,  0.3750,  0.5000, -0.7071,  0.7071,  0.0000 },
{ -0.9239,  0.3827,  0.0000,  0.4375,  0.5000, -0.9239,  0.3827,  0.0000 },
{ -1.0000, -0.0000,  0.0000,  0.5000,  0.5000, -1.0000, -0.0000,  0.0000 },
{ -0.9239, -0.3827,  0.0000,  0.5625,  0.5000, -0.9239, -0.3827,  0.0000 },
{ -0.7071, -0.7071,  0.0000,  0.6250,  0.5000, -0.7071, -0.7071,  0.0000 },
{ -0.3827, -0.9239,  0.0000,  0.6875,  0.5000, -0.3827, -0.9239,  0.0000 },
{  0.0000, -1.0000,  0.0000,  0.7500,  0.5000,  0.0000, -1.0000,  0.0000 },
{  0.3827, -0.9239,  0.0000,  0.8125,  0.5000,  0.3827, -0.9239,  0.0000 },
{  0.7071, -0.7071,  0.0000,  0.8750,  0.5000,  0.7071, -0.7071,  0.0000 },
{  0.9239, -0.3827,  0.0000,  0.9375,  0.5000,  0.9239, -0.3827,  0.0000 },
{  1.0000,  0.0000,  0.0000,  1.0000,  0.5000,  1.0000,  0.0000,  0.0000 }
};


int g_FastSphereTriData[84][3] = {
{ 0, 1, 2 },
{ 0, 3, 1 },
{ 3, 4, 1 },
{ 5, 6, 7 },
{ 5, 7, 8 },
{ 8, 7, 9 },
{ 3, 10, 4 },
{ 3, 11, 10 },
{ 11, 12, 10 },
{ 8, 9, 13 },
{ 8, 13, 14 },
{ 14, 13, 15 },
{ 2, 16, 17 },
{ 2, 1, 16 },
{ 1, 18, 16 },
{ 6, 19, 20 },
{ 6, 20, 7 },
{ 7, 20, 21 },
{ 1, 22, 18 },
{ 1, 4, 22 },
{ 4, 23, 22 },
{ 7, 21, 24 },
{ 7, 24, 9 },
{ 9, 24, 25 },
{ 4, 26, 23 },
{ 4, 10, 26 },
{ 10, 27, 26 },
{ 9, 25, 28 },
{ 9, 28, 13 },
{ 13, 28, 29 },
{ 10, 30, 27 },
{ 10, 12, 30 },
{ 12, 31, 30 },
{ 13, 29, 32 },
{ 13, 32, 15 },
{ 15, 32, 33 },
{ 17, 34, 35 },
{ 17, 16, 34 },
{ 16, 36, 34 },
{ 19, 35, 34 },
{ 19, 34, 20 },
{ 20, 34, 36 },
{ 16, 37, 36 },
{ 16, 18, 37 },
{ 18, 38, 37 },
{ 20, 36, 37 },
{ 20, 37, 21 },
{ 21, 37, 38 },
{ 18, 39, 38 },
{ 18, 22, 39 },
{ 22, 40, 39 },
{ 21, 38, 39 },
{ 21, 39, 24 },
{ 24, 39, 40 },
{ 22, 41, 40 },
{ 22, 23, 41 },
{ 23, 42, 41 },
{ 24, 40, 41 },
{ 24, 41, 25 },
{ 25, 41, 42 },
{ 23, 43, 42 },
{ 23, 26, 43 },
{ 26, 44, 43 },
{ 25, 42, 43 },
{ 25, 43, 28 },
{ 28, 43, 44 },
{ 26, 45, 44 },
{ 26, 27, 45 },
{ 27, 46, 45 },
{ 28, 44, 45 },
{ 28, 45, 29 },
{ 29, 45, 46 },
{ 27, 47, 46 },
{ 27, 30, 47 },
{ 30, 48, 47 },
{ 29, 46, 47 },
{ 29, 47, 32 },
{ 32, 47, 48 },
{ 30, 49, 48 },
{ 30, 31, 49 },
{ 31, 50, 49 },
{ 32, 48, 49 },
{ 32, 49, 33 },
{ 33, 49, 50 }
};


void DrawFastSphere( CMeshBuilder &meshBuilder, const Vector &center, float radius, int r, int g, int b )
{
	int i;

	int offset = meshBuilder.GetCurrentVertex();

	Vector pos;
	for (i = 0; i < 51; i++)
	{
		pos.x = g_FastSpherePosData[i][0] + center.x + g_FastSpherePosData[i][5] * radius;
		pos.y = g_FastSpherePosData[i][1] + center.y + g_FastSpherePosData[i][6] * radius;
		pos.z = g_FastSpherePosData[i][2] + center.z + g_FastSpherePosData[i][7] * radius;

		meshBuilder.Position3fv( pos.Base() );
		meshBuilder.Normal3fv( &g_FastSpherePosData[i][5] );
		meshBuilder.TexCoord2fv( 0, &g_FastSpherePosData[i][3] );
		meshBuilder.Color3ub( 255, 255, 255 );
		meshBuilder.AdvanceVertex();
	}

	for (i = 0; i < 84; i++)
	{
		meshBuilder.FastIndex( g_FastSphereTriData[i][0] + offset );
		meshBuilder.FastIndex( g_FastSphereTriData[i][1] + offset );
		meshBuilder.FastIndex( g_FastSphereTriData[i][2] + offset );
	}
}




//-----------------------------------------------------------------------------
// Purpose: Custom model rendering
//-----------------------------------------------------------------------------


static ConVar	sv_blr_cubewidth( "blr_cubewidth", "0.8", 0, "Set cubewidth (coarseness of the mesh)" );
static ConVar	sv_blr_render_radius( "blr_render_radius", "1.3", 0, "Set render radius (how far from particle center surface will be)" );
static ConVar	sv_blr_cutoff_radius( "blr_cutoff_radius", "3.3", 0, "Set cutoff radius (how far field extends from each particle)" );

static ConVar	sv_surface_center( "surface_center", "0", FCVAR_ARCHIVE, "Adjust render center" );
static ConVar	sv_surface_rotate("surface_rotate", "1", 0, "Whether to rotate for transparency");
static ConVar	sv_surface_testshape( "surface_testshape", "0", 0, "Use a test shape instead of the hydra" );
static ConVar	sv_surface_fountain( "surface_fountain", "0", FCVAR_ARCHIVE, "Turns on settings for rendering the fountain" );

static ConVar	sv_surface_draw( "surface_draw", "1", 0, "Draw the surface" );
static ConVar	sv_surface_wireframe( "surface_wireframe", "0", FCVAR_ARCHIVE, "Draw wireframe" );
static ConVar	sv_surface_material("surface_material", "3", FCVAR_ARCHIVE, "Choose a material from 0 to N");
static ConVar	sv_surface_shader("surface_shader", "", FCVAR_ARCHIVE, "Choose a shader");

static ConVar	sv_surface_use_tiler("surface_use_tiler", "1", 0, "Use the tiler");
static ConVar	sv_surface_draw_margin("surface_draw_margin", "0", 0, "If tiler is disabled, whether to draw the margin.");
static ConVar	sv_surface_tile("surface_tile", "1", 0, "If tiler is enabled, whether we draw all tiles or just the central one.");
static ConVar	sv_surface_max_tiles( "surface_max_tiles", "-1", 0, "The maximum number of tiles to draw" );
static ConVar	sv_surface_max_slices( "surface_max_slices", "-1", 0, "The maximum number of slices to draw" );

static ConVar	sv_surface_calc_uv_and_tan( "surface_calc_uv_and_tan", "1", FCVAR_ARCHIVE, "Calculate UVs and Tangents" );
static ConVar	sv_surface_calc_tan_only( "surface_calc_tan_only", "0", FCVAR_ARCHIVE, "Calculate Only Tangents" );
static ConVar	sv_surface_calc_color( "surface_calc_color", "0", FCVAR_ARCHIVE, "Just interpolate colors" );
static ConVar	sv_surface_calc_hifreq_color( "surface_calc_hifreq_color", "0", FCVAR_ARCHIVE, "Experimental hi-freq colors" );
static ConVar	sv_surface_calc_tile_color( "surface_calc_tile_color", "0", FCVAR_ARCHIVE, "Shows color of the tile" );

extern ConVar	mat_wireframe;

void C_NPC_Surface::GetRenderBounds( Vector& theMins, Vector& theMaxs )
{
	// BaseClass::GetRenderBounds( theMins, theMaxs );
	if(sv_surface_testshape.GetBool())
	{
		theMins.Init(-300.0f, 0.0f, 100.0f);
		theMaxs.Init(0.0f, 100.0f, 200.0f);
	}
	else
	{
		theMins = m_vecSurfacePos[0];
		theMaxs = m_vecSurfacePos[0];
		float surfaceRadius = m_flRadius * 3.0f;
		for (int i = 0; i < m_nActiveParticles; i++)
		{
			VectorMin( m_vecSurfacePos[i] - Vector( surfaceRadius, surfaceRadius, surfaceRadius ), theMins, theMins );
			VectorMax( m_vecSurfacePos[i] + Vector( surfaceRadius, surfaceRadius, surfaceRadius ), theMaxs, theMaxs );
		}
	}
	theMins -= GetRenderOrigin();
	theMaxs -= GetRenderOrigin();

	#if 0
	Vector avg = (theMins + theMaxs) * 0.5f;
	theMins = theMins - ((theMins - avg) * 0.75f);
	theMaxs = theMaxs - ((theMaxs - avg) * 0.75f);
	#endif

	#if 0
	Vector fountainOrigin(-1980, -1792, 1);
	theMins = fountainOrigin + Vector(-10, -10, -20);
	theMaxs = fountainOrigin + Vector(10, 10, 50);
	#endif

	// Msg( "origin  %.2f %.2f %.2f : mins %.2f %.2f %.2f  : maxs %.2f %.2f %.2f\n", GetRenderOrigin().x, GetRenderOrigin().y, GetRenderOrigin().z, theMins.x, theMins.y, theMins.z, theMaxs.x, theMaxs.y, theMaxs.z );

	//debugoverlay->AddBoxOverlay( GetRenderOrigin(), theMins, theMaxs, QAngle( 0, 0, 0 ), 0, 255, 0, 0, 0 );
}

bool C_NPC_Surface::IsTransparent()
{
	// TODO: Fix this
	return true;
}

bool C_NPC_Surface::UsesPowerOfTwoFrameBufferTexture()
{
	if(!m_pMaterial) return false;
	return m_pMaterial->NeedsPowerOfTwoFrameBufferTexture();
}

bool C_NPC_Surface::UsesFullFrameBufferTexture()
{
	if(!m_pMaterial) return false;
	return m_pMaterial->NeedsFullFrameBufferTexture();
}

__forceinline float sqr(float a) { return a*a; }

Vector lastPoint0Pos;

int C_NPC_Surface::DrawModel( int flags )
{
#if !defined( _X360 ) // X360TBD: Enable blobulator for EP3

	if (sv_surface_wireframe.GetBool())
	{
		if(sv_surface_material.GetInt() == 0) {
			m_pMaterial = materials->FindMaterial("shadertest/wireframe", TEXTURE_GROUP_OTHER);
		} else {
			m_pMaterial = materials->FindMaterial("shadertest/wireframevertexcolornocull", TEXTURE_GROUP_OTHER);
		}
	}
	else if (strlen( sv_surface_shader.GetString() ) > 0)
	{
		m_pMaterial = materials->FindMaterial( sv_surface_shader.GetString(), TEXTURE_GROUP_OTHER, true );
	}
	else
	{
		if(sv_surface_material.GetInt() == 0) {
			m_pMaterial = materials->FindMaterial( "models/debug/debugwhite", TEXTURE_GROUP_OTHER, true );
		} else if(sv_surface_material.GetInt() == 1) {
			m_pMaterial = materials->FindMaterial( "models/debug/debugwhite2", TEXTURE_GROUP_OTHER, true );
		} else if(sv_surface_material.GetInt() == 2) {
			m_pMaterial = materials->FindMaterial( "models/debug/debugwhite3", TEXTURE_GROUP_OTHER, true );
		} else if(sv_surface_material.GetInt() == 3) {
			m_pMaterial = materials->FindMaterial( "debug/debugvertexcolor", TEXTURE_GROUP_OTHER, true );
		} else if(sv_surface_material.GetInt() == 4) {
			m_pMaterial = materials->FindMaterial( "debug/env_cubemap_model", TEXTURE_GROUP_OTHER, true );
		} else if(sv_surface_material.GetInt() == 5) {
			m_pMaterial = materials->FindMaterial( "debug/env_cubemap_model_translucent_fountain", TEXTURE_GROUP_OTHER, true );
		} else if(sv_surface_material.GetInt() == 6) {
			m_pMaterial = materials->FindMaterial( "models/debug/debugmesh", TEXTURE_GROUP_OTHER, true );
		} else if(sv_surface_material.GetInt() == 7) {
			m_pMaterial = materials->FindMaterial( "models/debug/debugmesh_transparent", TEXTURE_GROUP_OTHER, true );
		} else if(sv_surface_material.GetInt() == 8) {
			m_pMaterial = materials->FindMaterial( "models/ihvtest/tongue_bumped", TEXTURE_GROUP_OTHER, true );
		} else if(sv_surface_material.GetInt() == 9) {
			m_pMaterial = materials->FindMaterial( "models/debug/debugbumps", TEXTURE_GROUP_OTHER, true );
		} else if(sv_surface_material.GetInt() == 10) {
			m_pMaterial = materials->FindMaterial( "debug/env_cubemap_model_translucent_no_bumps", TEXTURE_GROUP_OTHER, true );
		} else {
			//pMaterial = materials->FindMaterial( "effects/tp_refract", TEXTURE_GROUP_OTHER, true );
			//m_pMaterial = materials->FindMaterial( "debug/debugrefract", TEXTURE_GROUP_OTHER, true );
			//m_pMaterial = materials->FindMaterial( "shadertest/water_refract_only", TEXTURE_GROUP_OTHER, true );
			//m_pMaterial = materials->FindMaterial( "nature/sewer_water001", TEXTURE_GROUP_OTHER, true );
			
			m_pMaterial = materials->FindMaterial( "models/shadertest/predator", TEXTURE_GROUP_OTHER, true );
		}
	}

	Vector fountainOrigin(-1980, -1792, 1);
	if(sv_surface_fountain.GetBool())
	{
		modelrender->SetupLighting( fountainOrigin );
	}
	else
	{
		modelrender->SetupLighting( GetRenderOrigin() );
		//modelrender->SetupLighting( Vector(0,0,300) );
	}


	#define MAX_EXTRA_ELEMENTS 400
	static ImpParticle imp_particles[MAX_SURFACE_ELEMENTS + MAX_EXTRA_ELEMENTS]; // This doesn't specify alignment, might have problems with SSE
	int n_particles = 0;

	if(sv_surface_testshape.GetBool())
	{
		for (int i = -10; i <= 10; i++)
		{
			ImpParticle* imp_particle = &imp_particles[i+10];
			imp_particle->center.set(i*2.0f*m_flRadius, 0.0f, 0.0f);
			n_particles++;
		}
	}
	else
	{
		for (int i = 0 ; i < m_nActiveParticles; i++)
		{
			ImpParticle* imp_particle = &imp_particles[i];
			imp_particle->center = m_vecSurfacePos[i];
			imp_particle->setFieldScale( m_flSurfaceR[i] );
			imp_particle->interpolants[3] = m_flSurfaceV[i];
			n_particles++;
		}

		// This code adds a water surface to the fountain trough
		// using particles that oscillate up and down
		if(sv_surface_fountain.GetBool())
		{
			static float time = 0.0f;
			
			bool paused = m_vecSurfacePos[0] == lastPoint0Pos;
			lastPoint0Pos = m_vecSurfacePos[0]; //((CAI_BaseNPC::m_nDebugBits & bits_debugDisableAI) != 0);
			if(!paused) time += 0.1f;

			for (int i = -7; i <= 7; i++)
			for (int j = -7; j <= 7; j++)
			{
					ImpParticle* imp_particle = &imp_particles[n_particles++];
					imp_particle->center = fountainOrigin + Vector(i * 2.0f * m_flRadius, j * 2.0f * m_flRadius, 15.0f);
					float dist = sqrtf(sqr(imp_particle->center[0]) + sqr(imp_particle->center[1]));
					
					imp_particle->center[2] += 2.0f*sin(2.0f*time + 2.0f*dist);
					imp_particle->setFieldScale( 1.0f );
					imp_particle->interpolants[3] = 0.0f;
			}
			for (int i = -2; i <= 2; i++)
			for (int j = -2; j <= 2; j++)
			{
					ImpParticle* imp_particle = &imp_particles[n_particles++];
					imp_particle->center = fountainOrigin + Vector(i * 2.0f * m_flRadius, j * 2.0f * m_flRadius, 15.0f - 2.0f * m_flRadius);
					imp_particle->setFieldScale( 1.0f );
					imp_particle->interpolants[3] = 0.0f;
			}
			ImpParticle* imp_particle = &imp_particles[n_particles++];
			imp_particle->center = fountainOrigin + Vector(0.0f, 0.0f, 15.0f - 4.0f * m_flRadius);
			imp_particle->setFieldScale( 1.0f );
			imp_particle->interpolants[3] = 0.0f;
		}
	}



	if( !IsErrorMaterial( m_pMaterial ) )
	{
		if( !sv_surface_draw.GetBool())
		{
			Point3D eye = view->GetViewSetup()->origin;

			struct sortParticles_t
			{
				int no;
				float dist;
			};
			class C
			{
			public:
				static bool gt(sortParticles_t a, sortParticles_t b)
				{
					return a.dist < b.dist;
				}
			};
			SmartArray<sortParticles_t> sort_particles;
			sort_particles.ensureCapacity(n_particles);
			sort_particles.size = n_particles;
			for(int i = 0; i < n_particles; i++)
			{
				sort_particles[i].no = i;
				sort_particles[i].dist = imp_particles[i].center.length(eye);
			}
			sort_particles.sort<C>();

			CMatRenderContextPtr pRenderContext( materials );
			
			pRenderContext->MatrixMode( MATERIAL_MODEL );
			pRenderContext->Bind( m_pMaterial );

			IMesh* pMesh = pRenderContext->GetDynamicMesh( true );

			int vertMax = min( 24000 / 51, 32768 / (84 * 3) );

			int j = 0;
			// Msg( "point %.2f %.2f %.2f\n", m_vecSurfacePos[0].x, m_vecSurfacePos[0].y, m_vecSurfacePos[0].z );
			while (j < n_particles)
			{
				int total = min( n_particles - j, vertMax );

				CMeshBuilder meshBuilder;
				meshBuilder.Begin( pMesh, MATERIAL_TRIANGLES, total * 51, total * 84 * 3 );

				int i = 0;
				while (i < vertMax && j < n_particles)
				{
					ImpParticle* imp_particle = &imp_particles[sort_particles[j].no];
					if (imp_particle->scale > 0.01)
					{
						DrawFastSphere( meshBuilder, imp_particle->center.AsVector(), m_flRadius * imp_particle->scale, 255, 255, 255 );
						i++;
					}
					j++;
				}

				meshBuilder.End();
				pMesh->Draw();
			}
		}
		else
		{
			// Note: it is not good to have these static variables here.
			static RENDERER_CLASS* sweepRenderer = NULL;
			static ImpTiler* tiler = NULL;

			if(!sweepRenderer)
			{
				sweepRenderer = new RENDERER_CLASS();
				tiler = new ImpTiler(sweepRenderer);
			}

			tiler->setMaxNoTilesToDraw(sv_surface_max_tiles.GetInt());
			sweepRenderer->setMaxNoSlicesToDraw(sv_surface_max_slices.GetInt());

			RENDERER_CLASS::setCubeWidth(sv_blr_cubewidth.GetFloat());
			RENDERER_CLASS::setRenderR(sv_blr_render_radius.GetFloat());
			RENDERER_CLASS::setCutoffR(sv_blr_cutoff_radius.GetFloat());

			if(sv_surface_calc_uv_and_tan.GetBool())
			{
				RENDERER_CLASS::setCalcSignFunc(calcSign);
				RENDERER_CLASS::setCalcSign2Func(calcSign2);
				RENDERER_CLASS::setCalcCornerFunc(calcCornerNormalColorUVTan);
				RENDERER_CLASS::setCalcVertexFunc(calcVertexNormalNColorUVTan);
			}
			else if(sv_surface_calc_tan_only.GetBool())
			{
				RENDERER_CLASS::setCalcSignFunc(calcSign);
				RENDERER_CLASS::setCalcSign2Func(calcSign2);
				RENDERER_CLASS::setCalcCornerFunc(calcCornerNormalColorTanNoUV);
				RENDERER_CLASS::setCalcVertexFunc(calcVertexNormalNColorTanNoUV);
			}
			else if	(sv_surface_calc_color.GetBool())
			{
				RENDERER_CLASS::setCalcSignFunc(calcSign);
				RENDERER_CLASS::setCalcSign2Func(calcSign2);
				RENDERER_CLASS::setCalcCornerFunc(calcCornerNormalColor);
				RENDERER_CLASS::setCalcVertexFunc(calcVertexNormalNColor);
			}
			else if (sv_surface_calc_hifreq_color.GetBool())
			{
				RENDERER_CLASS::setCalcSignFunc(calcSign);
				RENDERER_CLASS::setCalcSign2Func(calcSign2);
				RENDERER_CLASS::setCalcCornerFunc(calcCornerNormalHiFreqColor);
				RENDERER_CLASS::setCalcVertexFunc(calcVertexNormalNColor);
			}
			else if (sv_surface_calc_tile_color.GetBool())
			{
				RENDERER_CLASS::setCalcSignFunc(calcSign);
				RENDERER_CLASS::setCalcSign2Func(calcSign2);
				RENDERER_CLASS::setCalcCornerFunc(calcCornerNormal);
				RENDERER_CLASS::setCalcVertexFunc(calcVertexNormalDebugColor);
			}
			else
			{
				RENDERER_CLASS::setCalcSignFunc(calcSign);
				RENDERER_CLASS::setCalcSign2Func(calcSign2);
				RENDERER_CLASS::setCalcCornerFunc(calcCornerNormal);
				RENDERER_CLASS::setCalcVertexFunc(calcVertexNormal);
			}
				

			Vector center; 
			if(sv_surface_testshape.GetBool())
			{
				center.Init(); // set center to 0,0,0
			}
			else if(sv_surface_fountain.GetBool())
			{
				center = fountainOrigin;
			}
			else
			{
				center.Init();

				if(sv_surface_center.GetBool())
				{
					for(int i = 0; i < m_nActiveParticles; i++)
					{
						center += m_vecSurfacePos[i];
					}
					center /= m_nActiveParticles;
				}
			}

			Vector transformedCenter = center * (1.0f/m_flRadius);

			CMatRenderContextPtr pRenderContext( materials );
			pRenderContext->MatrixMode( MATERIAL_MODEL );
			pRenderContext->Bind( m_pMaterial );
			pRenderContext->PushMatrix();
			pRenderContext->LoadIdentity();

			if(sv_surface_testshape.GetBool())
			{
				pRenderContext->Translate(-150.0f, 50.0f, 150.0f);
			}
			else
			{
				pRenderContext->Translate(center.x, center.y, center.z);
			}

			pRenderContext->Scale(m_flRadius, m_flRadius, m_flRadius);

			VMatrix rotationMatrix;
			VMatrix invRotationMatrix;
			Vector transformedEye;
			float angle = 0.0f;
			if(sv_surface_rotate.GetBool())
			{
				angle = view->GetViewSetup()->angles.y+180.0f;

				//ConMsg("Angle = %f\n", angle);
				pRenderContext->Rotate(angle, 0.0f, 0.0f, 1.0f);

				//VMatrix rotationMatrix2 = SetupMatrixAngles(view->GetViewSetup()->angles);
				//pRenderContext->MultMatrix(rotationMatrix2);
				//VMatrix rotationMatrix = rotationMatrix2.InverseTR(); //SetupMatrixAngles(-(view->GetViewSetup()->angles));

				rotationMatrix = SetupMatrixAxisRot(Vector(0.0f, 0.0f, 1.0f), -angle);
				invRotationMatrix = SetupMatrixAxisRot(Vector(0.0f, 0.0f, 1.0f), angle);
				Vector eye = view->GetViewSetup()->origin;
				transformedEye = (eye-center)*(1.0f/m_flRadius);
				transformedEye = rotationMatrix.ApplyRotation(transformedEye);
			}
			else
			{
				rotationMatrix.Identity();
				invRotationMatrix.Identity();
				transformedEye.Init();
				angle = 0.0f;
			}

			if(sv_surface_use_tiler.GetBool())
			{
				tiler->beginFrame(Point3D(0.0f, 0.0f, 0.0f), (void*)&pRenderContext, !(sv_surface_draw_margin.GetBool()));
			}
			else
			{
				sweepRenderer->beginFrame(!(sv_surface_draw_margin.GetBool()), (void*)&pRenderContext);
				sweepRenderer->setOffset(Point3D(0.0f, 0.0f, 0.0f));
				//sweepRenderer->beginTile();
			}

			for (int i = 0 ; i < n_particles; i++)
			{
				ImpParticle* imp_particle = &imp_particles[i];
				if(imp_particle->scale <= 0.1f) continue;

				Vector vParticle = imp_particle->center.AsVector();
				//vParticle.Init(imp_particle->center[0], imp_particle->center[1], imp_particle->center[2]);
				Vector transformedParticle = (vParticle-center) * (1.0f/m_flRadius);

				if(sv_surface_rotate.GetBool())
				{
					transformedParticle = rotationMatrix.ApplyRotation(transformedParticle);
				}

				Point3D pParticle = transformedParticle; 
				Point3D pCenter = transformedCenter;
				Point3D vec = (pParticle - pCenter);

				imp_particle->center = pParticle;
				//imp_particle->setFieldScale( max(1.2f - vec.length()/30.0f, 0.25f) );

				// interpolants[0..2] is the color. interpolants[3] is the v coordinate
				// imp_particle->interpolants[3] = min(max(1.4f - vec.length()/17.0f, 0.0f), 1.0f);
				// interpolants2[0..2] is the tangent vector.
				// interpolants3[0..2] and interpolants4[0..2] are the normal and
				// binormal which are used to generate a u coordinate
				imp_particle->interpolants2 = vec.unit();
				imp_particle->interpolants4.set(0.0f, 0.0f, -1.0f);
				imp_particle->interpolants3 = imp_particle->interpolants2.crossProduct(imp_particle->interpolants4);
				imp_particle->interpolants3.normalize();
				imp_particle->interpolants4 = imp_particle->interpolants2.crossProduct(imp_particle->interpolants3);
				imp_particle->interpolants4.normalize();
				
				if(sv_surface_use_tiler.GetBool())
				{
					tiler->insertParticle(imp_particle);
				}
				else
				{
					sweepRenderer->addParticle(imp_particle);
				}
			}

			if(sv_surface_use_tiler.GetBool())
			{
				if(sv_surface_tile.GetBool())
				{
					if(sv_surface_rotate.GetBool())
					{
						tiler->drawSurfaceSorted(Point3D(transformedEye));
					}
					else
					{
						tiler->drawSurface();
					}
				}
				else
				{
					tiler->drawTile(0,0,0);
				}
				tiler->endFrame();
			}
			else
			{
				sweepRenderer->endTile();
				sweepRenderer->endFrame();
			}

			if(sv_surface_max_tiles.GetInt() > 0 || sv_surface_tile.GetBool()==false)
			{
				//debugoverlay->AddBoxOverlay( fountainOrigin, Vector(-30, -30, -30), Vector(30, 30, 30), QAngle( 0, 0, 0 ), 0, 255, 0, 0, 0 );
				Vector overlayCenter = invRotationMatrix.ApplyRotation(tiler->getLastTilesOffset().AsVector() * m_flRadius);
				Vector mins = (tiler->getRenderDim() * (-0.5f* m_flRadius)).AsVector();
				Vector maxs = (tiler->getRenderDim() * (0.5f*m_flRadius)).AsVector();
				debugoverlay->AddBoxOverlay( overlayCenter + fountainOrigin, mins, maxs, QAngle( 0, angle, 0 ), 0, 255, 0, 0, 0 );

				if(sv_surface_max_slices.GetInt() > 0)
				{
					Vector sliceMins = mins;
					Vector sliceMaxs = maxs;
					sliceMins.x += (sweepRenderer->getLastSliceDrawn() - sweepRenderer->getMarginNCubes()) * sweepRenderer->getCubeWidth() * m_flRadius;
					sliceMaxs.x = sliceMins.x + sweepRenderer->getCubeWidth() * m_flRadius;
					
					debugoverlay->AddBoxOverlay( overlayCenter + fountainOrigin, sliceMins, sliceMaxs, QAngle( 0, angle, 0 ), 255, 0, 0, 0, 0 );
				}
			}

			pRenderContext->PopMatrix();
		}
	}

	#endif // !_X360

	return 1;
}

//-----------------------------------------------------------------------------
// Purpose: move the sound source to a point near the player
//-----------------------------------------------------------------------------

bool C_NPC_Surface::GetSoundSpatialization( SpatializationInfo_t& info )
{
	bool bret = BaseClass::GetSoundSpatialization( info );
	// Default things it's audible, put it at a better spot?
	if ( bret )
	{
		// TODO:  Note, this is where you could override the sound position and orientation and use
		//  an attachment points position as the sound source
		// You might have to issue C_BaseAnimating::AllowBoneAccess( true, false ); to allow
		//  bone setup during sound spatialization if you run into asserts...
	}

	return bret;
}



