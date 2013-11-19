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

#ifndef _XSIIMAGE_H
#define _XSIIMAGE_H

#include "Template.h"

class CSLImageData;
class CSLImageFX;
class CSLImageLibrary;

/** 
* Class XSIImage
*
* Describes an image
* \note This template was introduced in dotXSI Version 3.6
*/

class XSIEXPORT CSLImage
    : public CSLTemplate
{
public:
	//! Defines the number of channels of an image
	enum EChannels
	{
		SI_RGB = 3,		//!< red, green and blue channels>
		SI_RGBA = 4		//!< red, green, blue and alpha channels>
	};

	//! Defines the supported color depths for images
	enum EColorDepth
	{
		SI_24BITS = 24,	//!< 24 bits>
		SI_32BITS = 32	//!< 32 bits>
	};

	/*! Constructor
		\param in_pScene Parent scene
		\param in_pModel Parent model
		\param in_pTemplate Refered template
	*/
	CSLImage(CSLScene* in_pScene, CSLModel *in_pModel, CdotXSITemplate* in_pTemplate);

	//! Destructor
	virtual ~CSLImage();

	/*! Commits Semantic Layer data
		\return SI_Error Whether or not commit was a success
	*/
	SI_Error Synchronize();

	/*! Gets the type of this template
		\return ETemplateType The type of this template: XSI_IMAGE
	*/
	ETemplateType Type(){ return XSI_IMAGE; }

    virtual CSLAnimatableType* ParameterFromName(SI_Char *in_szName);

	virtual CSLAnimatableType* ParameterFromType(EFCurveType in_Type, SI_Char *in_szParameterName);

	/*! Gets the filename of the image
		\return SI_Char Filename
	*/
	SI_Char* GetSourceFile();

	/*! Sets the filename of the image
		\parame in_szFilename Filename
	*/
	SI_Void SetSourceFile( SI_Char* in_szFilename );

	/*! Gets the horizontal resolution of the image
		\return SI_Int Size
		\note A source file name and the internally stored CSLImageData class are mutually exclusive.  Setting the source file therefore deletes the image data on your behalf.
	*/
	SI_Int GetWidth();

	/*! Sets the horizontal resolution of the image
		\param in_nSize Size
	*/
	SI_Void SetWidth( SI_Int in_nSize );

	/*! Gets the vertical resolution of the image
		\return SI_Int Size
	*/
	SI_Int GetHeight();

	/*! Sets the vertical resolution of the image
		\param in_nSize Size
	*/
	SI_Void SetHeight( SI_Int in_nSize );

	/*! Gets the number of channels for a pixel
		\return EChannels Number of channels
	*/
	EChannels GetChannels();

	/*! Sets the number of channels for a pixel
		\param in_Channel Number of channels
	*/
	SI_Void SetChannels( EChannels in_Channel );

	/*! Gets the color depth of the image
		\return EColorDepth Color depth
	*/
	EColorDepth GetColorDepth();

	/*! Sets the color depth of the image
		\param in_Depth Color depth
	*/
	SI_Void SetColorDepth( EColorDepth in_Depth );

	/*! Gets the minimum horizontal crop value
		\return SI_Int Crop value
	*/
	SI_Float GetCropMinX();

	/*! Sets the minimum horizontal crop value
		\param in_fCrop Crop value
	*/
	SI_Void SetCropMinX( SI_Float in_fCrop );

	/*! Gets the maximum horizontal crop value
		\return SI_Float Crop value
	*/
	SI_Float GetCropMaxX();

	/*! Sets the maximum horizontal crop value
		\param in_fCropCrop value
	*/
	SI_Void SetCropMaxX( SI_Float in_fCrop );

	/*! Gets the minimum vertical crop value
		\return SI_Float Crop value
	*/
	SI_Float GetCropMinY();

	/*! Sets the minimum vertical crop value
		\param in_fCrop Crop value
	*/
	SI_Void SetCropMinY( SI_Float in_fCrop );

	/*! Gets the maximum vertical crop value
		\return SI_Float Crop value
	*/
	SI_Float GetCropMaxY();

	/*! Sets the maximum vertical crop value
		\param in_fCrop Crop value
	*/
	SI_Void SetCropMaxY( SI_Float in_fCrop );

	/*! Gets the frame rate
		\return SI_Float Actual frame rate
		\note A frame rate of 0 refers to the scene frame rate
	*/
	SI_Float GetFrameRate();

	/*! Sets the frame rate
		\param in_nRate Actual frame rate
		\note A frame rate of 0 refers to the scene frame rate
	*/
	SI_Void SetFrameRate( SI_Float in_nRate = 0.0 );

	/*! Gets the first frame
		\return SI_Int Frame number
	*/
	SI_Int GetFirstFrame();

	/*! Sets the first frame
		\param in_nFrame Frame number
	*/
	SI_Void SetFirstFrame( SI_Int in_nFrame );

	/*! Gets the last frame
		\return SI_Int Frame number
	*/
	SI_Int GetLastFrame();

	/*! Sets the last frame
		\param in_nFrame Frame number
	*/
	SI_Void SetLastFrame( SI_Int in_nFrame );

	///////////////////////////////////////////////////////////////////////////
	// Image data functionality ///////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	/*! Allocates and connect a new image data
		\return CSLImageData Newly allocated image data.  Null if there was already one.
	*/
	CSLImageData* CreateImageData();

	/*! Use a preallocated image data
		\param in_pImageData Pointer to the image data to use
		\return Pointer to the added data or null if there's already one
		\note A source file name and the internally stored CSLImageData class are mutually exclusive.  Setting an image data therefore clears the source file name.
	*/

	/*! Destroys the Image Data
		\return SI_Error Whether or not the image data could be destroyed
	*/
	SI_Error DestroyImageData();

	/*! Connects the image data
		\param in_pImageData New image data to use
		\note A source file name and the internally stored CSLImageData class are mutually exclusive.  Setting an image data therefore clears the source file name.
	*/
	CSLImageData* ConnectImageData( CSLImageData* in_pImageData );

	/*! Gets the image data
		\return CSLImageData Image data
		\note A source file name and the internally stored CSLImageData class are mutually exclusive.  Setting an image data therefore clears the source file name.
	*/
	CSLImageData* ImageData();
	

	///////////////////////////////////////////////////////////////////////////
	// Image FX Functionality /////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	/*! Creates a new Image FX
		\return The new ImageFX or null if there's already one
	*/
	CSLImageFX* CreateImageFX();

	/*! Destroys the Image FX
		\return SI_Error Whether or not the ImageFX could be destroyed
	*/
	SI_Error DestroyImageFX();

	/*! Connects an exisiting Image FX
		\param in_pToConnect Pointer to the ImageFX to use
		\return The added ImageFX or null if there's already one
	*/
	CSLImageFX* ConnectImageFX( CSLImageFX* in_pToConnect );

	/*! Gets a pointer to the Image FX
		\return CSLImageFX Pointer to the Image FX
	*/
	CSLImageFX* ImageFX();

private:
	CSLStringProxy m_SourceFile;
    CSLIntProxy m_SizeX;
    CSLIntProxy m_SizeY;
    CSLIntProxy m_Channels;
    CSLIntProxy m_BitsPerPixel;
    CSLFloatProxy m_CropMinX;
    CSLFloatProxy m_CropMaxX;
    CSLFloatProxy m_CropMinY;
    CSLFloatProxy m_CropMaxY;
    CSLFloatProxy m_FrameRate;
    CSLIntProxy m_FrameCount;
    CSLIntProxy m_FirstFrame;
    CSLIntProxy m_LastFrame;
	CSLImageData *m_pImageData;
	CSLImageFX *m_pImageFX;

	SI_Void *m_pReserved;
};

#endif
