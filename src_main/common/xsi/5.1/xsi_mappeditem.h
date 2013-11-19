//*****************************************************************************
/*!
   \file xsi_mappeditem.h
   \brief MappedItem class declaration.

   © Copyright 1998-2003 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************
#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIMAPPEDITEM_H__
#define __XSIMAPPEDITEM_H__

#include <xsi_siobject.h>

namespace XSI {

class ClipEffectItem;

//*****************************************************************************
/*! \class MappedItem xsi_mappeditem.h
	\brief A mapped item specifies the value driven by a specific clip instance
	(for example, an object's parameter or its geometry).

	You can get the MappedItem from Clip::GetMappedItems.

	In order to know exactly which type of AnimationSourceItem you are dealing
	with you can use SIObject::GetType which returns a value contained in the
	XSI::siAnimationSourceItemType enum.

	\sa Clip::GetMappedItems
	\since 4.0

	\eg Demonstrates how to use mapped item to look for a specific animation
	channel. Once this channel is found a clip effect is added onto it.

	\code
		using namespace XSI;

		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject myCube;
		root.AddGeometry( L"Cube", L"MeshSurface", L"", myCube );

		// Create the first animation source
		CValueArray args(9);
		CValue		outArg;
		args[0] = root;
		args[1] = L"cube.kine.local.posx,cube.kine.local.posy,cube.kine.local.posz";
		args[3] = L"StoredStaticPose";
		args[4] = true;
		args[7] = false;
		args[8] = false;
		app.ExecuteCommand( L"StoreAction", args, outArg );

		Source mySource(outArg);

		// Create the first clip
		CValueArray addClipArgs(6);
		addClipArgs[0] = root;
		addClipArgs[1] = mySource.GetFullName();
		addClipArgs[5] = L"MyClip1";
		app.ExecuteCommand( L"AddClip", addClipArgs, outArg );

		Clip myClip(outArg);
		CRefArray mappedItems = myClip.GetMappedItems();
		// Find the mapped item on which we want to add an effect.
		// We are looking for the local.posy parameter
		LONG i;
		MappedItem currItem;
		for(i = 0; i < mappedItems.GetCount(); ++i)
		{
			currItem = mappedItems[i];
			Parameter param = currItem.GetDestination();
			if(param.GetFullName().IsEqualNoCase(L"cube.kine.local.posy"))
			{
				CValueArray mappingRuleArgs(4);
				mappingRuleArgs[0] = myClip.GetFullName() + L".ActionClip";
				mappingRuleArgs[1] = param;
				mappingRuleArgs[2] = L"frame";
				mappingRuleArgs[3] = (LONG)(i + 1);

				app.ExecuteCommand(L"SetMappingRule", mappingRuleArgs, outArg);
				break;
			}
		}

		ClipEffectItem effectItem(currItem.GetClipEffectItem());
		app.LogMessage(L"The expression associated with the posy item is >> " +
						effectItem.GetExpression());
	\endcode
 */
//*****************************************************************************
class SICPPSDKDECL MappedItem : public SIObject
{
public:
	/*! Default constructor. */
	MappedItem();

	/*! Default destructor. */
	~MappedItem();

	/*! Constructor.
	* \param in_ref constant reference object.
	*/
	MappedItem(const CRef& in_ref);

	/*! Copy constructor.
	* \param in_obj constant class object.
	*/
	MappedItem(const MappedItem& in_obj);

	/*! Returns true if a given class type is compatible with this API class.
	* \param in_ClassID class type.
	* \return true if the class is compatible, false otherwise.
	*/
	bool IsA( siClassID in_ClassID) const;

	/*! Returns the type of the API class.
	* \return The class type.
	*/
	siClassID GetClassID() const;

	/*! Creates an object from another object. The newly created object is set to
	* empty if the input object is not compatible.
	* \param in_obj constant class object.
	* \return The new MappedItem object.
	*/
	MappedItem& operator=(const MappedItem& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	* set to empty if the input reference object is not compatible.
	* \param in_ref constant class object.
	* \return The new MappedItem object.
	*/
	MappedItem& operator=(const CRef& in_ref);

	/*! Returns the object which is driving the action or shape animation. This function is not
	available for compound clips. You can test to see whether the clip is a compound clip by
	testing if SIObject::GetClassID() == ::siClipContainerID on the Clip object.

	\warning In the case of Expressions or Constraints, this property will return an invalid
	CRef, because the MappedItem object does not contain the full Expression or Constraint objects.

	\return A reference to either a ShapeKey, FCurve or StaticSource.
	\sa \xl MappedItem.Source2
	*/
	CRef GetAnimationSource()const;

	/*! Returns the parameter which is driven by the mapped item. If the mapped item is a shape
	animation this function returns the cluster on which the shape key is located.
	\return A reference to either a Parameter or a Cluster.
	*/
	CRef GetDestination()const;

	/*! For all items driven by a clip there could be an associated clip effect which would affect
	the value after the evaluation of the animation source. This function returns the specific item
	in the ClipEffect which is associated to this mapped item.
	\return The associated ClipEffectItem.
	*/
	ClipEffectItem GetClipEffectItem()const;

	private:
	MappedItem * operator&() const;
	MappedItem * operator&();
};

}

#endif // __XSIMAPPEDITEM_H__
