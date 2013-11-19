//*****************************************************************************
/*!
   \file xsi_constraint.h
   \brief Constraint class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSICONSTRAINT_H__
#define __XSICONSTRAINT_H__

#include <xsi_property.h>

namespace XSI {

class X3DObject;

//*****************************************************************************
/*! \class Constraint xsi_constraint.h
   \brief The Constraint object represents a constraint in the scene graph.

	Constraints allow you to constrain objects to other object's animation. For example, you can constrain an object
	to always point in the direction of another object using the \a Direction constraint. You apply constraints on
	objects using Kinematics::AddConstraint. You can find the constraints that are being applied to an object using
	Kinematics::GetConstraints() or find which constraint is driving a particular parameter using Parameter::GetSource.

	\sa Kinematics

	\eg Illustrates how to apply a direction constraint to a sphere so that it always points
		in the direction of the null. We then enumerate all the constraints on the sphere
		using the \c kinematics.contraints property.

	\code
		using namespace XSI;
		Application app;
		Model root = app.GetActiveSceneRoot();

		Null myNull;
		root.AddNull( L"", myNull );

		X3DObject mySphere;
		root.AddGeometry( L"Sphere", L"MeshSurface", L"", mySphere );

		Kinematics kine(mySphere.GetKinematics());

		CRefArray constrainings(1);
		constrainings[0] = myNull;

		Constraint dirCns;
		kine.AddConstraint( L"Direction", constrainings, false, MATH::CVector3(),
			MATH::CVector3(), dirCns );

		CRefArray constraints( kine.GetConstraints() );

		for ( LONG i=0; i<(LONG)constraints.GetCount(); i++ )
		{
			SIObject siobj( constraints[i] );
			app.LogMessage( siobj.GetName() );
		}
	\endcode
 */
//*****************************************************************************
class SICPPSDKDECL Constraint : public Property
{
public:
	/*! Default constructor. */
	Constraint();

	/*! Default destructor. */
	~Constraint();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	Constraint(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	Constraint(const Constraint& in_obj);

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
	\return The new Constraint object.
	*/
	Constraint& operator=(const Constraint& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new Constraint object.
	*/
	Constraint& operator=(const CRef& in_ref);

	/*! Returns the object being constrained by the constraining objects.
	\return The constrained object.
	*/
	X3DObject GetConstrained() const;

	/*! Returns an array of constraining objects that are used to constrain an object.
	\return Array of references to the constraining X3DObject objects.
	*/
	CRefArray GetConstraining() const;

	/*! Returns the object acting as the UpVector reference.
	\return UpVector reference object.
	*/
	X3DObject GetUpVectorReference() const;

	/*! Sets the object acting as the UpVector reference. The UpVector reference can be removed by passing in an empty reference.
	\param in_obj The UpVector reference object.
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus PutUpVectorReference( const X3DObject& in_obj );

	private:
	Constraint * operator&() const;
	Constraint * operator&();
};


};

#endif // __XSICONSTRAINT_H__
