//*****************************************************************************
/*!
   \file xsi_lightrig.h
   \brief LightRig class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSILIGHTRIG_H__
#define __XSILIGHTRIG_H__

#include <xsi_rig.h>

namespace XSI {

//*****************************************************************************
/*! \class LightRig xsi_lightrig.h
	\brief The LightRig object is a specialized Rig object composed of a Light and a Null as its interest.
	Although a LightRig is typically defined with a spot light, it can be defined with any light preset.

	\sa Camera

  	\eg
	\code
		using namespace XSI;
		Application app;
		Model root = app.GetActiveSceneRoot();

		LightRig myLightRig;
		root.AddLightRig( L"Spot", L"MySpotRig", myLightRig );

		X3DObject spotInterest(myLightRig.GetLight().GetInterest());
		app.LogMessage( spotInterest.GetName() );
	\endcode
 */
//*****************************************************************************

class SICPPSDKDECL LightRig : public Rig
{
public:
	/*! Default constructor. */
	LightRig();

	/*! Default destructor. */
	~LightRig();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	LightRig(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	LightRig(const LightRig& in_obj);

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
	\return The new LightRig object.
	*/
	LightRig& operator=(const LightRig& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new LightRig object.
	*/
	LightRig& operator=(const CRef& in_ref);

	private:
	LightRig * operator&() const;
	LightRig * operator&();

	public:


	/*! Returns the Light object for this rig.
	*/
	Light GetLight();
};

};

#endif // __XSILIGHTRIG_H__
