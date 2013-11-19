//*****************************************************************************
/*!
   \file xsi_envelope.h
   \brief Envelope class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIENVELOPE_H__
#define __XSIENVELOPE_H__

#include <xsi_operator.h>
#include <xsi_doublearray.h>
#include <xsi_cluster.h>
#include <xsi_clusterproperty.h>

namespace XSI {

struct CColor;

//*****************************************************************************
/*! \class Envelope xsi_envelope.h
	\brief An envelope is an object or hierarchy that is assigned as a \a skin to a set of deformers such as IK chains.
	Envelopes move and deform in response to the movement of their deformers.  In this way, for example, a character
	moves as you animate its skeleton.

	Every point in an envelope is assigned to one or more deformers.  Weights determine the relative influence of a
	deformer on a given point of the envelope.

	Although it is common to use skeletons as deformers, you can actually use any object.  The geometry of a deformer
	does not matter because the points of the envelope are assigned to its center.  Bones are the exception; in this case,
	points are deformed by the whole length of the bone).

	\sa ChainBone, X3DObject, SceneItem

	\eg For examples that demonstrate how to access and set deformer weights, see Envelope::GetWeights and
		Envelope::SetDeformerWeights
 */
//*****************************************************************************
class SICPPSDKDECL Envelope : public Operator
{
public:
	/*! Default constructor. */
	Envelope();

	/*! Default destructor. */
	~Envelope();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	Envelope(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	Envelope(const Envelope& in_obj);

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
	\return The new Envelope object.
	*/
	Envelope& operator=(const Envelope& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new Envelope object.
	*/
	Envelope& operator=(const CRef& in_ref);

	/*! Returns an array of references to the deformer objects of this envelope.
	\note You can convert a CRefArray element to an X3DObject (to use with envelope functions such as
		Envelope::GetDeformerWeights) as follows:

	\code 			myEnvelope.GetDeformerWeights( X3DObject( myCRefArray[someIndex] ) ); \endcode

	\return Array of references to deformer objects

	\sa Envelope::GetWeights, Envelope::GetDeformerWeights, Envelope::SetDeformerWeights
	*/
	CRefArray GetDeformers() const;

	/*! Returns the weights for every deformer at a given frame.
	\note The returned array contains, for every cluster element of the envelope, a list of the weights of all deformers.
	For instance, if an envelope is composed of two cluster elements and two deformers, the two first elements of the
	array are the weights for the deformers of the first cluster element.  The third and fourth values represent the entries
	for the second cluster element.

	\par Array Layout:

	\code
	 ________________________________________________
	|  Index 0  |  Index 1  ||  Index 2  |  Index 3  |
	|-----------+-----------++-----------+-----------|
	|   Cluster element 0   ||   Cluster element 1   |
	|===========+===========++===========+===========|
	| Weight of | Weight of || Weight of | Weight of |
	|deformer 0 |deformer 1 ||deformer 0 |deformer 1 |
	 ------------------------------------------------
	\endcode

	\param in_dFrame Frame at which to get the deformers
	\return An array of weights.

	\sa CClusterPropertyElementArray::GetArray, CClusterPropertyElementArray::GetValueSize,
		Envelope::GetDeformerWeights, Envelope::SetDeformerWeights

	\eg

	\code
		using namespace XSI;
		Application app;
		Model root = app.GetActiveSceneRoot();

		Null myDeformer1;
		root.AddNull( L"", myDeformer1 );
		Null myDeformer2;
		root.AddNull( L"", myDeformer2 );

		KinematicState nullGlobalKineState(myDeformer1.GetKinematics().GetGlobal());
		Parameter param(nullGlobalKineState.GetParameters().GetItem(L"posx"));
		param.PutValue( (double)4.0 );

		param = nullGlobalKineState.GetParameters().GetItem(L"posy");
		param.PutValue( (double)3.0 );

		X3DObject mySphere;
		root.AddGeometry( L"Sphere", L"MeshSurface", L"", mySphere );
		param = mySphere.GetKinematics().GetGlobal().GetParameters().GetItem(L"posz");
		param.PutValue( (double)2.0 );

		CRefArray deformers(2);
		deformers[0] = myDeformer1;
		deformers[1] = myDeformer2;

		Envelope myEnvelope;
		mySphere.ApplyEnvelope(	deformers, siUnspecified, siUnspecified, myEnvelope );

		// get the weights at the current frame
		CTime time;
		CClusterPropertyElementArray weights = myEnvelope.GetWeights( time.GetTime() );
		CDoubleArray weightArray = weights.GetArray();

		wchar_t msg[30];
		LONG weightArrayCount = weightArray.GetCount();
		LONG deformerCount = weights.GetValueSize();

		// for each cluster element in the envelope
		for ( register LONG i = 0L; i < weightArrayCount; i += deformerCount )
		{
			swprintf( msg, L"Element #%d weights:", i / deformerCount  );
			CString csMsg( msg );

			// for each deformer of the current cluster element
			for ( register LONG j = 0L; j < deformerCount; j++ )
			{
				// show weight for current deformer
				swprintf( msg, L"\n\t\tDeformer #%d = %f", j, weightArray[i + j] );
				csMsg += msg;
			}
			app.LogMessage( csMsg );
		}
	\endcode
	*/
	CClusterPropertyElementArray GetWeights( double in_dFrame ) const;

	/*! Returns the cluster elements composing this envelope at a given frame.
	\param in_dFrame Frame at which to get the cluster elements.
	\return An array of cluster elements
	*/
	CClusterElementArray GetElements( double in_dFrame ) const;

	/*! Returns the weights of a specific deformer at a given frame.
	\param in_Deformer The deformer to get
	\param in_dFrame Frame at which to get the deformer
	\return An array of weights
	\sa Envelope::GetWeights, Envelope::GetDeformers, Envelope::SetDeformerWeights
	*/
	CDoubleArray GetDeformerWeights( const X3DObject &in_Deformer, double in_dFrame ) const;

	/*! Sets the weights of the specified deformer.
	\param in_Deformer The deformer to set
	\param in_Array Array of weight values to apply

	\return CStatus::OK
	\return CStatus::Fail

	\sa Envelope::GetDeformers, Envelope::GetWeights, Envelope::SetDeformerWeights

	\eg
	\code
		using namespace XSI;
		Application app;
		Model root = app.GetActiveSceneRoot();

		Null myDeformer1;
		root.AddNull( L"", myDeformer1 );
		Null myDeformer2;
		root.AddNull( L"", myDeformer2 );

		KinematicState nullGlobalKineState(myDeformer1.GetKinematics().GetGlobal());
		Parameter param(nullGlobalKineState.GetParameters().GetItem(L"posx"));
		param.PutValue( (double)4.0 );

		param = nullGlobalKineState.GetParameters().GetItem(L"posy");
		param.PutValue( (double)3.0 );

		X3DObject mySphere;
		root.AddGeometry( L"Sphere", L"MeshSurface", L"", mySphere );
		param = mySphere.GetKinematics().GetGlobal().GetParameters().GetItem(L"posz");
		param.PutValue( (double)2.0 );

		CRefArray deformers(2);
		deformers[0] = myDeformer1;
		deformers[1] = myDeformer2;

		Envelope myEnvelope;
		mySphere.ApplyEnvelope(	deformers, siUnspecified, siUnspecified, myEnvelope );

		// get the first deformer
		X3DObject firstDeformer( myEnvelope.GetDeformers()[0] );

		// Get its weights at frame 0
		CDoubleArray weights = myEnvelope.GetDeformerWeights( firstDeformer, 0.0 );

		// Set the weights to 75 for the current frame
		LONG weightCount = weights.GetCount();
		register LONG i = 0L;
		for ( ; i < weightCount; i++ ) weights[i] = 75.0;
		myEnvelope.SetDeformerWeights( firstDeformer, weights );

		wchar_t msg[128];
		CRefArray myDeformers = myEnvelope.GetDeformers();
		LONG deformerCount = myDeformers.GetCount();
		CTime frame;

		// for each deformer
		for ( i = 0L; i < deformerCount; i ++ )
		{
			swprintf( msg, L"Deformer #%d:", i );
			app.LogMessage( msg );

			// display the weights for the envelope at the current frame
			weights = myEnvelope.GetDeformerWeights( X3DObject(myDeformers[i]), frame.GetTime() );
			LONG elementCount = weights.GetCount();
			for ( register LONG i = 0L; i < elementCount; i++ )
			{
				swprintf( msg, L"Element %d = %f", i, weights[i] );
				app.LogMessage( msg );
			}
		}
	\endcode
	*/
	CStatus SetDeformerWeights( X3DObject &in_Deformer,	const CDoubleArray &in_Array );

	/*! Returns the color of the specified deformer.
	\param in_Deformer The deformer to get
	\param io_Color Color of the deformer (as a CColor)
	\return CStatus::OK
	\return CStatus::Fail
	\sa CColor
	*/
	CStatus GetDeformerColor( const X3DObject &in_Deformer, CColor &io_Color ) const;

	/*! Sets the color of a given deformer.
	\param in_Deformer The deformer to set
	\param in_Color New color of the deformer
	\return CStatus::OK
	\return CStatus::Fail
	\sa CColor
	*/
	CStatus SetDeformerColor( const X3DObject &in_Deformer, const CColor &in_Color );

private:
	Envelope * operator&() const;
	Envelope * operator&();
};

};

#endif // __XSIENVELOPE_H__
