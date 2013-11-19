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

#ifndef _SL_STRING_H
#define _SL_STRING_H

#include <SIBCUtil.h>		// SI_Int, SI_Float and other basic types 
#include <dotXSITemplate.h>	// CdotXSITemplate

//! Class that allows modification of a string parameter in a template
class XSIEXPORT CSLStringProxy
{
public:
	/*! Constructor. Connects to a parameter in the parent template
		\param in_pTemplate Pointer to the parent template
		\param in_Index Index of the parameter in the template
	 */
	CSLStringProxy(CdotXSITemplate *in_pTemplate, SI_Int in_Index );

	/*! Constructor Creates an unconnected proxy
	*/
	CSLStringProxy();

	/*! Connects the proxy to a parameter of a template
		\param in_pTemplate Parent template
		\param in_nIndex Index of the parameter in the parent template
		\retval SI_SUCCESS
	*/
	SI_Error Connect(CdotXSITemplate *in_pTemplate, SI_Int in_nIndex );

	/*! Assignment operator. Assigns \p in_Value to the parameter referred by this proxy.
		\param	in_Value	The new value
		\return Reference to the current object.
	*/
	CSLStringProxy& operator =(const CSLStringProxy &in_Value);

	/*! Assignment operator. Assigns \p in_Value to the parameter referred by this proxy.
		\param	in_Value	The new value
		\return Reference to the current object.
	*/
	CSLStringProxy& operator =(const char* in_Value);

	/*! Comparison operator. Compares \p in_ToCompare with the value of the parameter referred by this proxy.
		\param	in_ToCompare	The instance to compare values with
		\return	Whether or not the referred parameter and \p in_ToCompare are equal
		\note String comparison \b is case-sensitive
	*/
	SI_Bool operator ==(const CSLStringProxy& in_ToCompare);

	/*! Comparison operator. Compares \p in_ToCompare with the value of the parameter referred by this proxy.
		\param	in_ToCompare	The instance to compare values with
		\return	Whether or not the referred parameter and \p in_ToCompare are equal
		\note String comparison \b is case-sensitive
	*/
	SI_Bool operator ==(const char* in_ToCompare);

	//! Conversion operator
	operator char* ();

private:
	char** m_ppValue;

	// disable copy constructor
	CSLStringProxy( const CSLStringProxy &in_ToCopy ){};
};

#endif
