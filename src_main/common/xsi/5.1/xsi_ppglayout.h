//*****************************************************************************
/*!
   \file xsi_ppglayout.h
   \brief PPGLayout class declaration.

   © Copyright 1998-2003 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIPPGLAYOUT_H__
#define __XSIPPGLAYOUT_H__

#include <xsi_base.h>
#include <xsi_ppgitem.h>
#include <xsi_value.h>
namespace XSI {

//*****************************************************************************
/*! \class PPGLayout xsi_ppglayout.h
	\brief The %PPGLayout object represents the visual appearance of an object's state when it is inspected.
	(PPG is a common acronym for %Property Page). The layout makes it possible to group objects together, use
	special controls, hide obsolete parameters, add Buttons and in general create a presentation of an object's
	state that is pleasing to the eye.

	A PPGLayout is modeled in XSI as a linear list of PPGItem objects.  Each PPGItem represents either a parameter
	in the inspected object, or a special UI item like a Tab or Button.  In most cases the layout does not specify
	hard-coded pixel positions or dimensions for the PPGItems.  Instead %XSI automatically draws the controls with
	a sensible layout.  This allows the presentation to change in a pleasing manner when the user resizes the
	%property page.  However there is some support for providing hints, for example PPGItem::PutWidthPercentage, or
	even explicit sizes via PPGItem::PutAttribute.

	Many different controls are available for use on a %property page, see ::siPPGControlType.  All controls can be
	added using PPGLayout::AddItem but convenience functions are provided for some of the most common controls, for
	example PPGLayout::AddColor.

	Practically every built-in %XSI object has its own layout.  However, if there is no specific layout, %XSI displays
	a default layout that lists all the parameters in a simple fashion.

	Layouts are remembered in memory while %XSI is running.  The layouts for built-in objects are reloaded from SPDL
	files when %XSI restarts.  The layout for a CustomProperty object implemented as a PluginItem is defined by
	implementing the DefineLayout callback.

	You cannot directly modify an existing layout structure, apart from adding new items at the end or changing the
	attributes of items already in the layout. However to modify a layout it is often reasonable to call PPGLayout::Clear
	and then rebuild the entire layout with the changes included.  You can also hide parameters without removing them
	from the layout by setting the ::siNotInspectable flag (see Parameter::PutCapabilityFlag).

	\note If the layout is changed from within the %PPGEvent callback it is necessary to set the "Refresh" attribute
	to see the changes take effect. (see PPGEventContext).

	Normally the layout is associated with an object that remains in the scene.  However it is also possible to use this
	object to define the user interface of a temporary dialog box. For example a script can ask the user to specify some
	import or export options. This is done in conjunction with a temporary CustomProperty object and the \xl InspectObj
	command.

	\sa PPGItem, ProjectItem::GetPPGLayout, PPGEventContext, PluginRegistrar::RegisterProperty
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

		// Color is made up of 4 components
		oPSet.AddParameter( L"MyR", CValue::siDouble, siPersistable, L"", L"", 0.0,0.0,1.0,0.0,1.0, param ) ;
		oPSet.AddParameter( L"MyG", CValue::siDouble, siPersistable, L"", L"", 0.0,0.0,1.0,0.0,1.0, param ) ;
		oPSet.AddParameter( L"MyB", CValue::siDouble, siPersistable, L"", L"", 0.0,0.0,1.0,0.0,1.0, param ) ;
		oPSet.AddParameter( L"MyA", CValue::siDouble, siPersistable, L"", L"", 0.0,0.0,1.0,0.0,1.0, param ) ;

		oPSet.AddParameter( L"EnumCtrl", CValue::siInt4, siPersistable, L"", L"", 4l,4l,6l,4l,6l, param ) ;

		PPGLayout oPPGLayout = oPSet.GetPPGLayout() ;
		oPPGLayout.AddGroup( L"Pick a Color" ) ;

		PPGItem oItem = oPPGLayout.AddColor( L"MyR", L"", true ) ;
		oItem.PutAttribute( siUINoLabel, true ) ;

		oPPGLayout.EndGroup() ;

		CValueArray uiItems(4) ;
		uiItems[0] = L"item 1" ; uiItems[1] = 4l;
		uiItems[2] = L"item 2" ; uiItems[3] = 6l;

		oItem = oPPGLayout.AddEnumControl(
						L"EnumCtrl",
						uiItems,
						L"Choice",
						siControlCombo ) ;


		// Step 3: Show the user the dialog.  They can change the
		// parameter values and then click OK or Cancel

		CValueArray args(5) ;
		args[0] = oPSet ;
		args[2] = L"Check out this layout created from C++" ;
		args[3] = (LONG)siModal ;
		args[4] = false ;

		CValue retval ;

		app.ExecuteCommand( L"InspectObj", args, retval ) ;

		if ( !(bool)retval )
		{
			// Step 4: Read the values and do something with them.  Normally this
			//	would be where you call a custom command which does the operation

			app.LogMessage( L"User picked the color (" +
					oPSet.GetParameterValue( L"MyR" ).GetAsText() +	L"," +
					oPSet.GetParameterValue( L"MyG" ).GetAsText() + L"," +
					oPSet.GetParameterValue( L"MyB" ).GetAsText() + L"," +
					oPSet.GetParameterValue( L"MyA" ).GetAsText() +
					L")" ) ;

			if ( oPSet.GetParameterValue( L"EnumCtrl" ) == 4l )
			{
				app.LogMessage( L"User selected item 1" ) ;
			}
			else
			{
				app.LogMessage( L"User selected item 2" ) ;
			}
		}
		else
		{
			app.LogMessage( L"User cancelled" ) ;
		}

		//Step 5: Remove the pset now that we are done with it

		CValueArray deleteargs(1) ;
		deleteargs[0] = oPSet ;
		app.ExecuteCommand( L"DeleteObj", deleteargs, retval ) ;
	\endcode
*/
//*****************************************************************************

class SICPPSDKDECL PPGLayout : public CBase
{
public:
	/*! Default constructor. */
	PPGLayout();

	/*! Default destructor. */
	~PPGLayout();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	PPGLayout(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	PPGLayout(const PPGLayout& in_obj);

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
	\return The new PPGLayout object.
	*/
	PPGLayout& operator=(const PPGLayout& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new PPGLayout object.
	*/
	PPGLayout& operator=(const CRef& in_ref);

	/*! Adds a control to the PPGLayout.  Normally this new item represents a parameter on the object that is
	inspected, and allows a user to view and change its value.  For example this function can be used to add a
	Slider control which shows the value of a numeric parameter on a CustomProperty.

	It is possible to add any kind of control using this function.  However convenience functions such as
	PPGLayout::AddColor have been added for some of the most common controls.

	Often a call to this function is followed by one or more calls to PPGItem::PutAttribute
	to provide additional information about the appearance of the item.

	\param in_ParamName Scripting name of the Parameter.  The call does not fail even if no parameter
		by this name exists.  However in that case the control is not drawn as part of the layout.
		(See PPGItem::GetName).

	\param in_Label Most controls have a label, for example the text that appears to the left of a
		numeric slider.  If not specified here, the value returned from SIObject::GetName or
		Parameter::GetScriptName is shown instead. (See PPGItem::GetLabel).

	\param in_Type Specify the control to draw.  The ::siPPGControlType enum provides a complete list
		of possible controls. For example this argument can be used to specify that a parameter should
		be displayed as a combo box. (See PPGItem::GetType).  If not specified, %XSI uses a simple
		heuristic for determining what control to use, based on the Parameter::GetValueType.  For example
		strings have an edit control, numeric types like float and integer use a slider, and boolean
		parameters use a checkbox.

	\return The newly created PPGItem.
	\sa PPGItem::GetLabel, Parameter::GetScriptName
	*/
	PPGItem AddItem(const CString& in_ParamName, const CString& in_Label = CString(), const CString& in_Type = CString());


	/*! Adds a string to the PPGLayout.  This is very similar to PPGLayout::AddItem but it makes it easier to specify a
	multi-line string control.

	\param in_ParamName Scripting name of the Parameter.  The call does not fail even if no parameter by
		this name exists.  However in that case the control is not drawn as part of the layout.
		(See PPGItem::GetName).
	\param in_Label Most controls have a label, for example the text that appears to the left of a numeric
		slider.  If not specified here, the value returned from SIObject::GetName or Parameter::GetScriptName
		is shown instead. (See PPGItem::GetLabel).
	\param in_Multiline When True, the value of the string parameter is represented with a multi-line edit
		control.  This control is useful for allowing users to write script code or free form text. And, when
		the control is read-only, it is useful for showing detailed instructions to a user.  The control
		supports scroll bars in the cases where text is larger than the control's width or height. A multi-line
		edit has siControlEdit as its PPGItem::GetType and uses the siUICX, siUIStyle and siUIClass attributes
		(see PPGItem::GetAttribute).
	\param in_Height This parameter only applies when the MultiLine argument is true. It specifies the height,
		in pixels, of the multi-line control.

	\note It is not necessary to set the width because the control automatically resizes according to the
		size of the %property page.

	\sa PPGLayout::AddItem

	\eg
	\code
		using namespace XSI;
		Application app;
		Model root = app.GetActiveSceneRoot();

		CustomProperty oPSet ;
		root.AddProperty( L"CustomProperty", false, L"DescriptionPSet", oPSet) ;

		Parameter oParam ;
		oPSet.AddParameter( L"Description", CValue::siString, siPersistable, L"",L"",L"", oParam ) ;
		oPSet.GetPPGLayout().AddString( L"Description", L"", true, 200l ) ;
		oPSet.PutParameterValue( L"Description", CValue( L"Here is some\r\n\tmulti-line text" ) ) ;

		CValueArray args(5) ;
		args[0] = oPSet ;

		CValue retval = false ;
		app.ExecuteCommand( L"InspectObj", args, retval ) ;
	\endcode

	*/
	PPGItem AddString (const CString& in_ParamName, const CString& in_Label = CString(), bool in_Multiline = false, LONG in_Height = 120);

	/*! This is similar to PPGLayout::AddItem except it makes it easy to specify enumeration style controls like ::siControlCombo,
	::siControlRadio, and ::siControlIconList. These are controls which restrict the values that a users can set on a Parameter by
	offering a specific list of valid values.  Any numeric or string parameter type can be driven by these controls.

	\param in_ParamName Scripting name of the Parameter.  The call does not fail even if no parameter by this name
		exists.  However in that case the control is not drawn as part of the layout.  (See PPGItem::GetName).
	\param in_UIItemsArray A 1-Dimensional array of Label/Value pairs.  Each label is a CValue containing a CString
		and the value is a CValue whose type should match the Parameter::GetValueType of the associated Parameter.
		For example, for a bitfield, the array might contain ( L"Bit 0", 1l, L"Bit 1", 2l, L"Bit 2", 4l ).
	\param in_Label Most controls have a label, for example the text that appears to the left of a numeric slider.
		If not specified here, the value returned from SIObject::GetName or Parameter::GetScriptName is shown
		instead. (See PPGItem::GetLabel).
	\param in_UIType Specify the control to draw.  The ::siPPGControlType enum provides a complete list of possible
		controls, but in the case of this function only certain controls, such as ::siControlCombo, ::siControlRadio,
		::siControlCheck, and ::siControlIconList are supported.

	\sa PPGLayout::AddItem, PPGItem::GetUIItems
	*/
	PPGItem AddEnumControl (const CString& in_ParamName, const CValueArray& in_UIItemsArray, const CString& in_Label = CString(), const CString& in_UIType = CString(L"Combo"));

	/*! This is similar to PPGLayout::AddItem but it helps add a color widget to a layout.  There are two variations of the Color widget,
	::siControlRGBA and ::siControlRGB.  Rather than a single parameter, this control reads and writes a separate parameter for each
	"channel" of the color.

	To successfully create an RGBA color control on a CustomProperty, first add four ::siDouble or ::siFloat parameters to the control,
	for example "MyR", "MyG", "MyB", and "MyA". The naming scheme is not important but the parameters must be consecutive.  This
	parameters should have a UI Range of 0 to 1 because the values are stored in normalized format.  However you can have a larger
	actual Range if you want to support colors outside the normal color space. (See Parameter::GetSuggestedMax and Parameter::GetMax).

	The default control for a ::siDouble parameter is ::siControlNumber, but the color widget can be displayed instead by calling this
	method and specifying the name of the first parameter, for example "MyR".  To read the color value through the object model,
	just refer to the values of the four parameters, for example "MyG" will contain the normalized green component of the color.

	\note Shaders support colors directly as a native "strong" type.  So rather than having multiple parameters of type ::siDouble or
		::siFloat, they use a single parameter of type "Color".

	\param in_RedComponentParamName Script Name of the Parameter that represents the Red component of the color.
		The names of the Green, Blue and optional Alpha parameters do not need to be specified because %XSI uses
		the parameters that are consecutive after the Red component.
	\param in_Label Specify the text which appears to the left of the Color Widget. If not specified here, the
		value returned from SIObject::GetName or Parameter::GetScriptName appears instead. (See PPGItem::GetLabel).
	\param in_Alpha Specifies whether the color should include an Alpha channel.  This argument decides whether a
		::siControlRGBA or ::siControlRBG is created.
	*/
	PPGItem AddColor (const CString& in_RedComponentParamName, const CString& in_Label = CString(), bool in_Alpha = false );

	/*! Adds a button to a layout.  Unlike other controls a button doesn't have to be associated with any particular Parameter on the
	inspected object. Instead it basically serves the purpose of executing some logic when pressed.

	By default, the width of the button is automatically determined based on the text of the Label.  However this can be overridden with
	the ::siUICX attribute. (See PPGItem::PutAttribute)

	\param in_ScriptName 	A unique name for the button, which should not contain any space characters. This name is used
		to determine the logic subroutine to call when the button is pressed.
	\param in_Label Label of the button.  If not specified then the ScriptName is displayed.

	\sa PPGLayout::PutLogic, PPGLayout::AddItem
	*/
	PPGItem AddButton (const CString& in_ScriptName, const CString& in_Label = CString() );

	/*! Adds an \xl FCurveEditor to the layout.  It should be associated with an FCurve Parameter (see CustomProperty::AddFCurveParameter).
	This control is a full FCurve editor, and should not be confused with the Animation Divot.

	Like other Parameters you can add custom Logic code to the PPGLayout that is called whenever the FCurve changes.

	\param in_ScriptName Scripting name of the Parameter, which must be of type FCurve.  The call does not fail even if no
		parameter by this name exists.  However in that case the control is not drawn as part of the layout.  (See PPGItem::GetName).
	\param in_Height Height of the control.

	\note It is not necessary to set the width because the control automatically resizes according to the size of the %property page.

	\sa CustomProperty::AddFCurveParameter, FCurve, PPGLayout::GetLogic
	*/
	PPGItem AddFCurve (const CString& in_ScriptName, LONG in_Height = 300 ) ;


	/*! Static text is read-only text which appears directly on the property page.  It is similar to the label of a parameter but it is not
	associated with any parameter.  It is a useful control for adding basic documentation directly in the user interface. Use line breaks
	(\c \\n) in the text to force the text onto multiple lines.

	\note It is also still possible to read the text for a static control from a value of a string parameter, by using :::siControlStatic.
		(See the ::siPPGControlType enum).  However this function is easier and more efficient because it does not require any
		associated Parameter.

	\param in_Text Text to display.  This can be changed afterwards by calling PPGItem::PutLabel.
	\param in_Width Width of the text (in pixels).  If not specified, the text will use the entire width of the
		Property Page and will not word wrap even if it does not fit.
	\param in_Height Height of the text.  If not specified, the text's height is calculated based on the number
		of lines of text.

	\since 4.2
	*/
	PPGItem AddStaticText (const CString& in_Text, LONG in_Width = 0, LONG in_Height = 0 ) ;

	/*! Use this function as a tool for fine tuning the layout of a property page. It supports the insertion of blank space onto the layout.
	The most basic usage is to provide just a height value and to create a horizontal band of blank space. It can also be used to control
	the horizontal placement of other controls that are part of the same row.

	\param in_Width Width of the spacer (in pixels).  It is only necessary to specify this value when there are
		other controls in the same row.
	\param in_Height Height of the text.  If not specified the spacer will be a standard control height (22 pixels)

	\since 4.2
	*/

	PPGItem AddSpacer ( LONG in_Width = 0, LONG in_Height = 0 ) ;

	/*! Flushes the current layout, removing all PPGItems.  The normal use for this function would be to flush any existing layout right before
	defining a new layout. This function differs from PPGLayout::Delete in that it does not erase the complete layout, so attributes like the
	logic code are maintained (see PPGLayout::PutLogic and PPGLayout::PutAttribute). If no new items are added to the layout after calling
	PPGLayout::Clear then the %Property Page will be empty.
	\sa PPGLayout::Delete
	*/
	CStatus Clear ();
	/*! Deletes the entire layout, including all PPGItems, logic code and other PPGLayout attributes. If no new items are added to the layout
	after calling Delete then the %Property Page appears with a default layout, showing all the parameters in the order they were created.
	\sa PPGLayout::PutAttribute, PPGLayout::PutLogic, PPGLayout::Clear
	*/
	CStatus Delete ();

	/*! By default each item on a layout is drawn below the previous element. However, sometimes it is desirable to save space by putting multiple
	items on the same row.  This function lets you start a row of controls. All groups, parameters, buttons or other items added after this call
	will appear on the same horizontal row until PPGLayout::EndRow is called.

	This function is provided for convenience when creating the layout for an object.  Internally %XSI does not create PPGItem objects to represent
	when a row begins or ends.  Instead it uses the "continue" flag (see siUIContinue in the ::siPPGItemAttribute enum).

	\sa PPGLayout::EndRow, PPGItem::PutAttribute
	*/
	CStatus AddRow ();

	/*! Completes a row that was opened by a previous call to PPGLayout::AddRow.
	\sa PPGLayout::AddRow
	*/
	CStatus EndRow ();
	/*! Adds a group to the layout.  A group can be used to draw a box around related items, or as a way of arranging controls into columns.  It is
	possible to nest groups.

	Groups are represented in the layout with two special PPGItem objects, one for the beginning of the group and the other for the end.  Hence to
	change a label on a group it would be possible to enumerate through the PPGItems to find the item associated with the beginning of the group and
	to call PPGItem::PutLabel.

	\param in_Label When specified, the label is shown and a frame is drawn around the group.
	\param in_ShowFrame Flag whether to draw a frame around the items in the control. When a label is specified a frame
		is always drawn.  (See ::siUIShowFrame in the ::siPPGItemAttribute enum).
	\param in_WidthPercentage This parameter is only applicable if the group is part of a row.  It describes how much of
		the horizontal space should be taken by this group.  If not specified %XSI uses its own heuristic to lay out the
		group. For example, it might look better to split two groups 30%/70%, rather than using even spacing, if the second
		group has wider controls than the first.

	\sa PPGLayout::EndGroup, PPGItem::GetWidthPercentage
	*/
	PPGItem AddGroup (const CString& in_Label = CString(), bool in_ShowFrame = true, LONG in_WidthPercentage = 0 );

	/*! Completes a group that was opened by a previous call to PPGLayout::AddGroup.
	\sa PPGItem::AddGroup
	*/
	PPGItem EndGroup ();

	/*! Starts a new tab on a layout.  This allows a complex object to be represented on multiple "pages", rather than creating a very tall %property page.

	This should be the first item added to a multiple-tab layout.  If a layout only has a single tab then the tab control does not show up.

	\note No \c EndTab function is necessary on PPGLayout.  A tab is recognized as being complete when a new tab is started or the end of the
		layout is reached.

	\param in_Label The text that appears in the tab control.
	*/
	PPGItem AddTab (const CString& in_Label);

	/*! Returns the script code, if any, associated with the PPGLayout. This script code contains subroutines to react to events in the %property page.
	This property corresponds to the text found in the BeginScript/EndScript section (or equivalent Logic section) of a SPDL file.
	\sa PPGLayout::PutLogic
	*/
	CString GetLogic () const;

	/*! Sets the script code associated with the PPGLayout logic.  This script code contains subroutines to react to events in the %property page.
	This property corresponds to the text found in the BeginScript/EndScript section (or equivalent Logic section) of a SPDL file.

	The provided string must be a correctly-formed script, written in the language specified in the PPGLayout::GetLanguage section.  The script is
	not tested for syntax errors at the time this call is made.

	The logic script consists of subroutines implemented in any of the supported Scripting Languages. A routine named "OnInit", if defined, is called
	when the PPG first loads.  To react to a parameter changing, create a routine named "PARAMSCRIPTNAME_OnChanged".  To provide script code that gets
	executed when a button is pressed, create "BUTTONNAME_OnClicked".  In all these routines a global instance of the object PPG is available to get
	context information.

	This property makes it possible to still have %property page event handling on a custom object even when there is no associated SPDL file.  For
	example it would be possible to read a script file into a string and provide it as the value of this property.

	Rather than using this method to embed script code directly in the PPGLayout, it is also possible to specify an external file, using the
	::siUILogicFile attribute.  See PPGLayout::PutAttribute.

	It is not necessary to set this Property for a CustomProperty that is based on a PluginItem.  In that case it is possible to write the event
	handling callbacks directly in the same script file or dll that implements the Plugin.
	*/
	CStatus PutLogic ( const CString& in_strLogic );

	/*! Returns the language used in the logic code of the %Property Page.
	*/
	CString GetLanguage () const;

	/*! Sets the language used in the Logic code of the %Property Page, for example "VBScript", "JScript", "Python" or "PerlScript".  When not explicitly
	set, the default language will match whatever language the associated PluginItem has been implemented in.  If the object in question is not based on
	a Plugin then the default language is VBScript.
	*/
	CStatus PutLanguage ( const CString& in_strLogicLang );

	/*! Changes the value of an attribute of the PPGLayout.
	\param in_UIAttribute String from the ::siPPGLayoutAttribute enum, specifying which attribute to set.
	\param in_AttributeValue CValue containing a new value for the attribute.  It is possible to remove an attribute by
		providing an empty CValue.
	\sa PPGLayout::GetAttribute.
	*/
	CStatus PutAttribute ( const CString& in_UIAttribute, const CValue & in_AttributeValue );

	/*! The state of a PPGLayout is made up of two parts:
		\li The list of PPGItem objects, which define the controls and their arrangment
		\li The set of attributes, which is information that applies to the page as a whole.

	This information is exposed via this function. The ::siPPGLayoutAttribute enumeration provides a complete list of all these attributes and their meaning.

	Some of the most common attributes are also exposed directly as properties of the PPGLayout, for example PPGLayout::GetLogic.

	\param in_UIAttribute String from the siPPGLayoutAttribute enum, specifying which attribute to get.
	\return Contents of the attribute.  This is empty if the attribute is not supported or has not been set.
	\sa PPGLayout::PutAttribute
	*/
	CValue GetAttribute ( const CString& in_UIAttribute ) const;

	/*! Returns an item on the PPGLayout.  The requested name is compared with the value returned from PPGItem::GetName until a match is found.
	It is not defined which item you will get if there are multiple items on the layout with the same requested name.
	*/
	PPGItem GetItem( const CString& in_Name ) const;

	/*! Returns an item on the PPGLayout based on its index.
	*/
	PPGItem GetItem( LONG in_Index ) const;

	/*! Returns the number of items in the PPGLayout. This count includes all the controls, as well as special items like Groups and Tabs.
	*/
	LONG GetCount() const;


	private:
	PPGLayout * operator&() const;
	PPGLayout * operator&();
};

};
#endif // __XSIPPGLAYOUT_H__
