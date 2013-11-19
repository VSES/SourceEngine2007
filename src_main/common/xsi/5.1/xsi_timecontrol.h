//*****************************************************************************
/*!
   \file xsi_timecontrol.h
   \brief TimeControl class declaration.

   © Copyright 1998-2003 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************
#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSITIMECONTROL_H__
#define __XSITIMECONTROL_H__

#include <xsi_property.h>

namespace XSI {


//*****************************************************************************
/*! \class TimeControl xsi_timecontrol.h
	\brief The TimeControl object represents the clip's Time Control property, which is used to
	specify the time reference for a given Clip object.

	For example, this property may be
	used to add extra clip effects like cycling, bouncing, or holding for fractions of the
	clip length. Like ClipEffect data, the time control effects applied to a specific clip
	do not alter the original source, nor any other clip instantiated on that source.

	This class provides convenience accessors to some of the time control parameters (under
	the General and Extrapolation tabs on the property page). However, there are certain clip
	types which do not nest a time control property under them: for example, the animation mixer.
	For these, the TimeControl object returned by the Clip disables the access to parameters.

	All scripting properties of the TimeControl object are read-only; however, you can still
	use the pointer to the TimeControl object to write to its parameters (since the TimeControl
	object is a specialized kind of Property object).

	\sa Clip::GetTimeControl
	\since 4.0

	\eg Illustrates how to use the time control to clip the source.

	\code
		using namespace XSI;

		Application app;
		Model root = app.GetActiveSceneRoot();

		Null myNull;
		root.AddNull( L"", myNull );
		Parameter posx(	myNull.GetParameter( L"posx" ) );

		// Create and connect an function curve to the position x
		FCurve fcrv;
		posx.AddFCurve( siUnknownFCurve, fcrv );

		// Set the keys
		LONG i = 0;
		CTimeArray timeArray(3);
		CValueArray valueArray(3);

		timeArray[i] = 1;	valueArray[i] = (double)10; i++;
		timeArray[i] = 50; 	valueArray[i] = (double)0; i++;
		timeArray[i] = 100; valueArray[i] = (double)10; i++;

		fcrv.SetKeys(timeArray,valueArray);
		// This value should be 10;
		app.LogMessage(L"The animated value at frame 1" + posx.GetValue(1).GetAsText());

		// Creating the animation source
		CValueArray args(9);
		CValue		outArg;
		args[0] = root.GetFullName();
		args[1] = L"null.kine.local.posx";
		args[2] = CValue(1.0);
		args[3] = L"StoredFcvPose";
		args[4] = true;
		args[5] = CValue(11.0);
		args[6] = CValue(1001.0);
		args[7] = false;
		args[8] = false;
		app.ExecuteCommand( L"StoreAction", args, outArg );

		Source mySource(outArg);

		// Creating the first clip
		CValueArray addClipArgs(6);
		addClipArgs[0] = root;
		addClipArgs[1] = mySource.GetFullName();
		addClipArgs[5] = L"MyClip1";
		app.ExecuteCommand( L"AddClip", addClipArgs, outArg );

		Clip myClip(outArg);

		TimeControl myTimeControl = myClip.GetTimeControl();
		myTimeControl.PutParameterValue(L"ClipIn", 50l);
		app.LogMessage(L"The clip in value is " +
						CValue(myTimeControl.GetClipIn()).GetAsText());

		// Now the value should be 0.
		app.LogMessage(L"The value after the clip manipulation " +
						posx.GetValue(1).GetAsText());

	\endcode
 */
//*****************************************************************************
class SICPPSDKDECL TimeControl : public Property
{
public:
	/*! Default constructor. */
	TimeControl();

	/*! Default destructor. */
	~TimeControl();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	TimeControl(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	TimeControl(const TimeControl& in_obj);

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
	\return The new TimeControl object.
	*/
	TimeControl& operator=(const TimeControl& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new TimeControl object.
	*/
	TimeControl& operator=(const CRef& in_ref);

	/*! The Extrapolation values determine how a clip contributes to the animation
	outside of the frames on which it is defined. The ExtrapolationBefore value
	determines what happens before the clip.

	On the Time Control property editor, this appears on the Extrapolation tab:
	either Hold, Cycles, or Bounces, depending on what the ExtrapolationBeforeType
	is set to (see ::siTimeControlExtrapolationType).

	\retval out_extrapolationValue returns the extrapolation value.
	\return CStatus::OK success
	\return CStatus::Fail failure.
	*/
	CStatus GetExtrapolationBeforeValue
	(
		double&					out_extrapolationValue
	) const;

	/*! The Extrapolation values determine how a clip contributes to the animation
	outside of the frames on which it is defined. The ExtrapolationBefore value determines
	what happens before the clip.

	On the Time Control property editor, this appears on the Extrapolation tab under Before
	as the Type parameter. The value of this property determines what kind of value will be
	considered for the ExtrapolationBeforeValue. For example, if ExtrapolationBeforeType is
	set to 2 ('Cycle'), then the ExtrapolationBeforeValue will read from the 'Cycles' parameter.

	\retval out_extrapolationType returns the extrapolation type.
	\return CStatus::OK success
	\return CStatus::Fail failure.
	*/
	CStatus GetExtrapolationBeforeType
	(
		siTimeControlExtrapolationType&	out_extrapolationType
	) const;

	/*! The Extrapolation values determine how a clip contributes to the animation outside of
	the frames on which it is defined. The ExtrapolationAfter value determines what happens after
	the clip.

	On the Time Control property editor, this appears on the Extrapolation tab: either Hold, Cycles,
	or Bounces, depending on what the ExtrapolationAfterType is set to (see ::siTimeControlExtrapolationType).

	\retval out_extrapolationValue returns the extrapolation value.
	\return CStatus::OK success
	\return CStatus::Fail failure.
	*/
	CStatus GetExtrapolationAfterValue
	(
		double&					out_extrapolationValue
	) const;

	/*! The Extrapolation values determine how a clip contributes to the animation outside of the frames
	on which it is defined. The ExtrapolationAfter value determines what happens after the clip.

	On the Time Control property editor, this appears on the Extrapolation tab under After as the Type
	parameter. The value of this property determines what kind of value will be considered for the
	ExtrapolationAfterValue. For example, if ExtrapolationAfterType is set to 3 ('Bounce'), then the
	ExtrapolationAfterValue will read from the 'Bounces' parameter.

	\retval out_extrapolationType returns the extrapolation type.
	\return CStatus::OK success
	\return CStatus::Fail failure.
	*/
	CStatus GetExtrapolationAfterType
	(
		siTimeControlExtrapolationType&	out_extrapolationType
	) const;

	/*! Returns the first frame of the source that is used by the clip. Increase this value to trim a clip
	so that it begins part way through the duration of the source.

	On the Time Control property editor, this appears on the General tab under Source Clipping.

	\return A frame number.
	*/
	double GetClipIn() const;

	/*! Returns the last frame of the source that is used by the clip. Decrease this value to trim a clip
	so that it ends part way through the duration of the source.

	On the Time Control property editor, this appears on the General tab under Source Clipping.
	\return A frame number.
	*/
	double GetClipOut() const;

	/*! Returns the frame where the clip starts, in the local time of the clip container.

	On the Time Control property editor, this appears on the General tab under Time Reference.
	\return A frame number
	*/
	double GetStartOffset() const;

	/*! Returns the scaling of the clip in time. Increasing this value speeds up the relative time
	of the clip, decreasing the duration.

	On the Time Control property editor, this appears on the General tab under Time Reference.
	\return The clip scaling number
	*/
	double GetScale() const;

	private:
	TimeControl * operator&() const;
	TimeControl * operator&();
};

}

#endif // __XSITIMECONTROL_H__
