//*****************************************************************************
/*!
   \file xsi_chainroot.h
   \brief ChainRoot class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSICHAINROOT_H__
#define __XSICHAINROOT_H__

#include <xsi_chainelement.h>
#include <xsi_status.h>
#include <xsi_vector3.h>

namespace XSI {

class ChainBone;

//*****************************************************************************
/*! \class ChainRoot xsi_chainroot.h
	\brief The ChainRoot object represents the root of a skeleton chain.

	\sa ChainElement,ChainBone

   	\eg
	\code
		using namespace XSI;
		Application app;
		Model root = app.GetActiveSceneRoot();

		ChainRoot myChainRoot;
		root.Add3DChain(	MATH::CVector3(0,0,0),
							MATH::CVector3(1,0,0),
							MATH::CVector3(0,0,1),
							L"",
							myChainRoot);

		ChainBone myChainBone1;
		myChainRoot.AddBone(MATH::CVector3(3,3,3),siChainBonePin,L"",myChainBone1);

		ChainBone myChainBone2;
		myChainRoot.AddBone(MATH::CVector3(6,0,0),siChainBoneBallJoint,L"",myChainBone2);

		app.LogMessage(CString(L"The chain root name is : ") + myChainRoot.GetFullName());
	\endcode
 */
//*****************************************************************************

class SICPPSDKDECL ChainRoot : public ChainElement
{
public:
	/*! Default constructor. */
	ChainRoot();

	/*! Default destructor. */
	~ChainRoot();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	ChainRoot(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	ChainRoot(const ChainRoot& in_obj);

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
	\return The new ChainRoot object.
	*/
	ChainRoot& operator=(const ChainRoot& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new ChainRoot object.
	*/
	ChainRoot& operator=(const CRef& in_ref);

	/*! Returns an array containing references to ChainBone contained in this skeleton chain.
	\return Array of references to ChainBone objects contained in this skeleton chain.
	*/
	CRefArray GetBones() const;

	/*! Adds a ChainBone element at the end of this chain.
	\return Array of references to the ChainBone objects contained in this skeleton chain.
	\param in_newEffPos The new effector position.
	\param in_BoneType The type of bone to add.
	\param in_name		The name of the new chain.
	\param io_bone		The new bone.
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus AddBone
	(
		const MATH::CVector3&	in_newEffPos,
		siChainBoneType			in_BoneType,
		const CString&			in_name,
		ChainBone&				io_bone
	);

	private:
	ChainRoot * operator&() const;
	ChainRoot * operator&();
};

};

#endif // __XSICHAINROOT_H__
