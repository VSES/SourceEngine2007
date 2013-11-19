//*****************************************************************************
/*!
   \file xsi_nurbssample.h
   \brief NurbsSample and CNurbsSampleRefArray class declarations.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSINURBSSAMPLE_H__
#define __XSINURBSSAMPLE_H__

#include <xsi_sample.h>

namespace XSI {


//*****************************************************************************
/*! \class NurbsSample xsi_nurbssample.h
	\brief The NurbsSample object gives access to the NurbsSurfaceMesh sample geometry.

	\sa NurbsSurface::GetNurbsSamples, Sample, CNurbsSampleRefArray

	\eg
	\code
		using namespace XSI;

		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject mySphere;
		root.AddGeometry( L"Sphere", L"NurbsSurface", L"", mySphere );

		NurbsSurfaceMesh mySurfaceMesh(mySphere.GetActivePrimitive().GetGeometry());

		CNurbsSurfaceRefArray surfaces = mySurfaceMesh.GetSurfaces();

		NurbsSurface mySurface = surfaces.GetItem(0);

		CNurbsSampleRefArray mySamples = mySurface.GetNurbsSamples();

		double dU,dV;
		for(LONG i = 0;i < mySamples.GetCount(); ++i)
		{
			NurbsSample sample(mySamples[i]);
			sample.GetU(dU);
			sample.GetV(dV);

			app.LogMessage(L"The U and V value for this sample are " +
						CValue(dU).GetAsText() + L" " + CValue(dV).GetAsText());
		}
	\endcode
*/
//*****************************************************************************

class SICPPSDKDECL NurbsSample : public Sample
{
public:
	/*! Default constructor. */
	NurbsSample();

	/*! Default destructor. */
	~NurbsSample();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	NurbsSample(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	NurbsSample(const NurbsSample& in_obj);

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
	\return The new NurbsSample object.
	*/
	NurbsSample& operator=(const NurbsSample& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new NurbsSample object.
	*/
	NurbsSample& operator=(const CRef& in_ref);

	/*! Returns the U position value of this Sample on the NurbsSurface.
	\retval out_dUValue	The U position value.
	\return CStatus::OK success
	\return CStatus::Fail other failure
	*/
	CStatus GetU(double& out_dUValue)const;

	/*! Returns the V position value of this Sample on the NurbsSurface.
	\retval out_dVValue	The V position value.
	\return CStatus::OK success
	\return CStatus::Fail other failure
	*/
	CStatus GetV(double& out_dVValue)const;

	/*! Returns the U value of the sampling coordinate local to the NurbsSurface.
	\retval out_lUSamplingCoordinate	The U sampling coordinate.
	\return CStatus::OK success
	\return CStatus::Fail other failure
	*/
	CStatus GetUSamplingCoordinate(LONG& out_lUSamplingCoordinate)const;

	/*! Returns the V value of the sampling coordinate local to the NurbsSurface.
	\retval out_lVSamplingCoordinate	The V sampling coordinate.
	\return CStatus::OK success
	\return CStatus::Fail other failure
	*/
	CStatus GetVSamplingCoordinate(LONG& out_lVSamplingCoordinate)const;

	/*! Returns the U normalized value of this Sample on the NurbsSurface
	\retval out_dUNormalizedValue The U normalized position value.
	\return CStatus::OK success
	\return CStatus::Fail other failure
	*/
	CStatus GetUNormalized(double& out_dUNormalizedValue)const;

	/*! Returns the V normalized value of this Sample on the NurbsSurface
	\retval out_dVNormalizedValue The V normalized position value.
	\return CStatus::OK success
	\return CStatus::Fail other failure
	*/
	CStatus GetVNormalized(double& out_dVNormalizedValue)const;


	private:
	NurbsSample * operator&() const;
	NurbsSample * operator&();
};

//*****************************************************************************
/*! \class CNurbsSampleRefArray xsi_nurbssample.h
	\brief An array of NurbsSample objects representing the sample points on a NurbsSurface.

	\warning This specialized array is returned by NurbsSurface::GetSamples, it is not meant to be
		created and modified in user-defined functions. If you want to add and remove arbitrary items
		to a collection, you must use a CRefArray instead.

	\sa NurbsSurface::GetSamples

	\eg
	\code
		using namespace XSI;

		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject mySphere;
		root.AddGeometry( L"Sphere", L"NurbsSurface", L"", mySphere );

		NurbsSurfaceMesh mySurfaceMesh(mySphere.GetActivePrimitive().GetGeometry());

		CNurbsSurfaceRefArray surfaces = mySurfaceMesh.GetSurfaces();

		NurbsSurface mySurface = surfaces.GetItem(0);

		CNurbsSampleRefArray mySamples = mySurface.GetNurbsSamples();
	\endcode
*/
//*****************************************************************************
class SICPPSDKDECL CNurbsSampleRefArray : public CSampleRefArray
{
	public:
	/*! Default constructor. */
	CNurbsSampleRefArray();

	/*! Copy constructor.
	\param in_array constant CNurbsSurfaceRefArray reference object.
	*/
	CNurbsSampleRefArray( const CNurbsSampleRefArray& in_array );

	/*! Returns a NurbsSample object at a specified index in the array.
	\param in_index The index of the NurbsSurface.
	\return A reference to the NurbsSample object.
	*/
	CRef GetItem( LONG in_index ) const;

	/*! Returns the NurbsSample object in the array matching the specified name.
	\param in_name The name of the NurbsSurface.
	\return A reference to the NurbsSample object.
	*/
	CRef GetItem( const CString& in_name ) const;

	/*! Returns the number of NurbsSample objects in the array.
	\return The number of NurbsSample objects.
	*/
	LONG GetCount() const;

	/*! Returns true if this array is a valid object or false otherwise.
	\return true if valid, false otherwise.
	 */
	bool IsValid() const;

	/*! Returns the number of USampling coordinate valid indices.
	\retval out_lUSamplingCount number of USampling coordinate valid indices.
	\return CStatus::OK success
	\return CStatus::Fail other failure
	*/
	CStatus GetUSamplingCount(LONG& out_lUSamplingCount)const;

	/*! Returns the number of VSampling coordinate valid indices.
	\retval out_lVSamplingCount number of VSampling coordinate valid indices.
	\return CStatus::OK success
	\return CStatus::Fail other failure
	*/
	CStatus GetVSamplingCount(LONG& out_lVSamplingCount)const;

	/*! Returns the index of the sample point at given a UV sampling coordinate value.
	\param  in_lUSamplingCoordinate USamplingCoordinate.
	\param  in_lVSamplingCoordinate VSamplingCoordinate.
	\retval out_lIndex Returned index
	\retval out_lIndex NurbsSample global index.
	\return CStatus::OK success
	\return CStatus::Fail other failure
	*/
	CStatus GetIndexFromUVSamplingCoordinate
	(
		LONG	in_lUSamplingCoordinate,
		LONG	in_lVSamplingCoordinate,
		LONG&	out_lIndex
	)const;

	/*! Returns an array of Sample indices corresponding to UV sampling coordinate values.
	\param  in_aUVSamplingCoordinate Array of UV sampling coordinates.
	\retval out_aIndices Array of indices.
	\return CStatus::OK success
	\return CStatus::Fail other failure
	*/
	CStatus GetIndexArrayFromUVSamplingCoordinateArray
	(
		CLongArray&	in_aUVSamplingCoordinate,
		CLongArray&	out_aIndices
	)const;

	/*! Returns an array of UVSamplingCoordinate values for all Sample index.
	\param  in_aIndices Array of sample indices.
	\retval out_aUVSamplingCoordinate Array of UV sampling coordinates.
	\return CStatus::OK success
	\return CStatus::Fail other failure
	*/
	CStatus GetUVSamplingCoordinateArray
	(
		CLongArray&	in_aIndices,
		CLongArray&	out_aUVSamplingCoordinate
	)const;

	private:
	void* m_ptr;
};

};

#endif // __XSINURBSSAMPLE_H__
