//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// 4-23-98  
// JOHN:  implementation of interface between client-side DLL and game engine.
//  The cdll shouldn't have to know anything about networking or file formats.
//  This file is Win32-dependant
//
//=============================================================================//

#include "client_pch.h"
#include "getintersectingsurfaces_struct.h"
#include "gl_model_private.h"
#include "surfinfo.h"
#include "vstdlib/random.h"
#include "cdll_int.h"
#include "cmodel_engine.h"
#include "tmessage.h"
#include "console.h"
#include "snd_audio_source.h"
#include <vgui_controls/Controls.h>
#include <vgui/IInput.h>
#include "iengine.h"
#include "keys.h"
#include "con_nprint.h"
#include "tier0/vprof.h"
#include "sound.h"
#include "gl_rmain.h"
#include "proto_version.h"
#include "client_class.h"
#include "gl_rsurf.h"
#include "server.h"
#include "r_local.h"
#include "lightcache.h"
#include "gl_matsysiface.h"
#include "materialsystem/imaterialsystemhardwareconfig.h"
#include "materialsystem/materialsystem_config.h"
#include "materialsystem/imaterial.h"
#include "materialsystem/imaterialvar.h"
#include "materialsystem/itexture.h"
#include "istudiorender.h"
#include "l_studio.h"
#include "voice.h"
#include "enginestats.h"
#include "testscriptmgr.h"
#include "r_areaportal.h"
#include "host.h"
#include "vox.h"
#include "iprediction.h"
#include "icliententitylist.h"
#include "eiface.h"
#include "ivguicenterprint.h"
#include "engine/IClientLeafSystem.h"
#include "dt_recv_eng.h"
#include <vgui/IVGui.h>
#include "sys_dll.h"
#include "vphysics_interface.h"
#include "materialsystem/imesh.h"
#include "IOcclusionSystem.h"
#include "filesystem_engine.h"
#include "tier0/icommandline.h"
#include "client_textmessage.h"
#include "host_saverestore.h"
#include "cl_main.h"
#include "demo.h"
#include "vgui_baseui_interface.h"
#include "LocalNetworkBackdoor.h"
#include "lightcache.h"
#include "vgui/ISystem.h"
#include "Steam.h"
#include "ivideomode.h"
#include "icolorcorrectiontools.h"
#include "toolframework/itoolframework.h"
#include "engine/view_sharedv1.h"
#include "view.h"
#include "game/client/iclientrendertargets.h"
#include "tier2/tier2.h"
#include "matchmaking.h"
#include "inputsystem/iinputsystem.h"
#include "iachievementmgr.h"
#include "profile.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//-----------------------------------------------------------------------------
// forward declarations
//-----------------------------------------------------------------------------
IMaterial* BrushModel_GetLightingAndMaterial( const Vector &start, 
	const Vector &end, Vector &diffuseLightColor, Vector &baseColor );
const char *Key_NameForBinding( const char *pBinding );
void CL_GetBackgroundLevelName( char *pszBackgroundName, int bufSize, bool bMapName );
CreateInterfaceFn g_ClientFactory = NULL;
extern char gpszVersionString[32];
extern int g_iSteamAppID;
extern	CGlobalVars g_ServerGlobalVariables;

//-----------------------------------------------------------------------------
// globals
//-----------------------------------------------------------------------------
CSysModule		*g_ClientDLLModule = NULL; // also used by materialproxyfactory.cpp
bool g_bClientGameDLLGreaterThanV13;

void AddIntersectingLeafSurfaces( mleaf_t *pLeaf, GetIntersectingSurfaces_Struct *pStruct )
{
	SurfaceHandle_t *pHandle = &host_state.worldbrush->marksurfaces[pLeaf->firstmarksurface];
	for ( int iSurf=0; iSurf < pLeaf->nummarksurfaces; iSurf++ )
	{
		SurfaceHandle_t surfID = pHandle[iSurf];
		ASSERT_SURF_VALID( surfID );
		
		if ( MSurf_Flags(surfID) & SURFDRAW_SKY )
			continue;

		// Make sure we haven't already processed this one.
		bool foundSurf = false;
		for(int iTest=0; iTest < pStruct->m_nSetInfos; iTest++)
		{
			if(pStruct->m_pInfos[iTest].m_pEngineData == (void *)surfID)
			{
				foundSurf = true;
				break;
			}
		}
		if ( foundSurf )
			continue;

		// Make sure there's output room.
		if(pStruct->m_nSetInfos >= pStruct->m_nMaxInfos)
			return;
		SurfInfo *pOut = &pStruct->m_pInfos[pStruct->m_nSetInfos];
		pOut->m_nVerts = 0;
		pOut->m_pEngineData = (void *)surfID;

		// Build vertex list and bounding box.			
		Vector vMin( 1000000.0f,  1000000.0f,  1000000.0f);
		Vector vMax(-1000000.0f, -1000000.0f, -1000000.0f);
		for(int iVert=0; iVert < MSurf_VertCount( surfID ); iVert++)
		{
			int vertIndex = pStruct->m_pModel->brush.pShared->vertindices[MSurf_FirstVertIndex( surfID ) + iVert];

			pOut->m_Verts[pOut->m_nVerts] = pStruct->m_pModel->brush.pShared->vertexes[vertIndex].position;
			vMin = vMin.Min(pOut->m_Verts[pOut->m_nVerts]);
			vMax = vMax.Max(pOut->m_Verts[pOut->m_nVerts]);

			++pOut->m_nVerts;
			if(pOut->m_nVerts >= MAX_SURFINFO_VERTS)
				break;
		}

		// See if the sphere intersects the box.
		int iDim=0;
		for(; iDim < 3; iDim++)
		{
			if(((*pStruct->m_pCenter)[iDim]+pStruct->m_Radius) < vMin[iDim] || 
				((*pStruct->m_pCenter)[iDim]-pStruct->m_Radius) > vMax[iDim])
			{
				break;
			}
		}
		
		if(iDim == 3)
		{
			// (Couldn't reject the sphere in the loop above).
			pOut->m_Plane = MSurf_GetForwardFacingPlane( surfID );
			++pStruct->m_nSetInfos;
		}
	}
}

void GetIntersectingSurfaces_R(
	GetIntersectingSurfaces_Struct *pStruct,
	mnode_t *pNode
	)
{
	if(pStruct->m_nSetInfos >= pStruct->m_nMaxInfos)
		return;

	// Ok, this is a leaf. Check its surfaces.
	if(pNode->contents >= 0)
	{
		mleaf_t *pLeaf = (mleaf_t*)pNode;

		if(pStruct->m_bOnlyVisible && pStruct->m_pCenterPVS)
		{
			if(pLeaf->cluster < 0)
				return;

			if(!(pStruct->m_pCenterPVS[pLeaf->cluster>>3] & (1 << (pLeaf->cluster&7))))
				return;
		}

		// First, add tris from displacements.
		for ( int i = 0; i < pLeaf->dispCount; i++ )
		{
			IDispInfo *pDispInfo = MLeaf_Disaplcement( pLeaf, i );
			pDispInfo->GetIntersectingSurfaces( pStruct );
		}

		// Next, add brush tris.
		AddIntersectingLeafSurfaces( pLeaf, pStruct );
		return;
	}
	
	// Recurse.
	float dot;
	cplane_t *plane = pNode->plane;
	if ( plane->type < 3 )
	{
		dot = (*pStruct->m_pCenter)[plane->type] - plane->dist;
	}
	else
	{
		dot = pStruct->m_pCenter->Dot(plane->normal) - plane->dist;
	}

	// Recurse into child nodes.
	if(dot > -pStruct->m_Radius)
		GetIntersectingSurfaces_R(pStruct, pNode->children[SIDE_FRONT]);
	
	if(dot < pStruct->m_Radius)
		GetIntersectingSurfaces_R(pStruct, pNode->children[SIDE_BACK]);
}


//-----------------------------------------------------------------------------
// slow routine to draw a physics model
// NOTE: very slow code!!! just for debugging!
//-----------------------------------------------------------------------------
void DebugDrawPhysCollide( const CPhysCollide *pCollide, IMaterial *pMaterial, matrix3x4_t& transform, const color32 &color, bool drawAxes )
{
	if ( !pMaterial )
	{
		pMaterial = materials->FindMaterial("shadertest/wireframevertexcolor", TEXTURE_GROUP_OTHER);
	}

	CMatRenderContextPtr pRenderContext( materials );

	Vector *outVerts;
	int vertCount = physcollision->CreateDebugMesh( pCollide, &outVerts );
	if ( vertCount )
	{
		IMesh *pMesh = pRenderContext->GetDynamicMesh( true, NULL, NULL, pMaterial );

		CMeshBuilder meshBuilder;
		meshBuilder.Begin( pMesh, MATERIAL_TRIANGLES, vertCount/3 );

		for ( int j = 0; j < vertCount; j++ )
		{
			Vector out;
			VectorTransform( outVerts[j].Base(), transform, out.Base() );
			meshBuilder.Position3fv( out.Base() );
			meshBuilder.Color4ub( color.r, color.g, color.b, color.a );
			meshBuilder.TexCoord2f( 0, 0, 0 );
			meshBuilder.AdvanceVertex();
		}
		meshBuilder.End();
		pMesh->Draw();
	}
	physcollision->DestroyDebugMesh( vertCount, outVerts );

	// draw the axes
	if ( drawAxes )
	{
		Vector xaxis(10,0,0), yaxis(0,10,0), zaxis(0,0,10);
		Vector center;
		Vector out;

		MatrixGetColumn( transform, 3, center );
		IMesh *pMesh = pRenderContext->GetDynamicMesh( true, NULL, NULL, pMaterial );
		CMeshBuilder meshBuilder;
		meshBuilder.Begin( pMesh, MATERIAL_LINES, 3 );

		// X
		meshBuilder.Position3fv( center.Base() );
		meshBuilder.Color4ub( 255, 0, 0, 255 );
		meshBuilder.TexCoord2f( 0, 0, 0 );
		meshBuilder.AdvanceVertex();
		VectorTransform( xaxis.Base(), transform, out.Base() );
		meshBuilder.Position3fv( out.Base() );
		meshBuilder.Color4ub( 255, 0, 0, 255 );
		meshBuilder.TexCoord2f( 0, 0, 0 );
		meshBuilder.AdvanceVertex();

		// Y
		meshBuilder.Position3fv( center.Base() );
		meshBuilder.Color4ub( 0, 255, 0, 255 );
		meshBuilder.TexCoord2f( 0, 0, 0 );
		meshBuilder.AdvanceVertex();
		VectorTransform( yaxis.Base(), transform, out.Base() );
		meshBuilder.Position3fv( out.Base() );
		meshBuilder.Color4ub( 0, 255, 0, 255 );
		meshBuilder.TexCoord2f( 0, 0, 0 );
		meshBuilder.AdvanceVertex();

		// Z
		meshBuilder.Position3fv( center.Base() );
		meshBuilder.Color4ub( 0, 0, 255, 255 );
		meshBuilder.TexCoord2f( 0, 0, 0 );
		meshBuilder.AdvanceVertex();
		VectorTransform( zaxis.Base(), transform, out.Base() );
		meshBuilder.Position3fv( out.Base() );
		meshBuilder.Color4ub( 0, 0, 255, 255 );
		meshBuilder.TexCoord2f( 0, 0, 0 );
		meshBuilder.AdvanceVertex();
		meshBuilder.End();
		pMesh->Draw();
	}
}

//-----------------------------------------------------------------------------
//
// implementation of IVEngineHud
//
//-----------------------------------------------------------------------------

// UNDONE: Move this to hud export code, subsume previous functions
class CEngineClient : public IVEngineClient
{
public:
	CEngineClient();

	int		GetIntersectingSurfaces(
		const model_t *model,
		const Vector &vCenter, 
		const float radius,
		const bool bOnlyVisible,
		SurfInfo *pInfos, 
		const int nMaxInfos);

	Vector	GetLightForPoint(const Vector &pos, bool bClamp);
	Vector	GetLightForPointFast(const Vector &pos, bool bClamp);
	const char *ParseFile( const char *data, char *token, int maxlen );
	virtual bool CopyFile( const char *source, const char *destination );
	void GetScreenSize( int& w, int &h );
	void ServerCmd( const char *szCmdString, bool bReliable );
	void ClientCmd( const char *szCmdString );
	void ClientCmd_Unrestricted( const char *szCmdString );
	void SetRestrictServerCommands( bool bRestrict );
	void SetRestrictClientCommands( bool bRestrict );
	bool GetPlayerInfo( int ent_num, player_info_t *pinfo );
	client_textmessage_t *TextMessageGet( const char *pName );
	bool Con_IsVisible( void );
	int GetLocalPlayer( void );
	float GetLastTimeStamp( void );
	const model_t *LoadModel( const char *pName, bool bProp );
	void UnloadModel( const model_t *model, bool bProp );
	CSentence *GetSentence( CAudioSource *pAudioSource );
	float GetSentenceLength( CAudioSource *pAudioSource );
	bool IsStreaming( CAudioSource *pAudioSource ) const;

	// FIXME, move entirely to client .dll
	void GetViewAngles( QAngle& va );
	void SetViewAngles( QAngle& va );
	int GetMaxClients( void );
	void Key_Event( ButtonCode_t key, int down );
	const char *Key_LookupBinding( const char *pBinding );
	const char *Key_BindingForKey( ButtonCode_t &code );
	void StartKeyTrapMode( void );
	bool CheckDoneKeyTrapping( ButtonCode_t &key );
	bool IsInGame( void );
	bool IsConnected( void );
	bool IsDrawingLoadingImage( void );
	void Con_NPrintf( int pos, const char *fmt, ... );
	void Con_NXPrintf( const struct con_nprint_s *info, const char *fmt, ... );
	IMaterial *TraceLineMaterialAndLighting( const Vector &start, const Vector &end, 
		                                     Vector &diffuseLightColor, Vector &baseColor );
	int		IsBoxVisible( const Vector& mins, const Vector& maxs );
	int		IsBoxInViewCluster( const Vector& mins, const Vector& maxs );

	float Time();
	void Sound_ExtraUpdate( void );

	bool CullBox ( const Vector& mins, const Vector& maxs );
	const char *GetGameDirectory( void );
	const VMatrix& WorldToScreenMatrix();
	const VMatrix& WorldToViewMatrix();

	// Loads a game lump off disk
	int		GameLumpVersion( int lumpId ) const;
	int		GameLumpSize( int lumpId ) const;
	bool	LoadGameLump( int lumpId, void* pBuffer, int size );

	// Returns the number of leaves in the level
	int		LevelLeafCount() const;
	virtual ISpatialQuery* GetBSPTreeQuery();

	// Convert texlight to gamma...
	virtual void LinearToGamma( float* linear, float* gamma );

	// Get the lightstyle value
	virtual float LightStyleValue( int style );
	virtual void DrawPortals();

	// Computes light due to dynamic lighting at a point
	// If the normal isn't specified, then it'll return the maximum lighting
	virtual void ComputeDynamicLighting( Vector const& pt, Vector const* pNormal, Vector& color );

	// Computes light due to dynamic lighting at a point
	// If the normal isn't specified, then it'll return the maximum lighting
	// If pBoxColors is specified (it's an array of 6), then it'll copy the light contribution at each box side.
	virtual void ComputeLighting( const Vector& pt, const Vector* pNormal, bool bClamp, Vector& color, Vector *pBoxColors );

	// Returns the color of the ambient light
	virtual void GetAmbientLightColor( Vector& color );

	// Returns the dx support level
	virtual int	GetDXSupportLevel();
	
	virtual bool SupportsHDR();
	virtual void Mat_Stub( IMaterialSystem *pMatSys );

	// menu display
	virtual void GetChapterName( char *pchBuff, int iMaxLength );
	virtual char const *GetLevelName( void );
	virtual bool IsLevelMainMenuBackground( void );
	virtual void GetMainMenuBackgroundName( char *dest, int destlen );

	// Occlusion system control
	virtual void SetOcclusionParameters( const OcclusionParams_t &params );

	//-----------------------------------------------------------------------------
	// Purpose: Takes a trackerID and returns which player slot that user is in
	//			returns 0 if no player found with that ID
	//-----------------------------------------------------------------------------
	virtual int	GetPlayerForUserID(int userID);
#if !defined( NO_VOICE )
	virtual struct IVoiceTweak_s *GetVoiceTweakAPI( void );
#endif
	virtual void EngineStats_BeginFrame( void );
	virtual void EngineStats_EndFrame( void );
	virtual void FireEvents();
	virtual void CheckPoint( const char *pName );
	virtual int GetLeavesArea( int *pLeaves, int nLeaves );
	virtual bool DoesBoxTouchAreaFrustum( const Vector &mins, const Vector &maxs, int iArea );

	// Sets the hearing origin
	virtual void SetAudioState( const AudioState_t &audioState );

	//-----------------------------------------------------------------------------
	//
	// Sentence API
	//
	//-----------------------------------------------------------------------------

	virtual int SentenceGroupPick( int groupIndex, char *name, int nameLen );
	virtual int SentenceGroupPickSequential( int groupIndex, char *name, int nameLen, int sentenceIndex, int reset );
	virtual int SentenceIndexFromName( const char *pSentenceName );
	virtual const char *SentenceNameFromIndex( int sentenceIndex );
	virtual int SentenceGroupIndexFromName( const char *pGroupName );
	virtual const char *SentenceGroupNameFromIndex( int groupIndex );
	virtual float SentenceLength( int sentenceIndex );
	virtual void DebugDrawPhysCollide( const CPhysCollide *pCollide, IMaterial *pMaterial, matrix3x4_t& transform, const color32 &color );

	// Activates/deactivates an occluder...
	virtual void ActivateOccluder( int nOccluderIndex, bool bActive );
	virtual bool IsOccluded( const Vector &vecAbsMins, const Vector &vecAbsMaxs );
	virtual void *SaveAllocMemory( size_t num, size_t size );
	virtual void SaveFreeMemory( void *pSaveMem );
	virtual INetChannelInfo *GetNetChannelInfo( void );
	virtual bool IsPlayingDemo( void );
	virtual bool IsRecordingDemo( void );
	virtual bool IsPlayingTimeDemo( void );
	virtual bool IsPaused( void );
	virtual bool IsTakingScreenshot( void );
	virtual bool IsHLTV( void );
	virtual void GetUILanguage( char *dest, int destlen );

	// Can skybox be seen from a particular point?
	virtual SkyboxVisibility_t IsSkyboxVisibleFromPoint( const Vector &vecPoint );

	virtual const char* GetMapEntitiesString();
	virtual bool		IsInEditMode( void );
	virtual bool		IsInCommentaryMode( void );
	virtual float		GetScreenAspectRatio();

	virtual unsigned int		GetEngineBuildNumber() { return PROTOCOL_VERSION; }
	virtual const char *		GetProductVersionString() { return gpszVersionString; }
	virtual void				GrabPreColorCorrectedFrame( int x, int y, int width, int height );
	virtual bool				IsHammerRunning( ) const;

	// Stuffs the cmd into the buffer & executes it immediately (vs ClientCmd() which executes it next frame)
	virtual void				ExecuteClientCmd( const char *szCmdString );

	virtual bool MapHasHDRLighting( void) ;
	virtual int GetAppID();

	virtual void				SetOverlayBindProxy( int iOverlayID, void *pBindProxy );

	virtual bool				CopyFrameBufferToMaterial( const char *pMaterialName );

	// Matchmaking
	void						ChangeTeam( const char *pTeamName );
	virtual void				ReadConfiguration( const bool readDefault = false );

	virtual void SetAchievementMgr( IAchievementMgr *pAchievementMgr );
	virtual IAchievementMgr *GetAchievementMgr();

	virtual bool				MapLoadFailed( void );
	virtual void				SetMapLoadFailed( bool bState );

	virtual bool				IsLowViolence();
	virtual const char			*GetMostRecentSaveGame( void );
	virtual void				SetMostRecentSaveGame( const char *lpszFilename );

	virtual void				StartXboxExitingProcess();
	virtual bool				IsSaveInProgress();
	
	virtual uint				OnStorageDeviceAttached( void );
	virtual void				OnStorageDeviceDetached( void );

	virtual void				ResetDemoInterpolation( void );
	virtual bool				REMOVED_SteamRefreshLogin( const char *password, bool isSecure ) { return false; }
	virtual bool				REMOVED_SteamProcessCall( bool & finished ) { return false; }

private:
	SteamHandle_t				m_hRefreshLoginHandle;
};


//-----------------------------------------------------------------------------
// Singleton
//-----------------------------------------------------------------------------
static CEngineClient s_VEngineClient;
IVEngineClient *engineClient = &s_VEngineClient;
EXPOSE_SINGLE_INTERFACE_GLOBALVAR( CEngineClient, IVEngineClient, VENGINE_CLIENT_INTERFACE_VERSION, s_VEngineClient );


//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
CEngineClient::CEngineClient()
{
}

int	CEngineClient::GetIntersectingSurfaces(
	const model_t *model,
	const Vector &vCenter, 
	const float radius,
	const bool bOnlyVisible,
	SurfInfo *pInfos, 
	const int nMaxInfos)
{
	if ( !model )
		return 0;

	byte pvs[MAX_MAP_LEAFS/8];
	GetIntersectingSurfaces_Struct theStruct;
	theStruct.m_pModel = ( model_t * )model;
	theStruct.m_pCenter = &vCenter;
	theStruct.m_pCenterPVS = CM_Vis( pvs, sizeof(pvs), CM_LeafCluster( CM_PointLeafnum( vCenter ) ), DVIS_PVS );
	theStruct.m_Radius = radius;
	theStruct.m_bOnlyVisible = bOnlyVisible;
	theStruct.m_pInfos = pInfos;
	theStruct.m_nMaxInfos = nMaxInfos;
	theStruct.m_nSetInfos = 0;		

	// Go down the BSP.
	GetIntersectingSurfaces_R(
		&theStruct,
		&model->brush.pShared->nodes[ model->brush.firstnode ] );

	return theStruct.m_nSetInfos;
}

Vector	CEngineClient::GetLightForPoint(const Vector &pos, bool bClamp)
{
	Vector vRet;
	ComputeLighting( pos, NULL, bClamp, vRet, NULL );
	return vRet;
}

Vector CEngineClient::GetLightForPointFast(const Vector &pos, bool bClamp)
{
	Vector vRet;
	int leafIndex = CM_PointLeafnum(pos);
	vRet.Init();
	Vector cube[6];
	Mod_LeafAmbientColorAtPos( cube, pos, leafIndex );
	for ( int i = 0; i < 6; i++ )
	{
		vRet.x = fpmax(vRet.x, cube[i].x );
		vRet.y = fpmax(vRet.y, cube[i].y );
		vRet.z = fpmax(vRet.z, cube[i].z );
	}
	if ( bClamp )
	{
		if ( vRet.x > 1.0f )
			vRet.x = 1.0f;
		if ( vRet.y > 1.0f )
			vRet.y = 1.0f;
		if ( vRet.z > 1.0f )
			vRet.z = 1.0f;
	}
	return vRet;
}

const char *CEngineClient::ParseFile( const char *data, char *token, int maxlen )
{
	return ::COM_ParseFile( data, token, maxlen );
}

bool CEngineClient::CopyFile( const char *source, const char *destination )
{
	return ::COM_CopyFile( source, destination );
}

void CEngineClient::GetScreenSize( int& w, int &h )
{
	CMatRenderContextPtr pRenderContext( materials );
	pRenderContext->GetWindowSize( w, h );
}

void CEngineClient::ServerCmd( const char *szCmdString, bool bReliable )
{
	// info handling
	char buf[255];
	Q_snprintf( buf, sizeof( buf ), "cmd %s", szCmdString );

	CCommand args;
	args.Tokenize( buf );
	Cmd_ForwardToServer( args, bReliable );
}

void CEngineClient::ClientCmd( const char *szCmdString )
{
	// Only let them run commands marked with FCVAR_CLIENTCMD_CAN_EXECUTE.
	if ( cl.m_bRestrictClientCommands )
		Cbuf_AddExecutionMarker( eCmdExecutionMarker_Enable_FCVAR_CLIENTCMD_CAN_EXECUTE );
	
	Cbuf_AddText( szCmdString );
	Cbuf_AddText("\n");

	if ( cl.m_bRestrictClientCommands )
		Cbuf_AddExecutionMarker( eCmdExecutionMarker_Disable_FCVAR_CLIENTCMD_CAN_EXECUTE );
}

void CEngineClient::ClientCmd_Unrestricted( const char *szCmdString )
{
	Cbuf_AddText( szCmdString );
	Cbuf_AddText("\n");
}

void CEngineClient::SetRestrictServerCommands( bool bRestrict )
{
	cl.m_bRestrictServerCommands = bRestrict;
}

void CEngineClient::SetRestrictClientCommands( bool bRestrict )
{
	cl.m_bRestrictClientCommands = bRestrict;
}

void CEngineClient::ExecuteClientCmd( const char *szCmdString )
{
	Cbuf_AddText( szCmdString );
	Cbuf_AddText("\n");
	Cbuf_Execute();
}

bool CEngineClient::GetPlayerInfo( int ent_num, player_info_t *pinfo )
{
	ent_num--; // player list if offset by 1 from ents

	if ( ent_num >= cl.m_nMaxClients || ent_num < 0 )
	{
		Q_memset( pinfo, 0, sizeof( player_info_t ) );
		return false;
	}

	Assert( cl.m_pUserInfoTable );

	player_info_t *pi = (player_info_t*) cl.m_pUserInfoTable->GetStringUserData( ent_num, NULL );

	if ( !pi )
	{
		Q_memset( pinfo, 0, sizeof( player_info_t ) );
		return false;
	}

	Q_memcpy( pinfo, pi, sizeof( player_info_t ) );

	// Fixup from network order (little endian)
	CByteswap byteswap;
	byteswap.SetTargetBigEndian( false );
	byteswap.SwapFieldsToTargetEndian( pinfo );

	return true;
}

client_textmessage_t *CEngineClient::TextMessageGet( const char *pName )
{
	return ::TextMessageGet( pName );
}

bool CEngineClient::Con_IsVisible( void )
{
	return ::Con_IsVisible();
}

int CEngineClient::GetLocalPlayer( void )
{
	return cl.m_nPlayerSlot + 1;
}

float CEngineClient::GetLastTimeStamp( void )
{
	return cl.m_flLastServerTickTime;
}

bool CEngineClient::MapHasHDRLighting( void)
{
	return modelloader->LastLoadedMapHasHDRLighting();
}

const model_t *CEngineClient::LoadModel( const char *pName, bool bProp )
{
	return modelloader->GetModelForName( pName, bProp ? IModelLoader::FMODELLOADER_DETAILPROP : IModelLoader::FMODELLOADER_CLIENTDLL );
}

CSentence *CEngineClient::GetSentence( CAudioSource *pAudioSource )
{
	if (pAudioSource)
	{
		return pAudioSource->GetSentence();
	}
	return NULL;
}

float CEngineClient::GetSentenceLength( CAudioSource *pAudioSource )
{
	if (pAudioSource && pAudioSource->SampleRate() > 0 )
	{
		float length = (float)pAudioSource->SampleCount() / (float)pAudioSource->SampleRate();
		return length;
	}
	return 0.0f;
}

bool CEngineClient::IsStreaming( CAudioSource *pAudioSource ) const
{
	if ( pAudioSource )
	{
		return pAudioSource->IsStreaming();
	}
	return false;
}

// FIXME, move entirely to client .dll
void CEngineClient::GetViewAngles( QAngle& va )
{
	VectorCopy( cl.viewangles, va );
}

void CEngineClient::SetViewAngles( QAngle& va )
{
	cl.viewangles.x = AngleNormalize( va.x );
	cl.viewangles.y = AngleNormalize( va.y );
	cl.viewangles.z = AngleNormalize( va.z );
}

int CEngineClient::GetMaxClients( void )
{
	return cl.m_nMaxClients;
}

void CEngineClient::SetMapLoadFailed( bool bState )
{
	g_ServerGlobalVariables.bMapLoadFailed = bState;
}

bool CEngineClient::MapLoadFailed( void )
{
	return g_ServerGlobalVariables.bMapLoadFailed;
}

void CEngineClient::ReadConfiguration( const bool readDefault /*= false*/ )
{
	Host_ReadConfiguration( readDefault );
}

const char *CEngineClient::Key_LookupBinding( const char *pBinding )
{
	return ::Key_NameForBinding( pBinding );
}

const char *CEngineClient::Key_BindingForKey( ButtonCode_t &code )
{
	return ::Key_BindingForKey( code );
}

void CEngineClient::StartKeyTrapMode( void )
{
	Key_StartTrapMode();
}

bool CEngineClient::CheckDoneKeyTrapping( ButtonCode_t &code )
{
	return Key_CheckDoneTrapping( code );
}

bool CEngineClient::IsInGame( void )
{
	return cl.IsActive();
}

bool CEngineClient::IsConnected( void )
{
	return cl.IsConnected();
}

bool CEngineClient::IsDrawingLoadingImage( void )
{
	return scr_drawloading;
}

void CEngineClient::Con_NPrintf( int pos, const char *fmt, ... )
{
	va_list		argptr;
	char		text[4096];
	va_start (argptr, fmt);
	Q_vsnprintf(text, sizeof( text ), fmt, argptr);
	va_end (argptr);

	::Con_NPrintf( pos, "%s", text );
}

void CEngineClient::Con_NXPrintf( const struct con_nprint_s *info, const char *fmt, ... )
{
	va_list		argptr;
	char		text[4096];
	va_start (argptr, fmt);
	Q_vsnprintf(text, sizeof( text ), fmt, argptr);
	va_end (argptr);

	::Con_NXPrintf( info, "%s", text );
}

IMaterial *CEngineClient::TraceLineMaterialAndLighting( const Vector &start, const Vector &end, 
		                                 Vector &diffuseLightColor, Vector &baseColor )
{
	return BrushModel_GetLightingAndMaterial( start, end, diffuseLightColor, baseColor );
}

int	CEngineClient::IsBoxVisible( const Vector& mins, const Vector& maxs ) 
{
	return CM_BoxVisible( mins, maxs, Map_VisCurrent(), CM_ClusterPVSSize() );
}

int	CEngineClient::IsBoxInViewCluster( const Vector& mins, const Vector& maxs )
{
	// See comments in Map_VisCurrentCluster for why we might get a negative number.
	int curCluster = Map_VisCurrentCluster();
	if ( curCluster < 0 )
		return false;

	byte pvs[MAX_MAP_LEAFS/8];
	const byte *ppvs = CM_Vis( pvs, sizeof(pvs), curCluster, DVIS_PVS );
	return CM_BoxVisible(mins, maxs, ppvs, sizeof(pvs) );
}

float CEngineClient::Time()
{
	return Sys_FloatTime();
}

void CEngineClient::Sound_ExtraUpdate( void )
{
	// On xbox this is not necessary except for long pauses, so unhook this one
	if ( IsX360() )
		return;

	VPROF_BUDGET( "CEngineClient::Sound_ExtraUpdate()", VPROF_BUDGETGROUP_OTHER_SOUND );

	S_ExtraUpdate();
}

bool CEngineClient::CullBox ( const Vector& mins, const Vector& maxs )
{
	return R_CullBoxSkipNear( mins, maxs, g_Frustum );
}

const char *CEngineClient::GetGameDirectory( void )
{
	return com_gamedir;
}

const VMatrix& CEngineClient::WorldToScreenMatrix()
{
	// FIXME: this is only valid if we're currently rendering.  If not, it should use the player, or it really should pass one in.
	return g_EngineRenderer->WorldToScreenMatrix();
}

const VMatrix& CEngineClient::WorldToViewMatrix()
{
	// FIXME: this is only valid if we're currently rendering.  If not, it should use the player, or it really should pass one in.
	return g_EngineRenderer->ViewMatrix();
}

// Loads a game lump off disk
int	CEngineClient::GameLumpVersion( int lumpId ) const
{
	return Mod_GameLumpVersion( lumpId ); 
}

int	CEngineClient::GameLumpSize( int lumpId ) const 
{ 
	return Mod_GameLumpSize( lumpId ); 
}

bool CEngineClient::LoadGameLump( int lumpId, void* pBuffer, int size ) 
{ 
	return Mod_LoadGameLump( lumpId, pBuffer, size ); 
}

// Returns the number of leaves in the level
int	CEngineClient::LevelLeafCount() const
{
	return host_state.worldbrush->numleafs;
}

ISpatialQuery* CEngineClient::GetBSPTreeQuery()
{
	return g_pToolBSPTree;
}

// Convert texlight to gamma...
void CEngineClient::LinearToGamma( float* linear, float* gamma )
{
	gamma[0] = LinearToTexture( linear[0] ) / 255.0f;
	gamma[1] = LinearToTexture( linear[1] ) / 255.0f;
	gamma[2] = LinearToTexture( linear[2] ) / 255.0f;
}

// Get the lightstyle value
float CEngineClient::LightStyleValue( int style )
{
	return ::LightStyleValue( style );
}


void CEngineClient::DrawPortals()
{
	R_DrawPortals();
}

// Computes light due to dynamic lighting at a point
// If the normal isn't specified, then it'll return the maximum lighting
void CEngineClient::ComputeDynamicLighting( Vector const& pt, Vector const* pNormal, Vector& color )
{
	::ComputeDynamicLighting( pt, pNormal, color );
}

// Computes light due to dynamic lighting at a point
// If the normal isn't specified, then it'll return the maximum lighting
void CEngineClient::ComputeLighting( const Vector& pt, const Vector* pNormal, bool bClamp, Vector& color, Vector *pBoxColors )
{
	::ComputeLighting( pt, pNormal, bClamp, color, pBoxColors );
}

// Returns the color of the ambient light
void CEngineClient::GetAmbientLightColor( Vector& color )
{
	dworldlight_t* pWorldLight = FindAmbientLight();
	if (!pWorldLight)
		color.Init( 0, 0, 0 );
	else
		VectorCopy( pWorldLight->intensity, color );
}

// Returns the dx support level
int	CEngineClient::GetDXSupportLevel()
{
	return g_pMaterialSystemHardwareConfig->GetDXSupportLevel();
}

bool CEngineClient::SupportsHDR()
{
	// deprecated.
//	Assert( 0 );
	return false;
}

void CEngineClient::Mat_Stub( IMaterialSystem *pMatSys )
{
	materials = pMatSys;
	
	// Pass the call to the model renderer.
	if ( g_pStudioRender )
		g_pStudioRender->Mat_Stub( pMatSys );
}

void CEngineClient::GetChapterName( char *pchBuff, int iMaxLength )
{
	serverGameDLL->GetSaveComment( pchBuff, iMaxLength, 0.0f, 0.0f, true );
}

char const *CEngineClient::GetLevelName( void )
{
	if ( sv.IsDedicated() )
	{
		return "Dedicated Server";
	}
	else if ( !cl.IsConnected() )
	{
		return "";
	}

	return cl.m_szLevelName;
}

bool CEngineClient::IsLevelMainMenuBackground( void )
{
	return sv.IsLevelMainMenuBackground();
}

void CEngineClient::GetMainMenuBackgroundName( char *dest, int destlen )
{
	CL_GetBackgroundLevelName( dest, destlen, false );
}

// Occlusion system control
void CEngineClient::SetOcclusionParameters( const OcclusionParams_t &params )
{
	OcclusionSystem()->SetOcclusionParameters( params.m_flMaxOccludeeArea, params.m_flMinOccluderArea );
}

//-----------------------------------------------------------------------------
// Purpose: Takes a trackerID and returns which player slot that user is in
//			returns 0 if no player found with that ID
//-----------------------------------------------------------------------------
int	CEngineClient::GetPlayerForUserID(int userID)
{
	if ( !cl.m_pUserInfoTable )
		return 0;

	for ( int i = 0; i < cl.m_nMaxClients; i++ )
	{
		player_info_t *pi = (player_info_t*) cl.m_pUserInfoTable->GetStringUserData( i, NULL );

		if ( !pi )
			continue;

		// Fixup from network order (little endian)
		if ( LittleLong( pi->userID ) == userID )
		{
			// return as entity index
			return (i+1);
		}
	}

	return 0;
}

#if !defined( NO_VOICE )
struct IVoiceTweak_s *CEngineClient::GetVoiceTweakAPI( void )
{
	return &g_VoiceTweakAPI;
}
#endif

void CEngineClient::EngineStats_BeginFrame( void )
{
	g_EngineStats.BeginFrame();
}

void CEngineClient::EngineStats_EndFrame( void )
{
	g_EngineStats.EndFrame();
}

void CEngineClient::FireEvents()
{
	// Run any events queued up for this frame
	CL_FireEvents();
}

void CEngineClient::CheckPoint( const char *pName )
{
	GetTestScriptMgr()->CheckPoint( pName );
}

int CEngineClient::GetLeavesArea( int *pLeaves, int nLeaves )
{
	if ( nLeaves == 0 )
		return -1;

	int iArea = host_state.worldbrush->leafs[pLeaves[0]].area;
	for ( int i=1; i < nLeaves; i++ )
	{
		int iTestArea = host_state.worldbrush->leafs[pLeaves[i]].area;
		if ( iTestArea != iArea )
			return -1;
	}

	return iArea;
}

bool CEngineClient::DoesBoxTouchAreaFrustum( const Vector &mins, const Vector &maxs, int iArea )
{
	const Frustum_t *pFrustum = GetAreaFrustum( iArea );
	return !R_CullBoxSkipNear( mins, maxs, *pFrustum );
}

//-----------------------------------------------------------------------------
// Sets the hearing origin
//-----------------------------------------------------------------------------
void CEngineClient::SetAudioState( const AudioState_t &audioState )
{
	Host_SetAudioState( audioState );
}


//-----------------------------------------------------------------------------
//
// Sentence API
//
//-----------------------------------------------------------------------------

int CEngineClient::SentenceGroupPick( int groupIndex, char *name, int nameLen )
{
	return VOX_GroupPick( groupIndex, name, nameLen );
}


int CEngineClient::SentenceGroupPickSequential( int groupIndex, char *name, int nameLen, int sentenceIndex, int reset )
{
	return VOX_GroupPickSequential( groupIndex, name, nameLen, sentenceIndex, reset );
}

int CEngineClient::SentenceIndexFromName( const char *pSentenceName )
{
	int sentenceIndex = -1;
	
	VOX_LookupString( pSentenceName, &sentenceIndex );
	
	return sentenceIndex;
}

const char *CEngineClient::SentenceNameFromIndex( int sentenceIndex )
{
	return VOX_SentenceNameFromIndex( sentenceIndex );
}


int CEngineClient::SentenceGroupIndexFromName( const char *pGroupName )
{
	return VOX_GroupIndexFromName( pGroupName );
}

const char *CEngineClient::SentenceGroupNameFromIndex( int groupIndex )
{
	return VOX_GroupNameFromIndex( groupIndex );
}


float CEngineClient::SentenceLength( int sentenceIndex )
{
	return VOX_SentenceLength( sentenceIndex );
}

void CEngineClient::DebugDrawPhysCollide( const CPhysCollide *pCollide, IMaterial *pMaterial, matrix3x4_t& transform, const color32 &color )
{
	::DebugDrawPhysCollide( pCollide, pMaterial, transform, color, false );
}

// Activates/deactivates an occluder...
void CEngineClient::ActivateOccluder( int nOccluderIndex, bool bActive )
{
	OcclusionSystem()->ActivateOccluder( nOccluderIndex, bActive );
}

bool CEngineClient::IsOccluded( const Vector &vecAbsMins, const Vector &vecAbsMaxs )
{
	return OcclusionSystem()->IsOccluded( vecAbsMins, vecAbsMaxs );
}

void *CEngineClient::SaveAllocMemory( size_t num, size_t size )
{
	return ::SaveAllocMemory( num, size );
}

void CEngineClient::SaveFreeMemory( void *pSaveMem )
{
	::SaveFreeMemory( pSaveMem );
}

INetChannelInfo *CEngineClient::GetNetChannelInfo( void )
{
	return (INetChannelInfo*)cl.m_NetChannel;
}

bool CEngineClient::IsPlayingDemo( void )
{
	return demoplayer->IsPlayingBack();
}

bool CEngineClient::IsRecordingDemo( void )
{
	return demorecorder->IsRecording();
}

bool CEngineClient::IsPlayingTimeDemo( void )
{
	return demoplayer->IsPlayingTimeDemo();
}

bool CEngineClient::IsPaused( void )
{
	return cl.IsPaused();
}

bool CEngineClient::IsTakingScreenshot( void )
{
	return cl_takesnapshot;
}

bool CEngineClient::IsHLTV( void )
{
	return cl.ishltv;
}

void CEngineClient::GetUILanguage( char *dest, int destlen )
{
	const char *pStr = cl_language.GetString();
	if ( pStr )
	{
		V_strncpy( dest, pStr, destlen );
	}
	else if ( IsX360() )
	{
		dest[0] = 0;
	}
}

//-----------------------------------------------------------------------------
// Can skybox be seen from a particular point?
//-----------------------------------------------------------------------------
SkyboxVisibility_t CEngineClient::IsSkyboxVisibleFromPoint( const Vector &vecPoint )
{
	// In the mat_fullbright 1 case, it's always visible 
	// (we may have no lighting in the level, and vrad is where LEAF_FLAGS_SKY is computed)
	if ( g_pMaterialSystemConfig->nFullbright == 1 )
		return SKYBOX_3DSKYBOX_VISIBLE;

	int nLeaf = CM_PointLeafnum( vecPoint );
	int nFlags = GetCollisionBSPData()->map_leafs[nLeaf].flags;
	if ( nFlags & LEAF_FLAGS_SKY )
		return SKYBOX_3DSKYBOX_VISIBLE;
	return ( nFlags & LEAF_FLAGS_SKY2D ) ? SKYBOX_2DSKYBOX_VISIBLE : SKYBOX_NOT_VISIBLE;
}

const char* CEngineClient::GetMapEntitiesString()
{
	return CM_EntityString();
}

bool CEngineClient::IsInEditMode( void )
{
	return g_bInEditMode;
}

bool CEngineClient::IsInCommentaryMode( void )
{
	return g_bInCommentaryMode;
}

float CEngineClient::GetScreenAspectRatio()
{
	return GetScreenAspect( );
}

int CEngineClient::GetAppID()
{
	return g_iSteamAppID;
}

void CEngineClient::SetOverlayBindProxy( int iOverlayID, void *pBindProxy )
{
	OverlayMgr()->SetOverlayBindProxy( iOverlayID, pBindProxy );
}

void CEngineClient::ChangeTeam( const char *pTeamName )
{
	g_pMatchmaking->ChangeTeam( pTeamName );
}

//-----------------------------------------------------------------------------
// Returns true if copy occured
//-----------------------------------------------------------------------------
bool CEngineClient::CopyFrameBufferToMaterial( const char *pMaterialName )
{
	if ( !IsX360() )
	{
		// not for PC
		Assert( 0 );
		return false;
	}

	IMaterial *pMaterial = materials->FindMaterial( pMaterialName, TEXTURE_GROUP_OTHER );
	if ( pMaterial->IsErrorMaterial() )
	{
		// unknown material
		return false;
	}

	bool bFound;
	IMaterialVar *pMaterialVar = pMaterial->FindVar( "$baseTexture", &bFound, false );
	if ( !bFound || pMaterialVar->GetType() != MATERIAL_VAR_TYPE_TEXTURE )
	{
		// lack of expected $basetexture
		return false;
	}

	ITexture *pTexture = pMaterialVar->GetTextureValue();
	if ( !pTexture || !pTexture->IsRenderTarget() )
	{
		// base texture is not a render target
		return false;
	}

	CMatRenderContextPtr pRenderContext( materials );

	int width, height;
	pRenderContext->GetRenderTargetDimensions( width, height );
	if ( width != pTexture->GetActualWidth() || height != pTexture->GetActualHeight() )
	{
		// better be matched, not supporting a disparate blit in this context
		// disparate blit may very well use same RT we are trying to copy into
		return false;
	}

	pRenderContext->CopyRenderTargetToTexture( pTexture );
	return true;
}


//-----------------------------------------------------------------------------
// Used by the color correction UI
//-----------------------------------------------------------------------------
void CEngineClient::GrabPreColorCorrectedFrame( int x, int y, int width, int height )
{
	colorcorrectiontools->GrabPreColorCorrectedFrame( x, y, width, height );
}

//-----------------------------------------------------------------------------
// Is hammer running?
//-----------------------------------------------------------------------------
bool CEngineClient::IsHammerRunning( ) const
{
	return IsPC() ? InEditMode() : false;
}

extern IAchievementMgr *g_pAchievementMgr;

//-----------------------------------------------------------------------------
// Sets achievement mgr
//-----------------------------------------------------------------------------
void CEngineClient::SetAchievementMgr( IAchievementMgr *pAchievementMgr )
{
	g_pAchievementMgr = pAchievementMgr;
}

//-----------------------------------------------------------------------------
// Gets achievement mgr
//-----------------------------------------------------------------------------
IAchievementMgr *CEngineClient::GetAchievementMgr() 
{
	return g_pAchievementMgr;
}


//-----------------------------------------------------------------------------
// Called by the client to determine violence settings for things like ragdoll
// fading.
//-----------------------------------------------------------------------------
bool CEngineClient::IsLowViolence()
{
	return g_bLowViolence;
}

const char *CEngineClient::GetMostRecentSaveGame( void )
{
	return saverestore->GetMostRecentlyLoadedFileName();
}

void CEngineClient::SetMostRecentSaveGame( const char *lpszFilename )
{
	saverestore->SetMostRecentSaveGame( lpszFilename );
}

//-----------------------------------------------------------------------------
// Called by gameui to hint the engine that an exiting process has started.
// The Engine needs to stabilize to a safe quiet state. More frames are going
// to and have to run, but the true exit will occur.
//-----------------------------------------------------------------------------
void CEngineClient::StartXboxExitingProcess()
{
	if ( IsPC() )
	{
		// not for PC
		return;
	}

	g_pInputSystem->StopRumble();

	// save out the achievements
	g_pAchievementMgr->SaveGlobalStateIfDirty( false );

	S_StopAllSounds( true );

	// Shutdown QMS, need to go back to single threaded
	Host_AllowQueuedMaterialSystem( false );
}

bool CEngineClient::IsSaveInProgress()
{
	return saverestore->IsSaveInProgress();
}

extern IXboxSystem *g_pXboxSystem;

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
uint CEngineClient::OnStorageDeviceAttached( void )
{
	return g_pXboxSystem->OpenContainers();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CEngineClient::OnStorageDeviceDetached( void )
{
	XBX_SetStorageDeviceId( XBX_INVALID_STORAGE_ID );
	g_pXboxSystem->CloseContainers();
}

void CEngineClient::ResetDemoInterpolation( void )
{
	if( demorecorder->IsRecording() )
		demorecorder->ResetDemoInterpolation();
	if (demoplayer->IsPlayingBack() )
		demoplayer->ResetDemoInterpolation();
}


//-----------------------------------------------------------------------------
// The client DLL serves out this interface
//-----------------------------------------------------------------------------
IBaseClientDLL *g_ClientDLL = NULL;
IPrediction	*g_pClientSidePrediction = NULL;
IClientRenderTargets *g_pClientRenderTargets = NULL;
IClientEntityList *entitylist = NULL;
ICenterPrint *centerprint = NULL;
IClientLeafSystemEngine *clientleafsystem = NULL;
bool g_bClientLeafSystemV1;
ClientClass *g_pClientClassHead = NULL;

ClientClass *ClientDLL_GetAllClasses( void )
{
	if ( g_ClientDLL )
		return g_ClientDLL->GetAllClasses();
	else
		return g_pClientClassHead;
}

static void ClientDLL_InitRecvTableMgr()
{
	// Register all the receive tables.
	RecvTable *pRecvTables[MAX_DATATABLES];
	int nRecvTables = 0;
	for ( ClientClass *pCur = ClientDLL_GetAllClasses(); pCur; pCur=pCur->m_pNext )
	{
		ErrorIfNot( 
			nRecvTables < ARRAYSIZE( pRecvTables ), 
			("ClientDLL_InitRecvTableMgr: overflowed MAX_DATATABLES")
			);
		
		pRecvTables[nRecvTables] = pCur->m_pRecvTable;
		++nRecvTables;
	}

	RecvTable_Init( pRecvTables, nRecvTables );
}


static void ClientDLL_ShutdownRecvTableMgr()
{
	RecvTable_Term();
}

CreateInterfaceFn ClientDLL_GetFactory( void )
{
	return g_ClientFactory;
}

//-----------------------------------------------------------------------------
// Purpose: Loads the client DLL
// Input  :  - 
//-----------------------------------------------------------------------------
bool ClientDLL_Load()
{
	Assert ( !g_ClientDLLModule );

	// loads the client.dll, but ensures that the client dll is running under Steam
	// this will have to be undone when we want mods to be able to run
	g_ClientDLLModule = g_pFileSystem->LoadModule( "client", "GAMEBIN", false );
	if ( g_ClientDLLModule )
	{
		g_ClientFactory = Sys_GetFactory( g_ClientDLLModule );
		if ( g_ClientFactory )
		{
			g_ClientDLL = (IBaseClientDLL *)g_ClientFactory( CLIENT_DLL_INTERFACE_VERSION, NULL );
			// this is to ensure the old format of the string table is used for clients version 13 and older.
			// when the client version gets revved, there will need to be an else that sets this bool to true
			g_bClientGameDLLGreaterThanV13 = false;
			if ( !g_ClientDLL )
			{
				Sys_Error( "Could not get client.dll interface from library client" );
			}
		}
		else
		{
			Sys_Error( "Could not find factory interface in library client" );
		}
	}
	else
	{	
		// library failed to load
		Sys_Error( "Could not load library client" );
	}

	// Load the client render targets interface from the client .dll
	// NOTE: Its OK if this returns NULL, as some mods won't provide the interface and will just use the default behavior of the engine
	g_pClientRenderTargets = (IClientRenderTargets *)g_ClientFactory( CLIENTRENDERTARGETS_INTERFACE_VERSION, NULL );
	return true;
}

void InitExtraClientCmdCanExecuteVars()
{	
	// This can go away when we ship a client DLL with the FCVAR_CLIENTCMD_CAN_EXECUTE flag set on these cvars/concommands.
	Cmd_AddClientCmdCanExecuteVar( "cancelselect" );
	Cmd_AddClientCmdCanExecuteVar( "menuselect" );
	Cmd_AddClientCmdCanExecuteVar( "playgamesound" );
	Cmd_AddClientCmdCanExecuteVar( "_cl_classmenuopen" );
	Cmd_AddClientCmdCanExecuteVar( "cl_buy_favorite" );
	Cmd_AddClientCmdCanExecuteVar( "voice_modenable" );
	Cmd_AddClientCmdCanExecuteVar( "togglescores" );

	Cmd_AddClientCmdCanExecuteVar( "spec_next" );
	Cmd_AddClientCmdCanExecuteVar( "spec_prev" );
	Cmd_AddClientCmdCanExecuteVar( "spec_mode" );
	Cmd_AddClientCmdCanExecuteVar( "spec_menu" );
	Cmd_AddClientCmdCanExecuteVar( "spec_autodirector" );
	Cmd_AddClientCmdCanExecuteVar( "overview_zoom" );
	Cmd_AddClientCmdCanExecuteVar( "overview_mode" );
	Cmd_AddClientCmdCanExecuteVar( "overview_health" );
	Cmd_AddClientCmdCanExecuteVar( "overview_names" );
	Cmd_AddClientCmdCanExecuteVar( "overview_tracks" );
	Cmd_AddClientCmdCanExecuteVar( "overview_locked" );
	Cmd_AddClientCmdCanExecuteVar( "overview_alpha" );

	Cmd_AddClientCmdCanExecuteVar( "playgamesound" );
}

//-----------------------------------------------------------------------------
// Purpose: Inits the client .dll
//-----------------------------------------------------------------------------
void ClientDLL_Init( void )
{
	extern void CL_SetSteamCrashComment();

	// Assert ClientDLL_Load successfully created these interfaces, as we need them to init properly
	Assert ( g_ClientDLL );
	Assert ( g_ClientFactory );

	// this will get updated after we load a map, but this gets video info if we sys_error() prior to loading a map
	CL_SetSteamCrashComment();

	if ( g_ClientDLL )
	{
		COM_TimestampedLog( "g_ClientDLL->Init" );

		if ( !g_ClientDLL->Init(g_AppSystemFactory, g_AppSystemFactory, &g_ClientGlobalVariables ) )
		{
			Sys_Error("Client.dll Init() in library client failed.");
		}

		if ( g_ClientFactory )
		{
			COM_TimestampedLog( "g_pClientSidePrediction->Init" );

			// Load the prediction interface from the client .dll
			g_pClientSidePrediction = (IPrediction *)g_ClientFactory( VCLIENT_PREDICTION_INTERFACE_VERSION, NULL );
			if ( !g_pClientSidePrediction )
			{
				Sys_Error( "Could not get IPrediction interface from library client" );
			}
			g_pClientSidePrediction->Init();

			entitylist = ( IClientEntityList  *)g_ClientFactory( VCLIENTENTITYLIST_INTERFACE_VERSION, NULL );
			if ( !entitylist )
			{
				Sys_Error( "Could not get client entity list interface from library client" );
			}

			centerprint = ( ICenterPrint * )g_ClientFactory( VCENTERPRINT_INTERFACE_VERSION, NULL );
			if ( !centerprint )
			{
				Sys_Error( "Could not get centerprint interface from library client" );
			}

			clientleafsystem = ( IClientLeafSystemEngine *)g_ClientFactory( CLIENTLEAFSYSTEM_INTERFACE_VERSION, NULL );
			if ( clientleafsystem )
			{
				g_bClientLeafSystemV1 = false;
			}
			else if ( !clientleafsystem )
			{
				clientleafsystem = ( IClientLeafSystemEngine *)g_ClientFactory( CLIENTLEAFSYSTEM_INTERFACE_VERSION_1, NULL );
				if ( !clientleafsystem )
				{
					Sys_Error( "Could not get client leaf system interface from library client" );
				}
				else
				{
					g_bClientLeafSystemV1 = true;
				}
			}

			toolframework->ClientInit( g_ClientFactory );
		}

		// Don't want TF2 running less than DX 8
		if ( g_pMaterialSystemHardwareConfig && g_pMaterialSystemHardwareConfig->GetDXSupportLevel() < 80 )
		{
			if ( ( Q_stricmp( COM_GetModDirectory(), "tf" ) == 0 ) || ( Q_stricmp( COM_GetModDirectory(), "ep2" ) == 0 ) || ( Q_stricmp( COM_GetModDirectory(), "portal" ) == 0 ) )
			{
				Sys_Error( "This game has a minimum requirement of DirectX 8.0 to run properly." );
			}
		}
	}

	COM_TimestampedLog( "ClientDLL_InitRecvTableMgr" );

	ClientDLL_InitRecvTableMgr();
	
	InitExtraClientCmdCanExecuteVars();
}

//-----------------------------------------------------------------------------
// Purpose: Shuts down the client .dll
//-----------------------------------------------------------------------------
void ClientDLL_Shutdown( void )
{
	toolframework->ClientShutdown();

	ClientDLL_ShutdownRecvTableMgr();

	vgui::ivgui()->RunFrame();
	
	materials->UncacheAllMaterials();

	vgui::ivgui()->RunFrame();

	g_pClientSidePrediction->Shutdown();

	entitylist = NULL;
	g_pClientSidePrediction = NULL;
	g_ClientFactory = NULL;
	centerprint = NULL;

	g_ClientDLL->Shutdown();
}

//-----------------------------------------------------------------------------
// Purpose: Unloads the client .dll
// Input  :  - 
//-----------------------------------------------------------------------------
void ClientDLL_Unload()
{
	FileSystem_UnloadModule( g_ClientDLLModule );

	g_ClientDLL = NULL;
	g_ClientDLLModule = NULL;
	g_pClientRenderTargets = NULL;

}

//-----------------------------------------------------------------------------
// Purpose: Called when the game initializes and whenever the vid_mode is changed
//   so the HUD can reinitialize itself.
//-----------------------------------------------------------------------------
void ClientDLL_HudVidInit( void )
{
	g_ClientDLL->HudVidInit();
}

//-----------------------------------------------------------------------------
// Purpose: Allow client .dll to modify input data
//-----------------------------------------------------------------------------

void ClientDLL_ProcessInput( void )
{
	if ( !g_ClientDLL )
		return;

	VPROF("ClientDLL_ProcessInput");
	g_ClientDLL->HudProcessInput( cl.IsConnected() );
}


void ClientDLL_FrameStageNotify( ClientFrameStage_t frameStage )
{
	if ( !g_ClientDLL )
		return;

	g_ClientDLL->FrameStageNotify( frameStage );
}


//-----------------------------------------------------------------------------
// Purpose: Allow client .dll to think
//-----------------------------------------------------------------------------
void ClientDLL_Update( void )
{
	if ( sv.IsDedicated() )
		return;

	if ( !g_ClientDLL )
		return;

	g_ClientDLL->HudUpdate( true );
}



void ClientDLL_VoiceStatus( int entindex, bool bTalking )
{
	if( g_ClientDLL )
		g_ClientDLL->VoiceStatus( entindex, bTalking );
}




