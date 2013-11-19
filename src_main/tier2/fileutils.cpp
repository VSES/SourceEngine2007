//===== Copyright © 2005-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: Helper methods + classes for file access
//
//===========================================================================//

#include "tier2/fileutils.h"
#include "tier2/tier2.h"
#include "tier1/strtools.h"
#include "filesystem.h"
#include "tier0/icommandline.h"

ConVar fs_convert( "fs_convert", "1", 0, "Allow Xbox 360 files to be generated at runtime" );

//-----------------------------------------------------------------------------
// Builds a directory which is a subdirectory of the current mod
//-----------------------------------------------------------------------------
void GetModSubdirectory( const char *pSubDir, char *pBuf, int nBufLen )
{
	// Compute starting directory
	Assert( g_pFullFileSystem->GetSearchPath( "MOD", false, NULL, 0 ) < nBufLen );
	g_pFullFileSystem->GetSearchPath( "MOD", false, pBuf, nBufLen );
	char *pSemi = strchr( pBuf, ';' );
	if ( pSemi )
	{
		*pSemi = 0;
	}

	Q_StripTrailingSlash( pBuf );
	if ( pSubDir )
	{
		int nLen = Q_strlen( pSubDir );
		Q_strncat( pBuf, "\\", nBufLen, 1 );
		Q_strncat( pBuf, pSubDir, nBufLen, nLen );
	}

	Q_FixSlashes( pBuf );
}


//-----------------------------------------------------------------------------
// Builds a directory which is a subdirectory of the current mod's *content*
//-----------------------------------------------------------------------------
void GetModContentSubdirectory( const char *pSubDir, char *pBuf, int nBufLen )
{
	GetModSubdirectory( pSubDir, pBuf, nBufLen );
	char *pGame = Q_stristr( pBuf, "\\game\\" );
	if ( pGame )
	{
		int nDistToEnd = nBufLen - (int)( (size_t)pGame + 6 - (size_t)pBuf );
		memmove( pGame + 3, pGame, nDistToEnd - 3 );
		const char *pContent = "\\content\\";
		memcpy( pGame, pContent, 9 * sizeof(char) );
	}
	else
	{
		Warning( "Current mod isn't in a subdirectory of \"game\"!\n" );
	}
}

//-----------------------------------------------------------------------------
// Purpose: Generates an Xbox 360 filename from a PC filename
//-----------------------------------------------------------------------------
char *CreateX360Filename( const char *pSourceName, char *pTargetName, int targetLen )
{
	Q_StripExtension( pSourceName, pTargetName, targetLen );
	int idx = Q_strlen( pTargetName );

	// restore extension
	Q_snprintf( pTargetName, targetLen, "%s.360%s", pTargetName, &pSourceName[idx] );

	return pTargetName;
}

//-----------------------------------------------------------------------------
// Purpose: Generates a PC filename from a possible 360 name.
// Strips the .360. from filename.360.extension.
// Filenames might have multiple '.', need to be careful and only consider the
// last true extension. Complex filenames do occur:
// d:\foo\.\foo.dat
// d:\zip0.360.zip\foo.360.dat
// Returns source if no change needs to occur, othwerwise generates and
// returns target.
//-----------------------------------------------------------------------------
char *RestoreFilename( const char *pSourceName, char *pTargetName, int targetLen )
{
	// find extension
	// scan backward for '.', but not past a seperator
	int end = V_strlen( pSourceName ) - 1;
	while ( end > 0 && pSourceName[end] != '.' && !( pSourceName[end] == '\\' || pSourceName[end] == '/' ) )
	{
		--end;
	}

	if ( end >= 4 && pSourceName[end] == '.' && !V_strncmp( pSourceName + end - 4 , ".360", 4 ) )
	{
		// cull the .360, leave the trailing extension
		end -= 4;
		int length = min( end + 1, targetLen );
		V_strncpy( pTargetName, pSourceName, length );
		V_strncat( pTargetName, pSourceName + end + 4, targetLen );

		return pTargetName;
	}
	
	// source filename is as expected
	return (char *)pSourceName;
}

//-----------------------------------------------------------------------------
// Generate an Xbox 360 file if it doesn't exist or is out of date. This function determines
// the source and target path and whether the file needs to be generated. The caller provides
// a callback function to do the actual creation of the 360 file. "pExtraData" is for the caller to
// pass the address of any data that the callback function may need to access. This function
// is ONLY to be called by caller's who expect to have 360 versions of their file.
//-----------------------------------------------------------------------------
int UpdateOrCreate( const char *pSourceName, char *pTargetName, int targetLen, const char *pPathID, CreateCallback_t pfnCreate, bool bForce, void *pExtraData )
{
	//-----------------------------------------------------------------------------
	// Will re-activate later code after shipping, and pursue.
	// The data conversions are requiring a greater complexity, or are cross dependent.
	// New work needs to be done for a stable long term developer friendly solution.
	//-----------------------------------------------------------------------------
#if 1
	if ( pTargetName )
	{
		// caller could supply source as PC or 360 name, we want the PC filename
		char szFixedSourceName[MAX_PATH];
		pSourceName = RestoreFilename( pSourceName, szFixedSourceName, sizeof( szFixedSourceName ) );
		// caller wants us to provide 360 named version of source
		CreateX360Filename( pSourceName, pTargetName, targetLen );
	}

	// no conversion are performed by the game at runtime anymore...
	return UOC_NOT_CREATED;
#endif

	// caller could supply source as PC or 360 name, we want the PC filename
	char szFixedSourceName[MAX_PATH];
	pSourceName = RestoreFilename( pSourceName, szFixedSourceName, sizeof( szFixedSourceName ) );

	if ( pTargetName )
	{
		// caller wants us to provide 360 named version of source
		CreateX360Filename( pSourceName, pTargetName, targetLen );
	}

	// early out and do no further work
	if ( !fs_convert.GetBool() )
	{
		return UOC_NOT_CREATED;
	}

	// set early out trigger
	static bool bCommandLineChecked = false;
	if ( !bCommandLineChecked )
	{
		bCommandLineChecked = true;
		if ( ( g_pFullFileSystem->GetDVDMode() == DVDMODE_STRICT ) || CommandLine()->FindParm( "-noconvert" ) )
		{
			// no conversions during retail
			fs_convert.SetValue( 0 );
			return UOC_NOT_CREATED;
		}
	}

	if ( !g_pFullFileSystem )
	{
		return UOC_FAIL;
	}

	// determine location of source
	bool bSourceExists;
	char szFullSourcePath[MAX_PATH];
	PathTypeQuery_t SourcePathType = PATH_IS_NORMAL;
	if ( V_IsAbsolutePath( pSourceName ) )
	{
		V_strncpy( szFullSourcePath, pSourceName, sizeof( szFullSourcePath ) );
		bSourceExists = g_pFullFileSystem->FileExists( szFullSourcePath, NULL );
	}
	else
	{
		// resolve relative source to absolute
		bSourceExists = g_pFullFileSystem->RelativePathToFullPath( pSourceName, pPathID, szFullSourcePath, sizeof(szFullSourcePath), FILTER_NONE, &SourcePathType ) != NULL;
	}
	if ( !bSourceExists || IS_PACKFILE( SourcePathType ) )
	{
		// must have source to run conversion (but, some source's don't rightfully exist anyways) or ...
		// not allowing a conversion from a source file this is pak based or ...
		// cannot generate a target from any source that is inside a map's pak file
		return UOC_FAIL;
	}

	// determine location of target
	bool bTargetExists;
	char szFullTargetPath[MAX_PATH];
	PathTypeQuery_t TargetPathType = PATH_IS_NORMAL;
	char szTargetName[MAX_PATH];
	if ( !pTargetName )
	{
		pTargetName = szTargetName;
		targetLen = sizeof( szTargetName );
		CreateX360Filename( pSourceName, pTargetName, targetLen );
	}
	if ( V_IsAbsolutePath( pTargetName ) )
	{
		V_strncpy( szFullTargetPath, pTargetName, sizeof( szFullTargetPath ) );
		bTargetExists = g_pFullFileSystem->FileExists( szFullTargetPath, NULL );
	}
	else
	{
		// resolve relative target to absolute
		bTargetExists = g_pFullFileSystem->RelativePathToFullPath( pTargetName, pPathID, szFullTargetPath, sizeof(szFullTargetPath), FILTER_NONE, &TargetPathType ) != NULL;
		if ( bTargetExists && IS_REMOTE( TargetPathType ) && !IS_PACKFILE( TargetPathType ) )
		{
			// A target was found on the remote filesystem and not in a pack file.
			// make sure that source and target are in the same mod directory
			char szSourcePathOnly[MAX_PATH];
			char szTargetPathOnly[MAX_PATH];
			Q_ExtractFilePath( szFullSourcePath, szSourcePathOnly, sizeof( szSourcePathOnly ) );
			Q_ExtractFilePath( szFullTargetPath, szTargetPathOnly, sizeof( szTargetPathOnly ) );

			if ( Q_strnicmp( szSourcePathOnly, szTargetPathOnly, Q_strlen( szSourcePathOnly ) ) )
			{
				// Don't match
				bTargetExists = false;
			}
		}
		if ( !bTargetExists )
		{
			// use the full source path (which is non zip based) to resolve a valid writeable target
			CreateX360Filename( szFullSourcePath, szFullTargetPath, sizeof( szFullTargetPath ) );
		}
	}
	if ( bTargetExists && !IS_REMOTE( TargetPathType ) )
	{
		// cannot generate a target that resolves to the 360 local hard drive
		// target is taken as-is
		return UOC_NOT_CREATED;
	}

	// determine if the target file really needs to be created
	// creation occurs if it doesn't exist or it is out of date
	bool bCreate = true;
	if ( !bForce && bTargetExists )
	{
		long sourceFileTime = g_pFullFileSystem->GetFileTime( szFullSourcePath );
		long targetFileTime = g_pFullFileSystem->GetFileTime( szFullTargetPath );
		if ( sourceFileTime <= targetFileTime )
		{
			bCreate = false;
		}
	}	

	// run the creation function using absolute filenames
	// on ouput, caller can then expect that search paths will properly resolve a relative name to expected location
	int retval = UOC_NOT_CREATED;
	if ( bCreate )
	{
		// Call the create function callback
		if ( pfnCreate( szFullSourcePath, szFullTargetPath, NULL, pExtraData ) )
		{
			retval = UOC_CREATED;
			char *pShortPath = Q_stristr( szFullTargetPath, "game" );
			Msg( "Created %s\n", pShortPath ? pShortPath + 5 : szFullTargetPath );
		}
		else
		{
			// a hard failure, caller can't ignore, otherwise getting unintended data (i.e. searchpath fallthrough)
			retval = UOC_FAIL;
			Warning( "Failed to create: %s\n", szFullTargetPath );
			Assert( 0 );
		}
	}

	return retval;
}


//-----------------------------------------------------------------------------
// Returns the search path as a list of paths
//-----------------------------------------------------------------------------
void GetSearchPath( CUtlVector< CUtlString > &path, const char *pPathID )
{
	int nMaxLen = g_pFullFileSystem->GetSearchPath( pPathID, false, NULL, 0 );
	char *pBuf = (char*)_alloca( nMaxLen );
	g_pFullFileSystem->GetSearchPath( pPathID, false, pBuf, nMaxLen );

	char *pSemi;
	while ( NULL != ( pSemi = strchr( pBuf, ';' ) ) )
	{
		*pSemi = 0;
		path.AddToTail( pBuf );
		pBuf = pSemi + 1;
	}
	path.AddToTail( pBuf );
}


//-----------------------------------------------------------------------------
// Builds a list of all files under a directory with a particular extension
//-----------------------------------------------------------------------------
void AddFilesToList( CUtlVector< CUtlString > &list, const char *pDirectory, const char *pPathID, const char *pExtension )
{
	char pSearchString[MAX_PATH];
	Q_snprintf( pSearchString, MAX_PATH, "%s\\*", pDirectory );

	bool bIsAbsolute = Q_IsAbsolutePath( pDirectory );

	// get the list of files
	FileFindHandle_t hFind;
	const char *pFoundFile = g_pFullFileSystem->FindFirstEx( pSearchString, pPathID, &hFind );

	// add all the items
	CUtlVector< CUtlString > subDirs;
	for ( ; pFoundFile; pFoundFile = g_pFullFileSystem->FindNext( hFind ) )
	{
		char pChildPath[MAX_PATH];
		Q_snprintf( pChildPath, MAX_PATH, "%s\\%s", pDirectory, pFoundFile );

		if ( g_pFullFileSystem->FindIsDirectory( hFind ) )
		{
			if ( Q_strnicmp( pFoundFile, ".", 2 ) && Q_strnicmp( pFoundFile, "..", 3 ) )
			{
				subDirs.AddToTail( pChildPath );
			}
			continue;
		}

		// Check the extension matches
		if ( Q_stricmp( Q_GetFileExtension( pFoundFile ), pExtension ) )
			continue;

		char pFullPathBuf[MAX_PATH];
		char *pFullPath = pFullPathBuf;
		if ( !bIsAbsolute )
		{
			g_pFullFileSystem->RelativePathToFullPath( pChildPath, pPathID, pFullPathBuf, sizeof(pFullPathBuf) );
		}
		else
		{
			pFullPath = pChildPath;
		}

		strlwr( pFullPath );
		Q_FixSlashes( pFullPath );
		list.AddToTail( pFullPath );
	}

	g_pFullFileSystem->FindClose( hFind );

	int nCount = subDirs.Count();
	for ( int i = 0; i < nCount; ++i )
	{
		AddFilesToList( list, subDirs[i], pPathID, pExtension );
	}
}
