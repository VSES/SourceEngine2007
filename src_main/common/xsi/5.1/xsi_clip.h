//*****************************************************************************
/*!
   \file xsi_clip.h
   \brief Clip class declaration.

   © Copyright 1998-2003 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************
#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSICLIP_H__
#define __XSICLIP_H__

#include <xsi_projectitem.h>

namespace XSI {

class TimeControl;
class Source;
class ClipEffect;
class Property ;
//*****************************************************************************
/*! \class Clip xsi_clip.h
	\brief Clips are instances of animation and shape actions (ActionSource), audio and image sources
	(Source) and compound clips (ClipContainer).

	You can check to see the clip type (such as an audio clip) using SIObject::GetType which returns
	one of the values contained in the ::siClipType enum.

	This class also provides access to clip features such as its timing (TimeControl), any links
	to other clips (ClipRelation), and any applied effects (ClipEffect). In addition, you can get
	the underlying source on which this clip is instantiated (the Source object via Clip::GetSource)
	or the elements of that source (using either the MappedItem object via Clip::GetMappedItems or the
	AnimationSourceItem object via ActionSource::GetItems).

	\attention While a %Clip object represents compound, audio, and image clips, not all %Clip members
	are available for these types. For example, the GetMappedItems function returns an empty array when called
	on audio and image clips.

	\tip There is no function in the C++ API to create clips from sources, but you can use the \xl AddClip
	command, which returns a %Clip object. To access existing %Clip objects, you can get a CRefArray of clips
	in the Mixer via ClipContainer::GetClips.


	\sa ClipContainer, Model::GetMixer, Source, \xl AddClip, ClipContainer::GetClips, Track::GetClips,
		\xl CopyClipToTrack, \xl AddAudioClip, \xl ImportAudioAndAddClip, \xl ApplyShapeKey, \xl SaveShapeKey

	\since 4.0

	\eg Demonstrates how to create a %Clip using the \xl AddClip command and how to find that clip in the
		mixer using ClipContainer::GetClips. The clip will contain the static values of the local position of a cube.

	\code
		using namespace XSI;

		Application app;

		// NewScene command
		CValueArray cargs; CValue oarg;
		cargs.Add( L"" ); cargs.Add( false );
		app.ExecuteCommand( L"NewScene", cargs, oarg );
		cargs.Clear(); oarg.Clear();

		// Get the SceneRoot and create a cube in it
		Model root = app.GetActiveSceneRoot();
		X3DObject myCube; root.AddGeometry( L"Cube", L"MeshSurface", L"MyCube", myCube );

		// We need the relative name of the pos parameters to set up a proper source
		Parameter posx = myCube.GetParameter( L"posx" ); CString rposx = GetRelativePath(posx);
		Parameter posy = myCube.GetParameter( L"posy" ); CString rposy = GetRelativePath(posy);
		Parameter posz = myCube.GetParameter( L"posz" ); CString rposz = GetRelativePath(posz);

		// Set up a source on the cube's pos parameters using static values
		ActionSource src = root.AddActionSource( L"MyStoredStaticSource" );
		src.AddSourceItem( rposx, double(1.0), true );
		src.AddSourceItem( rposy, double(0.0), true );
		src.AddSourceItem( rposz, double(-2.5), true );

		// Instantiate the source with the AddClip command
		CValueArray clpArgs(9); CValue clpOut;
		clpArgs[0] = root.GetFullName();
		clpArgs[1] = src.GetFullName();
		app.ExecuteCommand( L"AddClip", clpArgs, clpOut );

		// Now find the clip through the mixer
		Mixer mxr = root.GetMixer();
		CRefArray cliplist = mxr.GetClips();
		for ( LONG i=0; i<cliplist.GetCount(); ++i ) {
			Clip clp( cliplist[i] );
			app.LogMessage( clp.GetFullName() + L" is a " + clp.GetType() );

			// Since a give clip may be a compound clip, it may be necessary to drill down
			if ( clp.GetType() == siClipAnimCompoundType || clp.GetType() == siClipShapeCompoundType ) {
				// First convert the clip to a clipcontainer
				ClipContainer compoundclip( cliplist[i] );
				CRefArray subclips = compoundclip.GetClips();
				for ( LONG j=0; j<subclips.GetCount(); ++j ) {
					Clip clp2( subclips[j] );
					app.LogMessage( clp2.GetFullName() + L" is a " + clp2.GetType() );
					// ... and so on
				}
			}
		}

		// Expected result:
		//INFO : Clip # 1 is a mixeranimclip: Mixer.Mixer_Anim_Track.MyStoredStaticSource_Clip

	\endcode
 */
//*****************************************************************************
class SICPPSDKDECL Clip : public ProjectItem
{
public:
	/*! Default constructor. */
	Clip();

	/*! Default destructor. */
	~Clip();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	Clip(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	Clip(const Clip& in_obj);

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
	\return The new Clip object.
	*/
	Clip& operator=(const Clip& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new Clip object.
	*/
	Clip& operator=(const CRef& in_ref);

	/*! Returns the TimeControl property associated with this clip.
	\return The TimeControl property object.
	*/
	TimeControl GetTimeControl() const;

	/*! Returns the Source object which is instantiated by this clip. For example, if the clip
	is an instance of an ActionSource (ie., an animation or shape AnimationSourceItem), then the
	ActionSource object is returned. If the clip is an instance of an audio or image source, the
	Source object is returned.
	\note The source returned here stores animation, audio, image, or model data. This is not
		the same as the animation data that is driving the parameter, such as an FCurve or a ShapeKey.
		To access those \xl DataSource elements, use MappedItem::GetAnimationSource on the
		Clip::GetMappedItems items.
	\return The Source or ActionSource instantiated by this clip.
	*/
	Source GetSource() const;

	/*! Returns the array of MappedItem objects which are driven by this clip.
	\warning If you try to use GetMappedItems on an audio source (::siClipAudioType), you
		will get an error. You can test for the audio source type with SIObject::GetType.
	\return Array of MappedItem objects.
	*/
	CRefArray GetMappedItems() const;

	/*! Returns the ClipEffect associated with this clip.
	\return The ClipEffect associated with this clip.
	\sa \xl GetMappingRule, \xl SetMappingRule
	*/
	ClipEffect GetEffect() const;

	/*!Adds a UserDataBlob or CustomProperty to the Clip. This can be useful for storing custom
	user data inside the %scene.
	\param in_preset Preset name or name of a self-installed CustomProperty. The type of property that
		is created is determined by this argument. For example, \c CustomProperty creates an empty
		CustomProperty and \c UserDataBlob creates a UserDataBlob. It is also possible to specify the
		name of a CustomProperty that is installed as a PluginItem.
	\param in_bBranch False is the only supported value.
	\param in_name Optional name for the new property (SIObject::GetName).  If no name is specified the name
		is determined based on the value for in_preset.
	\return The new Property object.
	\sa SceneItem::AddProperty, Clip::GetProperties
	*/
	Property 	AddProperty
	(
		const CString&	in_preset,
		bool			in_bBranch = false ,
		const CString&	in_name = CString()
	);

	/*! Returns an array of all applied Property objects on the object. A Clip may have nested CustomProperty
	or UserDataBlob properties.
	\sa SceneItem::GetProperties, Clip::AddProperty
	*/
	CRefArray GetProperties() const ;

	private:
	Clip * operator&() const;
	Clip * operator&();
};

}

#endif // __XSICLIP_H__
