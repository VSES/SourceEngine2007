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

#ifndef _GLOBALMATERIAL_H
#define _GLOBALMATERIAL_H

#include "Template.h"

// Forward declaration
class CSLBaseMaterial;

//! Identifies the global material of a model
class XSIEXPORT CSLGlobalMaterial
    : public CSLTemplate
{
public:
	//! Defines the various propagation types
	enum EPropagationType
	{
		SI_BRANCH,		/*!< Branch propagation */
		SI_NODE,		/*!< Node propagation */
		SI_INHERITED	/*!< Inherited propagation */
	};

    //! Constructor
	/**
	* \param in_pScene Parent Scene
	* \param in_pModel Parent Model
	* \param in_pTemplate Parent Template
	* \return an instance of CSLGlobalMaterial		
	* \note If the file contains an invalid propagation type, NODE is assumed.
	*/
	CSLGlobalMaterial
	(
		CSLScene* in_pScene, 
		CSLModel *in_pModel, 
		CdotXSITemplate* in_pTemplate
	);

	//! Destructor
    ~CSLGlobalMaterial();

	//! Get the referenced material
	/*!
	* \return CSLBaseMaterial Pointer to referenced material or null if there's none
	*/
    CSLBaseMaterial* GetMaterial();

	//! Set the referenced material
	/*!
	* \param in_pNewMaterial Pointer to new refered material.
	*/
	SI_Void SetMaterial( CSLBaseMaterial*  in_pNewMaterial);

    //! Get the propagation type
	/*!
	* \return EPropagationType The propagation type
	*/
	EPropagationType GetPropagationType();

	//! Set the propagation type
	/*!
	* \param in_PropagationType New type of propagation tp use
	*/
	SI_Void SetPropagationType( EPropagationType in_PropagationType );

	//! Returns the type of this template (GLOBAL_MATERIAL)
	/*!
		CSLTemplate::ETemplateType The type of this template (GLOBAL_MATERIAL)
	*/
	CSLTemplate::ETemplateType Type();

	//! Synchronize with the parent template data
	/*!
	* \return SI_Error Whether or not data could be updated
	*/
	SI_Error Synchronize();

private:
    CSLBaseMaterial* m_pMaterial;
    CSLStrEnumProxy<EPropagationType, SI_INHERITED>  m_PropagationType;

	// reserved for future use
	SI_Void *m_pReserved;
};

#endif
