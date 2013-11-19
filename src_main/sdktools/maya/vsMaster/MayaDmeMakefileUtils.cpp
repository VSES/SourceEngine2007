//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: Class for dealing with vgui focus issues across all plugins
//
// $NoKeywords: $
//===========================================================================//

#include "movieobjects/dmemakefileutils.h"
#include "movieobjects/dmemdlmakefile.h"
#include "movieobjects/dmedccmakefile.h"
#include "maya/mstring.h"
#include "maya/mglobal.h"
#include "maya/mfileio.h"
#include "imayavgui.h"
#include "filesystem.h"


//-----------------------------------------------------------------------------
// Implementation class
//-----------------------------------------------------------------------------
class CMayaDmeMakefileUtils : public CDmeMakefileUtils
{
	DECLARE_DMEMAKEFILE_UTIL_CLASS( CMayaDmeMakefileUtils, CDmeMakefileUtils );

public:
	CMayaDmeMakefileUtils(void);
	virtual ~CMayaDmeMakefileUtils(void);

private:
	DECLARE_COMPILEFUNC( CDmeMayaMakefile );
	DECLARE_COMPILEFUNC( CDmeMDLMakefile );
	DECLARE_OPENEDITORFUNC( CDmeSourceMayaFile );
};

IMPLEMENT_DMEMAKEFILE_UTIL_CLASS( CMayaDmeMakefileUtils );


//-----------------------------------------------------------------------------
// Purpose: singleton accessor
//-----------------------------------------------------------------------------
static CMayaDmeMakefileUtils s_MayaDmeMakefileUtils;
EXPOSE_SINGLE_INTERFACE_GLOBALVAR( CMayaDmeMakefileUtils, IDmeMakefileUtils, DMEMAKEFILE_UTILS_INTERFACE_VERSION, s_MayaDmeMakefileUtils );


//-----------------------------------------------------------------------------
// Constructor, destructor
//-----------------------------------------------------------------------------
CMayaDmeMakefileUtils::CMayaDmeMakefileUtils()
{
}

CMayaDmeMakefileUtils::~CMayaDmeMakefileUtils(void)
{
}


//-----------------------------------------------------------------------------
// Add compile dependencies
//-----------------------------------------------------------------------------
bool CMayaDmeMakefileUtils::PerformCompilationStep( CDmeMDLMakefile *pMakefile, CompilationStep_t step )
{
	if ( !pMakefile || ( step != BUILDING_STANDARD_DEPENDENCIES ) )
		return BaseClass::PerformCompilationStep( pMakefile, step );

	MString currentFile = MFileIO::currentFile();

	CUtlVector< CUtlString > outputs;
	int nCount = pMakefile->GetSourceCount();
	for ( int i = 0; i < nCount; ++i )
	{
		CDmeSource *pSource = pMakefile->GetSource( i );
		if ( !pSource )
			continue;

		CDmeMayaMakefile *pDependentMakefile = CastElement< CDmeMayaMakefile >( pSource->GetDependentMakefile() );
		if ( !pDependentMakefile )
			continue;

		// Does the output files exist?
		int j;
		pDependentMakefile->GetOutputs( outputs );
		int nOutputCount = outputs.Count();
		for ( j = 0; j < nOutputCount; ++j )
		{
			// If the file doesn't exist, we have to build it
			if ( !g_pFullFileSystem->FileExists( outputs[j] ) )
				break;
		}
		if ( j != nOutputCount )
		{
			AddCompilationTask( pDependentMakefile );
			continue;
		}

		int nSourceCount = pDependentMakefile->GetSourceCount();
		for ( j = 0; j < nSourceCount; ++j )
		{
			CDmeSourceMayaFile *pMayaSource = CastElement< CDmeSourceMayaFile >( pDependentMakefile->GetSource(j) );
			if ( !pMayaSource )
				continue;

			char pSourcePath[MAX_PATH];
			pDependentMakefile->GetSourceFullPath( pMayaSource, pSourcePath, sizeof(pSourcePath) );

			// Maya wants forward slashes
			Q_FixSlashes( pSourcePath, '/' );

			if ( Q_stricmp( currentFile.asChar(), pSourcePath ) )
				continue;

			// Queue up the compilation task
			AddCompilationTask( pDependentMakefile );
			break;
		}
	}

	return true;
}


//-----------------------------------------------------------------------------
// Exports a Maya file to a DMX file
//-----------------------------------------------------------------------------
bool CMayaDmeMakefileUtils::PerformCompilationStep( CDmeMayaMakefile *pMakeFile, CompilationStep_t step )
{
	if ( step != BEFORE_COMPILATION )
		return BaseClass::PerformCompilationStep( pMakeFile, step );

	MString currentFile = MFileIO::currentFile();

	int nCount = pMakeFile->GetSourceCount();
	for ( int i = 0; i < nCount; ++i )
	{
		CDmeSourceMayaFile *pMayaSource = CastElement< CDmeSourceMayaFile >( pMakeFile->GetSource(i) );
		if ( !pMayaSource )
			continue;

		char pSourcePath[MAX_PATH];
		pMakeFile->GetSourceFullPath( pMayaSource, pSourcePath, sizeof(pSourcePath) );
		 
		// Maya wants forward slashes
		Q_FixSlashes( pSourcePath, '/' );

		if ( Q_stricmp( currentFile.asChar(), pSourcePath ) )
			continue;

		// It's already open
		MString command = "saveChanges(\"\")";
		g_pMayaVGui->SetModalMode( true );

		int nResult;
		MStatus status = MGlobal::executeCommand( command, nResult );
		g_pMayaVGui->SetModalMode( false );
		if ( status != MStatus::kSuccess || ( nResult == 0 ) )
			return false;
		break;
	}

	return BaseClass::PerformCompilationStep( pMakeFile, step );
}


//-----------------------------------------------------------------------------
// Opens Maya with a particular file
//-----------------------------------------------------------------------------
void CMayaDmeMakefileUtils::OpenEditor( CDmeSourceMayaFile *pDmeSourceDCCFile )
{
	CDmeMayaMakefile *pMakefile = FindReferringElement< CDmeMayaMakefile >( pDmeSourceDCCFile, "sources" );
	if ( !pMakefile )
		return;

	MString currentFile = MFileIO::currentFile();

	char pSourcePath[MAX_PATH];
	pMakefile->GetSourceFullPath( pDmeSourceDCCFile, pSourcePath, sizeof(pSourcePath) );

	// Maya wants forward slashes
	Q_FixSlashes( pSourcePath, '/' );

	// It's already open
	if ( !Q_stricmp( currentFile.asChar(), pSourcePath ) )
		return;

	int nResult;
	MString loadCommand = "vsLoadFile \"";
	loadCommand += pSourcePath;
	loadCommand += "\"";

	MGlobal::executeCommand( loadCommand, nResult );
}
