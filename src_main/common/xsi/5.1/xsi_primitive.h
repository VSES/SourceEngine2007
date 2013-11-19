//*****************************************************************************
/*!
   \file xsi_primitive.h
   \brief Primitive class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIPRIMITIVE_H__
#define __XSIPRIMITIVE_H__

#include <xsi_projectitem.h>

namespace XSI {

class Geometry;
class ConstructionHistory;

//*****************************************************************************
/*! \class Primitive xsi_primitive.h
	\brief Represents the primitive object of a X3DObject, a primitive is the basic
	form that is the building block of modeling and animation.

	Primitive can
	be created with X3DObject.GetActivePrimitive.

	\sa PolygonMesh,Vertex, CVertexRefArray

	\eg
	\code
		using namespace XSI;
		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject myCube;
		root.AddGeometry( L"Cube", L"MeshSurface",L"myMesh",myCube);

		PolygonMesh myMesh(myCube.GetActivePrimitive().GetGeometry());
		CVertexRefArray vertices(myMesh.GetVertices());

		LONG lCount = vertices.GetCount();
		for (LONG i=0; i<lCount; i++ )
		{
			Vertex vertex(vertices.GetItem(i));

			CVertexRefArray neighbors(vertex.GetNeighborVertices(1));
			app.LogMessage(L"Number of neighbor vertices for Vertex " +
					CValue(i).GetAsText() + L": " +
					CValue(neighbors.GetCount()).GetAsText() );
		}
	\endcode
 */
//*****************************************************************************

class SICPPSDKDECL Primitive : public ProjectItem
{
public:
	/*! Default constructor. */
	Primitive();

	/*! Default destructor. */
	~Primitive();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	Primitive(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	Primitive(const Primitive& in_obj);

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
	\return The new Primitive object.
	*/
	Primitive& operator=(const Primitive& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new Primitive object.
	*/
	Primitive& operator=(const CRef& in_ref);

	/*! Returns a Geometry object containing the objects geometry
		\param in_time Time (in frames) at which to get property. The default value
		is DBL_MAX (representing the current time).
		\return The object's Geometry.
	*/
	Geometry GetGeometry( double in_time = DBL_MAX ) const;

	/*! Returns a snapshot of the Geometry at the specified time.
	\param in_time Time (in frames) at which to get property. Use DBL_MAX to
	specify the current time.
	Note: This argument is ignored inside operators.

	\param in_mode The construction mode is used to access a version of the
		geometry with specific deforms. The geometry positions you get depends
		on the mode you passed in.

		Valid construction modes include:
		\li \c siConstructionModeModeling: Gets the original geometry positions. This mode is typically
			used in export applications where geometry, shape and envelope positions are exported separately.

		\li \c siConstructionModePrimaryShape: Combines the geometry positions with the shape deformation.

		\li \c siConstructionModeSecondaryShape: Combines the geometry positions with the shape deformation,
			the envelope deformation and the deforms installed above the envelope such as the move point
			operators. This mode is typically used for plotting the final results of shape and envelope deformation.

		\li \c siConstructionModeAnimation: Combines the geometry positions with the shape and envelope
			deformation altogether.

		\li \c siConstructionModeDefault: Uses the current construction mode set in XSI.

	Note: This argument is ignored inside operators.
	
	\return The object's Geometry.
	\sa siConstructionMode

	\eg Demonstrates how to access the original point positions and then the shape and envelope deformations
	\code
		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject myCube;
		root.AddGeometry( L"Cube", L"MeshSurface",L"myMesh",myCube);

		Primitive cubePrimitive = myCube.GetActivePrimitive();

		CValue retval;
		CValueArray args(19);
		args[0] = myCube.GetFullName() + L".pnt[0]";
		args[1] = 10;
		args[2] = 10;
		args[3] = 10;
		args[17] = siConstructionModeAnimation;

		app.ExecuteCommand(L"Translate",args,retval );

		Geometry modelGeometry = cubePrimitive.GetGeometry(0,siConstructionModeModeling);
		Point modelPoint1 = modelGeometry.GetPoints()[0];
		app.LogMessage( L"modelPoint1: " + CString(modelPoint1.GetPosition()) );
			
		Geometry shapeGeometry = cubePrimitive.GetGeometry(0,siConstructionModeAnimation);
		Point shapePoint1 = shapeGeometry.GetPoints()[0];
		app.LogMessage( L"shapePoint1: " + CString(shapePoint1.GetPosition()) );	
	\endcode
	*/
	Geometry GetGeometry(	double				in_time,
							siConstructionMode	in_mode ) const;

	private:
	Primitive * operator&() const;
	Primitive * operator&();
};

};

#endif // __XSIPRIMITIVE_H__
