//======= Copyright © 1996-2006, Valve Corporation, All rights reserved. ======
//
// Purpose:
//
//=============================================================================

#ifndef VSTSHADER_H
#define VSTSHADER_H
#if defined( _WIN32 )
#pragma once
#endif


// Maya includes
#include <maya/MPxHwShaderNode.h>
#include <maya/MMessage.h>


// Valve includes
#include "tier1/utlstring.h"
#include "tier1/UtlStringMap.h"


// Local includes
#include "vsVtf.h"


//=============================================================================
//
//=============================================================================
class CVsVmtNode : public MPxHwShaderNode
{
public:
	CVsVmtNode();

	virtual ~CVsVmtNode();

	static  void *Factory();

	static  MStatus Initialize();

	virtual void postConstructor();

	virtual MStatus compute( const MPlug &mPlug, MDataBlock &mDataBlock );

	virtual MStatus	glBind( const MDagPath & );

	virtual MStatus	glUnbind( const MDagPath & );

	virtual MStatus	glGeometry(
		const MDagPath &,
		int prim,
		unsigned int writable,
		int indexCount,
		const unsigned int *indexArray,
		int vertexCount,
		const int *vertexIDs,
		const float *vertexArray,
		int normalCount,
		const float **normalArrays,
		int colorCount,
		const float **colorArrays,
		int texCoordCount,
		const float **texCoordArrays );

	virtual int normalsPerVertex() { return 1; }
	virtual int texCoordsPerVertex() { return 1; }

	virtual bool hasTransparency() { return false; }

	virtual MStatus getAvailableImages(
		const MString &uvSetName,
		MStringArray &imageNames );

	virtual MStatus renderImage(
		const MString &imageName,
		const float region[2][2],
		int &imageWidth,
		int &imageHeight);

	virtual MStatus renderSwatchImage( MImage &image );

	static MTypeId m_id;
	static const char *const s_name;

protected:
	void DetachSceneCallbacks();

	static void ReleaseCallback( void *clientData );

	void ReleaseEverything();

	void SyncAttributes();

	void SetupOpenGL( const bool forUvEditor = false, const char *pTextureName = NULL );

	MString EvaluateMaterial( const MString &vmtPath );

	// Shader input parameters
	static MObject m_iaUV;

	static MObject m_iaRenderAlpha;

	static MObject m_iaVmtPath;
	static MObject m_iaVmtForceDirty;
	static MObject m_oaVmtFullPath;

	static MObject m_oaTexture;
	static MObject m_oaTexName;
	static MObject m_oaTexPath;

	static MObject m_iaVtf;
	static MObject m_oaVtfPath;
	static MObject m_oaVtfFullPath;

	static MObject m_iaColor;

	// Maya callbacks monitored so OpenGL resources can be released before
	// their context is destroyed
	MCallbackId m_beforeNewCB;
	MCallbackId m_beforeOpenCB;
	MCallbackId m_beforeRemoveReferenceCB;
	MCallbackId m_mayaExitingCB;

	class CTexture
	{
	public:
		CTexture()
		: m_bSpecify( true )
		, m_bRenderAlpha( false )
		, m_pVsVtf( NULL )
		{
		}

		~CTexture()
		{
			if ( m_pVsVtf )
			{
				delete m_pVsVtf;
			}
		}

		void Clear()
		{
			m_texName.clear();
			m_texPath.clear();
			m_texFullPath.clear();
			m_bSpecify = true;
			if ( m_pVsVtf )
			{
				delete m_pVsVtf;
				m_pVsVtf = NULL;
			}
		}

		CVsVtf *GetVtf()
		{
			if ( m_pVsVtf == NULL )
			{
				m_pVsVtf = new CVsVtf;
				if ( m_pVsVtf )
				{
					m_pVsVtf->Load( m_texFullPath.asChar() );
				}
			}

			return m_pVsVtf;
		}

		MString m_texName;
		MString m_texPath;
		MString m_texFullPath;
		bool m_bSpecify;
		bool m_bRenderAlpha;
	protected:

		CVsVtf *m_pVsVtf;
	};

	CUtlVector< CTexture > m_textureList;
	int m_nTextureIndex;
};

#endif // VSTSHADER_H
