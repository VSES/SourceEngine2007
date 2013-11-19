//*****************************************************************************
/*!
   \file xsi_image.h
   \brief Image class declaration.

   © Copyright 1998-2004 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIIMAGE_H__
#define __XSIIMAGE_H__
#include <xsi_base.h>

namespace XSI {

struct CColor ;

//*****************************************************************************
/*! \class Image xsi_image.h
	\brief This class provides read-access to the representation of an image in memory.

	For the vast majority of image files %XSI will represent the data in memory as uncompressed 8-bit RGBA.
	This is true even if the original file has no alpha information.  However, high quality 16-bit and floating
	point images can also be read via Image::GetPixelArray.  You can determine what format the image data
	contains by means of Image::GetNumChannels and Image::GetChannelSize.

	\sa ImageClip2::GetImage
    \since 4.0

	\eg Demonstrates how to create an image clip and access information about its underlying
		image source.
	\code
		using namespace XSI;
		Application app ;

		// Create an image clip based on one of the
		// image files from the XSI installation
		CString strImagePath ;
		strImagePath = app.GetInstallationPath( siFactoryPath );
		strImagePath += L"/Data/XSI_SAMPLES/Pictures/upper_eyelash.jpg" ;

		CValueArray args(2);
		CValue retVal;
		args[0] = strImagePath ;
		args[1] = L"MySmallClip" ;

		app.ExecuteCommand( L"CreateImageClip", args, retVal ) ;

		ImageClip2 newClip = retVal ;
		Image myImage = newClip.GetImage() ;

		app.LogMessage( L"Image has resolution " + CValue( myImage.GetResX() ).GetAsText() +
					L" by " + CValue( myImage.GetResY() ).GetAsText() ) ;

		app.LogMessage( L"Image has " + CValue( myImage.GetNumChannels() ).GetAsText() +
					L" channels and " + CValue( myImage.GetChannelSize() ).GetAsText() +
					L" bytes per channel" ) ;

		app.LogMessage( L"Total size of the uncompressed image data is "
					+ CValue( myImage.GetPixelArraySize() / 1024l ).GetAsText()
					+ L" kb" ) ;

		// Get the pixel value at (45, 25)
		unsigned char aColor[4] ;
		myImage.GetPixelValue( 45.0, 25.0, aColor[0], aColor[1], aColor[2], aColor[3] ) ;

		// Alternatively we can use the CColor object, which holds the color as
		// normalized doubles
		CColor myColor ;
		myImage.GetPixelValue( 45.0, 25.0, myColor ) ;

		// Expect results of running this example:
		// INFO : "Image has resolution 875 by 300"
		// INFO : "Image has 4 channels and 1 bytes per channel"
		// INFO : "Total size of the uncompressed image data is 1025 kb"
	\endcode
 */
//*****************************************************************************
class SICPPSDKDECL Image : public CBase
{
public:
	/*! Default constructor. */
	Image();

	/*! Default destructor. */
	~Image();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	Image(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	Image(const Image& in_obj);

	/*! Returns true if a given class type is compatible with this API class.
	\param in_ClassID class type.
	\return true if the class is compatible, false otherwise.
	*/
	bool IsA( siClassID in_ClassID) const;

	/*! Returns the type of the API class.
	\return The class type.
	*/
	siClassID GetClassID() const;

	/*! Creates an object from another object. The newly created object is set to
	empty if the input object is not compatible.
	\param in_obj constant class object.
	\return The new Image object.
	*/
	Image& operator=(const Image& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new Image object.
	*/
	Image& operator=(const CRef& in_ref);

	/*! Reads the color value at a specified pixel value in the image. When \c in_X or \c in_Y are not whole
	numbers Bilinear interpolation is used to determine a subpixel value.  The bottom left of the image
	is considered pixel \c 0,0.
	*/
	CStatus GetPixelValue( float in_X, float in_Y, CColor & out_Pixels ) const;

	/*! Alternative signature of GetPixelValue which retrieves the color of the pixel as 4 RGBA bytes.

  	Currently this function only works for 8-bit RGBA images. For other image formats it is possible to read
  	pixel values by calling Image::GetPixelArray.
	*/
	CStatus GetPixelValue(
				float in_X,
				float in_Y,
				unsigned char & out_R,
				unsigned char & out_G,
				unsigned char & out_B,
				unsigned char & out_A ) const;

	/*! Returns the resolution, in pixels, of the horizontal dimension.
	*/
	LONG GetResX() const;

	/*! Returns the resolution, in pixels, of the vertical dimension.
	*/
	LONG GetResY() const;

	/*! Returns the number of channels per pixel.  Normally this returns 4, representing RGBA. %XSI will often
	represent an image in memory with 4 channels even if the underlying file format has no alpha information.
	*/
	LONG GetNumChannels() const ;

	/*! Returns the number of bytes per channel.  Normally this returns 1, but 16-bit images will return 2 and
	32-bit float images will return 4.
	*/
	LONG GetChannelSize() const ;

	/*! Provides low level access to the image data. The returned buffer should not be modified and the pointer
	should not be cached outside the lifetime of the Image object. The data is formatted as an array of pixel
	color values, row by row.

	You can find the exact format of each pixel by calling Image::GetNumChannels and Image::GetChannelSize.
	In the normal case, each pixel is represented by 4 bytes.
	*/
	const void * GetPixelArray() const ;

	/*! Returns the total size, in bytes, of the array returned by Image::GetPixelArray. It is expected that this
	is equal to the product of Image::GetResX, Image::GetResY, Image::GetNumChannels and Image::GetChannelSize.
	*/
	LONG GetPixelArraySize() const ;

	private:
	Image * operator&() const;
	Image * operator&();
};

};

#endif // __XSIIMAGE_H__
