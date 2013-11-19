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

#ifndef _SHADER_INSTANCE_DATA_H
#define _SHADER_INSTANCE_DATA_H

#include "Template.h"

class CSLVariantParameter;

//! Defines how a shader is shared beetween models
class XSIEXPORT CSLShaderInstanceData
    : public CSLTemplate
{
public:

	/*! Constructor	
	* \param in_pScene Parent scene
	* \param in_pModel Parent model
	* \param in_pTemplate Refered template
	* \return an instance of CSLShaderInstanceData
	*/
	CSLShaderInstanceData
	(
		CSLScene* in_pScene, 
		CSLModel *in_pModel, 
		CdotXSITemplate* in_pTemplate
	);

	/*! Destructor
	*/
	~CSLShaderInstanceData();

	//! Returns the type of this template
	/*!
	* \return ETemplateType The type of this template (XSI_SHADER_INSTANCE_DATA)
	*/
    inline ETemplateType Type();

	/*! Commits semantic layer data to the dotXSI template
	* \return SI_Error Whether or not data was commited correctly
	*/
	SI_Error Synchronize();

	// Reference Functionality ////////////////////////////////////////////////////
	
	//! Get the referenced Model
	/*!
	* \return CSLModel Pointer to referenced Model or null if there's none
	*/
    CSLModel* GetReference();

	//! Set the referenced Model
	/*!
	* \param in_pReferedModel Pointer to refered Model.
	*/
	SI_Void SetReference( CSLModel* in_pReferedModel);


	// Parameters Functionality ///////////////////////////////////////////////

	/*! Add a parameter to the parameter list
	* \return CSLParameter Pointer to the newly added parameter
	*/
	CSLVariantParameter* AddParameter();

	/*! Removes a parameter from the parameter list
	* \param in_nIndex Index of the parameter to remove
	* \return SI_Error Whether or not the parameter could be removed
	*/
	SI_Error RemoveParameter( SI_Int in_nIndex );

	/*! Removes a parameter from the parameter list
	* \param in_pToRemove Pointer to the parameter to remove
	* \return SI_Error Whether or not the parameter could be removed
	*/
	SI_Error RemoveParameter( CSLVariantParameter* in_pToRemove );

	/*! Removes all parameters from the parameter list
	* \return SI_Error Whether or not the list could be cleared
	*/
	SI_Error ClearParameters();

	/*! Adds an exisiting parameter to the parameter list
	* \param in_pToConnect Pointer to the parameter to add
	* \return The added parameter or null if it already exists in the list
	*/
	CSLVariantParameter* ConnectParameter( CSLVariantParameter* in_pToConnect );

	/*! Gets a pointer to the internally-stored parameter pointer list
	* \return CSLParameter Pointer to the list
	*/
	CSLVariantParameter** GetParameterList();

	/*! Gets the total number of parameters 
	* \return SI_Int Number of parameters
	*/
	SI_Int GetParameterCount();

	/*! Searches for a parameter by name
	* \param in_szName Name of the parameter to look for
	* \return CSLAnimatableType* Pointer to the found parameter or null if not found
	*/
	virtual CSLAnimatableType* ParameterFromName(SI_Char *in_szName);

	/*! Searches for a parameter by name and type
	* \param in_Type Type of the parameter to look for
	* \param in_szParameterName Name of the parameter to look for
	* \return CSLAnimatableType* Pointer to the found parameter or null if not found
	*/
	CSLAnimatableType* ParameterFromType(EFCurveType in_Type, SI_Char *in_szParameterName);

private:
	CSLModel *m_pReferedModel;
	CSIBCArray<CSLVariantParameter *> m_Parameters;

	SI_Void *m_pReserved;	// reserved for future extension
};


#endif // _SHADER_INSTANCE_DATA_H
