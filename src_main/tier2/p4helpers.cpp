//====== Copyright c 1996-2007, Valve Corporation, All rights reserved. =======//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//

#include "p4helpers.h"
#include "tier2/tier2.h"
#include "p4lib/ip4.h"

//////////////////////////////////////////////////////////////////////////
//
// CP4File implementation
//
//////////////////////////////////////////////////////////////////////////

CP4File::CP4File( char const *szFilename ) :
	m_sFilename( szFilename )
{
}

CP4File::~CP4File()
{
}

bool CP4File::Edit( void )
{
	return p4->OpenFileForEdit( m_sFilename.String() );
}

bool CP4File::Add( void )
{
	return p4->OpenFileForAdd( m_sFilename.String() );
}

// Is the file in perforce?
bool CP4File::IsFileInPerforce()
{
	return p4->IsFileInPerforce( m_sFilename.String() );
}


//////////////////////////////////////////////////////////////////////////
//
// CP4Factory implementation
//
//////////////////////////////////////////////////////////////////////////


CP4Factory::CP4Factory()
{
}

CP4Factory::~CP4Factory()
{
}

bool CP4Factory::SetDummyMode( bool bDummyMode )
{
	bool bOld = m_bDummyMode;
	m_bDummyMode = bDummyMode;
	return bOld;
}

void CP4Factory::SetOpenFileChangeList( const char *szChangeListName )
{
	if ( !m_bDummyMode )
		p4->SetOpenFileChangeList( szChangeListName );
}

CP4File *CP4Factory::AccessFile( char const *szFilename ) const
{
	if ( !m_bDummyMode )
		return new CP4File( szFilename );
	else
		return new CP4File_Dummy( szFilename );
}


// Default p4 factory
static CP4Factory s_static_p4_factory;
extern CP4Factory *g_p4factory = &s_static_p4_factory; // NULL before the factory constructs

