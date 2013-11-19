/*
 * Copyright 1995, 2000 Perforce Software.  All rights reserved.
 *
 * This file is part of Perforce - the FAST SCM System.
 */

/*
 * ClientUser -- default implementation of user interface for p4 client
 *
 * This file implements the p4 client command line interface by defining
 * the methods of the ClientUser class.  This interface reads from stdin,
 * writes to stdout and stderr, and invokes the user's editor.
 */

# define NEED_FILE

# ifdef OS_NT
# include <process.h>
# define NEED_FCNTL
# ifdef __BORLANDC__
# define ARGVCAST(x)	((char * const *)(x))
# else
# define ARGVCAST(x)	(x)
# endif
# endif

# include <clientapi.h>

# include <diff.h>
# include <enviro.h>
# include <echoctl.h>
# include <signaler.h>
# include <strops.h>
# include <runcmd.h>

# include <msgclient.h>

# if defined ( OS_MAC ) || defined ( OS_MACOSX )
# include "clientusermac.h"
# endif

/*
 * ClientUser::~ClientUser() - virtual destructor holder
 */

ClientUser::~ClientUser()
{
}

/*
 * ClientUser::Prompt() - prompt the user, and wait for a response.
 */

void
ClientUser::Prompt( const StrPtr &msg, StrBuf &buf, int noEcho, Error *e )
{
	printf( msg.Text() );
# ifdef OS_MAC
	fputc( '\n', stdout );
# endif

	fflush( stdout );
	fflush( stdin );

	// Turn off echoing

	NoEcho *setEcho = noEcho ? new NoEcho : 0;

	// Prompt'em

	buf.Clear();
	char *b = buf.Alloc( 1024 );

	if( !fgets( b, 1024, stdin ) )
	{
	    e->Set( MsgClient::Eof );
	    buf.SetEnd( b );
	}
	else
	{
	    // Set end and truncate any newline

	    buf.SetEnd( b += strlen( b ) );
	    
# ifdef USE_CR
	    /* 
 	     * Codewarrior (7.0?) oddity: 
 	     *	fprintf to file translates \n (10) -> \r.
 	     *	fgets from file translates \n (10) <- \r.
 	     *	fprintf to console translates \n (10) -> newline on screen.
 	     *	fgets from stdin DOES NOT translate "enter".
 	     */
	     
	    if( buf.Length() && buf.End()[ -1 ] == '\r' )
	    	buf.End()[ -1 ] = '\n';
# endif

	    if( buf.Length() && buf.End()[ -1 ] == '\n' )
	    {
		buf.SetEnd( buf.End() - 1 );
		buf.Terminate();
	    }
	}

	delete setEcho;
}

void
ClientUser::Help( const_char *const *help )
{
	for( ; *help; help++ )
	    printf( "%s\n", *help );
}

void
ClientUser::HandleError( Error *err )
{
	// Dumb implementation is just to format 
	// the error and call (old) OutputError.

	// API callers against 99.1+ servers can do better by
	// having HandleError probe 'err' directly for detail.

	StrBuf buf;
	err->Fmt( buf, EF_NEWLINE );
	OutputError( buf.Text() );
}

void
ClientUser::OutputError( const_char *errBuf )
{
	fflush( stdout );
# ifdef OS_VMS
	// Note - VMS likes the args this way,  Mac the other,
	// UNIX doesn't care.
	//
	fwrite( errBuf, strlen( errBuf ), 1, stderr );
# else
	fwrite( errBuf, 1, strlen( errBuf ), stderr );
# endif
}

void
ClientUser::OutputInfo( char level, const_char *data )
{
	switch( level )
	{
	default:
	case '0': break;
	case '1': printf( "... " ); break;
	case '2': printf( "... ... " ); break;
	}

# ifdef OS_VMS
	// Note - VMS likes the args this way,  Mac the other,
	// UNIX doesn't care.
	//
	fwrite( data, strlen( data ), 1, stdout );
# else
	fwrite( data, 1, strlen( data ), stdout );
# endif
	fputc( '\n', stdout );
}

void
ClientUser::OutputText( const_char *data, int length )
{

# ifdef OS_VMS
	// Note - VMS likes the args this way,  Mac the other,
	// UNIX doesn't care.
	//
	fwrite( data, length, 1, stdout );
# else
	fwrite( data, 1, length, stdout );
# endif
}

void
ClientUser::OutputBinary( const_char *data, int length )
{
# ifdef OS_NT
	// we rely on a trailing zero length buffer to
	// tell us to turn off binary output.

	if( binaryStdout == !length )
	{
	    // toggle
	    binaryStdout = !!length;
	    fflush( stdout );
	    setmode( fileno( stdout ), binaryStdout ? O_BINARY : O_TEXT );
	}
# endif

	fwrite( data, 1, length, stdout );
}

void
ClientUser::InputData( StrBuf *buf, Error *e )
{
	int n;

	buf->Clear();

	do {
# ifdef OS_MAC
	    // CW PRO 5 fread/read from stdin hangs machine
	    char *b = buf->Alloc( FileBufferSize + 1 );
	    n = fgets( b, FileBufferSize + 1, stdin ) ? strlen( b ) : 0;
# else
	    char *b = buf->Alloc( FileBufferSize );
	    n = read( 0, b, FileBufferSize);
# endif
	    buf->SetEnd( b + n );
	} while( n > 0 );

	buf->Terminate();
}

void
ClientUser::OutputStat( StrDict *varList )
{
	int i;
	StrBuf msg;
	StrRef var, val;

	// Dump out the variables, using the GetVar( x ) interface.
	// Don't display the function (duh), which is only relevant to rpc.

	for( i = 0; varList->GetVar( i, var, val ); i++ )
	{
	    if( var == "func" ) continue;

	    // otherAction and otherOpen go at level 2, as per 99.1 + earlier

	    msg.Clear();
	    msg << var << " " << val;
	    char level = strncmp( var.Text(), "other", 5 ) ? '1' : '2';
	    OutputInfo( level, msg.Text() );
	}

	// blank line

	OutputInfo( '0', "" );
}

void
ClientUser::ErrorPause( char *errBuf, Error *e )
{
	StrBuf buf;
	OutputError( errBuf );
# if defined( OS_MAC )
	Prompt( StrRef( "Hit enter to continue..." ), buf, 0, e );
# else
	Prompt( StrRef( "Hit return to continue..." ), buf, 0, e );
# endif
}

void
ClientUser::Edit( FileSys *f1, Error *e )
{
	Edit( f1, enviro, e );
}

void
ClientUser::Edit( FileSys *f1, Enviro * env, Error *e )
{
	// The presence of $SHELL on NT implies MKS, which has vi.
	// On VMS, we look for POSIX$SHELL - if set, we'll use vi.
	// Otherwise, EDIT.

	if( !f1->IsTextual() )
	{
	    e->Set( MsgClient::CantEdit ) << f1->Name();
	    return;
	}

# if defined( OS_MAC ) || defined ( OS_MACOSX )
	ClientUserMacEdit( f1, env, e, true );
# else
	StrBuf n;
	const char *editor;

	if( !( editor = env->Get( "P4EDITOR" ) ) )
	    editor = getenv( "EDITOR" );

# ifdef OS_NT
	if( !editor && !getenv( "SHELL" ) )
	    editor = "notepad";
# endif

# ifdef OS_VMS
	if( !editor && !getenv( "POSIX$SHELL" ) )
	    editor = "edit";
# endif

# ifdef OS_AS400
  if( !editor )
      editor = "edtf";
# endif

	// Bill Joy rules, Emacs maggots!

	if( !editor )
	    editor = "vi";

	RunCmd( editor, f1->Name(), 0, 0, 0, 0, e );

# endif /* OS_MAC */
}

void
ClientUser::Diff( FileSys *f1, FileSys *f2, int doPage, char *df, Error *e )
{
	if( !f1->IsTextual() || !f2->IsTextual() )
	{
	    if( f1->Compare( f2, e ) )
		printf( "(... files differ ...)\n" );
	    return;
	}

	// Call diff to do text compare

	const char *diff;
	const char *pager;

	if( !( diff = enviro->Get( "P4DIFF" ) ) )
	    diff = getenv( "DIFF" );

	if( !doPage )
	    pager = 0;
	else if( !( pager = enviro->Get( "P4PAGER" ) ) )
	    pager = getenv( "PAGER" );

	// Do our own diff
	// VMS ReadFile is busted.

# ifdef OS_VMS

	if( !diff )
	    diff = "diff";

# else /* not VMS */

	if( !diff )
	{
	    // diff expects to read files in raw mode, we must 
	    // create new FileSys to allow this.

	    FileSys *f1_bin = FileSys::Create( FST_BINARY );
	    f1_bin->Set( f1->Name() );

	    FileSys *f2_bin = FileSys::Create( FST_BINARY );
	    f2_bin->Set( f2->Name() );
	    
	    if( pager )
	    {
		FileSys *t = FileSys::CreateGlobalTemp( f1->GetType() );
		{
# ifndef OS_NEXT
		    ::
# endif
		    Diff d;
		    DiffFlags flags( df );
		    d.SetInput( f1_bin, f2_bin, flags, e );
		    if( !e->Test() ) d.SetOutput( t->Name(), e );
		    if( !e->Test() ) d.DiffWithFlags( flags );
		    d.CloseOutput( e );
		}

		if( !e->Test() ) RunCmd( pager, t->Name(), 0, 0, 0, 0, e );

		delete t;
	    }
	    else
	    {
# ifndef OS_NEXT
		::
# endif
	    	Diff d;
		DiffFlags flags( df );

		d.SetInput( f1_bin, f2_bin, flags, e );
		if( !e->Test() ) d.SetOutput( stdout );
		if( !e->Test() ) d.DiffWithFlags( flags );
		d.CloseOutput( e );
	    }

	    delete f1_bin;
	    delete f2_bin;
	    return;
	}

# endif /* VMS */

	// Build up flags args
	// Yuk.

	if( !df || !*df )
	{
	    RunCmd( diff, f1->Name(), f2->Name(), 0, 0, pager, e );
	}
	else
	{
	    StrBuf flags;
	    flags.Set( "-", 1 );
	    flags << df;
	    RunCmd( diff, flags.Text(), f1->Name(), f2->Name(), 0, pager, e );
	}
}

void
ClientUser::Merge( 
	FileSys *base,
	FileSys *leg1,
	FileSys *leg2,
	FileSys *result,
	Error *e )
{
	char *merger;
	
	if( !( merger = enviro->Get( "P4MERGE" ) ) && 
	    !( merger = getenv( "MERGE" ) ) )
	{
	    e->Set( MsgClient::NoMerger );
	    return;
	}

	RunCmd( merger, base->Name(), leg1->Name(), 
		leg2->Name(), result->Name(), 0, e );
}

void
ClientUser::RunCmd(
	const char *command,
	const char *arg1,
	const char *arg2,
	const char *arg3,
	const char *arg4,
	const char *pager,
	Error *e )
{
	fflush( stdout );

	signaler.Block();	// reset SIGINT to SIGDFL

	RunCommand cmd;

	cmd << command;
	if( arg1 ) cmd.AddArg( arg1 );
	if( arg2 ) cmd.AddArg( arg2 );
	if( arg3 ) cmd.AddArg( arg3 );
	if( arg4 ) cmd.AddArg( arg4 );
	if( pager ) cmd << " | " << pager;

# ifdef OS_MAC
	ToolServerExec( cmd.Text(), e );
# else 
	cmd.Run( e );
# endif 

	signaler.Catch();	// catch SIGINT again
}

FileSys *
ClientUser::File( FileSysType type )
{
	return FileSys::Create( type );
}

int
ClientUser::Resolve( ClientMerge *m, Error *e )
{
	// pun from ClientMergeStatus
	return (int)m->Resolve( e );
}

void
ClientUser::Message( Error *err )
{
	if( err->IsInfo() )
	{
	    // Info
	    StrBuf buf;
	    err->Fmt( buf, EF_PLAIN );
	    OutputInfo( err->GetGeneric() + '0', buf.Text() );
	}
	else
	{
	    // warn, failed, fatal
	    HandleError( err );
	}
}
