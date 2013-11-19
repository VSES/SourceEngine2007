/****************************************************************************************
THIS CODE IS PUBLISHED AS A SAMPLE ONLY AND IS PROVIDED "AS IS".
IN NO EVENT SHALL SOFTIMAGE, AVID TECHNOLOGY, INC. AND/OR THEIR RESPECTIVE 
SUPPLIERS BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY
DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
CONNECTION WITH THE USE OR PERFORMANCE OF THIS CODE . 
 
COPYRIGHT NOTICE. Copyright © 1999-2002 Avid Technology Inc. . All rights reserved. 

SOFTIMAGE is a registered trademark of Avid Technology Inc. or its subsidiaries 
or divisions. Windows NT is a registered trademark of Microsoft Corp. All other
trademarks contained herein are the property of their respective owners. 
****************************************************************************************/

#ifndef _SITIME_H_
#define _SITIME_H_

class CSLSceneInfo;
#include <SIBCArray.h>

#ifdef UNIX
#include <sys/time.h>
#endif

/**
* The SI_ModelTime class hold local frame information for a specific model. This is necessary since the XSIViewer Engine can
* handle multiple scenes at once each having their own start/end time and their own playback speed.
*/
class XSIEXPORT SI_ModelTime
{
private:
	CSLSceneInfo	*sceneInfo;

	// fundamental state of this structure
	SI_Float		currentTime;						// looping: 0<= currentTime < loopPeriod; otherwise: 0 <= currentTime <= singlePlayPeriod
	
	// calculated from sceneInfo in UpdateFrame and used in methods
	SI_Float		secondsPerFrame;					// 1/frameRate
	SI_Float		singlePlayPeriod;					// single play period
	SI_Float		loopPeriod;							// time for one loop
	SI_Float		externalCurrentFrame;

public:
	//***********************************************************************
	// Constructor/destructor
	//***********************************************************************
	//! constructor for SI_ModelTime
	/*! 
	\return A pointer to the new SI_ModelTime
	*/ 
	SI_ModelTime();

	//! destructor for SI_ModelTime
	/*! 
	\return nothing
	*/ 
	~SI_ModelTime();

	//! Returns the SemanticLayer object SceneInfo driving this ModelTime
	/*! 
	\return CSLSceneInfo*	Semantic Layer object that wraps the SI_Scene template
	\sa CSLSceneInfo
	*/ 
	CSLSceneInfo*	GetSceneInfo() { return sceneInfo; };

	//! Sets the Semantic Layer Scene Info object that will drive this ModelTime
	/*! 
	\param	CSLSceneInfo*	The SceneInfo Object
	\return Nothing
	\sa CSLSceneInfo
	*/ 
	void Set( CSLSceneInfo *sceneInfo );

	//! Update the ModelTime with a time delta (forward or backward).
	/*! 
	\param	SI_Float	Time increment (in milliseconds)
	\param	SI_Bool		Interpolate time between frames, or snap to frames
	\param	SI_Bool		Loop animation
	\return Nothing
	*/ 
	void UpdateFrame( SI_Float timeDelta, SI_Bool subFrameInterpolate, SI_Bool loop );

	//! Reset the ModelTime to it's start time
	/*! 
	\return Nothing
	*/ 
	void GoToStartFrame()
	{
		currentTime = 0;
	}

	//! Position the ModelTime to it's last frame
	/*! 
	\return Nothing
	*/ 
	void GoToEndFrame()
	{
		currentTime = singlePlayPeriod;
	}

	//! Position the ModelTime to a specific frame
	/*! 
	\param	SI_Float	Frame (in seconds)
	\return Nothing
	*/ 
	void GoToFrame( SI_Float in_fFrame )
	{
		currentTime = in_fFrame;
	}

	//! Get the current frame
	/*! 
	\return SI_Float	The ModelTime's current frame
	*/ 
	SI_Float GetFrame() const
	{
		return externalCurrentFrame;
	}

	//! Get the number of seconds needed for each frame
	/*! 
	\return SI_Float	The number of milliseconds needed to reach a new frame in this ModelTime
	*/ 
	SI_Float GetSecondsPerFrame() const
	{
		return secondsPerFrame;
	}
};

/**
* The SI_Time class calculate time for each SI_ModelTime in the scene. This class should be used to manipulate the animation
* in the Engine.
*
* \sa CSIBaseApplication::GetTimeInfo
*
* <B> Example 1 - Finding out the start and end frame of the current sequence </B>
\code
void GetAnimationLenght ( CSIViewerApplicationWin32* in_pApp, SI_Float &inout_fStart, SI_Float &inout_fEnd )
{
	inout_fStart = 1.0f;
	inout_fEnd = 100.0f;	// default to 100 frames

	//
	// Get the global SI_Time class
	//

	SI_Time *l_pTimeInfo = in_pApp->GetTimeInfo();

	//
	// Get the sequence SI_ModelTime
	//

	SI_ModelTime*	l_pModelTime = l_pTimeInfo->GetLastModelTime();

	//
	// Do we have a sequence loaded?
	//

	if ( l_pModelTime )
	{
		//
		// Get it's SceneInfo object
		//

		CSLSceneInfo*	l_pSceneInfo = l_pModelTime->GetSceneInfo();
		inout_fStart = l_pSceneInfo->GetStart();
		inout_fEnd = l_pSceneInfo->GetEnd();
	}

}
\endcode
*
* <B> Example 2 - Setting the current frame </B>
\code
void GotoFrame ( CSIViewerApplicationWin32* in_pApp, SI_Float in_fFrame )
{
	//
	// Get the global SI_Time class
	//

	SI_Time *l_pTimeInfo = in_pApp->GetTimeInfo();

	//
	// Get the sequence SI_ModelTime
	//

	SI_ModelTime*	l_pModelTime = l_pTimeInfo->GetLastModelTime();

	//
	// Do we have a sequence loaded?
	//

	if ( l_pModelTime )
	{
		//
		// Set it's new current frame
		//
		
		l_pModelTime->GoToFrame ( in_fFrame * l_pModelTime->GetSecondsPerFrame() );
	}
}
\endcode
*/
class XSIEXPORT SI_Time
{
public:
// scene playback information
	SI_Bool		subFrameInterpolate;
	SI_Bool		direction;
	SI_Bool		playing;
	SI_Bool		loop;
	SI_Bool		useFrameRate;
	
private:
	SI_Float	FPSPeriod;				// period of frame rate recalculation
	SI_Float	FPS;
	SI_Float	FPSElapsed;
	SI_Float	FPSFrames;				// using float type to avoid conversions
	SI_Float	minSecondsPerFrame;		// smallest time per frame of all loaded models

	CSIBCArray< SI_ModelTime* >		modelTimes;

#ifdef UNIX

    struct timeval	m_tvTimeCurrent;	//!< Holds the current time.
    struct timeval	m_tvTimeLast;		//!< Holds the last current time.
    struct timezone	m_tzEmpty;		    //!< Holds the timezone information (obsolete).

#endif

public:
	SI_Time( SI_Float frp );

	virtual ~SI_Time();
	
	//! Advance the animation to the next frame
	/*! 
	\return Nothing
	*/ 
	void NextFrame();

	//! Resets the playback to the first frame found in the last loaded file
	/*!
	\return success or failure code
	*/
	void ResetPlayback();

	//! start playback
	/*!
	\return success or failure code
	*/
	void StartPlayback();

	//! pause playback
	/*!
	\return success or failure code
	*/
	void PausePlayback();

	//! stop playback
	/*!
	\return success or failure code
	*/
	void StopPlayback();

	//! Advance the animation a specific number of frames
	/*! 
	\return Nothing
	*/ 
	void AdvancePlayback( SI_Float numFrames );

	//! Get the frames per second drawn by the Engine
	/*! 
	\return SI_Float	Frame per seconds
	*/ 
	SI_Float GetFPS() const
	{
		return FPS;
	}
	
	//! returns the status of the playhead
	/*!
	\return TRUE if playing, FALSE if not
	*/
	SI_Bool IsPlaying()
	{
		return playing;
	}

	void AddModelTime();

	void SetModelTime( CSLSceneInfo *si );

	void ClearModelTimes();

	/*! Deletes the last model time object in the array. This function is used when
		a scene is created, a model time is added for that scene, but the scene produces
		an error and cannot be loaded. The corresponding model time (the last to be created)
		needs to be deleted.
	*/
	SI_Void	ClearLastModelTime();

	SI_ModelTime *GetLastModelTime() const;

protected:
	virtual void ResetTimer() = 0;			// resets timer

	virtual SI_Float GetElapsedTime() = 0;		// returns elapsed time since last reset (or construction) in seconds

};

#endif
