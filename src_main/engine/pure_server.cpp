//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $Workfile:     $
// $Date:         $
// $NoKeywords: $
//=============================================================================//

#include "pure_server.h"
#include "tier1/KeyValues.h"


static char *g_SvPure2_ProtectedDirs[] =
{
	"sound",
	"models",
	"materials"
};

static bool IsProtectedBySvPure2( const char *pFilename )
{
	for ( int i=0; i < ARRAYSIZE( g_SvPure2_ProtectedDirs ); i++ )
	{
		const char *pProtectedDir = g_SvPure2_ProtectedDirs[i];
		int len = V_strlen( pProtectedDir );
		
		if ( V_strlen( pFilename ) < len+1 )
			return false;
		
		char tempStr[512];
		Assert( len < ARRAYSIZE( tempStr ) );
		memcpy( tempStr, pFilename, len );
		tempStr[len] = 0;
		
		if ( V_stricmp( tempStr, pProtectedDir ) == 0 )
		{
			if ( pFilename[len] == '/' || pFilename[len] == '\\' )
				return true;
		}		
	}
	return false;
}



CPureServerWhitelist::CCommand::CCommand()
{
}

CPureServerWhitelist::CCommand::~CCommand()
{
}


CPureServerWhitelist* CPureServerWhitelist::Create( IFileSystem *pFileSystem )
{
	CPureServerWhitelist *pRet = new CPureServerWhitelist;
	pRet->Init( pFileSystem );
	return pRet;
}


CPureServerWhitelist::CPureServerWhitelist()
{
	m_pFileSystem = NULL;
	m_LoadCounter = 0;
	m_AllowFromDiskList.m_pWhitelist = this;
	m_ForceMatchList.m_pWhitelist = this;
	m_RefCount = 1;
	m_bFullyPureMode = false;
}


CPureServerWhitelist::~CPureServerWhitelist()
{
	Term();
}


void CPureServerWhitelist::Init( IFileSystem *pFileSystem )
{
	Term();
	m_pFileSystem = pFileSystem;
}


void CPureServerWhitelist::Term()
{
	m_FileCommands.PurgeAndDeleteElements();
	m_RecursiveDirCommands.PurgeAndDeleteElements();
	m_NonRecursiveDirCommands.PurgeAndDeleteElements();
	m_pFileSystem = NULL;
	m_LoadCounter = 0;
}


bool CPureServerWhitelist::LoadFromKeyValues( KeyValues *kv )
{
	for ( KeyValues *pCurItem = kv->GetFirstValue(); pCurItem; pCurItem = pCurItem->GetNextValue() )
	{
		char szPathName[ MAX_PATH ];
		const char *pKeyValue = pCurItem->GetName();
		const char *pModifiers = pCurItem->GetString();
		if ( !pKeyValue || !pModifiers )
			continue;
	
		Q_strncpy( szPathName, pKeyValue, sizeof(szPathName) );
		Q_FixSlashes( szPathName );
		const char *pValue = szPathName;

		// Figure out the modifiers.
		bool bFromSteam = false, bAllowFromDisk = false, bCheckCRC = false;
		CUtlVector<char*> mods;
		V_SplitString( pModifiers, "+", mods );
		for ( int i=0; i < mods.Count(); i++ )
		{
			if ( V_stricmp( mods[i], "from_steam" ) == 0 )
				bFromSteam = true;
			else if ( V_stricmp( mods[i], "allow_from_disk" ) == 0 )
				bAllowFromDisk = true;
			else if ( V_stricmp( mods[i], "check_crc" ) == 0 )
				bCheckCRC = true;
			else
				Warning( "Unknown modifier in whitelist file: %s.\n", mods[i] );
		}
		mods.PurgeAndDeleteElements();
		if ( bFromSteam && (bAllowFromDisk || bCheckCRC) )
		{
			bAllowFromDisk = bCheckCRC = false;
			Warning( "Whitelist: from_steam not compatible with other modifiers (used on %s).\n", pValue );
			Warning( "           Other markers removed.\n" );
		}

		
		// Setup a new CCommand to hold this command.
		CPureServerWhitelist::CCommand *pCommand = new CPureServerWhitelist::CCommand;
		pCommand->m_LoadOrder = m_LoadCounter++;
		pCommand->m_bAllowFromDisk = bAllowFromDisk;
		pCommand->m_bCheckCRC = bCheckCRC;

		// Figure out if they're referencing a file, a recursive directory, or a nonrecursive directory.		
		CUtlDict<CCommand*,int> *pList;
		const char *pEndPart = V_UnqualifiedFileName( pValue );
		if ( Q_stricmp( pEndPart, "..." ) == 0 )
			pList = &m_RecursiveDirCommands;
		else if ( Q_stricmp( pEndPart, "*.*" ) == 0 )
			pList = &m_NonRecursiveDirCommands;
		else
			pList = &m_FileCommands;
		
		// If it's a directory command, get rid of the *.* or ...
		char filePath[MAX_PATH];
		if ( pList == &m_RecursiveDirCommands || pList == &m_NonRecursiveDirCommands )
			V_ExtractFilePath( pValue, filePath, sizeof( filePath ) );
		else
			V_strncpy( filePath, pValue, sizeof( filePath ) );
		
		V_FixSlashes( filePath );

		// Add the command to the appropriate list.
		if ( pList->Find( filePath ) == pList->InvalidIndex() )
		{
			pList->Insert( filePath, pCommand );
		}
		else
		{
			Error( "Pure server whitelist entry '%s' is a duplicate.\n", filePath );
		}		
	}	
	
	return true;
}


void CPureServerWhitelist::EnableFullyPureMode()
{
	// In this mode, all files must come from Steam.
	m_FileCommands.PurgeAndDeleteElements();
	m_RecursiveDirCommands.PurgeAndDeleteElements();
	m_NonRecursiveDirCommands.PurgeAndDeleteElements();
	m_bFullyPureMode = true;
}


bool CPureServerWhitelist::IsInFullyPureMode() const
{
	return m_bFullyPureMode;
}


void CPureServerWhitelist::UpdateCommandStats( CUtlDict<CPureServerWhitelist::CCommand*,int> &commands, int *pHighest, int *pLongestPathName )
{
	for ( int i=commands.First(); i != commands.InvalidIndex(); i=commands.Next( i ) )
	{
		*pHighest = max( *pHighest, commands[i]->m_LoadOrder );
		
		int len = V_strlen( commands.GetElementName( i ) );
		*pLongestPathName = max( *pLongestPathName, len );
	}
}

void CPureServerWhitelist::PrintCommand( const char *pFileSpec, const char *pExt, int maxPathnameLen, CPureServerWhitelist::CCommand *pCommand )
{
	// Get rid of the trailing slash if there is one.
	char tempFileSpec[MAX_PATH];
	V_strncpy( tempFileSpec, pFileSpec, sizeof( tempFileSpec ) );
	int len = V_strlen( tempFileSpec );
	if ( len > 0 && (tempFileSpec[len-1] == '/' || tempFileSpec[len-1] == '\\') )
		tempFileSpec[len-1] = 0;
		
	if ( pExt )
		Msg( "%s%c%s", tempFileSpec, CORRECT_PATH_SEPARATOR, pExt );
	else
		Msg( "%s", tempFileSpec );

	len = V_strlen( pFileSpec );
	for ( int i=len; i < maxPathnameLen+6; i++ )
	{
		Msg( " " );
	}
	
	Msg( "\t" );
	if ( pCommand->m_bAllowFromDisk && pCommand->m_bCheckCRC )
		Msg( "allow_from_disk+check_crc" );
	else if ( pCommand->m_bAllowFromDisk )
		Msg( "allow_from_disk" );
	else
		Msg( "from_steam" );
	
	Msg( "\n" );
}


int CPureServerWhitelist::FindCommandByLoadOrder( CUtlDict<CPureServerWhitelist::CCommand*,int> &commands, int iLoadOrder )
{
	for ( int i=commands.First(); i != commands.InvalidIndex(); i=commands.Next( i ) )
	{
		if ( commands[i]->m_LoadOrder == iLoadOrder )
			return i;
	}
	return -1;
}


void CPureServerWhitelist::PrintWhitelistContents()
{
	int highestLoadOrder = 0, longestPathName = 0;
	UpdateCommandStats( m_FileCommands, &highestLoadOrder, &longestPathName );
	UpdateCommandStats( m_RecursiveDirCommands, &highestLoadOrder, &longestPathName );
	UpdateCommandStats( m_NonRecursiveDirCommands, &highestLoadOrder, &longestPathName );
	
	for ( int iLoadOrder=0; iLoadOrder <= highestLoadOrder; iLoadOrder++ )
	{
		// Check regular file commands.
		int iCommand = FindCommandByLoadOrder( m_FileCommands, iLoadOrder );
		if ( iCommand != -1 )
		{
			PrintCommand( m_FileCommands.GetElementName( iCommand ), NULL, longestPathName, m_FileCommands[iCommand] );
		}
		else
		{
			// Check recursive commands.
			iCommand = FindCommandByLoadOrder( m_RecursiveDirCommands, iLoadOrder );
			if ( iCommand != -1 )
			{
				PrintCommand( m_RecursiveDirCommands.GetElementName( iCommand ), "...", longestPathName, m_RecursiveDirCommands[iCommand] );
			}
			else
			{
				// Check *.* commands.
				iCommand = FindCommandByLoadOrder( m_NonRecursiveDirCommands, iLoadOrder );
				if ( iCommand != -1 )
				{
					PrintCommand( m_NonRecursiveDirCommands.GetElementName( iCommand ), "*.*", longestPathName, m_NonRecursiveDirCommands[iCommand] );
				}
			}
		}
	}
}


void CPureServerWhitelist::Encode( CUtlBuffer &buf )
{
	EncodeCommandList( m_FileCommands, buf );
	EncodeCommandList( m_RecursiveDirCommands, buf );
	EncodeCommandList( m_NonRecursiveDirCommands, buf );
	buf.PutChar( (char)m_bFullyPureMode );
}


void CPureServerWhitelist::EncodeCommandList( CUtlDict<CPureServerWhitelist::CCommand*,int> &theList, CUtlBuffer &buf )
{
	buf.PutInt( theList.Count() );
	for ( int i=theList.First(); i != theList.InvalidIndex(); i = theList.Next( i ) )
	{
		CPureServerWhitelist::CCommand *pCommand = theList[i];
		
		unsigned char val = 0;
		if ( pCommand->m_bAllowFromDisk )
			val |= 0x01;
		if ( pCommand->m_bCheckCRC )
			val |= 0x02;
		
		buf.PutUnsignedChar( val );
		buf.PutUnsignedShort( pCommand->m_LoadOrder );
		buf.PutString( theList.GetElementName( i ) );
	}
}


void CPureServerWhitelist::Decode( CUtlBuffer &buf )
{
	DecodeCommandList( m_FileCommands, buf );
	DecodeCommandList( m_RecursiveDirCommands, buf );
	DecodeCommandList( m_NonRecursiveDirCommands, buf );

	if ( buf.GetBytesRemaining() >= 1 )
	{
		m_bFullyPureMode = (buf.GetChar() != 0);
	}
	else
	{
		m_bFullyPureMode = false;
	}
}


void CPureServerWhitelist::CacheFileCRCs()
{
	InternalCacheFileCRCs( m_FileCommands, k_eCacheCRCType_SingleFile );
	InternalCacheFileCRCs( m_NonRecursiveDirCommands, k_eCacheCRCType_Directory );
	InternalCacheFileCRCs( m_RecursiveDirCommands, k_eCacheCRCType_Directory_Recursive );
}


void CPureServerWhitelist::InternalCacheFileCRCs( CUtlDict<CCommand*,int> &theList, ECacheCRCType eType )
{
	for ( int i=theList.First(); i != theList.InvalidIndex(); i = theList.Next( i ) )
	{
		CCommand *pCommand = theList[i];
		if ( pCommand->m_bCheckCRC )
		{
			const char *pPathname = theList.GetElementName( i );
			m_pFileSystem->CacheFileCRCs( pPathname, eType, &m_ForceMatchList );
		}
	}
}


void CPureServerWhitelist::DecodeCommandList( CUtlDict<CPureServerWhitelist::CCommand*,int> &theList, CUtlBuffer &buf )
{
	int nCommands = buf.GetInt();
	
	for ( int i=0; i < nCommands; i++ )
	{
		CPureServerWhitelist::CCommand *pCommand = new CPureServerWhitelist::CCommand;
		
		unsigned char val = buf.GetUnsignedChar();
		pCommand->m_bAllowFromDisk = (( val & 0x01 ) != 0);
		pCommand->m_bCheckCRC      = (( val & 0x02 ) != 0);

		pCommand->m_LoadOrder = buf.GetUnsignedShort();

		char str[MAX_PATH];
		buf.GetString( str, sizeof( str )-1 );
		V_FixSlashes( str );
		
		theList.Insert( str, pCommand );
	}
}


CPureServerWhitelist::CCommand* CPureServerWhitelist::GetBestEntry( const char *pFilename )
{
	// NOTE: Since this is a user-specified file, we don't have the added complexity of path IDs in here.
	// So when the filesystem asks if a file is in the whitelist, we just ignore the path ID.
	
	// Make sure we have a relative pathname with fixed slashes..
	char relativeFilename[MAX_PATH];
	V_strncpy( relativeFilename, pFilename, sizeof( relativeFilename ) );

	// Convert the path to relative if necessary.
	if ( !V_IsAbsolutePath( relativeFilename ) || m_pFileSystem->FullPathToRelativePath( pFilename, relativeFilename, sizeof( relativeFilename ) ) )
	{
		V_FixSlashes( relativeFilename );
		
		// Get the directory this thing is in.
		char relativeDir[MAX_PATH];
		if ( !V_ExtractFilePath( relativeFilename, relativeDir, sizeof( relativeDir ) )	)
			relativeDir[0] = 0;
		
		
		// Check each of our dictionaries to see if there is an entry for this thing.
		CCommand *pBestEntry = NULL;
		
		pBestEntry = CheckEntry( m_FileCommands, relativeFilename, pBestEntry );
		if ( relativeDir[0] != 0 )
		{
			pBestEntry = CheckEntry( m_NonRecursiveDirCommands, relativeDir, pBestEntry );

			while ( relativeDir[0] != 0 )
			{
				// Check for this directory.
				pBestEntry = CheckEntry( m_RecursiveDirCommands, relativeDir, pBestEntry );
				if ( !V_StripLastDir( relativeDir, sizeof( relativeDir ) ) )
					break;
			}
		}
			
		return pBestEntry;
	}
	
	// Either we couldn't find an entry, or they specified an absolute path that we could not convert to a relative path.
	return NULL;
}


CPureServerWhitelist::CCommand* CPureServerWhitelist::CheckEntry( 
	CUtlDict<CPureServerWhitelist::CCommand*,int> &dict, 
	const char *pEntryName, 
	CPureServerWhitelist::CCommand *pBestEntry )
{
	int i = dict.Find( pEntryName );
	if ( i != dict.InvalidIndex() && (!pBestEntry || dict[i]->m_LoadOrder > pBestEntry->m_LoadOrder) )
		pBestEntry = dict[i];
	
	return pBestEntry;
}


void CPureServerWhitelist::Release()
{
	if ( --m_RefCount <= 0 )
		delete this;
}


IFileList* CPureServerWhitelist::GetAllowFromDiskList()
{
	++m_RefCount;
	return &m_AllowFromDiskList;
}


IFileList* CPureServerWhitelist::GetForceMatchList()
{
	++m_RefCount;
	return &m_ForceMatchList;
}


// --------------------------------------------------------------------------------------------------- //
// CAllowFromDiskList/CForceMatchList implementation.
// --------------------------------------------------------------------------------------------------- //

bool CPureServerWhitelist::CAllowFromDiskList::IsFileInList( const char *pFilename )
{
	// In "fully pure" mode, all files must come from disk.
	if ( m_pWhitelist->m_bFullyPureMode )
	{
		// Only protect maps, models, and sounds.
		if ( IsProtectedBySvPure2( pFilename ) )
			return false;
		else
			return true;
	}
		
	CCommand *pCommand = m_pWhitelist->GetBestEntry( pFilename );
	if ( pCommand )
		return pCommand->m_bAllowFromDisk;
	else
		return true;	// All files are allowed to come from disk by default.
}

void CPureServerWhitelist::CAllowFromDiskList::Release()
{
	m_pWhitelist->Release();
}

bool CPureServerWhitelist::CForceMatchList::IsFileInList( const char *pFilename )
{
	// In "fully pure" mode, all files must come from disk.
	// We shouldn't need this because all the relevant files should be loaded from Steam, but just in case, early out here.
	if ( m_pWhitelist->m_bFullyPureMode )
		return false;

	CCommand *pCommand = m_pWhitelist->GetBestEntry( pFilename );
	if ( pCommand )
		return pCommand->m_bCheckCRC;
	else
		return false;	// By default, no files require the CRC check.
}

void CPureServerWhitelist::CForceMatchList::Release()
{
	m_pWhitelist->Release();
}

