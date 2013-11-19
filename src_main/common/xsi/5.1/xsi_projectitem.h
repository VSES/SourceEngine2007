//*****************************************************************************
/*!
   \file xsi_projectitem.h
   \brief ProjectItem class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIPROJECTITEM_H__
#define __XSIPROJECTITEM_H__

#include <xsi_siobject.h>
#include <xsi_value.h>
#include <xsi_parameter.h>

namespace XSI {

class Model;
class X3DObject;
class PPGLayout;

//*****************************************************************************
/*! \class ProjectItem  xsi_projectitem.h
	\brief The ProjectItem class represents the base content class in %XSI from which
	all other content objects derive.  Its major feature is that it provides access to
	the Parameters which represent the state of the object.

	Many classes in %XSI support this interface, for example X3DObject, Property,
	Shader, Operator, and Primitive. The classes that do not support this are lightweight
	classes like Particle, Point, Geometry and Parameter, utility classes such as CValue,
	CColor, CGraphicSequencer or classes that are not content related such as Command,
	Application and UIToolkit.

 */
//*****************************************************************************

class SICPPSDKDECL ProjectItem : public SIObject
{
public:
	/*! Default constructor. */
	ProjectItem();

	/*! Default destructor. */
	~ProjectItem();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	ProjectItem(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	ProjectItem(const ProjectItem& in_obj);

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
	\return The new ProjectItem object.
	*/
	ProjectItem& operator=(const ProjectItem& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new ProjectItem object.
	*/
	ProjectItem& operator=(const CRef& in_ref);

	/*! Returns the names of all the families the object belongs to.
	\return String representing all families to which this object belongs.
	*/
	CString GetFamilies() const;

	/*! Returns a mask of all the object's capabilities.
	\return The object's capabilities.
	*/
	siCapabilities GetCapabilities() const;

	/*! Disables or enables any Capability flag set on the object.
	For example an object can be hidden from the Scene Explorer by setting the siNotInspectable flag.
	Changing the capability flags only affects one particular instance of the object.

	\param in_flag Specify the flag to change
	\param in_bEnable True to set the capability; false to remove it.
	\sa Parameter::GetCapabilities
	\since Gemini
	*/
	CStatus PutCapabilityFlag( siCapabilities in_flag, bool in_bEnable ) ;


	/*! Returns the object's selected state.
	\param in_branchFlag Is the object selected in node or branch? the default
	value is siNode. This argument only applies to objects that have children such as
	the X3DObject and Model objects.
	\return true if the current object is selected.
	*/
	bool GetSelected
	(
		siBranchFlag in_branchFlag = siNode
	) const;

	/*! Sets the object's selected state.
	\param in_newState new state
	\param in_branchFlag Is the object to be selected in node or branch? the default
		value is siNode. This argument only applies to objects that have children such as
		the X3DObject and Model objects.
	\return CStatus::OK success
	\return CStatus::Fail other failure
	*/
	CStatus PutSelected
	(
		bool in_newState,
		siBranchFlag in_branchFlag = siNode
	);

	/*! Returns an array of references to all owners of this object. If this function is
	called from the Update() context of a custom operator it will return an empty CRefArray.
	\return Array of references to owners.
	*/
	CRefArray GetOwners() const;

	/*! Returns an array of references to all parameters within the object that support
	read access; that is, you can get their value using Parameter::GetValue.

	\note This function builds a cache of all parameters. Building the cache
	can be costly performance wise. Use this function in cases where the
	object has many parameters and you need to access most of them. It is more
	suitable to use GetParameter if you only need to access a few parameters.

	\return Array of Parameter objects
	\sa	ProjectItem::GetParameter, Parameter::GetParameter

	\return Array of references to parameters.
	*/
	CParameterRefArray& GetParameters() const;

	/*! Returns the Model to which the object belongs. If this method is called from the
	Update() context of a custom operator it will return an invalid Model.
	\return The Model to which this object belongs.
	*/
	Model GetModel() const;

	/*! Returns true if the object has the required kind of pset relationship
	with its parent and false otherwise

	\note Prior to v5.0 this was called IsA(). This conflicted with the other
		IsA() method for testing the class id. In the scripting object model
		IsRelationship(siPSetRelationship) maps to \xl ProjectItem.IsA.

	\param in_PSetRelationship Type of PSet relationship to check.
	\return true if the object has the specified type of pset relationship with its parent; otherwise false.
	\since 5.0
	*/
	bool IsRelationship(siPSetRelationship in_PSetRelationship) const;

	/*! Returns true if the object belongs to specified family and false otherwise.
	\param in_family Family (or families) to check
	\return true if the object belongs to the specified family; otherwise false.
	*/
	bool BelongsTo( const CString& in_family) const;


	/*! Returns true if this object matches a given type and false otherwise.
	\param in_objectType an object type.
	\return true if this object matches a given type; otherwise false.
	*/
	bool IsKindOf(const CString& in_objectType) const;

	/*!Returns true if the object is animated by a specific animation source type and false otherwise.
	\param in_sourceType Animation source type. The default value is any source.
	\param in_bLocal Specifies whether the animation to check is local to the object or not. If true
		then all animated parameters that come from propagated properties or from intermediate nodes
		are ignored. Intermediate nodes are nested under the object but are not necessary owned by the
		object itself. For instance, the camera object nested under a \c Texture_Projection_Def property
		is not considered unless the target object is the projection property itself. When the flag is
		set to false (default), all parameters are considered.
	\return true if the object is animated; false otherwise.
	*/
	bool IsAnimated
	(
		siSourceType	in_sourceType = siAnySource,
		bool			in_bLocal = false
	) const;

	/*! Returns an array of references to parameters that are animated by a specific animation source type.
	If this method is called from the Update() context of a custom operator it will return an empty CRefArray.
	\param in_sourceType an object type.
	\param in_bLocal Specifies whether the animation to check is local to the object or not. If true
		then all animated parameters that come from propagated properties or from intermediate nodes
		are ignored. Intermediate nodes are nested under the object but are not necessary owned by the
		object itself. For instance, the camera object nested under a \c Texture_Projection_Def property
		is not considered unless the target object is the projection property itself. When the flag is
		set to false (default), all parameters are considered.
	\return Array of references to Parameter objects.
	*/
	CRefArray GetAnimatedParameters
	(
		siSourceType	in_sourceType = siAnySource,
		bool			in_bLocal = false
	) const;

	/*! Returns a version of the object at a particular time.
	\param in_dFrame Time in frames at which to evaluate object, the default value is DBL_MAX which
		means the current time.
	\return Array of references to Parameter objects.
	*/
	CRef EvaluateAt( double in_dFrame = DBL_MAX);

	/*! Sets the value of a parameter nested under this ProjectItem. If you try
	to set a read-only parameter, an error (CStatus::Unexpected) occurs. You can
	check if a parameter is	read-only by checking the condition:
	Parameter::Capabilities & siReadOnly == siReadOnly. Note that you can set a
	custom property parameter even if it is created with the readonly capability.

	\note This function does not build a cache of all parameter values. Use
	this function in cases where the object has many parameters but you only
	need to set the value on a few of them.

	\param in_scriptname	The parameter's scripting name.
	\param in_val	The parameter value to set.
	\param in_time	Time in frame, defaults to the current frame (represented by the value of DBL_MAX).
	\return CStatus::OK success
	\return CStatus::Fail other failure
	\return CStatus::Unexpected Trying to set a read-only parameter.
	\sa Parameter::PutValue, Parameter::GetScriptName,ProjectItem::PutParameterValue2
	*/
	CStatus PutParameterValue( const CString& in_scriptname, bool in_val, double in_time = DBL_MAX );

	/*! Sets the value of a parameter nested under this ProjectItem. If you try
	to set a read-only parameter, an error (CStatus::Unexpected) occurs. You can
	check if a parameter is	read-only by checking the condition:
	Parameter::Capabilities & siReadOnly == siReadOnly. Note that you can set a
	custom property parameter even if it is created with the readonly capability.

	\note This function does not build a cache of all parameter values. Use
	this function in cases where the object has many parameters but you only
	need to set the value on a few of them.

	\param in_scriptname	The parameter script name.
	\param in_val	The parameter value to set.
	\param in_time	Time in frame, defaults to the current frame(this is
	represented by the value of DBL_MAX).
	\return CStatus::OK success
	\return CStatus::Fail other failure
	\return CStatus::Unexpected Trying to set a read-only parameter.
	\sa ProjectItem::PutParameterValue2
	*/
	CStatus PutParameterValue( const CString& in_scriptname, double in_val, double in_time = DBL_MAX);

	/*! Sets the value of a parameter nested under this ProjectItem. If you try
	to set a read-only parameter, an error (CStatus::Unexpected) occurs. You can
	check if a parameter is	read-only by checking the condition:
	Parameter::Capabilities & siReadOnly == siReadOnly. Note that you can set a
	custom property parameter even if it is created with the readonly capability.

	\note This function does not build a cache of all parameter values. Use
	this function in cases where the object has many parameters but you only
	need to set the value on a few of them.

	\param in_scriptname	The parameter script name.
	\param in_val	The parameter value to set.
	\param in_time	Time in frame, defaults to the current frame(this is
	represented by the value of DBL_MAX).
	\return CStatus::OK success
	\return CStatus::Fail other failure
	\return CStatus::Unexpected Trying to set a read-only parameter.
	\sa ProjectItem::PutParameterValue2
	*/
	CStatus PutParameterValue( const CString& in_scriptname, float in_val, double in_time = DBL_MAX );

	/*! Sets the value of a parameter nested under this ProjectItem. If you try
	to set a read-only parameter, an error (CStatus::Unexpected) occurs. You can
	check if a parameter is	read-only by checking the condition:
	Parameter::Capabilities & siReadOnly == siReadOnly. Note that you can set a
	custom property parameter even if it is created with the readonly capability.

	\note This function does not build a cache of all parameter values. Use
	this function in cases where the object has many parameters but you only
	need to set the value on a few of them.

	\param in_scriptname	The parameter script name.
	\param in_val	The parameter value to set.
	\param in_time	Time in frame, defaults to the current frame(this is
	represented by the value of DBL_MAX).
	\return CStatus::OK success
	\return CStatus::Fail other failure
	\return CStatus::Unexpected Trying to set a read-only parameter.
	\sa ProjectItem::PutParameterValue2
	*/
	CStatus PutParameterValue( const CString& in_scriptname, short in_val, double in_time = DBL_MAX );

	/*! Sets the value of a parameter nested under this ProjectItem. If you try
	to set a read-only parameter, an error (CStatus::Unexpected) occurs. You can
	check if a parameter is	read-only by checking the condition:
	Parameter::Capabilities & siReadOnly == siReadOnly. Note that you can set a
	custom property parameter even if it is created with the readonly capability.

	\note This function does not build a cache of all parameter values. Use
	this function in cases where the object has many parameters but you only
	need to set the value on a few of them.

	\param in_scriptname	The parameter script name.
	\param in_val	The parameter value to set.
	\param in_time	Time in frame, defaults to the current frame(this is
	represented by the value of DBL_MAX).
	\return CStatus::OK success
	\return CStatus::Fail other failure
	\return CStatus::Unexpected Trying to set a read-only parameter.
	\sa ProjectItem::PutParameterValue2
	*/
	CStatus PutParameterValue( const CString& in_scriptname, LONG in_val, double in_time = DBL_MAX );

	/*! Sets the value of a parameter nested under this ProjectItem. If you try
	to set a read-only parameter, an error (CStatus::Unexpected) occurs. You can
	check if a parameter is	read-only by checking the condition:
	Parameter::Capabilities & siReadOnly == siReadOnly. Note that you can set a
	custom property parameter even if it is created with the readonly capability.

	\note This function does not build a cache of all parameter values. Use
	this function in cases where the object has many parameters but you only
	need to set the value on a few of them.

	\param in_scriptname	The parameter script name.
	\param in_val	The parameter value to set.
	\param in_time	Time in frame, defaults to the current frame(this is
	represented by the value of DBL_MAX).
	\return CStatus::OK success
	\return CStatus::Fail other failure
	\return CStatus::Unexpected Trying to set a read-only parameter.
	\sa ProjectItem::PutParameterValue2
	*/
	CStatus	PutParameterValue( const CString& in_scriptname, const CValue& in_val, double in_time = DBL_MAX );

	/*! Sets the value of a parameter nested under this ProjectItem. If you try
	to set a read-only parameter, an error (CStatus::Unexpected) occurs. You can
	check if a parameter is	read-only by checking the condition:
	Parameter::Capabilities & siReadOnly == siReadOnly. Note that you can set a
	custom property parameter even if it is created with the readonly capability.

	\note This function does not build a cache of all parameter values. Use
	this function in cases where the object has many parameters but you only
	need to set the value on a few of them.

	\param in_scriptname	The parameter script name.
	\param in_val	The parameter value to set.
	\param in_time	Time in frame, defaults to the current frame(this is
	represented by the value of DBL_MAX).
	\return CStatus::OK success
	\return CStatus::Fail other failure
	\return CStatus::Unexpected Trying to set a read-only parameter.
	\sa ProjectItem::PutParameterValue2
	*/
	CStatus PutParameterValue( const CString& in_scriptname, const CRef& in_val, double in_time = DBL_MAX );

	/*! Sets the value of a parameter nested under this ProjectItem. If you try
	to set a read-only parameter, an error (CStatus::Unexpected) occurs. You can
	check if a parameter is	read-only by checking the condition:
	Parameter::Capabilities & siReadOnly == siReadOnly. Note that you can set a
	custom property parameter even if it is created with the readonly capability.

	\note This function does not build a cache of all parameter values. Use
	this function in cases where the object has many parameters but you only
	need to set the value on a few of them.

	\param in_scriptname	The parameter script name.
	\param in_val	The parameter value to set.
	\param in_time	Time in frame, defaults to the current frame(this is
	represented by the value of DBL_MAX).
	\return CStatus::OK success
	\return CStatus::Fail other failure
	\return CStatus::Unexpected Trying to set a read-only parameter.
	\sa ProjectItem::PutParameterValue2
	*/
	CStatus PutParameterValue( const CString& in_scriptname, const CString& in_val, double in_time = DBL_MAX );

	/*! Returns the value of a parameter nested under the ProjectItem. If the
	parameter doesn't exist an empty CValue is returned. If the parameter value
	is a string	and it has not been set then an empty string is returned.

	\note This function does not build a cache of all parameter values.  Use this
	function in cases where the object has many parameters but you only need to
	get the value on a few of them.

	\param in_scriptname scripting name of the parameter for which we want to get the value.
	\param in_time Time in frame defaults to the current frame (represented by the value of DBL_MAX).

	\return The parameter value.
	\sa Parameter::GetValue, Parameter::GetScriptName
	*/
	CValue GetParameterValue( const CString& in_scriptname, double in_time=DBL_MAX )const;

	/*! Returns a particular parameter of this object, if the parameter script name does not exist the Parameter
	will be invalid and all functions will return default values.

	\note This function does not build a cache of all parameter values.  Use this
	function in cases where the object has many parameters but you only need to
	get a few of them.

	\param in_scriptname script name of the parameter.
	\return A Parameter object.
	\sa Parameter::GetParameters
	*/
	Parameter GetParameter( const CString& in_scriptname ) const;

	/*! Returns the layout associated with this project item.  The Layout defines how the Parameters of
	the ProjectItem are presented visually when the object is inspected.
	\since 4.0
	*/
	PPGLayout GetPPGLayout() const ;

	/*! Returns the lock type if the parameter is locked.
	\return The type of owner (::siLockOwnerNone if not locked).
	\since 4.0
	*/
	siLockType GetLockType();
	/*! Returns the lock level for this parameter.
	\return The level of lock (::siLockLevelNone if not locked).
	\since 4.0
	*/
	siLockLevel GetLockLevel();
	/*! Indicates whether or not a parameter is locked.
	\return true if the Parameter is locked.
	\since 4.0
	*/
	bool IsLocked();

	/*! Locks a parameter.
	\param in_level The level to lock. Can be ::siLockLevelAll for all levels
	\return CStatus
	\since 4.0
	*/
	CStatus SetLock(siLockLevel  in_level=siLockLevelAll);

	/*! Unlocks a parameter.
	\param in_level The level to unlock. Can be ::siLockLevelAll for all levels
	\param in_password The optional password to pass when the scene locks are password protected
	\return CStatus
	\since 4.0
	*/
	CStatus UnSetLock(siLockLevel in_level=siLockLevelAll, const CString&in_password=CString() );

	/*! Returns an array of references to parameters that are tagged by a specific tag
	\param in_tags the tags to check
	\param in_bLocal Specifies whether the tags to check is local to the parameters or not.
		If true then all tagged parameters that come from propagated properties or from
		intermediate nodes are ignored. Intermediate nodes are nested under the object but
		are not necessary owned by the object itself. For instance, the camera object nested
		under a \c Texture_Projection_Def property is not considered unless the target object
		is the projection property itself. When the flag is set to false (default), all
		parameters are considered.
	\return Array of references to Parameter objects.
	\since 4.0
	*/
	CRefArray GetTaggedParameters
	(
		siTags		in_tags = siTagAll,
		bool		in_bLocal = false
	) const;

	/*! Returns an array of references to parameters that are owner of a lock
	\param in_eLevel the level to check
	\param in_bLocal Specifies whether the lock to check is local to the object or not.
		If true then all lock owners that come from propagated properties or from
		intermediate nodes are ignored. Intermediate nodes are nested under the object
		but are not necessary owned by the object itself. For instance, the camera object
		nested under a \c Texture_Projection_Def property is not considered unless the
		target object is the projection property itself. When the flag is set to false
		(default), all parameters are considered.
	\return Array of references to Parameter objects.
	\since 4.0
	*/
	CRefArray GetLockOwners
	(
		siLockLevel in_eLevel = siLockLevelAll,
		bool		in_bLocal = false
	) const;

	/*! Creates a new runtime scripted operator and connects its output to this object. If the scripting file
	is not specified then a default implementation will be used. For parameter connections a simple assignment
	of the current value be use created for example 'out.value = 0.00'.

	Specifying the scripting language is optional. If not specified then the language associated with the file
	extension will be used. If this cannot be determined then the current scripting language user preference will
	used.

	\tip This is the C++ API version of the \xl %AddScriptedOpFromFile command, which also creates a runtime
		scripted operator. To create a Self-Installed Custom %Operator, use the ProjectItem::AddCustomOp instead.

	\param in_code The script containing the implementation of the scripted operator.
	\param in_inputs Array of objects or parameters to be connected to input ports.
	\param in_name Name of the new scripted operator
	\param in_language The script language of the new scripted operator. If not specified the value current
		user preference for the scripting language will be used.
	\param in_mode	Specifies the construction mode in which the operator will be applied. This only applies
		to output connections made to Geometry objects, the mode will be ignored for all other types of connections.
	\retval out_pst Optional error status
	\return The new runtime scripted operator
	\return Empty CRef() on failure
	\since 4.0
	*/
	CRef AddScriptedOp(
			const CString& in_code=L"",
			const CRefArray& in_inputs = CRefArray(),
			const CString& in_name=L"",
			const CString& in_language=L"",
			siConstructionMode in_mode = siConstructionModeDefault,
			CStatus* out_pst = 0 );

	/*! Creates a new scripted operator and connects its output to this object. If the scripting file
	is not specified then a default implementation will be used. For parameter connections a simple assignment
	of the current value be use created for example 'out.value = 0.00'.

	Specifying the scripting language is optional. If not specified then the language associated with the file
	extension will be used. If this cannot be determined then the current scripting language user preference will
	used.

	\tip This is the C++ API version of the \xl %AddScriptedOpFromFile command, which also creates a runtime
		scripted operator. To create a Self-Installed Custom %Operator, use the ProjectItem::AddCustomOp instead.

	\param in_filename The script filename containing the implementation of the scripted operator.
	\param in_inputs Array of objects or parameters to be connected to input ports.
	\param in_name Name of the new scripted operator
	\param in_language The script language of the new scripted operator. If not specified the value current
		user preference for the scripting language will be used.
	\param in_mode	Specifies the construction mode in which the operator will be applied. This only applies to
		output connections made to Geometry objects, the mode will be ignored for all other types of connections.
	\retval out_pst Optional error status
	\return A reference to the new scripted operator
	\return Empty CRef() on failure
	\since 4.0
	*/
	CRef AddScriptedOpFromFile( const CString& in_filename = L"", const CRefArray& in_inputs = CRefArray(), const CString& in_name = L"", const CString& in_language = L"", siConstructionMode in_mode = siConstructionModeDefault, CStatus* out_pst = 0 );

	/*! Creates a new Self-Installed Custom %Operator and connects its output to this object.
	\tip This is the C++ API version of the \xl %AddCustomOp command, which also creates a Self-Installed
		Custom %Operator. To create a runtime scripted operator, use the ProjectItem::AddScriptedOp function.
	\param in_type The type of a operator.  A PluginItem must be installed by this name.
	\param in_inputs Array of objects or parameters to be connected to input ports.
	\param in_name Name of the new custom operator.  If not specified the type is used to determine a name for the operator.
	\param in_mode	Specifies the construction mode in which the operator will be applied. This only applies to
		output connections made to Geometry objects, the mode will be ignored for all other types of connections.
	\retval io_pst Optional error status
	\return A reference to the CustomOperator
	\return Empty CRef() on failure
	\since Gemini
	*/
	CRef AddCustomOp(
			const CString& in_type,
			const CRefArray& in_inputs = CRefArray(),
			const CString& in_name=L"",
			siConstructionMode in_mode = siConstructionModeDefault,
			CStatus* io_pst = 0 );

	/*! Returns an array of references to objects that are master of the lock of this object.
	\return Array of references of the lock masters for this object. Empty if not locked.
	\since 4.0
	*/
	CRefArray GetLockOwners() const;

	/*! Returns the X3DObject to which the object belongs.
	\note If this function is called from the Update() context of a custom operator it will return an invalid X3DObject.
	\return The X3DObject to which this object belongs.
	\since 5.0
	*/
	X3DObject GetParent3DObject() const;

	/*! Returns the branch flag. The branch flag is used in the context of adding objects to the Selection or to a Group.
	You can check if an object was added to a group as a branch member or if an object has been branch selected
	by testing if the branchflag has been set to siBranch.

	\note To change the group member to a branch member you must remove and re-add the member to the group. To
		change a select object to a branch selected object you must deselect then reselect the object.

	\return ::siBranchFlag
	\since 5.0
	*/
	siBranchFlag GetBranchFlag() const;


	protected:
	mutable CParameterRefArray m_paramArray;

	private:
	ProjectItem * operator&() const;
	ProjectItem * operator&();

};

};

#endif // __XSIPROJECTITEM_H__
