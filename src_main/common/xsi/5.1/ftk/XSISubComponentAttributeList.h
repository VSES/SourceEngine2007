//***************************************************************************************
//
// File supervisor: Softimage 3D Games & 3D Bridge team
//
// (c) Copyright 2001-2005 Avid Technology, Inc. . All rights reserved.
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

#ifndef _XSISUBCOMPONENTATTRIBUTELIST_H
#define _XSISUBCOMPONENTATTRIBUTELIST_H

#include "Template.h"


//! Sub-component attribute list class for dotXSI 5.0+
/** 
* Describes a sub-component attribute list
*
* The CSLXSISubComponentAttributeList holds a list of values for a specific attribute of a given type and semantic.
*
* This class allows an attribute (position, normal, color, texcoord, etc.) to be associated with vertices or polynodes.
*/
class XSIEXPORT CSLXSISubComponentAttributeList
	: public CSLTemplate
{
public:
	/**
	* Proxy class that allows modifying a list of SI_Int like an array.
	*/
	typedef CSLArrayProxy<SI_Float, SI_Float, 1> CSLFloatArray;

	enum EAttributeType
	{
		FLOAT = 1,
		FLOAT2 = 2,
		FLOAT3 = 3,
		FLOAT4 = 4,
		FLOAT5 = 5,
	};

	/*! Constructor	
	* \param in_pScene Parent scene
	* \param in_pModel Parent model
	* \param in_pTemplate Refered template
	* \param in_Type the type of attribute to construct
	* \return an instance of CSLXSISubComponentAttributeList
	*/
	CSLXSISubComponentAttributeList(CSLScene *in_pScene, CSLModel *in_pModel, CdotXSITemplate *in_pTemplate);

	/*! Destructor
	*/
	virtual ~CSLXSISubComponentAttributeList();

	/*! Get the number of values in the attribute list.
	* \return	SI_Int	Number of attribute values.
	*/
	SI_Int							GetCount();

	/*! Get the semantic of the attribute list.
	* \return	SI_Char*	The semantic.
	*/
	SI_Char*						GetSemantic() { return m_Semantic; }

	/*! Set the semantic of the attribute list.
	* \param	SI_Char*	The semantic.
	* \return	None
	*/
	SI_Void 						SetSemantic(SI_Char* in_Semantic) { m_Semantic = in_Semantic; }

	/*! Get the type of the attribute value.
	* \return	EAttributeType	Type of the attribute value.
	*/
	EAttributeType					GetAttributeType() { return m_AttributeType; }

	/*! Set the type of the attribute value.
	* \param	EAttributeType	Type of the attribute value.
	* \return	None
	*/
	SI_Void							SetAttributeType(EAttributeType in_AttributeType);

	/*! Get the the attribute value array so that the values can be edited.
	* \return	CSLFloatArray*	The attribute value array.
	*/
	CSLFloatArray*					GetAttributeArray() { return &m_AttributeArray; }

	/*! Get the template type
	* \return ETemplateType the template type of the sub-component attribute list (XSI_SUB_COMPONENT_ATTRIBUTE_LIST)
	*/
	virtual ETemplateType			Type() { return XSI_SUB_COMPONENT_ATTRIBUTE_LIST; }

	/*! Commit the current template information
		\return SI_Error Whether or not the commit was successful
	*/
	SI_Error Synchronize();

private:
	CSLFloatArray					m_AttributeArray;
	CSLStringProxy					m_Semantic;
	EAttributeType					m_AttributeType;

	void *m_pReserved;	// reserved for future extension
};

#endif
