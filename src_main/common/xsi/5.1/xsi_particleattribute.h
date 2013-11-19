//*****************************************************************************
/*!
   \file xsi_particleattribute.h
   \brief ParticleAttribute class declaration.

   © Copyright 1998-2003 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIPARTICLEATTRIBUTE_H__
#define __XSIPARTICLEATTRIBUTE_H__

#include <xsi_siobject.h>
#include <xsi_value.h>
#include <xsi_vector3.h>
#include <xsi_vector4.h>

namespace XSI {

//*****************************************************************************
/*! \class ParticleAttribute xsi_particleattribute.h
	\brief Represents a user-defined Particle attribute, which is a custom parameter
	for particles originating from a ParticleType.

	The user-defined particle attribute can be used to customize a particle simulation.

	\sa ParticleCloud, ParticleCloudPrimitive, X3DObject::AddParticleCloud, ParticleType
	\since 4.0

	\eg Creates the initial state with a user defined particle attribute for a particle simulation

	\code
		using namespace XSI;
		Application app ;

		CValueArray args(2) ;
		CValue outArg;

		args[0] = (LONG) siSphereType ;
		app.ExecuteCommand( L"CreateParticleType", args, outArg ) ;

		ParticleType myParType = args[1] ;
		myParType.PutName( CString(L"CustomParType") ) ;

		myParType.AddAttribute(L"myVector3Attr", siPAVector3);

		CRefArray particleTypeArray ;
		particleTypeArray.Add( myParType ) ;

		// Create a particle cloud.  This has no emittor, and no particle operator
		// so the cloud state is completely the responsibility of the plug-in
		// and will not change even if the current frame is changed
		ParticleCloud myParticleCloud ;
		app.GetActiveSceneRoot().AddParticleCloud(
									particleTypeArray,
									L"MyCloud",
									myParticleCloud ) ;

		ParticleCloudPrimitive myParticlePrim = myParticleCloud.GetActivePrimitive() ;

		// Our "CustomParType" will automatically be used because
		// we specified it in the call to AddParticleCloud
		myParticlePrim.AddParticles( 10, CRef() ) ;

		// Setting the user defined particle attribute
		for ( LONG i = 0 ; i < 10 ; i++ )
		{
			Particle oParticle = myParticlePrim.GetParticle(i) ;
			CRefArray attributes = oParticle.GetAttributes() ;
			ParticleAttribute myPA(attributes.GetItem(L"myVector3Attr"));
			myPA.PutValue( MATH::CVector3(1.0,1.0, 1.0)) ;
		}
	\endcode
 */
//*****************************************************************************

class SICPPSDKDECL ParticleAttribute : public SIObject
{
public:
	/*! Default constructor. */
	ParticleAttribute();

	/*! Default destructor. */
	~ParticleAttribute();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	ParticleAttribute(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	ParticleAttribute(const ParticleAttribute& in_obj);

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
	\return The new ParticleAttribute object.
	*/
	ParticleAttribute& operator=(const ParticleAttribute& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new ParticleAttribute object.
	*/
	ParticleAttribute& operator=(const CRef& in_ref);


	/*! Returns the value of this attribute. For all numeric types you should be using the CValue version
	of ParticleAttribute::GetValue. If the type of the argument does not match the function will return an
	invalid argument method.
	\retval out_val container for the retrieved value.

	\return CStatus::OK		success
	\return CStatus::Fail	failure
	\return CStatus::InvalidArgument	attempts to use the wrong overloaded version of this function.
	*/
	CStatus GetValue(CValue& out_val) const;
	CStatus GetValue(MATH::CVector3& out_val) const;
	CStatus GetValue(MATH::CVector4& out_val) const;

	/*! Sets the value of this attribute. For all numeric types you should be using the CValue version of
	ParticleAttribute::PutValue. If the type of the argument does not match the method will return an invalid
	argument method.

	\param in_val new value to be set

	\return CStatus::OK		success
	\return CStatus::Fail	failure
	\return CStatus::InvalidArgument	attempts to use the wrong overloaded version of this function.
	*/
	CStatus PutValue(const CValue& in_val) const;
	CStatus PutValue(const MATH::CVector3& in_val) const;
	CStatus PutValue(const MATH::CVector4& in_val) const;

	/*! Returns the particle attribute type.
	\return The attribute type
	*/
	XSI::siParticleAttributeType GetAttributeType()const;

	private:
	ParticleAttribute * operator&() const;
	ParticleAttribute * operator&();
};

};

#endif // __XSIPARTICLEATTRIBUTE_H__
