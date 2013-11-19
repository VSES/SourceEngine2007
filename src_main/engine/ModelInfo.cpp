//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: 
//
// $Workfile:     $
// $NoKeywords: $
//===========================================================================//
#include "engine/ivmodelinfo.h"
#include "filesystem.h"
#include "gl_model_private.h"
#include "modelloader.h"
#include "l_studio.h"
#include "cmodel_engine.h"
#include "server.h"
#include "r_local.h"
#include "materialsystem/imaterialsystem.h"
#include "materialsystem/imaterial.h"
#include "lightcache.h"
#include "istudiorender.h"
#include "utldict.h"
#include "filesystem_engine.h"
#include "client.h"
#include "sys_dll.h"
#include "gl_rsurf.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//-----------------------------------------------------------------------------
// Gets the lighting center
//-----------------------------------------------------------------------------
static void R_StudioGetLightingCenter( IClientRenderable *pRenderable, studiohdr_t* pStudioHdr, const Vector& origin,
								const QAngle &angles, Vector* pLightingOrigin )
{
	Assert( pLightingOrigin );
	matrix3x4_t matrix;
	AngleMatrix( angles, origin, matrix );
	R_ComputeLightingOrigin( pRenderable, pStudioHdr, matrix, *pLightingOrigin );
}

static int R_StudioBodyVariations( studiohdr_t *pstudiohdr )
{
	mstudiobodyparts_t *pbodypart;
	int i, count;

	if ( !pstudiohdr )
		return 0;

	count = 1;
	pbodypart = pstudiohdr->pBodypart( 0 );

	// Each body part has nummodels variations so there are as many total variations as there
	// are in a matrix of each part by each other part
	for ( i = 0; i < pstudiohdr->numbodyparts; i++ )
	{
		count = count * pbodypart[i].nummodels;
	}
	return count;
}

static int ModelFrameCount( model_t *model )
{
	int count = 1;

	if ( !model )
		return count;

	if ( model->type == mod_sprite )
	{
		return model->sprite.numframes;
	}
	else if ( model->type == mod_studio )
	{
		count = R_StudioBodyVariations( ( studiohdr_t * )modelloader->GetExtraData( model ) );
	}

	if ( count < 1 )
		count = 1;
	
	return count;
}



//-----------------------------------------------------------------------------
// shared implementation of IVModelInfo
//-----------------------------------------------------------------------------
class CModelInfo : public IVModelInfoClient
{
public:
	virtual const model_t *FindOrLoadModel( const char *name ) const;
	virtual const char *GetModelName( const model_t *model ) const;
	virtual void GetModelBounds( const model_t *model, Vector& mins, Vector& maxs ) const;
	virtual void GetModelRenderBounds( const model_t *model, Vector& mins, Vector& maxs ) const;
	virtual int GetModelFrameCount( const model_t *model ) const;
	virtual int GetModelType( const model_t *model ) const;
	virtual void *GetModelExtraData( const model_t *model );
	virtual bool ModelHasMaterialProxy( const model_t *model ) const;
	virtual bool IsTranslucent( const model_t *model ) const;
	virtual bool IsModelVertexLit( const model_t *model ) const;
	virtual bool IsTranslucentTwoPass( const model_t *model ) const;
	virtual void RecomputeTranslucency( const model_t *model, int nSkin, int nBody, void /*IClientRenderable*/ *pClientRenderable );
	virtual int	GetModelMaterialCount( const model_t *model ) const;
	virtual void GetModelMaterials( const model_t *model, int count, IMaterial** ppMaterials );
	virtual void GetIlluminationPoint( const model_t *model, IClientRenderable *pRenderable, const Vector& origin, 
		const QAngle& angles, Vector* pLightingOrigin );
	virtual int GetModelContents( int modelIndex ) const;
	vcollide_t *GetVCollide( const model_t *model ) const;
	vcollide_t *GetVCollide( int modelIndex ) const;
	virtual const char *GetModelKeyValueText( const model_t *model );
	virtual bool GetModelKeyValue( const model_t *model, CUtlBuffer &buf );
	virtual float GetModelRadius( const model_t *model );
	virtual studiohdr_t *GetStudiomodel( const model_t *mod );
	virtual int GetModelSpriteWidth( const model_t *model ) const;
	virtual int GetModelSpriteHeight( const model_t *model ) const;

	virtual const studiohdr_t *FindModel( const studiohdr_t *pStudioHdr, void **cache, char const *modelname ) const;
	virtual const studiohdr_t *FindModel( void *cache ) const;
	virtual virtualmodel_t *GetVirtualModel( const studiohdr_t *pStudioHdr ) const;
	virtual byte *GetAnimBlock( const studiohdr_t *pStudioHdr, int iBlock ) const;

	byte *LoadAnimBlock( model_t *model, const studiohdr_t *pStudioHdr, int iBlock, cache_user_t *cache ) const;

	// NOTE: These aren't in the server version, but putting them here makes this code easier to write
	// Sets/gets a map-specified fade range
	virtual void					SetLevelScreenFadeRange( float flMinSize, float flMaxSize ) {}
	virtual void					GetLevelScreenFadeRange( float *pMinArea, float *pMaxArea ) const { *pMinArea = 0; *pMaxArea = 0; }

	// Sets/gets a map-specified per-view fade range
	virtual void					SetViewScreenFadeRange( float flMinSize, float flMaxSize ) {}

	// Computes fade alpha based on distance fade + screen fade
	virtual unsigned char			ComputeLevelScreenFade( const Vector &vecAbsOrigin, float flRadius, float flFadeScale ) const { return 0; }
	virtual unsigned char			ComputeViewScreenFade( const Vector &vecAbsOrigin, float flRadius, float flFadeScale ) const { return 0; }

	int GetAutoplayList( const studiohdr_t *pStudioHdr, unsigned short **pAutoplayList ) const;
	CPhysCollide *GetCollideForVirtualTerrain( int index );
	virtual int GetSurfacepropsForVirtualTerrain( int index ) { return CM_SurfacepropsForDisp(index); }

	virtual bool IsUsingFBTexture( const model_t *model, int nSkin, int nBody, void /*IClientRenderable*/ *pClientRenderable ) const;

	virtual MDLHandle_t	GetCacheHandle( const model_t *model ) const { return ( model->type == mod_studio ) ? model->studio : MDLHANDLE_INVALID; }

	// Returns planes of non-nodraw brush model surfaces
	virtual int GetBrushModelPlaneCount( const model_t *model ) const;
	virtual void GetBrushModelPlane( const model_t *model, int nIndex, cplane_t &plane, Vector *pOrigin ) const;
};

const model_t *CModelInfo::FindOrLoadModel( const char *name ) const
{
	// find the cached model from the server or client
	const model_t *pModel = GetModel( GetModelIndex( name ) );
	if ( pModel )
		return pModel;

	// load the model
	return modelloader->GetModelForName( name, IModelLoader::FMODELLOADER_CLIENTDLL );
}

const char *CModelInfo::GetModelName( const model_t *pModel ) const
{
	if ( !pModel )
	{
		return "?";
	}

	return modelloader->GetName( pModel );
}

void CModelInfo::GetModelBounds( const model_t *model, Vector& mins, Vector& maxs ) const
{
	VectorCopy( model->mins, mins );
	VectorCopy( model->maxs, maxs );
}

void CModelInfo::GetModelRenderBounds( const model_t *model, Vector& mins, Vector& maxs ) const
{
	if (!model)
	{
		mins.Init(0,0,0);
		maxs.Init(0,0,0);
		return;
	}

	switch( model->type )
	{
	case mod_studio:
		{
			studiohdr_t *pStudioHdr = ( studiohdr_t * )modelloader->GetExtraData( (model_t*)model );
			Assert( pStudioHdr );

			// NOTE: We're not looking at the sequence box here, although we could
			if (!VectorCompare( vec3_origin, pStudioHdr->view_bbmin ) || !VectorCompare( vec3_origin, pStudioHdr->view_bbmax ))
			{
				// clipping bounding box
				VectorCopy ( pStudioHdr->view_bbmin, mins);
				VectorCopy ( pStudioHdr->view_bbmax, maxs);
			}
			else
			{
				// movement bounding box
				VectorCopy ( pStudioHdr->hull_min, mins);
				VectorCopy ( pStudioHdr->hull_max, maxs);
			}
		}
		break;

	case mod_brush:
		VectorCopy( model->mins, mins );
		VectorCopy( model->maxs, maxs );
		break;

	default:
		mins.Init( 0, 0, 0 );
		maxs.Init( 0, 0, 0 );
		break;
	}
}

int CModelInfo::GetModelSpriteWidth( const model_t *model ) const
{
	// We must be a sprite to make this query
	if ( model->type != mod_sprite )
		return 0;

	return model->sprite.width;
}

int CModelInfo::GetModelSpriteHeight( const model_t *model ) const
{
	// We must be a sprite to make this query
	if ( model->type != mod_sprite )
		return 0;

	return model->sprite.height;
}

int CModelInfo::GetModelFrameCount( const model_t *model ) const
{
	return ModelFrameCount( ( model_t *)model );
}

int CModelInfo::GetModelType( const model_t *model ) const
{
	return model ? model->type : -1;
}

void *CModelInfo::GetModelExtraData( const model_t *model )
{
	return modelloader->GetExtraData( (model_t *)model );
}


//-----------------------------------------------------------------------------
// Purpose: Translate "cache" pointer into model_t, or lookup model by name
//-----------------------------------------------------------------------------
const studiohdr_t *CModelInfo::FindModel( const studiohdr_t *pStudioHdr, void **cache, char const *modelname ) const
{
	const model_t *model = (model_t *)*cache;

	if (!model)
	{
		// FIXME: what do I pass in here?
		model = modelloader->GetModelForName( modelname, IModelLoader::FMODELLOADER_SERVER );
		*cache = (void *)model;
	}

	return (const studiohdr_t *)modelloader->GetExtraData( (model_t *)model );
}


//-----------------------------------------------------------------------------
// Purpose: Translate "cache" pointer into model_t
//-----------------------------------------------------------------------------
const studiohdr_t *CModelInfo::FindModel( void *cache ) const
{
	return g_pMDLCache->GetStudioHdr( (MDLHandle_t)cache );
}


//-----------------------------------------------------------------------------
// Purpose: Return virtualmodel_t block associated with model_t
//-----------------------------------------------------------------------------
virtualmodel_t *CModelInfo::GetVirtualModel( const studiohdr_t *pStudioHdr ) const
{
	MDLHandle_t handle = (MDLHandle_t)pStudioHdr->virtualModel;
	return g_pMDLCache->GetVirtualModelFast( pStudioHdr, handle );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
byte *CModelInfo::GetAnimBlock( const studiohdr_t *pStudioHdr, int nBlock ) const
{
	MDLHandle_t handle = (MDLHandle_t)pStudioHdr->virtualModel;
	return g_pMDLCache->GetAnimBlock( handle, nBlock );
}

int CModelInfo::GetAutoplayList( const studiohdr_t *pStudioHdr, unsigned short **pAutoplayList ) const
{
	MDLHandle_t handle = (MDLHandle_t)pStudioHdr->virtualModel;
	return g_pMDLCache->GetAutoplayList( handle, pAutoplayList );
}


//-----------------------------------------------------------------------------
// Purpose: bind studiohdr_t support functions to engine
// FIXME: This should be moved into studio.cpp?
//-----------------------------------------------------------------------------
const studiohdr_t *studiohdr_t::FindModel( void **cache, char const *pModelName ) const
{
	MDLHandle_t handle = g_pMDLCache->FindMDL( pModelName );
	*cache = (void*)handle;
	return g_pMDLCache->GetStudioHdr( handle );
}

virtualmodel_t *studiohdr_t::GetVirtualModel( void ) const
{
	if ( numincludemodels == 0 )
		return NULL;
	return g_pMDLCache->GetVirtualModelFast( this, (MDLHandle_t)virtualModel );
}

byte *studiohdr_t::GetAnimBlock( int i ) const
{
	return g_pMDLCache->GetAnimBlock( (MDLHandle_t)virtualModel, i );
}

int	studiohdr_t::GetAutoplayList( unsigned short **pOut ) const
{
	return g_pMDLCache->GetAutoplayList( (MDLHandle_t)virtualModel, pOut );
}

const studiohdr_t *virtualgroup_t::GetStudioHdr( void ) const
{
	return g_pMDLCache->GetStudioHdr( (MDLHandle_t)cache );
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
bool CModelInfo::ModelHasMaterialProxy( const model_t *model ) const
{
	// Should we add skin & model to this function like IsUsingFBTexture()?
	return (model && (model->flags & MODELFLAG_MATERIALPROXY));
}

bool CModelInfo::IsTranslucent( const model_t *model ) const
{
	return (model && (model->flags & MODELFLAG_TRANSLUCENT));
}

bool CModelInfo::IsModelVertexLit( const model_t *model ) const
{
	// Should we add skin & model to this function like IsUsingFBTexture()?
	return (model && (model->flags & MODELFLAG_VERTEXLIT));
}

bool CModelInfo::IsTranslucentTwoPass( const model_t *model ) const
{
	return (model && (model->flags & MODELFLAG_TRANSLUCENT_TWOPASS));
}

bool CModelInfo::IsUsingFBTexture( const model_t *model, int nSkin, int nBody, void /*IClientRenderable*/ *pClientRenderable ) const
{
	bool bMightUseFbTextureThisFrame = (model && (model->flags & MODELFLAG_STUDIOHDR_USES_FB_TEXTURE));

	if ( bMightUseFbTextureThisFrame )
	{
		// Check each material's NeedsPowerOfTwoFrameBufferTexture() virtual func
		switch( model->type )
		{
			case mod_brush:
			{
				for (int i = 0; i < model->brush.nummodelsurfaces; ++i)
				{
					SurfaceHandle_t surfID = SurfaceHandleFromIndex( model->brush.firstmodelsurface+i, model->brush.pShared );
					IMaterial* material = MSurf_TexInfo( surfID, model->brush.pShared )->material;
					if ( material != NULL )
					{
						if ( material->NeedsPowerOfTwoFrameBufferTexture() )
						{
							return true;
						}
					}
				}
			}
			break;

			case mod_studio:
			{
				IMaterial *pMaterials[ 128 ];
				int materialCount = g_pStudioRender->GetMaterialListFromBodyAndSkin( model->studio, nSkin, nBody, ARRAYSIZE( pMaterials ), pMaterials );
				for ( int i = 0; i < materialCount; i++ )
				{
					if ( pMaterials[i] != NULL )
					{
						// Bind material first so all material proxies execute
						CMatRenderContextPtr pRenderContext( g_pMaterialSystem );
						pRenderContext->Bind( pMaterials[i], pClientRenderable );

						if ( pMaterials[i]->NeedsPowerOfTwoFrameBufferTexture() )
						{
							return true;
						}
					}
				}
			}
			break;
		}
	}

	return false;
}

void CModelInfo::RecomputeTranslucency( const model_t *model, int nSkin, int nBody, void /*IClientRenderable*/ *pClientRenderable )
{
	if ( model != NULL )
	{
		Mod_RecomputeTranslucency( (model_t *)model, nSkin, nBody, pClientRenderable );
	}
}

int	CModelInfo::GetModelMaterialCount( const model_t *model ) const
{
	if (!model)
		return 0;
	return Mod_GetMaterialCount( (model_t *)model );
}

void CModelInfo::GetModelMaterials( const model_t *model, int count, IMaterial** ppMaterials )
{
	if (model)
		Mod_GetModelMaterials( (model_t *)model, count, ppMaterials );
}

void CModelInfo::GetIlluminationPoint( const model_t *model, IClientRenderable *pRenderable, const Vector& origin, 
	const QAngle& angles, Vector* pLightingOrigin )
{
	Assert( model->type == mod_studio );
	studiohdr_t* pStudioHdr = (studiohdr_t*)GetModelExtraData(model);
	if (pStudioHdr)
	{
		R_StudioGetLightingCenter( pRenderable, pStudioHdr, origin, angles, pLightingOrigin );
	}
	else
	{
		*pLightingOrigin = origin;
	}
}

int CModelInfo::GetModelContents( int modelIndex ) const
{
	const model_t *pModel = GetModel( modelIndex );
	if ( pModel )
	{
		switch( pModel->type )
		{
		case mod_brush:
			return CM_InlineModelContents( modelIndex-1 );
		
		// BUGBUG: Studio contents?
		case mod_studio:
			return CONTENTS_SOLID;
		}
	}
	return 0;
}

#if !defined( _RETAIL )
extern double g_flAccumulatedModelLoadTimeVCollideSync;
#endif

vcollide_t *CModelInfo::GetVCollide( const model_t *pModel ) const
{
	if ( !pModel )
		return NULL;

	if ( pModel->type == mod_studio )
	{
#if !defined( _RETAIL )
		double t1 = Plat_FloatTime();
#endif
		vcollide_t *col = g_pMDLCache->GetVCollide( pModel->studio );
#if !defined( _RETAIL )
		double t2 = Plat_FloatTime();
		g_flAccumulatedModelLoadTimeVCollideSync += ( t2 - t1 );
#endif
		return col;
	}

	int i = GetModelIndex( GetModelName( pModel ) );
	if ( i >= 0 )
	{
		return GetVCollide( i );
	}

	return NULL;
}

vcollide_t *CModelInfo::GetVCollide( int modelIndex ) const
{
	// First model (index 0 )is is empty
	// Second model( index 1 ) is the world, then brushes/submodels, then players, etc.
	// So, we must subtract 1 from the model index to map modelindex to CM_ index
	// in cmodels, 0 is the world, then brushes, etc.
	if ( modelIndex < MAX_MODELS )
	{
		const model_t *pModel = GetModel( modelIndex );
		if ( pModel )
		{
			switch( pModel->type )
			{
			case mod_brush:
				return CM_GetVCollide( modelIndex-1 );
			case mod_studio:
				{
#if !defined( _RETAIL )
					double t1 = Plat_FloatTime();
#endif
					vcollide_t *col = g_pMDLCache->GetVCollide( pModel->studio );
#if !defined( _RETAIL )
					double t2 = Plat_FloatTime();
					g_flAccumulatedModelLoadTimeVCollideSync += ( t2 - t1 );
#endif
					return col;
				}
			}
		}
		else
		{
			// we may have the cmodels loaded and not know the model/mod->type yet
			return CM_GetVCollide( modelIndex-1 );
		}
	}
	return NULL;
}

// Client must instantiate a KeyValues, which will be filled by this method
const char *CModelInfo::GetModelKeyValueText( const model_t *model )
{
	if (!model || model->type != mod_studio)
		return NULL;

	studiohdr_t* pStudioHdr = g_pMDLCache->GetStudioHdr( model->studio );
	if (!pStudioHdr)
		return NULL;

	return pStudioHdr->KeyValueText();
}



bool CModelInfo::GetModelKeyValue( const model_t *model, CUtlBuffer &buf )
{
	if (!model || model->type != mod_studio)
		return false;

	studiohdr_t* pStudioHdr = g_pMDLCache->GetStudioHdr( model->studio );
	if (!pStudioHdr)
		return false;

	if ( pStudioHdr->numincludemodels == 0)
	{
		buf.PutString( pStudioHdr->KeyValueText() );
		return true;
	}

	virtualmodel_t *pVM = GetVirtualModel( pStudioHdr );

	if (pVM)
	{
		for (int i = 0; i < pVM->m_group.Count(); i++)
		{
			const studiohdr_t* pSubStudioHdr = pVM->m_group[i].GetStudioHdr();
			if (pSubStudioHdr && pSubStudioHdr->KeyValueText())
			{
				buf.PutString( pSubStudioHdr->KeyValueText() );
			}
		}
	}
	return true;
}


//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *model - 
// Output : float
//-----------------------------------------------------------------------------
float CModelInfo::GetModelRadius( const model_t *model )
{
	if ( !model )
		return 0.0f;
	return model->radius;
}


//-----------------------------------------------------------------------------
// Lovely studiohdrs
//-----------------------------------------------------------------------------
studiohdr_t *CModelInfo::GetStudiomodel( const model_t *model )
{
	if ( model->type == mod_studio )
		return g_pMDLCache->GetStudioHdr( model->studio );

	return NULL;
}

CPhysCollide *CModelInfo::GetCollideForVirtualTerrain( int index )
{
	return CM_PhysCollideForDisp( index );
}

// Returns planes of non-nodraw brush model surfaces
int CModelInfo::GetBrushModelPlaneCount( const model_t *model ) const
{
	if ( !model || model->type != mod_brush )
		return 0;

	return R_GetBrushModelPlaneCount( model );
}

void CModelInfo::GetBrushModelPlane( const model_t *model, int nIndex, cplane_t &plane, Vector *pOrigin ) const
{
	if ( !model || model->type != mod_brush )
		return;

	plane = R_GetBrushModelPlane( model, nIndex, pOrigin );
}



//-----------------------------------------------------------------------------
// implementation of IVModelInfo for server
//-----------------------------------------------------------------------------
class CModelInfoServer : public CModelInfo
{
public:
	virtual const model_t *GetModel( int modelindex ) const;
	virtual int GetModelIndex( const char *name ) const;
	virtual void GetModelMaterialColorAndLighting( const model_t *model, const Vector& origin,
		const QAngle& angles, trace_t* pTrace, Vector& lighting, Vector& matColor );
};

const model_t *CModelInfoServer::GetModel( int modelindex ) const
{
	return sv.GetModel( modelindex );
}

int CModelInfoServer::GetModelIndex( const char *name ) const
{
	return sv.LookupModelIndex( name );
}

void CModelInfoServer::GetModelMaterialColorAndLighting( const model_t *model, const Vector& origin,
	const QAngle& angles, trace_t* pTrace, Vector& lighting, Vector& matColor )
{
	Msg( "GetModelMaterialColorAndLighting:  Available on client only!\n" );
}


static CModelInfoServer	g_ModelInfoServer;
EXPOSE_SINGLE_INTERFACE_GLOBALVAR( CModelInfoServer, IVModelInfo, VMODELINFO_SERVER_INTERFACE_VERSION, g_ModelInfoServer );

// Expose IVModelInfo to the engine
IVModelInfo *modelinfo = &g_ModelInfoServer;


#ifndef SWDS
//-----------------------------------------------------------------------------
// implementation of IVModelInfo for client
//-----------------------------------------------------------------------------
class CModelInfoClient : public CModelInfo
{
public:
	// Sets/gets a map-specified fade range
	virtual void	SetLevelScreenFadeRange( float flMinSize, float flMaxSize );
	virtual void	GetLevelScreenFadeRange( float *pMinArea, float *pMaxArea ) const;

	// Sets/gets a map-specified per-view fade range
	virtual void	SetViewScreenFadeRange( float flMinSize, float flMaxSize );

	// Computes fade alpha based on distance fade + screen fade
	virtual unsigned char ComputeLevelScreenFade( const Vector &vecAbsOrigin, float flRadius, float flFadeScale ) const;
	virtual unsigned char ComputeViewScreenFade( const Vector &vecAbsOrigin, float flRadius, float flFadeScale ) const;

	virtual const model_t *GetModel( int modelindex ) const;
	virtual int GetModelIndex( const char *name ) const;
	virtual void GetModelMaterialColorAndLighting( const model_t *model, const Vector& origin,
		const QAngle& angles, trace_t* pTrace, Vector& lighting, Vector& matColor );

private:
	struct ScreenFadeInfo_t
	{
		float	m_flMinScreenWidth;	
		float	m_flMaxScreenWidth;	
		float	m_flFalloffFactor;
	};

	// Sets/gets a map-specified fade range
	void SetScreenFadeRange( float flMinSize, float flMaxSize, ScreenFadeInfo_t *pFade );
	unsigned char ComputeScreenFade( const Vector &vecAbsOrigin, float flRadius, float flFadeScale, const ScreenFadeInfo_t &fade ) const;

	ScreenFadeInfo_t m_LevelFade;
	ScreenFadeInfo_t m_ViewFade;
};

const model_t *CModelInfoClient::GetModel( int modelindex ) const
{
	return cl.GetModel( modelindex );
}

int CModelInfoClient::GetModelIndex( const char *name ) const
{
	return cl.LookupModelIndex( name );
}


//-----------------------------------------------------------------------------
// Sets/gets a map-specified fade range
//-----------------------------------------------------------------------------
void CModelInfoClient::SetScreenFadeRange( float flMinSize, float flMaxSize, ScreenFadeInfo_t *pFade )
{
	pFade->m_flMinScreenWidth = flMinSize;
	pFade->m_flMaxScreenWidth = flMaxSize;
	if ( pFade->m_flMaxScreenWidth <= pFade->m_flMinScreenWidth )
	{
		pFade->m_flMaxScreenWidth = pFade->m_flMinScreenWidth;
	}

	if (pFade->m_flMaxScreenWidth != pFade->m_flMinScreenWidth)
	{
		pFade->m_flFalloffFactor = 255.0f / (pFade->m_flMaxScreenWidth - pFade->m_flMinScreenWidth);
	}
	else
	{
		pFade->m_flFalloffFactor = 255.0f;
	}
}

void CModelInfoClient::SetLevelScreenFadeRange( float flMinSize, float flMaxSize )
{
	SetScreenFadeRange( flMinSize, flMaxSize, &m_LevelFade );
}

void CModelInfoClient::GetLevelScreenFadeRange( float *pMinArea, float *pMaxArea ) const
{
	*pMinArea = m_LevelFade.m_flMinScreenWidth;
	*pMaxArea = m_LevelFade.m_flMaxScreenWidth;
}


//-----------------------------------------------------------------------------
// Sets/gets a map-specified per-view fade range
//-----------------------------------------------------------------------------
void CModelInfoClient::SetViewScreenFadeRange( float flMinSize, float flMaxSize )
{
	SetScreenFadeRange( flMinSize, flMaxSize, &m_ViewFade );
}


//-----------------------------------------------------------------------------
// Computes fade alpha based on distance fade + screen fade
//-----------------------------------------------------------------------------
inline unsigned char CModelInfoClient::ComputeScreenFade( const Vector &vecAbsOrigin,
	float flRadius, float flFadeScale, const ScreenFadeInfo_t &fade ) const
{
	if ( ( fade.m_flMinScreenWidth <= 0 ) || (flFadeScale <= 0.0f) )
		return 255;

	CMatRenderContextPtr pRenderContext( materials );

	float flPixelWidth = pRenderContext->ComputePixelWidthOfSphere( vecAbsOrigin, flRadius ) / flFadeScale;

	unsigned char alpha = 0;
	if ( flPixelWidth > fade.m_flMinScreenWidth )
	{
		if ( (fade.m_flMaxScreenWidth >= 0) && (flPixelWidth < fade.m_flMaxScreenWidth) )
		{
			int nAlpha = fade.m_flFalloffFactor * (flPixelWidth - fade.m_flMinScreenWidth);
			alpha = clamp( nAlpha, 0, 255 );
		}
		else
		{
			alpha = 255;
		}
	}

	return alpha;
}

unsigned char CModelInfoClient::ComputeLevelScreenFade( const Vector &vecAbsOrigin, float flRadius, float flFadeScale ) const
{
	if ( IsXbox() )
	{
		return 255;
	}
	else
	{
		return ComputeScreenFade( vecAbsOrigin, flRadius, flFadeScale, m_LevelFade );
	}
}

unsigned char CModelInfoClient::ComputeViewScreenFade( const Vector &vecAbsOrigin, float flRadius, float flFadeScale ) const
{
	return ComputeScreenFade( vecAbsOrigin, flRadius, flFadeScale, m_ViewFade );
}


//-----------------------------------------------------------------------------
// A method to get the material color + texture coordinate
//-----------------------------------------------------------------------------
IMaterial* BrushModel_GetLightingAndMaterial( const Vector &start, 
	const Vector &end, Vector &diffuseLightColor, Vector &baseColor)
{
	float textureS, textureT;
	IMaterial *material;

	// TEMP initialize these values until we can find why R_LightVec is not assigning values to them
	textureS = 0;
	textureT = 0;

	SurfaceHandle_t surfID = R_LightVec( start, end, true, diffuseLightColor, &textureS, &textureT );
	if( !IS_SURF_VALID( surfID ) || !MSurf_TexInfo( surfID ) )
	{
//		ConMsg( "didn't hit anything\n" );
		return 0;
	}
	else
	{
		material = MSurf_TexInfo( surfID )->material;
		material->GetLowResColorSample( textureS, textureT, baseColor.Base() );
//		ConMsg( "%s: diff: %f %f %f base: %f %f %f\n", material->GetName(), diffuseLightColor[0], diffuseLightColor[1], diffuseLightColor[2], baseColor[0], baseColor[1], baseColor[2] );
		return material;
	}
}

void CModelInfoClient::GetModelMaterialColorAndLighting( const model_t *model, const Vector & origin,
	const QAngle & angles, trace_t* pTrace, Vector& lighting, Vector& matColor )
{
	switch( model->type )
	{
	case mod_brush:
		{
			Vector origin_l, delta, delta_l;
			VectorSubtract( pTrace->endpos, pTrace->startpos, delta );

			// subtract origin offset
			VectorSubtract (pTrace->startpos, origin, origin_l);

			// rotate start and end into the models frame of reference
			if (angles[0] || angles[1] || angles[2])
			{
				Vector forward, right, up;
				AngleVectors (angles, &forward, &right, &up);

				// transform the direction into the local space of this entity
				delta_l[0] = DotProduct (delta, forward);
				delta_l[1] = -DotProduct (delta, right);
				delta_l[2] = DotProduct (delta, up);
			}
			else
			{
				VectorCopy( delta, delta_l );
			}

			Vector end_l;
			VectorMA( origin_l, 1.1f, delta_l, end_l );

			R_LightVecUseModel( ( model_t * )model );
			BrushModel_GetLightingAndMaterial( origin_l, end_l, lighting, matColor );
			R_LightVecUseModel();
			return;
		}

	case mod_studio:
		{
			// FIXME: Need some way of getting the material!
			matColor.Init( 0.5f, 0.5f, 0.5f );

			// Get the lighting at the point
			LightingState_t lightingState;
			LightcacheGetDynamic_Stats stats;
			LightcacheGetDynamic( pTrace->endpos, lightingState, stats, LIGHTCACHEFLAGS_STATIC|LIGHTCACHEFLAGS_DYNAMIC|LIGHTCACHEFLAGS_LIGHTSTYLE|LIGHTCACHEFLAGS_ALLOWFAST );
			// Convert the light parameters into something studiorender can digest
			LightDesc_t desc[MAXLOCALLIGHTS];
			int count = 0;
			for (int i = 0; i < lightingState.numlights; ++i)
			{
				if (WorldLightToMaterialLight( lightingState.locallight[i], desc[count] ))
				{
					++count;
				}
			}

			// Ask studiorender to figure out the lighting
			g_pStudioRender->ComputeLighting( lightingState.r_boxcolor,
				count, desc, pTrace->endpos, pTrace->plane.normal, lighting );
			return;
		}
	}
}

static CModelInfoClient	g_ModelInfoClient;
EXPOSE_SINGLE_INTERFACE_GLOBALVAR( CModelInfoClient, IVModelInfoClient, VMODELINFO_CLIENT_INTERFACE_VERSION, g_ModelInfoClient );

// Expose IVModelInfo to the engine
IVModelInfoClient *modelinfoclient = &g_ModelInfoClient;
#endif
