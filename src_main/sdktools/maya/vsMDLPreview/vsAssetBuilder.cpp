//===== Copyright © 1996-2006, Valve Corporation, All rights reserved. ======//
//
// Shows the asset builder in a window
//
//===========================================================================//

#include "dme_controls/assetbuilder.h"
#include "movieobjects/dmemakefile.h"
#include "vsvguiwindow.h"
#include "maya/mglobal.h"
#include "maya/mstring.h"
#include "maya/mscenemessage.h"
#include "vsmayampxfactory.h"
#include "valvemaya.h"
#include <maya/MArgDatabase.h>

#define ASSET_FILE_FORMAT "model"

//-----------------------------------------------------------------------------
// Simply here to deal with a global concept of a makefile
//-----------------------------------------------------------------------------
class CVsAssetBuilderFrame : public CAssetBuilderFrame
{
	DECLARE_CLASS_SIMPLE( CVsAssetBuilderFrame, CAssetBuilderFrame );

public:
	CVsAssetBuilderFrame( vgui::Panel *pParent, const char *pTitle );
	virtual ~CVsAssetBuilderFrame();

	static CDmeMakefile* GetMakefile();

private:
	// The single makefile we know about
	static CDmeHandle< CDmeMakefile > s_hMakefile;
	static MCallbackId s_nSaveCallbackId;
	static CVsAssetBuilderFrame *s_pSingleton;

	static void SaveChangesCallback( void* clientData ); 
	friend void UninstallAssetBuilderCallback();
};


//-----------------------------------------------------------------------------
// Globals
//-----------------------------------------------------------------------------
CDmeHandle< CDmeMakefile > CVsAssetBuilderFrame::s_hMakefile;
MCallbackId CVsAssetBuilderFrame::s_nSaveCallbackId;
CVsAssetBuilderFrame *CVsAssetBuilderFrame::s_pSingleton;


//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
CVsAssetBuilderFrame::CVsAssetBuilderFrame( vgui::Panel *pParent, const char *pTitle ) :
	BaseClass( pParent, pTitle )
{
	Assert( s_pSingleton == NULL );

	// Hook to the current makefile
	if ( s_hMakefile.Get() )
	{
		Reset( s_hMakefile );
	}
	m_pAssetBuilder->DestroyMakefileOnClose( false );

	if ( s_nSaveCallbackId == 0 )
	{
		s_nSaveCallbackId = MSceneMessage::addCallback( MSceneMessage::kMayaExiting, SaveChangesCallback );
	}
	s_pSingleton = this;
}

CVsAssetBuilderFrame::~CVsAssetBuilderFrame()
{
	Assert( s_pSingleton == this );
	s_hMakefile = m_pAssetBuilder->GetRootMakeFile(); 
	s_pSingleton = NULL;
}


//-----------------------------------------------------------------------------
// Returns the current asset
//-----------------------------------------------------------------------------
CDmeMakefile* CVsAssetBuilderFrame::GetMakefile()
{
	return s_pSingleton ? s_pSingleton->m_pAssetBuilder->GetMakeFile() : s_hMakefile;
}


//-----------------------------------------------------------------------------
// Called to ask to save changes
//-----------------------------------------------------------------------------
void CVsAssetBuilderFrame::SaveChangesCallback( void* clientData )
{
	// FIXME: Hierarchical saving of all makefiles not possible here
	CDmeMakefile *pMakefile = GetMakefile();
	if ( !pMakefile || !pMakefile->IsDirty() )
		return;

	const char *pFileName = g_pDataModel->GetFileName( pMakefile->GetFileId() );
	if ( !pFileName )
	{
		// FIXME: Query for file name
		return;
	}

	char pFileBuf[MAX_PATH];
	Q_strncpy( pFileBuf, pFileName, sizeof(pFileBuf) );
	Q_FixSlashes( pFileBuf, '/' );

	// It's already open
	MString command = MString( "vsSaveChanges(\"" ) + pFileBuf + "\")";
	g_pMayaVGui->SetModalMode( true );

	int nResult;
	MStatus status = MGlobal::executeCommand( command, nResult );
	g_pMayaVGui->SetModalMode( false );
	if ( status != MStatus::kSuccess || ( nResult != 1 ) )
		return;
	
	CDmElement *pRoot = GetElement< CDmElement >( g_pDataModel->GetFileRoot( pMakefile->GetFileId() ) );
	if ( !pRoot )
	{
		pRoot = pMakefile;
	}

	const char *pEncoding = g_pDataModel->GetDefaultEncoding( ASSET_FILE_FORMAT );
	g_pDataModel->SaveToFile( pFileName, NULL, pEncoding, ASSET_FILE_FORMAT, pRoot ); 
}


void UninstallAssetBuilderCallback()
{
	if ( CVsAssetBuilderFrame::s_nSaveCallbackId != 0 )
	{
		MSceneMessage::removeCallback( CVsAssetBuilderFrame::s_nSaveCallbackId );
		CVsAssetBuilderFrame::s_nSaveCallbackId = 0;
	}
}


//-----------------------------------------------------------------------------
// Hook the vgui window into the class factory
//-----------------------------------------------------------------------------
INSTALL_MAYA_VGUI_WINDOW( CVsAssetBuilderFrame, "AssetBuilder", "vsAssetBuilder" );


//-----------------------------------------------------------------------------
//
// Purpose: A new Maya command dervied from MPxCommand
//
//-----------------------------------------------------------------------------
class CVsAssetCmd : public CVsMayaMPxCommand
{
public:
	CVsAssetCmd();
	virtual ~CVsAssetCmd();

	// Inherited from MPxCommand
	virtual MStatus doIt( const MArgList &mArgList );
	virtual bool isUndoable() const { return false; }
	virtual bool hasSyntax() const { return true; }

	// Inherited from CVsMayaMPxCommand
	virtual void SpecifySyntax( MSyntax &syntax, vm::CMSyntaxHelp &help );

private:
};


//-----------------------------------------------------------------------------
// Hook the command so we can see it in Maya
//-----------------------------------------------------------------------------
INSTALL_MAYA_MPXCOMMAND(
	CVsAssetCmd,
	vsAsset,
	"Command to manage assets displayed in the asset builder."	);


//-----------------------------------------------------------------------------
// Constructor, destructor
//-----------------------------------------------------------------------------
CVsAssetCmd::CVsAssetCmd()
{
}

CVsAssetCmd::~CVsAssetCmd()
{
}


//-----------------------------------------------------------------------------
// Specifies syntax
//-----------------------------------------------------------------------------
#define OPT_HELP "h"
#define OPT_GET_ASSET_NAME "a"
#define OPT_SAVE_ASSET "s"
#define OPT_SAVE_ASSET_ENCODING "se"

void CVsAssetCmd::SpecifySyntax( MSyntax &syntax, vm::CMSyntaxHelp &help )
{
	help.Clear();
	help.AddFlag( syntax, OPT_HELP, "help", "General", "Print this information" );
	help.AddFlag( syntax, OPT_GET_ASSET_NAME, "assetname", "General", "Display the current asset name" );
	help.AddFlag( syntax, OPT_SAVE_ASSET, "save", "General", "Saves the current asset using the keyvalues2 encoding" );
	help.AddFlag( syntax, OPT_SAVE_ASSET_ENCODING, "saveencoding", "General", "Saves the current asset, specify file encoding", MSyntax::kString );

	syntax.setObjectType( MSyntax::kNone );
}


//-----------------------------------------------------------------------------
// Does the work
//-----------------------------------------------------------------------------
MStatus CVsAssetCmd::doIt( const MArgList &mArgList )
{
	MStatus retVal = MS::kSuccess;
	MStatus mStatus = MS::kSuccess;
	MArgDatabase argDatabase( syntax(), mArgList, &mStatus );

	if ( argDatabase.isFlagSet( OPT_HELP ) )
	{
		GetSyntaxHelp()->PrintHelp( GetName().asChar(), "Manage assets" );
		return retVal;
	}
	 
	CDmeMakefile* pMakefile = CVsAssetBuilderFrame::GetMakefile();
	if ( argDatabase.isFlagSet( OPT_GET_ASSET_NAME ) )
	{
		const char *pFileName = pMakefile ? g_pDataModel->GetFileName( pMakefile->GetFileId() ) : "";

		char pFileBuf[MAX_PATH];
		Q_strncpy( pFileBuf, pFileName, sizeof(pFileBuf) );
		Q_FixSlashes( pFileBuf, '/' );

		Msg( "%s", pFileBuf );
		return retVal;
	}

	if ( argDatabase.isFlagSet( OPT_SAVE_ASSET ) || argDatabase.isFlagSet( OPT_SAVE_ASSET_ENCODING ) )
	{
		if ( !pMakefile )
			return retVal;

		MString fileEncoding;
		if ( !argDatabase.isFlagSet( OPT_SAVE_ASSET_ENCODING ) || !argDatabase.getFlagArgument( OPT_SAVE_ASSET_ENCODING, 0, fileEncoding ) )
		{
			fileEncoding = g_pDataModel->GetDefaultEncoding( ASSET_FILE_FORMAT );
		}
		const char *pFileName = g_pDataModel->GetFileName( pMakefile->GetFileId() );
		CDmElement *pRoot = GetElement< CDmElement >( g_pDataModel->GetFileRoot( pMakefile->GetFileId() ) );
		if ( !pRoot )
		{
			pRoot = pMakefile;
		}

		bool bOk = g_pDataModel->SaveToFile( pFileName, NULL, fileEncoding.asChar(), ASSET_FILE_FORMAT, pRoot ); 
		if ( !bOk )
		{
			retVal = MS::kFailure;
		}
		return retVal;
	}

	return retVal;
}
