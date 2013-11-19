//*****************************************************************************
/*!
   \file xsi_customproperty.h
   \brief CustomProperty class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSICUSTOMPROPERTY_H__
#define __XSICUSTOMPROPERTY_H__

#include <xsi_property.h>
#include <xsi_status.h>

namespace XSI {

class ProxyParameter ;

//*****************************************************************************
/*! \class CustomProperty xsi_customproperty.h
	\brief The CustomProperty class represents a custom parameter set property object. A custom property holds a set of
	parameters which can be created by the user or via scripting/programming.

	A custom property can be used to store user-defined data such as attributes for a game engine. A custom property can
	be accessed and modified just like any other Property object.

	You should not use the custom property functions for adding and removing parameters in conjuction with the
	CustomProperty object since the parameter list will get out of sync.

	\sa SceneItem::AddCustomProperty

	\eg
	\code
		using namespace XSI;
		Application app;
		Model root = app.GetActiveSceneRoot();

		Null null;
		root.AddNull( L"", null );

		CustomProperty cpset;
		null.AddCustomProperty(L"MyCustomPSet", false, cpset );

		Parameter x, y, bitfield, label, flag;

		cpset.AddParameter( L"x", CValue::siDouble, siAnimatable, L"Double", L"Double X", 10.0, 0.0, 100.0, 10.0, 90.0, x );
		cpset.AddParameter( L"y", CValue::siInt4, siAnimatable, L"Integer", L"Integer Y", (LONG)10, (LONG)0, (LONG)100, (LONG)10, (LONG)90, y );
		cpset.AddParameter( L"bitfield", CValue::siUInt1, siAnimatable, L"UByte", L"Bitfield", (UCHAR)32, bitfield );
		cpset.AddParameter( L"label", CValue::siString, siPersistable, L"Text", L"Text Field", L"Hello World", label );
		cpset.AddParameter( L"flag", CValue::siBool, siAnimatable, L"Flag", L"Boolean Field", true, flag );

	\endcode
 */
//*****************************************************************************

class SICPPSDKDECL CustomProperty : public Property
{
public:
	/*! Default constructor. */
	CustomProperty();

	/*! Default destructor. */
	~CustomProperty();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	CustomProperty(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	CustomProperty(const CustomProperty& in_obj);

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
	\return The new CustomProperty object.
	*/
	CustomProperty& operator=(const CustomProperty& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new CustomProperty object.
	*/
	CustomProperty& operator=(const CRef& in_ref);

	/*! Adds a new custom parameter to the CustomProperty object. The recommended parameter types are: ::siString, ::siBool,
	::siInt4, ::siUByte, and ::siDouble.

	The supported types are: ::siString, ::siBool, ::siDouble, ::siFloat, ::siInt4, ::siInt2, ::siUInt4, ::siUInt2, ::siByte,
	::siUByte

	\param in_scriptname The script name for the new Parameter object.
	\param in_type The data type of the new Parameter object.
	\param in_capabilities The capabilities of the parameter. This is a mask of ::siCapabilities values and is used to
		determine where the parameter is read-only or animatable etc.
	\param in_name The name of the Parameter object. This is the string that will be displayed in the scene explorer
		(unless \c Show \c Script \c Names is enabled). If this argument is not specified then the \c ScriptName
		argument will be visible to users as the parameter's name.  (See  SIObject::GetName and Parameter::GetScriptName).
		This string will appear as the parameter label when the CustomProperty is inspected, unless a specific label is
		specified in the PPGLayout (see PPGItem::PutLabel).
	\param in_description  This string is optional and does not appear in the user interface.
	\param in_default The default value of the custom parameter. A default value is required for parameter types other
		than ::siString and ::siBool.
	\param in_min	Minimum value of the custom parameter. A value is required for parameter types other than ::siString
		and ::siBool.
	\param in_max Maximum value of the custom parameter. A value is required for parameter types other than ::siString
		and ::siBool.
	\param in_suggestedmin Suggested minimum value of the custom parameter. The minimum value will be used if this
		parameter is not specified.  The suggested minimum should be equal or larger than the minimum.  This value
		is used to configure the range of UI controls.
	\param in_suggestedmax Suggested maximum value of the custom parameter. The maximum value will be used if this
		parameter is not specified.  The suggested maximum should be equal or smaller than the Maximum.  This value
		is used to configure the range of UI controls.
	\param  io_parameter New Parameter object.
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus AddParameter(
		const CString& in_scriptname,
		CValue::DataType in_type,
		INT in_capabilities,
		const CString& in_name,
		const CString& in_description,
		const CValue& in_default,
		const CValue& in_min,
		const CValue& in_max,
		const CValue& in_suggestedmin,
		const CValue& in_suggestedmax,
		Parameter&	io_parameter );

	/*! Adds a new custom parameter to the CustomProperty object using the default min and max for the specified type. The
	min/max for ::siDouble, ::siInt4 and ::siUByte are as follows:
	\par siDouble
	\code 	-1.7976931348623158e+308..1.7976931348623158e+308 \endcode
	\par siInt4
	\code 	-2147483648..2147483647 \endcode
	\par siUByte
	\code 	0..255 \endcode

	The recommended parameter types are: ::siString, ::siBool, ::siInt4, ::siUByte, and ::siDouble.

	The supported types are: ::siString, ::siBool, ::siDouble, ::siFloat, ::siInt4, ::siInt2, ::siUInt4, ::siUInt2, ::siByte,
	::siUByte

	\param in_scriptname The script name for the new Parameter object.
	\param in_type The data type of the new Parameter object.
	\param in_capabilities The capabilities of the parameter. This is a mask of ::siCapabilities values and
		is used to determine where the parameter is read-only or animatable etc.
	\param in_name The name of the Parameter object. This is the string that will be displayed in the scene explorer
		(unless \c Show \c Script \c Names is enabled). If this argument is not specified then the \c ScriptName
		argument will be visible to users as the parameter's name.  (See  SIObject::GetName and Parameter::GetScriptName).
		This string will appear as the parameter label when the CustomProperty is inspected, unless a specific label is
		specified in the PPGLayout (see PPGItem::PutLabel).
	\param in_description  This string is optional and does not appear in the user interface.
	\param in_default The default value of the custom parameter. A default value is required for parameter types other
		than ::siString and ::siBool.
	\param io_parameter New Parameter object.
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus AddParameter(
		const CString& in_scriptname,
		CValue::DataType in_type,
		INT in_capabilities,
		const CString& in_name,
		const CString& in_description,
		const CValue& in_default,
		Parameter&	io_parameter );

	/*! Adds an %FCurve parameter to the CustomProperty. An %FCurve of this type is often called a \a Profile \a Curve because,
	unlike most FCurves, this curve does not animate any parameter, but instead is available for use in any fashion the plug-in
	developer or user wishes.

	Unlike parameters that have simple types like an Integer or a String, this Parameter is an instance of an FCurve. It uses
	units of frames for its X axis, but because it does not directly animate any object this axis can actually be interpreted
	in any fashion.  An example use of an FCurve parameter can be found on the Bulge Operator (see \xl Bulge_op).

	You can get the %FCurve object via Parameter::GetValue. This should not be confused with the function for retrieving an FCurve
	that is animating a \a basic parameter (Parameter::GetSource).

	To view the %FCurve on the %Property Page for the %CustomProperty use PPGLayout::AddFCurve.

	\return The new Parameter object.
	\since 4.0
	*/
	Parameter AddFCurveParameter( const CString& in_scriptname ) ;

	/*! Adds a Grid parameter to the CustomProperty. The grid is a convenient way to store 1- or 2-dimensional arrays and the
	user can view and edit this data via the ::siControlGrid (see ::siPPGControlType enum).

	To change the contents of the grid via scripting use the GridData object, which is accessible via Parameter::GetValue.

	To include the Grid in the %Property Page of the CustomProperty use PPGLayout::AddItem.
	\since 4.0
	*/
	Parameter AddGridParameter( const CString& in_scriptname ) ;

	/*! Removes the specified custom parameter from the custom property object.
	\note Beginning with v4.2 it is possible to remove ProxyParameters using this function.

	\param  in_param Parameter to remove.
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus RemoveParameter( const Parameter& in_param );

	/*! Returns the binary representation of the CustomProperty.

	 This data is a compact representation, encoded in a unsigned \c char*.  Unlike a Preset, only the actual values are stored
	 in the binary representation, this is done for memory efficiency.

	 In the case of animated parameters the value recorded in the binary representation is the value at the current frame. Similarly,
	 the current value of any proxy parameters is recorded in the binary representation.

	 Normally SDK developers will not need to be concerned with the internal format of this binary data.  However, the following
	 information may be useful in cases where the data will be generated or processed outside of %XSI. The binary representation
	 begins with a two-byte header that serve as a \a magic \a number for error-handling purposes. The rest of the buffer is a
	 concatenation of the raw values of the properties of the custom property, in exactly the order they were originally added to
	 the custom property, and stored in little-endian byte order. For example, a double takes up 8 bytes, a float 4 bytes and an
	 unsigned char (::siUByte) takes 1 byte. Strings start with a 32-bit count of the number of characters, followed by the 16-bit
	 unicode representation of each character, with no string termination character.

	\param  io_buffer This is a byte string, containing binary data.  This string does not necessarily contain human readable
		text. Callers are responsible for freeing this string with CustomProperty::FreeBinaryData.
	\param  io_size	   The size of the binary data in bytes.
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus GetBinaryData( unsigned char *& io_buffer, UINT& io_size ) const;

	/*! Deallocates the memory that was allocated by a call to CustomProperty::GetBinaryData. Calling this function with a NULL
	pointer will fail with CStatus::InvalidArgument.
	\return CStatus::OK success
	\return CStatus::InvalidArgument
	\return CStatus::Fail failure
	*/
	CStatus FreeBinaryData( unsigned char *& io_buffer ) const ;


	/*! Sets the binary representation of the CustomProperty.

	 This data is a compact representation, encoded in a unsigned \c char*.  Unlike a Preset, only the actual values are stored in
	 the binary representation, this is done for memory efficiency.

	 You should not attempt to set the binary representation on a custom property object which is different from the custom property
	 from which the binary representation was retrieved.  In particular the number of parameters, the order of the parameters and the
	 type of the parameters must match.  If the custom property is different the API call will fail or will result in the custom property
	 set being filled with bogus values.  However it is possible to rearrange the order in which parameters are displayed on the property
	 page for a CustomProperty (by editing the SPDL file "Layout" section) without any incorrect results.

	 Normally SDK developers do not need to be concerned with the internal format of this binary data.  However, the following information
	 may be useful in cases where the data will be generated or processed outside of %XSI. The binary representation begins with a two-byte
	 header that serve as a \a magic \a number for error handling purposes. The rest of the buffer is a concatenation of the raw values of
	 the properties of the custom property, in exactly the order they were originally added to the custom property, and stored in little-endian
	 byte order. For example, a double takes up 8 bytes, a float 4 bytes and a unsigned char (::siUByte) takes 1 byte. Strings start with a
	 32-bit count of the number of characters, followed by the 16-bit unicode representation of each character, with no string termination
	 character.

	\param  in_buffer  Byte string containing binary data.
	\param  in_size	   The size of the binary data in bytes.
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus PutBinaryData( const unsigned char * in_buffer, UINT in_size );

	/*! Adds a ProxyParameter to the CustomProperty.
	\param in_ParamToProxy Parameter to which the new ProxyParameter will be connected.
	\param in_ProxyScriptName A script name for the new ProxyParameter.  If not specified a name is generated based on the
		fullname of the \c in_ParamToProxy argument.
	\param in_ProxyName A name to use for the new ProxyParameter.  If not specified a name is generated based on the fullname
		of the \c in_ParamToProxy argument.
	\return Newly created ProxyParameter
	\since v4.2
	*/
	ProxyParameter AddProxyParameter(
					Parameter in_ParamToProxy,
					const CString& in_ProxyScriptName = CString(),
					const CString& in_ProxyName = CString() ) ;

	/*! Creates a Parameter from a parameter definition.

	%Parameter definitions contain the definition of a parameter from which you can create new parameters on the fly for one or more property
	sets or operators. For example, you may want to use a double with the same min/max range and use it on multiple property sets or you may
	simply want a quick way to add parameters to a runtime custom operator. You can create new parameter definitions from the Factory class.
	\param  in_paramdef  Parameter definition object
	\return Parameter new parameter
	\sa Factory::CreateParamDef, Factory::CreateFCurveParamDef, Factory::CreateGridParamDef
	\since v5.0
	*/
	Parameter AddParameterFromDef( const CRef& in_paramdef ) ;

	private:
	CustomProperty * operator&() const;
	CustomProperty * operator&();
};

};

#endif // __XSICUSTOMPROPERTY_H__
