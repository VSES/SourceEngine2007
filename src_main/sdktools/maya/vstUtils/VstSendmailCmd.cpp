//======= Copyright © 1996-2006, Valve Corporation, All rights reserved. ======
//
// Purpose: Implementation of CVstSendmailCmd
//
//=============================================================================

// Windows includes
#include <windows.h>
#include <time.h>

// Maya includes
#include <maya/MArgDatabase.h>
#include <maya/MArgList.h>
#include <maya/MGlobal.h>
#include <maya/MSyntax.h>

// Valve includes
#include "valveMaya.h"
#include "VsMayaMPxFactory.h"

// Local includes

class CVstSendmailCmd : public CVsMayaMPxCommand
{
public:
	CVstSendmailCmd();

	virtual ~CVstSendmailCmd();

	virtual MStatus doIt(
		const MArgList &mArgList );

	virtual bool hasSyntax() const { return true; }

	// Inheritied from CVsMayaCommand
	void SpecifySyntax( MSyntax &mSyntax, ValveMaya::CMSyntaxHelp &mSyntaxHelp );

	MStatus Sendmail(
		const MStringArray &to,
		const MString &from,
		const MString &subject,
		const MString &message,
		const MString &server,
		bool verbose ) const;
};


//-----------------------------------------------------------------------------
// Hook the command so we can see it in Maya
//-----------------------------------------------------------------------------
INSTALL_MAYA_MPXCOMMAND(
	CVstSendmailCmd,
	vstSendmail,
	"Send an email message from mel" );


//-----------------------------------------------------------------------------
// Purpose: Creator
//-----------------------------------------------------------------------------
CVstSendmailCmd::CVstSendmailCmd()
{
}


//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CVstSendmailCmd::~CVstSendmailCmd()
{
}


//-----------------------------------------------------------------------------
//
// Purpose: MSyntax factory for the command
// Input  :
// Output : A Maya MSyntax class describing command's command line syntax
//
//-----------------------------------------------------------------------------

static const char *const kHelp( "h" );
static const char *const kTo( "t" );
static const char *const kFrom( "f" );
static const char *const kSubject( "s" );
static const char *const kMessage( "m" );
static const char *const kServer( "sv" );
static const char *const kVerbose( "v" );

void CVstSendmailCmd::SpecifySyntax(
	MSyntax &mSyntax,
	ValveMaya::CMSyntaxHelp &mSyntaxHelp )
{
	mSyntaxHelp.Clear();

	mSyntaxHelp.AddFlag( mSyntax, kHelp, "help", "General", "Prints this information" );

	mSyntaxHelp.AddFlag( mSyntax, kFrom, "from",
		"Sendmail",
		"Specifies who the message is from, if not specified it comes from <yourlogin>@valvesoftware.com",
		MSyntax::kString );

	mSyntaxHelp.AddFlag( mSyntax, kSubject, "subject",
		"Sendmail",
		"Specifies who the subject of the message",
		MSyntax::kString );

	mSyntaxHelp.AddFlag( mSyntax, kTo, "mailto",
		"Sendmail",
		"Specifies who the message should be sent to.  -mailto can be specified multiple times.\n"
		"   Also multiple email addresses can be specified by separating them with a ;.\n"
		"   e.g. -mailto \"foobar@valvesoftware.com\" -mailto \"foo@valvesoftware.com;bar@valvesoftware.com\"",
		MSyntax::kString );
	mSyntax.makeFlagMultiUse( kTo );

	mSyntaxHelp.AddFlag( mSyntax, kMessage, "message",
		"Sendmail",
		"Specifies the body of the message.",
		MSyntax::kString );

	mSyntaxHelp.AddFlag( mSyntax, kServer, "server",
		"Sendmail",
		"Specifies the mail server to send the mail to.  If not specified it defaults to exchange2",
		MSyntax::kString );

	mSyntaxHelp.AddFlag( mSyntax, kVerbose, "verbose",
		"Sendmail",
		"Print the traffic with the mail server" );

	mSyntax.enableEdit( false );
	mSyntax.enableQuery( false );
}


//-----------------------------------------------------------------------------
// Called when the user executes the command
//-----------------------------------------------------------------------------
MStatus CVstSendmailCmd::doIt(
	const MArgList &mArgList )
{
	MStatus mStatus;
	MArgDatabase mArgDatabase( syntax(), mArgList, &mStatus );

	if ( !mStatus )
		return mStatus;

	if ( mArgDatabase.isFlagSet( kHelp ) )
	{
		GetSyntaxHelp()->PrintHelp( GetName(), GetDesc() );
		return MS::kSuccess;
	}

	if ( !( mArgDatabase.isFlagSet( kTo ) && mArgDatabase.isFlagSet( kMessage ) ) )
	{
		merr << "Cannot send a message unless -mailto and -message are specified" << std::endl;
		return MS::kFailure;
	}

	MStringArray optTos;

	{
		const uint nTo( mArgDatabase.numberOfFlagUses( kTo ) );
		for ( uint i( 0 ); i != nTo; ++i )
		{
			MString optTo;
			MArgList tmpArgList;
			mArgDatabase.getFlagArgumentList( kTo, i, tmpArgList );
			tmpArgList.get( 0, optTo );
			if ( optTo.index( ';' ) || true )
			{
				MStringArray tmpArgs;
				optTo.split( ';', tmpArgs );
				if ( tmpArgs.length() || true )
				{
					for ( uint j( 0 ); j != tmpArgs.length(); ++j )
					{
						optTos.append( tmpArgs[ j ] );
					}
				}
				else
				{
					optTos.append( optTo );
				}
			}
		}

		if ( optTos.length() == 0 )
		{
			merr << "Can't figure out the email address(es)" << std::endl;
			return MS::kFailure;
		}
	}

	MString optFrom;
	if ( mArgDatabase.isFlagSet( kFrom ) )
	{
		mArgDatabase.getFlagArgument( kFrom, 0, optFrom );
	}
	else
	{
		char pTmpBuf[BUFSIZ];
		*pTmpBuf ='\0';
		DWORD dwSize( sizeof( pTmpBuf ) );

		GetUserName( pTmpBuf, &dwSize);
		if ( *pTmpBuf == '\0' )
		{
			optFrom = "unknown";
		}
		else
		{
			optFrom = MString( pTmpBuf ) + "@valvesoftware.com";
		}
	}

	MString optSubject;
	if ( mArgDatabase.isFlagSet( kSubject ) )
	{
		mArgDatabase.getFlagArgument( kSubject, 0, optSubject );
	}
	else
	{
		optSubject = "Mail From Maya With No Subject!";
	}

	MString optMessage;
	mArgDatabase.getFlagArgument( kMessage, 0, optMessage );

	MString optServer;
	if ( mArgDatabase.isFlagSet( kServer ) )
	{
		mArgDatabase.getFlagArgument( kServer, 0, optServer );
	}
	else
	{
		optServer = "exchange2";
	}

	MStatus retVal( Sendmail( optTos, optFrom, optSubject, optMessage, optServer, mArgDatabase.isFlagSet( kVerbose ) ) );

	if ( retVal )
	{
		MString result( "Mail sent to:" );
		for ( uint i( 0 ); i != optTos.length(); ++i )
		{
			result += MString( " \"" ) + optTos[ i ] + "\"";
		}
		setResult( result );
	}
	else
	{
		setResult( "Mail send failed" );
	}

	return retVal;
}


//-----------------------------------------------------------------------------
// A simple socket class
//-----------------------------------------------------------------------------
class CSimpleSocket
{
public:
	CSimpleSocket(
		bool verbose = false );

	~CSimpleSocket();

	bool CSimpleSocket::Open(
		const char *pServerName,
		int port );

	void Close();

	void SendString(
		const char *pString );

	bool WaitFor(
		const char *pString );

	SOCKET m_pSocket;
	bool m_verbose;
};


//-----------------------------------------------------------------------------
// Purpose: intialize sockets
//-----------------------------------------------------------------------------
CSimpleSocket::CSimpleSocket(
	bool verbose )
: m_pSocket( NULL )
, m_verbose( verbose )
{
	WSADATA wsData;

	WORD wVersionRequested = MAKEWORD(1, 1);
	WSAStartup(wVersionRequested, &wsData);
}


//-----------------------------------------------------------------------------
// Purpose: cleanup all socket resources
//-----------------------------------------------------------------------------
CSimpleSocket::~CSimpleSocket()
{
	Close();
	WSACleanup();
}


//-----------------------------------------------------------------------------
// Open a TCP socket on a given port & connect to a given server
//-----------------------------------------------------------------------------
bool CSimpleSocket::Open(
	const char *pServerName,
	int port )
{
	Close();

	SOCKADDR_IN sockAddr;
	SOCKET s;

	memset( &sockAddr, 0 , sizeof( sockAddr ) );

	s = socket( AF_INET, SOCK_STREAM, 0 );

	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr = inet_addr(pServerName);

	if (sockAddr.sin_addr.s_addr == INADDR_NONE)
	{
		LPHOSTENT lphost;
		lphost = gethostbyname(pServerName);
		if (lphost != NULL)
		{
			sockAddr.sin_addr.s_addr = ((LPIN_ADDR)lphost->h_addr)->s_addr;
		}
		else
		{
			WSASetLastError(WSAEINVAL);
			return FALSE;
		}
	}

	sockAddr.sin_port = htons((u_short)port);

	if ( connect( s, (SOCKADDR *)&sockAddr, sizeof(sockAddr) ) == SOCKET_ERROR )
	{
		return false;
	}

	m_pSocket = s;

	return true;
}


//-----------------------------------------------------------------------------
// Purpose: close the socket opened with SocketOpen()
// Input  : socket - 
//-----------------------------------------------------------------------------
void CSimpleSocket::Close()
{
	if ( m_pSocket )
	{
		closesocket( m_pSocket );
		m_pSocket = NULL;
	}
}


//-----------------------------------------------------------------------------
// Purpose: Write a string to the socket.  String is NULL terminated on input,
//			but terminator is NOT written to the socket
// Input  : socket - 
//			*pString - string to write
//-----------------------------------------------------------------------------
void CSimpleSocket::SendString(
	const char *pString )
{
	if ( !m_pSocket || !pString )
		return;

	const int strLen( strlen( pString ) );

	if ( !strLen )
		return;

	if ( send( m_pSocket, pString, strLen, 0 ) != SOCKET_ERROR )
	{
		if ( m_verbose )
		{
			minfo << pString;
			minfo.flush();
		}
	}
}


//-----------------------------------------------------------------------------
// Purpose: receive input from a socket until a certain string is received
//			ASSUME: socket data is all text
// Input  : socket - 
//			*pString - string to match, if NULL, just poll the socket once
//-----------------------------------------------------------------------------
bool CSimpleSocket::WaitFor(
	const char *pString )
{
	if ( !m_pSocket )
		return false;

	char buf[BUFSIZ];

	for ( ; ; )
	{
		fd_set readFDs;
		FD_ZERO( &readFDs );
		FD_SET( m_pSocket, &readFDs );

		struct timeval t;
		t.tv_sec = 5;
		t.tv_usec = 0;

		if ( select( 0, &readFDs, NULL, NULL, &t ) <= 0 )
			return false;	// Nothing to read

		if ( !FD_ISSET( m_pSocket, &readFDs ) )
			return false;	// Don't know why we got here... nothing to read on the only socket of interest?!?!

		u_long arg;
		if ( ioctlsocket( m_pSocket, FIONREAD, &arg ) != 0 )
			return false;	// ioctl failed

		if ( arg == 0 )
			return false;	// No data to read!

		const int pLen( recv( m_pSocket, buf, sizeof( buf ) - 1, MSG_PEEK ) );
		if ( pLen <= 0 )
			return false;	// No data to read still...

		const int len( recv( m_pSocket, buf, sizeof( buf ) - 1, 0 ) );
		buf[len] = '\0';
		if ( m_verbose )
		{
			minfo << buf;
			minfo.flush();
		}

		if ( !pString || strstr( buf, pString ) )
			return true;
	}
}


//-----------------------------------------------------------------------------
// Purpose: mail a text file using the SMTP mail server connected to socket
// Input  : socket - 
//			*pFrom - from address
//			*pTo -  to address
//			*pSubject - subject of the mail
//			*fp - text mode file opened
//-----------------------------------------------------------------------------

MStatus CVstSendmailCmd::Sendmail(
	const MStringArray &to,
	const MString &from,
	const MString &subject,
	const MString &message,
	const MString &server,
	bool verbose) const
{
	CSimpleSocket sSocket( verbose );

	if ( !sSocket.Open( server.asChar(), 25 ) )
	{
		merr << "Can't open socket to " << server << ":25" << std::endl;
		return MS::kFailure;
	}

	sSocket.WaitFor( "\n" );

	MString bufStr;

	sSocket.SendString( "HELO\r\n" );
	sSocket.WaitFor( "\r\n" );
	bufStr = MString( "MAIL FROM: <" ) + from + ">\r\n";
	sSocket.SendString( bufStr.asChar() );

	sSocket.WaitFor( "\n" );

	for (uint i = 0; i < to.length(); ++i)
	{
		bufStr = MString( "RCPT TO: <" ) + to[i] + ">\r\n";
		sSocket.SendString( bufStr.asChar() );
		sSocket.WaitFor( "\n" );
	}
	sSocket.SendString( "DATA\r\n" );

	sSocket.WaitFor( "\n" );

	time_t currentTime;
	time( &currentTime );
	struct tm *localTime = gmtime( &currentTime );

	static char *months[] = { "JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC" };
	char buf[BUFSIZ];
	_snprintf( buf, sizeof( buf ), "DATE: %02d %s %4d %02d:%02d:%02d\n", localTime->tm_mday, months[localTime->tm_mon], localTime->tm_year+1900,
		localTime->tm_hour, localTime->tm_min, localTime->tm_sec );
	sSocket.SendString( buf );

	bufStr = MString( "FROM: " ) + from + "\r\n";
	sSocket.SendString( bufStr.asChar() );

	for (uint i = 0; i < to.length(); ++i)
	{
		bufStr = MString( "TO: " ) + to[i] + "\r\n";
		sSocket.SendString( bufStr.asChar() );
	}

	bufStr = MString( "SUBJECT: " ) + subject + "\r\n\r\n";
	sSocket.SendString( bufStr.asChar() );

	sSocket.SendString( message.asChar() );

	sSocket.SendString( "\r\n.\r\n" );
	sSocket.SendString( "\r\n" );

	sSocket.SendString( "quit\r\n" );
	sSocket.SendString( "\r\n" );

	sSocket.Close();

	return MS::kSuccess;
}