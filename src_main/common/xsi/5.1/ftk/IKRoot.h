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

#ifndef _IKROOT_H
#define _IKROOT_H

#include "IK.h"

// Forward declaration
class CSLIKJoint;
class CSLIKEffector;


//! Describes the root of an IK chain.
class XSIEXPORT CSLIKRoot
	: public CSLIK
{
public:
	/*! Constructor
	* \param in_pScene	Pointer to the scene containing the root
	* \param in_pModel	Pointer to the model containing the root
	* \param in_pTemplate	Pointer to the CdotXSITemplate containing the root
	* \return an instance of CSLIKRoot
	*/
	CSLIKRoot(CSLScene* in_pScene, CSLModel *in_pModel, CdotXSITemplate* in_pTemplate);

	/*! Destructor
	*/
	virtual ~CSLIKRoot();

	/*!	Return the effector of the IK chain.
	* \return	CSLTexture2D*	Pointer to a texture 2D object.
	*/
	CSLModel* GetEffector();

	/*!	Return an array of all joints in the IK chain.
	* \return	CSLIKKoint**	Array of pointers to IK joint objects.
	*/
	CSLModel** GetJointList();

	/*!	Return the number of joints in the IK chain.
	* \return	SI_Int	Number of joint.
	*/
	SI_Int GetJointCount();

	/*! Connects a joint to this IK root
	* \param in_pNewJoint the joint to connect
	* \return CSLModel* the joint to connect
	*/
	CSLModel* ConnectJoint(CSLModel* in_pNewJoint);

	/*! Connects an effector to this IK root
	* \param in_pNewEffector the effector to connect
	* \return CSLModel* the effector to connect
	*/
	CSLModel* ConnectEffector(CSLModel* in_pNewEffector);

	/*! Synchronizes this IK root instance with its IO layer representation
	* \return SI_Error
	* \retval SI_SUCCESS this function always returns SI_SUCCESS
	*/
    virtual SI_Error Synchronize();

	/*! Get the template type
	* \return ETemplateType the template type of the IK root
	*/
	virtual ETemplateType Type();


private:
	CSIBCArray<CSLModel *> m_Joints;
	CSLModel* m_pEffector;

	void *m_pReserved;	// reserved for future extension
};

#endif
