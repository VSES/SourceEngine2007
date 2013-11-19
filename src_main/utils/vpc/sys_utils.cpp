//========= Copyright © 1996-2006, Valve Corporation, All rights reserved. ============//
//
// Purpose: VPC
//
//=====================================================================================//

#include "sys_utils.h"

//-----------------------------------------------------------------------------
//	Sys_Error
//
//-----------------------------------------------------------------------------
void Sys_Error( const char* format, ... )
{
	va_list		argptr;
	char		msg[MAX_SYSPRINTMSG];
		
	va_start( argptr,format );
	vsprintf_s( msg, sizeof( msg ), format, argptr );
	va_end( argptr );

	exit( 1 );
}

//-----------------------------------------------------------------------------
//	Sys_LoadFile
//
//-----------------------------------------------------------------------------
int Sys_LoadFile( const char* filename, void** bufferptr, bool bText )
{
	int	handle;
	long	length;
	char*	buffer;

	*bufferptr = NULL;

	if ( !Sys_Exists( filename ) )
		return ( -1 );
		
	handle = _open( filename, _O_RDONLY|( bText ? _O_TEXT : _O_BINARY) );
	if ( handle == -1 )
		Sys_Error( "Sys_LoadFile(): Error opening %s: %s", filename, strerror( errno ) );

	length = Sys_FileLength( NULL, handle, bText );
	buffer = ( char* )malloc( length+1 );

	int bytesRead = _read( handle, buffer, length );
	if ( !bText && ( bytesRead != length ) )
		Sys_Error( "Sys_LoadFile(): read truncated failure" );

	_close( handle );

	// text mode is truncated, add null for parsing
	buffer[bytesRead] = '\0';

	*bufferptr = ( void* )buffer;

	return ( length );
}

//-----------------------------------------------------------------------------
//	Sys_FileLength
//
//-----------------------------------------------------------------------------
long Sys_FileLength( const char* filename, int handle, bool bText )
{
	long	pos;
	long	length;

	if ( filename )
	{
		handle = _open( filename, _O_RDONLY | ( bText ? O_TEXT : _O_BINARY ) );
		if ( handle == -1 )
		{
			// file does not exist
			return ( -1 );
		}

		length = _lseek( handle, 0, SEEK_END );
		_close( handle );
	}
	else if ( handle != -1 )
	{
		pos    = _lseek( handle, 0, SEEK_CUR );
		length = _lseek( handle, 0, SEEK_END );
		_lseek( handle, pos, SEEK_SET );
	}
	else
	{
		return ( -1 );
	}

	return ( length );
}

//-----------------------------------------------------------------------------
//	Sys_StripPath
//
//	Removes path portion from a fully qualified name, leaving filename and extension.
//-----------------------------------------------------------------------------
void Sys_StripPath( const char* inpath, char* outpath )
{
	const char*	src;

	src = inpath + strlen( inpath );
	while ( ( src != inpath ) && ( *( src-1 ) != '\\' ) && ( *( src-1 ) != '/' ) && ( *( src-1 ) != ':' ) )
		src--;

	strcpy( outpath,src );
}

//-----------------------------------------------------------------------------
//	Sys_Exists
//
//	Returns TRUE if file exists.
//-----------------------------------------------------------------------------
bool Sys_Exists( const char* filename )
{
   FILE*	test;

   if ( ( test = fopen( filename, "rb" ) ) == NULL )
      return ( false );

   fclose( test );

   return ( true );
}

//-----------------------------------------------------------------------------
//	Sys_SkipWhitespace
//
//-----------------------------------------------------------------------------
char* Sys_SkipWhitespace( char *data, bool *pHasNewLines, int* pNumLines ) 
{
	int c;

	while ( ( c = *data ) <= ' ' ) 
	{
		if ( c == '\n' ) 
		{
			if ( pNumLines )
			{
				(*pNumLines)++;
			}

			if ( pHasNewLines )
			{
				*pHasNewLines = true;
			}
		}
		else if ( !c )
		{
			return ( NULL );
		}

		data++;
	}

	return data;
}

//-----------------------------------------------------------------------------
//	Sys_SkipToValidToken
//
//-----------------------------------------------------------------------------
char *Sys_SkipToValidToken( char *data, bool *pHasNewLines, int* pNumLines ) 
{
	int c;

	for ( ;; )
	{
		data = Sys_SkipWhitespace( data, pHasNewLines, pNumLines );

		c = *data;
		if ( !c )
		{
			break;
		}

		if ( c == '/' && data[1] == '/' )
		{
			// skip double slash comments
			data += 2;
			while ( *data && *data != '\n' )
			{
				data++;
			}
			if ( *data && *data == '\n' )
			{
				data++;
				if ( pNumLines )
				{
					(*pNumLines)++;
				}
				if ( pHasNewLines )
				{
					*pHasNewLines = true;
				}
			}
		}
		else if ( c == '/' && data[1] == '*' ) 
		{
			// skip /* */ comments
			data += 2;
			while ( *data && ( *data != '*' || data[1] != '/' ) )
			{
				if ( *data == '\n' )
				{
					if ( pNumLines )
					{
						(*pNumLines)++;
					}
					if ( pHasNewLines )
					{
						*pHasNewLines = true;
					}
				}
				data++;
			}

			if ( *data ) 
			{
				data += 2;
			}
		}
		else
		{
			break;
		}
	}

	return data;
}

//-----------------------------------------------------------------------------
//	Sys_SkipBracedSection
//
//	The next token should be an open brace.
//	Skips until a matching close brace is found.
//	Internal brace depths are properly skipped.
//-----------------------------------------------------------------------------
void Sys_SkipBracedSection( char** dataptr, int* numlines ) 
{
	char*	token;
	int	depth;

	depth = 0;
	do 
	{
		token = Sys_GetToken( dataptr, true, numlines );
		if ( token[1] == '\0' ) 
		{
			if ( token[0] == '{' )
				depth++;
			else if ( token[0] == '}' )
				depth--;
		}
	}
	while( depth && *dataptr );
}

//-----------------------------------------------------------------------------
//	Sys_SkipRestOfLine
//
//-----------------------------------------------------------------------------
void Sys_SkipRestOfLine( char** dataptr, int* numlines ) 
{
	char*	p;
	int	c;

	p = *dataptr;
	while ( ( c = *p++ ) != '\0' ) 
	{
		if ( c == '\n' ) 
		{
			if ( numlines )
				( *numlines )++;
			break;
		}
	}
	*dataptr = p;
}

//-----------------------------------------------------------------------------
//	Sys_PeekToken
//
//-----------------------------------------------------------------------------
char* Sys_PeekToken( char *dataptr, bool bAllowLineBreaks )
{
	char	*saved;
	char	*pToken;

	saved  = dataptr;
	pToken = Sys_GetToken( &saved, bAllowLineBreaks, NULL );

	return pToken;
}

//-----------------------------------------------------------------------------
//	Sys_GetToken
//
//-----------------------------------------------------------------------------
char* Sys_GetToken( char** dataptr, bool allowLineBreaks, int* pNumLines )
{
	char		c;
	char		endSymbol;
	int			len;
	bool		hasNewLines;
	char*		data;
	static char	token[MAX_SYSTOKENCHARS];

	c           = 0;
	data        = *dataptr;
	len         = 0;
	token[0]    = 0;
	hasNewLines = false;

	// make sure incoming data is valid
	if ( !data )
	{
		*dataptr = NULL;
		return ( token );
	}

	for ( ;; )
	{
		// skip whitespace
		data = Sys_SkipWhitespace( data, &hasNewLines, pNumLines );
		if ( !data )
		{
			*dataptr = NULL;
			return ( token );
		}
		
		if ( hasNewLines && !allowLineBreaks )
		{
			*dataptr = data;
			return ( token );
		}

		c = *data;

		if ( c == '/' && data[1] == '/' )
		{
			// skip double slash comments
			data += 2;
			while ( *data && *data != '\n' )
				data++;
			if ( *data && *data == '\n' )
			{
				data++;
				if ( pNumLines )
					(*pNumLines)++;
			}
		}
		else if ( c =='/' && data[1] == '*' ) 
		{
			// skip /* */ comments
			data += 2;
			while ( *data && ( *data != '*' || data[1] != '/' ) )
			{
				if ( *data == '\n' && pNumLines )
					(*pNumLines)++;
				data++;
			}

			if ( *data ) 
				data += 2;
		}
		else
			break;
	}

	// handle scoped strings "???" <???> [???]
	if ( c == '\"' || c == '<' || c == '[')
	{
		endSymbol = '\0';
		switch ( c )
		{
		case '\"':
			endSymbol = '\"';
			break;
		case '<':
			endSymbol = '>';
			break;
		case '[':
			endSymbol = ']';
			break;
		}
		data++;
		for ( ;; )
		{
			c = *data++;

			if ( c == endSymbol || !c )
			{
				token[len] = 0;
				*dataptr = (char*)data;
				return ( token );
			}

			if ( len < MAX_SYSTOKENCHARS )
				token[len++] = c;
		}
	}

	// parse a regular word
	do
	{
		if ( len < MAX_SYSTOKENCHARS )
			token[len++] = c;

		data++;
		c = *data;
	} 
	while ( c > ' ' );

	if ( len >= MAX_SYSTOKENCHARS ) 
		len = 0;

	token[len] = '\0';
	*dataptr   = (char*)data;
	
	return token;
}