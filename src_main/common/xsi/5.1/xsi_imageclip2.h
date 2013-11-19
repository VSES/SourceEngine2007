//*****************************************************************************
/*!
   \file xsi_imageclip2.h
   \brief ImageClip2 class declaration.

   © Copyright 1998-2003 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIIMAGECLIP2_H__
#define __XSIIMAGECLIP2_H__

#include <xsi_clip.h>

namespace XSI {

class Image ;

//*****************************************************************************
/*! \class ImageClip2 xsi_imageclip2.h
	\brief An ImageClip2 object is an instance of an image Source and is used as the container for
	applying composited image effects.

	You can get the Source object corresponding to the ImageClip2 using Clip::Source. Applications
	never create image clips explicitly. Instead, they are created by calling the \xl AddImageClip command.

    \note In order to fix a class hierarchy problem, this class was introduced in v4.0. Although the class
    	name is ImageClip2 it corresponds to the object model \xl %ImageClip object and its class id returned
    	by CBase::GetClassID is ::siImageClipID.

	\sa Source, Material::GetCurrentImageClip2
    \since 4.0

	\eg Illustrates how to get the file name, X and Y resolution from an ImageClip2 object by
		getting the Source corresponding to the image clip.

	\code
		using namespace XSI;
		Application app;
		CString strImagePath = app.GetInstallationPath(siFactoryPath) ;
		strImagePath += L"Data/XSI_SAMPLES/Pictures/circlelight3.tga" ;

		CValueArray args(1);
		CValue valImageClip2;
		args[0] = strImagePath ;

		app.ExecuteCommand( L"CreateImageClip", args, valImageClip2 );

		ImageClip2 clip( valImageClip2 );

		//retreiving the image source
		Source clipSource(clip.GetSource());

		CValue fileName = clipSource.GetParameterValue( L"path");
		CValue XRes = clipSource.GetParameterValue( L"XRes");
		CValue YRes = clipSource.GetParameterValue( L"YRes");

		app.LogMessage(L"The image file is : " + fileName.GetAsText());
		app.LogMessage(L"The image x resolution is : " + XRes.GetAsText());
		app.LogMessage(L"The image y resolution is : " + YRes.GetAsText());
	\endcode
 */
//*****************************************************************************
class SICPPSDKDECL ImageClip2 : public Clip
{
public:
	/*! Default constructor. */
	ImageClip2();

	/*! Default destructor. */
	~ImageClip2();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	ImageClip2(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	ImageClip2(const ImageClip2& in_obj);

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
	\return The new ImageClip object.
	*/
	ImageClip2& operator=(const ImageClip2& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new ImageClip2 object.
	*/
	ImageClip2& operator=(const CRef& in_ref);

	/*! Provides access to the image data.
	\note For performance considerations you should only call this function when the plug-in intends to read
		actual pixel values. The resolution can be determined more quickly via the Source object, as demonstrated
		in the example above.

	\param in_dblFrame This optional argument permits retrieval of any frame within an image sequence or movie
		file. If not specified, the frame pulled is the frame corresponding to the current play position at the
		time the ImageClip2 object was instantiated.

	\return The requested Image data
	*/
	Image GetImage( double in_dblFrame = DBL_MAX ) ;

	/*! Returns a smaller version of the Image. For circumstances where the full resolution of an image is not
	necessary, you can improve performance by requesting a reduced image. If you are only generating a thumbnail
	it is overkill to read millions of pixels from a film resolution image.

	\note The user preferences \c preferences.rendering.maximum_proxy_level and \c preferences.rendering.minimum_proxy_size
		can influence the results of this call. The image returned may be larger than requested.

	\param in_Ratio  Requested ratio for reduction.
	\param in_dblFrame This optional argument permits retrieval of any frame within an image sequence or movie
		file. If not specified, the frame pulled is the frame corresponding to the current play position at the
		time the ImageClip2 object was instantiated.

	\return Image representing the reduced image
	*/
	Image GetScaledDownImage( LONG in_Ratio, double in_dblFrame = DBL_MAX ) ;

	/*! Returns the image file name for this clip.
	\return The requested image file name.
	\since 5.0
	*/
	CString GetFileName() ;

	private:
	ImageClip2 * operator&() const;
	ImageClip2 * operator&();
};

};

#endif // __XSIIMAGECLIP2_H__
