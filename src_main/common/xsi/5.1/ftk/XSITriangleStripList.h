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

#ifndef _XSITRIANGLESTRIPLIST_H
#define _XSITRIANGLESTRIPLIST_H

#include "XSISubComponentList.h"
#include "Material.h"

//!Describes a list of triangle strip for dotXSI 5.0+
/**
* The CSLXSITriangleStripList class describes a list of triangle strips that are made up of N number of vertices.
* The polynodes in this list can have N number of attibutes like vertex, normals, UV and color information. 
*
* To find out, you may use GetVertexIndices() to get the mendatory vertex index attributes and
* the parent class (CSLXSISubComponentList) functionality to get the other generic attributes index.
*
* To get the number of triangle strips in the list, use GetTriangleStripCount(). 
* To find out how many vertices a particular triangle strip uses, get the list of triangle strip vertex count
* using GetTriangleStripNodeCountArray().
* 
*/
class XSIEXPORT CSLXSITriangleStripList
	: public CSLXSISubComponentList
{
public:
	/**
	* Proxy class that allows modifying a list of SI_Int like an array.
	*/
	typedef CSLArrayProxy<SI_Int, SI_Int, 1> CSLIntArray;

	/*! Constructor	
	* \param in_pScene Parent scene
	* \param in_pModel Parent model
	* \param in_pTemplate Refered template
	* \return an instance of CSLXSITriangleStripList
	*/
	CSLXSITriangleStripList(CSLScene *in_pScene, CSLModel *in_pModel, CdotXSITemplate *in_pTemplate);

	/*! Destructor
	*/
	virtual ~CSLXSITriangleStripList();

	/*! Returns the type of this template (XSI_TRIANGLE_STRIP_LIST)
		\return CSLTemplate::ETemplateType The type of this template (XSI_TRIANGLE_STRIP_LIST)
	*/
	virtual ETemplateType			Type() { return XSI_TRIANGLE_STRIP_LIST; }

	/*!	Return the material object of the triangle strip list.
		\return	CSLBaseMaterial*	Material object.
	*/
	CSLBaseMaterial*				GetMaterial() { return m_pMaterial; }

	/*!	Change the material object of the triangle strip list.
		\param	CSLBaseMaterial*	New material object.
		\return	None
	*/
	SI_Void							SetMaterial(CSLBaseMaterial *in_pNewMaterial) { m_pMaterial = in_pNewMaterial; }

	/*! Get the number of triangle strip.
	* \return	SI_Int	Number of triangle strip.
	*/
	SI_Int							GetTriangleStripCount();

	/*! Get the triangle strip node count array.
	* \return	CSLIntArray	Array of triangle strip node count.
	*/
	CSLXSITriangleStripList::CSLIntArray*	GetTriangleStripNodeCountArray();

	/*! Get the polynode vertex indices array.
	* \return	CSLIntArray	Array of polynode vertex indices.
	*/
	CSLIntArray*					GetVertexIndices();

	/*! Set the number of polynode in this triangle strip list.
	* \return	SI_Error	Error code.
	*/
	virtual SI_Error				SetCount(SI_Int);

	/*! Commit the current template information
		\return SI_Error Whether or not the commit was successful
	*/
	SI_Error Synchronize();

private:
	CSLIntArray						m_TriangleStripNodeCountArray;

	CSLIntArray						m_VertexIndices;

	CSLBaseMaterial*				m_pMaterial;

	void *m_pReserved;	// reserved for future extension
};

#endif
