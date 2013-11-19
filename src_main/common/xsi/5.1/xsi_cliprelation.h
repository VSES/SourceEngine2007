//*****************************************************************************
/*!
   \file xsi_cliprelation.h
   \brief ClipRelation class declaration.

   © Copyright 1998-2004 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************
#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSICLIPRELATION_H__
#define __XSICLIPRELATION_H__

#include <xsi_projectitem.h>

namespace XSI {

class Clip ;

//*****************************************************************************
/*! \class ClipRelation xsi_cliprelation.h
	\brief Clip relations are logical entities used to create a bi-directional constraint between two clips,
	the master and slave clip.

	By default, a %ClipRelation creates relationships between the start offset, clip in and clip out value of
	the TimeControl of the related clips. ClipRelations are bi-directional in the sense that, by modifying the
	value of one of the related parameter either on the master or slave clip it proportionally affects the value
	of the other clip.

	For each individual relationship there's an activeness Parameter (\c StartActiveness, \c ClipInActiveness
	and \c ClipOutActiveness) and an offset. The offset represents the difference between the value of the Master
	clip parameter and the Slave clip parameter.  Modifying the value of an offset results in changing the value
	of the slave clip relative to the master clip. It is possible to inverse the master and slave clip by using
	ClipRelation::SwapMasterAndSlaveClip.

	You can create a %ClipRelation using ClipContainer::AddRelation and access them using either Clip::GetRelations
	or ClipContainer::GetNestedRelations. In order to access the master and slave clip you can use
	ClipRelation::GetMasterClip or ClipRelation::GetSlaveClip.

	\sa Clip::GetRelations, ClipContainer::AddRelation, ClipContainer::GetNestedRelations
	\since 4.0

	\eg Demonstrates how to create a ClipRelation, how to access its offsets and
		how modifying one makes an impact on the TimeControl of the master and slave clip.

	\code
		using namespace XSI;
		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject myCube;
		root.AddGeometry( L"Cube", L"MeshSurface", L"", myCube );

		CValueArray args(9);
		CValue		outArg;
		args[0] = root;
		args[1] = L"cube.kine.local.posx,cube.kine.local.posy,cube.kine.local.posz";
		args[2] = 1l;
		args[3] = L"StoredStaticPose";
		args[4] = true;
		args[5] = 1l;
		args[6] = 5l;
		args[7] = false;
		args[8] = false;
		app.ExecuteCommand( L"StoreAction", args, outArg );

		Source mySource(outArg);

		CValueArray addClipArgs(6);
		addClipArgs[0] = root;
		addClipArgs[1] = mySource.GetFullName();
		addClipArgs[4] = 19l;
		addClipArgs[5] = L"MyClip";
		app.ExecuteCommand( L"AddClip", addClipArgs, outArg );

		Clip myClip(outArg);

		addClipArgs[4] = 47l;
		addClipArgs[5] = L"MyClip1";
		app.ExecuteCommand( L"AddClip", addClipArgs, outArg );

		Clip myClip1(outArg);

		Mixer myRootMixer = root.GetMixer();
		ClipRelation myClipRelation = myRootMixer.AddRelation(myClip, myClip1 , L"myRelation");

		app.LogMessage(L">>>Original ClipRelation information.");
		Print_ClipRelation_Info(myClipRelation);

		app.LogMessage(L">>>Modifying the startoffset value of the ClipRelation.");
		myClipRelation.PutParameterValue(L"StartOffset",
					(double)myClipRelation.GetParameterValue(L"StartOffset") + 3);
		Print_ClipRelation_Info(myClipRelation);

		app.LogMessage(L">>>Modifying the start value of the TimeControl of the master clip.");
		TimeControl myMasterClipTC = myClipRelation.GetMasterClip().GetTimeControl();
		myMasterClipTC.PutParameterValue(L"StartOffset",
						(double)myMasterClipTC.GetParameterValue(L"StartOffset") + 4);
		Print_ClipRelation_Info(myClipRelation);

		app.LogMessage(L">>>Swapping master and slave clip.");
		myClipRelation.SwapMasterAndSlaveClip();
		Print_ClipRelation_Info(myClipRelation);

		app.LogMessage(L">>>Modifying the startoffset value of the ClipRelation.");
		myClipRelation.PutParameterValue(L"StartOffset",
						(double)myClipRelation.GetParameterValue(L"StartOffset") + 3);
		Print_ClipRelation_Info(myClipRelation);

		void Print_ClipRelation_Info( ClipRelation& in_rClipRelation )
		{
			Application app;
			app.LogMessage(L"The clip relation name is: " + in_rClipRelation.GetName() );

			//Printing the offset between the master and slave clips
			app.LogMessage( L"The start offset is: " +
							 in_rClipRelation.GetParameterValue(L"StartOffset").GetAsText() );

			app.LogMessage( L"The ClipIn offset is: " +
							in_rClipRelation.GetParameterValue(L"ClipInOffset").GetAsText() );

			app.LogMessage( L"The ClipOut offset is: " +
							in_rClipRelation.GetParameterValue(L"ClipOutOffset").GetAsText() );

			// Printing the value related to the master clip
			Clip masterClip = in_rClipRelation.GetMasterClip();
			app.LogMessage( L"The master clip name is: " + masterClip.GetName() );

			TimeControl masterTimeControl = masterClip.GetTimeControl();
			app.LogMessage( L"The master clip start value is: " +
							masterTimeControl.GetParameterValue(L"StartOffset").GetAsText() );
			app.LogMessage( L"The master clip ClipIn value is: " +
							masterTimeControl.GetParameterValue(L"ClipIn").GetAsText() );
			app.LogMessage( L"The master clip ClipOut value is: " +
							masterTimeControl.GetParameterValue(L"ClipOut").GetAsText() );

			// Printing the value related to the slave clip
			Clip slaveClip = in_rClipRelation.GetSlaveClip();
			app.LogMessage( L"The slave clip name is: " + slaveClip.GetName() );

			TimeControl slaveTimeControl = slaveClip.GetTimeControl();
			app.LogMessage( L"The slave clip start value is: " +
							slaveTimeControl.GetParameterValue(L"StartOffset").GetAsText());
			app.LogMessage( L"The slave clip ClipIn value is: " +
							slaveTimeControl.GetParameterValue(L"ClipIn").GetAsText() );
			app.LogMessage( L"The slave clip ClipOut value is: " +
							slaveTimeControl.GetParameterValue(L"ClipOut").GetAsText() );

		}
	\endcode
 */
//*****************************************************************************
class SICPPSDKDECL ClipRelation : public ProjectItem
{
public:
	/*! Default constructor. */
	ClipRelation();

	/*! Default destructor. */
	~ClipRelation();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	ClipRelation(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	ClipRelation(const ClipRelation& in_obj);

	/*! Returns true if a given class type is compatible with this API class.
	\param in_ClassID class type.
	\returns bool true if the class is compatible, false otherwise.
	*/
	bool IsA( siClassID in_ClassID) const;

	/*! Returns the type of the API class.
	\returns siClassID the class type.
	*/
	siClassID GetClassID() const;

	/*! Creates an object from another object. The newly created object is set to
	empty if the input object is not compatible.
	\param in_obj constant class object.
	\returns ClipRelation& New ClipRelation object.
	*/
	ClipRelation& operator=(const ClipRelation& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\returns ClipRelation& New ClipRelation object.
	*/
	ClipRelation& operator=(const CRef& in_ref);

	/*! Returns the Master clip of a ClipRelation.
	\returns Clip master clip
	\sa ClipRelation::GetSlaveClip, ClipRelation::SwapMasterAndSlaveClip, ClipContainer::GetNestedRelations,
		ClipContainer::AddRelation
	\since 4.0
	*/
	Clip  GetMasterClip()const;

  	/*! Returns the Slave clip of a ClipRelation.
	\returns Clip slave clip
	\sa ClipRelation::GetMasterClip, ClipRelation::SwapMasterAndSlaveClip, ClipContainer::GetNestedRelations,
		ClipContainer::AddRelation
	\since 4.0
	*/
	Clip  GetSlaveClip()const;

	/*! Exchanges the master and slave clip of the ClipRelation and recomputes the offsets. When updating an
	offset this always affects the slave clip. Therefore this function is useful if, for example, you would
	like to modify the start offset of the Master clip relative to the Slave clip.
	\returns CStatus::OK success
	\returns CStatus::Fail failure
	\sa ClipRelation::GetSlaveClip, ClipRelation::GetMasterClip, ClipContainer::GetNestedRelations, ClipContainer::AddRelation
	\since 4.0
	*/
	CStatus SwapMasterAndSlaveClip();

	private:
	ClipRelation * operator&() const;
	ClipRelation * operator&();
};

}

#endif // __XSICLIPRELATION_H__
