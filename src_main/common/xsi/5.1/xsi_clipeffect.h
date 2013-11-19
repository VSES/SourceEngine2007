//*****************************************************************************
/*!
   \file xsi_clipeffect.h
   \brief ClipEffect class declaration.

   © Copyright 1998-2003 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************
#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSICLIPEFFECT_H__
#define __XSICLIPEFFECT_H__

#include <xsi_siobject.h>

namespace XSI {

class StaticKinematicState;
class CParameterRefArray;
enum siTimeReferenceType;

//*****************************************************************************
/*! \class ClipEffect xsi_clipeffect.h
	\brief A %ClipEffect represents the set of effects associated with a Clip. Each %ClipEffect object
	contains one item corresponding to a MappedItem.

	These ClipEffectItems contain expressions that control the clip without affecting the Source
	on which the clip was instantiated. For example, you can create a walk cycle with a progressive
	offset by using an expression similar to this:

	\code 	(cid * 10) + this \endcode

	ClipEffects are available from the Clip object using Clip::GetEffect.

	\sa Clip, MappedItem::GetClipEffectItem, ClipEffectItem, Clip::GetEffect, \xl GetMappingRule,
		\xl SetMappingRule
	\since 4.0

	\eg Demonstrates how to use MappedItems to look for a specific animation
		channel. Once this channel is found a clip effect is added onto it.

	\code
		using namespace XSI;
		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject myCube;
		root.AddGeometry( L"Cube", L"MeshSurface", L"", myCube );

		//Creating the first animation source
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

		//Creating the first clip
		CValueArray addClipArgs(6);
		addClipArgs[0] = root;
		addClipArgs[1] = mySource.GetFullName();
		addClipArgs[5] = L"MyClip1";
		app.ExecuteCommand( L"AddClip", addClipArgs, outArg );

		Clip myClip(outArg);
		CRefArray mappedItems = myClip.GetMappedItems();
		// Find the mapped item on which we wan't to add an effect.
		// We are looking for the local.posy parameter
		LONG i;
		MappedItem currItem;
		for(i = 0; i < mappedItems.GetCount(); ++i)
		{
			currItem = mappedItems[i];
			Parameter param = currItem.GetDestination();
			if(param.GetFullName().IsEqualNoCase(L"cube.kine.local.posy"))
			{
				CValueArray mappingRuleArgs(4);
				mappingRuleArgs[0] = myClip.GetFullName() + L".ActionClip";
				mappingRuleArgs[1] = param;
				mappingRuleArgs[2] = L"frame";
				mappingRuleArgs[3] = (LONG)(i + 1);

				app.ExecuteCommand(L"SetMappingRule", mappingRuleArgs, outArg);
				break;
			}
		}

		ClipEffectItem effectItem(currItem.GetClipEffectItem());
		app.LogMessage(L"The expression associated with the posy item is >> " +
						effectItem.GetExpression());
	\endcode

	\eg Illustrates how to get at the ClipEffect objects by creating some
	sources and clips with clip effects and then finding them again through the mapped
	items on the clips in the mixer.

	\code
		using namespace XSI;

		// Forward declarations
		CString GetRelativeNameForTarget( Parameter& in_param );
		void CreateStaticSource();


		Application app;

		// NewScene command
		CValueArray cargs; CValue oarg;
		cargs.Add( L"" ); cargs.Add( false );
		app.ExecuteCommand( L"NewScene", cargs, oarg );
		cargs.Clear(); oarg.Clear();

		// Set up the scene
		Model root = app.GetActiveSceneRoot();
		CreateStaticSource();

		// Get all the clips in the mixer and then the sources
		Mixer mix = root.GetMixer();
		CRefArray cliplist = mix.GetClips();
		for ( LONG i=0; i<cliplist.GetCount(); ++i ) {
			Clip clp( cliplist[i] );

			// Look at their mapped items, but avoid the audio clips
			if ( clp.GetType() != siClipAudioType ) {
				CRefArray mappings = clp.GetMappedItems();
				if ( mappings.IsValid() ) {
					app.LogMessage( L"Found " + CString(mappings.GetCount()) + L" mapped items(s)" );
					for ( LONG j=0; j<mappings.GetCount(); j++ ) {
						MappedItem mapitem( mappings[j] );

						// Each item on the clip effect may have an expression that controls
						// its behavior independently of the original source
						ClipEffectItem clpfxitem = mapitem.GetClipEffectItem();
						if ( clpfxitem.GetExpression() != L"" ) {
							app.LogMessage( L"...this clip has a clip effect item matching this expression:" );
							app.LogMessage( L"   " + clpfxitem.GetExpression() );
						}

						// The mapped item also provides access to the driven parameter
						Parameter dest( mapitem.GetDestination() );
						app.LogMessage( L"...destination parameter: " + dest.GetFullName() );
					}
				}

			}
		}


		// Expected Results:
		//INFO : Found 3 mapped items(s)
		//INFO : ...destination parameter: MyNull.kine.local.posx
		//INFO : ...destination parameter: MyNull.kine.local.posy
		//INFO : ...destination parameter: MyNull.kine.local.posy


		// This is a handy function to have around if you're going to create an action
		// source or clip on a parameter that is in a nested model (ie., not directly
		// under the Scene_Root) because AddClip will force a mapping template if your
		// parameter is not relative.
		CString GetRelativeNameForTarget( Parameter& in_param )
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

		// Convenience function
		void CreateStaticSource()
		{
			Application app;

		    // Get the SceneRoot and create a cone in it
		    Model root = app.GetActiveSceneRoot();
		    Null n; root.AddNull( L"MyNull", n );

		    // Get the parameters to use as targets
		    Parameter posx = n.GetParameter( L"posx" ); CString rposx = GetRelativeNameForTarget( posx );
		    Parameter posy = n.GetParameter( L"posy" ); CString rposy = GetRelativeNameForTarget( posy );
		    Parameter posz = n.GetParameter( L"posz" ); CString rposz = GetRelativeNameForTarget( posz );

		    // Create a stored static pose
		    ActionSource src = root.AddActionSource( L"StoredStaticPose" );
		    src.AddSourceItem( rposx, 5.5743, true );
		    src.AddSourceItem( rposy, 0.1953, true );
		    src.AddSourceItem( rposy, -0.0195, true );

		    // Instantiate it in the mixer
		    CValueArray clpArgs(9); CValue clpOut;
		    clpArgs[0] = root.GetFullName();
		    clpArgs[1] = src.GetFullName();
		    clpArgs[4] = CValue(4.0);                          // starts at frame 4
		    app.ExecuteCommand( L"AddClip", clpArgs, clpOut );
		}

	\endcode
 */
//*****************************************************************************
class SICPPSDKDECL ClipEffect : public SIObject
{
public:
	/*! Default constructor. */
	ClipEffect();

	/*! Default destructor. */
	~ClipEffect();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	ClipEffect(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	ClipEffect(const ClipEffect& in_obj);

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
	\return The new ClipEffect object.
	*/
	ClipEffect& operator=(const ClipEffect& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new ClipEffect object.
	*/
	ClipEffect& operator=(const CRef& in_ref);

	/*! Returns all items contained in this ClipEffect. There is one ClipEffectItem for every MappedItem.
	\return Array of references to ClipEffectItem objects.
	*/
	CRefArray GetItems()const;

	/*! Returns the parameters which are shared by all ClipEffectItem objects.
	\return Array of references to Parameter objects.
	*/
	CParameterRefArray GetVariables()const;

	/*! Returns true if the ClipEffect will be computed in the clip evaluation.
	\return True if the effect is active, false otherwise.
	*/
	bool GetIsActive()const;

	/*! Returns true if the ClipEffect transformation effect is active.
	\return True if the effect is active, false otherwise.
	*/
	bool GetPoseIsActive()const;

	/*! Returns either ::siOriginalClip (the effect will repeat itself over time) or ::siExtrapolatedClip
	(the effect spans over extrapolation).
	\return The time reference type (see the ::siTimeReferenceType enum).
	*/
	siTimeReferenceType GetTimeReference()const;

	/*! Returns the StaticKinematicState property containing the transformation which is used for this clip effect.
	\return StaticKinematicState representing the transformation used.
	*/
	StaticKinematicState GetPoseEffect()const;

	private:
	ClipEffect * operator&() const;
	ClipEffect * operator&();
};

}

#endif // __XSICLIPEFFECT_H__
