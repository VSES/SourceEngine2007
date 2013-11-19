//*****************************************************************************
/*!
   \file xsi_animsource.h
   \brief AnimationSource class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIANIMSOURCE_H__
#define __XSIANIMSOURCE_H__

#include <xsi_siobject.h>

namespace XSI {

//*****************************************************************************
/*! \class AnimationSource xsi_animsource.h
	\brief The AnimationSource object represents a special kind of \xl DataSource, for example, ActionSource,
	Constraint, FCurve, Expression, or StaticSource.

	The valid types of AnimationSources are listed in the ::siAnimationSourceItemType enum documentation.

	\sa AnimationSourceItem, GetSource, ActionSource, Constraint, FCurve, StaticSource, Parameter::GetSource
 */
//*****************************************************************************
class SICPPSDKDECL AnimationSource : public SIObject
{
public:
	/*! Default constructor. */
	AnimationSource();

	/*! Default destructor. */
	~AnimationSource();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	AnimationSource(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	AnimationSource(const AnimationSource& in_obj);

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
	\return The new AnimationSource object.
	*/
	AnimationSource& operator=(const AnimationSource& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new AnimationSource object.
	*/
	AnimationSource& operator=(const CRef& in_ref);

	private:
	AnimationSource * operator&() const;
	AnimationSource * operator&();
};

};

#endif // __XSIANIMSOURCE_H__
