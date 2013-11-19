//***************************************************************************************
//
// File supervisor: Softimage 3D Games & 3D Bridge team
//
// (c) Copyright 2001-2002 Avid Technology, Inc. . All rights reserved.
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

#ifndef _POLYGONLIST_H
#define _POLYGONLIST_H

#include "Template.h"

#ifndef _SL_INT_ARRAY_PROXY_EXPORT_
#define _SL_INT_ARRAY_PROXY_EXPORT_

//XSITEMPLATEEXP template class XSIEXPORT CSLArrayProxy<SI_Int, SI_Int, 1>;

#endif // _SL_INT_ARRAY_PROXY_EXPORT_

class CSLBaseMaterial;

//!Describes a list of n-sided polygons
/**
* The CSLPolygonList class describes a list of polygons that are made up of N number of vertices.
* The polygons in this list can have normals, UV and color information as well. 
*
* To find out, you may use the GetNormalIndices(), GetUVIndices() and GetColorIndices() and check 
* if the return value is non-NULL.
*
* To get the number of polygons in the list, use GetPolygonCount(). 
* To find out how many vertices a particular polygon uses, first get the list of polygon vertex count
* use GetPolygonVertexCountList().
* 
*/
class XSIEXPORT CSLPolygonList
	: public CSLTemplate
{
public:
	/**
	* Proxy class that allows modifying a list of SI_Int like an array.
	*/
	typedef CSLArrayProxy<SI_Int, SI_Int, 1> CSLIntArray;

	/*! Constructor
	* \param in_pScene	Pointer to the scene containing the polygon list
	* \param in_pModel	Pointer to the model containing the polygon list
	* \param in_pTemplate	Pointer to the CdotXSITemplate containing the polygon list
	* \return an instance of CSLPolygonList
	*/
	CSLPolygonList(CSLScene* in_pScene, CSLModel *in_pModel, CdotXSITemplate* in_pTemplate);

	/*! Destructor
	*/
	virtual ~CSLPolygonList();

	/*! Returns the number of polygons in this list.
	* \return SI_Int	Number of polygons
	*/
	SI_Int			GetPolygonCount () { return m_PolygonVertexCount.GetUsed(); }

	/*! Return the polygon vertex count array proxy.
	* \return CSLIntArray*	Pointer to an array proxy of polygon vertex count.
	*/
	CSLIntArray*	GetPolygonVertexCountList() { return &m_PolygonVertexCount; };

	/*! Returns an array of SI_Int containing the number of vertices for each polygon.
	* \return SI_Int* Array of SI_Int
	*/
	SI_Int* 		GetPolygonVertexCountListPtr() { return m_PolygonVertexCount.ArrayPtr(); }

	/*! Returns the number of vertices referred by all polygons in this list.
	* \return SI_Int	Number of vertices
	*/
	SI_Int			GetVertexCount () { return m_VertexIndices.GetUsed(); }

	/*! Return the vertex indices array proxy.
	* \return CSLIntArray*	Pointer to an array proxy of vertex indices.
	*/
	CSLIntArray*	GetVertexIndices() { return &m_VertexIndices; };

	/*! Returns an array of SI_Int containing the vertex index list.
	* \return SI_Int* Array of SI_Int
	*/
	SI_Int* 		GetVertexIndicesPtr() { return m_VertexIndices.ArrayPtr(); }

	/*! Create a normal indices array.
	* \return CSLIntArray*	Pointer to the newly created array.
	*/
	CSLIntArray*	CreateNormalIndices();

	/*! Return the normal indices array proxy.
	* \return CSLIntArray*	Pointer to an array proxy of normal indices.
	*/
	CSLIntArray*	GetNormalIndices() { return m_NormalIndices; }

	/*! Returns an array of SI_Int containing the normal index list.
	* \return SI_Int* Array of SI_Int
	*/
	SI_Int* 		GetNormalIndicesPtr();

	/*!	Destroy the normal indices array.
	* \return	SI_Error	Error code.
	*/
	SI_Error		DestroyNormalIndices();

	/*! Create a color indices array.
	* \return CSLIntArray*	Pointer to the newly created array.
	*/
	CSLIntArray*	CreateColorIndices();

	/*! Return the color indices array proxy.
	* \return CSLIntArray*	Pointer to an array proxy of color indices.
	*/
	CSLIntArray*	GetColorIndices() { return m_ColorIndices; }

	/*! Returns an array of SI_Int containing the color index list.
	* \return SI_Int* Array of SI_Int
	*/
	SI_Int* 		GetColorIndicesPtr();

	/*!	Destroy the color indices array.
	* \return	SI_Error	Error code.
	*/
	SI_Error		DestroyColorIndices();

	/*! Create a new UV indices array.
	* \return CSLIntArray*	Pointer to the newly created array.
	*/
	CSLIntArray*	AddUVArray();

	/*! Return the number of texture coordinate arrays.
	* \return SI_Int	Number of UV arrays.
	*/
	SI_Int			GetUVArrayCount();

	/*! Return the UV indices array proxy.
	* \param	in_nId	Identification of the UV array to retrieve.
	* \return CSLIntArray*	Pointer to an array proxy of UV indices.
	*/
	CSLIntArray*	GetUVIndices(SI_Int in_nId);

	/*! Returns an array of SI_Int containing the UV index list.
	* \param	in_nId	Identification of the UV array to retrieve.
	* \return SI_Int* Array of SI_Int
	*/
	SI_Int* 		GetUVIndicesPtr(SI_Int in_nId);

	/*! Remove the last UV indices array.
	* \return SI_Error	Error code.
	*/
	SI_Error		RemoveUVArray();

	/*! Remove all UV indices array.
	* \return SI_Error	Error code.
	*/
	SI_Error		ClearUVArrays();

	/*!	Return the material object of the polygon list.
	* \return	CSLBaseMaterial*	Texture projection name.
	*/
	CSLBaseMaterial*	GetMaterial() { return m_pMaterial; }

	/*!	Change the material object of the polygon list.
	* \param	in_pNewMaterial	New material object.
	* \return	SI_Void
	*/
	SI_Void				SetMaterial(CSLBaseMaterial *in_pNewMaterial) { m_pMaterial = in_pNewMaterial; }

	/*! Updates the template with information from this polygon list
	* \return SI_Error Whether or not the template could be updated
	*/
    virtual SI_Error Synchronize();

	/*! Returns the type of this template
	* \return ETemplateType
	*/
	virtual ETemplateType Type() { return SI_POLYGON_LIST; };

	/*! Gets a handle on one of this polygon list's parameters by using its name
	* \param in_szName the name of the parameter to get the handle from
	* \return CSLAnimatableType* the handle to the parameter
	*/
    virtual CSLAnimatableType* ParameterFromName(SI_Char *in_szName);

private:
	CSLBaseMaterial*		m_pMaterial;
	CSLIntArray 			m_PolygonVertexCount;
	CSLIntArray 			m_VertexIndices;

	CSLIntArray*			m_NormalIndices;
	CSLIntArray*			m_ColorIndices;
	CSIBCArray<CSLIntArray* >	m_UVIndices;

	SI_Bool 				m_bSingleUV;
	SI_Int					m_iArrayCount;

	void *m_pReserved;	// reserved for future extension
};

#endif
