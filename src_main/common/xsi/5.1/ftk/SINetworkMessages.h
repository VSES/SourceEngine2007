#ifndef _SINETWORKMESSAGES_H_
#define _SINETWORKMESSAGES_H_

#include <SIBCUtil.h>

#define SI_NETMSG_FREAD_PACKET_SIZE		256
#define SI_NET_FILE_TRANSFER_ATTEMPTS	5

//
// Messages
//

enum
{
	SI_NETMSG_LOADFILE,
	SI_NETMSG_MERGEFILE,
	SI_NETMSG_SETHOSTNAME,

	SI_NETMSG_FILEOPEN,
	SI_NETMSG_FILEOPEN_ANSWER,

	SI_NETMSG_EOF,
	SI_NETMSG_EOF_ANSWER,

	SI_NETMSG_FGETC,
	SI_NETMSG_FGETC_ANSWER,

	SI_NETMSG_FILECLOSE,
	SI_NETMSG_FILECLOSE_ANSWER,

	SI_NETMSG_FTELL,
	SI_NETMSG_FTELL_ANSWER,

	SI_NETMSG_FSEEK,
	SI_NETMSG_FSEEK_ANSWER,

	SI_NETMSG_FREAD,
	SI_NETMSG_FREAD_PACKET,
	SI_NETMSG_FREAD_END,

	SI_NETMSG_FREAD_CONFIRM,
	
	SI_NETMSG_SETPARAMVALUE,
	SI_NETMSG_GETPARAMHANDLE,
	SI_NETMSG_PARAMHANDLE,
	
	SI_NETMSG_CMDEND

};

#if defined(_WIN32) || defined(_WIN32_WCE) || defined(_XBOX)
#pragma pack( push )
#pragma pack( 1 )
#elif defined(CODEWARRIOR)
#pragma options align= native 
#endif

struct SINetMsgLoadFile
{
    SI_Char		l_szFilename[128];
};

struct SINetMsgSetParamValue
{
    SI_Long		m_lHandle;
	SI_Float	m_fValue;
};

struct SINetMsgSetHostName
{
    SI_Char		m_szHostName[128];
};

struct SINetMsgFileOpen
{
    SI_Char		m_szFilename[256];
	SI_Char		m_szMode[4];
};

struct SINetMsgFileOpenAnswer
{
    SI_Long		m_lHandle;
};

struct SINetMsgFileEOF
{
    SI_Long		m_lHandle;
};

struct SINetMsgFileEOFAnswer
{
	SI_Int		m_iResult;
};

struct SINetMsgFGETC
{
    SI_Long		m_lHandle;
};

struct SINetMsgFGETCAnswer
{
    SI_Int		m_iChar;
};


struct SINetMsgFileClose
{
    SI_Long		m_lHandle;
};

struct SINetMsgFileCloseAnswer
{
    SI_Int		m_iChar;
};

struct SINetMsgFileTell
{
    SI_Long		m_lHandle;

};

struct SINetMsgFileEnd
{
    SI_Long		m_lTotalBytes;

};


struct SINetMsgFileTellAnswer
{
    SI_Long		m_lOffset;
};

struct SINetMsgFileSeek
{
    SI_Long		m_lHandle;
	SI_Long		m_lOffset;
	SI_Int		m_iMode;

};

struct SINetMsgFileSeekAnswer
{
    SI_Int		m_iAnswer;

};

struct SINetMsgFileRead
{
    SI_Long		m_lHandle;
	SI_Long		m_lSize;

};

struct SINetMsgFileReadPacket
{
    SI_Long		m_lOffset;
	SI_Long		m_lSize;
	SI_Char		m_pData[SI_NETMSG_FREAD_PACKET_SIZE];

};

struct SINetMsgFileReadConfirm
{
    SI_Long		m_lOffset;
	SI_Long		m_lSize;
};

struct SINetMsgParamName
{
	SI_Char		m_szName[256];
};

struct SINetMsgParamHandle
{
	SI_Long		m_lHandle;
};

#if defined(_WIN32) || defined(_WIN32_WCE) || defined(_XBOX)
#pragma pack( pop )
#elif defined(CODEWARRIOR)
#pragma options align= reset
#endif

struct XSICOREEXPORT SINetMsg
{
    SI_Char m_bID;

    union
    {
		
        SINetMsgLoadFile		m_LoadFile;
		SINetMsgSetParamValue	m_ParamValue;
		SINetMsgSetHostName		m_HostName;

		SINetMsgFileOpen		m_FileOpen;
		SINetMsgFileOpenAnswer	m_FileOpenAnswer;


		SINetMsgFileEOF			m_FileEOF;
		SINetMsgFileEOFAnswer	m_FileEOFAnswer;

		SINetMsgFGETC			m_FGETC;
		SINetMsgFGETCAnswer		m_FGETCAnswer;

		SINetMsgFileClose		m_FileClose;
		SINetMsgFileCloseAnswer	m_FileCloseAnswer;

		SINetMsgFileTell		m_FileTell;
		SINetMsgFileTellAnswer	m_FileTellAnswer;

		SINetMsgFileSeek		m_FileSeek;
		SINetMsgFileSeekAnswer	m_FileSeekAnswer;

		SINetMsgFileRead		m_FileRead;
		SINetMsgFileReadPacket	m_FileReadPacket;

		SINetMsgFileReadConfirm m_FileReadConfirm;

		SINetMsgFileEnd			m_FileReadEnd;

		SINetMsgParamName		m_ParamName;
		SINetMsgParamHandle		m_ParamHandle;

    };

public:

	explicit SINetMsg( SI_Char in_bID = 0 );

	//
	// Accessors
	//

	SINetMsgLoadFile&			GetLoadFile()		{ return m_LoadFile;}
	SINetMsgSetParamValue&		GetSetParamValue()  { return m_ParamValue;}
	SINetMsgSetHostName&		GetHostName()		{ return m_HostName;}

	SINetMsgFileOpen&			GetFileOpen()			{ return m_FileOpen;}
	SINetMsgFileOpenAnswer&		GetFileOpenAnswer()		{ return m_FileOpenAnswer;}

	SINetMsgFileEOF&			GetFileEOF()		{ return m_FileEOF;};
	SINetMsgFileEOFAnswer&		GetFileEOFAnswer()		{ return m_FileEOFAnswer;};

	SINetMsgFGETC&				GetFGETC()			{ return m_FGETC;};
	SINetMsgFGETCAnswer&		GetFGETCAnswer()	{ return m_FGETCAnswer;};

	SINetMsgFileClose&			GetFileClose()		{ return m_FileClose;};
	SINetMsgFileCloseAnswer&	GetFileCloseAnswer(){ return m_FileCloseAnswer;};

	SINetMsgFileTell&			GetFileTell()		{ return m_FileTell;};
	SINetMsgFileTellAnswer&		GetFileTellAnswer()		{ return m_FileTellAnswer;};

	SINetMsgFileSeek&			GetFileSeek()		{ return m_FileSeek;};
	SINetMsgFileSeekAnswer&		GetFileSeekAnswer()		{ return m_FileSeekAnswer;};

	SINetMsgFileRead&			GetFileRead()		{ return m_FileRead; };
	SINetMsgFileReadPacket&		GetFileReadPacket()	{ return m_FileReadPacket; };
	
	SINetMsgFileReadConfirm&	GetFileReadConfirm() { return m_FileReadConfirm; };

	SINetMsgFileEnd&			GetFileReadEnd()	{ return m_FileReadEnd; };

	SINetMsgParamName&			GetParamName()		{ return m_ParamName; };
	SINetMsgParamHandle&		GetParamHandle()		{ return m_ParamHandle; };

		

    SI_Char GetId()			const	{ return m_bID; }
    SI_Int	GetMaxSize()	const	{ return sizeof(*this); } 
	SI_Int	GetSize()		const;
};


#endif
