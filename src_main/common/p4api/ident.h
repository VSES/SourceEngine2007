/*
 * Copyright 1995, 1996 Perforce Software.  All rights reserved.
 *
 * This file is part of Perforce - the FAST SCM System.
 */

# define IdentMagic "@(#)"

class Ident {

    public:
	void		GetMessage( StrBuf *s, int isServer = 0 );
	const char *	GetIdent() { return ident + sizeof( IdentMagic ) - 1; }
	const char *	GetDate() { return supportDate; }

    // private, but statically initialized

	const char	*ident;
	const char	*supportDate;

};
