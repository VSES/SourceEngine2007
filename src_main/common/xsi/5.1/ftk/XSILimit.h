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


#ifndef _LIMIT_H
#define _LIMIT_H

#include "Template.h"



//! Class for limits
/*!	Encapsulates the minimum and maximum limits
	\sa CSLAnimatableType
	\sa CSLTemplate
*/
class XSIEXPORT CSLXSILimit
	: public CSLTemplate
{
public:
	/*! Constructor
		\param in_pScene	Pointer to the scene 
		\param in_pModel	Pointer to the model 
		\param in_pTemplate	Pointer to the CdotXSITemplate 
	*/
	CSLXSILimit
	(
		CSLScene* in_pScene,
		CSLModel *in_pModel,
		CdotXSITemplate* in_pTemplate
	);
	//! Destructor
	virtual ~CSLXSILimit();

	ETemplateType Type(){ return XSI_LIMIT; }

	/*!	Returns the parameter connected to this limit.
		\return	CSLAnimatableType*	Pointer to a parameter proxy.
	*/
	CSLAnimatableType* GetParameter();

	/*! Connects a previously allocated parameter
		\param	in_pNewParameter	Parameter to connect
		\retval CSLAnimatableType*	The connected parameter (\p in_pNewParameter)
		\retval NULL A parameter was already connected.
	*/
	CSLAnimatableType* ConnectParameter(CSLAnimatableType* in_pNewParameter);

	/*! Gets the minimum value
		\retval SI_Float
	*/
	virtual SI_Float	GetMinimum();

	/*! Sets the minimum value
		\param in_fMin	The minimum value.
	*/
	virtual SI_Void		SetMinimum(SI_Float in_fMin);

	/*! Gets the maximum value
		\retval SI_Float
	*/
	virtual SI_Float	GetMaximum();

	/*! Sets the maximum value
		\param in_fMax	The maximum value.
	*/
	virtual SI_Void		SetMaximum(SI_Float in_fMax);

	/*! Gets the Minimum active value
		\retval SI_Bool
	*/
	virtual SI_Bool	GetMinimumActive();

	/*! Sets the Minimum active value
		\param in_bActive	Activeness.
	*/
	virtual SI_Void		SetMinimumActive(SI_Bool in_bActive);

	/*! Gets the Maximum active value
		\retval SI_Bool
	*/
	virtual SI_Bool	GetMaximumActive();

	/*! Sets the Maximum active value
		\param in_bActive	Activeness.
	*/
	virtual SI_Void		SetMaximumActive(SI_Bool in_bActive);

	/*! Gets the name of the parameter
		\return SI_Char*	The name.
	*/
	SI_Char* GetParameterName();

	/*! Sets the name of the parameter
		\param in_szParameterName	The name.
	*/
	SI_Void SetParameterName(SI_Char *in_szParameterName);

	/*!	Return the name (string) of a specified sub parameter.
		\param	in_pParameter	The sub parameter
		\return	SI_Char*	Name of the sub parameter.
	*/
    virtual const SI_Char* GetParameterName(CSLAnimatableType* in_pParameter);

	/*! Searches for a parameter by name
	* \param in_szName Name of the parameter to look for
	* \return CSLAnimatableType Pointer to the found parameter or null if not found
	*/
	virtual CSLAnimatableType *ParameterFromName(SI_Char *in_szName);

	/*! Searches for a parameter by name and type
	* \param in_Type Type of the parameter to look for
	* \param in_szName Name of the parameter to look for
	* \return CSLAnimatableType Pointer to the found parameter or null if not found
	*/
	virtual CSLAnimatableType *ParameterFromType(EFCurveType in_Type, SI_Char *in_szParameterName);

	/*! Evaluates this limit
	\retval SI_Error	SI_SUCCESS on success, SI_ERROR if the parameter is not connected
	*/
	virtual SI_Error	Evaluate();


private:
	
	CSLAnimatableType*	m_pParameter;
	CSLStringProxy		m_szParameterName;
	CSLFloatProxy		m_fMin;
	CSLFloatProxy		m_fMax;
	CSLBoolProxy		m_bMinActive;
	CSLBoolProxy		m_bMaxActive;

	void *m_pReserved;	// reserved for future extension
};

#endif
