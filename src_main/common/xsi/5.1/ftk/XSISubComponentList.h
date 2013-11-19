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

#ifndef _XSISUBCOMPONENTLIST_H
#define _XSISUBCOMPONENTLIST_H

#include "Template.h"

//! Sub-component list base class for dotXSI 5.0+
/** 
* Describes a sub-component list base class
*
* The CSLXSISubComponentList base class holds a list of attribute reference. Each attributes list are
* reference by name and a corresponding indices array matche the sub component node with the 
* attribute value.
*
* This class allows an element composed of sub-component (vertex list, polygon list, etc.) to reference
* to attrubutes (position, normal, colors, etc.) for its nodes, by name. A corresponding indices array
* is associated to each attribute reference to match the sub component node with the attribute value.
*/
class XSIEXPORT CSLXSISubComponentList
	: public CSLTemplate
{
public:
	/**
	* Proxy class that allows modifying a list of SI_Int like an array.
	*/
	typedef CSLArrayProxy<SI_Int, SI_Int, 1> CSLIntArray;

	/**
	* Proxy class that allows modifying a list of SI_Char* like an array.
	*/
	typedef CSLArrayProxy<SI_Char*, SI_Char*, 1> CSLStringArray;

	/*! Constructor	
	* \param in_pScene Parent scene
	* \param in_pModel Parent model
	* \param in_pTemplate Refered template
	* \return an instance of CSLXSISubComponentList
	*/
	CSLXSISubComponentList(CSLScene *in_pScene, CSLModel *in_pModel, CdotXSITemplate *in_pTemplate);

	/*! Destructor
	*/
	virtual ~CSLXSISubComponentList();

	/*! Get the number of node (polynode/vertex/etc.) in this sub component list.
	* \return	SI_Int	Number of sub component node.
	*/
	SI_Int								GetCount();

	/*! Set the number of node (polynode/vertex/etc.) in this sub component list.
	* \return	SI_Error	Error code.
	*/
	virtual SI_Error					SetCount(SI_Int);

	/*!	Get the number of attribute accessor.
	* \return	SI_Int	Number of attribute.
	*/
	SI_Int								GetAttributeCount();

	/*! Create a new attribute accessor element.
	* \return	SI_Int	The index of the newly created attribute elements.
	*/
	SI_Int								AddAttribute(SI_Char*);

	/*!	Remove the last attribute accessor.
	* \return	SI_Error	Error code.
	*/
	SI_Error							RemoveAttribute();

	/*!	Remove all the attribute accessors.
	* \return	SI_Error	Error code.
	*/
	SI_Error							ClearAttribute();

	/*! Get the attribute indices.
	* \return	CSLIntArray*	Attribute indices.
	*/
	CSLIntArray*						GetAttributeIndices(SI_Int in_iIndex);

	/*! Get the attribute indices array.
	* \return	CSIBCArray<CSLIntArray>*	Attribute indices array.
	*/
	CSIBCArray<CSLIntArray*>*			GetAttributeIndicesArray();

	/*! Get the attribute name array.
	* \return	CSIBCArray<CSLCharArray>*	Attribute name array.
	*/
	CSLStringArray*						GetAttributeNameArray();

	/*! Get the index of the first attribute with the specified name from the array.
	* \return	CSLArrayProxy<SI_Int, SI_Int, 1>*		The attribute indices array of the attibute with the specified name.
	*/
	CSLArrayProxy<SI_Int, SI_Int, 1>*	GetAttributeByName(SI_Char* in_pName);

private:
	SI_Int	m_iCount; // number of node

	CSIBCArray<CSLIntArray*>	m_AttributeIndicesArray;
	CSLStringArray			m_AttributeNameArray;

	void *m_pReserved;	// reserved for future extension
};

#endif
