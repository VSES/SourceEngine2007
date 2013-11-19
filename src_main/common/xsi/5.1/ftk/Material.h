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
* File:          Material.h
* Creation date: January 15, 2002
* Purpose:       Declaration of class 'Material' & 'BaseMaterial'
*
\******************************************************************************/
#ifndef _MATERIAL_H
#define _MATERIAL_H

#include "Template.h"

// Forward declaration
class CSLTexture2D;


/** 
* CSLBaseMaterial Class
*
* The CSLBaseMaterial defines a base material. From this base material, cast it in the 
* overridden class either CSLMaterial or CSLXSIMaterial using GetType();
*/
class XSIEXPORT CSLBaseMaterial
	: public CSLTemplate
{
public:
    virtual SI_Error Synchronize();

	virtual SI_Error GetCapabilities ( SI_Int in_iFeature, SI_Void* out_pValue );

protected:

	/*! Constructor
		\param in_pScene	Pointer to the scene containing the BaseMaterial
		\param in_pModel	Pointer to the model containing the BaseMaterial
		\param in_pTemplate	Pointer to the CdotXSITemplate containing the BaseMaterial
	*/
    CSLBaseMaterial(CSLScene* in_pScene, CSLModel *in_pModel, CdotXSITemplate* in_pTemplate);

private:
	void *m_pReserved;	// reserved for future extension
};

/** 
* CSLMaterial Class
*
* Defines a legacy SI_Material 
*/
class XSIEXPORT CSLMaterial
    : public CSLBaseMaterial
{
public:
	/** 
	* This enum lists possible shading models for a material.
	*/
	enum EShadingModel
	{
		CONSTANT,		/*!< Constant shading*/
		LAMBERT,		/*!< Lambert shading*/
		PHONG,			/*!< Phong shading*/
		BLINN,			/*!< Blinn shading*/
		SHADOW_OBJECT,	/*!< Shawdow object*/
		VERTEX_COLOR,	/*!< Vertex color lighting*/
	};

	/*! Constructor
		\param in_pScene	Pointer to the scene containing the Material
		\param in_pModel	Pointer to the model containing the Material
		\param in_pTemplate	Pointer to the CdotXSITemplate containing the Material
	*/
    CSLMaterial(CSLScene* in_pScene, CSLModel *in_pModel, CdotXSITemplate* in_pTemplate);

    /*! Destructor
	*/
	virtual ~CSLMaterial();

	/*!	Return the ambient color of the material.
		\return	CSIBCColorf	Ambient color.
	*/
	CSIBCColorf GetAmbientColor();

	/*!	Change the ambient color of the material.
		\param	CSIBCColorf&	New ambient color.
		\return	None
	*/
	SI_Void SetAmbientColor(CSIBCColorf &in_rColor);

	/*!	Return the diffuse color of the material.
		\return	CSIBCColorf	Diffuse color.
	*/
	CSIBCColorf GetDiffuseColor();

	/*!	Change the diffuset color of the material.
		\param	CSIBCColorf&	New diffuse color.
		\return	None
	*/
	SI_Void SetDiffuseColor(CSIBCColorf &in_rColor);

	/*!	Return the emissive color of the material.
		\return	CSIBCColorf	emissive color.
	*/
	CSIBCColorf GetEmissiveColor();

	/*!	Change the emissive color of the material.
		\param	CSIBCColorf&	New emissive color.
		\return	None
	*/
	SI_Void SetEmissiveColor(CSIBCColorf &in_rColor);

	/*!	Return the shading model of the material.
		\return	EShadingModel	Shading model.
	*/
	EShadingModel GetShadingModel();

	/*!	Change the shading model of the material.
		\param	EShadingModel	New shading model.
		\return	None
	*/
	SI_Void SetShadingModel(EShadingModel in_Value);

	/*!	Return the specular color of the material.
		\return	CSIBCColorf	Specular color.
	*/
	CSIBCColorf GetSpecularColor();

	/*!	Change the specular color of the material.
		\param	CSIBCColorf&	New specular color
		\return	None
	*/
	SI_Void SetSpecularColor(CSIBCColorf &in_rColor);

	/*!	Return the specular decay of the material.
		\return	SI_Float	Specular decay.
	*/
	SI_Float GetSpecularDecay();

	/*!	Change the specular decay of the material.
		\param	SI_Float	New Specular decay.
		\return	None
	*/
	SI_Void SetSpecularDecay(SI_Float in_fValue);

	/*!	Return the 2D texture of the material.
		\return	CSLTexture2D*	Pointer to a texture 2D object.
	*/
    CSLTexture2D* Texture2D();

	/*!	Create a new texture 2D for the material.
		\return	CSLTexture2D	Newly creates texture 2D object.
	*/
    CSLTexture2D* CreateTexture2D();

	/*!	Destroy the texture 2D of the material.
		\return	SI_Error	Error code.
	*/
	SI_Error DestroyTexture2D();

    CSLTexture2D* ConnectTexture2D(CSLTexture2D* in_pNewTexture2D);

    virtual SI_Error Synchronize();
    virtual ETemplateType Type();
    virtual CSLAnimatableType* ParameterFromName(SI_Char *in_szName);
    virtual CSLAnimatableType* ParameterFromType(EFCurveType in_Type, SI_Char *in_szParameterName);

private:
    CSLColorRGBAProxy m_DiffuseColor;
    CSLFloatProxy m_SpecularDecay;
    CSLColorRGBProxy m_SpecularColor;
    CSLColorRGBProxy m_EmissiveColor;
    CSLEnumProxy<EShadingModel, VERTEX_COLOR> m_ShadingModel;
    CSLColorRGBProxy m_AmbientColor;
    CSLTexture2D *m_pTexture2D;

	void *m_pReserved;	// reserved for future extension
};

#endif
