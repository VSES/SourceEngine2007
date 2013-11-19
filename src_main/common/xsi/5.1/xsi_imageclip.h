//*****************************************************************************
/*!
   \file xsi_imageclip.h
   \brief ImageClip class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIIMAGECLIP_H__
#define __XSIIMAGECLIP_H__

#include <xsi_projectitem.h>

namespace XSI {

//*****************************************************************************
/*! \class ImageClip xsi_imageclip.h
	\brief As of v4.0, this class is obsolete and is replaced by ImageClip2.
	\deprecated 4.0 (but plug-ins using this class will continue to run in XSI).
 */
//*****************************************************************************
class SICPPSDKDECL ImageClip : public ProjectItem
{
public:
	/*! Default constructor. */
	ImageClip();

	/*! Default destructor. */
	~ImageClip();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	ImageClip(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	ImageClip(const ImageClip& in_obj);

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
	ImageClip& operator=(const ImageClip& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new ImageClip object.
	*/
	ImageClip& operator=(const CRef& in_ref);

	private:
	ImageClip * operator&() const;
	ImageClip * operator&();
};

};

#endif // __XSIIMAGECLIP_H__
