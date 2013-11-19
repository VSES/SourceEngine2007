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
* File: 		 SurfaceMesh.h
* Creation date: January 15, 2002
* Purpose:		 Declaration of class 'CSLSurfaceMesh'
*
\******************************************************************************/
#ifndef _SURFACEMESH_H
#define _SURFACEMESH_H

#include "Primitive.h"


class CSLNurbsSurface;

/**
* Class CSLSurfaceMesh
* Describes a list of NURBS surfaces.
*
* \sa CSLPrimitive
*/
class XSIEXPORT CSLSurfaceMesh
	: public CSLPrimitive
{
public:
	CSLSurfaceMesh(CSLScene* in_pScene, CSLModel *in_pModel, CdotXSITemplate* in_pTemplate);
	virtual ~CSLSurfaceMesh();

	/*!	Add a new surface to the list.
		\return	CSLNurbsSurface*	Pointer to the newly created surface.
	*/
	CSLNurbsSurface* AddSurface();

	/*!	Return the number of surfaces in the list.
		\return	SI_Int	Number of surfaces.
	*/
	SI_Int GetSurfaceCount();

	/*!	Return an array of all surfaces in the list.
		\return	CSLNurbsSurface**	Array of pointers to surface objects.
	*/
	CSLNurbsSurface **Surfaces();

	/*!	Remove a single surface from the list.
		\param	SI_Int	Index of the surface to remove.
		\return	SI_Error	Error code.
	*/
	SI_Error RemoveSurface(SI_Int in_nIndex);

	/*!	Remove a single surface from the list.
		\param	CSLNurbsSurface*	Pointer to the surface to remove.
		\return	SI_Error	Error code.
	*/
	SI_Error RemoveSurface(CSLNurbsSurface* in_pSurface);

	/*!	Remove all surfaces from the list.
		\return	SI_Error	Error code.
	*/
	SI_Error ClearSurfaces();

	// Connection functions.
	CSLNurbsSurface* ConnectSurface(CSLNurbsSurface* in_pNewSurface);

    virtual SI_Error Synchronize();
    virtual ETemplateType Type();

private:
	CSIBCArray<CSLNurbsSurface *> m_Surfaces;

	void *m_pReserved;	// reserved for future extension
};

#endif
