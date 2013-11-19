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
/******************************************************************************\
*
* File: 		 TriangleStripList.h
* Creation date: January 15, 2002
* Purpose:		 Declaration of class 'CSLTriangleStripList'
*
\******************************************************************************/
#ifndef _TRIANGLESTRIPLIST_H
#define _TRIANGLESTRIPLIST_H

#include "Template.h"

class CSLTriangleStrip;
class CSLBaseMaterial;


/** 
* CSLTriangleStripList Class
*
* Describes a list of CSLTriangleStrips. A mesh can be made up of 3 types of polygons. N-sided polygons (CSLPolygonList), 
* Triangles (CSLTriangleList) and Tri-Strips (CSLTriangleStrip). The CSLTriangleStripList is simply a container of CSLTriangleStrips
* that share the same material. To access the triangle strips from a CSLMesh object (or to add new ones), you should use this class
* instead of working with the CSLTriangleStrip class directly.
*
*/
class XSIEXPORT CSLTriangleStripList
	: public CSLTemplate
{
public:
	CSLTriangleStripList(CSLScene* in_pScene, CSLModel *in_pModel, CdotXSITemplate* in_pTemplate);
	virtual ~CSLTriangleStripList();

	/*!	Add a new triangle strip to the list.
		\return	CSLTriangleStrip*	Pointer to the newly created triangle strip object.
	*/
	CSLTriangleStrip*				AddTriangleStrip();

	/*!	Returns the number of triangle strips in this list
		\return SI_Int	Number of triangle strips
	*/
	SI_Int							GetTriangleStripCount();

	/*!	Returns the list of triangle strips in the object.
		\return CSLTriangleStrip**	A list of pointers to triangle strips objects.
	*/
	CSLTriangleStrip**				TriangleStrips();

	/*!	Remove a single triangle strip from the list.
		\param	SI_Int	Index of the triangle strip to remove.
		\return	SI_Error	Error code.
	*/
	SI_Error						RemoveTriangleStrip(SI_Int in_nIndex);

	/*!	Remove a single triangle strip from the list.
		\param	CSLTriangleStripList*	Pointer to the triangle strip to remove.
		\return	SI_Error	Error code.
	*/
	SI_Error						RemoveTriangleStrip(CSLTriangleStrip *in_pTriangleStrip);

	/*!	Clears the entire list of triangle strips
		\return SI_Error	Error code
	*/
	SI_Error						ClearTriangleStrips();

	/*!	Return the material object of the triangle strip list.
		\return	CSLBaseMaterial*	Texture projection name.
	*/
	CSLBaseMaterial*				GetMaterial() { return m_pMaterial; }

	/*!	Change the material object of the triangle strip list.
		\param	CSLBaseMaterial*	New material object.
		\return	None
	*/
	SI_Void							SetMaterial(CSLBaseMaterial *in_pNewMaterial) { m_pMaterial = in_pNewMaterial; }

	// Connection functions
	CSLTriangleStrip*				ConnectTriangleStrip(CSLTriangleStrip* in_pNewTriStrip);

	virtual ETemplateType			Type() { return SI_TRIANGLE_STRIP_LIST; }
	virtual SI_Error				Synchronize();
    virtual CSLAnimatableType*		ParameterFromName(SI_Char *in_szName);

private:
	CSIBCArray<CSLTriangleStrip* >	m_TriangleStrips;
	CSLBaseMaterial*				m_pMaterial;

	void *m_pReserved;	// reserved for future extension
};

#endif
