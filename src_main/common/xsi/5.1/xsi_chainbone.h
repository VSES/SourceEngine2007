//*****************************************************************************
/*!
   \file xsi_chainbone.h
   \brief ChainBone class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSICHAINBONE_H__
#define __XSICHAINBONE_H__

#include <xsi_chainelement.h>

namespace XSI {

class Joint;

//*****************************************************************************
/*! \class ChainBone xsi_chainbone.h
	\brief The ChainBone object is part of a skeleton chain.

	\sa ChainRoot,ChainElement

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

		app.LogMessage(CString(L"The chain bone name is : ") + myChainBone2.GetFullName());
	\endcode
 */
//*****************************************************************************

class SICPPSDKDECL ChainBone : public ChainElement
{
public:
	/*! Default constructor. */
	ChainBone();

	/*! Default destructor. */
	~ChainBone();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	ChainBone(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	ChainBone(const ChainBone& in_obj);

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
	\return The new ChainBone object.
	*/
	ChainBone& operator=(const ChainBone& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new ChainBone object.
	*/
	ChainBone& operator=(const CRef& in_ref);

	/*! Returns the bone index in the skeleton chain.
	\return The bone index.
	*/
	LONG GetIndex() const;

	/*! Returns the joint property for this bone.
	\return The Joint of this bone.
	*/
	Joint GetJoint() const;

	private:
	ChainBone * operator&() const;
	ChainBone * operator&();
};

};

#endif // __XSICHAINBONE_H__
