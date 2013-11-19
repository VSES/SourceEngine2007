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

#ifndef _AMBIENCE_H
#define _AMBIENCE_H

#include "Template.h"

/*! Describe the abient color fo the scene.  You can retrieve ambience color
	information with the CSLScene::Ambience() method.

	\sa CSLScene
*/
class XSIEXPORT CSLAmbience
    : public CSLTemplate
{
public:
    /*!	Constructor
		\param in_pScene Pointer to the scene containing the CSLAmbience
		\param in_pModel Pointer to the model containing the CSLAmbience
		\param in_pTemplate Pointer to the CdotXSITemplate that defines the CSLAmbience
	*/
	CSLAmbience(CSLScene* in_pScene, CSLModel *in_pModel, CdotXSITemplate* in_pTemplate);
    virtual ~CSLAmbience();

	/*!	Returns the scene's ambient color. 
		\return CSIBCColorf	Color representing this object.
	*/
    CSIBCColorf GetColor();

	/*!	Changes the scene's ambient color.
		\param in_rColor a CSIBCColorf containing the new ambient color.
	*/
	SI_Void SetColor(CSIBCColorf &in_rColor);

	/*! Returns the type of this template
		\retval CSLTemplate::SI_AMBIENCE
	*/
	virtual ETemplateType Type();

	CSLAnimatableType* ParameterFromName(SI_Char *in_szName);
    CSLAnimatableType* ParameterFromType(EFCurveType in_Type, SI_Char *in_szParameterName);

private:
    CSLColorRGBProxy m_Color;

	void *m_pReserved;	// reserved for future extension
};

#endif
