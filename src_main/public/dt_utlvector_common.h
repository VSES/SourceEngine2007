//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#ifndef DT_UTLVECTOR_COMMON_H
#define DT_UTLVECTOR_COMMON_H
#ifdef _WIN32
#pragma once
#endif


#include "utlvector.h"


typedef void (*EnsureCapacityFn)( void *pVoid, int offsetToUtlVector, int len );
typedef void (*ResizeUtlVectorFn)( void *pVoid, int offsetToUtlVector, int len );

template< class T >
void UtlVector_InitializeAllocatedElements( T *pBase, int count )
{
	memset( pBase, 0, count * sizeof( T ) );
}

template< class T, class A >
class UtlVectorTemplate
{
public:
	static void ResizeUtlVector( void *pStruct, int offsetToUtlVector, int len )
	{
		CUtlVector<T,A> *pVec = (CUtlVector<T,A>*)((char*)pStruct + offsetToUtlVector);
		if ( pVec->Count() < len )
			pVec->AddMultipleToTail( len - pVec->Count() );
		else if ( pVec->Count() > len )
			pVec->RemoveMultiple( len, pVec->Count()-len );
	}

	static void EnsureCapacity( void *pStruct, int offsetToUtlVector, int len )
	{
		CUtlVector<T,A> *pVec = (CUtlVector<T,A>*)((char*)pStruct + offsetToUtlVector);
		
		int oldNumAllocated = pVec->Count();

		if ( oldNumAllocated < len )
		{
			pVec->EnsureCapacity( len );
			
			// This is important to do because EnsureCapacity doesn't actually call the constructors
			// on the elements, but we need them to be initialized, otherwise it'll have out-of-range
			// values which will piss off the datatable encoder.
			UtlVector_InitializeAllocatedElements( pVec->Base() + oldNumAllocated, len - oldNumAllocated );
		}
	}
};

template< class T, class A >
inline ResizeUtlVectorFn GetResizeUtlVectorTemplate( CUtlVector<T,A> &vec )
{
	return &UtlVectorTemplate<T,A>::ResizeUtlVector;
}

template< class T, class A >
inline EnsureCapacityFn GetEnsureCapacityTemplate( CUtlVector<T,A> &vec )
{
	return &UtlVectorTemplate<T,A>::EnsureCapacity;
}


// Format and allocate a string.
char* AllocateStringHelper( const char *pFormat, ... );

// Allocates a string for a data table name. Data table names must be unique, so this will
// assert if you try to allocate a duplicate.
char* AllocateUniqueDataTableName( bool bSendTable, const char *pFormat, ... );


#endif // DT_UTLVECTOR_COMMON_H
