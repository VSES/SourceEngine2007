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

#ifndef _ANGLE_H
#define _ANGLE_H

#include "Template.h"

/*! Defines the way angles are expressed in the scene.  You can retrieve the
	angle type with the CSLScene:Angle method.

	\note This template is ignored when imported into SOFTIMAGE|XSI, although 
	a warning appears if the angle is not expressed in degrees.

	\sa CSLScene
*/
class XSIEXPORT CSLAngle
    : public CSLTemplate
{
public:

	//! Angle units
	enum EAngleType
	{
		SI_DEGREES,	/*!< Angles are expressed in degrees */
		SI_RADIANS	/*!< Angles are expressed in radians */
	};

	//! Constructor
	/*!
		\param in_pScene Pointer to the scene containing the CSLAngle
		\param in_pModel Pointer to the model containing the CSLAngle
		\param in_pTemplate Pointer to the CdotXSITemplate defining the CSLAngle
	*/
	CSLAngle
	(
		CSLScene* in_pScene, 
		CSLModel *in_pModel, 
		CdotXSITemplate* in_pTemplate
	);
    
	~CSLAngle();

	/*! Gets the angle type
		\return The angle type
	*/
    EAngleType GetAngleType();

	/*! Sets the angle type
		\param in_Type The angle type
	*/
    void SetAngleType( EAngleType in_Type );

	/*! Returns the type of this template
		\retval CSLTemplate::SI_ANGLE
	*/
    inline ETemplateType Type();

private:
    CSLEnumProxy<EAngleType, SI_RADIANS> m_Angle;
	SI_Void *in_pReserved;	// reserved for future extension
};

#endif
