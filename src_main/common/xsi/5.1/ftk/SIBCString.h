//***************************************************************************************
//
// File supervisor: Softimage 3D Games & 3D Bridge team
//
// (c) Copyright 1999-2002 Avid Technology, Inc. . All rights reserved.
//
// SIBCString.h | Main header file for SIBCString implementation
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
#ifndef __CSIBCString_H__
#define __CSIBCString_H__

//***************************************************************************************
// Includes
//***************************************************************************************
#include <SIBCUtil.h>


//***************************************************************************************
// CSIBCString | General purpose string class
//***************************************************************************************

//! General purpose string class
/*! This class is used throughout the FTK as a standard method in dealing with strings.
	The strings are NULL-terminated strings. It supports many methods that perform useful 
	string operations.
*/
class XSICOREEXPORT CSIBCString
{
public:

	/*! Default Constructor.
		Constructs an empty string (length zero).
	*/
	CSIBCString();

	/*! Copy Constructor.
		Constructs a string idential to \p in_pString
		\param	in_pString	String to duplicate. Must not be of zero length.
	*/
	CSIBCString( const CSIBCString &in_pString );


	/*! Constructor
		Constructs a string object containing the NULL-terminated character string
		pointer to by \p in_pString.
		\param in_pString Pointer to the string to use for the new string. Cannot be NULL.
	*/
	CSIBCString(const SI_Char *in_pString );			// Constructor

	~CSIBCString();										// Default Destructor

	/*! Returns the length of the string.
		\return SI_Int	The length of the string.
	*/
	SI_Int GetLength();

	/*! Returns a character buffer with the contents of the string. This buffer should
		not be modified, and does not need to be freed after use. This function is generally
		used to output the text contained within the string.
		\return SI_Char	Pointer to a character buffer containing the string text.

		\b Example:
		\code
		SIBCString * t_pMyString = new SIBCString("FTK");

		// This is OK.
		printf("I think that %s is the best!\n", t_pMyString->GetText());

		// This is wrong.
		printf("I think that %s is the best!\n", t_pMyString);
		\endcode
	*/
	SI_Char *GetText();

	/*! Deletes the internal character buffer of the string, and sets the length to zero.
	*/
	void Clear();

	/*! Concatenate \p in_pString to the end of this string.
		\param in_pString Pointer to the string to concatenate.
		\return SI_Error Error value indicating the success or failure of this command.

		<ul>Possible returns:
			<li>SI_SUCCESS - The string was concatenated properly.
		</ul>
	*/
	SI_Error Concat(CSIBCString *in_pString);

	/*! Concatenate \p in_pString to the end of this string.
		\param in_pString	Pointer to a character buffer to concatenate.

		\return SI_Error Error value indicating the success or failure of this command.

		<ul>Possible returns:
			<li>SI_SUCCESS - The string was concatenated properly.
		</ul>
	*/
	SI_Error Concat(const SI_Char *in_pString);			// Concatenate to the string

	/*! Concatenate \p in_lInt to the end of this string, in string format.
		\param in_lInt Integer to concatenate to the end of this string.

		\return SI_Error Error value indicating the success or failure of this command.

		<ul>Possible returns:
			<li>SI_SUCCESS - The string was concatenated properly.
		</ul>

		\b Example:
		\code
		SIBCString * t_pMyString = SIBCString("FTK v");
		t_pMyString->Concat(3);
		t_pMyString->Concat(".");
		t_pMyString->Concat(6);
		printf("This is %s\n", t_pMyString.GetText());

		// output is "This is FTK v3.6"
		\endcode
	*/		
	SI_Error Concat(const SI_Int in_lInt);

	/*! Concatenate \p in_fFloat to the end of this string, in string format.
		\param in_fFloat Floating-point number to concatenate to the end of this string.

		\return SI_Error Error value indicating the success or failure of this command.

		<ul>Possible returns:
			<li>SI_SUCCESS - The string was concatenated properly.
		</ul>

		\b Example:
		\code
		SIBCString * t_pMyString = SIBCString("FTK v");
		t_pMyString->Concat(3.6f);
		printf("This is %s\n", t_pMyString.GetText());

		// output is "This is FTK v3.6"
		\endcode
	*/		
	SI_Error Concat(const SI_Float in_fFloat);			// Concatenate to the string

	/*! Assigns the value contained in the string \p in_pString to this string.
		\param in_pString	String containing data to assign to this string.
		\return SI_Error Error value indicating the success or failure of this command.

		<ul>Possible returns:
			<li>SI_SUCCESS - The string was assigned properly.
		</ul>
	*/
	SI_Error SetText(CSIBCString *in_pString);			// Assign to the string

	/*! Assigns the NULL-terminated string contained in the character buffer \p in_pString 
		to this string.
		\param in_pString	Pointer to the character buffer containing the NULL-terminated string to assign.
		\return SI_Error Error value indicating the success or failure of this command.

		<ul>Possible returns:
			<li>SI_SUCCESS - The string was assigned properly.
		</ul>
	*/
	SI_Error SetText(const SI_Char *in_pString);		// Assign to the string

	/*! Assigns the string representation of \p in_lInt to this string.
		\param in_lInt	Integer whose string representation should be assigned to this string 
		\return SI_Error Error value indicating the success or failure of this command.

		<ul>Possible returns:
			<li>SI_SUCCESS - The string was assigned properly.
		</ul>
	*/
	SI_Error SetText(const SI_Int in_lInt);				// Assign to the string

	/*! Assigns the string representation of \p in_fFloat to this string.
		\param in_fFloat	Floating-point number whose string representation should be assigned to this string 
		\return SI_Error Error value indicating the success or failure of this command.

		<ul>Possible returns:
			<li>SI_SUCCESS - The string was assigned properly.
		</ul>
	*/
	SI_Error SetText(const SI_Float in_fFloat);			// Assign to the string

	/*! Returns a value indicated the differences between this string and \p in_pString.
		\param in_pString	The string to compare this string with.
		\return	SI_Int	Integer value representing the lexicographic difference between this
				string and \p in_pString. Negative indicates this string is less than \p in_pString,
				Positive indicates this string is greater than \p in_pString, and zero indicates the
				strings are the same. 
				
				There are three special cases: 
				<ul>
					<li>\p in_pString and this string's	buffer are both NULL, function returns 0
					<li>\p in_pString is NULL, and this string's buffer is not NULL, function returns 1.
					<li>\p in_pString is not NULL, and this strings's buffer is NULL, function returns -1.
				</ul>
	*/
	SI_Int Compare(CSIBCString *in_pString);

	/*! Returns a value indicated the differences between this string and \p in_pString.
		\param in_pString	The character buffer containing the NULL-terminated string to compare this string with.
		\return	SI_Int	Integer value representing the lexicographic difference between this
				string and \p in_pString. Negative indicates this string is less than \p in_pString,
				Positive indicates this string is greater than \p in_pString, and zero indicates the
				strings are the same. 
				
				There are three special cases: 
				<ul>
					<li>\p in_pString and this string's	buffer are both NULL, function returns 0
					<li>\p in_pString is NULL, and this string's buffer is not NULL, function returns 1.
					<li>\p in_pString is not NULL, and this strings's buffer is NULL, function returns -1.
				</ul>
	*/
	SI_Int Compare(const SI_Char *in_pString);			// Compare with the string

	/*! Returns a value indicated the differences between this string and the string representation of \p in_lInt.
		\param in_lInt The integer value whose string representation is compared with this string.
		\return	SI_Int	Integer value representing the lexicographic difference between this
				string and the string representation of \p in_lInt. Negative indicates this string is less 
				than that string, positive indicates this string is greater than that string, and zero 
				indicates the strings are the same. 
	*/
	SI_Int Compare(const SI_Int in_lInt);				// Compare with the string

	/*! Returns a value indicated the differences between this string and the string representation of \p in_fFloat.
		\param in_lInt The floating-pointer number whose string representation is compared with this string.
		\return	SI_Int	Integer value representing the lexicographic difference between this
				string and the string representation of \p in_fFloat. Negative indicates this string is less 
				than that string, positive indicates this string is greater than that string, and zero 
				indicates the strings are the same. 
	*/
	SI_Int Compare(const SI_Float in_fFloat);			// Compare with the string

	/*! Converts all alphabetic characters contained within this string to lower-case characters.
		\return SI_Error Error value indicating the success or failure of this command.

		<ul>Possible returns:
			<li>SI_SUCCESS - The string was assigned properly.
		</ul>
	*/
	SI_Error LowerCase();

	/*! Converts all alphabetic characters contained within this string to upper-case characters.
		\return SI_Error Error value indicating the success or failure of this command.

		<ul>Possible returns:
			<li>SI_SUCCESS - The string was assigned properly.
		</ul>
	*/
	SI_Error UpperCase();								// Turns all characters in string to upper case

	/*! Assignment operator. Copies the value of \p i_pString into this string.
		\param	i_pString		String whose value is copied into this string.
		\return	CSIBCString&	Reference to this string.
	*/
	CSIBCString &operator = ( const CSIBCString & i_pString);

	/*! Assignment operator. Copies the value of \p i_pString into this string.
		\param	i_pString	Pointer to the character buffer containing a NULL-terminated string 
				whose value is copied into this string.
		\return	CSIBCString&	Reference to this string.
	*/
	CSIBCString &operator = ( const SI_Char * i_pString);

	/*! Equality operator. Determines whether \p i_pString and this string contain the same text.
		\param i_pString	Reference to the string to compare to.
		\return bool		true if strings contain the same text, false otherwise.
	*/
	bool operator == (CSIBCString & i_pString);

	/*! Equality operator. Determines whether \p i_pString and this string contain the same text.
		\param i_pString	Pointer to the character buffer containing the NULL-terminated string to compare to.
		\return bool		true if strings contain the same text, false otherwise.
	*/
	bool operator == (const SI_Char * i_pString);

	/*! Inequality operator. Determines whether \p i_pString and this string contain different text.
		\param i_pString	Reference to the string to compare to.
		\return bool		true if string contain different text, false other.
	*/
	bool operator != (CSIBCString & i_pString);					// Tests to see if strings are not equal

	/*!	Output debugging information for this string. This includes the address of the string,
		the length and contents of the string buffer.
	*/
	void Dump();

	/*! Returns the amount of memory used by the string. This does not include the size of the
		SIBCString class, only the data contained in its character buffer. In SIBCString (but not 
		necessarily in other classes) SIBCString::UsedMemory is equivalent to SIBCString::AllocatedMemory.
		\return	SI_UInt	Amount of memory used by this string (in bytes).
	*/
	SI_UInt UsedMemory();								// returns the amount of used memory

	/*! Returns the amount of memory allocated by the string. This does not include the size of the
		SIBCString class, only the data contained in its character buffer. In SIBCString (but not 
		necessarily in other classes) SIBCString::AllocatedMemory is equivalent to SIBCString::UsedMemory.
		\return	SI_UInt	Amount of memory used by this string (in bytes).
	*/
	SI_UInt AllocatedMemory();							// returns the amount of allocated memory.

protected:


private:

	SI_Int		m_lLength;								// Length of the string
	SI_Char		*m_pBuffer;								// Internal SI_Char buffer
};

//***************************************************************************************
// CSIAccumString | Optimized string class.
//***************************************************************************************

//! This string class is optimized to accumulate bytes at the end efficiently.
/*! When used below to read unsupported templates, it makes a BIG difference in performance 
	since we simply dont recalculate the string length every time. CSIBCString will do up to 
	three runs through the whole buffer when resizing, which gets extremely time consuming on long runs.
*/
class XSICOREEXPORT CSIAccumString
{
public:

	/*! Default constructor.
		Allocates an initial block of memory for string storage, and sets the
		string data empty.
	*/
	CSIAccumString() 
	{
		m_AllocSize = k_InitialBufSize;
		m_Buffer = (SI_Char*) malloc(m_AllocSize);
		m_BufferEnd = m_Buffer;
		*m_BufferEnd = 0;
	}

	~CSIAccumString()
	{
		free( m_Buffer );
	}

	/*! Deletes the internal character buffer of the string, and resets the allocated
		size to a default initial buffer size.
	*/
	void Clear()
	{
		free( m_Buffer );
		m_AllocSize = k_InitialBufSize;
		m_Buffer = (SI_Char*) malloc(m_AllocSize);
		m_BufferEnd = m_Buffer;
		*m_BufferEnd = 0;
	}

	/*! Contenates an SI_Byte to the current string.
		\param in_Byte	SI_Byte to concatenate to the current string.
	*/
	void ConcatByte( SI_Byte in_Byte )
	{
		SI_Int l_iCurrDelta = (SI_Int)(m_BufferEnd - m_Buffer);
		if ( l_iCurrDelta > ( m_AllocSize - 2 ) )
		{
			// Array doubling... Good enough.
			m_AllocSize <<= 1;
			m_Buffer = (SI_Char*) realloc(m_Buffer,m_AllocSize);
			m_BufferEnd = m_Buffer + l_iCurrDelta;
		}
		*m_BufferEnd = in_Byte;
		++m_BufferEnd;
		*m_BufferEnd = 0;
	}

	/*! Returns the length of the string.
		\return SI_Int	The length of the string.
	*/
	SI_Int GetLength() const { return (SI_Int)(m_BufferEnd - m_Buffer); }	

	/*! Returns a character buffer with the contents of the string. This buffer should
		not be modified, and does not need to be freed after use. This function is generally
		used to output the text contained within the string. (See SIBCString::GetText).
		\return SI_Char	Pointer to a character buffer containing the string text.
	*/
	SI_Char *GetText() const { return m_Buffer; }

private:

	// No copy semantics needed. Play safe.
	CSIAccumString(const CSIAccumString&);
	const CSIAccumString& operator=( const CSIAccumString &);

	SI_Char*		m_Buffer;
	SI_Char*		m_BufferEnd;
	SI_Int			m_AllocSize;
	static const SI_Int k_InitialBufSize;
};

#endif
// CSIBCString
