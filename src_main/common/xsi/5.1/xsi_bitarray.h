//*****************************************************************************
/*!
   \file xsi_bitarray.h
   \brief CBitArray class declaration.

   © Copyright 1998-2005 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIBITARRAY_H__
#define __XSIBITARRAY_H__

#include "sicppsdk.h"

namespace XSI {

class CBoolArray;
class CStatus;

//*****************************************************************************
/*! \class CBitArray xsi_bitarray.h
	\brief This class encapsulates an array of binary values represented as booleans, where true indicates
	a bit set to ON and false indicates a bit set to OFF.

	CBitArray is suitable for dealing with large arrays of boolean values in a compact way. Because CBitArray
	uses much less memory than CBoolArray would use to represent the same number of bits, it is strongly
	recommended to use it when a large set of flags are needed.

	\since 5.0

	\eg
	\code
		using namespace XSI;
		Application app;
		CBitArray myFlags(10);
		CBitArray myOtherFlags(20);

		myOtherFlags.SetAll(0, true);

		myFlags.Or( myOtherFlags );
		for (LONG i=0; i<myFlags.GetCount(); i++)
		{
			app.LogMessage( L"myFlags[" + CString(i) + L"] = " + CString(myFlags[i]) );
		}
	\endcode

 */
//*****************************************************************************

class SICPPSDKDECL CBitArray
{
public:
	/*! Default destructor. */
	~CBitArray();

	/*! Constructs a CBitArray object of a specified size. The values are initially set to false. If a size is
	not supplied, the object is created with no bit values, we must then use CBitArray::Resize to initialize the object.
	\param in_size Number of bit values in the new CBitArray.
	*/
	CBitArray( LONG in_size = 0 );

	/*! Constructs a CBitArray object that contains bit values copied from another CBitArray object.
	\param in_str Constant CBitArray reference object.
	*/
	CBitArray(const CBitArray& in_str);

	/*! Constructs a CBitArray object and initializes it with the values coming from a CBoolArray object.
	\param in_boolArray Constant CBoolArray reference object.
	*/
	CBitArray(const CBoolArray& in_boolArray);

	/*! Assignment operator. Copies the bit values of a specified CBitArray to this CBitArray object. This
	CBitArray is resized to fit the size of the specified CBitArray object.
	\param in_array CBitArray object that we want to assign.
	\return A reference to this CBitArray after it has been modified.
	*/
	CBitArray& operator=(const CBitArray& in_array) ;

	/*! Equality operator. Tests if one CBitArray has the same contents as another.
	\param in_array CBitArray with which we want to compare.
	\return True if all elements are the same; false if some or all elements are different.
	*/
	bool operator==(const CBitArray& in_array) const;

	/*! InEquality operator. Tests if two CBitArrays have different contents.
	\param in_array CBitArray with which we want to compare.
	\return True if all elements are different; false if some or all elements are the same.
	*/
	bool operator!=(const CBitArray& in_array) const;

	/*! Accessor to bit values at a given position. This function can only be called by constant objects, the
	returned value is read-only.
	\param in_pos Position in this zero-based array. The position must be smaller than the number of elements
		in the array, otherwise the results are unpredicted.
	\return A read-only bit value.
	*/
	const bool operator[]( LONG in_pos ) const;

	/*! Array element operator. Gets the bit value at a specified position.
	\param in_pos 0-based position in the array. The result is unpredicted if the position is invalid.
	\return A bit value.
	*/
	bool operator [] (LONG in_pos);

	/*! Adds a bit value at the end of this array.
	\param in_bVal New bit value to be added at the end of the array.
	\return CStatus::OK success
	*/
	CStatus Add( const bool& in_bVal );

	/*! Sets a bit value to true or false at the specified position.
	\param in_pos 0-based position in the array. The result is unpredicted if the position is invalid.
	\param in_bVal The boolean value to assign to the bit value.
	\return CStatus::OK success
	\return CStatus::InvalidArgument Position is out of bound
	*/
	CStatus Set(LONG in_pos, bool in_bVal);

	/*! Sets all bit values to true or false.
	\param in_bVal The boolean value to assign to the bit values.
	\return CStatus::OK success
	*/
	CStatus SetAll(bool in_bVal);

	/*! Reallocates memory for the array, preserves its contents if new size is larger than existing
	size. The new elements are set to false by default.
	\param in_size New size of the array.
	\param in_bVal If true the new bit values are set to true (ON) or are set to false (OFF) if the value is false.
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus Resize( LONG in_size, bool in_bVal = false );

	/*! Clears the memory and set the number of bit values to 0.
	\return CStatus::OK success
	*/
	CStatus Clear();

	/*! Returns the number of bit values in the array.
	\return The number of bit values.
	*/
	LONG GetCount() const ;

	/*! Returns the number of bit values in the array set to true.
	\return The number of bit values set to true.
	*/
	LONG GetTrueCount() const ;

	/*! Returns true if at least one bit value is set to true.
	\return True if at least one bit value is set to true.
	*/
	bool IsAny() const ;

	/*! Performs the bitwise AND operation on the bit values in the CBitArray against the corresponding elements
	in the specified CBitArray. Two bits combined by the AND operator return true if each bit is true; otherwise,
	their combination returns false. The two arrays don't need to be equal in size, the operation is performed up
	to the size of the smaller array.
	\param in_array CBitArray that is to be used to perform the bitwise AND operation.
	\return Reference to this CBitArray object.
	*/
	CBitArray& And( const CBitArray& in_array );

	/*! Performs the bitwise OR operation on the bit values in the CBitArray against the corresponding elements
	in the specified CBitArray. Two bits combined by the OR operator return true if at least one of the bits is true;
	if both bits are false, their combination returns false. The two arrays don't need to be equal in size, the
	operation is performed up to the size of the smaller array.
	\param in_array CBitArray that is to be used to perform the bitwise OR operation.
	\return Reference to this CBitArray object.
	*/
	CBitArray& Or( const CBitArray& in_array );

	/*! Performs the bitwise XOR operation on the bit values in the CBitArray against the corresponding elements in
	the specified CBitArray. Two bits combined by the XOR operator return true if at least one, but not both, of the
	bits is true; otherwise, their combination returns false. The two arrays don't need to be equal in size, the
	operation is performed up to the size of the smaller array.
	\param in_array CBitArray that is to be used to perform the bitwise OR operation.
	\return Reference to this CBitArray object.
	*/
	CBitArray& XOr( const CBitArray& in_array );

	/*! Inverts all bit values of this CBitArray object.
	\return Reference to this CBitArray object.
	*/
	CBitArray& FlipAll();

	/*! Inverts the value of a bit value at a specified position.
	\param in_pos 0-based position in the array. The result is unpredicted if the position is invalid.
	\return The new bit value.
	*/
	bool Flip(LONG in_pos);

	/*! Returns an iterator used by the fast bit-lookup functions. This function must be used for getting the right
	iterator, don't use the bit value position directly as the resulting lookup operations can be unpredictable.
	\param in_pos The position in the array where to start the fast lookup.
	\return A start iterator.
	\sa CBitArray::GetNextTrueBit, CBitArray::GetNextFalseBit
	*/
	LONG GetIterator( LONG in_pos = 0 ) const;

	/*! Allows fast bit-lookup operations. It starts at the current iterator position, and looks for the
	next bit set to true. If the lookup is successful, the bit value position in the array is returned.
	This position can then be used to access the bit value.

	\param in_Iterator The iterator where the lookup starts. The function sets the iterator with the position
		of the next bit set to true. If the end of the array is reached, the iterator is set with the total
		number of bit values in the array.
	\return True if the end of array is reached, false otherwise.
	\sa CBitArray::GetNextFalseBit, CBitArray::GetIterator

	\eg
	\code
		using namespace XSI;
		Application app;
		CBitArray bits(512);

		bits.Set( 0, true );
		bits.Set( 32, true );
		bits.Set( 44, true );
		bits.Set( 65, true );
		bits.Set( 312, true );

		LONG it = bits.GetIterator();
		while (bits.GetNextTrueBit(it))
		{
			app.LogMessage( L"bits[" + CString(it) + L"] = " + CString(bits[it]) );
		}

		// Expected results
		// INFO: bits[0] = true
		// INFO: bits[32] = true
		// INFO: bits[44] = true
		// INFO: bits[65] = true
		// INFO: bits[312] = true

	\endcode
	*/
	bool GetNextTrueBit( LONG& in_Iterator) const;

	/*! This function is similar to CBitArray::GetNextTrueBit, but looks up the bit values set to false instead.

	\param in_Iterator The iterator where the lookup starts. The function sets the iterator with the position
		of the next bit set to false. If the end of the array is reached, the iterator is set with the total
		number of bit values in the array.
	\return True if the end of array is reached, false otherwise.

	\sa CBitArray::GetNextTrueBit, CBitArray::GetIterator

	\eg
	\code
		using namespace XSI;
		Application app;
		CBitArray bits(512);

		bits.SetAll(true);
		bits.Set( 15, false );
		bits.Set( 31, false );
		bits.Set( 63, false );
		bits.Set( 127, false );
		bits.Set( 255, false );
		bits.Set( 511, false );

		LONG it = bits.GetIterator();
		while (bits.GetNextFalseBit(it))
		{
			app.LogMessage( L"bits[" + CString(it) + L"] = " + CString(bits[it]) );
		}

		// Expected results
		// INFO: bits[15] = false
		// INFO: bits[31] = false
		// INFO: bits[63] = false
		// INFO: bits[127] = false
		// INFO: bits[255] = false
		// INFO: bits[511] = false
	\endcode
	*/
	bool GetNextFalseBit( LONG& in_Iterator) const;

private:
	void * m_pImpl ;
};

};

#endif // __XSIBITARRAY_H__
