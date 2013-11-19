//*****************************************************************************
/*!
   \file xsi_knot.h
   \brief CKnotArray class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIKNOT_H__
#define __XSIKNOT_H__

#include <xsi_siobject.h>
#include <xsi_doublearray.h>

namespace XSI {

class CKnotArray;
class CControlPointRefArray;
class CNurbsSampleRefArray;


//*****************************************************************************
/*! \class CKnotArray xsi_knot.h
	\brief An array of knot values representing the NURBS curve or surface knots.

	\warning This specialized array is returned by NurbsCurve::GetKnots and NurbsSurface::GetKnots,
		it is not meant to be created and modified in user-defined functions. If you want to add and
		remove arbitrary items to a collection, you must use a CRefArray instead.

	\sa NurbsCurve::GetKnots, NurbsSurface::GetKnots, CNurbsCurveData, CNurbsSurfaceData

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
class SICPPSDKDECL CKnotArray : public SIObject
{
public:
	/*! Default constructor. */
	CKnotArray();

	/*! Default destructor. */
	~CKnotArray();

	/*! Copy constructor.
	\param in_array constant CKnotArray reference object.
	*/
	CKnotArray( const CKnotArray& in_array );

	/*! Copy constructor.
	\param in_ref constant CKnotArray reference object.
	*/
	CKnotArray( const CRef& in_ref );

	/*! Creates an object from another object. The newly created object is set
	to empty if the input object is not compatible.
	\param in_obj constant class object.
	\return The new CKnotArray object.
	*/
	CKnotArray& operator=(const CKnotArray& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new CKnotArray object.
	*/
	CKnotArray& operator=(const CRef& in_ref);

	/*! Returns true if a given class type is compatible with the underlying reference object.
	\param in_classID Class type.
	\return true if the class is compatible, false otherwise.
 	*/
	bool IsA(siClassID in_classID) const;

	/*! Returns the class type of this array
	\return siClassID Class type of this array.
	*/
	siClassID GetClassID() const;

	/*! Returns the value of a knot at a given index, if the index is invalid or the CKnotArray
	is invalid it will return DBL_MAX instead.
	\param in_index The index of the Knot.
	\return The Knot value.
	\return DBL_MAX if the index is invalid or the CKnotArray is invalid
	*/
	double GetItem( LONG in_index ) const;

	/*! Returns an array object containing the knot values.
	\return Array of knot values.
	*/
	CDoubleArray& GetArray() const;

	/*! Returns the number of Knot objects in the array.
	\return The number of knot objects.
	*/
	LONG GetCount() const;

	/*! Returns true if the parametric domain is closed.
	\retval out_bClosed Return value.
	\return CStatus::OK success
	\return CStatus::Fail other failure
	*/
	CStatus GetClosed(bool& out_bClosed) const;

	/*! Returns the degree.
	\retval out_lDegree returns the degree.
	\return CStatus::OK success
	\return CStatus::Fail other failure
	*/
	CStatus GetDegree(LONG& out_lDegree) const;

	/*! Returns the parameterization.
	\retval out_siParameterization The parameterization.
	\return CStatus::OK success
	\return CStatus::Fail other failure
	*/
	CStatus GetParameterization
	(
		siKnotParameterization& out_siParameterization
	) const;

	/*! Returns the multiplicity at a certain knot value.
	\param in_dKnotValue The knot value for which we want to know the multiplicity.
	\retval out_lMultiplicity The multiplicity for the given knot value.
	\return CStatus::OK success
	\return CStatus::Fail other failure
	*/
	CStatus GetMultiplicity(double in_dKnotValue,LONG& out_lMultiplicity)const;

	private:
	mutable CDoubleArray m_aKnotValues;
};

};

#endif // __XSIKNOT_H__
