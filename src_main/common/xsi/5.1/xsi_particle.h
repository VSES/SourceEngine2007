//*****************************************************************************
/*!
   \file xsi_particle.h
   \brief Particle class declaration.

   © Copyright 1998-2003 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIPARTICLE_H__
#define __XSIPARTICLE_H__

#include <xsi_siobject.h>
#include <xsi_value.h>
#include <xsi_status.h>
#include <xsi_vector3.h>
#include <xsi_rotation.h>
#include <xsi_color.h>

namespace XSI {


//*****************************************************************************
/*! \class Particle xsi_particle.h
	\brief Represents one particle in a particle cloud.

	Particles are accessible via ParticleCloudPrimitive::GetParticle.

	\sa ParticleCloud, ParticleCloudPrimitive, X3DObject::AddParticleCloud

	\eg Creates the initial state for a particle simulation

	\code
		using namespace XSI;
		Application app ;

		CValueArray args(2) ;
		CValue outArg;

		args[0] = (LONG) siSphereType ;
		app.ExecuteCommand( L"CreateParticleType", args, outArg ) ;

		Property myParType = args[1] ;
		myParType.PutName( CString(L"CustomParType") ) ;

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
		myParticlePrim.AddParticles( 1000, CRef() ) ;

		// Position the particles in a 3D grid
		for ( LONG i = 0 ; i < 10 ; i++ )
		{
			for ( LONG j = 0 ; j < 10 ; j++ )
			{
				for ( LONG k = 0 ; k < 10 ; k++ )
				{
					LONG index = i*100 + j*10 + k ;
					Particle oParticle = myParticlePrim.GetParticle(index) ;
					oParticle.PutPosition( MATH::CVector3(-5 + i, -3 +j, k)) ;

					//Sets different ages so that the particles
					//will die during the playback
					oParticle.PutAge( 1 + j * 10 ) ;
					oParticle.PutAgeLimit( 100 ) ;
				}
			}
		}

		// Call the CreateParticleCloud command to create a new
		// cloud which uses our "static" cloud as the initial state

		// This new cloud will include a particle operator so
		// it will perform a simulation, allowing us to see the
		// age change for the particles.
		args[0] = myParticleCloud ; // Where to add the particle cloud
		args[1] = CValue() ; // No emitter necessary, particles already exist
		app.ExecuteCommand( L"CreateParticleCloud", args, outArg ) ;

		// Sets some attributes on the new cloud's particle type
		CValueArray & NewParticleClouds = outArg ;
		ParticleCloud myNewCloud = NewParticleClouds[0] ;
		ParticleCloudPrimitive myNewCloudPrim = myNewCloud.GetActivePrimitive() ;
		Property myNewCloudParType = myNewCloudPrim.GetParticleTypes().GetItem(0) ;

		myNewCloudParType.PutParameterValue( L"Size", 0.3 ) ;
		myNewCloudParType.PutParameterValue( L"Red", 0.99 ) ;
		myNewCloudParType.PutParameterValue( L"Green", 0.00 ) ;
		myNewCloudParType.PutParameterValue( L"Blue", 0.10 ) ;
		myNewCloudParType.PutParameterValue( L"Alpha", 0.50 ) ;

		// Hide the original cloud (we could also delete it)
		args.Resize(1) ;
		args[0] = myParticleCloud ;
		app.ExecuteCommand( L"ToggleVisibility", args, outArg ) ;

		// Start playing to show the particles disappearing
		args.Resize(0) ;
		app.ExecuteCommand( L"PlayForwardsFromStart", args, outArg ) ;
  	\endcode

*/
//*****************************************************************************

class SICPPSDKDECL Particle : public SIObject
{
public:
	/*! Default constructor. */
	Particle();

	/*! Default destructor. */
	~Particle();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	Particle(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	Particle(const Particle& in_obj);

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
	\return The new Particle object.
	*/
	Particle& operator=(const Particle& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new Particle object.
	*/
	Particle& operator=(const CRef& in_ref);

	/*! Returns the index of the particle in the particle collection. A particle's index can change
	throughout a simulation when particles are added or removed from the cloud.
	*/
	LONG GetIndex() const;

	/*!	Returns the particle's ID. A particle ID is a number that uniquely identifies the particle
	within a cloud. It is invariant throughout the simulation.
	*/
	LONG GetID() const;

	/*! Returns the type ID for the particle.  A cloud can contain particles from multiple particle types.
	\sa ParticleCloudPrimitive::GetParticleTypes
	*/
	LONG GetTypeID() const;

	/*! Changes the type ID of the particle. */
	CStatus PutTypeID(LONG in_TypeID);

	/*! Returns the particle position */
	MATH::CVector3 GetPosition()  const;

	/*! Sets the particle position */
	CStatus PutPosition(const MATH::CVector3& in_vect);

	/*! Returns the particle velocity */
	MATH::CVector3 GetVelocity()  const;

	/*! Sets the particle velocity */
	CStatus PutVelocity(const MATH::CVector3& in_vect);

	/*! Returns the particle angular velocity */
	MATH::CRotation GetAngularVelocity()  const;

	/*! Sets the particle angular velocity */
	CStatus PutAngularVelocity(const MATH::CRotation& in_vect);

	/*! Returns the particle rotation */
	MATH::CVector3 GetRotation()  const;

	/*! Sets the particle rotation */
	CStatus PutRotation(const MATH::CVector3& in_vect);

	/*! Returns the distance that the particle has travelled since birth. */
	double GetPathLength() const ;

	/*! Gets the particle's age (frames since the particles birth).  This can be a useful
	function in a custom operator for detecting if a particle is freshly born to run special
	initialization code. */
	LONG GetAge() const ;

	/*! Sets the particle's age (in frames)*/
	CStatus PutAge( LONG in_age ) ;

	/*! Returns the particle life span (in frames) */
	LONG GetAgeLimit() const;

	/*! Sets the particle life span */
	CStatus PutAgeLimit(LONG in_agelimit);

	/*! Returns the random number generator seed for the particle */
	LONG GetSeed() const;

	/*! Sets the random number generator seed of the particle */
	CStatus PutSeed(LONG in_seed);

	/*! Returns the particle's color */
	CColor GetColor( ) const ;

	/*! Sets the particle's color */
	CStatus PutColor( const CColor & in_vColor ) ;

	/*! Returns the parametric surface coordinates where the particle was emitted. As these are
	parametric coordinates, they make more sense when the emitters are NURBS curves or surfaces.
	*/
	MATH::CVector3 GetUVW()  const;

	/*! Sets the parametric surface coordinates on the emitter where the particle is emitted.  As
	these are parametric coordinates, they make more sense when the emitters are NURBS curves or surfaces.
	*/
	CStatus PutUVW(const MATH::CVector3& in_vect);

	/*! Returns the sprite index for the particle. The sprite index refers to a frame in the image clip
	associated with the particle's particle type property. You can set the image clip to be used by the
	particle via the particletype. The sprite index is only used when the particle type's shape type is
	set to sprite.
	\sa ParticleCloudPrimitive::GetParticleTypes, Particle::GetTypeID, Particle::GetSpriteAngle
	*/
	LONG GetSpriteIndex() const;

	/*! Sets the sprite index of the particle.
	*/
	CStatus PutSpriteIndex(LONG in_index);

	/*! Returns the particle sprite angle. The sprite angle is the amount of rotation of the particle in
	the camera plane. The rotation is expressed in degrees and increases counter-clockwise.
	*/
	double GetSpriteAngle() const;

	/*! Sets the particle sprite angle.
	*/
	CStatus PutSpriteAngle(double in_angle);

	/*! Returns the particle's size */
	double GetSize() const ;

	/*! Sets the particle's size */
	CStatus PutSize( double in_size ) ;

	/*! Returns the particle's mass */
	double GetMass() const;

	/*! Sets the particle's size */
	CStatus PutMass(double in_mass);

	/*! Returns the particle's density */
	double GetDensity() const;

	/*! Returns the pressure on the particle */
	double GetPressure() const;

	/*! Returns an array of references to the user-defined particle attributes.
	\return Array of references to ParticleAttribute objects.
	\sa ParticleAttribute, ParticleType::AddAttribute
	\since 4.0
	*/
	CRefArray GetAttributes() const;

	private:
	Particle * operator&() const;
	Particle * operator&();
};

};
#endif // __XSIPARTICLE_H__
