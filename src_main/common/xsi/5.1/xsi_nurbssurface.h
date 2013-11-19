//*****************************************************************************
/*!
   \file xsi_nurbssurface.h
   \brief NurbsSurface and CNurbsSurfaceRefArray class declarations.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSINURBSSURFACE_H__
#define __XSINURBSSURFACE_H__

#include <xsi_facet.h>
#include <xsi_nurbsdata.h>

namespace XSI {

class CKnotArray;
class CControlPointRefArray;
class CNurbsSampleRefArray;


//*****************************************************************************
/*! \class NurbsSurface xsi_nurbssurface.h
	\brief The NurbsSurface object gives access to the X3DObject's nurbs surface Geometry.

	\sa PolygonMesh::GetPolygons, CPolygonFaceArray

	\eg
	\code
		using namespace XSI;

		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject mySphere;
		root.AddGeometry( L"Sphere", L"NurbsSurface", L"", mySphere );

		NurbsSurfaceMesh mySurfaceMesh(mySphere.GetActivePrimitive().GetGeometry());

		CNurbsSurfaceDataArray surfaces(2);

		NurbsSurface(mySurfaceMesh.GetSurfaces().GetItem(0)).Get(siSINurbs,surfaces[0]);

		X3DObject myCone;
		root.AddGeometry( L"Cone",L"NurbsSurface",L"", myCone );

		mySurfaceMesh = myCone.GetActivePrimitive().GetGeometry();

		NurbsSurface(mySurfaceMesh.GetSurfaces().GetItem(0)).Get(siSINurbs,surfaces[1]);

		X3DObject myMultiSurfaceMesh;
		root.AddNurbsSurfaceMesh(surfaces,siSINurbs,L"",myMultiSurfaceMesh);
	\endcode
*/
//*****************************************************************************

class SICPPSDKDECL NurbsSurface : public Facet
{
public:
	/*! Default constructor. */
	NurbsSurface();

	/*! Default destructor. */
	~NurbsSurface();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	NurbsSurface(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	NurbsSurface(const NurbsSurface& in_obj);

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
	\return The new PolygonFace object.
	*/
	NurbsSurface& operator=(const NurbsSurface& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new PolygonFace object.
	*/
	NurbsSurface& operator=(const CRef& in_ref);

	/*! Returns an array of all Knot values on this NurbsSurface object in a given direction.
	\param in_siNurbsDirection Direction in which we want the knot values.
	\return Array of all knots.
	*/
	CKnotArray GetKnots
	(
		siNurbsDirection	in_siNurbsDirection
	)const;

	/*! Returns an array of all ControlPoint objects on this NurbsSurface object.
	\return A ControlPoint array.
	*/
	CControlPointRefArray GetControlPoints()const;

	/*! Returns an array of all NurbsSample objects on this NurbsSurface object.
	\return A NurbsSample reference array.
	*/
	CNurbsSampleRefArray GetNurbsSamples()const;

	/*! Returns a complete data description of a nurbs surface.
	\param in_siNurbsFormat Specifies how the data is formatted.
	\retval out_surface The entire surface data description.
	\return CStatus::OK success
	\return CStatus::Fail other failure
	*/
	CStatus Get
	(
		siNurbsFormat			in_siNurbsFormat,
		CNurbsSurfaceData&		out_surface
	)const;


	/*! Creates a new geometry for this NurbsSurface from a complete data description of a nurbs surface.
	This is only available from compiled operators.

	\note If you use it on a object with some clusters and you change the topology the burden of updating
		the clusters is on the user.

	\warning UV ordering is different for the C++ API than it is for scripting. In scripting, a
		control point array is ordered U-V like so:
		<br><br>
		<tt> (0,0)...(0,cntV-1), (1,0)....(1,cntV-1),(cntU-1,0)...(cntU-1,cntV-1) </tt>
		<br><br>
		However, the order for the C++ API is V-U:
		<br><br>
		<tt> (0,0)...(cntU-1,0), (0,1)....(cntU-1,1)...(0,cntV-1)...(cntU-1,cntV-1) </tt>
		<br><br>
		Since NurbsSurface::Set expects the U-V order, you must reorder the array before passing it via the
		\c in_siNurbsFormat argument. See \xl UV_Ordering for a sample conversion function.

	\param in_surface Specify the new data description for this NurbsSurface.
	\param in_siNurbsFormat Specifies how the data is formatted.
	\return CStatus::OK success
	\return CStatus::AccessDenied function used outside of a plug-in operator context.
	*/
	CStatus Set
	(
		const CNurbsSurfaceData&	in_surface,
		siNurbsFormat				in_siNurbsFormat = siSINurbs
	);

	/*! Returns the Position , U tangent, V Tangent, and Normal at a given UV value.
	\param  in_dUValue The U Value at which we want to evaluate the surface.
	\param  in_dVValue The V Value at which we want to evaluate the surface.
	\retval	out_vPosition Position at the given UV.
	\retval	out_vUTangent Tangent in U at the given UV.
	\retval	out_vVTangent Tangent in V at the given UV.
	\retval	out_vNormal   Normal at the given UV.
	\return CStatus::OK success
	\return CStatus::Fail other failure
	*/
	CStatus EvaluatePosition
	(
		double					in_dUValue,
		double					in_dVValue,
		MATH::CVector3&			out_vPosition,
		MATH::CVector3&			out_vUTangent,
		MATH::CVector3&			out_vVTangent,
		MATH::CVector3&			out_vNormal
	)const;

	/*! Returns the Position, U tangent, V Tangent,Normal at a given normalized UV value.
	\param  in_dUNormalizedValue The normalized U Value(0.0 to 1.0) at which we want to evaluate the surface.
	\param  in_dVNormalizedValue The normalized V Value(0.0 to 1.0) at which we want to evaluate the surface.
	\retval	out_vPosition Position at the given UV.
	\retval	out_vUTangent Tangent in U at the given UV.
	\retval	out_vVTangent Tangent in V at the given UV.
	\retval	out_vNormal   Normal at the given UV.
	\return CStatus::OK success
	\return CStatus::Fail other failure
	*/
	CStatus EvaluateNormalizedPosition
	(
		double					in_dUNormalizedValue,
		double					in_dVNormalizedValue,
		MATH::CVector3&			out_vPosition,
		MATH::CVector3&			out_vUTangent,
		MATH::CVector3&			out_vVTangent,
		MATH::CVector3&			out_vNormal
	)const;

	/*! Returns the Normalized equivalent of UV values.
	\param  in_dUValue The U Value from which we want the normalized U Value.
	\param  in_dVValue The V Value from which we want the normalized V Value.
	\retval	out_dNormalizedUValue The normalized U Value(0.0 to 1.0).
	\retval	out_dNormalizedVValue The normalized V Value(0.0 to 1.0).
	\return CStatus::OK success
	\return CStatus::Fail other failure
	*/
	CStatus GetNormalizedUVFromUV
	(
		double					in_dUValue,
		double					in_dVValue,
		double&					out_dNormalizedUValue,
		double&					out_dNormalizedVValue
	)const;

	/*! Returns the UV value equivalent to the specified normalized UV values.
	\param  in_dNormalizedUValue The normalized UValue(0.0 to 1.0) from which we want the actual U Value.
	\param  in_dNormalizedVValue The normalized VValue(0.0 to 1.0) from which we want the actual V Value.
	\retval	out_dUValue The actual U value.
	\retval	out_dVValue The actual V value.
	\return CStatus::OK success
	\return CStatus::Fail other failure
	*/
	CStatus GetUVFromNormalizedUV
	(
		double					in_dNormalizedUValue,
		double					in_dNormalizedVValue,
		double&					out_dUValue,
		double&					out_dVValue
	)const;

	/*! Returns the number of trim curves on this nurbs surface.
	\return The number of trim curves.
	*/
	LONG GetTrimCount()const;

	/*! Returns a complete data description of a nurbs surface trim. A trim is a NurbsCurveList that is
	projected on a nurbs surface.
	\param  in_lTrimIndex The index of the trim curve for which we want the description.
	\param	in_siNurbsFormat Specifies how the data is formatted.
	\retval  out_trimcurve The entire data description of the trim curve.
	\return CStatus::OK success
	\return CStatus::Fail other failure
	*/
	CStatus GetTrim
	(
		LONG									in_lTrimIndex,
		siNurbsFormat							in_siNurbsFormat,
		CTrimCurveData&							out_trimcurve
	)const;

	private:
	NurbsSurface * operator&() const;
	NurbsSurface * operator&();
};

//*****************************************************************************
/*! \class CNurbsSurfaceRefArray xsi_nurbssurface.h
	\brief An array of NurbsSurface objects representing the surface in a nurbs surface mesh.

	\warning This specialized array is returned by NurbsSurfaceMesh::GetSurfaces, it is not meant to be
		created and modified in user-defined functions. If you want to add and remove arbitrary items
		to a collection, you must use a CRefArray instead.

	\sa NurbsSurfaceMesh::GetSurfaces

	\eg
	\code
		using namespace XSI;

		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject mySphere;
		root.AddGeometry( L"Sphere", L"NurbsSurface", L"", mySphere );

		NurbsSurfaceMesh mySurfaceMesh(mySphere.GetActivePrimitive().GetGeometry());

		CNurbsSurfaceRefArray surfaces = mySurfaceMesh.GetSurfaces();
	\endcode
*/
//*****************************************************************************
class SICPPSDKDECL CNurbsSurfaceRefArray : public CFacetRefArray
{
	public:
	/* Default constructor */
	CNurbsSurfaceRefArray();

	/*! Copy constructor.
	\param in_array constant CNurbsSurfaceRefArray reference object.
	*/
	CNurbsSurfaceRefArray( const CNurbsSurfaceRefArray& in_array );

	/*! Returns a NurbsSurface object at a specified index in the array.
	\param in_index The index of the NurbsSurface.
	\return A NurbsSurface reference object.
	*/
	CRef GetItem( LONG in_index ) const;

	/*! Returns the NurbsSurface object in the array matching the specified name.
	\param in_name The name of the NurbsSurface.
	\return A NurbsSurface reference object.
	*/
	CRef GetItem( const CString& in_name ) const;

	/*! Returns the number of NurbsSurface objects in the array.
	\return The number of NurbsSurface objects.
	*/
	LONG GetCount() const;

	/*! Returns true if this array is a valid object or false otherwise.
	\return true if valid, false otherwise.
	*/
	bool IsValid() const;

	private:
	void* m_ptr;
};

};

#endif // __XSINURBSSURFACE_H__
