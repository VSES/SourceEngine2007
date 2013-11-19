//*****************************************************************************
/*!
   \file xsi_statickinematicstate.h
   \brief StaticKinematicState class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSISTATICKINEMATICSTATETATE_H__
#define __XSISTATICKINEMATICSTATETATE_H__

#include <xsi_property.h>
#include <xsi_status.h>
#include <xsi_transformation.h>

namespace XSI {

//*****************************************************************************
/*! \class StaticKinematicState xsi_statickinematicstate.h
	\brief The StaticKinematicState object represents the base pose of an object. Objects that have access
	to the StaticKinematicState object include:

		\li X3DObject
		\li Cluster

	The static kinematic state on objects is created when applying envelopes, deform-by-spine or deform-by-curve
	operators. Currently an object can only have one static kinematic state. It represents a snapshot of the pose
	of the object when the %Envelope, DeformBySpine or DeformByCage operator was applied. It is also changed when
	doing Set Reference Pose in the Animate|Deform|%Envelope menu (see the \xl SetEnvelopeRefPoses command). If an
	object has all 3 operations applied on the same object they will all share the same static kinematic state.
	When doing a cage deform on top of an envelope for example the later operation will reset the static kinematic
	state. This means that it's usually better to be in the reference pose when adding these deforms (\xl ResetActor).

	The static kinematic state on clusters is created when the cluster is translated, rotated or scaled by the
	MoveComponent operator (see \xl MoveComponent_op). The static kinematic state is the transformation applied by
	the MoveComponent operator on the cluster. There is one or possibly two static kine state per move component on
	a cluster.  Therefore you end-up with multiple static kine on a cluster if you transform the cluster many times.
	The second static kine state on the cluster is created when moving clusters in relative mode.

	\sa Cluster::HasStaticKinematicState, Cluster::GetStaticKinematicStates, KinematicState,
		X3DObject::HasStaticKinematicState, X3DObject::GetStaticKinematicState

	\eg
	\code
		using namespace XSI;
		Application app;
		Model root = app.GetActiveSceneRoot();

		Null myNull;
		root.AddNull( L"", myNull );

		KinematicState nullGlobalKineState(myNull.GetKinematics().GetGlobal());
		Parameter param(nullGlobalKineState.GetParameters().GetItem(L"posx"));
		param.PutValue( (double)4.0 );

		param = nullGlobalKineState.GetParameters().GetItem(L"posy");
		param.PutValue( (double)3.0 );

		X3DObject mySphere;
		root.AddGeometry( L"Sphere", L"MeshSurface", L"", mySphere );
		param = mySphere.GetKinematics().GetGlobal().GetParameters().GetItem(L"posz");
		param.PutValue( (double)2.0 );

		CRefArray deformers(1);
		deformers[0] = myNull;

		Envelope myEnvelop;
		mySphere.ApplyEnvelope(	deformers, siUnspecified, siUnspecified, myEnvelop );
		app.LogMessage( mySphere.GetStaticKinematicState().GetClassIDName() );
	\endcode
 */
//*****************************************************************************

class SICPPSDKDECL StaticKinematicState : public Property
{
public:
	/*! Default constructor. */
	StaticKinematicState();

	/*! Default destructor. */
	~StaticKinematicState();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	StaticKinematicState(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	StaticKinematicState(const StaticKinematicState& in_obj);

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
	\return The new StaticKinematicState object.
	*/
	StaticKinematicState& operator=(const StaticKinematicState& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new StaticKinematicState object.
	*/
	StaticKinematicState& operator=(const CRef& in_ref);

	/*! Returns the base pose from an object.
	\param in_dFrame Frame at which to get the transform.
	\return The transformation object.
	*/
	MATH::CTransformation GetTransform(double in_dFrame = 0) const;

	/*! Sets the base pose of an object.
	\param in_transf The transformation object.
	\param in_dFrame Frame at which to set the transform.
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus	PutTransform( const MATH::CTransformation& in_transf,
		double in_dFrame = 0);

	private:
	StaticKinematicState * operator&() const;
	StaticKinematicState * operator&();
};

};

#endif // __XSISTATICKINEMATICSTATETATE_H__
