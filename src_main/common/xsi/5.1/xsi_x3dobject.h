//*****************************************************************************
/*!
   \file xsi_x3dobject.h
   \brief X3DObject class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIX3DOBJECT_H__
#define __XSIX3DOBJECT_H__

#include <xsi_sceneitem.h>
#include <xsi_status.h>
#include <xsi_math.h>
#include <xsi_nurbsdata.h>

namespace XSI {

class Primitive;
class Kinematics;
class CameraRig;
class LightRig;
class Camera;
class Light;
class Null;
class ChainRoot;
class StaticKinematicState;
class ParticleCloud;
class CMeshBuilder;

//*****************************************************************************
/*! \class X3DObject xsi_x3dobject.h
	\brief The X3DObject object represents a basic 3D object of a scene in XSI.

	The following X3DObject methods can be used for creating other X3DObject objects:

		\li X3DObject::Add2DChain
		\li X3DObject::Add3DChain
		\li X3DObject::AddCamera
		\li X3DObject::AddCameraRig
		\li X3DObject::AddGeometry
		\li X3DObject::AddLight
		\li X3DObject::AddLightRig
		\li X3DObject::AddModel
		\li X3DObject::AddNull
		\li X3DObject::AddNurbsCurve
		\li X3DObject::AddNurbsCurveList
		\li X3DObject::AddNurbsSurface
		\li X3DObject::AddNurbsSurfaceMesh
		\li X3DObject::AddPolygonMesh
		\li X3DObject::AddPrimitive

	You access the current local and global transformation of a 3D object via X3DObject::GetKinematics.

	X3DObject's geometry can be accessed from the active primitive's Primitive::GetGeometry function,
	which you can access from X3DObject::GetActivePrimitive.

	Vertex color and texture coordinates are stored in ClusterProperty objects nested under Cluster
	objects of type siSampledPointCluster (ClusterTypes). Clusters are stored under the Geometry object
	and accessed via Geometry::GetClusters.

	The material assigned to an object is accessible from X3DObject::GetMaterial.

	\sa Model, Primitive, Geometry, Cluster, PolygonMesh, ChainRoot, ChainBone, Null

	\eg
	\code
		using namespace XSI;
		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject myNewGeometry;
		root.AddGeometry( L"Cube", L"MeshSurface",L"myMesh",myNewGeometry);

		app.LogMessage( L"The object name: " + myNewGeometry.GetFullName() );
	\endcode

 */
//*****************************************************************************
class SICPPSDKDECL X3DObject : public SceneItem
{
public:
	/*! Default constructor. */
	X3DObject();

	/*! Default destructor. */
	~X3DObject();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	X3DObject(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	X3DObject(const X3DObject& in_obj);

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
	\return The new X3DObject object.
	*/
	X3DObject& operator=(const X3DObject& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new X3DObject object.
	*/
	X3DObject& operator=(const CRef& in_ref);

	/*! Returns an array containing references to all Primitive objects for this object.
	\return Array of Primitive references.
	*/
	CRefArray GetPrimitives() const;

	/*! Returns the 3D object's active primitive. There is an exception: although the scene root is a
	kind of Model it does not support primitives.
	\return The active Primitive.
	*/
	Primitive GetActivePrimitive() const;

	/*! Returns an array containing references to all X3DObject objects which are parented under this object.
	\return Array of X3DObject references.
	*/
	CRefArray GetChildren() const;

	/*! Returns an array containing references to all Model objects which are parented under this object.
	\param in_recursive Recurse if true, otherwise the search is done on the immediate children.
		The default value is true.
	\return Array of Model references.
	*/
	CRefArray GetModels( bool in_recursive = true) const;

	/*! Returns an array containing references to all materials used by the object, including materials on clusters.
	\return Array of Material references.
	\since 4.0
	*/
	CRefArray GetMaterials() const;

	/*! Returns the object's Kinematics object.
	\return The Kinematics object.
	*/
	Kinematics GetKinematics() const;

	/*! Creates and adds a new primitive as child of this object.
	\param in_preset Preset for type of primitive (e.g. L"Camera")
	\param in_name Name of child.
	\param io_3dobj	New child object.
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus AddPrimitive
	(
		const CString& in_preset,
		const CString& in_name ,
		X3DObject& io_3dobj
	);

	/*! Creates and adds a new geometry as child of this object.
	\param in_geompreset	Preset for type of primitive (e.g. "Sphere")
	\param in_convpreset	Type of geometry (e.g. "MeshSurface", "NurbsCurve", or "NurbsSurface").  An implicit
		object can be created by specifying an empty string.
	\param in_name			Name for new geometry
	\param io_3dobj			New child object
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus AddGeometry
	(
		const CString&	in_geompreset,
		const CString&	in_convpreset,
		const CString&	in_name,
		X3DObject&		io_3dobj
	);

	/*! Creates an empty PolygonMesh object and returns a CMeshBuilder object for defining a new polygon mesh geometry
	on the active primitive. The mesh builder object provides a rich and versatile function set for building a polygon
	mesh, therefore the use of this method is recommended over the older version one.

	\param in_name Name of new child object.
	\param out_3dobj New 3D Object.
	\param out_meshBuilder Mesh builder object.

	\return CStatus::OK success
	\sa PolygonMesh::GetMeshBuilder
	\since 5.0

	\eg
	\code
		using namespace XSI;

		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject my3DObj;
		CMeshBuilder msBuilder;
		root.AddPolygonMesh( L"TriangleMesh", my3DObj, msBuilder );

		// Add vertices to mesh
		double myVertices[54] =
		{
			-0.707107, -1.33333, -0.707107,
			-0.353553, 0.666667, -0.353553,
			-0.353553, -1.33333, -0.353553,
			3.33067e-016, 0.666667, -0.5,
			6.66134e-016, -1.33333, -1,
			0.707107, -1.33333, -0.707107,
			3.33067e-016, -1.33333, -0.5,
			0.353553, 0.666667, -0.353553,
			-1.38778e-016, -1.33333, 0.5,
			-2.77556e-016, -1.33333, 1,
			-1.38778e-016, 0.666667, 0.5,
			0.353553, -1.33333, 0.353553,
			0.707107, -1.33333, 0.707107,
			0.353553, 0.666667, 0.353553,
			0.5, -1.33333, 2.22045e-016,
			1, -1.33333, 4.44089e-016,
			0.5, 0.666667, 2.22045e-016,
			0.353553, -1.33333, -0.353553
		};

		msBuilder.AddVertices( 18, myVertices );

		// Adds triangles
		LONG myTriangles[60] =
		{
			4, 0, 1,
			17, 6, 4,
			17, 4, 5,
			5, 4, 3,
			15, 7, 16,
			15, 5, 7,
			6, 0, 4,
			11, 15, 12,
			14, 5, 15,
			14, 17, 5,
			5, 3, 7,
			12, 16, 13,
			6, 2, 0,
			8, 11, 12,
			8, 12, 9,
			9, 12, 13,
			9, 13, 10,
			12, 15, 16,
			4, 1, 3,
			11, 14, 15
		};

		msBuilder.AddTriangles( 20, myTriangles );

		// Generate the new mesh
		msBuilder.Build(false);

	\endcode
	*/
	CStatus AddPolygonMesh(	const CString&	in_name,
							X3DObject&		out_3dobj,
							CMeshBuilder&	out_meshBuilder );

	/*! Creates a PolygonMesh object. This is typically used for creating a polygon mesh from a set of geometry data.
	\param in_vertices		Array representing the polygon vertices.
	\param in_polygonData	An ordered array of polygon definitions, each polygon is defined by a list of
		elements, the first element of a polygon definition must be set with the number of indices for that
		polygon. The ordering of vertices must respect a ccw ordering to get out going normals (right-hand
		rule). For example, array of polygons with 4 indices each: \c {4,0,1,4,3,4,1,2,5,4... }
	\param in_name			Name of child.
	\param io_3dobj			New child object.
	\return CStatus::OK success
	\return CStatus::Fail failure

	\eg
	\code
		using namespace XSI;
		using namespace XSI::MATH;

		Application app;
		Model root = app.GetActiveSceneRoot();

		CVector3 v1(-1.0,1.0, 0.0);
		CVector3 v2(1.0,1.0,0.0);
		CVector3 v3(1.0,-1.0,0.0);
		CVector3 v4(-1.0,-1.0,0.0);

		CVector3Array verts(4);
		verts[0] = v1;
		verts[1] = v2;
		verts[2] = v3;
		verts[3] = v4;

		CLongArray indices(5);
		indices[0] = 4;
		indices[1] = 3;
		indices[2] = 2;
		indices[3] = 1;
		indices[4] = 0;

		X3DObject mySquare;
		CStatus check = root.AddPolygonMesh(verts,indices,L"mySquare",mySquare);
		assert( check == CStatus::OK );
	\endcode
	*/
	CStatus AddPolygonMesh
	(
		const MATH::CVector3Array&	in_vertices,
		const CLongArray&			in_polygonData,
		const CString&				in_name,
		X3DObject&					io_3dobj
	);

	/*! Creates a new model given a list of children objects and a name, the new model is parented under
	this X3DObject.
	\param in_objects		Array of references to children objects.
	\param in_name			Name of child.
	\param io_3dobj			New child object.
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus AddModel
	(
		const CRefArray&	in_objects,
		const CString&		in_name,
		Model&				io_3dobj
	);

	/*! Creates a new lattice given a preset name/object and a name, the input object(s) are connected to
	the new lattice object. A lattice object is used to control lattice deformations.
	\param in_preset		Lattice preset.
	\param in_objects		Array of references to objects which are going to be connected to the lattice.
	\param in_name			Name of new lattice.
	\param io_3dobj			New child object.
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus AddLattice
	(
		const CString&		in_preset,
		const CRefArray&	in_objects,
		const CString&		in_name,
		X3DObject&			io_3dobj
	);

	/*! Creates a new control primitive given a preset name/object and a name, the input object(s) are
	connected to the new control object. A Control object is a non-renderable object that can affect other
	objects, for example by creating a wave deformation or defining a physical force.
	\param in_preset		Lattice preset.
	\param in_objects		Array of references to objects which are going to be connected to the lattice.
	\param in_name			Name of new lattice.
	\param io_3dobj			New child object.
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus AddControl
	(
		const CString&		in_preset,
		const CRefArray&	in_objects,
		const CString&		in_name,
		X3DObject&			io_3dobj
	);

	/*! Creates a new CameraRig object given a camera preset name/object and a rig name. The CameraRig is
	parented under this X3DObject.
	\param in_preset	Preset for type of camera.
	\param in_name		Name of new CameraRig.
	\param io_rig		New CameraRig.
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus AddCameraRig
	(
		const CString&	in_preset,
		const CString&	in_name,
		CameraRig&		io_rig
	);

	/*! Creates a new LightRig object given a light preset name/object and a rig name. The LightRig is
	parented under this X3DObject.
	\param in_preset	Preset for type of light.
	\param in_name		Name of new LightRig.
	\param io_rig		New LightRig.
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus AddLightRig
	(
		const CString&	in_preset,
		const CString&	in_name,
		LightRig&		io_rig
	);

	/*! Creates a new Camera object given a camera preset name/object; an interest is also created as a
	sibling of the new camera. The new camera object is parented under this X3DObject. This function only
	creates a Camera object and its interest, whereas X3DObject::AddCameraRig creates a camera root object
	with the camera and its interest as children
	\param in_preset Preset name member of the Camera family (%Camera, Default_Camera, Telephoto, Wide_Angle, etc.)
	\param in_name		Name of new Camera.
	\param io_camera	New Camera.
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus AddCamera
	(
		const CString&	in_preset,
		const CString&	in_name,
		Camera&			io_camera
	);

	/*! Creates a new Light object given a light preset name/object, an interest is also created by default
	unless its specified otherwise with the \c AddInterest argument. The new light created is parented under
	this X3DObject. This function only creates a Light object with possibly an interest, whereas
	X3DObject::AddLighRig creates a Light root object with the Light and its interest as children.
	\param in_preset	Preset name member of the Light family (Light_Box, LightInfinite, LightPoint, LightSpot, LightSun, etc.)
	\param in_bInterest	Add an interest to the created Light. This argument can be set to true for creating a spot light object.
	\param in_name		Name of new Light.
	\param io_light	New Light.
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus AddLight
	(
		const CString& in_preset,
		bool		   in_bInterest,
		const CString& in_name,
		Light&		   io_light
	);

	/*! Creates and adds a Null as child of object.
	\param in_name		Name of new Null.
	\param io_null		New Null.
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus AddNull
	(
		const CString&	in_name,
		Null&			io_null
	);

	/*! Parents an array of X3DObject child objects under this 3D object. This function is typically used for moving
	child objects from other X3DObject objects.
	\param in_objects Array of X3DObject references to parent.
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus AddChild
	(
		const CRefArray& in_objects
	);

	/*! Cuts the hierarchy link of X3DObject child objects. Only the child objects that are currently parented to
	this object are cut, others are ignored. All cut objects are automatically moved under the X3DObject scene root.
	\param in_objects Array of X3DObject references to remove.
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus RemoveChild
	(
		const CRefArray& in_objects
	);

	/*! Cuts the hierarchy link of this child object with its parent. The object is automatically moved under the
	X3DObject scene root. This operation has no effect on child objects of the scene root.
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus RemoveFromParent();

	/*! Adds a 2D chain and parents the new chain under this object. A 2D chain has joint properties that can only
	rotate on their local Z axes
	\param in_rootPos	The root position of the new chain, in global coordinates.
	\param in_effPos	The effector position of the new chain, in global coordinates.
	\param in_normPlane	The chain's normal to the plane, in global coordinates.
	\param in_alignType The view reference used for computing the root orientation.
	\param in_name		Name of new chain.
	\param io_chainroot New chain root.
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus Add2DChain
	(
		const MATH::CVector3&	in_rootPos,
		const MATH::CVector3&	in_effPos,
		const MATH::CVector3&	in_normPlane,
		si2DChainAlignType		in_alignType,
		const CString&			in_name,
		ChainRoot&				io_chainroot
	);

	/*! Adds a 3D chain and parents the new chain under this object. A 3D chain has joint properties that can rotate
	on any axis.
	\param in_rootPos	The root position of the new chain, in global coordinates.
	\param in_effPos	The effector position of the new chain, in global coordinates.
	\param in_normPlane	The chain's normal to the plane, in global coordinates.
	\param in_name		Name of new chain.
	\param io_chainroot New chain root.
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus Add3DChain
	(
		const MATH::CVector3&	in_rootPos,
		const MATH::CVector3&	in_effPos,
		const MATH::CVector3&	in_normPlane,
		const CString&			in_name,
		ChainRoot&				io_chainroot
	);

	/*! Returns an array of reference to Cluster objects which are containing shape animation.
	\return Array of Cluster references.
	*/
	CRefArray GetShapeAnimatedClusters() const;

	/*! Finds the first X3DObject child object of an X3DObject object that matches a set of search criterias. The search
	is done recursively by default, the function returns the object that meet all supplied criterias such as name, type
	and family(ies). This function is typically used for retrieving an object if you know its type or name for instance.

	\param in_name A name expression with or without wildcard characters. The name can be empty if the name is not required.

	\param in_type The type of object defined by ::siType or an empty string if no type is used. The type can be a
		specific X3DObject type such as siModelType and si3DObjectType or a Primitive type such as siPolyMeshType
		or siSrfMeshPrimType. If a primitive type is supplied, the function consided the children objects defined
		with a primitive of this type. The types must be one of the following:

		\code
						siPolyMeshType							siCylinderPrimType						siNullPrimType
						si3DObjectType							siDiscPrimType							siOctahedronPrimTypee
						siArcPrimType							siDodecahedronPrimType					siSpherePrimType
						siAttractorCtrlType						siDragCtrlPrimType						siSpiralPrimType
						siCameraPrimType						siEddyCtrlPrimType						siSpotInterestPrimType
						siCameraRootPrimType					siFanType								siSpotRootPrimType
						siChainBonePrimType						siFurPrimType							siSquarePrimType
						siChainEffPrimType						siGeoShaderPrimType						siSrfMeshPrimType
						siChainRootPrimType						siGravityCtrlType						siTetrahedronPrimType
						siCirclePrimType						siGridPrimType							siTorusPrimType
						siCloudPrimType							siIcosahedronPrimType					siTurbulenceCtrlPrimType
						siConePrimType							siLatticePrimType						siVolumeDeformType
						siCrvListAggregatePrimType				siLightPrimType							siVortexCtrlType
						siCrvListPrimType						siModelNullPrimType						siWaveCtrlType
						siCubePrimType							siModelType								siWindType
		\endcode

	\param in_famArray An array of families defined by ::siFamily or an empty array if none are required. The families
		are used for narrowing down the search, the array can contain X3DObject families like si3DObjectFamily or primitive
		families such as siNurbsSurfaceMeshFamily and siNullPrimitiveFamily. Only the children objects that match one of the
		supplied families are considered. If primitive families are supplied then only the objects defined with a primitive
		that belongs to one of them are considered. The list of valid families are the following:

		\code
						si3DObjectFamily						siLatticeFamily
						siCameraFamily							siLightPrimitiveFamily
						siChainElementFamily					siMeshFamily
						siControlObjectFamily					siNullPrimitiveFamily
						siCurveFamily							siNurbsCurveListFamily
						siGeometryFamily						siNurbsSurfaceMeshFamily
						siGeometryShaderFamily					siSurfaceCurveFamily
						siImplicitGeometryFamily				siSurfaceFamily
		\endcode

	\param in_bRecursive Recurse if true, otherwise the search is done on the immediate children.

	\return X3DObject The object that matches the search criterias or empty object otherwise.

	\eg
	\code
		#include <xsi_application.h>
		#include <xsi_model.h>
		#include <xsi_x3dobject.h>

		using namespace XSI;

		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject mySphere;
		root.AddGeometry( L"Sphere", L"MeshSurface", L"mySphere", mySphere );

		X3DObject myCone;
		root.AddGeometry( L"Cone", L"MeshSurface", L"myCone", myCone );

		X3DObject anotherCone;
		root.AddGeometry( L"Cone", L"MeshSurface", L"anotherCone", anotherCone );

		X3DObject myCube;
		root.AddGeometry( L"Cube", L"NurbsSurface", L"myCube", myCube );

		app.LogMessage( L"** 1: Find the object whose name starts with 'Camera' **");
		X3DObject obj = root.FindChild(L"Camera*",L"",CStringArray());
		app.LogMessage( L"Found child: " + obj.GetName() );

		app.LogMessage( L"** 2: Find an object of type siLightPrimType **");
		obj = root.FindChild(L"",siLightPrimType,CStringArray());
		app.LogMessage( L"Found child: " + obj.GetName() );

		app.LogMessage( L"** 3: Find an object that belongs to siMeshFamily family **");
		CStringArray strFamilies;
		strFamilies.Add( siMeshFamily );
		obj = root.FindChild(L"",L"",strFamilies);
		app.LogMessage( L"Found child: " + obj.GetName() );

		app.LogMessage( L"** 4: Find the object whose name start with 'an' and belongs to siMeshFamily family **");
		obj = root.FindChild(L"an*",L"",strFamilies);
		app.LogMessage( L"Found child: " + obj.GetName() );
	\endcode

	*/
	X3DObject FindChild
	(
		const CString&		in_name,
		const CString&		in_type,
		const CStringArray& in_famArray,
		bool				in_bRecursive = true
	) const;

	/*! Finds all X3DObject children of an X3DObject object that match a set of search criteria. By default the search
	is done recursively, the function returns all objects satisfying the following criterias: name, type and family(ies).

	\param in_name A name expression with or without wildcard characters. The name can be empty to ignore the object
		name during the search.

	\param in_type The type of object defined by ::siType or an empty string if no type is used. The type can be a
		specific X3DObject type such as siModelType and si3DObjectType or a Primitive type such as siPolyMeshType
		or siSrfMeshPrimType. If a primitive type is supplied, the function consided the children objects defined
		with a primitive of this type. The types must be one of the following:

		\code
						siPolyMeshType							siCylinderPrimType						siNullPrimType
						si3DObjectType							siDiscPrimType							siOctahedronPrimTypee
						siArcPrimType							siDodecahedronPrimType					siSpherePrimType
						siAttractorCtrlType						siDragCtrlPrimType						siSpiralPrimType
						siCameraPrimType						siEddyCtrlPrimType						siSpotInterestPrimType
						siCameraRootPrimType					siFanType								siSpotRootPrimType
						siChainBonePrimType						siFurPrimType							siSquarePrimType
						siChainEffPrimType						siGeoShaderPrimType						siSrfMeshPrimType
						siChainRootPrimType						siGravityCtrlType						siTetrahedronPrimType
						siCirclePrimType						siGridPrimType							siTorusPrimType
						siCloudPrimType							siIcosahedronPrimType					siTurbulenceCtrlPrimType
						siConePrimType							siLatticePrimType						siVolumeDeformType
						siCrvListAggregatePrimType				siLightPrimType							siVortexCtrlType
						siCrvListPrimType						siModelNullPrimType						siWaveCtrlType
						siCubePrimType							siModelType								siWindType
		\endcode

	\param in_famArray An array of families defined by ::siFamily or an empty array if none are required. The families
		are used for narrowing down the search, the array can contain X3DObject families like si3DObjectFamily or primitive
		families such as siNurbsSurfaceMeshFamily and siNullPrimitiveFamily. Only the children objects that match one of the
		supplied families are considered. If primitive families are supplied then only the objects defined with a primitive
		that belongs to one of them are considered. The list of valid families are the following:

		\code
						si3DObjectFamily						siLatticeFamily
						siCameraFamily							siLightPrimitiveFamily
						siChainElementFamily					siMeshFamily
						siControlObjectFamily					siNullPrimitiveFamily
						siCurveFamily							siNurbsCurveListFamily
						siGeometryFamily						siNurbsSurfaceMeshFamily
						siGeometryShaderFamily					siSurfaceCurveFamily
						siImplicitGeometryFamily				siSurfaceFamily
		\endcode

	\param in_bRecursive Recurse if true, otherwise the search is done on the immediate children.
	\return CRefArray An array of X3DObject children that match the search criteria or an empty
		array if no objects are found.

	\eg
	\code
		#include <xsi_application.h>
		#include <xsi_model.h>
		#include <xsi_x3dobject.h>

		using namespace XSI;

		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject mySphere;
		root.AddGeometry( L"Sphere", L"MeshSurface", L"mySphere", mySphere );

		X3DObject myCone;
		root.AddGeometry( L"Cone", L"MeshSurface", L"myCone", myCone );

		X3DObject anotherCone;
		root.AddGeometry( L"Cone", L"MeshSurface", L"anotherCone", anotherCone );

		X3DObject myCube;
		root.AddGeometry( L"Cube", L"NurbsSurface", L"myCube", myCube );

		app.LogMessage( L"** 1: Find all objects whose names start with 'Camera' **");
		CRefArray childArray = root.FindChildren(L"Camera*",L"",CStringArray());
		for (LONG i =0; i<childArray.GetCount(); i++)
		{
			X3DObject obj = childArray[i];
			app.LogMessage( L"Found child: " + obj.GetName() );
		}

		app.LogMessage( L"** 2: Find all objects of type siLightPrimType **");
		childArray = root.FindChildren(L"",siLightPrimType,CStringArray());
		for (i =0; i<childArray.GetCount(); i++)
		{
			X3DObject obj = childArray[i];
			app.LogMessage( L"Found child: " + obj.GetName() );
		}

		app.LogMessage( L"** 3: Find all objects that belong to siNullPrimitiveFamily and siMeshFamily families **");
		CStringArray strFamilies;
		strFamilies.Add( siNullPrimitiveFamily );
		strFamilies.Add( siMeshFamily );
		childArray = root.FindChildren(L"",L"",strFamilies);
		for (i =0; i<childArray.GetCount(); i++)
		{
			X3DObject obj = childArray[i];
			app.LogMessage( L"Found child: " + obj.GetName() );
		}

		app.LogMessage( L"** 4: Find all objects whose names start with 'my' and belong to siMeshFamily family **");
		strFamilies.Clear();
		strFamilies.Add( siMeshFamily );
		childArray = root.FindChildren(L"my*",L"",strFamilies);
		for (i =0; i<childArray.GetCount(); i++)
		{
			X3DObject obj = childArray[i];
			app.LogMessage( L"Found child: " + obj.GetName() );
		}
	\endcode

	*/
	CRefArray FindChildren
	(
		const CString&		in_name,
		const CString&		in_type,
		const CStringArray& in_famArray,
		bool				in_bRecursive = true
	) const;

	/*! Returns true if the object has a base pose. The base pose is represented by a StaticKinematicState property.
	\return true if the object has a base pose.
	*/
	bool HasStaticKinematicState() const;

	/*! Returns the StaticKinematicState property for the object. properties on this 3D object.
	\return The StaticKinematicState base pose.
	*/
	StaticKinematicState GetStaticKinematicState() const;

	/*! Returns true if this object is animated by a specific animation source type or false otherwise. This function
	looks for animated parameters on the target object only and doesn't recursively visit the children objects.

	\param in_sourceType Animation source type. The default value is any source.
	\param in_bLocal Specifies whether the animation to check is local to the object or not. If true then all
		animated parameters that come from propagated properties or from intermediate nodes are ignored.
		Intermediate nodes are nested under the object but are not necessary owned by the object itself.
		For instance, the camera object nested under a \c Texture_Projection_Def property is not considered
		unless the target object is the projection property itself. When the flag is set to false (default),
		all parameters are considered.

	\return true if animated; false otherwise.

	\eg
	\code
		using namespace XSI;

		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject mySquare;
		root.AddPrimitive( L"Square", L"mySquare", mySquare );

		Parameter posxParam = mySquare.GetParameters().GetItem( L"posx" );

		CDoubleArray keys(4);
		keys[0] = 0.0; keys[1] = 0.0; keys[2] = 1.0; keys[3] = 1.0;

		FCurve fcrv;
		posxParam.AddFCurve( siStandardFCurve, keys, fcrv	);

		app.LogMessage( mySquare.GetName() + L" is animated: " +
			CValue(mySquare.IsAnimated( siAnySource )).GetAsText() );

		app.LogMessage( mySquare.GetName() + L" is node animated: " +
			CValue(mySquare.IsNodeAnimated( siAnySource )).GetAsText() );

		Null myNull;
		root.AddNull(L"MyNull", myNull);

		CRefArray children(1);
		children[0] = mySquare;

		myNull.AddChild( children );
		app.LogMessage( myNull.GetName() + L" is animated: " +
			CValue(myNull.IsAnimated( siAnySource )).GetAsText() );

		app.LogMessage( myNull.GetName() + L" is node animated: " +
			CValue(myNull.IsNodeAnimated( siAnySource )).GetAsText() );
	\endcode
	*/
	bool IsNodeAnimated
	(
		siSourceType	in_sourceType = siAnySource,
		bool			in_bLocal = false
	) const;

	/*! Returns an array of references to parameters animated by a specific animation source type. This function returns
	the parameters, if any, on this object only and does not visit the children.

	\param in_sourceType an object type.
	\param in_bLocal Specifies whether the animation to gather is local to the object or not. If the flag is true
		then all animated parameters that come from propagated properties or from intermediate nodes are ignored.
		Intermediate nodes are nested under the object but are not necessary owned by the object itself. For instance,
		the camera object nested under a \c Texture_Projection_Def property is not considered unless the target object
		is the projection property itself. When the flag is set to false (default), all parameters are considered.

	\return Array of Parameter references.

	\eg
	\code
		using namespace XSI;

		Application app;
		Model root = app.GetActiveSceneRoot();

		Null myNullParent;
		root.AddNull(L"MyNull", myNullParent);

		X3DObject mySquare;
		myNullParent.AddPrimitive( L"Square", L"mySquare", mySquare );

		X3DObject myCube;
		mySquare.AddPrimitive( L"Cube", L"myCube", myCube );

		// animate the null posx
		Parameter posxParam = myNullParent.GetParameters().GetItem( L"posx" );

		CDoubleArray keys(4);
		keys[0] = 0.0; keys[1] = 0.0; keys[2] = 1.0; keys[3] = 1.0;

		FCurve fcrv;
		posxParam.AddFCurve( siStandardFCurve, keys, fcrv );

		// animate the square posx
		posxParam = mySquare.GetParameters().GetItem( L"posx" );
		posxParam.AddFCurve( siStandardFCurve, keys, fcrv	);

		// animate the cube posx
		posxParam = myCube.GetParameters().GetItem( L"posx" );
		posxParam.AddFCurve( siStandardFCurve, keys, fcrv );

		// log all animated parameters
		CRefArray paramArray = myNullParent.GetAnimatedParameters();

		for (LONG i=0; i<paramArray.GetCount(); i++ )
		{
			Parameter param = paramArray[i];
			app.LogMessage( param.GetFullName() + L" is animated" );
		}

		// log all node animated parameters
		paramArray = myNullParent.GetNodeAnimatedParameters();

		for (i=0; i<paramArray.GetCount(); i++ )
		{
			Parameter param = paramArray[i];
			app.LogMessage( param.GetFullName() + L" is node animated" );
		}
	\endcode

	*/
	CRefArray GetNodeAnimatedParameters
	(
		siSourceType	in_sourceType = siAnySource,
		bool			in_bLocal = false
	) const;

	/*! Creates a NurbsCurveList object composed of one curve.
	\param in_curveData		A NurbsCurve data description.
	\param in_nurbsFormat   Specifies how the data is formatted.
	\param in_name			Name of child.
	\param io_3dobj			New child object.
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus AddNurbsCurve
	(
		const CNurbsCurveData&		in_curveData,
		siNurbsFormat				in_nurbsFormat,
		const CString&				in_name,
		X3DObject&					io_3dobj
	);

	/*! Creates a NurbsCurveList object. This is typically used for creating a nurbs surface from a set of geometry data.
	\param in_curveDataArray		An array of NurbsCurve data description.
	\param in_nurbsFormat   Specifies how the data is formatted.
	\param in_name			Name of child.
	\param io_3dobj			New child object.
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus AddNurbsCurveList
	(
		const CNurbsCurveDataArray&	in_curveDataArray,
		siNurbsFormat				in_nurbsFormat,
		const CString&				in_name,
		X3DObject&					io_3dobj
	);

	/*! Creates a NurbsSurfaceMesh object composed of one surface.
	\param in_surfaceData	An array of NurbsCurve data description.
	\param in_nurbsFormat   Specifies how the data is formatted.
	\param in_name			Name of child.
	\param io_3dobj			New child object.

	\warning UV ordering is different for the C++ API than it is for scripting. In scripting, a
		control point array is ordered U-V like so:
		<br><br>
		<tt> (0,0)...(0,cntV-1), (1,0)....(1,cntV-1),(cntU-1,0)...(cntU-1,cntV-1) </tt>
		<br><br>
		However, the order for the C++ API is V-U:
		<br><br>
		<tt> (0,0)...(cntU-1,0), (0,1)....(cntU-1,1)...(0,cntV-1)...(cntU-1,cntV-1) </tt>
		<br><br>
		Since X3DObject::AddNurbsSurface expects the U-V order, you must reorder the array before passing it via the
		\c in_nurbsFormat argument. See \xl UV_Ordering for a sample conversion function.

	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus AddNurbsSurface
	(
		const CNurbsSurfaceData&	in_surfaceData,
		siNurbsFormat				in_nurbsFormat,
		const CString&				in_name,
		X3DObject&					io_3dobj
	);

	/*! Creates a NurbsSurfaceMesh object. This is typically used for creating a nurbs surface from a set of geometry data.
	\param in_surfaceDataArray	An array of NurbsSurface data description.
	\param in_nurbsFormat		Specifies how the data is formatted.
	\param in_name				Name of child.
	\param io_3dobj				New child object.
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus AddNurbsSurfaceMesh
	(
		const CNurbsSurfaceDataArray&	in_surfaceDataArray,
		siNurbsFormat					in_nurbsFormat,
		const CString&					in_name,
		X3DObject&						io_3dobj
	);

	/*! Creates a Particle Cloud.  The cloud has no particle operator, no local material, and no emitter.
	This is suitable for creating a custom simulation with a custom operator.  Alternatively it is possible
	to call the \xl CreateParticleCloud command and then freeze the result.	Any particle type that will be
	specified in calls to ParticleCloudPrimitive::AddParticles should be added initially to the cloud by
	specifying it in the in_parTypes
	argument.
	\param in_parTypes		Optional array of Particle Type Properties to associate with the cloud.
	\param in_name			Name for the new cloud.
	\retval out_particlecloud	Returns the new particle cloud.
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus AddParticleCloud
	(
		const CRefArray&				in_parTypes,
		const CString&					in_name,
		ParticleCloud&					out_particlecloud
	);

	/*! Returns the local rotation. This is a shortcut. You can also retrieve the local scaling by
	calling X3DObject::GetKinematics, Kinematics::GetLocal, KinematicState::GetTransform and
	MATH::CTransformation::GetScaling.
	\return Local scaling.
	\since 3.5.1
	*/
	MATH::CVector3 GetLocalScaling()const;

	/*! Sets the local scaling. This is a shortcut. You can also set the local scaling by calling
	X3DObject::GetKinematics, Kinematics::GetLocal, KinematicState::PutTransform.
	\param  in_scaling local scaling.
	\return CStatus::OK success
	\return CStatus::Fail failure
	\since 3.5.1
	*/
	CStatus PutLocalScaling(MATH::CVector3 in_scaling);


	/*! Returns the local rotation. This is a shortcut. You can also retrieve the local rotation by
	calling X3DObject::GetKinematics, Kinematics::GetLocal, KinematicState::GetTransform and
	MATH::CTransformation::GetRotation.
	\return Local rotation
	\since 3.5.1
	*/
	MATH::CRotation GetLocalRotation()const;

	/*! Sets the local rotation. This is a shortcut. You can also set the local rotation by calling
	X3DObject::GetKinematics, Kinematics::GetLocal, KinematicState::PutTransform.
	\param  in_rotation local rotation.
	\return CStatus::OK success
	\return CStatus::Fail failure
	\since 3.5.1
	*/
	CStatus PutLocalRotation(MATH::CRotation in_rotation);

	/*! Returns the local translation. This is a shortcut. You can also retrieve the local translation
	by calling X3DObject::GetKinematics, Kinematics::GetLocal, KinematicState::GetTransform and
	MATH::CTransformation::GetTranslation.
	\return Local translation
	\since 3.5.1
	*/
	MATH::CVector3 GetLocalTranslation()const;

	/*! Sets the local scaling. This is a shortcut. You can also set the local scaling by calling
	X3DObject::GetKinematics, Kinematics::GetLocal, KinematicState::PutTransform.
	\param  in_translation local translation.
	\return CStatus::OK success
	\return CStatus::Fail failure
	\since 3.5.1
	*/
	CStatus PutLocalTranslation(MATH::CVector3 in_translation);

	private:
	X3DObject * operator&() const;
	X3DObject * operator&();
};

};

#endif // __XSIX3DOBJECT_H__
