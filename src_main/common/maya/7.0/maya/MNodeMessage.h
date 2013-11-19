
#ifndef _MNodeMessage
#define _MNodeMessage
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
// CLASS:    MNodeMessage
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MNodeMessage)
//
// This class is used to register callbacks for dependency node messages
// of specific dependency nodes.
// 
// There are 4 add callback methods which will add callbacks for the
// following messages
// 	   Attribute Changed
// 	   Attribute Added or Removed
// 	   Node Dirty
// 	   Name Changed
// 
// The first parameter passed to each of the add callback methods is the
// depenency node that will trigger the callback.
// 
// Callbacks that are registered for attribute changed/addedOrRemoved messages
// will be passed an AttributeMessage value as a parameter. This value
// indicates the type of attribute message that has occurred. See the 
// enum for all available messages.
//            
// Each method returns an id which is used to remove the callback.
// 
// To remove a callback use MMessage::removeCallback.
// All callbacks that are registered by a plug-in must be removed by that
// plug-in when it is unloaded. Failure to do so will result in a fatal error.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MMessage.h>

// *****************************************************************************

// DECLARATIONS

class MPlug;
class MObject;
class MDGModifier;

// *****************************************************************************

// CLASS DECLARATION (MNodeMessage)

/// Dependency node messages 
/**
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MNodeMessage : public MMessage
{ 
public:
	/// The type of attribute changed/addedOrRemoved messages that has occurred
	enum AttributeMessage {
		/// a connection has been made to an attribute of this node
		kConnectionMade			= 0x01,
		/// a connection has been broken for an attribute of this node
		kConnectionBroken		= 0x02,
		/// an attribute of this node has been evaluated
		kAttributeEval			= 0x04,
		/// an attribute value of this node has been set
		kAttributeSet			= 0x08,
		/// an attribute of this node has been locked 
		kAttributeLocked		= 0x10,
		/// an attribute of this node has been unlocked
		kAttributeUnlocked 		= 0x20,
		/// an attribute has been added to this node
		kAttributeAdded			= 0x40,
		/// an attribute has been removed from this node
		kAttributeRemoved		= 0x80,
		/// an attribute of this node has been renamed
		kAttributeRenamed		= 0x100,
		/// and attribute of this node has been marked keyable
		kAttributeKeyable		= 0x200,
		/// and attribute of this node has been marked unkeyable
		kAttributeUnkeyable		= 0x400,
		/// the connection was coming into the node
		kIncomingDirection		= 0x800,
		/// an array attribute has been added to this node
		kAttributeArrayAdded	= 0x1000,
		/// an array attribute has been removed from this node
		kAttributeArrayRemoved	= 0x2000,
		/// the otherPlug data has been set
		kOtherPlugSet			= 0x4000,
		/// last value of the enum 
		kLast					= 0x8000
	};

public:
	///
	static MCallbackId	addAttributeChangedCallback(
								MObject& node,
								void (*func)( AttributeMessage msg,
											  MPlug & plug,
											  MPlug & otherPlug,
											  void* clientData ),
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );
								
	///
	static MCallbackId	addAttributeAddedOrRemovedCallback(
								MObject& node,
								void (*func)( AttributeMessage msg,
											  MPlug & plug,
											  void* clientData ),
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );
	
	///
	static MCallbackId	addNodeDirtyCallback(
								MObject& node,
								void (*func)( void* clientData ),
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );
	
	///
	static MCallbackId	addNodeDirtyCallback(
								MObject& node,
								void (*func)( MObject& node, 
											  void* clientData ),
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	///
	static MCallbackId	addNodeDirtyCallback(
								MObject& node,
								void (*func)( MObject& node, 
											  MPlug& plug,
											  void* clientData ),
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	///
	static MCallbackId	addNameChangedCallback(
								MObject& node,
								void (*func)( MObject & node,
											  void* clientData ),
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	///
	static MCallbackId	addNameChangedCallback(
								MObject& node,
								void (*func)( MObject & node,
											  const MString & prevName,
											  void* clientData ),
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	//	Obsolete
	///
	static MCallbackId	addNodeAboutToDeleteCallback(
								MObject& node,
								void (*func)( MDGModifier& modifier,
											  void* clientData ),
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	///
	static MCallbackId	addNodeAboutToDeleteCallback(
								MObject& node,
								void (*func)( MObject &node,
											  MDGModifier& modifier,
											  void* clientData ),
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	///
	static MCallbackId	addNodePreRemovalCallback(
								MObject& node,
								void (*func)( MObject &node,
											  void* clientData ),
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );
		
	///	NodeDestroyed callback
	static MCallbackId	addNodeDestroyedCallback(
								MObject& node,
								void (*func)( void* clientData ),
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );
private: 
	static const char* className();
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************

#endif /* __cplusplus */
#endif /* _MNodeMessage */
