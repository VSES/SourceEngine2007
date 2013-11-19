//***************************************************************************************
//
// File supervisor: Softimage 3D Games & 3D Bridge team
//
// (c) Copyright 1999-2002 Avid Technology, Inc. . All rights reserved.
//
//***************************************************************************************

/****************************************************************************************
THIS CODE IS PUBLISHED AS A SAMPLE ONLY AND IS PROVIDED "AS IS".
IN NO EVENT SHALL SOFTIMAGE, AVID TECHNOLOGY, INC. AND/OR THEIR RESPECTIVE
SUPPLIERS BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY
DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
CONNECTION WITH THE USE OR PERFORMANCE OF THIS CODE .

COPYRIGHT NOTICE. Copyright © 1999-2002 Avid Technology Inc. . All rights reserved. 

SOFTIMAGE is a registered trademark of Avid Technology Inc. or its subsidiaries 
or divisions. Windows NT is a registered trademark of Microsoft Corp. All other
trademarks contained herein are the property of their respective owners. 
****************************************************************************************/

//***************************************************************************************
// Defines
//***************************************************************************************
#ifndef __CSIBCArray_H__
#define __CSIBCArray_H__

#ifdef _DEBUG
#include <assert.h>
#define _SI_ASSERT(x) assert(x)	
#else
#define _SI_ASSERT(exp)
#endif

#include <string.h>

// Disable "identifier was truncated to '255' characters in the debug information" warning.
#if defined(_WIN32) || defined(_WIN32_WCE) || defined( _XBOX )
#pragma warning( disable : 4786 )
#endif

//***************************************************************************************
// CSIBCArray | Simple, all-purpose chunked array used by other classes.
//***************************************************************************************

//! Simple, all-purpose chunked array used by other classes.
/*! This class is frequently used throughout the FTK and Semantic Layer. It is the desired
	method for using arrays.
*/
template < class CElemType > 
class CSIBCArray
{
    private :
        long        m_lNbUsedElem;      // Number of elements used in the array
		long		m_lNbAllocElem;		// Number of elements allocated to the array
        CElemType	*m_pElem;           // Array of elements

    public:

		/*! Constructor.
			Allocates \p in_lNbElem elements of storage in the new array.
			\param in_lNbElem	Initial element storage space to allocate. (Defaults to 0).
		*/
        inline CSIBCArray( const long in_lNbElem = 0 );
        inline ~CSIBCArray() { DisposeData(); }

		/*! Returns the number of element storage spaces that contain valid elements.
			\return long	Number of used element storage spaces.
			\sa CSIBCArray::GetSize
			\sa CSIBCArray::UsedMem
		*/
        inline long GetUsed() const;

		/*! Returns the number of allocated element storage spaces.
			\return long	Number of allocated element storage spaces.
			\sa CSIBCArray::GetUsed
			\sa CSIBCArray::AllocatedMem
		*/
		inline long GetSize() const;

		/*! Returns the amount memory used by element storage spaces that contain valid elements.
			\return	long	The amount of memory used by valid element storage (in bytes).
			\sa CSIBCArray::GetUsed
			\sa CSIBCArray::AllocatedMem
		*/
		inline long UsedMem() const;

		/*! Returns the total memory allocated by element storage spaces in the array. This does
			not include the size of the SIBCArray object itself.
			\return	long	The amount of memory allocated by element storage spaces (in bytes).
			\sa CSIBCArray::GetSize
			\sa CSIBCArray::AllocatedMem
		*/
		inline long AllocatedMem() const;

		/*! Marks \p in_lNbElem elements as used. If there is sufficient allocated space to reserve
			\p in_lNbElem, no extra element storage is allocated. Otherwise, storage space is resized
			to make room for exactly \p in_lNbElem storage spaces.
			\param in_lNbElem	Number of element storage spaces to reserve.
			\return	long		The number of elements used in the array.
			\sa CSIBCArray::Resize
			\sa CSIBCArray::Extend
			\sa CSIBCArray::InsertAt
		*/
		inline long Reserve( const long in_lNbElem );

		/*! Changes the number of cells used in the array to \p in_lNewNbElem. If there is sufficient
			space allocated to set \p in_lNewNbElem elements used, no space is allocated. Otherwise,
			twice the requested number of elements are allocated to the array, and \p in_lNewNbElem
			spaces are set to used. This is done so allocation occurs less frequently.
			\param	in_lNewNbElem	Number of elements to resize the array to.
			\return	long			Number of used elements in the resized array.
			\sa CSIBCArray::Reserve
			\sa CSIBCArray::Extend
			\sa CSIBCArray::InsertAt
		*/
        inline long Resize( const long in_lNewNbElem );

		/*! Extends the number of cells used in the array by \p in_lNbElem elements. This is equivalent
			to calling SIBCArray::Resize with \p in_lNbElem + SIBCArray::GetUsed().
			\param in_lNbElem	Number of extra elements to set to used in the array.
			\return	long		Number of used elements in the extended array.
			\sa CSIBCArray::Reserve
			\sa CSIBCArray::Resize
			\sa CSIBCArray::InsertAt
			\sa CSIBCArray::Add
		*/
        inline long Extend( const long in_lNbElem );

		/*! Adds an element to the end of the array. 
			\param	in_Elem	Element of the template declaration type to add at the end of the array.
			\return	long	Number of used elements in the array, with the new element added.
			\sa CSIBCArray::Reserve
			\sa CSIBCArray::Resize
			\sa CSIBCArray::Extend
			\sa CSIBCArray::InsertAt
			\sa CSIBCArray::operator[]
		*/
		inline long Add(CElemType	in_Elem);

		/*! Inserts \p nb element storage spaces at the array index \p idx.
			\param	idx	Index to insert element storage spaces at.
			\param	nb	Number of element storage spaces to insert.
			\return	long	Number of used elements in the array, after the insertion.
			\sa CSIBCArray::Reserve
			\sa CSIBCArray::Resize
			\sa CSIBCArray::Extend
			\sa CSIBCArray::DeleteAt
			\sa CSIBCArray::Set
		*/
        inline long InsertAt(const long idx, const long nb);

		/*!	Deletes \p nb element storage spaces at the array index \p idx.
			\param	idx	Index to begin element deletion
			\param	nb	Number of elements to delete.
			\sa CSIBCArray::InsertAt
			\sa CSIBCArray::Set
		*/
        inline void DeleteAt(const long idx, const long nb);

		/*!	Indexing operator. Returns a reference to the element at the index \p in_lIndex.
			\param	in_lIndex	Index of the parameter to get.
			\return	CElemType&	Reference to the element at the index \p in_lIndex.
			\sa CSIBCArray::Add
			\sa CSIBCArray::Set
		*/
        inline CElemType & operator [] ( const unsigned long in_lIndex ) const;

		/*!	Indexing operator. Returns a reference to the element at the index \p in_lIndex.
			\param	in_lIndex	Index of the parameter to get.
			\return	CElemType&	Reference to the element at the index \p in_lIndex.
			\sa CSIBCArray::Add
			\sa CSIBCArray::Set
		*/
        inline CElemType & operator [] ( const unsigned long in_lIndex );

		/*! Discards element storage spaces allocated but not used in the array, if at least
			\p i_MaxWasted elements are unused. Note: This function should only be used after
			major array sizing is done.
			\param	i_MaxWasted	Number of elements required to be wasted before packing occurs (defaults to 4).
			\return	long	Number of elements used in the array.
			\sa CSIBCArray::DisposeData
		*/
        inline long Pack( long i_MaxWasted = 4 );

		/*! Deletes all element storage spaces, and sets allocated and used element storage spaces both
			to zero.
			\sa CSIBCArray::Become
		*/
        inline void DisposeData( void );

		/*! Sets this array to contain all data from the \p in_rSrcObject array, and then
			sets the \p in_rSrcObject to contain no elements. This destroys all data originally
			contained within this array.

			\param	in_rSrcObject	The object to take data from.
			\return	int		Always returns zero.
			\sa CSIBCArray::Copy
		*/
		inline int Become( CSIBCArray<CElemType>& in_rSrcObject );

		/*! Allocates memory in which to store as many (or more) elements contained in \p in_rSrcObject,
			and copies the data from \p in_rSrcObject into this array. This destroy all data originally
			stored within this array.

			\param	in_rSrcObject	The array source to copy data from.
			\return	int				Returns the number of elements used.
			\sa CSIBCArray::operator=
		*/
        inline int Copy( const CSIBCArray<CElemType>& in_rSrcObject );

		/*! Sets this array to contain the same information as \p in_rSrcObject. The functionality is
			equivalent to SIBCArray::Copy.
			\param	in_rSrcObject	Array object to copy data from.
			\return	SIBCArray<CElemType>&	Reference to this array.
			\sa CSIBCArray::Copy
		*/			
		inline CSIBCArray<CElemType>& operator = ( const CSIBCArray<CElemType>& in_rSrcObject );

		/*! Returns the array used internally by this object.
			\return CElemType*	Pointer to the first element in the internal array used by this object.
		*/
        inline CElemType * ArrayPtr( void ) { return m_pElem; }			

		/*! Returns a pointer to the array used internally by this object.
			\return CElemType**	Pointer to a pointer to the first element in the internal array used 
			by this object.
		*/			
        inline CElemType * * ArrayPtrPtr( void ) { return &m_pElem; }

		/*! Sets a given number of elements, \p nb, starting at index \p start to the
			element given by \p value.
			\param	start	The index to start the modification of elements.
			\param	nb		Number of elements to set the value of.
			\param	value	The value to set the elements to.
		*/
        inline void Set( long start, long nb, CElemType value );

	private:

};

// Constructor
template <class CElemType>
inline CSIBCArray<CElemType>::CSIBCArray
(
    const long in_lNbElem
) : m_lNbUsedElem( 0 ),
    m_pElem( NULL ),
	m_lNbAllocElem( 0 )
{	
    if ( in_lNbElem > 0 )
        Resize( in_lNbElem );
}

//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Gets the current number of elements used in the array.
//
// class | CElemType | Type of element.
//
// Returns the number of used elements in the array.
//
//***************************************************************************************
template <class CElemType>
inline long  CSIBCArray<CElemType>::GetUsed() const
{
	return m_lNbUsedElem;
}

//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Gets the current number of elements allocated for the array.
//
// class | CElemType | Type of element.
//
// Returns the number of elements allocated in the array.
//
//***************************************************************************************
template <class CElemType>
inline long  CSIBCArray<CElemType>::GetSize() const
{
	return m_lNbAllocElem;
}

//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Returns how much memory used by the used array elements.
//
// class | CElemType | Type of element.
//
// Returns how much memory is used by used array elements.
//***************************************************************************************
template <class CElemType>
inline long CSIBCArray<CElemType>::UsedMem() const
{
	return m_lNbUsedElem * sizeof( CElemType );
}

//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Returns how much memory is allocated for the array.
//
// class | CElemType | Type of element.
//
// Returns how much memory is allocated for the array.
//***************************************************************************************
template <class CElemType>
inline long CSIBCArray<CElemType>::AllocatedMem() const
{
	return m_lNbAllocElem * sizeof( CElemType );
}

//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Like Resize, but allocates only the requested amount. If the requested amount is
//			greater than the specified one, it marks the number of used objects but
//			does not allocate any more.
//
// long | in_lNbElem | number of elements to allocate.
//
// class | CElemType | Type of element.
//
// returns the number of used elements.
//
//***************************************************************************************
template <class CElemType>
inline long CSIBCArray<CElemType>::Reserve( const long in_lNbElem )
{
    CElemType * l_pTmp;
	int loop;

	//If we've got allocated elements that are unused
	//use one of them instead of allocating more memory
    if ( m_lNbAllocElem >= in_lNbElem )
    {
        return ( m_lNbUsedElem = in_lNbElem );
    }
	else
	{
		//Create the new array consisting of the desired number of elements
		l_pTmp = new CElemType[in_lNbElem];
		
		//If it was successful, copy the old array over
    	if (l_pTmp != NULL )
    	{
			//Copy and remove memory
			if (m_pElem)
			{
/*
				memcpy( l_pTmp, m_pElem, m_lNbUsedElem * sizeof( CElemType ));
*/
				for (loop = 0; loop < m_lNbUsedElem; loop++)
					l_pTmp[loop] = m_pElem[loop];
				
				delete [] m_pElem;
			}

			//Assign member array to the newly created array.
        	m_pElem = l_pTmp;


			//Number allocated is different than the number used.
			m_lNbAllocElem = in_lNbElem;

        	return ( m_lNbUsedElem = in_lNbElem );
    	}

    	return m_lNbUsedElem;
	}
}

//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Resizes changes the number of used cells in the array.  This is the primary device
//			to change the size of the array.  If we already have enough space for the 
//			number requested, the array is not resized, just the number of used elements is changed.
//			If we need to allocate more space, we allocate double the requested amount and set the
//			the amount used to the number requested.  This prevents us from having to allocate more
//			frequently.
//
// long | in_lNewNbElem | number of elements to allocate.
//
// class | CElemType | Type of element.
//
// Returns the number of used elements
//
//***************************************************************************************
template <class CElemType>
inline long CSIBCArray<CElemType>::Resize( const long in_lNewNbElem )
{
    CElemType * l_pTmp;
    int i;

	//If we've got allocated elements that are unused
	//use one of them instead of allocating more memory
    if ( m_lNbAllocElem >= in_lNewNbElem )
    {
        return ( m_lNbUsedElem = in_lNewNbElem );
    }
	else
	{
		//Add 100% onto the array in order to buffer
		long l_lNbAlloc = in_lNewNbElem + in_lNewNbElem;
	    
		//Create the new array consisting of the desired number of elements
		l_pTmp = new CElemType[l_lNbAlloc];

		//If it was successful, copy the old array over
    	if (l_pTmp != NULL )
    	{
			//Copy and remove memory
			if (m_pElem)
			{
				for ( i = 0; i < m_lNbUsedElem; i++ )
				{
					l_pTmp[i] = m_pElem[i];
				}

				delete [] m_pElem;
			}

			//Assign member array to the newly created array.
        	m_pElem = l_pTmp;


			//Number allocated is different than the number used.
			m_lNbAllocElem = l_lNbAlloc;

        	return ( m_lNbUsedElem = in_lNewNbElem );
    	}

    	return m_lNbUsedElem;
	}
}

//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Calls resize with the current number used + the number requested.
//
// long | in_lNbElem | number of elements requested.
//
// class | CElemType | Type of element.
//
// rdesc	Returns the number of used elements.
//
//***************************************************************************************
template <class CElemType>
inline long CSIBCArray<CElemType>::Extend( const long in_lNbElem )
{
	return Resize( m_lNbUsedElem + in_lNbElem );
}


//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Copies another array.
//
// class | CElemType | Type of element.
//
// Returns the number of used elements.
//
//***************************************************************************************
template <class CElemType>
inline int CSIBCArray<CElemType>::Copy
(
    const CSIBCArray<CElemType> & in_rSrcObject
)
{
	int		loop;
	DisposeData();



	if (in_rSrcObject.GetUsed() > 0)
	{
		//Pre-allocate needed memory
		Reserve( in_rSrcObject.GetUsed() );

		//Say that we will use all of it.
		Resize( in_rSrcObject.GetUsed() );

		//Check to see that everything worked correctly
		if (m_lNbUsedElem >= in_rSrcObject.GetUsed())
		{
			//Copy over data from other object
			for (loop = 0; loop < m_lNbUsedElem; loop++)
				m_pElem[loop] = in_rSrcObject.m_pElem[loop];


			//Return the number of elements in the array
    		return m_lNbUsedElem;
		}
	}

	return 0;
}

//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Moves the array data from in_rSrcObject to this CSIBArray and sets in_rSrcObject to null.
//
// class | CElemType | Type of element.
//
// Returns 0.
//***************************************************************************************
template <class CElemType>
inline int CSIBCArray<CElemType>::Become( CSIBCArray<CElemType>& in_rSrcObject )
{
	DisposeData();

	//Copy values
	m_pElem			= in_rSrcObject.m_pElem;
	m_lNbUsedElem	= in_rSrcObject.m_lNbUsedElem;
	m_lNbAllocElem	= in_rSrcObject.m_lNbAllocElem;

	//kill source
	in_rSrcObject.m_pElem			= NULL;
	in_rSrcObject.m_lNbUsedElem		= 0;
	in_rSrcObject.m_lNbAllocElem	= 0;

	return 0;
}

//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Copies another array.
//
// class | CElemType | Type of element.
//
// Returns a pointer to the array.
//
//***************************************************************************************
template <class CElemType>
inline CSIBCArray<CElemType> & CSIBCArray<CElemType>::operator = 
(
	const CSIBCArray<CElemType> & i_That
)
{
    this->Copy( i_That );

	return *this;
}

//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Deletes all the data allocated to the object and sets the number used/allocated to 0.
//
// class | CElemType | Type of element.
//
// Returns void.
//
//***************************************************************************************

template <class CElemType> 
inline void CSIBCArray<CElemType>::DisposeData()
{
	//If the elements exist, delete them
    if ( m_pElem != NULL )
    {
		//Delete the array pointer and set it to null for reference
        delete [] m_pElem;
	}

	//Eliminate the counts to 0.
    m_lNbUsedElem = 0;
	m_lNbAllocElem = 0;
	m_pElem = NULL;
}

//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Removes the excess space after the array by allocating a new one of the same length
//	and then copying the old one over.  To be used after major array resizing only.
//
// long | i_MaxWasted | number of elements in array.
//
// class | CElemType | Type of element.
//
// Returns the number possible based on the used elements.
//
//***************************************************************************************

template <class CElemType> 
long CSIBCArray<CElemType>::Pack( long i_MaxWasted )
{
	int loop;

	//Check to see if packing is neccesary
    if (( m_lNbAllocElem - m_lNbUsedElem ) > i_MaxWasted )
    {
		
		//Update the number of allocated element count
		m_lNbAllocElem = m_lNbUsedElem;		

		//Create the new array
		CElemType * l_pTmp = new CElemType[m_lNbUsedElem];        

		//If we were successful, copy the old elements over
        if ( l_pTmp != NULL )
        {
			//If there are old elements, copy them
			if (m_pElem)
			{
				//Byte copy old elements (number * size)
/*
				memcpy( l_pTmp, m_pElem, m_lNbUsedElem * sizeof(CElemType) );
*/
				for (loop = 0; loop < m_lNbUsedElem; loop++)
					l_pTmp[loop] = m_pElem[loop];
				
				//Delete original array
				delete [] m_pElem;
			}

			//copy pointer to new array since old was deleted or null
            m_pElem = l_pTmp;
        }

    }

	//Return the number possible based on the used elements
    return m_lNbUsedElem;

}

//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Indexes into the array.
//
// class | CElemType | Type of element.
//
// Returns a reference to the element in the array.
//
//***************************************************************************************
template <class CElemType>
inline CElemType & CSIBCArray<CElemType>::operator []
(
    const unsigned long in_lIndex
) const
{
	_SI_ASSERT(in_lIndex < ((unsigned long)m_lNbUsedElem));
	_SI_ASSERT(in_lIndex >= 0);
    return m_pElem[in_lIndex];
}

// Operator [] overload
template <class CElemType>
inline CElemType & CSIBCArray<CElemType>::operator []
(
    const unsigned long in_lIndex
)
{
	_SI_ASSERT(in_lIndex < ((unsigned long)m_lNbUsedElem));
	_SI_ASSERT(in_lIndex >= 0);
    return m_pElem[in_lIndex];
}

//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Sets a range of elements to the specified value.
//
// long | start | first element in the array.
// long | nb	 | number of elements in the array.
//
// class | CElemType | Type of element.
//
// Returns void
//
//***************************************************************************************
template <class CElemType> 
inline void CSIBCArray<CElemType>::Set
( 
 
    long start, 
	long nb, 
	CElemType value 
)
{
	//Make sure elements exist
    if ( m_pElem != NULL )
    {
        long    l_lEnd = start + nb;

        if ( l_lEnd > m_lNbUsedElem )
            l_lEnd = m_lNbUsedElem;

        for ( long i = start; i < l_lEnd; i++ )
            m_pElem[i] = value;
    }
}

//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Inserts elements at the specified location.
//
// long | in_lIndex | index location.
// long | in_lNbElem | number of elements.
//
// class | CElemType | Type of element.
//
// Returns the number of used elements.
//
//***************************************************************************************
template <class CElemType>
inline long CSIBCArray<CElemType>::InsertAt
(
    const long in_lIndex,
    const long in_lNbElem
)
{
    long    l_lOldNb	= m_lNbUsedElem;
    long    l_lNb		= m_lNbUsedElem + in_lNbElem;
	long	l_lInsert	= in_lIndex;

	//Check to make sure it's inside the boudns
	if (l_lInsert > m_lNbUsedElem)
		l_lInsert = m_lNbUsedElem;

	//Ensure we have enough space
    Reserve( l_lNb );

	//Use all of it
	Resize( l_lNb );

   	if (l_lOldNb > in_lIndex)	
       	memmove(&m_pElem[in_lIndex + in_lNbElem], &m_pElem[in_lIndex], (unsigned int) (sizeof(CElemType) *  (l_lOldNb - in_lIndex)));

	return m_lNbUsedElem;    
}

//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Removes some cells at the place specified.
//
// long | in_lIndex | index location.
// long | in_lNbElem | number of elements in the array.
//
// class | CElemType | Type of element.
//
// Returns void.
//
//***************************************************************************************
template <class CElemType>
inline void CSIBCArray<CElemType>::DeleteAt
(
    const long in_lIndex,
    const long in_lNbElem
)
{
	long l_lToDelete = in_lNbElem;

	if ( l_lToDelete > m_lNbUsedElem  )
		l_lToDelete = m_lNbUsedElem;

    if ((in_lIndex + in_lNbElem) < m_lNbUsedElem )
        memmove( &m_pElem[ in_lIndex ], &m_pElem [ in_lIndex + l_lToDelete ], (unsigned int) (sizeof(CElemType) * (m_lNbUsedElem -  (in_lIndex + l_lToDelete ))));

	m_lNbUsedElem -= l_lToDelete;
}

//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// adds a new element
//
// CElemType | in_Elem | element to add at the end of the array
//
// class | CElemType | Type of element.
//
// Returns the number of used elements.
//
//***************************************************************************************
template <class CElemType>
inline long CSIBCArray<CElemType>::Add
(
    CElemType in_Elem
)
{
	Extend(1);
	m_pElem[m_lNbUsedElem - 1] = in_Elem;
	return m_lNbUsedElem;    

}

#ifdef _WIN32
#pragma warning(disable:4251)
#endif
                            
#endif // ALREADY_INCLUDED_CSIBCArray
