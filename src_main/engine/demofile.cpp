//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: 
//
//===========================================================================//

#include <tier0/dbg.h>
#include <tier1/strtools.h>
#include <utlbuffer.h>

#include "demofile.h"
#include "filesystem_engine.h"
#include "demo.h"
#include "proto_version.h"

void Host_EndGame (bool bShowMainMenu, const char *message, ...);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDemoFile::CDemoFile() :
#ifdef DEMO_FILE_UTLBUFFER
	m_Buffer()
#else
	m_hDemoFile( FILESYSTEM_INVALID_HANDLE )
#endif
{
}

CDemoFile::~CDemoFile()
{
	if ( IsOpen() )
	{
		Close();
	}
}

void CDemoFile::WriteSequenceInfo(int nSeqNrIn, int nSeqNrOut)
{
#ifdef DEMO_FILE_UTLBUFFER
	Assert( m_Buffer.IsOpen() );
	m_Buffer.PutInt( nSeqNrIn );
	m_Buffer.PutInt( nSeqNrOut );
#else
	Assert( m_hDemoFile != FILESYSTEM_INVALID_HANDLE );
	nSeqNrIn = LittleDWord( nSeqNrIn );
	nSeqNrOut = LittleDWord( nSeqNrOut );

	g_pFileSystem->Write( &nSeqNrIn, sizeof(int), m_hDemoFile );
	g_pFileSystem->Write( &nSeqNrOut, sizeof(int), m_hDemoFile );
#endif
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CDemoFile::ReadSequenceInfo(int &nSeqNrIn, int &nSeqNrOut)
{
#ifdef DEMO_FILE_UTLBUFFER
	Assert( m_Buffer.IsOpen() );
	nSeqNrIn = m_Buffer.GetInt( );
	nSeqNrOut = m_Buffer.GetInt( );
#else
	Assert( m_hDemoFile != FILESYSTEM_INVALID_HANDLE );
	g_pFileSystem->Read( &nSeqNrIn, sizeof(int), m_hDemoFile );
	g_pFileSystem->Read( &nSeqNrOut, sizeof(int), m_hDemoFile );

	nSeqNrIn = LittleDWord( nSeqNrIn );
	nSeqNrOut = LittleDWord( nSeqNrOut );
#endif
}


inline void ByteSwap_democmdinfo_t( democmdinfo_t &swap )
{
	swap.flags = LittleDWord( swap.flags );

	LittleFloat( &swap.viewOrigin.x, &swap.viewOrigin.x );
	LittleFloat( &swap.viewOrigin.y, &swap.viewOrigin.y );
	LittleFloat( &swap.viewOrigin.z, &swap.viewOrigin.z );

	LittleFloat( &swap.viewAngles.x, &swap.viewAngles.x );
	LittleFloat( &swap.viewAngles.y, &swap.viewAngles.y );
	LittleFloat( &swap.viewAngles.z, &swap.viewAngles.z );

	LittleFloat( &swap.localViewAngles.x, &swap.localViewAngles.x );
	LittleFloat( &swap.localViewAngles.y, &swap.localViewAngles.y );
	LittleFloat( &swap.localViewAngles.z, &swap.localViewAngles.z );

	LittleFloat( &swap.viewOrigin2.x, &swap.viewOrigin2.x );
	LittleFloat( &swap.viewOrigin2.y, &swap.viewOrigin2.y );
	LittleFloat( &swap.viewOrigin2.z, &swap.viewOrigin2.z );

	LittleFloat( &swap.viewAngles2.x, &swap.viewAngles2.x );
	LittleFloat( &swap.viewAngles2.y, &swap.viewAngles2.y );
	LittleFloat( &swap.viewAngles2.z, &swap.viewAngles2.z );

	LittleFloat( &swap.localViewAngles2.x, &swap.localViewAngles2.x );
	LittleFloat( &swap.localViewAngles2.y, &swap.localViewAngles2.y );
	LittleFloat( &swap.localViewAngles2.z, &swap.localViewAngles2.z );
}

void CDemoFile::WriteCmdInfo( democmdinfo_t& info )
{
	democmdinfo_t littleEndianInfo = info;
	ByteSwap_democmdinfo_t( littleEndianInfo );

#ifdef DEMO_FILE_UTLBUFFER
	Assert( m_Buffer.IsOpen() );
	m_Buffer.Put( &littleEndianInfo, sizeof(democmdinfo_t) );
#else
	Assert( m_hDemoFile != FILESYSTEM_INVALID_HANDLE );
	g_pFileSystem->Write( &littleEndianInfo, sizeof( democmdinfo_t ), m_hDemoFile );
#endif
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CDemoFile::ReadCmdInfo( democmdinfo_t& info )
{
#ifdef DEMO_FILE_UTLBUFFER
	Assert( m_Buffer.IsOpen() );
	m_Buffer.Get( &info, sizeof(democmdinfo_t) );
#else
	Assert( m_hDemoFile != FILESYSTEM_INVALID_HANDLE );
	g_pFileSystem->Read( &info, sizeof( democmdinfo_t ), m_hDemoFile );
#endif

	ByteSwap_democmdinfo_t( info );
}


//-----------------------------------------------------------------------------
// Purpose: 
// Input  : cmd - 
//			*fp - 
//-----------------------------------------------------------------------------
void CDemoFile::WriteCmdHeader( unsigned char cmd, int tick )
{
	Assert( cmd >= dem_signon && cmd <= dem_lastcmd );

#ifdef DEMO_FILE_UTLBUFFER
	Assert( m_Buffer.IsOpen() );
	m_Buffer.PutUnsignedChar( cmd );
	m_Buffer.PutInt( tick );
#else
	Assert( m_hDemoFile != FILESYSTEM_INVALID_HANDLE );
	tick = LittleDWord( tick );
		
	// Command
	g_pFileSystem->Write( &cmd, sizeof(byte), m_hDemoFile );
	g_pFileSystem->Write (&tick, sizeof(int), m_hDemoFile );
#endif

	/*char *cmdname[] = 
	{
		"dem_unknown",
		"dem_signon",
		"dem_packet",
		"dem_synctick",
		"dem_consolecmd",
		"dem_usercmd",
		"dem_datatables",
		"dem_stop"
	};

	DevMsg( "Demo Write: tick %i, cmd %s \n", tick, cmdname[cmd] );*/
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : cmd - 
//			dt - 
//			frame - 
//-----------------------------------------------------------------------------
void CDemoFile::ReadCmdHeader( unsigned char& cmd, int& tick )
{
#ifdef DEMO_FILE_UTLBUFFER
	Assert( m_Buffer.IsOpen() );
	cmd = m_Buffer.GetUnsignedChar( );
	if ( !m_Buffer.IsValid() )
	{
		ConDMsg("Missing end tag in demo file.\n");
		cmd = dem_stop;
		return;
	}

	if ( cmd <= 0 || cmd > dem_lastcmd )
	{
		ConDMsg("Unexepcted command token [%d] in .demo file\n", cmd );
		cmd = dem_stop;
		return;
	}

	tick = m_Buffer.GetInt( );
#else
	// Read the command
	Assert( m_hDemoFile != FILESYSTEM_INVALID_HANDLE );
	int r = g_pFileSystem->Read( &cmd, sizeof(byte), m_hDemoFile );
	if ( r <= 0 )
	{
		ConDMsg("Missing end tag in demo file.\n");
		cmd = dem_stop;
		return;
	}
	// Read the timestamp
	g_pFileSystem->Read( &tick, sizeof(int), m_hDemoFile );
	tick = LittleDWord( tick );
#endif
}

void CDemoFile::WriteConsoleCommand( const char *cmdstring, int tick )
{
	if ( !cmdstring || !cmdstring[0] )
		return;

#ifdef DEMO_FILE_UTLBUFFER
	if ( !m_Buffer.IsOpen() )
		return;
#else
	if ( m_hDemoFile == FILESYSTEM_INVALID_HANDLE )
		return;
#endif

	int len = Q_strlen( cmdstring ) + 1;
	if ( len >= 1024 )
	{
		DevMsg("CDemoFile::WriteConsoleCommand: command too long (>1024).\n");
		return;
	}

	WriteCmdHeader( dem_consolecmd, tick );

	WriteRawData( cmdstring, len );
}

const char *CDemoFile::ReadConsoleCommand()
{
	static char cmdstring[1024];
	
	ReadRawData( cmdstring, sizeof(cmdstring) );

	return cmdstring;
}

unsigned int CDemoFile::GetCurPos( bool bRead )
{
#ifdef DEMO_FILE_UTLBUFFER
	if ( !m_Buffer.IsOpen() )
		return 0;
	if ( bRead )
		return m_Buffer.TellGet();
	return m_Buffer.TellPut();
#else
	if ( m_hDemoFile == FILESYSTEM_INVALID_HANDLE )
		return 0;
	return g_pFileSystem->Tell( m_hDemoFile );
#endif
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : buf - 
//-----------------------------------------------------------------------------
void CDemoFile::WriteNetworkDataTables( bf_write *buf, int tick  )
{
	MEM_ALLOC_CREDIT();

#ifdef DEMO_FILE_UTLBUFFER
	if ( !m_Buffer.IsOpen() )
	{
		DevMsg("CDemoFile::WriteNetworkDataTables: Haven't opened file yet!\n" );
		return;
	}
#else
	if ( m_hDemoFile == FILESYSTEM_INVALID_HANDLE )
	{
		DevMsg("CDemoFile::WriteNetworkDataTables: file header handle == NULL.\n" );
		return;
	}
#endif

	WriteCmdHeader( dem_datatables, tick );

	WriteRawData( (char*)buf->GetBasePointer(), buf->GetNumBytesWritten() );
}


//-----------------------------------------------------------------------------
// Purpose: 
// Input  : expected_length - 
//			&demofile - 
//-----------------------------------------------------------------------------
int CDemoFile::ReadNetworkDataTables( bf_read *buf )
{
	if ( buf )
		return ReadRawData( (char*)buf->GetBasePointer(), buf->GetNumBytesLeft() );
	return ReadRawData( NULL, 0 ); // skip data
}


//-----------------------------------------------------------------------------
// Purpose: 
// Input  : cmdnumber - 
//-----------------------------------------------------------------------------
void CDemoFile::WriteUserCmd( int cmdnumber, const char *buffer, unsigned char bytes, int tick )
{
#ifdef DEMO_FILE_UTLBUFFER
	if ( !m_Buffer.IsOpen() )
		return;
#else
	if ( m_hDemoFile == FILESYSTEM_INVALID_HANDLE )
		return;
#endif

	WriteCmdHeader( dem_usercmd, tick );

#ifdef DEMO_FILE_UTLBUFFER
	m_Buffer.PutInt( cmdnumber );
#else
	cmdnumber = LittleDWord( cmdnumber );
	g_pFileSystem->Write( &cmdnumber, sizeof( int ), m_hDemoFile );
#endif

	WriteRawData( buffer, bytes );
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : discard - 
//-----------------------------------------------------------------------------
int CDemoFile::ReadUserCmd( char *buffer, int &size )
{
	int outgoing_sequence;
	
#ifdef DEMO_FILE_UTLBUFFER
	Assert( m_Buffer.IsOpen() );
	outgoing_sequence = m_Buffer.GetInt();
#else
	Assert( m_hDemoFile != FILESYSTEM_INVALID_HANDLE );
	g_pFileSystem->Read( &outgoing_sequence, sizeof( int ), m_hDemoFile );
	outgoing_sequence = LittleDWord( outgoing_sequence );
#endif

	size = ReadRawData( buffer, size );
	return outgoing_sequence;
}

//-----------------------------------------------------------------------------
// Purpose: Rewind from the current spot by the time stamp, byte code and frame counter offsets
//-----------------------------------------------------------------------------
void CDemoFile::SeekTo( int position, bool bRead )
{
#ifdef DEMO_FILE_UTLBUFFER
	Assert( m_Buffer.IsOpen() );
	if ( bRead )
	{
		m_Buffer.SeekGet( CUtlBuffer::SEEK_HEAD, position );
	}
	else
	{
		m_Buffer.SeekPut( CUtlBuffer::SEEK_HEAD, position );
	}
#else
	Assert( m_hDemoFile != FILESYSTEM_INVALID_HANDLE );
	g_pFileSystem->Seek( m_hDemoFile, position, FILESYSTEM_SEEK_HEAD );
#endif
}

//-----------------------------------------------------------------------------
// Purpose: 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
int CDemoFile::ReadRawData( char *buffer, int length )
{
	int size;
	
#ifdef DEMO_FILE_UTLBUFFER
	Assert( m_Buffer.IsOpen() );
	size = m_Buffer.GetInt();
#else
	Assert( m_hDemoFile != FILESYSTEM_INVALID_HANDLE );
	// read length of data block
	g_pFileSystem->Read( &size, sizeof( int ), m_hDemoFile );
	size = LittleDWord( size );
#endif

	if ( buffer && (length < size) )
	{
		DevMsg("CDemoFile::ReadRawData: buffe overflow (%i).\n", size );
		return -1;
	}

	if ( !buffer )
	{
		// just skip it
#ifdef DEMO_FILE_UTLBUFFER
		m_Buffer.SeekGet( CUtlBuffer::SEEK_CURRENT, size );
#else
		g_pFileSystem->Seek( m_hDemoFile, size, FILESYSTEM_SEEK_CURRENT );
#endif
		return size;
	}

	if ( length < size )
	{
		// given buffer is too small
		DevMsg("CDemoFile::ReadRawData: buffer overflow (%i).\n", size );
#ifdef DEMO_FILE_UTLBUFFER
		m_Buffer.SeekGet( CUtlBuffer::SEEK_CURRENT, size );
#else
		g_pFileSystem->Seek( m_hDemoFile, size, FILESYSTEM_SEEK_CURRENT );
#endif
		return -1;
	}

	// read data into buffer
#ifdef DEMO_FILE_UTLBUFFER
	m_Buffer.Get( buffer, size );
	if ( !m_Buffer.IsValid() )
	{
		Host_EndGame(true, "Error reading demo message data.\n");
		return -1;
	}
#else
	int r = g_pFileSystem->Read( buffer, size, m_hDemoFile );
	if ( r != size )
	{
		Host_EndGame(true, "Error reading demo message data.\n");
		return -1;
	}
#endif

	return size;
}

void CDemoFile::WriteRawData( const char *buffer, int length )
{
	MEM_ALLOC_CREDIT();

#ifdef DEMO_FILE_UTLBUFFER
	Assert( m_Buffer.IsOpen() );
	m_Buffer.PutInt( length );
	m_Buffer.Put( buffer, length );
#else
	Assert( m_hDemoFile != FILESYSTEM_INVALID_HANDLE );
	int littleEndianLength = LittleDWord( length );
	g_pFileSystem->Write( &littleEndianLength, sizeof( int ), m_hDemoFile );
	g_pFileSystem->Write( buffer, length, m_hDemoFile );
#endif
}

inline void ByteSwap_demoheader_t( demoheader_t &swap )
{
	swap.demoprotocol = LittleDWord( swap.demoprotocol );
	swap.networkprotocol = LittleDWord( swap.networkprotocol );
	LittleFloat( &swap.playback_time, &swap.playback_time );
	swap.playback_ticks = LittleDWord( swap.playback_ticks );
	swap.playback_frames = LittleDWord( swap.playback_frames );
	swap.signonlength = LittleDWord( swap.signonlength );
}

void CDemoFile::WriteDemoHeader()
{
	demoheader_t littleEndianHeader = m_DemoHeader;
	ByteSwap_demoheader_t( littleEndianHeader );

	// goto file start
#ifdef DEMO_FILE_UTLBUFFER
	m_Buffer.SeekPut( CUtlBuffer::SEEK_HEAD, 0 );
	m_Buffer.Put( &littleEndianHeader, sizeof(demoheader_t) );
#else
	g_pFileSystem->Seek(m_hDemoFile, 0, FILESYSTEM_SEEK_HEAD);
	g_pFileSystem->Write( &littleEndianHeader, sizeof(demoheader_t), m_hDemoFile );
#endif
}

demoheader_t *CDemoFile::ReadDemoHeader()
{
	bool bOk;
	Q_memset( &m_DemoHeader, 0, sizeof(m_DemoHeader) );

#ifdef DEMO_FILE_UTLBUFFER
	if ( !m_Buffer.IsOpen() )
		return NULL;
	m_Buffer.SeekGet( CUtlBuffer::SEEK_HEAD, 0 );
	m_Buffer.Get( &m_DemoHeader, sizeof(demoheader_t) );
	bOk = m_Buffer.IsValid();
#else
	if ( m_hDemoFile == FILESYSTEM_INVALID_HANDLE )
		return NULL;	// file not open

	// goto file start
	g_pFileSystem->Seek( m_hDemoFile, 0, FILESYSTEM_SEEK_HEAD );
	int r = g_pFileSystem->Read( &m_DemoHeader, sizeof(demoheader_t), m_hDemoFile );
	bOk = ( r == sizeof(demoheader_t) );
#endif

	ByteSwap_demoheader_t( m_DemoHeader );

	if ( !bOk )
		return NULL;  // reading failed

	if ( Q_strcmp( m_DemoHeader.demofilestamp, DEMO_HEADER_ID ) )
	{
		ConMsg( "%s has invalid demo header ID.\n", m_szFileName );
		return NULL;
	}

	if ( m_DemoHeader.networkprotocol != PROTOCOL_VERSION 
#if defined( DEMO_BACKWARDCOMPATABILITY )
		&&  m_DemoHeader.networkprotocol < PROTOCOL_VERSION_12 
#endif
		)
	{
		ConMsg ("ERROR: demo network protocol %i outdated, engine version is %i \n", 
			m_DemoHeader.networkprotocol, PROTOCOL_VERSION );

		return NULL;
	}

	if ( ( m_DemoHeader.demoprotocol > DEMO_PROTOCOL) ||
		 ( m_DemoHeader.demoprotocol < 2 ) )
	{
		ConMsg ("ERROR: demo file protocol %i outdated, engine version is %i \n", 
			m_DemoHeader.demoprotocol, DEMO_PROTOCOL );

		return NULL;
	}

	return &m_DemoHeader;
}

void CDemoFile::WriteFileBytes( FileHandle_t fh, int length )
{
#ifdef DEMO_FILE_UTLBUFFER
	int   copysize = length;
	char  copybuf[COM_COPY_CHUNK_SIZE];

	while ( copysize > COM_COPY_CHUNK_SIZE )
	{
		g_pFileSystem->Read ( copybuf, COM_COPY_CHUNK_SIZE, fh );
		m_Buffer.Put( copybuf, COM_COPY_CHUNK_SIZE );
		copysize -= COM_COPY_CHUNK_SIZE;
	}

	g_pFileSystem->Read ( copybuf, copysize, fh );
	m_Buffer.Put( copybuf, copysize );

	g_pFileSystem->Flush ( fh );
#else
	COM_CopyFileChunk( m_hDemoFile, fh, length );
#endif
}

bool CDemoFile::Open(const char *name, bool bReadOnly)
{
#ifdef DEMO_FILE_UTLBUFFER
	if ( m_Buffer.IsOpen() )
#else
	if ( m_hDemoFile != FILESYSTEM_INVALID_HANDLE )
#endif
	{
		ConMsg ("CDemoFile::Open: file already open.\n");
		return false;
	}

	m_szFileName[0] = 0;  // clear name
	Q_memset( &m_DemoHeader, 0, sizeof(m_DemoHeader) ); // and demo header

	bool bOk;
#ifdef DEMO_FILE_UTLBUFFER
	m_Buffer.Open( name, NULL, bReadOnly ? CUtlBuffer::READ_ONLY : 0 );
	bOk = m_Buffer.IsOpen() && m_Buffer.IsValid();
#else
	m_hDemoFile = g_pFileSystem->Open( name, bReadOnly ? "rb" : "wb" );
	bOk = ( m_hDemoFile != FILESYSTEM_INVALID_HANDLE );
#endif

	if ( !bOk )
	{
		ConMsg ("CDemoFile::Open: couldn't open file %s for %s.\n", 
			name, bReadOnly?"reading":"writing" );
#ifdef DEMO_FILE_UTLBUFFER
		m_Buffer.Close();
#endif
		return false;
	}

	Q_strncpy( m_szFileName, name, sizeof(m_szFileName) );
	return true;
}

bool CDemoFile::IsOpen()
{
#ifdef DEMO_FILE_UTLBUFFER
	return m_Buffer.IsOpen();
#else
	return m_hDemoFile != FILESYSTEM_INVALID_HANDLE;
#endif
}

void CDemoFile::Close()
{
#ifdef DEMO_FILE_UTLBUFFER
	m_Buffer.Close();
#else
	if ( m_hDemoFile != FILESYSTEM_INVALID_HANDLE )
	{
		if ( g_pFileSystem ) // can get called during shutdown, with a NULL g_pFileSystem!
		{
			g_pFileSystem->Close(m_hDemoFile);
		}
		m_hDemoFile = FILESYSTEM_INVALID_HANDLE;
	}
#endif
}

int CDemoFile::GetSize()
{
#ifdef DEMO_FILE_UTLBUFFER
	return m_Buffer.TellMaxPut();
#else
	g_pFileSystem->Seek( m_hDemoFile, 0, FILESYSTEM_SEEK_TAIL );
	return g_pFileSystem->Tell( m_hDemoFile );
#endif
}

