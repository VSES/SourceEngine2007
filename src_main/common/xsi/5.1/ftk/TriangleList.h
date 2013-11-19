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
* File: 		 TriangleList.h
* Creation date: January 15, 2002
* Purpose:		 Declaration of class 'CSLTriangleList'
*
\******************************************************************************/
#ifndef _TRIANGLELIST_H
#define _TRIANGLELIST_H

#include "Template.h"

class CSLBaseMaterial;

struct SLTriangle
{
	SI_Int	m_iVtx1;
	SI_Int	m_iVtx2;
	SI_Int	m_iVtx3;
};

/** 
* CSLTriangleList Class
*
* The CSLTriangleList class describes a list of triangles. 
*
* It may also contain information about normals, UV coordinates and color. To find out, 
* you may use the GetNormalIndices(), GetUVIndices() and GetColorIndices() and check 
* if the return value is non-NULL.
*
* To get the number of triangles in the list, use GetTriangleCount(). 
* \sa CSLPolygonList
* \sa CSLTriStripList
*/
class XSIEXPORT CSLTriangleList
	: public CSLTemplate
{
public:
	/**
	* Proxy class that allows modifying a list of SLTriangle structures like an array.
	*/
	typedef CSLArrayProxy<SLTriangle, SI_Int, 3> CSLTriangleArray;

	CSLTriangleList(CSLScene* in_pScene, CSLModel *in_pModel, CdotXSITemplate* in_pTemplate);
	virtual ~CSLTriangleList();

	/*!	Returns the number of triangles in this list.
		\return SI_Int	Number of triangles
	*/
	SI_Int					GetTriangleCount() { return m_VertexIndices.GetUsed(); };

	/*!	Returns an array of SI_Int containing the vertex index list.
		\return SI_Int*	Array of SI_Int
	*/
	SI_Int*					GetVertexIndicesPtr() { return (SI_Int *) m_VertexIndices.ArrayPtr(); }

	/*! Return the vertex indices array proxy.
		\return CSLTriangleArray*	Pointer to an array proxy of vertex indices.
	*/
	CSLTriangleArray*		GetVertexIndices() { return &m_VertexIndices; }

	/*! Create a normal indices array.
		\return CSLTriangleArray*	Pointer to the newly created array.
	*/
	CSLTriangleArray*		CreateNormalIndices();

	/*! Return the normal indices array proxy.
		\return CSLTriangleArray*	Pointer to an array proxy of normal indices.
	*/
	CSLTriangleArray*		GetNormalIndices() { return m_NormalIndices; }

	/*!	Returns an array of SI_Int containing the normal index list.
		\return SI_Int*	Array of SI_Int
	*/
	SI_Int*					GetNormalIndicesPtr();

	/*!	Destroy the normal indices array.
		\return	SI_Error	Error code.
	*/
	SI_Error				DestroyNormalIndices();

	/*! Create a color indices array.
		\return CSLTriangleArray*	Pointer to the newly created array.
	*/
	CSLTriangleArray*		CreateColorIndices();

	/*! Return the color indices array proxy.
		\return CSLTriangleArray*	Pointer to an array proxy of color indices.
	*/
	CSLTriangleArray*		GetColorIndices() { return m_ColorIndices; }

	/*!	Returns an array of SI_Int containing the color index list.
		\return SI_Int*	Array of SI_Int
	*/
	SI_Int*					GetColorIndicesPtr();

	/*!	Destroy the color indices array.
		\return	SI_Error	Error code.
	*/
	SI_Error				DestroyColorIndices();

	/*! Create a new UV indices array.
		\return CSLTriangleArray*	Pointer to the newly created array.
	*/
	CSLTriangleArray*		AddUVArray();

	/*! Return the number of texture coordinate arrays.
		\return SI_Int	Number of UV arrays.
	*/
	SI_Int					GetUVArrayCount();

	/*!	Returns an array of SI_Int containing the UV index list.
		\return SI_Int*	Array of SI_Int
	*/
	SI_Int*					GetUVIndicesPtr(SI_Int in_nId);

	/*! Return the UV indices array proxy.
		\param	SI_Int	Identification of the UV array to retrieve.
		\return CSLTriangleArray*	Pointer to an array proxy of UV indices.
	*/
	CSLTriangleArray*		GetUVIndices(SI_Int in_nId);

	/*! Remove the last UV indices array.
		\return SI_Error	Error code.
	*/
	SI_Error				RemoveUVArray();

	/*! Remove all UV indices array.
		\return SI_Error	Error code.
	*/
	SI_Error				ClearUVArrays();

	/*!	Return the material object of the triangle list.
		\return	CSLBaseMaterial*	Texture projection name.
	*/
	CSLBaseMaterial*		GetMaterial() { return m_pMaterial; }

	/*!	Change the material object of the triangle list.
		\param	CSLBaseMaterial*	New material object.
		\return	None
	*/
	SI_Void					SetMaterial(CSLBaseMaterial *in_pNewMaterial) { m_pMaterial = in_pNewMaterial; }

	virtual ETemplateType	Type() { return SI_TRIANGLE_LIST; }
	virtual SI_Error		Synchronize();
    virtual CSLAnimatableType* ParameterFromName(SI_Char *in_szName);

private:
	CSLBaseMaterial*		m_pMaterial;
	CSLTriangleArray		m_VertexIndices;

	CSLTriangleArray*		m_NormalIndices;
	CSIBCArray<CSLTriangleArray* >	m_UVIndices;
	CSLTriangleArray*		m_ColorIndices;

	SI_Bool 				m_bSingleUV;
	SI_Int					m_iArrayCount;

	void *m_pReserved;	// reserved for future extension
};

#endif
