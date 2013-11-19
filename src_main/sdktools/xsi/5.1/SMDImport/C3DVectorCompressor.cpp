// C3DVectorCompressor.cpp: implementation of the C3DVectorCompressor class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "C3DVectorCompressor.h"


//------------------------------------------------------------------------------
// Function name	: C3DVectorCompressor::GetVertex
// Description	    : Returns the vertex matching the index
// Return type		: HRESULT
// Argument         : ULONG , CSIBCVector3D*
//------------------------------------------------------------------------------
HRESULT C3DVectorCompressor::GetVertex( ULONG l_lIndex, uvvec* out_pVector3 )
{
	if (l_lIndex > m_VectorIndex.size())
		return S_FALSE;

	if ( out_pVector3 == NULL )
	{
		return E_INVALIDARG;
	}

	uvvec l_pFound = m_VectorIndex[ l_lIndex ];
	
	memcpy ( out_pVector3, &l_pFound, sizeof(uvvec) );

	return S_OK;
}

//------------------------------------------------------------------------------
// Function name	: C3DVectorCompressor::AddVertex
// Description	    : Adds a new vertex to the compressor
// Return type		: HRESULT
// Argument         : 3 doubles (x,y,z)
//------------------------------------------------------------------------------
HRESULT C3DVectorCompressor::AddVertex( double x, double y, double z, double u, double v,SMDVertex* refVertex, long* out_Index )
{
	uvvec*	l_pVector3 = new uvvec;
	if (l_pVector3 == NULL) return E_OUTOFMEMORY;

	l_pVector3->x = x;
	l_pVector3->y = y;
	l_pVector3->z = z;
	l_pVector3->u = u;
	l_pVector3->v = v;
	l_pVector3->refVertex = refVertex;
	
	HRESULT	l_hr = S_FALSE;

	if ( m_bCompress == TRUE )
	{
		C3DVectorMapIterator	l_iter = m_VectorMap.find( *l_pVector3 );

		if (l_iter == m_VectorMap.end())
		{
			ULONG	l_index = m_VectorIndex.size();
			m_VectorMap[*l_pVector3] = l_index;
			m_VectorIndex.push_back(*l_pVector3);
			l_hr = S_OK;
			
			if ( out_Index != NULL )
				*out_Index = l_index;
		}
		else
		{
			//
			// As we have it already, then discard it.
			//
			delete l_pVector3;

						
			if ( out_Index != NULL )
				*out_Index = (*l_iter).second;
			 
		}
	} else {

		//
		// Dont compress, just add it !
		//

		ULONG	l_index = m_VectorIndex.size();
		m_VectorMap[*l_pVector3] = l_index;
		m_VectorIndex.push_back(*l_pVector3);
		l_hr = S_OK;
		
		if ( out_Index != NULL )
			*out_Index = l_index;
	}

	return l_hr;
}

//------------------------------------------------------------------------------
// Function name	: C3DVectorCompressor::GetIndex
// Description	    : Returns the index matching the vertex
// Return type		: LONG
// Argument         : 3 doubles (x,y,z)
//------------------------------------------------------------------------------
LONG C3DVectorCompressor::GetIndex( double x, double y, double z, double u, double v )
{
	uvvec*	l_pVector3 = new uvvec;
	
	l_pVector3->x = x;
	l_pVector3->y = y;
	l_pVector3->z = z;
	l_pVector3->u = u;
	l_pVector3->v = v;

	C3DVectorMapIterator	l_iter = m_VectorMap.find( (uvvec&)*l_pVector3 );
	if (l_iter == m_VectorMap.end())
		return -1;

	return (*l_iter).second;
}

//------------------------------------------------------------------------------
// Function name	: C3DVectorCompressor::GetCount
// Description	    : Returns the number of vectors in the compressor
// Return type		: LONG
// Argument         : Nada
//------------------------------------------------------------------------------
LONG C3DVectorCompressor::GetCount ( )
{
	return (LONG)m_VectorMap.size();
}