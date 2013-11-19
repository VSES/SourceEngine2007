//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//
#include "cbase.h"
#include "smoke_fog_overlay.h"
#include "materialsystem/IMaterial.h"
#include "materialsystem/IMesh.h"
#include "view.h"
#include "ClientEffectPrecacheSystem.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

static IMaterial *g_pSmokeFogMaterial = NULL;


float		g_SmokeFogOverlayAlpha;
Vector		g_SmokeFogOverlayColor;

CLIENTEFFECT_REGISTER_BEGIN( PrecacheSmokeFogOverlay )
CLIENTEFFECT_MATERIAL( "particle/screenspace_fog" )
CLIENTEFFECT_REGISTER_END()

void InitSmokeFogOverlay()
{
	TermSmokeFogOverlay();
	
	g_SmokeFogOverlayAlpha = 0;

	if(materials)
	{
		g_pSmokeFogMaterial = materials->FindMaterial( "particle/screenspace_fog", TEXTURE_GROUP_CLIENT_EFFECTS );
		if(g_pSmokeFogMaterial)
			g_pSmokeFogMaterial->IncrementReferenceCount();
	}
}


void TermSmokeFogOverlay()
{
	if(g_pSmokeFogMaterial)
	{
		g_pSmokeFogMaterial->DecrementReferenceCount();
		g_pSmokeFogMaterial = NULL;
	}
}


void DrawSmokeFogOverlay()
{
	if(g_SmokeFogOverlayAlpha == 0 || !g_pSmokeFogMaterial || !materials)
		return;

	// Hard-coded for now..
	g_SmokeFogOverlayColor.Init( 0.3, 0.3, 0.3 );
	
	CMatRenderContextPtr pRenderContext( materials );

	pRenderContext->MatrixMode( MATERIAL_PROJECTION );
	pRenderContext->LoadIdentity();
	pRenderContext->Ortho( 0, 0, 1, 1, -99999, 99999 );

	pRenderContext->MatrixMode( MATERIAL_VIEW );
	pRenderContext->LoadIdentity();

	pRenderContext->MatrixMode( MATERIAL_MODEL );
	pRenderContext->LoadIdentity();

	IMesh* pMesh = pRenderContext->GetDynamicMesh( false, NULL, NULL, g_pSmokeFogMaterial );
	CMeshBuilder meshBuilder;

	static float dist = 10;

	Vector vColor = g_SmokeFogOverlayColor;
	vColor.x = min(max(vColor.x, 0), 1);
	vColor.y = min(max(vColor.y, 0), 1);
	vColor.z = min(max(vColor.z, 0), 1);
	float alpha = min(max(g_SmokeFogOverlayAlpha, 0), 1);

	meshBuilder.Begin( pMesh, MATERIAL_QUADS, 1 );

	meshBuilder.Position3f( 0, 0, dist );
	meshBuilder.Color4f( vColor.x, vColor.y, vColor.z, alpha );
	meshBuilder.TexCoord2f( 0, 0.0f, 0.0f );
	meshBuilder.AdvanceVertex();

	meshBuilder.Position3f( 0, 1, dist );
	meshBuilder.Color4f( vColor.x, vColor.y, vColor.z, alpha );
	meshBuilder.TexCoord2f( 0, 0.0f, 0.0f );
	meshBuilder.AdvanceVertex();

	meshBuilder.Position3f( 1, 1, dist );
	meshBuilder.Color4f( vColor.x, vColor.y, vColor.z, alpha );
	meshBuilder.TexCoord2f( 0, 0.0f, 0.0f );
	meshBuilder.AdvanceVertex();

	meshBuilder.Position3f( 1, 0, dist );
	meshBuilder.Color4f( vColor.x, vColor.y, vColor.z, alpha );
	meshBuilder.TexCoord2f( 0, 0.0f, 0.0f );
	meshBuilder.AdvanceVertex();

	meshBuilder.End();
	pMesh->Draw();
}
