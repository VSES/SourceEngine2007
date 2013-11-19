//*****************************************************************************
/*!	\file xsi_pluginitem.h
	\brief PluginItem class declaration.

	© Copyright 1998-2003 Avid Technology, Inc. and its licensors. All rights
	reserved. This file contains confidential and proprietary information of
	Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
	end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIPLUGINITEM_H__
#define __XSIPLUGINITEM_H__

#include <xsi_siobject.h>

namespace XSI {

//*****************************************************************************
/*! \class PluginItem xsi_pluginitem.h
	\brief The PluginItem class contains the information on each item of a plug-in.

	A plug-in item is basically an XSI customizable component such as:

		\li Filter
		\li Command
		\li Operator
		\li   etc.

	\since 4.0
 */
//*****************************************************************************

class SICPPSDKDECL PluginItem : public SIObject
{
public:
	/*! Default constructor. */
	PluginItem();

	/*! Default destructor. */
	~PluginItem();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	PluginItem(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	PluginItem(const PluginItem& in_obj);

	/*! Returns true if a given class type is compatible with this API class.
	\param in_ClassID class type.
	\return true if the class is compatible, false otherwise.
	*/
	bool IsA( siClassID in_ClassID) const;

	/*! Returns the type of the API class.
	\return The class type.
	*/
	siClassID GetClassID() const;

	/*! Creates an object from another object. The new object is set to
	empty if the input object is not compatible.
	\param in_obj constant class object.
	\return The new PluginItem object.
	*/
	PluginItem& operator=(const PluginItem& in_obj);

	/*! Creates an object from a reference object. The new object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new PluginItem object.
	*/
	PluginItem& operator=(const CRef& in_ref);

	/*! Sets the help path for this plug-in item
	 \param in_strHelp The help path
	 \return CStatus OK if successful
	 \return CStatus Fail if not successful
	 */
	CStatus PutHelp( const CString& in_strHelp );

	/*! Sets the categories for this plug-in item
	 \param in_strCat A comma-separated string representing the categories of
	 this plug-in item.
	 \return CStatus OK if successul
	 */
	CStatus PutCategories( const CString& in_strCat );

	private:
	PluginItem * operator&() const;
	PluginItem * operator&();
};

};

#endif // __XSIPluginItem_H__
