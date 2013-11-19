//====== Copyright © 1996-2005, Valve Corporation, All rights reserved. =======
//
// Purpose: Uses mp3 code from:  http://www.codeproject.com/audio/MPEGAudioInfo.asp
//
// There don't appear to be any licensing restrictions for using this code:
//
/*
- Readme - MPEG Audio Info Tool V2.0 - 2004-11-01

Description:
This tool can display information about MPEG audio files. It supports
MPEG1, MPEG2, MPEG2.5 in all three layers. You can get all the fields
from the MPEG audio frame in each frame of the file. Additionally you
can check the whole file for inconsistencies.


This tool was written as an example on how to use the classes:
CMPAFile, CMPAHeader, CVBRHeader and CMPAException.

The article MPEG Audio Frame Header on Sourceproject
[http://www.codeproject.com/audio/MPEGAudioInfo.asp] 
provides additional information about these classes and the frame header
in general.

This tool was written with MS Visual C++ 7.1. The MFC library is
statically linked.
*/
//=============================================================================

#ifndef MPAFILE_H
#define MPAFILE_H
#ifdef _WIN32
#pragma once
#endif

#pragma once

#include "vbrheader.h"
#include "mpaheader.h"
#include "FileSystem.h"

// exception class
class CMPAException
{
public:
	
	enum ErrorIDs
	{
		ErrOpenFile,
		ErrSetPosition,
		ErrReadFile,
		EndOfBuffer,
		NoVBRHeader,
		IncompleteVBRHeader,
		NoFrameInTolerance,
		NoFrame
	};

	CMPAException(ErrorIDs ErrorID, LPCTSTR szFile, LPCTSTR szFunction = NULL, bool bGetLastError=false );
	// copy constructor (necessary because of LPSTR members)
	CMPAException(const CMPAException& Source);
	~CMPAException(void);

	ErrorIDs GetErrorID() { return m_ErrorID; }

	void ShowError();

private:
	ErrorIDs m_ErrorID;
	bool m_bGetLastError;
	LPTSTR m_szFunction;
	LPTSTR m_szFile;
};


class CMPAFile
{
public:
	CMPAFile(LPCTSTR szFile, DWORD dwFileOffset, FileHandle_t hFile = FILESYSTEM_INVALID_HANDLE );
	~CMPAFile(void);

	DWORD CMPAFile::ExtractBytes( DWORD& dwOffset, DWORD dwNumBytes, bool bMoveOffset = true );
	LPCTSTR GetFilename() const { return m_szFile; };

	bool CMPAFile::GetNextFrame();
	bool CMPAFile::GetPrevFrame();
	bool CMPAFile::GetFirstFrame();
	bool CMPAFile::GetLastFrame();

private:
	static const DWORD m_dwInitBufferSize;

	// methods for file access
	void Open( LPCTSTR szFilename );
	void SetPosition( int offset );
	DWORD Read( LPVOID pData, DWORD dwSize, DWORD dwOffset );

	void FillBuffer( DWORD dwOffsetToRead );

	static DWORD m_dwBufferSizes[MAXTIMESREAD];

	// concerning file itself
	FileHandle_t m_hFile;
	LPTSTR m_szFile;
	bool m_bMustReleaseFile;

public:	
	DWORD m_dwBegin;	// offset of first MPEG Audio frame
	DWORD m_dwEnd;		// offset of last MPEG Audio frame (estimated)
	bool m_bVBRFile;

	DWORD m_dwBytesPerSec;

	CMPAHeader* m_pMPAHeader;
	DWORD m_dwFrameNo;

	CVBRHeader* m_pVBRHeader;		// XING or VBRI

	// concerning read-buffer
	DWORD m_dwNumTimesRead;
	BYTE* m_pBuffer;
	DWORD m_dwBufferSize;
};

#endif // MPAFILE_H
