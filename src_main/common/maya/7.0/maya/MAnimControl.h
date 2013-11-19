#ifndef _MAnimControl
#define _MAnimControl
//
//-
// ==========================================================================
// Copyright (C) 1995 - 2005 Alias Systems Corp. and/or its licensors.  All 
// rights reserved.
// 
// The coded instructions, statements, computer programs, and/or related 
// material (collectively the "Data") in these files contain unpublished 
// information proprietary to Alias Systems Corp. ("Alias") and/or its 
// licensors, which is protected by Canadian and US federal copyright law and 
// by international treaties.
// 
// The Data may not be disclosed or distributed to third parties or be copied 
// or duplicated, in whole or in part, without the prior written consent of 
// Alias.
// 
// THE DATA IS PROVIDED "AS IS". ALIAS HEREBY DISCLAIMS ALL WARRANTIES RELATING 
// TO THE DATA, INCLUDING, WITHOUT LIMITATION, ANY AND ALL EXPRESS OR IMPLIED 
// WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE. IN NO EVENT SHALL ALIAS BE LIABLE FOR ANY DAMAGES 
// WHATSOEVER, WHETHER DIRECT, INDIRECT, SPECIAL, OR PUNITIVE, WHETHER IN AN 
// ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, OR IN EQUITY, 
// ARISING OUT OF ACCESS TO, USE OF, OR RELIANCE UPON THE DATA.
// ==========================================================================
//+
//
// *****************************************************************************
//
// CLASS:    MAnimControl
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MAnimControl)
//
//  This class provide access to the values that control how an animation is
//  played.  This includes the minimum and maximum frames included in the
//  playback, whether the playback loops, and whether the animation runs
//  in all views, or only the active view, etc.
//
//  Methods also exist that mirror the functionality of the controls found
//  on the time slider in the UI to start and stop the playback.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTime.h>
#include <maya/MTypes.h>
#include <maya/MFnAnimCurve.h>

// *****************************************************************************

// DECLARATIONS

// *****************************************************************************

// CLASS DECLARATION (MAnimControl)

/// Control over animation playback and values (OpenMayaAnim)
/**
 Retrieve and set animation parameters and control playback
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAANIM_EXPORT MAnimControl
{
public:
	///
	enum PlaybackMode {
		/// Play the animation once then stop
		kPlaybackOnce,
		/// Play the animation continuously
		kPlaybackLoop,
		/// Play the animation forwards, then backwards continuously
		kPlaybackOscillate
	};

	///
	enum PlaybackViewMode {
		/// The animation plays back in all views
		kPlaybackViewAll,
		/// The animation plays back in only the active view
		kPlaybackViewActive
	};


	///
											MAnimControl();
	///
	virtual									~MAnimControl();
	///
	static MAnimControl::PlaybackMode		playbackMode();
	///
	static MStatus							setPlaybackMode( PlaybackMode
															 newMode );
	///
	static MAnimControl::PlaybackViewMode	viewMode();
	///
	static MStatus							setViewMode( PlaybackViewMode
														 newMode );
	///
	static double							playbackBy();
	///
	static MStatus							setPlaybackBy( const double& newTime );
	///
	static MTime							minTime();
	///
	static MTime							maxTime();
	///
	static MStatus							setMinTime( MTime newMinTime );
	///
	static MStatus							setMaxTime( MTime newMaxTime );
	///
	static MStatus							setMinMaxTime( MTime min,
														   MTime max );
	///
	static MTime							animationStartTime();
	///
	static MTime							animationEndTime();
	///
	static MStatus							setAnimationStartTime( MTime newStartTime );
	///
	static MStatus							setAnimationEndTime( MTime newEndTime );
	///
	static MStatus							setAnimationStartEndTime( MTime newStartTime,
														   MTime newEndTime );
	///
	static MTime							currentTime();
	///
	static MStatus							setCurrentTime( const MTime&
															newTime );
	///
	static double							playbackSpeed();
	///
	static MStatus							setPlaybackSpeed( double newSpeed );
	///
	static MStatus							playForward();
	///
	static MStatus							playBackward();
	///
	static bool								isPlaying();
	///
	static MStatus							stop();
	///
	static bool			autoKeyMode ();
	///
	static MStatus		setAutoKeyMode ( bool mode );
	///
	static MFnAnimCurve::TangentType globalInTangentType (
												MStatus * ReturnStatus = NULL );
	///
	static MStatus		setGlobalInTangentType (const MFnAnimCurve::TangentType
												&tangentType );
	///
	static MFnAnimCurve::TangentType globalOutTangentType (
												MStatus * ReturnStatus = NULL );
	///
	static MStatus		setGlobalOutTangentType (const MFnAnimCurve::TangentType
												 &tangentType );
	///
	static bool			weightedTangents( MStatus * ReturnStatus = NULL );
	///
	static MStatus		setWeightedTangents( bool weightState );

protected:
// No protected members

private:
// No private members

};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MAnimControl */
