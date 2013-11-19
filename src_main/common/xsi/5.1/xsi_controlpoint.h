//*****************************************************************************
/*!
   \file xsi_controlpoint.h
   \brief ControlPoint and CControlPointRefArray class declarations.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSICONTROLPOINT_H__
#define __XSICONTROLPOINT_H__

#include <xsi_point.h>

namespace XSI {

class CKnotArray;
class CControlPointRefArray;
class CNurbsSampleRefArray;


//*****************************************************************************
/*! \class ControlPoint xsi_controlpoint.h
	\brief The ControlPoint of a NurbsSurface or a NurbsCurve.

	\sa NurbsCurve""GetControlPoints, NurbsSurface::GetControlPoints, CControlPointRefArray

	\eg Shows how to set create a NurbsCurve without knots.
	\code
		using namespace XSI;
		using namespace MATH;
		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject arc;
		root.AddGeometry(L"Arc",L"NurbsCurve",L"",arc);

		NurbsCurveList curveList = arc.GetActivePrimitive().GetGeometry();

		NurbsCurve curve = curveList.GetCurves().GetItem(0);

		CControlPointRefArray cpArray = curve.GetControlPoints();

		CNurbsCurveData curveData;
		curveData.m_aControlPoints.Resize(cpArray.GetCount());

		for(LONG i = 0;i < cpArray.GetCount();++i)
		{
			ControlPoint cp = cpArray.GetItem(i);
			curveData.m_aControlPoints[i].Set(	cp.GetX(),
												cp.GetY(),
												cp.GetZ(),
												cp.GetW());
		}

		X3DObject arcCopyWithoutKnots;
		root.AddNurbsCurve(curveData,siSINurbs,L"",arcCopyWithoutKnots);
	\endcode
*/
//*****************************************************************************

class SICPPSDKDECL ControlPoint : public Point
{
public:
	/*! Default constructor. */
	ControlPoint();

	/*! Default destructor. */
	~ControlPoint();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	ControlPoint(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	ControlPoint(const ControlPoint& in_obj);

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
	ControlPoint& operator=(const ControlPoint& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new PolygonFace object.
	*/
	ControlPoint& operator=(const CRef& in_ref);


	/*! Returns the X value of this ControlPoint.
	\return The X value or \c DBL_MAX if it fails.
	*/
	double GetX()const;

	/*! Sets the X value of this ControlPoint. The value can only be set from within a compiled operator or
	if there's no operator connected to this geometry.
	\param  in_dX The new X value.
	\return CStatus::OK success
	\return CStatus::AccessDenied function used outside of a plug-in operator context.
	*/
	CStatus PutX(double in_dX);

	/*! Returns the Y value of this ControlPoint.
	\return The Y value or \c DBL_MAX if it fails.
	*/
	double GetY()const;

	/*! Sets the Y value of this ControlPoint. The value can only be set from within a compiled operator or
	if there's no operator connected to this geometry.
	\param  in_dY The new Y value.
	\return CStatus::OK success
	\return CStatus::AccessDenied function used outside of a plug-in operator context.
	*/
	CStatus PutY(double in_dY);

	/*! Returns the Z value of this ControlPoint.
	\return The Z value or \c DBL_MAX if it fails.
	*/
	double GetZ()const;

	/*! Sets the Z value of this ControlPoint. The value can only be set from within a compiled operator or
	if there's no operator connected to this geometry.
	\param  in_dZ The new Z value.
	\return CStatus::OK success
	\return CStatus::AccessDenied function used outside of a plug-in operator context.
	*/
	CStatus PutZ(double in_dZ);

	/*! Returns the W value of this ControlPoint.
	\return The W value or \c DBL_MAX if it fails.
	*/
	double GetW()const;

	/*! Sets the W value of this ControlPoint. The value can only be set from within a compiled operator or
	if there's no operator connected to this geometry.
	\param  in_dW The new W value.
	\return CStatus::OK success
	\return CStatus::AccessDenied function used outside of a plug-in operator context.
	*/
	CStatus PutW(double in_dW);

	/*! Returns whether the control point is a boundary or not.
	\return True if the control point is a boundary, false if not.
	\since 4.2
	*/
	bool GetIsBoundary()const;

	private:
	ControlPoint* operator&() const;
	ControlPoint* operator&();
};

//*****************************************************************************
/*! \class CControlPointRefArray xsi_controlpoint.h
	\brief An array of ControlPoint object references representing the NurbsCurve or NurbsSurface control points.

	\warning This specialized array is returned by NurbsCurve::GetControlPoints and NurbsSurface::GetControlPoints,
		it is not meant to be created and modified in user-defined functions. If you want to add and remove arbitrary
		items to a collection, you must use a CRefArray instead.

	\sa NurbsCurve::GetControlPoints, NurbsSurface::GetControlPoints, ControlPoint, CNurbsCurveData, CNurbsSurfaceData

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

		CNurbsCurveData curveData;
		cpArray.GetArray(curveData.m_aControlPoints);

		curveData.m_bClosed = true;

		X3DObject circleCopyWithoutKnots;
		root.AddNurbsCurve(curveData,siSINurbs,L"",circleCopyWithoutKnots);
	\endcode
*/
//*****************************************************************************
class SICPPSDKDECL CControlPointRefArray :  public CPointRefArray
{
public:
	/*! Default constructor. */
	CControlPointRefArray();

	/*! Default destructor. */
	~CControlPointRefArray();

	/*! Copy constructor.
	\param in_array constant CControlPointRefArray reference object.
	*/
	CControlPointRefArray( const CControlPointRefArray& in_array );

	/*! Returns the NurbsCurve object at a specified index in the array.
	\param in_index The index of the NurbsCurve.
	\return A reference to the NurbsCurve object.
	*/
	CRef GetItem( LONG in_index ) const;

	/*! Returns the NurbsCurve object in the array matching the specified name.
	\param in_name The name of the NurbsCurve.
	\return A reference to the NurbsCurve object.
	*/
	CRef GetItem( const CString& in_name ) const;

	/*! Returns the number of NurbsSurface objects in the array.
	\return The number of NurbsSurface objects.
	*/
	LONG GetCount() const;

	/*! Returns whether this array is a valid object.
	\return true if valid, false otherwise.
	*/
	bool IsValid() const;

	/*! Returns an array of control points for this nurbs component.
	\retval	out_vPositions An array of positions for the control points.
	\return CStatus::OK success.
	\return CStatus::Fail other failure.
	*/
	CStatus GetArray(MATH::CVector4Array& out_vPositions)const;

	/*! Sets an array of control points for this nurbs component. This function can only be used within a compiled
	operator or if no operators are attach to this geometry.
	\param  in_vPositions The new positions for the control points.
	\return CStatus::OK success.
	\return CStatus::CStatus::AccessDenied function used outside of a plug-in operator context.
	*/
	CStatus PutArray(MATH::CVector4Array& in_vPositions);

	/*! Returns the size of the ControlPoint reference array in a particular direction. For NurbsCurves use ::siUDirection.
	\param  in_siDirection Direction of dimension required (one of the ::siNurbsDirection values).
	\retval  out_lDimension The actual dimension.
	\return CStatus::OK success.
	\return CStatus::InvalidArgument if the direction is invalid for this type of component
		(for example, ::siVDirection for a NurbsCurve).
	*/
	CStatus Dimension
	(
		siNurbsDirection	in_siDirection,
		LONG&				out_lDimension
	)const;

	private:
	void* m_ptr;
};

};

#endif // __XSICONTROLPOINT_H__
