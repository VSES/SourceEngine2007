//*****************************************************************************
/*!
   \file xsi_updatecontext.h
   \brief UpdateContext class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIUPDATECONTEXT_H__
#define __XSIUPDATECONTEXT_H__

#include <xsi_siobject.h>
#include <xsi_value.h>
#include <xsi_status.h>

namespace XSI {

class Operator;

//*****************************************************************************
/*! \class UpdateContext xsi_updatecontext.h
	\brief This object represents the operator update context. It gives access to the operator and the
	evaluation time. The UpdateContext is only valid within the context of the Update() function for
	operators that are based on a Preset and SPDL. Self-Installed CustomOperators use the
	OperatorContext object instead.

	\sa Operator, OperatorContext
 */
//*****************************************************************************

class SICPPSDKDECL UpdateContext : public SIObject
{
public:
	/*! Default constructor. */
	UpdateContext();

	/*! Default destructor. */
	~UpdateContext();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	UpdateContext(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	UpdateContext(const UpdateContext& in_obj);

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
	\return The new UpdateContext object.
	*/
	UpdateContext& operator=(const UpdateContext& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new UpdateContext object.
	*/
	UpdateContext& operator=(const CRef& in_ref);

	private:
	UpdateContext * operator&() const;
	UpdateContext * operator&();

	public:

	/*! Returns the Operator object for the update context.
	\return The current Operator.
	*/
	Operator	GetOperator();

	/*! Returns the time in frames at which the Operator is being evaluated.
	\return Time (frame) at which the Operator is being evaluated.
	*/
	double		GetCurrentFrame();

	/*! Returns the user data stored in the UpdateContext.
	\return The user data.
	*/
	CValue		GetUserData();

	/*! Sets user data which is passed to the compiled scripted operator from evaluation to evaluation.
	\param in_val user data to be set for the operator instance.
	\return CStatus::OK success
	\return CStatus::Fail failure.
	*/
	CStatus 	PutUserData(const CValue& in_val);


};

};

#endif // __XSIUPDATECONTEXT_H__
