//====== Copyright © 1996-2004, Valve Corporation, All rights reserved. =======
//
// A class for computing things with CDmeMesh data
//
//=============================================================================

#ifndef DMMESHCOMP_H
#define DMMESHCOMP_H

#ifdef _WIN32
#pragma once
#endif


// Valve includes
#include "mathlib/mathlib.h"
#include "tier1/utlvector.h"
#include "tier1/utllinkedlist.h"


// Forward declarations
class CDmeMesh;
class CDmeVertexData;


//=============================================================================
// TODO: This works in the local space of the mesh... add option to transform
// the positions into world space
//=============================================================================
class CDmMeshComp
{
public:
	CDmMeshComp( CDmeMesh *pMesh, CDmeVertexData *pPassedBase = NULL );

	class CVert
	{
	public:
		CVert();

		int Index() const;

		const CUtlVector< int > *VertexIndices() const;

		const Vector *Position() const;

		bool operator==( const CVert &rhs ) const;

	protected:
		friend class CDmMeshComp;

		int m_index;								// Index in the position data
		const CUtlVector< int > *m_pVertexIndices;	// Pointer to a list of the vertex indices for this vertex
		const Vector *m_pPosition;
	};

	class CEdge
	{
	public:
		CEdge();

		int GetVertIndex( int edgeRelativeVertexIndex ) const;

		CVert *GetVert( int edgeRelativeVertexIndex ) const;

		bool IsBorderEdge() const { return m_faceCount == 1; }

		bool ConnectedTo( const CEdge *pEdge ) const { return m_pVert0 == pEdge->m_pVert0 || m_pVert0 == pEdge->m_pVert1 || m_pVert1 == pEdge->m_pVert0 || m_pVert1 == pEdge->m_pVert1; }

		Vector EdgeVector() const;

		// Returns true if the edge starts and stops at the same point in local space
		bool operator==( const CEdge &rhs ) const;

	protected:
		friend class CDmMeshComp;

		CVert *m_pVert0;
		CVert *m_pVert1;
		int m_faceCount;
	};

	class CFace
	{
	public:
	protected:
		friend class CDmMeshComp;

		CUtlVector< CVert * > m_verts;
		CUtlVector< CEdge * > m_edges;
		CUtlVector< bool > m_edgeReverseMap;
	};

	CDmeVertexData *BaseState() { return m_pBase; }

	CVert *CreateVert( int nIndex, const CUtlVector< int > &vertexIndices, const Vector &vert );

	CVert *FindVert( int vIndex );

	CEdge *FindOrCreateEdge( int vIndex0, int vIndex1, bool *pReverse = NULL );

	CEdge *FindEdge( int vIndex0, int vIndex1, bool *pReverse = NULL );

	CFace *CreateFace( const CUtlVector< CVert * > &verts, const CUtlVector< CEdge * > &edges, const CUtlVector< bool > &edgeReverseMap );

	int FindEdgesWithVert( int vIndex, CUtlVector< CEdge * > &edges );

	int FindFacesWithVert( int vIndex, CUtlVector< CFace * > &faces );

	int FindNeighbouringVerts( int vIndex, CUtlVector< CVert * > &verts );

	int GetBorderEdges( CUtlVector< CUtlVector< CEdge * > > &borderEdges );

	CDmeMesh *m_pMesh;
	CDmeVertexData *m_pBase;
	CUtlFixedLinkedList< CVert > m_verts;
	CUtlFixedLinkedList< CEdge > m_edges;
	CUtlFixedLinkedList< CFace > m_faces;
};


#endif // DMEMESHCOMP_H
