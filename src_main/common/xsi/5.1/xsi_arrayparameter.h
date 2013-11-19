//*****************************************************************************
/*!
   \file xsi_arrayparameter.h
   \brief ArrayParameter class declaration.

   © Copyright 1998-2003 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSI_ARRAYPARAMETER_H__
#define __XSI_ARRAYPARAMETER_H__

#include <xsi_status.h>
#include <xsi_parameter.h>

namespace XSI {

// *******************************************************************
/*! \class ArrayParameter xsi_arrayparameter.h
	\brief An ArrayParameter is a specialization of a Parameter.

	It can contain any number of sub-parameters, all of the same type. Its contents can be moved around, new
	parameters added and old ones removed.

	\note Currently only shaders support the notion of an array. The type of the parameters contained within
		the array are declared in the shader's SPDL.

	\sa Parameter
	\since 4.0
*/
// *******************************************************************

class SICPPSDKDECL ArrayParameter : public Parameter
{
public:
	/*! Constructs an ArrayParameter object. */
	ArrayParameter( );

	/*! Destroys an ArrayParameter object. */
	~ArrayParameter( );

	/*! Constructs an ArrayParameter object from a reference to another ArrayParameter object.
	\param in_ref A reference to a group.
	*/
	ArrayParameter(const CRef& in_ref);

	/*! Constructs a new ArrayParameter object from an existing ArrayParameter object.
	\param in_obj An existing Group object to copy into this Group object.
	*/
	ArrayParameter(const ArrayParameter& in_obj);

	/*! Returns true if this object supports the functionality of a specified class. For example,
	an ArrayParameter is a type of Parameter, so an ArrayParameter object supports Parameter functionality.
	\param in_ClassID Test if this object supports this class.
	\return True if this object supports the specified class, and false otherwise.
	*/
	bool IsA( siClassID in_ClassID) const;

	/*! Returns the class ID for this object.
	\return The class ID.
	*/
	siClassID GetClassID() const;

	/*! Assigns an ArrayParameter object to an existing ArrayParameter object.
	\param in_obj An ArrayParameter object to be copied into this object.
	\return The reinitialized ArrayParameter object.
	*/
	ArrayParameter& operator=(const ArrayParameter& in_obj);

	/*! Assigns a CRef to this ArrayParameter object. The ArrayParameter object is cleared if the CRef is not
	a reference to an object that supports the ArrayParameter class.
	\param in_ref A reference to an object that supports the ArrayParameter class.
	\return The reinitialized ArrayParameter object.
	*/
	ArrayParameter& operator=(const CRef& in_ref);

	/*! Returns the parameter at a given index. If the index is out of bounds, this operator returns an
	empty, invalid Parameter object.
	\param in_index Integer index of the parameter to get.
	\return The parameter at the given index
	\return Empty parameter if the index is out of bounds
	*/
	Parameter operator[]( LONG in_index );

	/*! Returns the size of the array. If the array is empty or invalid, GetCount returns 0.
	\return The number of parameters in the array.
	*/
	LONG GetCount( void ) const;

	/*! Appends a new parameter to this array. The type of the new parameter is determined by the SPDL
	definition of the array parameter.
	\return The new parameter.
	*/
	Parameter Add( void );

	/*! Removes a parameter from the array given its index.
	\param in_lIndex The index of the parameter to remove.
	\return CStatus::OK if succeeded.
	\return CStatus::Fail if the index is out of bounds.
	*/
	CStatus Remove( LONG in_lIndex );

	/*! Removes a parameter from the array.
	\param in_parameter Index of the parameter to remove.
	\return CStatus::OK if successful.
	\return CStatus::Fail if the parameter is not a member of the array.
	*/
	CStatus Remove( const Parameter &in_parameter );

	/*! Moves a parameter up or down in the array relative to its current position. The new position of
	the parameter is calculated by adding \c in_lUpDown to the current index and clamping the result to
	the array boundaries (0 to ArrayParameter::GetCount - 1).
	\param in_lIndex The index of the parameter to move.
	\param in_lUpDown Positive or negative amount to move the parameter.
	\return CStatus::OK if successful.
	\return CStatus::Fail if the index is out of bounds.
	*/
	CStatus Move( LONG in_lIndex, LONG in_lUpDown );

	/*! Moves a given parameter up or down in the array relative to its current position. The new position
	of the parameter is calculated by adding \c in_lUpDown to the current index and clamping the result to
	the array boundaries (0 to ArrayParameter::GetCount - 1).
	\param in_parameter The parameter to move.
	\param in_lUpDown Positive or negative amount to move the parameter.
	\return CStatus::OK if successful.
	\return CStatus::Fail if the given parameter is not a member of the array.
	*/
	CStatus Move( const Parameter &in_parameter, LONG in_lUpDown );

	/*! Appends a new parameter to the array. The parameter is initialized with the specified value. The
	parameter type is determined by the SPDL definition of the array. If the specified value does not match
	the type specified in the SPDL, Add returns an invalid Parameter object. Use this function to add items
	to shaders such as the camera lens flare shader, which have object and light lists.
	\return The new parameter.
	*/
	Parameter Add( const CValue& );

private:
	ArrayParameter* operator&() const;
	ArrayParameter* operator&();

};

};

#endif // __XSI_ARRAYPARAMETER_H__
