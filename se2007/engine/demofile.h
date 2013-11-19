//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: 
//
//===========================================================================//

#ifndef DEMOFILE_H
#define DEMOFILE_H
#ifdef _WIN32
#pragma once
#endif

//#ifdef _X360
#define DEMO_FILE_UTLBUFFER 1
//#endif

#include "demo.h"

#ifdef DEMO_FILE_UTLBUFFER
#include "tier2/utlstreambuffer.h"
#else
#include <filesystem.h>
#endif


#include "tier1/bitbuf.h"


//-----------------------------------------------------------------------------
// Demo file 
//-----------------------------------------------------------------------------
class CDemoFile  
{
public:
	CDemoFile();
	~CDemoFile();

	bool	Open(const char *name, bool bReadOnly);
	bool	IsOpen();
	void	Close();

	void	SeekTo( int position, bool bRead );
	unsigned int GetCurPos( bool bRead );
	int		GetSize();

	void	WriteRawData( const char *buffer, int length );
	int		ReadRawData( char *buffer, int length );

	void	WriteSequenceInfo(int nSeqNrIn, int nSeqNrOutAck);
	void	ReadSequenceInfo(int &nSeqNrIn, int &nSeqNrOutAck);

	void	WriteCmdInfo( democmdinfo_t& info );
	void	ReadCmdInfo( democmdinfo_t& info );

	void	WriteCmdHeader( unsigned char cmd, int tick );
	void	ReadCmdHeader( unsigned char& cmd, int& tick );
	
	void	WriteConsoleCommand( const char *cmd, int tick );
	const char *ReadConsoleCommand( void );

	void	WriteNetworkDataTables( bf_write *buf, int tick );
	int		ReadNetworkDataTables( bf_read *buf );
	
	void	WriteUserCmd( int cmdnumber, const char *buffer, unsigned char bytes, int tick );
	int		ReadUserCmd( char *buffer, int &size );

	void	WriteDemoHeader();
	demoheader_t *ReadDemoHeader();

	void	WriteFileBytes( FileHandle_t fh, int length );

public:
	char			m_szFileName[MAX_PATH];	//name of current demo file
	demoheader_t    m_DemoHeader;  //general demo info

private:
#ifdef DEMO_FILE_UTLBUFFER
	CUtlStreamBuffer m_Buffer;
#else
	FileHandle_t	m_hDemoFile;	// filesystem handle
#endif
};

#endif // DEMOFILE_H
