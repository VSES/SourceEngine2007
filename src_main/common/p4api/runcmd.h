/*
 * Copyright 1995, 2003 Perforce Software.  All rights reserved.
 *
 * This file is part of Perforce - the FAST SCM System.
 */

/*
 * RunCommand() -- Just run a command and capture its output
 *
 * RunCommand is a StrBuf that can run its contents as an OS command.
 * It has static members for passing in a command wholesale, and methods
 * for quoting args to protect them from the shell.
 *
 * Except where notes, these are implemented for UNIX, NT, VMS, MAC,
 * MACOSX, and OS2.
 *
 * Public methods:
 *	RunCommand::Run() - run the command
 *			Used by the client for launching editor, diff.
 *
 *	RunCommand::RunOut() - run the command, capturing stdout
 *			Used to run triggers for 'p4 submit'.
 *			Not implemented for MAC, VMS.
 *
 *	RunCommand::RunInWindow() - create a window to run the command
 *			Used by p4web for launching editor, resolve.
 *			Not implemented for MAC, VMS.
 *
 *	RunCommand::RunChild() - launch a subprocess whose stdin/stdout
 *			are the given fds.
 *			
 *	RunCommand::AddArg() - add a single argument
 *	RunCommand::SetArgs() - clear the command buffer and add args
 */

class RunCommand : public StrBuf {

    public:
	static int Run( const StrPtr &cmd, Error *e );
	static int RunOut( const StrPtr &cmd, StrBuf &result, Error *e );
	static int RunInWindow( const StrPtr &cmd, Error *e );
	static int RunChild( const StrPtr &cmd, int rfd, int wfd, Error *e );

	int	Run( Error *e ) 
		{ return Run( *this, e ); }

	int	RunOut( StrBuf &r, Error *e ) 
		{ return RunOut( *this, r, e ); }

	int 	RunInWindow( Error *e ) 
		{ return RunInWindow( *this, e ); }

	int	RunChild( int rfd, int wfd, Error *e ) 
		{ return RunChild( *this, rfd, wfd, e ); }

	void	SetArgs( int argc, const char **argv );
	void	AddArg( const char *arg );

} ;

