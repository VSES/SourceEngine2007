//*****************************************************************************
/*!
   \file xsi_ogllight.h
   \brief OGLLight class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIOGLLIGHT_H__
#define __XSIOGLLIGHT_H__

#include <xsi_base.h>
#include <xsi_math.h>
#include <xsi_color.h>

namespace XSI {

//*****************************************************************************
/*! \class OGLLight xsi_ogllight.h
	\brief The OGLLight object represents the OGL light properties of a Light object.

	OGLLight objects are read-only.

	\sa Light

  	\eg
	\code
		using namespace XSI;
		Application app;
		Model root = app.GetActiveSceneRoot();

		Light myLight;
		root.AddLight( L"Spot", false, L"", myLight);

		OGLLight myOGLLight(myLight.GetOGLLight());

		app.LogMessage( L"The light is of type : " + myOGLLight.GetType() );
	\endcode
 */
//*****************************************************************************

class SICPPSDKDECL OGLLight : public CBase
{
public:
	/*! Default constructor. */
	OGLLight();

	/*! Default destructor. */
	~OGLLight();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	OGLLight(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	OGLLight(const OGLLight& in_obj);

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
	\return The new OGLLight object.
	*/
	OGLLight& operator=(const OGLLight& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new OGLLight object.
	*/
	OGLLight& operator=(const CRef& in_ref);


	/*! Returns the type of light.
	\return The type of light.
	*/
	siLightType		GetType() const;

	/*! Returns the color for this light.
	\return The color of the light.
	*/
	CColor			GetColor() const;

	/*! Returns the vector position of the light.
	\return The vector position of the light.
	*/
	MATH::CVector3	GetLightPosition() const;

	/*! Returns the interest vector position of the light.
	\return The interest position vector of the light.
	*/
	MATH::CVector3	GetLightInterestPosition() const;

	/*! Returns the cone angle for a spot light.
	\return The cone angle for a spot light.
	*/
	double			GetConeAngle() const;

	private:
	OGLLight * operator&() const;
	OGLLight * operator&();

};

};

#endif // __XSIOGLLIGHT_H__
