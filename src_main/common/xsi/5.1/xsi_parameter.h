//*****************************************************************************
/*!
   \file xsi_parameter.h
   \brief Parameter and CParameterRefArray class declarations.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIPARAMETER_H__
#define __XSIPARAMETER_H__

#include <xsi_siobject.h>
#include <xsi_value.h>
#include <xsi_status.h>

#pragma warning(disable:4251)

namespace XSI {

class FCurve;
class Expression ;
class Parameter;
class Model;
class X3DObject;

//*****************************************************************************
/*! \class CParameterRefArray xsi_parameter.h
	\brief An array of Parameters.

	\warning This specialized array is returned by ProjectItem::GetParameters and Parameter::GetParameters,
		it is not meant to be created and modified in user-defined functions. If you want to add and remove
		arbitrary items to a collection, you must use a CRefArray instead.

	\sa ProjectItem::GetParameters, Parameter::GetParameters, CRefArray, Parameter

	\eg
	\code
		using namespace XSI;
		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject myCube;
		root.AddGeometry( L"Cube", L"MeshSurface", CString(L"MyCube"), myCube );

		CParameterRefArray params = myCube.GetParameters();

		Parameter posx(params.GetItem( L"posx" ));
		Parameter posy(params.GetItem( L"posy" ));
		Parameter posz(params.GetItem( L"posz" ));

		posx.PutValue(5.5);
		posy.PutValue(8.0);

		app.LogMessage( L"Cube's pos parameter: " +
			CValue(posx.GetValue()).GetAsText() + L"," +
			CValue(posy.GetValue()).GetAsText() + L"," +
			CValue(posz.GetValue()).GetAsText() );
	\endcode
*/
//*****************************************************************************
class SICPPSDKDECL CParameterRefArray : public CRefArray
{
	public:

	/*! Default constructor. */
	CParameterRefArray();

	/*! Copy constructor.  Performs a shallow copy, so any change
	to one instance will appear in any of the copies.
	*/
	CParameterRefArray( const CParameterRefArray& );

	/*! Default destructor. */
	~CParameterRefArray();

	/*! Assign a CParameterRefArray object to this one.
	\param in_refArray A constant CParameterRefArray object.
	\return A new reference object.
	*/
	CParameterRefArray& operator=( const CParameterRefArray& in_refArray );

	/*! Returns a CRef object contained in the list given an object scripting
	name.  If the requested parameter does not exist in the array then
	an invalid CRef is returned.
	\param in_scriptname The scripting name of a parameter to retrieve.
	\return A new CRef object.
	\sa Parameter::GetScriptName
	\sa CRef::IsValid
	*/
	CRef GetItem( const CString& in_scriptname ) const;

	/*! Adds a CRef object at the end of this array. The CRef must be Valid and
	refer to a Parameter, otherwise it is ignored. This operation does not
	actually add a parameter to any associated XSI object.
	\param in_item CRef object.
	\sa CustomProperty::AddParameter
	*/
	void Add( const CRef& in_item );

	/*! Removes a parameter from the CParameterRefArray.  This does not actually
	remove the parameter from any associated XSI object.  No action is performed if the
	requested parameter is not found in the CParameterRefArray.
	\param in_scriptname scriptname of parameter.
	\sa CustomProperty::RemoveParameter
	*/
	void RemoveItem( const CString& in_scriptname );

	/*! Creates and returns an array of FCurve objects by plotting the parameters
	values from consecutive frames of an animation. The resulting FCurve objects
	are	transients and not connected to any XSI components. All resulting FCurve
	objects are parented to the Application object.

	\param in_startFrame First frame to plot. Use DBL_MAX to specify the first frame in the timeline. Defaults to DBL_MAX.
	\param in_stopFrame Last frame to plot. Use DBL_MAX to specify the last frame in the timeline. Defaults to DBL_MAX.
	\param in_stepFrame Step between frames. Defaults to 1.
	\param in_fcurveTypeToPlot Type of fcurve required for plotting. Defaults to siStandardFCurve.
	\param in_interpolation Type of interpolation required for the resulting fcurves. This argument is only relevant if
		in_fcurveTypeToPlot is siStandardFCurve. Defauts to siCubicInterpolation.
	\param in_bFitValues Fits the fcurves through the plotted values. Defaults to false.
	\param in_fitTolerance Tolerance value used for fitting the fcurves through the plotted values. Defaults to 0.01.
	\param in_bProcessRotation Processes the rotation curves to ensure their continuity. Defaults to false. For processing
		continuous rotations the rotation channels must be listed consecutively. e.g. cube.local.rotx, cube.local.roty,
		cube.local.rotz otherwise the rotation for that object will not be processed.
	\param in_bDisplayProgressBar Display a progress bar control to indicates the progress of the plot operation. Defaults to true.

	\return Array of FCurve objects. The returned array of FCurve objects matches the parameters in size.
		Parameters that are filtered out (e.g. invalid parameter to plot) have an invalid FCurve object
		in the returned array.

	\sa FCurve
	\since 5.0
	*/
	CRefArray PlotAnimation(
		double					in_startFrame=DBL_MAX,
		double					in_stopFrame=DBL_MAX,
		double					in_stepFrame=1,
		siFCurveType			in_fcurveTypeToPlot=siStandardFCurve,
		siFCurveInterpolation	in_interpolation=siCubicInterpolation,
		bool					in_bFitValues=false,
		double					in_fitTolerance=0.01,
		bool					in_bProcessRotation=false,
		bool					in_bDisplayProgressBar=true );

	/*! Returns the value of a parameter identified with a scripting name. The value returned is empty if the input name
	is invalid.
	\note This function can be more efficient than getting a Parameter object and then calling Parameter::GetValue to get
		the value. Use this function in cases where you only need to get the value on a parameter.
	\param in_scriptname	The parameter scripting name (see Parameter::GetScriptName).
	\param in_time Time in frame defaults to the current frame if in_time is set with DBL_MAX.
	\return The parameter value.
	\since Gemini
	*/
	CValue GetValue( const CString& in_scriptname, double in_time=DBL_MAX ) const;

	/*! Sets a parameter value by scripting name. If you try to set a read-only parameter, an error (CStatus::Unexpected)
	occurs.  You can check if a parameter is read-only by checking the condition:

	\code Parameter::GetCapabilities && siReadOnly == siReadOnly \endcode
	\tip You can set a custom property parameter even if it is created with the read-only capability.
	\note This function can be more efficient than getting a Parameter object and then calling Parameter::PutValue to set
		the value. Use this function in cases where you only need to set the value on a parameter.

	\param in_scriptname	The parameter scripting name.
	\param in_val The parameter value to set.
	\param in_time Time in frame defaults to the current frame if in_time is set with DBL_MAX.
	\return CStatus::OK success
	\return CStatus::Fail other failure
	\return CStatus::Unexpected Trying to set a read-only parameter.
	\since Gemini
	*/
	CStatus PutValue( const CString& in_scriptname, const CValue&	in_val, double in_time=DBL_MAX );

	private:
	void* m_pIndices;
};

//*****************************************************************************
/*! \class Parameter xsi_parameter.h
	\brief Represents a parameter value within the ProjectItem, for example, the angle parameter
	of the twist operator. It is possible to create custom defined parameters on the CustomProperty object.

	\eg
	\code
		using namespace XSI;
		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject myCube;
		root.AddGeometry( L"Cube", L"MeshSurface", CString(L"MyCube"), myCube );

		// print the names of all parameter exposed by the cube object
		CRefArray params = myCube.GetParameters();

		for (LONG i = 0 ; i < params.GetCount(); ++i )
		{
			Parameter param(params[i]);
			app.LogMessage( param.GetScriptName() + L" = " +
				param.GetValue().GetAsText() );
		}
	\endcode
*/
//*****************************************************************************

class SICPPSDKDECL Parameter : public SIObject
{
public:
	/*! Constructs a Parameter object. */
	Parameter();

	/*! Destroys a Parameter object. */
	~Parameter();

	/*! Constructs a Parameter object from a CRef object.
	\param in_ref A reference to a group.
	*/
	Parameter(const CRef& in_ref);

	/*! Constructs a new Parameter object from an existing Parameter object.
	\param in_obj An existing Parameter object to copy into this Parameter object.
	*/
	Parameter(const Parameter& in_obj);

	/*! Returns True if this object supports the functionality of a specified class. For example,
	a Parameter is a type of SIObject, so a Parameter object supports SIObject functionality.
	\param in_ClassID Test if this object supports this class.
	\return True if this object supports the specified class, and false otherwise.
	*/
	bool IsA( siClassID in_ClassID) const;

	/*! Gets the class ID for this object.
	\return The class ID.
	*/
	siClassID GetClassID() const;

	/*! Assigns a Parameter object to an existing Parameter object.
	\param in_obj A Parameter object to be copied into this object.
	\return The reinitialized Parameter object.
	*/
	Parameter& operator=(const Parameter& in_obj);

	/*! Assigns a CRef to this Parameter object. The Parameter object is cleared
	if the CRef is not a reference to an object that supports the Parameter class.
	\param in_ref A reference to an object that supports the Parameter class.
	\return The reinitialized Parameter object.
	*/
	Parameter& operator=(const CRef& in_ref);

	/*! Gets a mask of all the object's siCapabilities values.
	\return The capability value.
	*/
	LONG GetCapabilities() const;

	/*! Disables or enables any Capability flag set on the Parameter.  For example a parameter can be
	set to "read-only" or "animatable".  Changing the capability flags only affects one particular instance
	of the parameter.

	\param in_flag Specify the flag to change
	\param in_bEnable True to set the capability; false to remove it.
	\sa Parameter::GetCapabilities
	\since 4.0
	*/
	CStatus PutCapabilityFlag( siCapabilities in_flag, bool in_bEnable ) ;


	/*! Returns the object's marked state
	\return true if object is marked, false otherwise.
	*/
	bool GetMarked() const;

	/*! Sets the object's marked state. Marking is consider an subset
	of selection and can only be set if the object parent of the parameter is first
	selected. A parameter can only be successfully marked if it has a value type
	that is not equal to siEmpty.

	Marking parameters is a way of selecting which parameters you want to use for
	a specific animation task. For example, you can mark parameters so that only
	their animation is copied or removed, or you mark the parameters you want to
	store in an action to be used in the animation mixer. You can also mark parameters
	to be used when scaling or offsetting an animation, or with linked parameters or
	scripted operators.

	Marking parameters is also one of the most common and useful tools for setting keys.
	By marking only the parameters you need, you can keep the animation information small.

	The marking will be set relative to the first group type object ( X3DObject, Model, Group )
	in the parameter's path. If there is no group type object  in the path then the marking
	will be set relative to the parameter's immediate parent.

	\param in_bVal Marked state flag
	\return CStatus::OK success
	\return CStatus::Fail failure parameter could not be marked
	*/
	CStatus PutMarked( bool in_bVal );

	/*! Returns an array of all parameters nested within the parameter object. In XSI parameters
	are often organized into a hierarchy, with parameters like Visibility and Kinematics
	representing entire groups of parameters rather than a single value. This function provides
	access to the parameters nested within such a parameter. For normal parameters that do not
	have any children the returned parameter array will be empty.

	\note This function builds a cache of all nested parameters. Building the cache can be
		costly performance-wise. Use this function in cases where the object has many parameters
		and you need to access many of them. It is more suitable to use Parameter::GetParameter or
		ProjectItem::GetParameter if you only need to access a few of them.

	\return Array of references to Parameter objects
	\sa	Parameter::GetParameter, ProjectItem::GetParameters, ProjectItem::GetParameter
	\since Gemini
	*/
	CParameterRefArray& GetParameters() const;

	/*! Returns a particular parameter nested within this parameter object. If the parameter scripting
	name does not exist the Parameter will be invalid and all functions will return default values.
	\param in_scriptname The scripting name of the parameter (see Parameter::GetScriptName).
	\return A Parameter object
	\since 3.5.1
	*/
	Parameter GetParameter( const CString& in_scriptname ) const;

	/*! Returns the parameter value. If the parameter is unable to retrieve the
	value, it will return an empty CValue. If the parameter value is a string
	and it has not been set then it will return an empty string.
	\param in_time Time in frame
	\return The parameter value.
	*/
	CValue GetValue(double in_time=DBL_MAX) const;

    /*! Sets the parameter value. If you try to set a read-only parameter, an
    error (CStatus::Unexpected) occurs.  You can check if a parameter is
	read-only by checking the condition: Parameter::GetCapabilities &&
	siReadOnly == siReadOnly. Note that you can set a custom property parameter
	even if it is created with the readonly capability.

	\note Prior to the v5.0 the time argument had the default value 0, this was
		incorrect and would have give the incorrect value if the parameter was
		animated and the current time was not set to frame 0.

	\param in_val The parameter value to set.
	\param in_time Time in frame, defaults to the current frame (indicated by the value of DBL_MAX).
	\return CStatus::OK success
	\return CStatus::Fail other failure
	\return CStatus::Unexpected Trying to set a read-only parameter.
	*/
	CStatus PutValue(const CValue& in_val, double in_time=DBL_MAX );

	/*! Returns the parameter's script name.
	\return The parameter's scripting name.
	*/
	CString GetScriptName() const;

	/*! Returns the descriptive string of a Parameter. The description is a
	longer, more descriptive version of the parameter name.
	\return The parameter's description string.
	*/
	CString	GetDescription() const;

	/*! Returns the type of the parameter. For example a check box parameter
	would return siBool and a text based parameter would return siString.
	This is different from the SIObject::GetType property, which refers to the
	type of the parameter object, in other words "Parameter".
	\return The parameter's value type.
	*/
	CValue::DataType GetValueType() const;

	/*! Returns the minimum value that the parameter could possibly have.
	This property is only valid for numeric parameters. Other value types,
	for example siString do not have concept of Minimum or Maximum.
	\return The minimum value.
	*/
	CValue GetMin() const;

	/*! Returns the maximum value that the parameter could possibly have.
	This property is only valid for numeric parameters. Other value types, for
	example siString do not have concept of Minimum or Maximum.
	\return The maximum value.
	*/
	CValue GetMax() const;

	/*! Returns the suggested minimum value for the parameter. A parameter can
	have two separate ranges - the Min/Max which defines the entire range of
	legal values and the Suggested Minimum/Suggested Maximum which defines a
	sub-range of the Min/Max that makes the most sense.  For example an
	enormous number of subdivisions may theoretically be possible on a geometry,
	but for performance reasons a smaller range can be suggested to the user.
	The suggested Min/Max values are used when determining the slider ranges
	for controls when parameters are inspected. This property is only valid for
	numeric parameters. Other value types, for example siString do not have
	concept of Minimum or Maximum.
	\return The suggested minimum value.
	*/
	CValue GetSuggestedMin() const;

	/*! Returns the suggested minimum value for the parameter. A parameter can
	have two separate ranges - the Min/Max which defines the entire range of
	legal values and the Suggested Minimum/Suggested Maximum which defines a
	sub-range of the Min/Max that makes the most sense.  For example an
	enormous number of subdivisions may theoretically be possible on a geometry,
	but for performance reasons a smaller range can be suggested to the user.
	The Suggested Min/Max values are used when determining the slider ranges
	for controls when parameters are inspected. This property is only valid for
	numeric parameters. Other value types, for example siString do not have
	concept of Minimum or Maximum.
	\return The suggested maximum value.
	*/
	CValue GetSuggestedMax() const;

	/*! Returns the default value of the parameter. Some parameters have a
	default value, which defines what the initial value of the parameter is.
	\return The default value.
	*/
	CValue GetDefault() const;

	/*! Returns a parameter's data source e.g. ActionSource, Constraint FCurve,
	ImageClip, Operator, Shader, StaticSource, Mixer. If no source is connected
	then an empty CRef object is returned.
	\return A reference to the data source connected to this parameter.
	*/
	CRef GetSource() const;

	/*! Sets a parameter's data source e.g. ActionSource, Constraint FCurve,
	ImageClip, Operator, Shader, StaticSource, Mixer. You can remove the data source
	by assigning an empty CRef object. This function is only supported for
	shader parameters.
	\param in_source data source to connect.
	\return CStatus::OK success
	\return CStatus::Fail Operation failed or operation performed on a
	non-shader parameter.
	*/
	CStatus	PutSource( const CRef& in_source );

	/*! Creates and connects an FCurve object to this parameter.
	\param in_Type function curve type
	\param io_FCurve New FCurve object created
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus AddFCurve
	(
		siFCurveType		in_Type,
		FCurve&				io_FCurve
	);


	/*! Creates and connects an Expression object to this parameter.
	\param in_ExpressionDefinition The expression formula
	\return Newly created Expression.  Use the CBase::IsValid() function on the returned object
		to test whether this function has failed.
	\since 5.0
	*/
	Expression AddExpression
	(
		CString&		in_ExpressionDefinition
	);


	/*! Creates a data source( e.g. ActionSource, Constraint FCurve,
	ImageClip, Operator, Shader, StaticSource.) object from a preset name and
	connects it to the Parameter. This method can be used to connect a shader
	to a parameter from the shader preset.
	\param in_strName Preset name
	\param in_strFamily Preset family name
	\param io_prevSource The CRef object that was previously connected.
	\param io_source The new CRef object.
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus ConnectFromPreset
	(
		const CString&	in_strName,
		const CString&	in_strFamily,
		CRef&			io_prevSource,
		CRef&			io_source
	);

	/*! Creates a data source ( e.g. ActionSource, Constraint FCurve,
	ImageClip, Operator, Shader, StaticSource.) object from file and connects
	it to the parameter. This method can be used to connect a shader to a
	parameter from the shader preset file name.
	\param in_strName Preset name
	\param io_prevSource The CRef data source object that was previously connected.
	\param io_source The new CRef to a data source object.
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus ConnectFromFile
	(
		const CString&	in_strName,
		CRef&			io_prevSource,
		CRef&			io_source
	);

	/*! Creates a data source ( e.g. ActionSource, Constraint FCurve,
	ImageClip, Operator, Shader, StaticSource.) object from a progid string
	and connects it to the parameter.
	\param in_strName Preset name
	\param io_prevSource The CRef data source object that was previously connected.
	\param io_source The new CRef data source object.
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus ConnectFromProgID
	(
		const CString&	in_strName,
		CRef&			io_prevSource,
		CRef&			io_source
	);

	/*! Connects a data source ( e.g. ActionSource, Constraint FCurve,
	ImageClip, Operator, Shader, StaticSource.) object to the parameter.
	\param in_source A reference to the data source object to connect.
	\param io_prevSource A reference to the data source object that was previously connected.
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus Connect
	(
		const CRef&	in_source,
		CRef&		io_prevSource
	);

	/*! Disconnects a source (e.g. FCurve, Expression, ImageClip, Operator, Shader) object from the parameter.
	\return CStatus::OK success
	\return CStatus::Fail failure
	\since 5.0
	*/
	CStatus Disconnect();

	/*! Disconnects a source (e.g. FCurve, Expression, ImageClip, Operator, Shader) object from the parameter.
	\param io_prevSource A reference to the previously connected source object.
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus Disconnect( CRef& io_prevSource );

	/*! Sets the parameter value for a parameter which is nested under the current parameter. If you try to set
	a read-only parameter, an error (CStatus::Unexpected) occurs.  You can check if a parameter is read-only by
	checking the condition:

	/code Parameter::GetCapabilities && siReadOnly == siReadOnly \endcode
	\tip You can set a custom property parameter even if it is created with the read-only capability.
	\note This function does not build a cache of all parameter values.  Use this function in cases where
		the object has many parameters but you only need to set the value on a few of them.

	\param in_scriptname	The parameter scripting name (see Parameter::GetScriptName).
	\param in_val	The parameter value to set.
	\param in_time	Time in frame, defaults to the current frame (represented by the value of DBL_MAX).
	\return CStatus::OK success
	\return CStatus::Unexpected Trying to set a read-only parameter.
	\return CStatus::Fail other failure
	*/
	CStatus PutParameterValue
	(
		const CString&	in_scriptname,
		bool			in_val,
		double			in_time = DBL_MAX
	);

	/*! Sets the parameter value for a parameter which is nested under the current parameter. If you try to set
	a read-only parameter, an error (CStatus::Unexpected) occurs.  You can check if a parameter is read-only by
	checking the condition:

	/code Parameter::GetCapabilities && siReadOnly == siReadOnly \endcode
	\tip You can set a custom property parameter even if it is created with the read-only capability.
	\note This function does not build a cache of all parameter values.  Use this function in cases where
		the object has many parameters but you only need to set the value on a few of them.

	\param in_scriptname	The parameter scripting name (see Parameter::GetScriptName).
	\param in_val	The parameter value to set.
	\param in_time	Time in frame, defaults to the current frame (represented by the value of DBL_MAX).
	\return CStatus::OK success
	\return CStatus::Unexpected Trying to set a read-only parameter.
	\return CStatus::Fail other failure
	*/
	CStatus PutParameterValue
	(
		const CString&	in_scriptname,
		double			in_val,
		double			in_time = DBL_MAX
	);

	/*! Sets the parameter value for a parameter which is nested under the current parameter. If you try to set
	a read-only parameter, an error (CStatus::Unexpected) occurs.  You can check if a parameter is read-only by
	checking the condition:

	/code Parameter::GetCapabilities && siReadOnly == siReadOnly \endcode
	\tip You can set a custom property parameter even if it is created with the read-only capability.
	\note This function does not build a cache of all parameter values.  Use this function in cases where
		the object has many parameters but you only need to set the value on a few of them.

	\param in_scriptname	The parameter scripting name (see Parameter::GetScriptName).
	\param in_val	The parameter value to set.
	\param in_time	Time in frame, defaults to the current frame (represented by the value of DBL_MAX).
	\return CStatus::OK success
	\return CStatus::Unexpected Trying to set a read-only parameter.
	\return CStatus::Fail other failure
	*/
	CStatus PutParameterValue
	(
		const CString&	in_scriptname,
		float			in_val,
		double			in_time = DBL_MAX
	);
	/*! Sets the parameter value for a parameter which is nested under the current parameter. If you try to set
	a read-only parameter, an error (CStatus::Unexpected) occurs.  You can check if a parameter is read-only by
	checking the condition:

	/code Parameter::GetCapabilities && siReadOnly == siReadOnly \endcode
	\tip You can set a custom property parameter even if it is created with the read-only capability.
	\note This function does not build a cache of all parameter values.  Use this function in cases where
		the object has many parameters but you only need to set the value on a few of them.

	\param in_scriptname	The parameter scripting name (see Parameter::GetScriptName).
	\param in_val	The parameter value to set.
	\param in_time	Time in frame, defaults to the current frame (represented by the value of DBL_MAX).
	\return CStatus::OK success
	\return CStatus::Unexpected Trying to set a read-only parameter.
	\return CStatus::Fail other failure
	*/
	CStatus PutParameterValue
	(
		const CString&	in_scriptname,
		short			in_val,
		double			in_time = DBL_MAX
	);

	/*! Sets the parameter value for a parameter which is nested under the current parameter. If you try to set
	a read-only parameter, an error (CStatus::Unexpected) occurs.  You can check if a parameter is read-only by
	checking the condition:

	/code Parameter::GetCapabilities && siReadOnly == siReadOnly \endcode
	\tip You can set a custom property parameter even if it is created with the read-only capability.
	\note This function does not build a cache of all parameter values.  Use this function in cases where
		the object has many parameters but you only need to set the value on a few of them.

	\param in_scriptname	The parameter scripting name (see Parameter::GetScriptName).
	\param in_val	The parameter value to set.
	\param in_time	Time in frame, defaults to the current frame (represented by the value of DBL_MAX).
	\return CStatus::OK success
	\return CStatus::Unexpected Trying to set a read-only parameter.
	\return CStatus::Fail other failure
	*/
	CStatus PutParameterValue
	(
		const CString&	in_scriptname,
		LONG			in_val,
		double			in_time = DBL_MAX
	);

	/*! Sets the parameter value for a parameter which is nested under the current parameter. If you try to set
	a read-only parameter, an error (CStatus::Unexpected) occurs.  You can check if a parameter is read-only by
	checking the condition:

	/code Parameter::GetCapabilities && siReadOnly == siReadOnly \endcode
	\tip You can set a custom property parameter even if it is created with the read-only capability.
	\note This function does not build a cache of all parameter values.  Use this function in cases where
		the object has many parameters but you only need to set the value on a few of them.

	\param in_scriptname	The parameter scripting name (see Parameter::GetScriptName).
	\param in_val	The parameter value to set.
	\param in_time	Time in frame, defaults to the current frame (represented by the value of DBL_MAX).
	\return CStatus::OK success
	\return CStatus::Unexpected Trying to set a read-only parameter.
	\return CStatus::Fail other failure
	*/
	CStatus	PutParameterValue
	(
		const CString&	in_scriptname,
		const CValue&	in_val,
		double			in_time = DBL_MAX
	);

	/*! Sets the parameter value for a parameter which is nested under the current parameter. If you try to set
	a read-only parameter, an error (CStatus::Unexpected) occurs.  You can check if a parameter is read-only by
	checking the condition:

	/code Parameter::GetCapabilities && siReadOnly == siReadOnly \endcode
	\tip You can set a custom property parameter even if it is created with the read-only capability.
	\note This function does not build a cache of all parameter values.  Use this function in cases where
		the object has many parameters but you only need to set the value on a few of them.

	\param in_scriptname	The parameter scripting name (see Parameter::GetScriptName).
	\param in_val	The parameter value to set.
	\param in_time	Time in frame, defaults to the current frame (represented by the value of DBL_MAX).
	\return CStatus::OK success
	\return CStatus::Unexpected Trying to set a read-only parameter.
	\return CStatus::Fail other failure
	*/
	CStatus PutParameterValue
	(
		const CString&	in_scriptname,
		const CRef&		in_val,
		double			in_time = DBL_MAX
	);

	/*! Sets the parameter value for a parameter which is nested under the current parameter. If you try to set
	a read-only parameter, an error (CStatus::Unexpected) occurs.  You can check if a parameter is read-only by
	checking the condition:

	/code Parameter::GetCapabilities && siReadOnly == siReadOnly \endcode
	\tip You can set a custom property parameter even if it is created with the read-only capability.
	\note This function does not build a cache of all parameter values.  Use this function in cases where
		the object has many parameters but you only need to set the value on a few of them.

	\param in_scriptname	The parameter scripting name (see Parameter::GetScriptName).
	\param in_val	The parameter value to set.
	\param in_time	Time in frame, defaults to the current frame (represented by the value of DBL_MAX).
	\return CStatus::OK success
	\return CStatus::Unexpected Trying to set a read-only parameter.
	\return CStatus::Fail other failure
	*/
	CStatus PutParameterValue
	(
		const CString&	in_scriptname,
		const CString&	in_val,
		double			in_time = DBL_MAX
	);

	/*! Returns the parameter value for a parameter which is nested under the current parameter. If the parameter
	is unable to retrieve the value, it will return an empty CValue. If the parameter value is a string and it has
	not been set then it will return an empty string.

	\note This function does not build a cache of all parameter values.  Use this function in cases where
		the object has many parameters but you only need to get the value on a few of them.

	\param in_name Name of the parameter for which we want to get the value.
	\param in_time Time in frame defaults to the current frame (represented by the value of DBL_MAX).
	\return The parameter value.
	*/
	CValue GetParameterValue
	(
		const CString& in_name,
		double in_time=DBL_MAX
	) const;

	/*! Returns true if the object is animated by a specific animation source type and false otherwise.
	\param in_sourceType Animation source type,the default value is any source.
	\return true if the object is animated, false otherwise.

	\eg
	\code
		using namespace XSI;
		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject myGrid;
		root.AddGeometry( L"Grid", L"MeshSurface", CString(L"MyGrid"), myGrid );

		Parameter posxParam = myGrid.GetParameters().GetItem( L"posx" );

		FCurve fcrv;
		posxParam.AddFCurve( siStandardFCurve, fcrv );

		CRefArray paramArray = myGrid.GetParameters();

		for (LONG i=0; i<paramArray.GetCount(); i++ )
		{
			Parameter param = paramArray[i];
			app.LogMessage( param.GetFullName() + L" is node animated: "
				+ CValue(param.IsAnimated()).GetAsText() );
		}
	\endcode
	*/
	bool IsAnimated
	(
		siSourceType in_sourceType = siAnySource
	) const;

	/*! Returns an array of references to parameters animated by a specific animation source type. Only Compound
	parameters can return an array of animated parameters.
	\param in_sourceType An object type.
	\return Array of references to the parameters.

	\eg
	\code
		using namespace XSI;
		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject myGrid;
		root.AddGeometry( L"Grid", L"MeshSurface", CString(L"MyGrid"), myGrid );

		Parameter posxParam = myGrid.GetParameters().GetItem( L"posx" );

		FCurve fcrv;
		posxParam.AddFCurve( siStandardFCurve, fcrv );

		CRefArray paramArray = myGrid.GetAnimatedParameters();

		for (LONG i=0; i<paramArray.GetCount(); i++ )
		{
			Parameter param = paramArray[i];
			app.LogMessage( param.GetFullName() + L" is node animated" );
		}
	\endcode

	*/
	CRefArray GetAnimatedParameters(
		siSourceType in_sourceType = siAnySource) const;

	/*! Returns the lock type if the parameter is locked.
	\return The type of owner
	\return ::siLockTypeNone if not locked
	\since 4.0
	*/
	siLockType GetLockType();
	/*! Returns the lock level
	\return The level of lock for this parameter
	\return ::siLockLevelNone if not locked
	\since 4.0
	*/
	siLockLevel GetLockLevel();
	/*! Returns if an parameter is locked or not.
	\return true if it is locked
	\return false otherwise
	\since 4.0
	*/
	bool IsLocked();

	/*! Locks a parameter
	\param in_level The level to lock (use ::siLockLevelAll to lock it for all levels)
	\return CStatus
	\since 4.0
	*/
	CStatus SetLock(siLockLevel  in_level=siLockLevelAll);

	/*! Unlocks a parameter.
	\param in_level The level to unlock (use ::siLockLevelAll to lock it for all levels)
	\param in_password The optional password to pass when the scene locks are password protected
	\return CStatus
	\since 4.0
	*/
	CStatus UnSetLock(siLockLevel in_level=siLockLevelAll, const CString&in_password=CString() );

	/*! Returns the tags (::siTags) on this parameter. Tags are similar to marked parameters except that
	you can use tags to flag a parameter for further processing by a script or custom command. Also, tags
	are saved with the scene, unlike marked parameters.

	Tags are easy to query and organize, and you can key, plot, store, and transfer tags to another program
	using the object model or C++ API. You can set one tag on a parameter or up to 10 tags named Tag1, Tag2,
	etc. Many parameters can be tagged with the same Tag value, which allows you to easily get the list of
	all parameters for a specific tag.
	\return The tags on this parameter.
	\since 4.0
	*/
	siTags GetTags();

	/*! Sets the tags on this parameter. Tags are similar to marked parameters except that you can use tags
	to flag a parameter for further processing by a script or custom command. Also, tags are saved with the
	scene, unlike marked parameters.

	Tags are easy to query and organize, and you can key, plot, store, and transfer tags to another program
	using the object model or C++ API. You can set one tag on a parameter or up to 10 tags named Tag1, Tag2,
	etc. Many parameters can be tagged with the same Tag value, which allows you to easily get the list of
	all parameters for a specific tag.
	\param in_eVal The Tags to set
	\return CStatus::OK success
	\return CStatus::Fail failure
	\since 4.0
	*/
	CStatus PutTags( siTags in_eVal );

	/*! Creates a new runtime scripted operator and connects its output to this object. If the script code
	is not specified then a default implementation will be used. For parameter connections a simple
	assignment of the current value be use created for example 'out.value = 0.00'.

	Specifying the scripting language is optional. If not specified then current scripting language user
	preference will used.

	\tip This is the C++ API version of the \xl %AddScriptedOp command, which also creates a runtime scripted
		operator. To create a Self-Installed Custom %Operator, use the Parameter::AddCustomOp instead.

	\param in_code The script code containing the implementation of the scripted operator.
	\param in_inputs Array of objects or parameters to be connected to input ports.
	\param in_name Name of the new scripted operator
	\param in_language The script language of the new scripted operator. If not specified the value current
		user preference for the scripting language will be used.
	\param io_pst Returned error status
	\return A reference to the new runtime scripted operator
	\return Empty CRef() on failure
	\since 4.0
	*/
	CRef AddScriptedOp( const CString& in_code=L"", const CRefArray& in_inputs = CRefArray(), const CString& in_name=L"", const CString& in_language=L"", CStatus* io_pst = 0 );

	/*! Creates a new scripted operator and connects its output to this object. If the scripting file
	is not specified then a default implementation will be used. For parameter connections a simple assignment
	of the current value be use created for example 'out.value = 0.00'.

	Specifying the scripting language is optional. If not specified then the language associated with the file
	extension will be used. If this cannot be determined then the current scripting language user preference will
	used.

	\tip This is the C++ API version of the \xl %AddScriptedOpFromFile command, which also creates a runtime
		scripted operator. To create a Self-Installed Custom %Operator, use the Parameter::AddCustomOp instead.

	\param in_filename The script filename containing the implementation of the scripted operator.
	\param in_inputs Array of objects or parameters to be connected to input ports.
	\param in_name Name of the new scripted operator
	\param in_language The script language of the new scripted operator. If not specified the value current
		user preference for the scripting language will be used.
	\param io_pst Returned error status
	\return A reference to the new scripted operator
	\return Empty CRef() on failure
	\since 4.0
	*/
	CRef AddScriptedOpFromFile( const CString& in_filename = L"", const CRefArray& in_inputs = CRefArray(), const CString& in_name = L"", const CString& in_language = L"", CStatus* io_pst = 0 );

	/*! Creates a new Self-Installed Custom %Operator and connects its output to this object.
	\tip This is the C++ API version of the \xl %AddCustomOp command, which also creates a Self-Installed
		Custom %Operator. To create a runtime scripted operator, use the Parameter::AddScriptedOp function.
	\param in_type The type of a operator.  A PluginItem must be installed by this name.
	\param in_inputs Array of objects or parameters to be connected to input ports.
	\param in_name Name of the new custom operator.  If not specified the type is used to determine a name for the operator.
	\param io_pst Returned error status
	\return A reference to the new CustomOperator
	\return Empty CRef() on failure
	\since Gemini
	*/
	CRef AddCustomOp( const CString& in_type, const CRefArray& in_inputs = CRefArray(), const CString& in_name=L"", CStatus* io_pst = 0 );

	/*! Returns the X3DObject to which the parameter belongs.
	/note If this method is called from the Update() context of a custom operator it will return an invalid X3DObject.
	\return The X3DObject to which this object belongs.
	\since 5.0
	*/
	X3DObject GetParent3DObject() const;

	/*! Returns the Model to which the parameter belongs.
	/note If this method is called from the Update() context of a custom operator
	it will return an invalid Model.
	\return The Model to which this object belongs.
	\since 5.0
	*/
	Model GetModel() const;

	/*!	Returns true if the parameter has instance values. A parameter has instance values
	if the parameter belongs to a shared object, such as a shared property or a shader
	connected to a shared material. Note, however, that not all shader parameters support
	instance values.

	Parameters with instance values include ImageShader.tspace_id, Material.ImageClipName,
	Material.UV, and Material.CAV. For example, a material is owned by the Material Library but may be used
	by many objects. Different objects can have different values for the ImageShader.tspace_id parameter.

	See the MaterialLibrary object for an example of the usage.

	\return True if the parameter has instance values
	\sa	Material::GetShaderInputType, Shader::GetShaderInputType
	\since 5.0
	*/
	bool HasInstanceValue() const;

	/*! Returns true if the parameter instance value supports this type of value.
	This is useful for Shaders that need to determine what types of object a texturespace
	parameter supports. Some texturespace parameters may support vertex color (CAV) properties only, while others may
	support a more extensive list of properties.

	Texturespace parameters are instance values that support object binding. This allows an instance value
	to be a string value that references an object that may already exist or one that is yet to be created.
	The parameter is 'bound' to the object when the parameter is used.

	If in_val is a string, IsSupportedInstanceValue tries to bind to the referenced object, and tests if it is supported.
	If the object doesn't exist, IsSupportedInstanceValue returns true, because the object may be created later.

	See the MaterialLibrary object for an example usage.

	\sa	Material::GetShaderInputType, Shader::GetShaderInputType

	\param in_obj The object to test the value
	\param in_val The value to test.
	\return True if the parameter instance value supports this type of value
	\since 5.0
	*/
	bool IsSupportedInstanceValue(const CRef& in_obj, const CValue& in_val ) const;

	/*! Returns an instance value for the specified object. Instance values apply only to shared Property objects
	or Shader objects connected to a shared Material object. For example,
	a Shader texturespace parameter is an instance value and so is a rendermap tspace_id parameter.

	Instance values cannot be animated, so this function does not need a time argument.

	See the MaterialLibrary object for an example of the usage.

	\sa	Parameter::HasInstanceValue, Material::GetShaderInputType, sa	Shader::GetShaderInputType

	\param in_obj The object for which the instance value should be retrieved.
	\param in_bindtoobject If the instance value supports object binding then GetInstanceValue
		attempts to retrieve and return that object as the instance value. If the object
		doesn't exist, GetInstanceValue fails.
	\return Instance value
	\since 5.0
	*/
	CValue GetInstanceValue(const CRef& in_obj, bool in_bindtobject = false ) const;

	/*! Sets an instance value for a specified object. Instance values  apply only to shared Property objects
	or Shader objects connected to a shared Material object. For example, the texturespace parameter is an
	instance value.

	Instance values cannot be animated, so this function does not need a time argument.

	For parameters supporting object binding (such as texturespace), %PutInstanceValue accepts a valid CRef
	as an instance value. It checks that the object is supported and sets the instance value to
	the name of the object. If the object is not supported, %PutInstanceValue returns CStatus::Fail.

	If the object does not exist, you can set the instance value to the name of the object by setting in_bindtovalidate to false.

	See the MaterialLibrary object for an example of the usage.

	\sa	Parameter::HasInstanceValue, Parameter::IsSupportedInstanceValue, Material::GetShaderInputType, Shader::GetShaderInputType

	\param in_obj The object to which the instance value should be assigned.
	\param in_val The instance value for the object. If the parameter supports object binding then
		this may be a CString containing the name of the object. The value may also be a CRef, in
		which case, if the object is supported then the object name is used
		as the instance value.
	\param in_bindtovalidate If the instance value supports object binding and the in_val is a string,
		then %PutInstanceValue tests if the object specified by the string exists and is supported.
		If the object does not exist, %PutInstanceValue returns CStatus::Fail.
	\return CStatus::OK success
	\return CStatus::Fail failure
	\since 5.0
	*/
	CStatus PutInstanceValue(const CRef& in_obj, const CValue& in_val, bool in_bindtovalidate = false);

	protected:
	mutable CParameterRefArray m_paramArray;

	private:
	Parameter * operator&() const;
	Parameter * operator&();
};

};
#endif // __XSIPARAMETER_H__
