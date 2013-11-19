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

#ifndef _CUSTOMPSET_H
#define _CUSTOMPSET_H

#include "Template.h"

// Forward declaration
class CSLVariantParameter;
class CSLXSICustomParamInfo;

//! Class representing customer parameter sets
class XSIEXPORT CSLCustomPSet
    : public CSLTemplate
{
public:
	/*! Constructor	
	* \param in_pScene Parent scene
	* \param in_pModel Parent model
	* \param in_pTemplate Refered template
	* \return an instance of CSLCustomPSet
	*/
    CSLCustomPSet(CSLScene* in_pScene, CSLModel *in_pModel, CdotXSITemplate* in_pTemplate);

	//! Destructor
    virtual ~CSLCustomPSet();

	/*! Commit the current template information
	* \return SI_Error Whether or not the commit was successful
	*/
    virtual SI_Error Synchronize();

	/*! Returns the type of this template (SI_CUSTOM_PSET)
	* \return CSLTemplate::ETemplateType The type of this template (SI_CUSTOM_PSET)
	*/
    virtual ETemplateType Type() { return SI_CUSTOM_PSET; }

	//! Defines the various propagation types
	enum EPropagationType
	{
		SI_BRANCH,		/*!< Branch propagation */
		SI_NODE,		/*!< Node propagation */
		SI_INHERITED	/*!< Inherited propagation */
	};

	//! Get the propagation type
	/*!
	* \return EPropagationType The propagation type
	*/
	EPropagationType GetPropagationType();

	//! Set the propagation type
	/*!
	* \param in_PropagationType New type of propagation tp use
	* \return SI_Void
	*/
	SI_Void SetPropagationType( EPropagationType in_PropagationType );

	///////////////////////////////////////////////////////////////////////////
	// Parameters Functionality ///////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

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

	/*! Removes all connections points from the parameter list
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
	* \return CSLAnimatableType Pointer to the found parameter or null if not found
	*/
	virtual CSLAnimatableType* ParameterFromName(SI_Char *in_szName);

	/*! Searches for a parameter by name and type
	* \param in_Type Type of the parameter to look for
	* \param in_szName Name of the parameter to look for
	* \return CSLAnimatableType Pointer to the found parameter or null if not found
	*/
    virtual CSLAnimatableType* ParameterFromType(EFCurveType in_Type, SI_Char *in_szParameterName);

	///////////////////////////////////////////////////////////////////////////
	// Custom parameter information
	///////////////////////////////////////////////////////////////////////////

	/*! Creates a new custom parameter information
	* \param in_nIndex Index of the parameter where to attach the new custom parameter information
	* \return CSLXSICustomParamInfo Pointer to the new custom parameter information or null creation could not be done
	* \note Introduced in 3.6
	*/
	CSLXSICustomParamInfo* CreateCustomParamInfo( SI_Int in_nIndex );

	/*! Connects custom parameter information
	* \param in_pToConnect Pointer to the custom parameter information
	* \return CSLXSICustomParamInfo Pointer to the newly connected parameter information or null if the refered parameter does not exist or if there's already a custom parameter information
	* \note Introduced in 3.6
	*/
	CSLXSICustomParamInfo* ConnectCustomParamInfo( CSLXSICustomParamInfo *in_pToConnect );

	/*! Gets the custom parameter information type
	* \return char* NULL if there is no type. NULL if the file is < 5.0
	* \note Introduced in 5.0
	*/
	char *GetType();

	/*! Sets the custom parameter information type. Note, this only works with 5.0 files
	* \param in_pType The new type
	* \return void
	* \note Introduced in 5.0
	*/
	void SetType(char *in_pType);

private:
	CSLStrEnumProxy<EPropagationType, SI_INHERITED>  m_PropagationType;
    CSIBCArray<CSLVariantParameter*> m_Parameters; 

	SI_Void *m_pReserved;	// Reserved for future use
};

#endif
