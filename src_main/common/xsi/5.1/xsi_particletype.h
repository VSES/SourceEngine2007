//*****************************************************************************
/*!
   \file xsi_particletype.h
   \brief ParticleType class declaration.

   © Copyright 1998-2003 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIPARTICLETYPE_H__
#define __XSIPARTICLETYPE_H__

#include <xsi_property.h>
#include <xsi_value.h>
#include <xsi_status.h>
#include <xsi_string.h>
#include <xsi_decl.h>


namespace XSI {


//*****************************************************************************
/*! \class ParticleType xsi_particletype.h
	\brief Represents a particle type. A particle type serves as a definition of the
	particles on the initial state of a particle simulation.

	A particle cloud contains one or more particle type(s). All particle types in a scene
	can be found under Scene.Particles container.

	\sa ParticleCloud, ParticleCloudPrimitive, X3DObject::AddParticleCloud, ParticleAttribute

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

class SICPPSDKDECL ParticleType : public Property
{
public:
	/*! Default constructor. */
	ParticleType();

	/*! Default destructor. */
	~ParticleType();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	ParticleType(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	ParticleType(const ParticleType& in_obj);

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
	\return The new ParticleType object.
	*/
	ParticleType& operator=(const ParticleType& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new ParticleType object.
	*/
	ParticleType& operator=(const CRef& in_ref);

	/*!	Returns the scene index for this particle type, which corresponds
	to the index of this particle type under the Scene.Particles container.
	\return The ParticleType ID.
	*/
	LONG GetID() const;

	/*! Adds a new user-defined attribute to this particle type. You must
	specify a type (ParticleAttribute.GetAttributeType) and a unique name for
	the new attribute.  This function fails if another user-defined attribute
	with the same name exists.

	\param	in_name  name of the defined attribute
	\param	in_type  type of the defined attribute

	\return CStatus::OK success
	\return CStatus::Fail other failure
	\return CStatus::InvalidArgument if in_type is siPAUndefined
	*/
	CStatus AddAttribute
	(
		const CString&					in_name,
		XSI::siParticleAttributeType	in_type
	)const;

	/*! Removes a user-defined attribute from this particle type by specifying the name.
	\param	in_name  name of the attribute to remove.
	\return CStatus::OK success
	\return CStatus::Fail other failure
	\return CStatus::False the attribute of name in_name was not found on the ParticleType.
	*/
	CStatus RemoveAttribute(const CString& in_name )const;

	/*! Returns an array containing the names of all user-defined attributes
	for this particle type.
	\return Array of custom attribute names.
	*/
	CStringArray GetAttributeNames()const;

	/*! Returns the data type of the specified attribute.  If no attribute
	matches the specified name, it returns siPAUndefined.
	\param	in_name  name of the attribute to remove.
	\return Type of a given attribute.
	\return siPAUndefined if no match is found
	*/
	XSI::siParticleAttributeType GetAttributeType( const CString& in_name )const;

	private:
	ParticleType * operator&() const;
	ParticleType * operator&();
};

};
#endif // __XSIPARTICLETYPE_H__
