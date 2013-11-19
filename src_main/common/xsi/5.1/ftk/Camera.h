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

#ifndef _CAMERA_H
#define _CAMERA_H

#include "Primitive.h"
#include "ConstrainableType.h"

//! Class describing a camera and its interest
/** 
* Describes a camera and its interest.
*/
class XSIEXPORT CSLCamera
	: public CSLPrimitive, public CSLConstrainableType
{
public:

	/*! Constructor
		\param in_pScene	Pointer to the scene containing the Camera
		\param in_pModel	Pointer to the model containing the Camera
		\param in_pTemplate	Pointer to the CdotXSITemplate containing the Camera
	*/
	CSLCamera(CSLScene* in_pScene, CSLModel *in_pModel, CdotXSITemplate* in_pTemplate);

	/*! Destructor
	*/
	virtual ~CSLCamera();

	/*!	Return the far plane of the camera. 
		\return SI_Float	Far plane.
	*/
	SI_Float GetFarPlane();

	/*!	Change the far plane of the camera.
		\param	SI_Float	New far plane. value
	*/
	SI_Void SetFarPlane(SI_Float in_fValue);

	/*!	Return the field of view of the camera. 
		\return SI_Float	Field of view.
	*/
	SI_Float GetFieldOfView();

	/*!	Change the field of view of the camera.
		\param	SI_Float	New field of view value.
	*/
	SI_Void SetFieldOfView(SI_Float in_fValue);

	/*!	Return the position of the camera's interest. 
		\return CSIBCVector3D	Interest position.
	*/
	CSIBCVector3D GetInterestPosition();

	/*!	Change the position of the camera's interest.
		\param	CSIBCVector3D&	New interest position.
	*/
	SI_Void SetInterestPosition(CSIBCVector3D& in_rValue);

	/*!	Return the position of the camera. 
		\return CSIBCVector3D	Position.
	*/
	CSIBCVector3D GetPosition();

	/*!	Change the position of the camera.
		\param	CSIBCVector3D&	New position.
	*/
	SI_Void SetPosition(CSIBCVector3D& in_rValue);

	/*!	Return the near plane of the camera. 
		\return SI_Float	Near plane value.
	*/
	SI_Float GetNearPlane();

	/*!	Change the near plane of the camera.
		\param	SI_Float	New near plane value.
	*/
	SI_Void SetNearPlane(SI_Float in_fValue);

	/*!	Return the roll of the camera. 
		\return SI_Float	Roll.
	*/
	SI_Float GetRoll();

	/*!	Change the roll of the camera.
		\param	SI_Float	New roll value.
	*/
	SI_Void SetRoll(SI_Float in_fValue);

	/*!	Add a new constraint to the camera.
		\param	EConstraintType	Type of constraint to create.
		\return	CSLConstraint*	Pointer to the newly created constraint object.
	*/
	CSLConstraint* AddConstraint(CSLConstraint::EConstraintType in_ConstraintType);

    virtual SI_Error Synchronize();
    virtual ETemplateType Type();
    virtual CSLAnimatableType* ParameterFromName(SI_Char *in_szName);

	/*!	Returns an AnimatableParameter from a EFCurveType.
		\param	EFCurveType	FCurve Type
		\param	SI_Char*	The name of the parameter if this FCurve this is unknown.
		\return	SI_Error	Error code.
		\note	The Camera supports the following FCurve types:
				- SI_POSITION_X
				- SI_POSITION_Y
				- SI_POSITION_Z
				- SI_INTEREST_X
				- SI_INTEREST_Y
				- SI_INTEREST_Z
				- SI_ROLL
				- SI_FOV
				- SI_NEAR
				- SI_FAR
	*
	* <B> Example - Adding FCurves on a Camera </B>
	*
	\code

		CSLFCurve* l_pFCurveX = l_pCamera->CreateFCurve( CSLTemplate::SI_POSITION_X, CSLTemplate::SI_CUBIC );
		CSLFCurve* l_pFCurveY = l_pCamera->CreateFCurve( CSLTemplate::SI_POSITION_Y, CSLTemplate::SI_CUBIC );
		CSLFCurve* l_pFCurveZ = l_pCamera->CreateFCurve( CSLTemplate::SI_POSITION_Z, CSLTemplate::SI_CUBIC );

		CSLFCurve* l_pFCurveRoll = l_pCamera->CreateFCurve( CSLTemplate::SI_ROLL, CSLTemplate::SI_CUBIC );

		
		// Add the keys etc...

	\endcode

	*/
    virtual CSLAnimatableType* ParameterFromType(EFCurveType in_Type, SI_Char *in_szParameterName);

private:
	CSLVector3DProxy m_InterestPosition;
	CSLVector3DProxy m_Position;
	CSLFloatProxy m_Roll;
	CSLFloatProxy m_FieldOfView;
	CSLFloatProxy m_NearPlane;
	CSLFloatProxy m_FarPlane;

	void *m_pReserved;	// reserved for future extension
};

#endif
