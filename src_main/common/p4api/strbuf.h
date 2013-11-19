/*
 * Copyright 1995, 1996 Perforce Software.  All rights reserved.
 *
 * This file is part of Perforce - the FAST SCM System.
 */

/*
 * StrBuf.h - multipurpose buffers
 *
 * StrPtr, StrRef, and StrBuf are used throughout the system, as buffers
 * for storing just about any variable length byte data. 
 *
 * StrPtr is a low-cost (no constructor, no destructor, 8 byte) 
 * pointer/length pair to mutable data.  It has a variety of methods
 * to mangle it.
 *
 * StrRef is a kind-of StrPtr that allows the buffer pointer to be set.
 * As StrPtr doesn't allow this, a StrPtr object itself isn't useful.
 *
 * StrNum is a kind-of StrPtr with a temporary buffer whose only purpose
 * is to hold the string representation of an int.
 *
 * StrBuf is a kind-of StrPtr that allocates and extends it own buffer.
 *
 * Classes:
 *
 *	StrPtr - a pointer/length for arbitrary data
 *	StrRef - StrPtr that can be set
 *	StrBuf - StrPtr of privately allocated data
 *	StrNum - StrPtr that holds a string of an int
 *
 * Methods:
 *
 *	StrPtr::Clear() - set length = 0
 *	StrPtr::Text() - return buffer pointer
 *	StrPtr::Value() - return buffer pointer (old name)
 *	StrPtr::Length() - return buffer length
 *	StrPtr::GetEnd() - return pointer to character past end
 *	StrPtr::Atoi() - convert to integer and return
 *	StrPtr::SetLength() - set only length
 *	StrPtr::SetEnd() - set length by calculating from start
 *	StrPtr::[] - get a single character
 *	StrPtr::XCompare() - case exact string compare
 *	StrPtr::CCompare() - case folding string compare
 *	StrPtr::SCompare() - case aware string compare -- see strbuf.cc
 *	StrPtr::SEqual() - case aware character compare -- see strbuf.cc
 *	StrPtr::Contains() - finds a substring
 *	StrPtr::== - compare contents with buffer
 *	StrPtr::!= - compare contents with buffer
 *	StrPtr::< - compare contents with buffer
 *	StrPtr::<= - compare contents with buffer
 *	StrPtr::> - compare contents with buffer
 *	StrPtr::>= - compare contents with buffer
 *	StrPtr::StrCpy() - copy string out to a buffer
 *	StrPtr::StrCat() - copy string out to end of a buffer
 *	StrPtr::CaseFolding() - (static) SCompare sorts A < a, a < B
 *	StrPtr::CaseIgnored() - (static) SCompare sorts A == a, a < B
 *	StrPtr::CaseHybrid() - (static) SCompare sorts Ax < ax, aa < AX
 *	StrPtr::SetCaseFolding() - (static) 0=UNIX, 1=NT, 2=HYBRID
 *
 *	---
 *
 *	StrRef::Set() - set pointer/length
 *	StrRef::+= - move pointer/length
 *
 *	---
 *
 *	StrBuf::StringInit() - mimic actions of constructor
 *	StrBuf::Set() - allocate and fill from buffer
 *	StrBuf::Append() - extend and terminate from buffer
 *	StrBuf::Extend() - append contents from buffer
 *	StrBuf::Terminate() - terminate buffer
 *	StrBuf::Alloc() - allocate space in buffer and return pointer
 *	StrBuf::<< - Append contents from buffer or number
 *	StrBuf::Indent() - fill by indenting contents of another buffer
 *	StrBuf::Expand() - expand a string doing %var substitutions
 *	
 */

class StrBuf;

class StrPtr {

    public:
	// Setting, getting

	char *	Text() const
		{ return buffer; }

	char *	Value() const
		{ return buffer; }

	int 	Length() const
		{ return length; }

	char *	End() const
		{ return buffer + length; }

	int	Atoi() const
		{ return atoi( buffer ); }

	void	SetLength() 
		{ length = strlen( buffer ); }

	void	SetLength( int len ) 
		{ length = len; }

	void	SetEnd( char *p ) 
		{ length = p - buffer; }

	char	operator[]( int x ) const
		{ return buffer[x]; }

	// Compare -- p4ftp legacy

	int	Compare( const StrPtr &s ) const
		{ return SCompare( s ); }

	// CCompare/SCompare/XCompare

	int	CCompare( const StrPtr &s ) const
		{ return CCompare( buffer, s.buffer ); }

	int	SCompare( const StrPtr &s ) const
		{ return SCompare( buffer, s.buffer ); }

	static int CCompare( const char *a, const char *b );
	static int SCompare( const char *a, const char *b );
	static int SCompare( unsigned char a, unsigned char b );
	static int SEqualF( unsigned char a, unsigned char b );

	static int SEqual( unsigned char a, unsigned char b )
		{ 
		    switch( a^b ) 
		    { 
		    default: return 0;
		    case 0: return 1;
		    case 'A'^'a': return SEqualF( a, b );
		    }
		}

	int	SCompareN( const StrPtr &s ) const;

	int	XCompare( const StrPtr &s ) const
		{ return strcmp( buffer, s.buffer ); }

	static int XCompare( const char *a, const char *b )
		{ return strcmp( a, b ); }

	int	XCompareN( const StrPtr &s ) const
		{ return strncmp( buffer, s.buffer, length ); }

	// More comparing

	const char *Contains( const StrPtr &s ) const
		{ return strstr( Text(), s.Text() ); }

	int	operator ==( const char *buf ) const
		{ return strcmp( buffer, buf ) == 0; }

	int	operator !=( const char *buf ) const
		{ return strcmp( buffer, buf ) != 0; }

	int	operator <( const char *buf ) const
		{ return strcmp( buffer, buf ) < 0; }

	int	operator <=( const char *buf ) const
		{ return strcmp( buffer, buf ) <= 0; }

	int	operator >( const char *buf ) const
		{ return strcmp( buffer, buf ) > 0; }

	int	operator >=( const char *buf ) const
		{ return strcmp( buffer, buf ) >= 0; }

	int	operator ==( const StrPtr &s ) const
		{ return strcmp( buffer, s.buffer ) == 0; }

	int	operator !=( const StrPtr &s ) const
		{ return strcmp( buffer, s.buffer ) != 0; }

	int	operator <( const StrPtr &s ) const
		{ return strcmp( buffer, s.buffer ) < 0; }

	int	operator <=( const StrPtr &s ) const
		{ return strcmp( buffer, s.buffer ) <= 0; }

	int	operator >( const StrPtr &s ) const
		{ return strcmp( buffer, s.buffer ) > 0; }

	int	operator >=( const StrPtr &s ) const
		{ return strcmp( buffer, s.buffer ) >= 0; }

	// Copying out
	// Includes EOS

	void	StrCpy( char *b ) const
		{ memcpy( b, buffer, length + 1 ); }

	void	StrCat( char *b ) const
		{ memcpy( b + strlen( b ), buffer, length + 1 ); }


    friend class StrBuf;
    friend class StrRef;

    protected:
	char	*buffer;
	int	length;

    public:

	// Case sensitive server?

	static int CaseFolding()
		{ return caseUse != ST_UNIX; }

	static int CaseIgnored()
		{ return caseUse == ST_WINDOWS; }

	static int CaseHybrid()
		{ return caseUse == ST_HYBRID; }

	static void SetCaseFolding( int c )
		{ caseUse = (CaseUse)c; }

    private:

	enum CaseUse { ST_UNIX, ST_WINDOWS, ST_HYBRID };

	static CaseUse caseUse;

} ;

class StrRef : public StrPtr {

    public:

		StrRef() {}

		StrRef( const StrPtr &s )
		{ Set( &s ); }

		StrRef( const char *buf )
		{ Set( (char *)buf ); }

		StrRef( const char *buf, int len )
		{ Set( (char *)buf, len ); }

	static const StrPtr &Null()
		{ return null; }

	void	operator =(const StrPtr &s)
		{ Set( &s ); }

	void	operator =(const char *buf)
		{ Set( (char *)buf ); }

	void	operator +=( int l )
		{ buffer += l; length -= l; }

	void 	Set( char *buf )
		{ Set( buf, strlen( buf ) ); }
		 
	void	Set( char *buf, int len )
		{ buffer = buf; length = len; }

	void	Set( const StrPtr *s )
		{ Set( s->buffer, s->length ); }

	void	Set( const StrPtr &s )
		{ Set( s.buffer, s.length ); }

    private:
    	static	StrRef null;

} ;

class StrBuf : public StrPtr {

    public:
		StrBuf() 
		{ StringInit(); }

	void	StringInit()
		{ length = size = 0; buffer = nullStrBuf; }

		~StrBuf()
		{ if( buffer != nullStrBuf ) delete []buffer; }

	// copy constructor, assignment

		StrBuf( const StrBuf &s )
		{ StringInit(); Set( &s ); }

	void	operator =(const StrBuf &s)
		{ Set( &s ); }

	void	operator =(const StrRef &s)
		{ Set( &s ); }

	void	operator =(const StrPtr &s)
		{ Set( &s ); }

	void	operator =(const char *buf)
		{ Set( buf ); }

	// Setting, getting

	void 	Clear( void )
		{ length = 0; }

	void	Set( const char *buf )
		{ Clear(); Append( buf ); }

	void	Set( const StrPtr *s )
		{ Clear(); Append( s ); }

	void	Set( const StrPtr &s )
		{ Clear(); Append( &s ); }

	void	Set( const char *buf, int len )
		{ Clear(); Append( buf, len ); }

	void	Extend( const char *buf, int len )
		{ memcpy( Alloc( len ), buf, len ); }

	void	Extend( char c )
		{ *Alloc(1) = c; }

	void 	Terminate() 
		{ Extend(0); --length; }

	void	Append( const char *buf );

	void	Append( const StrPtr *s );

	void	Append( const char *buf, int len );

	char *	Alloc( int len )
		{
		    int oldlen = length;

		    if( ( length += len ) > size )
			Grow( oldlen );

		    return buffer + oldlen;
		}

	// string << -- append string/number

	StrBuf& operator <<( const char *s )
		{ Append( s ); return *this; }

	StrBuf& operator <<( const StrPtr *s )
		{ Append( s ); return *this; }

	StrBuf& operator <<( const StrPtr &s )
		{ Append( &s ); return *this; }

	StrBuf& operator <<( int v );

    private:
	int	size;

	void	Grow( int len );

	static char nullStrBuf[];
} ;

class StrNum : public StrPtr {

    public:
		StrNum() {} 

		StrNum( int v ) 
		{ Set( v ); }

		StrNum( int ok, int v )
		{ if( ok ) Set( v ); else buffer = buf, length = 0; }

	void	Set( int v );

    private:
		char buf[24];

} ;
