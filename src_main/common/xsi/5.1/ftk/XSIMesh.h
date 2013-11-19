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
* File: 		 XSIMesh.h
* Creation date: January 17, 2005
* Purpose:		 Declaration of class 'CSLXSIMesh'
*
\******************************************************************************/
#ifndef _XSIMESH_H
#define _XSIMESH_H

#include "XSIGeometry.h"

class CSLXSIVertexList;
class CSLXSIPolygonList;
class CSLXSITriangleList;
class CSLXSITriangleStripList;
class CSLXSIShape;


/**
* CSLXSIMesh class
*
* Describes a mesh primitive for dotXSI 5.0+.
* 
* The CSLXSIMesh class is used to manipulate the vertex and the xsi polygon, tiangle or tri-strip lists.
*/
class XSIEXPORT CSLXSIMesh
	: public CSLXSIGeometry
{
public:
	CSLXSIMesh(CSLScene* in_pScene, CSLModel *in_pModel, CdotXSITemplate* in_pTemplate);
	virtual ~CSLXSIMesh();

	/*!	Clears the entire list of polygon lists
		\return SI_Error	Error code
	*/
	SI_Error						ClearXSIPolygonLists();

	/*!	Clears the entire list of triangle lists
		\return SI_Error	Error code
	*/
	SI_Error						ClearXSITriangleLists();

	/*!	Clears the entire list of triangle strip lists
		\return SI_Error	Error code
	*/
	SI_Error						ClearXSITriangleStripLists();

	/*!	Returns the number of polygon lists for this mesh
		\return SI_Int	Number of polygon lists
	*/
	SI_Int							GetXSIPolygonListCount();

	/*!	Returns the number of triangle lists for this mesh
		\return SI_Int	Number of triangle lists
	*/
	SI_Int							GetXSITriangleListCount();

	/*!	Returns the number of triangle strip lists for this mesh
		\return SI_Int	Number of triangle strip lists
	*/
	SI_Int							GetXSITriangleStripListCount();

	/*!	Returns the list of polygon lists used by this mesh
		\return CSLXSIPolygonList**	A list of CSLXSIPolygonList
	*/
	CSLXSIPolygonList**				XSIPolygonLists();

	/*!	Returns the list of triangle lists used by this mesh
		\return CSLXSITriangleList** A list of CSLXSITriangleList
	*/
	CSLXSITriangleList**			XSITriangleLists();

	/*!	Returns the list of triangle strip lists used by this mesh
		\return CSLXSITriangleStripList**	A list of CSLXSITriangleStripList
	*/
	CSLXSITriangleStripList**		XSITriangleStripLists();

	/*!	Add a new polygon list to the mesh.
		\return	CSLXSIPolygonList*	Pointer to the newly created polygon list object.
	*/
	CSLXSIPolygonList* 				AddXSIPolygonList();

	/*!	Add a new triangle list to the mesh.
		\return	CSLXSITriangleList*	Pointer to the newly created triangle list object.
	*/
	CSLXSITriangleList*				AddXSITriangleList();

	/*!	Add a new triangle strip list to the mesh.
		\return	CSLXSITriangleStripList*	Pointer to the newly created triangle strip list object.
	*/
	CSLXSITriangleStripList*		AddXSITriangleStripList();

	/*!	Remove a single polygon list from the mesh.
		\param	SI_Int	Index of the polygon list to remove.
		\return	SI_Error	Error code.
	*/
	SI_Error						RemoveXSIPolygonList(SI_Int l_nIndex);

	/*!	Remove a single triangle list from the mesh.
		\param	SI_Int	Index of the triangle list to remove.
		\return	SI_Error	Error code.
	*/
	SI_Error						RemoveXSITriangleList(SI_Int l_nIndex);

	/*!	Remove a single triangle strip list from the mesh.
		\param	SI_Int	Index of the triangle strip list to remove.
		\return	SI_Error	Error code.
	*/
	SI_Error						RemoveXSITriangleStripList(SI_Int l_nIndex);

	/*!	Remove a single polygon list from the mesh.
		\param	CSLXSIPolygonList*	Pointer to the polygon list to remove.
		\return	SI_Error	Error code.
	*/
	SI_Error						RemoveXSIPolygonList(CSLXSIPolygonList* in_pObject);

	/*!	Remove a single triangle list from the mesh.
		\param	CSLXSITriangleList*	Pointer to the triangle list to remove.
		\return	SI_Error	Error code.
	*/
	SI_Error						RemoveXSITriangleList(CSLXSITriangleList* in_pObject);

	/*!	Remove a single triangle strip list from the mesh.
		\param	CSLXSITriangleStripList*	Pointer to the triangle strip list to remove.
		\return	SI_Error	Error code.
	*/
	SI_Error						RemoveXSITriangleStripList(CSLXSITriangleStripList* in_pObject);

	/*!	Return the shape object of the mesh.
		\return	CSLXSIShape*	Pointer to a shape object.
	*/
	CSLXSIShape*					XSIShape() { return m_pXSIShape; };

	/*!	Return the vertex list object of the mesh.
		\return	CSLXSIVertexList*	Pointer to a vertex list object.
	*/
	CSLXSIVertexList*				XSIVertexList() { return m_pXSIVertexList; };

	/*! Connects a shape
	* \param CSLXSIShape* the shape to connect
	* \return The connected shape
	*/
	CSLXSIShape*					ConnectXSIShape(CSLXSIShape*);

	/*! Connects a vertex list
	* \param CSLXSIVertexList* the vertex list to connect
	* \return The connected vertex list
	*/
	CSLXSIVertexList* 				ConnectXSIVertexList(CSLXSIVertexList*);

	/*! Connects a polygon list
	* \param CSLXSIPolygonList* the polygon list to connect
	* \return The connected polygon list
	*/
	CSLXSIPolygonList* 				ConnectXSIPolygonList(CSLXSIPolygonList*);

	/*! Connects a triangle list
	* \param CSLXSITriangleList* the triangle list to connect
	* \return The connected triangle list
	*/
	CSLXSITriangleList*				ConnectXSITriangleList(CSLXSITriangleList*);

	/*! Connects a triangle strip list
	* \param CSLXSITriangleStripList* the triangle strip list to connect
	* \return The connected triangle strip list
	*/
	CSLXSITriangleStripList*		ConnectXSITriangleStripList(CSLXSITriangleStripList*);

	/*! Synchronizes this xsi mesh instance with its IO layer representation
	* \return SI_Error	Error code.
	*/
	virtual SI_Error				Synchronize();

	/*! Returns the type of this template (XSI_MESH)
		\return CSLTemplate::ETemplateType The type of this template (XSI_MESH)
	*/
	virtual ETemplateType			Type() { return XSI_MESH; };

	virtual SI_Error				Fix();
    virtual CSLAnimatableType*		ParameterFromName(SI_Char *in_szName);

private:
	CSLXSIShape* m_pXSIShape;
	CSLXSIVertexList *m_pXSIVertexList;
	CSIBCArray<CSLXSIPolygonList *> m_XSIPolygonLists;
	CSIBCArray<CSLXSITriangleList *> m_XSITriangleLists;
	CSIBCArray<CSLXSITriangleStripList *> m_XSITriangleStripLists;

	void *m_pReserved;	// reserved for future extension
};

#endif
