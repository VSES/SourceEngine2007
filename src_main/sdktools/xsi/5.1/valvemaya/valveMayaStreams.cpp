//======= Copyright © 1996-2006, Valve Corporation, All rights reserved. ======
//
// Purpose:
//
//=============================================================================

// Maya includes

#include <maya/MGlobal.h>
#include <maya/MString.h>

// Valve includes

#include "valveMaya.h"

//-----------------------------------------------------------------------------
//
// Purpose: A iostream streambuf which puts stuff in the Maya GUI
//
//-----------------------------------------------------------------------------

class CmayaStreamBuf : public std::streambuf
{
public:
	enum StreamType { kInfo, kWarning, kError };

	CmayaStreamBuf( const StreamType i_streamType = kInfo )
	: m_streamType( i_streamType )
	{}

	virtual int sync();
	virtual int overflow( int );
	virtual int underflow() { return EOF; }

protected:
	void outputString();

	StreamType m_streamType;
	MString m_string;
};

int
CmayaStreamBuf::sync()
{
	const int n = pptr() - pbase();

	if ( pbase() && n )
	{
		const MString mString( pbase(), n );

		switch (m_streamType) {
		case kWarning:
			MGlobal::displayWarning(mString);
			break;
		case kError:
			MGlobal::displayError(mString);
			break;
		default:
			MGlobal::displayInfo(mString);
			break;
		}
	}

	return n;
}

int
CmayaStreamBuf::overflow(int ch)
{
	const int n = pptr() - pbase();

	if ( n && sync() )
		return EOF;

	if ( ch != EOF )
	{
		if ( ch == '\n' )
			outputString();
		else {
			const char cCh( ch );
			const MString mString( &cCh, 1 );

			m_string += mString;
		}
	}

	pbump( -n );

	return 0;
}

void
CmayaStreamBuf::outputString()
{
	switch ( m_streamType )
	{
	case kWarning:
		MGlobal::displayWarning( m_string );
		break;
	case kError:
		MGlobal::displayError( m_string );
		break;
	default:
		MGlobal::displayInfo( m_string );
		break;
	}

  m_string.clear();
}

static CmayaStreamBuf minfoBuf( CmayaStreamBuf::kInfo );
static CmayaStreamBuf mwarnBuf( CmayaStreamBuf::kWarning );
static CmayaStreamBuf merrBuf( CmayaStreamBuf::kError );

std::ostream minfo(&minfoBuf);
std::ostream mwarn(&mwarnBuf);
std::ostream merr(&merrBuf);