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
* File:          MaterialLibrary.h
* Creation date: January 15, 2002
* Purpose:       Declaration of class 'MaterialLibrary'
*
\******************************************************************************/
#ifndef _MATERIALLIBRARY_H
#define _MATERIALLIBRARY_H

#include "Template.h"

// Forward declaration
class CSLBaseMaterial;
class CSLMaterial;
class CSLXSIMaterial;


/** 
* CSLMaterialLibrary class
* Handles the material library. From this class you can add or remove materials that can be used
* in the scene.
*/
class XSIEXPORT CSLMaterialLibrary
	: public CSLTemplate
{
public:
	CSLMaterialLibrary(CSLScene* in_pScene, CSLModel *in_pModel, CdotXSITemplate* in_pTemplate);
	virtual ~CSLMaterialLibrary();

	/*!	Add a new material to the material library.
		\return	CSLMaterial*	Pointer to the newly created material object.
	*/
	CSLMaterial* AddMaterial();

	/*!	Add a new XSI material to the material library.
		\return	CSLMaterial*	Pointer to the newly created XSI material object.
	*/
	CSLXSIMaterial* AddXSIMaterial();

	/*!	Remove all materials from the material library.
		\return	SI_Error	Error code.
	*/
	SI_Error ClearMaterials();

	/*!	Return an array of all materials in the library.
		\return	CSLBaseMaterial**	Array of pointers to material objects.
	*/
	CSLBaseMaterial** GetMaterialList();

	/*!	Return the number of materials in the library.
		\return	SI_Int	Number of materials.
	*/
	SI_Int GetMaterialCount();

	/*!	Remove a single material from the library.
		\param	SI_Int	Index of the material to remove.
		\return	SI_Error	Error code.
	*/
	SI_Error RemoveMaterial(SI_Int in_nIndex);

	/*!	Remove a single material from the library.
		\param	CSLBaseMaterial*	Pointer to the material to remove.
		\return	SI_Error	Error code.
	*/
	SI_Error RemoveMaterial(CSLBaseMaterial *in_pMaterial);

	CSLBaseMaterial* FindMaterial( const SI_Char* in_szName );

	CSLBaseMaterial* ConnectMaterial(CSLBaseMaterial* in_pNewMaterial);

    virtual SI_Error Synchronize();
    virtual ETemplateType Type();

private:
	CSIBCArray<CSLBaseMaterial *> m_Materials;

	void *m_pReserved;	// reserved for future extension
};

#endif
