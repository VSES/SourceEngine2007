//====== Copyright © 1996-2005, Valve Corporation, All rights reserved. =======
//
// Purpose: 
//
//=============================================================================

#ifndef VBRHEADER_H
#define VBRHEADER_H
#ifdef _WIN32
#pragma once
#endif

// for XING VBR Header flags
#define FRAMES_FLAG     0x0001
#define BYTES_FLAG      0x0002
#define TOC_FLAG        0x0004
#define VBR_SCALE_FLAG  0x0008

class CMPAFile;

class CVBRHeader
{
public:
	enum VBRHeaderType
	{
		NoHeader,
		XINGHeader,
		VBRIHeader
	};

	CVBRHeader( CMPAFile* pMPAFile, VBRHeaderType HeaderType, DWORD dwOffset );
	~CVBRHeader(void);

	static bool IsVBRHeaderAvailable( CMPAFile* pMPAFile, VBRHeaderType& HeaderType, DWORD& dwOffset );
	bool SeekPoint(float fPercent, DWORD& dwSeekPoint);

	DWORD m_dwBytesPerSec;
	DWORD m_dwBytes;		// total number of bytes
	DWORD m_dwFrames;		// total number of frames

private:
	static DWORD m_dwXINGOffsets[2][2];

	static bool CheckID( CMPAFile* pMPAFile, char ch0, char ch1, char ch2, char ch3, DWORD& dwOffset );
	static bool CheckXING( CMPAFile* pMPAFile, DWORD& dwOffset );
	static bool CheckVBRI( CMPAFile* pMPAFile, DWORD& dwOffset );

	bool ExtractLAMETag( DWORD dwOffset );
	bool ExtractXINGHeader( DWORD dwOffset );	
	bool ExtractVBRIHeader( DWORD dwOffset );

	DWORD SeekPointXING(float fPercent)const ;
	DWORD SeekPointVBRI(float fPercent) const;
	DWORD SeekPointByTimeVBRI(float fEntryTimeMS) const;

	CMPAFile* m_pMPAFile;
public:	
	VBRHeaderType m_HeaderType;
	DWORD m_dwOffset;
	DWORD m_dwQuality;	// quality (0..100)
	int* m_pnToc;				// TOC points for seeking (must be freed)
	DWORD m_dwTableSize;	// size of table (number of entries)	

	// only VBRI
	float m_fDelay;	
	DWORD m_dwTableScale;	// for seeking
	DWORD m_dwBytesPerEntry;
    DWORD m_dwFramesPerEntry;
	DWORD m_dwVersion;
};

#endif // VBRHEADER_H
