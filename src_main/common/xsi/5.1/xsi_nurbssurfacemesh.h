//*****************************************************************************
/*!
   \file xsi_nurbssurfacemesh.h
   \brief NurbsSurfaceMesh class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSINURBSSURFACEMESH_H__
#define __XSINURBSSURFACEMESH_H__

#include <xsi_geometry.h>
#include <xsi_nurbsdata.h>
#include <xsi_pointlocatordata.h>

namespace XSI {

class CControlPointRefArray;
class CNurbsSurfaceRefArray;
class CKnotArray;
class CTrimDataArray;
class NurbsSurface;

//*****************************************************************************
/*! \class NurbsSurfaceMesh xsi_nurbssurfacemesh.h
	\brief A NurbsSurfaceMesh is a kind of Geometry.

	We can navigate the NurbsSurfaceMesh using the generic terms such as Facets or Points or we can
	choose to use navigate using terms that are specific to the NurbsSurfaceMesh such as NurbsSurfaces,
	ControlPoints. There are some functions that do not fit with a generic terms such as Knots, which
	are specific to NURBS, this is why there are specific terms as well as generic terms.

	\sa Primitive::GetGeometry, X3DObject::AddGeometry, X3DObject::AddNurbsSurfaceMesh

	\eg
	\code
		using namespace XSI;

		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject mySphere;
		root.AddGeometry( L"Sphere", L"NurbsSurface", L"", mySphere );

		NurbsSurfaceMesh mySurfaceMesh(mySphere.GetActivePrimitive().GetGeometry());
	\endcode
*/
//*****************************************************************************

class SICPPSDKDECL NurbsSurfaceMesh : public Geometry
{
public:
	/*! Default constructor. */
	NurbsSurfaceMesh();

	/*! Default destructor. */
	~NurbsSurfaceMesh();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	NurbsSurfaceMesh(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	NurbsSurfaceMesh(const NurbsSurfaceMesh& in_obj);

	/*! Copy constructor.
	\param in_obj Geometry object.
	*/
	NurbsSurfaceMesh(const Geometry& in_obj);

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
	\return The new NurbsCurveList object.
	*/
	NurbsSurfaceMesh& operator=(const NurbsSurfaceMesh& in_obj);

	/*! Creates a Polygon object from a Geometry object. The newly created
	object is set to empty if the input Geometry object is not compatible.
	\param in_geom constant class object.
	\return The new NurbsCurveList object.
	*/
	NurbsSurfaceMesh& operator=(const Geometry& in_geom);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new NurbsCurveList object.
	*/
	NurbsSurfaceMesh& operator=(const CRef& in_ref);

	/*! Returns an array of all NurbsSurface objects on this NurbsSurfaceMesh object.
	\return A NurbsSurface array.
	*/
	CNurbsSurfaceRefArray GetSurfaces()const;

	/*! Returns the surface index, position of the point on it, UV values and
	distance from the given position.
	\param in_vPosition A position expressed in the NurbsSurfaceMesh object frame of reference.
	\retval out_lSurfaceIndex The surface index to which the position is the closest.
	\retval out_dSquaredDistance The distance of the input position to the curve point.
	\retval out_dUValue The corresponding UValue on the NurbsSurface.
	\retval out_dVValue The corresponding VValue on the NurbsSurface.
	\retval out_vPosition The actual curve point position at U value.
	\return CStatus::OK success
	\return CStatus::Fail other failure
	*/
	CStatus GetClosestSurfacePosition
	(
		MATH::CVector3&			in_vPosition,
		LONG&					out_lSurfaceIndex,
		double&					out_dSquaredDistance,
		double&					out_dUValue,
		double&					out_dVValue,
		MATH::CVector3&			out_vPosition
	)const;


	/*! Returns a complete data description of a nurbs surface mesh.
	\param in_siNurbsFormat	Specifies how the data is formatted.
	\retval out_surfaces The data description of all surfaces.
	\return CStatus::OK success
	\return CStatus::Fail other failure
	*/
	CStatus Get
	(
		siNurbsFormat				in_siNurbsFormat,
		CNurbsSurfaceDataArray&		out_surfaces
	)const;

	/*! Sets from a complete data description of the nurbs surface mesh. This is only available
	from compiled operators. For non-compiled operators this property can only be set if the
	object has been frozen.

	\note If you use this function on an object with clusters and you change the topology
		the burden of updating the clusters is on the user.

	\param in_surfaces NurbsSurfaces data description.
	\param in_siNurbsFormat Specifies how the data is formatted.
	\return CStatus::OK success
	\return CStatus::AccessDenied function used outside of a plug-in operator context.
	*/
	CStatus Set
	(
		const CNurbsSurfaceDataArray&	in_surfaces,
		siNurbsFormat					in_siNurbsFormat = siSINurbs
	);


	/*! Adds a trimmed surface to the mesh.
	\param in_surface The surface descriptsion
	\param in_trims	The trims data that will apply to this surface.
	\param in_siNurbsFormat The format of representation for the nurbs data.
	\retval out_Surface The newly created surface.
	\return CStatus::OK success
	\return CStatus::AccessDenied function used outside of a plug-in operator
	context.
	*/
	CStatus AddSurfaceWithTrim
	(
		const CNurbsSurfaceData&	in_surface,
		const CTrimCurveDataArray&	in_trims,
		siNurbsFormat				in_siNurbsFormat,
		NurbsSurface&				out_Surface
	);

	/*! Adds a surface to the mesh.
	\param in_surface The surface description
	\param in_siNurbsFormat The format of representation for the nurbs data.
	\retval out_Surface The newly created surface.
	\return CStatus::OK success
	\return CStatus::AccessDenied function used outside of a plug-in operator context.
	*/
	CStatus AddSurface
	(
		const CNurbsSurfaceData&	in_surface,
		siNurbsFormat				in_siNurbsFormat,
		NurbsSurface&				out_Surface
	);

	/*! Returns the subsurface indices on which point locators are defined. Notice that this
	information is part of the NURBS surface mesh point locator's definition, and depends on
	the topology only (won't change if the geometry is deformed).

	The normalized UV parameters within the subsurface can be queried with
	NurbsSurfaceMesh::GetNormalizedUVArray.

	\param in_ptLocators Contains the point locations to be queried.
	\param in_nbPointLocatorsIndices Number of point locators to be queried (-1 if all)
	\param in_pPointLocatorsIndices %Point locator indices to be queried (not used if in_nbPointLocatorsIndices is -1)
	\retval out_pIndices Returned subsurface indices. Size must be in_ptLocators.GetCount() if
		in_nbPointLocatorsIndices is -1, in_nbPointLocatorsIndices otherwise.

	\return CStatus::OK success
	\return CStatus::Fail failure

	\sa NurbsSurfaceMesh::GetNormalizedUVArray, NurbsSurfaceMesh::ConstructPointLocators
	\since 5.0

	\eg This example topologically describes the point locators resulting from the
		shrink-wrapping of a cube onto a NURBS surface sphere.

	\code
		using namespace XSI;
		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject meshCubeObj;
		root.AddGeometry( L"Cube", L"MeshSurface", L"", meshCubeObj );
		PolygonMesh meshCubeGeom( meshCubeObj.GetActivePrimitive().GetGeometry() );

		X3DObject NURBSSphereObj;
		root.AddGeometry( L"Sphere", L"NurbsSurface", L"", NURBSSphereObj );
		NurbsSurfaceMesh NURBSSphereGeom( NURBSSphereObj.GetActivePrimitive().GetGeometry() );

		MATH::CVector3Array posArray = meshCubeGeom.GetPoints().GetPositionArray();
		PointLocatorData cubeOnSpherePointLocators = NURBSSphereGeom.GetClosestLocations(posArray.GetCount(), (double*)&posArray[0]);

		LONG i;
		for(i = 0; i < cubeOnSpherePointLocators.GetCount(); i++)
		{
			LONG subsurface;
			NURBSSphereGeom.GetSubSurfaceIndexArray(
							cubeOnSpherePointLocators,
							1, &i, &subsurface);
			float UVs[2];
			NURBSSphereGeom.GetNormalizedUVArray(
							cubeOnSpherePointLocators,
							1, &i, UVs);
			app.LogMessage(L"Point locator " + CString(CValue(i)) + L" is on subsurface " + CString(CValue(subsurface)) +
							L" at {U = " + CString(CValue(UVs[0])) + L" , V = " + CString(CValue(UVs[1])) + L"}.");
		}
		//Expected results:
		//INFO : Point locator 0 is on subsurface 0 at {U = 1.35528e-016 , V = 0.30423}.
		//INFO : Point locator 1 is on subsurface 0 at {U = 0.75 , V = 0.304065}.
		//INFO : Point locator 2 is on subsurface 0 at {U = 2.45219e-017 , V = 0.695454}.
		//etc.
	\endcode
	*/
	CStatus GetSubSurfaceIndexArray
	(
		const PointLocatorData& in_ptLocators,
		LONG in_nbPointLocatorsIndices, //-1 if all
		const LONG* in_pPointLocatorsIndices, // 0 if all
		LONG* out_pIndices
	) const;

	/*! Returns the normalized UV surface parameters defining point locators. Notice that this
	information is part of NURBS surface mesh point locator's definition, and depends on the
	topology only (won't change if the geometry is deformed).

	The subsurface index on which the normalized UVs apply can be queried with
	NurbsSurfaceMesh::GetSubSurfaceIndexArray.

	\param in_ptLocators Contains the point locations to be queried.
	\param in_nbPointLocatorsIndices Number of point locators to be queried (-1 if all)
	\param in_pPointLocatorsIndices %Point locator indices to be queried (not used if in_nbPointLocatorsIndices is -1)
	\retval out_pNormalizedUVs Returned UV normalized values. Size must be 2*in_ptLocators.GetCount() if
		in_nbPointLocatorsIndices is -1, 2*in_nbPointLocatorsIndices otherwise.

	\return CStatus::OK success
	\return CStatus::Fail failure

	\sa NurbsSurfaceMesh::GetSubSurfaceIndexArray, NurbsSurfaceMesh::ConstructPointLocators
	\since 5.0

	\eg Another example using this function can be found in NurbsSurfaceMesh::GetSubSurfaceIndexArray
		method's description.

	\eg This example creates point locators at random surface locations, and positions a Null at
		each of these point locators.

	\code
		using namespace XSI;

		void CreateNullsAtPointLocations( X3DObject& inObj, const PointLocatorData& inPointLocators )
		{
			Geometry geom( inObj.GetActivePrimitive().GetGeometry() );

			std::vector<double> posData, normData;
			posData.resize(inPointLocators.GetCount()*3);
			normData.resize(inPointLocators.GetCount()*3);

			geom.EvaluatePositions(inPointLocators, -1, 0, &posData.front());
			geom.EvaluateNormals(inPointLocators, siInterpolatedVertexGeometricNormals, -1, 0, &normData.front());

			MATH::CVector3 trans;
			MATH::CRotation rot;

			LONG i;
			for (i = 0; i < (LONG)posData.size(); i+=3)
			{
				Null nullObj;
				inObj.AddNull(L"",nullObj);

				trans.Set(posData[i], posData[i+1], posData[i+2]);
				nullObj.PutLocalTranslation(trans);

				trans.Set(normData[i], normData[i+1], normData[i+2]);
				rot.SetFromXYZAxes( trans, trans, trans );
				nullObj.PutLocalRotation(rot);
			}
		}

		float GetNormalizedRandom(){return float(rand())/RAND_MAX;}

		LONG RandInRange(LONG in_bound)
		{
			float fRand = GetNormalizedRandom()*0.99999f;
			return (LONG)(in_bound*fRand);
		}

		void main()
		{
			Application app;
			Model root = app.GetActiveSceneRoot();

			X3DObject NURBSSphereObj;
			root.AddGeometry( L"Sphere", L"NurbsSurface", L"", NURBSSphereObj );
			NurbsSurfaceMesh NURBSSphereGeom( NURBSSphereObj.GetActivePrimitive().GetGeometry() );

			LONG nbPtLocators = 50;

			std::vector<LONG> subSurfaceIndexArray;
			std::vector<float> normalizedUVArray;

			subSurfaceIndexArray.resize(nbPtLocators);
			normalizedUVArray.resize(nbPtLocators*2);

			LONG i;
			for(i = 0; i < nbPtLocators; i++)
			{
				subSurfaceIndexArray[i] = 0;
				normalizedUVArray[i*2] = NormalizedRand();
				normalizedUVArray[i*2+1] = NormalizedRand();
			}
			PointLocatorData randomPointLocators = NURBSSphereGeom.ConstructPointLocators(nbPtLocators, &subSurfaceIndexArray.front(), &normalizedUVArray.front());

			CreateNullsAtPointLocations(NURBSSphereObj, randomPointLocators);
		}
	\endcode
	*/
	CStatus GetNormalizedUVArray
	(
		const PointLocatorData& in_ptLocators,
		LONG in_nbPointLocatorsIndices, //-1 if all
		const LONG* in_pPointLocatorsIndices, // 0 if all
		float* out_pNormalizedUVs
	) const;

	/*! Builds a PointLocatorData from NurbsSurfaceMesh-specific topological information. Subsurface
	indices and normalized UV surface parameters are required in order to define each point locator.
	The UV array should contain two values for each specified subsurface index.

	Notice that the returned point locators can be evaluated on any %NurbsSurfaceMesh instance having
	the same topology.

	\param in_nbPointLocators Number of points locators to be constructed
	\param in_pSubSurfaceIndices Subsurface indices (size must be in_nbPoints)
	\param in_pNormalizedUVs Normalized UV coordinates on the subsurface (size must be in_nbPoints*2)

	\return A new PointLocatorData object (.IsValid() == false if failed)

	\sa NurbsSurfaceMesh::GetNormalizedUVArray, NurbsSurfaceMesh::GetSubSurfaceIndexArray,
		Geometry::GetSurfacePointLocatorsFromPoints, PolygonMesh::ConstructPointLocators
	\since 5.0
	*/
	PointLocatorData ConstructPointLocators
	(
		LONG			in_nbPointLocators,
		const LONG*		in_pSubSurfaceIndices,
		const float*	in_pNormalizedUVs
	) const;

	private:
	NurbsSurfaceMesh * operator&() const;
	NurbsSurfaceMesh * operator&();
};

};

#endif // __XSINURBSSURFACEMESH_H__
