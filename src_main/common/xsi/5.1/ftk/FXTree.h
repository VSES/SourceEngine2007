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

#ifndef _FXTREE_H
#define _FXTREE_H

#include "Template.h"

class CSLFXOperator;

//! Representation of an XSI FX Tree
class XSIEXPORT CSLFXTree
    : public CSLTemplate
{
public:

	/*! Constructor
	* \param in_pScene Parent scene
	* \param in_pModel Parent model
	* \param in_pTemplate Refered template
	* \return an instance of CSLFXTree
	*/
    CSLFXTree(CSLScene* in_pScene, CSLModel *in_pModel, CdotXSITemplate* in_pTemplate);

	//! Destructor
    virtual ~CSLFXTree();

	/*! Commits semantic layer data
	* \return SI_Error Whether or not commit was a success
	*/
	SI_Error Synchronize();

	/*! Gets the type of this template
	* \return ETemplateType The type of this template: XSI_FXTREE
	*/
	ETemplateType Type(){ return XSI_FXTREE; }

	/*! Gets a parameter handle by name
	* \param in_szName the name of the parameter to get
	* \return CSLAnimatableType* the found parameter
	*/
    virtual CSLAnimatableType*		ParameterFromName(SI_Char *in_szName);

	///////////////////////////////////////////////////////////////////////////
	// FXOperators Functionality //////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	/*! Add a FXOperator to the FXOperator list
	* \return CSLFXOperator Pointer to the newly added FXOperator
	*/
	CSLFXOperator* AddFXOperator();

	/*! Removes a FXOperator from the FXOperator list
	* \param in_nIndex Index of the FXOperator to remove
	* \return SI_Error Whether or not the FXOperator could be removed
	*/
	SI_Error RemoveFXOperator( SI_Int in_nIndex );

	/*! Removes a FXOperator from the FXOperator list
	* \param in_pToRemove Pointer to the FXOperator to remove
	* \return SI_Error Whether or not the FXOperator could be removed
	*/
	SI_Error RemoveFXOperator( CSLFXOperator* in_pToRemove );

	/*! Removes all operators from the FXOperator list
	* \return SI_Error Whether or not the list could be cleared
	*/
	SI_Error ClearFXOperators();

	/*! Adds an exisiting FXOperator to the FXOperator list
	* \param in_pToConnect Pointer to the FXOperator to add
	* \return The added FXOperator or null if it already exists in the list
	*/
	CSLFXOperator* ConnectFXOperator( CSLFXOperator* in_pToConnect );

	/*! Gets a pointer to the internally-stored FXOperator pointer list
	* \return CSLFXOperator Pointer to the list
	*/
	CSLFXOperator** GetFXOperatorList();

	/*! Gets the total number of FXOperators 
	* \return SI_Int Number of FXOperators
	*/
	SI_Int GetFXOperatorCount();

	/*! Searches for a FXOperator by name
	* \param in_szName Name of the FXOperator to look for
	* \return CSLFXOperator Pointer to the found FXOperator or null if not found
	*/
	virtual CSLFXOperator* FindFXOperator(SI_Char *in_szName);

private:
    CSIBCArray<CSLFXOperator*> m_FXOperators;

	SI_Void *m_pReserved;
};

#endif
