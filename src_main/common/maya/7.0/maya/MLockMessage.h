#ifndef _MLockMessage
#define _MLockMessage
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
// CLASS:    MLockMessage
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MLockMessage)
//
//	This class is used to register callbacks for conditional node and
//	plug locking. These callbacks allow you to intercept events that
//	involve the internal testing of the lock flag on a node or
//	plug. If the node, or plug, is locked the assigned API callback
//	routine, if attached to the node, will be invoked, and, at this
//	time, you have the option to override the lock. This allows the
//	API programmer the ability to ignore a lock and allow a lock event
//	to occur. This allows the API programmer to adjust the level of
//	granularity on the locking mechanism. There are three levels of
//	locking callbacks.
//
//		\begin{itemize}
//		\item {\bf DAG Level Events}
//		\item {\bf All Other Node Level Events}
//      \item {\bf Plug Level Events} 
//		\end{itemize}
//
// To remove a callback use MMessage::removeCallback.  All callbacks
// that are registered by a plug-in must be removed by that plug-in
// when it is unloaded. Failure to do so will result in a fatal error.
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
class MDagPath; 

// *****************************************************************************

// CLASS DECLARATION (MLockMessage)

/// Node/plug locking messages.
/**
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MLockMessage : public MMessage
{ 
public:
	/// Plug related locking events 
	enum LockPlugEvent { 
		/// An invalid plug event received. 
		kInvalidPlug = 0,
		/// User is trying to lock the plug. 
		kPlugLockAttr,
		/// User is trying to unlock the plug. 
		kPlugUnlockAttr,			
		/// Plug lock query on plug value change.
		kPlugAttrValChange,			
		/// Plug lock query on a remove attribute event. 
		kPlugRemoveAttr,
		/// Plug lock query on a rename attribute event. 
		kPlugRenameAttr,
		/// Plug lock query on a connect to a locked attribute.
		kPlugConnect, 
		/// Plug lock query on a disconnect from a locked source attribute.
		kPlugDisconnect, 
		/// Used for internal bounds checking. 
		kLastPlug
	}; 

	/// DAG related locking events 
	enum LockDAGEvent { 
		/// Invalid DAG event received. 
		kInvalidDAG = 0,
		/// Lock query on group action. 	
		kGroup,
		/// Lock query on ungroup action. 
		kUnGroup,
   		/// Lock query on reparent action. 
		kReparent,
		/// Lock query on a child add action. 
		kChildReorder,
		/// Lock query on an instance event.
		kCreateNodeInstance,
    	/// Lock query on node whose children will be instanced.  
		kCreateChildInstance,
		/// Lock query on node whose parent will be instanced. 
		kCreateParentInstance,
		/// Used for internal bounds checking. 
		kLastDAG
	}; 
	
	/// Miscellaneous lock event types
	enum LockEvent { 
		/// Invalid node lock event received.  
		kInvalid = 0,
		/// Lock query on a rename event. 
		kRename,
		/// Lock query on a delete event. 
		kDelete,
		/// Lock query on a lock event. 
		kLockNode,
		/// Lock query on an unlock event. 
		kUnlockNode,
		/// Lock query on an add attribute event. 
		kAddAttr,
		/// Lock query on an remove attribute event. 
		kRemoveAttr,
		/// Lock query on an rename attribute event. 
		kRenameAttr,
		/// Lock query on an unlock attribute event. 
		kUnlockAttr, 
		/// Lock query on a lock attribute event. 
		kLockAttr,
		/// Used for internal bounds checking.  
		kLast
	}; 

public:
	///
	static MCallbackId  setNodeLockQueryCallback( 
								MObject& node, 
								void (*func)( MObject &node, 
											  MObject &aux,
											  void *clientData, 
											  LockEvent eventType, 
											  bool &decision ), 
								void *clientData = NULL, 
								MStatus *status = NULL );  
	///
	static MCallbackId  setNodeLockDAGQueryCallback( 
								MDagPath& path, 
								void (*func)( MDagPath &dagPath, 
											  MDagPath &otherPath, 
											  void *clientData, 
											  LockDAGEvent eventType, 
											  bool &decision ), 
								void *clientData = NULL, 
								MStatus *status = NULL ); 
	/// 
	static MCallbackId  setPlugLockQueryCallback( 
								MPlug& plug, 
								void (*func)( MPlug &plug, 
											  MPlug &otherPlug, 
											  void *clientData, 
											  LockPlugEvent eventType, 
											  bool &decision ), 
								void *clientData = NULL, 
								MStatus *status = NULL ); 
	///
	static MCallbackId  setPlugLockQueryCallback( 
								MObject& node, 
								void (*func)( MPlug &plug, 
											  MPlug &otherPlug, 
											  void *clientData, 
											  LockPlugEvent eventType, 
											  bool &decision ), 
								void *clientData = NULL, 
								MStatus *status = NULL ); 

protected: 
	// No protected. 

private: 
	static const char* className();
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************

#endif /* __cplusplus */
#endif /* _MLockMessage */
