//*****************************************************************************
/*!
   \file xsi_math.h
   \brief MATH namespace declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIMATH_H__
#define __XSIMATH_H__

#include "sicppsdk.h"
#include "xsi_vector3.h"
#include "xsi_vector4.h"
#include "xsi_matrix3.h"
#include "xsi_matrix4.h"
#include "xsi_transformation.h"
#include "xsi_quaternion.h"
#include "xsi_rotation.h"

namespace XSI {

/*! \namespace XSI::MATH
	\brief All Math classes and functions defined in the C++ API are assigned to the
	MATH namespace which is nested under the XSI namespace.

	Members of the MATH namespace can be identified explicitly with the symbol MATH for code
	clarity. To simplify the code however, namespace members can be referenced without the
	namespace symbol by using the C++ using directive.

	\eg Demonstrates using namespace
	\code
		// namespace qualification
		XSI::MATH::CVector3 v(1,1,0);

		// CVector referenced without explicit qualification
		using namespace XSI::MATH;
		CVector3 v(1,1,0);
	\endcode

*/
namespace MATH {

	/*! PI constant.
	*/
	static const double PI(3.14159265358979323846);

	/*! Converts an angle in degrees to an angle in radians.
	\param in_dDegrees angle in degrees.
	\return Angle in radians.
	 */
	inline double DegreesToRadians (double in_dDegrees)
	{ return (in_dDegrees * (PI/180.0));}

	/*! Converts an angle in radians to an angle in degrees.
	\param in_dRadians angle in radians.
	\return Angle in degrees.
	 */
	inline double RadiansToDegrees (double in_dRadians)
	{ return (in_dRadians * (180.0/PI));}



	/*! Converts a Pose described in a given ObjectSpace to WorldSpace
	\param in_transfoObjectSpace ObjectSpace into which the pose is defined.
	\param in_transfoPose Pose to convert.
	\return The world Pose.
	\sa CTransformation, KinematicState, Kinematics::GetGlobal, Kinematics::GetLocal

	\eg Demonstrates how to use MapObjectPoseToWorldSpace
	\code
		using namespace XSI;
		using namespace MATH;
		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject myCube;
		root.AddGeometry( L"Cube", L"MeshSurface",L"",myCube );

		KinematicState  globalKinematicState = myCube.GetKinematics().GetGlobal();
		globalKinematicState.PutParameterValue(L"posy", 4.0);
		globalKinematicState.PutParameterValue(L"posx", 2.0);

		X3DObject myCube2;
		myCube.AddGeometry( L"Cube", L"MeshSurface",L"",myCube2);

		globalKinematicState = myCube2.GetKinematics().GetGlobal();
		globalKinematicState.PutParameterValue(L"posz",3.0);
		globalKinematicState.PutParameterValue(L"rotx",45.0);

		CTransformation localTransformation =
								myCube2.GetKinematics().GetLocal().GetTransform();
		CTransformation globalTransformation =
								myCube.GetKinematics().GetGlobal().GetTransform();

		CVector3 translation(localTransformation.GetTranslation());

		app.LogMessage(
			L"The translation of the cube relative to its parent: x " +
			CValue(translation.GetX()).GetAsText() +
			L" y " +
			CValue(translation.GetY()).GetAsText() +
			L" z " +
			CValue(translation.GetZ()).GetAsText());

		CTransformation worldTransformation =
						MapObjectPoseToWorldSpace(	globalTransformation,
													localTransformation);

		translation = worldTransformation.GetTranslation();

		app.LogMessage(	L"The translation of the cube relative to " +
						CString(L"the origin of the universe: x ") +
						CValue(translation.GetX()).GetAsText() +
						L" y " + CValue(translation.GetY()).GetAsText() +
						L" z " + CValue(translation.GetZ()).GetAsText());
	\endcode
	 */
	SICPPSDKDECL CTransformation MapObjectPoseToWorldSpace
	(
		const CTransformation&	in_transfoObjectSpace,
		const CTransformation&	in_transfoPose
	);


	/*! Converts an Orientation described in a given ObjectSpace to WorldSpace.
	\param in_transfoObjectSpace ObjectSpace in which the orientation is defined.
	\param in_vector3Orientation Orientation to convert.
	\return The world orientation.
	\sa CTransformation, KinematicState, Kinematics::GetGlobal, Kinematics::GetLocal

	\eg Demonstrates MapObjectOrientationToWorldSpace
	\code
		using namespace XSI;
		using namespace MATH;
		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject myCube;
		root.AddGeometry( L"Cube", L"MeshSurface",L"",myCube );

		KinematicState  globalKinematicState = myCube.GetKinematics().GetGlobal();
		globalKinematicState.PutParameterValue(L"posy", 4.0);
		globalKinematicState.PutParameterValue(L"posx", 2.0);
		globalKinematicState.PutParameterValue(L"rotx", 45.0);

		CTransformation localTransformation =
								myCube.GetKinematics().GetLocal().GetTransform();

		PolygonMesh mesh = myCube.GetActivePrimitive().GetGeometry();
		CVector3Array normalArray = mesh.GetPolygons().GetNormalArray();
		CVector3 orientation(normalArray[1]);

		CVector3 worldOrientation =
					MapObjectOrientationToWorldSpace(	localTransformation,
														orientation);

		app.LogMessage(
				L"The second polygon normal corresponds to ( " +
				CValue(RadiansToDegrees(worldOrientation.GetX())).GetAsText() +
				L", " +
				CValue(RadiansToDegrees(worldOrientation.GetY())).GetAsText() +
				L", " +
				CValue(RadiansToDegrees(worldOrientation.GetZ())).GetAsText() +
				L") in the world space" );
	\endcode
	*/
	SICPPSDKDECL CVector3 MapObjectOrientationToWorldSpace
	(
		const CTransformation&	in_transfoObjectSpace,
		const CVector3&				in_vector3Orientation
	);


	/*! Converts a Position described in a given ObjectSpace to WorldSpace.
	\param in_transfoObjectSpace ObjectSpace in which the position is defined.
	\param in_vector3Position Position to convert.
	\return The world position.
	\sa CTransformation, KinematicState, Kinematics::GetGlobal, Kinematics::GetLocal

	\eg Demonstrates MapObjectPositionToWorldSpace
		\code
		using namespace XSI;
		using namespace MATH;
		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject myCube;
		root.AddGeometry( L"Cube", L"MeshSurface",L"",myCube );

		KinematicState  globalKinematicState = myCube.GetKinematics().GetGlobal();
		globalKinematicState.PutParameterValue(L"posy", 4.0);
		globalKinematicState.PutParameterValue(L"posx", 2.0);

		CTransformation localTransformation = myCube.GetKinematics().GetLocal().GetTransform();

		CPointRefArray pointArray = myCube.GetActivePrimitive().GetGeometry().GetPoints();
		CVector3 position(Point(pointArray.GetItem(0)).GetPosition());

		app.LogMessage(L"The local position is: X " +
					  CValue(position.GetX()).GetAsText() + CString(L" Y ") +
					  CValue(position.GetY()).GetAsText() + CString(L" Z ") +
					  CValue(position.GetZ()).GetAsText());

		CVector3 worldPosition =
						MapObjectPositionToWorldSpace(	localTransformation,
														position);

		app.LogMessage(L"The world position is: X " +
					  CValue(worldPosition.GetX()).GetAsText() + CString(L" Y ") +
					  CValue(worldPosition.GetY()).GetAsText() + CString(L" Z ") +
					  CValue(worldPosition.GetZ()).GetAsText());
	\endcode
	 */
	SICPPSDKDECL CVector3 MapObjectPositionToWorldSpace
	(
		const CTransformation&	in_transfoObjectSpace,
		const CVector3&			in_vector3Position
	);

	/*! Converts a Pose described in WorldSpace to a different ObjectSpace
	\param in_transfoObjectSpace ObjectSpace in which the position is defined.
	\param in_transfoPose Pose to convert.
	\return The converted Pose.
	 */
	SICPPSDKDECL CTransformation MapWorldPoseToObjectSpace
	(
		const CTransformation&	in_transfoObjectSpace,
		const CTransformation&	in_transfoPose
	);

	/*! Converts an orientation described in WorldSpace to a different ObjectSpace.
	\param in_transfoObjectSpace ObjectSpace in which the position is defined.
	\param in_vector3Orientation Orientation to convert.
	\return The converted orientation.
	 */
	SICPPSDKDECL CVector3 MapWorldOrientationToObjectSpace
	(
		const CTransformation&	in_transfoObjectSpace,
		const CVector3&			in_vector3Orientation
	);

	/*! Converts a position described in WorldSpace to a position in a different ObjectSpace.
	\param in_transfoObjectSpace ObjectSpace in which we want to convert the position.
	\param in_vector3Position Position to convert.
	\return The converted position.
	\sa CTransformation, KinematicState, Kinematics::GetGlobal, Kinematics::GetLocal

	\eg Demonstrates MapWorldPositionToObjectSpace
	\code
		using namespace XSI;
		using namespace MATH;
		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject myCube;
		root.AddGeometry( L"Cube", L"MeshSurface", L"", myCube );

		KinematicState  globalKinematicState = myCube.GetKinematics().GetGlobal();
		globalKinematicState.PutParameterValue(L"posy", 4.0);
		globalKinematicState.PutParameterValue(L"posx", 2.0);
		globalKinematicState.PutParameterValue(L"rotx", 30.0);

		CTransformation localTransformation =
								myCube.GetKinematics().GetLocal().GetTransform();
		CVector3 position(1.0,1.0,1.0);
		CVector3 convertedPosition =
					MapWorldPositionToObjectSpace(	localTransformation,
													position);

		app.LogMessage(L"The position (1,1,1) is (" +
					CValue(convertedPosition.GetX()).GetAsText() + L", " +
					CValue(convertedPosition.GetY()).GetAsText() + L", " +
					CValue(convertedPosition.GetZ()).GetAsText() +
					L" in the cube object space");
	\endcode
	 */
	SICPPSDKDECL CVector3 MapWorldPositionToObjectSpace
	(
		const CTransformation&	in_transfoObjectSpace,
		const CVector3&			in_vector3Position
	);

	/*! Converts a pose described in an ObjectSpace to a pose in a different ObjectSpace.
	\param in_transfoObjectSpace ObjectSpace in which the pose is described.
	\param in_transfoSpace Space in which we want to convert the pose.
	\param in_transfoPose Pose to convert.
	\return The converted transformation.
	\sa CTransformation, KinematicState, Kinematics::GetGlobal, Kinematics::GetLocal

	\eg Demonstrates MapObjectPoseToObjectSpace
	\code
		using namespace XSI;
		using namespace MATH;
		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject myCube;
		root.AddGeometry( L"Cube", L"MeshSurface", L"",myCube );

		KinematicState  globalKinematicState = myCube.GetKinematics().GetGlobal();
		globalKinematicState.PutParameterValue(L"posy", 4.0);
		globalKinematicState.PutParameterValue(L"posx", 2.0);

		X3DObject myCube2;
		myCube.AddGeometry( L"Cube", L"MeshSurface", L"", myCube2 );

		globalKinematicState = myCube2.GetKinematics().GetGlobal();
		globalKinematicState.PutParameterValue(L"posz", 3.0);
		globalKinematicState.PutParameterValue(L"rotz", 45.0);

		X3DObject myCone;
		root.AddGeometry( L"Cone", L"MeshSurface", L"",myCone );

		globalKinematicState = myCube2.GetKinematics().GetGlobal();
		globalKinematicState.PutParameterValue(L"posy", -2.0);
		globalKinematicState.PutParameterValue(L"rotz", 90.0);

		CTransformation localTransformation =
							myCube2.GetKinematics().GetLocal().GetTransform();
		CTransformation parentTransformation =
							myCube.GetKinematics().GetGlobal().GetTransform();
		CVector3 translation(localTransformation.GetTranslation());
		CTransformation destinationTransformation =
							myCone.GetKinematics().GetGlobal().GetTransform();

		app.LogMessage(
			L"The translation of the cube relative to its parent: x " +
			CValue(translation.GetX()).GetAsText() + CString(L" Y ") +
			CValue(translation.GetY()).GetAsText() + CString(L" Z ") +
			CValue(translation.GetZ()).GetAsText());

		CTransformation convertedTranformation =
				MapObjectPoseToObjectSpace(	parentTransformation,
											destinationTransformation,
											localTransformation);

		translation = convertedTranformation.GetTranslation();

		app.LogMessage(
			L"The translation of the cube relative to the cone: x " +
			CValue(translation.GetX()).GetAsText() + CString(L" Y ") +
			CValue(translation.GetY()).GetAsText() + CString(L" Z ") +
			CValue(translation.GetZ()).GetAsText());
	\endcode
	 */
	SICPPSDKDECL CTransformation MapObjectPoseToObjectSpace
	(
		const CTransformation&	in_transfoObjectSpace,
		const CTransformation&	in_transfoSpace,
		const CTransformation&	in_transfoPose
	);

	/*! Converts an orientation described in an ObjectSpace to an orientation in a different ObjectSpace.
	\param in_transfoObjectSpace ObjectSpace in which the orientation is described.
	\param in_transfoSpace Space in which we want to convert the orientation.
	\param in_vector3Orientation Orientation to convert.
	\sa CTransformation, KinematicState, Kinematics::GetGlobal, Kinematics::GetLocal

	\eg Demonstrates MapObjectOrientationToObjectSpace
	\code
		using namespace XSI;
		using namespace MATH;
		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject myCube;
		root.AddGeometry( L"Cube", L"MeshSurface", L"",myCube );

		KinematicState  globalKinematicState = myCube.GetKinematics().GetGlobal();
		globalKinematicState.PutParameterValue(L"posy", 4.0);
		globalKinematicState.PutParameterValue(L"posx", 2.0);

		X3DObject myCube2;
		myCube.AddGeometry( L"Cube", L"MeshSurface", L"",myCube2 );

		globalKinematicState = myCube2.GetKinematics().GetGlobal();
		globalKinematicState.PutParameterValue(L"posz", 3.0);
		globalKinematicState.PutParameterValue(L"rotz", 45.0);

		X3DObject myCone;
		root.AddGeometry( L"Cone", L"MeshSurface", L"", myCone );

		globalKinematicState = myCube2.GetKinematics().GetGlobal();
		globalKinematicState.PutParameterValue(L"posy", -2.0);
		globalKinematicState.PutParameterValue(L"rotz", 90.0);

		CTransformation localTransformation =
								myCube2.GetKinematics().GetLocal().GetTransform();

		PolygonMesh myMesh = myCube.GetActivePrimitive().GetGeometry();
		CVector3Array normalArray = myMesh.GetPolygons().GetNormalArray();

		CVector3 orientation(normalArray[1]);

		CTransformation destinationTransformation =
								myCone.GetKinematics().GetGlobal().GetTransform();

		CVector3 convertedOrientation =
			MapObjectOrientationToObjectSpace(	localTransformation,
												destinationTransformation,
												orientation);

		app.LogMessage(L"The second face of the cube normal vector " +
				CString(L"represented in the cone referential( " )+
				CValue(RadiansToDegrees(convertedOrientation.GetX())).GetAsText()
				+ L", " +
				CValue(RadiansToDegrees(convertedOrientation.GetY())).GetAsText()
				+ L", " +
				CValue(RadiansToDegrees(convertedOrientation.GetZ())).GetAsText() +
				L" ) in the cube object space");
	\endcode
	 */
	SICPPSDKDECL CVector3 MapObjectOrientationToObjectSpace
	(
		const CTransformation&	in_transfoObjectSpace,
		const CTransformation&	in_transfoSpace,
		const CVector3&			in_vector3Orientation
	);


	/*! Converts a position described in an ObjectSpace to a position in a different ObjectSpace.
	\param in_transfoObjectSpace ObjectSpace in which the position is described.
	\param in_transfoSpace Space in which we want to convert the position.
	\param in_vector3Position Position to convert.
	\return The converted position.
	\sa CTransformation, KinematicState, GetGlobal, GetLocal

	\eg Demonstrates MapObjectPositionToObjectSpace
	\code
		using namespace XSI;
		using namespace MATH;
		//Forward declaration
		bool Above(X3DObject&, X3DObject&);

		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject myCube;
		root.AddGeometry( L"Cube", L"MeshSurface", L"", myCube );

		KinematicState globalKinematicState = myCube.GetKinematics().GetGlobal();
		globalKinematicState.PutParameterValue(L"posy", 4.0);
		globalKinematicState.PutParameterValue(L"posx", 2.0);

		X3DObject myGrid;
		oRoot.AddGeometry( L"Grid", L"MeshSurface", L"",myGrid );
		myCube.GetKinematics().GetGlobal().PutParameterValue(L"posy",-2.0);

		if( Above(myCube,myGrid)
			app.LogMessage(L"The cube is above the grid");
		else
			app.LogMessage(L"The cube is below the grid");

		//Function that returns true if in_obj1 is above in_obj2.
		//Which means that all " y" values of in_obj1 vertices relative to
		//in_obj2 are bigger than in_obj2 "y" values
		bool Above(X3DObject& in_obj1, X3DObject& in_obj2)
		{
			double biggestY(0.0);

			CVector3Array positionArray;
			CVector3Array::iterater it;
			Geometry myGeom = in_obj1.GetActivePrimitive().GetGeoemtry()
			myGeom.Points().PositionArray(positionArray);

			//Determine the upper position in obj1;
			for(LONG i = 0; i < positionArray.GetCount(); ++i)
			{
				if(positionArray[i].GetY() > biggestY)
				{ biggestY = positionArray[i].GetY();}
			}

			CTransformation globalTransformation1,globalTransformation2;
			globalTransformation1 = in_obj1.GetKinematics().GetGlobal().GetTransform();
			globalTransformation2 = in_obj2.GetKinematics().GetGlobal().GetTransform();

			Geometry myGeom = in_obj2.GetActivePrimitive().GetGeometry();
			myGeom.Points().PositionArray(positionArray);

			CVector3 convertedPosition;
			//Determine if all vertices of in_obj1 are above in_obj2
			i= 0;
			for(; i < positionArray.GetCount(); ++i)
			{
				convertdPosition =
					MapObjectPositionToObjectSpace(	globalTransformation1,
													globalTransformation2,
													positionArray[i]);

				if(convertdPosition.GetY() < biggestY)
				{ return false;}
			}

			return true;
		}

	\endcode
	 */
	SICPPSDKDECL CVector3 MapObjectPositionToObjectSpace
	(
		const CTransformation&	in_transfoObjectSpace,
		const CTransformation&	in_transfoSpace,
		const CVector3&			in_vector3Position
	);

};
};

#endif // __XSIMATH_H__
