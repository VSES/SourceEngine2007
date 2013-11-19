// stdafx.cpp : source file that includes just the standard includes
//	MapImport.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

void W2AHelper( LPSTR out_sz, LPCWSTR in_wcs, int in_cch )
{
	if ( out_sz != NULL && 0 != in_cch )
	{
		out_sz[0] = '\0' ;

		if ( in_wcs != NULL )
		{
			size_t l_iLen = 0;
			l_iLen = ::wcstombs( out_sz, in_wcs, ( in_cch < 0 ) ? ::wcslen(in_wcs) + 1 : (size_t) in_cch ) ;

			if ( in_cch > 0 ) { out_sz[ in_cch - 1 ] = '\0'; }
		}
	}
}

//////////////////////////////////////////////////////////////////////
// Helper functions
//////////////////////////////////////////////////////////////////////
char * GetNextToken(char *in_buffer, const char *in_separators)
{
	static const char * s_commentStr = "//";
	static const char * s_newLineStr = "\r\n";

	// This is to replace the strtok() use in the plugin so that it 
	// will skip over lines that have comments in them.
	char * l_token = strtok(in_buffer, in_separators);

	// check for "//" in the string... anything found after this until the 
	// next newline is a comment
	char * l_comment = strstr(l_token, s_commentStr);
	if (l_comment == NULL)
	{
		// there was no comment in this token
		return l_token;
	}
	else if (l_comment == l_token)
	{
		// the comment is the whole	token (right at	the	start)
		// so advance to the end of	the	line
		// Make	sure that we only skip to the end of the line
		char *pNextChar	= l_token +	strlen(	l_token	) +	1;
		if ( *pNextChar	!= '\r'	&& *pNextChar != '\n' )
		{
			strtok(NULL, s_newLineStr);
		}

		l_token	= GetNextToken(NULL, in_separators);
		return l_token;
	}
	else
	{
		// the comment was found in the middle of a token so it's something like this:
		// --> here_is_the_start_of_my_token//then_a_comment_right_in_the_middle
		// first we need to truncate the token we're about the return at the comment
		// then read on until the next end of line
		l_token[l_comment - l_token] = '\0';
		return l_token;
	}
}