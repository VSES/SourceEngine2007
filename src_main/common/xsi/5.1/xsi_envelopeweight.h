//*****************************************************************************
/*!
   \file xsi_envelopeweight.h
   \brief EnvelopeWeight class declaration.

   © Copyright 1998-2005 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIENVWPROP_H__
#define __XSIENVWPROP_H__

#include <xsi_clusterproperty.h>

#pragma warning(disable:4251)

namespace XSI {

//*****************************************************************************
/*! \class EnvelopeWeight xsi_envelopeweight.h
	\brief An envelope weight is a specialized kind of ClusterProperty used to store the weights for each point on an envelope.

	You can use this class to access the deformer objects that control the shape of the envelope.

	\since 5.0
*/
//*****************************************************************************

class SICPPSDKDECL EnvelopeWeight : public ClusterProperty
{
public:
	/*! Default constructor. */
	EnvelopeWeight();

	/*! Default destructor. */
	~EnvelopeWeight();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	EnvelopeWeight(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	EnvelopeWeight(const EnvelopeWeight& in_obj);

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
	\return The new EnvelopeWeight object.
	*/
	EnvelopeWeight& operator=(const EnvelopeWeight& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new EnvelopeWeight object.
	*/
	EnvelopeWeight& operator=(const CRef& in_ref);

	/*! Returns the deformers of the envelope that control the shape of the envelope.
	\return Array of references to the X3DObject deformers.
	*/
	CRefArray GetDeformers() const;

	/*! Sets the vertex weight values for a given deformer affecting the underlying envelope. This function sets the
	values directly on the property and the operation is not undoable.

	For large data sets, you can use an offset index to set the values in chunks. The offset is used for indexing into
	\c in_pVertices which allows you to optimize the memory allocation required for setting the vertex weight values.

	\param in_deformer The envelope deformer objects.
	\param in_pVertices Array of size equal to \c in_nVertices containing the vertex indices to set.
	\param in_pValues Array of size equal to \c in_nVertices containing the weight values.
	\param in_nVertices Number of vertices in \c in_pVertices.

	\return CStatus::OK Success.
	\return CStatus::InvalidArgument Returns an error if in_nVertices is greater than the number of weights for the
		specified deformer or smaller than zero. Also returns an error if \c in_pVertices or \c in_pValues are invalid.

	\sa CClusterPropertyBuilder, CGeometryAccessor::GetClusterProperties, Material::GetClusterProperties
	\since 5.0
	*/
	CStatus SetValues(
		const CRef&		in_deformer,
		const LONG*		in_pVertices,
		const float*	in_pValues,
		LONG			in_nVertices );

	/*! Sets the vertex weight values for a given deformer affecting the underlying envelope. This function sets the vertex
	elements in the order specified by \c in_pValues. Values are set directly on the property and the operation is not undoable.

	For large data sets, you can use an offset index to set the values in chunks. The offset is used for indexing into cluster
	elements which allows you to optimize the memory allocation required for setting the element values.

	\param in_deformer The envelope deformer objects.
	\param in_pValues Array of size equal to \c in_nValues containing the weight values.
	\param in_nValues Number of values in \c in_pValues.
	\param in_nOffset Specifies the starting index in the vertex elements of the cluster.

	\return CStatus::OK Success.
	\return CStatus::InvalidArgument Returns an error if \c in_nVertices is greater than the number of weights for the
		specified deformer or smaller than zero. Also returns an error if \c in_pValues is invalid.
	\sa CClusterPropertyBuilder, CGeometryAccessor::GetClusterProperties, Material::GetClusterProperties
	\since 5.0
	*/
	CStatus SetValues(
		const CRef&		in_deformer,
		const float*	in_pValues,
		LONG			in_nValues,
		LONG			in_nOffset=0 );

	private:
	EnvelopeWeight * operator&() const;
	EnvelopeWeight * operator&();
};

};

#endif // __XSIENVWPROP_H__
