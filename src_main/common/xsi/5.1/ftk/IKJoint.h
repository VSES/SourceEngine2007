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

#ifndef _IKJOINT_H
#define _IKJOINT_H

#include "IK.h"

// Forward declaration
class CSLJointSolverType;

//! Describes one joint of an IK chain.
class XSIEXPORT CSLIKJoint
	: public CSLIK
{
public:
	//! Joint solver types
	enum EJointSolverType
	{
		SI_DEFAULT,			//!< default joint type
		SI_2D,				//!< 2D joint
		SI_3D,				//!< 3D joint
	};

	/*! Constructor
	* \param in_pScene	Pointer to the scene containing the joint
	* \param in_pModel	Pointer to the model containing the joint
	* \param in_pTemplate	Pointer to the CdotXSITemplate containing the joint
	* \return an instance of CSLIKJoint
	*/
	CSLIKJoint(CSLScene* in_pScene, CSLModel *in_pModel, CdotXSITemplate* in_pTemplate, CSLModel* in_pRoot);

	/*! Destructor
	*/
	virtual ~CSLIKJoint();

	/*! Return the length value of the joint.
	* \return SI_Float	Length value.
	*/
	SI_Float GetLength();

	/*! Change the length value of the joint.
	* \param	in_fValue	New length value.
	* \return SI_Void
	*/
	SI_Void SetLength(SI_Float in_fValue);

	/*! Return the prefered rotation of the joint.
	* \return CSIBCVector3D	Preferred rotation value.
	*/
	CSIBCVector3D GetPreferredRotation();

	/*! Change the preffered rotation value of the joint.
	* \param	in_rValue	New preferred rotation value.
	* \return SI_Void
	*/
	SI_Void SetPreferredRotation(CSIBCVector3D &in_rValue);

	/*! Return a flag indicating if the joint is a pseudo-root.
	* \return SI_Bool Pseudo-root status.
	*/
	SI_Bool GetPseudoRoot();

	/*! Change the flag indicating the joint is a pseudo-root.
	* \param	in_bValue New Pseudo-root status.
	* \return SI_Void
	*/
	SI_Void SetPseudoRoot(SI_Bool in_bValue);

	/*! Return the rotation limit activation flag of the joint.
	* \return SI_Bool Rrotation limit activation flag.
	*/
	SI_Bool GetRotationLimitActivation();

	/*! Change the rotation limit activation flag of the joint.
	* \param	in_bValue New rotation limit activation flag.
	* \return SI_Void
	*/
	SI_Void SetRotationLimitActivation(SI_Bool in_bValue);

	/*! Return the maximum rotation limit of the joint.
	* \return CSIBCVector3D	Maximum rotation limit.
	*/
	CSIBCVector3D GetRotationLimitMaximum();

	/*! Change the maximum rotation limit value of the joint.
	* \param	in_rValue	New maximum rotation limit.
	* \return SI_Void
	*/
	SI_Void SetRotationLimitMaximum(CSIBCVector3D &in_rValue);

	/*! Return the minimum rotation limit of the joint.
	* \return CSIBCVector3D	Minimum rotation limit.
	*/
	CSIBCVector3D GetRotationLimitMinimum();

	/*! Change the minimum rotation limit value of the joint.
	* \param	in_rValue	New minimum rotation limit.
	* \return SI_Void
	*/
	SI_Void SetRotationLimitMinimum(CSIBCVector3D &in_rValue);

	/*! Return the solver type of the joint.
	* \return EJointSolverType	Solver type.
	*/
	EJointSolverType GetSolverType();

	/*! Change the solver type of the joint.
	* \param	in_Type	New solver type.
	* \return SI_Void
	*/
	SI_Void SetSolverType(EJointSolverType in_Type);

	/*! Return the stiffness value of the joint.
	* \return SI_Float	Stiffness value.
	*/
	SI_Float GetStiffness();

	/*! Change the stiffness value of the joint.
	* \param	in_fValue	New stiffness value.
	* \return SI_Void
	*/
	SI_Void SetStiffness(SI_Float in_fValue);

	/*! Return the stiffness activation flag of the joint.
	* \return SI_Bool Stiffness activation flag.
	*/
	SI_Bool GetStiffnessActivation();

	/*! Change the stiffness activation flag of the joint.
	* \param	in_bValue New stiffness activation flag.
	* \return SI_Void
	*/
	SI_Void SetStiffnessActivation(SI_Bool in_bValue);

	/*! Synchronizes this IK joint instance with its IO layer representation
	* \return SI_Error
	* \retval SI_SUCCESS this function always returns SI_SUCCESS
	*/
    virtual SI_Error Synchronize();

	/*! Get the template type
	* \return ETemplateType the template type of the joint
	*/
	virtual ETemplateType Type();

	/*! Gets a handle on one of this light's parameters by using its name
	* \param in_szName the name of the parameter to get the handle from
	* \return CSLAnimatableType* the handle to the parameter
	*/
    virtual CSLAnimatableType* ParameterFromName(SI_Char *in_szName);

	/*! Validate that the parent is of valid type
	* \param in_pNewParent the parent to validate against
	* \return SI_Bool whether the parent is valid or not
	*/
	virtual SI_Bool ValidateParent(CSLModel *in_pNewParent);

private:
	CSLStrEnumProxy<EJointSolverType, SI_3D> m_SolverType;
	CSLVector3DProxy m_PreferredRotation;
	CSLBoolProxy m_RotationLimitActivation;
	CSLVector3DProxy m_RotationLimitMinimum;
	CSLVector3DProxy m_RotationLimitMaximum;
	CSLBoolProxy m_PseudoRoot;
	CSLBoolProxy m_StiffnessActivation;
	CSLFloatProxy m_Stiffness;
	CSLFloatProxy m_Length;

	void *m_pReserved;	// reserved for future extension
};

#endif
