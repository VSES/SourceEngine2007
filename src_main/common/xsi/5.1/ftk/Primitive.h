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

#ifndef _PRIMITIVE_H
#define _PRIMITIVE_H

#include "Template.h"

//! Base class for primitive objects
class XSIEXPORT CSLPrimitive
    : public CSLTemplate
{
public:

	/*! Destructor
	*/
    virtual ~CSLPrimitive();

	/*! Validate a parent for this primitive
	* \param in_pNewParent the parent to validate
	* \return SI_Bool whether the parent is valid or not
	*/
	virtual SI_Bool ValidateParent(CSLModel *in_pNewParent);

protected:

	/*! Constructor
	* \param in_pScene	Pointer to the scene containing the Primitive
	* \param in_pModel	Pointer to the model containing the Primitive
	* \param in_pTemplate	Pointer to the CdotXSITemplate containing the Primitive
	* \return an instance of CSLPrimitive
	*/
    CSLPrimitive(CSLScene* in_pScene, CSLModel *in_pModel, CdotXSITemplate* in_pTemplate);
};

#endif
