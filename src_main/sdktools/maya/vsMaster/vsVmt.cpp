//======= Copyright © 1996-2007, Valve Corporation, All rights reserved. ======
//
// Maya surface shader proxy for Valve VMT ( Valve Material )
//
// Reads in a VMT file, enumerates all of the shader attributes to find the
// textures and then selectively uses a texture to render the image in Maya
//
//=============================================================================


// Maya includes
#include <maya/MArrayDataBuilder.h>
#include <maya/MDagPath.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFloatVector.h>
#include <maya/MFnStringData.h>
#include <maya/MGlobal.h>
#include <maya/MPlug.h>
#include <maya/MString.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MSceneMessage.h>


// Valve includes
#include "filesystem.h"
#include "materialsystem/imaterial.h"
#include "materialsystem/imaterialsystem.h"
#include "materialsystem/imaterialvar.h"
#include "materialsystem/IShader.h"
#include "materialsystem/itexture.h"
#include "valveMaya.h"


// OpenGL
#include <GL/gl.h>


// Local includes
#include "vsVmt.h"

// Statics
MTypeId CVsVmtNode::m_id( 0x0010544a );
const char *const CVsVmtNode::s_name( "vsVmt" );

MObject CVsVmtNode::m_iaUV;

MObject CVsVmtNode::m_iaRenderAlpha;

MObject CVsVmtNode::m_iaVmtPath;
MObject CVsVmtNode::m_iaVmtForceDirty;
MObject CVsVmtNode::m_oaVmtFullPath;

MObject CVsVmtNode::m_oaTexture;
MObject CVsVmtNode::m_oaTexName;
MObject CVsVmtNode::m_oaTexPath;

MObject CVsVmtNode::m_iaVtf;
MObject CVsVmtNode::m_oaVtfPath;
MObject CVsVmtNode::m_oaVtfFullPath;

MObject CVsVmtNode::m_iaColor;


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CVsVmtNode::CVsVmtNode()
: m_nTextureIndex( -1 )
{
	// Add the scene callbacks
	m_beforeNewCB  = MSceneMessage::addCallback( MSceneMessage::kBeforeNew, ReleaseCallback, this);
	m_beforeOpenCB = MSceneMessage::addCallback( MSceneMessage::kBeforeOpen, ReleaseCallback, this );
	m_beforeRemoveReferenceCB = MSceneMessage::addCallback( MSceneMessage::kBeforeRemoveReference, ReleaseCallback, this );
	m_mayaExitingCB = MSceneMessage::addCallback( MSceneMessage::kMayaExiting, ReleaseCallback, this );
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CVsVmtNode::~CVsVmtNode()
{
	if ( m_beforeNewCB )
	{
		MMessage::removeCallback( m_beforeNewCB );
		m_beforeNewCB = NULL;
	}

	if ( m_beforeOpenCB )
	{
		MMessage::removeCallback( m_beforeOpenCB );
		m_beforeOpenCB = NULL;
	}

	if ( m_beforeRemoveReferenceCB )
	{
		MMessage::removeCallback( m_beforeRemoveReferenceCB );
		m_beforeRemoveReferenceCB = NULL;
	}

	if ( m_mayaExitingCB )
	{
		MMessage::removeCallback( m_mayaExitingCB );
		m_mayaExitingCB = NULL;
	}
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void *CVsVmtNode::Factory()
{
    return new CVsVmtNode();
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MStatus CVsVmtNode::Initialize()
{
	MFnCompoundAttribute cFn;
	MFnNumericAttribute nFn;
	MFnTypedAttribute tFn;

	// uv
	MObject iaU = nFn.create( "uCoord", "u", MFnNumericData::kFloat, 0.0 );
	MObject iaV = nFn.create( "vCoord", "v", MFnNumericData::kFloat, 0.0 );
	m_iaUV = nFn.create( "uvCoord", "uv", iaU, iaV );
	nFn.setStorable( false );
	nFn.setWritable( true );
	nFn.setRenderSource( true );
	nFn.setHidden( true );

	m_iaRenderAlpha = nFn.create( "renderAlpha", "rena", MFnNumericData::kBoolean, false );
	nFn.setKeyable( true );

	MObject debugEmptyObj = MFnStringData().create( "debug/debugempty" );
	m_iaVmtPath = tFn.create( "vmtPath", "vmtp", MFnData::kString, debugEmptyObj );

	m_iaVmtForceDirty = nFn.create( "vmtForceDirty", "vfd", MFnNumericData::kInt, 0 );
	nFn.setHidden( true );
	nFn.setStorable( false );
	nFn.setConnectable( false );

	m_oaVmtFullPath = tFn.create( "vmtFullPath", "vmtf", MFnData::kString );
	tFn.setStorable( false );

	m_oaTexName = tFn.create( "texName", "texn", MFnData::kString );
	tFn.setWritable( false );
	tFn.setConnectable( false );
	tFn.setStorable( false );

	m_oaTexPath = tFn.create( "texPath", "texp", MFnData::kString );
	tFn.setWritable( false );
	tFn.setConnectable( false );
	tFn.setStorable( false );

	m_oaTexture = cFn.create( "texture", "tex" );
	cFn.setWritable( false );
	cFn.setConnectable( false );
	cFn.setStorable( false );
	cFn.setArray( true );
	cFn.setUsesArrayDataBuilder( true );
	cFn.addChild( m_oaTexName );
	cFn.addChild( m_oaTexPath );

	MObject defVtfObj = MFnStringData().create( "$basetexture" );
	m_iaVtf = tFn.create( "vtf", "vtf", MFnData::kString, defVtfObj );

	m_oaVtfPath = tFn.create( "vtfPath", "vtfp", MFnData::kString );
	tFn.setStorable( false );

	m_oaVtfFullPath = tFn.create( "vtfFullPath", "vtff", MFnData::kString );
	tFn.setStorable( false );

	m_iaColor = nFn.createColor( "color", "c" );
	nFn.setDefault( 0.5f, 0.5f, 0.5f );

	addAttribute( m_iaUV );

	addAttribute( m_iaRenderAlpha );
	addAttribute( m_iaVmtPath );
	addAttribute( m_iaVmtForceDirty );
	addAttribute( m_oaVmtFullPath );

	addAttribute( m_oaTexture );

	addAttribute( m_iaVtf );
	addAttribute( m_oaVtfPath );
	addAttribute( m_oaVtfFullPath );

	addAttribute( m_iaColor );

	attributeAffects( m_iaVmtPath, m_oaVmtFullPath );
	attributeAffects( m_iaVmtForceDirty, m_oaVmtFullPath );

	attributeAffects( m_iaVmtPath, m_oaTexture );
	attributeAffects( m_iaVmtForceDirty, m_oaTexture );

	attributeAffects( m_iaVmtPath, m_oaTexName );
	attributeAffects( m_iaVmtForceDirty, m_oaTexName );

	attributeAffects( m_iaVmtPath, m_oaTexPath );
	attributeAffects( m_iaVmtForceDirty, m_oaTexPath );

	attributeAffects( m_iaVmtPath, m_oaVtfPath );
	attributeAffects( m_iaVmtForceDirty, m_oaVtfPath );
	attributeAffects( m_oaTexture, m_oaVtfPath );
	attributeAffects( m_oaTexName, m_oaVtfPath );
	attributeAffects( m_oaTexPath, m_oaVtfPath );
	attributeAffects( m_iaVtf, m_oaVtfPath );
	attributeAffects( m_iaRenderAlpha, m_oaVtfPath );

	attributeAffects( m_iaVmtPath, m_oaVtfFullPath );
	attributeAffects( m_iaVmtForceDirty, m_oaVtfFullPath );
	attributeAffects( m_oaTexture, m_oaVtfFullPath );
	attributeAffects( m_oaTexName, m_oaVtfFullPath );
	attributeAffects( m_oaTexPath, m_oaVtfFullPath );
	attributeAffects( m_iaVtf, m_oaVtfFullPath );
	attributeAffects( m_oaVtfPath, m_oaVtfFullPath );

	attributeAffects( m_iaUV, outColor );
	attributeAffects( m_iaVmtPath, outColor );
	attributeAffects( m_iaVmtForceDirty, outColor );
	attributeAffects( m_iaColor, outColor );

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CVsVmtNode::postConstructor()
{
	setMPSafe( false );

	MObject thisObj = thisMObject();

	MPlug mPlug( thisObj, m_oaVmtFullPath );
	mPlug.setLocked( true );

	mPlug.setAttribute( m_oaVtfPath );
	mPlug.setLocked( true );

	mPlug.setAttribute( m_oaVtfFullPath );
	mPlug.setLocked( true );
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CVsVmtNode::ReleaseEverything()
{
	m_textureList.RemoveAll();
	m_nTextureIndex = -1;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CVsVmtNode::ReleaseCallback( void *clientData )
{
	reinterpret_cast< CVsVmtNode * >( clientData )->ReleaseEverything();
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MStatus CVsVmtNode::compute(
	const MPlug &mPlug,
	MDataBlock &mDataBlock ) 
{
	if ( mPlug == m_oaVmtFullPath || mPlug == m_oaTexture || mPlug == m_oaTexName || mPlug == m_oaTexPath )
	{
		MString vmtPath = mDataBlock.inputValue( m_iaVmtPath ).asString();
		MString vmtFullPath = EvaluateMaterial( vmtPath.expandEnvironmentVariablesAndTilde() );
		const int nTextureCount = m_textureList.Count();

		MArrayDataBuilder tADB( &mDataBlock, m_oaTexture, nTextureCount );
		for ( int i = 0; i < nTextureCount; ++i )
		{
			MDataHandle tDH = tADB.addElement( i );
			const CTexture &tex = m_textureList[ i ];
			tDH.child( m_oaTexName ).set( tex.m_texName );
			tDH.child( m_oaTexPath ).set( tex.m_texPath );
		}

		int nLogicalIndex = nTextureCount - 1;
		if ( mPlug == m_oaTexture )
		{
			nLogicalIndex = mPlug.logicalIndex();
		}
		else if ( mPlug == m_oaTexName || mPlug == m_oaTexPath )
		{
			nLogicalIndex = mPlug.parent().logicalIndex();
		}

		for ( int i = nTextureCount; i <= nLogicalIndex; ++i )
		{
			MDataHandle tDH = tADB.addElement( i );
			tDH.child( m_oaTexName ).set( "INVALID" );
			tDH.child( m_oaTexPath ).set( "INVALID" );
		}

		mDataBlock.outputArrayValue( m_oaTexture ).set( tADB );
		mDataBlock.outputValue( m_oaVmtFullPath ).set( vmtFullPath );
	}
	else if ( mPlug == m_oaVtfPath || mPlug == m_oaVtfFullPath )
	{
		// Just ensure everything we need is computed
		mDataBlock.inputValue( m_oaVmtFullPath ).asString();
		const MString &vtf = mDataBlock.inputValue( m_iaVtf ).asString();

		int nTextureIndex = -1;
		const int nTextureCount = m_textureList.Count();
		for ( int i = 0; i < nTextureCount; ++i )
		{
			if ( !Q_stricmp( vtf.asChar(), m_textureList[ i ].m_texName.asChar() ) )
			{
				nTextureIndex = i;
				break;
			}
		}

		if ( nTextureIndex >= 0 )
		{
			CTexture &texture = m_textureList[ nTextureIndex ];
			mDataBlock.outputValue( m_oaVtfPath ).set( texture.m_texPath );
			mDataBlock.outputValue( m_oaVtfFullPath ).set( texture.m_texFullPath );
			m_nTextureIndex = nTextureIndex;
			const bool &bRenderAlpha = mDataBlock.inputValue( m_iaRenderAlpha ).asBool();
			if ( texture.m_bRenderAlpha != bRenderAlpha )
			{
				texture.m_bRenderAlpha = bRenderAlpha;
				texture.m_bSpecify = true;
			}
		}
		else
		{
			const MString errString( MString( "ERROR: Cannot find texture \"" ) + vtf + "\" in vmt" );
			mDataBlock.outputValue( m_oaVtfPath ).set( errString );
			mDataBlock.outputValue( m_oaVtfFullPath ).set( errString );
			m_nTextureIndex = -1;
		}
	}
	else if (
		mPlug == outColor ||
		mPlug == outColorR ||
		mPlug == outColorG ||
		mPlug == outColorB )
	{
		mDataBlock.inputValue( m_oaVtfFullPath ).asString();

		// TODO: Sample Texture @ UV

//		const float2 &uv = mDataBlock.inputValue( m_iaUV ).asFloat2();

		MDataHandle inColor( mDataBlock.outputValue( m_iaColor ) );
		MDataHandle outColor( mDataBlock.outputValue( outColor ) );
		outColor.set( inColor.asFloatVector() );

		return MS::kSuccess;
	}

	return MS::kUnknownParameter;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MStatus	CVsVmtNode::glBind(
	const MDagPath & )
{
	SyncAttributes();

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MStatus	CVsVmtNode::glUnbind(
	const MDagPath & )
{
	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MStatus	CVsVmtNode::glGeometry(
	const MDagPath & /* mDagPath */,
	int prim,
	unsigned int /* writable */,
	int indexCount,
	const unsigned int * indexArray,
	int /* vertexCount */,
	const int * /* vertexIDs */,
	const float * vertexArray,
	int normalCount,
	const float ** normalArrays,
	int /* colorCount */,
	const float ** /* colorArrays */,
	int texCoordCount,
	const float ** texCoordArrays)
{
	// TODO: Figure out what really needs to be pushed

	glPushAttrib( GL_ALL_ATTRIB_BITS );
	glPushClientAttrib( GL_CLIENT_VERTEX_ARRAY_BIT );

	SetupOpenGL();

	glVertexPointer( 3, GL_FLOAT, 0, vertexArray );
	glEnableClientState( GL_VERTEX_ARRAY );

	if ( normalCount > 0 )
	{
		glNormalPointer( GL_FLOAT, 0, normalArrays[0] );
		glEnableClientState( GL_NORMAL_ARRAY );
	}

	if ( texCoordCount > 0 )
	{
		glTexCoordPointer( 2, GL_FLOAT, 0, texCoordArrays[0] );
		glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	}

	bool bAlpha = false;

	if ( !glIsEnabled( GL_CULL_FACE ) && m_nTextureIndex >= 0 && m_nTextureIndex < m_textureList.Count() )
	{
		CTexture &texture = m_textureList[ m_nTextureIndex ];
		if ( texture.m_bRenderAlpha )
		{
			const CVsVtf *pVtf = texture.GetVtf();
			if ( pVtf && pVtf->HasAlpha() )
			{
				bAlpha = true;
			}
		}
	}

	if ( bAlpha )
	{
		glEnable( GL_CULL_FACE );
		const GLboolean bBlendEnabled = glIsEnabled( GL_BLEND );
		glEnable( GL_BLEND );

		glCullFace( GL_FRONT );
		glDrawElements( prim, indexCount, GL_UNSIGNED_INT, indexArray );

		glCullFace( GL_BACK );
		glDrawElements( prim, indexCount, GL_UNSIGNED_INT, indexArray );

		glDisable( GL_CULL_FACE );
		if ( !bBlendEnabled )
		{
			glDisable( GL_BLEND );
		}
	}
	else
	{
		glDrawElements( prim, indexCount, GL_UNSIGNED_INT, indexArray );
	}

	glPopClientAttrib();
	glPopAttrib();

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
// Purpose: Tell maya what images are available for the UV texture editor
//-----------------------------------------------------------------------------

MStatus
CVsVmtNode::getAvailableImages(
	const MString & /* uvSetName */,
	MStringArray &imageNames )
{
	SyncAttributes();

	imageNames.append( "current" );

	const int nTextureCount = m_textureList.Count();
	for ( int i = 0; i < nTextureCount; ++i )
	{
		const CTexture &tex = m_textureList[ i ];
		imageNames.append( tex.m_texName );
	}
	
	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
// Purpose: Render the background image for the UV texture editor
//-----------------------------------------------------------------------------
MStatus
CVsVmtNode::renderImage(
	const MString &imageName,
	const float region[2][2],
	int & /* imageWidth */,
	int & /* imageHeight */)
{
	SyncAttributes();

	glPushAttrib( GL_ALL_ATTRIB_BITS );
	glPushClientAttrib( GL_CLIENT_VERTEX_ARRAY_BIT );

	SetupOpenGL( true, imageName == "current" ? NULL : imageName.asChar() );

	glBegin( GL_QUADS );

	glTexCoord2f( region[0][0], region[0][1] );
	glVertex2f( region[0][0], region[0][1] );

	glTexCoord2f( region[0][0], region[1][1] );
	glVertex2f( region[0][0], region[1][1] );

	glTexCoord2f( region[1][0], region[1][1] );
	glVertex2f( region[1][0], region[1][1] );

	glTexCoord2f( region[1][0], region[0][1] );
	glVertex2f( region[1][0], region[0][1] );

	glEnd();

	glPopClientAttrib();
	glPopAttrib();

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
// Purpose: Ensure that the internal state of the class matches the Maya UI
//-----------------------------------------------------------------------------
void
CVsVmtNode::SyncAttributes()
{
	MObject thisObj( thisMObject() );

	MPlug vtfPathP( thisObj, m_oaVtfPath );
	MString vtfPath;
	vtfPathP.getValue( vtfPath );
}


//-----------------------------------------------------------------------------
// Purpose: Set up the OpenGL state just the way we likes it before drawing
//-----------------------------------------------------------------------------
void CVsVmtNode::SetupOpenGL( bool bUvEditor /* = false */, const char *pTextureName /* = NULL */ )
{
	if ( bUvEditor )
	{
		glDisable( GL_LIGHTING );
		glDisable( GL_DEPTH_TEST );
	}
	else
	{
		glColorMaterial( GL_FRONT_AND_BACK,GL_EMISSION );
		glEnable( GL_COLOR_MATERIAL );
		glColor3f( 0.0f, 0.0f, 0.0f );
		glDisable( GL_COLOR_MATERIAL );

		glColorMaterial( GL_FRONT_AND_BACK, GL_DIFFUSE );
		glEnable( GL_COLOR_MATERIAL );

		if ( glIsEnabled( GL_CULL_FACE ) )
		{
			// XRay Mode
			glColor4f( 1.0f, 1.0f, 1.0f, 0.3f );
		}
		else
		{
			glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
		}

		glDisable( GL_COLOR_MATERIAL );

		glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );

		glDisable( GL_TEXTURE_GEN_S );
		glDisable( GL_TEXTURE_GEN_T );
	}

	CTexture *pTexture = NULL;

	if ( pTextureName )
	{
		const int nTextureCount = m_textureList.Count();
		for ( int i = 0; i < nTextureCount; ++i )
		{
			if ( !Q_stricmp( pTextureName, m_textureList[ i ].m_texName.asChar() ) )
			{
				pTexture = &m_textureList[ i ];
				break;
			}
		}
	}
	else if ( m_nTextureIndex >= 0 )
	{
		pTexture = &m_textureList[ m_nTextureIndex ];
	}

	if ( !pTexture )
		return;

	CVsVtf *pVtf = pTexture->GetVtf();

	if ( !pVtf )
		return;

	glEnable( GL_TEXTURE_2D );

	if ( pTexture->m_bSpecify )
	{
		pVtf->Specify( pTexture->m_bRenderAlpha );

		if ( bUvEditor )
		{
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
		}

		pTexture->m_bSpecify = false;
	}

	pVtf->Bind();
}


//-----------------------------------------------------------------------------
// Purpose: Render a swatch for hypershade
//-----------------------------------------------------------------------------
MStatus CVsVmtNode::renderSwatchImage( MImage &mImage )
{
	SyncAttributes();

	if ( m_nTextureIndex < 0 )
	{
		// TODO: Render sad face
		return MS::kFailure;
	}

	CVsVtf *pVtf = m_textureList[ m_nTextureIndex ].GetVtf();
	if ( pVtf )
		return pVtf->CopyToImage( mImage );

	// TODO: Render sad face
	return MS::kFailure;
}


//-----------------------------------------------------------------------------
// Load the VMT from disk, then cross reference every set attribute with
// the shader and figure out which ones are textures and store in the internal
// array
//-----------------------------------------------------------------------------
MString CVsVmtNode::EvaluateMaterial( const MString &vmtPath )
{
	m_textureList.RemoveAll();
	m_nTextureIndex = -1;

	int nShaders = g_pMaterialSystem->ShaderCount();
	if ( nShaders <= 0 )
		return MString( "ERROR: Cannot Find Any Shaders" );

	char buf0[ MAX_PATH ];
	char buf1[ MAX_PATH ];

	// Clean up the passed material name
	if ( !g_pFullFileSystem->FullPathToRelativePath( vmtPath.asChar(), buf0, sizeof( buf0 ) ) )
	{
		// Already was a relative path... hopefully
		Q_strncpy( buf0, vmtPath.asChar(), sizeof( buf0 ) );
	}

	Q_FixSlashes( buf0, '/' );
	if ( !Q_strnicmp( "materials/", buf0, 10 ) )
	{
		Q_StripExtension( buf0 + 10, buf1, sizeof( buf1 ) );
	}
	else
	{
		Q_StripExtension( buf0, buf1, sizeof( buf1 ) );
	}
	Q_StripTrailingSlash( buf1 );

	IMaterial *pMaterial = g_pMaterialSystem->FindMaterial( buf1, TEXTURE_GROUP_MODEL );
	if ( !pMaterial || pMaterial->IsErrorMaterial() )
		return MString( "ERROR: Cannot Find Material From Vmt Path: " ) + buf1;

	MString vmtFullPath( "ERROR: Can't Compute Full Vmt Path From: " );
	vmtFullPath += buf1;

	Q_snprintf( buf0, sizeof( buf0 ), "materials/%s.vmt", buf1 );

	if ( g_pFullFileSystem->RelativePathToFullPath( buf0, "GAME", buf1, sizeof( buf1 ) ) )
	{
		vmtFullPath = buf1;
	}

	pMaterial->Refresh();

	IShader **ppShaders = reinterpret_cast< IShader ** >( alloca( nShaders * sizeof( IShader * ) ) );
	nShaders = g_pMaterialSystem->GetShaders( 0, nShaders, ppShaders );

	const char *pShaderName = pMaterial->GetShaderName();

	for ( int i = 0; i < nShaders; ++i )
	{
		IShader *pShader = ppShaders[ i ];
		if ( pShader && !Q_strcmp( pShaderName, pShader->GetName() ) )
		{
			const int nShaderParamCount = pShader->GetNumParams();
			const int nMaterialParamCount = pMaterial->ShaderParamCount();
			IMaterialVar **pMaterialVars = pMaterial->GetShaderParams();

			if ( !pMaterialVars )
				return MString( "ERROR: Cannot Get Material Vars From: " ) + vmtFullPath;

			for ( int j = 0; j < nShaderParamCount; ++j )
			{
				if ( pShader->GetParamType( j ) == SHADER_PARAM_TYPE_TEXTURE )
				{
					const char *pParamName = pShader->GetParamName( j );

					for ( int k = 0; k < nMaterialParamCount; ++k )
					{
						IMaterialVar *pMaterialVar = pMaterialVars[ k ];
						if ( !Q_stricmp( pParamName, pMaterialVar->GetName() ) )
						{
							if ( pMaterialVar->IsDefined() )
							{
								CTexture &texture = m_textureList[ m_textureList.AddToTail() ];
								texture.Clear();
								texture.m_texName = pMaterialVar->GetName();
								texture.m_texPath = pMaterialVar->GetStringValue();

								if ( !g_pFullFileSystem->FullPathToRelativePath( texture.m_texPath.asChar(), buf0, sizeof( buf0 ) ) )
								{
									Q_strncpy( buf0, texture.m_texPath.asChar(), sizeof( buf0 ) );
								}
								Q_FixSlashes( buf0, '/' );
								Q_StripExtension( buf0, buf1, sizeof( buf1 ) );
								Q_StripTrailingSlash( buf1 );
								Q_snprintf( buf0, sizeof( buf0 ), "materials/%s.vtf", buf1 );

								g_pFullFileSystem->RelativePathToFullPath( buf0, "GAME", buf1, sizeof( buf1 ) );
								texture.m_texFullPath = buf1;
							}
						}
					}
				}
			}

			return vmtFullPath;
		}
	}

	return MString( "ERROR: Cannot Get Shader From VMT: " ) + vmtFullPath;
}