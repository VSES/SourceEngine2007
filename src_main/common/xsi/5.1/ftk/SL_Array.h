//***************************************************************************************
//
// File supervisor: Softimage 3D Games & 3D Bridge team
//
// (c) Copyright 2001-2002 Avid Technology, Inc. . All rights reserved.
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

#ifndef __SL_ARRAY_H__
#define __SL_ARRAY_H__

#if defined(_WIN32) || defined(_WIN32_WCE) || defined(_XBOX)
// Disable "identifier was truncated to '255' characters in the debug information" warning.
#pragma warning( disable : 4786 )
#endif // defined(_WIN32) || defined(_WIN32_WCE)

#include <SIBCArray.h>	// CdotXSITemplate
#include "SL_Int.h"	// CSLIntProxy
#include <new.h>

// Align structure elements on 4-byte boundaries.
#if defined(_WIN32) || defined(_WIN32_WCE) || defined(_XBOX)
#pragma pack(push,4)
#elif defined(CODEWARRIOR)
#pragma options align= mac86k4byte
#endif

//! Proxy class that allows the manipulation of an array parameter in a template.
/*!
	\param CElemType Element type contained in the array.
	\param CSubElemType Sub element type composing the \p CElemType.  Can be the same as \p CElemType if \p StructSize equals one.
	\param StructSize Number of \p CSubElemType that composes a single \p CElemType.

	\note The distinction between CElemType and CSubElemType hides the gory 
	details of the way an element is stored.  For instance, an RGB value array
	could be stored as a SI_Float array.  This would normally mean that you 
	would have to access the red, green and blue values individually.  The 
	array proxy lets you access the three elements as if the array was in fact
	an array of RGB structure.
	
	\b Example:
	\code
	struct RGB
	{
		SI_Float m_fR;
		SI_Float m_fG;
		SI_Float m_fB;
	} l_RGB;

	// Connect the proxy to a parameter that stores an SI_Float array
	// for RGB values.  The array in the template is a sequence of
	// float triplets that represent red, green and blue values.
	CSLArrayProxy<RGB, SI_Float, 3> l_RGBArrayProxy;
	l_RGBArrayProxy.Connect( l_pParentTemplate, l_nRGBParamIndex );

	// get the first RGB entry
	l_RGB = l_RGBArrayProxy[0]; // the first 3 floats for the RGB value.
	\endcode
*/
template <class CElemType, class CSubElemType, SI_Int StructSize>
class XSIEXPORT CSLArrayProxy
{
public:
	/*! Constructor. Connects to an array parameter in the parent template.
		\param in_pTemplate Pointer to the parent template
		\param in_Index Index of the array paramater in the template
	 */
    CSLArrayProxy( CdotXSITemplate *in_pTemplate, SI_Long in_Index);

	/*! Default Constructor. Does not call Connect.
	*/
	CSLArrayProxy();

	/*! Destructor
		\warning The destructor does \b not deallocate the referred parameter.
	*/
    ~CSLArrayProxy();

	/*! Connects to an array parameter in the parent template.
		\param in_pTemplate Pointer to the parent template
		\param in_lArrayIndex Index of the array in the template
		\retval SI_SUCCESS
	 */
	SI_Error Connect( CdotXSITemplate *in_pTemplate, SI_Long in_lArrayIndex );

	/*! Returns the number of used cells
		\return The number of used cells (i.e. the ones you can access)
		\note The size of the internally stored array might diverge from the actual 
		number of cells that are currently used.  If you need to know the total number 
		of allocated cells, you should	use the CSLArrayProxy::GetSize function instead.
	*/
    inline SI_Long GetUsed() const;		

	/*! Returns the number of allocated cells
		\return The number of cells allocated
		\note The number of cells that are being used can be retrieved with the
		CSLArrayProxy::GetUsed function.
	*/
	inline SI_Long GetSize() const;		

	/*! Returns The total size of the used cells
		\return Size in byte
		\sa CSLArrayProxy::GetUsed
	*/
	inline SI_Long UsedMem() const;		

	/*! Returns the total size of the allocated cells including the unused ones.
		\return Size in byte
		\sa CSLArrayProxy::GetSize
	*/
	inline SI_Long AllocatedMem() const;	

	/*! Adds a specific number of used cells to the array as required
		\param in_lNbElem Number of cells to add
		\return The number of used cells
		\note If the allocated size is large enough, no cells are going to
		be allocated.  If you expect to add lots of cells frequently, you 
		should consider using CSLArrayProxy::Resize instead.

		\sa CSLArrayProxy::Resize
		\sa CSLArrayProxy::GetSize
		\sa CSLArrayProxy::GetUsed
	*/
	SI_Long Reserve( const SI_Long in_lNbElem );			

	/*! Adds used cells to the array as required.
		\return The number of used cells
		\note If the allocated size is large enough, no cells are going to
		be allocated.  Otherwise, \p in_lNbElem used cells are going to be 
		added twice.  This is can improve performance by allocating memory 
		less frequently than CSLArrayProxy::Reserve would.  If you will need
		\b lots of used cells in the near future, you should consider using 
		CSLArrayProxy::Extend.

		\sa CSLArrayProxy::Reserve
		\sa CSLArrayProxy::Extend
		\sa CSLArrayProxy::GetSize
		\sa CSLArrayProxy::GetUsed
	*/
    SI_Long Resize( const SI_Long in_lNewNbElem );			

	/*! Extends the number of used cells by a substantial amount if there's
		not enough used cells.
		\param in_lNewNbElem The number of additional used cells to add
		\return The number of used cells
		\note This is the equivalent of calling CSLArrayProxy::Resize with the
		actual number of cells plus	\p in_lNewNbElem.  This means that if 
		there's not enough room for this amout of used cells, twice as much 
		will be allocated (that's more than twice the original size of the 
		array).  You should therefore use this function when you expect the 
		number of used cells to grow a lot in order to improve performance.

		\sa CSLArrayProxy::Reserve
		\sa CSLArrayProxy::Resize
	*/
    SI_Long Extend( const SI_Long in_lNbElem );			

	/*! Inserts an exact number of used cells at the provided index
		\param in_lIndex Insertion point
		\param in_lNbElem Number of used cells to add
		\return The number of used cells
		\note Enough used cells are going to be added to the array if it
		cannot support the new number of used cells.

		\note The array will only be increased by the number of missing
		used cells slots.  If you expect a lot of insertion, you should
		consider calling CSLArrayProxy::Resize or CSLArrayProxy::Extend
		before using this function in order to improve performance.

		\sa CSLArrayProxy::Add
		\sa CSLArrayProxy::Extend
		\sa CSLArrayProxy::Resize
	*/
    SI_Long InsertAt(const SI_Long in_lIndex, const SI_Long in_lNbElem);
	
	/*! Adds an element at the end of the array
		\param in_Elem Element to add
		\return The number of used cells
		\note The array will resize if there is not enough unused cells.
		
		\sa CSLArrayProxy::InsertAt
		\sa CSLArrayProxy::Extend
		\sa CSLArrayProxy::Resize
	*/
	inline SI_Long Add(CElemType in_Elem);

	/*! Deletes a given number of element at a the specified index
		\param in_lIndex	The index of the element where deletion must start
		\param in_lNbElem	The number of elements to delete (must be positive)
	*/
    SI_Void DeleteAt(const SI_Long in_lIndex, const SI_Long in_lNbElem);	

	/*! Gets an element at a given index
		\param in_lIndex	Index of the element to retrieve
		\return	The element at \p in_lIndex
	*/
    inline CElemType & operator [] ( const SI_Long in_lIndex ) const;	

	/*! Gets an element at a given index
		\param in_lIndex	Index of the element to retrieve
		\return	The element at \p in_lIndex
	*/
    inline CElemType & operator [] ( const SI_Long in_lIndex );

	/*! Deallocates unused cells
		\param i_lMaxWasted Maximum number of allowed unused cells
		\return The number of used cells
		\note Packing is performed only if there is more than \p i_lMaxWasted 
		unused cells.
		\warning This costly function should only be called after major 
		resizing.
	*/
    SI_Long Pack( SI_Long i_lMaxWasted = 4L );		

	/*! Releases the referred array
		\note An array of 1 element will replace the former array.
	*/
    SI_Void DisposeData( SI_Void );				

	/*! Copy data from another array
		\param in_rSrcObject	The array to copy data from
		\return	The number of used cells
		\warning The current array's content will be deallocated
	*/
	SI_Int Copy( const CSIBCArray<CElemType>& in_rSrcObject );

	/*! Copy data from another array
		\param in_rSrcObject	The array to copy data from
		\return The number of used cells
		\warning The current array's content will be deallocated
	*/
	SI_Int Copy( const CSLArrayProxy<CElemType, CSubElemType, StructSize>& in_rSrcObject );

	/*! Copy data from another array
		\param in_rSrcObject	The array to copy data from
		\return A reference to the current array
		\warning The current array's content will be deallocated
	*/
	CSLArrayProxy& operator = ( const CSIBCArray<CElemType>& in_rSrcObject );

	/*! Copy data from another array
		\param in_rSrcObject	The array to copy data from
		\return Reference to the current array
		\warning The current array's content will be deallocated
	*/
	CSLArrayProxy& operator = ( const CSLArrayProxy<CElemType, CSubElemType, StructSize>& in_rSrcObject );

    /*! Returns a pointer to the internally referred array.
		\return Pointer to the array
	*/
	inline CElemType* ArrayPtr( SI_Void );

	/*! Sets values in a given range
		\param start	The index of first element to set
		\param in_lNbElem	The number of elements to set
		\param value	New value for the elements in the given range.
	*/
    inline SI_Void Set( SI_Long start, SI_Long in_lNbElem, CElemType value );	

private :
    SI_Long m_lNbUsedElem;
	SI_Long m_lNbAllocElem;

	SI_TinyVariant *m_pVariant;

	SI_Long ExpandArray
	( 
		const SI_Long in_lNewSize, 
		const SI_Long in_lExpandRatio = 1
	);
};

///////////////////////////////////////////////////////////////////////////////
// implementation

template <class CElemType, class CSubElemType, SI_Int StructSize>
CSLArrayProxy<CElemType, CSubElemType, StructSize>::CSLArrayProxy
( 
	CdotXSITemplate *in_pTemplate, 
	SI_Long in_lArrayIndex
)
{
	Connect(in_pTemplate, in_lArrayIndex);
}

template <class CElemType, class CSubElemType, SI_Int StructSize>
CSLArrayProxy<CElemType, CSubElemType, StructSize>::CSLArrayProxy
( 
)
{
}

template <class CElemType, class CSubElemType, SI_Int StructSize> 
CSLArrayProxy<CElemType, CSubElemType, StructSize>::~CSLArrayProxy()
{ 
}

template <class CElemType, class CSubElemType, SI_Int StructSize>
SI_Error CSLArrayProxy<CElemType, CSubElemType, StructSize>::Connect
( 
	CdotXSITemplate *in_pTemplate, 
	SI_Long in_lArrayIndex
) 
{
	// get the pointer to the array
	_SI_ASSERT(in_pTemplate);
	CdotXSIParam *l_pParam = in_pTemplate->Params().Item( in_lArrayIndex );

	_SI_ASSERT(l_pParam);
	m_pVariant = l_pParam->GetVariantPtr();

	// get the current size of the array
	_SI_ASSERT(m_pVariant);

	if(m_pVariant->p_voidVal == NULL)
	{
		m_pVariant->numElems = 0;
	}

	m_lNbUsedElem = m_pVariant->numElems / StructSize;
	m_lNbAllocElem = m_lNbUsedElem;

	return SI_SUCCESS;
}

template <class CElemType, class CSubElemType, SI_Int StructSize> 
inline SI_Long CSLArrayProxy<CElemType, CSubElemType, StructSize>::GetUsed() const
{
	return m_lNbUsedElem;
}

template <class CElemType, class CSubElemType, SI_Int StructSize> 
inline SI_Long CSLArrayProxy<CElemType, CSubElemType, StructSize>::GetSize() const
{
	return m_lNbAllocElem;
}

template <class CElemType, class CSubElemType, SI_Int StructSize> 
inline SI_Long CSLArrayProxy<CElemType, CSubElemType, StructSize>::UsedMem() const
{
	return GetUsed() * StructSize;
}

template <class CElemType, class CSubElemType, SI_Int StructSize> 
inline SI_Long CSLArrayProxy<CElemType, CSubElemType, StructSize>::AllocatedMem() const
{
	return GetSize() * StructSize;
}

template <class CElemType, class CSubElemType, SI_Int StructSize> 
SI_Long CSLArrayProxy<CElemType, CSubElemType, StructSize>::Reserve
( 
	const SI_Long in_lNbElem 
)
{
	return ExpandArray(in_lNbElem);
}

template <class CElemType, class CSubElemType, SI_Int StructSize> 
SI_Long CSLArrayProxy<CElemType, CSubElemType, StructSize>::Resize
( 
	const SI_Long in_lNewNbElem 
)
{
	return ExpandArray( in_lNewNbElem, 2 );
}

template <class CElemType, class CSubElemType, SI_Int StructSize> 
SI_Long CSLArrayProxy<CElemType, CSubElemType, StructSize>::Extend
( 
	const SI_Long in_lNbElem 
)
{
	return Resize( m_lNbUsedElem + in_lNbElem );
}

template <class CElemType, class CSubElemType, SI_Int StructSize> 
SI_Long CSLArrayProxy<CElemType, CSubElemType, StructSize>::InsertAt
(
	const SI_Long in_lIndex, 
	const SI_Long in_lNbElem
)
{
	_SI_ASSERT(in_lNbElem);	// inserting 0 elements does not make any sense

    SI_Long    l_lOldNb	= m_lNbUsedElem;
    SI_Long    l_lNb		= m_lNbUsedElem + in_lNbElem;
	SI_Long	l_lInsert	= in_lIndex;

	//Check to make sure it's inside the boudns
	if (l_lInsert > m_lNbUsedElem)
		l_lInsert = m_lNbUsedElem;

	//Ensure we have enough space
    Reserve( l_lNb );

   	if (l_lOldNb > in_lIndex)
	{
		CElemType *l_pDest = ((CElemType*)m_pVariant->p_voidVal) + (in_lIndex + in_lNbElem);
		CElemType *l_pSrc  = ((CElemType*)m_pVariant->p_voidVal) + in_lIndex;

		memmove( l_pDest, l_pSrc, (l_lOldNb - in_lIndex) * sizeof(CSubElemType) * StructSize);
	}

	m_pVariant->numElems = m_lNbUsedElem * StructSize;
	return m_lNbUsedElem;
}

template <class CElemType, class CSubElemType, SI_Int StructSize> 
SI_Void CSLArrayProxy<CElemType, CSubElemType, StructSize>::DeleteAt
(
	const SI_Long in_lIndex, 
	const SI_Long in_lNbElem
)
{
	_SI_ASSERT(in_lNbElem);  // deleting 0 elements does not make any sense
	SI_Long l_lToDelete = in_lNbElem;

	if ( l_lToDelete > m_lNbUsedElem  )
		l_lToDelete = m_lNbUsedElem;

    if ((in_lIndex + in_lNbElem) < m_lNbUsedElem )
	{
		CElemType *l_pSrc = ((CElemType*)m_pVariant->p_voidVal) + in_lIndex + in_lNbElem;
		CElemType *l_pDest= ((CElemType*)m_pVariant->p_voidVal) + in_lIndex ;
		CElemType *l_pEnd = ((CElemType*)m_pVariant->p_voidVal) + m_lNbUsedElem;

		memmove( l_pDest, l_pSrc, (l_pEnd - l_pSrc) * sizeof(CSubElemType) * StructSize);
	}

	m_lNbUsedElem = m_lNbUsedElem - l_lToDelete;
	m_pVariant->numElems = m_lNbUsedElem * StructSize;
}

template <class CElemType, class CSubElemType, SI_Int StructSize> 
inline CElemType& CSLArrayProxy<CElemType, CSubElemType, StructSize>::operator []
( 
	const SI_Long in_lIndex 
) const
{
	_SI_ASSERT(in_lIndex < ((SI_Long)m_lNbUsedElem));
	_SI_ASSERT(in_lIndex >= 0);

	return *( ( (CElemType*) m_pVariant->p_voidVal ) + in_lIndex);
}

template <class CElemType, class CSubElemType, SI_Int StructSize> 
inline CElemType& CSLArrayProxy<CElemType, CSubElemType, StructSize>::operator []
( 
	const SI_Long in_lIndex 
)
{
	_SI_ASSERT(in_lIndex < ((SI_Long)m_lNbUsedElem));
	_SI_ASSERT(in_lIndex >= 0);

	return *( ( (CElemType*) m_pVariant->p_voidVal ) + in_lIndex);
}

template <class CElemType, class CSubElemType, SI_Int StructSize> 
SI_Long CSLArrayProxy<CElemType, CSubElemType, StructSize>::Pack
( 
	SI_Long i_lMaxWasted 
)
{
	//Check to see if packing is neccesary
    if (( m_lNbAllocElem - m_lNbUsedElem ) > i_lMaxWasted )
    {
		//Create the new array
		CElemType * l_pNewArray = (CElemType *) FTK_calloc( m_lNbUsedElem * StructSize, sizeof( CSubElemType )) ;

		//If we were successful
        if ( l_pNewArray != NULL )
        {
			// copy exisiting array data into new array
			CElemType *l_pCurrentArray = (CElemType*)m_pVariant->p_voidVal;
			register SI_Long i;

			for ( i = 0; i < GetUsed(); i++)
			{
				l_pNewArray[i] = l_pCurrentArray[i];
			}

			//Delete original array
			for ( i = 0; i < GetUsed(); i++)
			{
				l_pCurrentArray[i].~CElemType();
			}
			FTK_free((void *) l_pCurrentArray );

			//copy pointer to new array since old was deleted or null
            m_pVariant->p_voidVal = l_pNewArray;

			// update number of allocated elements
			m_lNbAllocElem = m_lNbUsedElem;		
			m_pVariant->numElems = m_lNbUsedElem * StructSize;
        }
    }

	return m_lNbUsedElem;
}

template <class CElemType, class CSubElemType, SI_Int StructSize> 
SI_Void CSLArrayProxy<CElemType, CSubElemType, StructSize>::DisposeData
()
{
	CElemType *l_pCurrentArray = (CElemType*)m_pVariant->p_voidVal;
	for (register SI_Long i = 0; i < m_lNbAllocElem; i++ )
	{
		l_pCurrentArray[i].~CElemType();
	}
	FTK_free( (void *) m_pVariant->p_voidVal );
	m_lNbAllocElem = 0; m_lNbUsedElem = 0; 
	m_pVariant->numElems = 0;

	Reserve(1);	// we don't want a null ptr here.  
}

template <class CElemType, class CSubElemType, SI_Int StructSize> 
SI_Int CSLArrayProxy<CElemType, CSubElemType, StructSize>::Copy
( 
	const CSIBCArray<CElemType>& in_rSrcObject 
)
{
	DisposeData();

	if (in_rSrcObject.GetUsed() > 0)
	{
		//Pre-allocate needed memory
		Reserve( in_rSrcObject.GetUsed() );

		//Check to see that everything worked correctly
		if (m_lNbUsedElem >= in_rSrcObject.GetUsed())
		{
			CElemType *l_pDestinationArray = (CElemType*)m_pVariant->p_voidVal;
			
			//Copy over data from other object
			for ( register SI_Long i = 0; i < m_lNbUsedElem; i++)
			{
				l_pDestinationArray[i] =  in_rSrcObject[i];
			}

			//Returns the number of elements in the array
    		return m_lNbUsedElem;
		}
	}

	return 0;
}

template <class CElemType, class CSubElemType, SI_Int StructSize> 
SI_Int CSLArrayProxy<CElemType, CSubElemType, StructSize>::Copy
( 
	const CSLArrayProxy<CElemType, CSubElemType, StructSize>& in_rSrcObject 
)
{
	DisposeData();

	if (in_rSrcObject.GetUsed() > 0)
	{
		//Pre-allocate needed memory
		Reserve( in_rSrcObject.GetUsed() );

		//Check to see that everything worked correctly
		if (m_lNbUsedElem >= in_rSrcObject.GetUsed())
		{
			CElemType *l_pDestinationArray = (CElemType*)m_pVariant->p_voidVal;
			
			//Copy over data from other object
			for ( register SI_Long i = 0; i < m_lNbUsedElem; i++)
			{
				l_pDestinationArray[i] =  in_rSrcObject[i];
			}

			//Returns the number of elements in the array
    		return m_lNbUsedElem;
		}
	}

	return 0;
}

template <class CElemType, class CSubElemType, SI_Int StructSize> 
CSLArrayProxy<CElemType, CSubElemType, StructSize>& CSLArrayProxy<CElemType, CSubElemType, StructSize>::operator =
( 
	const CSIBCArray<CElemType>& in_rSrcObject 
)
{
	Copy(in_rSrcObject);
	return *this;
}

template <class CElemType, class CSubElemType, SI_Int StructSize> 
CSLArrayProxy<CElemType, CSubElemType, StructSize>& CSLArrayProxy<CElemType, CSubElemType, StructSize>::operator =
( 
	const CSLArrayProxy<CElemType, CSubElemType, StructSize>& in_rSrcObject 
)
{
	Copy(in_rSrcObject);
	return *this;
}

template <class CElemType, class CSubElemType, SI_Int StructSize> 
inline CElemType* CSLArrayProxy<CElemType, CSubElemType, StructSize>::ArrayPtr()
{ 
	return (CElemType*)m_pVariant->p_voidVal;
}			

template <class CElemType, class CSubElemType, SI_Int StructSize> 
inline SI_Void CSLArrayProxy<CElemType, CSubElemType, StructSize>::Set
( 
	SI_Long in_lstart, 
	SI_Long in_lNbElem, 
	CElemType in_Value 
)
{
    SI_Long    l_lEnd = in_lstart + in_lNbElem;

    if ( l_lEnd > m_lNbUsedElem )
	{
        l_lEnd = m_lNbUsedElem;
	}

	CElemType* l_pArrayToSet = (CElemType*)m_pVariant->p_voidVal;
    for ( register SI_Long i = in_lstart; i < l_lEnd; i++ )
	{
        l_pArrayToSet[i] = in_Value;
	}
}

/*! \internal Increases the size of the referred array as required.  Will allocate
	in_l_ExpandRation * in_lNewSize used cells if there is not enough memory.
	\param in_lNewSize Elements to add
	\param in_lExpandRatio Element multiplier when there's not enough memory
	\return Total number of used cells
*/
template <class CElemType, class CSubElemType, SI_Int StructSize> 
SI_Long CSLArrayProxy<CElemType, CSubElemType, StructSize>::ExpandArray
( 
	const SI_Long in_lNewSize,
	const SI_Long in_lExpandRatio	
)
{
	// if there's already enough elements
	if ( GetSize() >= in_lNewSize )
    {
		m_lNbUsedElem = in_lNewSize;
		m_pVariant->numElems = m_lNbUsedElem * StructSize;

		return m_lNbUsedElem;
    }
	// we need to allocate some new elements
	else
	{
		// compute new size for the array
		SI_Long l_lNewArraySize = in_lNewSize * in_lExpandRatio;
		
		// allocate new array
		CElemType * l_pNewArray = (CElemType *) FTK_calloc( sizeof( CSubElemType), l_lNewArraySize * StructSize );
				
		if (l_pNewArray) 
		{
			// copy exisiting array data into new array
			CElemType *l_pCurrentArray = (CElemType*)m_pVariant->p_voidVal;
			register SI_Long i;

			for ( i = 0; i < GetUsed(); i++)
			{
				l_pNewArray[i] = l_pCurrentArray[i];
			}
			for ( i = GetUsed(); i< l_lNewArraySize; i++)
			{
				::new((void*)&l_pNewArray[i]) CElemType;
			}

			//Delete original array
			for ( i = 0; i < GetUsed(); i++)
			{
				l_pCurrentArray[i].~CElemType();
			}
			// flush the old array and replace it with the new one
			FTK_free((void *) l_pCurrentArray );

			m_pVariant->p_voidVal = l_pNewArray;

			// set the total number of raw elements in the variant
			m_pVariant->numElems = in_lNewSize * StructSize;

			// set allocated size to the size of the new array
			m_lNbAllocElem = l_lNewArraySize;
			
			return m_lNbUsedElem = in_lNewSize;
		}
	}

	// expand not required or memory allocation failed
	return m_lNbUsedElem;
}

template <class CElemType, class CSubElemType, SI_Int StructSize>
inline SI_Long CSLArrayProxy<CElemType, CSubElemType, StructSize>::Add
(
    CElemType in_Elem
)
{
	Extend(1);
	(*this)[m_lNbUsedElem - 1] = in_Elem;
	return m_lNbUsedElem;    
}

#if defined(_WIN32) || defined(_WIN32_WCE) || defined(_XBOX)
#pragma pack(pop)
#elif defined (CODEWARRIOR)
#pragma options align= reset
#endif

#endif // __SL_ARRAY_H__
