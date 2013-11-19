//*****************************************************************************
/*!
   \file xsi_graphicsequencer.h
   \brief CGraphicSequencer class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIXSIGRAPHICSEQUENCER_H__
#define __XSIXSIGRAPHICSEQUENCER_H__

#include <xsi_decl.h>
#include <xsi_status.h>
#include <xsi_ref.h>
#include <xsi_material.h>

namespace XSI {

//*****************************************************************************
/*! \class CGraphicSequencer xsi_graphicsequencer.h
	\brief The CGraphicSequencer object offers several services to enable custom rendering passes in the OpenGL viewport.

	You cannot instantiate a CGraphicSequencer object directly. Rather, you receive an instance when your XGS plug-ins
	are called; that is, in a Custom Display callback or a Custom OpenGL %Pass.

	\sa GraphicSequencerContext, ::siViewMode, ::siXGSRenderFlag
	\since 4.0
 */
//*****************************************************************************

class SICPPSDKDECL CGraphicSequencer
{
public:
	/*! Default constructor. */
	CGraphicSequencer();

	/*! Default destructor. */
	~CGraphicSequencer();

	/*! Registers a new Display callback with the OGS.
	\param in_szName				The display callback name
	\param in_lStateFlags			The callback state flags
	\param in_eType					The type of callback (see ::siDisplayCallback)
	\param in_eViewMode				The view mode (see ::siViewMode)
	\param in_szCustomViewModeName	The name to use for the new custom view mode
	\return CStatus::OK if the new display callback was registered
	\return CStatus::Fail if the registration failed
	*/
	CStatus RegisterDisplayCallback  (	const CString&			in_szName,
										const LONG				in_lStateFlags,
										const siDisplayCallback in_eType,
										const siViewMode		in_eViewMode,
										const CString&			in_szCustomViewModeName) const;

	/*! Registers a new %Pass callback with the OGS.
	\warning Not Implemented. Use CGraphicSequencer::RegisterDisplayCallback with the in_eType parameter set to ::siPass instead.
	\param in_szPassName			The pass callback name
	\param in_lStateFlags			The callback state flags
	\param in_lPassFlags			The pass callback flags
	\param in_szRelativePassName	The name of the pass used for relative insertion.
	\return CStatus::OK if the new pass callback was registered
	\return CStatus::Fail if the registration failed
	*/
	CStatus RegisterPassCallback  	(	const CString&			in_szPassName,
										const LONG				in_lStateFlags,
										const LONG 				in_lPassFlags,
										const CString&			in_szRelativePassName ) const;

	/*! Renders the scene using the specified legacy mode.
	\param in_eViewMode		The legacy mode to use
	\param in_lRenderFlags	The render flags to use (one of the ::siXGSRenderFlag values)
	\return CStatus::OK if the scene was successfully rendered
	\return CStatus::Fail if the registration failed or if the mode was not found
	\sa ::siXGSRenderFlag
	*/
	CStatus RenderSceneUsingMode		( const siViewMode in_eViewMode, INT in_lRenderFlags) const;

	/*! Renders the scene using the specified Shader material.
	\param in_szShaderName	The name of the Shader to use for rendering
	\param in_lRenderFlags	The render flags to use (one of the ::siXGSRenderFlag values)
	\return CStatus::OK if the scene was successfully rendered
	\return CStatus::Fail if the registration failed or if the material was invalid
	\sa ::siXGSRenderFlag
	*/
	CStatus RenderSceneUsingMaterial	( const CString&	in_szShaderName, INT in_lRenderFlags ) const;

	/*! Renders a list of objects using the specified legacy mode.
	\param in_pObjectList	The list of objects to draw (array of references to objects)
	\param in_eViewMode		The legacy mode to use
	\return CStatus::OK if the list was successfully rendered
	\return CStatus::Fail if the registration failed or if the mode was not found
	*/
	CStatus RenderListUsingMode		( const CRefArray&	in_pObjectList,
										  const siViewMode in_eViewMode ) const;

	/*! Renders a list of objects using a specific Shader material.
	\param in_pObjectList	The list of objects to draw (array of references to objects)
	\param in_szShaderName	The name of the Shader to use for rendering
	\return CStatus::OK if the list was successfully rendered
	\return CStatus::Fail if the registration failed or if the mode was not found
	*/
	CStatus RenderListUsingMaterial		( const CRefArray&	in_pObjectList,
										  const CString&	in_szShaderName ) const;

	/*! Returns the number of states matching the specified type.
	\param in_eType			The state type to count (one of the ::siXGSStateType values)
	\retval out_pNbStates	Pointer to an integer holding the number of states
	\return CStatus::OK on success
	\return CStatus::Fail if the arguments are invalid
	*/
	CStatus GetNbStates		( siXGSStateType in_eType, INT *out_pNbStates) const;

	/*! Deletes a state by its identification (CString).
	\param in_eType	The state type to delete (one of the ::siXGSStateType values)
	\param in_pID 	String identifying the state
	\return CStatus::OK on success
	\return CStatus::Fail if the state was not found
	*/
	CStatus DeleteState		( siXGSStateType in_eType, const CString&	in_pID ) const;

	/*! Creates a new state.
	\param in_eType				The state type to create (one of the ::siXGSStateType values)
	\param in_pID 				String identifying the new state
	\param in_pValuePtr			The state value
	\retval out_pStateHandle	The state handle
	\return CStatus::OK on success
	\return CStatus::Fail if the arguments are invalid
	*/
	CStatus CreateState		( siXGSStateType in_eType, const CString&	in_pID, void* in_pValuePtr, void** out_pStateHandle) const;

	/*! Returns the state handle by its identification (CString).
	\param in_eType				The state type to get (one of the ::siXGSStateType values)
	\param in_pID 				String identifying the state
	\retval out_pStateHandle	The state handle
	\return CStatus::OK on success
	\return CStatus::Fail if the arguments are invalid
	*/
	CStatus GetStateHandle	( siXGSStateType in_eType, const CString&	in_pID, void** out_pStateHandle) const;

	/*! Returns the specified state's value.
	\param in_eType			The state type to get (one of the ::siXGSStateType values)
	\param in_lStateHandle	The handle to the state
	\retval out_pStateValue	Pointer to the state value
	\return CStatus::OK on success
	\return CStatus::Fail if the arguments are invalid
	*/
	CStatus GetStateValue	( siXGSStateType in_eType, void* in_lStateHandle, void* out_pStateValue) const;

	/*! Sets the specified state's value.
	\param in_eType			The state type to set (one of the ::siXGSStateType values)
	\param in_lStateHandle	The handle to the state
	\param in_pStateValue	Pointer to the state value
	\return CStatus::OK on success
	\return CStatus::Fail if the arguments are invalid
	*/
	CStatus SetStateValue	( siXGSStateType in_eType, void* in_lStateHandle, void* in_pStateValue) const;

	/*! Indicates whether the scene has changed since the last frame.
	\retval out_bBool	True if the scene has changed, false otherwise
	\return CStatus::OK on success
	\return CStatus::Fail if the arguments are invalid
	*/
	CStatus GetSceneDirtyFlag	( bool* out_bBool ) const;

	/*! Returns a flag that indicates the context in which the rendering is occuring.
	\retval out_lFlags	The context flags. These are bitwise flags of ::siXGSContextFlag values
	\return CStatus::OK on success
	\return CStatus::Fail if the arguments are invalid
    \sa ::siXGSContextFlag
	\since 4.2
	*/
	CStatus GetContextFlags	( LONG* out_lFlags ) const;



private:

	CGraphicSequencer * operator&() const;
	CGraphicSequencer * operator&();

	void*	m_pImpl;
};

}

#endif // __XSIXSIGRAPHICSEQUENCER_H__
