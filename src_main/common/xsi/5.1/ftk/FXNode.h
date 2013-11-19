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

#ifndef _FXNODE_H
#define _FXNODE_H

#include "Template.h"

class CSLVariantParameter;
class CSLFXOperatorConnection;

//! Representation of an operator in the FX Tree
class XSIEXPORT CSLFXOperator
    : public CSLTemplate
{
public:

	//! Describes the various output types
	enum EOutputType
	{
		SI_DEFAULT = 0,
	};
	/*! Constructor
	* \param in_pScene Parent scene
	* \param in_pModel Parent model
	* \param in_pTemplate Refered template
	* \return an instance of CSLFCOperator
	*/
    CSLFXOperator(CSLScene* in_pScene, CSLModel *in_pModel, CdotXSITemplate* in_pTemplate);

	//! Destructor
    virtual ~CSLFXOperator();

	/*! Gets the type of this template
	* \return ETemplateType The type of this template: XSI_FXOPERATOR
	*/
	ETemplateType Type(){ return XSI_FXOPERATOR; }

	/*! Commits semantic layer data
	* \return SI_Error Whether or not commit was a success
	*/
	SI_Error Synchronize();


	/*! Gets the OutputType
	* \return SI_Int The OutputType
	*/
	EOutputType GetOutputType();

	/*! Sets the OutputType
	* \param in_OutputType New output type to use
	*/
	SI_Void SetOutputType( EOutputType in_OutputType );

	/*! Gets the EffectID
	* \return SI_Char Pointer to a string representing the EffectID
	*/
	SI_Char* GetEffectID();

	/*! Sets the EffectID
	* \param in_szProgID Pointer to a string representing the EffectID
	*/
	SI_Void SetEffectID( SI_Char* in_szProgID );

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
	* \param in_szParameterName Name of the parameter to look for
	* \return CSLAnimatableType Pointer to the found parameter or null if not found
	*/
    virtual CSLAnimatableType* ParameterFromType(EFCurveType in_Type, SI_Char *in_szParameterName);

	///////////////////////////////////////////////////////////////////////////
	// Connection points functionality ////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////
	
	/*! Add a connection point to the connection point list
	* \return CSLFXOperatorConnection* Pointer to the newly added connection point
	*/
	CSLFXOperatorConnection* AddConnectionPoint();

	/*! Removes a connection point from the connection point list
	* \param in_nIndex Index of the connection point to remove
	* \return SI_Error Whether or not the connection point could be removed
	*/
	SI_Error RemoveConnectionPoint( SI_Int in_nIndex );

	/*! Removes a connection point from the connection point list
	* \param in_pToRemove Pointer to the connection point to remove
	* \return SI_Error Whether or not the connection point could be removed
	*/
	SI_Error RemoveConnectionPoint( CSLFXOperatorConnection* in_pToRemove );

	/*! Removes all connections points from the connection point list
	* \return SI_Error Whether or not the list could be cleared
	*/
	SI_Error ClearConnectionPoints();

	/*! Adds an exisiting connection point to the connection point list
	* \param in_pToConnect Pointer to the connection point to add
	* \return The added connection point or null if it already exists in the list
	*/
	CSLFXOperatorConnection* ConnectConnectionPoint( CSLFXOperatorConnection* in_pToConnect );

	/*! Gets a pointer to the internally-stored connection point pointer list
	* \return CSLConnectionPoint Pointer to the list
	*/
	CSLFXOperatorConnection** GetConnectionPointList();

	/*! Gets the total number of connection points 
	* \return SI_Int Number of connection points
	*/
	SI_Int GetConnectionPointCount();

private:
    CSLStringProxy m_ProgID;
    CSLEnumProxy<EOutputType, SI_DEFAULT> m_OutputType;
	CSIBCArray<CSLFXOperatorConnection*> m_ConnectionPoints;
	CSIBCArray<CSLVariantParameter*> m_Parameters;

	SI_Void *m_pReserved;
};

#endif
