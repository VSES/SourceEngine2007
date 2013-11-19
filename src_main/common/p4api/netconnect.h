/*
 * Copyright 1995, 1996 Perforce Software.  All rights reserved.
 *
 * This file is part of Perforce - the FAST SCM System.
 */

/*
 * NetConnect.h - RPC connection handler
 *
 * Classes Defined:
 *
 *	NetEndPoint - an endpoint for making connections 
 *	NetTransport - an RPC connection to/from a remote host 
 *
 * Description:
 *
 *	These classes provide abstract base classes for an endpoint for
 *	a connection, and a connection itself.
 *
 *	It should go without saying, but destructing a NetTransport must 
 *	imply a Close().
 *
 * Public Methods:
 *
 *	NetEndPoint::Listen() - set up for subsequent Accept()
 *	NetEndPoint::ListenCheck() - see if we can listen on the given address
 *	NetEndPoint::CheaterCheck() - check if supplied port is the licensed one
 *	NetEndPoint::Unlisten() - cancel Listen()
 *	NetEndPoint::Transport() - return an appropriate NetTransport
 *	NetEndPoint::GetListenAddress() - return address suitable for Listen()
 *
 *	NetTransport::Accept() - accept a single incoming connection
 *	NetTransport::Connect() - make a single outgoing connection
 *	NetTransport::Send() - send stream data
 *	NetTransport::Receive() - receive stream data
 *	NetTransport::Close() - close connection
 *
 *	NetTransport::GetAddress() - return connection's local address
 *	NetTransport::GetPeerAddress() - return address of the peer
 *	NetTransport::GetBuffering() - return transport level send buffering
 */

# ifndef __NETCONNECT_H__
# define __NETCONNECT_H__

# include <error.h>

# define RAF_NAME 0x01	// get symbolic name
# define RAF_PORT 0x02	// append port number

class KeepAlive;
class NetTransport;

class NetEndPoint {

    public:
	static NetEndPoint *	Create( const char *addr, Error *e );
	StrPtr &		GetAddress() { return service; }

	virtual			~NetEndPoint();

	virtual StrPtr		*GetListenAddress( int raf_flags ) = 0;

	virtual void		Listen( Error *e ) = 0;
	virtual void		ListenCheck( Error *e ) = 0;
	virtual int		CheaterCheck( const char *port ) = 0;
	virtual void		Unlisten() = 0;

	virtual NetTransport *	Connect( Error *e ) = 0;
	virtual NetTransport *	Accept( Error *e ) = 0;

	virtual int 		IsSingle() = 0;

    protected:

	StrBuf			service;	// endpoint name

} ;

class NetTransport : public KeepAlive {

    public:
	virtual		~NetTransport();

	virtual StrPtr *GetAddress( int raf_flags ) = 0;
	virtual StrPtr *GetPeerAddress( int raf_flags ) = 0;

	virtual void	Send( char *buffer, int length, Error *e ) = 0;
	virtual int	Receive( char *buffer, int length, Error *e ) = 0;
	virtual void	Close() = 0;
	virtual void	SetBreak( KeepAlive *breakCallback ) = 0;
	virtual int	GetBuffering() = 0;

	// DO NOT USE -- experimental only!

	virtual int	GetFd() { return -1; }

} ;

# endif // # ifndef __NETCONNECT_H__
