// C3DVectorCompressor.cpp: implementation of the C3DVectorCompressor class.
//
//////////////////////////////////////////////////////////////////////

#include <StdAfx.h>
#include "C3DVectorCompressor.h"

C3DVectorCompressor::~C3DVectorCompressor()
{
	m_VectorMap.clear();
	m_VectorIndex.clear();

}

//------------------------------------------------------------------------------
// Function name	: C3DVectorCompressor::GetVertex
// Description	    : Returns the vertex matching the index
// Return type		: HRESULT
// Argument         : ULONG , CSIBCVector3D*
//------------------------------------------------------------------------------
HRESULT C3DVectorCompressor::GetVertex( ULONG l_lIndex, CSIBCVector3D* out_pVector3 )
{
	if (l_lIndex > m_VectorIndex.size())
		return S_FALSE;

	if ( out_pVector3 == NULL )
	{
		return E_INVALIDARG;
	}

	CSIBCVector3D l_pFound = m_VectorIndex[ l_lIndex ];
	
	out_pVector3->Set ( l_pFound.GetX(), l_pFound.GetY(), l_pFound.GetZ() );

	return S_OK;
}

//------------------------------------------------------------------------------
// Function name	: C3DVectorCompressor::AddVertex
// Description	    : Adds a new vertex to the compressor
// Return type		: HRESULT
// Argument         : 3 doubles (x,y,z)
//------------------------------------------------------------------------------
HRESULT C3DVectorCompressor::AddVertex( double x, double y, double z )
{
	CSIBCVector3D	l_pVector3;
	
	l_pVector3.Set ( (float)x, (float)y, (float)z );

	HRESULT	l_hr = S_FALSE;

	if ( m_bCompress == TRUE )
	{
		C3DVectorMapIterator	l_it = m_VectorMap.begin();
		bool found = false;

		while ( l_it != m_VectorMap.end() )
		{
			CSIBCVector3D	l_v = (*l_it).first;

			float diff1 = fabs( l_v.m_fX - l_pVector3.m_fX );
			float diff2 = fabs( l_v.m_fY - l_pVector3.m_fY );
			float diff3 = fabs( l_v.m_fZ - l_pVector3.m_fZ );


			if (( diff1 < 0.001f ) && ( diff2 < 0.001f ) && ( diff3 < 0.001f ))
			{
				found = true;
				break;

			}

			l_it++;

		}

		if ( !found )
		{

			ULONG	l_index = m_VectorIndex.size();
			m_VectorMap[l_pVector3] = l_index;
			m_VectorIndex.push_back(l_pVector3);
			l_hr = S_OK;
		}
		else
		{
			//
			// As we have it already, then discard it.
			//
//			delete l_pVector3;
		}
	

	} else {

		//
		// Dont compress, just add it !
		//

		ULONG	l_index = m_VectorIndex.size();
		m_VectorMap[l_pVector3] = l_index;
		m_VectorIndex.push_back(l_pVector3);
		l_hr = S_OK;

	}

	return l_hr;
}

//------------------------------------------------------------------------------
// Function name	: C3DVectorCompressor::GetIndex
// Description	    : Returns the index matching the vertex
// Return type		: LONG
// Argument         : 3 doubles (x,y,z)
//------------------------------------------------------------------------------
LONG C3DVectorCompressor::GetIndex( double x, double y, double z )
{
	CSIBCVector3D	l_pVector3;
	
	l_pVector3.Set ( (float)x, (float)y, (float)z );

	C3DVectorMapIterator	l_it = m_VectorMap.begin();
	bool found = false;
	int index = -1;

	while ( l_it != m_VectorMap.end() )
	{
		CSIBCVector3D	l_v = (*l_it).first;

		float diff1 = fabs( l_v.m_fX - l_pVector3.m_fX );
		float diff2 = fabs( l_v.m_fY - l_pVector3.m_fY );
		float diff3 = fabs( l_v.m_fZ - l_pVector3.m_fZ );


		if (( diff1 < 0.001f ) && ( diff2 < 0.001f ) && ( diff3 < 0.001f ))
		{
			found = true;
			index = (*l_it).second;
			break;

		}

		l_it++;

	}

	return index;
}

//------------------------------------------------------------------------------
// Function name	: C3DVectorCompressor::GetCount
// Description	    : Returns the number of vectors in the compressor
// Return type		: LONG
// Argument         : Nada
//------------------------------------------------------------------------------
LONG C3DVectorCompressor::GetCount ( )
{
	if ( m_bCompress )
	{
		return (LONG)m_VectorMap.size();
	} 
	
	return (LONG)m_VectorIndex.size();
	
}