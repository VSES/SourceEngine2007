//====== Copyright © 1996-2004, Valve Corporation, All rights reserved. =======
//
// Purpose: 
//
//=============================================================================

#include "packfile.h"
#include "tier0/platform.h"

#include "FileSystem_Helpers.h"
#include "UtlVector.h"
#include "utlrbtree.h"
#include "utlsymbol.h"
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <io.h>
#include <minmax.h>
#include <direct.h>


// structures for building pack files
#pragma pack(1)
typedef struct
{
	char name[ 112 ];
	int64 filepos;
	int64 filelen;
} packfile64_t;

typedef struct
{
	char id[ 4 ];
	int64 dirofs;
	int64 dirlen;
} packheader64_t;

typedef struct
{
	char id[ 8 ];
	int64 packheaderpos;
	int64 originalfilesize;
} packappenededheader_t;
#pragma pack()

//-----------------------------------------------------------------------------
// Purpose: creates a new empty pack file
//-----------------------------------------------------------------------------
bool Pack_CreateFile(const char *pakfile)
{
	// need to use 64-bit capable file I/O calls to handle the giant pack files
	int fh = _open(pakfile, _O_CREAT | _O_RDWR | _O_BINARY | _O_TRUNC, _S_IREAD | _S_IWRITE );
	if (fh == -1)
		return false;

	// create empty pak header & end signature
	packheader64_t header = { { 'P', 'K', '6', '4' }, 0, 0 };
	packappenededheader_t appendheader = { { 'P', 'A', 'C', 'K', 'A', 'P', 'P', 'E' } };
	appendheader.packheaderpos = 0;
	appendheader.originalfilesize = 0;
	_write(fh, &header, sizeof(header));
	_write(fh, &appendheader, sizeof(appendheader));

	// finished clearing
	_close(fh);
	assert(Pack_Validate(pakfile));
	return true;
}

//-----------------------------------------------------------------------------
// Purpose: Deletes any existing files from the pak file
//-----------------------------------------------------------------------------
void Pack_Clear(const char *pakfile)
{
	// need to use 64-bit capable file I/O calls to handle the giant pack files
	int fh = _open(pakfile, _O_RDWR | _O_BINARY);
	if (fh == -1)
	{
		printf("Couldn't open pakfile '%s'\n", pakfile);
		exit(0);
		return;
	}

	// jump to the end of the file, see if it has a header
	packappenededheader_t appended;
	_lseeki64(fh, (long)(0 - sizeof(packappenededheader_t)), SEEK_END);
	_read(fh, &appended, sizeof(appended));

	int64 iOriginalFileSize = _filelengthi64(fh);

	if (appended.id[0] == 'P'
		&& appended.id[1] == 'A'
		&& appended.id[2] == 'C'
		&& appended.id[3] == 'K'
		&& appended.id[4] == 'A'
		&& appended.id[5] == 'P'
		&& appended.id[6] == 'P'
		&& appended.id[7] == 'E')
	{
		// we have an appended pack file, look up the header
		iOriginalFileSize = appended.originalfilesize;

		// kill any existing pack file
		assert(iOriginalFileSize >= 0 && iOriginalFileSize <= _filelengthi64(fh));
		_chsize(fh, (long)iOriginalFileSize);
	}

	// create empty pak header & end signature
	packheader64_t header = { { 'P', 'K', '6', '4' }, 0, 0 };
	packappenededheader_t appendheader = { { 'P', 'A', 'C', 'K', 'A', 'P', 'P', 'E' } };
	_lseeki64(fh, 0, SEEK_END);
	appendheader.packheaderpos = _telli64(fh);
	appendheader.originalfilesize = iOriginalFileSize;
	_write(fh, &header, sizeof(header));
	_write(fh, &appendheader, sizeof(appendheader));

	assert( _filelengthi64(fh) == iOriginalFileSize + sizeof(header) + sizeof(appendheader) );

	// finished clearing
	_close(fh);

	assert(Pack_Validate(pakfile));
}

//-----------------------------------------------------------------------------
// Purpose: Appends a single file to the pak file
//-----------------------------------------------------------------------------
bool Pack_AppendFile(const char *pakfile, const char *file)
{
	if (!Pack_StartAppendingFile(pakfile, file))
		return false;

	assert(Pack_Validate(pakfile));

	// read in the file
	int hFile = _open(file, _O_RDONLY | _O_BINARY);
	if (hFile == -1)
	{
		printf("Couldn't find resource file '%s'\n", file);
		return false;
	}

	// start copying
	char *tempBuf = (char *)malloc(1024 * 1024);
	int64 iFileRemaining = _filelengthi64(hFile);
	while (iFileRemaining > 0)
	{
		int bytesToCopy = (int)min(iFileRemaining, (1024 * 1024));
		iFileRemaining -= bytesToCopy;

		// read
		int bytes = _read(hFile, tempBuf, bytesToCopy);
		bytes;
		assert(bytes == bytesToCopy);

		Pack_AppendChunkToFile(tempBuf, bytesToCopy);
	}

	free(tempBuf);
	_close(hFile);

	Pack_FinishAppendingFile();

	assert(Pack_Validate(pakfile));
	return true;
}

//-----------------------------------------------------------------------------
// Purpose: Holds temporary data for when appending files
//-----------------------------------------------------------------------------
struct PackFileAppendInfo_t
{
	int m_hPack;
	int64 m_iOldPakFileSize;
	int64 m_iOldHeaderPos;
	int64 m_iOriginalFileSize;
	int m_iFilesInPack;
	packfile64_t *m_pPackFiles;
	char m_szNewFileName[_MAX_PATH];
	int64 m_iNewFileLength;
};

PackFileAppendInfo_t g_Pack;

//-----------------------------------------------------------------------------
// Purpose: Starts adding a file to the packfile 
//-----------------------------------------------------------------------------
bool Pack_StartAppendingFile(const char *pakfile, const char *pFileName)
{
	memset(&g_Pack, 0, sizeof(g_Pack));

	if (strlen(pFileName) >= 112)
	{
		printf("Couldn't append file '%s', filename is too long (> 128 characters)\n", pFileName);
		return false;
	}
	strncpy(g_Pack.m_szNewFileName, pFileName, sizeof(g_Pack.m_szNewFileName));

	g_Pack.m_hPack = _open(pakfile, _O_RDWR | _O_BINARY);
	if (g_Pack.m_hPack == -1)
	{
		printf("Couldn't open pakfile '%s'\n", pakfile);
		return false;
	}

	g_Pack.m_iOldPakFileSize = _filelengthi64(g_Pack.m_hPack);

	// jump to the end of the file, see if it has a header
	packappenededheader_t appended;
	_lseeki64(g_Pack.m_hPack, (long)(0 - sizeof(packappenededheader_t)), SEEK_END);
	_read(g_Pack.m_hPack, &appended, sizeof(appended));

	if (!(appended.id[0] == 'P'
		&& appended.id[1] == 'A'
		&& appended.id[2] == 'C'
		&& appended.id[3] == 'K'
		&& appended.id[4] == 'A'
		&& appended.id[5] == 'P'
		&& appended.id[6] == 'P'
		&& appended.id[7] == 'E'))
	{
		printf("Invalid pack file '%s'\n", pakfile);
		return false;
	}

	g_Pack.m_iOldHeaderPos = appended.packheaderpos;
	g_Pack.m_iOriginalFileSize = appended.originalfilesize;

	// read in the pack file header
	packheader64_t header;
	_lseeki64(g_Pack.m_hPack, appended.packheaderpos, SEEK_SET);
	_read(g_Pack.m_hPack, &header, sizeof(header));
	g_Pack.m_iFilesInPack = (int)(header.dirlen / sizeof(packfile64_t));
	g_Pack.m_iFilesInPack += 1;
	
	// read in the directory
	g_Pack.m_pPackFiles = new packfile64_t[g_Pack.m_iFilesInPack];
	if (header.dirlen)
	{
		_lseeki64(g_Pack.m_hPack, appended.packheaderpos + header.dirofs, SEEK_SET);
		_read(g_Pack.m_hPack, g_Pack.m_pPackFiles, (long)header.dirlen);
	}

	// we're going to put the new file where the current packheader is, and move the header forward in the file

	// set back to the header position to write the new file
	_lseeki64(g_Pack.m_hPack, appended.packheaderpos, SEEK_SET);

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: Appends a chunk to the currently open file
//-----------------------------------------------------------------------------
void Pack_AppendChunkToFile(const void *pv, int bytesToWrite)
{
	// write
	int bytes = _write(g_Pack.m_hPack, pv, bytesToWrite);
	assert(bytes == bytesToWrite);
	bytes;

	// record the increased size
	g_Pack.m_iNewFileLength += bytesToWrite;
}

//-----------------------------------------------------------------------------
// Purpose: Finishes the file, finishes writing the header
//-----------------------------------------------------------------------------
void Pack_FinishAppendingFile()
{
	if (!g_Pack.m_hPack || g_Pack.m_hPack == -1 )
		return;

	// update the header
	// update all the existing pack file references since the file has moved
	int64 iHeaderPos = _telli64(g_Pack.m_hPack);

	packheader64_t header = { { 'P', 'K', '6', '4' }, 0, 0 };
	header.dirofs = sizeof(header);
	header.dirlen = sizeof(packfile64_t) * g_Pack.m_iFilesInPack;
	// set the new packfile references
	g_Pack.m_pPackFiles[g_Pack.m_iFilesInPack - 1].filelen = g_Pack.m_iNewFileLength;
	g_Pack.m_pPackFiles[g_Pack.m_iFilesInPack - 1].filepos = (0 - g_Pack.m_iNewFileLength);
	strncpy(g_Pack.m_pPackFiles[g_Pack.m_iFilesInPack - 1].name, g_Pack.m_szNewFileName, sizeof(g_Pack.m_pPackFiles[g_Pack.m_iFilesInPack - 1].name));

	// update all the existing pack file references
	int64 iOldHeaderPos = g_Pack.m_iOldHeaderPos;
	for (int i = 0; i < g_Pack.m_iFilesInPack - 1; i++)
	{
		assert(g_Pack.m_pPackFiles[i].name[0]);
		g_Pack.m_pPackFiles[i].filepos += (iOldHeaderPos - iHeaderPos);
	}

	packappenededheader_t appended = { { 'P', 'A', 'C', 'K', 'A', 'P', 'P', 'E' } };
	appended.originalfilesize = g_Pack.m_iOriginalFileSize;
	appended.packheaderpos = iHeaderPos;

	// write out header, pack directory, and postfix header
	_write(g_Pack.m_hPack, &header, sizeof(header));
	_write(g_Pack.m_hPack, g_Pack.m_pPackFiles, (unsigned int)header.dirlen);
	_write(g_Pack.m_hPack, &appended, sizeof(appended));

	// filelength should be increased by the size
	assert( _filelengthi64(g_Pack.m_hPack) == g_Pack.m_iOldPakFileSize + sizeof(packfile64_t) + g_Pack.m_iNewFileLength );

	// header + file
	assert( _filelengthi64(g_Pack.m_hPack) == (appended.packheaderpos + sizeof(header) + header.dirlen + sizeof(appended)) );

	_close(g_Pack.m_hPack);
	g_Pack.m_hPack = NULL;

	delete [] g_Pack.m_pPackFiles;

	// finished appending
	printf("Added file '%s'\n", g_Pack.m_szNewFileName);
}

//-----------------------------------------------------------------------------
// Purpose: total size of pack file in megabytes
//-----------------------------------------------------------------------------
int Pack_GetSizeMB()
{
	return (int)(_filelengthi64(g_Pack.m_hPack) / (1024 * 1024));
}

//-----------------------------------------------------------------------------
// Purpose: total size of pack file in megabytes
//-----------------------------------------------------------------------------
int Pack_GetFileSizeMB(const char *pakfile)
{
	int hPack = _open(pakfile, _O_RDWR | _O_BINARY);
	if (hPack == -1)
		return 0;
	int iMB = (int)(_filelengthi64(hPack) / (1024 * 1024));
	_close(hPack);
	return iMB;
}

//-----------------------------------------------------------------------------
// Purpose: Lists all of the files in a pakfile
// Input  : *pakfile - 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool Pack_Validate(const char *pakfile)
{
	int hPack = _open(pakfile, _O_RDWR | _O_BINARY);
	if (hPack == -1)
	{
		printf("Couldn't open pakfile '%s'\n", pakfile);
		return false;
	}

	// jump to the end of the file, see if it has a header
	int64 iFileSize = _filelengthi64(hPack);
	iFileSize;
	packappenededheader_t appended;
	_lseeki64(hPack, (long)(0 - sizeof(packappenededheader_t)), SEEK_END);
	_read(hPack, &appended, sizeof(appended));

	if (!(appended.id[0] == 'P'
		&& appended.id[1] == 'A'
		&& appended.id[2] == 'C'
		&& appended.id[3] == 'K'
		&& appended.id[4] == 'A'
		&& appended.id[5] == 'P'
		&& appended.id[6] == 'P'
		&& appended.id[7] == 'E'))
	{
		printf("Invalid pack file '%s'\n", pakfile);
		return false;
	}

	// read in header
	packheader64_t header;
	_lseeki64(hPack, appended.packheaderpos, SEEK_SET);
	_read(hPack, &header, sizeof(header));
	int iFilesInPack = (int)(header.dirlen / sizeof(packfile64_t));

	// header + file
	assert( iFileSize == (appended.packheaderpos + sizeof(header) + header.dirlen + sizeof(appended)) );
	
	// read in the directory
	packfile64_t *packfiles = new packfile64_t[iFilesInPack];
	if (header.dirlen)
	{
		_lseeki64(hPack, appended.packheaderpos + header.dirofs, SEEK_SET);
		_read(hPack, packfiles, (long)header.dirlen);
	}

	// list
	for (int i = 0; i < iFilesInPack; i++)
	{
		assert(packfiles[i].name[0]);
		assert(packfiles[i].filelen);
		// printf("File '%s'  size %d\n", packfiles[i].name, (int)packfiles[i].filelen);
	}

	_close(hPack);
	return true;
}

