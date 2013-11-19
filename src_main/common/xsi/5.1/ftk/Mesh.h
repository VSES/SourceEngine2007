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
* File: 		 Mesh.h
* Creation date: January 15, 2002
* Purpose:		 Declaration of class 'CSLMesh'
*
\******************************************************************************/
#ifndef _MESH_H
#define _MESH_H

#include "Geometry.h"

class CSLPolygonList;
class CSLTriangleList;
class CSLTriangleStripList;
class CSLBaseShape;


/**
* CSLMesh class
*
* Describes a mesh primitive.
* 
* The CSLMesh class is used to manipulate the polygon, tiangle or tri-strip lists.
*/
class XSIEXPORT CSLMesh
	: public CSLGeometry
{
public:
	CSLMesh(CSLScene* in_pScene, CSLModel *in_pModel, CdotXSITemplate* in_pTemplate);
	virtual ~CSLMesh();

	/*!	Clears the entire list of polygon lists
		\return SI_Error	Error code
	*/
	SI_Error						ClearPolygonLists();

	/*!	Clears the entire list of triangle lists
		\return SI_Error	Error code
	*/
	SI_Error						ClearTriangleLists();

	/*!	Clears the entire list of triangle strip lists
		\return SI_Error	Error code
	*/
	SI_Error						ClearTriangleStripLists();

	/*!	Returns the number of polygon lists for this mesh
		\return SI_Int	Number of polygon lists
	*/
	SI_Int							GetPolygonListCount();

	/*!	Returns the number of triangle lists for this mesh
		\return SI_Int	Number of triangle lists
	*/
	SI_Int							GetTriangleListCount();

	/*!	Returns the number of triangle strip lists for this mesh
		\return SI_Int	Number of triangle strip lists
	*/
	SI_Int							GetTriangleStripListCount();

	/*!	Returns the list of polygon lists used by this mesh
		\return CSLPolygonList**	A list of CSLPolygonList
	SI_Int							GetTriangleListCount();
	SI_Int							GetTriangleStripListCount();
	*/
	CSLPolygonList**				PolygonLists();

	/*!	Returns the list of triangle lists used by this mesh
		\return CSLTriangleList** A list of CSLTriangleList
	*/
	CSLTriangleList**				TriangleLists();

	/*!	Returns the list of triangle strip lists used by this mesh
		\return CSLTriangleStripList**	A list of CSLTriangleStripList
	*/
	CSLTriangleStripList**			TriangleStripLists();

	/*!	Add a new polygon list to the mesh.
		\return	CSLPolygonList*	Pointer to the newly created polygon list object.
	*/
	CSLPolygonList* 				AddPolygonList();

	/*!	Add a new triangle list to the mesh.
		\return	CSLTriangleList*	Pointer to the newly created triangle list object.
	*/
	CSLTriangleList*				AddTriangleList();

	/*!	Add a new triangle strip list to the mesh.
		\return	CSLTriangleStripList*	Pointer to the newly created triangle strip list object.
	*/
	CSLTriangleStripList*			AddTriangleStripList();

	/*!	Remove a single polygon list from the mesh.
		\param	SI_Int	Index of the polygon list to remove.
		\return	SI_Error	Error code.
	*/
	SI_Error						RemovePolygonList(SI_Int l_nIndex);

	/*!	Remove a single triangle list from the mesh.
		\param	SI_Int	Index of the triangle list to remove.
		\return	SI_Error	Error code.
	*/
	SI_Error						RemoveTriangleList(SI_Int l_nIndex);

	/*!	Remove a single triangle strip list from the mesh.
		\param	SI_Int	Index of the triangle strip list to remove.
		\return	SI_Error	Error code.
	*/
	SI_Error						RemoveTriangleStripList(SI_Int l_nIndex);

	/*!	Remove a single polygon list from the mesh.
		\param	CSLPolygonList*	Pointer to the polygon list to remove.
		\return	SI_Error	Error code.
	*/
	SI_Error						RemovePolygonList(CSLPolygonList* in_pObject);

	/*!	Remove a single triangle list from the mesh.
		\param	CSLTriangleList*	Pointer to the triangle list to remove.
		\return	SI_Error	Error code.
	*/
	SI_Error						RemoveTriangleList(CSLTriangleList* in_pObject);

	/*!	Remove a single triangle strip list from the mesh.
		\param	CSLTriangleStripList*	Pointer to the triangle strip list to remove.
		\return	SI_Error	Error code.
	*/
	SI_Error						RemoveTriangleStripList(CSLTriangleStripList* in_pObject);

	/*!	Return the shape object of the mesh.
		\return	CSLBaseShape*	Pointer to a shape object.
	*/
	CSLBaseShape*					Shape() { return m_pShape; };

	// Connection functions
	CSLBaseShape*					ConnectShape(CSLBaseShape*);
	CSLPolygonList* 				ConnectPolygonList(CSLPolygonList*);
	CSLTriangleList*				ConnectTriangleList(CSLTriangleList*);
	CSLTriangleStripList*			ConnectTriangleStripList(CSLTriangleStripList*);

	virtual SI_Error				Synchronize();
	virtual SI_Error				Fix();
	virtual ETemplateType			Type() { return SI_MESH; };
    virtual CSLAnimatableType*		ParameterFromName(SI_Char *in_szName);

private:
	CSLBaseShape* m_pShape;
	CSIBCArray<CSLPolygonList *> m_PolygonLists;
	CSIBCArray<CSLTriangleList *> m_TriangleLists;
	CSIBCArray<CSLTriangleStripList *> m_TriangleStripLists;

	void *m_pReserved;	// reserved for future extension
};

#endif
