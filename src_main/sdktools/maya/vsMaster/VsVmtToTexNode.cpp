//======= Copyright © 1996-2006, Valve Corporation, All rights reserved. ======
//
// Purpose: Converts .vmt material paths to texture map names
//			Does so via VPROJECT parsing gameinfo.txt and then searching
//			for the appropriate .vmt file, parsing it and then 
//
//=============================================================================

#include <io.h>
// Maya includes
#include <maya/MFileIO.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnStringData.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MPlugArray.h>

// Valve includes
#include "valveMaya.h"
#include "VsMayaMPxFactory.h"
#include "KeyValues.h"
#include "FileSystem.h"
#include "tier1/utlbuffer.h"
#include "tier2/tier2.h"

class CVsVmtToTexNode : public CVsMayaMPxNode
{
public:
	static MStatus Initialize();

	virtual void postConstructor();

	virtual MStatus compute(
		const MPlug &mPlug,
		MDataBlock &mDataBlock );

protected:
	static void VmtError(
		const MString &materialPath,
		const MString &textureId,
		MString &texturePath,
		MString &statusMsg );

	static void VmtToTex(
		const MString &materialPath,
		const MString &textureId,
		MString &texturePath,
		MString &statusMsg );

	static MObject m_iaCachedVProject;
	static MObject m_caVProject;

	static MObject m_iaMaterialPath;
	static MObject m_iaTextureId;

	static MObject m_oaCachedTexturePath;
	static MObject m_oaTexturePath;

	static MObject m_oaStatus;
};


INSTALL_MAYA_MPXSHADERNODE(
	CVsVmtToTexNode,
	vsVmtToTex,
	0x0010e4c0,
	CVsVmtToTexNode::Initialize,
	"utility/general",
	"Converts a Valve material path to a texture map filename."
);


// Statics

MObject CVsVmtToTexNode::m_iaCachedVProject;
MObject CVsVmtToTexNode::m_caVProject;

MObject CVsVmtToTexNode::m_iaMaterialPath;
MObject CVsVmtToTexNode::m_iaTextureId;

MObject CVsVmtToTexNode::m_oaCachedTexturePath;
MObject CVsVmtToTexNode::m_oaTexturePath;

MObject CVsVmtToTexNode::m_oaStatus;

//-----------------------------------------------------------------------------
// Lock some attributes
//-----------------------------------------------------------------------------
void CVsVmtToTexNode::postConstructor()
{
	const MObject thisObj( thisMObject() );

	MPlug( thisObj, m_caVProject ).setLocked( true );
	MPlug( thisObj, m_oaTexturePath ).setLocked( true );
	MPlug( thisObj, m_oaStatus ).setLocked( true );
}

//-----------------------------------------------------------------------------
// Initialize the node, add static attributes, etc...
//-----------------------------------------------------------------------------

MStatus CVsVmtToTexNode::Initialize()
{
	MFnNumericAttribute nFn;
	MFnTypedAttribute tFn;

	MObject defCachedVProjectObj( MFnStringData().create( "" ) );
	m_iaCachedVProject = tFn.create( "cachedVProject", "cavp", MFnData::kString, defCachedVProjectObj );
	tFn.setHidden( true );
	tFn.setReadable( false );
	tFn.setWritable( false );
	tFn.setConnectable( false );

	m_caVProject = nFn.create( "vproject", "vp", MFnNumericData::kBoolean, true );
	nFn.setHidden( true );
	tFn.setWritable( false );
	nFn.setStorable( false );

	MObject defMaterialPathObj( MFnStringData().create( "debug\\debugempty" ) );
	m_iaMaterialPath = tFn.create( "materialPath", "mp", MFnData::kString, defMaterialPathObj );
	tFn.setReadable( false );

	MObject defTextureIdObj( MFnStringData().create( "$basetexture" ) );
	m_iaTextureId = tFn.create( "textureId", "ti", MFnData::kString, defTextureIdObj );
	tFn.setReadable( false );

	MObject defCachedTexturePathObj( MFnStringData().create( "" ) );
	m_oaCachedTexturePath = tFn.create( "cachedTexturePath", "ctp", MFnData::kString, defCachedTexturePathObj );
	tFn.setHidden( true );
	tFn.setReadable( false );
	tFn.setWritable( false );
	tFn.setConnectable( false );

	MObject defTexturePathObj( MFnStringData().create( "" ) );
	m_oaTexturePath = tFn.create( "texturePath", "tp", MFnData::kString, defTexturePathObj );
	tFn.setWritable( false );
	tFn.setStorable( false );

	MObject defStatusObj( MFnStringData().create( "Ok" ) );
	m_oaStatus = tFn.create( "status", "st", MFnData::kString, defStatusObj );
	tFn.setStorable( false );

	addAttribute( m_iaCachedVProject );
	addAttribute( m_caVProject );

	addAttribute( m_iaMaterialPath );
	addAttribute( m_iaTextureId );

	addAttribute( m_oaCachedTexturePath );
	addAttribute( m_oaTexturePath );

	addAttribute( m_oaStatus );

	attributeAffects( m_iaCachedVProject, m_caVProject );

	attributeAffects( m_iaCachedVProject, m_oaCachedTexturePath );
	attributeAffects( m_caVProject, m_oaCachedTexturePath );
	attributeAffects( m_iaMaterialPath, m_oaCachedTexturePath );
	attributeAffects( m_iaTextureId, m_oaCachedTexturePath );

	attributeAffects( m_iaCachedVProject, m_oaTexturePath );
	attributeAffects( m_caVProject, m_oaTexturePath );
	attributeAffects( m_oaCachedTexturePath, m_oaTexturePath );
	attributeAffects( m_iaMaterialPath, m_oaTexturePath );
	attributeAffects( m_iaTextureId, m_oaTexturePath );

	attributeAffects( m_iaCachedVProject, m_oaStatus );
	attributeAffects( m_caVProject, m_oaStatus );
	attributeAffects( m_oaCachedTexturePath, m_oaStatus );
	attributeAffects( m_iaMaterialPath, m_oaStatus );
	attributeAffects( m_iaTextureId, m_oaStatus );

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
//
// Purpose: Set the outputs based on the inputs, in this case figure out
//          where a texture map might exist for a given material path
// Input  :
// Output : MStatus::kSuccess if everything was ok
//
//-----------------------------------------------------------------------------

MStatus CVsVmtToTexNode::compute(
	const MPlug &mPlug,
	MDataBlock &mDataBlock )
{
	if ( mPlug == m_caVProject )
	{
		const MString vproject( getenv( "VPROJECT" ) );
		if ( mDataBlock.inputValue( m_iaCachedVProject ).asString() != vproject )
		{
			// Set cached vproject
			mDataBlock.outputValue( m_iaCachedVProject ).set( vproject );

			// Compute & save the cached & current texture path because vproject has changed
			MString texturePath;
			MString statusMsg;

			VmtToTex(
				mDataBlock.inputValue( m_iaMaterialPath ).asString(),
				mDataBlock.inputValue( m_iaTextureId ).asString(),
				texturePath,
				statusMsg );

			mDataBlock.outputValue( m_oaCachedTexturePath ).set( texturePath );
			mDataBlock.outputValue( m_oaStatus ).set( statusMsg );
		}

		// Make sure we know we've cleaned up any vproject changes required
		mDataBlock.outputValue( m_caVProject ).setClean();
	}
	else if ( mPlug == m_oaCachedTexturePath )
	{
		// Just compute it, it got here for some reason
		MString texturePath;
		MString statusMsg;

		VmtToTex(
			mDataBlock.inputValue( m_iaMaterialPath ).asString(),
			mDataBlock.inputValue( m_iaTextureId ).asString(),
			texturePath,
			statusMsg );

		mDataBlock.outputValue( m_oaCachedTexturePath ).set( texturePath );
		mDataBlock.outputValue( m_oaTexturePath ).set( texturePath );
		mDataBlock.outputValue( m_oaStatus ).set( statusMsg );
	}
	else if ( mPlug == m_oaTexturePath || mPlug == m_oaStatus )
	{
		// Make sure things aren't just because vproject changed
		mDataBlock.inputValue( m_caVProject ).asBool();

		// Copy from cache
		mDataBlock.outputValue( m_oaTexturePath ).set( mDataBlock.inputValue( m_oaCachedTexturePath ).asString() );
		mDataBlock.outputValue( m_oaStatus ).setClean();

		return MS::kSuccess;
	}

	return MS::kUnknownParameter;
}


//-----------------------------------------------------------------------------
// lookup debug/debugempty and if that fails hard code a path
//-----------------------------------------------------------------------------
void CVsVmtToTexNode::VmtError(
	const MString &materialPath,
	const MString &textureId,
	MString &texturePath,
	MString &statusMsg )
{
	// Try looking up debug/debugempty once, if it fails a second time guess at debug/debugempty

	if ( materialPath == "debug/debugempty" )
	{
		return;
	}

	texturePath = "";

	MString junkStatus;
	VmtToTex( "debug/debugempty", "$basetexture", texturePath, junkStatus );

	if ( texturePath == "" )
	{
		char pTmpBuf0[ MAX_PATH ];
		char pTmpBuf1[ MAX_PATH ];

		const char *const vproject( getenv( "VPROJECT" ) );
		if ( vproject && *vproject != '\0' )
		{
			Q_strncpy( pTmpBuf0, vproject, sizeof( pTmpBuf0 ) );
			Q_ComposeFileName( pTmpBuf0, "materialsrc/debug/debugempty.tga", pTmpBuf1, sizeof( pTmpBuf1 ) );
			Q_StrSubst( pTmpBuf1, "game", "content", pTmpBuf0, sizeof( pTmpBuf0 ) );
			texturePath = pTmpBuf0;
		}
		else
		{
			statusMsg = "VPROJECT undefined!";
		}
	}
}

KeyValues *FindKey( KeyValues *pKeyValues, const char *pKeyString )
{
	KeyValues *pFoundKey = pKeyValues->FindKey( pKeyString );
	if ( pFoundKey )
		return pFoundKey;

	for ( KeyValues *pKey = pKeyValues->GetFirstTrueSubKey(); pKey; pKey = pKey->GetNextTrueSubKey() )
	{
		return FindKey ( pKey, pKeyString );
	}

	return NULL;
}


//-----------------------------------------------------------------------------
// Return a path to a texture file based on the given vmt path
//-----------------------------------------------------------------------------
void CVsVmtToTexNode::VmtToTex(
	const MString &materialPath,
	const MString &textureId,
	MString &texturePath,
	MString &statusMsg )
{
	// Oh... if only CORRECT_PATH_SEPARATOR in strtools.h wasn't single quoted... :(
	// Q_PATHSEP == Quoted Path Separator!
#ifdef _WIN32
#define Q_PATHSEP "\\"
#elif _LINUX
#define Q_PATHSEP "/"
#endif

	// Convert material path into a .vmt file
	char pTmpBuf0[ MAX_PATH ] = { "materials" };
	char pTmpBuf1[ MAX_PATH ];

	Q_ComposeFileName( pTmpBuf0, materialPath.asChar(), pTmpBuf1, sizeof( pTmpBuf1 ) );
	Q_SetExtension( pTmpBuf1, ".vmt", sizeof( pTmpBuf1 ) );
	Q_FixSlashes( pTmpBuf1 );

	CUtlBuffer utlBuf( 0, 0, CUtlBuffer::TEXT_BUFFER );
	if ( g_pFullFileSystem->ReadFile( pTmpBuf1, "game", utlBuf ) )
	{
		KeyValues *pKeyValues( new KeyValues( "" ) );
		if ( pKeyValues->LoadFromBuffer( pTmpBuf1, utlBuf ) )
		{
			KeyValues *pTextureKey = FindKey( pKeyValues, textureId.asChar() );
			const char *pTexturePath( NULL );

			if ( pTextureKey )
			{
				pTexturePath = pTextureKey->GetString();
			}

			if ( pTexturePath && *pTexturePath != '\0')
			{
				Q_strncpy( pTmpBuf0, "materials", sizeof( pTmpBuf0 ) );
				Q_ComposeFileName( pTmpBuf0, pTexturePath, pTmpBuf1, sizeof( pTmpBuf1 ) );
				Q_SetExtension( pTmpBuf1, ".vtf", sizeof( pTmpBuf1 ) );
				Q_FixSlashes( pTmpBuf1 );
				g_pFullFileSystem->RelativePathToFullPath( pTmpBuf1, "game", pTmpBuf0, sizeof( pTmpBuf0 ) );
				Q_StrSubst( pTmpBuf0, Q_PATHSEP "materials" Q_PATHSEP, Q_PATHSEP "materialsrc" Q_PATHSEP, pTmpBuf1, sizeof( pTmpBuf1 ) );
				Q_StrSubst( pTmpBuf1, Q_PATHSEP "game" Q_PATHSEP, Q_PATHSEP "content" Q_PATHSEP, pTmpBuf0, sizeof( pTmpBuf0 ) );
				
				Q_SetExtension( pTmpBuf0, ".tga", sizeof( pTmpBuf0 ) );
				if( _access( pTmpBuf0, 04 ) != 0 )
				{
					Q_StripExtension( pTmpBuf0, pTmpBuf1, sizeof( pTmpBuf1 ) );
					Q_SetExtension( pTmpBuf1, ".psd", sizeof( pTmpBuf1 ) );
					if( _access( pTmpBuf1, 04 ) != 0 )
					{
						statusMsg = MString( "Can't find \"" ) + textureId + MString( "\" In \"" ) + pTmpBuf1 + MString( "\"" );
						VmtError( materialPath, textureId, texturePath, statusMsg );
						return;
					}
					texturePath = pTmpBuf1;
					statusMsg = "Ok";
					
				}
				else
				{
					texturePath = pTmpBuf0;
					statusMsg = "Ok";
				}
			}
			else
			{
				statusMsg = MString( "Can't find \"" ) + textureId + MString( "\" In \"" ) + pTmpBuf1 + MString( "\"" );
				VmtError( materialPath, textureId, texturePath, statusMsg );
			}
		}
		else
		{
			statusMsg = MString( "Can't open \"" ) + pTmpBuf1 + MString( "\" from \"game\"" );
			VmtError( materialPath, textureId, texturePath, statusMsg );
		}

		pKeyValues->deleteThis();
	}
	else
	{
		statusMsg = MString( "Can't open \"" ) + pTmpBuf1 + MString( "\"" );
		VmtError( materialPath, textureId, texturePath, statusMsg );
	}
}

