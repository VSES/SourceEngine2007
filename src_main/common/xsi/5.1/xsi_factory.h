//*****************************************************************************
/*!
   \file xsi_factory.h
   \brief Factory class declaration.

   © Copyright 1998-2003 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSI_FACTORY_H__
#define __XSI_FACTORY_H__

#include "sicppsdk.h"
#include <xsi_siobject.h>
#include "xsi_status.h"
#include "xsi_value.h"

namespace XSI {

class CustomOperator;
class CString;

//*************************************************************************
/*! \class Factory xsi_factory.h
	\brief This class represents a Factory class and is used to create certain types of objects.

	\sa CustomOperator, Application::GetFactory
	\since 4.0

	\eg
	\code
		using namespace XSI;
		Application app;
		Factory factory = app.GetFactory();

		CString strCode = L"function MyOperator_Update( ctx, Out, InPosY ) { Out.Value = InPosY.Value; }";

		CustomOperator op = factory.CreateScriptedOp( L"MyOperator", strCode, L"JScript" );

		Model root = app.GetActiveSceneRoot();

		CStatus st;
		Null null1; st = root.AddNull(L"MyNull1", null1);
		Null null2; st = root.AddNull(L"MyNull2", null2);

		op.AddOutputPort(null1.GetParameter( L"PosX" ));
		op.AddInputPort(null2.GetParameter( L"PosY" ));

		op.Connect();

	\endcode
 */
//*************************************************************************
class SICPPSDKDECL Factory : public SIObject
{
public:
	/*! Default constructor. */
	Factory();

	/*! Default destructor. */
	~Factory();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	Factory(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	Factory(const Factory& in_obj);

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
	\return The new Factory object.
	*/
	Factory& operator=(const Factory& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new Factory object.
	*/
	Factory& operator=(const CRef& in_ref);

	/*! Creates an %XSI object from a type name.  This function can be used to create
	a temporary CustomProperty that is not connected to the scene.  It can also be used
	to instantiated a new instance of a Self-Installed CustomOperator.

	\param name Type name.  In the case of a Self-Installed CustomProperty or CustomOperator
		this is the name as provided in PluginRegistrar::RegisterProperty and PluginRegistrar::RegisterOperator.
	\retval pst The return status (optional).
	\return A reference to the new object, for example a CustomOperator or CustomProperty.
	\sa ProjectItem::CreateCustomOp, PluginRegistrar::RegisterOperator, PluginRegistrar::RegisterProperty
	*/
	CRef CreateObject( const CString& name, CStatus* pst = 0 );

	/*! Creates an object from a preset and optional preset family name.
	\param presetname Preset name.
	\param family Preset family name.
	\retval pst The return status (optional).
	\return A reference to the new Factory object.
	*/
	CRef CreateObjectFromPreset( const CString& presetname, const CString& family = L"", CStatus* pst = 0 );

	/*! Creates an object from a preset file. The preset filename can be relative or partially
	complete for example you can find the Shear operator by specifying \c Operators\\Shear. If
	the preset is not found in the users folder then the users addon, workgroup, workgroup addon
	and factory locations will be searched in that order.
	\param  in_filename The filename of the preset.
	\retval out_st The return status (optional).
	\return A reference to the new object.
	\return Empty reference if the function failed.
	*/
	CRef CreateObjectFromFile( const CString& in_filename, CStatus* out_st = 0 );

	/*! Creates a scripted version of a CustomOperator object with the code specified (a runtime
	scripted operator). To create a Self-Installed Custom %Operator use Factory::CreateObject instead.
	\param  name The name of the operator.
	\param  code The implementation code.
	\param  language The implementation language.
	\retval pst The return status (optional).
	\return A reference to the new object.
	\return Empty reference if the function failed.
	*/
	CRef CreateScriptedOp( const CString& name=L"", const CString& code=L"", const CString& language=L"", CStatus* pst = 0 );

	/*! Creates a scripted version of a CustomOperator object with the logic in an associated script file.
	\param  name The name of the operator.
	\param  filename The file where the operator is implementated.
	\param  language The implementation language.
	\retval pst The return status (optional).
	\return A reference to the new object.
	\return Empty reference if the function failed.
	*/
	CRef CreateScriptedOpFromFile( const CString& name, const CString& filename, const CString& language = L"", CStatus* pst = 0 );

	/*! Creates a parameter definition (simplified API for creating parameters of type long, double, etc.).

	Parameter definitions contain the definition of a parameter from which you can create new parameters on the fly for one or
	more property sets or operators. For example, you may want to use a double with the same min/max range and use it on multiple
	property sets or you may simply want a quick way to add parameters to a runtime custom operator. You can create new parameter
	definitions from the Factory class.

	By default the parameter is persistable and animatable with the exception of string parameters which are not animatable. The
	recommended parameter types are: ::siString, ::siBool, ::siInt4, ::siUByte, and ::siDouble.

	The supported types are: ::siString, ::siBool, ::siDouble, ::siFloat, ::siInt4, ::siInt2, ::siUInt4, ::siUInt2, ::siByte, ::siUByte.

	\param in_scriptname The script name for the new Parameter object.
	\param in_type The data type of the new Parameter object.
	\param in_capabilities The capabilities of the parameter. This is a mask of ::siCapabilities values and is used to
		determine whether the parameter is read-only or animatable etc.
	\param in_name The name of the Parameter object. This is the string that will be displayed in the scene explorer.
	\param in_description Description of the custom parameter. If specified, this is a more descriptive name that
		will appear as the parameter label when the customer property is inspected. For example, the long name of a
		parameter might be a user friendly "Use Light Color as Energy" while the name is a more script friendly "use_color"
	\param in_default The default value of the custom parameter. A default value is required for parameter types other than
		::siString and ::siBool.
	\param in_min Minimum value of the custom parameter. A value is required for parameter types other than ::siString and ::siBool.
	\param in_max Maximum value of the custom parameter. A value is required for parameter types other than ::siString and ::siBool.
	\param in_suggestedmin Suggested minimum value of the custom parameter. The minimum value will be used if this parameter
		is not specified.  The suggested minimum should be equal or larger than the minimum.  This value is used to configure
		the range of UI controls.
	\param in_suggestedmax Suggested maximum value of the custom parameter. The maximum value will be used if this parameter
		is not specified.  The suggested maximum should be equal or smaller than the Maximum.  This value is used to configure
		the range of UI controls.
	\retval pst The return status (optional).
	\return A reference to the new parameter definition.
	*/
	CRef CreateParamDef(
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
		CStatus* pst = 0 );

	/*! Creates a parameter definition (simplified API for creating parameters of type long, double, etc.).

	Parameter definitions contain the definition of a parameter from which you can create new parameters on the fly for one or more
	property sets or operators. For example, you may want to use a double with the same min/max range and use it on multiple property
	sets or you may simply want a quick way to add parameters to a runtime custom operator. You can create new parameter definitions
	from the Factory class.

	By default the parameter is persistable and animatable with the exception of string parameters which are not animatable. The
	recommended parameter types are: ::siString, ::siBool, ::siInt4, ::siUByte, and ::siDouble.

	The supported types are: ::siString, ::siBool, ::siDouble, ::siFloat, ::siInt4, ::siInt2, ::siUInt4, ::siUInt2, ::siByte, ::siUByte

	\param in_scriptname The script name for the new Parameter object.
	\param in_type The data type of the new Parameter object.
	\param in_default The default value of the custom parameter. A default value is required for parameter types other than
		::siString and ::siBool.
	\param in_min	Minimum value of the custom parameter. A value is required for parameter types other than ::siString
		and ::siBool.
	\param in_max Maximum value of the custom parameter. A value is required for parameter types other than ::siString
		and ::siBool.
	\retval pst The return status (optional).
	\return A reference to the new parameter definition.
	*/
	CRef CreateParamDef(
		const CString& in_scriptname,
		CValue::DataType in_type,
		const CValue& in_default,
		const CValue& in_min,
		const CValue& in_max,
		CStatus* pst = 0 );

	/*! Creates a parameter definition (simplified API for creating parameters of type long, double, etc.).

	Parameter definitions contain the definition of a parameter from which you can create new parameters on the fly for one or more
	property sets or operators. For example, you may want to use a double with the same min/max range and use it on multiple property
	sets or you may simply want a quick way to add parameters to a runtime custom operator. You can create new parameter definitions
	from the Factory class.

	By default the parameter is persistable and animatable with the exception of string parameters which are not animatable. The
	recommended parameter types are: ::siString, ::siBool, ::siInt4, ::siUByte, and ::siDouble.

	The supported types are: ::siString, ::siBool, ::siDouble, ::siFloat, ::siInt4, ::siInt2, ::siUInt4, ::siUInt2, ::siByte, ::siUByte

	\param in_scriptname The script name for the new Parameter object.
	\param in_type The data type of the new Parameter object.
	\param in_default The default value of the custom parameter. A default value is required for parameter types other than
		::siString and ::siBool.
	\retval pst The return status (optional).
	\return A reference to the new parameter definition.
	*/
	CRef CreateParamDef(
		const CString& in_scriptname,
		CValue::DataType in_type,
		const CValue& in_default,
		CStatus* pst = 0 );

	/*! Creates an %FCurve parameter definition for use with the CustomProperty or CustomOperator. An %FCurve of this type is often
	called a \a Profile \a Curve because, unlike most FCurves, this curve does not animate any parameter, but instead is available
	for use in any fashion the plug-in developer or user wishes.

	Unlike parameters that have simple types like an Integer or a String, this Parameter is an instance of an FCurve. It uses units
	of Frames for its X axis, but because it does not directly animate any object this axis can actually be interpreted in any fashion.
	An example use of an FCurve parameter can be found on the Bulge Operator (see \xl Bulge_op).

	You can access the %FCurve object via Parameter::GetValue. This should not be confused with the function for retrieving an FCurve
	that is animating a \a basic parameter (Parameter::GetSource).

	In order to view the FCurve on the %Property Page for the CustomProperty or CustomOperator, use PPGLayout::AddFCurve.

	\param in_scriptname The script name for the new Parameter object.
	\retval pst The return status (optional).
	\return A reference to the new ParamDef object.
	\since 5.0
	*/
	CRef CreateFCurveParamDef( const CString& in_scriptname, CStatus* pst = 0) ;

	/*! Creates a GridData parameter definition for use with the CustomProperty or CustomOperator. The grid is a convenient way to
	store 1- or 2-dimensional arrays and the user can view and edit this data via ::siControlGrid (see ::siPPGControlType enum).

	To change the contents of the grid via scripting use the GridData object, which is accessible via Parameter::GetValue.

	To include the GridData in the %Property Page of the CustomProperty or CustomOperator, use the PPGLayout::AddItem.
	\since 5.0
	*/
	CRef CreateGridParamDef( const CString& in_scriptname, CStatus* pst = 0 ) ;

private:

	void* m_ptr;
};

};

#endif //__XSI_FACTORY_H__
