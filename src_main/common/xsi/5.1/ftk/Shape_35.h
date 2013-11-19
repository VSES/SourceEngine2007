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

#ifndef _SHAPE_35_H
#define _SHAPE_35_H

#include "BaseShape.h"

class CSLUVCoordArray;

//! Mesh shape class for dotXSI 3.5+
/** 
* Describes a mesh shape
*
* The CSLShape holds the mesh's vertex positions, normals, texture coordinates and color 
* information.
*
* This class allows a shape to contain more than one array of texture coordinates.
* Also, each array contains information about its texture projetion.
*/
class XSIEXPORT CSLShape_35
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
	CSLShape_35(CSLScene *in_pScene, CSLModel *in_pModel, CdotXSITemplate *in_pTemplate, EShapeType in_Type);

	/*! Destructor
	*/
	virtual ~CSLShape_35();

	/*!	Add a new texture coordinate array.
	* \return	CSLUVCoordArray*	Pointer to the newly created array.
	*/
	CSLUVCoordArray*				AddUVCoordArray();

	/*!	Return the number of texture coordinate arrays.
	* \return	SI_Int	Number of UV arrays.
	*/
	SI_Int							GetUVCoordArrayCount();

	/*!	Return an array of all texture coordinate arrays.
	* \return	CSLUVCoordArray**	Array of pointers to UV array.
	*/
	CSLUVCoordArray**				UVCoordArrays();

	/*!	Remove the last texture coordinate array.
	* \return	SI_Error	Error code.
	*/
	SI_Error						RemoveUVCoordArray();

	/*!	Remove all texture coordinate arrays.
	* \return	SI_Error	Error code.
	*/
	SI_Error						ClearUVCoordArrays();

	/*! Get the template type
	* \return ETemplateType the template type of the shape surface
	*/
	virtual ETemplateType			Type() { return SI_SHAPE35; };

	/*! Synchronizes this shape instance with its IO layer representation
	* \return SI_Error
	* \retval SI_SUCCESS this function always returns SI_SUCCESS
	*/
	virtual SI_Error				Synchronize();

private:
	CSIBCArray<CSLUVCoordArray *>	m_UVCoordArrays;

	void *m_pReserved;	// reserved for future extension
};

#endif
