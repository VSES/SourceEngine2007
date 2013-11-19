//*****************************************************************************
/*!
   \file xsi_mixer.h
   \brief Animation %Mixer class declaration.

   © Copyright 1998-2004 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************
#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIMIXER_H__
#define __XSIMIXER_H__

#include <xsi_clipcontainer.h>

namespace XSI {


//*****************************************************************************
/*! \class Mixer xsi_mixer.h
	\brief This represents the Animation %Mixer, which is nested directly under the Model. The Mixer
	object is a ClipContainer and so provides access to the standard elements of a mixer: its
	Track objects, Transition objects, Clip objects, and ClipRelation objects.

	You can check if an object has a mixer by calling Model::HasMixer. If it doesn't, you can
	create one with Model::AddMixer.

	The Mixer stores all of the Model's high-level animation and audio clips, so when you call
	the ClipContainer::GetClips function on the Mixer, you get the clips from only the current
	model (there is no recursion). If any of these are compound clips, none of its contents are
	returned, just the top level.

	\sa Track, Clip, Transition, Model::GetMixer
	\since 4.0

	\eg Demonstrates how to create a compound clip using the C++ API. It also shows
		that when retrieving clips from the ClipContainer it will only return clips
		which are on tracks nested directly under it.

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
		args[3] = L"StoredStaticPose";
		args[4] = true;
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
		app.LogMessage(L"First created clip " + myClip.GetFullName() );

		myCube.PutParameterValue(L"posx", 3.0f);

		// Creating the second animation source
		args[1] = L"cube.kine.local.posx,cube.kine.local.posy,cube.kine.local.posz";
		args[5] = 7l;
		args[6] = 9l;
		app.ExecuteCommand( L"StoreAction", args, outArg );

		mySource = outArg;

		// Creating the second clip from the second source.
		addClipArgs[0] = root;
		addClipArgs[1] = mySource.GetFullName();
		addClipArgs[5] = L"MyClip1";
		app.ExecuteCommand( L"AddClip", addClipArgs, outArg );

		Clip myClip2(outArg);
		app.LogMessage(L"Second created clip " + myClip2.GetFullName() );

		ClipContainer myMixer = root.GetMixer().GetRef();
		CRefArray myClips = myMixer.GetClips();

		app.LogMessage(L"List of clips retrieved from the container");
		LONG i;
		for(i = 0; i< myClips.GetCount(); ++i)
		{
			app.LogMessage(L"Clip " + CValue(i).GetAsText() +
							Clip(myClips[i]).GetFullName());
		}

		// Creating a compound clip from the clips created earlier
		args.Resize(2);
		args[0] = root;
		args[1] = myClip.GetFullName() + L"," + myClip2.GetFullName();
		app.ExecuteCommand( L"CreateCompound", args, outArg );

		ClipContainer myCompound(outArg);
		app.LogMessage(L"Compound clip " + myCompound.GetFullName() );

		myClips = myCompound.GetClips();
		app.LogMessage(L"List of clips retreived from the compound");
		for(i = 0; i< myClips.GetCount(); ++i)
		{
			app.LogMessage(L"Clip " + CValue(i).GetAsText() +
							Clip(myClips[i]).GetFullName());
		}

		// Now retrieving once again the clips from under the mixer.
		// This should only return the CompoundClip.
		myClips = myMixer.GetClips();

		app.LogMessage(L"List of clips retreived from the mixer after the creation of the compound");
		for(i = 0; i< myClips.GetCount(); ++i)
		{
			app.LogMessage(L"Clip " + CValue(i).GetAsText() +
							Clip(myClips[i]).GetFullName());
		}

	\endcode
 */
//*****************************************************************************
class SICPPSDKDECL Mixer : public ClipContainer
{
public:
	/*! Default constructor. */
	Mixer();

	/*! Default destructor. */
	~Mixer();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	Mixer(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	Mixer(const Mixer& in_obj);

	/*! Returns true if a given class type is compatible with this API class.
	\param in_ClassID class type.
	\returns bool true if the class is compatible, false otherwise.
	*/
	bool IsA( siClassID in_ClassID) const;

	/*! Returns the type of the API class.
	\returns siClassID the class type.
	*/
	siClassID GetClassID() const;

	/*! Creates an object from another object. The newly created object is set to
	empty if the input object is not compatible.
	\param in_obj constant class object.
	\returns Mixer& New Mixer object.
	*/
	Mixer& operator=(const Mixer& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\returns Mixer& New Mixer object.
	*/
	Mixer& operator=(const CRef& in_ref);

	private:
	Mixer * operator&() const;
	Mixer * operator&();
};

}

#endif // __XSIMIXER_H__
