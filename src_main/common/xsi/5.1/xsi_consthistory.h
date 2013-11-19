//*****************************************************************************
/*
   \file xsi_consthistory.h
   \brief ConstructionHistory object class.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSICONSTRUCTIONHISTORY_H__
#define __XSICONSTRUCTIONHISTORY_H__

#include <xsi_siobject.h>

namespace XSI {

//*****************************************************************************
/*! \class ConstructionHistory xsi_consthistory.h
	The ConstructionHistory class represents the Operator stack under a Primitive. It allows you to browse through
	the stack one operator at a time by enumerating the ConstructionHistory, which you can get with
	Primitive::GetConstructionHistory.
	\warning Not implemented.
 */
//*****************************************************************************
class SICPPSDKDECL ConstructionHistory : public SIObject
{
public:
	/*! Default constructor. */
	ConstructionHistory();

	/*! Default destructor. */
	~ConstructionHistory();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	ConstructionHistory(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	ConstructionHistory(const ConstructionHistory& in_obj);

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
	\return The new ConstructionHistory object.
	*/
	ConstructionHistory& operator=(const ConstructionHistory& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new ConstructionHistory object.
	*/
	ConstructionHistory& operator=(const CRef& in_ref);

	private:
	ConstructionHistory * operator&() const;
	ConstructionHistory * operator&();
};

};

#endif // __XSICONSTRUCTIONHISTORY_H__
