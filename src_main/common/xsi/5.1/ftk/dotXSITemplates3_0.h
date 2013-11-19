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


#ifndef	DOTXSI_30_DEFINED
#define	DOTXSI_30_DEFINED
#include "dotXSITemplate.h"

/*! \addtogroup regfunc_30 Registration functions for dotXSI 3.0
*  @{
*/

/*! SI_Ambience template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the SI_Ambience template
*/
CdotXSITemplate*	Register_SI_Ambience();

/*! SI_Angle template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the SI_Angle template
*/
CdotXSITemplate*	Register_SI_Angle();

/*! SI_Camera template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the SI_Camera template
*/
CdotXSITemplate*	Register_SI_Camera();

/*! SI_Cluster template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the SI_Cluster template
*/
CdotXSITemplate*	Register_SI_Cluster();

/*! SI_Constraint template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the SI_Constraint template
*/
CdotXSITemplate*	Register_SI_Constraint();

/*! SI_CoordinateSystem template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the SI_CoordinateSystem template
*/
CdotXSITemplate*	Register_SI_CoordinateSystem();

/*! XSI_CurveList template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the XSI_CurveList template
*/
CdotXSITemplate*	Register_XSI_CurveList();

/*! SI_Envelope template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the SI_Envelope template
*/
CdotXSITemplate*	Register_SI_Envelope();

/*! SI_EnvelopeList template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the SI_EnvelopeList template
*/
CdotXSITemplate*	Register_SI_EnvelopeList();

/*! SI_FileInfo template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the SI_FileInfo template
*/
CdotXSITemplate*	Register_SI_FileInfo();

/*! SI_FCurve template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the SI_FCurve template
*/
CdotXSITemplate*	Register_SI_FCurve();

/*! SI_Fog template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the SI_Fog template
*/
CdotXSITemplate*	Register_SI_Fog();

/*! SI_FrameBasePoseMatrix template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the SI_FrameBasePoseMatrix template
*/
CdotXSITemplate*	Register_SI_FrameBasePoseMatrix();

/*! FrameTransformMatrix template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the FrameTransformMatrix template
*/
CdotXSITemplate*	Register_FrameTransformMatrix();

/*! SI_GlobalMaterial template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the SI_GlobalMaterial template
*/
CdotXSITemplate*	Register_SI_GlobalMaterial();

/*! SI_IK_Effector template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the SI_IK_Effector template
*/
CdotXSITemplate*	Register_SI_IK_Effector();

/*! SI_IK_Joint template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the SI_IK_Joint template
*/
CdotXSITemplate*	Register_SI_IK_Joint();

/*! SI_IK_Root template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the SI_IK_Root template
*/
CdotXSITemplate*	Register_SI_IK_Root();

/*! SI_ImageClip template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the SI_ImageClip template
*/
CdotXSITemplate*	Register_SI_ImageClip();

/*! SI_Instance template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the SI_Instance template
*/
CdotXSITemplate*	Register_SI_Instance();

/*! SI_Light template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the SI_Light template
*/
CdotXSITemplate*	Register_SI_Light();

/*! SI_Material template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the SI_Material template
*/
CdotXSITemplate*	Register_SI_Material();

/*! SI_MaterialLibrary template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the SI_MaterialLibrary template
*/
CdotXSITemplate*	Register_SI_MaterialLibrary();

/*! SI_Mesh template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the SI_Mesh template
*/
CdotXSITemplate*	Register_SI_Mesh();

/*! SI_Model template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the SI_Model template
*/
CdotXSITemplate*	Register_SI_Model();

/*! Frame template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the Frame template
*/
CdotXSITemplate*	Register_Frame();

/*! SI_Null template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the SI_Null template
*/
CdotXSITemplate*	Register_SI_Null();

/*! SI_NurbsSurface template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the SI_NurbsSurface template
*/
CdotXSITemplate*	Register_SI_NurbsSurface();

/*! SI_NurbsCurve template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the SI_NurbsCurve template
*/
CdotXSITemplate*	Register_SI_NurbsCurve();

/*! SI_PatchSurface template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the SI_PatchSurface template
*/
CdotXSITemplate*	Register_SI_PatchSurface();

/*! SI_PolygonList template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the SI_PolygonList template
*/
CdotXSITemplate*	Register_SI_PolygonList();

/*! SI_Scene template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the SI_Scene template
*/
CdotXSITemplate*	Register_SI_Scene();

/*! SI_Shape template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the SI_Shape template
*/
CdotXSITemplate*	Register_SI_Shape();

/*! SI_ShapeAnimation template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the SI_ShapeAnimation template
*/
CdotXSITemplate*	Register_SI_ShapeAnimation();

/*! XSI_SurfaceMesh template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the XSI_SurfaceMesh template
*/
CdotXSITemplate*	Register_XSI_SurfaceMesh();

/*! SI_Texture2D template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the SI_Texture2D template
*/
CdotXSITemplate*	Register_SI_Texture2D();

/*! SI_Transform template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the SI_Transform template
*/
CdotXSITemplate*	Register_SI_Transform();

/*! SI_TriangleList template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the SI_TriangleList template
*/
CdotXSITemplate*	Register_SI_TriangleList();

/*! SI_TriStrip template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the SI_TriStrip template
*/
CdotXSITemplate*	Register_SI_TriStrip();

/*! SI_TriStripList template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the SI_TriStripList template
*/
CdotXSITemplate*	Register_SI_TriStripList();

/*! SI_VertexWeight template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the SI_VertexWeight template
*/
CdotXSITemplate*	Register_SI_VertexWeight();

/*! SI_Visibility template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the SI_Visibility template
*/
CdotXSITemplate*	Register_SI_Visibility();

/*! XSI_CustomPSet template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the XSI_CustomPSet template
*/
CdotXSITemplate*	Register_XSI_CustomPSet();

/*! XSI_StaticValues template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the XSI_StaticValues template
*/
CdotXSITemplate*	Register_XSI_StaticValues();

/*! XSI_Mixer template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the XSI_Mixer template
*/
CdotXSITemplate*	Register_XSI_Mixer();

/*! XSI_Action template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the XSI_Action template
*/
CdotXSITemplate*	Register_XSI_Action();

/*! XSI_Track template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the XSI_Track template
*/
CdotXSITemplate*	Register_XSI_Track();

/*! XSI_Actionclip template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the XSI_Actionclip template
*/
CdotXSITemplate*	Register_XSI_Actionclip();

/*! XSI_TimeControl template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the XSI_TimeControl template
*/
CdotXSITemplate*	Register_XSI_TimeControl();

/*! XSI_Extrapolation template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the XSI_Extrapolation template
*/
CdotXSITemplate*	Register_XSI_Extrapolation();

/*! @} */

extern	TemplateCreation	RegistrationTable30[];




#endif
