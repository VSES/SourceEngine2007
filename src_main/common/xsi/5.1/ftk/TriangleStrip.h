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
* File:          TriangleStrip.h
* Creation date: January 15, 2002
* Purpose:       Declaration of class 'CSLTriangleStrip'
*
\******************************************************************************/
#ifndef _TRIANGLESTRIP_H
#define _TRIANGLESTRIP_H

#include "Template.h"

class CSLTriangleStripList;

#ifndef _SL_INT_ARRAY_PROXY_EXPORT_
#define _SL_INT_ARRAY_PROXY_EXPORT_
#endif 

/** 
* Describes a triangle strip
*
* The CSLTriangleStrip class describes a single triangle strip.
* The triangles in this list can have normals, UV and color information as well. 
*
* To find out, you may use the GetNormalIndices(), GetUVIndices() and GetColorIndices() and check 
* if the return value is non-NULL.
*
* \sa CSLTriangleStripList
* \sa CSLTriangleList
* \sa CSLPolygonList
*/
class XSIEXPORT CSLTriangleStrip
    : public CSLTemplate
{
public:
	/**
	* Proxy class that allows modifying a list of SI_Int like an array.
	*/
	typedef CSLArrayProxy<SI_Int, SI_Int, 1> CSLIntArray;

	CSLTriangleStrip(CSLScene* in_pScene, CSLModel *in_pModel, CdotXSITemplate* in_pTemplate, CSLTriangleStripList* in_pTriStripList);
	virtual ~CSLTriangleStrip();

	/*!	Returns the number of vertices in this strip.
		\return SI_Int	Number of vertices.
	*/
	SI_Int					GetVertexCount () { return m_VertexIndices.GetUsed(); }

	/*! Return the vertex indices array proxy.
		\return CSLIntArray*	Pointer to an array proxy of vertex indices.
	*/
	CSLIntArray*			GetVertexIndices() { return &m_VertexIndices; }

	/*! Returns an array of SI_Int containing the vertex index list.
		\return SI_Int* Array of SI_Int
	*/
	SI_Int*					GetVertexIndicesPtr() { return m_VertexIndices.ArrayPtr(); }

	/*! Create a normal indices array.
		\return CSLIntArray*	Pointer to the newly created array.
	*/
	CSLIntArray*			CreateNormalIndices();

	/*! Return the normal indices array proxy.
		\return CSLIntArray*	Pointer to an array proxy of normal indices.
	*/
	CSLIntArray*			GetNormalIndices() { return m_NormalIndices; }

	/*! Returns an array of SI_Int containing the normal index list.
		\return SI_Int* Array of SI_Int
	*/
	SI_Int*					GetNormalIndicesPtr();

	/*!	Destroy the normal indices array.
		\return	SI_Error	Error code.
	*/
	SI_Error				DestroyNormalIndices();

	/*! Create a color indices array.
		\return CSLIntArray*	Pointer to the newly created array.
	*/
	CSLIntArray*			CreateColorIndices();

	/*! Return the color indices array proxy.
		\return CSLIntArray*	Pointer to an array proxy of color indices.
	*/
	CSLIntArray*			GetColorIndices() { return m_ColorIndices; }

	/*! Returns an array of SI_Int containing the color index list.
		\return SI_Int* Array of SI_Int
	*/
	SI_Int*					GetColorIndicesPtr();

	/*!	Destroy the color indices array.
		\return	SI_Error	Error code.
	*/
	SI_Error				DestroyColorIndices();

	/*! Create a new UV indices array.
		\return CSLIntArray*	Pointer to the newly created array.
	*/
	CSLIntArray*			AddUVArray();

	/*! Return the number of texture coordinate arrays.
		\return SI_Int	Number of UV arrays.
	*/
	CSLIntArray*			GetUVIndices(SI_Int in_nId);

	/*! Return the UV indices array proxy.
		\param	SI_Int	Identification of the UV array to retrieve.
		\return CSLIntArray*	Pointer to an array proxy of UV indices.
	*/
	SI_Int*					GetUVIndicesPtr(SI_Int in_nId);

	/*! Returns an array of SI_Int containing the UV index list.
		\param	SI_Int	Identification of the UV array to retrieve.
		\return SI_Int* Array of SI_Int
	*/
	SI_Error				RemoveUVArray();

	/*! Remove the last UV indices array.
		\return SI_Error	Error code.
	*/
	SI_Error				ClearUVArrays();

	/*! Remove all UV indices array.
		\return SI_Error	Error code.
	*/
	SI_Int					GetUVArrayCount();

	virtual ETemplateType	Type() { return SI_TRIANGLE_STRIP; }
	virtual SI_Error		Synchronize();

private:
	CSLIntArray				m_VertexIndices;

	CSLIntArray*			m_NormalIndices;
	CSIBCArray<CSLIntArray* >	m_UVIndices;
	CSLIntArray*			m_ColorIndices;

	SI_Bool 				m_bSingleUV;
	SI_Int					m_iArrayCount;

	void *m_pReserved;	// reserved for future extension
};

#endif
