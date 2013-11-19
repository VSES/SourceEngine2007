//====== Copyright © 1996-2004, Valve Corporation, All rights reserved. =======
//
// Implementation of CDmMeshComp - CDmeMesh computation class
//
//=============================================================================


// Valve includes
#include "movieobjects/dmmeshcomp.h"
#include "movieobjects/dmefaceset.h"
#include "movieobjects/dmemesh.h"
#include "movieobjects/dmevertexdata.h"


// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


//=============================================================================
//
//=============================================================================
CDmMeshComp::CDmMeshComp( CDmeMesh *pMesh, CDmeVertexData *pPassedBase )
: m_pMesh( pMesh )
, m_pBase( NULL )
{
	m_pBase = pPassedBase ? pPassedBase : pMesh->GetCurrentBaseState();
	if ( !m_pBase )
		return;

	const FieldIndex_t pIndex( m_pBase->FindFieldIndex( CDmeVertexData::FIELD_POSITION ) );
	if ( pIndex < 0 )
		return;

	const CUtlVector< Vector > &pData( m_pBase->GetPositionData() );
	const CUtlVector<int> &pIndices( m_pBase->GetVertexIndexData( CDmeVertexData::FIELD_POSITION ) );

	const int nVertices( pData.Count() );
	if ( nVertices <= 0 )
		return;

	// Create vertices
	for ( int i = 0; i < nVertices; ++i )
	{
		const CUtlVector< int > &vertexIndices = m_pBase->FindVertexIndicesFromDataIndex( CDmeVertexData::FIELD_POSITION, i );
		CreateVert( i, vertexIndices, pData[ i ] );
	}

	// Create edges and faces
	const int nFaceSets( pMesh->FaceSetCount() );
	for ( int i = 0; i < nFaceSets; ++i )
	{
		CDmeFaceSet *pFaceSet( pMesh->GetFaceSet( i ) );
		const int nIndices( pFaceSet->NumIndices() );
		if ( nIndices < 4 )	// At least a triangle and a -1
			continue;

		int facePosIndex( -1 );
		int edgePosIndex0( -1 );
		int edgePosIndex1( -1 );

		CUtlVector< CVert * > verts;
		CUtlVector< CEdge * > edges;
		CUtlVector< bool > edgeReverseMap;
		bool bReverse = false;

		for ( int j( 0 ); j < nIndices; ++j )
		{
			const int faceVertexIndex( pFaceSet->GetIndex( j ) );

			if ( faceVertexIndex < 0 )
			{
				// End of face
				edgePosIndex0 = edgePosIndex1;
				edgePosIndex1 = facePosIndex;

				Assert( edgePosIndex0 >= 0 );
				Assert( edgePosIndex1 >= 0 );

				edges.AddToTail( FindOrCreateEdge( edgePosIndex0, edgePosIndex1, &bReverse ) );
				edgeReverseMap.AddToTail( bReverse );

				CreateFace( verts, edges, edgeReverseMap );

				facePosIndex = -1;
				verts.RemoveAll();
				edges.RemoveAll();
				edgeReverseMap.RemoveAll();
				continue;
			}

			if ( facePosIndex < 0 )
			{
				// First vertex
				facePosIndex = pIndices[ faceVertexIndex ];
				edgePosIndex1 = facePosIndex;
				verts.AddToTail( FindVert( edgePosIndex1 ) );
				continue;
			}

			// 2nd through last vertex
			edgePosIndex0 = edgePosIndex1;
			edgePosIndex1 = pIndices[ faceVertexIndex ];
			verts.AddToTail( FindVert( edgePosIndex1 ) );

			Assert( edgePosIndex0 >= 0 );
			Assert( edgePosIndex1 >= 0 );

			edges.AddToTail( FindOrCreateEdge( edgePosIndex0, edgePosIndex1, &bReverse ) );
			edgeReverseMap.AddToTail( bReverse );
		}
	}
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CDmMeshComp::CVert::CVert()
: m_index( -1 )
, m_pVertexIndices( NULL )
, m_pPosition( NULL )
{
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int CDmMeshComp::CVert::Index() const
{
	return m_index;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const CUtlVector< int > *CDmMeshComp::CVert::VertexIndices() const
{
	return m_pVertexIndices;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const Vector *CDmMeshComp::CVert::Position() const
{
	return m_pPosition;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CDmMeshComp::CVert::operator==( const CVert &rhs ) const
{
	return ( m_pPosition->DistToSqr( *rhs.m_pPosition ) < FLT_EPSILON );
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CDmMeshComp::CEdge::CEdge()
: m_pVert0( NULL )
, m_pVert1( NULL )
, m_faceCount( 0 )
{
}


//-----------------------------------------------------------------------------
// Returns the mesh relative vertex index given the edge relative vertex index
//-----------------------------------------------------------------------------
int CDmMeshComp::CEdge::GetVertIndex( int edgeRelativeVertexIndex ) const
{
	if ( edgeRelativeVertexIndex == 0 && m_pVert0 )
		return m_pVert0->Index();

	if ( edgeRelativeVertexIndex == 1 && m_pVert1 )
		return m_pVert1->Index();

	return -1;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CDmMeshComp::CVert *CDmMeshComp::CEdge::GetVert( int edgeRelativeVertexIndex ) const
{
	if ( edgeRelativeVertexIndex == 0 )
		return m_pVert0;

	if ( edgeRelativeVertexIndex == 1 )
		return m_pVert1;

	return NULL;
}


//-----------------------------------------------------------------------------
// Returns true if the edge starts and stops at the same position in space
// The order of the vertices is not checked
//-----------------------------------------------------------------------------
bool CDmMeshComp::CEdge::operator==( const CEdge &rhs ) const
{
	return (
		( *m_pVert0 == *rhs.m_pVert0 && *m_pVert1 == *rhs.m_pVert1 ) ||
		( *m_pVert0 == *rhs.m_pVert1 && *m_pVert1 == *rhs.m_pVert0 ) );
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Vector CDmMeshComp::CEdge::EdgeVector() const
{
	if ( m_pVert0 && m_pVert1 )
		return *m_pVert1->Position() - *m_pVert0->Position();

	return vec3_origin;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CDmMeshComp::CVert *CDmMeshComp::CreateVert( int nIndex, const CUtlVector< int > &vertexIndices, const Vector &vert )
{
	CVert *pVert = &m_verts[ m_verts.AddToTail() ];
	pVert->m_index = nIndex;
	pVert->m_pVertexIndices = &vertexIndices;
	pVert->m_pPosition = &vert;

	return pVert;
}


//-----------------------------------------------------------------------------
// Finds or Creates an edge... Can still return NULL if vertices do not exist
//-----------------------------------------------------------------------------
CDmMeshComp::CEdge *CDmMeshComp::FindOrCreateEdge( int vIndex0, int vIndex1, bool *pReverse /* = NULL */ )
{
	CEdge *pEdge = FindEdge( vIndex0, vIndex1, pReverse );
	if ( pEdge )
		return pEdge;

	CVert *pVert0 = FindVert( vIndex0 );
	if ( pVert0 == NULL )
		return NULL;

	CVert *pVert1 = FindVert( vIndex1 );
	if ( pVert1 == NULL )
		return NULL;

	pEdge = &m_edges[ m_edges.AddToTail() ];

	pEdge->m_pVert0 = pVert0;
	pEdge->m_pVert1 = pVert1;

	if ( pReverse )
	{
		*pReverse = false;
	}

	return pEdge;
}


//-----------------------------------------------------------------------------
// Returns the edge between vIndex0 & vIndex1 (or vica versa) NULL if not found
//-----------------------------------------------------------------------------
CDmMeshComp::CEdge *CDmMeshComp::FindEdge( int vIndex0, int vIndex1, bool *pReverse /* = NULL */ )
{
	for ( int ei( m_edges.Head() ); ei != m_edges.InvalidIndex(); ei = m_edges.Next( ei ) )
	{
		CEdge &e( m_edges[ ei ] );

		if ( e.GetVertIndex( 0 ) == vIndex0 && e.GetVertIndex( 1 ) == vIndex1 )
		{
			if ( pReverse )
			{
				*pReverse = false;
			}
			return &e;
		}

		if ( e.GetVertIndex( 1 ) == vIndex0 && e.GetVertIndex( 0 ) == vIndex1 )
		{
			if ( pReverse )
			{
				*pReverse = true;
			}
			return &e;
		}
	}

	return NULL;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CDmMeshComp::CVert *CDmMeshComp::FindVert( int vIndex )
{
	for ( int vi( m_verts.Head() ); vi != m_verts.InvalidIndex(); vi = m_verts.Next( vi ) )
	{
		CVert &v( m_verts[ vi ] );
		if ( vIndex == v.Index() )
			return &v;
	}

	return NULL;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CDmMeshComp::CFace *CDmMeshComp::CreateFace( const CUtlVector< CVert * > &verts, const CUtlVector< CEdge * > &edges, const CUtlVector< bool > &edgeReverseMap )
{
	CFace *pFace = &m_faces[ m_faces.AddToTail() ];

	pFace->m_verts.RemoveAll();
	pFace->m_verts.AddVectorToTail( verts );

	pFace->m_edges.RemoveAll();
	pFace->m_edges.AddVectorToTail( edges );

	pFace->m_edgeReverseMap.RemoveAll();
	pFace->m_edgeReverseMap.AddVectorToTail( edgeReverseMap );

	for ( int nEdgeIndex = edges.Count() - 1; nEdgeIndex >= 0; --nEdgeIndex )
	{
		edges[ nEdgeIndex ]->m_faceCount += 1;
	}

	return pFace;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int CDmMeshComp::FindEdgesWithVert( int vIndex, CUtlVector< CEdge * > &edges )
{
	edges.RemoveAll();

	for ( int ei( m_edges.Head() ); ei != m_edges.InvalidIndex(); ei = m_edges.Next( ei ) )
	{
		CEdge &e( m_edges[ ei ] );
		if ( e.GetVertIndex( 0 ) == vIndex || e.GetVertIndex( 1 ) == vIndex )
		{
			edges.AddToTail( &e );
		}
	}

	return edges.Count();
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int CDmMeshComp::FindFacesWithVert( int vIndex, CUtlVector< CFace * > &faces )
{
	faces.RemoveAll();

	for ( int fi( m_faces.Head() ); fi != m_faces.InvalidIndex(); fi = m_faces.Next( fi ) )
	{
		CFace &face( m_faces[ fi ] );
		for ( int i = 0; i < face.m_verts.Count(); ++i )
		{
			if ( face.m_verts[ i ]->Index() == vIndex )
			{
				faces.AddToTail( &face );
				break;
			}
		}
	}

	return faces.Count();
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int CDmMeshComp::FindNeighbouringVerts( int vIndex, CUtlVector< CVert * > &verts )
{
	verts.RemoveAll();

	CUtlVector< CEdge * > edges;
	const int nEdges = FindEdgesWithVert( vIndex, edges );

	for ( int i = 0; i < nEdges; ++i )
	{
		CEdge &e( *edges[ i ] );
		if ( e.GetVertIndex( 0 ) == vIndex )
		{
			verts.AddToTail( e.GetVert( 1 ) );
		}
		else
		{
			verts.AddToTail( e.GetVert( 0 ) );
		}
	}

	return verts.Count();
}


//-----------------------------------------------------------------------------
// Find all edges that are only used by 1 face
//-----------------------------------------------------------------------------
int CDmMeshComp::GetBorderEdges( CUtlVector< CUtlVector< CEdge * > > &borderEdgesList )
{
	int retVal = 0;

	borderEdgesList.RemoveAll();

	bool connected;

	for ( int ei( m_edges.Head() ); ei != m_edges.InvalidIndex(); ei = m_edges.Next( ei ) )
	{
		CEdge *pEdge( &m_edges[ ei ] );
		if ( pEdge->IsBorderEdge() )
		{
			++retVal;
			connected = false;

			for ( int i = borderEdgesList.Count() - 1; !connected && i >= 0; --i )
			{
				CUtlVector< CEdge * > &borderEdges = borderEdgesList[ i ];
				for ( int j = borderEdges.Count() - 1; j >= 0; --j )
				{
					if ( borderEdges[ j ]->ConnectedTo( pEdge ) )
					{
						borderEdges.AddToTail( pEdge );
						connected = true;
						break;
					}
				}
			}

			if ( !connected )
			{
				CUtlVector< CEdge * > &borderEdges = borderEdgesList[ borderEdgesList.AddToTail() ];
				borderEdges.AddToTail( pEdge );
			}
		}
	}

	// Shrink the borderEdgesList to minimum number required

	bool anyConnected = false;
	do 
	{
		anyConnected = false;

		for ( int i = borderEdgesList.Count() - 1; i >= 0; --i )
		{
			CUtlVector< CEdge * > &srcBorderEdges = borderEdgesList[ i ];
			for ( int j = srcBorderEdges.Count() - 1; j >= 0; --j )
			{
				CEdge *pSrcEdge = srcBorderEdges[ j ];
				connected = false;

				for ( int k = 0; !connected && k < i; ++k )
				{
					CUtlVector< CEdge * > &dstBorderEdges = borderEdgesList[ k ];
					for ( int l = dstBorderEdges.Count() - 1; l >= 0; --l )
					{
						if ( dstBorderEdges[ l ]->ConnectedTo( pSrcEdge ) )
						{
							connected = true;
							anyConnected = true;
							dstBorderEdges.AddToTail( pSrcEdge );
							srcBorderEdges.Remove( j );
							break;
						}
					}
				}
			}

			if ( srcBorderEdges.Count() == 0 )
			{
				borderEdgesList.Remove( i );
			}
		}
	} while( anyConnected );

	return retVal;
}