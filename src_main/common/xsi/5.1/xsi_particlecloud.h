//*****************************************************************************
/*!
   \file xsi_particlecloud.h
   \brief Particle Cloud class declaration.

   © Copyright 1998-2003 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIPARTICLECLOUD_H__
#define __XSIPARTICLECLOUD_H__

#include <xsi_x3dobject.h>
#include <xsi_value.h>
#include <xsi_status.h>

namespace XSI {

//*****************************************************************************
/*! \class ParticleCloud xsi_particlecloud.h
	\brief This class represents a Particle Cloud in an %XSI scene.

	All particle-specific functionality is exposed via the ParticleCloudPrimitive, which is accessible by
	calling X3DObject::GetActivePrimitive.

	Normally a ParticleCloud is created with the \xl CreateParticleCloud command.  Alternatively,
	X3DObject::AddParticleCloud will create a basic particle cloud suitable for custom simulations
	driven by a custom particle operator.

	\sa Particle, ParticleCloud

	\eg
	\code
		using namespace XSI;
		Application app ;

		CValueArray args(2) ;
		CValue outArg;

		//Move ahead to 1 second on the timeline
		args[0] = L"PlayControl.Current" ;
		args[1] = 30l ;
		app.ExecuteCommand( L"SetValue", args, outArg ) ;

		// Create a particle cloud using CreateParticleCloud command
		// This automatically creates a particle type and a particle emitter object

		args[0] = app.GetActiveSceneRoot().GetRef() ; //Where to add the particle cloud
		args[1] = L"grid" ;	// Type of emitter object
		app.ExecuteCommand( L"CreateParticleCloud", args, outArg ) ;

		CValueArray & NewParticleClouds = outArg ;
		ParticleCloud myCloud = NewParticleClouds[0] ;

		myCloud.PutName( CString( L"CloudDemo" ) ) ;

		// Particles are accessed via the Primitive
		ParticleCloudPrimitive myParticlePrim = myCloud.GetActivePrimitive() ;

		app.LogMessage( L"There are " +
						CValue(myParticlePrim.GetCount()).GetAsText() +
						L" particles" );

		// Get access to the particle type associated with the cloud
		CRefArray myParticleTypes = myParticlePrim.GetParticleTypes() ;
		Property myParType = myParticleTypes[0] ;
		app.LogMessage( L"Associated particle type " + myParType.GetFullName() ) ;
  	\endcode
*/
//*****************************************************************************

class SICPPSDKDECL ParticleCloud : public X3DObject
{
public:
	/*! Default constructor. */
	ParticleCloud();

	/*! Default destructor. */
	~ParticleCloud();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	ParticleCloud(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	ParticleCloud(const ParticleCloud& in_obj);

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
	\return The new ParticleCloud object.
	*/
	ParticleCloud& operator=(const ParticleCloud& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new ParticleCloud object.
	*/
	ParticleCloud& operator=(const CRef& in_ref);

	private:
	ParticleCloud * operator&() const;
	ParticleCloud * operator&();
};

};
#endif // __XSIPARTICLECLOUD_H__
