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

#ifndef __ENUMDICTIONNARY_H__
#define __ENUMDICTIONNARY_H__

#include <SIBCUtil.h>	// SI_Char, _SI_STRCMP

#include "StrMap.h"		// StrMap

//! Template class that converts string to enum, or enum to strings
/*! 
	\param Enum Enumerated type
	\param size Value of the last valid entry of the enumerated type

	\b Example:
	\code
		// the enum type
		enum MyEnum
		{
			SomeValue = 1,
			SomeOtherValue = 2,
			TheLastValue = 4
		};

		// the strings matching the enum values
		static const SI_Char* g_szMyEnumStrings[] =
		{
			"SomeValue",
			"SomeOtherValue",
			"TheLastValue"
		};

		// The Enum dictionary to convert from enum to string
		// or string to enum
		CEnumDictionary<MyEnum, TheLastValue> l_dict( g_szMyEnumStrings );
	\endcode
*/
template 
<
	class Enum,		
	size_t size
> 
class CEnumDictionary
{
public:
	//! Constructor
	/*!
		\param in_ppStringArray	Array containing the strings.
		\warning \p in_ppStringArray is not freed upon destruction.
	
		\b Example:
		\code
		// example of typical in_ppStringArray
		static const SI_Char* g_szMyEnumStrings[] =
		{
			"SomeValue",
			"SomeOtherValue",
			"TheLastValue"
		};
		\endcode
	*/
    inline CEnumDictionary( SI_Char const* * in_ppStringArray )
    { 
        m_ppArray = in_ppStringArray; 
    }

	~CEnumDictionary()
	{
	}

	//! Convert from enum to string
	/*!
		\param in_Value Enum value to convert
		\return String representation of the enum value
		\warning The returned string should \b not be modified

		\b Example:
		\code
		CEnumDictionary<EnumType, LastEnumValue> l_EnumDict(g_szMyEnumStrings);
		const SI_Char *l_szStringValue = l_EnumDict.ToString(LastEnumValue);
		\endcode
	*/
    inline const SI_Char* ToString( Enum in_Value ) const
    { 
		return (m_ppArray)[(size_t)in_Value]; 
    }

	//! Convert from string to enum
	/*!
		\param out_rEnum Resulting enum value
		\param in_pValue String value to convert
		\retval false There is no matching enum value
		\retval true The matching enum value was found

		\b Example:
		\code
		CEnumDictionary<EnumType, LastEnumValue> l_EnumDict(g_szMyEnumStrings);
		EnumType l_EnumValue;
		if (!l_EnumDict.ToEnum(l_EnumValue, "LastEnumValue"))
		{
			// conversion failed
		}
		\endcode
	*/
	inline SI_Bool ToEnum( Enum &out_rEnum, const SI_Char* in_pValue ) const
	{
		for (int v=0;v<=size;v++)
		{
			if ( !_SI_STRCMP( m_ppArray[v], in_pValue ) )
			{
				out_rEnum = (Enum)v;
				return SI_TRUE;
			}
		}
		return SI_FALSE;
	}
private:
	SI_Char const** m_ppArray;
};

#endif //__ENUMDICTIONNARY_H__
