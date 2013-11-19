//*****************************************************************************
/*!
   \file xsi_nurbscurve.h
   \brief NurbsCurve and CNurbsCurveRefArray class declarations.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSINURBSCURVE_H__
#define __XSINURBSCURVE_H__

#include <xsi_siobject.h>
#include <xsi_math.h>
#include <xsi_nurbsdata.h>

namespace XSI {

class CKnotArray;
class SubComponent;
class CControlPointRefArray;
class CNurbsSampleRefArray;


//*****************************************************************************
/*! \class NurbsCurve xsi_nurbscurve.h
	\brief The NurbsCurve object gives access to the geometry curves of an X3DObject's primitive.

	\sa NurbsCurveList.GetCurves(), CNurbsCurveRefArray

	\eg
	\code
		using namespace XSI;
		using namespace MATH;
		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject circle;
		root.AddGeometry(L"Circle",L"NurbsCurve",L"",circle);

		NurbsCurveList curveList = circle.GetActivePrimitive().GetGeometry();

		NurbsCurve curve = curveList.GetCurves().GetItem(0);

		CControlPointRefArray cpArray = curve.GetControlPoints();
		CKnotArray knotArray = curve.GetKnots();

		CNurbsCurveData curveData;
		cpArray.GetArray(curveData.m_aControlPoints);

		knotArray.GetClosed(curveData.m_bClosed);

		X3DObject circleCopyWithoutKnots;
		root.AddNurbsCurve(curveData,siSINurbs,L"",circleCopyWithoutKnots);
	\endcode
*/
//*****************************************************************************

class SICPPSDKDECL NurbsCurve : public SIObject
{
public:
	/*! Default constructor. */
	NurbsCurve();

	/*! Default destructor. */
	~NurbsCurve();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	NurbsCurve(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	NurbsCurve(const NurbsCurve& in_obj);

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
	\return The new NurbsCurve object.
	*/
	NurbsCurve& operator=(const NurbsCurve& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new NurbsCurve object.
	*/
	 NurbsCurve& operator=(const CRef& in_ref);

	/*! Returns an array of all Knot values on this NurbsCurve object.
	\return Array of knots.
	*/
	CKnotArray GetKnots()const;

	/*! Returns an array of all ControlPoint objects on this NurbsCurve object.
	\return Array of ControlPoint objects.
	*/
	CControlPointRefArray GetControlPoints()const;


	/*! Returns a complete data description of a nurbs curve.
	\param in_siNurbsFormat Specifies how the data is formatted.
	\retval out_curve The entire curve data description.
	\return CStatus::OK success
	\return CStatus::Fail other failure
	*/
	CStatus Get
	(
		siNurbsFormat			in_siNurbsFormat,
		CNurbsCurveData&		out_curve
	)const;


	/*! Sets the current description from a complete data description of nurbs curve.  This is only
	available from compiled operators; if you use it on an object with some clusters and you change
	the topology the burden of updating the clusters is on the user.
	\param in_curve The new data description for this NurbsCurve.
	\param in_siNurbsFormat Specifies how the data is formatted.
	\return CStatus::OK success
	\return CStatus::AccessDenied function used outside of a plug-in operator context.
	*/
	CStatus Set
	(
		const CNurbsCurveData&	in_curve,
		siNurbsFormat			in_siNurbsFormat = siSINurbs
	);

	/*! Returns the Position, Tangent, Normal and BiNormal at a	given U value.
	\param  in_dUValue The U Value at which we want to evaluate the curve.
	\retval	out_vPosition Position at the given U.
	\retval	out_vTangent Tangent at the given U.
	\retval	out_vNormal  Normal at the given U.
	\retval	out_vBiNormal Bi-Normal at the given U.
	\return CStatus::OK success
	\return CStatus::Fail other failure
	*/
	CStatus EvaluatePosition
	(
		double					in_dUValue,
		MATH::CVector3&			out_vPosition,
		MATH::CVector3&			out_vTangent,
		MATH::CVector3&			out_vNormal,
		MATH::CVector3&			out_vBiNormal
	)const;

	/*! Returns the Position, Tangent, Normal and BiNormal at a	given normalized U value.
	\param  in_dUNormalizedValue The normalized UValue(0.0 to 1.0) at which we want to evaluate the curve.
	\retval	out_vPosition Position at the given U.
	\retval	out_vTangent Tangent at the given U.
	\retval	out_vNormal  Normal at the given U.
	\retval	out_vBiNormal Bi-Normal at the given U.
	\return CStatus::OK success
	\return CStatus::Fail other failure
	*/
	CStatus EvaluateNormalizedPosition
	(
		double					in_dUNormalizedValue,
		MATH::CVector3&			out_vPosition,
		MATH::CVector3&			out_vTangent,
		MATH::CVector3&			out_vNormal,
		MATH::CVector3&			out_vBiNormal
	)const;

	/*! Returns a 3D vector giving the position, tangent in U direction, normal and bi-normal at a given percentage along the curve.
	\param  in_dPercentage The percentange(with values between 0.0 and 100.0 inclusive) of the
		length of the curve at which we want to evaluate the curve.
	\retval	out_vPosition Position at the given percentage.
	\retval	out_vTangent Tangent at the given percentage.
	\retval	out_vNormal  Normal at the given percentage.
	\retval	out_vBiNormal Bi-Normal at the given percentage.
	\return CStatus::OK success
	\return CStatus::Fail other failure
	*/
	CStatus EvaluatePositionFromPercentage
	(
		double					in_dPercentage,
		MATH::CVector3&			out_vPosition,
		MATH::CVector3&			out_vTangent,
		MATH::CVector3&			out_vNormal,
		MATH::CVector3&			out_vBiNormal
	)const;

	/*! Returns the degree of the curve.
	\retval  out_lDegree The degree of the curve.
	\return CStatus::OK success
	\return CStatus::Fail other failure
	*/
	CStatus GetDegree(LONG&	out_lDegree)const;

	/*! Returns the length of the curve.
	\retval  out_dLength The length of the curve.
	\return CStatus::OK success
	\return CStatus::Fail other failure
	*/
	CStatus GetLength(double& out_dLength)const;

	/*! Returns the Normalized equivalent of U value.
	\param  in_dUValue The UValue from which we want the Normalized equivalent.
	\retval  out_dNormalizedUValue The normalized U value equivalent.
	\return CStatus::OK success
	\return CStatus::Fail other failure
	*/
	CStatus GetNormalizedUFromU
	(
		double					in_dUValue,
		double&					out_dNormalizedUValue
	)const;

	/*! Returns the U actual value equivalent to the specified normalized U value.
	\param  in_dNormalizedUValue The normalized UValue(0.0 to 1.0) from which we want the actual value equivalent.
	\retval  out_dUValue The actual U value equivalent.
	\return CStatus::OK success
	\return CStatus::Fail other failure
	*/
	CStatus GetUFromNormalizedU
	(
		double					in_dNormalizedUValue,
		double&					out_dUValue
	)const;

	/*! Returns the U value at a certain percentage along the curve.
	\param  in_dPercentage A percentage (with values between 0.0 and 100.0 inclusive) of the length of the curve.
	\retval  out_dUValue The actual U value equivalent.
	\return CStatus::OK success
	\return CStatus::Fail other failure
	*/
	CStatus GetUFromPercentage
	(
		double					in_dPercentage,
		double&					out_dUValue
	)const;

	/*! Returns the percentage (with values between 0.0 and 100.0 inclusive) along the curve at the given U value.
	\param  in_dUValue The U value at which we want to evaluate the curve.
	\retval  out_dPercentage The actual U value equivalent.
	\return CStatus::OK success
	\return CStatus::Fail other failure
	*/
	CStatus GetPercentageFromU
	(
		double					in_dUValue,
		double&					out_dPercentage
	)const;

	/*! Returns the index of the NurbsCurve in the geometry's NurbsCurveList.
	\return The NurbsCurve index
	\return -1 if the NurbsCurve object is not initialized.
	*/
	LONG GetIndex() const;

	/*! Creates a SubComponent object from this NurbsCurve object.
	\return A SubComponent object.
	*/
	SubComponent GetSubComponent() const;

	/*! Navigates the NurbsCurve objects of the underlying geometry as specifed by the navigation component type. The
	navigation is circular, therefore if you ask for siNextComponent on the last NurbsCurve it returns the first NurbsCurve.
	\param in_siNavigate Type of direction for navigating the Facet objects.
	\return The Point object being visited.
	*/
	NurbsCurve Navigate(siNavigateComponentType in_siNavigate) const;

	private:
	NurbsCurve * operator&() const;
	NurbsCurve * operator&();
};

//*****************************************************************************
/*! \class CNurbsCurveRefArray xsi_nurbscurve.h
	\brief An array of NurbsCurve objects representing the curve in a curve list.

	\warning This specialized array is returned by NurbsCurveList::GetCurves, it is not meant to be
		created and modified in user-defined functions. If you want to add and remove arbitrary items
		to a collection, you must use a CRefArray instead.

	\sa NurbsCurveList::GetCurves

	\eg
	\code
		using namespace XSI;

		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject myArc;
		root.AddGeometry( L"Arc", L"NurbsCurve", L"", myArc );

		NurbsCurveList curvelist( myArc.GetActivePrimitive().GetGeometry() );

		CNurbsCurveRefArray curves(curvelist.GetCurves());
	\endcode
*/
//*****************************************************************************
class SICPPSDKDECL CNurbsCurveRefArray : public CRefArray
{
public:
	/*! Default constructor. */
	CNurbsCurveRefArray();

	/*! Default destructor. */
	~CNurbsCurveRefArray();

	/*! Copy constructor.
	\param in_array constant CNurbsCurveRefArray reference object.
	*/
	CNurbsCurveRefArray( const CNurbsCurveRefArray& in_array );

	/*! Returns a NurbsCurve object at the specified index in the array.
	\param in_index The index of the NurbsCurve.
	\return A reference to a NurbsCurve object.
	*/
	CRef GetItem( LONG in_index ) const;

	/*! Returns the NurbsCurve object in the array matching the specified name.
	\param in_name The name of the NurbsCurve.
	\return A reference to a NurbsCurve object.
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

	/*! Returns an array of indices in the geometry NurbsCurve array
	\return An array of indices.
	*/
	CLongArray GetIndexArray() const;

	/*! Creates a SubComponent object from this NurbsCurve array,
	\return A SubComponent object
	*/
	SubComponent GetSubComponent() const;

	/*! Navigates the NurbsCurve objects of the underlying geometry as specifed by the navigation component
	type. The function returns an array of NurbsCurve objects corresponding to each Point object visited.
	\param in_siNavigate Type of direction for navigating the NurbsCurve objects.
	\return An array of Point objects.
	*/
	CNurbsCurveRefArray Navigate(siNavigateComponentType in_siNavigate) const;

	private:
	void* m_ptr;
};

};

#endif // __XSINURBSCURVE_H__
