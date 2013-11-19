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

#ifndef _XSIMATERIALINFO_H
#define _XSIMATERIALINFO_H

#include "Template.h"

class CSLXSIMaterial;

/** 
* Class CSLXSIMaterialInfo
*
* Provides additional information regarding a CSLXSIMaterial*
* \note This template was introduced in dotXSI Version 3.6
*/

class XSIEXPORT CSLXSIMaterialInfo
    : public CSLTemplate
{
public:

	//! Defines the various wrap types
	enum EWrapType
	{
		SI_CLAMP,	/*!< Clamp texture */
		SI_REPEAT	/*!< Repeat texture */
	};

	/*! Constructor
		\param in_pScene Pointer to the scene
		\param in_pModel Pointer to the model
		\param in_pTemplate Pointer to the parent template
	*/
	CSLXSIMaterialInfo(CSLScene* in_pScene, CSLModel *in_pModel, CdotXSITemplate* in_pTemplate);

    //! Destructor
	virtual ~CSLXSIMaterialInfo();

	/*! Gets the type of this template
		\return ETemplateType The type of this template: XSI_MATERIAL_INFO
	*/
	ETemplateType Type(){ return XSI_MATERIAL_INFO; }
	
    virtual CSLAnimatableType* ParameterFromName(SI_Char *in_szName);

	/*! Gets the UWrap type
		\return EWrapType The type
	*/
	EWrapType GetUWrap();

	/*! Sets the UWrap type
		\param In_Type The type
	*/
	SI_Void SetUWrap( EWrapType in_Type );

	/*! Gets the VWrap type
		\return EWrapType The type
	*/
	EWrapType GetVWrap();

	/*! Sets the VWrap type
		\param In_Type The type
	*/
	SI_Void SetVWrap( EWrapType in_Type );

private:
    CSLEnumProxy<EWrapType, SI_REPEAT> m_UWrap;
    CSLEnumProxy<EWrapType, SI_REPEAT> m_VWrap;

	SI_Void *m_pReserved;
};

#endif
