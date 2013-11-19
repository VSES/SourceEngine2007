//====== Copyright © 1996-2004, Valve Corporation, All rights reserved. =======
//
// A class representing an MDL
//
//=============================================================================

#ifndef DMEMDL_H
#define DMEMDL_H

#ifdef _WIN32
#pragma once
#endif

#include "movieobjects/dmeshape.h"
#include "datacache/imdlcache.h"

#include "mathlib/vector.h"
#include <string>
#include <vector>


//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------
class CDmeTransform;
class IMorph;
class IMaterial;
struct SubdivMesh_t;
class IMesh;
class CDmeDrawSettings;
struct matrix3x4_t;
class CStudioHdr;


//-----------------------------------------------------------------------------
// A class representing an MDL
//-----------------------------------------------------------------------------
class CDmeMDL : public CDmeShape
{
	DEFINE_ELEMENT( CDmeMDL, CDmeShape );

public:
	virtual void Draw( const matrix3x4_t& shapeToWorld, CDmeDrawSettings *pDrawSettings = NULL );
	virtual void Resolve();

	void DrawInEngine( bool bDrawInEngine );
	bool IsDrawingInEngine() const;

	void SetMDL( MDLHandle_t handle );
	MDLHandle_t GetMDL( ) const;
	float GetRadius() const; // NOTE: This radius is one that is centered at the origin
	void GetBoundingSphere( Vector &vecCenter, float &flRadius );

	// Computes bone-to-world transforms
	void SetUpBones( const matrix3x4_t& shapeToWorld, int nMaxBoneCount, matrix3x4_t *pOutputMatrices );

public:
	CDmaColor m_Color;
	CDmaVar<int> m_nSkin;
	CDmaVar<int> m_nBody;
	CDmaVar<int> m_nSequence;
	CDmaVar<int> m_nLOD;
	CDmaVar<float> m_flPlaybackRate;
	CDmaVar<float> m_flTime;
	CDmaVar<Vector> m_vecViewTarget;
	CDmaVar<bool> m_bWorldSpaceViewTarget;

	// Returns the bounding box for the model
	void GetBoundingBox( Vector *pMins, Vector *pMaxs ) const;

private:
	// Transform from DME to engine coordinates
	void EngineToDmeMatrix( matrix3x4_t& dmeToEngine );

	// Addref/Release the MDL handle
	void ReferenceMDL( const char *pMDLName );
	void UnreferenceMDL();

	// Returns a mask indicating which bones to set up
	int BoneMask( );

	// Computes bone-to-world transforms
	void SetUpBones( CStudioHdr &studioHdr, const matrix3x4_t& shapeToWorld, int nMaxBoneCount, matrix3x4_t *pOutputMatrices );

	MDLHandle_t m_MDLHandle;
	bool m_bDrawInEngine;
};

#endif // DMEMDL_H
