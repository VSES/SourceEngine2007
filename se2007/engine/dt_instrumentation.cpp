//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//
#include "dt_instrumentation.h"
#include "utlvector.h"
#include "utllinkedlist.h"
#include "tier0/fasttimer.h"
#include "utllinkedlist.h"
#include "tier0/dbg.h"
#include "tier1/utlstring.h"
#include "dt_recv_decoder.h"
#include "filesystem.h"
#include "filesystem_engine.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

bool g_bDTIEnabled = false;
const char *g_pDTIFilename = NULL;


class CDTIProp
{
public:
	CDTIProp()
	{
		m_nDecodes = m_nDataBits = m_nIndexBits = 0;
	}

	CUtlString m_Name;
	int m_nDecodes;
	int m_nDataBits;
	int m_nIndexBits;
};


class CDTIRecvTable
{
public:
	CDTIRecvTable()
	{
		m_bSawAction = false;
	}

	CUtlString m_Name;
	CUtlVector<CDTIProp> m_Props;
	bool m_bSawAction;
};


CUtlLinkedList<CDTIRecvTable*, int> g_DTIRecvTables;


void DTI_Init( const char *pDTIFilename )
{
	g_bDTIEnabled = true;
	g_pDTIFilename = pDTIFilename;
}


void DTI_Term()
{
	DTI_Flush();
	g_DTIRecvTables.PurgeAndDeleteElements();
}


void DTI_Flush()
{
	if ( !g_bDTIEnabled )
		return;

	FileHandle_t fp = g_pFileSystem->Open( g_pDTIFilename, "wt" );
	if( fp != FILESYSTEM_INVALID_HANDLE )
	{
		// Write the header.
		g_pFileSystem->FPrintf( fp,
			"Class"
			"\tProp"
			
			"\tDecode Count"
			
			"\tTotal Bits"
			"\tAvg Bits"
			
			"\tTotal Index Bits"
			"\tAvg Index Bits"
			"\n" );
	
		FOR_EACH_LL( g_DTIRecvTables, iTable )
		{
			CDTIRecvTable *pTable = g_DTIRecvTables[iTable];
			
			if ( !pTable->m_bSawAction )
				continue;
			
			for ( int iProp=0; iProp < pTable->m_Props.Count(); iProp++ )
			{
				CDTIProp *pProp = &pTable->m_Props[iProp];

				if ( pProp->m_nDecodes == 0 )
					continue;
			
				g_pFileSystem->FPrintf( fp,
					// Class/Prop names
					"%s"
					"\t%s"
					
					// Decode count
					"\t%d"

					// Total/Avg bits
					"\t%d"
					"\t%.3f"

					// Total/Avg index bits
					"\t%d"
					"\t%.3f"

					"\n",
					
					// Class/Prop names
					pTable->m_Name.String(),
					pProp->m_Name.String(),

					// Decode count
					pProp->m_nDecodes,

					// Total/Avg bits
					pProp->m_nDataBits,
					(float)pProp->m_nDataBits / pProp->m_nDecodes,

					// Total/Avg index bits
					pProp->m_nIndexBits,
					(float)pProp->m_nIndexBits / pProp->m_nDecodes
					);
			}
		}

		g_pFileSystem->Close( fp );

		Msg( "DTI: wrote client stats into %s.\n", g_pDTIFilename );
	}
}


void DTI_HookRecvDecoder( CRecvDecoder *pDecoder )
{
	if ( !g_bDTIEnabled )
		return;

	CDTIRecvTable *pTable = new CDTIRecvTable;
	pTable->m_Name.Set( pDecoder->GetName() );
	
	pTable->m_Props.SetSize( pDecoder->GetNumProps() );
	for ( int i=0; i < pTable->m_Props.Count(); i++ )
	{
		const SendProp *pSendProp = pDecoder->GetSendProp( i );
		pTable->m_Props[i].m_Name.Set( pSendProp->GetName() );
	}
	
	g_DTIRecvTables.AddToTail( pTable );

	pDecoder->m_pDTITable = pTable;
}


void _DTI_HookDeltaBits( CRecvDecoder *pDecoder, int iProp, int nDataBits, int nIndexBits )
{
	CDTIRecvTable *pTable = pDecoder->m_pDTITable;
	if ( !pTable )
		return;

	CDTIProp *pProp = &pTable->m_Props[iProp];
	pProp->m_nDecodes++;
	pProp->m_nDataBits += nDataBits;
	pProp->m_nIndexBits += nIndexBits;

	pTable->m_bSawAction = true;
}



