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

#ifndef _XSISHAPE_H
#define _XSISHAPE_H

#include "Template.h"
#include "Scene.h"
#include "Model.h"
#include "XSISubComponentAttributeList.h"



//! XSI Mesh shape class for dotXSI 5.0+
/** 
* Describes a xsi mesh shape
*
* The CSLXSIShape holds a generic sub component attribute list that describes vertex or ploynode attributes.
* It implements helper for the following attributes: vertex positions, multiple normals,
* multiple texture coordinates and multiple colors.
*
*/
class XSIEXPORT CSLXSIShape
	: public CSLTemplate
{
public:
	enum EXSIShapeType
	{
		XSI_ORDERED,	/*!< The shape is of ordered type */
		XSI_INDEXED,	/*!< The shape is of indexed type */
	};

	/*! Constructor	
	* \param in_pScene Parent scene
	* \param in_pModel Parent model
	* \param in_pTemplate Refered template
	* \param in_Type the type of shape to construct
	* \return an instance of CSLShape
	*/
	CSLXSIShape(CSLScene *in_pScene, CSLModel *in_pModel, CdotXSITemplate *in_pTemplate, EXSIShapeType in_Type);

	/*! Destructor
	*/
	virtual ~CSLXSIShape();

	/*!	Add a new sub component attribute list.
	* \return	CSLXSISubComponentAttributeList*	Pointer to the newly created list.
	*/
	CSLXSISubComponentAttributeList*	AddAttributeList(CSLXSISubComponentAttributeList::EAttributeType in_AttributeType);

	/*!	Return the number of sub component attribute lists.
	* \return	SI_Int	Number of sub component attribute lists.
	*/
	SI_Int								GetAttributeListCount();

	/*!	Return the number of sub component attribute lists with the given semantic.
	* \return	SI_Int	Number of sub component attribute lists with the given semantic.
	*/
	SI_Int								GetAttributeListCountBySemantic(SI_Char* in_pSemantic);

	/*!	Return an array of all sub component attribute list.
	* \return	CSLXSISubComponentAttributeList**	Array of pointers to sub component attribute list.
	*/
	CSIBCArray<CSLXSISubComponentAttributeList*>*	AttributeLists();

	/*!	Remove the last sub component attribute list.
	* \return	SI_Error	Error code.
	*/
	SI_Error							RemoveAttributeList();

	/*!	Remove the specified sub component attribute list.
	* \return	SI_Error	Error code.
	*/
	SI_Error	RemoveAttributeList( CSLXSISubComponentAttributeList* in_pAttributeList );

	/*!	Remove the specified sub component attribute list.
	* \return	SI_Error	Error code.
	*/
	SI_Error	RemoveAttributeList( SI_Int in_iIndex );

	/*!	Remove all sub component attribute list.
	* \return	SI_Error	Error code.
	*/
	SI_Error							ClearAttributeLists();

	/*!	Remove all sub component attribute list that has the given semantic.
	* \return	SI_Error	Error code.
	*/
	SI_Error							ClearAttributeListsBySemantic(SI_Char *in_pSemantic);

	/*!	Connect a new sub component attribute list.
	* \return	CSLXSISubComponentAttributeList		The newly connected sub component attribute list.
	*/
	CSLXSISubComponentAttributeList*	ConnectAttributeList( CSLXSISubComponentAttributeList* in_pNewXSISubComponentAttributeList );

	/*!	Get the first sub component attribute list with the corresponding semantic.
	* \return	CSLXSISubComponentAttributeList*	Pointer to the first sub component attribute list with the corresponding semantic.
	*/
	CSLXSISubComponentAttributeList* CSLXSIShape::GetFirstAttributeListBySemantic( SI_Char* in_pSemantic );

	/*!	Get the next sub component attribute list with the corresponding semantic.
	* \return	CSLXSISubComponentAttributeList*	Pointer to the next sub component attribute list with the corresponding semantic.
	*/
	CSLXSISubComponentAttributeList* CSLXSIShape::GetNextAttributeListBySemantic( SI_Char* in_pSemantic );

	//**********************************************************************************
	// Vertex position attribute helper (always only one vertex position attibute list)
	//**********************************************************************************

	/*!	Add a new POSITION attribute list.
	* \return	CSLXSISubComponentAttributeList*	Pointer to the newly created POSITION attribute list.
	*/
	CSLXSISubComponentAttributeList*	AddVertexPositionList();

	/*!	Get the POSITION attribute list.
	* \return	CSLXSISubComponentAttributeList*	Pointer to the POSITION attribute list.
	*/
	CSLXSISubComponentAttributeList*	GetVertexPositionList();

	/*!	Remove the last POSITION attribute list.
	* \return	SI_Error	Error code.
	*/
	SI_Error							RemoveVertexPositionList();

	//**********************************************************************************
	// Normal attribute helper
	//**********************************************************************************

	/*!	Add a new NORMAL attribute list.
	* \return	CSLXSISubComponentAttributeList*	Pointer to the newly created NORMAL attribute list.
	*/
	CSLXSISubComponentAttributeList*	AddNormalList();

	/*!	Get the first NORMAL attribute list.
	* \return	CSLXSISubComponentAttributeList*	Pointer to the first NORMAL attribute list.
	*/
	CSLXSISubComponentAttributeList*	GetFirstNormalList();

	/*!	Get the next NORMAL attribute list.
	* \return	CSLXSISubComponentAttributeList*	Pointer to the next NORMAL attribute list.
	*/
	CSLXSISubComponentAttributeList*	GetNextNormalList();

	/*!	Get the number of NORMAL attribute list.
	* \return	SI_Int								The number of NORMAL attribute list.
	*/
	SI_Int								GetNormalListCount();

	/*!	Remove the last NORMAL attribute list.
	* \return	SI_Error	Error code.
	*/
	SI_Error							RemoveNormalList();

	/*!	Clear all the NORMAL attribute list.
	* \return	SI_Error	Error code.
	*/
	SI_Error							ClearNormalLists();

	//**********************************************************************************
	// Color attribute helper
	//**********************************************************************************

	/*!	Add a new COLOR attribute list.
	* \return	CSLXSISubComponentAttributeList*	Pointer to the newly created COLOR attribute list.
	*/
	CSLXSISubComponentAttributeList*	AddColorList();

	/*!	Get the first COLOR attribute list.
	* \return	CSLXSISubComponentAttributeList*	Pointer to the first COLOR attribute list.
	*/
	CSLXSISubComponentAttributeList*	GetFirstColorList();

	/*!	Get the next COLOR attribute list.
	* \return	CSLXSISubComponentAttributeList*	Pointer to the next COLOR attribute list.
	*/
	CSLXSISubComponentAttributeList*	GetNextColorList();

	/*!	Get the number of COLOR attribute list.
	* \return	SI_Int								The number of COLOR attribute list.
	*/
	SI_Int								GetColorListCount();

	/*!	Remove the last COLOR attribute list.
	* \return	SI_Error	Error code.
	*/
	SI_Error							RemoveColorList();

	/*!	Clear all the COLOR attribute list.
	* \return	SI_Error	Error code.
	*/
	SI_Error							ClearColorLists();

	//**********************************************************************************
	// TexCoord attribute helper
	//**********************************************************************************

	/*!	Add a new TEXCOORD attribute list.
	* \return	CSLXSISubComponentAttributeList*	Pointer to the newly created TEXCOORD attribute list.
	*/
	CSLXSISubComponentAttributeList*	AddTexCoordList();

	/*!	Get the first TEXCOORD attribute list.
	* \return	CSLXSISubComponentAttributeList*	Pointer to the first TEXCOORD attribute list.
	*/
	CSLXSISubComponentAttributeList*	GetFirstTexCoordList();

	/*!	Get the next TEXCOORD attribute list.
	* \return	CSLXSISubComponentAttributeList*	Pointer to the next TEXCOORD attribute list.
	*/
	CSLXSISubComponentAttributeList*	GetNextTexCoordList();

	/*!	Get the number of TEXCOORD attribute list.
	* \return	SI_Int								The number of TEXCOORD attribute list.
	*/
	SI_Int								GetTexCoordListCount();

	/*!	Remove the last TEXCOORD attribute list.
	* \return	SI_Error	Error code.
	*/
	SI_Error							RemoveTexCoordList();

	/*!	Clear all the TEXCOORD attribute list.
	* \return	SI_Error	Error code.
	*/
	SI_Error							ClearTexCoordLists();

	//**********************************************************************************
	// WeightMap attribute helper
	//**********************************************************************************

	/*!	Add a new WEIGHTMAP attribute list.
	* \return	CSLXSISubComponentAttributeList*	Pointer to the newly created WEIGHTMAP attribute list.
	*/
	CSLXSISubComponentAttributeList*	AddWeightMapList();

	/*!	Get the first WEIGHTMAP attribute list.
	* \return	CSLXSISubComponentAttributeList*	Pointer to the first WEIGHTMAP attribute list.
	*/
	CSLXSISubComponentAttributeList*	GetFirstWeightMapList();

	/*!	Get the next WEIGHTMAP attribute list.
	* \return	CSLXSISubComponentAttributeList*	Pointer to the next WEIGHTMAP attribute list.
	*/
	CSLXSISubComponentAttributeList*	GetNextWeightMapList();

	/*!	Get the number of WEIGHTMAP attribute list.
	* \return	SI_Int								The number of WEIGHTMAP attribute list.
	*/
	SI_Int								GetWeightMapListCount();

	/*!	Remove the last WEIGHTMAP attribute list.
	* \return	SI_Error	Error code.
	*/
	SI_Error							RemoveWeightMapList();

	/*!	Clear all the WEIGHTMAP attribute list.
	* \return	SI_Error	Error code.
	*/
	SI_Error							ClearWeightMapLists();

	/*! Get the template type
	* \return ETemplateType the template type of the shape surface (XSI_SHAPE)
	*/
	virtual ETemplateType				Type() { return XSI_SHAPE; };

	/*! Get the shape type
	* \return EXSIShapeType the type of the shape surface (XSI_ORDERED or XSI_INDEXED)
	*/
	EXSIShapeType						GetShapeType() { return m_eShapeType; }

	/*! Commit the current template information
		\return SI_Error Whether or not the commit was successful
	*/
	SI_Error Synchronize();

private:
	CSIBCArray<CSLXSISubComponentAttributeList *>	m_AttributeLists;
	SI_Int											m_iCurrentAttributeListIndex;

	EXSIShapeType									m_eShapeType;

	void *m_pReserved;	// reserved for future extension
};

#endif
