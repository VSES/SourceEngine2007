//*****************************************************************************
/*!
   \file xsi_chaineffector.h
   \brief ChainEffector class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSICHAINEFFECTOR_H__
#define __XSICHAINEFFECTOR_H__

#include <xsi_chainelement.h>

namespace XSI {

class Joint;

//*****************************************************************************
/*! \class ChainEffector xsi_chaineffector.h
	\brief The ChainEffector object is part of a skeleton chain.

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

		ChainEffector myChainEffector(myChainBone2.GetEffector());

		app.LogMessage(CString(L"The chain bone name is : ") + myChainEffector.GetFullName());
	\endcode

 */
//*****************************************************************************

class SICPPSDKDECL ChainEffector : public ChainElement
{
public:
	/*! Default constructor. */
	ChainEffector();

	/*! Default destructor. */
	~ChainEffector();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	ChainEffector(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	ChainEffector(const ChainEffector& in_obj);

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
	\return The new ChainEffector object.
	*/
	ChainEffector& operator=(const ChainEffector& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new ChainEffector object.
	*/
	ChainEffector& operator=(const CRef& in_ref);

	/*! Returns the joint property for this bone.
	\return The Joint of this bone.
	*/
	Joint GetJoint() const;

	private:
	ChainEffector * operator&() const;
	ChainEffector * operator&();
};

};

#endif // __XSICHAINEFFECTOR_H__
