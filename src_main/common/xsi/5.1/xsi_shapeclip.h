//*****************************************************************************
/*!
   \file xsi_shapeclip.h
   \brief ShapeClip class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIPASS_H__
#define __XSIPASS_H__

#include <xsi_projectitem.h>

namespace XSI {

//*****************************************************************************
/*! \class ShapeClip xsi_shapeclip.h
	\brief A ShapeClip is an instance of a shape key (source) at a particular position along a shape
	track in the animation mixer.

	You can create shape clips using the \xl SaveShapeKey command.
	To enumerate existing shape clips you must pass the full string path of the shape track's
	ClipList to the \xl EnumElements command which returns a collection of ShapeClip objects.

	You can create multiple clips of the same source (using the \xl AddClip command), thereby
	returning to the same shape several times in the same animation, or rearrange the order of
	the shape clips on the tracks to change the shape animation completely.

	ShapeClip allows you to access the shape animation stored in clusters by using Clip::GetMappedItems.
	Each MappedItem object returns the ShapeKey object (a specialized kind of ClusterProperty) from
	MappedItem::GetAnimationSource. ShapeKeys contain shape animation data and you can use SIObject::GetParent
	on the ShapeKey to get the shape-animated Cluster object.

	When you save a shape key in local reference mode (siShapeLocalReferenceMode) only the deltas
	to the point's local referential are stored. If you need to get the absolute position for each
	point in the shape you will need the local referential for the point and the original position
	of the point before the shape key was applied.

	In order to get the local referential (see \xl Geometry0D.LocalReferenceFrame) and the original
	position for each point you will need to mute the shape combiner and all other operators above
	it using the \xl DeactivateAbove command.

	\sa \xl SaveShapeKey, \xl Geometry.SaveShapeKey, \xl GetDrivingActions, \xl EnumElements

*/
//*****************************************************************************

class SICPPSDKDECL ShapeClip : public ProjectItem
{
public:
	/*! Default constructor. */
	ShapeClip();

	/*! Default destructor. */
	~ShapeClip();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	ShapeClip(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	ShapeClip(const ShapeClip& in_obj);

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
	\return The new ShapeClip object.
	*/
	ShapeClip& operator=(const ShapeClip& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new ShapeClip object.
	*/
	ShapeClip& operator=(const CRef& in_ref);

	private:
	ShapeClip * operator&() const;
	ShapeClip * operator&();
};

};

#endif // __XSIPASS_H__
