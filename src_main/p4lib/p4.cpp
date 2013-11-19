//====== Copyright © 1996-2005, Valve Corporation, All rights reserved. =======
//
// Purpose: 
//
//=============================================================================

// prevent Error function from being defined, since it inteferes with the P4 API
#define Error Warning
#include "p4lib/ip4.h"
#include "tier1/UtlVector.h"
#include "tier1/UtlSymbol.h"
#include "tier1/UtlBuffer.h"
#include "tier1/utlstring.h"
#include "tier1/utlmap.h"
#undef Error

#include "filesystem.h"

#undef Verify
#include "clientapi.h"

#include <time.h>
#include <ctype.h>
#include <io.h>
#include <sys/stat.h>

#include <windows.h>

#define CLIENTSPEC_BUFFER_SIZE		(8 * 1024)


//-----------------------------------------------------------------------------
// Stores info about the client path
//-----------------------------------------------------------------------------
struct CClientPathRecord
{
	char m_szDepotPath[MAX_PATH];
	char m_szClientPath[MAX_PATH];
	bool m_bNegative;
};

	
//-----------------------------------------------------------------------------
// Global interfaces
//-----------------------------------------------------------------------------
static IFileSystem *g_pFileSystem;


//-----------------------------------------------------------------------------
// Purpose: Interface to accessing P4 commands
//-----------------------------------------------------------------------------
class CP4 : public CBaseAppSystem<IP4>
{
public:
	// Destructor
	virtual ~CP4();

	// Methods of IAppSystem
	virtual bool Connect( CreateInterfaceFn factory );
	virtual InitReturnVal_t Init();
	virtual void *QueryInterface( const char *pInterfaceName );
	virtual void Shutdown();
	virtual void Disconnect();

	// Inherited from IP4
	P4Client_t &GetActiveClient();
 	virtual void RefreshActiveClient();
	virtual void SetActiveClient(const char *clientname);
	virtual void GetDepotFilePath(char *depotFilePath, const char *filespec, int size);
	virtual void GetClientFilePath(char *clientFilePath, const char *filespec, int size);
	virtual void GetLocalFilePath(char *localFilePath, const char *filespec, int size);
	virtual CUtlVector<P4File_t> &GetFileList(const char *path);
	virtual CUtlVector<P4File_t> &GetFileListUsingClientSpec( const char *pPath, const char *pClientSpec );
	virtual void GetOpenedFileList( CUtlVector<P4File_t> &fileList );
	virtual void GetOpenedFileList( const char *pRootDirectory, CUtlVector<P4File_t> &fileList );
	virtual void GetOpenedFileListInPath( const char *pPathID, CUtlVector<P4File_t> &fileList );
	virtual CUtlVector<P4Revision_t> &GetRevisionList(const char *path, bool bIsDir);
	virtual CUtlVector<P4Client_t> &GetClientList();
	virtual void RemovePathFromActiveClientspec(const char *path);
	virtual void SetOpenFileChangeList( const char *pChangeListName );
	virtual bool OpenFileForAdd( const char *fullpath );
	virtual bool OpenFileForEdit(const char *fullpath);
	virtual bool OpenFileForDelete(const char *fullpath);
	virtual bool SubmitFile( const char *pFullPath, const char *pDescription );
	virtual bool RevertFile( const char *pFullPath );
	virtual bool OpenFilesForAdd( int nCount, const char **ppFullPathList );
	virtual bool OpenFilesForEdit( int nCount, const char **ppFullPathList );
	virtual bool OpenFilesForDelete( int nCount, const char **ppFullPathList );
	virtual bool SubmitFiles( int nCount, const char **ppFullPathList, const char *pDescription );
	virtual bool RevertFiles( int nCount, const char **ppFullPathList );
	virtual bool IsFileInPerforce( const char *fullpath );
	virtual P4FileState_t GetFileState( const char *pFullPath );
	virtual bool GetFileInfo( const char *pFullPath, P4File_t *pFileInfo );

	virtual const char *GetDepotRoot();
	virtual int GetDepotRootLength();
	virtual const char *GetLocalRoot();
	virtual int GetLocalRootLength();
	virtual const char *String( CUtlSymbol s ) const;
	virtual bool GetClientSpecForFile( const char *pFullPath, char *pClientSpec, int nMaxLen );
	virtual bool GetClientSpecForDirectory( const char *pFullPathDir, char *pClientSpec, int nMaxLen );
	virtual bool GetClientSpecForPath( const char *pPathId, char *pClientSpec, int nMaxLen );
	virtual void OpenFileInP4Win( const char *pFullPath );
	virtual bool IsConnectedToServer( bool bRetry = true );
	virtual const char *GetLastError();
	char const * GetOpenFileChangeListNum(); // Returns NULL for default

	// Convert a depot file to a local file user the current client mapping
	bool DepotFileToLocalFile( const char *pDepotFile, char *pLocalFile, int nBufLen );

	// Accessors
	ClientApi &GetClientApi() { return m_Client; }
	ClientUser &GetClientUser() { return m_User; }

private:
	// Helper for operations on multiple files
	typedef void (*PerforceOp_t)( int nCount, const char **ppFullPathList, const char *pDescription );
	bool PerformPerforceOp( PerforceOp_t op, int nCount, const char **ppFullPathList, const char *pDescription );
	bool PerformPerforceOp( const char *pOperation, int nCount, const char **ppFullPathList );
	bool PerformPerforceOp( const char *pOperation, const char *pFullPath );

	bool PerformPerforceOpCurChangeList( const char *pOperation, int nCount, const char **ppFullPathList );
	bool PerformPerforceOpCurChangeList( const char *pOperation, const char *pFullPath );

	void RefreshClientData();

	bool m_bConnectedToServer;
	P4Client_t m_ActiveClient;
	CUtlVector< CClientPathRecord >	m_ClientMapping;
	char m_szDepotRoot[_MAX_PATH];
	char m_szLocalRoot[_MAX_PATH];
	int m_iDepotRootLength;
	int m_iLocalRootLength;
	CUtlString m_sChangeListName, m_sCachedChangeListNum;
	int m_nCachedChangeListNumber;

	// internal
	ClientApi m_Client;
	ClientUser m_User;
};


//-----------------------------------------------------------------------------
// global instance
//-----------------------------------------------------------------------------
CP4 p4;
EXPOSE_SINGLE_INTERFACE_GLOBALVAR( CP4, IP4, P4_INTERFACE_VERSION, p4 );


//-----------------------------------------------------------------------------
// Scoped class to push and pop a particular client spec 
//-----------------------------------------------------------------------------
class CScopedClientSpec
{
public:
	CScopedClientSpec( const char *pClientSpec );	
	~CScopedClientSpec();

private:
	char m_pOldClientSpec[MAX_PATH];
	bool m_bClientSpecNeedsRestore;
};

CScopedClientSpec::CScopedClientSpec( const char *pClientSpec )
{
	m_bClientSpecNeedsRestore = false;
	Q_strncpy( m_pOldClientSpec, p4.GetClientApi().GetClient().Text(), sizeof(m_pOldClientSpec) );
	if ( Q_stricmp( m_pOldClientSpec, pClientSpec ) )
	{
		p4.SetActiveClient( pClientSpec );
		m_bClientSpecNeedsRestore = true;
	}
}

CScopedClientSpec::~CScopedClientSpec()
{
	if ( m_bClientSpecNeedsRestore )
	{
		p4.SetActiveClient( m_pOldClientSpec );
	}
}


class CScopedFileClientSpec
{
public:
	CScopedFileClientSpec( const char *pFullPath );	
	~CScopedFileClientSpec();

private:
	char m_pOldClientSpec[MAX_PATH];
	bool m_bClientSpecNeedsRestore;
};

CScopedFileClientSpec::CScopedFileClientSpec( const char *pFullPath )
{
	m_bClientSpecNeedsRestore = false;
	char pClientSpec[MAX_PATH];
	if ( p4.GetClientSpecForFile( pFullPath, pClientSpec, sizeof(pClientSpec) ) )
	{
		Q_strncpy( m_pOldClientSpec, p4.GetClientApi().GetClient().Text(), sizeof(m_pOldClientSpec) );
		if ( Q_stricmp( m_pOldClientSpec, pClientSpec ) )
		{
			p4.SetActiveClient( pClientSpec );
			m_bClientSpecNeedsRestore = true;
		}
	}
}

CScopedFileClientSpec::~CScopedFileClientSpec()
{
	if ( m_bClientSpecNeedsRestore )
	{
		p4.SetActiveClient( m_pOldClientSpec );
	}
}


//-----------------------------------------------------------------------------
// Scoped class to push and pop a particular client spec 
//-----------------------------------------------------------------------------
class CScopedDirClientSpec
{
public:
	CScopedDirClientSpec( const char *pFullPathDir );	
	~CScopedDirClientSpec();

private:
	char m_pOldClientSpec[MAX_PATH];
	bool m_bClientSpecNeedsRestore;
};

CScopedDirClientSpec::CScopedDirClientSpec( const char *pFullPathDir )
{
	m_bClientSpecNeedsRestore = false;
	char pClientSpec[MAX_PATH];
	if ( p4.GetClientSpecForDirectory( pFullPathDir, pClientSpec, sizeof(pClientSpec) ) )
	{
		Q_strncpy( m_pOldClientSpec, p4.GetClientApi().GetClient().Text(), sizeof(m_pOldClientSpec) );
		if ( Q_stricmp( m_pOldClientSpec, pClientSpec ) )
		{
			p4.SetActiveClient( pClientSpec );
			m_bClientSpecNeedsRestore = true;
		}
	}
}

CScopedDirClientSpec::~CScopedDirClientSpec()
{
	if ( m_bClientSpecNeedsRestore )
	{
		p4.SetActiveClient( m_pOldClientSpec );
	}
}


//-----------------------------------------------------------------------------
// Scoped class to push and pop a particular client spec 
//-----------------------------------------------------------------------------
class CScopedPathClientSpec
{
public:
	CScopedPathClientSpec( const char *pPathID );	
	~CScopedPathClientSpec();

private:
	char m_pOldClientSpec[MAX_PATH];
	bool m_bClientSpecNeedsRestore;
};

CScopedPathClientSpec::CScopedPathClientSpec( const char *pPathID )
{
	m_bClientSpecNeedsRestore = false;
	char pClientSpec[MAX_PATH];
	if ( p4.GetClientSpecForPath( pPathID, pClientSpec, sizeof(pClientSpec) ) )
	{
		Q_strncpy( m_pOldClientSpec, p4.GetClientApi().GetClient().Text(), sizeof(m_pOldClientSpec) );
		p4.SetActiveClient( pClientSpec );
		m_bClientSpecNeedsRestore = true;
	}
}

CScopedPathClientSpec::~CScopedPathClientSpec()
{
	if ( m_bClientSpecNeedsRestore )
	{
		p4.SetActiveClient( m_pOldClientSpec );
	}
}


//-----------------------------------------------------------------------------
// Purpose: utility function to split a typical P4 line output into var and value
//-----------------------------------------------------------------------------
static void SplitP4Output(const_char *data, char *pszCmd, char *pszInfo, int bufLen)
{
	Q_strncpy(pszCmd, data, bufLen);
	
	char *mid = Q_strstr(pszCmd, " ");
	if (mid)
	{
		*mid = 0;
		Q_strncpy(pszInfo, data + (mid - pszCmd) + 1, bufLen);
	}
	else
	{
		pszInfo[0] = 0;
	}
}


//-----------------------------------------------------------------------------
// Purpose: base class for parse input from the P4 server
//-----------------------------------------------------------------------------
template< class T >
class CDataRetrievalUser : public ClientUser
{
public:
	CUtlVector<T> &GetData()
	{
		return m_Data;
	}

	// call this to start retrieving data
	void InitRetrievingData()
	{
		m_bAwaitingNewRecord = true;
		m_pData = &m_Data;
		m_pData->RemoveAll();
	}

	// call this to start retrieving data  into an external piece of memory
	void InitRetrievingData( CUtlVector<T> *pData )
	{
		m_bAwaitingNewRecord = true;
		m_pData = pData;
		m_pData->RemoveAll();
	}

	// implement this to parse out input from the server into the specified object
	virtual void OutputRecord(T &obj, const char *pszVar, const char *pszInfo) = 0;


private:
	bool m_bAwaitingNewRecord;
	CUtlVector<T> m_Data;
	CUtlVector<T> *m_pData;

	virtual void OutputInfo(char level, const_char *data)
	{
		if ( Q_strlen(data) < 1 )
		{
			// end of a record, await the new one
			m_bAwaitingNewRecord = true;
			return;
		}

		if ( m_bAwaitingNewRecord )
		{
			// add in the new record
			T &record = m_pData->Element( m_pData->AddToTail() );
			memset( &record, 0, sizeof( record ) );
			Construct( &record );
			m_bAwaitingNewRecord = false;
		}

		// parse
		char szVar[_MAX_PATH];
		char szInfo[_MAX_PATH];
		SplitP4Output( data, szVar, szInfo, sizeof( szVar ) );

		// emit
		T &record = m_pData->Element( m_pData->Count() - 1 );
		OutputRecord( record, szVar, szInfo );
	}
};


//-----------------------------------------------------------------------------
// Purpose: Retrieves a file list
//-----------------------------------------------------------------------------
class CFileUser : public CDataRetrievalUser<P4File_t>
{
public:
	void RetrieveDir(const char *dir)
	{
		// clear the list
		InitRetrievingData();

		// search for the paths - this string gets all revisions of a file, 
		// from the version they have locally to the current version
		// this is so we can see if the local files are out of date
		char szSearch[MAX_PATH];
		Q_snprintf(szSearch, sizeof(szSearch), "%s/*", dir);

		// gets directories first
		char *argv[] = { "-C", (char *)szSearch, NULL };
		p4.GetClientApi().SetArgv( 2, argv );
		p4.GetClientApi().Run("dirs", this);

		// now get files, '-Rc' restricts files to client mapping
		char *argv2[] = { "-Rc", "-Op", (char *)szSearch, NULL };
		p4.GetClientApi().SetArgv( 3, argv2 );
		p4.GetClientApi().Run("fstat", this);

		ComputeLocalDirectoryNames( GetData() );
	}

	void RetrieveFile(const char *filespec)
	{
		// clear the list
		InitRetrievingData();

		// now get files, '-Rc' restricts files to client mapping
		char *argv2[] = { "-Rc", "-Op", (char *)filespec, NULL };
		p4.GetClientApi().SetArgv( 3, argv2 );
		p4.GetClientApi().Run("fstat", this);
		ComputeLocalDirectoryNames( GetData() );
	}
	// Show how file names map through the client view
	// For each file in filespec, three names are produced:
	//   depotFile      the name in the depot
	//   clientFile     the name on the client in Perforce syntax
	//   localFile      the name on the client in local syntax
	void RetrieveWhereabouts(const char *filespec)
	{
		// clear the list
		InitRetrievingData();

		// figure out where filespec is
		char *argv[] = { (char *)filespec, NULL };
		p4.GetClientApi().SetArgv( 1, argv );
		p4.GetClientApi().Run("where", this);
	}

	void RetrieveOpenedFiles( const char *filespec )
	{
		// clear the list
		InitRetrievingData();
		char *argv[] = { (char *)filespec, NULL };
		p4.GetClientApi().SetArgv( 1, argv );
		p4.GetClientApi().Run("opened", this);
		ComputeLocalFileNames( GetData() );
	}

	void RetrieveOpenedFiles( CUtlVector<P4File_t> &fileList )
	{
		// clear the list
		InitRetrievingData( &fileList );
		p4.GetClientApi().Run( "opened", this );
		ComputeLocalFileNames( fileList );
	}

private:
	void ComputeLocalFileNames( CUtlVector<P4File_t> &fileList )
	{
		// Need to construct valid local paths since opened doesn't do it for us
		char pMatchPattern[MAX_PATH];
		Q_snprintf( pMatchPattern, sizeof(pMatchPattern), "//%s/", p4.GetClientApi().GetClient().Text() );
		int nLen = Q_strlen( pMatchPattern );

		int nCount = fileList.Count();
		for ( int i = 0; i < nCount; ++i )
		{
			if ( fileList[i].m_bDir )
				continue;

			const char *pClientPath = p4.String( fileList[i].m_sClientFile );
			if ( Q_stristr( pClientPath, pMatchPattern ) != pClientPath )
				continue;

			char pLocalPath[MAX_PATH];
			Q_snprintf( pLocalPath, sizeof(pLocalPath), "%s\\%s", p4.GetLocalRoot(), pClientPath + nLen );  
			Q_FixSlashes( pLocalPath );
			fileList[i].m_sLocalFile = pLocalPath;
		}
	}

	void ComputeLocalDirectoryNames( CUtlVector<P4File_t> &fileList )
	{
		// Need to construct valid local paths since opened doesn't do it for us
		int nCount = fileList.Count();
		for ( int i = 0; i < nCount; ++i )
		{
			if ( !fileList[i].m_bDir )
				continue;

			char pLocalPath[MAX_PATH];
			const char *pDepotPath = p4.String( fileList[i].m_sDepotFile );
			if ( !p4.DepotFileToLocalFile( pDepotPath, pLocalPath, sizeof(pLocalPath) ) )
				continue;

			fileList[i].m_sLocalFile = pLocalPath;
		}
	}

	virtual void OutputRecord(P4File_t &file, const char *szCmd, const char *szInfo)
	{
		if (!Q_strcmp(szCmd, "headRev"))
		{
			file.m_iHeadRevision = atoi(szInfo);
		}
		else if (!Q_strcmp(szCmd, "haveRev"))
		{
			file.m_iHaveRevision = atoi(szInfo);
		}
		else if (!Q_strcmp(szCmd, "headAction") && !Q_strcmp(szInfo, "delete"))
		{
			file.m_bDeleted = true;
		}
		else if (!Q_strcmp(szCmd, "action"))
		{
			if (!Q_strcmp(szInfo, "edit"))
			{
				file.m_eOpenState = P4FILE_OPENED_FOR_EDIT;
			}
			else if (!Q_strcmp(szInfo, "delete"))
			{
				file.m_eOpenState = P4FILE_OPENED_FOR_DELETE;
			}
			else if (!Q_strcmp(szInfo, "add"))
			{
				file.m_eOpenState = P4FILE_OPENED_FOR_ADD;
			}
			else if (!Q_strcmp(szInfo, "integrate"))
			{
				file.m_eOpenState = P4FILE_OPENED_FOR_INTEGRATE;
			}
		}
		else if (!Q_strcmp(szCmd, "change"))
		{
			file.m_iChangelist = atoi(szInfo);
		}
		else
		{
			// extract the path
			char pFilePath[_MAX_PATH];
			Q_strncpy( pFilePath, szInfo, sizeof( pFilePath ) );
			char *pFileName = Q_strrchr(pFilePath, '/');
			if (pFileName)
			{
				++pFileName;
			}

			if ( !Q_stricmp( szCmd, "dir" ) )
			{
				// new directory, add to the list
				file.m_sPath = pFilePath;
				file.m_sDepotFile = pFilePath;
				file.m_sName = pFileName;
				file.m_bDir = true;
			}
			else if ( !Q_strcmp( szCmd, "depotFile" ) )
			{
				char *pCruft = Q_strstr( pFilePath, "//%%1" );
				if (pCruft)
					*pCruft = 0;
				file.m_sDepotFile = pFilePath;

				// Now that we've stored off the depot file, split file into path + name
				if (pFileName)
				{
					*(pFileName - 1) = 0;
				}

				file.m_sPath = pFilePath;
				file.m_sName = pFileName;
				file.m_bDir = false;
			}
			else if (!Q_stricmp(szCmd, "clientFile"))
			{
				char *pCruft = Q_strstr( pFilePath, "//%%1" );
				if (pCruft)
					*pCruft = 0;
				file.m_sClientFile = pFilePath;
			}
			else if (!Q_stricmp(szCmd, "path"))
			{
				char *pCruft = Q_strstr(pFilePath, "\\%%1");
				if (pCruft)
					*pCruft = 0;
				file.m_sLocalFile = pFilePath;
			}
		}
	}
};
CFileUser g_FileUser;
CFileUser g_WhereUser;


//-----------------------------------------------------------------------------
// Purpose: Retrieves a history list
//-----------------------------------------------------------------------------
class CRevisionHistoryUser : public CDataRetrievalUser<P4Revision_t>
{
public:
	void RetrieveHistory(const char *path, bool bDir)
	{
		InitRetrievingData();

		// search for the paths - this string gets all revisions of a file, 
		// from the version they have locally to the current version
		// this is so we can see if the local files are out of date
		char szSearch[MAX_PATH];
		if (bDir)
		{
			Q_snprintf(szSearch, sizeof(szSearch), "%s/...", path);
		}
		else
		{
			Q_snprintf(szSearch, sizeof(szSearch), "%s", path);
		}

		// set to view long output, to show the time, and to have a maximum number of results
		char *argv[] = { "-l", "-t", "-m", "50", (char *)szSearch, NULL };
		p4.GetClientApi().SetArgv( 5, argv );
		p4.GetClientApi().Run("changes", this);

	}

private:
	virtual void OutputRecord(P4Revision_t &revision, const char *szCmd, const char *szInfo)
	{
		if (!Q_strcmp(szCmd, "change"))
		{
			revision.m_iChange = atoi(szInfo);
		}
		else if (!Q_strcmp(szCmd, "user"))
		{
			revision.m_sUser = szInfo;
		}
		else if (!Q_strcmp(szCmd, "client"))
		{
			revision.m_sClient = szInfo;
		}
		else if (!Q_strcmp(szCmd, "status"))
		{
		}
		else if (!Q_strcmp(szCmd, "desc"))
		{
			revision.m_Description = szInfo;
		}
		else if (!Q_strcmp(szCmd, "time"))
		{
			long iTime = atoi(szInfo);

			struct tm *gmt = gmtime(reinterpret_cast<time_t*>(&iTime));
	
			revision.m_nYear = gmt->tm_year + 1900; 
			revision.m_nMonth = gmt->tm_mon + 1;
			revision.m_nDay = gmt->tm_mday;
			revision.m_nHour = gmt->tm_hour;
			revision.m_nMinute = gmt->tm_min; 
			revision.m_nSecond = gmt->tm_sec;
		}
	}
};
CRevisionHistoryUser g_RevisionHistoryUser;


//-----------------------------------------------------------------------------
// Purpose: Retreives list of clients
//-----------------------------------------------------------------------------
class CClientSpecUser : public CDataRetrievalUser<P4Client_t>
{
public:
	void RetrieveClients()
	{
		InitRetrievingData();

		// we just get the entire list
		p4.GetClientApi().Run("clients", this);
	}

private:
	virtual void OutputRecord(P4Client_t &client, const char *szCmd, const char *szInfo)
	{
		if (!Q_strcmp(szCmd, "client"))
		{
			client.m_sName = szInfo;
		}
		else if (!Q_strcmp(szCmd, "Owner"))
		{
			client.m_sUser = szInfo;
		}
		else if (!Q_strcmp(szCmd, "Root"))
		{
			client.m_sLocalRoot = szInfo;
		}
		else if (!Q_strcmp(szCmd, "Host"))
		{
			client.m_sHost = szInfo;
		}
		else
		{
			Msg("Unknown field %s = %s\n", szCmd, szInfo);
		}
	}
};
CClientSpecUser g_ClientspecUser;


//-----------------------------------------------------------------------------
// Purpose: holder and modifier of clientspec file mapings
//-----------------------------------------------------------------------------
class CClientspecMap
{
public:
	char m_szFullClientspec[CLIENTSPEC_BUFFER_SIZE];
	CUtlVector<CClientPathRecord> m_PathMap;
	char m_szLocalPath[_MAX_PATH];

	void ReadClientspec(const char *clientspec)
	{
		// Get the local path
		char *pszSearch = "\n\nRoot:";
		char *pStr = Q_strstr( clientspec, pszSearch );
		if (pStr)
		{
			pStr += Q_strlen( pszSearch );

			// parse out next satring
			CUtlBuffer parse(pStr, Q_strlen(pStr), CUtlBuffer::TEXT_BUFFER | CUtlBuffer::READ_ONLY );

			// get the local path
			parse.GetString( m_szLocalPath, sizeof(m_szLocalPath) );
		}

		pszSearch = "\n\nView:\n";
		pStr = Q_strstr(clientspec, pszSearch);
		if (pStr)
		{
			pStr += Q_strlen(pszSearch);

			// take a full copy of the string
			Q_strncpy(m_szFullClientspec, clientspec, sizeof(m_szFullClientspec));

			// parse out strings
			CUtlBuffer parse(pStr, Q_strlen(pStr), CUtlBuffer::TEXT_BUFFER|CUtlBuffer::READ_ONLY);

			while (parse.IsValid())
			{
				CClientPathRecord record;
				record.m_bNegative = false;

				// get the depot path
				parse.GetString(record.m_szDepotPath, sizeof(record.m_szDepotPath));
				if (Q_strlen(record.m_szDepotPath) < 1)
					break;

				if (!Q_stricmp(record.m_szDepotPath, "-"))
				{
					// it's the negation sign, get the next line
					parse.GetString(record.m_szDepotPath, sizeof(record.m_szDepotPath));
					record.m_bNegative = true;
				}
				if (record.m_szDepotPath[0] == '-')
				{
					// negation sign is stuck on the end, remove
					record.m_bNegative = true;
					memmove(record.m_szDepotPath, record.m_szDepotPath + 1, sizeof(record.m_szDepotPath) - 1);
				}

				// get the next sign
				parse.GetString(record.m_szClientPath, sizeof(record.m_szClientPath));

				// append to list
				m_PathMap.AddToTail(record);
			}
		}
	}

	void WriteClientspec(char *pszDest, int destSize)
	{
		// assemble the new view
		char szView[CLIENTSPEC_BUFFER_SIZE] = { 0 };
		for (int i = 0; i < m_PathMap.Count(); i++)
		{
			CClientPathRecord &record = m_PathMap[i];

			Q_strncat(szView, "     ", sizeof(szView), COPY_ALL_CHARACTERS);
			if (record.m_bNegative)
			{
				Q_strncat(szView, "-", sizeof(szView), COPY_ALL_CHARACTERS);
			}
			Q_strncat(szView, record.m_szDepotPath, sizeof(szView), COPY_ALL_CHARACTERS);
			Q_strncat(szView, " ", sizeof(szView), COPY_ALL_CHARACTERS);
			Q_strncat(szView, record.m_szClientPath, sizeof(szView), COPY_ALL_CHARACTERS);
			Q_strncat(szView, " \n", sizeof(szView), COPY_ALL_CHARACTERS);
		}

		// find the place the view is set
		char *pszSearch = "\n\nView:\n";
		char *pStr = Q_strstr(m_szFullClientspec, pszSearch);
		if (pStr)
		{
			char *pDest = (pStr + strlen(pszSearch));

			// replace the view with the new view
			Q_strncpy(pDest, szView, sizeof(m_szFullClientspec) - (pDest - m_szFullClientspec));
		}

		// emit
		Q_strncpy(pszDest, m_szFullClientspec, destSize);
	}

	void RemovePathFromClient(const char *path)
	{
		// work out how the path record contains the specified path
		for (int i = 0; i < m_PathMap.Count(); i++)
		{
			// look for the first path that matches, comparing the two strings up to before the "/..." in szDepotPath
			if (!Q_strnicmp(m_PathMap[i].m_szDepotPath, path, Q_strlen(m_PathMap[i].m_szDepotPath) - 4))
			{
				CClientPathRecord &baseRecord = m_PathMap[i];

				// we have a match, build the negative case
				CClientPathRecord negativeRecord;
				negativeRecord.m_bNegative = true;
				Q_snprintf(negativeRecord.m_szDepotPath, sizeof(negativeRecord.m_szDepotPath), "%s/...", path);

				// build the clientspec side of the mapping

				// aa/b/... //client/BAH/...
				//- aa/b/d/... //client/BAH/d/

				// find the common strings in both
				int iPos = 0;
				while (baseRecord.m_szDepotPath[iPos] == negativeRecord.m_szDepotPath[iPos])
					++iPos;

				char *pszNegPaths = negativeRecord.m_szDepotPath + iPos;

				// append the neg paths to the existing baseRecord.szClientPath
				Q_strncpy(negativeRecord.m_szClientPath, baseRecord.m_szClientPath, sizeof(negativeRecord.m_szClientPath));
				// strip off the last slash
				char *pszDir = Q_strstr(negativeRecord.m_szClientPath, "/...");
				if (pszDir)
				{
					*pszDir = 0;
				}

				// append the new client paths to negate
				Q_strncat(negativeRecord.m_szClientPath, "/", sizeof(negativeRecord.m_szClientPath), COPY_ALL_CHARACTERS);
				Q_strncat(negativeRecord.m_szClientPath, pszNegPaths, sizeof(negativeRecord.m_szClientPath), COPY_ALL_CHARACTERS);

				m_PathMap.InsertAfter(i, negativeRecord);
				break;
			}
		}
	}

	// calculates the depot root
	void GetCommonDepotRoot(char *pszDest, int destSize)
	{
		int nCount = m_PathMap.Count(); 
		if ( nCount == 0 )
		{
			pszDest[0] = 0;
			return;
		}

		// walk each mapping, and find the longest common starting substring
		Q_strncpy(pszDest, m_PathMap[0].m_szDepotPath, destSize);
		for (int i = 1; i < nCount; i++)
		{
			if ( m_PathMap[i].m_bNegative )
				continue;

			// see how much we compare
			for (char *pszD = pszDest, *pszS = m_PathMap[i].m_szDepotPath; *pszD && *pszS; ++pszD, ++pszS)
			{
				if (*pszD != *pszS)
				{
					*pszD = 0;
					break;
				}
			}
		}

		// remove any "..." at the end
		char *pszD = Q_strstr(pszDest, "...");
		if (pszD)
		{
			*pszD = 0;
		}
	}
};


//-----------------------------------------------------------------------------
// Purpose: tool for editing clientspecs
//-----------------------------------------------------------------------------
class CClientspecEditUser : public ClientUser
{
public:
	void RetrieveClient(const char *pszClient)
	{
		// we just get the entire list
		char *argv[] = { "-o", (char *)pszClient, NULL };
		p4.GetClientApi().SetArgv( 2, argv );
		p4.GetClientApi().Run("client", this);
	}

	// saves out the new path spec to the 
	void WriteClientspec()
	{
		// write out the new clientspec
		char *argv[] = { "-i", NULL, NULL };
		p4.GetClientApi().SetArgv( 1, argv );
		p4.GetClientApi().Run("client", this);
	}

	CClientspecMap m_Map;

private:
	virtual void OutputInfo( char level, const_char *data )
	{
		char szVar[CLIENTSPEC_BUFFER_SIZE], szInfo[CLIENTSPEC_BUFFER_SIZE];
		SplitP4Output(data, szVar, szInfo, sizeof(szInfo));

		if (!Q_stricmp(szVar, "data"))
		{
			m_Map.ReadClientspec(szInfo);
		}
	}

	// called to read data
	virtual void InputData( StrBuf *strbuf, Error *e )
	{
		char szView[CLIENTSPEC_BUFFER_SIZE];
		m_Map.WriteClientspec(szView, sizeof(szView));
		strbuf->Set(szView);
		e->Clear();
	}

	virtual void OutputError( const_char *errBuf )
	{
		Msg("p4 error: %s", errBuf);
	}
};


//-----------------------------------------------------------------------------
// Purpose: Retreives list of clients
//-----------------------------------------------------------------------------
class CInfoUser : public ClientUser
{
public:
	void RetrieveInfo()
	{
		memset(&m_Client, 0, sizeof(m_Client));

		// we just get the entire list
		p4.GetClientApi().Run("info", this);
	}

	P4Client_t m_Client;

private:
	virtual void OutputInfo(char level, const_char *data)
	{
		char szCmd[_MAX_PATH];
		char szInfo[_MAX_PATH];

		SplitP4Output(data, szCmd, szInfo, sizeof(szCmd));

		if (!Q_stricmp(szCmd, "userName"))
		{
			m_Client.m_sUser = szInfo;
		}
		else if (!Q_stricmp(szCmd, "clientName"))
		{
			m_Client.m_sName = szInfo;
		}
		else if (!Q_stricmp(szCmd, "clientHost"))
		{
			m_Client.m_sHost = szInfo;
		}
		else if (!Q_stricmp(szCmd, "clientRoot"))
		{
			m_Client.m_sLocalRoot = szInfo;
		}
	}
};
CInfoUser g_InfoUser;


// Changelist description structure
struct ChangelistDesc_t
{
	int id;
	time_t m_tTimeStamp;
	CUtlSymbol m_sUser;
	CUtlSymbol m_sClient;
	CUtlSymbol m_sStatus;
	CUtlSymbol m_sDescription;
};


//-----------------------------------------------------------------------------
// Purpose: tool for creating new changelists
//-----------------------------------------------------------------------------
class CChangelistCreateUser : public CDataRetrievalUser<int>
{
public:
	void CreateChangelist( const char *pDescription )
	{
		InitRetrievingData();

		// Will force InputData to be called
		m_pDescription = ( pDescription && pDescription[0] ) ? pDescription : "I'm a loser who didn't type a description. Mock me at your earliest convenience.";
		char *argv[] = { "-i", NULL };
		p4.GetClientApi().SetArgv( 1, argv );
		p4.GetClientApi().Run("change", this);
	}

private:
	// called to read data from stdin
	virtual void InputData( StrBuf *strbuf, Error *e )
	{
		char svChangelist[1024];
		P4Client_t &activeClient = p4.GetActiveClient();

		Q_snprintf( svChangelist, sizeof(svChangelist),
			"Change:\tnew\n\nClient:\t%s\n\nUser:\t%s\n\nStatus:\tnew\n\n"
			"Description:\n\t%s\n\nFiles:\n\n",
			activeClient.m_sName.String(), activeClient.m_sUser.String(), m_pDescription );

		strbuf->Set(svChangelist);
		e->Clear();
	}

	virtual void OutputRecord( int &nChangelist, const char *szCmd, const char *szInfo)
	{
		if (!Q_strcmp(szCmd, "Change"))
		{
			nChangelist = atoi(szInfo);
		}
	}

	const char *m_pDescription;
};
CChangelistCreateUser g_ChangelistCreateUser;


//-----------------------------------------------------------------------------
// Purpose: tool for finding an existing changelist
//-----------------------------------------------------------------------------
class CChangelistFindUser : public CDataRetrievalUser<ChangelistDesc_t>
{
public:
	void ListChangelists( void )
	{
		InitRetrievingData();

		char *argv[] = { "-l", "-t", "-s", "pending", "-c", p4.GetClientApi().GetClient().Text(), NULL };
		p4.GetClientApi().SetArgv( 6, argv );
		p4.GetClientApi().Run("changes", this);
	}

private:
	virtual void OutputRecord( ChangelistDesc_t &cl, const char *szCmd, const char *szInfo)
	{
		if ( !Q_strcmp( szCmd, "change" ) )
		{
			cl.id = atoi( szInfo );
		}
		else if ( !Q_strcmp( szCmd, "time" ) )
		{
			cl.m_tTimeStamp = atoi( szInfo );
		}
		else if ( !Q_strcmp( szCmd, "user" ) )
		{
			cl.m_sUser = szInfo;
		}
		else if ( !Q_strcmp( szCmd, "client" ) )
		{
			cl.m_sClient = szInfo;
		}
		else if ( !Q_strcmp( szCmd, "status" ) )
		{
			cl.m_sStatus = szInfo;
		}
		else if ( !Q_strcmp( szCmd, "desc" ) )
		{
			CUtlVector<char> arrInfo;
			arrInfo.SetCount( 1 + strlen( szInfo ) );
			memcpy( arrInfo.Base(), szInfo, arrInfo.Count() );
			for ( int nCount = arrInfo.Count() - 1; nCount -- > 0; )
			{
				if ( isspace( arrInfo[ nCount ] ) )
					arrInfo[ nCount ] = 0;
				else
					break;
			}

			cl.m_sDescription = arrInfo.Base();
		}
	}
};
CChangelistFindUser g_ChangelistFindUser;


//-----------------------------------------------------------------------------
// Purpose: only used to handle the error callback
//-----------------------------------------------------------------------------
class CErrorHandlerUser : public ClientUser
{
public:
	CErrorHandlerUser() : m_errorSeverity( E_EMPTY ), m_uiFlags( 0 ) {}

	virtual void OutputError( const_char *errBuf )
	{
		m_errorSeverity = max( m_errorSeverity, E_WARN ); // this is a guess - it could have been E_FATAL or E_FAILED

		Msg("p4 error: %s", errBuf);

		m_errorBuf.Append( errBuf );
	}
	virtual void HandleError( Error *err )
	{
		m_errorSeverity = max( m_errorSeverity, ( ErrorSeverity )err->GetSeverity() );

		StrBuf buf;
		err->Fmt( buf, EF_NEWLINE );

		if ( V_strstr( buf.Text(), "can't edit exclusive file already opened" ) )
		{
			m_errorSeverity = max( m_errorSeverity, E_WARN ); // p4 sends this is an info message, even though the file doesn't get edited!
		}

		if ( ( m_uiFlags & eDisableFiltering ) || ShallOutputErrorStringBuffer( buf ) )
			Msg( "%s: %s", err->FmtSeverity(), buf.Text() );

		m_errorBuf.Append( &buf );
	}

	// Message is the only method that gets called on a p4 error
	// even though the p4 documentation claims that HandleError (and therefore OutputError)
	// should get called via the defalt Message implementation
	virtual void Message( Error *err )
	{
		HandleError( err );
	}

	void ResetErrorState()
	{
		m_errorSeverity = E_EMPTY;
		m_errorBuf.Clear();
	}
	ErrorSeverity GetErrorState()
	{
		return m_errorSeverity;
	}
	const char *GetErrorString()
	{
		return m_errorBuf.Text();
	}
	void SetErrorString( const char *errStr )
	{
		m_errorBuf.Set( errStr );
	}

	enum Flags {
		eDisableFiltering				=	1 << 0,		// Disable filtering output
		eFilterClUselessSpew			=	1 << 1,		// Filter "already opened", "add of exisiting", "can't change" msgs
	};

	// Sets the new flag mask, add prevails over remove if the flag specified in both.
	// SetFlags( x, 0 ) - enables flag x
	// SetFlags( 0, x ) - removes flag x
	// SetFlags( mask, ~0 ) - completely sets the flag mask
	uint32 SetFlags( uint32 uiAdd, uint32 uiRemove ) { uint32 uiOld = m_uiFlags; m_uiFlags = ( ( m_uiFlags & ~uiRemove ) | uiAdd ); return uiOld; }

protected:
	// Performs filtering before the message is output to screen,
	// if "ShallOutputErrorStringBuffer" returns false, then the message
	// is not printed on screen, otherwise the modified strbug is printed.
	virtual bool ShallOutputErrorStringBuffer( StrBuf &buf );

private:
	ErrorSeverity m_errorSeverity;
	StrBuf m_errorBuf;
	uint32 m_uiFlags;
};
CErrorHandlerUser g_ErrorHandlerUser;

bool CErrorHandlerUser::ShallOutputErrorStringBuffer( StrBuf &buf )
{
	char const *pText = buf.Text();

	if ( m_uiFlags & eFilterClUselessSpew )
	{
		if ( V_strstr( pText, "already opened for edit" ) )
			return false;
		if ( V_strstr( pText, "already opened for add" ) )
			return false;
		if ( V_strstr( pText, "currently opened for edit" ) )
			return false;
		if ( V_strstr( pText, "currently opened for add" ) )
			return false;
		if ( V_strstr( pText, "add of existing file" ) )
			return false;
		if ( V_strstr( pText, "add existing file" ) )
			return false;
		if ( V_strstr( pText, "can't change from" ) )
			return false;
		if ( V_strstr( pText, "no such file" ) )
			return false;
		if ( V_strstr( pText, "not on client" ) )
			return false;
	}

	return true;
}


//-----------------------------------------------------------------------------
// Purpose: tool for creating new changelists
//-----------------------------------------------------------------------------
class CSubmitUser : public ClientUser
{
public:
	void Submit( int nChangeList )
	{
		// NOTE: -i doesn't appear to work with -c
		// We have to set up the description when creating the changelist in the first place
		m_pDescription = NULL;
		char pBuf[128];
		Q_snprintf( pBuf, sizeof(pBuf), "%d", nChangeList );
		char *argv[] = { "-c", pBuf, NULL };
		p4.GetClientApi().SetArgv( 2, argv );
		p4.GetClientApi().Run( "submit", &g_ErrorHandlerUser );
	}

private:
	// called to read data from stdin
	virtual void InputData( StrBuf *strbuf, Error *e )
	{
		Assert( m_pDescription );

		char svChangelist[1024];
		P4Client_t &activeClient = p4.GetActiveClient();

		Q_snprintf( svChangelist, sizeof(svChangelist),
			"Change:\tnew\n\nClient:\t%s\n\nUser:\t%s\n\nStatus:\tnew\n\n"
			"Description:\n\t%s\n\nFiles:\n\n",
			activeClient.m_sName.String(), activeClient.m_sUser.String(), m_pDescription );

		strbuf->Set(svChangelist);
		e->Clear();
	}

	const char *m_pDescription;
};
CSubmitUser g_SubmitUser;


int FindOrCreateChangelist( const char *pDescription )
{
	int iCreatedEmptyCl = 0, iFoundChangelist = 0;
	CUtlMap<int, ChangelistDesc_t const *> mapFoundChangelists( DefLessFunc( int ) );

	CUtlSymbol symDesc = pDescription;

find_changelists:
	mapFoundChangelists.RemoveAll();
	g_ChangelistFindUser.ListChangelists();
	for ( int k = 0; k < g_ChangelistFindUser.GetData().Count(); ++ k )
	{
		ChangelistDesc_t const &cl = g_ChangelistFindUser.GetData()[ k ];

		if ( symDesc == cl.m_sDescription )
			mapFoundChangelists.Insert( cl.id, &cl );
	}

	if ( mapFoundChangelists.Count() )
		iFoundChangelist = mapFoundChangelists.Key( mapFoundChangelists.FirstInorder() );
	else if ( iCreatedEmptyCl > 0 )
		return iCreatedEmptyCl;

	if ( iFoundChangelist > 0 )
	{
		// Check if we created a changelist that has to be deleted
		if ( iCreatedEmptyCl > 0 && iCreatedEmptyCl != iFoundChangelist )
		{
			// Delete changelist
			char chClNumber[50];
			sprintf( chClNumber, "%d", iCreatedEmptyCl );
			char *argv[] = { "-d", chClNumber, NULL };
			p4.GetClientApi().SetArgv( 2, argv );
			p4.GetClientApi().Run( "change", &g_ErrorHandlerUser );
		}

		return iFoundChangelist;
	}
	else
	{
		// We did not find a changelist, create a new one
		g_ChangelistCreateUser.CreateChangelist( pDescription );

		iCreatedEmptyCl = g_ChangelistCreateUser.GetData().Count() ? g_ChangelistCreateUser.GetData()[0] : 0;
		if ( !iCreatedEmptyCl )
			return 0;

		// Now we have a changelist created, check again to avoid multithreading bugs
		goto find_changelists;
	}
}


//-----------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------
CP4::~CP4()
{
	// Prevents a hang if Shutdown isn't called before the process exits
	if ( m_bConnectedToServer )
	{
		Shutdown();
	}
}


//-----------------------------------------------------------------------------
// Connect, disconnect
//-----------------------------------------------------------------------------
bool CP4::Connect( CreateInterfaceFn factory )
{
	g_pFileSystem = (IFileSystem*)factory( FILESYSTEM_INTERFACE_VERSION, NULL );
	return ( g_pFileSystem != NULL );
}

void CP4::Disconnect()
{
	g_pFileSystem = NULL;
}
	

//-----------------------------------------------------------------------------
// Purpose: Startup
//-----------------------------------------------------------------------------
InitReturnVal_t CP4::Init()
{
	// set the protocol return all data as key/value pairs
	m_Client.SetProtocol( "tag", "" );

	// connect to the p4 server
	Error e;
	m_Client.Init( &e );
	m_bConnectedToServer = e.Test() == 0;

	RefreshClientData();

	return INIT_OK;
}


//-----------------------------------------------------------------------------
// Purpose: Cleanup
//-----------------------------------------------------------------------------
void CP4::Shutdown()
{
	Error e;
	m_Client.Final(&e);
	m_bConnectedToServer = false;
}


//-----------------------------------------------------------------------------
// Purpose: refreshes client data
//-----------------------------------------------------------------------------
void CP4::RefreshClientData()
{
	if ( !m_bConnectedToServer )
		return;

	// retrieve our login info
	g_InfoUser.RetrieveInfo();
	m_ActiveClient = g_InfoUser.m_Client;

	// calculate our common depot root
	CClientspecEditUser user;
	user.RetrieveClient( GetActiveClient().m_sName.String() );

	user.m_Map.GetCommonDepotRoot(m_szDepotRoot, sizeof(m_szDepotRoot));
	m_iDepotRootLength = Q_strlen(m_szDepotRoot);
	m_ClientMapping = user.m_Map.m_PathMap;
	Q_strncpy( m_szLocalRoot, user.m_Map.m_szLocalPath, sizeof(m_szLocalRoot) );
	m_iLocalRootLength = Q_strlen(m_szLocalRoot);

	SetOpenFileChangeList( m_sChangeListName.String() );
}


//-----------------------------------------------------------------------------
// Convert a depot file to a local file user the current client mapping
//-----------------------------------------------------------------------------
bool CP4::DepotFileToLocalFile( const char *pDepotFile, char *pLocalPath, int nBufLen )
{
	// Need to construct valid local paths since opened doesn't do it for us
	char pClientPattern[MAX_PATH];
	Q_snprintf( pClientPattern, sizeof(pClientPattern), "//%s/", m_Client.GetClient().Text() );
	int nClientLen = Q_strlen( pClientPattern );

	int nMatchCount = 0;
	int nCount = m_ClientMapping.Count();
	for ( int i = 0; i < nCount; ++i )
	{
		const char *pDepotPath = m_ClientMapping[i].m_szDepotPath;
		int nLen = Q_strlen( pDepotPath );
		if ( nMatchCount > nLen )
			continue;

		bool bRecursive = !Q_stricmp( &pDepotPath[nLen-4], "/..." );
		bool bInDirectory = !Q_stricmp( &pDepotPath[nLen-2], "/*" );
		if ( !bRecursive && !bInDirectory )
			continue;

		// FIXME: Do this fixup when we create m_ClientMapping?
		char pMatchingString[MAX_PATH];
		Q_strncpy( pMatchingString, pDepotPath, bRecursive ? nLen-2 : nLen );
		if ( Q_stristr( pDepotFile, pMatchingString ) != pDepotFile )
			continue;

		// Skip subdirectories if it's in the directory
		if ( bInDirectory )
		{
			const char *pRelativePath = pDepotFile + nLen - 1;
			if ( strchr( pRelativePath, '\\' ) || strchr( pRelativePath, '/' ) )
				continue;
		}

		const char *pClientPath = p4.String( m_ClientMapping[i].m_szClientPath );
		int nPathLen = Q_strlen( pClientPath );

		bool bClientRecursive = !Q_stricmp( &pClientPath[nPathLen-4], "/..." );
		bool bClientInDirectory = !Q_stricmp( &pClientPath[nPathLen-2], "/*" );
		if ( !bClientRecursive && !bClientInDirectory )
			continue;

		char pTruncatedClientPath[MAX_PATH];
		Q_strncpy( pTruncatedClientPath, pClientPath, bClientRecursive ? nPathLen-2 : nPathLen );
 		if ( Q_stristr( pTruncatedClientPath, pClientPattern ) != pTruncatedClientPath )
			continue;												   

		if ( nClientLen < nPathLen - 3 )
		{
			Q_snprintf( pLocalPath, nBufLen, "%s\\%s%s", p4.GetLocalRoot(), pTruncatedClientPath + nClientLen, bRecursive ? pDepotFile + nLen - 3 : pDepotFile + nLen - 1 );  
		}
		else
		{
			Q_snprintf( pLocalPath, nBufLen, "%s\\%s", p4.GetLocalRoot(), bRecursive ? pDepotFile + nLen - 3 : pDepotFile + nLen - 1 );  
		}
		Q_FixSlashes( pLocalPath );
		nMatchCount = nLen;
	}

	return ( nMatchCount > 0 );
}


//-----------------------------------------------------------------------------
// Refreshes the current client from p4 settings
//-----------------------------------------------------------------------------
void CP4::RefreshActiveClient()
{
	if ( !IsConnectedToServer() )
		return;

	RefreshClientData();
}


//-----------------------------------------------------------------------------
// Query interface
//-----------------------------------------------------------------------------
void *CP4::QueryInterface( const char *pInterfaceName )
{
	return Sys_GetFactoryThis()( pInterfaceName, NULL );
}


//-----------------------------------------------------------------------------
// Gets a string for a symbol
//-----------------------------------------------------------------------------
const char *CP4::String( CUtlSymbol s ) const
{
	return s.String();
}

	
//-----------------------------------------------------------------------------
// Purpose: data accessor
//-----------------------------------------------------------------------------
const char *CP4::GetDepotRoot()
{
	if ( !IsConnectedToServer() )
		return NULL;

	return m_szDepotRoot;
}

//-----------------------------------------------------------------------------
// Purpose: data accessor
//-----------------------------------------------------------------------------
int CP4::GetDepotRootLength()
{
	if ( !IsConnectedToServer() )
		return -1;

	return m_iDepotRootLength;
}

const char *CP4::GetLocalRoot()
{
	if ( !IsConnectedToServer() )
		return NULL;

	return m_szLocalRoot;
}

int CP4::GetLocalRootLength()
{
	if ( !IsConnectedToServer() )
		return -1;

	return m_iLocalRootLength;
}

	
//-----------------------------------------------------------------------------
// Purpose: translates filespecs to depotFile paths
//-----------------------------------------------------------------------------
void CP4::GetDepotFilePath(char *depotFilePath, const char *filespec, int size)
{
	if ( !IsConnectedToServer() )
	{
		if ( size > 0 )
		{
			depotFilePath[ 0 ] = '\0';
		}
		return;
	}

	g_WhereUser.RetrieveWhereabouts(filespec);
	Q_strncpy(depotFilePath, g_WhereUser.GetData()[0].m_sDepotFile.String(), size);
}

//-----------------------------------------------------------------------------
// Purpose: translates filespecs to clientFile paths
//-----------------------------------------------------------------------------
void CP4::GetClientFilePath(char *clientFilePath, const char *filespec, int size)
{
	if ( !IsConnectedToServer() )
	{
		if ( size > 0 )
		{
			clientFilePath[ 0 ] = '\0';
		}
		return;
	}

	g_WhereUser.RetrieveWhereabouts(filespec);
	Q_strncpy(clientFilePath, g_WhereUser.GetData()[0].m_sClientFile.String(), size);
}

//-----------------------------------------------------------------------------
// Purpose: translates filespecs to localFile paths
//-----------------------------------------------------------------------------
void CP4::GetLocalFilePath(char *localFilePath, const char *filespec, int size)
{
	if ( !IsConnectedToServer() )
	{
		if ( size > 0 )
		{
			localFilePath[ 0 ] = '\0';
		}
		return;
	}

	g_WhereUser.RetrieveWhereabouts(filespec);
	Q_strncpy(localFilePath, g_WhereUser.GetData()[0].m_sLocalFile.String(), size);
}

//-----------------------------------------------------------------------------
// Purpose: returns a list of files
//-----------------------------------------------------------------------------
CUtlVector<P4File_t> &CP4::GetFileList( const char *pPath )
{
	if ( !IsConnectedToServer() )
	{
		static CUtlVector< P4File_t > dummy;
		return dummy;
	}

	CScopedDirClientSpec spec( pPath );
	g_FileUser.RetrieveDir( pPath );
	return g_FileUser.GetData();
}


//-----------------------------------------------------------------------------
// retreives the list of files in a path, using a known client spec
//-----------------------------------------------------------------------------
CUtlVector<P4File_t> &CP4::GetFileListUsingClientSpec( const char *pPath, const char *pClientSpec )
{
	if ( !IsConnectedToServer() )
	{
		static CUtlVector< P4File_t > dummy;
		return dummy;
	}

	CScopedClientSpec spec( pClientSpec );
	g_FileUser.RetrieveDir( pPath );
	return g_FileUser.GetData();
}


//-----------------------------------------------------------------------------
// Purpose: returns the list of files opened for edit/integrate/delete 
//-----------------------------------------------------------------------------
void CP4::GetOpenedFileList( CUtlVector<P4File_t> &fileList )
{
	if ( !IsConnectedToServer() )
	{
		fileList.RemoveAll();
		return;
	}

	g_FileUser.RetrieveOpenedFiles( fileList );
}

void CP4::GetOpenedFileList( const char *pRootDirectory, CUtlVector<P4File_t> &fileList )
{
	if ( !IsConnectedToServer() )
	{
		fileList.RemoveAll();
		return;
	}

	CScopedDirClientSpec spec( pRootDirectory );
	g_FileUser.RetrieveOpenedFiles( fileList );
}

void CP4::GetOpenedFileListInPath( const char *pPathID, CUtlVector<P4File_t> &fileList )
{
	if ( !IsConnectedToServer() )
	{
		fileList.RemoveAll();
		return;
	}

	CScopedPathClientSpec spec( pPathID );
	g_FileUser.RetrieveOpenedFiles( fileList );
}

	
//-----------------------------------------------------------------------------
// Purpose: file history
//-----------------------------------------------------------------------------
CUtlVector<P4Revision_t> &CP4::GetRevisionList(const char *path, bool bIsDir)
{
	if ( !IsConnectedToServer() )
	{
		static CUtlVector< P4Revision_t > dummy;
		return dummy;
	}

	g_RevisionHistoryUser.RetrieveHistory(path, bIsDir);
	return g_RevisionHistoryUser.GetData();
}

//-----------------------------------------------------------------------------
// Purpose: returns a list of clients
//-----------------------------------------------------------------------------
CUtlVector<P4Client_t> &CP4::GetClientList()
{
	if ( !IsConnectedToServer() )
	{
		static CUtlVector< P4Client_t > dummy;
		return dummy;
	}

	g_ClientspecUser.RetrieveClients();
	return g_ClientspecUser.GetData();
}

//-----------------------------------------------------------------------------
// Purpose: sets the active client
//-----------------------------------------------------------------------------
void CP4::SetActiveClient(const char *clientname)
{
	if ( !IsConnectedToServer() )
		return;

	m_Client.SetClient(clientname);
	RefreshClientData();
}

//-----------------------------------------------------------------------------
// Purpose: returns the name of the currently opened clientspec
//-----------------------------------------------------------------------------
P4Client_t &CP4::GetActiveClient()
{
	return m_ActiveClient;
}


//-----------------------------------------------------------------------------
// Purpose: cloaks a folder from the current view
//-----------------------------------------------------------------------------
void CP4::RemovePathFromActiveClientspec(const char *path)
{
	if ( !IsConnectedToServer() )
		return;

	// read in the clientspec
	CClientspecEditUser user;
	user.RetrieveClient(GetActiveClient().m_sName.String());

	// get the extra info
	user.m_Map.RemovePathFromClient(path);

	// refresh the list
	user.WriteClientspec();
}


//-----------------------------------------------------------------------------
// Finds a client spec for a directory. Is destructive to the passed-in directory
//-----------------------------------------------------------------------------
bool CP4::GetClientSpecForDirectory( const char *pFullPathDir, char *pClientSpec, int nMaxLen )
{
	if ( nMaxLen == 0 )
		return false;

	pClientSpec[ 0 ] = '\0';

	if ( !IsConnectedToServer() )
		return false;

	char pCurPath[ MAX_PATH ];
	Q_strncpy( pCurPath, pFullPathDir, sizeof(pCurPath) );
	Q_StripTrailingSlash( pCurPath );

	do
	{
		char pP4ConfigPath[MAX_PATH];
		Q_strncpy( pP4ConfigPath, pCurPath, MAX_PATH );
		Q_strncat( pP4ConfigPath, "\\p4config", MAX_PATH, MAX_PATH );
		if ( g_pFileSystem->FileExists( pP4ConfigPath ) )
		{
			char temp[1024];
			CUtlBuffer buf( temp, sizeof(temp), CUtlBuffer::TEXT_BUFFER | CUtlBuffer::EXTERNAL_GROWABLE );
			if ( g_pFileSystem->ReadFile( pP4ConfigPath, NULL, buf ) )
			{
				if ( !buf.GetToken( "=" ) )
				{
					Warning( "Unable to parse file %s!\n", pP4ConfigPath );
				}
				else
				{
					int i = 0;
					while ( buf.IsValid() )
					{
						char c = buf.GetChar();
						if ( isspace(c) )
							break;

						pClientSpec[i++] = c;
						if ( i >= nMaxLen - 1 )
							break;
					}
					pClientSpec[i] = 0;
					return true;
				}
			}
			else
			{
				Warning( "Unable to read file %s!\n", pP4ConfigPath );
			}
		}

		Q_StripLastDir( pCurPath, sizeof(pCurPath) );
		Q_StripTrailingSlash( pCurPath );
		if ( Q_strlen( pCurPath ) <= 2 )
			break;

	} while (true);

	return false;
}


//-----------------------------------------------------------------------------
// Returns which clientspec a file lies under
//-----------------------------------------------------------------------------
bool CP4::GetClientSpecForFile( const char *pFullPath, char *pClientSpec, int nMaxLen )
{
	// Strip off the file name
	char pCurPath[MAX_PATH];
	Q_strncpy( pCurPath, pFullPath, sizeof(pCurPath) );
	Q_StripFilename( pCurPath );

	// Recursively search subdirectories until we find a p4config file.
	return GetClientSpecForDirectory( pCurPath, pClientSpec, nMaxLen );
}


//-----------------------------------------------------------------------------
// Returns which clientspec a filesystem path ID lies under
//-----------------------------------------------------------------------------
bool CP4::GetClientSpecForPath( const char *pPathId, char *pClientSpec, int nMaxLen )
{
	if ( !IsConnectedToServer() )
	{
		if ( nMaxLen > 0 )
		{
			pClientSpec[ 0 ] = '\0';
		}
		return false;
	}

	char pPathBuf[2048];
	if ( g_pFileSystem->GetSearchPath( pPathId, false, pPathBuf, sizeof(pPathBuf) ) == 0 )
		return false;

	// FIXME: Would be faster to not check for duplication and just
	// pick the first client spec it sees. Should I not test the additional paths?
	bool bFirstPath = true;
	bool bFoundClientSpec = false;
	char pTempClientSpec[MAX_PATH];
	char *pPath = pPathBuf;
	while ( pPath )
	{
		// Find the next path
		char *pCurPath = pPath;
		char *pSemi = strchr( pPath, ';' );
		if ( pSemi )
		{
			*pSemi = 0;
			pPath = pSemi+1;
		}
		else
		{
			pPath = NULL;
		}

		// Recursively search subdirectories until we find a p4config file.
		if ( GetClientSpecForDirectory( pCurPath, pClientSpec, nMaxLen ) )
		{
			bFoundClientSpec = true;
			if ( bFirstPath )
			{
				Q_strncpy( pTempClientSpec, pClientSpec, sizeof(pTempClientSpec) );
			}
			else
			{
				// Paths are on different client specs
				if ( Q_stricmp( pClientSpec, pTempClientSpec ) )
				{
					pClientSpec[0] = 0;
					return false;
				}
			}
		}
	}
	return bFoundClientSpec;
}

void CP4::SetOpenFileChangeList( const char *pChangeListName )
{
	if ( !m_sChangeListName.Length() ||
		 !pChangeListName || !*pChangeListName ||
		 strcmp( m_sChangeListName.String(), pChangeListName ) ||
		 pChangeListName == m_sChangeListName.String() )
	{
		m_sChangeListName.Set( pChangeListName );
		m_sCachedChangeListNum.Set( "0" );
		m_nCachedChangeListNumber = 0;

		// Valid changelist
		{
			uint32 uiSpewFlag = CErrorHandlerUser::eFilterClUselessSpew;
			int bAdd = m_sChangeListName.Length();
			g_ErrorHandlerUser.SetFlags( bAdd ? uiSpewFlag : 0, bAdd ? 0 : uiSpewFlag );
		}
	}
}

char const * CP4::GetOpenFileChangeListNum()
{
	if ( m_sChangeListName.Length() )
	{
		if ( !m_nCachedChangeListNumber )
		{
			m_nCachedChangeListNumber = FindOrCreateChangelist( m_sChangeListName.String() );
			if ( m_nCachedChangeListNumber )
				m_sCachedChangeListNum.Format( "%d", m_nCachedChangeListNumber );
		}

		if ( m_nCachedChangeListNumber )
			return m_sCachedChangeListNum.String();
	}
	return NULL;
}

static bool MakeFilesWritable( int nCount, const char **ppFullPathList )
{
	bool bResult = true;

	// Make sure we can make as many files writable as possible
	for ( int k = 0; k < nCount; ++ k )
	{
		char const *szFile = ppFullPathList[ k ];
		if ( !access( szFile, 02 ) )
			continue;
		if ( !chmod( szFile, _S_IWRITE | _S_IREAD ) )
			continue;
		bResult = false;
	}

	return bResult;
}

	
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CP4::OpenFileForAdd( const char *fullpath )
{
	return PerformPerforceOpCurChangeList( "add", fullpath );
}

	
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CP4::OpenFileForEdit( const char *fullpath )
{
	if ( !PerformPerforceOpCurChangeList( "edit", fullpath ) )
		return false;

	return MakeFilesWritable( 1, &fullpath );
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CP4::OpenFileForDelete( const char *pFullPath )
{
	return PerformPerforceOpCurChangeList( "delete", pFullPath );
}


//-----------------------------------------------------------------------------
// Submit file
//-----------------------------------------------------------------------------
bool CP4::SubmitFile( const char *pFullPath, const char *pDescription )
{
	return SubmitFiles( 1, &pFullPath, pDescription );
}


//-----------------------------------------------------------------------------
// Revert file
//-----------------------------------------------------------------------------
bool CP4::RevertFile( const char *pFullPath )
{
	return PerformPerforceOp( "revert", pFullPath );
}


//-----------------------------------------------------------------------------
// Helper for operations on multiple files
//-----------------------------------------------------------------------------
bool CP4::PerformPerforceOp( PerforceOp_t op, int nCount, const char **ppFullPathList, const char *pDescription )
{
	g_ErrorHandlerUser.ResetErrorState();

	if ( !IsConnectedToServer() )
	{
		g_ErrorHandlerUser.SetErrorString( "Not connected to P4 server\n" );
		return false;
	}

	if ( nCount == 0 )
		return true;

	char pOldClientSpec[MAX_PATH];
	Q_strncpy( pOldClientSpec, m_Client.GetClient().Text(), sizeof(pOldClientSpec) );

	char pCurrentClientSpec[MAX_PATH];
 	Q_strncpy( pCurrentClientSpec, pOldClientSpec, sizeof(pCurrentClientSpec) );

	int nFirstIndex = 0;
	while ( nFirstIndex < nCount )
	{
		char pClientSpec[MAX_PATH];

		bool bChangeSpec = false;
		int i;
		for ( i = nFirstIndex; i < nCount; ++i )
		{
			if ( p4.GetClientSpecForFile( ppFullPathList[i], pClientSpec, sizeof(pClientSpec) ) )
			{
				if ( Q_stricmp( pCurrentClientSpec, pClientSpec ) )
				{
					bChangeSpec = true;
					break;
				}
			}
		}

		if ( i != nFirstIndex )
		{
			op( i - nFirstIndex, &ppFullPathList[nFirstIndex], pDescription );
			nFirstIndex = i;
		}

		if ( bChangeSpec )
		{
			p4.SetActiveClient( pClientSpec );
 			Q_strncpy( pCurrentClientSpec, pClientSpec, sizeof(pCurrentClientSpec) );
		}
	}

	if ( Q_stricmp( pCurrentClientSpec, pOldClientSpec ) )
	{
		p4.SetActiveClient( pOldClientSpec );
	}

	return g_ErrorHandlerUser.GetErrorState() < E_WARN;
}

static const char *s_pOperation;
void SimplePerforceOp( int nCount, const char **ppFullPathList, const char *pDescription )
{
	p4.GetClientApi().SetArgv( nCount, const_cast<char**>( ppFullPathList ) );
	p4.GetClientApi().Run( s_pOperation, &g_ErrorHandlerUser );
}

bool CP4::PerformPerforceOp( const char *pOperation, int nCount, const char **ppFullPathList )
{
	s_pOperation = pOperation;
	return PerformPerforceOp( SimplePerforceOp, nCount, ppFullPathList, NULL );
}

void SimplePerforceOpCurChangeList( int nCount, const char **ppFullPathList, const char *pDescription )
{
	if ( char const *szClNum = p4.GetOpenFileChangeListNum() )
	{
		CUtlVector< const char * > arrArgv;
		arrArgv.SetCount( nCount + 3 );
		arrArgv[0] = "-c";
		arrArgv[1] = szClNum;
		for ( int k = 0; k < nCount; ++ k )
			arrArgv[ 2 + k ] = ppFullPathList[ k ];
		arrArgv[ nCount + 2 ] = NULL;

		p4.GetClientApi().SetArgv( nCount + 2, const_cast<char**>( arrArgv.Base() ) );
		p4.GetClientApi().Run( s_pOperation, &g_ErrorHandlerUser );
	}
	else
	{
		p4.GetClientApi().SetArgv( nCount, const_cast<char**>( ppFullPathList ) );
		p4.GetClientApi().Run( s_pOperation, &g_ErrorHandlerUser );
	}
}

bool CP4::PerformPerforceOpCurChangeList( const char *pOperation, int nCount, const char **ppFullPathList )
{
	if ( m_sChangeListName.Length() )
	{
		s_pOperation = pOperation;
		return PerformPerforceOp( SimplePerforceOpCurChangeList, nCount, ppFullPathList, NULL );
	}
	else
	{
		return PerformPerforceOp( pOperation, nCount, ppFullPathList );
	}
}

bool CP4::PerformPerforceOp( const char *pOperation, const char *pFullPath )
{
	g_ErrorHandlerUser.ResetErrorState();

	if ( !IsConnectedToServer() )
	{
		g_ErrorHandlerUser.SetErrorString( "Not connected to P4 server\n" );
		return false;
	}

	CScopedFileClientSpec spec( pFullPath );

	char *argv[] = { (char *)pFullPath, NULL };
	m_Client.SetArgv( 1, argv );
	m_Client.Run( pOperation, &g_ErrorHandlerUser );

	return g_ErrorHandlerUser.GetErrorState() < E_WARN;
}

bool CP4::PerformPerforceOpCurChangeList( const char *pOperation, const char *pFullPath )
{
	g_ErrorHandlerUser.ResetErrorState();

	if ( !IsConnectedToServer() )
	{
		g_ErrorHandlerUser.SetErrorString( "Not connected to P4 server\n" );
		return false;
	}

	CScopedFileClientSpec spec( pFullPath );

	char *argv[] = { "-c", "", (char *)pFullPath, NULL }, **pArgv = argv;
	int numArgv = 3;

	if ( char const *szClNum = GetOpenFileChangeListNum() )
	{
		argv[ 1 ] = const_cast< char * >( szClNum );
	}
	else
	{
		pArgv += 2;
		numArgv -= 2;
	}
	
	m_Client.SetArgv( numArgv, pArgv );
	m_Client.Run( pOperation, &g_ErrorHandlerUser );

	return g_ErrorHandlerUser.GetErrorState() < E_WARN;
}

bool CP4::OpenFilesForAdd( int nCount, const char **ppFullPathList )
{
	return PerformPerforceOpCurChangeList( "add", nCount, ppFullPathList );
}

bool CP4::OpenFilesForEdit( int nCount, const char **ppFullPathList )
{
	if ( !PerformPerforceOpCurChangeList( "edit", nCount, ppFullPathList ) )
		return false;

	return MakeFilesWritable( nCount, ppFullPathList );
}

bool CP4::OpenFilesForDelete( int nCount, const char **ppFullPathList )
{
	return PerformPerforceOpCurChangeList( "delete", nCount, ppFullPathList );
}

bool CP4::RevertFiles( int nCount, const char **ppFullPathList )
{
	return PerformPerforceOp( "revert", nCount, ppFullPathList );
}


void SubmitPerforceOp( int nCount, const char **ppFullPathList, const char *pDescription )
{
	Assert( nCount > 0 );
	if ( nCount <= 0 )
		return;

	// First, create a new changelist
	g_ChangelistCreateUser.CreateChangelist( pDescription );
	if ( g_ChangelistCreateUser.GetData().Count() == 0 )
		return;

	int nChangeList = g_ChangelistCreateUser.GetData()[0];

	// Next, move all files to that new changelist
	char pBuf[128];
	Q_snprintf( pBuf, sizeof(pBuf), "%d", nChangeList );
	char **ppArgv = (char **)_alloca( (nCount + 2) * sizeof(char*) ); 
	ppArgv[0] = "-c";
	ppArgv[1] = pBuf;
	memcpy( &ppArgv[2], ppFullPathList, nCount * sizeof(char*) );
	p4.GetClientApi().SetArgv( nCount+2, ppArgv );
	p4.GetClientApi().Run( "reopen", &g_ErrorHandlerUser );

	// Finally, submit that changelist
	g_SubmitUser.Submit( nChangeList );
}

bool CP4::SubmitFiles( int nCount, const char **ppFullPathList, const char *pDescription )
{
	return PerformPerforceOp( SubmitPerforceOp, nCount, ppFullPathList, pDescription );
}

	
//-----------------------------------------------------------------------------
// Opens a file in p4 win
//-----------------------------------------------------------------------------
void CP4::OpenFileInP4Win( const char *pFullPath )
{
	if ( !IsConnectedToServer() )
		return;

	char pClientSpec[MAX_PATH];
	char pSystem[512];
	if ( GetClientSpecForFile( pFullPath, pClientSpec, sizeof(pClientSpec) ) )
	{
		Q_snprintf( pSystem, sizeof(pSystem), "p4win -q -c %s -s %s", pClientSpec, pFullPath );
	}
	else
	{
		Q_snprintf( pSystem, sizeof(pSystem), "p4win -q -s %s", pFullPath );
	}

	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	ZeroMemory( &pi, sizeof(pi) );

	// Start the child process. 
	CreateProcess( NULL, // No module name (use command line). 
		pSystem,			// Command line. 
		NULL,             // Process handle not inheritable. 
		NULL,             // Thread handle not inheritable. 
		FALSE,            // Set handle inheritance to FALSE. 
		0,                // No creation flags. 
		NULL,             // Use parent's environment block. 
		NULL,             // Use parent's starting directory. 
		&si,              // Pointer to STARTUPINFO structure.
		&pi );             // Pointer to PROCESS_INFORMATION structure.
}


//-----------------------------------------------------------------------------
// Is this file in perforce?
//-----------------------------------------------------------------------------
bool CP4::IsFileInPerforce( const char *fullpath )
{
	if ( !IsConnectedToServer() )
		return false;

	CScopedFileClientSpec spec( fullpath );

	g_FileUser.RetrieveFile( fullpath );
	if ( g_FileUser.GetData().Count() == 0 )
		return false;

	// Return deleted files as not being in perforce
	return !g_FileUser.GetData()[0].m_bDeleted;
}


//-----------------------------------------------------------------------------
// Is this file opened for edit?
//-----------------------------------------------------------------------------
P4FileState_t CP4::GetFileState( const char *pFullPath )
{
	if ( !IsConnectedToServer() )
		return P4FILE_UNOPENED;

	CScopedFileClientSpec spec( pFullPath );

	g_FileUser.RetrieveOpenedFiles( pFullPath );
	if ( g_FileUser.GetData().Count() == 0 )
		return P4FILE_UNOPENED;
	return g_FileUser.GetData()[0].m_eOpenState;
}


//-----------------------------------------------------------------------------
// Returns file information for a single file
//-----------------------------------------------------------------------------
bool CP4::GetFileInfo( const char *pFullPath, P4File_t *pFileInfo )
{
	if ( !IsConnectedToServer() )
		return false;

	CScopedFileClientSpec spec( pFullPath );

	g_FileUser.RetrieveFile( pFullPath );
	if ( g_FileUser.GetData().Count() != 1 )
		return false;

	memcpy( pFileInfo, &g_FileUser.GetData()[0], sizeof(P4File_t) );
	return true;
}

	
//-----------------------------------------------------------------------------
// Are we connected to the server? (and should we reconnect?)
//-----------------------------------------------------------------------------
bool CP4::IsConnectedToServer( bool bRetry )
{
	if ( bRetry && m_bConnectedToServer )
	{
		// don't comment this back in until it's called less often to avoid spamming the server
		// (currently, this is called every time the ifm file menu is opened)
		// this means that we won't detect losing the server until after a failed Run() cmd
		// -jd

//		m_Client.Run( "monitor show" ); // do something to test if the server's still up
		if ( m_Client.Dropped() )
		{
			Shutdown();
		}
	}

	if ( !m_bConnectedToServer && bRetry )
	{
		Init();
	}

	return m_bConnectedToServer;
}

//-----------------------------------------------------------------------------
// retrieves the last error from the last op (which is likely to span multiple lines)
// this is only valid after OpenFile[s]For{Add,Edit,Delete} or {Submit,Revert}File[s]
//-----------------------------------------------------------------------------
const char *CP4::GetLastError()
{
	return g_ErrorHandlerUser.GetErrorString();
}