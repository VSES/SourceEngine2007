//*****************************************************************************
/*!
   \file xsi_nurbsdata.h
   \brief Contains the following Nurbs data structures and class declarations:
   			\li CNurbsCurveData struct and CNurbsCurveDataArray class
   			\li CTrimCurveData struct and CTrimCurveDataArray class
   			\li CNurbsSurfaceData struct and CNurbsCurveDataArray class

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSINURBSDATA_H__
#define __XSINURBSDATA_H__

#include <xsi_decl.h>
#include <xsi_vector4.h>
#include <xsi_doublearray.h>
#include <xsi_longarray.h>

namespace XSI {

class CStatus;

//*************************************************************************
/*! \struct CNurbsCurveData xsi_nurbsdata.h
	\brief This structure represents a data description for a NurbsCurve.

	\sa NurbsCurve, CNurbsCurveDataArray, NurbsCurve::Set, NurbsCurve::Get,
		X3DObject::AddNurbsCurve
 */
//*************************************************************************
struct CNurbsCurveData
{
	/*! Default Constructor. */
	CNurbsCurveData()
		: m_bClosed(false),
		  m_lDegree(3),
		  m_siParameterization(siNonUniformParameterization){}

	/*! Equality operator.
	\param in_curveData CNurbsCurveData to compare with.
	\return true if they are equal; false if they are not equal.
	*/
	bool operator==(const CNurbsCurveData& in_curveData)const
	{
		return ((m_aControlPoints == in_curveData.m_aControlPoints) &&
				(m_aKnots == in_curveData.m_aKnots) &&
				(m_bClosed == in_curveData.m_bClosed) &&
				(m_lDegree == in_curveData.m_lDegree) &&
				(m_siParameterization == in_curveData.m_siParameterization));
	}

	/*! Inequality operator.
	\param in_curveData CNurbsCurveData to compare with.
	\return true if they are not equal; false if they are equal.
	*/
	bool operator!=(const CNurbsCurveData& in_curveData)const
	{
		return !operator==(in_curveData);
	}

	/*! Array of ControlPoint values. */
	MATH::CVector4Array	           m_aControlPoints;

	/*! Array of Knot values.*/
	CDoubleArray               m_aKnots;
	/*! Specifies if the curve is closed.*/
	bool		               m_bClosed;
	/*! Degree of the nurbs curve.*/
	LONG		               m_lDegree;

	/*! Parameterization factor of the nurbs curve.*/
	siKnotParameterization     m_siParameterization;
};

//*************************************************************************
/*! \class CNurbsCurveDataArray xsi_nurbsdata.h
	\brief The CNurbsCurveDataArray is an uni-dimensional array of CNurbsCurveData.

	The array is zero-based, which means that the first item index is 0 and the larger index
	is one less that the number of elements contained in the array.

	\sa CNurbsCurveData, X3DObject::AddNurbsCurveList, NurbsCurveList::Get,
		NurbsCurveList::Set
*/
//*************************************************************************
class SICPPSDKDECL CNurbsCurveDataArray
{
public:
	/*! Constructs a CNurbsCurveDataArray and optionally initializes the array
	to a known size.
	\param in_size Size of array, defaults to 0.
	*/
	CNurbsCurveDataArray( LONG in_size=0);

	/*! Constructs a CNurbsCurveDataArray object from another
	CNurbsCurveDataArray object.
	\param in_array constant CNurbsCurveDataArray object.
	*/
	CNurbsCurveDataArray(const CNurbsCurveDataArray& in_array);

	/*! Default destructor. */
	~CNurbsCurveDataArray();

	/*! Assignment operator.
	\param in_array constant class object.
	\return A reference to this array.
	 */
	CNurbsCurveDataArray& operator=( const CNurbsCurveDataArray& in_array );

	/*! Returns the number of items in this CNurbsCurveDataArray
	\return The number of items in the array.
	*/
	LONG GetCount() const;

	/*! Adds a CNurbsCurveData at the end of this array.
	\param in_item New item to be added at the end of the array.
	\return CStatus::OK success
	*/
	CStatus Add( const CNurbsCurveData& in_item );

	/*! Erases all elements contained in the array.
	\return CStatus::OK success
	*/
	CStatus Clear();

	/*! Reallocates memory for the array, preserves its contents if the new size is
	larger than existing size.
	\param in_size New size of the array.
	\return CStatus::OK success
	\return CStatus::InvalidArgument in_size < 0
	*/
	CStatus Resize( LONG in_size);

	/*! Accessor to elements at a given index. This function can only be called by constant
	objects, and the returned value is read-only.
	\param in_index index in this zero-based array.  The index must be greater or equal to zero
		and smaller than the number of element in the array, otherwise the results are unpredictable.
	\return A read-only reference to the indexed item.
	*/
	const CNurbsCurveData& operator[]( LONG in_index ) const;

	/*! Accessor to elements at a given index.
	\param in_index index in this zero-based array.  The index must be greater or equal to zero
		and smaller than the number of element in the array, otherwise the results are unpredictable.
	\return A reference to the indexed item.
	*/
	CNurbsCurveData& operator[]( LONG in_index );

	/*! Equality operator.
	\param in_array CNurbsCurveDataArray to compare with.
	\return true if in_array contains the same number of elements as this array and that all members
		of in_array are equal to the corresponding one contained in this array.
	*/
	bool operator==(const CNurbsCurveDataArray& in_array) const;

	/*! Inequality
	\param in_array
	\return true if one member of in_array is different of the corresponding members in this array or
		if the arrays are not of the same size.
	*/
	bool operator!=(const CNurbsCurveDataArray& in_array) const;

private:
	void* m_ptr;
};

//*************************************************************************
/*! \struct CTrimCurveData xsi_nurbsdata.h
	\brief This structure represents a data description for a TrimCurve.

	\sa CTrimCurveDataArray,NurbsSurface::GetTrim,
 */
//*************************************************************************
struct CTrimCurveData
{
	/*! Default Constructor. */
	CTrimCurveData()
		: m_bIsBoundary(false),
		  m_lProjectionType(0){}

	/*! Equality operator.
	\param in_trimData CTrimCurveData to compare with.
	\return true if they are equal; false if they are not equal.
	*/
	bool operator==(const CTrimCurveData& in_trimData)const
	{
		return ((m_bIsBoundary == in_trimData.m_bIsBoundary) &&
				(m_lProjectionType == in_trimData.m_lProjectionType) &&
				(m_aCurvesData == in_trimData.m_aCurvesData));
	}

	/*! Inequality operator.
	\param in_trimData CTrimCurveData to compare with.
	\return true if they are not equal; false if they are equal.
	*/
	bool operator!=(const CTrimCurveData& in_trimData)const
	{
		return ! operator==(in_trimData);
	}

	/*! Specifies whether the trim is a boundary */
	bool			m_bIsBoundary;
	/*! Projection type of this trim 0(UVSpace), 1(World space) */
	LONG			m_lProjectionType;

	/*! Curves making up the trim curve*/
	CNurbsCurveDataArray   m_aCurvesData;
};

//*************************************************************************
/*! \class CTrimCurveDataArray xsi_nurbsdata.h
	\brief The CTrimCurveDataArray is an uni-dimensional array of CTrimCurveData.

	The array is zero-based, which means that the first item index is 0 and the larger index is
	one less that the number of elements contained in the array.

	\sa CTrimCurveData, NurbsSurfaceMesh::AddSurfaceWithTrim
*/
//*************************************************************************
class SICPPSDKDECL CTrimCurveDataArray
{
public:
	/*! Constructs a CTrimCurveDataArray and optionally initializes the array
	to a known size.
	\param in_size Size of array, defaults to 0.
	*/
	CTrimCurveDataArray( LONG in_size=0);

	/*! Constructs a CTrimCurveDataArray object from another CTrimCurveDataArray object.
	\param in_array constant CTrimCurveDataArray object.
	*/
	CTrimCurveDataArray(const CTrimCurveDataArray& in_array);

	/*! Default destructor. */
	~CTrimCurveDataArray();

	/*! Assignment operator.
	\param in_array constant class object.
	\return A reference to this array.
	 */
	CTrimCurveDataArray& operator=( const CTrimCurveDataArray& in_array );

	/*! Returns the number of items in this CTrimCurveDataArray
	\return The number of items in the array.
	*/
	LONG GetCount() const;

	/*! Adds a CTrimCurveData at the end of this array.
	\param in_item New item to be added at the end of the array.
	\return CStatus::OK success
	*/
	CStatus Add( const CTrimCurveData& in_item );

	/*! Erases all elements contained in the array.
	\return CStatus::OK success
	*/
	CStatus Clear();

	/*! Reallocates memory for the array, preserves its contents if the new size is
	larger than existing size.
	\param in_size New size of the array.
	\return CStatus::OK success
	\return CStatus::InvalidArgument in_size < 0
	*/
	CStatus Resize( LONG in_size);

	/*! Accessor to elements at a given index. This function can only be called by constant objects,
	and the returned value is read-only.
	\param in_index index in this zero-based array.  The index must be greater or equal to zero
		and smaller than the number of element in the array, otherwise the results are unpredictable.
	\return A read-only reference to the indexed item.
	*/
	const CTrimCurveData& operator[]( LONG in_index ) const;

	/*! Accessor to elements at a given index.
	\param in_index index in this zero-based array.  The index must be greater or equal to zero and
		smaller than the number of element in the array, otherwise the results are unpredictable.
	\return A reference to the indexed item..
	*/
	CTrimCurveData& operator[]( LONG in_index );

	/*! Equality operator.
	\param in_array CTrimCurveDataArray to compare with.
	\return true if in_array contains the same number of elements as this array and if all members of
		in_array are equal to the corresponding one contained in this array.
	*/
	bool operator==(const CTrimCurveDataArray& in_array) const;

	/*! Inequality operator.
	\param in_array
	\return true if one member of in_array is different from the corresponding members in thisarray or
		if the arrays are not of the same size.
	*/
	bool operator!=(const CTrimCurveDataArray& in_array) const;

private:

	void* m_ptr;
};


//*************************************************************************
/*! \struct CNurbsSurfaceData xsi_nurbsdata.h
	\brief This structure represents a data description for a NurbsSurface.

	\sa CNurbsSurfaceDataArray, X3DObject::AddNurbsSurface, NurbsSurface::Get, NurbsSurface::Set
 */
//*************************************************************************
struct CNurbsSurfaceData
{
	/*! Default Constructor. */
	CNurbsSurfaceData()
		:	m_bUClosed(false),
			m_bVClosed(false),
			m_lUDegree(3),
			m_lVDegree(3),
			m_siUParameterization(siNonUniformParameterization),
			m_siVParameterization(siNonUniformParameterization){}

	/*! Equality operator.
	\param in_surfaceData CNurbsSurfaceData to compare with.
	\return true if they are equal; false if they are not equal.
	*/
	bool operator==(const CNurbsSurfaceData& in_surfaceData)const
	{
		return ((m_aControlPoints == in_surfaceData.m_aControlPoints) &&
				(m_lNbUControlPoints == in_surfaceData.m_lNbUControlPoints) &&
				(m_lNbVControlPoints == in_surfaceData.m_lNbVControlPoints) &&
				(m_aUKnots == in_surfaceData.m_aUKnots) &&
				(m_aVKnots == in_surfaceData.m_aVKnots) &&
				(m_bUClosed == in_surfaceData.m_bUClosed) &&
				(m_bVClosed == in_surfaceData.m_bVClosed) &&
				(m_lUDegree == in_surfaceData.m_lUDegree) &&
				(m_lVDegree == in_surfaceData.m_lVDegree) &&
				(m_siUParameterization == in_surfaceData.m_siUParameterization) &&
				(m_siVParameterization == in_surfaceData.m_siVParameterization));
	}

	/*! Inequality operator.
	\param in_surfaceData CNurbsSurfaceData to compare with.
	\return true if they are not equal; false if they are equal.
	*/
	bool operator!=(const CNurbsSurfaceData& in_surfaceData)const
	{
		return ! operator==(in_surfaceData);
	}

	/*! Array of ControlPoint values.
	\note UV ordering is different for the C++ API than it is for scripting. In scripting, a
		control point array is ordered U-V like so:
		<br><br>
		<tt> (0,0)...(0,cntV-1), (1,0)....(1,cntV-1),(cntU-1,0)...(cntU-1,cntV-1) </tt>
		<br><br>
		However, the order for the C++ API is V-U:
		<br><br>
		<tt> (0,0)...(cntU-1,0), (0,1)....(cntU-1,1)...(0,cntV-1)...(cntU-1,cntV-1) </tt>
		<br><br>
		Since functions like NurbsSurface::Set and X3DObject::AddNurbsSurface expect the
		U-V order, you must reorder the array before passing it to these functions. See
		\xl UV_Ordering for a sample conversion function.
	*/
	MATH::CVector4Array	      m_aControlPoints;
	/*! Number of ControlPoints in U*/
	LONG					  m_lNbUControlPoints;
	/*! Number of ControlPoints in V*/
	LONG					  m_lNbVControlPoints;
	/*! U Knot values */
	CDoubleArray              m_aUKnots;
	/*! V Knot values */
	CDoubleArray              m_aVKnots;
	/*! Specify if the surface is closed  in U direction.*/
	bool		              m_bUClosed;
	/*! Specify if the surface is closed  in V direction.*/
	bool		              m_bVClosed;
	/*! Specify if the degree of the nurbs surface in U.*/
	LONG		              m_lUDegree;
	/*! Specify if the degree of the nurbs surface in V.*/
	LONG		              m_lVDegree;
	/*! Parameterization factor of the nurbs surface in U.*/
	siKnotParameterization    m_siUParameterization;
	/*! Parameterization factor of the nurbs surface in V.*/
	siKnotParameterization    m_siVParameterization;
};


//*************************************************************************
/*! \class CNurbsSurfaceDataArray xsi_nurbsdata.h
	\brief The CNurbsSurfaceDataArray is an uni-dimensional array of CNurbsSurfaceData objects.

	The array is zero-based, which means that the first item index is 0 and the larger index is
	one less that the number of elements contained in the array.

	\sa CNurbsSurfaceData, X3DObject::AddNurbsSurfaceMesh, NurbsSurfaceMesh::Get, NurbsSurfaceMesh::Set
*/
//*************************************************************************
class SICPPSDKDECL CNurbsSurfaceDataArray
{
public:
	/*! Constructs a CNurbsSurfaceDataArray and optionally initializes the array to a known size.
	\param in_size Size of array, defaults to 0.
	*/
	CNurbsSurfaceDataArray( LONG in_size=0);

	/*! Constructs a CNurbsSurfaceDataArray object from another CNurbsSurfaceDataArray object.
	\param in_array constant CNurbsSurfaceDataArray object.
	*/
	CNurbsSurfaceDataArray(const CNurbsSurfaceDataArray& in_array);

	/*! Default destructor. */
	~CNurbsSurfaceDataArray();

	/*! Assignment operator.
	\param in_array constant class object.
	\return A reference to this array.
	 */
	CNurbsSurfaceDataArray& operator=( const CNurbsSurfaceDataArray& in_array );

	/*! Returns the number of items in this CNurbsSurfaceDataArray
	\return The number of items in the array.
	*/
	LONG GetCount() const;

	/*! Adds a CNurbsSurfaceData at the end of this array.
	\param in_item New item to be added at the end of the array.
	\return CStatus::OK success
	*/
	CStatus Add( const CNurbsSurfaceData& in_item );

	/*! Erases all elements contained in the array.
	\return CStatus::OK success
	*/
	CStatus Clear();

	/*! Reallocates memory for the array, preserves its contents if the new size is larger than existing size.
	\param in_size New size of the array.
	\return CStatus::OK success
	\return CStatus::InvalidArgument in_size < 0
	*/
	CStatus Resize( LONG in_size);

	/*! Accessor to elements at a given index. This function can only be called by constant objects, and the
	returned value is read-only.
	\param in_index index in this zero-based array.  The index must be greater or equal to zero and smaller
		than the number of element in the array, otherwise the results are unpredictable.
	\return A read-only reference to the indexed item.
	*/
	const CNurbsSurfaceData& operator[]( LONG  in_index ) const;

	/*! Accessor to elements at a given index.
	\param in_index index in this zero-based array.  The index must be greater or equal to zero and smaller
		than the number of element in the array, otherwise the results are unpredictable.
	\return A reference to the indexed item.
	*/
	CNurbsSurfaceData& operator[](LONG in_index );

	/*! Equality operator.
	\param in_array CNurbsSurfaceDataArray to compare with.
	\return true if in_array contains the same number of elements than this array and that all members of
		in_array are equal to the corresponding one contained in this array.
	*/
	bool operator==(const CNurbsSurfaceDataArray& in_array) const;

	/*! Inequality operator
	\param in_array
	\return true if one member of in_array is different of the corresponding members in this array or if
		the arrays are not of the same size.
	*/
	bool operator!=(const CNurbsSurfaceDataArray& in_array) const;

private:

	void* m_ptr;
};



};

#endif
