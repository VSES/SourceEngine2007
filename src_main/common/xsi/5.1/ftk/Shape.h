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

#ifndef _SHAPE_H
#define _SHAPE_H

#include "BaseShape.h"

//! Mesh shape class
/** 
* Describes a mesh shape
*
* The CSLShape holds the mesh's vertex positions, normals, texture coordinates and color 
* information.
*/
class XSIEXPORT CSLShape
	: public CSLBaseShape
{
public:

	/*! Constructor	
	* \param in_pScene Parent scene
	* \param in_pModel Parent model
	* \param in_pTemplate Refered template
	* \param in_Type the type of shape to construct
	* \return an instance of CSLShape
	*/
	CSLShape(CSLScene* in_pScene, CSLModel *in_pModel, CdotXSITemplate* in_pTemplate, EShapeType in_Type);

	/*! Destructor
	*/
	virtual ~CSLShape();

	/*! Create a texture coordinates array.
	* \return CSLVector2DArray*	Pointer to the newly created array.
	*/
	CSLVector2DArray*				CreateUVCoords();

	/*! Create an indexed texture coordinates array.
	* \return CSLIndexedVector2DArray*	Pointer to the newly created indexed array.
	*/
	CSLIndexedVector2DArray*		CreateIndexedUVCoords();

	/*! Return the number of texture coordinates.
	* \return SI_Int	Number of UV.
	*/
	SI_Int							GetUVCoordCount();

	/*! Return the texture coordinate array proxy.
	* \return CSLVector2DArray*	Pointer to an array proxy of UV.
	*/
	CSLVector2DArray*				GetUVCoordList();

	/*! Returns an array of CSIBCVector2D that describes the texture coordinates for this mesh.
	* \return CSIBCVector2D*	Array of texture coordinates
	*/
	CSIBCVector2D*					GetUVCoordListPtr();

	/*! Return the indexed texture coordinate array proxy.
	* \return CSLIndexedVector2DArray*	Pointer to an array proxy of indexed UV.
	*/
	CSLIndexedVector2DArray*		GetIndexedUVCoordList();

	/*! Returns an array of SLIndexedVector2D that describes the indexed texture coordinates for this mesh.
	* \return SLIndexedVector2D*	Array of indexed texture coordinates
	*/
	SLIndexedVector2D*				GetIndexedUVCoordListPtr();

	/*!	Destroy the texture coordinates array.
	* \return	SI_Error	Error code.
	*/
	SI_Error						DestroyUVCoords();

	/*! Get the template type
	* \return ETemplateType the template type of the shape surface
	*/
	virtual ETemplateType			Type() { return SI_SHAPE; };

	/*! Synchronizes this shape instance with its IO layer representation
	* \return SI_Error
	* \retval SI_SUCCESS this function always returns SI_SUCCESS
	*/
	virtual SI_Error				Synchronize();

private:
	CSLVector2DArray*				m_pUVCoords;
	CSLIndexedVector2DArray*		m_pIndexedUVCoords;
	CSLIntProxy*					m_iNbUVCoords;
	CSLStringProxy* 				m_pUVStringProxy;

	void *m_pReserved;	// reserved for future extension
};

#endif

