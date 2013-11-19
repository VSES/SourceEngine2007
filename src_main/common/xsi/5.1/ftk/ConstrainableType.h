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

#ifndef _CONSTRAINABLETYPE_H
#define _CONSTRAINABLETYPE_H

#include "Constraint.h"

/*! Base classe for all templates supporting constraints
*/
class XSIEXPORT CSLConstrainableType
{
public:
	/*! Add a constraint to the constraint list
		\return CSLConstraint Pointer to the newly added constraint
	*/
	virtual CSLConstraint* AddConstraint(CSLConstraint::EConstraintType in_ConstraintType) = 0;

	/*! Removes a constraint from the constraint list
		\param in_nIndex Index of the constraint to remove
		\return SI_Error Whether or not the constraint could be removed
	*/
    SI_Error RemoveConstraint( SI_Int in_nIndex );
	
	/*! Removes a constraint from the constraint list
		\param in_pToRemove Pointer of the constraint to remove
		\return SI_Error Whether or not the constraint could be removed
	*/
    SI_Error RemoveConstraint( CSLConstraint *in_pToRemove );
	
	/*! Gets the total number of constraints in the constraints list
		\return Number of constraints
	*/
    SI_Int GetConstraintCount();

	/*! Gets a pointer to the internally-stored constraint pointer list
		\return CSLConstraint Pointer to the list
	*/
	CSLConstraint** GetConstraintList();

	/*! Removes all constraints from the internally-stored list and deallocate them
		\return SI_Error Whether or not removal was a success
	*/
	SI_Error ClearConstraints();

	/*! Adds a reference to an existing constraint
		\param in_pNewCluster Pointer to the new object to refer to
		\return Pointer to the new reference
	*/
	CSLConstraint* ConnectConstraint( CSLConstraint* in_pNewConstraint );

	// overloaded methods /////////////////////////////////////////////////////
	SI_Error Synchronize();

protected:
	CSLConstrainableType(CSLScene* in_pScene);
	virtual ~CSLConstrainableType();

private:
	CSIBCArray<CSLConstraint*> m_Constraints;
	CSLScene *m_pScene;
};


#endif // _CONSTRAINABLETYPE_H

