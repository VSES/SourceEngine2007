//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef PURE_SERVER_H
#define PURE_SERVER_H
#ifdef _WIN32
#pragma once
#endif


#include "ifilelist.h"
#include "tier1/utldict.h"
#include "tier1/utlbuffer.h"
#include "filesystem.h"


class KeyValues;


class CPureServerWhitelist
{
public:
	friend class CAllowFromDiskList;
	friend class CForceMatchList;

	// Call this to free it.
	static CPureServerWhitelist* Create( IFileSystem *pFileSystem );
	void				Release();
	
	// Get the file lists.
	IFileList*			GetAllowFromDiskList();		// This is the list of files that can be loaded off disk. Make sure to Release() it when finished!
	IFileList*			GetForceMatchList();		// This is the list of files that must match CRCs. Make sure to Release() it when finished!
	
	// Load up any entries in the KV file.
	bool				LoadFromKeyValues( KeyValues *kv );

	// Encode for networking.
	void				Encode( CUtlBuffer &buf );
	void				Decode( CUtlBuffer &buf );

	// Calls IFileSystem::CacheFileCRCs for all files and directories marked check_crc.
	void				CacheFileCRCs();

	// Instead of loading keyvalues, you can call this to make it force all files to come from Steam.
	// This is used with sv_pure 2.
	void				EnableFullyPureMode();
	bool				IsInFullyPureMode() const;

	// Used by sv_pure command when you're connected to a server.
	void				PrintWhitelistContents();

private:

	CPureServerWhitelist();
	~CPureServerWhitelist();

	void				Init( IFileSystem *pFileSystem );
	void				Term();

private:

	class CAllowFromDiskList : public IFileList
	{
	public:
		virtual bool			IsFileInList( const char *pFilename );
		virtual void			Release();

		CPureServerWhitelist	*m_pWhitelist;
	};
	class CForceMatchList : public IFileList
	{
	public:
		virtual bool			IsFileInList( const char *pFilename );
		virtual void			Release();
	
		CPureServerWhitelist	*m_pWhitelist;
	};

	class CCommand
	{
	public:
		CCommand();
		~CCommand();

	public:		
		bool			m_bAllowFromDisk;
		bool			m_bCheckCRC;
		unsigned short	m_LoadOrder;	// What order this thing was specified in the whitelist file.
	};


private:

	void				UpdateCommandStats( CUtlDict<CPureServerWhitelist::CCommand*,int> &commands, int *pHighest, int *pLongestPathName );
	void				PrintCommand( const char *pFileSpec, const char *pExt, int maxPathnameLen, CPureServerWhitelist::CCommand *pCommand );
	int					FindCommandByLoadOrder( CUtlDict<CPureServerWhitelist::CCommand*,int> &commands, int iLoadOrder );

	void InternalCacheFileCRCs( CUtlDict<CCommand*,int> &theList, ECacheCRCType eType );

	CCommand* GetBestEntry( const char *pFilename );

	void EncodeCommandList( CUtlDict<CPureServerWhitelist::CCommand*,int> &theList, CUtlBuffer &buf );
	void DecodeCommandList( CUtlDict<CPureServerWhitelist::CCommand*,int> &theList, CUtlBuffer &buf );

	CPureServerWhitelist::CCommand* CheckEntry( 
		CUtlDict<CPureServerWhitelist::CCommand*,int> &dict, 
		const char *pEntryName, 
		CPureServerWhitelist::CCommand *pBestEntry );

private:
	unsigned short	m_LoadCounter;	// Incremented as we load things so their m_LoadOrder increases.
	int m_RefCount;

	// Commands are applied to files in order.
	CUtlDict<CCommand*,int>	m_FileCommands;				// file commands
	CUtlDict<CCommand*,int> m_RecursiveDirCommands;		// ... commands
	CUtlDict<CCommand*,int> m_NonRecursiveDirCommands;	// *.* commands
	IFileSystem *m_pFileSystem;
	
	CAllowFromDiskList m_AllowFromDiskList;
	CForceMatchList m_ForceMatchList;
	
	// In this mode, all files must come from Steam.
	bool m_bFullyPureMode;
};


CPureServerWhitelist* CreatePureServerWhitelist( IFileSystem *pFileSystem );


#endif // PURE_SERVER_H

