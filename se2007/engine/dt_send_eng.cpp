//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#include <tier0/dbg.h>
#include <tier0/vprof.h>
#include <tier0/icommandline.h>
#include <commonmacros.h>
#include <checksum_crc.h>

#include "dt_send_eng.h"
#include "dt_encode.h"
#include "dt_instrumentation_server.h"
#include "dt_stack.h"
#include "common.h"
#include "packed_entity.h"

// memdbgon must be the last include file in a .cpp file!!!
#include <tier0/memdbgon.h>

extern int host_framecount;
CRC32_t SendTable_ComputeCRC();

class CSendTablePrecalc;
class CSendNode;

extern bool Sendprop_UsingDebugWatch();


// This stack doesn't actually call any proxies. It uses the CSendProxyRecipients to tell
// what can be sent to the specified client.
class CPropCullStack : public CDatatableStack
{
public:
						CPropCullStack( 
							CSendTablePrecalc *pPrecalc, 
							int iClient, 
							const CSendProxyRecipients *pOldStateProxies,
							const int nOldStateProxies, 
							const CSendProxyRecipients *pNewStateProxies,
							const int nNewStateProxies
							) :
							
							CDatatableStack( pPrecalc, (unsigned char*)1, -1 ),
							m_pOldStateProxies( pOldStateProxies ),
							m_nOldStateProxies( nOldStateProxies ),
							m_pNewStateProxies( pNewStateProxies ),
							m_nNewStateProxies( nNewStateProxies )
						{
							m_pPrecalc = pPrecalc;
							m_iClient = iClient;
						}

	inline unsigned char*	CallPropProxy( CSendNode *pNode, int iProp, unsigned char *pStructBase )
	{
		if ( pNode->GetDataTableProxyIndex() == DATATABLE_PROXY_INDEX_NOPROXY )
		{
			return (unsigned char*)1;
		}
		else
		{
			Assert( pNode->GetDataTableProxyIndex() < m_nNewStateProxies );
			bool bCur = m_pNewStateProxies[ pNode->GetDataTableProxyIndex() ].m_Bits.Get( m_iClient ) != 0;

			if ( m_pOldStateProxies )
			{
				Assert( pNode->GetDataTableProxyIndex() < m_nOldStateProxies );
				bool bPrev = m_pOldStateProxies[ pNode->GetDataTableProxyIndex() ].m_Bits.Get( m_iClient ) != 0;
				if ( bPrev != bCur )
				{
					if ( bPrev )
					{
						// Old state had the data and the new state doesn't.
						return 0;
					}
					else
					{
						// Add ALL properties under this proxy because the previous state didn't have any of them.
						for ( int i=0; i < pNode->m_nRecursiveProps; i++ )
						{
							if ( m_nNewProxyProps < ARRAYSIZE( m_NewProxyProps ) )
							{
								m_NewProxyProps[m_nNewProxyProps] = pNode->m_iFirstRecursiveProp + i;
							}
							else
							{
								Error( "CPropCullStack::CallPropProxy - overflowed m_nNewProxyProps" );
							}

							++m_nNewProxyProps;
						}

						// Tell the outer loop that writes to m_pOutProps not to write anything from this
						// proxy since we just did.
						return 0;
					}
				}
			}

			return (unsigned char*)bCur;
		}
	}

	virtual void RecurseAndCallProxies( CSendNode *pNode, unsigned char *pStructBase )
	{
		// Remember where the game code pointed us for this datatable's data so 
		m_pProxies[ pNode->GetRecursiveProxyIndex() ] = pStructBase;

		for ( int iChild=0; iChild < pNode->GetNumChildren(); iChild++ )
		{
			CSendNode *pCurChild = pNode->GetChild( iChild );
			
			unsigned char *pNewStructBase = NULL;
			if ( pStructBase )
			{
				pNewStructBase = CallPropProxy( pCurChild, pCurChild->m_iDatatableProp, pStructBase );
			}

			RecurseAndCallProxies( pCurChild, pNewStructBase );
		}
	}
	
	inline void AddProp( int iProp )
	{
		if ( m_nOutProps < m_nMaxOutProps )
		{
			m_pOutProps[m_nOutProps] = iProp;
		}
		else
		{
			Error( "CPropCullStack::AddProp - m_pOutProps overflowed" );
		}

		++m_nOutProps;
	}


	void CullPropsFromProxies( const int *pStartProps, int nStartProps, int *pOutProps, int nMaxOutProps )
	{
		m_nOutProps = 0;
		m_pOutProps = pOutProps;
		m_nMaxOutProps = nMaxOutProps;
		m_nNewProxyProps = 0;

		Init();

		// This list will have any newly available props written into it. Write a sentinel at the end.
		m_NewProxyProps[m_nNewProxyProps] = PROP_SENTINEL;
		int *pCurNewProxyProp = m_NewProxyProps;

		for ( int i=0; i < nStartProps; i++ )
		{
			int iProp = pStartProps[i];

			// Fill in the gaps with any properties that are newly enabled by the proxies.
			while ( *pCurNewProxyProp < iProp )
			{
				AddProp( *pCurNewProxyProp );
				++pCurNewProxyProp;
			}

			// Now write this property's index if the proxies are allowing this property to be written.
			if ( IsPropProxyValid( iProp ) )
			{
				AddProp( iProp );

				// avoid that we add it twice.
				if ( *pCurNewProxyProp == iProp )
					++pCurNewProxyProp;
			}
		}

		// add any remaining new proxy props
		while ( *pCurNewProxyProp < PROP_SENTINEL )
		{
			AddProp( *pCurNewProxyProp );
			++pCurNewProxyProp;
		}
	}

	int GetNumOutProps()
	{
		return m_nOutProps;
	}


private:
	CSendTablePrecalc		*m_pPrecalc;
	int						m_iClient;	// Which client it's encoding out for.
	const CSendProxyRecipients	*m_pOldStateProxies;
	const int					m_nOldStateProxies;
	
	const CSendProxyRecipients	*m_pNewStateProxies;
	const int					m_nNewStateProxies;

	// The output property list.
	int						*m_pOutProps;
	int						m_nMaxOutProps;
	int						m_nOutProps;

	int m_NewProxyProps[MAX_DATATABLE_PROPS+1];
	int m_nNewProxyProps;
};



// ----------------------------------------------------------------------------- //
// CDeltaCalculator encapsulates part of the functionality for calculating deltas between entity states.
//
// To use it, initialize it with the from and to states, then walk through the 'to' properties using
// SeekToNextToProp(). 
//
// At each 'to' prop, either call SkipToProp or CalcDelta
// ----------------------------------------------------------------------------- //
class CDeltaCalculator
{
public:

						CDeltaCalculator(
							CSendTablePrecalc *pPrecalc,
							const void *pFromState,
							const int nFromBits,
							const void *pToState,
							const int nToBits,
							int *pDeltaProps,
							int nMaxDeltaProps,
							const int objectID );
						
						~CDeltaCalculator();

	// Returns the next prop index in the 'to' buffer. Returns PROP_SENTINEL if there are no more.
	// If a valid property index is returned, you MUST call PropSkip() or PropCalcDelta() after calling this.
	int					SeekToNextProp();	

	// Ignore the current 'to' prop.
	void				PropSkip();

	// Seek the 'from' buffer up to the current property, determine if there is a delta, and
	// write the delta (and the property data) into the buffer if there is a delta.
	void				PropCalcDelta();

	// check if current propert is non zero, if so put it into delta indices list
	void				PropCalcNonZero();

	// Returns the number of deltas detected throughout the 
	int					GetNumDeltasDetected();
		

private:
	CSendTablePrecalc	*m_pPrecalc;

	bf_read				m_bfFromState;
	bf_read				m_bfToState;
	
	CDeltaBitsReader	m_FromBitsReader;
	CDeltaBitsReader	m_ToBitsReader;

	int					m_ObjectID;
	
	// Current property indices.
	int					m_iFromProp;
	int					m_iToProp;

	// Bit position into m_bfToState where the current prop (m_iToProp) starts.
	int					m_iToStateStart;

	// Output..
	int					*m_pDeltaProps;
	int					m_nMaxDeltaProps;
	int					m_nDeltaProps;
};


CDeltaCalculator::CDeltaCalculator(
	CSendTablePrecalc *pPrecalc,
	const void *pFromState,
	const int nFromBits,
	const void *pToState,
	const int nToBits,
	int *pDeltaProps,
	int nMaxDeltaProps,
	const int objectID ) 
	
	: m_bfFromState( "CDeltaCalculator->m_bfFromState", pFromState, Bits2Bytes( nFromBits ), nFromBits ),
	m_FromBitsReader( &m_bfFromState ),
	m_bfToState( "CDeltaCalculator->m_bfToState", pToState, Bits2Bytes( nToBits ), nToBits ),
	m_ToBitsReader( &m_bfToState )
{
	m_pPrecalc = pPrecalc;
	m_ObjectID = objectID;

	m_pDeltaProps = pDeltaProps;
	m_nMaxDeltaProps = nMaxDeltaProps;
	m_nDeltaProps = 0;
	
	// Walk through each property in the to state, looking for ones in the from 
	// state that are missing or that are different.
	if ( pFromState)
	{
		m_iFromProp = NextProp( &m_FromBitsReader );
	}
	else
	{
		m_iFromProp = PROP_SENTINEL;
	}
	
	m_iToProp = -1;
}


CDeltaCalculator::~CDeltaCalculator()
{
	// Make sure we didn't overflow.
	ErrorIfNot( 
		m_nDeltaProps <= m_nMaxDeltaProps && !m_bfFromState.IsOverflowed() && !m_bfToState.IsOverflowed(), 
		( "SendTable_CalcDelta: overflowed on datatable '%s'.", m_pPrecalc->GetSendTable()->GetName() ) 
		);

	// We may not have read to the end of our input bits, but we don't care.
	m_FromBitsReader.ForceFinished();
	m_ToBitsReader.ForceFinished();
}


inline int CDeltaCalculator::SeekToNextProp()
{
	m_iToProp = NextProp( &m_ToBitsReader );
	return m_iToProp;
}
		

inline void CDeltaCalculator::PropSkip()
{
	Assert( m_iToProp != -1 );
	SkipPropData( &m_bfToState, m_pPrecalc->GetProp( m_iToProp ) );
}

void CDeltaCalculator::PropCalcNonZero()
{
	const SendProp *pProp = m_pPrecalc->GetProp( m_iToProp );

	if ( !g_PropTypeFns[pProp->m_Type].IsEncodedZero( pProp, &m_bfToState ) )
	{
		// add non-zero property to index list
		if ( m_nDeltaProps < m_nMaxDeltaProps )
		{
			m_pDeltaProps[m_nDeltaProps] = m_iToProp;
		}
		++m_nDeltaProps;
	}
}

void CDeltaCalculator::PropCalcDelta()
{
	// Skip any properties in the from state that aren't in the to state.
	while ( m_iFromProp < m_iToProp )
	{
		SkipPropData( &m_bfFromState, m_pPrecalc->GetProp( m_iFromProp ) );
		m_iFromProp = NextProp( &m_FromBitsReader );
	}

	const SendProp *pProp = m_pPrecalc->GetProp( m_iToProp );

	int bChange = true;

	if ( m_iFromProp == m_iToProp )
	{
		// The property is in both states, so compare them and write the index 
		// if the states are different.
		bChange = g_PropTypeFns[pProp->m_Type].CompareDeltas( pProp, &m_bfFromState, &m_bfToState );
		
		// Seek to the next properties.
		m_iFromProp = NextProp( &m_FromBitsReader );
	}
	else
	{
		// Only the 'to' state has this property, so just skip its data and register a change.
		SkipPropData( &m_bfToState, pProp );
	}

	if ( bChange )
	{
		// Write the property index.
		if ( m_nDeltaProps < m_nMaxDeltaProps )
		{
			m_pDeltaProps[m_nDeltaProps] = m_iToProp;
			m_nDeltaProps++;
		}
	}
}


inline int CDeltaCalculator::GetNumDeltasDetected()
{
	return m_nDeltaProps;
}


// ----------------------------------------------------------------------------- //
// CEncodeInfo
// Used by SendTable_Encode.
// ----------------------------------------------------------------------------- //
class CEncodeInfo : public CServerDatatableStack
{
public:
					CEncodeInfo( CSendTablePrecalc *pPrecalc, unsigned char *pStructBase, int objectID ) :
						CServerDatatableStack( pPrecalc, pStructBase, objectID )
					{
					}

public:

	bf_write	*m_pOut;
	bool		m_bEncodeAll;
	int			m_ObjectID;
	
	// Track sizes of what we write..
	int			m_nOverheadBits;
	int			m_nDataBits;

	CDeltaBitsWriter	*m_pDeltaBitsWriter;
};



// ------------------------------------------------------------------------ //
// Globals.
// ------------------------------------------------------------------------ //

CUtlVector< SendTable* > g_SendTables;
CRC32_t	g_SendTableCRC = 0;



// ------------------------------------------------------------------------ //
// SendTable functions.
// ------------------------------------------------------------------------ //

static bool s_debug_info_shown = false;
static int  s_debug_bits_start = 0;


static inline void ShowEncodeDeltaWatchInfo( 
	const SendTable *pTable,
	const SendProp *pProp, 
	bf_read &buffer,
	const int objectID,
	const int index )
{
	if ( !ShouldWatchThisProp( pTable, objectID, pProp->GetName()) )
		return;
	
	static int lastframe = -1;
	if ( host_framecount != lastframe )
	{
		lastframe = host_framecount;
		ConDMsg( "delta entity: %i\n", objectID );
	}

	// work on copy of bitbuffer
	bf_read copy = buffer;

	s_debug_info_shown = true;

	DecodeInfo info;
	info.m_pStruct = NULL;
	info.m_pData = NULL;
	info.m_pRecvProp = NULL;
	info.m_pProp = pProp;
	info.m_pIn = &copy;
	info.m_ObjectID = objectID;
	info.m_Value.m_Type = (SendPropType)pProp->m_Type;
	
	int startBit = copy.GetNumBitsRead();

	g_PropTypeFns[pProp->m_Type].Decode( &info );

	int bits = copy.GetNumBitsRead() - startBit;

	const char *type = g_PropTypeFns[pProp->m_Type].GetTypeNameString();
	const char *value = info.m_Value.ToString();

	ConDMsg( "+ %s %s, %s, index %i, bits %i, value %s\n", pTable->GetName(), pProp->GetName(), type, index, bits, value );
}


static void SendTable_EncodeProp( CEncodeInfo *pInfo, unsigned long iProp )
{
	// Write the index.
	pInfo->m_pDeltaBitsWriter->WritePropIndex( iProp );

	const SendProp *pProp = pInfo->GetCurProp();

	// Call their proxy to get the property's value.
	DVariant var;
	
	pProp->GetProxyFn()( 
		pProp,
		pInfo->GetCurStructBase(), 
		pInfo->GetCurStructBase() + pProp->GetOffset(), 
		&var, 
		0, // iElement
		pInfo->m_ObjectID
		);

	// Encode it.
	int iStartPos = pInfo->m_pOut->GetNumBitsWritten();

	g_PropTypeFns[pProp->m_Type].Encode( 
		pInfo->GetCurStructBase(), 
		&var, 
		pProp, 
		pInfo->m_pOut, 
		pInfo->m_ObjectID
		); 

	pInfo->m_nDataBits += ( pInfo->m_pOut->GetNumBitsWritten() - iStartPos ); // record # bits written.
}


static bool SendTable_IsPropZero( CEncodeInfo *pInfo, unsigned long iProp )
{
	const SendProp *pProp = pInfo->GetCurProp();

	// Call their proxy to get the property's value.
	DVariant var;
	
	pProp->GetProxyFn()( 
		pProp,
		pInfo->GetCurStructBase(), 
		pInfo->GetCurStructBase() + pProp->GetOffset(), 
		&var, 
		0, // iElement
		pInfo->m_ObjectID
		);

	return g_PropTypeFns[pProp->m_Type].IsZero( pInfo->GetCurStructBase(), &var, pProp );
}


int SendTable_CullPropsFromProxies( 
	const SendTable *pTable,
	
	const int *pStartProps,
	int nStartProps,

	const int iClient,
	
	const CSendProxyRecipients *pOldStateProxies,
	const int nOldStateProxies, 
	
	const CSendProxyRecipients *pNewStateProxies,
	const int nNewStateProxies,
	
	int *pOutProps,
	int nMaxOutProps
	)
{
	Assert( !( nNewStateProxies && !pNewStateProxies ) );
	CPropCullStack stack( pTable->m_pPrecalc, iClient, pOldStateProxies, nOldStateProxies, pNewStateProxies, nNewStateProxies );
	
	stack.CullPropsFromProxies( pStartProps, nStartProps, pOutProps, nMaxOutProps );

	ErrorIfNot( stack.GetNumOutProps() <= nMaxOutProps, ("CullPropsFromProxies: overflow in '%s'.", pTable->GetName()) );
	return stack.GetNumOutProps();
}


// compares properties and writes delta properties, it ignores reciepients
int SendTable_WriteAllDeltaProps(
	const SendTable *pTable,					
	const void *pFromData,
	const int	nFromDataBits,
	const void *pToData,
	const int nToDataBits,
	const int nObjectID,
	bf_write *pBufOut )
{
	// Calculate the delta props.
	int deltaProps[MAX_DATATABLE_PROPS];

	int nDeltaProps = SendTable_CalcDelta(
		pTable, 
		pFromData,
		nFromDataBits,
		pToData,
		nToDataBits,
		deltaProps,
		ARRAYSIZE( deltaProps ),
		nObjectID );

	// Write the properties.
	SendTable_WritePropList( 
		pTable,
		pToData,				// object data
		nToDataBits,
		pBufOut,				// output buffer
		nObjectID,
		deltaProps,
		nDeltaProps );

	return nDeltaProps;
}


bool SendTable_Encode(
	const SendTable *pTable,
	const void *pStruct, 
	bf_write *pOut, 
	int objectID,
	CUtlMemory<CSendProxyRecipients> *pRecipients,
	bool bNonZeroOnly
	)
{
	CSendTablePrecalc *pPrecalc = pTable->m_pPrecalc;
	ErrorIfNot( pPrecalc, ("SendTable_Encode: Missing m_pPrecalc for SendTable %s.", pTable->m_pNetTableName) );
	if ( pRecipients )
	{
		ErrorIfNot(	pRecipients->NumAllocated() >= pPrecalc->GetNumDataTableProxies(), ("SendTable_Encode: pRecipients array too small.") );
	}

	VPROF( "SendTable_Encode" );

	CServerDTITimer timer( pTable, SERVERDTI_ENCODE );

	// This writes and delta-compresses the delta bits.
	CDeltaBitsWriter deltaBitsWriter( pOut );

	// Setup all the info we'll be walking the tree with.
	CEncodeInfo info( pPrecalc, (unsigned char*)pStruct, objectID );
	
	info.m_pOut = pOut;
	info.m_ObjectID = objectID;
	info.m_nDataBits = 0;
	info.m_nOverheadBits = 0; // unused
	info.m_pDeltaBitsWriter = &deltaBitsWriter;
	info.m_pRecipients = pRecipients;	// optional buffer to store the bits for which clients get what data.

	info.Init();
	
	int iNumProps = pPrecalc->GetNumProps();

	for ( int iProp=0; iProp < iNumProps; iProp++ )
	{
		// skip if we don't have a valid prop proxy
		if ( !info.IsPropProxyValid( iProp ) )
			continue;

		info.SeekToProp( iProp );
        
		// skip empty prop if we only encode non-zero values
		if ( bNonZeroOnly && SendTable_IsPropZero(&info, iProp) )
			continue;

		SendTable_EncodeProp( &info, iProp );
	}

	return !pOut->IsOverflowed();
}


void SendTable_WritePropList(
	const SendTable *pTable,
	const void *pState,
	const int nBits,
	bf_write *pOut,
	const int objectID,
	const int *pCheckProps,
	const int nCheckProps )
{
	if ( nCheckProps == 0 )
	{
		// Write single final zero bit, signifying that there no changed properties
		pOut->WriteOneBit( 0 );
		return;
	}

	bool bDebugWatch = Sendprop_UsingDebugWatch();

	s_debug_info_shown = false;
	s_debug_bits_start = pOut->GetNumBitsWritten();
	
	CSendTablePrecalc *pPrecalc = pTable->m_pPrecalc;
	CDeltaBitsWriter deltaBitsWriter( pOut );
	
	bf_read inputBuffer( "SendTable_WritePropList->inputBuffer", pState, BitByte( nBits ), nBits );
	CDeltaBitsReader inputBitsReader( &inputBuffer );

	// Ok, they want to specify a small list of properties to check.
	int iToProp = NextProp( &inputBitsReader );
	int i = 0;
	while ( i < nCheckProps )
	{
		// Seek the 'to' state to the current property we want to check.
		while ( iToProp < pCheckProps[i] )
		{
			SkipPropData( &inputBuffer, pPrecalc->GetProp( iToProp ) );
			iToProp = NextProp( &inputBitsReader );
		}

		if ( iToProp == PROP_SENTINEL )
		{
			break;
		}
		else if ( iToProp == pCheckProps[i] )
		{
			const SendProp *pProp = pPrecalc->GetProp( iToProp );

			// Show debug stuff.
			if ( bDebugWatch )
			{
				ShowEncodeDeltaWatchInfo( pTable, pProp, inputBuffer, objectID, iToProp );
			}

			// See how many bits the data for this property takes up.
			int iStartBit = inputBuffer.GetNumBitsRead();
			SkipPropData( &inputBuffer, pProp );
			int nToStateBits = inputBuffer.GetNumBitsRead() - iStartBit;

			TRACE_PACKET( ( "    Send Field (%s) = %d (%d bytes)\n", pProp->GetName(), nToStateBits, ( nToStateBits + 7 ) / 8 ) );

			// Write the data into the output.
			deltaBitsWriter.WritePropIndex( iToProp );
			inputBuffer.Seek( iStartBit );
			pOut->WriteBitsFromBuffer( &inputBuffer, nToStateBits );

			// Seek to the next prop.
			iToProp = NextProp( &inputBitsReader );
		}

		++i;
	}

	if ( s_debug_info_shown )
	{
		int  bits = pOut->GetNumBitsWritten() - s_debug_bits_start;
		ConDMsg( "= %i bits (%i bytes)\n", bits, Bits2Bytes(bits) );
	}

	inputBitsReader.ForceFinished(); // avoid a benign assert
}


int SendTable_CalcDelta(
	const SendTable *pTable,
	
	const void *pFromState,
	const int nFromBits,
	
	const void *pToState,
	const int nToBits,
	
	int *pDeltaProps,
	int nMaxDeltaProps,
	
	const int objectID
	)
{
	CServerDTITimer timer( pTable, SERVERDTI_CALCDELTA );

	VPROF( "SendTable_CalcDelta" );
	
	// Trivial reject.
	if ( CompareBitArrays( pFromState, pToState, nFromBits, nToBits ) )
	{
		return 0;
	}

	// Now just walk through each property in the 'to' buffer and write it in if it's a new property
	// or if it's different from the previous state.
	CDeltaCalculator deltaCalc( 
		pTable->m_pPrecalc, 
		pFromState, nFromBits, 
		pToState, nToBits, 
		pDeltaProps, nMaxDeltaProps, 
		objectID );

	// Just calculate a delta for each prop in the 'to' state.
	while ( deltaCalc.SeekToNextProp() != PROP_SENTINEL )
	{
		if ( pFromState )
		{
			// check against from state
			deltaCalc.PropCalcDelta();
		}
		else
		{
			// check against zero elements
			deltaCalc.PropCalcNonZero();
		}
	}
	
	// Return the # of properties that changed between 'from' and 'to'.
	return deltaCalc.GetNumDeltasDetected();
}

bool SendTable_WriteInfos( SendTable *pTable, bf_write *pBuf )
{
	pBuf->WriteString( pTable->GetName() );
	pBuf->WriteUBitLong( pTable->GetNumProps(), PROPINFOBITS_NUMPROPS );

	// Send each property.
	for ( int iProp=0; iProp < pTable->m_nProps; iProp++ )
	{
		const SendProp *pProp = &pTable->m_pProps[iProp];

		pBuf->WriteUBitLong( (unsigned int)pProp->m_Type, PROPINFOBITS_TYPE );
		pBuf->WriteString( pProp->GetName() );
		// we now have some flags that aren't networked so strip them off
		unsigned int networkFlags = pProp->GetFlags() & ((1<<PROPINFOBITS_FLAGS)-1);
		pBuf->WriteUBitLong( networkFlags, PROPINFOBITS_FLAGS );

		if( pProp->m_Type == DPT_DataTable )
		{
			// Just write the name and it will be able to reuse the table with a matching name.
			pBuf->WriteString( pProp->GetDataTable()->m_pNetTableName );
		}
		else
		{
			if ( pProp->IsExcludeProp() )
			{
				pBuf->WriteString( pProp->GetExcludeDTName() );
			}
			else if ( pProp->GetType() == DPT_Array )
			{
				pBuf->WriteUBitLong( pProp->GetNumElements(), PROPINFOBITS_NUMELEMENTS );
			}
			else
			{			
				pBuf->WriteBitFloat( pProp->m_fLowValue );
				pBuf->WriteBitFloat( pProp->m_fHighValue );
				pBuf->WriteUBitLong( pProp->m_nBits, PROPINFOBITS_NUMBITS );
			}
		}
	}

	return !pBuf->IsOverflowed();
}



// Spits out warnings for invalid properties and forces property values to
// be in valid ranges for the encoders and decoders.
static void SendTable_Validate( CSendTablePrecalc *pPrecalc )
{
	SendTable *pTable = pPrecalc->m_pSendTable;
	for( int i=0; i < pTable->m_nProps; i++ )
	{
		SendProp *pProp = &pTable->m_pProps[i];
		
		if ( pProp->GetArrayProp() )
		{
			if ( pProp->GetArrayProp()->GetType() == DPT_DataTable )
			{
				Error( "Invalid property: %s/%s (array of datatables) [on prop %d of %d (%s)].", pTable->m_pNetTableName, pProp->GetName(), i, pTable->m_nProps, pProp->GetArrayProp()->GetName() );
			}
		}
		else
		{
			ErrorIfNot( pProp->GetNumElements() == 1, ("Prop %s/%s has an invalid element count for a non-array.", pTable->m_pNetTableName, pProp->GetName()) );
		}
			
		// Check for 1-bit signed properties (their value doesn't get down to the client).
		if ( pProp->m_nBits == 1 && !(pProp->GetFlags() & SPROP_UNSIGNED) )
		{
			DataTable_Warning("SendTable prop %s::%s is a 1-bit signed property. Use SPROP_UNSIGNED or the client will never receive a value.\n", pTable->m_pNetTableName, pProp->GetName());
		}
	}

	for ( int i = 0; i < pPrecalc->GetNumProps(); ++i )
	{
		const SendProp *pProp = pPrecalc->GetProp( i );
		if ( pProp->GetFlags() & SPROP_ENCODED_AGAINST_TICKCOUNT )
		{
			pTable->SetHasPropsEncodedAgainstTickcount( true );
			break;
		}
	}
}


static void SendTable_CalcNextVectorElems( SendTable *pTable )
{
	for ( int i=0; i < pTable->GetNumProps(); i++ )
	{
		SendProp *pProp = pTable->GetProp( i );
		
		if ( pProp->GetType() == DPT_DataTable )
		{
			SendTable_CalcNextVectorElems( pProp->GetDataTable() );
		}
		else if ( pProp->GetOffset() < 0 )
		{
			pProp->SetOffset( -pProp->GetOffset() );
			pProp->SetFlags( pProp->GetFlags() | SPROP_IS_A_VECTOR_ELEM );
		}
	}
}


static bool SendTable_InitTable( SendTable *pTable )
{
	if( pTable->m_pPrecalc )
		return true;
	
	// Create the CSendTablePrecalc.	
	CSendTablePrecalc *pPrecalc = new CSendTablePrecalc;
	pTable->m_pPrecalc = pPrecalc;

	pPrecalc->m_pSendTable = pTable;
	pTable->m_pPrecalc = pPrecalc;

	SendTable_CalcNextVectorElems( pTable );

	// Bind the instrumentation if -dti was specified.
	pPrecalc->m_pDTITable = ServerDTI_HookTable( pTable );

	// Setup its flat property array.
	if ( !pPrecalc->SetupFlatPropertyArray() )
		return false;

	SendTable_Validate( pPrecalc );
	return true;
}


static void SendTable_TermTable( SendTable *pTable )
{
	if( !pTable->m_pPrecalc )
		return;

	delete pTable->m_pPrecalc;
	Assert( !pTable->m_pPrecalc ); // Make sure it unbound itself.
}


int SendTable_GetNumFlatProps( SendTable *pSendTable )
{
	CSendTablePrecalc *pPrecalc = pSendTable->m_pPrecalc;
	ErrorIfNot( pPrecalc,
		("SendTable_GetNumFlatProps: missing pPrecalc.")
	);
	return pPrecalc->GetNumProps();
}

CRC32_t SendTable_CRCTable( CRC32_t &crc, SendTable *pTable )
{
	CRC32_ProcessBuffer( &crc, (void *)pTable->m_pNetTableName, Q_strlen( pTable->m_pNetTableName) );

	int nProps = LittleLong( pTable->m_nProps );
	CRC32_ProcessBuffer( &crc, (void *)&nProps, sizeof( pTable->m_nProps ) );

	// Send each property.
	for ( int iProp=0; iProp < pTable->m_nProps; iProp++ )
	{
		const SendProp *pProp = &pTable->m_pProps[iProp];

		int type = LittleLong( pProp->m_Type );
		CRC32_ProcessBuffer( &crc, (void *)&type, sizeof( type ) );
		CRC32_ProcessBuffer( &crc, (void *)pProp->GetName() , Q_strlen( pProp->GetName() ) );

		int flags = LittleLong( pProp->GetFlags() );
		CRC32_ProcessBuffer( &crc, (void *)&flags, sizeof( flags ) );

		if( pProp->m_Type == DPT_DataTable )
		{
			CRC32_ProcessBuffer( &crc, (void *)pProp->GetDataTable()->m_pNetTableName, Q_strlen( pProp->GetDataTable()->m_pNetTableName ) );
		}
		else
		{
			if ( pProp->IsExcludeProp() )
			{
				CRC32_ProcessBuffer( &crc, (void *)pProp->GetExcludeDTName(), Q_strlen( pProp->GetExcludeDTName() ) );
			}
			else if ( pProp->GetType() == DPT_Array )
			{
				int numelements = LittleLong( pProp->GetNumElements() );
				CRC32_ProcessBuffer( &crc, (void *)&numelements, sizeof( numelements ) );
			}
			else
			{	
				float lowvalue;
				LittleFloat( &lowvalue, &pProp->m_fLowValue );
				CRC32_ProcessBuffer( &crc, (void *)&lowvalue, sizeof( lowvalue ) );

				float highvalue;
				LittleFloat( &highvalue, &pProp->m_fHighValue );
				CRC32_ProcessBuffer( &crc, (void *)&highvalue, sizeof( highvalue ) );

				int	bits = LittleLong( pProp->m_nBits );
				CRC32_ProcessBuffer( &crc, (void *)&bits, sizeof( bits ) );
			}
		}
	}

	return crc;
}

void SendTable_PrintStats( void )
{
	int numTables = 0;
	int numFloats = 0;
	int numStrings = 0;
	int numArrays = 0;
	int numInts = 0;
	int numVecs = 0;
	int numSubTables = 0;
	int numSendProps = 0;
	int numFlatProps = 0;
	int numExcludeProps = 0;

	for ( int i=0; i < g_SendTables.Count(); i++ )
	{
		SendTable *st =  g_SendTables[i];
		
		numTables++;
		numSendProps += st->GetNumProps();
		numFlatProps += st->m_pPrecalc->GetNumProps();

		for ( int j=0; j < st->GetNumProps(); j++ )
		{
			SendProp* sp = st->GetProp( j );

			if ( sp->IsExcludeProp() )
			{
				numExcludeProps++;
				continue; // no real sendprops
			}

			if ( sp->IsInsideArray() )
				continue;

			switch( sp->GetType() )
			{
				case DPT_Int	: numInts++; break;
				case DPT_Float	: numFloats++; break;
				case DPT_Vector : numVecs++; break;
				case DPT_String : numStrings++; break;
				case DPT_Array	: numArrays++; break;
				case DPT_DataTable : numSubTables++; break;
			}
		}
	}

	Msg("Total Send Table stats\n");
	Msg("Send Tables : %i\n", numTables );
	Msg("Send Props  : %i\n", numSendProps );
	Msg("Flat Props  : %i\n", numFlatProps );
	Msg("Int Props   : %i\n", numInts );
	Msg("Float Props : %i\n", numFloats );
	Msg("Vector Props: %i\n", numVecs );
	Msg("String Props: %i\n", numStrings );
	Msg("Array Props : %i\n", numArrays );
	Msg("Table Props : %i\n", numSubTables );
	Msg("Exclu Props : %i\n", numExcludeProps );
}



bool SendTable_Init( SendTable **pTables, int nTables )
{
	ErrorIfNot( g_SendTables.Count() == 0,
		("SendTable_Init: called twice.")
	);

	// Initialize them all.
	for ( int i=0; i < nTables; i++ )
	{
		if ( !SendTable_InitTable( pTables[i] ) )
			return false;
	}

	// Store off the SendTable list.
	g_SendTables.CopyArray( pTables, nTables );

	g_SendTableCRC = SendTable_ComputeCRC( );

	if ( CommandLine()->FindParm("-dti" ) )
	{
		SendTable_PrintStats();
	}

	return true;	
}
void SendTable_Term()
{
	// Term all the SendTables.
	for ( int i=0; i < g_SendTables.Count(); i++ )
		SendTable_TermTable( g_SendTables[i] );

	// Clear the list of SendTables.
	g_SendTables.Purge();
	g_SendTableCRC = 0;
}

//-----------------------------------------------------------------------------
// Purpose: Computes the crc for all sendtables for the data sent in the class/table definitions
// Output : CRC32_t
//-----------------------------------------------------------------------------
CRC32_t SendTable_ComputeCRC()
{
	CRC32_t result;
	CRC32_Init( &result );

	// walk the tables and checksum them
	int c = g_SendTables.Count();
	for ( int i = 0 ; i < c; i++ )
	{
		SendTable *st = g_SendTables[ i ];
		result = SendTable_CRCTable( result, st );
	}


	CRC32_Final( &result );

	return result;
}

SendTable *SendTabe_GetTable(int index)
{
	return  g_SendTables[index];
}

int	SendTable_GetNum()
{
	return g_SendTables.Count();
}

//-----------------------------------------------------------------------------
// Purpose: 
// Output : CRC32_t
//-----------------------------------------------------------------------------
CRC32_t SendTable_GetCRC()
{
	return g_SendTableCRC;
}


//-----------------------------------------------------------------------------
// Purpose: check integrity of an unpacked entity send table
//-----------------------------------------------------------------------------
bool SendTable_CheckIntegrity( SendTable *pTable, const void *pData, const int nDataBits )
{
#ifdef _DEBUG
	if ( pData == NULL && nDataBits == 0 )
		return true;

	bf_read bfRead(	"SendTable_CheckIntegrity", pData, Bits2Bytes(nDataBits), nDataBits );
	CDeltaBitsReader bitsReader( &bfRead );

	int iProp = PROP_SENTINEL;
	int iLastProp = -1;
	int nMaxProps = pTable->m_pPrecalc->GetNumProps();
	int nPropCount = 0;

	Assert( nMaxProps > 0 && nMaxProps < MAX_DATATABLE_PROPS );

	while( -1 != (iProp = bitsReader.ReadNextPropIndex()) )
	{
		Assert( (iProp>=0) && (iProp<nMaxProps) );

		// must be larger
		Assert( iProp > iLastProp );

		const SendProp *pProp = pTable->m_pPrecalc->GetProp( iProp );

		Assert( pProp );

		// ok check that SkipProp & IsEncodedZero read the same bit length
		int iStartBit = bfRead.GetNumBitsRead();
		g_PropTypeFns[ pProp->GetType() ].SkipProp( pProp, &bfRead );
		int nLength = bfRead.GetNumBitsRead() - iStartBit;

		Assert( nLength > 0 ); // a prop must have some bits

		bfRead.Seek( iStartBit ); // read it again

		g_PropTypeFns[ pProp->GetType() ].IsEncodedZero( pProp, &bfRead );

		Assert( nLength == (bfRead.GetNumBitsRead() - iStartBit) ); 

		nPropCount++;
		iLastProp = iProp;
	}

	Assert( nPropCount <= nMaxProps );
	Assert( bfRead.GetNumBytesLeft() < 4 ); 
	Assert( !bfRead.IsOverflowed() );

#endif

	return true;
}





