//*****************************************************************************
/*!
   \file xsi_source.h
   \brief Source class declaration.

   © Copyright 1998-2003 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************
#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSISOURCE_H__
#define __XSISOURCE_H__

#include <xsi_projectitem.h>

namespace XSI {

class Property ;

//*****************************************************************************
/*! \class Source xsi_source.h
	\brief Sources are references to data which are instantiated. For example, a shape source becomes a
	ShapeClip in the animation mixer when it is instantiated there.

	There are several types of sources:

		\par Action animation:
			Stores animation data with the \xl RelativeName of a parameter. For example, an action animation
			source, when instantiated, may include an fcurve driving an object's position in space over
			time, etc. Action animation is a specialized kind of Source object called an ActionSource
			object--see the ActionSource documentation for more details.

		\par Shape animation:
			Stores shape data on point clusters relative to the reference shape in ShapeKeys. Shape
			animation is a specialized kind of Source object called an ActionSource object--see the
			ActionSource documentation for more details.

		\par Audio files:
			Audio sources add to the scene content when instantiated (rather than modifying scene data
			like action and shape animation). Audio sources contain references to audio files (supported
			formats: \c *.aiff/.aif, \c *.aifc, \c *.avi, \c *.mov, \c *.qt, and \c *.wav).

		\par Image files:
			Image sources add to the scene content when instantiated (rather than modifying scene data
			like action and shape animation). Image sources contain references to image files (see the
			Image object for more details).

		\par Model references:
			Like audio and image sources, model sources are always references to model files (\c *.emdl,
			\c *.xsi). They are accessible in the UI through a property that holds information about the
			referenced model's file and its resolution (see ModelSource for a list of supported parameters).

	\sa Clip::GetSource, ActionSource, Model::GetSources, \xl AddImageSource, \xl AddModelSource, \xl ImportAudio
	\since 4.0

	\eg Illustrates how to retrieve the image file name used for the texture of an object. It also illustrates
		how to retrieve the x and y resolution of that image.

	\code
		Application app;

		// NewScene command
		CValueArray cargs; CValue oarg;
		cargs.Add( L"" ); cargs.Add( false );
		app.ExecuteCommand( L"NewScene", cargs, oarg );
		cargs.Clear(); oarg.Clear();

		// Get the SceneRoot and create a cube in it
		Model root = app.GetActiveSceneRoot();
		X3DObject myCube; root.AddGeometry( L"Cube", L"MeshSurface", L"MyCube", myCube );

		// Create an image source and clip
		CValueArray shrArgs(5); CValue shrOut;
		shrArgs[1] = myCube.GetFullName();
		shrArgs[3] = CValue(3.0); //siUnspecified;
		shrArgs[4] = CValue(3.0); //siLetLocalMaterialsOverlap;
		app.ExecuteCommand( L"ApplyShader", shrArgs, shrOut );

		CValueArray prjArgs(8); CValue prjOut;
		prjArgs[0] = myCube.GetFullName();
		prjArgs[1] = CValue(4.0); //siTxtSpherical;
		prjArgs[2] = CValue(4.0); //siTxtDefaultSpherical;
		prjArgs[3] = L"Texture_Projection";
		prjArgs[5] = CValue(0.0); //siRelDefault;
		app.ExecuteCommand( L"CreateProjection", prjArgs, prjOut );

		CValueArray txtArgs(9); CValue txtOut;
		txtArgs[1] = myCube.GetFullName();
		txtArgs[2] = CValue(1.0);
		txtArgs[3] = true;
		txtArgs[3] = CValue(1.0); //siReplaceAndBlendInPreset;
		app.ExecuteCommand( L"BlendInTextureLayers", txtArgs, txtOut );

		// Get the source and the clip
		CRefArray mats = myCube.GetMaterials();
		Material mat( mats[0] );
		ImageClip2 imgClip = mat.GetCurrentImageClip2 ();
		Source imgsrc( imgClip.GetSource() );

		// Log some info
		app.LogMessage( L"Fullname of source: " + imgsrc.GetFullName() );
		Parameter filename = imgsrc.GetParameter( L"FileName" );
		Parameter xres = imgsrc.GetParameter( L"XRes" );
		Parameter yres = imgsrc.GetParameter( L"YRes" );
		app.LogMessage( L"The image filename is " + filename.GetValue().GetAsText() );
		app.LogMessage( L"The image x resolution is " + xres.GetValue().GetAsText() );
		app.LogMessage( L"The image y resolution is " + yres.GetValue().GetAsText() );

		// Expected result:
		//INFO : Fullname of source: Sources.noIcon_pic
		//INFO : The image filename is S:\Application\rsrc\noIcon.pic
		//INFO : The image x resolution is 256
		//INFO : The image y resolution is 256

	\endcode


	\eg Demonstrates how to find all sources in the scene when there are several levels of models all with
		their own sources. Something to note is that only ActionSources are returned from the Model.

	\code
		using namespace XSI;

		// Forward declarations
		CRefArray SearchEveryModel( Model& in_model );
		CString GetRelativeNameForTarget( Parameter& in_param );
		void CreateShapeAction( Model& in_model );
		void CreateAudioSource( Model& in_model );
		void CreateImageSource( Model& in_model );
		void CreateModelSource();


		Application app;

		// NewScene command
		CValueArray cargs; CValue oarg;
		cargs.Add( L"" ); cargs.Add( false );
		app.ExecuteCommand( L"NewScene", cargs, oarg );
		cargs.Clear(); oarg.Clear();

		// Set up the scene
		Model root = app.GetActiveSceneRoot();
		CRefArray dummy;
		Model m1; root.AddModel( dummy, L"Larry", m1 );
		Model m2; m1.AddModel( dummy, L"Curly", m2 );
		Model m3; m2.AddModel( dummy, L"Moe", m3 );
		CreateShapeAction( m2 );
		CreateImageSource( m1 );
		CreateAudioSource( m3 );
		CreateModelSource();

		// Start with the top level model (the Scene_Root) and the use the Models property
		// to search recursively. Since all action sources are stored under the model, once
		// we've visited each model, we will have found each source in the scene.
		CRefArray allsources = SearchEveryModel( root );
		if ( allsources.GetCount() > LONG(0) ) {
			app.LogMessage( L"SearchEveryModel results: " + allsources.GetAsText() );
		}


		// Expected Results:
		//INFO : MODEL: Scene_Root
		//INFO : MODEL: Larry
		//INFO : MODEL: Curly
		//INFO : MODEL: Moe
		//INFO : MODEL: Referenced_Biped_Skeleton_Generated
		//INFO : SearchEveryModel results: Curly.Mixer.ShapeKey,Curly.Mixer.Shape_ClusterClip_source,
		//Curly.Mixer.ShapeKey1,Curly.Mixer.ShapeKey2,Sources.Referenced_Biped_Skeleton_Generated.DefaultPose


		CRefArray SearchEveryModel( Model& in_model )
		{
			Application app;
			app.LogMessage( L"MODEL: " + in_model.GetFullName() );

			// Start with the top-level model
			// Get a CRefArray of all sources found under this model
			CRefArray results;
			CRefArray foundsrcs = in_model.GetSources();
			if ( foundsrcs.GetCount() > LONG(0) ) {
				// We can just copy the whole array over
				results = foundsrcs;
			}

			// For every model nested within this scene, check it for sources
			CRefArray modellist = in_model.GetModels( true );
			for ( LONG i=0; i<modellist.GetCount(); ++i ) {
				Model mdl( modellist[i] );
				app.LogMessage( L"MODEL: " + mdl.GetFullName() );

				CRefArray morefound = mdl.GetSources();
				if ( morefound.GetCount() > LONG(0) ) {
					// We can append the whole array to the one we already have
					results += morefound;
				}
			}

			// Return the CRefArray
			return results;
		}



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

		// Helper functions to create & instantiate sources
		void CreateShapeAction( Model& in_model )
		{
			Application app;
			X3DObject obj; in_model.AddGeometry( L"Sphere", L"NurbsSurface", L"MySphere", obj );

			// SetSelFilter command
			CValueArray args; CValue out;
			args.Add( L"Point" );
			app.ExecuteCommand( L"SetSelFilter", args, out );

			// CreateCluster command
			CValueArray args1; CValue out1;
			args1.Add( obj.GetFullName() + L".pnt[(5,4),(6,4),(7,4),(0,5),(1,5),(2,5),(3,5),(4,5),(5,5),(6,5),(7,5),(0,6),(1,6),(2,6),(3,6)]" );
			app.ExecuteCommand( L"CreateCluster", args1, out1 );

			// StoreShapeKey
			CValueArray args2(8); CValue out2;
			args2[0] = out1;
			args2[1] = L"MyShape";
			args2[2] = 1.0;			//siShapeObjectReferenceMode
			app.ExecuteCommand( L"StoreShapeKey", args2, out2 );
		}


		void CreateAudioSource( Model& in_model )
		{
			Application app;
			CString mdlname = in_model.GetFullName();

			// Get the mixer
			Mixer mixmaster;
			if ( in_model.HasMixer() ) {
				mixmaster = in_model.GetMixer();
			} else {
				mixmaster = in_model.AddMixer();
			}

			// AddTrack command
			CValueArray trkArgs; CValue trkOut;
			trkArgs.Add( mdlname );
			trkArgs.Add( mixmaster.GetFullName() );
			trkArgs.Add( CValue(2.0) );
			app.ExecuteCommand( L"AddTrack", trkArgs, trkOut );
			Track aud_track( trkOut );

			// ImportAudio command
			CValueArray impArgs; CValue impOut;
			impArgs.Add( mdlname );
			impArgs.Add( L"C:\\Program Files\\Messenger\\NEWALERT.WAV" );
			app.ExecuteCommand( L"ImportAudio", impArgs, impOut );
			Source aud_src( impOut );

			// AddAudioClip command
			CValueArray clpArgs; CValue clpOut;
			clpArgs.Add( mdlname );
			clpArgs.Add( aud_src.GetFullName() );
			clpArgs.Add( mixmaster.GetFullName() );
			clpArgs.Add( aud_track.GetFullName() );
			clpArgs.Add( CValue(22.0) );
			app.ExecuteCommand( L"AddAudioClip", clpArgs, clpOut );
		}


		void CreateImageSource( Model& in_model )
		{
			Application app;
			X3DObject cube; in_model.AddGeometry( L"Cube", L"MeshSurface", L"MyCube", cube );

			// ApplyShader command
			CValueArray shdrArgs(5); CValue shdrOut;
			shdrArgs[3] = 3.0;				//siUnspecified
			shdrArgs[4] = 3.0;				//siLetLocalMaterialsOverlap
			app.ExecuteCommand( L"ApplyShader", shdrArgs, shdrOut );

			// CreateProjection command
			CValueArray txtArgs(8); CValue txtOut;
			txtArgs[0] = cube.GetFullName();
			txtArgs[1] = 4.0;			//siTxtSpherical
			txtArgs[2] = 4.0;			//siTxtDefaultSpherical
			txtArgs[3] = L"";
			txtArgs[4] = L"Texture_Projection";
			txtArgs[6] = 0.0;			//siRelDefault
			app.ExecuteCommand( L"CreateProjection", txtArgs, txtOut );

			// BlendInTextureLayers command
			CValueArray blndArgs(9); CValue blndOut;
			blndArgs[0] = L"Image";
			blndArgs[1] = cube.GetFullName();
			blndArgs[2] = CValue(1.0);
			blndArgs[3] = false;
			blndArgs[4] = 1.0;			//siReplaceAndBlendInPreset
			blndArgs[5] = true;
			blndArgs[6] = true;
			blndArgs[7] = false;
			blndArgs[8] = false;
			app.ExecuteCommand( L"BlendInTextureLayers", blndArgs, blndOut );
		}


		void CreateModelSource()
		{
			Application app;
			CString path = app.GetInstallationPath( siFactoryPath );

			// Importing a model creates a model source
			CValueArray args(7); CValue out;
			args[0] = path + L"\\Data\\XSI_SAMPLES\\Models\\Biped_Skeleton_Generated.emdl";
			args[2] = true;
			app.ExecuteCommand( L"ImportModel", args, out );
		}

	\endcode
 */
//*****************************************************************************
class SICPPSDKDECL Source : public ProjectItem
{
public:
	/*! Default constructor. */
	Source();

	/*! Default destructor. */
	~Source();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	Source(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	Source(const Source& in_obj);

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
	\return The new Source object.
	*/
	Source& operator=(const Source& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new Source object.
	*/
	Source& operator=(const CRef& in_ref);

	/*! Adds a property such as a UserDataBlob or CustomProperty to the Source.  This
	can be useful for storing custom user data inside the %scene.
	\sa Source::GetProperties, SceneItem::AddProperty
	*/
	Property 	AddProperty
	(
		const CString&	in_preset,
		bool			in_bBranch = false ,
		const CString&	in_name = CString()
	);

	/*! Returns an array of all applied Property objects on the object. A Source may have
	nested CustomProperty or UserDataBlob properties.
	\sa Source::AddProperty, SceneItem::GetProperties
	*/
	CRefArray GetProperties() const ;

	private:
	Source * operator&() const;
	Source * operator&();
};

}

#endif // __XSISOURCE_H__
