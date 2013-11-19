//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//
//--------------------------------------------------------------------------------------------------------------
// download.cpp
// 
// Implementation file for optional HTTP asset downloading
// Author: Matthew D. Campbell (matt@turtlerockstudios.com), 2004
//--------------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------------------------------------------

#ifdef _WIN32

// fopen is needed for the bzip code
#undef fopen

#if !defined( _X360 )
#include "winlite.h"
#include <WinInet.h>
#endif

#include <assert.h>

#include "download.h"
#include "download_internal.h"

#include "client.h"

#include <KeyValues.h>
#include "filesystem.h"
#include "filesystem_engine.h"
#include "server.h"
#include "vgui_baseui_interface.h"
#include "tier0/vcrmode.h"

#include "../utils/bzip2/bzlib.h"

#if defined( _X360 )
#include "xbox/xbox_win32stubs.h"
#endif

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

extern IFileSystem *g_pFileSystem;
static const char *CacheDirectory = "cache";
static const char *CacheFilename = "cache/DownloadCache.db";
Color DownloadColor			(   0, 200, 100, 255 );
Color DownloadErrorColor	( 200, 100, 100, 255 );
Color DownloadCompleteColor	( 100, 200, 100, 255 );

//--------------------------------------------------------------------------------------------------------------
static char * CloneString( const char *original )
{
	char *newString = new char[ Q_strlen( original ) + 1 ];
	Q_strcpy( newString, original );

	return newString;
}

//--------------------------------------------------------------------------------------------------------------
// Class Definitions
//--------------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------
// Purpose: Implements download cache manager
//--------------------------------------------------------------------------------------------------------------
class DownloadCache
{
public:
	DownloadCache();
	~DownloadCache();
	void Init();

	void GetCachedData( RequestContext *rc );			///< Loads cached data, if any
	void PersistToDisk( const RequestContext *rc );		///< Writes out a completed download to disk
	void PersistToCache( const RequestContext *rc );	///< Writes out a partial download (lost connection, user abort, etc) to cache

private:
	KeyValues *m_cache;

	void GetCacheFilename( const RequestContext *rc, char cachePath[_MAX_PATH] );
	void GenerateCacheFilename( const RequestContext *rc, char cachePath[_MAX_PATH] );

	void BuildKeyNames( const char *gamePath );			///< Convenience function to build the keys to index into m_cache
	char m_cachefileKey[BufferSize + 64];
	char m_timestampKey[BufferSize + 64];
};
static DownloadCache *TheDownloadCache = NULL;

//--------------------------------------------------------------------------------------------------------------
DownloadCache::DownloadCache()
{
	m_cache = NULL;
}

//--------------------------------------------------------------------------------------------------------------
DownloadCache::~DownloadCache()
{
}

//--------------------------------------------------------------------------------------------------------------
void DownloadCache::BuildKeyNames( const char *gamePath )
{
	if ( !gamePath )
	{
		m_cachefileKey[0] = 0;
		m_timestampKey[0] = 0;
		return;
	}

	char *tmpGamePath = CloneString( gamePath );
	char *tmp = tmpGamePath;
	while ( *tmp )
	{
		if ( *tmp == '/' || *tmp == '\\' )
		{
			*tmp = '_';
		}
		++tmp;
	}
	Q_snprintf( m_cachefileKey, sizeof( m_cachefileKey ), "cachefile_%s", tmpGamePath );
	Q_snprintf( m_timestampKey, sizeof( m_timestampKey ), "timestamp_%s", tmpGamePath );

	delete[] tmpGamePath;
}

//--------------------------------------------------------------------------------------------------------------
void DownloadCache::Init()
{
	if ( m_cache )
	{
		m_cache->deleteThis();
	}

	m_cache = new KeyValues( "DownloadCache" );
	m_cache->LoadFromFile( g_pFileSystem, CacheFilename, NULL );
	g_pFileSystem->CreateDirHierarchy( CacheDirectory, "DEFAULT_WRITE_PATH" );
}

//--------------------------------------------------------------------------------------------------------------
void DownloadCache::GetCachedData( RequestContext *rc )
{
	if ( !m_cache )
		return;

	char cachePath[_MAX_PATH];
	GetCacheFilename( rc, cachePath );

	if ( !(*cachePath) )
		return;

	FileHandle_t fp = g_pFileSystem->Open( cachePath, "rb" );

	if ( fp == FILESYSTEM_INVALID_HANDLE )
		return;

	int size = g_pFileSystem->Size(fp);
	rc->cacheData = new unsigned char[size];
	int status = g_pFileSystem->Read( rc->cacheData, size, fp );
	g_pFileSystem->Close( fp );
	if ( !status )
	{
		delete[] rc->cacheData;
		rc->cacheData = NULL;
	}
	else
	{
		BuildKeyNames( rc->gamePath );
		rc->nBytesCached = size;
		strncpy( rc->cachedTimestamp, m_cache->GetString( m_timestampKey, "" ), BufferSize );
	}
}

//--------------------------------------------------------------------------------------------------------------
/**
 *  Takes a data stream compressed with bzip2, and writes it out to disk, uncompresses it, and deletes the
 *  compressed version.
 */
static bool DecompressBZipToDisk( const char *outFilename, const char *srcFilename, char *data, int bytesTotal )
{
	if ( g_pFileSystem->FileExists( outFilename ) || !data || bytesTotal < 1 )
	{
		return false;
	}

	// Create the subdirs
	char * tmpDir = CloneString( outFilename );
	COM_CreatePath( tmpDir );
	delete[] tmpDir;

	// open the file for writing
	char fullSrcPath[MAX_PATH];
	Q_MakeAbsolutePath( fullSrcPath, sizeof( fullSrcPath ), srcFilename, com_gamedir );

	if ( !g_pFileSystem->FileExists( fullSrcPath ) )
	{
		// Write out the .bz2 file, for simplest decompression
		FileHandle_t ifp = g_pFileSystem->Open( fullSrcPath, "wb" );
		if ( !ifp )
		{
			return false;
		}
		int bytesWritten = g_pFileSystem->Write( data, bytesTotal, ifp );
		g_pFileSystem->Close( ifp );
		if ( bytesWritten != bytesTotal )
		{
			// couldn't write out all of the .bz2 file
			g_pFileSystem->RemoveFile( srcFilename );
			return false;
		}
	}

	// Prepare the uncompressed filehandle
	FileHandle_t ofp = g_pFileSystem->Open( outFilename, "wb" );
	if ( !ofp )
	{
		g_pFileSystem->RemoveFile( srcFilename );
		return false;
	}

	// And decompress!
	const int OutBufSize = 65536;
	char    buf[ OutBufSize ];
	BZFILE *bzfp = BZ2_bzopen( fullSrcPath, "rb" );
	while ( 1 )
	{
		int bytesRead = BZ2_bzread( bzfp, buf, OutBufSize );
		if ( bytesRead < 0 )
		{
			break; // error out
		}

		if ( bytesRead > 0 )
		{
			int bytesWritten = g_pFileSystem->Write( buf, bytesRead, ofp );
			if ( bytesWritten != bytesRead )
			{
				break; // error out
			}
		}
		else
		{
			g_pFileSystem->Close( ofp );
			BZ2_bzclose( bzfp );
			g_pFileSystem->RemoveFile( srcFilename );
			return true;
		}
	}

	// We failed somewhere, so clean up and exit
	g_pFileSystem->Close( ofp );
	BZ2_bzclose( bzfp );
	g_pFileSystem->RemoveFile( srcFilename );
	g_pFileSystem->RemoveFile( outFilename );
	return false;
}

//--------------------------------------------------------------------------------------------------------------
void DownloadCache::PersistToDisk( const RequestContext *rc )
{
	if ( !m_cache )
		return;

	if ( rc && rc->data && rc->nBytesTotal )
	{
		char gamePath[MAX_PATH];
		if ( rc->bIsBZ2 )
		{
			Q_StripExtension( rc->gamePath, gamePath, sizeof( gamePath ) );
		}
		else
		{
			Q_strncpy( gamePath, rc->gamePath, sizeof( gamePath ) );
		}

		if ( !g_pFileSystem->FileExists( gamePath ) )
		{
			// Create the subdirs
			char * tmpDir = CloneString( gamePath );
			COM_CreatePath( tmpDir );
			delete[] tmpDir;

			bool success = false;
			if ( rc->bIsBZ2 )
			{
				success = DecompressBZipToDisk( gamePath, rc->gamePath, reinterpret_cast< char * >(rc->data), rc->nBytesTotal );
			}
			else
			{
				FileHandle_t fp = g_pFileSystem->Open( gamePath, "wb" );
				if ( fp )
				{
					g_pFileSystem->Write( rc->data, rc->nBytesTotal, fp );
					g_pFileSystem->Close( fp );
					success = true;
				}
			}

			if ( success )
			{
				// write succeeded.  remove any old data from the cache.
				char cachePath[_MAX_PATH];
				GetCacheFilename( rc, cachePath );
				if ( cachePath[0] )
				{
					g_pFileSystem->RemoveFile( cachePath, NULL );
				}

				BuildKeyNames( rc->gamePath );
				KeyValues *kv = m_cache->FindKey( m_cachefileKey, false );
				if ( kv )
				{
					m_cache->RemoveSubKey( kv );
				}
				kv = m_cache->FindKey( m_timestampKey, false );
				if ( kv )
				{
					m_cache->RemoveSubKey( kv );
				}
			}
		}
	}

	m_cache->SaveToFile( g_pFileSystem, CacheFilename, NULL );
}

//--------------------------------------------------------------------------------------------------------------
void DownloadCache::PersistToCache( const RequestContext *rc )
{
	if ( !m_cache || !rc || !rc->data || !rc->nBytesTotal || !rc->nBytesCurrent )
		return;

	char cachePath[_MAX_PATH];
	GenerateCacheFilename( rc, cachePath );

	FileHandle_t fp = g_pFileSystem->Open( cachePath, "wb" );
	if ( fp )
	{
		g_pFileSystem->Write( rc->data, rc->nBytesCurrent, fp );
		g_pFileSystem->Close( fp );

		m_cache->SaveToFile( g_pFileSystem, CacheFilename, NULL );
	}
}

//--------------------------------------------------------------------------------------------------------------
void DownloadCache::GetCacheFilename( const RequestContext *rc, char cachePath[_MAX_PATH] )
{
	BuildKeyNames( rc->gamePath );
	const char *path = m_cache->GetString( m_cachefileKey, NULL );
	if ( !path || strncmp( path, CacheDirectory, strlen(CacheDirectory) ) )
	{
		cachePath[0] = 0;
		return;
	}
	strncpy( cachePath, path, _MAX_PATH );
	cachePath[_MAX_PATH-1] = 0;
}

//--------------------------------------------------------------------------------------------------------------
void DownloadCache::GenerateCacheFilename( const RequestContext *rc, char cachePath[_MAX_PATH] )
{
	GetCacheFilename( rc, cachePath );
	BuildKeyNames( rc->gamePath );

	m_cache->SetString( m_timestampKey, rc->cachedTimestamp );
	//ConDColorMsg( DownloadColor,"DownloadCache::GenerateCacheFilename() set %s = %s\n", m_timestampKey, rc->cachedTimestamp );

	if ( !*cachePath )
	{
		const char * lastSlash = strrchr( rc->gamePath, '/' );
		const char * lastBackslash = strrchr( rc->gamePath, '\\' );
		const char *gameFilename = rc->gamePath;
		if ( lastSlash || lastBackslash )
		{
			gameFilename = max( lastSlash, lastBackslash ) + 1;
		}
		for( int i=0; i<1000; ++i )
		{
			Q_snprintf( cachePath, _MAX_PATH, "%s/%s%4.4d", CacheDirectory, gameFilename, i );
			if ( !g_pFileSystem->FileExists( cachePath ) )
			{
				m_cache->SetString( m_cachefileKey, cachePath );
				//ConDColorMsg( DownloadColor,"DownloadCache::GenerateCacheFilename() set %s = %s\n", m_cachefileKey, cachePath );
				return;
			}
		}
		// all 1000 were invalid?!?
		Q_snprintf( cachePath, _MAX_PATH, "%s/overflow", CacheDirectory );
		//ConDColorMsg( DownloadColor,"DownloadCache::GenerateCacheFilename() set %s = %s\n", m_cachefileKey, cachePath );
		m_cache->SetString( m_cachefileKey, cachePath );
	}
}

//--------------------------------------------------------------------------------------------------------------
// Purpose: Implements download manager class
//--------------------------------------------------------------------------------------------------------------
class DownloadManager
{
public:
	DownloadManager();
	~DownloadManager();

	void Queue( const char *baseURL, const char *gamePath );
	void Stop() { Reset(); }
	int GetQueueSize() { return m_queuedRequests.Count(); }

	bool Update();	///< Monitors download thread, starts new downloads, and updates progress bar

	bool FileReceived( const char *filename, unsigned int requestID );
	bool FileDenied( const char *filename, unsigned int requestID );

	bool HasMapBeenDownloadedFromServer( const char *serverMapName );
	void MarkMapAsDownloadedFromServer( const char *serverMapName );

private:
	void Reset();						///< Cancels any active download, as well as any queued ones

	void PruneCompletedRequests();		///< Check download requests that have been completed to see if their threads have exited
	void CheckActiveDownload();			///< Checks download status, and updates progress bar
	void StartNewDownload();			///< Starts a new download if there are queued requests

	void UpdateProgressBar();

	typedef CUtlVector< RequestContext * > RequestVector;
	RequestVector m_queuedRequests;		///< these are requests waiting to be spawned
	RequestContext *m_activeRequest;	///< this is the active request being downloaded in another thread
	RequestVector m_completedRequests;	///< these are waiting for the thread to exit

	int m_lastPercent;					///< last percent value the progress bar was updated with (to avoid spamming it)
	int m_totalRequests;				///< Total number of requests (used to set the top progress bar)

	int m_RequestIDCounter;				///< global increasing request ID counter

	typedef CUtlVector< char * > StrVector;
	StrVector m_downloadedMaps;			///< List of maps for which we have already tried to download assets.
};

//--------------------------------------------------------------------------------------------------------------
static DownloadManager TheDownloadManager;

//--------------------------------------------------------------------------------------------------------------
DownloadManager::DownloadManager()
{
	m_activeRequest = NULL;
	m_lastPercent = 0;
	m_totalRequests = 0;
}

//--------------------------------------------------------------------------------------------------------------
DownloadManager::~DownloadManager()
{
	Reset();

	for ( int i=0; i<m_downloadedMaps.Count(); ++i )
	{
		delete[] m_downloadedMaps[i];
	}
	m_downloadedMaps.RemoveAll();
}

//--------------------------------------------------------------------------------------------------------------
bool DownloadManager::HasMapBeenDownloadedFromServer( const char *serverMapName )
{
	if ( !serverMapName )
		return false;

	for ( int i=0; i<m_downloadedMaps.Count(); ++i )
	{
		const char *oldServerMapName = m_downloadedMaps[i];
		if ( oldServerMapName && !stricmp( serverMapName, oldServerMapName ) )
		{
			return true;
		}
	}
	return false;
}

bool DownloadManager::FileDenied( const char *filename, unsigned int requestID )
{
	if ( !m_activeRequest )
		return false;

	if ( m_activeRequest->nRequestID != requestID )
		return false;

	if ( m_activeRequest->bAsHTTP )
		return false;

	ConDColorMsg( DownloadErrorColor, "Error downloading %s\n", m_activeRequest->gamePath );
	UpdateProgressBar();

	// try to download the next file
	m_completedRequests.AddToTail( m_activeRequest );
	m_activeRequest = NULL;

	return true;
}

bool DownloadManager::FileReceived( const char *filename, unsigned int requestID )
{
	if ( !m_activeRequest )
		return false;

	if ( m_activeRequest->nRequestID != requestID )
		return false;

	if ( m_activeRequest->bAsHTTP )
		return false;

	ConDColorMsg( DownloadCompleteColor, "Download finished!\n" );
	UpdateProgressBar();

	m_completedRequests.AddToTail( m_activeRequest );
	m_activeRequest = NULL;

	return true;
}

//--------------------------------------------------------------------------------------------------------------
void DownloadManager::MarkMapAsDownloadedFromServer( const char *serverMapName )
{
	if ( !serverMapName )
		return;

	if ( HasMapBeenDownloadedFromServer( serverMapName ) )
		return;

	m_downloadedMaps.AddToTail( CloneString( serverMapName ) );


	return;
}

//--------------------------------------------------------------------------------------------------------------
void DownloadManager::Queue( const char *baseURL, const char *gamePath )
{
	bool bAsHTTP = false;
	if ( !gamePath )
	{
		return;
	}

#ifndef _DEBUG
	if ( sv.IsActive() )
	{
		return;	// don't try to download things for the local server (in case a map is missing sounds etc that
				// aren't needed to play.
	}
#endif

	// only http downloads
	if ( baseURL && (!Q_strnicmp( baseURL, "http://", 7 ) || !Q_strnicmp( baseURL, "https://", 8 )) )
	{
		bAsHTTP = true;
	}

	if ( g_pFileSystem->FileExists( gamePath ) )
	{
		return; // don't download existing files
	}

	if ( Q_strstr( gamePath, "//" ) )
	{
		return;
	}

	if ( Q_strstr( gamePath, "\\\\" ) )
	{
		return;
	}

	if ( Q_strstr( gamePath, ":" ) )
	{
		return;
	}

	if ( Q_strstr( gamePath, "lua/" ) )
		return; // don't download into lua/ folder

	if ( Q_strstr( gamePath, "gamemodes/" ) ) 
		return; // don't download into gamemodes/ folder 

	if ( Q_strstr( gamePath, "addons/" ) ) 
		return; // don't download into addons/ folder 


	// Disallow .. in paths, but allow multiple periods otherwise.  This way we can download blah.dx80.vtx etc.
	const char *backup = strstr( gamePath, ".." );
	const char *extension = strrchr( gamePath, '.' );

	if ( backup || !extension )
		return;

	int baseLen = strlen( extension );
	if ( baseLen > 4 || baseLen < 3 )
		return;

	if ( !Q_strcasecmp( extension, ".cfg" ) )
		return;

	if ( !Q_strcasecmp( extension, ".lst" ) )
		return;

	if ( !Q_strcasecmp( extension, ".exe" ) )
		return;

	if ( !Q_strcasecmp( extension, ".vbs" ) )
		return;

	if ( !Q_strcasecmp( extension, ".com" ) )
		return;

	if ( !Q_strcasecmp( extension, ".bat" ) )
		return;

	if ( !Q_strcasecmp( extension, ".dll" ) )
		return;

	if ( !Q_strcasecmp( extension, ".ini" ) )
		return;

	if ( !Q_strcasecmp( extension, ".log" ) )
		return;

	if ( !Q_strcasecmp( extension, ".lua" ) )
		return;

	if ( bAsHTTP && !g_pFileSystem->FileExists( va( "%s.bz2", gamePath ) ) )
	{
		// Queue up an HTTP download of the bzipped asset, in case it exists.
		// When a bzipped download finishes, we'll uncompress the file to it's
		// original destination, and the queued download of the uncompressed
		// file will abort.

		++m_totalRequests;
		if ( !TheDownloadCache )
		{
			TheDownloadCache = new DownloadCache;
			TheDownloadCache->Init();
		}

		RequestContext *rc = new RequestContext;
		m_queuedRequests.AddToTail( rc );

		memset( rc, 0, sizeof(RequestContext) );

		rc->status = HTTP_CONNECTING;

		Q_strncpy( rc->basePath, com_gamedir, BufferSize );
		Q_strncpy( rc->gamePath, gamePath, BufferSize );
		Q_strncat( rc->gamePath, ".bz2", BufferSize, COPY_ALL_CHARACTERS );
		Q_FixSlashes( rc->gamePath, '/' ); // only matters for debug prints, which are full URLS, so we want forward slashes
		Q_strncpy( rc->serverURL, cl.m_NetChannel->GetRemoteAddress().ToString(), BufferSize );

		rc->bIsBZ2 = true;
		rc->bAsHTTP = true;
		Q_strncpy( rc->baseURL, baseURL, BufferSize );
		Q_strncat( rc->baseURL, "/", BufferSize, COPY_ALL_CHARACTERS );

		//ConDColorMsg( DownloadColor, "Queueing %s%s.\n", rc->baseURL, gamePath );
	}

	++m_totalRequests;
	if ( !TheDownloadCache )
	{
		TheDownloadCache = new DownloadCache;
		TheDownloadCache->Init();
	}

	RequestContext *rc = new RequestContext;
	m_queuedRequests.AddToTail( rc );

	memset( rc, 0, sizeof(RequestContext) );

	rc->status = HTTP_CONNECTING;

	Q_strncpy( rc->basePath, com_gamedir, BufferSize );
	Q_strncpy( rc->gamePath, gamePath, BufferSize );
	Q_FixSlashes( rc->gamePath, '/' ); // only matters for debug prints, which are full URLS, so we want forward slashes
	Q_strncpy( rc->serverURL, cl.m_NetChannel->GetRemoteAddress().ToString(), BufferSize );

	if ( bAsHTTP )
	{
		rc->bAsHTTP = true;
		Q_strncpy( rc->baseURL, baseURL, BufferSize );
		Q_strncat( rc->baseURL, "/", BufferSize, COPY_ALL_CHARACTERS );
	}
	else
	{
		rc->bAsHTTP = false;
	}

	//ConDColorMsg( DownloadColor, "Queueing %s%s.\n", rc->baseURL, gamePath );
}

//--------------------------------------------------------------------------------------------------------------
void DownloadManager::Reset()
{
	// ask the active request to bail
	if ( m_activeRequest )
	{
		ConDColorMsg( DownloadColor, "Aborting download of %s\n", m_activeRequest->gamePath );
		if ( m_activeRequest->nBytesTotal && m_activeRequest->nBytesCurrent )
		{
			// Persist partial data to cache
			TheDownloadCache->PersistToCache( m_activeRequest );
		}
		m_activeRequest->shouldStop = true;
		m_completedRequests.AddToTail( m_activeRequest );
		m_activeRequest = NULL;
		//TODO: StopLoadingProgressBar();
	}

	// clear out any queued requests
	for ( int i=0; i<m_queuedRequests.Count(); ++i )
	{
		ConDColorMsg( DownloadColor, "Discarding queued download of %s\n", m_queuedRequests[i]->gamePath );
		delete m_queuedRequests[i];
	}
	m_queuedRequests.RemoveAll();

	if ( TheDownloadCache )
	{
		delete TheDownloadCache;
		TheDownloadCache = NULL;
	}

	m_lastPercent = 0;
	m_totalRequests = 0;
}

//--------------------------------------------------------------------------------------------------------------
// Check download requests that have been completed to see if their threads have exited
void DownloadManager::PruneCompletedRequests()
{
	for ( int i=m_completedRequests.Count()-1; i>=0; --i )
	{
		if ( m_completedRequests[i]->threadDone || !m_completedRequests[i]->bAsHTTP )
		{
			if ( m_completedRequests[i]->cacheData )
			{
				delete[] m_completedRequests[i]->cacheData;
			}
			delete m_completedRequests[i];
			m_completedRequests.Remove( i );
		}
	}
}

//--------------------------------------------------------------------------------------------------------------
// Checks download status, and updates progress bar
void DownloadManager::CheckActiveDownload()
{
	if ( !m_activeRequest )
		return;

	if ( !m_activeRequest->bAsHTTP )
	{
		UpdateProgressBar();
		return;
	}

	
	// check active request for completion / error / progress update
	switch ( m_activeRequest->status )
	{
	case HTTP_DONE:
		ConDColorMsg( DownloadCompleteColor, "Download finished!\n" );
		UpdateProgressBar();
		if ( m_activeRequest->nBytesTotal )
		{
			// Persist complete data to disk, and remove cache entry
			//TODO: SetSecondaryProgressBarText( m_activeRequest->gamePath );
			TheDownloadCache->PersistToDisk( m_activeRequest );
			m_activeRequest->shouldStop = true;
			m_completedRequests.AddToTail( m_activeRequest );
			m_activeRequest = NULL;
			if ( !m_queuedRequests.Count() )
			{
				//TODO: StopLoadingProgressBar();
				//TODO: Cbuf_AddText("retry\n");
			}
		}
		break;
	case HTTP_ERROR:
		ConDColorMsg( DownloadErrorColor, "Error downloading %s%s\n", m_activeRequest->baseURL, m_activeRequest->gamePath );
		UpdateProgressBar();

		// try to download the next file
		m_activeRequest->shouldStop = true;
		m_completedRequests.AddToTail( m_activeRequest );
		m_activeRequest = NULL;
		if ( !m_queuedRequests.Count() )
		{
			//TODO: StopLoadingProgressBar();
			//TODO: Cbuf_AddText("retry\n");
		}
		break;
	case HTTP_FETCH:
		UpdateProgressBar();
		// Update progress bar
		//TODO: SetSecondaryProgressBarText( m_activeRequest->gamePath );
		if ( m_activeRequest->nBytesTotal )
		{
			int percent = ( m_activeRequest->nBytesCurrent * 100 / m_activeRequest->nBytesTotal );
			if ( percent != m_lastPercent )
			{
				/*
				ConDColorMsg( DownloadColor, "Downloading %s%s: %3.3d%% - %d of %d bytes\n",
					m_activeRequest->baseURL, m_activeRequest->gamePath,
					percent, m_activeRequest->nBytesCurrent, m_activeRequest->nBytesTotal );
					*/
				m_lastPercent = percent;
				//TODO: SetSecondaryProgressBar( m_lastPercent * 0.01f );
			}
		}
		break;
	}
}

//--------------------------------------------------------------------------------------------------------------
// Starts a new download if there are queued requests
void DownloadManager::StartNewDownload()
{
	if ( m_activeRequest || !m_queuedRequests.Count() )
		return;

	while ( !m_activeRequest && m_queuedRequests.Count() )
	{
		// Remove one request from the queue and make it active
		m_activeRequest = m_queuedRequests[0];
		m_queuedRequests.Remove( 0 );

		if ( g_pFileSystem->FileExists( m_activeRequest->gamePath ) )
		{
			ConDColorMsg( DownloadColor, "Skipping existing file %s%s.\n", m_activeRequest->baseURL, m_activeRequest->gamePath );
			m_activeRequest->shouldStop = true;
			m_activeRequest->threadDone = true;
			m_completedRequests.AddToTail( m_activeRequest );
			m_activeRequest = NULL;
		}
	}

	if ( !m_activeRequest )
		return;

	if ( g_pFileSystem->FileExists( m_activeRequest->gamePath ) )
	{
		m_activeRequest->shouldStop = true;
		m_activeRequest->threadDone = true;
		m_completedRequests.AddToTail( m_activeRequest );
		m_activeRequest = NULL;
		return; // don't download existing files
	}

	if ( m_activeRequest->bAsHTTP )
	{
		// Check cache for partial match
		TheDownloadCache->GetCachedData( m_activeRequest );

		//TODO: ContinueLoadingProgressBar( "Http", m_totalRequests - m_queuedRequests.Count(), 0.0f );
		//TODO: SetLoadingProgressBarStatusText( "#GameUI_VerifyingAndDownloading" );
		//TODO: SetSecondaryProgressBarText( m_activeRequest->gamePath );
		//TODO: SetSecondaryProgressBar( 0.0f );
		UpdateProgressBar();

		ConDColorMsg( DownloadColor, "Downloading %s%s.\n", m_activeRequest->baseURL, m_activeRequest->gamePath );
		m_lastPercent = 0;

		// Start the thread
		DWORD threadID;
		VCRHook_CreateThread(NULL, 0, DownloadThread, m_activeRequest, 0, &threadID );
	}
	else
	{
		UpdateProgressBar();
		ConDColorMsg( DownloadColor, "Downloading %s.\n", m_activeRequest->gamePath );
		m_lastPercent = 0;
		
		m_activeRequest->nRequestID = cl.m_NetChannel->RequestFile( m_activeRequest->gamePath );
	}
}

//--------------------------------------------------------------------------------------------------------------
void DownloadManager::UpdateProgressBar()
{
	if ( !m_activeRequest )
	{
		return;
	}

	wchar_t filenameBuf[MAX_OSPATH];
	float progress = 0.0f;

	if ( m_activeRequest->bAsHTTP )
	{
		int overallPercent = (m_totalRequests - m_queuedRequests.Count() - 1) * 100 / m_totalRequests;
		int filePercent = 0;
		if ( m_activeRequest->nBytesTotal > 0 )
		{	
			filePercent = ( m_activeRequest->nBytesCurrent * 100 / m_activeRequest->nBytesTotal );
		}

		progress = (overallPercent + filePercent * 1.0f / m_totalRequests) * 0.01f;
	}
	else
	{
		int received, total;
		cl.m_NetChannel->GetStreamProgress( FLOW_INCOMING, &received, &total );
		
		progress = (float)(received)/(float)(total);
	}

	_snwprintf( filenameBuf, 256, L"Downloading %hs", m_activeRequest->gamePath );
	EngineVGui()->UpdateCustomProgressBar( progress, filenameBuf );
}

//--------------------------------------------------------------------------------------------------------------
// Monitors download thread, starts new downloads, and updates progress bar
bool DownloadManager::Update()
{
	PruneCompletedRequests();
	CheckActiveDownload();
	StartNewDownload();

	return m_activeRequest != NULL;
}

//--------------------------------------------------------------------------------------------------------------
// Externally-visible function definitions
//--------------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------
bool CL_DownloadUpdate(void)
{
	return TheDownloadManager.Update();
}

//--------------------------------------------------------------------------------------------------------------
void CL_HTTPStop_f(void)
{
	TheDownloadManager.Stop();
}

bool CL_FileReceived( const char *filename, unsigned int requestID )
{
	return TheDownloadManager.FileReceived( filename, requestID );
}

bool CL_FileDenied( const char *filename, unsigned int requestID )
{
	return TheDownloadManager.FileDenied( filename, requestID );
}

//--------------------------------------------------------------------------------------------------------------
extern ConVar sv_downloadurl;
void CL_QueueDownload( const char *filename )
{
	TheDownloadManager.Queue( sv_downloadurl.GetString(), filename );
}

//--------------------------------------------------------------------------------------------------------------
int CL_GetDownloadQueueSize(void)
{
	return TheDownloadManager.GetQueueSize();
}

//--------------------------------------------------------------------------------------------------------------
int CL_CanUseHTTPDownload(void)
{
	if ( sv_downloadurl.GetString()[0] )
	{
		const char *serverMapName = va( "%s:%s", sv_downloadurl.GetString(), cl.m_szLevelName );
		return !TheDownloadManager.HasMapBeenDownloadedFromServer( serverMapName );
	}
	return 0;
}

//--------------------------------------------------------------------------------------------------------------
void CL_MarkMapAsUsingHTTPDownload(void)
{
	const char *serverMapName = va( "%s:%s", sv_downloadurl.GetString(), cl.m_szLevelName );
	TheDownloadManager.MarkMapAsDownloadedFromServer( serverMapName );
}


//--------------------------------------------------------------------------------------------------------------

#else // !_WIN32

//--------------------------------------------------------------------------------------------------------------
void CL_HTTPUpdate(void)
{
}

//--------------------------------------------------------------------------------------------------------------
void CL_HTTPStop_f(void)
{
}

//--------------------------------------------------------------------------------------------------------------
void CL_QueueHTTPDownload( const char *filename )
{
}

//--------------------------------------------------------------------------------------------------------------
int CL_GetDownloadQueueSize(void)
{
	return 0;
}

//--------------------------------------------------------------------------------------------------------------
int CL_CanUseHTTPDownload(void)
{
	return 0;
}

//--------------------------------------------------------------------------------------------------------------
void CL_MarkMapAsUsingHTTPDownload(void)
{
}

#endif // _WIN32
