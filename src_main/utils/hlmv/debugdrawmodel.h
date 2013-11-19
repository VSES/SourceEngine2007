//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#ifndef DEBUGDRAWMODEL_H
#define DEBUGDRAWMODEL_H
#ifdef _WIN32
#pragma once
#endif

int DebugDrawModel( IStudioRender *pStudioRender, DrawModelInfo_t& info, matrix3x4_t *pBoneToWorld, const Vector &modelOrigin,
			        int *pLodUsed, float *pMetric, int flags = STUDIORENDER_DRAW_ENTIRE_MODEL );
int DebugDrawModelNormals( IStudioRender *pStudioRender, DrawModelInfo_t& info, matrix3x4_t *pBoneToWorld, const Vector &modelOrigin,
			        int *pLodUsed, float *pMetric, int flags = STUDIORENDER_DRAW_ENTIRE_MODEL );
int DebugDrawModelTangentS( IStudioRender *pStudioRender, DrawModelInfo_t& info, matrix3x4_t *pBoneToWorld, const Vector &modelOrigin,
			        int *pLodUsed, float *pMetric, int flags = STUDIORENDER_DRAW_ENTIRE_MODEL );
int DebugDrawModelTangentT( IStudioRender *pStudioRender, DrawModelInfo_t& info, matrix3x4_t *pBoneToWorld, const Vector &modelOrigin,
			        int *pLodUsed, float *pMetric, int flags = STUDIORENDER_DRAW_ENTIRE_MODEL );
int DebugDrawModelBoneWeights( IStudioRender *pStudioRender, DrawModelInfo_t& info, matrix3x4_t *pBoneToWorld, const Vector &modelOrigin,
			        int *pLodUsed, float *pMetric, int flags = STUDIORENDER_DRAW_ENTIRE_MODEL  );
int DebugDrawModelBadVerts( IStudioRender *pStudioRender, DrawModelInfo_t& info, matrix3x4_t *pBoneToWorld, const Vector &modelOrigin,
							  int *pLodUsed, float *pMetric, int flags = STUDIORENDER_DRAW_ENTIRE_MODEL  );
int DebugDrawModelWireframe( IStudioRender *pStudioRender, DrawModelInfo_t& info, matrix3x4_t *pBoneToWorld, const Vector &modelOrigin,
						   int *pLodUsed, float *pMetric, const Vector &color, int flags = STUDIORENDER_DRAW_ENTIRE_MODEL );

#endif // DEBUGDRAWMODEL_H
