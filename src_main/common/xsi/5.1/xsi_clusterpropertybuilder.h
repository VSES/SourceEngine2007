//*****************************************************************************
/*!
   \file xsi_clusterpropertybuilder.h
   \brief CClusterPropertyBuilder class declaration.

   © Copyright 1998-2005 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSICLSPROPBUILDER_H__
#define __XSICLSPROPBUILDER_H__

#include <xsi_base.h>
#include <xsi_value.h>
#include <xsi_status.h>
#include <xsi_longarray.h>

#pragma warning(disable:4251)

namespace XSI {

//*****************************************************************************
/*! \class CClusterPropertyBuilder xsi_clusterpropertybuilder.h
   \brief Provides the services to accelerate the process of creating new cluster properties on a geometry.

	The class saves you from creating specific clusters in order to add your properties by creating default
	clusters. These clusters are reused by the builder when adding subsequent properties. CClusterPropertyBuilder
	is convenient for importing data in %XSI where performance is critical. All operations performed by the
	builder are undoable but it is recommended to disable the undo service when using the builder for maximum results.

	You use PolygonMesh::GetClusterPropertyBuilder to create an instance of CClusterPropertyBuilder. At creation
	time the builder automatically freezes all operators currently installed on the geometry. This ensures that
	subsequent operations for setting the property values will not be affected by any operators that might be
	installed on existing clusters.

	\since 5.0

	\eg Adding a vertex color and a weight property with the cluster property builder.
	\code
		using namespace XSI;

		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject myCube;
		root.AddGeometry( L"Cube", L"MeshSurface",L"myCube",myCube);

		PolygonMesh polymesh = myCube.GetActivePrimitive().GetGeometry();

		CClusterPropertyBuilder cpBuilder = polymesh.GetClusterPropertyBuilder();

		// add a vertex color property on a complete cluster
		ClusterProperty cp = cpBuilder.AddVertexColor();

		// set the vertex color for polygon 0 (nodes 0 1 2 3)
		float colors[] =
		{
			1.f, 0.f, 0.f, 0.f,
			0.f, 1.f, 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			1.f, 0.f, 1.f, 0.f,
		};

		cp.SetValues( colors, 4 );

		// add a weight map property on vertices 0 2 4 6
		X3DObject myCube2;
		root.AddGeometry( L"Cube", L"MeshSurface", L"myCube2", myCube2 );

		polymesh = myCube2.GetActivePrimitive().GetGeometry();

		cpBuilder = polymesh.GetClusterPropertyBuilder();

		cp = cpBuilder.AddWeightMap(false);


		float weights[] = { .11f, .12f, .13f, .14f };
		LONG elems[] = {0,2,4,6};

		cp.SetValues( elems, weights, 4 );

		CFloatArray wValues;
		cp.GetValues( wValues );
		app.LogMessage( L"Weight element 0: " + CString(wValues[0]) );
		app.LogMessage( L"Weight element 2: " + CString(wValues[2]) );
		app.LogMessage( L"Weight element 4: " + CString(wValues[4]) );
		app.LogMessage( L"Weight element 6: " + CString(wValues[6]) );

		// Expected results:
		// INFO: Weight element 0: 0.11
		// INFO: Weight element 2: 0.12
		// INFO: Weight element 4: 0.13
		// INFO: Weight element 6: 0.14

	\endcode
*/
//*****************************************************************************

class SICPPSDKDECL CClusterPropertyBuilder : public CBase
{
public:

	/*! Default constructor. */
	CClusterPropertyBuilder();

	/*! Default destructor. */
	~CClusterPropertyBuilder();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	CClusterPropertyBuilder(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	CClusterPropertyBuilder(const CClusterPropertyBuilder& in_obj);

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
	\return The new CClusterPropertyBuilder object.
	*/
	CClusterPropertyBuilder& operator=(const CClusterPropertyBuilder& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new CClusterPropertyBuilder object.
	*/
	CClusterPropertyBuilder& operator=(const CRef& in_ref);

	/*! Adds a UV property to a default node cluster. The function adds the new property on the first node
	cluster found on the geometry or creates a new one if none exists. The cluster used is always the size
	of the geometry (ie., complete cluster) and the newly created property is always resized to match the
	number of elements on the cluster.
	\return A reference to the newly created ClusterProperty object.
	\sa ClusterProperty::SetValues
	*/
	CRef AddUV();

	/*! Adds a UV property to a specified node cluster. The cluster must match the size of the geometry (ie.,
	complete cluster). If the cluster doesn't exist or is not complete, the function attempts to reuse an
	existing node cluster or creates a new one otherwise. The newly created property is always resized to
	match the number of elements on the cluster.
	\param io_strPropName The name of the new property to create.
	\param io_strClusterName The name of a cluster where to add the property.
	\return A reference to the newly created ClusterProperty object.
	\sa ClusterProperty::SetValues
	*/
	CRef AddUV( CString& io_strPropName, CString& io_strClusterName );

	/*! Adds a user normal property to a default node cluster. The newly created property is always resized to
	match the number of elements on the cluster.

	The size of the default cluster must match the number of elements on the geometry if \c in_bCompleteCluster
	is true. If no default cluster exists or none matches the complete cluster criteria, a new one is created.
	In this case a complete cluster is created if \c in_bCompleteCluster is true, otherwise an empty one is
	created. Empty clusters can be extended later when values are added to the property with ClusterProperty::SetValues.

	\param in_bCompleteCluster True if the number of elements of the cluster must match the size of the
		geometry. Defaults to true.
	\return A reference to the newly created ClusterProperty object.
	\sa ClusterProperty::SetValues
	*/
	CRef AddUserNormal(bool in_bCompleteCluster=true);

	/*! Adds a user normal property to a specified node cluster. The newly created property is always resized to
	match the number of elements on the cluster.

	The size of the specified cluster must match the number of elements on the geometry if \c in_bCompleteCluster
	is true. If no cluster exists or none matches the criterias, a new one is created. In this case a complete
	cluster is created \c if in_bCompleteCluster is true, otherwise an empty one is created. Empty clusters can be
	extended later when values are added to the property with ClusterProperty::SetValues.

	\param io_strPropName The name of the new property to create.
	\param io_strClusterName The name of a cluster where to add the property.
	\param in_bCompleteCluster True if the number of elements of the cluster must match the size of the
		geometry. Defaults to true.
	\return A reference to the newly created ClusterProperty object.
	\sa ClusterProperty::SetValues
	*/
	CRef AddUserNormal(
		CString&	io_strPropName,
		CString&	io_strClusterName,
		bool		in_bCompleteCluster=true );

	/*! Adds a vertex color property to a default node cluster. This function adds the new property on the first node
	cluster found on the geometry or creates a new one if none exists. The cluster used is always the size of the
	geometry (ie., complete cluster) and the newly created property is always resized to match the number of elements
	on the cluster.
	\return A reference to the newly created ClusterProperty object.
	\sa ClusterProperty::SetValues
	*/
	CRef AddVertexColor();

	/*! Adds a vertex color property on a specified node cluster. The cluster must match the size of the geometry
	(ie., complete cluster). A new cluster is created if the specified	cluster doesn't exist or is not complete. The
	newly created property is always resized to match the number of elements on the cluster.
	\param io_strPropName The name of the new property to create.
	\param io_strClusterName The name of a cluster where to add the property.
	\return A reference to the newly created ClusterProperty object.
	\sa ClusterProperty::SetValues
	*/
	CRef AddVertexColor( CString& io_strPropName, CString& io_strClusterName );

	/*! Adds a weight map property to a default node cluster. The newly created property is always resized to match the
	number of elements on the cluster.

	The size of the default cluster must match the number of elements on the geometry if \c in_bCompleteCluster is true.
	If no default cluster exists or none matches the complete cluster criteria, a new one is created. In this case a
	complete cluster is created if \c in_bCompleteCluster is true, otherwise an empty one is created. Empty clusters can
	be extended later when values are added to the property with ClusterProperty::SetValues.

	The newly created property is always resized to match the number of elements on the cluster.

	\param in_bCompleteCluster True if the number of elements of the cluster must match the size of the
		geometry. Defaults to true.
	\return A reference to the newly created ClusterProperty object.
	\sa ClusterProperty::SetValues
	*/
	CRef AddWeightMap(bool in_bCompleteCluster=true);

	/*! Adds a weight map property to a specified node cluster. The newly created property is always resized to match the
	number of elements on the cluster.

	The size of the specified cluster must match the number of elements on the geometry if \c in_bCompleteCluster is true.
	If no cluster exists or none matches the criterias, a new one is created. In this case a complete cluster is created
	if \c in_bCompleteCluster is true, otherwise an empty one is created. Empty clusters can be extended later when values
	are added to the property with ClusterProperty::SetValues.

	\param io_strPropName The name of the new property to create.
	\param io_strClusterName The name of a cluster where to add the property.
	\param in_bCompleteCluster True if the number of elements of the cluster must match the size of the
		geometry. Defaults to true.
	\return A reference to the newly created ClusterProperty object.
	\sa ClusterProperty::SetValues
	*/
	CRef AddWeightMap(
		CString&	io_strPropName,
		CString&	io_strClusterName,
		bool		in_bCompleteCluster=true );

	/*! Adds an envelope weight property and envelope operator to a default vertex cluster. The property and cluster are
	connected to the operator according to the list of input deformers. The newly created property is resized to match the
	number of elements on the cluster.

	The size of the default cluster must match the number of elements on the geometry if \c in_bCompleteCluster is true.
	If no default cluster exists or none matches the complete cluster criteria, a new one is created. In this case a complete
	cluster is created if \c in_bCompleteCluster is true, otherwise an empty one is created. Empty clusters can be extended
	later when values are added to the property with EnvelopeWeight::SetValues. If the specified cluster is already connected
	to an envelope operator, the input deformers are connected to the existing operator.

	\param in_deformers Array of X3DObject deformers to connect.
	\param in_bCompleteCluster True if the number of elements of the cluster must match the size of the
		geometry. Defaults to true.
	\return A reference to the newly created EnvelopeWeight object or the existing EnvelopeWeight object
		if an envelope is already set on the cluster.
	\sa EnvelopeWeight::SetValues

	\eg
	\code
		Application app;

		Model root = app.GetActiveSceneRoot();

		// Create the deformers
		Null myDeformer1;
		root.AddNull( L"", myDeformer1 );

		Null myDeformer2;
		root.AddNull( L"", myDeformer2 );

		// Set the deformers position
		KinematicState nullGlobalKineState(myDeformer1.GetKinematics().GetGlobal());
		Parameter param(nullGlobalKineState.GetParameters().GetItem(L"posx"));
		param.PutValue( (double)4.0 );

		param = nullGlobalKineState.GetParameters().GetItem(L"posy");
		param.PutValue( (double)5.0 );

		nullGlobalKineState = myDeformer2.GetKinematics().GetGlobal();
		param = nullGlobalKineState.GetParameters().GetItem(L"posx");
		param.PutValue( (double)-4.0 );

		param = nullGlobalKineState.GetParameters().GetItem(L"posy");
		param.PutValue( (double)5.0 );

		// Add the envelope weight property on a sphere and connect the 2 deformers
		// note: AddEnvelopeWeight also creates a non-complete vertex cluster along
		// with the envelope operator for connecting the 2 deformer.

		X3DObject mySphere;
		root.AddGeometry( L"Sphere", L"MeshSurface", L"mySphere", mySphere );

		// Get a cluster property builder from the new sphere
		PolygonMesh polymesh = mySphere.GetActivePrimitive().GetGeometry();
		CClusterPropertyBuilder cpBuilder = polymesh.GetClusterPropertyBuilder();

		CRefArray deformers(2);
		deformers[0] = myDeformer1;
		deformers[1] = myDeformer2;

		EnvelopeWeight ewp = cpBuilder.AddEnvelopeWeight(deformers,false);

		float weights[] =
		{
			75.f, 60.f, 50.f, 0.f,	// myDeformer1
			25.f, 40.f, 50.f, 100.f // myDeformer2
		};

		// Set the envelope weights for these specific vertex indices
		LONG vertexIndices[] = {8,15,36,43};
		LONG nIndices = 4;

		ewp.SetValues( deformers[0], vertexIndices, weights, nIndices );
		ewp.SetValues( deformers[1], vertexIndices, &weights[4], nIndices );

		// Log the weight values set on the geometry
		// note: Since the weights are set on a partial vertex cluster, it is
		// recommended to use a CBitArray (i.e. vertexIndexFlags) for getting
		// the weight values set on the geometry. The position of the true elements
		// in vertexIndexFlags indicates the vertex set with a weight.
		CBitArray vertexIndexFlags;
		CFloatArray valueArray;
		ewp.GetValues( valueArray, vertexIndexFlags );

		LONG vIdx = vertexIndexFlags.GetIterator();
		LONG nDeformers = deformers.GetCount();

		while (vertexIndexFlags.GetNextTrueBit(vIdx))
		{
			CString str(vIdx);
			str += L":";

			// compute the index to access valueArray
			LONG nIdx = vIdx*nDeformers;

			// get weights for each deformer at vertex vIdx
			for (LONG i=0; i<nDeformers; i++)
			{
				float val = valueArray[nIdx+i];
				str += CString(L" ") + CString(val);
			}
			app.LogMessage( str );
		}

		// Log results:
		// INFO : 8: 75 25
		// INFO : 15: 60 40
		// INFO : 36: 50 50
		// INFO : 43: 0 100
	\endcode
	*/
	CRef AddEnvelopeWeight(
		const CRefArray&	in_deformers,
		bool				in_bCompleteCluster=true );

	/*! Adds an envelope weight property and envelope operator on a specified vertex cluster. The new property and cluster
	are connected to the operator according to the list of input deformers. The newly created property is resized to match
	the number of elements on the cluster.

	The size of the specified cluster must match the number of elements on the geometry if \c in_bCompleteCluster is true.
	If no cluster exists or none matches the criterias, a new one is created. In this case a complete cluster is created if
	\c in_bCompleteCluster is true, otherwise an empty one is created. Empty clusters can be extended later when values are
	added to the property with EnvelopeWeight::SetValues. If the specified cluster is already connected to an envelope operator,
	the input deformers are connected to the existing operator.

	\param in_deformers Array of X3DObject deformers to connect.
	\param io_strPropName The name of the new property to create.
	\param io_strClusterName The name of a cluster where to add the property.
	\param in_bCompleteCluster True if the number of elements of the cluster must match the size of the
		geometry. Defaults to true.
	\return A reference to the newly created EnvelopeWeight object or the existing EnvelopeWeight object
		if an envelope is already set on the cluster.
	\sa EnvelopeWeight::SetValues

	\eg
	\code
		Application app;

		Model root = app.GetActiveSceneRoot();

		X3DObject myCube;
		root.AddGeometry( L"Cube", L"MeshSurface", L"myCube", myCube );

		// Create a 3 bones chain under the cube
		ChainRoot myChainRoot;
		myCube.Add2DChain(
			CVector3(3, 0, 3),
			CVector3(4, 0, 0),
			CVector3(-90, 0, 0),
			si2DChainTop,
			L"My2DChain",
			myChainRoot );

		ChainBone myBone1;
		myChainRoot.AddBone(
			CVector3(3,0,-3),
			siChainBonePin,
			L"myBone1",
			myBone1 );

  		ChainBone myBone2;
		myChainRoot.AddBone(
			CVector3(-3,0,-3),
			siChainBonePin,
			L"myBone2",
			myBone2 );

		// Get a cluster property builder from the cube
		PolygonMesh polymesh = myCube.GetActivePrimitive().GetGeometry();
		CClusterPropertyBuilder cpBuilder = polymesh.GetClusterPropertyBuilder();

		// Add the envelope weight property.
		// note: AddEnvelopeWeight also creates a complete vertex cluster along with
		// the envelope operator for connecting the 3 bones chain.
		CRefArray deformers(myChainRoot.GetBones());

		CString strPropName(L"MyEnvelopeWeightProperty");
		CString strClsName(L"MyEnvelopeWeightCluster");
		EnvelopeWeight ewp = cpBuilder.AddEnvelopeWeight( deformers, strPropName, strClsName );

		// Set the weight values for each vertex on the cube
		float envweights[] =
		{
			20.f, 60.f, 50.f, 70.f, 50.f, 50.f, 40.f, 70.f, // bone 0
			20.f, 20.f, 40.f, 10.f,  0.f, 50.f, 40.f, 10.f, // bone 1
			60.f, 20.f, 10.f, 20.f, 50.f,  0.f,	20.f, 20.f  // bone 2
		};

		for (ULONG i=0; i<deformers.GetCount(); i++)
		{
			CRef def = deformers[i];
			ewp.SetValues( def, &envweights[i*8], 8 );
		}

		// Read the weight values from the property and log
		CFloatArray valueArray;
		ewp.GetValues( valueArray );

		LONG nDeformers = deformers.GetCount();
		for (LONG i=0, v=0; i<valueArray.GetCount(); i += nDeformers)
		{
			CString str(v++);
			str += L":";
			LONG nIdx = i;
			for ( LONG j=0; j<nDeformers; j++ )
			{
				float val = valueArray[nIdx++];
				str += CString(L" ") + CString(val);
			}
			app.LogMessage( str );
		}

		// Log results:
		// INFO : 0: 20 20 60
		// INFO : 1: 60 20 20
		// INFO : 2: 50 40 10
		// INFO : 3: 70 10 20
		// INFO : 4: 50  0 50
		// INFO : 5: 50 50  0
		// INFO : 6: 40 40 20
		// INFO : 7: 70 10 20

	\endcode
	*/
	CRef AddEnvelopeWeight(
		const CRefArray&	in_deformers,
		CString&			io_strPropName,
		CString&			io_strClusterName,
		bool				in_bCompleteCluster=true );

	/*! Adds a shape key property to a default node cluster. The newly created property is always resized to match
	the number of elements on the cluster. The function doesn't apply the new shape key to the underlying object's
	mixer automatically, you must use the \xl ApplyShapeKey command to do so.

	The size of the default cluster must match the number of elements on the geometry if \c in_bCompleteCluster is
	true. If no default cluster exists or none matches the complete cluster criteria, a new one is created. In this
	case a complete cluster is created if \c in_bCompleteCluster is true, otherwise an empty one is created. Empty
	clusters can be extended later when values are added to the property with ClusterProperty::SetValues.

	\param in_refMode The reference mode for the new shape key to create. All shape keys created on the same cluster
		must use the same reference mode. The function ensures that all shape keys are created with the reference
		mode used by the first shape key created on the cluster. Defaults to ::siShapeObjectReferenceMode.
	\param in_bCompleteCluster True if the number of elements of the cluster must match the size of the
		geometry. Defaults to true.
	\return A reference to the newly created ClusterProperty object.
	\sa ClusterProperty::SetValues
	*/
	CRef AddShapeKey(
		XSI::siShapeReferenceMode	in_refMode = XSI::siShapeObjectReferenceMode,
		bool						in_bCompleteCluster=true );

	/*! Adds a shape key property to a specified node cluster. The newly created property is always resized to match
	the number of elements on the cluster. The function doesn't apply the new shape key to the underlying object's
	mixer automatically, you must use the \xl ApplyShapeKey command to do so.

	The size of the specified cluster must match the number of elements on the geometry if \c in_bCompleteCluster is
	true. If no cluster exists or none matches the criterias, a new one is created. In this case a complete cluster is
	created if \c in_bCompleteCluster is true, otherwise an empty one is created. Empty clusters can be extended later
	when values are added to the property with ClusterProperty::SetValues.

	\param in_refMode The reference mode for the new shape key to create. All shape keys created on the same cluster
		must use the same reference mode. The function ensures that all shape keys are created with the reference
		mode used by the first shape key created on the cluster. Defaults to ::siShapeObjectReferenceMode.
	\param io_strPropName The name of the new property to create.
	\param io_strClusterName The name of a cluster where to add the property.
	\param in_bCompleteCluster True if the number of elements of the cluster must match the size of the
		geometry. Defaults to true.
	\return A reference to the newly created ClusterProperty object.
	\sa ClusterProperty::SetValues
	*/
	CRef AddShapeKey(
		XSI::siShapeReferenceMode	in_refMode,
		CString&					io_strPropName,
		CString&					io_strClusterName,
		bool						in_bCompleteCluster=true );

	/*! Adds a user data map property to a default cluster of a specific type. The newly created property is always
	resized to match the number of elements on the cluster.

	The size of the default cluster must match the number of elements on the geometry if \c in_bCompleteCluster is true.
	If no default cluster exists or none matches the criterias, a new one is created. In this case a complete cluster
	is created if \c in_bCompleteCluster is true, otherwise an empty one is created. Empty clusters can be extended
	later when values are added to the property with UserDataMap::SetValues or UserDataMap::SetFixedValues.

	\param in_clsType The type of the cluster to use.
	\param in_bCompleteCluster True if the number of elements of the cluster must match the size of the
		geometry. Defaults to true.
	\return A reference to the newly created UserDataMap object.
	\sa UserDataMap::SetValues, UserDataMap::SetFixedValues
	*/
	CRef AddUserDataMap(
		XSI::siClusterType	in_clsType,
		bool				in_bCompleteCluster=true );

	/*! Adds user data map property to a cluster of a specific type. The newly created property is always resized to
	match the number of elements on the cluster.

	The size of the specified cluster must match the number of elements on the geometry if \c in_bCompleteCluster is
	true. If no cluster exists or none matches the criterias, a new one is created. In this case a complete cluster is
	created if \c in_bCompleteCluster is true, otherwise an empty one is created. Empty clusters can be extended later
	when values are added to the property with UserDataMap::SetValues or UserDataMap::SetFixedValues.

	\param in_clsType The type of the cluster to use.
	\param io_strPropName The name of the new property to create.
	\param io_strClusterName The name of a cluster where to add the property. The cluster type must match \c in_clsType,
		otherwise a new cluster is created.
	\param in_bCompleteCluster True if the number of elements of the cluster must match the size of the
		geometry. Defaults to true.
	\return A reference to the newly created UserDataMap object.
	\sa UserDataMap::SetValues, UserDataMap::SetFixedValues
	*/
	CRef AddUserDataMap(
		XSI::siClusterType	in_clsType,
		CString&			io_strPropName,
		CString&			io_strClusterName,
		bool				in_bCompleteCluster=true );

	private:
	CClusterPropertyBuilder * operator&() const;
	CClusterPropertyBuilder * operator&();
};

};

#endif // __XSICLSPROPBUILDER_H__
