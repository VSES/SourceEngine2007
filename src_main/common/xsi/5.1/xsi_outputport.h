//*****************************************************************************
/*!
   \file xsi_outputport.h
   \brief OutputPort class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIOUTPUTPORT_H__
#define __XSIOUTPUTPORT_H__

#include <xsi_port.h>

namespace XSI {

class CValue;

//*****************************************************************************
/*! \class OutputPort xsi_outputport.h
	\brief An OutputPort object represents an output port of an Operator.

	\sa Port, InputPort, Operator
 */
//*****************************************************************************
class SICPPSDKDECL OutputPort : public Port
{
public:
	/*! Default constructor. */
	OutputPort();

	/*! Default destructor. */
	~OutputPort();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	OutputPort(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	OutputPort(const OutputPort& in_obj);

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
	empty if the Output object is not compatible.
	\param in_obj constant class object.
	\return The new Port object.
	*/
	OutputPort& operator=(const OutputPort& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the Output reference object is not compatible.
	\param in_ref constant class object.
	\return The new OutputPort object.
	*/
	OutputPort& operator=(const CRef& in_ref);

	/*! Returns the current value of the data attached to the port.
	\return Current value. The value is empty if the port is not connected or if the function failed.
	*/
	CValue GetValue() const;

	/*! Sets the current value of the data attached to the port.
	\param in_val new value for the data connected on this port.
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus PutValue( CValue in_val);

	/*! Returns true if the target connected to port is created when operator is applied, false otherwise
	\return true if target is created
	\since 4.0
	*/
	bool GetCreated() const;

	private:
	OutputPort * operator&() const;
	OutputPort * operator&();
};


};

#endif // __XSIOUTPUTPORT_H__
