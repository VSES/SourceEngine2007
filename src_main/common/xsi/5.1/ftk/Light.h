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


#ifndef _LIGHT_H
#define _LIGHT_H

#include "Primitive.h"
#include "ConstrainableType.h"
#include "LightInfo.h"

//! Base class for lights
class XSIEXPORT CSLLight
	: public CSLPrimitive
	, public CSLConstrainableType
{
public:

	/*! Synchronizes this light instance with its IO layer representation
	* \return SI_Error
	* \retval SI_SUCCESS this function always returns SI_SUCCESS
	*/
    virtual SI_Error Synchronize();

	/*! Gets a handle on one of this light's parameters by using its name
	* \param in_szName the name of the parameter to get the handle from
	* \return CSLAnimatableType* the handle to the parameter
	*/
    virtual CSLAnimatableType* ParameterFromName(SI_Char *in_szName);

	/*! Gets a handle on one of this light's parameters by using its name and type
	* \param in_szParameterName the name of the parameter to get the handle from
	* \param in_Type the type of the parameter to get the handle from
	* \return CSLAnimatableType* the handle to the parameter
	*/    
	virtual CSLAnimatableType* ParameterFromType(EFCurveType in_Type, SI_Char *in_szParameterName);

	/*! Destructor
	*/
	virtual ~CSLLight();

	/*! Return the color of the light.
	* \return CSIBCColorf Light color.
	*/
	CSIBCColorf GetColor();

	/*! Change the color of the light.
	* \param in_rColor New light color.
	* \return SI_Void
	*/
	SI_Void SetColor(CSIBCColorf &in_rColor);

	/*! Return the position of the light.
	* \return CSIBCVector3D Light position.
	*/
	CSIBCVector3D GetPosition();

	/*! Change the position of the light.
	* \param in_rValue New light position.
	* \return SI_Void
	*/
	SI_Void SetPosition(CSIBCVector3D &in_rValue);

	/*! Add a new constraint to the light.
	* \param in_ConstraintType Type of constraint to create.
	* \return CSLConstraint* Pointer to the newly created constraint object.
	*/
	CSLConstraint* AddConstraint(CSLConstraint::EConstraintType in_ConstraintType);
	
	///////////////////////////////////////////////////////////////////////////
	// light info Functionality /////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	/*! Creates a new light info
	* \return CSLLightInfo* The new light info or null if there's already one
	*/
	CSLLightInfo* CreateLightInfo();

	/*! Destroys the light info
	* \return SI_Error Whether or not the light info could be destroyed
	*/
	SI_Error DestroyLightInfo();

	/*! Connects an exisiting light info
	* \param in_pToConnect Pointer to the light info to use
	* \return CSLLightInfo* The added light info or null if there's already one
	*/
	CSLLightInfo* ConnectLightInfo( CSLLightInfo* in_pToConnect );

	/*! Gets a pointer to the light info
	* \return CSLLightInfo* Pointer to the light info
	*/
	CSLLightInfo* LightInfo();

protected:
	CSLLight(CSLScene* in_pScene, CSLModel *in_pModel, CdotXSITemplate* in_pTemplate);

	virtual SI_Bool ConstraintTypeIsValid(CSLConstraint::EConstraintType in_ConstraintType) = 0;

private:
	CSLColorRGBProxy m_Color;
    CSLVector3DProxy m_Position;
	CSLLightInfo *m_pLightInfo;

	void *m_pReserved;	// reserved for future extension
};

#endif
