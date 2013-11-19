//*****************************************************************************
/*!
   \file xsi_animationsourceitem.h
   \brief AnimationSourceItem class declaration.

   © Copyright 1998-2003 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************
#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIANIMATIONSOURCEITEM_H__
#define __XSIANIMATIONSOURCEITEM_H__

#include <xsi_siobject.h>

namespace XSI {

class StaticSource;
class CValue;
//*****************************************************************************
/*! \class AnimationSourceItem xsi_animationsourceitem.h
    \brief An ActionSource contains a collection of animation source items, which you can get at using
    ActionSource::GetItems. The %AnimationSourceItem class provides access to the animation \xl DataSource
    (for example, an FCurve) and a target entity such as the Parameter or Cluster which the animation source
    drives.

	One way to create action animation is to create an empty ActionSource and then populate it afterwards
	using ActionSource::AddSourceItem. In that case you can add a StaticSource as the animation source using
	AnimationSourceItem::SetAsStatic.

	In order to know exactly which type of AnimationSourceItem you are dealing with you can use SIObject::GetType
	which returns one of the values contained in the ::siAnimationSourceItemType enum.

	\sa ActionSource::GetItems, FCurve, StaticSource, Expression, Constraint, ShapeKey, Model::AddActionSource,
		ActionSource::AddSourceItem, AnimationSourceItem::SetAsStatic

  	\since 4.0

	\eg Illustrates creating a source and accessing the source items contained in it.

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

		ActionSource mySource(outArg);
		CRefArray aSourceItems = mySource.GetItems();

		for(LONG i = 0; i < aSourceItems.GetCount();++i)
		{
			SourceItem currItem(aSourceItems[i]);
			app.LogMessage(L" The animated target is " + currItem.GetTarget());
			app.LogMessage(L" The type of animation is " + currItem.GetType());
			//In this case its a static value
			//you can switch on type.
			StaticValue myVal = currItem.GetSource();
			app.LogMessage(L" The value is " + myVal.GetValue().GetAsText());
		}
	\endcode

	\eg Illustrates how to create an empty ActionSource using Model::AddActionSource, populate
		it with 2 static values and 1 fcurve using ActionSource::AddSourceItem, and then replace
		the fcurvesource with a static value for the 2nd and change the static value for the 3rd
		using AnimationSourceItem::SetAsStatic.

	\code
		using namespace XSI;

		// Forward declarations
		CString GetRelativeName( Parameter& in_param );
		FCurve MakeAnFCurve( Null& in_obj, CString in_param );


		Application app;

		// NewScene command
		CValueArray cargs; CValue oarg;
		cargs.Add( L"" ); cargs.Add( false );
		app.ExecuteCommand( L"NewScene", cargs, oarg );
		cargs.Clear();

		// Get the scene root
		Model root = app.GetActiveSceneRoot();

		// Create a null and get pointers to the pos parameters
		Null n; root.AddNull( L"null", n );

		Parameter posx = n.GetParameter( L"posx" ); CString rposx = GetRelativeName(posx);
		Parameter posy = n.GetParameter( L"posy" ); CString rposy = GetRelativeName(posy);
		Parameter posz = n.GetParameter( L"posz" ); CString rposz = GetRelativeName(posz);

		// Create an empty actionsource and then populate it with static values
		ActionSource src = root.AddActionSource( L"StaticActionSource" );
		FCurve fc = MakeAnFCurve( n, L"posy" );
		src.AddSourceItem( rposx, 2.0 );	// static value
		src.AddSourceItem( rposy, fc );		// fcurve
		src.AddSourceItem( rposz );		// default static value = 0.0

		// Now change the value on the posz parameter using SetAsStatic and change
		// the posy parameter source to an fcurve
		CRefArray animsrcitems = src.GetItems();
		for ( LONG i=0; i<animsrcitems.GetCount(); ++i ) {
			AnimationSourceItem srcitem(animsrcitems[i]);

			// Get a pointer to the source item (it can be either a StaticSource or an FCurve)
			CRef ref = srcitem.GetSource();
			if ( ref.IsA(siFCurveID) ) {
				FCurve datasrc(ref);

				// Print the FCurve type
				app.LogMessage( L"AnimationSourceItem[" + CString(i) + L"] (" + srcitem.GetTarget()
					+ L") has " + CString(datasrc.GetNumKeys()) + L" keys" );
			}

			if ( ref.IsA(siStaticSourceID) ) {
				StaticSource datasrc(ref);

				// Print the current value
				app.LogMessage( L"AnimationSourceItem[" + CString(i) + L"] (" + srcitem.GetTarget()
					+ L") has this static value: " + CString( datasrc.GetValue() ) );
			}

			// AnimationSourceItem::GetTarget returns the \xl RelativeName
			if ( srcitem.GetTarget() == rposy ) {
				StaticSource staticvalue;
				srcitem.SetAsStatic( CValue(1.0), staticvalue );

				// Check for the new value (we know it's a StaticSource because we just set it)
				StaticSource datasrc( srcitem.GetSource() );	// refresh
				app.LogMessage( L"New static value = " + CString( datasrc.GetValue() ) );
			}

			if ( srcitem.GetTarget() == rposz ) {
				StaticSource staticvalue;
				srcitem.SetAsStatic( CValue(1.5), staticvalue );

				// Check for the new value (we know it's a StaticSource because we just set it)
				StaticSource datasrc( srcitem.GetSource() );	// refresh
				app.LogMessage( L"New static value = " + CString( datasrc.GetValue() ) );
			}
		}

		//INFO : AnimationSourceItem[0] (null.kine.local.posx) has this static value: 2
		//INFO : AnimationSourceItem[1] (null.kine.local.posy) has 4 keys
		//INFO : New static value = 1
		//INFO : AnimationSourceItem[2] (null.kine.local.posz) has this static value: 0
		//INFO : New static value = 1.5


		// Function to remove the name of the model from the FullName of the specified parameter.
		// This is necessary when setting up a source that will later be used to instantiate a
		// clip when the parameter lives under a model other than the Scene_Root.
		CString GetRelativeName( Parameter& in_param )
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
		FCurve MakeAnFCurve( Null& in_obj, CString in_param )
		{
			Parameter p = in_obj.GetParameter( in_param );
			FCurve fc; p.AddFCurve( siStandardFCurve, fc );
			fc.AddKey( CTime(1.0), CValue(1.25) );
			fc.AddKey( CTime(25.0), CValue(2.0) );
			fc.AddKey( CTime(40.0), CValue(2.75) );
			fc.AddKey( CTime(85.0), CValue(0.0) );

			return fc;
		}

	\endcode
 */
//*****************************************************************************
class SICPPSDKDECL AnimationSourceItem : public SIObject
{
public:
	/*! Default constructor. */
	AnimationSourceItem();

	/*! Default destructor. */
	~AnimationSourceItem();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	AnimationSourceItem(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	AnimationSourceItem(const AnimationSourceItem& in_obj);

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
	\return The new AnimationSourceItem object.
	*/
	AnimationSourceItem& operator=(const AnimationSourceItem& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new AnimationSourceItem object.
	*/
	AnimationSourceItem& operator=(const CRef& in_ref);

	/*! Returns true if the animation source item is active.
	\return true if the animation source item is active.

	\eg This example illustrates how to create a simple actionsource, access the actionsource
		from the model and mute one of the source items. The modified actionsource is then used
		to create an action clip.

	\code
		using namespace XSI;

		// Forward declaration
		void MakeFCActionSource( Null& in_null );


		Application app;

		// NewScene command
		CValueArray cargs; CValue oarg;
		cargs.Add( L"" ); cargs.Add( false );
		app.ExecuteCommand( L"NewScene", cargs, oarg );
		cargs.Clear(); oarg.Clear();

		Model root = app.GetActiveSceneRoot();

		// Create a simple actionsource from some animation on the null's position
		Null n; root.AddNull( L"null", n );
		MakeFCActionSource( n );

		// Get the actionsource from the model
		CRefArray sources = root.GetSources();
		app.LogMessage( L"Number of sources on " + root.GetFullName() + L": " + CString(sources.GetCount()) );
		ActionSource src( sources.GetItem(0) );

		// Find animation source item with posx and mute it
		CRefArray srcitems = src.GetItems();
		for ( LONG i=0; i<srcitems.GetCount(); ++i ) {
			AnimationSourceItem animsrcitm( srcitems[i] );

			// Find the right target by looking for 'posx' with the StringSearch command
			cargs.Add( animsrcitm.GetTarget() );
			cargs.Add( L"posx" );
			cargs.Add( false );
			app.ExecuteCommand( L"StringSearch", cargs, oarg );
			if ( oarg.GetAsText() != L"-1" ) {
				app.LogMessage( L"muting " + animsrcitm.GetTarget() );
				animsrcitm.PutIsActive( false );
				break;			// Quit looking once it's found
			}
			cargs.Clear(); oarg.Clear();
		}

		// Apply actionsource with muted posx
		CValueArray args; CValue outarg;
		args.Add( root.GetFullName() );
		args.Add( src.GetFullName() );
		args.Add( root.GetFullName() );			// Simple clips take the model as compound owner
		app.ExecuteCommand( L"AddClip", args, outarg );

		// Find the clip
		Mixer mxr = root.GetMixer();
		CRefArray cliplist = mxr.GetClips();
		Clip clp( cliplist[0] );

		// Test the source items
		src  = clp.GetSource();
		srcitems = src.GetItems();
		for ( i=0; i<srcitems.GetCount(); ++i ) {
			AnimationSourceItem animsrcitm( srcitems[i] );
			app.LogMessage( animsrcitm.GetFullName() + L" " + animsrcitm.GetTarget() + L" is active: "
				+ CString(animsrcitm.GetIsActive()) );
		}

		// Expected results:
		//INFO : Number of sources on Scene_Root: 1
		//INFO : muting null.kine.local.posx
		//INFO : AnimationSourceItem null.kine.local.posx is active: false
		//INFO : AnimationSourceItem null.kine.local.posy is active: true
		//INFO : AnimationSourceItem null.kine.local.posz is active: true


		// Function to remove the name of the model from the FullName of the specified parameter.
		// This is necessary when setting up a source that will later be used to instantiate a
		// clip when the parameter lives under a model other than the Scene_Root.
		CString GetRelativeName( Parameter& in_param )
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

		// Create a simple actionsource from animation on the null's position
		void MakeFCActionSource( Null& in_null )
		{
			Application app;

			CTimeArray time; time.Add( CTime(1.000) ); time.Add( CTime(50.000) ); time.Add( CTime(100.000) );
			CValueArray vals;

			// X
			Parameter posx = in_null.GetParameter( L"posx" ); CString rposx = GetRelativeName(posx);
			FCurve fcx; posx.AddFCurve( siStandardFCurve, fcx );
			vals.Add( CValue(-8.153) ); vals.Add( CValue(0.197) ); vals.Add( CValue(9.413) );
			fcx.SetKeys( time, vals ); vals.Clear();

			// Y
			Parameter posy = in_null.GetParameter( L"posy" ); CString rposy = GetRelativeName(posy);
			FCurve fcy; posy.AddFCurve( siStandardFCurve, fcy );
			vals.Add( CValue(7.015) ); vals.Add( CValue(-1.92) ); vals.Add( CValue(7.015) );
			fcy.SetKeys( time, vals ); vals.Clear();

			// Z
			Parameter posz = in_null.GetParameter( L"posz" ); CString rposz = GetRelativeName(posz);
			FCurve fcz; posz.AddFCurve( siStandardFCurve, fcz );
			vals.Add( CValue(-0.702) ); vals.Add( CValue(0.192) ); vals.Add( CValue(-0.702) );
			fcz.SetKeys( time, vals );

			// StoreAction command
			CValueArray args; CValue outarg;
			args.Add( app.GetActiveSceneRoot().GetFullName() );
			args.Add( rposx + L"," + rposy + L"," + rposz );
			args.Add( CValue(2.0) );
			args.Add( L"StoredFcvAction" );
			args.Add( true );
			args.Add( CValue(1.0) );
			args.Add( CValue(100.0) );
			app.ExecuteCommand( L"StoreAction", args, outarg );
		}

	\endcode
	*/
	bool GetIsActive()const;

	/*! Sets the activeness of the animation source item (true = active; false = muted).
	\param in_bActiveState activation state.
	\return CStatus::OK success
	\return CStatus::Fail failure
	\eg See AnimationSourceItem::GetIsActive for an example of how to use the PutIsActive function.
	*/
	CStatus PutIsActive(bool in_bActiveState);

	/*! Returns the animation source (FCurve, StaticSource or ShapeKey).
	\return The animation source reference.
	\eg See AnimationSourceItem::PutSource (under the PrintSourceInfo function)
		for an example of how to use the GetSource function
	*/
	CRef GetSource()const;

	/*! Sets the animation source (FCurve or StaticSource).
	\param in_source The animation source reference.
	\sa AnimationSourceItem::SetAsStatic

	\eg Demonstrates how to overwrite an existing FCurve animation source item
		with a new FCurve using the PutSource function.

	\code
		using namespace XSI;

		// Forward declarations
		CString GetRelativeName( Parameter& in_param );
		void MakeNewFCrvSource();
		void MakeNewFCrv( FCurve& out_fc );
		void PrintClipInfo( Clip& in_clip );
		void PrintSourceInfo( ActionSource& in_src );


		Application app;

		// NewScene command
		CValueArray cargs; CValue oarg;
		cargs.Add( L"" ); cargs.Add( false );
		app.ExecuteCommand( L"NewScene", cargs, oarg );
		cargs.Clear(); oarg.Clear();

		// Get the SceneRoot
		Model root = app.GetActiveSceneRoot();
		CString rootName = root.GetFullName();

		// New fcurve ActionSource
		MakeNewFCrvSource();
		Null n = root.FindChild( L"", siNullPrimType, CStringArray() );

		// Get the ActionSource from the model & instantiate it as a clip
		CRefArray sources = root.GetSources();
		ActionSource src( sources.GetItem(0) );

		// AddClip command
		CValueArray clipArgs; CValue outClip;
		clipArgs.Add( rootName );
		clipArgs.Add( src.GetRef().GetAsText() );
		clipArgs.Add( rootName );								// Simple clips take the model as compound owner
		app.ExecuteCommand( L"AddClip", clipArgs, outClip );

		// Set up another fc (external to a source)
		FCurve fc; MakeNewFCrv( fc );

		// Find the clip
		Mixer mxr = root.GetMixer();
		CRefArray cliplist = mxr.GetClips();
		Clip clp(cliplist[0]);
		app.LogMessage( L"BEFORE source modifications:" );
		PrintClipInfo( clp );

		// Copy the fc onto the fc-based source
		CRefArray srcitems = src.GetItems();
		AnimationSourceItem oldSrcItem( srcitems.GetItem(0) );
		oldSrcItem.PutSource( fc.GetRef() );

		// Print the values on the action source after modifications to the source
		// (the instanced action in the clip and its underlying source have the same values)
		app.LogMessage( L"AFTER source modifications:" );
		PrintClipInfo( clp );

		// Expected results:
		//INFO : BEFORE source modifications:
		//INFO : Reporting on Mixer.Mixer_Anim_Track.StoredFcvAction_Clip clip:
		//INFO :  - Source used = Sources.Scene_Root.StoredFcvAction
		//INFO :  --- SrcItem #0 target = MyAnimatedNull.kine.local.posx and its datasource is a FCurve
		//INFO :             (evaluates to -2.84068 at frame 30)
		//INFO :  --- SrcItem #1 target = MyAnimatedNull.kine.local.posy and its datasource is a FCurve
		//INFO :             (evaluates to 1.3305 at frame 30)
		//INFO :  --- SrcItem #2 target = MyAnimatedNull.kine.local.posz and its datasource is a FCurve
		//INFO :             (evaluates to -0.133232 at frame 30)
		//INFO :
		//INFO : AFTER source modifications:
		//INFO : Reporting on Mixer.Mixer_Anim_Track.StoredFcvAction_Clip clip:
		//INFO :  - Source used = Sources.Scene_Root.StoredFcvAction
		//INFO :  --- SrcItem #0 target = MyAnimatedNull.kine.local.posx and its datasource is a FCurve
		//INFO :             (evaluates to 2.84068 at frame 30)
		//INFO :  --- SrcItem #1 target = MyAnimatedNull.kine.local.posy and its datasource is a FCurve
		//INFO :             (evaluates to 1.3305 at frame 30)
		//INFO :  --- SrcItem #2 target = MyAnimatedNull.kine.local.posz and its datasource is a FCurve
		//INFO :             (evaluates to -0.133232 at frame 30)
		//INFO :


		// Function to remove the name of the model from the FullName of the specified parameter.
		// This is necessary when setting up a source that will later be used to instantiate a
		// clip when the parameter lives under a model other than the Scene_Root.
		CString GetRelativeName( Parameter& in_param )
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

		// Create a simple (fcurve) actionsource based on a null's position
		void MakeNewFCrvSource()
		{
			Application app;
			Model root = app.GetActiveSceneRoot();
			Null n; root.AddNull( L"MyAnimatedNull", n );

			CTimeArray time; time.Add( CTime(1.000) ); time.Add( CTime(50.000) ); time.Add( CTime(100.000) );
			CValueArray vals;

			// X
			Parameter posx = n.GetParameter( L"posx" ); CString rposx = GetRelativeName(posx);
			FCurve fcx; posx.AddFCurve( siStandardFCurve, fcx );
			vals.Add( CValue(-8.153) ); vals.Add( CValue(0.197) ); vals.Add( CValue(9.413) );
			fcx.SetKeys( time, vals ); vals.Clear();

			// Y
			Parameter posy = n.GetParameter( L"posy" ); CString rposy = GetRelativeName(posy);
			FCurve fcy; posy.AddFCurve( siStandardFCurve, fcy );
			vals.Add( CValue(7.015) ); vals.Add( CValue(-1.92) ); vals.Add( CValue(7.015) );
			fcy.SetKeys( time, vals ); vals.Clear();

			// Z
			Parameter posz = n.GetParameter( L"posz" ); CString rposz = GetRelativeName(posz);
			FCurve fcz; posz.AddFCurve( siStandardFCurve, fcz );
			vals.Add( CValue(-0.702) ); vals.Add( CValue(0.192) ); vals.Add( CValue(-0.702) );
			fcz.SetKeys( time, vals );

			// StoreAction command
			CValueArray args; CValue outArg;
			args.Add( CRef(root) );
			args.Add( rposx + L"," + rposy + L"," + rposz );
			args.Add( 2.0 );
			args.Add( L"StoredFcvAction" );
			args.Add( true );
			args.Add( 1.0 );
			args.Add( 100.0 );
			app.ExecuteCommand( L"StoreAction", args, outArg );
		}


		// Convenience function to create an fcurve
		void MakeNewFCrv( FCurve& out_fc )
		{
			Application app;
			Model root = app.GetActiveSceneRoot();
			Null n; root.AddNull( L"MyDummyNull", n );
			CRefArray kids; kids.Add( n );
			Model m; root.AddModel( kids, L"MyDummyModel", m );

			CTimeArray time; time.Add( CTime(1.000) ); time.Add( CTime(50.000) ); time.Add( CTime(100.000) );
			CValueArray vals; vals.Add( CValue(8.153) ); vals.Add( CValue(-0.197) ); vals.Add( CValue(-9.413) );

			// X
			Parameter posx = n.GetParameter( L"posx" );
			posx.AddFCurve( siStandardFCurve, out_fc );
			out_fc.SetKeys( time, vals );
		}

		// Convenience function to print out information about the clip's source
		void PrintClipInfo( Clip& in_clip )
		{
			Application app;

			// Clip
			app.LogMessage( L"Reporting on " + in_clip.GetFullName() + L" clip:" );

			// Clip's source
			PrintSourceInfo( ActionSource(in_clip.GetSource()) );
		}

		// Convenience function to print out information about the source
		void PrintSourceInfo( ActionSource& in_src )
		{
			Application app;

			// Source
			app.LogMessage( L" - Source used = " + in_src.GetFullName() );

			// Source's items
			CRefArray itemsRef = in_src.GetItems();
			for ( LONG i=0; i<itemsRef.GetCount(); ++i ) {
				AnimationSourceItem animsrcitm( itemsRef[i] );
				app.LogMessage( L" --- SrcItem #" + CString(i) + L" target = " + animsrcitm.GetTarget()
					+ L" and its datasource is a " + animsrcitm.GetSource().GetClassIDName() );

				// Underlying data source is an FCurve
				if ( animsrcitm.GetSource().GetClassID() == siFCurveID ) {
					FCurve fc( animsrcitm.GetSource() );
					app.LogMessage( L"            (evaluates to " + fc.Eval( CTime(30.0) ).GetAsText() + L" at frame 30)" );
				}

				// Underlying data source is a StaticSource
				if ( animsrcitm.GetSource().GetClassID() == siStaticSourceID ) {
					StaticSource sv( animsrcitm.GetSource() );
					app.LogMessage( L"            (evaluates to " + sv.GetValue().GetAsText() + L")" );
				}
			}

			// Extra blank line
			app.LogMessage( L"" );
		}
	\endcode
	*/
	CStatus PutSource(CRef in_source);

	/*! Returns the complete name of the animation target (the Parameter to which the
	animation has been applied) as a \xl RelativeName.
	\note ActionSources store the relative name instead of the FullName of a
		parameter so actions can be easily copied from one Model to another.
	\return The \xl RelativeName (SIObject::GetFullName without the modelname prefix).

	\eg This example illustrates how to create a simple actionsource from some position animation.
	   	AnimationSourceItem::Source property is used to get the fcurve source and modify the keys.

   	\code
		using namespace XSI;

		// Forward declarations
		CString GetRelativeName( Parameter& in_param );
		void MakeNewFCrvSource( Null& in_null );
		void PrintClipInfo( Clip& in_clip );
		void PrintSourceInfo( ActionSource& in_src );


		Application app;

		// NewScene command
		CValueArray cargs; CValue oarg;
		cargs.Add( L"" ); cargs.Add( false );
		app.ExecuteCommand( L"NewScene", cargs, oarg );
		cargs.Clear(); oarg.Clear();

		// Get the SceneRoot
		Model root = app.GetActiveSceneRoot();

		// Create a new model and add a null
		Model m; root.AddModel( CRefArray(), L"MyModel", m );
		Null n; m.AddNull( L"MyNull", n );

		// Add an fcurve ActionSource based on the new null
		MakeNewFCrvSource( n );

		// Get the ActionSource from the model
		CRefArray sources = m.GetSources();
		ActionSource src( sources.GetItem(0) );
		app.LogMessage( L"BEFORE source modifications:" );
		PrintSourceInfo( src );

		// Get the relative name for the posx parameter
		Parameter posx = n.GetParameter( L"posx" );
		CString rp = GetRelativeName( posx );

		// Find animation source item with posx
		CRefArray srcitems = src.GetItems();
		for ( LONG i=0; i<srcitems.GetCount(); ++i ) {
			AnimationSourceItem itm( srcitems.GetItem(i) );
			if ( itm.GetTarget() == rp ) {
				// Change the fcurve keys on the posx source
				FCurve fc( itm.GetSource() );
				CTimeArray t; t.Add( CTime(1) ); t.Add( CTime(50) ); t.Add( CTime(100) );
				CValueArray v; v.Add( CValue(-8.0) ); v.Add( CValue(8.0) ); v.Add( CValue(9.0) );
				fc.SetKeys( t, v );

				break;
			}
		}

		// Print modified info
		app.LogMessage( L"AFTER source modifications:" );
		PrintSourceInfo( src );


		// Expected results:
		//INFO : BEFORE source modifications:
		//INFO :  - Source used = Sources.MyModel.StoredFcvAction
		//INFO :    Underlying fc evaluates to -2.84068 at frame 30) on MyNull.kine.local.posx
		//INFO :
		//INFO : AFTER source modifications:
		//INFO :  - Source used = Sources.MyModel.StoredFcvAction
		//INFO :    Underlying fc evaluates to 2.1793 at frame 30) on MyNull.kine.local.posx
		//INFO :


		// Function to remove the name of the model from the FullName of the specified parameter.
		// This is necessary when setting up a source that will later be used to instantiate a
		// clip when the parameter lives under a model other than the Scene_Root.
		CString GetRelativeName( Parameter& in_param )
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

		// Create a simple (fcurve) actionsource based on a null's position
		void MakeNewFCrvSource( Null& in_null )
		{
			Application app;
			Model mdl = in_null.GetModel();

			CTimeArray time; time.Add( CTime(1.000) ); time.Add( CTime(50.000) ); time.Add( CTime(100.000) );
			CValueArray vals;

			// X
			Parameter posx = in_null.GetParameter( L"posx" ); CString rposx = GetRelativeName(posx);
			FCurve fcx; posx.AddFCurve( siStandardFCurve, fcx );
			vals.Add( CValue(-8.153) ); vals.Add( CValue(0.197) ); vals.Add( CValue(9.413) );
			fcx.SetKeys( time, vals ); vals.Clear();

			// Y
			Parameter posy = in_null.GetParameter( L"posy" ); CString rposy = GetRelativeName(posy);
			FCurve fcy; posy.AddFCurve( siStandardFCurve, fcy );
			vals.Add( CValue(7.015) ); vals.Add( CValue(-1.92) ); vals.Add( CValue(7.015) );
			fcy.SetKeys( time, vals ); vals.Clear();

			// Z
			Parameter posz = in_null.GetParameter( L"posz" ); CString rposz = GetRelativeName(posz);
			FCurve fcz; posz.AddFCurve( siStandardFCurve, fcz );
			vals.Add( CValue(-0.702) ); vals.Add( CValue(0.192) ); vals.Add( CValue(-0.702) );
			fcz.SetKeys( time, vals );

			// Build an action with the fcurves as source items
			ActionSource src = mdl.AddActionSource( L"StoredFcvAction" );
			src.AddSourceItem( rposx, fcx, true );
			src.AddSourceItem( rposy, fcy, true );
			src.AddSourceItem( rposz, fcz, true );
		}

		// Convenience function to print out information about the source
		void PrintSourceInfo( ActionSource& in_src )
		{
			Application app;
			app.LogMessage( L" - Source used = " + in_src.GetFullName() );

			// Get the Source's first item
			CRefArray itemsRef = in_src.GetItems();
			AnimationSourceItem animsrcitm( itemsRef[0] );

			// Get underlying fcurve
			FCurve fc( animsrcitm.GetSource() );
			app.LogMessage( L"   Underlying fc evaluates to " + fc.Eval( CTime(30.0) ).GetAsText()
				+ L" at frame 30 on " + animsrcitm.GetTarget() );

			// Extra blank line
			app.LogMessage( L"" );
		}

	\endcode
	*/
	CString GetTarget()const;

	/*! Sets the complete name of the object to be animated by this animation source item as a \xl RelativeName.
	\note ActionSources store the relative name instead of the FullName of a parameter so actions can be easily
		copied from one Model to another.
	\param in_target The \xl RelativeName (SIObject::GetFullName without the modelname prefix)
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus PutTarget(CString& in_target);

	/*! Sets the source of the source item as a StaticSource (static value).
	\param in_vValue The value of the AnimationSourceItem.
	\retval out_static The newly created StaticSource.
	\return CStatus::OK success
	\return CStatus::Fail failure

	\eg Illustrates some of the different ways you can work with the value of an AnimationSourceItem's
		StaticSource, from creating using AddSourceItem and SetAsStaticValue to editing an existing static
		value using the GetSournce/PutSource combo.

	\code
		using namespace XSI;

		// Forward declarations
		CString GetRelativeName( Parameter& in_param );
		void MakeStaticActionSource();
		void PrintClipInfo( Clip& in_clip );
		void PrintSourceInfo( ActionSource& in_src );


		Application app;

		// NewScene command
		CValueArray cargs; CValue oarg;
		cargs.Add( L"" ); cargs.Add( false );
		app.ExecuteCommand( L"NewScene", cargs, oarg );
		cargs.Clear(); oarg.Clear();

		// Get the SceneRoot
		Model root = app.GetActiveSceneRoot();
		CString rootName = root.GetFullName();

		// New static ActionSource
		MakeStaticActionSource();
		Null n = root.FindChild( L"", siNullPrimType, CStringArray() );

		// Get the ActionSource from the model & instantiate it as a clip
		CRefArray sources = root.GetSources();
		ActionSource src( sources.GetItem(0) );

		// AddClip command
		CValueArray clipArgs; CValue outClip;
		clipArgs.Add( rootName );
		clipArgs.Add( src.GetRef().GetAsText() );
		clipArgs.Add( rootName );								// Simple clips take the model as compound owner
		app.ExecuteCommand( L"AddClip", clipArgs, outClip );

		// Find the clip
		Mixer mxr = root.GetMixer();
		CRefArray cliplist = mxr.GetClips();
		Clip clp(cliplist[0]);
		app.LogMessage( L"BEFORE source modifications:" );
		PrintClipInfo( clp );

		// Get the AnimationSourceItem
		CRefArray srcitems = src.GetItems();
		AnimationSourceItem oldSrcItem( srcitems.GetItem(0) );

		// Use the AnimationSourceItem::GetSource function to get the StaticSource and then change its value
		StaticSource sv( oldSrcItem.GetSource() );
		sv.PutValue( CValue(2.5) );

		// Print the values on the action source after modifications to the source
		app.LogMessage( L"AFTER source modifications:" );
		PrintClipInfo( clp );

		// Expected results:
		//INFO : BEFORE source modifications:
		//INFO : Reporting on Mixer.Mixer_Anim_Track.Action_Clip clip:
		//INFO :  - Source used = Sources.Scene_Root.Action
		//INFO :  --- SrcItem #0 target = MyAnimatedNull.kine.local.posx and its datasource is a StaticSource
		//INFO :             (evaluates to 0)
		//INFO :  --- SrcItem #1 target = MyAnimatedNull.kine.local.posy and its datasource is a StaticSource
		//INFO :             (evaluates to 1)
		//INFO :  --- SrcItem #2 target = MyAnimatedNull.kine.local.posz and its datasource is a StaticSource
		//INFO :             (evaluates to 2)
		//INFO :
		//INFO : AFTER source modifications:
		//INFO : Reporting on Mixer.Mixer_Anim_Track.Action_Clip clip:
		//INFO :  - Source used = Sources.Scene_Root.Action
		//INFO :  --- SrcItem #0 target = MyAnimatedNull.kine.local.posx and its datasource is a StaticSource
		//INFO :             (evaluates to 2.5)
		//INFO :  --- SrcItem #1 target = MyAnimatedNull.kine.local.posy and its datasource is a StaticSource
		//INFO :             (evaluates to 1)
		//INFO :  --- SrcItem #2 target = MyAnimatedNull.kine.local.posz and its datasource is a StaticSource
		//INFO :             (evaluates to 2)
		//INFO :


		// Function to remove the name of the model from the FullName of the specified parameter.
		// This is necessary when setting up a source that will later be used to instantiate a
		// clip when the parameter lives under a model other than the Scene_Root.
		CString GetRelativeName( Parameter& in_param )
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

		// Create a simple actionsource based on static values
		void MakeStaticActionSource()
		{
			Application app;
			Model root = app.GetActiveSceneRoot();
			Null n; root.AddNull( L"MyAnimatedNull", n );

			// Get the relative names for the parameter targets
			Parameter posx = n.GetParameter( L"posx" ); CString rposx = GetRelativeName(posx);
			Parameter posy = n.GetParameter( L"posy" ); CString rposy = GetRelativeName(posy);
			Parameter posz = n.GetParameter( L"posz" ); CString rposz = GetRelativeName(posz);

			// Create an empty action source
			ActionSource src = root.AddActionSource();

			// Populate it with a static value using ActionSource::AddSourceItem (default=0.0)
			src.AddSourceItem( rposx );

			// Populate it with a specified static value using ActionSource::AddSourceItem
			src.AddSourceItem( rposy, double(1.0), true );

			// Populate it with the default static value, then set a real value later with AnimationSourceItem::SetAsStatic
			AnimationSourceItem animsrcitem = src.AddSourceItem( rposz );
			animsrcitem.SetAsStatic( CValue(2.0), StaticSource() );
		}

		// Convenience function to print out information about the clip's source
		void PrintClipInfo( Clip& in_clip )
		{
			Application app;

			// Clip
			app.LogMessage( L"Reporting on " + in_clip.GetFullName() + L" clip:" );

			// Clip's source
			PrintSourceInfo( ActionSource(in_clip.GetSource()) );
		}

		// Convenience function to print out information about the source
		void PrintSourceInfo( ActionSource& in_src )
		{
			Application app;

			// Source
			app.LogMessage( L" - Source used = " + in_src.GetFullName() );

			// Source's items
			CRefArray itemsRef = in_src.GetItems();
			for ( LONG i=0; i<itemsRef.GetCount(); ++i ) {
				AnimationSourceItem animsrcitm( itemsRef[i] );
				app.LogMessage( L" --- SrcItem #" + CString(i) + L" target = " + animsrcitm.GetTarget() + L" and its datasource is a " + animsrcitm.GetSource().GetClassIDName() );

				// Underlying data source is an FCurve
				if ( animsrcitm.GetSource().GetClassID() == siFCurveID ) {
					FCurve fc( animsrcitm.GetSource() );
					app.LogMessage( L"            (evaluates to " + fc.Eval( CTime(30.0) ).GetAsText() + L" at frame 30)" );
				}

				// Underlying data source is a StaticSource
				if ( animsrcitm.GetSource().GetClassID() == siStaticSourceID ) {
					StaticSource sv( animsrcitm.GetSource() );
					app.LogMessage( L"            (evaluates to " + sv.GetValue().GetAsText() + L")" );
				}
			}

			// Extra blank line
			app.LogMessage( L"" );
		}
	\endcode
	*/
	CStatus SetAsStatic
	(
		CValue&			in_vValue,
		StaticSource&	out_static
	);

	private:
	AnimationSourceItem * operator&() const;
	AnimationSourceItem * operator&();
};

}

#endif // __XSIANIMATIONSOURCEITEM_H__
