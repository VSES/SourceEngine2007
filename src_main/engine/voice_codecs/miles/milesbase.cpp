//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#include "milesbase.h"
#include "tier0/dbg.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

static int s_MilesRefCount = 0;


void IncrementRefMiles()
{
	if(s_MilesRefCount == 0)
	{
		AIL_set_redist_directory("." MSS_REDIST_DIR_NAME);
		AIL_startup();
	}
	
	++s_MilesRefCount;
}

void DecrementRefMiles()
{
	Assert(s_MilesRefCount > 0);
	--s_MilesRefCount;
	if(s_MilesRefCount == 0)
	{
		CProvider::FreeAllProviders();
		AIL_shutdown();
	}
}


// ------------------------------------------------------------------------ //
// CProvider functions.
// ------------------------------------------------------------------------ //

CProvider *CProvider::s_pHead = NULL;


CProvider::CProvider( HPROVIDER hProvider )
{
	m_hProvider = hProvider;

	// Add to the global list of CProviders.
	m_pNext = s_pHead;
	s_pHead = this;
}


CProvider::~CProvider()
{
	RIB_free_provider_library( m_hProvider );

	// Remove from the global list.
	CProvider **ppPrev = &s_pHead;
	for ( CProvider *pCur=s_pHead; pCur; pCur=pCur->m_pNext )
	{
		if ( pCur == this )
		{
			*ppPrev = m_pNext;
			return;
		}

		ppPrev = &pCur->m_pNext;
	}
}


CProvider* CProvider::FindProvider( HPROVIDER hProvider )
{
	for ( CProvider *pCur=s_pHead; pCur; pCur=pCur->m_pNext )
	{
		if ( pCur->GetProviderHandle() == hProvider )
		{
			return pCur;
		}
	}
	return NULL;
}


void CProvider::FreeAllProviders()
{
	CProvider *pNext;
	for ( CProvider *pCur=s_pHead; pCur; pCur=pNext )
	{
		pNext = pCur->m_pNext;
		delete pCur;
	}
}


HPROVIDER CProvider::GetProviderHandle()
{
	return m_hProvider;
}


// ------------------------------------------------------------------------ //
// ASISTRUCT functions.
// ------------------------------------------------------------------------ //
ASISTRUCT::ASISTRUCT()
{
	Clear();
	IncrementRefMiles();
}

ASISTRUCT::~ASISTRUCT()
{
	Shutdown();
	DecrementRefMiles();
}


void ASISTRUCT::Clear()
{
	m_pProvider = NULL;
	ASI_stream_open = NULL;
	ASI_stream_process = NULL;
	ASI_stream_close = NULL;
	ASI_stream_attribute = NULL;
	ASI_stream_set_preference = NULL;
	ASI_stream_seek = NULL;
	OUTPUT_BITS = NULL;
	OUTPUT_CHANNELS = NULL;
	INPUT_BITS = NULL;
	INPUT_CHANNELS = NULL;
	POSITION = NULL;
	m_stream = NULL;
}


bool ASISTRUCT::Init( void *pCallbackObject, const char *pInputFileType, const char *pOutputFileType, AILASIFETCHCB cb )
{
	// Get the provider.
	HPROVIDER hProvider = RIB_find_files_provider( "ASI codec", 
		"Output file types", pOutputFileType, "Input file types", pInputFileType );

	if ( !hProvider )
		return false;

	m_pProvider = CProvider::FindProvider( hProvider );
	if ( !m_pProvider )
	{
		m_pProvider = new CProvider( hProvider );
	}

	if ( !m_pProvider )
		return false;

	// Get the decoder.
	RIB_INTERFACE_ENTRY RECVSTR[] =
	{
		{ RIB_FUNCTION,   "ASI_stream_attribute",		(U32) &ASI_stream_attribute,		RIB_NONE },
		{ RIB_FUNCTION,   "ASI_stream_open",			(U32) &ASI_stream_open,				RIB_NONE },
		{ RIB_FUNCTION,   "ASI_stream_close",			(U32) &ASI_stream_close,			RIB_NONE },
		{ RIB_FUNCTION,   "ASI_stream_process",			(U32) &ASI_stream_process,			RIB_NONE },
		{ RIB_FUNCTION,   "ASI_stream_set_preference",	(U32) &ASI_stream_set_preference,	RIB_NONE },
		{ RIB_ATTRIBUTE,  "Output sample rate",         (U32) &OUTPUT_RATE,					RIB_NONE },
		{ RIB_ATTRIBUTE,  "Output sample width",		(U32) &OUTPUT_BITS,					RIB_NONE },
		{ RIB_ATTRIBUTE,  "Output channels",			(U32) &OUTPUT_CHANNELS,				RIB_NONE },
		{ RIB_ATTRIBUTE,  "Input sample rate",          (U32) &INPUT_RATE,					RIB_NONE },
		{ RIB_ATTRIBUTE,  "Input channels",				(U32) &INPUT_CHANNELS,				RIB_NONE },
		{ RIB_ATTRIBUTE,  "Input sample width",			(U32) &INPUT_BITS,					RIB_NONE },
		{ RIB_ATTRIBUTE,  "Minimum input block size",	(U32) &INPUT_BLOCK_SIZE,			RIB_NONE },
		{ RIB_ATTRIBUTE,  "Position",					(U32) &POSITION,					RIB_NONE },
	};

	RIBRESULT result = RIB_request( m_pProvider->GetProviderHandle(), "ASI stream", RECVSTR );
	if(result != RIB_NOERR)
		return false;

	
	// This function doesn't exist for the voice DLLs, but it's not fatal in that case.
	RIB_INTERFACE_ENTRY seekFn[] = 
	{
		{ RIB_FUNCTION,   "ASI_stream_seek", (U32) &ASI_stream_seek,				RIB_NONE }
	};
	result = RIB_request( m_pProvider->GetProviderHandle(), "ASI stream", seekFn );
	if(result != RIB_NOERR)
		ASI_stream_seek = NULL;


	m_stream = ASI_stream_open((U32)pCallbackObject, cb, 0);
	if(!m_stream)
		return false;

	return true;
}

void ASISTRUCT::Shutdown()
{
	if ( m_pProvider )
	{
		if (m_stream && ASI_stream_close)
		{
			ASI_stream_close(m_stream);
			m_stream = NULL;
		}

		//m_pProvider->Release();
		m_pProvider = NULL;
	}

	Clear();
}


int ASISTRUCT::Process( void *pBuffer, unsigned int bufferSize )
{
	return ASI_stream_process( m_stream, pBuffer, bufferSize );
}


bool ASISTRUCT::IsActive() const
{
	return m_stream != NULL ? true : false;
}


unsigned int ASISTRUCT::GetAttribute( HATTRIB attribute )
{
	return ASI_stream_attribute( m_stream, attribute );
}

void ASISTRUCT::Seek( int position )
{
	if ( !ASI_stream_seek )
		Error( "ASI_stream_seek called, but it doesn't exist." );

	ASI_stream_seek( m_stream, (S32)position );
}

void ASISTRUCT::SetAttribute( HATTRIB attribute, unsigned int value )
{
	ASI_stream_set_preference( m_stream, attribute, &value );
}


