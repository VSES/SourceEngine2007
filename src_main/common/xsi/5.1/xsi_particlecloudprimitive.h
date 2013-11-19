//*****************************************************************************
/*!
   \file xsi_particlecloudprimitive.h
   \brief ParticleCloudPrimitive class declaration.

   © Copyright 1998-2003 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIPARTICLECLOUDPRIMITIVE_H__
#define __XSIPARTICLECLOUDPRIMITIVE_H__

#include <xsi_primitive.h>
#include <xsi_value.h>
#include <xsi_status.h>
#include <xsi_longarray.h>
#include <xsi_particle.h>
#include <xsi_doublearray.h>

namespace XSI {

//*****************************************************************************
/*! \class ParticleCloudPrimitive xsi_particlecloudprimitive.h
	\brief This class provides access to the collection of particles that make up a ParticleCloud.
	\sa Particle, ParticleCloudPrimitive

	\eg
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
		// so the simulation is completely the responsibility of the plug-in
		ParticleCloud myParticleCloud ;
		app.GetActiveSceneRoot().AddParticleCloud(
									particleTypeArray,
									L"CustomSimulation",
									myParticleCloud ) ;

		// Particles are accessed via the Primitive
		ParticleCloudPrimitive myParticlePrim = myParticleCloud.GetActivePrimitive() ;

		myParticlePrim.AddParticles( 31, myParType ) ;

		// Position the particles based on the quadradic formula f(x)=.5x^2 - 6
		for ( LONG i = 0 ; i < myParticlePrim.GetCount() ; i++ )
		{
			Particle particle = myParticlePrim.GetParticle(i) ;

			double xval = (i/3.0)-5 ;
			particle.PutPosition( MATH::CVector3( xval, (0.5 * xval * xval)-6, 0 ) ) ;
		}
  	\endcode
*/
//*****************************************************************************

class SICPPSDKDECL ParticleCloudPrimitive : public Primitive
{
public:
	/*! Default constructor. */
	ParticleCloudPrimitive();

	/*! Default destructor. */
	~ParticleCloudPrimitive();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	ParticleCloudPrimitive(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	ParticleCloudPrimitive(const ParticleCloudPrimitive& in_obj);

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
	\return The new ParticleCloudPrimitive object.
	*/
	ParticleCloudPrimitive& operator=(const ParticleCloudPrimitive& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new ParticleCloudPrimitive object.
	*/
	ParticleCloudPrimitive& operator=(const CRef& in_ref);

	/*! Adds particles to a cloud.  This function can only be called from custom operators
	or on clouds with no Operator.  For example, this function will fail on a cloud created
	with the \xl CreateParticleCloud command unless the cloud is frozen.

	\param in_NbParticles Number of new particles to add.
	\param in_ParType Reference to an existing particle type object.
	\note The particle type should be one of the particle types initially
		specified in the call to X3DObject::CreateParticleCloud.  If no particle type
		is specified then the first particle type associated with the cloud is used.

	\since 4.0
	*/
	CStatus AddParticles( LONG in_NbParticles, const CRef& in_ParType ) ;

	/* Obsolete. Maintained for backward compatibility only. */
	CStatus AddParticles( LONG in_NbParticles, CRef& in_ParType ) ;

	/*! Removes particles by index.  This function can only be called from custom operators
	or on clouds with no Operator.
	\param in_IndexArray Array with the indices of the particles to remove.
	*/
	CStatus RemoveParticles(const CLongArray & in_IndexArray ) ;

	/*! Removes particles by their IDs.  This function can only be called from custom operators
	or on clouds with no Operator.
	\param in_IDArray Array with the particle IDs of the particles to remove.
	\sa Particle::GetID
	*/
	CStatus RemoveParticlesWithIDs(const CLongArray & in_IDArray ) ;


	/*! Returns the number of particles in the cloud.
	*/
	LONG GetCount() const ;

	/*! Returns the Particle object at the specified index.
	\param in_index Index of the particle.  Particles always have an index between 0 and Particle::GetCount - 1.
	*/
	Particle GetParticle( LONG in_index ) const ;

	/*! Returns an array of the particle types associated with the particle cloud. A particle
	cloud might have particles with different types on the same cloud.  An associated particle
	type is created automatically when the \xl CreateParticleCloud command is called.  A particle
	type can also be created by calling the \xl CreateParticleType command and associated with a
	cloud when X3DObject::AddParticleCloud is called.
	\sa ParticleType
	*/
	CRefArray GetParticleTypes() const ;

	/*! Searches for a particle by its ID. Because the index of a particle may change during a
	simulation but its ID will not, it is sometimes convenient to lookup a particle by its ID.
	However finding particles by this mechanism is moderately less efficient than using Particle::GetParticle.
	\param in_ID
	\return The Particle object.
	\return Invalid object if the requested ID was not found.
	\sa Particle::GetID, Particle::GetIndex
	*/
	Particle FindParticle( LONG in_ID ) const ;

	/*! Sets the positions for all the particles in the cloud with a single API call. This approach
	is more efficient than calling Particle::PutPosition on every single particle.
	\param in_values Array with new X,Y,Z values for each particle, ordered as
		\code X(0), Y(0), Z(0)... X(n-1), Y(n-1), Z(n-1) \endcode
	\sa Particle::PutPosition
	*/
	CStatus PutPositionArray( const CDoubleArray& in_values ) ;

	/*! Returns the positions for all the particles in the cloud with a single API call. This approach
	is more efficient than calling Particle::GetPosition on every single particle.
	\retval out_values Returned array contains the X,Y,Z values for each particle, ordered as
		\code X(0), Y(0), Z(0)... X(n-1), Y(n-1), Z(n-1) \endcode
	\sa Particle::GetPosition
	*/
	CStatus GetPositionArray( CDoubleArray& out_values ) ;

	/*! Sets the velocities for all the particles in the cloud with a single API call. This approach
	is more efficient than calling Particle::PutVelocity on every single particle.
	\param in_values Array with X,Y,Z values for each particle, ordered as
		\code X(0), Y(0), Z(0)... X(n-1), Y(n-1), Z(n-1) \endcode
	\sa Particle::PutVelocity
	*/
	CStatus PutVelocityArray( const CDoubleArray& in_values ) ;

	/*! Returns the velocities for all the particles in the cloud with a single API call. This approach
	is more efficient than calling Particle::GetVelocity on every single particle.
	\retval out_values Returned array will contain the X,Y,Z values for each particle, ordered as
		\code X(0), Y(0), Z(0)... X(n-1), Y(n-1), Z(n-1) \endcode
	\sa Particle::GetVelocity
	*/
	CStatus GetVelocityArray( CDoubleArray& out_values ) ;

	/*! Sets the rotation for all the particles in the cloud with a single API call. This approach is
	more efficient than calling Particle::PutRotation on every single particle.
	\param in_values Array with new X,Y,Z values (in radians) for each particle, ordered as
		\code X(0), Y(0), Z(0)... X(n-1), Y(n-1), Z(n-1) \endcode
	\sa Particle::PutRotation
	*/
	CStatus PutRotationArray( const CDoubleArray& in_values ) ;

	/*! Returns the rotation vectors for all the particles in the cloud with a single API call.
	 This approach is more efficient than calling Particle::GetRotation on every single particle.
	\retval out_values Returned array will contain the X,Y,Z values (in radians) for each particle, ordered as
		\code X(0), Y(0), Z(0)... X(n-1), Y(n-1), Z(n-1) \endcode
	\sa Particle::GetRotation
	*/
	CStatus GetRotationArray( CDoubleArray& out_values ) ;

	/*! Sets the angular velocities for all the particles in the cloud with a single API call.This
	approach is more efficient than calling Particle::PutAngularVelocity on every single particle.
	\param in_values Array with new X,Y,Z Euler angle values (in radians) for each particle, ordered as
	\code X(0), Y(0), Z(0)... X(n-1), Y(n-1), Z(n-1) \endcode
	\sa Particle::PutAngularVelocity
	*/
	CStatus PutAngularVelocityArray( const CDoubleArray& in_values ) ;

	/*! Returns the angular velocities for all the particles in the cloud with a single API call. This
	approach is more efficient than calling Particle::GetAngularVelocity on every single particle.
	\retval out_values Returned array will contain the X,Y,Z Euler angles (in radians) for each
		particle, ordered as \code X(0), Y(0), Z(0)... X(n-1), Y(n-1), Z(n-1) \endcode
	\sa Particle::GetAngularVelocity
	*/
	CStatus GetAngularVelocityArray( CDoubleArray& out_values ) ;

	/*! Sets the emission position in UVW coordinates for all the particles in the cloud with a single API
	call. This approach is more efficient than calling Particle::PutUVW on every single particle.
	\param in_values Array with new U,V,W position values for each particle, ordered as
		\code U(0), V(0), W(0)... U(n-1), V(n-1), W(n-1) \endcode
	\sa Particle::PutUVW
	*/
	CStatus PutUVWArray( const CDoubleArray& in_values ) ;

	/*! Returns the emission position in UVW coordinates for all the particles in the cloud with a single
	API call. This approach is more efficient than calling Particle::GetUVW on every single particle.
	\retval out_values Returned array will contain the U,V,W values for each particle, ordered as
		\code U(0), V(0), W(0)... U(n-1), V(n-1), W(n-1) \endcode
	\sa Particle::GetUVW
	*/
	CStatus GetUVWArray( CDoubleArray& out_values ) ;

	/*! Sets the color of all particles in the cloud with a single API call. This approach is more efficient
	than calling Particle::PutColor on every single particle.
	\param in_values Array with R,G,B,A color values for each particle, ordered as
		\code R(0), G(0), B(0), A(0)... R(n-1), G(n-1), B(n-1), A(n-1) \endcode
	\sa Particle::PutColor
	*/
	CStatus PutColorArray( const CDoubleArray& in_values ) ;

	/*! Returns the colors of all the particles in the cloud with a single API call. This approach is more
	efficient than calling Particle::GetColor on every single particle.
	\retval out_values Returned array will contain the R,G,B,A values for each particle, ordered as
		\code R(0), G(0), B(0), A(0)... R(n-1), G(n-1), B(n-1), A(n-1) \endcode
	\sa Particle::GetColor
	*/
	CStatus GetColorArray( CDoubleArray& out_values ) ;

	/*! Sets the size of each particle in the cloud with a single API call. This approach is more efficient
	than calling Particle::PutSize on every single particle.
	\param in_values Array with new size values for each particle.
	\sa Particle::PutSize
	*/
	CStatus PutSizeArray( const CDoubleArray& in_values ) ;

	/*! Returns the size of each particle in the cloud with a single API call. This approach is more efficient
	than calling Particle::GetSize on every single particle.
	\retval out_values Returned array will contain the size of each particle
	\sa Particle::GetSize
	*/
	CStatus GetSizeArray( CDoubleArray& out_values ) ;

	/*! Sets the sprite angle of each particle in the cloud with a single API call. This approach is more
	efficient than calling Particle::PutSpriteAngle on every single particle.
	\param in_values Array with new sprite angle values for each particle.
	\sa Particle::PutSpriteAngle
	*/
	CStatus PutSpriteAngleArray( const CDoubleArray& in_values ) ;

	/*! Returns the sprite angle for all the particles in the cloud with a single API call. This approach is more
	efficient than calling Particle::GetSpriteAngle on every single particle.
	\retval out_values Returned array will contain the sprite angle of each particle
	\sa Particle::GetSpriteAngle
	*/
	CStatus GetSpriteAngleArray( CDoubleArray& out_values ) ;

	/*! Sets the mass of each particle in the cloud with a single API call. This approach is more efficient than
	calling Particle::PutMass on every single particle.
	\param in_values Array with new mass values for each particle.
	\sa Particle::PutMass
	*/
	CStatus PutMassArray( const CDoubleArray& in_values ) ;

	/*! Returns the mass of each particle in the cloud with a single API call. This approach is more efficient
	than calling Particle::GetMass on every single particle.
	\retval out_values Returned array will contain the mass of each particle
	\sa Particle::GetMass
	*/
	CStatus GetMassArray( CDoubleArray& out_values ) ;

	/*! Returns the density of each particle in the cloud with a single API call. This approach is more efficient
	than calling Particle::GetDensity on every single particle.
	\retval out_values Returned array will contain the density of each particle
	\sa Particle::GetDensity
	*/
	CStatus GetDensityArray( CDoubleArray& out_values ) ;

	/*! Returns the pressure on each particle in the cloud with a single API call. This approach is more efficient
	than calling Particle::GetPressure on every single particle.
	\retval out_values Returned array will contain the pressure of each particle
	\sa Particle::GetPressure
	*/
	CStatus GetPressureArray( CDoubleArray& out_values ) ;

	/*! Returns the length of the path travelled by each particles in the cloud with a single API call. This approach
	is more efficient than calling Particle::GetPathLength on every single particle.
	\retval out_values Returned array will contain the path length of each particle
	\sa Particle::GetPathLength
	*/
	CStatus GetPathLengthArray( CDoubleArray& out_values ) ;

	/*! Sets the age of each particle in the cloud with a single API call. This approach is more efficient than calling
	Particle::PutAge on every single particle.
	\param in_values Array with new age values for each particle.
	\sa Particle::PutAge
	*/
	CStatus PutAgeArray( const CLongArray& in_values ) ;

	/*! Returns the age of each particle in the cloud with a single API call. This approach is more efficient than
	calling Particle::GetAge on every single particle.
	\retval out_values Returned array will contain the age of each particle
	\sa Particle::GetAge
	*/
	CStatus GetAgeArray( CLongArray& out_values ) ;

	/*! Sets the random number generation seed of each particle in the cloud with a single API call. This approach is
	more efficient than calling Particle::PutSeed on every single particle.
	\param in_values Array with new seed values for each particle.
	\sa Particle::PutSeed
	*/
	CStatus PutSeedArray( const CLongArray& in_values ) ;

	/*! Returns the random number generation seed of each particle in the cloud with a single API call.  This approach
	is more efficient than calling Particle::GetSeed on every single particle.
	\retval out_values Returned array will contain the seed of each particle
	\sa Particle::GetSeed
	*/
	CStatus GetSeedArray( CLongArray& out_values ) ;

	/*! Sets the life span of each particle in the cloud with a single API call. This approach is more efficient than
	calling Particle::PutAgeLimit on every single particle.
	\param in_values Array with new age limit values for each particle.
	\sa Particle::PutAgeLimit
	*/
	CStatus PutAgeLimitArray( const CLongArray& in_values ) ;

	/*! Returns the life span of each particle in the cloud with a single API call. This approach is more efficient
	than calling Particle::GetAgeLimit on every single particle.
	\retval out_values Returned array will contain the age limit of each particle
	\sa Particle::GetAgeLimit
	*/
	CStatus GetAgeLimitArray( CLongArray& out_values ) ;

	/*! Sets the sprite index of each particle in the cloud with a single API call. This approach is more efficient
	than calling Particle::PutSpriteIndex on every single particle.
	\param in_values Array with new sprite index values for each particle.
	\sa Particle::PutSpriteIndex
	*/
	CStatus PutSpriteIndexArray( const CLongArray& in_values ) ;

	/*! Returns the sprite index of each particle in the cloud with a single API call. This approach is more efficient
	than calling Particle::GetSpriteIndex on every single particle.
	\retval out_values Returned array will contain the sprite index of each particle
	\sa Particle::GetSpriteIndex
	*/
	CStatus GetSpriteIndexArray( CLongArray& out_values ) ;

	/*! Sets the particle type ID of each particle in the cloud with a single API call. This approach is more efficient
	than calling Particle::PutTypeID on every single particle.
	\param in_values Array with new particle type IDs for each particle.
	\sa Particle::PutTypeID
	*/
	CStatus PutTypeIDArray( const CLongArray& in_values ) ;

	/*! Returns the particle type ID of each particle in the cloud with a single API call. This approach is more efficient
	than calling Particle::GetTypeID on every single particle.
	\retval out_values Returned array will contain the type ID of each particle
	\sa Particle::GetTypeID
	*/
	CStatus GetTypeIDArray( CLongArray& out_values ) ;

	/*! Returns the ID of each particle in the cloud with a single API call. This approach is more efficient than calling
	Particle::GetID on every single particle.
	\param in_values Returned array will contain the ID of each particle
	\sa Particle::GetID
	*/
	CStatus GetIDArray( CLongArray& in_values ) ;

	private:
	ParticleCloudPrimitive * operator&() const;
	ParticleCloudPrimitive * operator&();
};

};
#endif // __XSIPARTICLECLOUDPRIMITIVE_H__
