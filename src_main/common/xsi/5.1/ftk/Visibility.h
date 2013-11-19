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

/******************************************************************************\
*
* File:          Visibility.h
* Creation date: January 15, 2002
* Purpose:       Declaration of class 'Visibility'
*
\******************************************************************************/
#ifndef _VISIBILITY_H
#define _VISIBILITY_H

#include "Template.h"

/** 
* Class CSLVisibility
* Describes the visibility of a model.
*
* The Visibility of a model can be switched on and off using this template. (which basically
* wraps the SI_Visibility template).
*/
class XSIEXPORT CSLVisibility
    : public CSLTemplate
{
public:
    CSLVisibility(CSLScene* in_pScene, CSLModel *in_pModel, CdotXSITemplate* in_pTemplate);
    virtual ~CSLVisibility();

	/*!	Return the visibility value.
		\return	SI_Bool	Visibility value.
	*/
	SI_Bool GetVisibility();

	/*!	Change the visibility value.
		\param	SI_Bool	New visibility value.
		\return	None.
	*/
	SI_Void SetVisibility(SI_Bool in_rValue);

    virtual CSLAnimatableType* ParameterFromName(SI_Char *in_szName);
    virtual CSLAnimatableType* ParameterFromType(EFCurveType in_Type, SI_Char *in_szParameterName);
    virtual ETemplateType Type();

private:
    CSLBoolProxy m_Visiblility;

	void *m_pReserved;	// reserved for future extension
};

#endif
