//*****************************************************************************
/*!
   \file xsi_chainelement.h
   \brief ChainElement class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSICHAINELEMENT_H__
#define __XSICHAINELEMENT_H__

#include <xsi_x3dobject.h>

namespace XSI {

class ChainEffector;
class ChainRoot;

//*****************************************************************************
/*! \class ChainElement xsi_chaineffector.h
	\brief The ChainElement object represents a component of a skeleton chain.

	All components of a skeleton chain inherit the functions of this class.

	\sa ChainRoot,ChainBone

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

		ChainElement myRoot(myChainBone1.GetRoot());

		app.LogMessage(CString(L"The chain element name is : ") + myRoot.GetFullName());
	\endcode

 */
//*****************************************************************************

class SICPPSDKDECL ChainElement : public X3DObject
{
public:
	/*! Default constructor. */
	ChainElement();

	/*! Default destructor. */
	~ChainElement();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	ChainElement(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	ChainElement(const ChainElement& in_obj);

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
	\return The new ChainElement object.
	*/
	ChainElement& operator=(const ChainElement& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new ChainElement object.
	*/
	ChainElement& operator=(const CRef& in_ref);

	/*! Returns the root object of the skeleton chain.
	\return The root object of the skeleton chain.
	*/
	ChainRoot		GetRoot() const;

	/*! Returns the effector object of the skeleton chain.
	\return The effector of this skeleton chain.
	*/
	ChainEffector	GetEffector() const;

	private:
	ChainElement * operator&() const;
	ChainElement * operator&();
};

};

#endif // __XSICHAINELEMENT_H__
