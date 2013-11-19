//*****************************************************************************
/*!
   \file xsi_string.h
   \brief CString and CStringArray class declarations.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSISTRING_H__
#define __XSISTRING_H__

#include "sicppsdk.h"
#include <stdlib.h>

namespace XSI {

class CStatus;
class CValue;
class CStringArray;

//*****************************************************************************
/*! \class CString xsi_string.h
	\brief Character strings in the %XSI SDK are represented with the CString class. CString consists of a
	variable-length sequence of wide characters, which are 16-bit values rather than the 8-bit values common
	with "char" strings.

	Using wide characters allows unicode strings to be represented in the SDK. Therefore you must use the
	\c L character before a character or a string constant to designate the wide character type constant.

	\sa CStringArray

	\eg Using the \c L character to designate the wide character type constant
	\code
		using namespace XSI;
		CString myString(L"Mystring");
	\endcode
 */
//*****************************************************************************

class SICPPSDKDECL CString
{
public:
	/*! Default constructor. */
	CString();

	/*! Default destructor. */
	virtual ~CString();

	/*! Constructs a CString object from another CString object. The contents of the string are duplicated.
	\param in_str constant CString reference object.
	*/
	CString(const CString& in_str);

	/*! Constructs a CString object from a null-terminated wide character string. The contents of the input
	string are duplicated inside the CString.
	\param in_str constant wchar_t* reference pointer.
	*/
	CString(const wchar_t * in_str );

	/*! Constructs a CString object from a CValue object. This constructor allows you to initialize a CString
	object with the string representation of a CValue object.
	\param in_val constant reference to a CValue object.
	\sa CValue::GetAsText

	\eg
	\code
		using namespace XSI;

		// Old way of converting a LONG to a CString object
		CString str = CValue((LONG)myLong).GetAsText();

		// New way
		CString str2 = CValue((LONG)10);

	\endcode
	*/
	explicit CString( const CValue& in_val );

	/*! Accessor to a null-terminated, wide character representation of the string. This buffer cannot be modified.
	\return The wide char representation for this string.

	\eg
	\code
		using namespace XSI;
		CString q( L"Data" ) ;

		// Get read-only access to the internal string buffer
		const wchar_t* l_pBuffer = q.GetWideString() ;

		// We can now call normal C runtime library string functions
		if ( 0==wcscmp( l_pBuffer, L"Data" ) )
		{
			// As expected
		}

	\endcode
	*/
	const wchar_t * GetWideString() const ;

	/*! Accessor to a null-terminated, ascii character representation of the string.	This buffer is temporary and
	cannot be modified. It should not be accessed after the content of the original CString object changes or is destroyed.

	\return The ascii char representation for this string.
	\since 5.0

	\eg
	\code
		using namespace XSI;
		CString q( L"Data" ) ;

		// Get read-only access to the internal string buffer
		const char* l_pBuffer = q.GetAsciiString() ;

		// We can now call normal C runtime library string functions
		if ( 0==strcmp( l_pBuffer, "Data" ) )
		{
			// As expected
		}

	\endcode
	*/
	const char* GetAsciiString() const ;

	/*! Sets the CString object with a null-terminated ascii character string. The contents of the specified string is
	duplicated inside the CString.

	\param in_pszStr constant char* reference pointer. The string is set to an empty string if the input string is
		empty or NULL.

	\return CStatus::OK success
	\return CStatus::False String set to empty.

	\sa CString::GetAsciiString
	\since 5.0

	\eg
	\code
		using namespace XSI;
		CString q;

		q.PutAsciiString( "Data" ) ;

		// Get read-only access to the internal string buffer
		const char* l_pBuffer = q.GetAsciiString() ;

		// We can now call normal C runtime library string functions
		if ( 0==strcmp( l_pBuffer, "Data" ) )
		{
			// As expected
		}

	\endcode
	*/
	CStatus PutAsciiString(const char* in_pszStr);

	/*! Returns the number of characters in the string.  Does not include the null-termination character.  This is not
	the number of bytes required to hold the string because each characters is 2 bytes long.
	\return The number of characters in the string.
	*/
	ULONG Length() const ;

	/*! Tests whether the string is empty.
	\return true if the string is empty; false otherwise.
	*/
	bool IsEmpty() const ;

	/*! Clears the string's contents.
	*/
	void Clear() ;

	/*! Assignment operator.  The contents of the string are duplicated.
	\param in_rhs CString object that we want to assign.
	\return A reference to the newly created object.
	*/
	CString& operator=(const CString& in_rhs) ;

	/*! Assignment operator  The contents of the string are duplicated.
	\param in_rhs wide character string that we want to assign.
	\return A reference to the newly created object.
	*/
	CString& operator=(const wchar_t * in_rhs) ;

	/*! Equality operator.  Tests whether one string has the same contents as another.
	\note Comparison is case-sensitive.
	\param in_refStr CString with which we want to compare.
	\return true if all characters are the same; false if some or all of the characters are different.
	*/
	bool operator==(const CString& in_refStr) const;

	/*! Equality operator.  Tests whether the CString has the same contents as a wide character string.
	\param in_pwstr Pointer to a string with which we want to compare.
	\return true if all characters are the same; false if some or all characters are different.
	*/
	bool operator==(const wchar_t* in_pwstr) const;

	/*! Case-insensitive comparison.
	\param in_rhs CString with which we want to compare.
	\return true if the strings are the same; false otherwise.

	\eg
	\code
		using namespace XSI;
		CString A( L"A" ) ;
		CString a( L"a" ) ;

		A == a ;					// False
		A.IsEqualNoCase( a ) ;		// True
		A.IsEqualNoCase( L" a" ) ;	// False
	\endcode

	*/
	bool IsEqualNoCase( const CString& in_rhs ) const ;

	/*! Case insensitive comparison.  Tests whether one string has the same contents as another without regard to the case.
	\param in_pwstr Wide character string we want to compare.
	\return true if the strings are the same; false otherwise.
	*/
	bool IsEqualNoCase( const wchar_t* in_pwstr ) const ;

	/*! Parses the string and returns the sub-strings delimited by a given string. If the delimiter is empty the space
	character is used. If the string is empty, the function returns an empty array.
	\param in_strDel String delimiter.
	\return CStringArray Array constaining the sub-strings.
	\since Gemini
	*/
	CStringArray Split( const CString& in_strDel ) const;

	/*! Inequality operator.  Tests whether two CStrings have different contents.
	\note Comparison is case-sensitive.
	\param in_refStr Pointer to a string with which we want to compare.
	\return true if some or all characters are different; false if all characters are the same.
	*/
	bool operator!=(const CString& in_refStr) const;

	/*! Inequality operator.  Tests whether a CString has different contents than a wide character string.
	\note Comparison is case-sensitive.
	\param in_pwstr Pointer to a string with which we want to compare.
	\return true if some or all characters are different; false if all characters are the same.
	*/
	bool operator!=(const wchar_t* in_pwstr) const;

	/*! Concatenation operator.
	\param in_refStr Pointer to a string that we want to add to the end.
	\return A reference to this string after it has been modified.

	\eg
	\code
		CString a( L"A" ) ;
		CString b( L"b" ) ;

		a += b ;
		// String now contains L"Ab"
	\endcode
	*/
	CString& operator+=(const CString& in_refStr);

	/*! Concatenation operator.
	\param in_pwstr Wide character string that we want to append.
	\return A reference to this string after it has been modified.
	*/
	CString& operator+=(const wchar_t* in_pwstr);

	/*! Concatenation operator.
	\param in_wchar concatenate a single character at the end.
	\return A reference to this string after it has been modified.
	*/
	CString& operator+=(const wchar_t in_wchar);

	/*! Addition operator.
	\param in_refStr string to add together with this string
	\return A new string that contains the concatenation of this string and in_refStr.
	*/
	CString operator+(const CString& in_refStr) const;

	/*! Addition operator.
	\param in_pwstr wide character string to add together with this string
	\return A new string that contains the concatenation of this string and in_pwstr.
	*/
	CString operator+(const wchar_t* in_pwstr) const;

	/*! Addition operator.
	\param in_wchar a single wide character to add together with this string
	\return A new string that contains the concatenation of this string and in_wchar.
	*/
	CString operator+(const wchar_t in_wchar) const;

	/*! Array element operator. Returns a single wide character contained in the string at a given position.
	\param in_index 0-based position in the array.
	\return A wide character.
	\return 0xffff if the position is invalid.
	*/
	wchar_t operator [] (ULONG in_index) const;

private:

	//*************************************************************************
	/*! \struct StringData xsi_string.h
		\brief Private class used for holding the CString data. Not available 
		outside this class.
	*/
	//*************************************************************************
	struct StringData
	{
		void * m_pImpl ;

		//! CString content.
		/*!
			This data member points to the content of CString and is typically 
			used to inspect a CString instance from the debugger.
		*/
		wchar_t* m_pDebugStr; 
	};
	/*! CString data.*/
	StringData* m_pData;
};

/*! Addition operator
\return A new string that contains the concatenation of the two arguments.
*/
SICPPSDKDECL CString operator+( const wchar_t* in_pwstr, const CString& in_refStr ) ;

/*! Addition operator
\return A new string that contains the concatenation of the two arguments.
*/
SICPPSDKDECL CString operator+( const wchar_t in_wchar, const CString& in_refStr ) ;


//*************************************************************************
/*! \class CStringArray xsi_string.h
	\brief The CStringArray is an uni-dimensional array of CString objects.

	The array is zero-based, which means that the first item index is 0 and the largest index is one less than
	the number of elements contained in the array.

	\sa CString, CRefArray::Filter, X3DObject::FindChild, X3DObject::FindChildren
*/
//*************************************************************************
class SICPPSDKDECL CStringArray
{
public:
	/*! Constructs a CStringArray and optionally initializes the array to a known size.
	\param in_size Initial size of array, defaults to 0.  If you know the exact number of items
		you want to save in the array, it can be more efficient to specify the value using this
		argument, rather than calling CStringArray::Add() many times.
	*/
	CStringArray( LONG in_size=0);

	/*! Constructs a CStringArray object from another CStringArray object.
	\param in_array read-only CStringArray object.
	*/
	CStringArray(const CStringArray& in_array );

	/*! Default destructor. */
	~CStringArray();

	/*! Assignment operator. Current contents of this array are replaced by a copy of in_array's contents.
	\param in_array read-only class object.
	\return A reference to this array.
	 */
	CStringArray& operator=( const CStringArray& in_array );

	/*! Returns the number of items in this CStringArray
	\return The number of items in the array.
	*/
	LONG GetCount() const;

	/*! Adds a CString at the end of this array's.  This increases the size of the Array. The CString is copied.
	\param in_item New item to be added at the end of the array.
	\return CStatus::OK success
	*/
	CStatus Add( const CString& in_item );

	/*! Erases all elements contained in the array.
	\return CStatus::OK success
	*/
	CStatus Clear();

	/*! Reallocates memory for the array.  Existing contents are preserved, except for items at the end of the
	array if the new size is smaller than the existing size.
	\param in_size New size of the array.
	\return CStatus::OK success
	\return CStatus::InvalidArgument in_size < 0
	*/
	CStatus Resize( LONG in_size);

	/*! Accessor to elements at a given index. The returned value is read-only.
	\param in_index index in this zero-based array.  The index must be greater or equal to zero
		and smaller than the number of elements in the array, otherwise the results are unpredictable.
	\return A read-only reference to the indexed item.
	*/
	const CString& operator[]( LONG in_index ) const;

	/*! Accessor to elements at a given index.
	\param in_index index in this zero-based array.  The index must be greater or equal to zero
		and smaller than the number of elements in the array, otherwise the results are unpredictable.
	\return A reference to the indexed item.
	*/
	CString& operator[]( LONG in_index );

	/*! Equality operator.
	\param in_array CStringArray to compare with.
	\return true if in_array contains the same number of elements as this array and if all members
		of in_array are equal to the corresponding one contained in this array.
	*/
	bool operator==(const CStringArray& in_array) const;

	/*! Inequality operator.
	\param in_array
	\return true if any member of in_array is different from the corresponding members in this
		array or if the arrays are not of the same size.
	*/
	bool operator!=(const CStringArray& in_array) const;

private:
	void* m_ptr;
};

};

#endif // __XSISTRING_H__
