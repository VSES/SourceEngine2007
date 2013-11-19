//*****************************************************************************
/*!
   \file xsi_sceneitem.h
   \brief SceneItem class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSISCENEITEM_H__
#define __XSISCENEITEM_H__

#include <xsi_projectitem.h>
#include <xsi_customproperty.h>
#include <xsi_status.h>

namespace XSI {

class Material;
class Envelope;
class Property;

//*****************************************************************************
/*! \class SceneItem xsi_sceneitem.h
	\brief The SceneItem object represents an object in the scene (that is, something with properties)
	in %XSI from which all other scene objects derive.

 */
//*****************************************************************************

class SICPPSDKDECL SceneItem : public ProjectItem
{
public:
	/*! Default constructor. */
	SceneItem();

	/*! Default destructor. */
	~SceneItem();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	SceneItem(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	SceneItem(const SceneItem& in_obj);

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
	\return The new SceneItem object.
	*/
	SceneItem& operator=(const SceneItem& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new SceneItem object.
	*/
	SceneItem& operator=(const CRef& in_ref);

	/*! Returns an array of references to all applied Property objects on the object. For example,
	if you have a material installed on the node and the branch, and the X3DObject is also in a Group
	that has a material, then only the group material will appear in this array.
	\return Array of references to properties.
	*/
	CRefArray 	GetProperties() const;

	/*! Returns the object's applied Material object.
	\return The Material applied on this object.
	*/
	Material	GetMaterial() const;

	/*! Returns an array of references to Envelope object on this object.
	\return Array of references to the envelopes applied on this object.
	*/
	CRefArray 	GetEnvelopes() const;

	/*!	Applies a flexible envelope to the specified object.
	\param in_deformers			Array of references to object which will act as deformers.
	\param in_DeformOnBranch	Use all objects in deformers branch.
	\param in_EnvOnBranch		Apply envelope to all objects in the SceneItem's branch.
	\param io_env				The Envelope object.
	\return CStatus::OK success
	\return CStatus::Fail other failure
	*/
	CStatus 	ApplyEnvelope
	(
		const CRefArray& in_deformers,
		siBranchFlag	 in_DeformOnBranch,
		siBranchFlag	 in_EnvOnBranch,
		Envelope&		 io_env
	);

	/*! Returns an array of references to all applied local Property objects on the  object.
	\return Array of references to properties.
	*/
	CRefArray 	GetLocalProperties() const;

	/*! Creates and adds material to a SceneItem object. If the SceneItem is a Model then the
	BranchFlag will be ignored since a material can only be added on a Model in branch.
	\param in_preset Name of a shader preset to apply to object. The shader preset is optional. If no
		shader preset is supplied then a material is created and added but it will have not shaders
		connected to its parameters.
	\param in_branch Apply material on branch or the node of object.
	\param in_name	 Represents the name of the new Material object.
	\param io_mat    The added material.
	\return CStatus::OK success
	\return CStatus::Fail other failure
	*/
	CStatus 	AddMaterial
	(
		const CString&	in_preset,
		bool			in_branch,
		const CString&	in_name,
		Material&		io_mat
	);

	/*! Creates and adds a property to a SceneItem object.
	\param in_preset This argument contains the filename or full path to a Preset file. The type of
		property that is created is determined by this argument. For example \c CustomProperty creates
		an empty CustomProperty and \c UserDataMap creates a UserDataMap. Is is also possible to specify
		the name of a CustomProperty that is installed as a PluginItem.
	\param in_bBranch Apply property on branch or the node of object.
	\param in_name	 The name of the new Property object (see SIObject::GetName). If not specified
		the name is derived from the in_preset argument.
	\return The new Property object.
	\sa SceneItem::AddCustomProperty, SceneItem::GetProperties, SceneItem::GetLocalProperties, CustomProperty,
		UserDataMap, UserDataBlob, PluginRegistrar::RegisterProperty
	\since 4.0
	*/
	Property AddProperty
	(
		const CString&	in_preset,
		bool			in_bBranch = false,
		const CString&	in_name = CString()
	);

	/*! As of v4.0, this signature is obsolete and is replaced by the other signature.
	\deprecated 4.0 (but still maintained for backward compatibility)
	*/
	CStatus 	AddProperty
	(
		const CString&	in_preset,
		bool			in_bBranch ,
		const CString&	in_name,
		Property&		out_prop
	);

	/*! Creates and adds an empty CustomProperty to a SceneItem object.
	\param in_name	 Represents the name of the new CustomProperty object.
	\param in_bBranch True to apply property on branch or the node of object.
	\retval out_customprop   Returns the added property.
	\return CStatus::OK success
	\return CStatus::Fail other failure
	*/
	CStatus 	AddCustomProperty
	(
		const CString&	in_name,
		bool			in_bBranch ,
		CustomProperty&	out_customprop
	);

	/*! Assigns a material to the scene item object
	\param in_Material	 The material to assign
	\return CStatus::OK success
	\return CStatus::Fail other failure
	\since 4.0
	*/
	CStatus		SetMaterial
	(
		Material&		in_Material
	);

	private:
	SceneItem * operator&() const;
	SceneItem * operator&();
};

};

#endif // __XSISCENEITEM_H__
