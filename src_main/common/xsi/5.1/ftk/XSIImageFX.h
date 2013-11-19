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
#ifndef _XSIIMAGEFX_H
#define _XSIIMAGEFX_H

#include "Template.h"
class CSLImage;

/** 
* Class CSLImageFX
*
* Defines image transformation effects
* \note This template was introduced in dotXSI Version 3.6
*/

class XSIEXPORT CSLImageFX
    : public CSLTemplate
{
public:
	//! Defines the various scaling types
	enum EScalingType
	{
		SI_NONE,	/*!< 0-none */
		SI_FULL,	/*!< 1-fullsize */
		SI_HALF,	/*!< 2-halfsize */
		SI_QUARTER,	/*!< 3-quartersize */
		SI_128,		/*!< 4-128 X 128 */
		SI_256,		/*!< 5-256 X 256 */
		SI_512,		/*!< 6-512 X 512 */
		SI_CUSTOM,	/*!< 7- Custom (use scale X and scale Y) */
	};

	/*! Constructor
		\param in_pScene Parent scene
		\param in_pModel Parent model
		\param in_pTemplate Refered template
	*/
	CSLImageFX(CSLScene* in_pScene, CSLModel *in_pModel, CdotXSITemplate* in_pTemplate);

	//! Destructor
	virtual ~CSLImageFX();

	/*! Gets the type of this template
		\return ETemplateType The type of this template (XSI_IMAGE_FX)
	*/
	ETemplateType Type(){ return XSI_IMAGE_FX; }

    virtual CSLAnimatableType* ParameterFromName(SI_Char *in_szName);

	virtual CSLAnimatableType* ParameterFromType(EFCurveType in_Type, SI_Char *in_szParameterName);

	/*! Get the Hue value
		\return SI_Float	Hue
	*/
	SI_Float GetHue();

	/*! Set the Hue value
		\param	SI_Float	new Hue value
	*/
	SI_Void SetHue( SI_Float in_fValue );

	/*! Get the Gain value
		\return SI_Float	Gain
	*/
	SI_Float GetGain();

	/*! Set the Gain value
		\param	SI_Float	new Gain value
	*/
	SI_Void SetGain( SI_Float in_fValue );

	/*! Get the Saturation value
		\return SI_Float	Saturation
	*/
	SI_Float GetSaturation();

	/*! Set the Saturation value
		\param	SI_Float	new Saturation value
	*/
	SI_Void SetSaturation( SI_Float in_fValue );

	/*! Get the Brightness value
		\return SI_Float	Brightness
	*/
	SI_Float GetBrightness();

	/*! Set the Brightness value
		\param	SI_Float	new Brightness value
	*/
	SI_Void SetBrightness( SI_Float in_fValue );

	/*! Get the Blur radius value
		\return SI_Float	Blur radius
	*/
	SI_Float GetBlurRadius();
	
	/*! Set the Blur radius value
		\param	SI_Float	new Blur radius value
	*/
	SI_Void SetBlurRadius( SI_Float in_fValue );

	/*! Get the Blur amount value
		\return SI_Float	Blur amount
	*/
	SI_Float GetBlurAmount();

	/*! Set the Blur amount value
		\param	SI_Float	new Blur amount value
	*/
	SI_Void SetBlurAmount( SI_Float in_fValue );

	/*! Get the Blur alpha value
		\return SI_Float	Blur alpha
	*/
	SI_Bool GetBlurAlpha();

	/*! Set the Blur Alpha value
		\param	SI_Float	new Blur Alpha value
	*/
	SI_Void SetBlurAlpha( SI_Bool in_bValue );

	/*! Get the scaling type
		\return EScalingType	scaling type
	*/
	EScalingType GetScalingType();

	/*! Set the Scaling type value
		\param	EScalingType the new scaling type
	*/
	SI_Void SetScalingType( EScalingType in_Type );

	/*! Get the horizontal scale value
		\return SI_Float	horizontal scale
	*/
	SI_Float GetHorizontalScale();

	/*! Set the horizontal scale value
		\param SI_Float	horizontal scale
	*/
	SI_Void SetHorizontalScale( SI_Float in_fValue );

	/*! Get the vertical scale value
		\return SI_Float	vertical scale
	*/
	SI_Float GetVerticalScale();

	/*! Set the vertical scale value
		\param SI_Float	vertical scale
	*/
	SI_Void SetVerticalScale( SI_Float in_fValue );

	/*! Get a value indicating wether the image is horizontally flipped
		\return SI_Bool	Horitonzal flip value
	*/
	SI_Bool GetFlipHorizontal();
	
	/*! Set a value indicating wether the image is horizontally flipped
		\param SI_Bool	Horitonzal flip value
	*/
	SI_Void SetFlipHorizontal( SI_Bool in_bValue );

	/*! Get a value indicating wether the image is vertically flipped
		\return SI_Bool	Vertical flip value
	*/
	SI_Bool GetFlipVertical();

	/*! Set a value indicating wether the image is vertically flipped
		\param SI_Bool	Vertical flip value
	*/
	SI_Void SetFlipVertical( SI_Bool in_bValue );

	/*! Get a value indicating wether the image is converted to grayscale
		\return SI_Bool	converted to grayscale
	*/
	SI_Bool GetRGBA2Greyscale();

	/*! Set a value indicating wether the image is converted to grayscale
		\param SI_Bool	converted to grayscale
	*/
	SI_Void SetRGBA2Greyscale( SI_Bool in_bValue );

	/*! Get a value indicating wether the image is converted to 16bit
		\return SI_Bool	converted to 16bit
	*/
	SI_Bool GetConvertTo16Bits();

	/*! Set a value indicating wether the image is converted to 16bit
		\param SI_Bool	converted to 16bit
	*/
	SI_Void SetConvertTo16Bits( SI_Bool in_bValue );

private:
	CSLFloatProxy m_Hue;
    CSLFloatProxy m_Gain;
    CSLFloatProxy m_Saturation;
    CSLFloatProxy m_Brightness;
    CSLFloatProxy m_BlurRadius;
    CSLFloatProxy m_BlurAmount;
    CSLBoolProxy m_BlurAlpha;
    CSLEnumProxy<EScalingType, SI_CUSTOM> m_ScalingType;
    CSLFloatProxy m_ScaleX;
    CSLFloatProxy m_ScaleY;
    CSLBoolProxy m_FlipHorizontal;
    CSLBoolProxy m_FlipVertical;
    CSLBoolProxy m_RGBA2Greyscale;
    CSLBoolProxy m_BitsPerChannel;

	SI_Void *m_pReserved;
};

#endif
