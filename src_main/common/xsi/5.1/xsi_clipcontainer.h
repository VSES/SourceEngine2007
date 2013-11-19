//*****************************************************************************
/*!
   \file xsi_clipcontainer.h
   \brief ClipContainer class declaration.

   © Copyright 1998-2003 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************
#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSICLIPCONTAINER_H__
#define __XSICLIPCONTAINER_H__

#include <xsi_clip.h>

namespace XSI {

class ClipRelation;
//*****************************************************************************
/*! \class ClipContainer xsi_clipcontainer.h
	\brief A ClipContainer can represent either the Mixer or a compound Clip. Both can
	contain other Clips, Tracks and Transitions.

	The Mixer class is a specialization of this class. This class provides access to the
	basic elements of the Mixer: Clip objects, Track objects, Transition objects, and ClipRelation
	objects.

	You can create a compound clip with the \xl CreateCompound command.

	\sa Track, Clip, Transition, Model::GetMixer
	\since 4.0

	\eg Demonstrates how to create a compound clip using the C++ API. It also shows that when
	retrieving clips from the ClipContainer it will only return clips which are on tracks nested
	directly under it.

	\code
		using namespace XSI;
		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject myCube;
		root.AddGeometry( L"Cube", L"MeshSurface", L"", myCube );

		//Creating the first animation source
		CValueArray args(9);
		CValue outArg;
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
		for(LONG i = 0; i< myClips.GetCount(); ++i) {
			app.LogMessage(L"Clip " + CValue(i).GetAsText() + Clip(myClips[i]).GetFullName());
		}

		// Creating a compound clip from the clips created earlier
		args.Resize(2);
		args[0] = root;
		args[1] = myClip.GetFullName() + L"," + myClip2.GetFullName();
		app.ExecuteCommand( L"CreateCompound", args, outArg );

		ClipContainer myCompound(outArg);
		app.LogMessage(L"Compound clip " + myCompound.GetFullName() );

		myClips = myCompound.GetClips();
		app.LogMessage(L"List of clips retrieved from the compound");
		for(i = 0; i< myClips.GetCount(); ++i) {
			app.LogMessage(L"Clip " + CValue(i).GetAsText() + Clip(myClips[i]).GetFullName());
		}

		// Now retrieving once again the clips from under the mixer.
		// This should only return the CompoundClip.
		myClips = myMixer.GetClips();

		app.LogMessage(L"List of clips retrieved from the mixer after the creation of the compound");
		for(i = 0; i< myClips.GetCount(); ++i) {
			app.LogMessage(L"Clip " + CValue(i).GetAsText() + Clip(myClips[i]).GetFullName());
		}
	\endcode
 */
//*****************************************************************************
class SICPPSDKDECL ClipContainer : public Clip
{
public:
	/*! Default constructor. */
	ClipContainer();

	/*! Default destructor. */
	~ClipContainer();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	ClipContainer(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	ClipContainer(const ClipContainer& in_obj);

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
	\returns ClipContainer& New ClipContainer object.
	*/
	ClipContainer& operator=(const ClipContainer& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\returns ClipContainer& New ClipContainer object.
	*/
	ClipContainer& operator=(const CRef& in_ref);

	/*! Returns an array of all clips contained under this container.
	All tracks are searched for clips, but the search does not traverse into
	nested clip containers. Consider the following mixer configuration:
	\code
			Mixer
				|
				Track
					|
					Clip 1
					CompoundClip1
						|
						Track1
							|
							Clip2
						Track2
							|
							Clip3
	\endcode
	Calling GetClips on the mixer object returns Clip1 and CompoundClip1.\n
	Calling GetClips on CompoundClip1 returns Clip2 and Clip3.

	\returns CRefArray of Clip and ClipContainer objects.
	\since 4.0
	*/
	CRefArray GetClips() const;

	/*! Returns all Track objects directly under the ClipContainer.
	\returns CRefArray of Track objects.
	\since 4.0
	*/
	CRefArray GetTracks() const;

	/*! Returns all Transition objects directly under this ClipContainer.
	\returns CRefArray of Transition objects.
	\since 4.0
	*/
	CRefArray GetTransitions() const;

	/*! Returns all ClipRelation objects nested under the ClipContainer.
	\returns Array of ClipRelation objects.
	\since 4.0
	\eg See ClipContainer::AddRelation for an example using this function.
	*/
	CRefArray GetNestedRelations() const;

	/*! Adds a relation between two clip objects' TimeControl property sets.
	The relation is by default between the StartOffset, ClipIn and ClipOut
	values of the master and slave clip.

	\note This function will fail if the addition of a new relation would
		create an evaluation cycle. For example, assume you have three clips
		A, B and C. If you already have a ClipRelation between A-B and between
		B-C then attempting to add a ClipRelation between C-A will fail.

	\param in_rMasterClip Master clip.
	\param in_rSlaveClip Slave clip.
	\param in_name Optional name for the relation.

	\returns ClipRelation object.

	\sa ClipContainer::GetNestedRelations, TimeControl::GetStartOffset,
		TimeControl::GetClipIn, TimeControl::GetClipOut, \xl AddClipRelation

	\since 4.0

	\eg Demonstrates how to create a master-slave clip relation and then access that relation to swap them.

	\code
		using namespace XSI;

		// Forward declaration
		CString GetRelativePath( Parameter& in_param );

		Application app;

		// NewScene command
		CValueArray cargs; CValue oarg;
		cargs.Add( L"" ); cargs.Add( false );
		app.ExecuteCommand( L"NewScene", cargs, oarg );
		cargs.Clear(); oarg.Clear();

		// Get the SceneRoot and create a cone in it
		Model root = app.GetActiveSceneRoot();
		X3DObject myCone; root.AddGeometry( L"Cone", L"MeshSurface", L"MyCone", myCone );

		// Get the parameters to use as targets
		Parameter posx = myCone.GetParameter( L"posx" ); CString rposx = GetRelativePath( posx );
		Parameter posy = myCone.GetParameter( L"posy" ); CString rposy = GetRelativePath( posy );
		Parameter posz = myCone.GetParameter( L"posz" ); CString rposz = GetRelativePath( posz );

		// Create a stored static pose
		ActionSource src = root.AddActionSource( L"StoredStaticPose" );
		src.AddSourceItem( rposx, 5.5743, true );
		src.AddSourceItem( rposy, 0.1953, true );
		src.AddSourceItem( rposy, -0.0195, true );

		// Instantiate it as two separate clips in the mixer
		CValueArray clpArgs(9); CValue clpOut;
		clpArgs[0] = root.GetFullName();
		clpArgs[1] = src.GetFullName();
		clpArgs[4] = CValue(19.0);							// starts at frame 19
		app.ExecuteCommand( L"AddClip", clpArgs, clpOut );
		Clip clip1( clpOut );

		clpArgs[4] = CValue(47.0); 							// starts at frame 47
		app.ExecuteCommand( L"AddClip", clpArgs, clpOut );
		Clip clip2( clpOut );

		// Get the mixer and add a relation between the two clips
		Mixer mix = root.GetMixer();
		ClipRelation rel = mix.AddRelation( clip1, clip2, L"myRelation" );

		// Go back through the mixer to get the nested relations
		CRefArray nested = mix.GetNestedRelations();
		app.LogMessage( L"The number of clip relation(s) nested under the mixer is: " + CString(nested.GetCount()) );
		for ( LONG i=0; i<nested.GetCount(); ++i ) {
			ClipRelation foundrel( nested[i] );

			// Who is the master and who is the slave?
			Clip master = foundrel.GetMasterClip();
			Clip slave = foundrel.GetSlaveClip();
			app.LogMessage( L"Master is " + master.GetFullName() );
			app.LogMessage( L"Slave is " + slave.GetFullName() );

			// Switch master and slave and redisplay the info
			foundrel.SwapMasterAndSlaveClip();
			app.LogMessage( L"Master is now " + foundrel.GetMasterClip().GetFullName() );
			app.LogMessage( L"Slave is now " + foundrel.GetSlaveClip().GetFullName() );
		}

		// Expected results:
		//INFO : The number of clip relation(s) nested under the mixer is: 1
		//INFO : Master is Mixer.Mixer_Anim_Track.StoredStaticPose_Clip
		//INFO : Slave is Mixer.Mixer_Anim_Track1.StoredStaticPose_Clip1
		//INFO : Master is now Mixer.Mixer_Anim_Track1.StoredStaticPose_Clip1
		//INFO : Slave is now Mixer.Mixer_Anim_Track.StoredStaticPose_Clip

		// Function to remove the name of the model from the FullName of the specified parameter.
		// This is necessary when setting up a source that will later be used to instantiate a
		// clip when the parameter lives under a model other than the Scene_Root.
		CString GetRelativePath( Parameter& in_param )
		{
			Model mdl = in_param.GetModel();
			CString mdlname = mdl.GetFullName();
			if ( mdlname.IsEqualNoCase(L"Scene_Root") ) {
				return in_param.GetFullName();
			} else {
				CString tmp = in_param.GetFullName();
				CString lookfor = mdlname + L".";
				CString foundsofar = L"";
				CString relpath = L"";
				for ( ULONG i=0; i<tmp.Length(); ++i ) {
					if ( foundsofar.IsEqualNoCase(lookfor) ) {
						relpath += tmp[i];
					} else {
						foundsofar += tmp[i];
					}
				}
				return relpath;
			}
		}
	\endcode
	*/
	ClipRelation AddRelation
	(
		const Clip&		in_rMasterClip,
		const Clip&		in_rSlaveClip,
		const CString&  in_name = CString()
	);


	private:
	ClipContainer * operator&() const;
	ClipContainer * operator&();
};

}

#endif // __XSICLIPCONTAINER_H__
