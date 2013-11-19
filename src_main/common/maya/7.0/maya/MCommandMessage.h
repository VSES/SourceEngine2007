#ifndef _MCommandMessage
#define _MCommandMessage
//
//-
// ==========================================================================
// Copyright  (C)  Alias Systems,  a division  of  Silicon Graphics  Limited.
// All rights  reserved.  These coded  instructions,  statements and computer
// programs  contain  unpublished information  proprietary to  Alias Systems,
// a  division  of  Silicon  Graphics  Limited,  which  is  protected by  the
// Canadian  and  US federal copyright law and  may not be disclosed to third
// parties or  copied  or  duplicated,  in  whole  or in part,  without prior
// written  consent  of Alias Systems, a division of Silicon Graphics Limited
// ==========================================================================
//+
//
// CLASS:    MCommandMessage
//
// ****************************************************************************
//
// CLASS DESCRIPTION (MCommandMessage)
//
//  This class is used to register for a callback for when certain MEL command
//	related messages are generated.
//	
//	The add callback method returns an id which is used to remove the
//	callback.
//
//  To remove a callback use MMessage::removeCallback.
//
//	All callbacks that are registered by a plug-in must be removed by
//	that plug-in when it is unloaded.  Failure to do so will result in
//	a fatal error.
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MMessage.h>
#include <maya/MString.h>

// ****************************************************************************

// DECLARATIONS


// ****************************************************************************

// CLASS DECLARATION (MCommandMessage)

/**
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

/// Helper class to handle MEL output
class OPENMAYA_EXPORT MCommandMessage : public MMessage
{ 
public:
	///
	enum MessageType {
		/// Command history
		kHistory,
		/// General information
		kInfo,
		/// Warning message
		kWarning,
		/// Error message
		kError,
		/// Result from a command execution in the command window
		kResult,
		/// Stack trace
		kStackTrace
	};
	///
	static MCallbackId	addCommandCallback(
								void (*func)( const MString &, 
											  void* clientData ),
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	///
	static MCallbackId	addCommandOutputCallback(
								void (*func)( const MString & message,
											  MessageType messageType,
											  void* clientData ),
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );
private: 
	static const char* className();
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// ****************************************************************************

#endif /* __cplusplus */
#endif /* _MCommandMessage */
