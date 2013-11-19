//*****************************************************************************
/*!
   \file xsi_fcurve.h
   \brief FCurve class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIFCURVE_H__
#define __XSIFCURVE_H__

#include <xsi_animsource.h>
#include <xsi_fcurvekey.h>
#include <xsi_time.h>
#include <xsi_value.h>
#include <xsi_longarray.h>
#include <xsi_doublearray.h>

namespace XSI {

//*****************************************************************************
/*! \class FCurve xsi_fcurve.h
	\brief The FCurve (function curve) class provides access to a set of functions for getting and setting %XSI fcurve attributes,
	as well as creating, retrieving, modifying and deleting fcurve keys.

	FCurves can be created and connected to parameters using the Parameter::AddFCurve function. FCurves are supported by the
	following parameter types: ::siBool, ::siDouble, ::siFloat, ::siInt4, ::siInt2, ::siByte, ::siByte, ::siUInt4, ::siUInt2 and
	::siUByte (see ::siVariantType for definitions of these types).

	\note Interpolation (how the value is evaluated between FCurveKeys) can be constant, linear, or cubic. \a Cubic means that a
		Bezier curve is calculated as the interpolation between the keys. %XSI uses cubic Bezier curves defined as follows:

	\par Definition from <A HREF="http://en.wikipedia.org/wiki/De_Casteljau%27s_algorithm">de Casteljau's algorithm</a>:
		Four points A, B, C and D in the plane or in three-dimensional space define a cubic Bezier curve. The curve
		starts at A going toward B and arrives at D coming from the direction of C. In general, it will not pass
		through B or C; these points are only there to provide directional information. The distance between A and B
		determines "how long" the curve moves into direction B before turning towards D."

	\par The parametric form of the curve is:

	\code 			B(t) = A(1-t)^3 + 3Bt(1-t)^2 + 3Ct^2(1-t) + Dt^3 \endcode

	\sa Parameter::GetSource, FCurveKey

	\eg
	\code
		using namespace XSI;
		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject myCube;
		root.AddGeometry( L"Cube", L"MeshSurface", L"", myCube );

		// create and connect an fcurve to the position x
		Parameter posx(	myCube.GetParameter(L"posx") );

		FCurve fcrv;
		posx.AddFCurve( siUnknownFCurve, fcrv );

		// show the new fcurve type
		siFCurveType fcrvType = fcrv.GetFCurveType();

		CString strFcrvType;

		switch ( fcrvType )
		{
			case siUnknownFCurve: strFcrvType = L"siUnknownFCurve"; break;
			case siBooleanFCurve: strFcrvType = L"siBooleanFCurve"; break;
			case siIntegerFCurve: strFcrvType = L"siIntegerFCurve"; break;
			case siStandardFCurve: strFcrvType = L"siStandardFCurve"; break;
			case siRawDataFCurve: strFcrvType = L"siRawDataFCurve"; break;
			default:
				strFcrvType = L"Invalid FCurve type";
		}

		app.LogMessage( L"FCurve type : " + strFcrvType );
	\endcode
 */
//*****************************************************************************

class SICPPSDKDECL FCurve : public AnimationSource
{
public:
	/*! Default constructor. */
	FCurve();

	/*! Default destructor. */
	~FCurve();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	FCurve(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	FCurve(const FCurve& in_obj);

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
	\return The new FCurve object.
	*/
	FCurve& operator=(const FCurve& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new FCurve object.
	*/
	FCurve& operator=(const CRef& in_ref);

	/*! Returns the value type of the function curve.
	\return The value type of the fcurve.
	*/
	CValue::DataType 		GetValueType() const;

	/*! Returns the function curve's %FCurve type (standard, boolean, raw, etc.).
	\return The fcurve type (one of the ::siFCurveType values).
	*/
	siFCurveType			GetFCurveType(void) const;

	/*! Sets the function curve's curve type.
	\param in_type Function curve type.
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus					PutFCurveType(siFCurveType in_type);

	/*! Returns true if the function curve is locked.
	\return True if the fcurve is locked
	*/
	bool					GetLocked(void) const;

	/*! Sets the locked state of the function curve.
	\param in_value new locked state
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus					PutLocked(bool in_value);

	/*! Returns the fcurve's dependency on the frame rate (time). If the fcurve is set as time-dependent it will adjust
	when the scene's frame rate changes. Typically you don't want this to be set to true when the fcurve is a profile
	curve that does not use time as a factor.

	\a Animation fcurves are typically time-dependent; that is, they generally represent change in a parameter's value over
	time (for example, a change in an object's X position from frame 10 to frame 90).

	\a Profile fcurves usually do not use frame values on either axis. Profile fcurves are used in deformations to allow
	a user to fine-tune an effect (for example, the Bulge operator uses percentage of amplitude on the Y-axis over distance
	from the deformation center on the X-axis).

	\return True if the fcurve X axis represents time
	\since 5.0
	*/
	bool					GetDependsOnFrameRate(void) const;

	/*! Sets the fcurve's dependency on the frame rate (time). If the fcurve is set as time-dependent it will adjust
	when the scene's frame rate changes. Typically you don't want this to be set to true when the fcurve is a profile
	curve that does not use time as a factor.

	\a Animation fcurves are typically time-dependent; that is, they generally represent change in a parameter's value over
	time (for example, a change in an object's X position from frame 10 to frame 90).

	\a Profile fcurves usually do not use frame values on either axis. Profile fcurves are used in deformations to allow
	a user to fine-tune an effect (for example, the Bulge operator uses percentage of amplitude on the Y-axis over distance
	from the deformation center on the X-axis).

	\param in_value New time dependency type
	\return CStatus::OK success
	\return CStatus::Fail failure
	\since 5.0
	*/
	CStatus					PutDependsOnFrameRate(bool in_value);

	/*! Returns the value of the function curve when there are no keys.
	\return The fcurve value.
	*/
	CValue					GetNoKeyValue(void) const;

	/*! Sets the value of the function curve when there are no keys.
	\param in_value function curve value
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus					PutNoKeyValue(const CValue& in_value);

	/*! Returns the extrapolation type of the function curve.
	\return The fcurve's extrapolation type (one of the ::siFCurveExtrapolation values).
	*/
	siFCurveExtrapolation 	GetExtrapolation(void) const;

	/*! Sets the extrapolation type of the function curve.
	\param in_type The fcurve's extrapolation type (one of the ::siFCurveExtrapolation values).
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus					PutExtrapolation(siFCurveExtrapolation in_type);

	/*! Returns the interpolation type of a standard function curve.
	\return The fcurve's interpolation type (one of the ::siFCurveInterpolation values).
	*/
	siFCurveInterpolation 	GetInterpolation(void) const;

	/*! Sets the interpolation type of a standard function curve.
	\param in_type The fcurve's interpolation type (one of the ::siFCurveInterpolation values).
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus					PutInterpolation(siFCurveInterpolation in_type);

	/*! Returns the lower-clamping value of the function curve.
	\return The fcurve's lower-clamping value.
	*/
	CValue					GetLowClamp(void) const;

	/*! Sets the lower-clamping value of the function curve.
	\param in_value The fcurve's lower-clamping value.
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus					PutLowClamp(const CValue& in_value);

	/*! Returns the higher-clamping value of the function curve.
	\return The fcurve's higher-clamping value.
	*/
	CValue					GetHighClamp(void) const;

	/*! Sets the higher-clamping value of the function curve.
	\param in_value The fcurve's higher-clamping value.
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus					PutHighClamp(const CValue& in_value);

	/*! Evaluates the function curve at a given frame. If the curve is discontinuous, the right limit is returned.
	\param in_frame The frame at which to evaluate the function curve.
	\return The value of the fcurve at a given frame.
	*/
	CValue					Eval(const CTime& in_frame) const;

	/*! Returns an array of FCurveKey objects on the function curve.
	\return Array of references to FCurveKey objects.
	*/
	CRefArray				GetKeys(void) const;

	/*! Sets the keys of the function curve, previous keys are discarded if the argument \c in_keepexistingkeys = false.
	\param in_timearray An array of times
	\param in_valuearray An array of values
	\param in_keepexistingkeys (optional--defaults to false)
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus					SetKeys(
	const CTimeArray& in_timearray,
	const CValueArray& in_valuearray,
	bool in_keepexistingkeys = false );

	/*! Sets the keys of the function curve, previous keys are discarded if the argument \c in_keepexistingkeys = false.
	This function can only be used with standard fcurves, since only standard fcurves support key interpolation and key
	tangents.

	The tangents are optional; however, if specified, the auto tangent constraint will be turned off to allow the tangents
	to be set without modification. This may lead to an invalid curve depending on the tangent values. If you want the
	tangents to be automatically calculated pass an empty array for each of the tangent values or specify both the x,y
	values to be \c DBL_MAX.

	\param in_timearray An array of times
	\param in_valuearray An array of values
	\param in_keyinterpolationarray An array of key interpolations
	\param in_lefttanxarray An array of left tan x, time value relative to key time (optional)
	\param in_lefttanyarray An array of left tan y, a value relative to key value (optional)
	\param in_righttanxarray An array of right tan x, a value relative to key value (optional)
	\param in_righttanyarray An array of right tan y (optional)
	\param in_keepexistingkeys (optional--defaults to false)

	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus					SetKeys(
	const CTimeArray&	in_timearray,
	const CDoubleArray& in_valuearray,
	const CLongArray&	in_keyinterpolationarray,
	const CTimeArray& in_lefttanxarray = CTimeArray(),
	const CDoubleArray& in_lefttanyarray = CDoubleArray(),
	const CTimeArray& in_righttanxarray = CTimeArray(),
	const CDoubleArray& in_righttanyarray = CDoubleArray(),
	bool				in_keepexistingkeys = false );

	/*! Sets the values of the tangency values on both sides of the key value.
	\param in_array An array of tangency values (LeftTanX, LeftTanY, RightTanX, RightTanY). The LeftTanX and RightTanX
		must be specified in frame units.
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus					SetKeyTangents( const CDoubleArray& in_array );

	/*! Returns true if the fcurve is using the SI3D style of tangent constraints.
	\return true for SI3D tangent constraint style.
	*/
	bool GetSI3DStyle(void) const;

	/*! Sets the style of tangent constraints to use. True to use the SI3D style of tangent constraints.
	\param in_value True to use SI3D tangent constraints; false for %XSI tangent constraints.
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus PutSI3DStyle(bool in_value);

	/*! Adds a new key at the specified range with the specified value. The tolerance argument can be used to merge all
	keys within a certain range. The range is defined as \c Frame - \c Tolerance and \c Frame + \c Tolerance. The merged
	key will inherit the constraints of the nearest key within this range.

	\param in_frame New key frame
	\param in_value New key value
	\param in_interp Key interpolation
	\param in_ltx Left tangent x
	\param in_lty Left tangent y
	\param in_rtx Right tangent x
	\param in_rty Right tangent y
	\param in_tol By default a tolerance of 0.5 frame is specified.
	\param in_okl True to override the key locks

	\return index > 0 (success)
	\return index == -1 (failure)
	*/
	LONG AddKey(
			const CTime& in_frame,
			const CValue& in_value,
			siFCurveKeyInterpolation in_interp = siDefaultKeyInterpolation,
			double in_ltx = DBL_MAX,
			double in_lty = DBL_MAX,
			double in_rtx = DBL_MAX,
			double in_rty = DBL_MAX,
			double in_tol = DBL_MAX,
			bool in_okl = false );

	/*! Resamples the fcurve, adding keys from startframe to endframe at the interval defined by the step argument.
	A valid range is defined as:

	\code 		startframe < endframe and step < 0.25 * ( endframe - startframe ) \endcode

	By default the frame range specified by the playcontrol's \c In and \c Out parameters will be used. A key is
	created at the start frame and end frame. The number of samples created between these keys is:

	\code 		( endframe - startframe ) / step \endcode

	Depending on the frame rate there may be a rounding error with the last key resulting in an extra key being
	created at the end.

	\param in_startframe The start frame of the range to resample
	\param in_endframe The end frame of the range to resample
	\param in_step Step
	\param in_keepexistingkeys True to keep existing keys in resample range
	\param in_overridekeylock True to override the key locks
	\return CStatus::OK success
	\return CStatus::AccessDenied if the fcurve or fcurvekeys are locked
	\return CStatus::Fail failure

	\sa FCurve::Smooth,FCurve::Fit	
	*/
	CStatus Resample(
				const CTime& in_startframe = CTime(DBL_MAX,CTime::CUSTOM),
				const CTime& in_endframe = CTime(DBL_MAX,CTime::CUSTOM),
				double in_step = 1,
				bool in_keepexistingkeys = false,
				bool in_overridekeylock = false );

	/*! Smooths the FCurve in a given time span using a filter by removing sharp
	spikes and jags from the FCurve by averaging between keys. The FCurve is
	smoothed first and re-sampled at a sampling step of \c in_dSamplingStep.

	\param in_start The start time of the range to smooth. Must be less than the end time (\c in_end value).
		Defaults to the first key frame on the fcurve or, if there are no keys, the
		<a href="ms-its:xsiparams.chm::/xsiparams/PlayControl.html">PlayControl</a>.In frame value.
	\param in_end The end time of the range to smooth. Defaults to the last key frame or, if there are no
		keys, the <a href="ms-its:xsiparams.chm::/xsiparams/PlayControl.html">PlayControl</a>.Out frame value.
	\param in_dFilterSize Defines the number of keys that are averaged to
		calculate a new key. A high filter size will yield a much straighter curve.
		Defaults to \c 5 and must be greater than \c 1.
	\param in_dSamplingStep The sampling step applied to the FCurve after smoothing.
		Must be less than \c in_dFilterSize. Defaults to \c 1.
	\param in_filterType Specifies the type of filter we want to use for smoothing (see ::siFCurveSmoothFilterType).
		Defaults to ::siFCurveAverageFilterType.
	\param in_dGaussianVariance Variance of the Gaussian filter. Controls the
		degree of smoothing. A higher variance smooths the function curve more.
		Defaults to \c 1 and must be greater than \c 0.

	\return CStatus::OK success
	\return CStatus::AccessDenied if the FCurve or FCurveKeys are locked
	\return CStatus::InvalidArgument One of the argument is invalid.
	\return CStatus::Fail failure

	\sa FCurve::Fit,FCurve::Resample
	\since Gemini
	*/
	CStatus Smooth(	const CTime& in_start = CTime(DBL_MAX,CTime::CUSTOM),
					const CTime& in_end = CTime(DBL_MAX,CTime::CUSTOM),
					double in_dFilterSize=5,
					double in_dSamplingStep=1,
					siFCurveSmoothFilterType in_filterType=siFCurveAverageFilterType,
					double in_dGaussianVariance=1 );

	/*! Fits the FCurve within a time span by removing redundant keys while preserving the original shape of a function curve.

	\param in_start The start time of the range to fit. Must be less than the end time (\c in_end value).
		Defaults to the first key frame on the fcurve or, if there are no keys, the
		<a href="ms-its:xsiparams.chm::/xsiparams/PlayControl.html">PlayControl</a>.In frame value.
	\param in_end The end time of the range to fit. Defaults to the last key frame or, if there are no keys,
		the <a href="ms-its:xsiparams.chm::/xsiparams/PlayControl.html">PlayControl</a>.Out frame value.
	\param in_dFittingTolerance Adjusts the closeness of the fit (that is, Maximum distance between the FCurve
		and the resulting fitted FCurve). Smaller values will respect the original shape of the curve more by
		using more keys, while larger values will use less keys and result in a fit that deviates from the
		original curve. Defaults to \c 0.1 and must be greater than \c 0.

	\return CStatus::OK success
	\return CStatus::AccessDenied if the FCurve or FCurveKeys are locked
	\return CStatus::InvalidArgument One of the argument is invalid.
	\return CStatus::Fail failure

	\sa FCurve::Smooth, FCurve::Resample
	\since Gemini
	*/
	CStatus Fit(	const CTime& in_start = CTime(DBL_MAX,CTime::CUSTOM),
					const CTime& in_end = CTime(DBL_MAX,CTime::CUSTOM),
					double in_dFittingTolerance = 0.1);

	/*! Copies the specified %FCurve (source) to this %FCurve (target). 

	\note The operation is undoable. Any pending undo session previously set 
	with <object idref="FCurve.BeginEdit"/> is automatically ended before 
	copying the %FCurve.

	\param in_fcurve The source FCurve to copy.

	\return CStatus::OK success
	\return CStatus::AccessDenied if the %FCurve or FCurveKeys are locked
	\return CStatus::InvalidArgument The source FCurve is invalid.
	\return CStatus::Fail Input %FCurve type is not compatible. You cannot set an
	%FCurve which is connected to a non-boolean parameter with a boolean %FCurve 
	(i.e. siBooleanFCurve).

	\sa Parameter.AddFCurve
	\since Gemini

	\eg
	\code
		using namespace XSI;
		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject myCube;
		root.AddGeometry( L"Cube", L"MeshSurface", L"", myCube );

		// Create and connect an fcurve to position X
		Parameter posx(	myCube.GetParameter(L"posx") );

		FCurve posxFc;
		posx.AddFCurve( siStandardFCurve, posxFc );

		// Add some keys
		posxFc.Resample();
		posxFc.PutLowClamp( 25.0 );
		posxFc.PutHighClamp( 75.0 );

		// Create and connect an fcurve to rotation X
		Parameter rotx(	myCube.GetParameter(L"rotx") );

		FCurve rotxFc;
		rotx.AddFCurve( siStandardFCurve, rotxFc );

		// Set rotxFc with the posx fcurve
		rotxFc.Set( posxFc );

		// Expected output:
		//'INFO : rotxFc nb keys : 100
		//'INFO : rotxFc low clamp: 25
		//'INFO : rotxFc high clamp: 75
		app.LogMessage( L"rotxFc nb keys : " + CString(rotxFc.GetKeys().GetCount()) );
		app.LogMessage( L"rotxFc low clamp: " + CString(rotxFc.GetLowClamp()) );
		app.LogMessage( L"rotxFc high clamp: " + CString(rotxFc.GetHighClamp()) );
	\endcode
	*/
	CStatus Set( const FCurve& in_fcurve );
    
	/*! Removes the nearest key to the specified frame within the specified tolerance range.
	\param in_frame The frame to look for the key
	\param in_tolerance By default a tolerance of \c 0.5 frame is specified.
	\param in_overridekeylock True to override the key locks
	\return CStatus::OK success
	\return CStatus::AccessDenied the fcurve or key are locked
	\return CStatus::Fail failure
	*/
	CStatus RemoveKey( const CTime& in_frame, double in_tolerance = DBL_MAX, bool in_overridekeylock = false );

	/*! Removes the key at the specified key index.
	\param in_index The key index
	\param in_overridekeylock True to override the key locks
	\return CStatus::OK success
	\return CStatus::InvalidArgument index arguments are out of range
	\return CStatus::AccessDenied the fcurve or key are locked
	\return CStatus::Fail failure
	*/
	CStatus RemoveKeyAtIndex( LONG in_index, bool in_overridekeylock = false );

	/*! Removes a specified number of keys starting at the specified key index. By default \c startframe is the
	first key and \c endframe is the last key.
	\param in_index The start key index
	\param in_count Number of keys to remove. The value specified must be greater than or equal to \c 1.
	\param in_overridekeylock True to override the key locks
	\return CStatus::OK success
	\return CStatus::InvalidArgument index or count arguments are out of range
	\return CStatus::AccessDenied the fcurve or key are locked
	\return CStatus::Fail failure
	*/
	CStatus RemoveKeysAtIndex( LONG in_index, LONG in_count, bool in_overridekeylock = false );

	/*! Removes all keys within the specified range. By default \c startframe is the first key and \c endframe
	is the last key.
	\param in_startframe The start frame of the range to search
	\param in_endframe The end frame of the range to search
	\param in_overridekeylock True to override the key locks
	\return The number of keys removed (success)
	\return -1 (failure)
	*/
	LONG RemoveKeys( const CTime& in_startframe = CTime(DBL_MAX,CTime::CUSTOM), const CTime& in_endframe = CTime(DBL_MAX,CTime::CUSTOM), bool in_overridekeylock = false );

	/*! Returns whether a key can be found at the specified frame within the tolerance range.
	\param in_frame The frame to look for a key
	\param in_tolerance By default a tolerance of \c 0.5 frame is specified.
	\return True if a key can be found at the specified frame within the tolerance range
	*/
	bool KeyExists( const CTime& in_frame, double in_tolerance = DBL_MAX ) const;

	/*! Returns the nearest key to the specified frame within the tolerance range.
	\param in_frame The frame to look for a key
	\param in_tolerance By default a tolerance of \c 0.5 frame is specified.
	\return The nearest FCurveKey.
	\return Empty CRef object if the key does not exist.
	*/
	FCurveKey GetKey( const CTime& in_frame, double in_tolerance = DBL_MAX ) const;

	/*! Returns the number of keys within the specified range. By default \c startframe is the first key and
	\c endframe is the last key.
	\param in_startframe The start frame of the range to search
	\param in_endframe The end frame of the range to search
	\return The number of keys on the fcurve.
	*/
	LONG GetNumKeys( const CTime& in_startframe = CTime(DBL_MAX,CTime::CUSTOM), const CTime& in_endframe = CTime(DBL_MAX,CTime::CUSTOM) ) const;

	/*! Returns the key at the specified index.
	\param in_index The index of the key to get.
	\return The FCurveKey at the specified index.
	\return CStatus::InvalidArgument error if the index is out of range
	*/
	FCurveKey GetKeyAtIndex( LONG in_index ) const;

	/*! Returns the index of the nearest key to the specified frame within the tolerance range.
	\param in_frame The frame to look for a key
	\param in_tolerance By default a tolerance of 0.5 frame is specified.
	\return The index of the key.
	\return \c -1 if the key does not exist.
	*/
	LONG GetKeyIndex( const CTime& in_frame, double in_tolerance = DBL_MAX ) const;

	/*! Returns the frame of the key at the specified index.
	\param in_index The index of the key to get.
	\return The key time.
	\return CStatus::InvalidArgument error if the index is out of range.
	*/
	CTime GetKeyTime( LONG in_index ) const;

	/*! Returns the value of the key at a specified index.
	\param in_index The index of the key to get.
	\return The key value.
	\return CStatus::InvalidArgument error if the index is out of range.
	*/
	CValue GetKeyValue( LONG in_index ) const;

	/*! Sets the time (in frames) and value of the key at the specified index.
	\param in_index The index of key to set
	\param in_frame New key frame
	\param in_value New key value
	\param in_overridekeylock True to override the key lock
	\return CStatus::OK success
	\return CStatus::InvalidArgument if the index is out of range
	\return CStatus::AccessDenied if the fcurve or key is locked
	\return CStatus::Fail error if another key already exists at the new frame value
	*/
	CStatus SetKey( LONG in_index, const CTime& in_frame, const CValue& in_value, bool in_overridekeylock = false );

	/*! Returns true if a bulk edit of the fcurve is in progress.
	\return True if a bulk edit of the fcurve is in progress
	*/
	bool IsEditing(void) const;

	/*! Starts recording bulk edits made to the fcurve. A call to FCurve::BeginEdit must be paired with a call to
	FCurve::EndEdit, which will commit your changes to the undo/redo stack.

	Editing changes can be undone by calling FCurve::UndoEdit and you can check if the fcurve is being bulk edited
	by using FCurve::IsEditing. If you try to begin another bulk edit a warning is issued, and the current editing
	session is committed to the undo/redo stack.
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus BeginEdit(void);

	/*! Stops recording bulk edits made to the fcurve and commits changes to the undo/redo stack. A call to
	FCurve::EndEdit must follow a call to FCurve::BeginEdit.

	\note If you call FCurve::EndEdit when there is no bulk edit in progress, the function will return an error.
	You can check if a bulk edit is in process by using the FCurve::IsEditing function.
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus EndEdit(void);

	/*! Undoes the bulk edits made to the fcurve and closes the current bulk edit.
	\note If you call FCurve::UndoEdit when there is no bulk edit is progress, the function returns CStatus::False.
	\return CStatus::OK success
	\return CStatus::False no bulk edit in progress to undo.
	\return CStatus::Fail failure
	*/
	CStatus UndoEdit(void);

private:
	FCurve * operator&() const;
	FCurve * operator&();
};

}; // namespace XSI

#endif // __XSIFCURVE_H__
