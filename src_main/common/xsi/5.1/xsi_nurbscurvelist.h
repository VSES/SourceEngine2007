//*****************************************************************************
/*!
   \file xsi_nurbscurvelist.h
   \brief NurbsCurveList class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSINURBSCURVELIST_H__
#define __XSINURBSCURVELIST_H__

#include <xsi_geometry.h>
#include <xsi_nurbsdata.h>

namespace XSI {

class CControlPointRefArray;
class CNurbsCurveRefArray;
class NurbsCurve;
class CKnotArray;

//*****************************************************************************
/*! \class NurbsCurveList xsi_nurbscurvelist.h
	\brief A NurbsCurveList is a kind of Geometry and is a collection of NurbsCurve objects.

	We can navigate the NurbsCurveList using the generic terms such as Points or we can choose
	to use navigate using terms that are specific to the NurbsCurveList such as NurbsCurves,
	ControlPoints. There are some functions that do not fit with a generic terms such as Knots,
	which are specific to NURBS, this is why there are specific terms as well as generic terms.

	A nurbs curve is defined as follows (surface can be defined similarly in 2 dimensions degree):
	k, order: k+1, Number of Control Points: n, Knot sequence = t(0), t(1)... t(k)... t(n)... t(n+k+1)
	Where the knot sequence is non-decreasing which means t(0) <= t(1) <= ..... <= t(n)... <= t(n+k+1)

	The above is a general definition for a nurbs curve and IGES standard supports this definition.
	However in most industrial design systems, people put some constraints on the knot sequence: for
	a degree k curve, the first k+1 knots and the last k+1 knots are equal, e.g. t(0) = t(1) = ... =
	t(k) and t(n+1) = t(n+2) =... = t(n+k+1). This ensures the curve passes the start ControlPoint and
	end ControlPoint which is a nice property to control the final shape of nurbs curve. For now we only
	support this constrained version of IGES format as we expect most design systems are using this format.

	\sa Primitive::GetGeometry, X3DObject::AddGeometry

	\eg
	\code
		using namespace XSI;

		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject myArc;
		root.AddGeometry( L"Arc", L"NurbsCurve", L"", myArc );

		NurbsCurveList curvelist( myArc.GetActivePrimitive().GetGeometry() );
	\endcode
*/
//*****************************************************************************

class SICPPSDKDECL NurbsCurveList : public Geometry
{
public:
	/*! Default constructor. */
	NurbsCurveList();

	/*! Default destructor. */
	~NurbsCurveList();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	NurbsCurveList(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	NurbsCurveList(const NurbsCurveList& in_obj);

	/*! Copy constructor.
	\param in_obj Geometry object.
	*/
	NurbsCurveList(const Geometry& in_obj);

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
	NurbsCurveList& operator=(const NurbsCurveList& in_obj);

	/*! Creates a Polygon object from a Geometry object. The newly created
	object is set to empty if the input Geometry object is not compatible.
	\param in_geom Source geometry
	\return The new NurbsCurveList object.
	*/
	NurbsCurveList& operator=(const Geometry& in_geom);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new NurbsCurveList object.
	*/
	NurbsCurveList& operator=(const CRef& in_ref);

	/*! Returns true if the curve list is closed or false otherwise.
	\retval out_bClosed return true if the curve list is closed or false otherwise.
	\return CStatus::OK success
	\return CStatus::Fail other failure
	*/
	CStatus GetClosed(bool& out_bClosed) const;

	/*! Returns an array of all NurbsCurve objects on this NurbsCurveList object.
	\return Array of NurbsCurve objects.
	*/
	CNurbsCurveRefArray GetCurves()const;

	/*! Returns an array of all ControlPoint objects on this NurbsCurveList object.
	\return A ControlPoint array.
	*/
	CControlPointRefArray GetControlPoints()const;

	/*! Returns an array of all ControlPoint objects on this NurbsCurveList object. This
	corresponds to the sum of the length of the NurbsCurve composing this NurbsCurveList.
	\retval out_dLength returns the length of the NurbsCurveList
	\return CStatus::OK success
	\return CStatus::Fail other failure
	*/
	CStatus GetLength(double& out_dLength)const;

	/*! Returns the curve index, position of the point on it, U value and distance from the given position.
	\param in_vPosition A position expressed in the NurbsCurveList object frame of reference.
	\retval out_lCurveIndex The curve index to which the position is the closest.
	\retval out_dSquaredDistance The squared distance of the specified position to the curve point.
	\retval out_dUValue The corresponding UValue on the NurbsCurve.
	\retval out_vPosition The actual curve point position at U value.
	\return CStatus::OK success
	\return CStatus::Fail other failure
	*/
	CStatus GetClosestCurvePosition
	(
		MATH::CVector3&		in_vPosition,
		LONG&					out_lCurveIndex,
		double&					out_dSquaredDistance,
		double&					out_dUValue,
		MATH::CVector3&			out_vPosition
	)const;

	/*! Returns a complete data description of this nurbs curve list.
	\param in_siNurbsFormat Specifies how the data is formatted.
	\retval out_curves The entire curve data description.
	\return CStatus::OK success
	\return CStatus::Fail other failure
	*/
	CStatus Get
	(
		siNurbsFormat				in_siNurbsFormat,
		CNurbsCurveDataArray&		out_curves
	)const;

	/*! Sets the current NurbsCurveList geometry, from a complete data description of a nurbs
	curve list. This is only available from compiled operators. If you use it on a object with
	clusters and you change the topology the burden of updating the clusters is on the user.
	\param in_curves The data for the new representation of this NurbsCurveList.
	\param in_siNurbsFormat Specifies how the data is formatted.
	\return CStatus::OK success
	\return CStatus::AccessDenied function used outside of a plug-in operator context.
	*/
	CStatus Set
	(
		const CNurbsCurveDataArray&	in_curves,
		siNurbsFormat				in_siNurbsFormat = siSINurbs
	);

	/*! Adds a curve to this NurbsCurveList object.
	\param in_curve The data description for the new NurbsCurve.
	\param in_siNurbsFormat Specifies how the data is formatted.
	\retval out_NurbsCurve The new NurbsCurve.
	\return CStatus::OK success
	\return CStatus::AccessDenied function used outside of a plug-in operator context.
	*/
	CStatus AddCurve
	(
		const CNurbsCurveData&		in_curve,
		siNurbsFormat				in_siNurbsFormat,
		NurbsCurve&					out_NurbsCurve
	);

	private:
	NurbsCurveList * operator&() const;
	NurbsCurveList * operator&();
};

};

#endif // __XSINURBSCURVELIST_H__
