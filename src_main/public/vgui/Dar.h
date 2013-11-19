//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: Holds the enumerated list of default cursors
//
// $NoKeywords: $
//=============================================================================//

#ifndef DAR_H
#define DAR_H

#ifdef _WIN32
#pragma once
#endif

#include <stdlib.h>
#include <string.h>
#include <vgui/VGUI.h>
#include "tier1/utlvector.h"

#include "tier0/memdbgon.h"

namespace vgui
{

//-----------------------------------------------------------------------------
// Purpose: Simple lightweight dynamic array implementation
//-----------------------------------------------------------------------------
template<class ELEMTYPE> class Dar : public CUtlVector< ELEMTYPE >
{
	typedef CUtlVector< ELEMTYPE > BaseClass;
	
public:
	Dar()
	{
	}
	Dar(int initialCapacity) :
		BaseClass( 0, initialCapacity )
	{
	}

public:
	void SetCount(int count)
	{
		EnsureCount( count );
	}
	int GetCount()
	{
		return Count();
	}
	int AddElement(ELEMTYPE elem)
	{
		return AddToTail( elem );
	}
	void MoveElementToEnd( ELEMTYPE elem )
	{
		if ( Count() == 0 )
			return;

		// quick check to see if it's already at the end
		if ( Element( Count() - 1 ) == elem )
			return;

		int idx = Find( elem );
		if ( idx == InvalidIndex() )
			return;

		Remove( idx );
		AddToTail( elem );
	}
	// returns the index of the element in the array, -1 if not found
	int FindElement(ELEMTYPE elem)
	{
		return Find( elem );
	}
	bool HasElement(ELEMTYPE elem)
	{
		if ( FindElement(elem) != InvalidIndex() )
		{
			return true;
		}
		return false;
	}
	int PutElement(ELEMTYPE elem)
	{
		int index = FindElement(elem);
		if (index >= 0)
		{
			return index;
		}
		return AddElement(elem);
	}
	// insert element at index and move all the others down 1
	void InsertElementAt(ELEMTYPE elem,int index)
	{
		InsertBefore( index, elem );
	}
	void SetElementAt(ELEMTYPE elem,int index)
	{
		EnsureCount( index + 1 );
		Element( index ) = elem;
	}
	void RemoveElementAt(int index)
	{
		Remove( index );
	} 

	void RemoveElementsBefore(int index)
	{
		if ( index <= 0 )
			return;
		RemoveMultiple( 0, index - 1 );
	}  

	void RemoveElement(ELEMTYPE elem)
	{
		FindAndRemove( elem );
	}

	void *GetBaseData()
	{
		return Base();
	}

	void CopyFrom(Dar<ELEMTYPE> &dar)
	{
		CoypArray( dar.Base(), dar.Count() );
	}
};

}

#include "tier0/memdbgoff.h"

#endif // DAR_H
