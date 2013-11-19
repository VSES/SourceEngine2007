//*****************************************************************************
/*!
   \file xsi_clipeffectitem.h
   \brief ClipEffectItem class declaration.

   © Copyright 1998-2003 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************
#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSICLIPEFFECTITEM_H__
#define __XSICLIPEFFECTITEM_H__

#include <xsi_siobject.h>

namespace XSI {

class Parameter;
//*****************************************************************************
/*! \class ClipEffectItem xsi_clipeffectitem.h
	\brief A ClipEffectItem is an expression which is bound to a MappedItem of a Clip.

	You can get the ClipEffectItem from either ClipEffect::GetItems or MappedItem::GetClipEffectItem.

	\sa Clip, MappedItem, Model::GetMixer, Track, Transition, \xl GetMappingRule, \xl SetMappingRule
	\since 4.0

	\eg Demonstrates how to use a mapped item to look for a specific animation
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

 */
//*****************************************************************************
class SICPPSDKDECL ClipEffectItem : public SIObject
{
public:
	/*! Default constructor. */
	ClipEffectItem();

	/*! Default destructor. */
	~ClipEffectItem();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	ClipEffectItem(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	ClipEffectItem(const ClipEffectItem& in_obj);

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
	\return The new ClipEffectItem object.
	*/
	ClipEffectItem& operator=(const ClipEffectItem& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new ClipEffectItem object.
	*/
	ClipEffectItem& operator=(const CRef& in_ref);

	/*! Returns the parameter which is specific to this ClipEffectItem object. You can use the value of
	this parameter within the ClipEffectItem expression by using the token "param".
	\return The Parameter which is specific to this ClipEffectItem.
	*/
	Parameter GetParameter()const;

	/*! Returns the string representation of the expression used by the ClipEffectItem. The valid tokens
	that can be used in the context of this expression can be found in the Animation guide under the clip
	effect expression section.

	\return The expression value.

	\eg Demonstrates how to set and get the expression on the ClipEffectItem

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

		// Get the first clip in the mixer
		Mixer mix = root.GetMixer();
		CRefArray cliplist = mix.GetClips();
		Clip clp( cliplist[0] );

		// Avoid the audio clips
		if ( clp.GetType() != siClipAudioType )
		{
			CRefArray mappings = clp.GetMappedItems();
			if ( mappings.IsValid() )
			{
				for ( LONG i=0; i<mappings.GetCount(); i++ )
				{
					MappedItem mapitem( mappings[i] );

					// Get the driven parameter, looking for posy
					Parameter driven( mapitem.GetDestination() );
					if ( driven.GetFullName() == L"MyNull.kine.local.posy" )
					{
						// SetMappingRule command
						CValueArray mapArgs; CValue mapOut;
						mapArgs.Add( clp.GetFullName() + L".ActionClip" );
						mapArgs.Add( driven.GetFullName() );
						mapArgs.Add( L"frame" );
						mapArgs.Add( CValue(i+1.0) );
						app.ExecuteCommand( L"SetMappingRule", mapArgs, mapOut );

						// If there is an expression on the clip effect item, display it
						ClipEffectItem clpfxitm = mapitem.GetClipEffectItem();
						if ( clpfxitm.GetExpression() != L"" )
						{
							app.LogMessage( L"The ClipEffectItem driving the " + driven.GetFullName()
								+ L" parameter has this expression: " + clpfxitm.GetExpression() );
						}

						break;
					}
				}
			}
		}


		// Expected Results:
		//INFO : The ClipEffectItem driving the MyNull.kine.local.posy parameter has this expression: frame


		// This is a handy function to have around if you're going to create an action
		// source or clip on a parameter that is in a nested model (ie., not directly
		// under the Scene_Root) because AddClip will force a mapping template if your
		// parameter is not relative.
		CString GetRelativeNameForTarget( Parameter& in_param )
		{
			Model mdl = in_param.GetModel();
			CString mdlname = mdl.GetFullName();
			if ( mdlname.IsEqualNoCase(L"Scene_Root") )
			{
				return in_param.GetFullName();
			} else {
				CString tmp = in_param.GetFullName();
				CString lookfor = mdlname + L".";
				CString foundsofar = L"";
				CString relpath = L"";
				for ( ULONG i=0; i<tmp.Length(); ++i )
				{
					if ( foundsofar.IsEqualNoCase(lookfor) )
					{
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
	CString GetExpression()const;


	private:
	ClipEffectItem * operator&() const;
	ClipEffectItem * operator&();
};

}

#endif // __XSICLIPEFFECTITEM_H__
