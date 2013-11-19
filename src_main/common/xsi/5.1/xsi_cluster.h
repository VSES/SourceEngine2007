//*****************************************************************************
/*!
   \file xsi_cluster.h
   \brief Cluster and CClusterElementArray class declarations.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSICLUSTER_H__
#define __XSICLUSTER_H__

#include <xsi_sceneitem.h>
#include <xsi_status.h>
#include <xsi_longarray.h>

namespace XSI {

class X3DObject;
class SubComponent;
class CClusterElementArray;
class CBitArray;

//*****************************************************************************
/*! \class Cluster xsi_cluster.h
	\brief A cluster represents a set of components on a Geometry.

	For example, the polygons of the left arm of a human model might be grouped together by creating a cluster.

	A cluster has a type (see SIObject::GetType) which corresponds to the component type that it contains,
	(\c pnt, \c knot, \c poly). A geometry can have multiple clusters, including clusters of different types.
	Any particular component might be included in multiple clusters. The ::ClusterTypes that are supported on
	a geometry depend on the type of object. For example only NurbsSurface or NurbsCurve can have a ::siKnotCluster.

	Clusters can be created on the geometry of a ParticleCloud. These clusters are of type ::siVertexCluster but
	they contain Particles.

	A cluster that includes all the components of a particular type is said to be \a always \a complete, which you
	can verify with Cluster::IsAlwaysComplete. An always-complete cluster always covers all components of the geometry,
	even when components are added by a modeling operation; however, notice that a cluster which covers all components
	of a geometry is not necessarily always-complete. Other clusters may contain a subset of components. In all cases
	the indices of the components in the cluster do not directly match the indices of the component on the geometry.
	The mapping between cluster indices and geometry indices is available through Cluster::GetElements and Cluster::FindIndices.

	Clusters are useful for storing per-component data with a ClusterProperty or UserDataMap. Clusters can also have
	their own CustomProperty objects. All these properties can be created with SceneItem::AddProperty and enumerated
	with SceneItem::GetLocalProperties. Notice that some types of ClusterProperty require the cluster to be always complete.

	\warning Operations that change the geometry, for example adding an edge to a mesh, can result in the cluster
	growing or shrinking.

	\note As of v5.0, the base property SIObject::GetParent called on a Cluster object returns a Primitive instead of
	an X3DObject.

	\sa Geometry, CClusterElementArray

	\eg
	\code
		using namespace XSI;
		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject myGrid;
		root.AddGeometry( L"Grid", L"MeshSurface", L"", myGrid );

		Cluster myCls;
		CLongArray indices(9);
		indices[0] = 8;	indices[1] = 16; indices[2] = 24; indices[3] = 32;
		indices[4] = 40; indices[5] = 48; indices[6] = 56; indices[7] = 64;
		indices[8] = 72;

		myGrid.GetActivePrimitive().GetGeometry().AddCluster(siVertexCluster, L"",
			indices, myCls );

		app.LogMessage( L"Cluster type: " + myCls.GetClassIDName() );
	\endcode
 */
//*****************************************************************************
class SICPPSDKDECL Cluster : public SceneItem
{
public:
	/*! Default constructor. */
	Cluster();

	/*! Default destructor. */
	~Cluster();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	Cluster(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	Cluster(const Cluster& in_obj);

	/*! Returns true if a given class type is compatible with this API class.
	\param in_ClassID class type.
	\return true if the class is compatible, false otherwise.
	*/
	bool IsA( siClassID in_ClassID) const;

	/*! Returns the type of the API class.
	\return The class type.
	*/
	siClassID GetClassID() const;

	/*! Creates an object from another object. The newly created object is set
	to empty if the input object is not compatible.
	\param in_obj constant class object.
	\return The new Cluster object.
	*/
	Cluster& operator=(const Cluster& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new Cluster object.
	*/
	Cluster& operator=(const CRef& in_ref);

	/*! Returns an array of geometry indices represented by the cluster.
	\return The new CClusterElementArray object.
	*/
	CClusterElementArray GetElements() const;

	/*! Returns the cluster center reference.
	\return The cluster reference object.
	*/
	X3DObject GetCenterReference() const;

	/*! Sets the cluster center reference. If the reference is empty, the ClusterCenter operator is deleted.
	\param in_centerObj Object to be used as the center reference.
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus PutCenterReference(const X3DObject& in_centerObj);

	/*! Returns true if the cluster has a base pose. The base pose is represented by a StaticKinematicState property.
	\return True if the cluster has a base pose, false otherwise.
	*/
	bool HasStaticKinematicState() const;

	/*! Returns an array containing all StaticKinematicState properties on the cluster.
	\return Array of references to StaticKinematicState properties.
	*/
	CRefArray GetStaticKinematicStates() const;

	/*! Creates a SubComponent from the cluster type.
	\return A SubComponent object.
	*/
	SubComponent CreateSubComponent();

	/*! Returns the cluster index corresponding to the specified geometry component index.
	\param  in_geometryIndex Geometry component index.
	\retval  out_clusterIndex  Cluster index corresponding to the geometry index or -1 if the geometry index
		is not part of the cluster.
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus FindIndex
	(
		LONG in_geometryIndex,
		LONG& out_clusterIndex
	) const;

	/*! Returns the cluster indices corresponding to the specified geometry component indices.
	\param  in_geometryIndices Array of geometry component indices.
	\retval  out_clusterIndices  Array of cluster indices corresponding to the geometry indices or -1 if the
		geometry index is not part of the cluster.
	\return CStatus::OK success
	\return CStatus::Fail failure

	\eg
	\code
		using namespace XSI;
		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject myGrid;
		root.AddGeometry( L"Grid", L"MeshSurface", L"", myGrid );

		CLongArray indices(3);
		indices[0] = 0;	indices[1] = 3; indices[2] = 22;

		Geometry myMesh(myGrid.GetActivePrimitive().GetGeometry());
		Cluster myCls;
		myMesh.AddCluster(siVertexCluster, L"",	indices, myCls );

		Facet myFacet(myMesh.GetFacets().GetItem(0));
		CLongArray aPointIndices = myFacet.GetPoints().GetIndexArray();

		CLongArray aClusterIndices;
		myCls.FindIndices(aPointIndices, aClusterIndices);

		for (LONG i=0; i<(LONG)aClusterIndices.GetCount(); ++i)
		{
			if ( aClusterIndices[i] == -1 )
				app.LogMessage(	L"The point index " +
								CValue(aPointIndices[i]).GetAsText() +
								L" is not part of the cluster.");
			else
				app.LogMessage(L"Cluster index for point index " +
					CValue(aPointIndices[i]).GetAsText() + L"= " +
					CValue(aClusterIndices[i]).GetAsText());
		}
	\endcode
	*/
	CStatus FindIndices
	(
		const CLongArray& in_geometryIndices,
		CLongArray& out_clusterIndices
	) const;

	/*! Returns true if the cluster is maintained as always complete. If so, the cluster will always cover
	all geometry's components even under modeling operations.
	\return True if the cluster is always complete, false otherwise.
	\since 5.0
	*/
	bool IsAlwaysComplete() const;

	/*! Returns an array of flags containing the geometry elements set on this cluster. The array size matches
	the number of elements on the geometry for this cluster type. A true flag indicates a cluster element, the
	position of the flag in the array corresponds to the cluster element.

	If the cluster was generated by an instance of CGeometryAccessor initialized with a subdivision level > 0,
	the array matches the number of subdivided elements on the underlying geometry.

	\note This function will only work for clusters defined on a polygon mesh geometry.

	\return CStatus::OK success
	\return CStatus::False No elements defined on the cluster
	\return CStatus::Fail Error

	\sa CGeometryAccessor, CClusterPropertyBuilder
	\since 5.0

	\eg
	\code
		using namespace XSI;
		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject myCone;
		root.AddGeometry( L"Cone", L"MeshSurface", L"", myCone );

		PolygonMesh myMesh(myCone.GetActivePrimitive().GetGeometry());

		CClusterPropertyBuilder cpBuilder = myMesh.GetClusterPropertyBuilder();

		// add a vertex color property on the geometry, this creates a complete node
		// cluster on the geometry
		ClusterProperty color = cpBuilder.AddVertexColor();

		// add a weight map property on the geometry, this creates an empty vertex
		// cluster
		ClusterProperty wmap = cpBuilder.AddWeightMap(false);

		// set the color vertex values
		LONG indices[] = {0, 3, 22};

		float colors[] =
		{
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f
		};

		color.SetValues( indices, colors, 3 );

		// set the weight map values
		float weights[] = { 1.0f, 0.5f, 0.75f };

		wmap.SetValues( indices, weights, 3 );

		// get a geometry accessor initialized with a subdivision level of 1
		CGeometryAccessor ga = myMesh.GetGeometryAccessor(siConstructionModeModeling, siCatmullClark, 1);

		// get the node and vertex clusters created with CClusterPropertyBuilder
		CRefArray clusters = ga.GetAllClusters();

		// log the clusters element ids
		for (LONG i=0; i<clusters.GetCount(); i++)
		{
			Cluster cls = clusters[i];

			CBitArray flags;
			CStatus st = cls.GetGeometryElementFlags( flags );
			st.AssertSucceeded();

			app.LogMessage( L"Cluster name: " + cls.GetName() );
			app.LogMessage( L"Nb geometry elements: " + CString(flags.GetCount()) );
			app.LogMessage( L"Nb cluster elements: " + CString(flags.GetTrueCount()) );

			LONG idx = flags.GetIterator();
			while( flags.GetNextTrueBit( idx ) )
			{
				app.LogMessage( CString(idx) + L":" + CString( flags[idx] ) );
			}
		}
	\endcode
	*/
	CStatus GetGeometryElementFlags( CBitArray& out_flags ) const;

	private:
	Cluster * operator&() const;
	Cluster * operator&();

};

//*************************************************************************
/*! \class CClusterElementArray xsi_cluster.h
	\brief An array of cluster elements in a Cluster object.

	A cluster maps the geometry component (Point, Segment, Facet, Sample point) indices to cluster element indices.
	The value of the element in a cluster element array is the geometry component's index.

	A cluster element array is read-only.

	\warning This specialized array is returned by Cluster::GetElements, it is not meant to be created
		and modified in user-defined functions. If you want to add and remove arbitrary items to a
		collection, you must use a CRefArray instead.

	\sa Cluster::GetElements

	\eg
	\code
		using namespace XSI;
		using namespace XSI;
		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject myGrid;
		root.AddGeometry( L"Grid", L"MeshSurface", L"", myGrid );

		CLongArray indices(9);
		indices[0] = 8;	indices[1] = 16; indices[2] = 24; indices[3] = 32;
		indices[4] = 40; indices[5] = 48; indices[6] = 56; indices[7] = 64;
		indices[8] = 72;

		Cluster myCls;
		myGrid.GetActivePrimitive().GetGeometry().AddCluster(siVertexCluster, L"",
			indices, myCls );

		CClusterElementArray elements(myCls.GetElements());

		CLongArray values(elements.GetArray());

		for (LONG i=0; i<values.GetCount(); ++i)
		{
			app.LogMessage(L"Element " + CValue(i).GetAsText() + L"= " +
				CValue(values[i]).GetAsText());
		}
	\endcode
*/
//*************************************************************************
class SICPPSDKDECL CClusterElementArray
{
	public:
	/*! Default constructor*/
	CClusterElementArray();

	/*! Copy constructor.
	\param in_array constant CClusterElementArray object.
	*/
	CClusterElementArray(const CClusterElementArray& in_array);

	/*!Default destructor*/
	virtual ~CClusterElementArray();

	/*! Creates an array from a reference object.
	\param in_array constant CClusterElementArray object.
	\return The new CClusterElementArray object.
	*/
	CClusterElementArray& operator=(const CClusterElementArray& in_array);

	/*! Returns an array of element values.
	\return Array of element values
	*/
	CLongArray GetArray() const;

	/*! Returns the number of cluster element values
	\return The number of cluster element values.
	*/
	LONG GetCount() const;

	/*! Returns the cluster element value at a given index (0-based)
	\param in_index Element index.
	\return The element value.
	*/
	LONG GetItem( LONG in_index ) const;

	/*! Returns cluster element values by indices.
	\param in_indices Array of indices
	\return Array of element values
	*/
	CLongArray GetItemsByIndex( const CLongArray& in_indices ) const;

	/*! Searches the array for a given element and returns its index
	\param in_element Element index.
	\return The element index or -1 if the element is not found.
	*/
	LONG FindIndex( LONG in_element ) const;

	private:
	void* m_ptr;
};

};

#endif // __XSICLUSTER_H__
