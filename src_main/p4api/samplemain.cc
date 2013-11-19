/*
 * samplemain.cc - a p4 client API example
 *
 * This file is part of the p4api.tar distribution.
 *
 * This barebones example simply mimics the regular p4 command line
 * program.  
 *
 * Generally, compiling with the C++ compiler and linking with the
 * three provided libraries is sufficient to build this sample program.
 * 
 * See the Perforce Client API documentation (p4api.txt) at 
 * www.perforce.com/perforce/technical.html for further information.
 *
 * $Id: //depot/r04.2/p4/tests/samplemain.cc#1 $
 */

# include "clientapi.h"

int main( int argc, char **argv );
int main( int argc, char **argv )
{
	ClientUser ui;
	ClientApi client;
	StrBuf msg;
	Error e;

	// Any special protocol mods

	// client.SetProtocol( "tag" );

	// Connect to server

	client.Init( &e );

	if( e.Test() )
	{
	    e.Fmt( &msg );
	    fprintf( stderr, "%s\n", msg.Text() );
	    return 1;
	}

	// Run the command "argv[1] argv[2...]"

	client.SetArgv( argc - 2, argv + 2 );
	client.Run( argv[1], &ui );

	// Close connection

	client.Final( &e );

	if( e.Test() )
	{
	    e.Fmt( &msg );
	    fprintf( stderr, "%s\n", msg.Text() );
	    return 1;
	}
	
	return 0;
}
