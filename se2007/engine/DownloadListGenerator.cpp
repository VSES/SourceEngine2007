//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#include "mathlib/vector.h"
#include "DownloadListGenerator.h"
#include "filesystem.h"
#include "filesystem_engine.h"
#include "sys.h"
#include "cmd.h"
#include "common.h"
#include "quakedef.h"
#include "vengineserver_impl.h"
#include "tier1/strtools.h"
#include "tier0/icommandline.h"
#include "checksum_engine.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <tier0/dbg.h>

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

CDownloadListGenerator g_DownloadListGenerator;
CDownloadListGenerator &DownloadListGenerator()
{
	return g_DownloadListGenerator;
}

ConVar	sv_logdownloadlist( "sv_logdownloadlist", IsX360() ? "0" : "1" );

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CDownloadListGenerator::CDownloadListGenerator()
	: m_AlreadyWrittenFileNames( 0, 0, true )
{
	m_hReslistFile = FILESYSTEM_INVALID_HANDLE;
	m_pStringTable = NULL;
	m_mapName[0] = 0;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CDownloadListGenerator::SetStringTable( INetworkStringTable *pStringTable )
{
	if ( IsX360() )
	{
		// not supporting
		return;
	}

	m_pStringTable = pStringTable;

	// reset the duplication list
	m_AlreadyWrittenFileNames.RemoveAll();

	// add in the bsp file to the list, and its node graph and nav mesh
	char path[_MAX_PATH];
	Q_snprintf(path, sizeof(path), "maps\\%s.bsp", m_mapName);
	OnResourcePrecached(path);

	bool useNodeGraph = true;
	KeyValues *modinfo = new KeyValues("ModInfo");
	if ( modinfo->LoadFromFile( g_pFileSystem, "gameinfo.txt" ) )
	{
		useNodeGraph = modinfo->GetInt( "nodegraph", 1 ) != 0;
	}
	modinfo->deleteThis();

	if ( useNodeGraph )
	{
		Q_snprintf(path, sizeof(path), "maps\\graphs\\%s.ain", m_mapName);
		OnResourcePrecached(path);
	}

	Q_snprintf(path, sizeof(path), "maps\\%s.nav", m_mapName);
	OnResourcePrecached(path);

	char resfilename[MAX_OSPATH];
	KeyValues *resfilekeys = new KeyValues( "resourefiles" );

	Q_snprintf( resfilename, sizeof( resfilename), "maps/%s.res", m_mapName );
	
	if ( !resfilekeys->LoadFromFile( g_pFileSystem, resfilename, "GAME" ) )
		return; // no customized resource file found

	KeyValues *entry = resfilekeys->GetFirstSubKey();

	while ( entry )
	{
		OnResourcePrecached( entry->GetName() );

		entry = entry->GetNextKey();
	}
	resfilekeys->deleteThis();
}

//-----------------------------------------------------------------------------
// Purpose: call to mark level load/end
//-----------------------------------------------------------------------------
void CDownloadListGenerator::OnLevelLoadStart(const char *levelName)
{
	if ( IsX360() )
	{
		// not supporting
		return;
	}

	// close the previous level reslist, if any
	if (m_hReslistFile != FILESYSTEM_INVALID_HANDLE)
	{
		g_pFileSystem->Close(m_hReslistFile);
		m_hReslistFile = FILESYSTEM_INVALID_HANDLE;
	}

	// reset the duplication list
	m_AlreadyWrittenFileNames.RemoveAll();

	if ( sv_logdownloadlist.GetBool() )
	{
		// open the new level reslist
		char path[MAX_OSPATH];
		g_pFileSystem->CreateDirHierarchy( "DownloadLists", "MOD" );
		Q_snprintf(path, sizeof(path), "DownloadLists/%s.lst", levelName);
		m_hReslistFile = g_pFileSystem->Open(path, "wt", "GAME");
	}

	// add a slash to the end of com_gamedir, so we can only deal with files for this mod
	Q_snprintf( m_gameDir, sizeof(m_gameDir), "%s/", com_gamedir );
	Q_FixSlashes( m_gameDir );

	// save off the map name
	Q_snprintf( m_mapName, sizeof( m_mapName ), levelName );
}

//-----------------------------------------------------------------------------
// Purpose: call to mark level load/end
//-----------------------------------------------------------------------------
void CDownloadListGenerator::OnLevelLoadEnd()
{
	if ( IsX360() )
	{
		// not supporting
		return;
	}

	if ( m_hReslistFile != FILESYSTEM_INVALID_HANDLE )
	{
		g_pFileSystem->Close(m_hReslistFile);
		m_hReslistFile = FILESYSTEM_INVALID_HANDLE;
	}
	m_pStringTable = NULL;
}

//-----------------------------------------------------------------------------
// Purpose: logs and handles mdl files being precaches
//-----------------------------------------------------------------------------
void CDownloadListGenerator::OnModelPrecached(const char *relativePathFileName)
{
	if ( IsX360() )
	{
		// not supporting
		return;
	}

	if (Q_strstr(relativePathFileName, ".vmt"))
	{
		// it's a materials file, make sure that it starts in the materials directory, and we get the .vtf
		char file[_MAX_PATH];

		if (!Q_strnicmp(relativePathFileName, "materials", strlen("materials")))
		{
			Q_strncpy(file, relativePathFileName, sizeof(file));
		}
		else
		{
			// prepend the materials directory
			Q_snprintf(file, sizeof(file), "materials\\%s", relativePathFileName);
		}

		OnResourcePrecached(file);

		// get the matching vtf file
		char *ext = Q_strstr(file, ".vmt");
		if (ext)
		{
			Q_strncpy(ext, ".vtf", 5);
			OnResourcePrecached(file);
		}
	}
	else
	{
		OnResourcePrecached(relativePathFileName);
	}
}

//-----------------------------------------------------------------------------
// Purpose: logs sound file access
//-----------------------------------------------------------------------------
void CDownloadListGenerator::OnSoundPrecached(const char *relativePathFileName)
{
	if ( IsX360() )
	{
		// not supporting
		return;
	}

	// skip any special characters
	if (!isalnum(relativePathFileName[0]))
	{
		++relativePathFileName;
	}

	// prepend the sound/ directory if necessary
	char file[_MAX_PATH];
	if (!Q_strnicmp(relativePathFileName, "sound", strlen("sound")))
	{
		Q_strncpy(file, relativePathFileName, sizeof(file));
	}
	else
	{
		// prepend the materials directory
		Q_snprintf(file, sizeof(file), "sound\\%s", relativePathFileName);
	}

	OnResourcePrecached(file);
}

//-----------------------------------------------------------------------------
// Purpose: logs the precache as a file access
//-----------------------------------------------------------------------------
void CDownloadListGenerator::OnResourcePrecached(const char *relativePathFileName)
{
	if ( IsX360() )
	{
		// not supporting
		return;
	}

	// ignore empty string
	if (relativePathFileName[0] == 0)
	{
		return;
	}

	// ignore files that start with '*' since they signify special models
	if (relativePathFileName[0] == '*')
	{
		return;
	}

	char fullPath[_MAX_PATH];
	if (g_pFileSystem->GetLocalPath(relativePathFileName, fullPath, sizeof(fullPath)))
	{
		OnResourcePrecachedFullPath(fullPath);
	}
}


//-----------------------------------------------------------------------------
// Purpose: marks a precached file as needing a specific CRC on the client
//-----------------------------------------------------------------------------
void CDownloadListGenerator::ForceExactFile( const char *relativePathFileName, ConsistencyType consistency )
{
	if ( IsX360() )
	{
		// not supporting
		return;
	}

	if ( !m_pStringTable )
		return;

	if ( consistency != CONSISTENCY_EXACT && consistency != CONSISTENCY_SIMPLE_MATERIAL )
	{
		consistency = CONSISTENCY_EXACT;
	}

	CRC32_t crc;
	bool error = true;
	char file[_MAX_PATH];
	const char *filePtr = relativePathFileName;

	if (Q_strstr(relativePathFileName, ".vmt") || Q_strstr(relativePathFileName, ".vtf"))
	{
		// it's a materials file, make sure that it starts in the materials directory, and we get the .vtf
		if (!Q_strnicmp(relativePathFileName, "materials", strlen("materials")))
		{
			Q_strncpy(file, relativePathFileName, sizeof(file));
		}
		else
		{
			// prepend the materials directory
			Q_snprintf(file, sizeof(file), "materials\\%s", relativePathFileName);
		}
		error = !CRC_File( &crc, file );
		filePtr = file;
	}
	else
	{
		error = !CRC_File( &crc, relativePathFileName );
	}

	if ( error )
	{
		DevMsg( "Failed to CRC %s\n", relativePathFileName );
	}
	else
	{
		char relativeFileName[_MAX_PATH];
		Q_strncpy( relativeFileName, filePtr, sizeof( relativeFileName ) );
		Q_FixSlashes( relativeFileName );

		ExactFileUserData userData;
		userData.consistencyType = consistency;
		userData.crc = crc;

		int index = m_pStringTable->FindStringIndex( relativeFileName );
		if ( index != INVALID_STRING_INDEX )
		{
			m_pStringTable->SetStringUserData( index, sizeof( ExactFileUserData ), &userData );
		}
		else
		{
			m_pStringTable->AddString( true, relativeFileName, sizeof( ExactFileUserData ), &userData );
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: marks a precached model as having a maximum size on the client
//-----------------------------------------------------------------------------
void CDownloadListGenerator::ForceModelBounds( const char *relativePathFileName, const Vector &mins, const Vector &maxs )
{
	if ( IsX360() )
	{
		// not supporting
		return;
	}

	if ( !m_pStringTable )
		return;

	if ( !relativePathFileName )
		relativePathFileName = "";

	if (!Q_stristr(relativePathFileName, ".mdl"))
	{
		DevMsg( "Warning - trying to enforce model bounds on %s\n", relativePathFileName );
		return;
	}

	char relativeFileName[_MAX_PATH];
	Q_strncpy( relativeFileName, relativePathFileName, sizeof( relativeFileName ) );
	Q_FixSlashes( relativeFileName );

	ModelBoundsUserData userData;
	userData.consistencyType = CONSISTENCY_BOUNDS;
	userData.mins = mins;
	userData.maxs = maxs;

	int index = m_pStringTable->FindStringIndex( relativeFileName );
	if ( index != INVALID_STRING_INDEX )
	{
		m_pStringTable->SetStringUserData( index, sizeof( ModelBoundsUserData ), &userData );
	}
	else
	{
		m_pStringTable->AddString( true, relativeFileName, sizeof( ModelBoundsUserData ), &userData );
	}
}

//-----------------------------------------------------------------------------
// Purpose: Logs out file access to a file
//-----------------------------------------------------------------------------
void CDownloadListGenerator::OnResourcePrecachedFullPath( char *fullPathFileName )
{
	if ( IsX360() )
	{
		// not supporting
		return;
	}

	Q_FixSlashes( fullPathFileName );

	if ( !g_pFileSystem->FileExists( fullPathFileName ) )
	{
		return;	// don't allow files for download the server doesn't have
	}

	if ( Q_strncasecmp( m_gameDir, fullPathFileName, Q_strlen( m_gameDir ) ) )
	{
		return;	// the game dir must be part of the full name
	}

	// make sure the filename hasn't already been written
	UtlSymId_t filename = m_AlreadyWrittenFileNames.Find( fullPathFileName );
	if ( filename != UTL_INVAL_SYMBOL )
	{
		return;
	}

	// record in list, so we don't write it again
	m_AlreadyWrittenFileNames.AddString( fullPathFileName );

	// add extras for mdl's
	if (Q_strstr(fullPathFileName, ".mdl"))
	{
		// it's a model, get it's other files as well
		char file[_MAX_PATH];
		Q_strncpy(file, fullPathFileName, sizeof(file) - 10);
		char *ext = Q_strstr(file, ".mdl");

		Q_strncpy(ext, ".vvd", 10);
		OnResourcePrecachedFullPath(file);

		Q_strncpy(ext, ".ani", 10);
		OnResourcePrecachedFullPath(file);

		Q_strncpy(ext, ".dx80.vtx", 10);
		OnResourcePrecachedFullPath(file);

		Q_strncpy(ext, ".dx90.vtx", 10);
		OnResourcePrecachedFullPath(file);

		Q_strncpy(ext, ".sw.vtx", 10);
		OnResourcePrecachedFullPath(file);

		Q_strncpy(ext, ".phy", 10);
		OnResourcePrecachedFullPath(file);

		Q_strncpy(ext, ".jpg", 10);
		OnResourcePrecachedFullPath(file);
	}

	// strip it down relative to the root directory of the game (for steam)
	char const *relativeFileName = Q_stristr( fullPathFileName, com_gamedir );
	if ( relativeFileName )
	{
		// Skip the basedir and slash
		relativeFileName += ( Q_strlen( com_gamedir ) + 1 );
	}

	if ( !relativeFileName )
	{
		return;
	}

	FileHandle_t handle = m_hReslistFile;
	if ( handle != FILESYSTEM_INVALID_HANDLE )
	{
		g_pFileSystem->Write("\"", 1, handle);
		g_pFileSystem->Write( relativeFileName, Q_strlen(relativeFileName), handle );
		g_pFileSystem->Write("\"\n", 2, handle);
	}
	if ( m_pStringTable )
	{
		m_pStringTable->AddString( true, relativeFileName );
	}
}


	
