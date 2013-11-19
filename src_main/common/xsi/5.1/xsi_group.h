//*****************************************************************************
/*!
   \file xsi_group.h
   \brief Group class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIGROUP_H__
#define __XSIGROUP_H__

#include <xsi_sceneitem.h>

namespace XSI {

//*****************************************************************************
/*! \class Group xsi_group.h
	\brief The Group class represents a collection of related objects.
 */
//*****************************************************************************

class SICPPSDKDECL Group : public SceneItem
{
public:
	/*! Constructs a Group object. */
	Group();

	/*! Destroys a Group object. */
	~Group();

	/*! Constructs a Group object from a CRef object.
	\param in_ref A reference to a group.
	*/
	Group(const CRef& in_ref);

	/*! Constructs a new Group object from an existing Group object.
	\param in_obj An existing Group object to copy into this Group object.
	*/
	Group(const Group& in_obj);


	/*! Returns true if this object supports the functionality of a specified class. For example,
	a Group is a type of SceneItem, so a Group object supports SceneItem functionality.
	\param in_ClassID Test if this object supports this class.
	\return True if this object supports the specified class, and false otherwise.
	*/
	bool IsA( siClassID in_ClassID) const;

	/*! Returns the class ID for this object.
	\return The class ID.
	*/
	siClassID GetClassID() const;

	/*! Assigns a Group object to an existing Group object.
	\param in_obj A Group object to be copied into this object.
	\return The reinitialized Group object.
	*/
	Group& operator=(const Group& in_obj);

	/*! Assigns a CRef to this Group object. The Group object is cleared if the CRef is not a reference
	to an object that supports the Group class.
	\param in_ref A reference to an object that supports the Group class.
	\return The reinitialized Group object.
	*/
	Group& operator=(const CRef& in_ref);

	/*! Tests whether an object is a member of the group. For large numbers of objects, it is faster to
	enumerate the group members and use SIObject::IsEqualTo to find the object you're looking for and
	ProjectItem::GetBranchFlag to check whether the object was added as a branch member.
	\param in_ref The object to find in the group.
	\param in_branchmember If True, check if the object is a branch member.
	\return True if the object is a member, and False otherwise.
	\since 5.0
	*/
	bool IsMember(const CRef& in_ref, bool in_branchmember=false) const;

	/*! Returns the SceneItem objects, such as X3DObject and Cluster, that represent the group members.
	\return An array containing the members of the group.
	\since 5.0
	*/
	CRefArray GetMembers() const;

	/*! Returns the SceneItem objects, such as X3DObject and Cluster, for the group members and the
	children of any branch members.
	\return An array containing the members of the group.
	\since 5.0
	*/
	CRefArray GetExpandedMembers() const;

	/*! Adds a single member to the group.
	\param in_ref The object to add to the group.
	\param in_branchmember If set to true, the object is added as a branch member. All objects in a branch
		member's hierarchy inherit properties applied to the group.
	\since 5.0
	*/
	CStatus AddMember(const CRef& in_ref, bool in_branchmember=false );

	/*! Adds the specified members to the group.
	\param in_refs An array of objects to add to the group.
	\param in_branchmember If set to true, the objects are added as branch members. All objects in a branch
		member's hierarchy inherit properties applied to the group.
	\since 5.0
	*/
	CStatus AddMembers(const CRefArray& in_refs, bool in_branchmember=false);

	/*! Removes a single member from the group.
	\param in_ref The member to remove.
	\since 5.0
	*/
	CStatus RemoveMember(const CRef& in_ref);

	/*! Removes the specified members from the group.
	\param in_refs An array of members to remove.
	\since 5.0
	*/
	CStatus RemoveMembers(const CRefArray& in_refs);

	/*! Removes all members from the group.
	\since 5.0
	*/
	CStatus RemoveAllMembers(void);

	private:
	Group * operator&() const;
	Group * operator&();
};

};

#endif // __XSIGROUP_H__
