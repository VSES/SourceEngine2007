//*****************************************************************************
/*!
   \file xsi_scene.h
   \brief Scene class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSISCENE_H__
#define __XSISCENE_H__

#include <xsi_projectitem.h>

namespace XSI {

class Model;
class Property;
class Layer;
class Pass;
class Library;
class SimulationEnvironment;
class FileReference;

//*****************************************************************************
/*! \class Scene xsi_scene.h
	\brief The %Scene object represents an XSI scene that is loaded in memory.

	You can access most of the contents of the scene using Scene::GetRoot, which returns the root Model.

	The full path of the %Scene is stored inside the \c FileName Parameter (see the example below
	for more details).

	The scripting name of the scene is stored inside the \c Name Parameter

	\note The scripting name is a valid variable name for scripting languages. It must contain only
		alphanumerals \c A-Z, \c a-z, \c 0-9, and must not begin with a numeral. Therefore, if you
		want to save a scene with a filename that begins with a numeral, XSI prefixes an underscore
		(_) to the actual filename you used.

	\sa Project, Application, Project::GetActiveScene

	\eg Demonstrates how to get the actual filename and the real name of the active scene.

	\code
		// Shortcut to application
		Application app = Application();

		// Start with a fresh scene
		CValue outArg;
		CValueArray args; args.Add( L"" ); args.Add( false );
		app.ExecuteCommand( L"NewScene", args, outArg );

		// Get the active scene as a scene object and then get its root
		Project proj = app.GetActiveProject();
		Scene scn = proj.GetActiveScene();
		Model root = scn.GetRoot();

		// Add a null and a cube to the scene root
		Null n; root.AddNull( L"Thing1", n );
		X3DObject c; root.AddGeometry( L"Cube", L"MeshSurface", L"Thing2", c );

		// Save the scene as "2Things.scn" under the projects directory
		CString sProjectsDir = app.GetInstallationPath( siProjectPath );
		args.Clear(); args.Add( sProjectsDir + L"\\2Things.scn" );
		app.ExecuteCommand( L"SaveSceneAs", args, outArg );

		// Save the actual scene name in a variable and display it
		CParameterRefArray params = scn.GetParameters();
		Parameter p = params.GetItem( L"Filename" );
		CString sRealName = p.GetValue( double(1) );
		app.LogMessage( L"My real name is " + sRealName );

		// Save the scripting name in a variable and display it
		Parameter q = params.GetItem( L"Name" );
		CString sScriptingName = q.GetValue( double(1) );
		app.LogMessage( L"My scripting name is " + sScriptingName );

		// Expected result:
		//INFO : My real name is <project location>\2Things.scn
		//INFO : My scripting name is _2Things
	\endcode
 */
//*****************************************************************************

class SICPPSDKDECL Scene : public ProjectItem
{
public:
	/*! Default constructor. */
	Scene();

	/*! Default destructor. */
	~Scene();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	Scene(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	Scene(const Scene& in_obj);

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
	\return The new Scene object.
	*/
	Scene& operator=(const Scene& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new Scene object.
	*/
	Scene& operator=(const CRef& in_ref);

	/*! Returns the scene's root model (\c Scene_Root). The scene root is the entry point for a lot of
	functionality in the C++ API. For example, to add a 3D scene object you call X3DObject::AddGeometry
	on the scene root; to get the Mixer object you need to call Model::GetMixer on the scene root, etc.

	This member is typically called from the active (or current) scene which you can get by calling
	Project::GetActiveScene on the active project, which is available from Application::GetActiveProject.
	However, there is a more direct way to access the scene root directly from the Application class
	which is preferable: Application.GetActiveSceneRoot.

	\return The scene_root Model
	\sa Project::ActiveScene, Application::GetActiveProject, Application::GetActiveSceneRoot
	*/
	Model 		GetRoot() const;

	/*! Returns the %Scene Colors property (found under the scene's Preferences in the UI). The %Scene
	Colors property allows you to control the color that objects and components display in their selected,
	unselected, and unselectable state.

	The value of each color Parameter represents an RGB value as a LONG (0 to 1023). For more details, see
	the explanation for how Wireframe colors are represented in the CColor documentation.

	\return The scene colors Property.
	\sa Preferences, CColor, Property, Property::GetParameters, Parameter
	*/
	Property	GetColors() const;

	/*! Returns the currently active Layer object. Layers help you organize, view, and edit the contents
	in your scene, but they only affect the final render if you want them to (they are not associated
	with render passes).
	\return The current Layer.
	\sa Scene::GetLayers, Layer
	\since 4.0
	*/
	Layer 		GetActiveLayer() const;

	/*! Returns the currently active render Pass object. A render pass creates a picture
	layer of a scene that can be composited with any other pass(es) to create a complete
	image. Passes also allow you to quickly re-render a single layer without re-rendering
	the entire scene.
	\return The current Pass.
	\sa Scene::GetPasses, Pass
	\since 4.0
	*/
	Pass 		GetActivePass() const;

	/*! Returns an array of references to the Layer objects associated with the scene.
	\return Array of all Layer objects.
	\sa Scene::GetActiveLayer, Layer
	\since 4.0
	*/
	CRefArray 	GetLayers() const;

	/*! Returns an array of references to the Pass objects associated with the scene.
	\return Array of all Pass objects.
	\sa Scene::GetActivePass, Pass
	\since 4.0
	*/
	CRefArray 	GetPasses() const;

	/*! Returns the currently active MaterialLibrary. A material library is a common container for materials
	used in a scene. Every Material you create is added to a library. You can then assign it to one or more
	objects (or clusters, hierarchies, groups, partitions, and so on). This makes it easy to share a single
	material between multiple scene elements while having only the one material to manage.
	\return The current Material Library.
	\sa Scene::GetMaterialLibraries, MaterialLibrary, Material, Library
	\since 4.0
	*/
	Library		GetActiveMaterialLibrary() const;

	/*! Returns an array of references to the MaterialLibrary objects associated with the scene. This list
	contains all material libraries under the %Source container ("Scene.Sources").
	\return Array of references to all material libraries.
	\sa Scene::GetActiveMaterialLibrary, MaterialLibrary, Material, Library
	\since 4.0
	*/
	CRefArray 	GetMaterialLibraries() const;

	/*! Returns the currently active SimulationEnvironment. All elements that are part of a rigid body
	simulation are controlled within a simulation environment. A simulation environment is created
	as soon as you make an object into a rigid body. You can also create environments so that you can
	have multiple simulation environments in one scene.
	\return The current SimulationEnvironment.
	\sa Scene::GetSimulationEnvironments, SimulationEnvironment
	\since 4.2
	*/
	SimulationEnvironment	GetActiveSimulationEnvironment() const;

	/*! Returns an array of references to the SimulationEnvironment objects associated with the scene.
	This list contains all simulation environments under the Environments container ("Scene.Environments").
	\return Array of references to all simulation environments.
	\sa Scene::GetActiveSimulationEnvironment, SimulationEnvironment
	\since 4.2
	*/
	CRefArray 	GetSimulationEnvironments() const;

	/*! Returns an array of references to the properties installed on the scene.
	\return Array of references to the scene's Property objects
	\sa Property::GetParameters, Parameter
	*/
	CRefArray	GetProperties() const;

	/*! Returns a list of FileReference objects containing all external files related to this scene. Only
	input files are added to this list (for example graphics files, audio files, referenced model files, etc.).
	\return Array of references to FileReference objects.
	\sa Scene::GetExternalFileFromGUID, Model::GetExternalFiles, FileReference
	\since 5.0
	*/
	CRefArray GetExternalFiles() const;

	/*! Returns the FileReference object which corresponds to the specified unique indentifier (GUID).

	\note Only source files are in the external file list (for example, graphics files, audio files,
		referenced model files, etc.).

	\return a FileReference object
	\sa Scene::GetExternalFiles, Model::GetExternalFiles, FileReference
	\since 5.0
	*/
	FileReference GetExternalFileFromGUID(const CString &in_csGUID)const;

	/*! Returns an array of all ImageClip2 objects associated with the scene. %Image clips
	and their sources are always stored under the scene in the Clips ("Scene.Clips.Image") and Sources
	("Scene.Sources.Image") containers. <br/>

	This differs from animation clips and sources for animation which are stored under the Track, Mixer, and
	Model where they are used, so Scene::GetImageClips returns all image clips in the scene, whereas
	ClipContainer::GetClips and Track::GetClips return only clips for the specific container (mixer or track).

	\return Array of references to image clips.
	\sa ImageClip2, Image
	\since 5.0
	*/
	CRefArray GetImageClips() const;

	private:
	Scene * operator&() const;
	Scene * operator&();
};

};

#endif // __XSISCENE_H__
