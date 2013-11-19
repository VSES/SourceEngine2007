//*****************************************************************************
/*!
   \file xsi_track.h
   \brief Track class declaration.

   © Copyright 1998-2003 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************
#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSITRACK_H__
#define __XSITRACK_H__

#include <xsi_projectitem.h>

namespace XSI {


//*****************************************************************************
/*! \class Track xsi_track.h
	\brief A Track represents the container of clips used to sequence clips in the ClipContainer.

	You can retrieve the track object using ClipContainer.GetTracks. A track contains only a specific
	type of clips; for example, audio clips (where Clip.GetType() == siClipAudioType) are the only
	type of tracks found in an audio track (where Track.GetType() == siTrackAudioType). The base property
	SIObject::GetType returns one of the values contained in siTrackType.

	Tracks can only be added using the \xl AddTrack command (there is currently no way to add a track using the C++ API).

	\sa ClipContainer, Clip, \xl AddTrack
	\since 4.0

	\eg Demonstrates how to create tracks and retrieve all tracks within a ClipContainer.
		This example will create the following hierarchy:

	\code
		\*

		    Mixer
		      |
		      |- Track (an animation track)
		      |    |
		      |    |- CompoundClip
		      |            |
		      |            |- Track4 (an animation track)
		      |
		      |- Track1 (an animation track)
		      |- Track2 (a shape track)
		      |- Track3 (an audio track)

		*\
	\endcode

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
		app.LogMessage(L"First created Track " + SIObject(myClip.GetParent()).GetFullName() );

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
		app.LogMessage(L"Second created Track " + SIObject(myClip2.GetParent()).GetFullName() );

		// Adding a shape track
		ClipContainer myMixer = root.GetMixer().GetRef();
		CValueArray addTrackArgs(3);
		addTrackArgs[0] = root;
		addTrackArgs[1] = myMixer;
		addTrackArgs[2] = 1l;//Shape track
		app.ExecuteCommand( L"AddTrack", addTrackArgs, outArg );
		Track myShapeTrack(outArg);
		app.LogMessage(L"Third created Track " + myShapeTrack.GetFullName() );

		// Adding an audio track
		addTrackArgs[2] = 2l;//Audio track
		app.ExecuteCommand( L"AddTrack", addTrackArgs, outArg );
		Track myAudioTrack(outArg);
		app.LogMessage(L"Fourth created Track " + myAudioTrack.GetFullName() );

		// Creating a compound clip from the clips created earlier
		// This will implicitly create a track for the nested clip
		args.Resize(2);
		args[0] = root;
		args[1] = myClip.GetFullName() + L"," + myClip2.GetFullName();
		app.ExecuteCommand( L"CreateCompound", args, outArg );

		// Now retreiving all tracks recursively.

		// First lets get the tracks which are directly under the mixer.
		CRefArray myTracks = myMixer.GetTracks();
		CRefArray myResultingTracks;

		LONG i;
		for(i = 0; i< myTracks.GetCount(); ++i)
		{
			Track currentTrack(myTracks[i]);
			myResultingTracks.Add(currentTrack);
			CRefArray trackClips = currentTrack.GetClips();
			for(LONG j = 0; j < trackClips.GetCount();++j)
			{
				ClipContainer compoundClip(trackClips[0]);

				if(compoundClip.IsValid())
				{
					// We have a compound so let's fetch the nested tracks.
					CRefArray nestedTracks = compoundClip.GetTracks();
					for(LONG k = 0; k < nestedTracks.GetCount(); ++k)
					{
						myResultingTracks.Add(nestedTracks[k]);
					}
				}
			}
		}

		app.LogMessage(L"The resulting complete track list is the following: ");
		for(i = 0; i < myResultingTracks.GetCount(); ++i)
		{
			app.LogMessage(Track(myResultingTracks[i]).GetFullName());
		}
	\endcode
 */
//*****************************************************************************
class SICPPSDKDECL Track : public ProjectItem
{
public:
	/*! Default constructor. */
	Track();

	/*! Default destructor. */
	~Track();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	Track(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	Track(const Track& in_obj);

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
	\return The new Track object.
	*/
	Track& operator=(const Track& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new Track object.
	*/
	Track& operator=(const CRef& in_ref);

	/*! Returns the clips contained on that track.
	\return Array of Clip or ClipContainer objects found on the track.
	*/
	CRefArray GetClips() const;

	private:
	Track * operator&() const;
	Track * operator&();
};

}

#endif // __XSITRACK_H__
