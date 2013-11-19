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

#ifndef _ACTIONFCURVE_H
#define _ACTIONFCURVE_H

#include "BaseFCurve.h"

/*! Wraps all functionality for an FCurve used in a mixer.
	\sa CSLMixer
	\sa CSLAction
*/
class XSIEXPORT CSLActionFCurve
    : public CSLBaseFCurve
{
public:

	/*! Constructor
		\param in_pScene	Pointer to the scene containing the ActionFCurve
		\param in_pModel	Pointer to the model containing the ActionFCurve
		\param in_pTemplate	Pointer to the CdotXSITemplate containing the ActionFCurve
		\param in_InterpolationType	FCurve interpolation type
	*/
    CSLActionFCurve(CSLScene* in_pScene, CSLModel *in_pModel, CdotXSITemplate* in_pTemplate, EFCurveInterpolationType in_InterpolationType);

	virtual ~CSLActionFCurve();
    
	/*! Gets the XSI path of the animated parameter
		\return SI_Char The XSI Path of the animated parameter
	*/
	SI_Char* GetParameterXSIPath();

	/*! Sets the XSI path of the animated parameter
		\param in_szPath The XSI Path of the animated parameter
	*/
	SI_Void SetParameterXSIPath( SI_Char *in_szPath );

	SI_Error Synchronize();

	/*! Returns the type of this template
		\retval CSLTemplate::SI_ACTION_FCURVE
	*/
    ETemplateType Type() { return SI_ACTION_FCURVE; }

	/*! Sets Parameter
		\param in_pParameter Pointer to the parameter to use
	*/
	SI_Void SetParameter( CSLAnimatableType *in_pParameter );

	/*! Gets the active flag
		\return The active state of the fcurve
	*/
	SI_Bool GetActive();

	/*! Sets the active flag
		\return in_bNew The new active state of the fcurve
	*/
	SI_Void SetActive( SI_Bool in_bNew );

private:
    CSLStringProxy m_XSIPath;
	SI_Bool m_bActive;
	
	SI_Void *m_pReserved;
};

#endif
