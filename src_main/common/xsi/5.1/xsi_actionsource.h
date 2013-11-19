//*****************************************************************************
/*!	\file xsi_actionsource.h
	\brief ActionSource class declaration.

	© Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
	reserved. This file contains confidential and proprietary information of
	Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
	end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIACTIONSOURCE_H__
#define __XSIACTIONSOURCE_H__

#include <xsi_projectitem.h>

namespace XSI {

class CStatus;
class Property;
class AnimationSourceItem;

//*****************************************************************************
/*! \class ActionSource xsi_actionsource.h
	\brief The ActionSource class is a specialization of the Source class, representing sources for
	action and shape animation clips.

	These types of sources are composed of \xl DataSource items (such as FCurve objects, Expression
	objects, etc.), which you can access as AnimationSourceItem objects via ActionSource::GetItems.
	See the siAnimationSourceItemType enum for a list of supported animation source types.

	Actions are based on changes to parameters, usually called \a low-level \a animation because
	it is parameter-based. Typically, action components are FCurve items, Constraint items, StaticSource
	items (static poses), and ShapeKey items. You can bundle these components as action sources
	using Model::AddActionSource.

	Shape animation is based on changes to the positions of points, thereby changing the overall shape
	of the geometry from the reference shape. (The reference shape comprises the results of the modeling
	operator stack.) These changes to the clusters are saved as ShapeKey objects under the \b Clusters
	node of the object's %Geometry in the UI. In the Mixer, the %ShapeKey is instantiated as a ShapeClip.

	\note In action sources, the path to the parameter to which it will be applied by default is
	always stored as a \xl RelativeName so that sources can be instantiated under any other model
	without having to necessarily provide a connection mapping template.

	Action sources are often tied to the %Mixer because of the Mixer's interaction with Clips; however,
	actions which have never been instantiated or which represent caching in the SimulationEnvironment
	may not be associated with any %Mixer or %Model, but only under the \c Sources container or the \c %Scene
	> \c Environments > \c Environment > \c Caches container. In these cases, some of the functionality
	is limited (for example, calling the base properties SIObject::GetParent or ProjectItem::GetModel
	will not work since there is no %Model that contains these sources.

	To get a pointer to an existing action source from its clip, you can use Clip::GetSource. All action
	sources, whether instantiated or not, are also available as a CRefArray of Source objects (DataSources)
	from the Model to which they belong via Model::GetSources.

	To create %ActionSource objects using the C++ API, you can use Model::AddActionSource. There is also
	a variety of scripting commands that create %ActionSource objects by loading presets
	(\xl LoadActionPreset command), storing current low-level animation (\xl SIStoreAction command and
	\xl StoreAction), and by plotting (\xl PlotToAction and \xl PlotAndApplyAction).

	\note In order to avoid breaking backward compatibility the class hierarchy of %ActionSource has
	remained the same, however you can build a Source class from the CRef of an %ActionSource class.

	\sa Clip::GetSource, Model::GetSources, Model::AddActionSource, \xl ImportAction,
	\xl LoadActionPreset, \xl SIStoreAction, \xl StoreAction, \xl PlotToAction,
	\xl PlotAndApplyAction, \xl CreateEnvironmentCache, \xl StoreEnvironmentCacheToMixer

  	\eg This example illustrates how to create a simple actionsource from position
	animation. AnimationSourceItem::GetSource gets the fcurve source and modifies the keys.

	\code
		using namespace XSI;

		// Forward declaration
		CString GetRelativePath( Parameter& in_param );


		Application app;

		// NewScene command
		CValueArray cargs; CValue oarg;
		cargs.Add( L"" ); cargs.Add( false );
		app.ExecuteCommand( L"NewScene", cargs, oarg );
		cargs.Clear();

		// Get the scene root
		Model root = app.GetActiveSceneRoot();

		//
		// Create a simple actionsource from animation on the null's position
		//
		Null n; root.AddNull( L"null", n );
		CTimeArray time; time.Add( CTime(1.000) ); time.Add( CTime(50.000) ); time.Add( CTime(100.000) );
		CValueArray vals;

		// X
		Parameter posx = n.GetParameter( L"posx" ); CString rposx = GetRelativePath(posx);
		FCurve fcx; posx.AddFCurve( siStandardFCurve, fcx );
		vals.Add( CValue(-8.153) ); vals.Add( CValue(0.197) ); vals.Add( CValue(9.413) );
		fcx.SetKeys( time, vals ); vals.Clear();

		// Y
		Parameter posy = n.GetParameter( L"posy" ); CString rposy = GetRelativePath(posy);
		FCurve fcy; posy.AddFCurve( siStandardFCurve, fcy );
		vals.Add( CValue(7.015) ); vals.Add( CValue(-1.92) ); vals.Add( CValue(7.015) );
		fcy.SetKeys( time, vals ); vals.Clear();

		// Z
		Parameter posz = n.GetParameter( L"posz" ); CString rposz = GetRelativePath(posz);
		FCurve fcz; posz.AddFCurve( siStandardFCurve, fcz );
		vals.Add( CValue(-0.702) ); vals.Add( CValue(0.192) ); vals.Add( CValue(-0.702) );
		fcz.SetKeys( time, vals );

		// StoreAction command
		cargs.Add( root.GetName() );
		cargs.Add( rposx+L","+rposy+L","+rposz );
		cargs.Add( CValue(2.0) );
		app.ExecuteCommand( L"StoreAction", cargs, oarg ); cargs.Clear();

		// Get the actionsource from the model
		CRefArray srclist = root.GetSources();
		ActionSource src = srclist.GetItem(0);

		// Find animation source item with posx and mute it
		CRefArray srcitems = src.GetItems();
		for ( LONG i=0; i<srcitems.GetCount(); ++i ) {
			AnimationSourceItem item = srcitems[i];
			if ( item.GetTarget() == posx.GetFullName() ) {
			   app.LogMessage( L"muting " + item.GetTarget() );		//INFO : muting null.kine.local.posx
			   item.PutIsActive( false );
			   break;
			}
		}

		// Use AddClip command to instantiate the action with muted posx as a clip in the mixer
		cargs.Add( n.GetModel().GetName() ); cargs.Add( src.GetName() );
		app.ExecuteCommand( L"AddClip", cargs, oarg );


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
//*****************************************************************************
class SICPPSDKDECL ActionSource : public ProjectItem
{
public:
	/*! Default constructor. */
	ActionSource();

	/*! Default destructor. */
	~ActionSource();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	ActionSource(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	ActionSource(const ActionSource& in_obj);

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
	\return The new ActionSource object.
	*/
	ActionSource& operator=(const ActionSource& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new ActionSource object.
	*/
	ActionSource& operator=(const CRef& in_ref);

	/*! Returns an array of AnimationSourceItem objects for this source. For now only shape and animation
	sources return source items.
	\return Array of animation items contained in the source.
	\sa \xl %ActionSource.SourceItems
	\since 4.0
	*/
	CRefArray GetItems()const;

	/*! Reloads an animation source from disk. This function is useful when you want to find externally stored
	action sources that have become disconnected from a Clip.

	You can also use this function to reload a source which has been offloaded. If the source has external
	storage and the file is valid, the source removes all current AnimationSourceItem objects and reloads them
	from the source file.

	Reloading the source disconnects all action Clip objects that use this source and reconnects them (this
	happens even if the source is stored internally or if the file is invalid). Once the paths are updated,
	reloading the source reconnects everything correctly.

	\return CStatus::OK success
	\return CStatus::Fail is returned in case of failure
	\since 4.0

  	\eg Illustrates how to use the Reload method on an ActionSource.

	\code
		using namespace XSI;

		// Forward declaration
		CString GetRelativePath( Parameter& in_param );


		Application app;

		// NewScene command
		CValueArray cargs; CValue oarg;
		cargs.Add( L"" ); cargs.Add( false );
		app.ExecuteCommand( L"NewScene", cargs, oarg );
		cargs.Clear();

		// Get the scene root
		Model root = app.GetActiveSceneRoot();

		//
		// Create a simple actionsource from animation on the null's position
		//
		Null n; root.AddNull( L"null", n );
		CTimeArray time; time.Add( CTime(1.000) ); time.Add( CTime(50.000) ); time.Add( CTime(100.000) );
		CValueArray vals;

		// X
		Parameter posx = n.GetParameter( L"posx" ); CString rposx = GetRelativePath(posx);
		FCurve fcx; posx.AddFCurve( siStandardFCurve, fcx );
		vals.Add( CValue(-8.153) ); vals.Add( CValue(0.197) ); vals.Add( CValue(9.413) );
		fcx.SetKeys( time, vals ); vals.Clear();

		// Y
		Parameter posy = n.GetParameter( L"posy" ); CString rposy = GetRelativePath(posy);
		FCurve fcy; posy.AddFCurve( siStandardFCurve, fcy );
		vals.Add( CValue(7.015) ); vals.Add( CValue(-1.92) ); vals.Add( CValue(7.015) );
		fcy.SetKeys( time, vals ); vals.Clear();

		// Z
		Parameter posz = n.GetParameter( L"posz" ); CString rposz = GetRelativePath(posz);
		FCurve fcz; posz.AddFCurve( siStandardFCurve, fcz );
		vals.Add( CValue(-0.702) ); vals.Add( CValue(0.192) ); vals.Add( CValue(-0.702) );
		fcz.SetKeys( time, vals );

		// Create a new model so that we can have another null called "null"
		Model mdl; CRefArray kids; root.AddModel( kids, L"Model", mdl );

		// Create a new ActionSource
		ActionSource src = mdl.AddActionSource( L"MyActionSource" );

		// Use the fcurves on the null object to create the actionsource items
		src.AddSourceItem( rposx, posx.GetSource() );
		src.AddSourceItem( rposy, posx.GetSource() );
		src.AddSourceItem( rposz, posx.GetSource() );

		// Create another null called 'null'
		Null nn; mdl.AddNull( L"null", nn );

		// Set the action storage to external (text):
		src.PutParameterValue( L"storage", CValue(2.0) );

		// Use AddClip command to instantiate the action as a clip in the mixer
		cargs.Resize( 9 );
		cargs[0] = nn.GetFullName();
		cargs[1] = L"Sources.Animation.Model.MyActionSource";
		cargs[4] = CValue( 16.0 );
		cargs[8] = false;
		app.ExecuteCommand( L"AddClip", cargs, oarg );
		cargs.Clear();

		// Save the scene, in order to save the action externally
		cargs.Add( app.GetInstallationPath( siProjectPath ) + L"\\Scenes\\ASTestScene.scn" );
		cargs.Add( true );
		app.ExecuteCommand( L"SaveSceneAs", cargs, oarg );

		// So, calling Reload on the source will disconnect the source, reload the action and reconnect the source.
		src.Reload();


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
	CStatus Reload();

	/*! Offloads an animation source on disk. The source storage must be set to external and the file must be valid.
	The source is saved and all current AnimationSourceItem objects are removed.  All Clip objects that use the source
	are notified and are disconnected (the clip's color will turn gray) and the source icon in the explorer changes.

	\note Offloading is only available for animation and not for shape or audio.

	\return CStatus::OK success
	\return CStatus::Fail is returned in case of failure
	\since 4.0

  	\eg Illustrates how to use the Offload method on an ActionSource.

	\code
		using namespace XSI;

		// Forward declaration
		CString GetRelativePath( Parameter& in_param );


		Application app;

		// NewScene command
		CValueArray cargs; CValue oarg;
		cargs.Add( L"" ); cargs.Add( false );
		app.ExecuteCommand( L"NewScene", cargs, oarg );
		cargs.Clear();

		// Get the scene root
		Model root = app.GetActiveSceneRoot();

		//
		// Create a simple actionsource from animation on the null's position
		//
		Null n; root.AddNull( L"null", n );
		CTimeArray time; time.Add( CTime(1.000) ); time.Add( CTime(50.000) ); time.Add( CTime(100.000) );
		CValueArray vals;

		// X
		Parameter posx = n.GetParameter( L"posx" ); CString rposx = GetRelativePath(posx);
		FCurve fcx; posx.AddFCurve( siStandardFCurve, fcx );
		vals.Add( CValue(-8.153) ); vals.Add( CValue(0.197) ); vals.Add( CValue(9.413) );
		fcx.SetKeys( time, vals ); vals.Clear();

		// Y
		Parameter posy = n.GetParameter( L"posy" ); CString rposy = GetRelativePath(posy);
		FCurve fcy; posy.AddFCurve( siStandardFCurve, fcy );
		vals.Add( CValue(7.015) ); vals.Add( CValue(-1.92) ); vals.Add( CValue(7.015) );
		fcy.SetKeys( time, vals ); vals.Clear();

		// Z
		Parameter posz = n.GetParameter( L"posz" ); CString rposz = GetRelativePath(posz);
		FCurve fcz; posz.AddFCurve( siStandardFCurve, fcz );
		vals.Add( CValue(-0.702) ); vals.Add( CValue(0.192) ); vals.Add( CValue(-0.702) );
		fcz.SetKeys( time, vals );

		// Create a new model so that we can have another null called "null"
		Model mdl; CRefArray kids; root.AddModel( kids, L"Model", mdl );

		// Create a new ActionSource
		ActionSource src = mdl.AddActionSource( L"MyActionSource" );

		// Use the fcurves on the null object to create the actionsource items
		src.AddSourceItem( rposx, posx.GetSource() );
		src.AddSourceItem( rposy, posx.GetSource() );
		src.AddSourceItem( rposz, posx.GetSource() );

		// Create another null called 'null'
		Null nn; mdl.AddNull( L"null", nn );

		// Set the action storage to external (text):
		src.PutParameterValue( L"storage", CValue(2.0) );
		src.GetFullName();

		// Use AddClip command to instantiate the action as a clip in the mixer
		cargs.Resize( 9 );
		cargs[0] = nn.GetFullName();
		cargs[1] = L"Sources.Animation.Model.MyActionSource";
		cargs[4] = CValue( 16.0 );
		cargs[8] = false;
		app.ExecuteCommand( L"AddClip", cargs, oarg );
		cargs.Clear();

		// Save the scene, in order to save the action externally
		cargs.Add( app.GetInstallationPath( siProjectPath ) + L"\\Scenes\\ASTestScene.scn" );
		cargs.Add( true );
		app.ExecuteCommand( L"SaveSceneAs", cargs, oarg );

		// Offload the sources.
		src.Offload();


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
	CStatus Offload();

	/*! Partially offloads the fcurve item's one or many action sources for a given range of frames.  Note that this
	applies only to FCurve items in the action.

	The action source needs to have an external storage in order to be able to execute this command. Before offloading
	an action source, if the action is dirty it will be exported. Once offloaded or partially offloaded, an action source
	will never export the data again. When an action source is partially offloaded, the status parameter of the source
	becomes siActionStatusPartiallyLoaded.

	\return CStatus::OK success
	\return CStatus::Fail is returned in case of failure
	\sa \xl %ActionSource.SourceItems
	\since 4.0

  	\eg This example demonstrates how to partially offload an animation source.

	\code
		using namespace XSI;

		// Forward declaration
		ActionSource AddActionSourceDemo( X3DObject& sph );


		Application app;

		// NewScene command
		CValueArray cargs; CValue oarg;
		cargs.Add( L"" ); cargs.Add( false );
		app.ExecuteCommand( L"NewScene", cargs, oarg );
		cargs.Clear();

		// Get the scene root
		Model root = app.GetActiveSceneRoot();

		// Create an animation source on a sphere.
		X3DObject sphere; root.AddGeometry( L"Sphere", L"MeshSurface", L"MySphere", sphere );
		CRefArray kids; kids.Add( sphere );
		Model mdl; root.AddModel( kids, L"MyModel", mdl );
		ActionSource src = AddActionSourceDemo( sphere );

		// Add a clip of the source in the mixer
		cargs.Add( mdl.GetFullName() );
		cargs.Add( src.GetFullName() );
		app.ExecuteCommand( L"AddClip", cargs, oarg );
		cargs.Clear();

		// We need to set the external storage to external if we want to be able
		// to offload the source
		src.PutParameterValue( L"storage", CValue(3.0) );

		// Now offload the fcurve in the section of frames 10 to 20
		src.PartialOffload( LONG(10.0), LONG(20.0), false );
		app.LogMessage( src.GetParameterValue(L"status") );
		app.LogMessage( src.GetParameterValue(L"OffloadedRanges") );

		// Note that this partial offload as not been added to the OffloadedRanges parameter of the source.
		// reloading the action will dispose the temporary offloaded ranges
		src.Reload();

		src.PartialOffload( LONG(12.0), LONG(22.0), true );
		app.LogMessage( src.GetParameterValue(L"status") );
		app.LogMessage( src.GetParameterValue(L"OffloadedRanges") );

		// Expected result:
		//INFO : 1
		//INFO :
		//INFO : 1
		//INFO : 12,22


		// Function to create an animation source on a sphere (converted from commands cut-and-pasted
		// from Script Editor's History)
		ActionSource AddActionSourceDemo( X3DObject& sph )
		{
			Application app;

			// For executing scripting commands
			CValueArray args; CValue outarg;

			// Get the name and the pos param string
			CString namestr = sph.GetFullName();
			CString posparamstr = namestr + L"*\/kine.local.pos";

			//Translate(null, -7.85065649868519, 0.715575131336405, -7.15575131336405E-02,
			//	siRelative, siView, siObj, siXYZ, null, null, null, null, null, null, null, null, null);
			args.Add( namestr );
			args.Add( CValue(-7.85065649868519) ); args.Add( CValue(0.715575131336405) ); args.Add( CValue(-7.15575131336405E-02) );
			args.Add( CValue(1.0) ); args.Add( CValue(4.0) ); args.Add( CValue(0.0) ); args.Add( CValue(0.0) );
			app.ExecuteCommand( L"Translate", args, outarg );
			args.Clear();

			//SaveKey("sphere.kine.local.posx,sphere.kine.local.posy,sphere.kine.local.posz", 1, null, null);
			args.Add( posparamstr );
			args.Add( CValue(1.0) );
			app.ExecuteCommand( L"SaveKey", args, outarg );
			args.Clear();

			//SetValue("PlayControl.Key", 10, null);
			args.Add( L"PlayControl.Key" );
			args.Add( CValue(10.0) );
			app.ExecuteCommand( L"SetValue", args, outarg );
			args.Clear();

			//SetValue("PlayControl.Current", 10, null);
			args.Add( L"PlayControl.Current" );
			args.Add( CValue(10.0) );
			app.ExecuteCommand( L"SetValue", args, outarg );
			args.Clear();

			//Translate(null, 4.19500728937374, 7.06630442194682, -0.706630442194682,
			//	siRelative, siView, siObj, siXYZ, null, null, null, null, null, null, null, null, null);
			args.Add( namestr );
			args.Add( CValue(4.19500728937374) ); args.Add( CValue(7.06630442194682) ); args.Add( CValue(-0.706630442194682) );
			args.Add( CValue(1.0) ); args.Add( CValue(4.0) ); args.Add( CValue(0.0) ); args.Add( CValue(0.0) );
			app.ExecuteCommand( L"Translate", args, outarg );
			args.Clear();

			//SaveKey("sphere.kine.local.posx,sphere.kine.local.posy,sphere.kine.local.posz", 10, null, null);
			args.Add( posparamstr );
			args.Add( CValue(10.0) );
			app.ExecuteCommand( L"SaveKey", args, outarg );
			args.Clear();

			//SetValue("PlayControl.Key", 20, null);
			args.Add( L"PlayControl.Key" );
			args.Add( CValue(20.0) );
			app.ExecuteCommand( L"SetValue", args, outarg );
			args.Clear();

			//SetValue("PlayControl.Current", 20, null);
			args.Add( L"PlayControl.Current" );
			args.Add( CValue(20.0) );
			app.ExecuteCommand( L"SetValue", args, outarg );
			args.Clear();

			//Translate(null, 4.25493596493627, -7.84151081422792, 0.784151081422792,
			//	siRelative, siView, siObj, siXYZ, null, null, null, null, null, null, null, null, null);
			args.Add( namestr );
			args.Add( CValue(4.25493596493627) ); args.Add( CValue(-7.84151081422792) ); args.Add( CValue(0.784151081422792) );
			args.Add( CValue(1.0) ); args.Add( CValue(4.0) ); args.Add( CValue(0.0) ); args.Add( CValue(0.0) );
			app.ExecuteCommand( L"Translate", args, outarg );
			args.Clear();

			//SaveKey("sphere.kine.local.posx,sphere.kine.local.posy,sphere.kine.local.posz", 20, null, null);
			args.Add( posparamstr );
			args.Add( CValue(20.0) );
			app.ExecuteCommand( L"SaveKey", args, outarg );
			args.Clear();

			//SetValue("PlayControl.Key", 30, null);
			args.Add( L"PlayControl.Key" );
			args.Add( CValue(30.0) );
			app.ExecuteCommand( L"SetValue", args, outarg );
			args.Clear();

			//SetValue("PlayControl.Current", 30, null);
			args.Add( L"PlayControl.Current" );
			args.Add( CValue(30.0) );
			app.ExecuteCommand( L"SetValue", args, outarg );
			args.Clear();

			//Translate(null, 4.1350786138113, 7.57317013997673, -0.757317013997673,
			//	siRelative, siView, siObj, siXYZ, null, null, null, null, null, null, null, null, null);
			args.Add( namestr );
			args.Add( CValue(4.1350786138113) ); args.Add( CValue(7.57317013997673) ); args.Add( CValue(-0.757317013997673) );
			args.Add( CValue(1.0) ); args.Add( CValue(4.0) ); args.Add( CValue(0.0) ); args.Add( CValue(0.0) );
			app.ExecuteCommand( L"Translate", args, outarg );
			args.Clear();

			//SaveKey("sphere.kine.local.posx,sphere.kine.local.posy,sphere.kine.local.posz", 30, null, null);
			args.Add( posparamstr );
			args.Add( CValue(30.0) );
			app.ExecuteCommand( L"SaveKey", args, outarg );
			args.Clear();

			//SetValue("PlayControl.Key", 40, null);
			args.Add( L"PlayControl.Key" );
			args.Add( CValue(40.0) );
			app.ExecuteCommand( L"SetValue", args, outarg );
			args.Clear();

			//SetValue("PlayControl.Current", 40, null);
			args.Add( L"PlayControl.Current" );
			args.Add( CValue(40.0) );
			app.ExecuteCommand( L"SetValue", args, outarg );
			args.Clear();

			//Translate(null, 2.21736099581185, -7.48372324855972, 0.748372324855972,
			//	siRelative, siView, siObj, siXYZ, null, null, null, null, null, null, null, null, null);
			args.Add( namestr );
			args.Add( CValue(2.21736099581185) ); args.Add( CValue(-7.48372324855972) ); args.Add( CValue(0.748372324855972) );
			args.Add( CValue(1.0) ); args.Add( CValue(4.0) ); args.Add( CValue(0.0) ); args.Add( CValue(0.0) );
			app.ExecuteCommand( L"Translate", args, outarg );
			args.Clear();

			//SaveKey("sphere.kine.local.posx,sphere.kine.local.posy,sphere.kine.local.posz", 40, null, null);
			args.Add( posparamstr );
			args.Add( CValue(40.0) );
			app.ExecuteCommand( L"SaveKey", args, outarg );
			args.Clear();

			//StoreAction("", "Model.Sphere.kine.local.posx,Model.Sphere.kine.local.posy,Model.Sphere.kine.local.posz",
			//	2, "simplemove", true, 1, 40, false, false);
			args.Add( sph.GetModel().GetFullName() );
			args.Add( posparamstr );
			args.Add( CValue(2.0) );
			args.Add( L"simplemove" );
			args.Add( true );
			args.Add( CValue(1.0) );
			args.Add( CValue(40.0) );
			args.Add( false );
			args.Add( false );
			app.ExecuteCommand( L"StoreAction", args, outarg );

			return outarg;
		}

	\endcode
	*/
	CStatus PartialOffload
	(
		LONG in_dStartFrame,
		LONG in_dEndFrame,
		bool in_bRemember
	);

	/*! Adds a property such as a UserDataBlob or CustomProperty to the ActionSource.  This can be
	useful for storing custom user data inside the %scene.
   \param in_preset Preset name or name of a self-installed CustomProperty
   \param in_bBranch Should always be false
   \param in_name Optional name for the new property.  If no name is
		specified the name is determined based on in_preset.
   \sa Source::GetProperties, SceneItem::AddProperty
   \since 4.0
	*/
	Property 	AddProperty
	(
		const CString&	in_preset,
		bool			in_bBranch = false ,
		const CString&	in_name = CString()
	);

	/*! Returns an array of all applied Property objects on the object. An ActionSource may have nested
	CustomProperty or UserDataBlob properties.
   \sa Source::AddProperty, SceneItem::GetProperties
   \since 4.0
	*/
	CRefArray GetProperties() const ;


	/*! Adds an AnimationSourceItem to the ActionSource created with Model::AddActionSource. Use this
	function to set FCurve source items.
	\tip To change the source on an AnimationSourceItem to a static value, use AnimationSourceItem::SetAsStatic.
	\param in_target This is the relative name (RelativePath) of the parameter or cluster.
	   The relative name is the fullname minus the name of the model. See \xl RelativeName.
	\param in_source For now the only supported source is an FCurve.
	\param in_bActive True to activate the new source item

	\return AnimationSourceItem
	\sa Model::AddActionSource, AnimationSourceItem, \xl RelativeName
	\since 4.0

	\eg See the AnimationSourceItem page for an example of how to create an empty ActionSource, populate it
		and then change the source. The ActionSource::Reload page also has an example using this function.
	*/
	AnimationSourceItem  AddSourceItem
	(
		const CString&	in_target,
		const CRef&		in_source,
		bool			in_bActive = false
	);

	/*! Adds an AnimationSourceItem to the ActionSource created with Model::AddActionSource. Use this function
	to set StaticSource source items.
	\tip To change the source on an AnimationSourceItem to a different static value, use AnimationSourceItem::SetAsStatic.
	\param in_target This is the relative name (RelativePath) of the parameter or cluster, The relative name is
		the fullname minus the name of the model. See \xl RelativeName.
	\param in_staticvalue Adds a static source item to the ActionSource.
	\param in_bActive True to activate the new source item

	\return AnimationSourceItem
	\sa AnimationSourceItem::SetAsStatic, StaticSource, \xl RelativeName, Model::AddActionSource
	\since 4.0

	\eg See the AnimationSourceItem page for an example of how to create an empty ActionSource, populate it and
		then change the source.

	*/
	AnimationSourceItem  AddSourceItem
	(
		const CString&	in_target,
		double			in_staticvalue = 0.0,
		bool			in_bActive = false
	);

	private:
	ActionSource * operator&() const;
	ActionSource * operator&();
};

};

#endif // __XSIACTIONSOURCE_H__
