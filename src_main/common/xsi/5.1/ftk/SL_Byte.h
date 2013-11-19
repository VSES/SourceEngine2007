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

#ifndef _SL_BYTE_H
#define _SL_BYTE_H

#include "AnimatableType.h" // CSLAnimatableType

//! Class that allows modification of a byte parameter in a template
class XSIEXPORT CSLByteProxy : public CSLAnimatableType
{
public:
	/*! Constructor. Connects to a byte parameter in a template
		\param in_pTemplate Pointer to the template
		\param in_nIndex Index of the parameter in the template
	 */
	CSLByteProxy(CdotXSITemplate *in_pTemplate, SI_Int in_nIndex );

    /*! Assignment operator. Assigns \p in_Value to the parameter referred by this proxy.
		\param	in_Value	The new value
		\return Reference to the current object.
	*/
	CSLByteProxy& operator =(const CSLByteProxy &in_Value);

	/*! Assignment operator. Assigns \p in_Value to the parameter referred by this proxy.
		\param	in_Value	The new value
		\return Reference to the current object.
	*/
    CSLByteProxy& operator =(const SI_Byte &in_Value);

	/*! Comparison operator. Compares \p in_ToCompare with the value of the parameter referred by this proxy.
		\param	in_ToCompare	The instance to compare values with
		\return	Whether or not the referred parameter and \p in_ToCompare are equal
	*/
    SI_Bool operator ==(const CSLByteProxy &in_ToCompare);

	/*! Comparison operator. Compares \p in_ToCompare with the value of the parameter referred by this proxy.
		\param	in_ToCompare	The instance to compare values with
		\return	Whether or not the referred parameter and \p in_ToCompare are equal
	*/
    SI_Bool operator ==(const SI_Byte &in_ToCompare);

	//! Conversion operator
    operator SI_Byte();

	/*! Returns the animatable type of this class
		\return CSLAnimatableType::SI_BYTE_TYPE
	*/
	virtual EElementType Type();

	virtual SI_Float GetFloatValue();
	virtual SI_Void SetFloatValue(SI_Float in_fValue);

private:
	CSLByteProxy( CSLByteProxy &in_pBasicType ){}
    SI_Byte* m_pValue;
};

///////////////////////////////////////////////////////////////////////////////

//! Class that allows modification of a usigned byte parameter in a template
class XSIEXPORT CSLUByteProxy : public CSLAnimatableType
{
public:
	/*! Constructor. Connects to an unsigned byte parameter in a template
		\param in_pTemplate Pointer to the template
		\param in_nIndex Index of the parameter in the template
	 */
	CSLUByteProxy(CdotXSITemplate *in_pTemplate, SI_Int in_nIndex );

    /*! Assignment operator. Assigns \p in_Value to the parameter referred by this proxy.
		\param	in_Value	The new value
		\return Reference to the current object.
	*/
	CSLUByteProxy& operator =(const CSLUByteProxy &in_Value);

	/*! Assignment operator. Assigns \p in_Value to the parameter referred by this proxy.
		\param	in_Value	The new value
		\return Reference to the current object.
	*/
    CSLUByteProxy& operator =(const SI_UByte &in_Value);

	/*! Comparison operator. Compares \p in_ToCompare with the value of the parameter referred by this proxy.
		\param	in_ToCompare	The instance to compare values with
		\return	Whether or not the referred parameter and \p in_ToCompare are equal
	*/
    SI_Bool operator ==(const CSLUByteProxy &in_ToCompare);

	/*! Comparison operator. Compares \p in_ToCompare with the value of the parameter referred by this proxy.
		\param	in_ToCompare	The instance to compare values with
		\return	Whether or not the referred parameter and \p in_ToCompare are equal
	*/
    SI_Bool operator ==(const SI_UByte &in_ToCompare);

	//! Conversion operator
    operator SI_UByte();

	/*! Returns the animatable type of this class
		\return CSLAnimatableType::SI_UBYTE_TYPE
	*/
	virtual EElementType Type();

	virtual SI_Float GetFloatValue();
	virtual SI_Void SetFloatValue(SI_Float in_fValue);

private:
	CSLUByteProxy( CSLUByteProxy &in_pBasicType ){}
    SI_UByte* m_pValue;
};

#endif
