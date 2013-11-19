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

#ifndef _STATICVALUE_H
#define _STATICVALUE_H

#include "Template.h"

/*! Stores a pose (static action).  You can create a CSLStaticValue with the 
	CSLAction::AddStaticValue method.
	\sa CSLAction
*/
class XSIEXPORT CSLStaticValue
    : public CSLTemplate
{
public:
	/*! Constructor
		\param in_pScene Parent scene
		\param in_pModel Parent model
		\param in_pTemplate Referred template
	*/
    CSLStaticValue(CSLScene* in_pScene, CSLModel *in_pModel, CdotXSITemplate* in_pTemplate);
    virtual ~CSLStaticValue();

	/*! Returns the type of the template
		\return CSLTemplate::ETemplateType The type of this template (XSI_STATIC_VALUES)
	*/
	CSLTemplate::ETemplateType Type(){ return XSI_STATIC_VALUES; }
    
	/*! Gets whether or not the parameter is active.
		\return SI_Bool Value of the flag
	*/
	SI_Bool GetActive();

	/*! Sets whether or not the parameter is active.
		\param in_bActive Value of the flag
	*/
	SI_Void SetActive( SI_Bool in_bActive );

	/*! Gets the name of the parameter
		\return SI_Char Name of the parameter
	*/
	SI_Char* GetParameterName();

	/*! Sets the name of the parameter
		\param in_szName Name of the parameter
	*/
	SI_Void SetParameterName( SI_Char* in_szName );

	/*! Gets the value of the parameter
		\return SI_Float Value of the parameter
	*/
	SI_Float GetValue();

	/*! Sets the value of the parameter
		\param in_fValue Value of the parameter
	*/
	SI_Void SetValue( SI_Float in_fValue );

private:
    CSLBoolProxy m_Active;
    CSLStringProxy m_ParameterName;
    CSLDoubleProxy m_Value;

	SI_Void *m_pReserved;
};

#endif
