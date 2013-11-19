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

#ifndef _IKEFFECTOR_H
#define _IKEFFECTOR_H

#include "IK.h"


//! Describes the effector of an IK chain.
class XSIEXPORT CSLIKEffector
	: public CSLIK
{
public:

	/*! Constructor
	* \param in_pScene	Pointer to the scene containing the effector
	* \param in_pModel	Pointer to the model containing the effector
	* \param in_pTemplate	Pointer to the CdotXSITemplate containing the effector
	* \return an instance of CSLIKEffector
	*/
	CSLIKEffector(CSLScene* in_pScene, CSLModel *in_pModel, CdotXSITemplate* in_pTemplate, CSLModel* in_pRoot);

	/*! Destructor
	*/
	virtual ~CSLIKEffector();

	/*! Return the inherit rotation flag of the effector.
	* \return SI_Bool Inherit rotation flag.
	*/
	SI_Bool GetRotationFlag();

	/*! Change the inherit rotation flag of the joint.
	* \param	in_bValue New inherit rotation flag.
	* \return SI_Void
	*/
	SI_Void SetRotationFlag(SI_Bool in_bValue);

	/*! Synchronizes this IK effector instance with its IO layer representation
	* \return SI_Error
	* \retval SI_SUCCESS this function always returns SI_SUCCESS
	*/
    virtual SI_Error Synchronize();

	/*! Get the template type
	* \return ETemplateType the template type of the effector
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
	CSLIntProxy m_RotationFlag;

	void *m_pReserved;	// reserved for future extension
};

#endif
