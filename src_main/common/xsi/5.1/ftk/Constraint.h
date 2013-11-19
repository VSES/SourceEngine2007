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

#ifndef _CONSTRAINT_H
#define _CONSTRAINT_H

#include "Template.h"

// Forward declaration
class CSLModel;


//! Describes a constraint.
class XSIEXPORT CSLConstraint
	: public CSLTemplate
{
public:
	/** 
	* The EConstraintType enum contains possible types for a constraint
	*/
	enum EConstraintType
	{
		SI_SCALING,			//!<Scaling constraint
		SI_ORIENTATION,		//!<Orientation constraint
		SI_DIRECTION,		//!<Direction constraint
		SI_POSITION,		//!<Position constraint
		SI_INTEREST,		//!<Interest constraint
		SI_UP_VECTOR,		//!<Up Vector constraint
		SI_PREFERED_AXIS,	//!<Prefered Axis constraint
	};

	/*! Constructor
	* \param in_pScene	Pointer to the scene containing the constraint
	* \param in_pModel	Pointer to the model containing the constraint
	* \param in_pTemplate	Pointer to the CdotXSITemplate containing the constraint
	* \return an instance of CSLConstraint
	*/
	CSLConstraint(CSLScene* in_pScene, CSLModel *in_pModel, CdotXSITemplate* in_pTemplate, EConstraintType in_ConstraintType);

	/*! Destructor
	*/
	virtual ~CSLConstraint();

	/*!	Return the reference model of the constraint.
	* \return	CSLModel*	Pointer to the reference model.
	*/
	CSLModel*				GetReference();

	/*!	Return the type of the constraint.
	* \return	EConstraintType	Type of constraint.
	*/
	EConstraintType			GetConstraintType();

	/*!	Add a new constraining object to the constraint.
	* \param	in_pConstrainingObject	Pointer to the constraining object.
	* \return	SI_Error	Error code.
	*/
	SI_Error				AddConstrainingObject(CSLModel* in_pConstrainingObject);

	/*!	Return the number of constraining objects in the constraint.
	* \return	SI_Int	Number of constraining objects.
	*/
	SI_Int					GetConstrainingObjectCount();

	/*!	Return an array of all constraining objects in the constraint.
	* \return	CSLBaseMaterial**	Array of pointers to model objects.
	*/
	CSLModel**				ConstrainingObjects();

	/*!	Remove a single constraining object from the constraint.
	* \param	in_nIndex	Index of the constraining object to remove.
	* \return	SI_Error	Error code.
	*/
	SI_Error				RemoveConstrainingObject(SI_Int in_nIndex);

	/*!	Remove a single constraining object from the constraint.
	* \param	in_pConstrainingObject	Pointer to the constraining object (model) to remove.
	* \return	SI_Error	Error code.
	*/
	SI_Error				RemoveConstrainingObject(CSLModel* in_pConstrainingObject);

	/*!	Remove all constraining objects from the constraint.
	* \return	SI_Error	Error code.
	*/
	SI_Error				ClearConstrainingObjects();

	/*!	Create a up vector constraint on the constraint.
	* \return	CSLConstraint*	Newly creates constraint object.
	*/
	CSLConstraint*			CreateUpVector();

	/*!	Return the up vector constraint of the constraint.
	* \return	CSLConstraint*	Pointer to a constraint object.
	*/
	CSLConstraint*			UpVector();

	/*!	Destroy the up vector constraint of the constraint.
	* \return	SI_Error	Error code.
	*/
	SI_Error				DestroyUpVector();

	// Connection functions
	/*! Connects with an up vector constraint
	* \param in_pUpVector the up vector constraint to connect to
	* \return CSLConstraint* the up vector constraint
	*/
	CSLConstraint*			ConnectUpVector(CSLConstraint* in_pUpVector);

	/*! Connects with a constraining object
	* \param in_pModel the constraining object to connect to
	* \return CSLModel* the constraining model
	*/
	CSLModel*				ConnectConstrainingObject(CSLModel* in_pModel);

	/*! Synchronizes this constraint instance with its IO layer representation
	* \return SI_Error
	* \retval SI_SUCCESS this function always returns SI_SUCCESS
	*/
    virtual SI_Error		Synchronize();

	/*! Get the template type
	* \return ETemplateType the template type of the constraint
	*/
	virtual ETemplateType	Type() { return CSLTemplate::SI_CONSTRAINT; }

private:
	CSLStrEnumProxy<EConstraintType, SI_PREFERED_AXIS>	m_ConstraintType;
	CSIBCArray<CSLModel *>	m_ConstrainingModels;
	CSLConstraint*			m_pUpVector;

	void *m_pReserved;	// reserved for future extension
};

#endif
