//*****************************************************************************
/*!
   \file xsi_dictionary.h
   \brief Dictionary class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIDICTIONARY_H__
#define __XSIDICTIONARY_H__

#include <xsi_siobject.h>

namespace XSI {

//*****************************************************************************
/*! \class Dictionary xsi_dictionary.h
	The dictionary object contains application definitions such as Family objects. See ::siFamily for a list of
	available families.

	\note Not implemented.
 */
//*****************************************************************************
class SICPPSDKDECL Dictionary : public SIObject
{
public:
	/*! Default constructor. */
	Dictionary();

	/*! Default destructor. */
	~Dictionary();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	Dictionary(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	Dictionary(const Dictionary& in_obj);

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
	\return The new Dictionary object.
	*/
	Dictionary& operator=(const Dictionary& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new Dictionary object.
	*/
	Dictionary& operator=(const CRef& in_ref);

	private:
	Dictionary * operator&() const;
	Dictionary * operator&();
};

};

#endif // __XSIDICTIONARY_H__
