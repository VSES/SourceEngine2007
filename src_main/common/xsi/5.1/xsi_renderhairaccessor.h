//*****************************************************************************
/*!
   \file xsi_renderhairaccessor.h
   \brief CRenderHairAccessor class declaration.

   © Copyright 1998-2005 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIRENDERHAIRACCESSOR_H__
#define __XSIRENDERHAIRACCESSOR_H__

#include <xsi_base.h>
#include <xsi_status.h>

namespace XSI {

class CFloatArray;
class CLongArray;

//*****************************************************************************
/*! \class CRenderHairAccessor xsi_renderhairaccessor.h
	\brief This class provides access to the render hair information exposed by the
	hair primitive. This information includes: the vertex positions of each
	generated render hair, the root/tip radius values, the UVW, weight map and
	vertex color values for each render hair root location.

	CRenderHairAccessor provides a mechanism to access the data in chunks which
	allows you to optimize the memory allocation required for accessing such
	a large data set.

	Use HairPrimitive::GetRenderHairAccessor to create an instance of
	CRenderHairAccessor.

	\sa HairPrimitive
	\since 5.0

	\eg This example demonstrates how to use the render hair accessor
		to extract the render hair data from a hair primitive.
	\code
		using namespace XSI;
		Application app;
		Model root = app.GetActiveSceneRoot();

		// create a cone with hair along with a uv property
		X3DObject myCone;
		root.AddGeometry( L"Cone", L"MeshSurface",L"myCone",myCone);

		Material myMat;
		myCone.AddMaterial(L"Phong", false, L"myMat", myMat);

		Shader sh = myMat.GetShaders().GetItem(0);
		Parameter p = sh.GetParameters().GetItem(L"diffuse");

		// connect an image parameter and add a UV property on the diffuse param
		CRef newSource;
		CRef prevSource;
		p.ConnectFromPreset( L"Image", siTextureShaderFamily, prevSource, newSource );

		// creates a projection i.e. UV property
		CValueArray args(7);
		CValue outArg;
		args[0] = myCone.GetFullName();
		args[1] = CString(L"siTxtSpherical");
		args[2] = CString(L"siTxtDefaultSpherical");
		args[3] = CValue();
		args[4] = CString(L"Texture_Projection");
		args[5] = CValue();
		args[6] = CValue((LONG)XSI::siRelDefault);

		app.ExecuteCommand( L"CreateProjection", args, outArg );

		// assign the projection to the cone
		args.Resize(3);
		args[0] = myCone.GetFullName();
		args[1] = CString(L"Sources.Materials.DefaultLib.myMat.Phong.Image.tspace_id");
		args[2] = CString(L"Texture_Projection");

		app.ExecuteCommand( L"SetInstanceDataValue", args, outArg );

		// add hair to the cone
		args.Resize(1);
		args[0] = myCone;
		app.ExecuteCommand( L"ApplyHairOp", args, outArg ) ;

		// transfer the uv property to the hair primitive
		X3DObject myHair = myCone.FindChild( L"Hair", L"", CStringArray() );

		args.Resize(2);
		args[0] = myHair.GetFullName();
		args[1] = CString(L"myCone.polymsh.cls.Texture_Coordinates_AUTO.Texture_Projection");

		app.ExecuteCommand( L"PropertyTransfer", args, outArg ) ;

		// setup a render hair accessor for 100 hairs with a chunk size of 10
		HairPrimitive myHairPrim( myHair.GetActivePrimitive() );

		CRenderHairAccessor rha = myHairPrim.GetRenderHairAccessor(	100, 10 );

		LONG nReqChunkSize = rha.GetRequestedChunkSize();
		app.LogMessage( L"nReqChunkSize: " + CValue(nReqChunkSize).GetAsText() );

		LONG nReqHairCount = rha.GetRequestedHairCount();
		app.LogMessage( L"nReqHairCount: " + CValue(nReqHairCount).GetAsText() );

		LONG nUVs = rha.GetUVCount();
		app.LogMessage( L"nUVs: " + CValue(nUVs).GetAsText() );

		// get the values in chunks
		LONG nChunk = 0;
		LONG i=0;
		while( rha.Next() )
		{
			app.LogMessage( L"Chunk: " + CValue(nChunk++).GetAsText() );

			// get the number of vertices for each render hair
			// note: this array is used for iterating over the render hair position
			// and radius values
			CLongArray verticesCountArray;
			rha.GetVerticesCount(verticesCountArray);

			// get the render hair positions
			CFloatArray posVals;
			rha.GetVertexPositions(posVals);

			LONG k = 0;
			app.LogMessage( L"Render hair position values: " );
			for (i=0; i<verticesCountArray.GetCount(); i++ )
			{
				LONG nCount = verticesCountArray[i];
				for (LONG j=0; j<nCount; j++ )
				{
					CString strVals;
					strVals=
						CValue(posVals[k++]).GetAsText() + CString(L" ") +
						CValue(posVals[k++]).GetAsText() + CString(L" ") +
						CValue(posVals[k++]).GetAsText() ;

					app.LogMessage( CValue(i).GetAsText() + L": " + strVals );
				}
			}

			// get the render hair radius
			app.LogMessage( L"Render hair radius values: " );
			CFloatArray radVals;
			rha.GetVertexRadiusValues(radVals);

			k = 0;
			for (i=0; i<verticesCountArray.GetCount(); i++ )
			{
				LONG nCount = verticesCountArray[i];
				for (LONG j=0; j<nCount; j++ )
				{
					app.LogMessage( CValue(i).GetAsText() + L": " +
						CValue(radVals[k++]).GetAsText() );
				}
			}

			// get the uv values
			app.LogMessage( L"Render hair uv values: " );
			for (i=0; i<nUVs; i++)
			{
				app.LogMessage( L"    UV: " +  rha.GetUVName(i) );

				CFloatArray uvVals;
				rha.GetUVValues( i, uvVals );

				for (LONG j=0,k=0; j<uvVals.GetCount(); j+=3, k++)
				{
					CString strVals;
					strVals =
						CValue(uvVals[j+0]).GetAsText() + L" " +
						CValue(uvVals[j+1]).GetAsText() + L" " +
						CValue(uvVals[j+2]).GetAsText() ;

					app.LogMessage( CValue(k).GetAsText() + L": " + strVals );
				}
			}
		}

  	\endcode
*/
//*****************************************************************************

class SICPPSDKDECL CRenderHairAccessor : public CBase
{
public:
	/*! Default constructor. */
	CRenderHairAccessor();

	/*! Default destructor. */
	~CRenderHairAccessor();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	CRenderHairAccessor(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	CRenderHairAccessor(const CRenderHairAccessor& in_obj);

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
	\return The new CRenderHairAccessor object.
	*/
	CRenderHairAccessor& operator=(const CRenderHairAccessor& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new CRenderHairAccessor object.
	*/
	CRenderHairAccessor& operator=(const CRef& in_ref);

	/*! Sets the accessor to the next available chunk of render hair data for
	reading. The position of the next chunk is computed based on the chunk
	size used for initializing the accessor. When the accessor is first
	initialized, there is no data available and CRenderHairAccessor::Next has
	to be called to receive the first chunk of data.

	\return True if there is data to read or false otherwise.

	\sa CRenderHairAccessor::Reset, HairPrimitive::GetRenderHairAccessor,
		CRenderHairAccessor::GetRequestedChunkSize
	*/
	bool Next();

	/*! Sets the accessor to the first chunk of render hair data for reading.
	\return CStatus::OK Success.
	\sa CRenderHairAccessor::Next
	*/
	CStatus Reset();

	/*! Returns the requested size of each render hair chunk. This number is
	always larger than or equal to the value returned from
	CRenderHairAccessor::GetChunkHairCount().

	\return long Chunk size. Returns -1 if the function fails.

	\sa CRenderHairAccessor::Next, HairPrimitive::GetRenderHairAccessor,
		CRenderHairAccessor::GetChunkHairCount
	*/
	LONG GetRequestedChunkSize() const;

	/*! Returns the maximum number of render hairs to access. This value is
	specified when the accessor is first created and does not correspond to
	the	maximum number of render hairs available on the primitive.

	\return LONG Requested number of render hairs. Returns -1 if the function fails.
	\sa HairPrimitive::GetRenderHairAccessor
	*/
	LONG GetRequestedHairCount() const;

	/*! Returns the number of render hairs available for the current chunk.
	If hairs have been trimmed due to cut or density maps, this number can
	be less than the requested chunk size, even zero.

	\return LONG Number of render hairs for the current chunk. Returns -1 if the function fails.
	\sa CRenderHairAccessor::Next
	*/
	LONG GetChunkHairCount() const;

	/*! Returns an array containing the number of vertices for each generated
	render hair. This array can be used to iterate over the array of
	vertices and radius values.

	\retval out_values Array of vertex count.
	\return CStatus::OK Success

	\sa GetRenderHairAccessor::GetVertexPositions, GetRenderHairAccessor::GetVertexRadiusValues
	*/
	CStatus GetVerticesCount(CLongArray& out_values) const;

	/*! Returns an array of vertex positions (XYZ) for each render hair in
	local space. The vertex points are packed sequentially, the first being
	the root location, the next one in the sequence being the next
	connected point, all the way to the tip of the hair. Use the array
	returned by GetRenderHairAccessor::GetVerticesCount to find the number of
	vertices for each render hair in order to iterate over the vertex positions.

	\retval out_values XYZ values.
	\return CStatus::OK success
	\sa GetRenderHairAccessor::GetVerticesCount
	*/
	CStatus GetVertexPositions(CFloatArray& out_values) const;

	/*! Returns an array of radius values at each render hair vertex. The radius
	values are packed sequentially, the first being the radius of the root
	location, the next one in the sequence being the next connected point
	radius, all the way to the tip of the hair. Use the array returned by
	GetRenderHairAccessor::GetVerticesCount to find the number of vertices for
	each render hair in order to iterate over the vertex radius values.

	\retval out_values Radius values for each vertex of a render hair.
	\return CStatus::OK success
	\sa GetRenderHairAccessor::GetVerticesCount
	*/
	CStatus GetVertexRadiusValues(CFloatArray& out_values) const;

	/*! Returns the surface normal (XYZ) of the hair's emitter surface, from
	which each render hair is emitted.
	\retval out_values Surface normal values for each render hair.
	\return CStatus::OK success
	\sa GetRenderHairAccessor::GetVerticesCount
	*/
	CStatus GetHairSurfaceNormalValues(CFloatArray &out_values) const;

	/*! Returns the number of UV properties on the primitive.
	\return LONG Number of UV properties. Returns -1 if the function fails.
	\sa GetRenderHairAccessor::GetUVName, GetRenderHairAccessor::GetUVValues
	*/
	LONG GetUVCount() const;

	/*! Returns the number of vertex color properties on the primitive.
	\return LONG Number of vertex color properties. Returns -1 if the function fails.
	\sa GetRenderHairAccessor::GetVertexColorName, GetRenderHairAccessor::GetVertexColorValues
	*/
	LONG GetVertexColorCount() const;

	/*! Returns the number of weight map properties on the primitive.
	\return LONG Number of weight map properties. Returns -1 if the function fails.
	\sa GetRenderHairAccessor::GetWeightMapName, GetRenderHairAccessor::GetWeightMapValues
	*/
	LONG GetWeightMapCount() const;

	/*! Returns the name of the UV property at a given index. The index refers to
	the position of the property on the primitive.

	\param in_nIdx Property index.
	\return CString Name of the property.
	\sa GetRenderHairAccessor::GetUVCount, GetRenderHairAccessor::GetUVValues
	*/
	CString GetUVName( LONG in_nIdx ) const;

	/*! Returns the name of the vertex color property at a given index. The index
	refers to the position of the property on the primitive.

	\param in_nIdx Property index.
	\return CString Name of the property.
	\sa GetRenderHairAccessor::GetVertexColorCount, GetRenderHairAccessor::GetVertexColorValues
	*/
	CString GetVertexColorName( LONG in_nIdx ) const;

	/*! Returns the name of the weight map property at a given index. The index
	refers to the position of the property on the primitive.

	\param in_nIdx Property index.
	\return CString Name of the property.
	\sa GetRenderHairAccessor::GetWeightMapCount, GetRenderHairAccessor::GetWeightMapValues
	*/
	CString GetWeightMapName( LONG in_nIdx ) const;

	/*! Returns an array of UVW values for the UV property at the specified index. The values match
	the render hair root locations on the primitive. The index refers to the position of the property
	on the primitive.

	In general, the size of the returned array is GetRenderHairAccessor::GetChunkHairCount() * 3.
	However, in the case where render hairs are generated in an area where they get cut due to a cut
	map or where the density decreases, the number of values returned is smaller.

	\param in_nIdx Property index.
	\retval out_values UVW values.
	\return CStatus::OK success
	\sa GetRenderHairAccessor::GetUVCount, GetRenderHairAccessor::GetUVName
	*/
	CStatus GetUVValues( LONG in_nIdx, CFloatArray& out_values ) const;

	/*! Returns an array of vertex color values (RGBA) for the vertex color property at the specified
	index. The values match the render hair root locations on the primitive. The index refers to the
	position of the property on the primitive.

	In general, the size of the returned array is GetRenderHairAccessor::GetChunkHairCount() * 4.
	However, in the case where render hairs are generated in an area where they get cut due to a cut
	map or where the density decreases, the number of values returned is smaller.

	\param in_nIdx Property index.
	\retval out_values RGBA color values ranging from 0.0 to 1.0.
	\return CStatus::OK success
	\sa GetRenderHairAccessor::GetVertexColorCount, GetRenderHairAccessor::GetVertexColorName
	*/
	CStatus GetVertexColorValues( LONG in_nIdx, CFloatArray& out_values ) const;

	/*! Returns an array of weight map values (W) for the weight map property at the specified index.
	The values match the render hair root locations on the primitive. The index refers to the position
	of the property on the primitive.

	In general, the size of the returned array is GetRenderHairAccessor::GetChunkHairCount(). However,
	in the case where render hairs are generated in an area where they get cut due to a cut map or where
	the density decreases, the number of values returned is smaller.

	\param in_nIdx Property index.
	\retval out_values Weight map values.
	\return CStatus::OK success
	\sa GetRenderHairAccessor::GetWeightMapCount, GetRenderHairAccessor::GetWeightMapName
	*/
	CStatus GetWeightMapValues( LONG in_nIdx, CFloatArray& out_values ) const;

	private:
	CRenderHairAccessor * operator&() const;
	CRenderHairAccessor * operator&();
};

};
#endif // __XSIRENDERHAIRACCESSOR_H__
