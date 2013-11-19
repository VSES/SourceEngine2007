//===== Copyright © 1996-2006, Valve Corporation, All rights reserved. ======//
//
// Purpose: class for keeping track of all the references that exist to an object.  When the object
// being referenced is freed, all of the pointers pointing at it will become null.
//
// $Revision: $
// $NoKeywords: $
//===========================================================================//

#ifndef UTLOBJECTREFERENCE_H
#define UTLOBJECTREFERENCE_H

#ifdef _WIN32
#pragma once
#endif

#include "tier1/utlintrusivelist.h"
#include "mathlib/mathlib.h"


template<class T> class CUtlReference
{
public:
	CUtlReference(void)
	{
		m_pNext = m_pPrev = NULL;
		m_pObject = NULL;
	}
  
	CUtlReference(T *pObj)
	{
		m_pNext = m_pPrev = NULL;
		AddRef( pObj );
	}

	~CUtlReference(void)
	{
		KillRef();
	}
  
	void Set(T *pObj)
	{
		if ( m_pObject != pObj )
		{
			KillRef();
			AddRef( pObj );
		}
	}
  
	T * operator()(void) const
	{
		return m_pObject;
	}

	operator T*()
	{
		return m_pObject;
	}

	operator const T*() const
	{
		return m_pObject;
	}

	T* operator->()
	{ 
		return m_pObject; 
	}

	const T* operator->() const
	{ 
		return m_pObject; 
	}

	CUtlReference &operator=( const CUtlReference& otherRef )
	{
		Set( otherRef.m_pObject );
		return *this;
	}

	bool operator==( const CUtlReference& o ) const
	{
		return ( o.m_pObject == m_pObject );
	}	

public:
	CUtlReference *m_pNext;
	CUtlReference *m_pPrev;

	T *m_pObject;

	void AddRef( T *pObj )
	{
		m_pObject = pObj;
		if ( pObj )
		{
			pObj->m_References.AddToHead( this );
		}
	}

	void KillRef(void)
	{
		if ( m_pObject )
		{
			m_pObject->m_References.RemoveNode( this );
			m_pObject = NULL;
		}
	}

};

template<class T> class CUtlReferenceList : public CUtlIntrusiveDList< CUtlReference<T> >
{
public:
	~CUtlReferenceList( void )
	{
		CUtlReference<T> *i = CUtlIntrusiveDList<CUtlReference<T> >::m_pHead;
		while( i )
		{
			CUtlReference<T> *n = i->m_pNext;
			i->m_pNext = NULL;
			i->m_pPrev = NULL;
			i->m_pObject = NULL;
			i = n;
		}
		CUtlIntrusiveDList<CUtlReference<T> >::m_pHead = NULL;
	}
};


//-----------------------------------------------------------------------------
// Put this macro in classes that are referenced by CUtlReference
//-----------------------------------------------------------------------------
#define DECLARE_REFERENCED_CLASS( _className )				\
	private:												\
		CUtlReferenceList< _className > m_References;		\
		template<class T> friend class CUtlReference;


#endif





