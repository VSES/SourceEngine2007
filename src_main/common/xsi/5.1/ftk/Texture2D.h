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
* File: 		 Texture2D.h
* Creation date: January 15, 2002
* Purpose:		 Declaration of class 'CSLTexture2D'
*
\******************************************************************************/
#ifndef _TEXTVRE2D_H
#define _TEXTVRE2D_H

#include "Template.h"


/** 
* Describes a 2D texture.
*/
class XSIEXPORT CSLTexture2D
	: public CSLTemplate
{
public:
	/** 
	* This enum lists all mapping type supported by the texture 2D.
	*/
	enum EMappingType
	{
		SI_XY_PROJECTION,
		SI_XZ_PROJECTION,
		SI_YZ_PROJECTION,
		SI_UV_MAP,
		SI_UV_MAP_WRAPPED,
		SI_CYLINDRICAL_PROJECTION,
		SI_SPHERICAL_PROJECTION,
		SI_REFLECTION_MAP,
	};

	/** 
	* This enum lists all blending type supported by the texture 2D.
	*/
	enum EBlendingType
	{
		SI_ALPHA_MASK,
		SI_INTENSITY_MASK,
		SI_NO_MASK,
		SI_RGB_MODULATION,
	};

	CSLTexture2D(CSLScene* in_pScene, CSLModel *in_pModel, CdotXSITemplate* in_pTemplate);
	virtual ~CSLTexture2D();

	/*! Return the name of the texture's image file.
		\return SI_Char*	Image file name.
	*/
	SI_Char*			GetImageFileName() { return m_ImageName; }

	/*! Change the name of the texture's image file.
		\param	SI_Char*	New image file name.
		\return None
	*/
	SI_Void 			SetImageFileName(SI_Char* in_szValue) { m_ImageName = in_szValue; }

	/*! Return the mapping type of the texture.
		\return EMappingType	Mapping type.
	*/
	EMappingType		GetMappingType() { return m_MappingType; }

	/*! Change the mapping type of the texture.
		\param	EMappingType	New mapping type.
		\return None
	*/
	SI_Void 			SetMappingType(EMappingType in_Type) { m_MappingType = in_Type; }

	/*! Return the width in pixel of the image.
		\return SI_Int	Image width.
	*/
	SI_Int				GetImageWidth() { return m_Width; }

	/*! Change the width of the image.
		\param	SI_Int	New image width.
		\return None
	*/
	SI_Void 			SetImageWidth(SI_Int in_nValue) { m_Width = in_nValue; }

	/*! Return the height in pixel of the image.
		\return SI_Int	Image height.
	*/
	SI_Int				GetImageHeight() { return m_Height; }

	/*! Change the height of the image.
		\param	SI_Int	New image height.
		\return None
	*/
	SI_Void 			SetImageHeight(SI_Int in_nValue) { m_Height = in_nValue; }

	/*! Return the start of the cropping region in V.
		\return SI_Int	Start of the cropping region in V.
	*/
	SI_Int				GetCropUMin() { return m_CropUMin; }

	/*! Change the start of the cropping region in V.
		\param	SI_Int	New start of the cropping region in V.
		\return None
	*/
	SI_Void 			SetCropUMin(SI_Int in_nValue) { m_CropUMin = in_nValue; }

	/*! Return the end of the cropping region in V.
		\return SI_Int	End of the cropping region in V.
	*/
	SI_Int				GetCropUMax() { return m_CropUMax; }

	/*! Change the end of the cropping region in V.
		\param	SI_Int	New end of the cropping region in V.
		\return None
	*/
	SI_Void 			SetCropUMax(SI_Int in_nValue) { m_CropUMax = in_nValue; }

	/*! Return the start of the cropping region in V.
		\return SI_Int	Start of the cropping region in V.
	*/
	SI_Int				GetCropVMin() { return m_CropVMin; }

	/*! Change the start of the cropping region in V.
		\param	SI_Int	New start of the cropping region in V.
		\return None
	*/
	SI_Void 			SetCropVMin(SI_Int in_nValue) { m_CropVMin = in_nValue; }

	/*! Return the end of the cropping region in V.
		\return SI_Int	End of the cropping region in V.
	*/
	SI_Int				GetCropVMax() { return m_CropVMax; }

	/*! Change the end of the cropping region in V.
		\param	SI_Int	New end of the cropping region in V.
		\return None
	*/
	SI_Void 			SetCropVMax(SI_Int in_nValue) { m_CropVMax = in_nValue; }

	/*! Indicates if the U and V orientations are swapped.
		\return SI_Bool UV swap state.
	*/
	SI_Bool 			GetUVSwapFlag() { return m_UVSwap; }

	/*! Change the U and V orientations swapping state.
		\param	SI_Bool New UV swap state.
		\return None
	*/
	SI_Void 			SetUVSwapFlag(SI_Bool in_bState) { m_UVSwap = in_bState; }

	/*! Return the number of horizontal repetitions of the image.
		\return SI_Int	Number of horizontal repetitions.
	*/
	SI_Int				GetURepeat() { return m_URepeat; }

	/*! Change the number of horizontal repetitions of the image.
		\param	SI_Int	New number of horizontal repetitions.
		\return None
	*/
	SI_Void 			SetURepeat(SI_Int in_nValue) { m_URepeat = in_nValue; }

	/*! Return the number of vertical repetitions of the image.
		\return SI_Int	Number of vertical repetitions.
	*/
	SI_Int				GetVRepeat() { return m_VRepeat; }

	/*! Change the number of vertical repetitions of the image.
		\param	SI_Int	New number of vertical repetitions.
		\return None
	*/
	SI_Void 			SetVRepeat(SI_Int in_nValue) { m_VRepeat = in_nValue; }

	/*! Return the flag that indicates if the texture is mirrored horizontally.
		\return SI_Bool U alternate state.
	*/
	SI_Bool 			GetUAlternate() { return m_UAlternate; }

	/*! Change the flag that indicates if the texture is mirrored horizontally.
		\param	SI_Bool New U alternate state.
		\return None
	*/
	SI_Void 			SetUAlternate(SI_Bool in_bState) { m_UAlternate = in_bState; }

	/*! Return the flag that indicates if the texture is mirrored horizontally.
		\return SI_Bool V alternate state.
	*/
	SI_Bool 			GetVAlternate() { return m_VAlternate; }

	/*! Change the flag that indicates if the texture is mirrored horizontally.
		\param	SI_Bool New V alternate state.
		\return None
	*/
	SI_Void 			SetVAlternate(SI_Bool in_bState) { m_VAlternate = in_bState; }

	/*! Return the scaling of image in U.
		\return SI_Float	Scaling of image in U.
	*/
	SI_Float			GetUScale() { return m_UScale; }

	/*! Change the scaling of image in U.
		\param	SI_Float	New scaling of image in U.
		\return None
	*/
	SI_Void 			SetUScale(SI_Float in_fValue) { m_UScale = in_fValue; }

	/*! Return the scaling of image in V.
		\return SI_Float	Scaling of image in V.
	*/
	SI_Float			GetVScale() { return m_VScale; }

	/*! Change the scaling of image in V.
		\param	SI_Float	New scaling of image in V.
		\return None
	*/
	SI_Void 			SetVScale(SI_Float in_fValue) { m_VScale = in_fValue; }

	/*! Return the offset of image in U.
		\return SI_Float	Offset of image in U.
	*/
	SI_Float			GetUOffset() { return m_UOffset; }

	/*! Change the offset of image in U.
		\param	SI_Float	New offset of image in U.
		\return None
	*/
	SI_Void 			SetUOffset(SI_Float in_fValue) { m_UOffset = in_fValue; }

	/*! Return the offset of image in V.
		\return SI_Float	Offset of image in V.
	*/
	SI_Float			GetVOffset() { return m_VOffset; }

	/*! Change the offset of image in V.
		\param	SI_Float	New offset of image in V.
		\return None
	*/
	SI_Void 			SetVOffset(SI_Float in_fValue) { m_VOffset = in_fValue; }

	/*! Return the projection matrix of the texture.
		\return CSIBCMatrix4x4	projection matrix.
	*/
	CSIBCMatrix4x4		GetProjectionMatrix() { return m_ProjectionMatrix; }

	/*! Change the projection matrix of the texture.
		\param	CSIBCMatrix4x4& New projection matrix.
		\return None
	*/
	SI_Void 			SetProjectionMatrix(CSIBCMatrix4x4& in_Value) { m_ProjectionMatrix = in_Value; }

	/*! Return the type of blending between texture and material attributes.
		\return EBlendingType	Blending type.
	*/
	EBlendingType		GetBlendingType() { return m_BlendingType; }

	/*! Change the type of blending between texture and material attributes.
		\param	EBlendingType	New blending type.
		\return None
	*/
	SI_Void 			SetBlendingType(EBlendingType in_Type) { m_BlendingType = in_Type; }

	/*! Return the normalized contribution of texture attributes (ambient, diffuse, specular, transparency, reflectivity).
		\return SI_Float	Contribution of texture attributes.
	*/
	SI_Float			GetBlending() { return m_Blending; }

	/*! Change the normalized contribution of texture attributes.
		\param	SI_Float	New contribution of texture attributes.
		\return None
	*/
	SI_Void 			SetBlending(SI_Float in_fValue) { m_Blending = in_fValue; }

	/*! Return the normalized contribution of texture pixel colors to the material ambient color.
		\return SI_Float	Contribution of texture.
	*/
	SI_Float			GetAmbient() { return m_Ambient; }

	/*! Change the normalized contribution of texture pixel colors to the material ambient color.
		\param	SI_Float	New contribution of texture.
		\return None
	*/
	SI_Void 			SetAmbient(SI_Float in_fValue) { m_Ambient = in_fValue; }

	/*! Return the normalized contribution of texture pixel colors to the material diffuse color.
		\return SI_Float	Contribution of texture.
	*/
	SI_Float			GetDiffuse() { return m_Diffuse; }

	/*! Change the normalized contribution of texture pixel colors to the material diffuse color.
		\param	SI_Float	New contribution of texture.
		\return None
	*/
	SI_Void 			SetDiffuse(SI_Float in_fValue) { m_Diffuse = in_fValue; }

	/*! Return the normalized contribution of texture pixel colors to the material specular color.
		\return SI_Float	Contribution of texture.
	*/
	SI_Float			GetSpecular() { return m_Specular; }

	/*! Change the normalized contribution of texture pixel colors to the material specular color.
		\param	SI_Float	New contribution of texture.
		\return None
	*/
	SI_Void 			SetSpecular(SI_Float in_fValue) { m_Specular = in_fValue; }

	/*! Return the normalized contribution of texture pixel colors to the material transparency level.
		\return SI_Float	Contribution of texture.
	*/
	SI_Float			GetTransparency() { return m_Transparency; }

	/*! Change the normalized contribution of texture pixel colors to the material transparency level.
		\param	SI_Float	New contribution of texture.
		\return None
	*/
	SI_Void 			SetTransparency(SI_Float in_fValue) { m_Transparency = in_fValue; }

	/*! Return the normalized contribution of texture pixel colors to the material reflectivity level.
		\return SI_Float	Contribution of texture.
	*/
	SI_Float			GetReflectivity() { return m_Reflectivity; }

	/*! Change the normalized contribution of texture pixel colors to the material reflectivity level.
		\param	SI_Float	New contribution of texture.
		\return None
	*/
	SI_Void 			SetReflectivity(SI_Float in_fValue) { m_Reflectivity = in_fValue; }

	/*! Return the bump mapping intensity and/or displacement of geometry along surface normals.
		\return SI_Float	Bump mapping intensity.
	*/
	SI_Float			GetRoughness() { return m_Roughness; }

	/*! Change the bump mapping intensity.
		\param	SI_Float	New bump mapping intensity.
		\return None
	*/
	SI_Void 			SetRoughness(SI_Float in_fValue) { m_Roughness = in_fValue; }

	virtual ETemplateType Type() { return SI_TEXTURE_2D; }

private:
	CSLStringProxy		m_ImageName;
    CSLEnumProxy<EMappingType, SI_REFLECTION_MAP>	m_MappingType;
	CSLIntProxy 		m_Width;
	CSLIntProxy 		m_Height;
	CSLIntProxy 		m_CropUMin;
	CSLIntProxy 		m_CropUMax;
	CSLIntProxy 		m_CropVMin;
	CSLIntProxy 		m_CropVMax;
	CSLBoolProxy		m_UVSwap;
	CSLIntProxy 		m_URepeat;
	CSLIntProxy 		m_VRepeat;
	CSLBoolProxy		m_UAlternate;
	CSLBoolProxy		m_VAlternate;
	CSLFloatProxy		m_UScale;
	CSLFloatProxy		m_VScale;
	CSLFloatProxy		m_UOffset;
	CSLFloatProxy		m_VOffset;
	CSLMatrix4x4Proxy	m_ProjectionMatrix;
    CSLEnumProxy<EBlendingType, SI_RGB_MODULATION>	m_BlendingType;
	CSLFloatProxy		m_Blending;
	CSLFloatProxy		m_Ambient;
	CSLFloatProxy		m_Diffuse;
	CSLFloatProxy		m_Specular;
	CSLFloatProxy		m_Transparency;
	CSLFloatProxy		m_Reflectivity;
	CSLFloatProxy		m_Roughness;

	void *m_pReserved;	// reserved for future extension
};

#endif
