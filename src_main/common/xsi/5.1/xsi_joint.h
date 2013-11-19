//*****************************************************************************
/*!
   \file xsi_joint.h
   \brief Joint class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************
#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIJOINT_H__
#define __XSIJOINT_H__

#include <xsi_property.h>
#include <xsi_status.h>

namespace XSI {

class X3DObject;

//*****************************************************************************
/*! \class Joint xsi_joint.h
	\brief The Joint object represents a Kinematic Joint property of a ChainBone or ChainEffector object.

	\sa ChainElement,ChainBone, ChainRoot

	\eg
	\code
		using namespace XSI;
		Application app;
		Model root = app.GetActiveSceneRoot();

		Null null1;
		root.AddNull(L"UpVectorReference",null1);

		Null null2;
		root.AddNull(L"PreferredAxisPreference",null2);

		ChainRoot myChainRoot;
		root.Add3DChain(	MATH::CVector3(0,0,0),
							MATH::CVector3(1,0,0),
							MATH::CVector3(0,0,1),
							L"",
							myChainRoot);

		CRefArray boneRefArray = myChainRoot.GetBones();

		Joint myJoint(ChainBone(boneRefArray.GetItem(0)).GetJoint());

		myJoint.PutParameterValue(L"resplane",(LONG)siUpVectorPlane);

		myJoint.PutUpVectorReference(null1);

		myJoint.PutPreferredAxisReference(null2);
	\endcode
 */
//*****************************************************************************
class SICPPSDKDECL Joint : public Property
{
public:
	/*! Default constructor. */
	Joint();

	/*! Default destructor. */
	~Joint();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	Joint(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	Joint(const Joint& in_obj);

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
	\return The new Joint object.
	*/
	Joint& operator=(const Joint& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new Joint object.
	*/
	Joint& operator=(const CRef& in_ref);

	/*! Returns the X3DObject object acting as the upvector reference.
	\return The X3DObject object acting as the upvector reference.
	*/
	X3DObject	GetUpVectorReference() const;

	/*! Sets the X3DObject object acting as the upvector reference.
	\param in_UpVectorReference X3DObject that will act as the upvector reference.
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus		PutUpVectorReference
	(
		const X3DObject& in_UpVectorReference
	) const;

	/*! Returns the X3DObject object acting as the preferred axis reference.
	\return The X3DObject object acting as the preferred axis reference.
	*/
	X3DObject	GetPreferredAxisReference() const;

	/*! Sets the X3DObject object acting as the preferred axis reference.
	\param in_PreferredAxisReference X3DObject that will act as the preferred axis reference.
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus		PutPreferredAxisReference
	(
		const X3DObject& in_PreferredAxisReference
	);

	private:
	Joint * operator&() const;
	Joint * operator&();
};

};

#endif // __XSIJOINT_H__
