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

#ifndef _DOUBLE_H
#define _DOUBLE_H

#include <SIBCMath.h>		// SI_EPSILON
#include "AnimatableType.h" // CSLAnimatableType

//! Class that allows modification of a double parameter in a template
class XSIEXPORT CSLDoubleProxy : public CSLAnimatableType
{
public:
	/*! Constructor. Connects to a double parameter in the template
		\param in_pTemplate Pointer to the template
		\param in_Index Index of the parameter in the template
		\param in_dThreshold Tolerance value used to determine equality between two doubles.
		\sa CSLDoubleProxy::Threshold
		\sa CSLDoubleProxy::SetThreshold
	 */
    CSLDoubleProxy(CdotXSITemplate *in_pTemplate, SI_Int in_nIndex, SI_Double in_dThreshold = SI_EPSILON);

	/*! Assignment operator. Assigns \p in_Value to the parameter referred by this proxy.
		\param	in_Value	The new value
		\return Reference to the current object.
	*/
    CSLDoubleProxy& operator =(const CSLDoubleProxy &in_Value);

	/*! Assignment operator. Assigns \p in_Value to the parameter referred by this proxy.
		\param	in_Value	The new value
		\return Reference to the current object.
	*/
    CSLDoubleProxy& operator =(const SI_Double &in_Value);

	/*! Comparison operator. Compares \p in_ToCompare with the value of the parameter referred by this proxy.
		\param	in_ToCompare	The instance to compare values with
		\return	Whether or not the referred parameter and \p in_ToCompare are equal
		\note Comparison is performed with a threshold value.  You can perform 
		exact comparison by setting the Threshold value to zero.
		\sa CSLDoubleProxy::SetThreshold
		\sa CSLDoubleProxy::Threshold
	*/
    SI_Bool operator ==(const CSLDoubleProxy &in_ToCompare);

	/*! Comparison operator. Compares \p in_ToCompare with the value of the parameter referred by this proxy.
		\param	in_ToCompare	The instance to compare values with
		\return	Whether or not the referred parameter and \p in_ToCompare are equal
		\note Comparison is performed with a threshold value.  You can perform 
		exact comparison by setting the Threshold value to zero.
		\sa CSLDoubleProxy::SetThreshold
		\sa CSLDoubleProxy::Threshold
	*/
    SI_Bool operator ==(const SI_Double &in_ToCompare);

	//! Conversion operator
    operator SI_Double();

	/*! Returns the threshold value
		\return Tolerance value used to determine equality between two doubles.
		\sa CSLDoubleProxy::SetThreshold
	*/
	SI_Double Threshold();

	/*! Sets the thresold value
		\param in_dThreshold Tolerance value used to determine equality between two doubles.
		\b Example
		\code
		// no threshold, the values must be absolutely equal
		CSLDoubleProxy l_dProxy( in_pTemplate, in_paramIdx, 0.0 );
		l_dProxy = 0.00501;

		if ( l_dProxy == 0.005 )
		{
			// We get here because abs(0.00501 - 0.005) > 0
		}

		// use a tolerance of 0.0001
		l_dProxy.SetThreshold( 0.0001 ); 

		if ( l_dProxy == 0.005 )
		{
			// We get here because abs(0.00501 - 0.005) <= 0.0001
		}

		\endcode
		\sa CSLDoubleProxy::Threshold
	*/
	void SetThreshold( SI_Double in_dThreshold );

	/*! Returns the animatable type of this class
		\return CSLAnimatableType::SI_DOUBLE_TYPE
	*/
	virtual EElementType Type();

	virtual SI_Float GetFloatValue();
	virtual SI_Void SetFloatValue(SI_Float in_fValue);

private:
	CSLDoubleProxy( CSLDoubleProxy &in_pBasicType ){}
	SI_Bool Equal( SI_Double in_Double1, SI_Double in_Double2 );

	SI_Double m_dThreshold;
    SI_Double* m_pValue;
};

#endif
