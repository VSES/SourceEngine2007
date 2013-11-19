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

#ifndef _INSTANCE_H
#define _INSTANCE_H

#include "Primitive.h"



//! Describes an instance primitive..
class XSIEXPORT CSLInstance
	: public CSLPrimitive
{
public:
	/*! Constructor
	* \param in_pScene	Pointer to the scene containing the instance
	* \param in_pModel	Pointer to the model containing the instance
	* \param in_pTemplate	Pointer to the CdotXSITemplate containing the instance
	* \return an instance of CSLInstance
	*/
	CSLInstance(CSLScene* in_pScene, CSLModel *in_pModel, CdotXSITemplate* in_pTemplate);

	/*! Destructor
	*/
	virtual ~CSLInstance();

	/*!	Return the reference model of the instance.
	* \return	CSLModel*	Pointer to the reference model.
	*/
	CSLModel* GetReference();

	/*!	Change the reference model object of the instance.
	* \param	in_pModel	New reference model object.
	* \return	SI_Void
	*/
	SI_Void SetReference(CSLModel* in_pModel);

	/*! Synchronizes this instance with its IO layer representation
	* \return SI_Error
	* \retval SI_SUCCESS this function always returns SI_SUCCESS
	*/
    virtual SI_Error Synchronize();

	/*! Get the template type
	* \return ETemplateType the template type of the instance
	*/
    virtual ETemplateType Type() { return SI_INSTANCE; }

private:
	CSLModel* m_pReference;

	void *m_pReserved;	// reserved for future extension
};

#endif
