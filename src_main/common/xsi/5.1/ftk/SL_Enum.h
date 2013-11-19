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

#ifndef __SL_ENUM_H__
#define __SL_ENUM_H__

#if defined(_WIN32) || defined (_WIN32_WCE) || defined(_XBOX)
// Disable "identifier was truncated to '255' characters in the debug information" warning.
#pragma warning( disable : 4786 )
#endif // defined(_WIN32) || defined (_WIN32_WCE)

#include "SL_Int.h"			// CSLIntProxy
#include "SL_String.h"		// CSLStringProxy
#include "EnumDictionary.h"	// CEnumDictionary

//! Class that allows modification of an enum implemented as an integer in a template
template <class EnumType, SI_Int MaxValue>
class XSIEXPORT CSLEnumProxy
	: public CSLAnimatableType
{
public:
	/*! Constructor. Connects to an enum parameter implemented as an integer in the template
		\param in_pTemplate Pointer to the template
		\param in_Index Index of the parameter in the template
	 */
	CSLEnumProxy(CdotXSITemplate *in_pTemplate, SI_Int in_nIndex );

	/*! Assignment operator. Assigns the value of \p in_Value to this class
		\param	in_Value	The new value
		\return Reference to the current object.
	*/
    CSLEnumProxy& operator =(const CSLEnumProxy &in_Value);

	/*! Assignment operator. Assigns \p in_Value to the parameter referred by this proxy.
		\param	in_Value	The new value
		\return Reference to the current object.
	*/
    CSLEnumProxy& operator =(const EnumType &in_Value);

	/*! Comparison operator. Compares \p in_ToCompare with the value of the parameter referred by this proxy.
		\param	in_ToCompare	The instance to compare values with
		\return	Whether or not the referred parameter and \p in_ToCompare are equal
	*/
    SI_Bool operator ==(const CSLEnumProxy &in_ToCompare);

	/*! Comparison operator. Compares \p in_ToCompare with the value of the parameter referred by this proxy.
		\param	in_ToCompare	The instance to compare values with
		\return	Whether or not the referred parameter and \p in_ToCompare are equal
	*/
    SI_Bool operator ==(const EnumType &in_ToCompare);

	//! Checks if the internally stored enum value is valid
	/*!
		\retval True if the stored enum is within valid range
		\retval False if the stored enum is invalid
	*/
	SI_Bool IsValid();

	//! Conversion operator
	/*!
		\return The stored enum value
	*/
    operator EnumType();

	/*! Returns the animatable type of this class
		\return CSLAnimatableType::SI_ENUM_TYPE
	*/
	virtual EElementType Type();

	virtual SI_Float GetFloatValue();
	virtual SI_Void SetFloatValue(SI_Float in_fValue);

private:
	CSLEnumProxy( CSLEnumProxy &in_pBasicType ){}
    CSLIntProxy m_Value;
};

//! Class that allows modification of an enum implemented as a string in a template
template <class EnumType, SI_Int MaxValue>
class CSLStrEnumProxy
	: public CSLAnimatableType
{
public:
	/*! Constructor. Connects to an enum parameter implemented as a string in the template
		\param in_pTemplate Pointer to the template
		\param in_Index Index of the parameter in the template
	 */
	CSLStrEnumProxy(CdotXSITemplate *in_pTemplate, SI_Int in_nIndex, CEnumDictionary<EnumType, MaxValue> *in_pDictionary);

	/*! Assignment operator. Assigns the value of \p in_Value to this class
		\param	in_Value	The new value
		\return Reference to the current object.
	*/
    CSLStrEnumProxy& operator =(const CSLStrEnumProxy &in_Value);

	/*! Assignment operator. Assigns the value of \p in_Value to this class
		\param	in_Value	The new value
		\return Reference to the current object.
	*/
    CSLStrEnumProxy& operator =(const EnumType &in_Value);

		/*! Comparison operator. Compares \p in_ToCompare with the value of the parameter referred by this proxy.
		\param	in_ToCompare	The instance to compare values with
		\return	Whether or not the referred parameter and \p in_ToCompare are equal
	*/

    SI_Bool operator ==(const CSLStrEnumProxy &in_ToCompare);

	/*! Comparison operator. Compares \p in_ToCompare with the value of the parameter referred by this proxy.
		\param	in_ToCompare	The instance to compare values with
		\return	Whether or not the referred parameter and \p in_ToCompare are equal
	*/
    SI_Bool operator ==(const EnumType &in_ToCompare);

	//! Checks if the internally stored enum value is valid
	/*!
		\retval True if the stored enum is within valid range
		\retval False if the stored enum is invalid
	*/
	SI_Bool IsValid();

	//! Conversion operator
	/*!
		\return The stored enum value
	*/
    operator EnumType();

	/*! Returns the animatable type of this class
		\return CSLAnimatableType::SI_STRING_ENUM_TYPE
	*/
	virtual EElementType Type();
	
	virtual SI_Float GetFloatValue();
	virtual SI_Void SetFloatValue(SI_Float in_fValue);

private:
	CSLStrEnumProxy( CSLStrEnumProxy &in_pBasicType ){}

	CEnumDictionary<EnumType, MaxValue> *m_pDictionary;
	EnumType m_EnumValue;
    CSLStringProxy m_StringValue;
};


///////////////////////////////////////////////////////////////////////////////
// template implementation
///////////////////////////////////////////////////////////////////////////////

template <class EnumType, SI_Int MaxValue>
CSLEnumProxy<EnumType,MaxValue>::CSLEnumProxy
(
	CdotXSITemplate *in_pTemplate, 
	SI_Int in_nIndex 
) : m_Value(in_pTemplate, in_nIndex)
{
}

template <class EnumType, SI_Int MaxValue>
inline CSLEnumProxy<EnumType,MaxValue>& CSLEnumProxy<EnumType,MaxValue>::operator =
(
	const CSLEnumProxy &in_Value
)
{
	m_Value = in_Value.m_Value;
	return *this;
}

template <class EnumType, SI_Int MaxValue>
inline CSLEnumProxy<EnumType,MaxValue>& CSLEnumProxy<EnumType,MaxValue>::operator =
(
	const EnumType &in_Value
)
{
	m_Value = in_Value;
	return *this;
}

template <class EnumType, SI_Int MaxValue>
inline SI_Bool CSLEnumProxy<EnumType,MaxValue>::operator ==
(
	const CSLEnumProxy<EnumType,MaxValue> &in_ToCompare
)
{
	return m_Value == in_ToCompare.m_Value;
}

template <class EnumType, SI_Int MaxValue>
inline SI_Bool CSLEnumProxy<EnumType,MaxValue>::operator ==
(
	const EnumType &in_nToCompare
)
{
	return m_Value == in_nToCompare;
}

template <class EnumType, SI_Int MaxValue>
inline CSLEnumProxy<EnumType,MaxValue>::operator EnumType()
{
	return (EnumType)((SI_Int)m_Value);
}

template <class EnumType, SI_Int MaxValue>
inline SI_Bool CSLEnumProxy<EnumType,MaxValue>::IsValid
(
)
{
	return m_Value <= MaxValue;
}

template <class EnumType, SI_Int MaxValue>
CSLAnimatableType::EElementType CSLEnumProxy<EnumType,MaxValue>::Type
(
)
{
	return SI_ENUM_TYPE;
}

template <class EnumType, SI_Int MaxValue>
SI_Float CSLEnumProxy<EnumType,MaxValue>::GetFloatValue
(
)
{
	return (SI_Float) ( (SI_Int) m_Value);
}

template <class EnumType, SI_Int MaxValue>
SI_Void CSLEnumProxy<EnumType,MaxValue>::SetFloatValue
(
	SI_Float	in_fValue
)
{
	if(in_fValue <= MaxValue)
	{
		m_Value = (SI_Int) in_fValue;
	}
}

///////////////////////////////////////////////////////////////////////////////

template <class EnumType, SI_Int MaxValue>
CSLStrEnumProxy<EnumType,MaxValue>::CSLStrEnumProxy
(
	CdotXSITemplate *in_pTemplate, 
	SI_Int in_nIndex, 
	CEnumDictionary<EnumType, MaxValue> *in_pDictionary
) : m_StringValue(in_pTemplate, in_nIndex)
  , m_pDictionary(in_pDictionary)
{
	// if the string is invalid
	if (!m_pDictionary->ToEnum( m_EnumValue, m_StringValue ))
	{
		// set the enum to an invalid value,
		m_EnumValue = (EnumType)(MaxValue + 1);
	}
}

template <class EnumType, SI_Int MaxValue>
inline CSLStrEnumProxy<EnumType,MaxValue>& CSLStrEnumProxy<EnumType,MaxValue>::operator =
(
	const CSLStrEnumProxy &in_Value
)
{
	m_EnumValue = in_Value.m_EnumValue;
	m_StringValue = m_pDictionary->ToString( m_EnumValue );
	
	return *this;
}

template <class EnumType, SI_Int MaxValue>
inline CSLStrEnumProxy<EnumType,MaxValue>& CSLStrEnumProxy<EnumType,MaxValue>::operator =
(
	const EnumType &in_Value
)
{
	m_EnumValue = in_Value;
	m_StringValue = m_pDictionary->ToString( m_EnumValue );
	
	return *this;
}

template <class EnumType, SI_Int MaxValue>
inline SI_Bool CSLStrEnumProxy<EnumType,MaxValue>::operator ==
(
	const CSLStrEnumProxy<EnumType,MaxValue> &in_ToCompare
)
{
	return m_EnumValue == in_ToCompare.m_EnumValue;
}

template <class EnumType, SI_Int MaxValue>
inline SI_Bool CSLStrEnumProxy<EnumType,MaxValue>::operator ==
(
	const EnumType &in_nToCompare
)
{
	return m_EnumValue == in_nToCompare;
}

template <class EnumType, SI_Int MaxValue>
inline CSLStrEnumProxy<EnumType,MaxValue>::operator EnumType()
{
	return m_EnumValue;
}

template <class EnumType, SI_Int MaxValue>
inline SI_Bool CSLStrEnumProxy<EnumType,MaxValue>::IsValid
(
)
{
	return m_EnumValue <= MaxValue;
}

template <class EnumType, SI_Int MaxValue>
CSLAnimatableType::EElementType CSLStrEnumProxy<EnumType,MaxValue>::Type
(
)
{
	return SI_STRING_ENUM_TYPE;
}

template <class EnumType, SI_Int MaxValue>
SI_Float CSLStrEnumProxy<EnumType,MaxValue>::GetFloatValue
(
)
{
	return (SI_Float) m_EnumValue;
}

template <class EnumType, SI_Int MaxValue>
SI_Void CSLStrEnumProxy<EnumType,MaxValue>::SetFloatValue
(
	SI_Float	in_fValue
)
{
	m_EnumValue = (EnumType) ( (SI_Int) in_fValue);
	m_StringValue = m_pDictionary->ToString( m_EnumValue );
}

#endif //__SL_ENUM_H__
