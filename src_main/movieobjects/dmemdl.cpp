//====== Copyright © 1996-2004, Valve Corporation, All rights reserved. =======
//
// Purpose: 
//
//=============================================================================
#include "movieobjects/dmemdl.h"
#include "movieobjects/dmetransform.h"
#include "movieobjects_interfaces.h"
#include "datamodel/dmelementfactoryhelper.h"
#include "datacache/imdlcache.h"
#include "istudiorender.h"
#include "bone_setup.h"
#include "tier3/tier3.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

// Fixme, this might not be the best spot for this
class IGlobalFlexController
{
public:
	virtual int				FindGlobalFlexController( char const *name ) = 0;
	virtual char const		*GetGlobalFlexControllerName( int idx ) = 0;
};


//-----------------------------------------------------------------------------
// Expose this class to the scene database 
//-----------------------------------------------------------------------------
IMPLEMENT_ELEMENT_FACTORY( DmeMDL, CDmeMDL );


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CDmeMDL::OnConstruction()
{
	m_bDrawInEngine = false;
	m_MDLHandle = MDLHANDLE_INVALID;
//	SetAttributeValueElement( "transform", CreateElement< CDmeTransform >() );
//	SetAttributeValue( "mdlfilename", "models/alyx.mdl" );
	m_Color.InitAndSet( this, "color", Color( 255, 255, 255, 255 ) );
	m_nSkin.InitAndSet( this, "skin", 0 );
	m_nBody.InitAndSet( this, "body", 0 );
	m_nSequence.InitAndSet( this, "sequence", 0 );
	m_nLOD.InitAndSet( this, "lod", 0 );
	m_flPlaybackRate.InitAndSet( this, "playbackrate", 30.0f );
	m_flTime.InitAndSet( this, "time", 0.0f );
	m_vecViewTarget.Init( this, "viewTarget" );
	m_bWorldSpaceViewTarget.Init( this, "worldSpaceViewTarget" );
}

void CDmeMDL::OnDestruction()
{
	UnreferenceMDL();
}


void CDmeMDL::SetMDL( MDLHandle_t handle )
{
	UnreferenceMDL();
	m_MDLHandle = handle;
	if ( m_MDLHandle != MDLHANDLE_INVALID )
	{
		g_pMDLCache->AddRef( m_MDLHandle );

		studiohdr_t *pHdr = g_pMDLCache->GetStudioHdr( m_MDLHandle );

		if ( pHdr )
		{
			for ( LocalFlexController_t i = LocalFlexController_t(0); i < pHdr->numflexcontrollers; ++i )
			{
				pHdr->pFlexcontroller( i )->localToGlobal = i;
			}
		}
	}

	Vector vecMins, vecMaxs;
	GetBoundingBox( &vecMins, &vecMaxs );

	m_vecViewTarget.Set( Vector( 0.0f, vecMaxs.z, 100.0f ) );
}

MDLHandle_t CDmeMDL::GetMDL( ) const
{
	return m_MDLHandle;
}

	
//-----------------------------------------------------------------------------
// Addref/Release the MDL handle
//-----------------------------------------------------------------------------
void CDmeMDL::ReferenceMDL( const char *pMDLName )
{
	if ( !g_pMDLCache )
		return;

	if ( pMDLName && pMDLName[0] )
	{
		Assert( m_MDLHandle == MDLHANDLE_INVALID );
		m_MDLHandle = g_pMDLCache->FindMDL( pMDLName );
	}
}

void CDmeMDL::UnreferenceMDL()
{
	if ( !g_pMDLCache )
		return;

	if ( m_MDLHandle != MDLHANDLE_INVALID )
	{
		g_pMDLCache->Release( m_MDLHandle );
		m_MDLHandle = MDLHANDLE_INVALID;
	}
}


//-----------------------------------------------------------------------------
// This function gets called whenever an attribute changes
//-----------------------------------------------------------------------------
void CDmeMDL::Resolve()
{
	/*
	IDmAttribute *pMDLFilename = GetAttribute( "mdlfilename" );
	if ( pMDLFilename && pMDLFilename->HasFlag( FATTRIB_DIRTY ) )
	{
		UnreferenceMDL();
		ReferenceMDL( GetAttributeValueString( "mdlfilename" ) );
		return;
	}
	*/
}

	
//-----------------------------------------------------------------------------
// Loads the model matrix based on the transform
//-----------------------------------------------------------------------------
void CDmeMDL::DrawInEngine( bool bDrawInEngine )
{
	m_bDrawInEngine = bDrawInEngine;
}

bool CDmeMDL::IsDrawingInEngine() const
{
	return m_bDrawInEngine;
}
	

//-----------------------------------------------------------------------------
// Returns the bounding box for the model
//-----------------------------------------------------------------------------
void CDmeMDL::GetBoundingBox( Vector *pMins, Vector *pMaxs ) const
{
	if ( !g_pMaterialSystem || !g_pMDLCache || !g_pStudioRender )
	{
		pMins->Init();
		pMaxs->Init();
		return;
	}

	if ( m_MDLHandle == MDLHANDLE_INVALID )
	{
		pMins->Init();
		pMaxs->Init();
		return;
	}

	pMins->Init( FLT_MAX, FLT_MAX );
	pMaxs->Init( -FLT_MAX, -FLT_MAX );

	studiohdr_t *pStudioHdr = g_pMDLCache->GetStudioHdr( m_MDLHandle );
	if ( !VectorCompare( vec3_origin, pStudioHdr->view_bbmin ) || !VectorCompare( vec3_origin, pStudioHdr->view_bbmax ))
	{
		// look for view clip
		*pMins = pStudioHdr->view_bbmin;
		*pMaxs = pStudioHdr->view_bbmax;
	}
	else if ( !VectorCompare( vec3_origin, pStudioHdr->hull_min ) || !VectorCompare( vec3_origin, pStudioHdr->hull_max ))
	{
		// look for hull
		*pMins = pStudioHdr->hull_min;
		*pMaxs = pStudioHdr->hull_max;
	}

	// Else use the sequence box
	mstudioseqdesc_t &seqdesc = pStudioHdr->pSeqdesc( m_nSequence );
	VectorMin( seqdesc.bbmin, *pMins, *pMins );
	VectorMax( seqdesc.bbmax, *pMaxs, *pMaxs );
}


//-----------------------------------------------------------------------------
// Returns the radius of the model as measured from the origin
//-----------------------------------------------------------------------------
float CDmeMDL::GetRadius() const
{
	Vector vecMins, vecMaxs;
	GetBoundingBox( &vecMins, &vecMaxs );
	float flRadius = vecMaxs.Length();
	float flRadius2 = vecMins.Length();
	if ( flRadius2 > flRadius )
	{
		flRadius = flRadius2;
	}
	return flRadius;
}

	
//-----------------------------------------------------------------------------
// Transform from DME to engine coordinates
//-----------------------------------------------------------------------------
void CDmeMDL::EngineToDmeMatrix( matrix3x4_t& engineToDme )
{
	VMatrix rotation, rotationZ;
	MatrixBuildRotationAboutAxis( rotation, Vector( 1, 0, 0 ), -90 );
	MatrixBuildRotationAboutAxis( rotationZ, Vector( 0, 1, 0 ), -90 );
	ConcatTransforms( rotationZ.As3x4(), rotation.As3x4(), engineToDme );
}


//-----------------------------------------------------------------------------
// Returns a more accurate bounding sphere
//-----------------------------------------------------------------------------
void CDmeMDL::GetBoundingSphere( Vector &vecCenter, float &flRadius )
{
	Vector vecMins, vecMaxs, vecEngineCenter;
	GetBoundingBox( &vecMins, &vecMaxs );
	VectorAdd( vecMins, vecMaxs, vecEngineCenter );
	vecEngineCenter *= 0.5f;
	flRadius = vecMaxs.DistTo( vecEngineCenter );

	// Rotate the root transform to make it align with DMEs
	// DMEs up vector is the y axis
	if ( !m_bDrawInEngine )
	{
		matrix3x4_t engineToDme;
		EngineToDmeMatrix( engineToDme );
		VectorTransform( vecEngineCenter, engineToDme, vecCenter );
	}
	else
	{
		vecCenter = vecEngineCenter;
	}
}

	
//-----------------------------------------------------------------------------
// Draws the mesh
//-----------------------------------------------------------------------------
void CDmeMDL::Draw( const matrix3x4_t &shapeToWorld, CDmeDrawSettings *pDrawSettings /* = NULL */ )
{
	if ( !g_pMaterialSystem || !g_pMDLCache || !g_pStudioRender )
		return;

	if ( m_MDLHandle == MDLHANDLE_INVALID )
		return;

	// Color + alpha modulation
	Vector white( m_Color.r() / 255.0f, m_Color.g() / 255.0f, m_Color.b() / 255.0f );
	g_pStudioRender->SetColorModulation( white.Base() );
	g_pStudioRender->SetAlphaModulation( m_Color.a() / 255.0f );

	DrawModelInfo_t info;
	info.m_pStudioHdr = g_pMDLCache->GetStudioHdr( m_MDLHandle );
	info.m_pHardwareData = g_pMDLCache->GetHardwareData( m_MDLHandle );
	info.m_Decals = STUDIORENDER_DECAL_INVALID;
	info.m_Skin = m_nSkin;
	info.m_Body = m_nBody;
	info.m_HitboxSet = 0;
	info.m_pClientEntity = NULL;
	info.m_pColorMeshes = NULL;
	info.m_bStaticLighting = false;
	info.m_Lod = m_nLOD;

	// FIXME: Deal with lighting
	for ( int i = 0; i < 6; ++ i )
	{
		info.m_vecAmbientCube[i].Init( 1, 1, 1 );
	}

	info.m_nLocalLightCount = 0;
//	info.m_LocalLightDescs;
	
	matrix3x4_t *pBoneToWorld = g_pStudioRender->LockBoneMatrices( info.m_pStudioHdr->numbones );
	SetUpBones( shapeToWorld, info.m_pStudioHdr->numbones, pBoneToWorld );
	g_pStudioRender->UnlockBoneMatrices();

	Vector vecWorldViewTarget;
	if ( m_bWorldSpaceViewTarget )
	{
		vecWorldViewTarget = m_vecViewTarget;
	}
	else
	{
		VectorTransform( m_vecViewTarget, shapeToWorld, vecWorldViewTarget );
	}
	g_pStudioRender->SetEyeViewTarget( info.m_pStudioHdr, info.m_Body, vecWorldViewTarget );

	// FIXME: Why is this necessary!?!?!?
	CMatRenderContextPtr pRenderContext( g_pMaterialSystem );
	if ( !m_bDrawInEngine )
	{
		pRenderContext->CullMode(MATERIAL_CULLMODE_CCW);
	}

    // Set default flex values
	float *pFlexWeights = NULL;
	const int nFlexDescCount = info.m_pStudioHdr->numflexdesc;
	if ( nFlexDescCount )
	{
		CStudioHdr cStudioHdr( info.m_pStudioHdr, g_pMDLCache );
		float flexDefaults[ MAXSTUDIOFLEXCTRL * 4 ];
		memset( flexDefaults, 0, MAXSTUDIOFLEXCTRL * 4 * sizeof( float ) );

		g_pStudioRender->LockFlexWeights( info.m_pStudioHdr->numflexdesc, &pFlexWeights );
		cStudioHdr.RunFlexRules( flexDefaults, pFlexWeights );
		g_pStudioRender->UnlockFlexWeights();
	}

	Vector vecModelOrigin;
	MatrixGetColumn( shapeToWorld, 3, vecModelOrigin );
	g_pStudioRender->DrawModel( NULL, info, pBoneToWorld, pFlexWeights, NULL,
		vecModelOrigin, STUDIORENDER_DRAW_ENTIRE_MODEL );

	// FIXME: Why is this necessary!?!?!?
	if ( !m_bDrawInEngine )
	{
		pRenderContext->CullMode( MATERIAL_CULLMODE_CW );
	}
}


//-----------------------------------------------------------------------------
// Returns a mask indicating which bones to set up
//-----------------------------------------------------------------------------
int CDmeMDL::BoneMask( void )
{
	return BONE_USED_BY_VERTEX_AT_LOD( m_nLOD );
}

void CDmeMDL::SetUpBones( CStudioHdr &studioHdr, const matrix3x4_t& shapeToWorld, int nMaxBoneCount, matrix3x4_t *pBoneToWorld )
{
	// Default to middle of the pose parameter range
	float pPoseParameter[MAXSTUDIOPOSEPARAM];
	for ( int i = 0; i < MAXSTUDIOPOSEPARAM; ++i )
	{
		pPoseParameter[i] = 0.5f;
	}

	int nFrameCount = Studio_MaxFrame( &studioHdr, m_nSequence, pPoseParameter );
	if ( nFrameCount == 0 )
	{
		nFrameCount = 1;
	}
	float flCycle = ( m_flTime * m_flPlaybackRate ) / nFrameCount;

	// FIXME: We're always wrapping; may want to determing if we should clamp
	flCycle -= (int)(flCycle);

	Vector		pos[MAXSTUDIOBONES];
	Quaternion	q[MAXSTUDIOBONES];

	InitPose( &studioHdr, pos, q, BoneMask( ) );
	AccumulatePose( &studioHdr, NULL, pos, q, m_nSequence, flCycle, pPoseParameter, BoneMask( ), 1.0f, m_flTime );

	// FIXME: Try enabling this?
//	CalcAutoplaySequences( pStudioHdr, NULL, pos, q, pPoseParameter, BoneMask( ), flTime );

	// Root transform
	matrix3x4_t rootToWorld, temp;

	// Rotate the root transform to make it align with DMEs
	// DMEs up vector is the y axis
	if ( !m_bDrawInEngine )
	{
		matrix3x4_t engineToDme;
		EngineToDmeMatrix( engineToDme );
		ConcatTransforms( engineToDme, shapeToWorld, rootToWorld );
	}
	else
	{
		MatrixCopy( shapeToWorld, rootToWorld );
	}

	if ( nMaxBoneCount > studioHdr.numbones() )
	{
		nMaxBoneCount = studioHdr.numbones();
	}

	for ( int i = 0; i < nMaxBoneCount; i++ ) 
	{
		// If it's not being used, fill with NAN for errors
#ifdef _DEBUG
		if ( !(studioHdr.pBone( i )->flags & BoneMask()))
		{
			int j, k;
			for (j = 0; j < 3; j++)
			{
				for (k = 0; k < 4; k++)
				{
					pBoneToWorld[i][j][k] = VEC_T_NAN;
				}
			}
			continue;
		}
#endif

		matrix3x4_t boneMatrix;
		QuaternionMatrix( q[i], boneMatrix );
		MatrixSetColumn( pos[i], 3, boneMatrix );

		if (studioHdr.pBone(i)->parent == -1) 
		{
			ConcatTransforms( rootToWorld, boneMatrix, pBoneToWorld[i] );
		} 
		else 
		{
			ConcatTransforms( pBoneToWorld[ studioHdr.pBone(i)->parent ], boneMatrix, pBoneToWorld[i] );
		}
	}
}

void CDmeMDL::SetUpBones( const matrix3x4_t& shapeToWorld, int nMaxBoneCount, matrix3x4_t *pOutputMatrices )
{
	CStudioHdr studioHdr( g_pMDLCache->GetStudioHdr( m_MDLHandle ), g_pMDLCache );
	SetUpBones( studioHdr, shapeToWorld, nMaxBoneCount, pOutputMatrices );
}
