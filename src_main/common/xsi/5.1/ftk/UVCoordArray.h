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
* File: 		 UVCoordArray.h
* Creation date: January 23, 2002
* Purpose:		 Declaration of class 'CSLUVCoordArray'
*
\******************************************************************************/
#ifndef _UVCOORDARRAY_H
#define _UVCOORDARRAY_H

#include "BaseShape.h"

/** 
* CSLUVCoordArray Class
*
* Class representing an array of UV coordinates
*
* \sa CSLShape_35
*/
class XSIEXPORT CSLUVCoordArray
{
public:
	CSLUVCoordArray(CdotXSITemplate *in_pTemplate, SI_Int in_nFirstIndex, CSLBaseShape::EShapeType in_Type);
	virtual ~CSLUVCoordArray();

	/*! Return the number of texture coordinates.
		\return SI_Int	Number of UV.
	*/
	SI_Int									GetUVCoordCount();

	/*! Return the texture coordinate array proxy.
		\return CSLVector2DArray*	Pointer to an array proxy of UV.
	*/
	CSLBaseShape::CSLVector2DArray* 		GetUVCoordList();

	/*! Returns an array of CSIBCVector2D that describes the texture coordinates for this mesh.
		\return CSIBCVector2D*	Array of texture coordinates
	*/
	CSIBCVector2D*							GetUVCoordListPtr();

	/*! Return the indexed texture coordinate array proxy.
		\return CSLIndexedVector2DArray*	Pointer to an array proxy of indexed UV.
	*/
	CSLBaseShape::CSLIndexedVector2DArray*	GetIndexedUVCoordList();

	/*! Returns an array of SLIndexedVector2D that describes the indexed texture coordinates for this mesh.
		\return SLIndexedVector2D*	Array of indexed texture coordinates
	*/
	SLIndexedVector2D*						GetIndexedUVCoordListPtr();

	/*!	Return the name of the texture projection.
		\return	SI_Char*	Texture projection name.
	*/
	SI_Char*								GetTextureProjection();

	/*!	Change the name of the texture projection.
		\param	SI_Char*	New texture projection name.
		\return	None
	*/
	SI_Void 								SetTextureProjection(SI_Char* in_szValue);

	/*!	Synchronize will all data in the FTK is up to date.
		\return	SI_Error	Error code
	*/
	SI_Error								Synchronize();

private:
	CSLIntProxy								m_NbUVCoords;
	CSLStringProxy							m_TextureProjection;
	CSLBaseShape::CSLVector2DArray*			m_pUVCoords;
	CSLBaseShape::CSLIndexedVector2DArray*	m_pIndexedUVCoords;

	void *m_pReserved;	// reserved for future extension
    CSIBCVector2D*									GetUVCoords();
};

#endif
