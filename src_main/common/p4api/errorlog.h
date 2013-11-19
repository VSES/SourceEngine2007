/*
 * Copyright 1995, 1996 Perforce Software.  All rights reserved.
 */

/*
 * ErrorLog.h - report layered errors
 *
 * Class Defined:
 *
 *	ErrorLog - write errors to log/syslog (static)
 *
 * Public methods:
 *
 *	ErrorLog::Report() - blurt out the contents of the Error to stderr
 *	ErrorLog::Abort() - blurt out an error and exit
 *	ErrorLog::Fmt() - format an error message
 *
 *	ErrorLog::SetLog() - redirect Abort() and Report() to named file
 *	ErrorLog::SetTag() - replace standard tag used by Report()
 *
 *	ErrorLog::SetSyslog() - redirect error messages to syslog on UNIX.
 *	ErrorLog::UnsetSyslog() - Cancel syslog redirection. Revert to log file.
 */

/*
 * class ErrorLog - write errors to log/syslog
 */

class ErrorLog {

    public:
	static void	Abort( const Error *e );
	static void	Report( const Error *e );

	// Global settings

	static void	SetLog( const char *file );
	static void	SetSyslog() { useSyslog = 1; }
	static void	UnsetSyslog() { useSyslog = 0; }
	static void	SetTag( const char *tag ) { errorTag = tag; }

    private:

	static const char *errorTag;
	static void	*errorLog;
	static int	useSyslog;

} ;

/*
 * AssertError() - in case you need a global error to Abort() on
 */

extern Error AssertError;
