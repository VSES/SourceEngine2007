//***************************************************************************************
//
// File supervisor: Softimage 3D Games & 3D Bridge team
//
// (c) Copyright 2001-2002 Avid Technology, Inc. . All rights reserved.
//
//***************************************************************************************

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

#ifndef _NURBSSURFACE_H
#define _NURBSSURFACE_H

#include "Geometry.h"


#ifndef _SL_FLOAT_ARRAY_PROXY_EXPORT_
#define _SL_FLOAT_ARRAY_PROXY_EXPORT_

//XSITEMPLATEEXP template class XSIEXPORT CSLArrayProxy<SI_Float, SI_Float, 1>;
//XSITEMPLATEEXP template class XSIEXPORT CSLArrayProxy<CSIBCVector4D, SI_Float, 4>;

#endif // _SL_FLOAT_ARRAY_PROXY_EXPORT_

class CSLNurbsProjection;
class CSLNurbsTrim;


//! Describes a single NURBS surface.
class XSIEXPORT CSLNurbsSurface
	: public CSLGeometry
{
public:
	/**
	* Proxy class that allows modifying a list of SI_Float like an array.
	*/
	typedef CSLArrayProxy<SI_Float, SI_Float, 1>		CSLFloatArray;

	/**
	* Proxy class that allows modifying a list of CSIBCVector4D objects like an array.
	*/
	typedef CSLArrayProxy<CSIBCVector4D, SI_Float, 4> 	CSLVector4DArray;

	/** 
	* The EParametrizationType enum contains possible types for knot parametrization
	*/
	enum EParametrizationType
	{
		SI_UNIFORM,			//!< Uniform parametrization
		SI_NON_UNIFORM,		//!< Non-uniform parametrization
		SI_CHORD_LENGTH,	//!< Chord length parametrization
		SI_CENTRIPETAL,		//!< Centripetal parametrization
	};

	/*! Constructor	
	* \param in_pScene Parent scene
	* \param in_pModel Parent model
	* \param in_pTemplate Refered template
	* \return an instance of CSLNurbsSurface
	*/
	CSLNurbsSurface(CSLScene* in_pScene, CSLModel *in_pModel, CdotXSITemplate* in_pTemplate);

	/*! Destructor
	*/
	virtual ~CSLNurbsSurface();

	/*! Validates the content of this NURBS surface and fixes it if necessary
	* \return SI_Error whether the fix has been successful
	*/
	virtual SI_Error		Fix();

	/*! Synchronizes this NURBS surface instance with its IO layer representation
	* \return SI_Error
	* \retval SI_SUCCESS this function always returns SI_SUCCESS
	*/
    virtual SI_Error Synchronize();

	/*! Get the template type
	* \return ETemplateType the template type of the NURBS surface
	*/
    virtual ETemplateType	Type() { return SI_NURBS_SURFACE; }

	/*! Return the degree of the surface in the U direction.
	* \return SI_Int	Degree value in U.
	*/
	SI_Int					GetUDegree();

	/*! Change the degree of the surface in the U direction.
	* \param	SI_Int	New degree value in U.
	* \return SI_Void
	*/
	SI_Void					SetUDegree(SI_Int in_nValue);

	/*! Return the degree of the surface in the V direction.
	* \return SI_Int	Degree value in V.
	*/
	SI_Int					GetVDegree();

	/*! Change the degree of the surface in the V direction.
	* \param	SI_Int	New degree value in V.
	* \return SI_Void
	*/
	SI_Void					SetVDegree(SI_Int in_nValue);

	/*! Return the closed flag of the surface in the U direction.
	* \return SI_Bool Closed flag in U.
	*/
	SI_Bool					GetUClosedFlag();

	/*! Change the closed flag of the surface in the U direction.
	* \param	SI_Bool New closed flag in U.
	* \return SI_Void
	*/
	SI_Void					SetUClosedFlag(SI_Bool in_nValue);

	/*! Return the closed flag of the surface in the V direction.
	* \return SI_Bool Closed flag in V.
	*/
	SI_Bool					GetVClosedFlag();

	/*! Change the closed flag of the surface in the V direction.
	* \param	SI_Bool New closed flag in V.
	* \return SI_Void
	*/
	SI_Void					SetVClosedFlag(SI_Bool in_nValue);

	/*! Return the parametrization type of the surface.
	* \return EParametrizationType	Parametrization type.
	*/
	EParametrizationType	GetParametrization();

	/*! Change the parametrization type of the surface.
	* \param	EParametrizationType	New parametrization type.
	* \return SI_Void
	*/
	SI_Void					SetParametrization(EParametrizationType in_Type);

	/*! Return the number of knots of the surface in the U direction.
	* \return SI_Int	Number of knots in U.
	*/
	SI_Int					GetUKnotCount();

	/*! Return the knot values array proxy in the U direction.
	* \return CSLFloatArray* Pointer to an array proxy of knots values in U.
	*/
	CSLFloatArray*			GetUKnotList();

	/*! Returns an array of SI_Float containing the knot values in the U direction.
	* \return SI_Float*		Array of knots values in U.
	*/
	SI_Float*				GetUKnotListPtr();

	/*! Return the number of knots of the surface in the V direction.
	* \return SI_Int	Number of knots in V.
	*/
	SI_Int					GetVKnotCount();

	/*! Return the knot values array proxy in the V direction.
	* \return CSLFloatArray* Pointer to an array proxy of knots values in V.
	*/
	CSLFloatArray*			GetVKnotList();

	/*! Returns an array of SI_Float containing the knot values in the V direction.
	* \return SI_Float*		Array of knots values in V.
	*/
	SI_Float*				GetVKnotListPtr();

	/*! Return the number of control points of the surface in the U direction.
	* \return SI_Int	Number of control points in U.
	*/
	SI_Int					GetUControlPointCount();

	/*! Change the number of control points of the surface in the U direction.
	* \param	EParametrizationType	New number of control points in U.
	* \return SI_Void.
	*/
	SI_Void					SetUControlPointCount(SI_Int in_nValue);

	/*! Return the number of control points of the surface in the V direction.
	* \return SI_Int	Number of control points in V.
	*/
	SI_Int					GetVControlPointCount();

	/*! Change the number of control points of the surface in the V direction.
	* \param	EParametrizationType	New number of control points in V.
	* \return SI_Void.
	*/
	SI_Void					SetVControlPointCount(SI_Int in_nValue);

	/*! Return the total number of control points of the surface.
	* \return SI_Int	Number of knots.
	*/
	SI_Int					GetControlPointCount();

	/*! Return the control point values array proxy.
	* \return CSLVector4DArray*	Pointer to an array proxy of indexed normals.
	*/
	CSLVector4DArray*		GetControlPointList();

	/*! Returns an array of SI_Float containing the control point values.
	* \return SI_Float*		Array of control point values.
	*/
	CSIBCVector4D*			GetControlPointListPtr();

	///////////////////////////////////////////////////////////////////////////
	// Nurbs projection functionnality ////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	/*! Adds an nurbs projection to the nurbs surface
	* \return CSLNurbsProjection Pointer to the newly added nurbs projection
	* \note The scene must support 3.6 to add a nurbs projection
	*/
	CSLNurbsProjection* AddNurbsProjection();

	/*! Removes a nurbs projection from the nurbs surface
	* \param in_nIndex Index of the nurbs projection to remove
	* \return SI_Error Whether or not the nurbs projection could be removed
	*/
	SI_Error RemoveNurbsProjection( SI_Int in_nIndex );

	/*! Removes an nurbs projection from the nurbs surface
	* \param in_pToRemove Pointer to the nurbs projection to remove
	* \return SI_Error Whether or not the nurbs projection could be removed
	*/
	SI_Error RemoveNurbsProjection( CSLNurbsProjection* in_pToRemove );

	/*! Removes all nurbs projections from the nurbs surface
	* \return SI_Error Whether or not the list could be cleared
	*/
	SI_Error ClearNurbsProjections();

	/*! Adds an exisiting nurbs projection to the nurbs surface
	* \param in_pToConnect Pointer to the nurbs projection to add
	* \return The added nurbs projection or null if it already exists in the list
	*/
	CSLNurbsProjection* ConnectNurbsProjection( CSLNurbsProjection* in_pToConnect );

	/*! Gets an array of all nurbs projection contained on this nurbs surface
	* \return CSLNurbsProjection Pointer to the list
	* \note The scene must support 3.6 to add a nurbs projection
	*/
	CSLNurbsProjection** GetNurbsProjections();

	/*! Gets the total number of nurbs projections
	* \return SI_Int Number of nurbs projections
	*/
	SI_Int GetNurbsProjectionCount();

	/*! Searches for a given nurbs projection
	* \param in_szNurbsProjectionName Name of the nurbs projection to look for
	* \return CSLNurbsProjection Pointer to the nurbs projection or null if it cannot be found
	*/
	CSLNurbsProjection* FindNurbsProjection( SI_Char* in_szNurbsProjectionName );

	///////////////////////////////////////////////////////////////////////////
	// Nurbs trim functionnality //////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	/*! Adds an nurbs trim to the nurbs surface
	* \return CSLNurbsTrimList Pointer to the newly added nurbs trim
	*/
	CSLNurbsTrim* AddNurbsTrim();

	/*! Removes a nurbs trim from the nurbs surface
	* \param in_nIndex Index of the nurbs trim to remove
	* \return SI_Error Whether or not the nurbs trim could be removed
	* \note The scene must support 3.6 to add a nurbs trim
	*/
	SI_Error RemoveNurbsTrim( SI_Int in_nIndex );

	/*! Removes an nurbs trim from the nurbs surface
	* \param in_pToRemove Pointer to the nurbs trim to remove
	* \return SI_Error Whether or not the nurbs trim could be removed
	*/
	SI_Error RemoveNurbsTrim( CSLNurbsTrim* in_pToRemove );

	/*! Removes all nurbs trims from the nurbs surface
	* \return SI_Error Whether or not the list could be cleared
	*/
	SI_Error ClearNurbsTrims();

	/*! Adds an exisiting nurbs trim to the nurbs surface
	* \param in_pToConnect Pointer to the nurbs trim to add
	* \return The added nurbs trim or null if it already exists in the list
	*/
	CSLNurbsTrim* ConnectNurbsTrim( CSLNurbsTrim* in_pToConnect );

	/*! Gets an array of all nurbs trims contained on this nurbs surface
	* \return CSLNurbsTrim Pointer to the list
	* \note The scene must support 3.6 to add a nurbs trim
	*/
	CSLNurbsTrim** GetNurbsTrims();

	/*! Gets the total number of nurbs trims
	* \return SI_Int Number of nurbs trims
	*/
	SI_Int GetNurbsTrimCount();

	/*! Searches for a given nurbs trim
	* \param in_szNurbsTrimName Name of the nurbs trim to look for
	* \return CSLNurbsTrim Pointer to the nurbs trim or null if it cannot be found
	*/
	CSLNurbsTrim* FindNurbsTrim( SI_Char* in_szNurbsTrimName );

private:
	CSLIntProxy 			m_UDegree;
	CSLIntProxy 			m_VDegree;
	CSLBoolProxy			m_UClosed;
	CSLBoolProxy			m_VClosed;
	CSLEnumProxy<EParametrizationType, SI_CENTRIPETAL>	m_Parametrization;
	CSLIntProxy 			m_NbUControlPoints;
	CSLIntProxy 			m_NbVControlPoints;
	CSLFloatArray			m_UKnots;
	CSLFloatArray			m_VKnots;
	CSLVector4DArray		m_ControlPoints;
	CSIBCArray<CSLNurbsProjection*> m_NurbsProjections;
	CSIBCArray<CSLNurbsTrim*> m_NurbsTrims;

	void *m_pReserved;	// reserved for future extension
};

#endif
