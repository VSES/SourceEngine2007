//====== Copyright © 1996-2005, Valve Corporation, All rights reserved. =======
//
// Purpose: 
//
//=============================================================================

#ifndef MPAHEADER_H
#define MPAHEADER_H
#ifdef _WIN32
#pragma once
#endif

#pragma once

#define MPA_HEADER_SIZE 4	// MPEG-Audio Header Size 32bit
#define MAXTIMESREAD 5

class CMPAFile;

class CMPAHeader
{
public:
	CMPAHeader( CMPAFile* pMPAFile, DWORD dwExpectedOffset = 0, bool bSubsequentFrame = false, bool bReverse = false );
	~CMPAHeader();

	bool SkipEmptyFrames();

	// bitrate is in bit per second, to calculate in bytes => (/ 8)
	DWORD GetBytesPerSecond() const { return m_dwBitrate / 8; };
	// calc number of seconds from number of frames
	DWORD GetLengthSecond(DWORD dwNumFrames) const { return dwNumFrames * m_dwSamplesPerFrame / m_dwSamplesPerSec; };
	DWORD GetBytesPerSecond( DWORD dwNumFrames, DWORD dwNumBytes ) const { return dwNumBytes / GetLengthSecond( dwNumFrames ); };
	bool IsMono() const { return (m_ChannelMode == SingleChannel)?true:false; };
	// true if MPEG2/2.5 otherwise false
	bool IsLSF() const { return m_bLSF;	};	

private:
	static const DWORD m_dwMaxRange;
	static const DWORD m_dwTolerance;
	static const DWORD m_dwSamplingRates[4][3];
	static const DWORD m_dwPaddingSizes[3];
	static const DWORD m_dwBitrates[2][3][15];
	static const DWORD m_dwSamplesPerFrames[2][3];
	static const DWORD m_dwCoefficients[2][3];

	// necessary for CRC check (not yet implemented)
	static const DWORD m_dwSideinfoSizes[2][3][2];
	static const WORD wCRC16Table[256];

	bool m_bLSF;		// true means lower sampling frequencies (=MPEG2/MPEG2.5)
	CMPAFile* m_pMPAFile;

public:
	static LPCTSTR m_szLayers[];
	static LPCTSTR m_szMPEGVersions[];
	static LPCTSTR m_szChannelModes[];
	static LPCTSTR m_szEmphasis[];

	enum MPAVersion
	{
		MPEG25 = 0,
		MPEGReserved,
		MPEG2,
		MPEG1		
	}m_Version;

	enum MPALayer
	{
		Layer1,
		Layer2,
		Layer3,
		LayerReserved
	}m_Layer;

	enum Emphasis
	{
		EmphNone = 0,
		Emph5015,
		EmphReserved,
		EmphCCITJ17
	}m_Emphasis;

	enum ChannelMode
	{
		Stereo,
		JointStereo,
		DualChannel,
		SingleChannel
	}m_ChannelMode;
	
	DWORD m_dwSamplesPerSec;
	DWORD m_dwSamplesPerFrame;
	DWORD m_dwBitrate;	// in bit per second (1 kb = 1000 bit, not 1024)
	DWORD m_dwSyncOffset;
	DWORD m_dwComputedFrameSize, m_dwRealFrameSize;
	DWORD m_dwPaddingSize;

	// flags
	bool m_bCopyright, m_bPrivate, m_bOriginal;
	bool m_bCRC; 
	BYTE m_ModeExt;
	
private:
	enum HeaderError
	{
		noError,
		noSync,
		freeBitrate,
		headerCorrupt
	};

	HeaderError DecodeHeader( DWORD dwHeader, bool bSimpleDecode = false );
	inline HeaderError IsSync( DWORD dwOffset, bool bExtended );
};

#endif // MPAHEADER_H
