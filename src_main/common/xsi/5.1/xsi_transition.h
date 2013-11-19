//*****************************************************************************
/*!
   \file xsi_transition.h
   \brief Transition class declaration.

   © Copyright 1998-2003 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************
#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSITRANSITION_H__
#define __XSITRANSITION_H__

#include <xsi_projectitem.h>

namespace XSI {

class Clip;
//*****************************************************************************
/*! \class Transition xsi_transition.h
	\brief This object represents a transition between two clips. A transition is an interpolation used
	to smooth passing from the end of one clip to the beginning of another.

	You can get at the Transition object from the ClipContainer::GetTransitions function. Calling SIObject::GetType
	on the Transition object returns one of the strings defined under the siTransitionType.

	\sa Track, Clip, Model::GetMixer, ClipContainer, \xl AddTransition
	\since 4.0

	\eg Demonstrates how to create a Transition between two clips.

	\code
		using namespace XSI;

		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject myCube;
		root.AddGeometry( L"Cube", L"MeshSurface", L"", myCube );

		// Creating the first animation source
		CValueArray args(9);
		CValue		outArg;
		args[0] = root;
		args[1] = L"cube.kine.local.posx,cube.kine.local.posy,cube.kine.local.posz";
		args[2] = 1l;
		args[3] = L"StoredStaticPose";
		args[4] = true;
		args[5] = 1l;
		args[6] = 5l;
		args[7] = false;
		args[8] = false;
		app.ExecuteCommand( L"StoreAction", args, outArg );

		Source mySource(outArg);

		// Creating the first clip
		CValueArray addClipArgs(6);
		addClipArgs[0] = root;
		addClipArgs[1] = mySource.GetFullName();
		addClipArgs[5] = L"MyClip1";
		app.ExecuteCommand( L"AddClip", addClipArgs, outArg );

		Clip myClip(outArg);

		myCube.PutParameterValue(L"posx", 3.0f);

		// Creating the second animation source
		args[1] = L"cube.kine.local.posx,cube.kine.local.posy,cube.kine.local.posz";
		args[5] = 7l;
		args[6] = 9l;
		app.ExecuteCommand( L"StoreAction", args, outArg );

		mySource = outArg;

		// Creating the second clip from the second source.
		// Put it on the same track as the first one.
		addClipArgs[0] = root;
		addClipArgs[1] = mySource.GetFullName();
		addClipArgs[3] = myClip.GetParent();
		addClipArgs[5] = L"MyClip1";
		app.ExecuteCommand( L"AddClip", addClipArgs, outArg );

		Clip myClip2(outArg);

		CValueArray addTransitionArgs(3);
		addTransitionArgs[0] = myClip;
		addTransitionArgs[1] = myClip2;
		addTransitionArgs[2] = 0l;

		app.ExecuteCommand( L"AddTransition", addTransitionArgs, outArg );

		Transition myTransition(outArg);
		app.LogMessage(L"The transition " + myTransition.GetFullName() +
			L" is of type " + myTransition.GetType());

		Clip startClip = myTransition.GetStartClip();
		app.LogMessage(L"The clip at the beginning of the transition is " +
						startClip.GetFullName());
		Clip endClip = myTransition.GetEndClip();
		app.LogMessage(L"The clip at the end of the transition is " +
						endClip.GetFullName());

	\endcode
 */
//*****************************************************************************
class SICPPSDKDECL Transition : public ProjectItem
{
public:
	/*! Default constructor. */
	Transition();

	/*! Default destructor. */
	~Transition();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	Transition(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	Transition(const Transition& in_obj);

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
	\return The new Transition object.
	*/
	Transition& operator=(const Transition& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new Transition object.
	*/
	Transition& operator=(const CRef& in_ref);

	/*! Returns the clip at the beginning of the Transition.
	\return The Clip at the start of the transition.
	*/
	Clip GetStartClip() const;

	/*! Returns the clip at the end of the Transition.
	\return The Clip at the end of the transition.
	*/
	Clip GetEndClip() const;

	/*! Returns the channels which are used in this transition. The channels are the objects for which the value
	is interpolated, common objects retrieved with MappedItem::GetDestination.
	\warning Not implemented.
	\return Array of objects for which there's an interpolation. Currently, this function returns an empty array.
	*/
	CRefArray GetItems() const;

	private:
	Transition * operator&() const;
	Transition * operator&();
};

}

#endif // __XSITRANSITION_H__
