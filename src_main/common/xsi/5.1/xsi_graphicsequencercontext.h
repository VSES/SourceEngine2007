//*****************************************************************************
/*!
   \file xsi_graphicsequencercontext.h
   \brief GraphicSequencerContext class declaration.

   © Copyright 1998-2003 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIGRAPHICSEQUENCERCONTEXT_H__
#define __XSIGRAPHICSEQUENCERCONTEXT_H__

#include <xsi_base.h>
#include <xsi_value.h>
#include <xsi_graphicsequencer.h>

namespace XSI {

//*****************************************************************************
/*! \class GraphicSequencerContext xsi_graphicsequencercontext.h
	\brief This class represents the plug-in context for the Graphic Sequencer (CGraphicSequencer).

	\sa Context, CGraphicSequencer
	\since 4.0
*/
//*****************************************************************************

class SICPPSDKDECL GraphicSequencerContext : public CBase
{
public:

	/*! Default constructor. */
	GraphicSequencerContext();

	/*! Default destructor. */
	~GraphicSequencerContext();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	GraphicSequencerContext(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	GraphicSequencerContext(const GraphicSequencerContext& in_obj);

	/*! Returns true if a given class type is compatible with this API class.
	\param in_ClassID class type.
	\return true if the class is compatible, false otherwise.
	*/
	bool IsA( siClassID in_ClassID) const;

	/*! Returns the type of the API class.
	\return The class type.
	*/
	siClassID GetClassID() const;

	/*! Creates an object from another object.
	\param in_obj constant class object.
	\return The new Context object.
	*/
	GraphicSequencerContext& operator=(const GraphicSequencerContext& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new Context object.
	*/
	GraphicSequencerContext& operator=(const CRef& in_ref);

	/*! Returns the user data stored in the context.
	\return The raw user data.
	*/
	CValue		GetUserData();

	/*! Allows you to store a CValue inside the context object. This is called \a User \a Data because %XSI does not care about the
	content or meaning of the data.  This information is not persisted.  A common use for this feature is to store a pointer to a
	newly allocated C++ object during an \c Init callback,and then to use Context::GetUserData to retrieve the pointer when other
	callbacks are made.
	\note In the case of storing a pointer in the \c Init callback, the pointer should be destroyed in the \c Term callback.
	\param in_val User data to be set for the operator instance.
	\return CStatus::OK success
	\return CStatus::Fail failure.
	 */
	CStatus 	PutUserData(const CValue& in_val);

	/*! Returns the graphic sequencer object (CGraphicSequencer).
	\return The CGraphicSequencer object.
	*/
	CGraphicSequencer	GetGraphicSequencer();

	private:
	GraphicSequencerContext * operator&() const;
	GraphicSequencerContext * operator&();
};

};
#endif // __XSIGRAPHICSEQUENCERCONTEXT_H__
