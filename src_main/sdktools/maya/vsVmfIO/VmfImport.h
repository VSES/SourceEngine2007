//======= Copyright © 1996-2006, Valve Corporation, All rights reserved. ======
//
// Purpose:
//
//=============================================================================

#ifndef VMFIMPORT_H
#define VMFIMPORT_H
#if defined( _WIN32 )
#pragma once
#endif

#pragma warning(disable : 4702)   // warning C4702: unreachable code


// Standard includes
#include <string>
#include <map>
#include <stack>


// Maya includes
#include <maya/MComputation.h>
#include <maya/MProgressWindow.h>
#include <maya/MObject.h>
#include <maya/MObjectArray.h>
#include <maya/MDagModifier.h>
#include <maya/MFloatArray.h>
#include <maya/MFloatPoint.h>
#include <maya/MFloatPointArray.h>
#include <maya/MIntArray.h>


// Valve includes
#include "valveMaya.h"


// Local Includes
#include "vbsp.h"


class CVmfImport
{
public:
	CVmfImport();

	virtual ~CVmfImport();

	void Clear();

	MStatus DoIt(
		const MString &filename,
		MDagModifier *pDagModifier,
		MObjectArray *pUndoObjs );

	static CVmfImport *GetVmfImporter() { return m_pVmfImport; }

	void AddSideCallback(
		mapbrush_t *pMapBrush,
		side_t *pSide,
		const brush_texture_t &brushTexture,
		const Vector &pp0,
		const Vector &pp1,
		const Vector &pp2 );

	void MapBrushToMayaCallback(
		mapbrush_t *mapBrush );

	void ImportLightCallback(
		const char *pId,
		const char *pOrigin,
		const char *pLight,
		const char *pLightHDR,
		const char *pLightScaleHDR,
		const char *pQuadraticAttn );

	void ImportLightSpotCallback(
		const char *pId,
		const char *pOrigin,
		const char *pAngles,
		const char *pPitch,
		const char *pLight,
		const char *pLightHDR,
		const char *pLightScaleHDR,
		const char *pQuadraticAttn,
		const char *pInnerCone,
		const char *pCone,
		const char *pExponent );

	void ImportLightDynamicCallback(
		const char *pId,
		const char *pOrigin,
		const char *pAngles,
		const char *pPitch,
		const char *pLight,
		const char *pQuadraticAttn,
		const char *pInnerCone,
		const char *pCone,
		const char *pBrightness,
		const char *pDistance,
		const char *pSpotlightRadius );

	void ImportLightEnvironmentCallback(
		const char *pId,
		const char *pOrigin,
		const char *pAngles,
		const char *pPitch,
		const char *pLight,
		const char *pLightHDR,
		const char *pLightScaleHDR,
		const char *pAmbient,
		const char *pAmbientHDR,
		const char *pAmbientScaleHDR,
		const char *pSunSpreadAngle );

	void ImportModelCallback(
		const char *pModel,
		const char *pId,
		const char *pAngles,
		const char *pOrigin );

	bool m_optBakeUVs;
	bool m_optImportTools;
	bool m_optImportDev;
	bool m_optImportBrushes;
	bool m_optImportLights;
	bool m_optImportModels;
	MString m_optTextureArchive;

protected:
	MObject m_pObj;
	MObject m_brushObj;
	MObject m_lightObj;
	MObject m_mdlObj;
	MDagModifier *m_pDagModifier;
	MObjectArray *m_pUndoObjs;
	MComputation m_mComputation;
	MProgressWindow m_mProgressWindow;
	static CVmfImport *m_pVmfImport;

	struct SeInfo_s
	{
		MObject m_sgObj;	// Shading Group Object
		MObject m_btObj;	// Base Texture File Node Object
		MObject m_b2Obj;	// Bump Map 2D Node Object
		int m_width;		// Width of the texture
		int m_height;		// Height of the texture
	};

	std::map< std::string, SeInfo_s > m_texMap;

	struct SideInfo_s
	{
		MFloatPoint m_p0;	// Plane points
		MFloatPoint m_p1;
		MFloatPoint m_p2;
		brush_texture_t m_brushTexture;
	};

	typedef std::map< const side_t *, SideInfo_s > SideMap_t;

	struct Solid_s
	{
		mapbrush_t *m_pMapBrush;
		SideMap_t m_sideMap;

		Solid_s(
			mapbrush_t *pMapBrush )
		: m_pMapBrush( pMapBrush )
		{
		}
	};

	std::stack< Solid_s > m_solids;

	std::map< std::string, MDagPath > m_mdlCache;

	bool GetBrushCounts(
		const mapbrush_t *pMapBrush,
		SideMap_t &sideMap,
		uint &nPolygons,
		uint &nVertices,
		uint &nFaceVertices ) const;

	static void GetSideCounts(
		bool dispOnly,
		const side_t *pSide,
		uint &nPolygons,
		uint &nVertices,
		uint &nFaceVertices );

	static void GetMeshWindingData(
		const SideInfo_s &sideInfo,
		const winding_t *pWinding,
		MFloatPointArray &vertexArray,
		MIntArray &polygonCounts,
		MIntArray &polygonConnects,
		MFloatArray &uArray,
		MFloatArray &vArray );

	static void GetMeshDispData(
		const SideInfo_s &sideInfo,
		const winding_t *pWinding,
		const mapdispinfo_t *pDispInfo,
		MFloatPointArray &vertexArray,
		MIntArray &polygonCounts,
		MIntArray &polygonConnects,
		MFloatArray &uArray,
		MFloatArray &vArray );

	void AssignMaterial(
		const MDagPath &mDagPath,
		MIntArray &cIndices,
		const mapbrush_t *pMapBrush,
		const brush_texture_t &brushTexture,
		MFloatArray &uArray,
		MFloatArray &vArray,
		MFloatArray &luArray,
		MFloatArray &lvArray );

	void CreateShadingGroup(
		const MDagPath &mDagPath,
		MIntArray &cIndices,
		const MString &shaderType,		// The type of shader to create (lambert, blinn, etc...)
		const MString &shaderName,		// What to name the created shading group & shader (Shading group suffixed with SG)
		MObject &sgObj,					// The Maya MObject of the created shading group node
		MObject &ssObj );				// The Maya MObject of the created surface shader node

	MObject CreateFileNode(
		const MString &textureName );

	MObject CreateBump2DNode();

	MObject CreateProjectionNode(
		const MDagPath &mDagPath,
		const brush_texture_t &brushTexture,
		int width,
		int height );

	MObject GetShadingEngine(
		const MDagPath &mDagPath,
		MIntArray &cIndices,
		const brush_texture_t &brushTexture,
		int &width,
		int &height );

	static void AssignProjectedUVs(
		const MDagPath &mDagPath,
		MObject &cObj,
		const brush_texture_t &brushTexture,
		const int width,
		const int height,
		MFloatArray &uArray,
		MFloatArray &vArray,
		MFloatArray &luArray,
		MFloatArray &lvArray );

	void ConvertBrush(
		const mapbrush_t *pMapBrush,
		SideMap_t &sideMap );

	static void GetTexturePathsFromMaterialPath(
		const MString &materialPath,
		MString &baseTexturePath,
		MString &bumpMapPath );

	MString GetTgaFromTexturePath(
		const MString &texturePath ) const;

	bool SkipSide( const SideInfo_s &sideInfo ) const;

	MString TextureArchive(
		const MString &filename ) const;

	static MPlug GetNextDefaultRenderUtilitiesPlug();
};

#endif // VMFIMPORT_H
