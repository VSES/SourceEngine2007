//*****************************************************************************
/*!
   \file xsi_ppgitem.h
   \brief PPGItem class declaration.

   © Copyright 1998-2003 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIPPGITEM_H__
#define __XSIPPGITEM_H__

#include <xsi_base.h>
#include <xsi_value.h>

namespace XSI {

//*****************************************************************************
/*! \class PPGItem xsi_ppgitem.h
	\brief This object represents a component of the user interface of a %Property Page.

	Normally a %PPGItem represents a user interface control that is directly related to a Parameter of the
	object that is inspected. For example a %PPGItem of type ::siControlString would display the value of a
	string parameter.  Similarly a %PPGItem of type ::siControlFCurve would show the FCurve contained inside
	a FCurve Parameter.  The connection between the control and its associated Parameter is established based
	on the the PPGItem's Name (see PPGItem::GetName).

	Some PPGItems do not directly correspond to a parameter, for example Buttons, Tabs and Groups.

	Each PPGItem exposes a series of "attributes" that can be used to tweak its appearance. Some of the most
	common attributes are exposed directly as properties of the object, for example PPGItem::GetLabel, whereas
	the more specialized or uncommon attributes can be accessed via PPGItem::GetAttribute.

	PPGItem objects are accessible via the PPGLayout of a ProjectItem.

	\sa ProjectItem::GetPPGLayout, PPGLayout::AddItem, PPGLayout::GetItem
	\since 4.0

	\eg
	\code
		using namespace XSI;
		Application app;
		Model root = app.GetActiveSceneRoot();

		// Step 1: Create a custom pset and define the data on it
		CustomProperty oPSet ;
		root.AddProperty( L"CustomProperty", false, L"Demo", oPSet) ;

		Parameter param ;
		oPSet.AddParameter( L"Check1", CValue::siBool, siPersistable, L"", L"", CValue(), param ) ;
		oPSet.AddParameter( L"Check2", CValue::siBool, siPersistable, L"", L"", CValue(), param ) ;

		PPGLayout oPPGLayout = oPSet.GetPPGLayout() ;
		oPPGLayout.AddGroup( L"", true ) ;
		oPPGLayout.AddRow() ;
		oPPGLayout.AddItem( L"Check1" ) ;

		PPGItem oItem = oPPGLayout.AddItem( L"Check2" ) ;
		oItem.PutLabel( L"Check box 2" ) ;

		oPPGLayout.EndRow() ;
		oPPGLayout.EndGroup() ;

		//We can look up and change the items afterwards
		oItem = oPPGLayout.GetItem( L"Check1" ) ;
		oItem.PutLabel( L"Check Box 1" ) ;

		//Show our PPG
		CValueArray args(5) ;
		args[0] = oPSet ;

		CValue retval = false ;
		app.ExecuteCommand( L"InspectObj", args, retval ) ;
	\endcode

	\eg Shows a property page with controls for picking files and folders. This sort of UI is often
	used in import/export plug-ins.
	\code
		using namespace XSI;
		Application app;
		Model root = app.GetActiveSceneRoot();

		CustomProperty oPSet ;
		root.AddProperty( L"CustomProperty", false, L"Demo", oPSet) ;

		Parameter param ;
		oPSet.AddParameter( L"ExportFile", CValue::siString, siPersistable, L"", L"", CValue(), param ) ;
		oPSet.AddParameter( L"ImportFile", CValue::siString, siPersistable, L"", L"", CValue(), param ) ;
		oPSet.AddParameter( L"FolderPicker", CValue::siString, siPersistable, L"", L"", CValue(), param ) ;

		PPGLayout oPPGLayout = oPSet.GetPPGLayout() ;
		PPGItem oItem = oPPGLayout.AddItem( L"ExportFile", L"Export Filename", siControlFilePath ) ;

		// You can use the string directly, or the typdef siUIInitialDir (see siPPGItemAttribute enum)
		oItem.PutAttribute( L"initialdir", L"project" ) ;
		oItem.PutAttribute( siUIFileFilter, L"Text files (*.txt)|*.txt|All Files (*.*)|*.*||" ) ;

		oItem = oPPGLayout.AddItem( L"ImportFile", L"", siControlFilePath ) ;

		oItem.PutAttribute( siUIFileFilter, L"xsiaddon files (*.xsiaddon)|*.xsiaddon|"
											L"spdl files (*.spdl)|*.spdl|"
											L"All Files (*.*)|*.*||" ) ;
		oItem.PutAttribute( siUIOpenFile, true ) ;
		oItem.PutAttribute( siUIFileMustExist, true ) ;

		oPPGLayout.AddItem( L"FolderPicker", L"", siControlFolder ) ;

		CValueArray args(5) ;
		args[0] = oPSet ;
		CValue retval = false ;
		app.ExecuteCommand( L"InspectObj", args, retval ) ;
	\endcode
*/
//*****************************************************************************

class SICPPSDKDECL PPGItem : public CBase
{
public:
	/*! Default constructor. */
	PPGItem();

	/*! Default destructor. */
	~PPGItem();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	PPGItem(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	PPGItem(const PPGItem& in_obj);

	/*! Returns true if a given class type is compatible with this API class.
	\param in_ClassID class type.
	\return  true if the class is compatible, false otherwise.
	*/
	bool IsA( siClassID in_ClassID) const;

	/*! Returns the type of the API class.
	\return  The class type.
	*/
	siClassID GetClassID() const;

	/*! Creates an object from another object.
	\param in_obj constant class object.
	\return  The new PPGItem object.
	*/
	PPGItem& operator=(const PPGItem& in_obj);

	/*! Creates an object from a reference object. The newly created object is set to empty if the input
	reference object is not compatible.
	\param in_ref constant class object.
	\return  The new PPGItem object.
	*/
	PPGItem& operator=(const CRef& in_ref);

	/*! Returns the name of the item.  For most controls the name matches the script name of the parameter
	that is inspected by the control (see Parameter::GetScriptName). For ::siControlRGBA this is the name
	of the parameter that represents the Red Component of the color (see PPGLayout::AddColor).
	\sa PPGItem::PutName, PPGItem::GetLabel, Parameter::GetScriptName, ProjectItem::GetPPGLayout
	*/
	CString GetName() const ;

	/*! Sets the name of the item.
	\sa PPGItem::GetName, Parameter::GetScriptName, ProjectItem::GetPPGLayout
	*/

	CStatus PutName( const CString& in_Name ) ;

	/*! Returns the label of the PPGItem.
	\sa PPGItem::PutLabel, PPGItem::GetName, Parameter::GetScriptName, PPGLayout::AddItem
	*/
	CString GetLabel() const ;

	/*! Returns the label of the PPGItem.  For many controls this text appears to the left
	of the control.  When not specified the name of the parameter (see SIObject::GetName and
	or Parameter::GetScriptName) is displayed.

	For convenience methods like PPGLayout::AddItem allow you to specify
	the label without an extra call to this method.

	This attribute is optional for Groups and Buttons and mandatory for Tabs.  See
	PPGLayout::AddGroup, PPGLayout::AddButton and
	PPGLayout::AddTab.

	To draw a control without any label use the siUINoLabel or siUIValueOnly flags.
	(See ::siPPGItemAttribute)

	\sa PPGItem::GetLabel, PPGItem::GetName, Parameter::GetScriptName, PPGLayout::AddItem
	*/
	CStatus PutLabel( const CString& in_Label ) ;

	/*! Returns the control type to display on the %Property Page.
	\return  String containing one of the strings in the ::siPPGControlType enum.
	\sa PPGItem::PutType
	*/
	CString GetType() const ;

	/*! Sets the control type to display on the %Property Page.  This
	property is optional - when not set %XSI picks a control
	based on the type of the parameter (see Parameter::GetValueType).
	For example a siDouble uses a siControlNumber.

	\param in_Type CString containing one of the strings in the ::siPPGControlType enum
	\sa PPGLayout::AddItem
	*/
	CStatus PutType( const CString& in_Type ) ;

	/*! Returns the list of items and values associated with a control.
	This property is used by controls which display items from a fixed list of choices, for example
	siControlCombo, siControlCheck, siControlRadio, and siControlIconList.

	\return  CValueArray containing the label/value pairs
	\sa	PPGLayout::AddEnumControl, PPGItem::PutUIItems
	*/
	CValueArray GetUIItems() const ;

	/*! Sets the list of items and values associated with a control
	as a 1-dimensional array of Label/Value pairs.  This property is used by
	controls which display items from a fixed list of choices, for example
	siControlCombo, siControlCheck, siControlRadio, and siControlIconList.
	Each label is a string and the value is a CValue whose type should match
	the type of the associated parameter (see Parameter::GetValueType).

	\param in_items CValueArray containing a label and value for each item
	\sa	PPGLayout::AddEnumControl, PPGItem::GetUIItems
	*/
	CStatus PutUIItems( const CValueArray & in_items ) ;

	/*! Returns the WidthPercentage.
	\sa PPGItem::PutWidthPercentage
	*/
	LONG GetWidthPercentage() const ;

	/*! Returns the WidthPercentage. This property is only applicable
	if the group is part of a row. It gives a hint to %XSI about how
	much of the horizontal space should be taken by this item. If not specified
	%XSI uses its own heuristic to lay out the row.

	\sa PPGLayout::AddGroup, PPGItem::GetWidthPercentage
	*/
	CStatus PutWidthPercentage(LONG in_percent) ;

	/*! Returns the width of the label of a control, in terms of its
	percentage of the control's total width.
	\sa	PPGItem::GetLabelMinPixels, PPGItem::GetWidthPercentage, PPGItem::GetAttribute
	*/
	LONG GetLabelPercentage() const ;

	/*! Sets the width of the label of a control, in terms of its
	percentage of the control's total width.  When not set %XSI will
	use its own heuristic, but this can be useful to give a hint when tweaking
	the layout.  For example this can be useful to force a LONG
	label onto multiple lines rather than squish the controls.

	To completely hide a label use the siUINoLabel or siValueOnly UI Attribute
	(see ::siPPGItemAttribute).

	This function should always be used in conjunction with PPGItem::PutLabelMinPixels.

	\sa PPGItem::GetLabelPercentage, PPGItem::PutLabelMinPixels, PPGItem::PutWidthPercentage,
	PPGItem::PutAttribute
	*/
	CStatus PutLabelPercentage(LONG in_percent) ;

	/*! Returns the minimum width, in pixels, to use when drawing a label.
	\sa PPGItem::GetLabelPercentage, PPGItem::PutLabelMinPixels, PPGItem::GetAttribute
	*/
	LONG GetLabelMinPixels() const ;

	/*! Sets the minimum width, in pixels, to use when drawing a label.
	A label will get split into multiple lines when there is not enough
	space on a single line.

	This function should always be used in conjunction with
	PPGItem::PutLabelPercentage

	\sa PPGItem::PutLabelPercentage, PPGItem::GetLabelMinPixels, PPGItem::PutAttribute
	*/

	CStatus PutLabelMinPixels(LONG in_pixels) ;

	/*! Sets an attribute on the control.  Each control
	has a collection of attributes which can be used to tweak its
	appearance or to provide it with additional states.  Some of
	these attributes are general and apply to almost any control,
	for example siUIAlignCenter.  Others are specific to the
	control, for example siUINoSlider (which only applies to
	numeric controls).

	If the attribute argument is not a recognized attribute this method
	fails.  It is possible to remove an attribute by calling this
	method with an empty CValue as the AttributeValue argument.

	For convenience, some of the most common attributes have been exposed
	directly as properties, for example PPGItem::PutLabel
	and PPGItem::PutUIItems.

	\param in_UIAttribute CString specifying which attribute to set.  The
		::siPPGItemAttribute enum provides a complete list of supported attributes.
	\param in_AttributeValue CValue with the new value for the attribute.

	\sa PPGItem::GetAttribute, PPGLayout::GetAttribute
	*/

	CStatus PutAttribute ( const CString& in_UIAttribute,
						   const CValue& in_AttributeValue );

	/*! Returns an attribute on the control.  Each control
	has a collection of attributes which can be used to tweak its
	appearance or to provide it with additional states.  Some of
	these attributes are general and apply to almost any control,
	for example siUIAlignCenter.  Others are specific to the
	control, for example siUINoSlider (which only applies to
	numeric controls).

	If the attribute argument is not a recognized attribute the call
	to read it fails.  Most of the attributes are optional and
	%XSI returns an empty CValue if the attribute has not been set.

	For convenience, some of the most common attributes have been exposed
	directly as properties, for example PPGItem::GetLabel
	and PPGItem::GetUIItems.

	\param in_UIAttribute CString specifying which attribute to get.  The
		::siPPGItemAttribute enum provides a complete list of supported attributes.
	\return  CValue containing the attribute value.  An empty
		CValue is returned if the attribute has not been set.

	\sa PPGItem::GetAttribute, PPGLayout::GetAttribute

	\eg
	\code
		using namespace XSI;
		Application app;
		Model root = app.GetActiveSceneRoot();

		// Create a simple pset with two string parameters
		CustomProperty oPSet ;
		root.AddProperty( L"CustomProperty", false, L"Demo", oPSet) ;

		Parameter oParam ;
		oPSet.AddParameter( L"P1", CValue::siString,
							siPersistable, L"", L"", CValue(), oParam ) ;
		oPSet.AddParameter( L"P2", CValue::siString,
							siPersistable, L"", L"", CValue(), oParam ) ;

		// Set out the layout.  We don't need to set any attributes
		// because of convenience arguments in AddItem and AddString

		PPGLayout oLayout = oPSet.GetPPGLayout();
		oLayout.AddItem( L"P1", L"P1 Label", siControlString ) ;
		oLayout.AddString( L"P2", L"P2 Label", true, 400 ) ;

		// Look under the hood at some of the attributes that were set
		// by the above calls

		PPGItem oItem1 = oLayout.GetItem( L"P1" ) ;
		app.LogMessage( L"P1-Attribute:Label        Value:" +
						oItem1.GetAttribute( siUILabel ).GetAsText() ) ;
		app.LogMessage( L"P1-Attribute:Type         Value:" +
						oItem1.GetAttribute( siUIType ).GetAsText() ) ;

		// Label and Type can also be read via the convenient methods
		app.LogMessage( L"P1-GetLabel               Value:" + oItem1.GetLabel() ) ;
		app.LogMessage( L"P1-GetType                Value:" + oItem1.GetType() ) ;

		// When we use AddString we get some other interesting attributes automatically
		// set for us
		PPGItem oItem2 = oLayout.GetItem( L"P2" ) ;
		app.LogMessage( L"P2-Attribute:Label		Value:" +
						oItem2.GetAttribute( siUILabel ).GetAsText() ) ;

		// Type on a multi-line is "edit" instead of the "string" control
		// used on normal single line strings.
		app.LogMessage( L"P2-Attribute:Type         Value:" +
						oItem2.GetAttribute( siUIType ).GetAsText() ) ;
		app.LogMessage( L"P2-Attribute:CY(height)   Value:" +
						oItem2.GetAttribute( siUICY ).GetAsText() ) ;

		// This is a low-level attribute containing the specific window style
		// In most cases you would never need to read or set this value
		app.LogMessage( L"P2-Attribute:Style        Value:" +
						oItem2.GetAttribute( siUIStyle ).GetAsText() ) ;
	\endcode
	*/
	CValue GetAttribute ( const CString& in_UIAttribute ) const;

	private:
	PPGItem * operator&() const;
	PPGItem * operator&();
};

};
#endif // __XSIPPGITEM_H__
