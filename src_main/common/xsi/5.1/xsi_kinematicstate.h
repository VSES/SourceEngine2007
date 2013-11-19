//*****************************************************************************
/*!
   \file xsi_kinematicstate.h
   \brief KinematicState class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIKINEMATICSTATE_H__
#define __XSIKINEMATICSTATE_H__

#include <xsi_parameter.h>
#include <xsi_transformation.h>

namespace XSI {

//*****************************************************************************
/*! \class KinematicState xsi_kinematicstate.h
	\brief The KinematicState represents the current pose of an X3DObject object whereas the base
	pose of an object is represented by the StaticKinematicState.

	\sa Kinematics::GetGlobal, Kinematics::GetLocal, StaticKinematicState

	\eg
	\code
		using namespace XSI;
		Application app;
		Model root = app.GetActiveSceneRoot();

		Null myNull;
		root.AddNull( L"", myNull );

		app.LogMessage( myNull.GetKinematics().GetGlobal().GetClassIDName() );
	\endcode
 */
//*****************************************************************************

class SICPPSDKDECL KinematicState : public Parameter
{
public:
	/*! Default constructor. */
	KinematicState();

	/*! Default destructor. */
	~KinematicState();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	KinematicState(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	KinematicState(const KinematicState& in_obj);

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
	\return The new KinematicState object.
	*/
	KinematicState& operator=(const KinematicState& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new KinematicState object.
	*/
	KinematicState& operator=(const CRef& in_ref);

	/*! Returns the transformation of an object.
	\param in_dFrame Frame at which to get the transform, it defaults to the current frame (DBL_MAX indicates the current frame).
	\return The Transformation object.
	*/
	MATH::CTransformation GetTransform( double in_dFrame = DBL_MAX) const;

	/*! Sets the transformation of an object. Currently you cannot set the transform at a different time other than the current time.
	\param in_transf The transformation object.
	\param in_dFrame Frame at which to set the transform (not supported yet).
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus PutTransform( const MATH::CTransformation& in_transf,
		double in_dFrame = DBL_MAX );

	private:
	KinematicState * operator&() const;
	KinematicState * operator&();
};

};

#endif // __XSIKINEMATICSTATE_H__
