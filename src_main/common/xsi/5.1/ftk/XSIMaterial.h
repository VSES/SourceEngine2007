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

#ifndef _XSIMATERIAL_H
#define _XSIMATERIAL_H

#include "Material.h"

class CSLConnectionPoint;
class CSLXSIShader;
class CSLXSIMaterialInfo;

/** 
* Class CSLXSIMaterial
*
* Describes an XSIMaterial
*
* The role of the XSIMaterial class is to help build a shader tree that describes an effect.
* They are containers of CSLXSIShader objects and CSLConnectionPoint objects.
*
* To get more information on XSI Material, see also the SOFTIMAGE|XSI Documentation on the
* render tree and the SOFTIMAGE|XSI SDK Documentation on Realtime Shaders.
*
* \note This template was introduced in dotXSI Version 3.6
*/
class XSIEXPORT CSLXSIMaterial
    : public CSLBaseMaterial
{
public:
	/*! Constructor
		\param in_pScene Parent scene
		\param in_pModel Parent model
		\param in_pTemplate Refered template
	*/
    CSLXSIMaterial(CSLScene* in_pScene, CSLModel *in_pModel, CdotXSITemplate* in_pTemplate);
	
	//! Destructor
	virtual ~CSLXSIMaterial();

	/*! Returns the type of this template (XSI_MATERIAL)
		\return CSLTemplate::ETemplateType The type of this template (XSI_MATERIAL)
	*/
	CSLTemplate::ETemplateType Type(){ return XSI_MATERIAL; }

	/*! Commit the current template information
		\return SI_Error Whether or not the commit was successful
	*/
	SI_Error Synchronize();

	/*! Searches for a parameter
		\param in_szName Name of the parameter to look for
		\return CSLAnimatableType Pointer to the found parameter, null otherwise
	*/
	virtual CSLAnimatableType* ParameterFromName(SI_Char *in_szName);

	///////////////////////////////////////////////////////////////////////////
	// Connection points functionality ////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////
	
	/*! Add a connection point to the connection point list
		\return CSLConnectionPoint Pointer to the newly added connection point
	*/
	CSLConnectionPoint* AddConnectionPoint();

	/*! Removes a connection point from the connection point list
		\param in_nIndex Index of the connection point to remove
		\return SI_Error Whether or not the connection point could be removed
	*/
	SI_Error RemoveConnectionPoint( SI_Int in_nIndex );

	/*! Removes a connection point from the connection point list
		\param in_pToRemove Pointer to the connection point to remove
		\return SI_Error Whether or not the connection point could be removed
	*/
	SI_Error RemoveConnectionPoint( CSLConnectionPoint* in_pToRemove );

	/*! Removes all connections points from the connection point list
		\return SI_Error Whether or not the list could be cleared
	*/
	SI_Error ClearConnectionPoints();

	/*! Adds an exisiting connection point to the connection point list
		\param in_pToConnect Pointer to the connection point to add
		\return The added connection point or null if it already exists in the list
	*/
	CSLConnectionPoint* ConnectConnectionPoint( CSLConnectionPoint* in_pToConnect );

	/*! Gets a pointer to the internally-stored connection point pointer list
		\return CSLConnectionPoint Pointer to the list
	*/
	CSLConnectionPoint** GetConnectionPointList();

	/*! Gets the total number of connection points 
		\return SI_Int Number of connection points
	*/
	SI_Int GetConnectionPointCount();
	
	///////////////////////////////////////////////////////////////////////////
	// Shaders functionality //////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////
	
	/*! Adds a shader to the shader list
		\return CSLXSIShaderPointer to the newly added shader
	*/
	CSLXSIShader* AddShader();

	/*! Removes a shader from the shader list
		\param in_nIndex Index of the shader to remove
		\return SI_Error Whether or not the shader could be removed
	*/
	SI_Error RemoveShader( SI_Int in_nIndex );

	/*! Removes a shader from the shader list
		\param in_pToRemove Pointer to the shader to remove
		\return SI_Error Whether or not the shader could be removed
	*/
	SI_Error RemoveShader( CSLXSIShader* in_pToRemove );

	/*! Removes all connections points from the shader list
		\return SI_Error Whether or not the list could be cleared
	*/
	SI_Error ClearShaders();

	/*! Adds an exisiting shader to the shader list
		\param in_pToConnect Pointer to the shader to add
		\return The added shader or null if it already exists in the list
	*/
	CSLXSIShader* ConnectShader( CSLXSIShader* in_pToConnect );

	/*! Gets a pointer to the internally-stored shader pointer list
		\return CSLXSIShader Pointer to the list
	*/
	CSLXSIShader** GetShaderList();

	/*! Gets the total number of Shaders 
		\return SI_Int Number of Shaders
	*/
	SI_Int GetShaderCount();

	/*! Searches for a given shader
		\param in_szShaderName Name of the shader to look for
		\return CSLXSIShader Pointer to the shader or null if it cannot be found
	*/
	CSLXSIShader* FindShader( SI_Char* in_szShaderName );

	///////////////////////////////////////////////////////////////////////////
	// Material Info functionality ////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	/*! Creates a new material info
		\return The new material info or null if there's already one
	*/
	CSLXSIMaterialInfo* CreateMaterialInfo();

	/*! Destroys the material info
		\return SI_Error Whether or not the material info could be destroyed
	*/
	SI_Error DestroyMaterialInfo();

	/*! Connects an exisiting material info
		\param in_pToConnect Pointer to the material info to use
		\return The added material info or null if there's already one
	*/
	CSLXSIMaterialInfo* ConnectMaterialInfo( CSLXSIMaterialInfo* in_pToConnect );

	/*! Gets a pointer to the material info
		\return CSLXSIMaterialInfo Pointer to the material info
	*/
	CSLXSIMaterialInfo* GetMaterialInfo();

// Members
private:
    CSIBCArray<CSLConnectionPoint*> m_ConnectionPoints;
    CSIBCArray<CSLXSIShader*> m_Shaders;
	CSLXSIMaterialInfo *m_pMaterialInfo;

	SI_Void *m_pReserved;	// reserved for future use
};

#endif // _XSIMATERIAL_H
