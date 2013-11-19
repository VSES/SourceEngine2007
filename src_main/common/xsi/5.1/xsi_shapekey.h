//*****************************************************************************
/*!
   \file xsi_shapekey.h
   \brief ShapeKey class declaration.

   © Copyright 1998-2003 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************
#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSISHAPEKEY_H__
#define __XSISHAPEKEY_H__

#include <xsi_clusterproperty.h>

namespace XSI {


//*****************************************************************************
/*! \class ShapeKey xsi_shapekey.h
	\brief A ShapeKey represents a special ClusterProperty which is used to store a specific
	geometry. ShapeKeys provide information about how the shape of a cluster changes by either
	storing its absolute value, an offset based on the object referential or values base on the
	local reference frame of vertices.

	You can find out the key's reference mode by getting the value of the KeyType parameter.
	To convert from one reference mode to the other, use the \xl ConvertShapeReferenceMode command.

	\warning The KeyType parameter uses a different set of values from the ::siShapeReferenceMode
		enum. For more information, see \xl ShapeAnimation.

	Because a ShapeKey modifies scene content when instantiated, it is an AnimationSource and as such
	can be accessed by using AnimationSourceItem::GetSource.

	\sa AnimationSourceItem::GetSource, ActionSource::GetItems, ShapeClip, ActionSource,
		Clip::GetSource, \xl StoreShapeKey, \xl SelectShapeKey

	\since 4.0

	\eg This example illustrates how to create a ShapeKey

	\code
		using namespace XSI;
		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject myCube;
		root.AddGeometry( L"Cube", L"MeshSurface", L"", myCube );

		// Creating the first animation source
		CValueArray args(3);
		CValue		outArg;
		args[0] = myCube;
		args[1] = L"myShapeKey";
		args[2] = (LONG)siShapeAbsoluteReferenceMode;
		app.ExecuteCommand( L"StoreShapeKey", args, outArg );

		ShapeKey myShapeKey(outArg);
		app.LogMessage(L"ShapeKey " + myShapeKey.GetFullName());

	\endcode
 */
//*****************************************************************************
class SICPPSDKDECL ShapeKey : public ClusterProperty
{
public:
	/*! Default constructor. */
	ShapeKey();

	/*! Default destructor. */
	~ShapeKey();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	ShapeKey(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	ShapeKey(const ShapeKey& in_obj);

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
	\return The new ShapeKey object.
	*/
	ShapeKey& operator=(const ShapeKey& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new ShapeKey object.
	*/
	ShapeKey& operator=(const CRef& in_ref);

	/*! Returns the name of the shape group to which the shape key object belongs.
	Shape groups are a way to semantically organize shape keys within a given cluster.
	This organization is visible within the shape manager view.
	\since 5.0
	*/
	CString GetShapeGroupName()const;

	/*! Sets the name of the shape group to which the shape key object belongs.
	Shape groups are a way to semantically organize shape keys within a given cluster.
	This organization is visible within the shape manager view.

	\note If you attempt to assign a non-existing shape group name to a shape key, a new shape group
		will be created.

	\param in_shapeGroupName name of the shape group to which you want to associate the shape key.

	\since 5.0
	*/
	CStatus PutShapeGroupName(const CString& in_shapeGroupName);

	private:
	ShapeKey * operator&() const;
	ShapeKey * operator&();
};

}

#endif // __XSISHAPEKAY_H__
