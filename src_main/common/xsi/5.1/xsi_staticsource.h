//*****************************************************************************
/*!
   \file xsi_staticsource.h
   \brief StaticSource class declaration.

   © Copyright 1998-2003 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************
#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSISTATICSOURCE_H__
#define __XSISTATICSOURCE_H__

#include <xsi_siobject.h>

namespace XSI {

class CValue;
//*****************************************************************************
/*! \class StaticSource xsi_staticsource.h
	\brief The StaticSource object represents a constant value stored as an AnimationSourceItem. When instantiated,
	the parameter value takes on the value of the StaticSource. A static pose is a kind of a StaticSource.

	This object can be one of the AnimationSourceItem objects in an ActionSource.

	\sa AnimationSourceItem::GetSource, AnimationSourceItem::SetAsStatic, ActionSource::AddSourceItem
	\since 4.0

	\eg This example illustrates creating a source and accessing the source items contained in it.

	\code
		using namespace XSI;

		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject myCube;
		root.AddGeometry( L"Cube", L"MeshSurface", L"", myCube );

		// Create the first animation source
		CValueArray args(9);
		CValue		outArg;
		args[0] = root;
		args[1] = L"cube.kine.local.posx,cube.kine.local.posy,cube.kine.local.posz";
		args[3] = L"StoredStaticPose";
		args[4] = true;
		args[7] = false;
		args[8] = false;
		app.ExecuteCommand( L"StoreAction", args, outArg );

		ActionSource mySource(outArg);
		CRefArray aSourceItems = mySource.GetItems();

		for ( LONG i=0; i<aSourceItems.GetCount(); ++i ) {
			AnimationSourceItem currItem(aSourceItems[i]);
			app.LogMessage( L" The animated target is " + currItem.GetTarget() );
			app.LogMessage( L" The type of animation is " + currItem.GetType() );

			// In this case it's a static value you can switch on type.
			StaticSource myVal = currItem.GetSource();
			app.LogMessage( L" The value is " + myVal.GetValue().GetAsText() );
		}

	\endcode
 */
//*****************************************************************************
class SICPPSDKDECL StaticSource : public SIObject
{
public:
	/*! Default constructor. */
	StaticSource();

	/*! Default destructor. */
	~StaticSource();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	StaticSource(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	StaticSource(const StaticSource& in_obj);

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
	\return The new StaticSource object.
	*/
	StaticSource& operator=(const StaticSource& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new StaticSource object.
	*/
	StaticSource& operator=(const CRef& in_ref);

	/*! Returns a value which is used as a constant value for the corresponding AnimationSourceItem.
	\return Constant value for an animation source.
	*/
	CValue GetValue()const;

	/*! Sets a value which is used as a constant value for the corresponding AnimationSourceItem.
	\param in_val	New value for this animation SourceItem.
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus PutValue(const CValue& in_val);

	private:
	StaticSource * operator&() const;
	StaticSource * operator&();
};

}

#endif // __XSISTATICSOURCE_H__
