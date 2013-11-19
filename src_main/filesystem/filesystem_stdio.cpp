//====== Copyright © 1996-2005, Valve Corporation, All rights reserved. =======//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifdef _LINUX
#include "linux_support.cpp"
#endif
#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#endif

#include "basefilesystem.h"
#include "tier0/dbg.h"
#include "tier0/threadtools.h"
#ifdef _WIN32
#include "tier0/tslist.h"
#endif
#include "tier1/convar.h"
#include "tier0/vcrmode.h"
#include "tier0/vprof.h"
#include "tier1/fmtstr.h"
#include "tier1/utlrbtree.h"

#ifdef _X360
#undef WaitForSingleObject
#endif

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

ASSERT_INVARIANT( SEEK_CUR == FILESYSTEM_SEEK_CURRENT );
ASSERT_INVARIANT( SEEK_SET == FILESYSTEM_SEEK_HEAD );
ASSERT_INVARIANT( SEEK_END == FILESYSTEM_SEEK_TAIL );

//-----------------------------------------------------------------------------

class CFileSystem_Stdio : public CBaseFileSystem
{
public:
	CFileSystem_Stdio();
	~CFileSystem_Stdio();

	// Used to get at older versions
	void *QueryInterface( const char *pInterfaceName );

	// Higher level filesystem methods requiring specific behavior
	virtual void GetLocalCopy( const char *pFileName );
	virtual int	HintResourceNeed( const char *hintlist, int forgetEverything );
	virtual bool IsFileImmediatelyAvailable(const char *pFileName);
	virtual WaitForResourcesHandle_t WaitForResources( const char *resourcelist );
	virtual bool GetWaitForResourcesProgress( WaitForResourcesHandle_t handle, float *progress /* out */ , bool *complete /* out */ );
	virtual void CancelWaitForResources( WaitForResourcesHandle_t handle );
	virtual bool IsSteam() const { return false; }
	virtual	FilesystemMountRetval_t MountSteamContent( int nExtraAppId = -1 ) { return FILESYSTEM_MOUNT_OK; }

	bool GetOptimalIOConstraints( FileHandle_t hFile, unsigned *pOffsetAlign, unsigned *pSizeAlign, unsigned *pBufferAlign );
	void *AllocOptimalReadBuffer( FileHandle_t hFile, unsigned nSize, unsigned nOffset );
	void FreeOptimalReadBuffer( void *p );

protected:
	// implementation of CBaseFileSystem virtual functions
	virtual FILE *FS_fopen( const char *filename, const char *options, unsigned flags, int64 *size, CFileLoadInfo *pInfo );
	virtual void FS_setbufsize( FILE *fp, unsigned nBytes );
	virtual void FS_fclose( FILE *fp );
	virtual void FS_fseek( FILE *fp, int64 pos, int seekType );
	virtual long FS_ftell( FILE *fp );
	virtual int FS_feof( FILE *fp );
	virtual size_t FS_fread( void *dest, size_t destSize, size_t size, FILE *fp );
	virtual size_t FS_fwrite( const void *src, size_t size, FILE *fp );
	virtual bool FS_setmode( FILE *fp, FileMode_t mode );
	virtual size_t FS_vfprintf( FILE *fp, const char *fmt, va_list list );
	virtual int FS_ferror( FILE *fp );
	virtual int FS_fflush( FILE *fp );
	virtual char *FS_fgets( char *dest, int destSize, FILE *fp );
	virtual int FS_stat( const char *path, struct _stat *buf );
	virtual int FS_chmod( const char *path, int pmode );
	virtual HANDLE FS_FindFirstFile(const char *findname, WIN32_FIND_DATA *dat);
	virtual bool FS_FindNextFile(HANDLE handle, WIN32_FIND_DATA *dat);
	virtual bool FS_FindClose(HANDLE handle);
	virtual int FS_GetSectorSize( FILE * );

private:
	bool CanAsync() const
	{
		m_bCanAsync;
	}

	bool m_bMounted;
	bool m_bCanAsync;
};


//-----------------------------------------------------------------------------
// Per-file worker classes
//-----------------------------------------------------------------------------
abstract_class CStdFilesystemFile
{
public:
	virtual ~CStdFilesystemFile() {}
	virtual void FS_setbufsize( unsigned nBytes ) = 0;
	virtual void FS_fclose() = 0;
	virtual void FS_fseek( int64 pos, int seekType ) = 0;
	virtual long FS_ftell() = 0;
	virtual int FS_feof() = 0;
	virtual size_t FS_fread( void *dest, size_t destSize, size_t size ) = 0;
	virtual size_t FS_fwrite( const void *src, size_t size ) = 0;
	virtual bool FS_setmode( FileMode_t mode ) = 0;
	virtual size_t FS_vfprintf( const char *fmt, va_list list ) = 0;
	virtual int FS_ferror() = 0;
	virtual int FS_fflush() = 0;
	virtual char *FS_fgets( char *dest, int destSize ) = 0;
	virtual int FS_GetSectorSize() { return 1; }
};

//---------------------------------------------------------

class CStdioFile : public CStdFilesystemFile
{
public:
	static CStdioFile *FS_fopen( const char *filename, const char *options, int64 *size );

	virtual void FS_setbufsize( unsigned nBytes );
	virtual void FS_fclose();
	virtual void FS_fseek( int64 pos, int seekType );
	virtual long FS_ftell();
	virtual int FS_feof();
	virtual size_t FS_fread( void *dest, size_t destSize, size_t size);
	virtual size_t FS_fwrite( const void *src, size_t size );
	virtual bool FS_setmode( FileMode_t mode );
	virtual size_t FS_vfprintf( const char *fmt, va_list list );
	virtual int FS_ferror();
	virtual int FS_fflush();
	virtual char *FS_fgets( char *dest, int destSize );

private:
	CStdioFile( FILE *pFile )
		: m_pFile( pFile )
	{
	}

	FILE *m_pFile;
};

//-----------------------------------------------------------------------------

#ifdef _WIN32
class CWin32ReadOnlyFile : public CStdFilesystemFile
{
public:
	static bool CanOpen( const char *filename, const char *options );
	static CWin32ReadOnlyFile *FS_fopen( const char *filename, const char *options, int64 *size );

	virtual void FS_setbufsize( unsigned nBytes ) {}
	virtual void FS_fclose();
	virtual void FS_fseek( int64 pos, int seekType );
	virtual long FS_ftell();
	virtual int FS_feof();
	virtual size_t FS_fread( void *dest, size_t destSize, size_t size);
	virtual size_t FS_fwrite( const void *src, size_t size ) { return 0; }
	virtual bool FS_setmode( FileMode_t mode ) { Error( "Can't set mode, open a second file in right mode\n" ); return false; }
	virtual size_t FS_vfprintf( const char *fmt, va_list list ) { return 0; }
	virtual int FS_ferror() { return 0;	}
	virtual int FS_fflush() { return 0; }
	virtual char *FS_fgets( char *dest, int destSize );
	virtual int FS_GetSectorSize() { return m_SectorSize; }

private:
	CWin32ReadOnlyFile( HANDLE hFileUnbuffered, HANDLE hFileBuffered, int sectorSize, int64 fileSize, bool bOverlapped )
	 :	m_hFileUnbuffered( hFileUnbuffered ),
		m_hFileBuffered( hFileBuffered ),
		m_ReadPos( 0 ),
		m_Size( fileSize ),
		m_SectorSize( sectorSize ),
		m_bOverlapped( bOverlapped )
	{
	}

	int64				m_ReadPos;
	int64				m_Size;
	HANDLE				m_hFileUnbuffered;
	HANDLE				m_hFileBuffered;
	CThreadFastMutex	m_Mutex;
	int					m_SectorSize;
	bool				m_bOverlapped;
};

#endif

//-----------------------------------------------------------------------------
// singleton
//-----------------------------------------------------------------------------
CFileSystem_Stdio g_FileSystem_Stdio;
#if defined(_WIN32) && defined(DEDICATED)
CBaseFileSystem *BaseFileSystem_Stdio( void )
{
	return &g_FileSystem_Stdio;
}
#endif
 
#ifdef DEDICATED // "hack" to allow us to not export a stdio version of the FILESYSTEM_INTERFACE_VERSION anywhere

IFileSystem *g_pFileSystem = &g_FileSystem_Stdio;
IBaseFileSystem *g_pBaseFileSystem = &g_FileSystem_Stdio;

#else

EXPOSE_SINGLE_INTERFACE_GLOBALVAR( CFileSystem_Stdio, IFileSystem, FILESYSTEM_INTERFACE_VERSION, g_FileSystem_Stdio );
EXPOSE_SINGLE_INTERFACE_GLOBALVAR( CFileSystem_Stdio, IBaseFileSystem, BASEFILESYSTEM_INTERFACE_VERSION, g_FileSystem_Stdio );

#endif

//-----------------------------------------------------------------------------

#ifndef _RETAIL
bool UseOptimalBufferAllocation()
{
	static bool bUseOptimalBufferAllocation = ( IsX360() || ( !IsLinux() && Q_stristr( Plat_GetCommandLine(), "-unbuffered_io" ) != NULL ) );
	return bUseOptimalBufferAllocation;
}
ConVar filesystem_unbuffered_io( "filesystem_unbuffered_io", "1", 0, "" );
#define UseUnbufferedIO() ( UseOptimalBufferAllocation() && filesystem_unbuffered_io.GetBool() )
#else
#define UseUnbufferedIO() true
#endif

ConVar filesystem_native( "filesystem_native", "1", 0, "Use native FS or STDIO" );
ConVar filesystem_max_stdio_read( "filesystem_max_stdio_read", IsX360() ? "64" : "16", 0, "" );
ConVar filesystem_report_buffered_io( "filesystem_report_buffered_io", "0" );

//-----------------------------------------------------------------------------
// constructor
//-----------------------------------------------------------------------------
CFileSystem_Stdio::CFileSystem_Stdio()
{
	m_bMounted = false;
	m_bCanAsync = true;
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CFileSystem_Stdio::~CFileSystem_Stdio()
{
	Assert(!m_bMounted);
}


//-----------------------------------------------------------------------------
// QueryInterface: 
//-----------------------------------------------------------------------------
void *CFileSystem_Stdio::QueryInterface( const char *pInterfaceName )
{
	// We also implement the IMatSystemSurface interface
	if (!Q_strncmp(	pInterfaceName, FILESYSTEM_INTERFACE_VERSION, Q_strlen(FILESYSTEM_INTERFACE_VERSION) + 1))
		return (IFileSystem*)this;

	return CBaseFileSystem::QueryInterface( pInterfaceName );
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
bool CFileSystem_Stdio::GetOptimalIOConstraints( FileHandle_t hFile, unsigned *pOffsetAlign, unsigned *pSizeAlign, unsigned *pBufferAlign )
{
	unsigned sectorSize;
	
	if ( hFile && UseOptimalBufferAllocation() )
	{
		CFileHandle *fh = ( CFileHandle *)hFile;
		sectorSize = fh->GetSectorSize();

		if ( !sectorSize || ( fh->m_pPackFileHandle && ( fh->m_pPackFileHandle->AbsoluteBaseOffset() % sectorSize ) ) )
		{
			sectorSize = 1;
		}
	}
	else
	{
		sectorSize = 1;
	}

	if ( pOffsetAlign )
	{
		*pOffsetAlign = sectorSize;
	}

	if ( pSizeAlign )
	{
		*pSizeAlign = sectorSize;
	}

	if ( pBufferAlign )
	{
		if ( IsX360() )
		{
			*pBufferAlign = 4;
		}
		else
		{
			*pBufferAlign = sectorSize;
		}
	}

	return ( sectorSize > 1 );
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void *CFileSystem_Stdio::AllocOptimalReadBuffer( FileHandle_t hFile, unsigned nSize, unsigned nOffset )
{
	if ( !UseOptimalBufferAllocation() )
	{
		return CBaseFileSystem::AllocOptimalReadBuffer( hFile, nSize, nOffset );
	}

	unsigned sectorSize;
	if ( hFile != FILESYSTEM_INVALID_HANDLE )
	{
		CFileHandle *fh = ( CFileHandle *)hFile;
		sectorSize = fh->GetSectorSize();

		if ( !nSize )
		{
			nSize = fh->Size();
		}

		if ( fh->m_pPackFileHandle )
		{
			nOffset += fh->m_pPackFileHandle->AbsoluteBaseOffset();
		}
	}
	else
	{
		// an invalid handle gets a fake "optimal" but valid buffer
		// this path is for a caller that isn't doing i/o, 
		// but needs an "optimal" buffer that can end up passed to FreeOptimalReadBuffer()
		sectorSize = 4;
	}

	bool bOffsetIsAligned = ( nOffset % sectorSize == 0 );
	unsigned nAllocSize = ( bOffsetIsAligned ) ? AlignValue( nSize, sectorSize ) : nSize;

	if ( IsX360() )
	{
		return malloc( nAllocSize );
	}
	else
	{
		unsigned nAllocAlignment = ( bOffsetIsAligned ) ? sectorSize : 4;
		return _aligned_malloc( nAllocSize, nAllocAlignment );
	}
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CFileSystem_Stdio::FreeOptimalReadBuffer( void *p )
{
	if ( !UseOptimalBufferAllocation() )
	{
		CBaseFileSystem::FreeOptimalReadBuffer( p );
		return;
	}

	if ( p )
	{
		if ( IsX360() )
		{
			free( p );
		}
		else
		{
			 _aligned_free( p );
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
FILE *CFileSystem_Stdio::FS_fopen( const char *filename, const char *options, unsigned flags, int64 *size, CFileLoadInfo *pInfo )
{
	CStdFilesystemFile *pFile = NULL;

	if ( pInfo )
		pInfo->m_bLoadedFromSteamCache = false;


#ifdef _WIN32
	if ( CWin32ReadOnlyFile::CanOpen( filename, options ) )
	{
		pFile = CWin32ReadOnlyFile::FS_fopen( filename, options, size );
		if ( pFile )
		{
			return (FILE *)pFile;
		}
	}
#endif

	pFile = CStdioFile::FS_fopen( filename, options, size );

	return (FILE *)pFile;
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
void CFileSystem_Stdio::FS_setbufsize( FILE *fp, unsigned nBytes )
{
	CStdFilesystemFile *pFile = ((CStdFilesystemFile *)fp);
	pFile->FS_setbufsize( nBytes );
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
void CFileSystem_Stdio::FS_fclose( FILE *fp )
{
	CStdFilesystemFile *pFile = ((CStdFilesystemFile *)fp);
	pFile->FS_fclose();
	delete pFile;
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
void CFileSystem_Stdio::FS_fseek( FILE *fp, int64 pos, int seekType )
{
	CStdFilesystemFile *pFile = ((CStdFilesystemFile *)fp);
	pFile->FS_fseek( pos, seekType );
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
long CFileSystem_Stdio::FS_ftell( FILE *fp )
{
	CStdFilesystemFile *pFile = ((CStdFilesystemFile *)fp);
	return pFile->FS_ftell();
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
int CFileSystem_Stdio::FS_feof( FILE *fp )
{
	CStdFilesystemFile *pFile = ((CStdFilesystemFile *)fp);
	return pFile->FS_feof();
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
size_t CFileSystem_Stdio::FS_fread( void *dest, size_t destSize, size_t size, FILE *fp )
{
	CStdFilesystemFile *pFile = ((CStdFilesystemFile *)fp);
	size_t nBytesRead = pFile->FS_fread( dest, destSize, size);

	Trace_FRead( nBytesRead, fp );

	return nBytesRead;
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
size_t CFileSystem_Stdio::FS_fwrite( const void *src, size_t size, FILE *fp )
{
	CStdFilesystemFile *pFile = ((CStdFilesystemFile *)fp);

	size_t nBytesWritten = pFile->FS_fwrite(src, size);

	return nBytesWritten;
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
bool CFileSystem_Stdio::FS_setmode( FILE *fp, FileMode_t mode )
{
	CStdFilesystemFile *pFile = ((CStdFilesystemFile *)fp);
	return pFile->FS_setmode( mode );
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
size_t CFileSystem_Stdio::FS_vfprintf( FILE *fp, const char *fmt, va_list list )
{
	CStdFilesystemFile *pFile = ((CStdFilesystemFile *)fp);
	return pFile->FS_vfprintf(fmt, list);
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
int CFileSystem_Stdio::FS_ferror( FILE *fp )
{
	CStdFilesystemFile *pFile = ((CStdFilesystemFile *)fp);
	return pFile->FS_ferror();
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
int CFileSystem_Stdio::FS_fflush( FILE *fp )
{
	CStdFilesystemFile *pFile = ((CStdFilesystemFile *)fp);
	return pFile->FS_fflush();
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
char *CFileSystem_Stdio::FS_fgets( char *dest, int destSize, FILE *fp )
{
	CStdFilesystemFile *pFile = ((CStdFilesystemFile *)fp);
	return pFile->FS_fgets(dest, destSize);
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *path - 
//			pmode - 
// Output : int
//-----------------------------------------------------------------------------
int CFileSystem_Stdio::FS_chmod( const char *path, int pmode )
{
	if ( !path )
		return -1;

	int rt = _chmod( path, pmode );
#if !defined(_WIN32)
	if (rt==-1)
	{
		const char *file =findFileInDirCaseInsensitive(path);
		if (file)
		{
			rt=_chmod(file,pmode);
		}
	}	
#endif
	return rt;
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
int CFileSystem_Stdio::FS_stat( const char *path, struct _stat *buf )
{
	if ( !path )
	{
		return -1;
	}

	int rt = _stat( path, buf );
#if !defined( _WIN32 )
	if ( rt == -1 )
	{
		const char *file = findFileInDirCaseInsensitive( path );
		if ( file )
		{
			rt = _stat( file, buf );
		}
	}	
#endif
	return rt;
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
HANDLE CFileSystem_Stdio::FS_FindFirstFile(const char *findname, WIN32_FIND_DATA *dat)
{
	return ::FindFirstFile(findname, dat);
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
bool CFileSystem_Stdio::FS_FindNextFile(HANDLE handle, WIN32_FIND_DATA *dat)
{
	return (::FindNextFile(handle, dat) != 0);
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
bool CFileSystem_Stdio::FS_FindClose(HANDLE handle)
{
	return (::FindClose(handle) != 0);
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
int CFileSystem_Stdio::FS_GetSectorSize( FILE *fp )
{
	CStdFilesystemFile *pFile = ((CStdFilesystemFile *)fp);
	return pFile->FS_GetSectorSize();
}

//-----------------------------------------------------------------------------
// Purpose: files are always immediately available on disk
//-----------------------------------------------------------------------------
bool CFileSystem_Stdio::IsFileImmediatelyAvailable(const char *pFileName)
{
	return true;
}

// enable this if you want the stdio filesystem to pretend it's steam, and make people wait for resources
//#define DEBUG_WAIT_FOR_RESOURCES_API

#if defined(DEBUG_WAIT_FOR_RESOURCES_API)
static float g_flDebugProgress = 0.0f;
#endif

//-----------------------------------------------------------------------------
// Purpose: steam call, unnecessary in stdio
//-----------------------------------------------------------------------------
WaitForResourcesHandle_t CFileSystem_Stdio::WaitForResources( const char *resourcelist )
{
#if defined(DEBUG_WAIT_FOR_RESOURCES_API)
	g_flDebugProgress = 0.0f;
#endif

	return 1;
}

//-----------------------------------------------------------------------------
// Purpose: steam call, unnecessary in stdio
//-----------------------------------------------------------------------------
bool CFileSystem_Stdio::GetWaitForResourcesProgress( WaitForResourcesHandle_t handle, float *progress /* out */ , bool *complete /* out */ )
{
#if defined(DEBUG_WAIT_FOR_RESOURCES_API)
	g_flDebugProgress += 0.002f;
	if (g_flDebugProgress < 1.0f)
	{
		*progress = g_flDebugProgress;
		*complete = false;
		return true;
	}
#endif

	// always return that we're complete
	*progress = 0.0f;
	*complete = true;
	return true;
}

//-----------------------------------------------------------------------------
// Purpose: steam call, unnecessary in stdio
//-----------------------------------------------------------------------------
void CFileSystem_Stdio::CancelWaitForResources( WaitForResourcesHandle_t handle )
{
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CFileSystem_Stdio::GetLocalCopy( const char *pFileName )
{
	// do nothing. . everything is local.
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
int CFileSystem_Stdio::HintResourceNeed( const char *hintlist, int forgetEverything )
{
	// do nothing. . everything is local.
	return 0;
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
CStdioFile *CStdioFile::FS_fopen( const char *filename, const char *options, int64 *size )
{
	FILE *pFile = NULL;

	// stop newline characters at end of filename
	Assert(!strchr(filename, '\n') && !strchr(filename, '\r'));
	
	pFile = fopen(filename, options);
	if (pFile && size)
	{
		// todo: replace with filelength()? 
		struct _stat buf;
		int rt = _stat( filename, &buf );
		if (rt == 0)
		{
			*size = buf.st_size;
		}
	}

#if !defined _WIN32
	if(!pFile && !strchr(options,'w') && !strchr(options,'+') ) // try opening the lower cased version
	{
		const char *file =findFileInDirCaseInsensitive(filename);
		if ( file )
		{	
			pFile = fopen( file, options );

			if (pFile && size)
			{
				// todo: replace with filelength()? 
				struct _stat buf;
				int rt = _stat( file, &buf );
				if (rt == 0)
				{
					*size = buf.st_size;
				}
			}
		}
	}
#endif

	if ( pFile )
	{
		return new CStdioFile( pFile );
	}

	return NULL;
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
void CStdioFile::FS_setbufsize( unsigned nBytes )
{
#ifdef _WIN32
	if ( nBytes )
	{
		setvbuf( m_pFile, NULL, _IOFBF,  32768 );
	}
	else
	{
		setvbuf( m_pFile, NULL, _IONBF,  0 );
		// hack to make microsoft stdio not always read one stray byte on odd sized files
		m_pFile->_bufsiz = 1;
	}
#endif
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
void CStdioFile::FS_fclose()
{
	fclose(m_pFile);
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
void CStdioFile::FS_fseek( int64 pos, int seekType )
{
	fseek( m_pFile, pos, seekType );
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
long CStdioFile::FS_ftell()
{
	return ftell(m_pFile);
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
int CStdioFile::FS_feof()
{
	return feof(m_pFile);
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
size_t CStdioFile::FS_fread( void *dest, size_t destSize, size_t size )
{
	// read (size) of bytes to ensure truncated reads returns bytes read and not 0
	return fread( dest, 1, size, m_pFile );
}


#define WRITE_CHUNK		(256 * 1024)

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//
// This routine breaks data into chunks if the amount to be written is beyond WRITE_CHUNK (256kb)
// Windows can fail on monolithic writes of ~12MB or more, so we work around that here
//-----------------------------------------------------------------------------
size_t CStdioFile::FS_fwrite( const void *src, size_t size )
{
	if ( size > WRITE_CHUNK )
	{
		size_t remaining = size;
		const byte* current = (const byte *) src;
		size_t total = 0;

		while ( remaining > 0 )
		{
			size_t bytesToCopy = min(remaining, WRITE_CHUNK);

			total += fwrite(current, 1, bytesToCopy, m_pFile);

			remaining -= bytesToCopy;
			current += bytesToCopy;
		}

		Assert( total == size );
		return total;
	}

	return fwrite(src, 1, size, m_pFile);// return number of bytes written (because we have size = 1, count = bytes, so it return bytes)
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
bool CStdioFile::FS_setmode( FileMode_t mode )
{
#ifdef _WIN32
	int fd = _fileno( m_pFile );
	int newMode = ( mode == FM_BINARY ) ? _O_BINARY : _O_TEXT;
	return ( _setmode( fd, newMode) != -1 );
#else
	return false;
#endif
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
size_t CStdioFile::FS_vfprintf( const char *fmt, va_list list )
{
	return vfprintf(m_pFile, fmt, list);
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
int CStdioFile::FS_ferror()
{
	return ferror(m_pFile);
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
int CStdioFile::FS_fflush()
{
	return fflush(m_pFile);
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
char *CStdioFile::FS_fgets( char *dest, int destSize )
{
	return fgets(dest, destSize, m_pFile);
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
#ifdef _WIN32

ConVar filesystem_use_overlapped_io( "filesystem_use_overlapped_io", "1", 0, "" );
#define UseOverlappedIO() filesystem_use_overlapped_io.GetBool()

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
int GetSectorSize( const char *pszFilename )
{
	if ( ( !pszFilename[0] || !pszFilename[1] ) ||
		 ( pszFilename[0] == '\\' && pszFilename[1] == '\\' ) ||
		 ( pszFilename[0] == '/' && pszFilename[1] == '/' ) )
	{
		// Cannot determine sector size with a UNC path (need volume identifier)
		return 0;
	}

	if ( IsX360() )
	{
		// purposely dvd centric, which is also the worst case
		return XBOX_DVD_SECTORSIZE;
	}

#if defined( _WIN32 ) && !defined( FILESYSTEM_STEAM ) && !defined( _X360 )
	char szAbsoluteFilename[MAX_FILEPATH];
	if ( pszFilename[1] != ':' )
	{
		Q_MakeAbsolutePath( szAbsoluteFilename, sizeof(szAbsoluteFilename), pszFilename );
		pszFilename = szAbsoluteFilename;
	}

	DWORD sectorSize = 1;

	struct DriveSectorSize_t
	{
		char volume;
		DWORD sectorSize;
	};

	static DriveSectorSize_t cachedSizes[4];

	char volume = tolower( *pszFilename );

	int i;
	for ( i = 0; i < ARRAYSIZE(cachedSizes) && cachedSizes[i].volume; i++ )
	{
		if ( cachedSizes[i].volume == volume )
		{
			sectorSize = cachedSizes[i].sectorSize;
			break;
		}
	}

	if ( sectorSize == 1 )
	{
		char root[4] = "X:\\";
		root[0] = *pszFilename;

		DWORD ignored;
		if ( !GetDiskFreeSpace( root, &ignored, &sectorSize, &ignored, &ignored ) )
		{
			sectorSize = 0;
		}

		if ( i < ARRAYSIZE(cachedSizes) )
		{
			cachedSizes[i].volume = volume;
			cachedSizes[i].sectorSize = sectorSize;
		}
	}

	return sectorSize;
#else
	return 0;
#endif
}


//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------

class CThreadIOEventPool
{
public:
	~CThreadIOEventPool()
	{
		CThreadEvent *pEvent;

		while ( m_Events.PopItem( &pEvent ) )
		{
			delete pEvent;
		}
	}

	CThreadEvent *GetEvent()
	{
		CThreadEvent *pEvent;

		if ( m_Events.PopItem( &pEvent ) )
		{
			return pEvent;
		}

		return new CThreadEvent;
	}

	void ReleaseEvent( CThreadEvent *pEvent )
	{
		m_Events.PushItem( pEvent );
	}

private:
	CTSList<CThreadEvent *> m_Events;
};


CThreadIOEventPool g_ThreadIOEvents;


//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
bool CWin32ReadOnlyFile::CanOpen( const char *filename, const char *options )
{
	return ( options[0] == 'r' && options[1] == 'b' && options[2] == 0 && filesystem_native.GetBool() );
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------

static HANDLE OpenWin32File( const char *filename, bool bOverlapped, bool bUnbuffered, int64 *pFileSize )
{
	HANDLE hFile;

	DWORD createFlags = FILE_ATTRIBUTE_NORMAL;
		
	if ( bOverlapped )
	{
		createFlags |= FILE_FLAG_OVERLAPPED;
	}

	if ( bUnbuffered )
	{
		createFlags |= FILE_FLAG_NO_BUFFERING;
	}

	hFile = ::CreateFile( filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, createFlags, NULL );
	if ( hFile != INVALID_HANDLE_VALUE && !*pFileSize )
	{
		LARGE_INTEGER fileSize;
		if ( !GetFileSizeEx( hFile, &fileSize ) )
		{
			CloseHandle( hFile );
			hFile = INVALID_HANDLE_VALUE;
		}
		*pFileSize = fileSize.QuadPart;
	}
	return hFile;
}

CWin32ReadOnlyFile *CWin32ReadOnlyFile::FS_fopen( const char *filename, const char *options, int64 *size )
{
	Assert( CanOpen( filename, options ) );

	int sectorSize = 0;
	bool bTryUnbuffered = ( UseUnbufferedIO() && ( sectorSize = GetSectorSize( filename ) ) != 0 );
	bool bOverlapped = UseOverlappedIO();

	HANDLE hFileUnbuffered = INVALID_HANDLE_VALUE;
	int64 fileSize = 0;

	if ( bTryUnbuffered )
	{
		hFileUnbuffered = OpenWin32File( filename, bOverlapped, true, &fileSize );
		if ( hFileUnbuffered == INVALID_HANDLE_VALUE )
		{
			return NULL;
		}
	}

	HANDLE hFileBuffered = OpenWin32File( filename, bOverlapped, false, &fileSize );
	if ( hFileBuffered == INVALID_HANDLE_VALUE )
	{
		if ( hFileUnbuffered != INVALID_HANDLE_VALUE )
		{
			CloseHandle( hFileUnbuffered );
		}
		return NULL;
	}

	if ( size )
	{
		*size = fileSize;
	}

	return new CWin32ReadOnlyFile( hFileUnbuffered, hFileBuffered, ( sectorSize ) ? sectorSize : 1, fileSize, bOverlapped );
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
void CWin32ReadOnlyFile::FS_fclose()
{
	if ( m_hFileUnbuffered != INVALID_HANDLE_VALUE )
	{
		CloseHandle( m_hFileUnbuffered );
	}

	if ( m_hFileBuffered != INVALID_HANDLE_VALUE )
	{
		CloseHandle( m_hFileBuffered );
	}
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
void CWin32ReadOnlyFile::FS_fseek( int64 pos, int seekType )
{
	switch ( seekType )
	{
	case SEEK_SET:
		m_ReadPos = pos;
		break;

	case SEEK_CUR:
		m_ReadPos += pos;
		break;

	case SEEK_END:
		m_ReadPos = m_Size - pos;
		break;
	}
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
long CWin32ReadOnlyFile::FS_ftell()
{
	return m_ReadPos;	
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
int CWin32ReadOnlyFile::FS_feof()
{
	return ( m_ReadPos >= m_Size );	
}

// ends up on a thread's stack, don't blindly increase without awareness of that implication
// 360 threads have small stacks, using small buffer of the worst case quantum sector size
#if !defined( _X360 )
#define READ_TEMP_BUFFER	( 32*1024 )
#else
#define READ_TEMP_BUFFER	( 2*XBOX_DVD_SECTORSIZE )
#endif

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
size_t CWin32ReadOnlyFile::FS_fread( void *dest, size_t destSize, size_t size )
{
	VPROF_BUDGET( "CWin32ReadOnlyFile::FS_fread", VPROF_BUDGETGROUP_OTHER_FILESYSTEM );

	if ( !size || ( m_hFileUnbuffered == INVALID_HANDLE_VALUE && m_hFileBuffered == INVALID_HANDLE_VALUE ) )
	{
		return 0;
	}

	CThreadEvent *pEvent = NULL;

	if ( destSize == (size_t)-1 )
	{
		destSize = size;
	}

	byte tempBuffer[READ_TEMP_BUFFER];
	HANDLE hReadFile = m_hFileBuffered;
	int nBytesToRead = size;
	byte *pDest = (byte *)dest;
	int64 offset = m_ReadPos;

	if ( m_hFileUnbuffered != INVALID_HANDLE_VALUE )
	{
		const int destBaseAlign = ( IsX360() ) ? 4 : m_SectorSize;
		bool bDestBaseIsAligned = ( (DWORD)dest % destBaseAlign == 0 );
		bool bCanReadUnbufferedDirect = ( bDestBaseIsAligned && ( destSize % m_SectorSize == 0 ) && ( m_ReadPos % m_SectorSize == 0 ) );

		if ( bCanReadUnbufferedDirect )
		{
			// fastest path, unbuffered
			nBytesToRead = AlignValue( size, m_SectorSize );
			hReadFile = m_hFileUnbuffered;
		}
		else
		{
			// not properly aligned, snap to alignments
			// attempt to perform single unbuffered operation using stack buffer
			int64 alignedOffset = AlignValue( ( m_ReadPos - m_SectorSize ) + 1, m_SectorSize );
			unsigned int alignedBytesToRead = AlignValue( ( m_ReadPos - alignedOffset ) + size, m_SectorSize );
			if ( alignedBytesToRead <= sizeof( tempBuffer ) - destBaseAlign )
			{
				// read operation can be performed as unbuffered follwed by a post fixup
				nBytesToRead = alignedBytesToRead;
				offset = alignedOffset;
				pDest = AlignValue( tempBuffer, destBaseAlign );
				hReadFile = m_hFileUnbuffered;
			}
		}
	}

	OVERLAPPED overlapped = { 0 };	
	if ( m_bOverlapped )
	{
		pEvent = g_ThreadIOEvents.GetEvent();
		overlapped.hEvent = *pEvent;
	}

#ifdef REPORT_BUFFERED_IO
	if ( hReadFile == m_hFileBuffered && filesystem_report_buffered_io.GetBool() )
	{
		Msg( "Buffered Operation :(\n" );
	}
#endif

	// some disk drivers will fail if read is too large
	static int MAX_READ = filesystem_max_stdio_read.GetInt()*1024*1024;
	const int MIN_READ = 64*1024;
	bool bReadOk = true;
	DWORD nBytesRead = 0;
	size_t result = 0;
	int64 currentOffset = offset;

	while ( bReadOk && nBytesToRead > 0 )
	{
		int nCurBytesToRead = min( nBytesToRead, MAX_READ );
		DWORD nCurBytesRead = 0;

		overlapped.Offset = currentOffset & 0xFFFFFFFF;
		overlapped.OffsetHigh = ( currentOffset >> 32 ) & 0xFFFFFFFF;

		bReadOk = ( ::ReadFile( hReadFile, pDest + nBytesRead, nCurBytesToRead, &nCurBytesRead, &overlapped ) != 0 );
		if ( !bReadOk )
		{
			if ( m_bOverlapped && GetLastError() == ERROR_IO_PENDING )
			{
				bReadOk = true;
			}
		}

		if ( bReadOk )
		{
			if ( GetOverlappedResult( hReadFile, &overlapped, &nCurBytesRead, true ) )
			{
				nBytesRead += nCurBytesRead;
				nBytesToRead -= nCurBytesToRead;
				currentOffset += nCurBytesRead;
			}
			else
			{
				bReadOk = false;
			}
		}
		 
		if ( !bReadOk )
		{
			DWORD dwError = GetLastError();

			if ( IsX360() )
			{
				if ( dwError == ERROR_DISK_CORRUPT || dwError == ERROR_FILE_CORRUPT )
				{
					FSDirtyDiskReportFunc_t func = g_FileSystem_Stdio.GetDirtyDiskReportFunc();
					if ( func )
					{
						func();
						result = 0;
					}
				}
			}

			if ( dwError == ERROR_NO_SYSTEM_RESOURCES && MAX_READ > MIN_READ )
			{
				MAX_READ /= 2;
				bReadOk = true;
				DevMsg( "ERROR_NO_SYSTEM_RESOURCES: Reducing max read to %d bytes\n", MAX_READ );
			}
			else
			{
				DevMsg( "Unknown read error %d\n", dwError );
			}
		}
	}

	if ( bReadOk )
	{
		if ( nBytesRead && hReadFile == m_hFileUnbuffered && pDest != dest )
		{
			int nBytesExtra = ( m_ReadPos - offset );
			nBytesRead -= nBytesExtra;
			if ( nBytesRead )
			{
				memcpy( dest, (byte *)pDest + nBytesExtra, size );
			}
		}

		result = min( nBytesRead, size );
	}

	if ( m_bOverlapped )
	{
		pEvent->Reset();
		g_ThreadIOEvents.ReleaseEvent( pEvent );
	}

	m_ReadPos += result;

	return result;
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
char *CWin32ReadOnlyFile::FS_fgets( char *dest, int destSize ) 
{  
	if ( FS_feof() )
	{
		return NULL;
	}
	int nStartPos = m_ReadPos;
	int nBytesRead = FS_fread( dest, destSize, destSize );
	if ( !nBytesRead )
	{
		return NULL;
	}

	dest[min( nBytesRead, destSize - 1)] = 0;
	char *pNewline = strchr( dest, '\n' );
	if ( pNewline )
	{
		// advance past, leave \n
		pNewline++;
		*pNewline = 0;
	}
	else
	{
		pNewline = &dest[min( nBytesRead, destSize - 1)];
	}
	m_ReadPos = nStartPos + ( pNewline - dest ) + 1;

	return dest; 
}


#endif
