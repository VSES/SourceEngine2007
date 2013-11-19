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


#ifndef _BASEFCURVE_H
#define _BASEFCURVE_H

#include "Template.h"

extern CEnumDictionary<CSLTemplate::EFCurveInterpolationType, CSLTemplate::SI_CUBIC> g_FCurveInterpolationTypeDictionary;

//! Base class for all key frame types
/*! A key represents the value of a CSLAnimatableType at a given time.
	\sa CSLBaseFCurve
*/
struct CSLBaseKey
{
	SI_Float m_fTime;	/*!< The time associated with the key */
	SI_Float m_fValue;  /*!< Value associated with the key */
};

/**
* Describes a constant fcurve key.
*/
struct CSLConstantKey : public CSLBaseKey
{
};


/**
* Describes a linear fcurve key.
*/
struct CSLLinearKey : public CSLBaseKey
{
};


/**
* Describes a hermite fcurve key.
*/
struct CSLHermiteKey : public CSLBaseKey
{
	SI_Float m_fInTangent;
	SI_Float m_fOutTangent;
};


/**
* Describes a cubic fcurve key.
*/
struct CSLCubicKey : public CSLBaseKey
{
	SI_Float m_fLeftTanX;
	SI_Float m_fLeftTanY;
	SI_Float m_fRightTanX;
	SI_Float m_fRightTanY;
};

//! Base class of all FCurves
/*!	A function curve is a graph representing the animation of parameters
	(CSLAnimatableType).  When you edit an FCurve, you change the 
	animation.  You can create an FCurve with the CSLTemplate::CreateFCurve 
	and CSLTemplate::CreateParameterFCurve.

	\note You can create FCurves for all CSLTemplate-derived classes with 
	CSLAnimatableType parameters.

	\sa CSLAnimatableType
	\sa CSLTemplate
*/
class XSIEXPORT CSLBaseFCurve
	: public CSLTemplate
{
public:
	/*! Proxy class that allows modifying constant fcurve keys like an array.
	*/
	typedef CSLArrayProxy<CSLConstantKey, SI_Float, 2> CSLConstantKeyArray;

	/*! Proxy class that allows modifying linear fcurve keys like an array.
	*/
	typedef CSLArrayProxy<CSLLinearKey, SI_Float, 2> CSLLinearKeyArray;

	/*! Proxy class that allows modifying hermite fcurve keys like an array.
	*/
	typedef CSLArrayProxy<CSLHermiteKey, SI_Float, 4> CSLHermiteKeyArray;

	/*! Proxy class that allows modifying cubic fcurve keys like an array.
	*/
	typedef CSLArrayProxy<CSLCubicKey, SI_Float, 6> CSLCubicKeyArray;

	//! Destructor
	virtual ~CSLBaseFCurve();

	/*!	Returns an array of constant fcurve keys.
		\retval	CSLConstantKeyArray*	Array of constant fcurve keys.
		\retval NULL Invalid interpolation type.
		\sa CSLBaseFCurve::GetInterpolationType
	*/
	CSLConstantKeyArray* GetConstantKeyList();

	/*!	Returns an array of cubic fcurve keys.
		\retval	CSLCubicKeyArray*	Array of cubic fcurve keys.
		\retval NULL Invalid interpolation type.
		\sa CSLBaseFCurve::GetInterpolationType
	*/
	CSLCubicKeyArray* GetCubicKeyList();

	/*!	Returns an array of hermite fcurve keys.
		\retval	CSLHermiteKeyArray*	Array of hermite fcurve keys.
		\retval NULL Invalid interpolation type.
		\sa CSLBaseFCurve::GetInterpolationType
	*/
	CSLHermiteKeyArray* GetHermiteKeyList();

	/*!	Returns an array of linear fcurve keys.
		\retval	CSLLinearKeyArray*	Array of linear fcurve keys.
		\retval NULL Invalid interpolation type.
		\sa CSLBaseFCurve::GetInterpolationType
	*/
	CSLLinearKeyArray* GetLinearKeyList();

	/*!	Returns an array of constant fcurve keys.
		\retval	CSLConstantKeyArray*	Array of constant fcurve keys.
		\retval NULL Invalid interpolation type.
		\sa CSLBaseFCurve::GetInterpolationType
	*/
	CSLConstantKey* GetConstantKeyListPtr();

	/*!	Returns an array of cubic fcurve keys.
		\retval	CSLCubicKeyArray*	Array of cubic fcurve keys.
		\retval NULL Invalid interpolation type.
		\sa CSLBaseFCurve::GetInterpolationType
	*/
	CSLCubicKey* GetCubicKeyListPtr();

	/*!	Returns an array of hermite fcurve keys.
		\retval	CSLHermiteKeyArray*	Array of hermite fcurve keys.
		\retval NULL Invalid interpolation type.
		\sa CSLBaseFCurve::GetInterpolationType
	*/
	CSLHermiteKey* GetHermiteKeyListPtr();

	/*!	Returns an array of linear fcurve keys.
		\retval	CSLLinearKeyArray*	Array of linear fcurve keys.
		\retval NULL Invalid interpolation type.
		\sa CSLBaseFCurve::GetInterpolationType
	*/
	CSLLinearKey* GetLinearKeyListPtr();

	/*!	Returns the interpolation type of the fcurve.
		\retval	EFCurveInterpolationType	Type of fcurve interpolation.
		\retval NULL Invalid interpolation type.
		\sa CSLBaseFCurve::GetInterpolationType
	*/
	CSLTemplate::EFCurveInterpolationType GetInterpolationType();

	/*!	Returns the number of elements required to represent a key.
		\return	SI_Int	Number of key values.
	*/
	SI_Int GetKeyValueCount();

	/*!	Returns the number of keys in the fcurve.
		\return	SI_Int	Number of keys.
		\sa CSLBaseKey
	*/
	SI_Int GetKeyCount();

	/*!	Evalutes the FCurve at a given time
		\param	in_fTime	The time
		\retval	SI_SUCCESS
		\sa CSLBaseKey
	*/
	SI_Error Evaluate( SI_Float in_fTime );

	/*!	Returns the value of the last evaluation
		\return	SI_Float	Value of the last evaluation
		\sa CSLBaseFCurve::Evaluate
	*/
	SI_Float GetLastEvaluation() { return m_fLastEvaluation; };

	/*!	Returns the value of the curve at the time specified or at the next key 
		\param	in_fFloat	The time
		\return	SI_Float	The value
	*/
	SI_Float GetValueAtNextFrame ( SI_Float in_fFloat );

	/*!	Returns the value of the curve at the time specified or at the previous key 
		\param	in_fFloat	The time
		\return	SI_Float	The value
	*/
	SI_Float GetValueAtPrevFrame ( SI_Float in_fFloat );

	/*!	Returns the key index located on or before the time passed in
		\param	in_fFloat	The time
		\return	SI_Int	Key index
		\sa CSLBaseKey
	*/
	SI_Int	FindKeyAtOrBefore ( SI_Float in_fFloat );

	/*!	Returns the parameter connected to this fcurve.
		\return	CSLAnimatableType*	Pointer to a parameter proxy.
	*/
	CSLAnimatableType* GetParameter();

	/*! Connects a previously allocated parameter
		\param	in_pNewParameter	Parameter to connect
		\retval CSLAnimatableType*	The connected parameter (\p in_pNewParameter)
		\retval NULL A parameter was already connected.
	*/
	CSLAnimatableType* ConnectParameter(CSLAnimatableType* in_pNewParameter);

	virtual SI_Error Synchronize();

protected:

	/*! Constructor
		\param in_pScene	Pointer to the scene containing the BaseFCurve
		\param in_pModel	Pointer to the model containing the BaseFCurve
		\param in_pTemplate	Pointer to the CdotXSITemplate containing the BaseFCurve
		\param in_InterpolationType	The type of interpolation to be used in the BaseFCurve
	*/
	CSLBaseFCurve
	(
		CSLScene* in_pScene,
		CSLModel *in_pModel,
		CdotXSITemplate* in_pTemplate,
		EFCurveInterpolationType in_InterpolationType
	);
	CSLAnimatableType* m_pParameter;

private:
	CSLStrEnumProxy<EFCurveInterpolationType, SI_CUBIC> m_InterpolationType;
	union
	{
		CSLConstantKeyArray* m_pConstantKeys;
		CSLLinearKeyArray* m_pLinearKeys;
		CSLHermiteKeyArray* m_pHermiteKeys;
		CSLCubicKeyArray* m_pCubicKeys;
	};
	
	SI_Int				m_iCurrentKey;		// for evaluation optimization
	SI_Float			m_fCurrentTime;		// @DEBUG
	SI_Float			m_fLastEvaluation;	// Hold the value of the last evaluation

	inline SI_Float	GetTimeAtKey ( SI_Int );

	void *m_pReserved;	// reserved for future extension
};

#endif
