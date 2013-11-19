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

#ifndef _FLOAT_H
#define _FLOAT_H

#include <SIBCMath.h>		// SI_EPSILON
#include "AnimatableType.h" // CSLAnimatableType

//! Class that allows modification of a float parameter in a template
class XSIEXPORT CSLFloatProxy : public CSLAnimatableType
{
public:
	/*! Constructor. Connects to a parameter in the parent template
		\param in_pTemplate Pointer to the parent template
		\param in_Index Index of the parameter in the template
		\param in_fThreshold Tolerance value used to determine equality between two floats.
		\sa CSLFloatProxy::Threshold
		\sa CSLFloatProxy::SetThreshold
	 */
    CSLFloatProxy(CdotXSITemplate *in_pTemplate, SI_Int in_nIndex, SI_Float in_fThreshold = SI_EPSILON);

	/*! Assignment operator. Assigns \p in_Value to the parameter referred by this proxy.
		\param	in_Value	The new value
		\return Reference to the current object.
	*/
    CSLFloatProxy& operator =(const CSLFloatProxy &in_Value);

	/*! Assignment operator. Assigns \p in_Value to the parameter referred by this proxy.
		\param	in_Value	The new value
		\return Reference to the current object.
	*/
    CSLFloatProxy& operator =(const SI_Float &in_Value);

	/*! Comparison operator. 
		Compares \p in_ToCompare with the value of the parameter referred by this proxy.
		\param	in_ToCompare	The instance to compare values with
		\return	Whether or not the referred parameter and \p in_ToCompare are 
		equal
		\note Comparison is performed with a threshold value.  You can perform 
		exact comparison by setting the Threshold value to zero.
		\sa CSLFloatProxy::Threshold
		\sa CSLFloatProxy::SetThreshold
	*/
    SI_Bool operator ==(const CSLFloatProxy &in_ToCompare);

	/*! Comparison operator. Compares \p in_ToCompare with the value of the parameter referred by this proxy.
		\param	in_ToCompare	The instance to compare values with
		\return	Whether or not the referred parameter and \p in_ToCompare are equal
		\note Comparison is performed with a threshold value.  You can perform 
		exact comparison by setting the Threshold value to zero.
		\sa CSLFloatProxy::Threshold
		\sa CSLFloatProxy::SetThreshold
	*/
    SI_Bool operator ==(const SI_Float &in_ToCompare);

	//! Conversion operator
    operator SI_Float();

	/*! Returns the threshold value
		\return Tolerance value used to determine equality between two floats.
		\sa CSLFloatProxy::SetThreshold
	*/
	SI_Float Threshold();

	/*! Sets the thresold value
		\param in_fThreshold Tolerance value used to determine equality between two floats.
		\b Example
		\code
		// no threshold, the values must be absolutely equal
		CSLFloatProxy l_fProxy( in_pTemplate, in_paramIdx, 0.0 );
		l_fProxy = 0.00501;

		if ( l_fProxy == 0.005F )
		{
			// We get here because abs(0.00501 - 0.005) > 0
		}

		// use a tolerance of 0.0001
		l_fProxy.SetThreshold( 0.0001F ); 

		if ( l_fProxy == 0.005F )
		{
			// We get here because abs(0.00501 - 0.005) <= 0.0001
		}

		\endcode
		\sa CSLDoubleProxy::Threshold
	*/
	void SetThreshold( SI_Float in_fThreshold );

	/*! Returns the animatable type of this class
		\return CSLAnimatableType::SI_FLOAT_TYPE
	*/
	virtual EElementType Type();

	virtual SI_Float GetFloatValue();
	virtual SI_Void SetFloatValue(SI_Float in_fValue);

private:
	// no copy constructor
	CSLFloatProxy( CSLFloatProxy &in_pBasicType ){}
	
	SI_Bool Equal( SI_Float in_Float1, SI_Float in_Float2 );

	SI_Float m_fThreshold;
    SI_Float* m_pValue;
};

#endif
