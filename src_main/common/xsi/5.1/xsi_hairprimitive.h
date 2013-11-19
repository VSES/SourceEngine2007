//*****************************************************************************
/*!
   \file xsi_hairprimitive.h
   \brief HairPrimitive class declaration.

   © Copyright 1998-2005 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIHAIRPRIMITIVE_H__
#define __XSIHAIRPRIMITIVE_H__

#include <xsi_primitive.h>
#include <xsi_renderhairaccessor.h>
#include <xsi_status.h>

namespace XSI {

//*****************************************************************************
/*! \class HairPrimitive xsi_hairprimitive.h
	\brief This class represents the primitive of an X3DObject used for simulating hair in XSI and can be
	accessed with X3DObject::GetActivePrimitive.

	HairPrimitive provides access to the CRenderHairAccessor object which allows you to extract the render
	hair data from the primitive.

	\sa CRenderHairAccessor
	\since 5.0

	\eg
	\code
		using namespace XSI;

		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject myCube;
		root.AddGeometry( L"Cube", L"MeshSurface",L"myCube",myCube);

		CValueArray args(1);
		CValue outArg;
		args[0] = myCube;
		app.ExecuteCommand( L"ApplyHairOp", args, outArg ) ;

		X3DObject myHair = myCube.FindChild( L"Hair", L"", CStringArray() );

		HairPrimitive myHairPrim = myHair.GetActivePrimitive();

		bool bHair = myHairPrim.IsValid() && myHairPrim.IsA( siHairPrimitiveID ) == true;
		app.LogMessage( L"Found Hair object: " + CValue(bHair).GetAsTest() );
  	\endcode
*/
//*****************************************************************************

class SICPPSDKDECL HairPrimitive : public Primitive
{
public:
	/*! Default constructor. */
	HairPrimitive();

	/*! Default destructor. */
	~HairPrimitive();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	HairPrimitive(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	HairPrimitive(const HairPrimitive& in_obj);

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
	\return The new HairPrimitive object.
	*/
	HairPrimitive& operator=(const HairPrimitive& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new HairPrimitive object.
	*/
	HairPrimitive& operator=(const CRef& in_ref);

	/*! Returns a render hair accessor for this primitive. The render hair accessor generates a renderable
	representation of a groomed hair primitive. The render hair is iterated over in chunks, rather than all
	at once, to avoid problems with excessive memory consumption.

	\param in_nRequestedHairCount Maximum number of hairs desired. The actual number can be less depending
		on cut and density map settings.
	\param in_nChunksize Number of hairs to generate on each iteration. By default the chunk size is set to 10000.
	\param in_time Time (in frames) at which to get the accessor. The default value is DBL_MAX which means
		the current time in frames.

	\return The new CRenderHairAccessor object.
	\return Invalid if in_nRequestedHairCount is 0 or in_nChunksize is larger than in_nRequestedHairCount.

	\sa CRenderHairAccessor
	*/
	CRenderHairAccessor GetRenderHairAccessor(
		LONG in_nRequestedHairCount,
		LONG in_nChunksize = 10000,
		double in_time = DBL_MAX ) const;

	private:
	HairPrimitive * operator&() const;
	HairPrimitive * operator&();
};

};
#endif // __XSIHAIRPRIMITIVE_H__
