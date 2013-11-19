//====== Copyright © 1996-2005, Valve Corporation, All rights reserved. =======
//
// Purpose: 
//
//=============================================================================

#include "winlite.h"
#include "mpafile.h"
#include "tier1/UtlRBTree.h"

extern IFileSystem *g_pFullFileSystem;


// exception class
CMPAException::CMPAException(ErrorIDs ErrorID, LPCTSTR szFile, LPCTSTR szFunction, bool bGetLastError ) :
m_ErrorID( ErrorID ), m_bGetLastError( bGetLastError )
{
	m_szFile = _tcsdup(szFile);
	m_szFunction = _tcsdup(szFunction);
}

// copy constructor (necessary for exception throwing without pointers)
CMPAException::CMPAException(const CMPAException& Source)
{
	m_ErrorID = Source.m_ErrorID;
	m_bGetLastError = Source.m_bGetLastError;
	m_szFile = _tcsdup(Source.m_szFile);
	m_szFunction = _tcsdup(Source.m_szFunction);
}

// destructor
CMPAException::~CMPAException()
{
	if( m_szFile )
		free( m_szFile );
	if( m_szFunction )
		free( m_szFunction );
}

// should be in resource file for multi language applications
LPCTSTR m_szErrors[] = 
{
	_T("Can't open the file."), 
	_T("Can't set file position."),
	_T("Can't read from file."),
	_T("Reached end of buffer."),
	_T("No VBR Header found."),
	_T("Incomplete VBR Header."),
	_T("No subsequent frame found within tolerance range."),
	_T("No frame found.")

};

#define MAX_ERR_LENGTH 256
void CMPAException::ShowError()
{
	TCHAR szErrorMsg[MAX_ERR_LENGTH] = {0};
	TCHAR szHelp[MAX_ERR_LENGTH];

	// this is not buffer-overflow-proof!
	if( m_szFunction )
	{
		_stprintf( szHelp, _T("%s: "), m_szFunction );
		_tcscat( szErrorMsg, szHelp );
	}
	if( m_szFile )
	{
		_stprintf( szHelp, _T("'%s'\n"), m_szFile );
		_tcscat( szErrorMsg, szHelp );
	}
	_tcscat( szErrorMsg, m_szErrors[m_ErrorID] );

	if( m_bGetLastError )
	{
		// get error message of last system error id
		LPVOID pMsgBuf;
		if (FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
							NULL,
							GetLastError(),
							MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
							(LPTSTR) &pMsgBuf,
							0,
							NULL ))
		{
			_tcscat( szErrorMsg, _T("\n") );
			_tcscat( szErrorMsg, (LPCTSTR)pMsgBuf );
			LocalFree( pMsgBuf );
		}
	}
	// show error message
	Warning( "%s\n", szErrorMsg );
}

// 1KB is inital buffersize, each time the buffer needs to be increased it is doubled
const DWORD CMPAFile::m_dwInitBufferSize = 1024;	


CMPAFile::CMPAFile( LPCTSTR szFile, DWORD dwFileOffset, FileHandle_t hFile ) :
m_pBuffer(NULL), m_dwBufferSize(0), m_dwBegin( dwFileOffset ), m_dwEnd(0),
m_dwNumTimesRead(0), m_bVBRFile( false ), m_pVBRHeader(NULL), m_bMustReleaseFile( false ),
m_pMPAHeader(NULL), m_hFile( hFile ), m_szFile(NULL), m_dwFrameNo(1)
{
	// open file, if not already done
	if( m_hFile == FILESYSTEM_INVALID_HANDLE )
	{
		Open( szFile );
		m_bMustReleaseFile = true;
	}
	// save filename
	m_szFile = _tcsdup( szFile );
	
	// set end of MPEG data (assume file end)
	if( m_dwEnd <= 0 )
	{
		// get file size
		m_dwEnd = g_pFullFileSystem->Size( m_hFile );
	}

	// find first valid MPEG frame
	m_pMPAHeader = new CMPAHeader( this );

	// is VBR header available?
	CVBRHeader::VBRHeaderType HeaderType = CVBRHeader::NoHeader;
	DWORD dwOffset = m_pMPAHeader->m_dwSyncOffset;
	if( CVBRHeader::IsVBRHeaderAvailable( this, HeaderType, dwOffset ) )
	{
		try
		{
			// read out VBR header
			m_pVBRHeader = new CVBRHeader( this, HeaderType, dwOffset );
			
			m_bVBRFile = true;
			m_dwBytesPerSec = m_pVBRHeader->m_dwBytesPerSec;
			if( m_pVBRHeader->m_dwBytes > 0 )
				m_dwEnd = m_dwBegin + m_pVBRHeader->m_dwBytes;
		}

		catch(CMPAException& Exc)
		{
			Exc.ShowError();
		}	
	}
	
	if( !m_pVBRHeader )
	{
		// always skip empty (32kBit) frames
		m_bVBRFile = m_pMPAHeader->SkipEmptyFrames();
		m_dwBytesPerSec = m_pMPAHeader->GetBytesPerSecond();	
	}
}

bool CMPAFile::GetNextFrame()
{
	DWORD dwOffset = m_pMPAHeader->m_dwSyncOffset + m_pMPAHeader->m_dwRealFrameSize;
	try
	{
		CMPAHeader* pFrame = new CMPAHeader( this, dwOffset, false );
	
		delete m_pMPAHeader;
		m_pMPAHeader = pFrame;
		if( m_dwFrameNo > 0 )
			m_dwFrameNo++;
	}
	catch(...)
	{
		return false;
	}
	return true;
}

bool CMPAFile::GetPrevFrame()
{
	DWORD dwOffset = m_pMPAHeader->m_dwSyncOffset-MPA_HEADER_SIZE;
	try
	{
		// look backward from dwOffset on
		CMPAHeader* pFrame = new CMPAHeader( this, dwOffset, false, true );
	
		delete m_pMPAHeader;
		m_pMPAHeader = pFrame;
		if( m_dwFrameNo > 0 )
			m_dwFrameNo --;
	}
	catch(...)
	{
		return false;
	}
	
	return true;
}

bool CMPAFile::GetFirstFrame()
{
	DWORD dwOffset = 0;
	try
	{
		CMPAHeader* pFrame = new CMPAHeader( this, dwOffset, false );
	
		delete m_pMPAHeader;
		m_pMPAHeader = pFrame;
		m_dwFrameNo = 1;
	}
	catch(...)
	{
		return false;
	}
	return true;
}

bool CMPAFile::GetLastFrame()
{
	DWORD dwOffset = m_dwEnd - m_dwBegin - MPA_HEADER_SIZE;
	try
	{
		// look backward from dwOffset on
		CMPAHeader* pFrame = new CMPAHeader( this, dwOffset, false, true );
	
		delete m_pMPAHeader;
		m_pMPAHeader = pFrame;
		m_dwFrameNo = 0;
	}
	catch(...)
	{
		return false;
	}
	
	return true;
}

// destructor
CMPAFile::~CMPAFile(void)
{
	delete m_pMPAHeader;
	
	if( m_pVBRHeader )
		delete m_pVBRHeader;
	
	if( m_pBuffer )
		delete[] m_pBuffer;
	
	// close file
	if( m_bMustReleaseFile )
		g_pFullFileSystem->Close( m_hFile );	

	if( m_szFile )
		free( (void*)m_szFile );
}

// open file
void CMPAFile::Open( LPCTSTR szFilename )
{
	// open with CreateFile (no limitation of 128byte filename length, like in mmioOpen)
	m_hFile = g_pFullFileSystem->Open( szFilename, "rb", "GAME" );//::CreateFile( szFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	if( m_hFile == FILESYSTEM_INVALID_HANDLE )
	{
		// throw error
		throw CMPAException( CMPAException::ErrOpenFile, szFilename, _T("CreateFile"), true );
	}
}

// set file position
void CMPAFile::SetPosition( int offset )
{
	/*
	LARGE_INTEGER liOff;

	liOff.QuadPart = lOffset;
	liOff.LowPart = ::SetFilePointer(m_hFile, liOff.LowPart, &liOff.HighPart, dwMoveMethod );
	if (liOff.LowPart  == INVALID_SET_FILE_POINTER && GetLastError() != NO_ERROR )
	{
		// throw error
		throw CMPAException( CMPAException::ErrSetPosition, m_szFile, _T("SetFilePointer"), true );
	}
	*/

	g_pFullFileSystem->Seek( m_hFile, offset, FILESYSTEM_SEEK_HEAD );
}

// read from file, return number of bytes read
DWORD CMPAFile::Read( LPVOID pData, DWORD dwSize, DWORD dwOffset )
{
	DWORD dwBytesRead = 0;
	
	// set position first
	SetPosition( m_dwBegin+dwOffset );

	//if( !::ReadFile( m_hFile, pData, dwSize, &dwBytesRead, NULL ) )
	//	throw CMPAException( CMPAException::ErrReadFile, m_szFile, _T("ReadFile"), true );
	dwBytesRead = g_pFullFileSystem->Read( pData, dwSize, m_hFile );

	return dwBytesRead;
}

// convert from big endian to native format (Intel=little endian) and return as DWORD (32bit)
DWORD CMPAFile::ExtractBytes( DWORD& dwOffset, DWORD dwNumBytes, bool bMoveOffset )
{	
	Assert( dwNumBytes > 0 );
	Assert( dwNumBytes <= 4 );	// max 4 byte

	// enough bytes in buffer, otherwise read from file
	if( !m_pBuffer || ( ((int)(m_dwBufferSize - dwOffset)) < (int)dwNumBytes) )
		FillBuffer( dwOffset + dwNumBytes );

	DWORD dwResult = 0;

	// big endian extract (most significant byte first) (will work on little and big-endian computers)
	DWORD dwNumByteShifts = dwNumBytes - 1;

	for( DWORD n=dwOffset; n < dwOffset+dwNumBytes; n++ )
	{
		dwResult |= m_pBuffer[n] << 8*dwNumByteShifts--; // the bit shift will do the correct byte order for you                                                           
	}
	
	if( bMoveOffset )
		dwOffset += dwNumBytes;

	return dwResult;
}

// throws exception if not possible
void CMPAFile::FillBuffer( DWORD dwOffsetToRead )
{
	DWORD dwNewBufferSize;
	
	// calc new buffer size
	if( m_dwBufferSize == 0 )
		dwNewBufferSize = m_dwInitBufferSize;
	else
		dwNewBufferSize = m_dwBufferSize*2;

	// is it big enough?
	if( dwNewBufferSize < dwOffsetToRead )
		dwNewBufferSize = dwOffsetToRead;
	
	// reserve new buffer
	BYTE* pNewBuffer = new BYTE[dwNewBufferSize];
	
	// take over data from old buffer
	if( m_pBuffer )
	{
		memcpy( pNewBuffer, m_pBuffer, m_dwBufferSize );

		// release old buffer 
		delete[] m_pBuffer;
	}
	m_pBuffer = pNewBuffer;
	
	// read <dwNewBufferSize-m_dwBufferSize> bytes from offset <m_dwBufferSize>
	DWORD dwBytesRead = Read( m_pBuffer+m_dwBufferSize, dwNewBufferSize-m_dwBufferSize, m_dwBufferSize );

	// no more bytes in buffer than read out from file
	m_dwBufferSize += dwBytesRead;
}

// Uses mp3 code from:  http://www.codeproject.com/audio/MPEGAudioInfo.asp

struct MP3Duration_t
{
	FileNameHandle_t h;
	float			duration;

	static bool LessFunc( const MP3Duration_t& lhs, const MP3Duration_t& rhs )
	{
		return lhs.h < rhs.h;
	}
};

CUtlRBTree< MP3Duration_t, int >	g_MP3Durations( 0, 0, MP3Duration_t::LessFunc );

float GetMP3Duration_Helper( char const *filename )
{
	float duration = 60.0f;

	// See if it's in the RB tree already...
	char fn[ 512 ];
	Q_snprintf( fn, sizeof( fn ), "sound/%s", filename );

	FileNameHandle_t h = g_pFullFileSystem->FindOrAddFileName( fn );

	MP3Duration_t search;
	search.h = h;
	
	int idx = g_MP3Durations.Find( search );
	if ( idx != g_MP3Durations.InvalidIndex() )
	{
		return g_MP3Durations[ idx ].duration;
	}

	try
	{
		CMPAFile MPAFile( fn, 0 );
		if ( MPAFile.m_dwBytesPerSec != 0 )
		{
			duration = (float)(MPAFile.m_dwEnd - MPAFile.m_dwBegin) / (float)MPAFile.m_dwBytesPerSec;
		}
	}
	catch ( ... )
	{
	}

	search.duration = duration;
	g_MP3Durations.Insert( search );

	return duration;
}
