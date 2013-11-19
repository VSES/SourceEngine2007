//*****************************************************************************
/*!
   \file xsi_kinematics.h
   \brief Kinematics class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIKINEMATICS_H__
#define __XSIKINEMATICS_H__

#include <xsi_property.h>
#include <xsi_vector3.h>
#include <xsi_kinematicstate.h> 

namespace XSI {

class Constraint;

//*****************************************************************************
/*! \class Kinematics xsi_kinematics.h
	\brief The Kinematics object represents the kinematics property of an X3DObject object.

	\sa X3DObject::GetKinematics

	\eg Illustrates how to convert a position from local coordinates to global coordinates.

	\code
		using namespace XSI;
		Application app;
		Model root = app.GetActiveSceneRoot();

		Model myModel;
		CRefArray emptyModel;
		root.AddModel( emptyModel, L"MyModel", myModel );
		myModel.GetParameter(L"posx").PutValue( (double)5.0 );

		Null myNull;
		myModel.AddNull( L"", myNull );
		myNull.GetParameter(L"posx").PutValue( (double)5.0 );

		// get local position
		MATH::CVector3 v3Pos( myNull.GetParameterValue(L"posx"),
							myNull.GetParameterValue(L"posy"),
							myNull.GetParameterValue(L"posz") );

		app.LogMessage( L"Local position = " +
						CValue(v3Pos.GetX()).GetAsText() + L", " +
						CValue(v3Pos.GetY()).GetAsText() + L", " +
						CValue(v3Pos.GetZ()).GetAsText() );

		// transform local to global position
		MATH::CVector3 v3GlobalPos = MATH::MapObjectPositionToObjectSpace(
			myNull.GetKinematics().GetGlobal().GetTransform(),
			myNull.GetKinematics().GetLocal().GetTransform(),
			v3Pos );

		app.LogMessage( L"Global position = " +
						CValue(v3GlobalPos.GetX()).GetAsText() + L", " +
						CValue(v3GlobalPos.GetY()).GetAsText() + L", " +
						CValue(v3GlobalPos.GetZ()).GetAsText() );
	\endcode
*/
//*****************************************************************************
class SICPPSDKDECL Kinematics : public Property
{
public:
	/*! Default constructor. */
	Kinematics();

	/*! Default destructor. */
	~Kinematics();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	Kinematics(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	Kinematics(const Kinematics& in_obj);

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
	\return The new Kinematics object.
	*/
	Kinematics& operator=(const Kinematics& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new Kinematics object.
	*/
	Kinematics& operator=(const CRef& in_ref);

	/*! Returns an array of Constraint objects that are constraining the parent of this object.
	\return Array of Constraint objects.
	*/
	CRefArray GetConstraints() const;

	/*! Applies a constraint to this object's parent.
	\param in_preset The type of constraint to add.
	\param in_cnsning An array of X3DObject references that will act as constrainers.
	\param in_bComp Compensation mode, true to turn compensation on.
	\param in_offsetCnsned Optional offset which is applied in translation on the constrained object.
		The offset is used for Distance and Position constraints.
	\param in_offsetCnsning Optional offset applied on the constraining object for Distance, Position,
		Surface and Trajectory constraints. The offset will be applied in rotation for the Orientation
		constraint, in translation for the Direction constraint and in scaling for the Scaling constraint.
	\retval out_constraint New constraint object
	\return CStatus::OK success
	\return CStatus::InvalidArgument Preset or constraining objects are invalid.
	\return CStatus::Fail failure
	*/
	CStatus	AddConstraint(	const CString& in_preset,
							const CRefArray& in_cnsning,
							bool in_bComp,
							const MATH::CVector3& in_offsetCnsned,
							const MATH::CVector3& in_offsetCnsning,
							Constraint& out_constraint);

	/*! Returns a KinematicState object containing the local transformation of the X3DObject object (that
	is, the transformation relative to the parent).
	\return The KinematicState object.
	\sa X3DObject::GetLocalTranslation, X3DObject::GetLocalScaling, X3DObject::GetLocalRotation,
		X3DObject::PutLocalTranslation, X3DObject::PutLocalScaling, X3DObject::PutLocalRotation
	*/
	KinematicState GetLocal() const;

	/*! Returns a KinematicState object containing the global transformation of the X3DObject object (that
	is, the transformation relative to the parent).
	\return The KinematicState object.
	*/
	KinematicState GetGlobal() const;

	private:
	Kinematics * operator&() const;
	Kinematics * operator&();
};

};

#endif // __XSIKINEMATICS_H__
