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

/******************************************************************************\
*
* File:          Template.h
* Creation date: January 15, 2002
* Purpose:       Declaration of class 'Template'
*
\******************************************************************************/
#ifndef _TEMPLATE_H
#define _TEMPLATE_H

#if defined(_WIN32) || defined(_WIN32_WCE) || defined(_XBOX)

// Disable "identifier was truncated to '255' characters in the debug information" warning.
#pragma warning( disable : 4786 )

// Disable warnings on extern before template instantiation.
#pragma warning( disable : 4231 )

#endif // defined(_WIN32) || defined(_WIN32_WCE)  || defined(_XBOX)

// Include files
#include "SLTypes.h"
#include <XSIParser.h>
#include <dotXSIDefines.h>


// Forward class declaration
class CSLScene;
class CSLModel;
class CSLCustomPSet;
class CSLUserDataBlob;
class CSLFCurve;
class CSLXSILimit;

class CSLTemplateExtension_5_0
{
public:
	CSLTemplateExtension_5_0() : m_szFullName(NULL) {};
	~CSLTemplateExtension_5_0() {};

    CSIBCArray<CSLUserDataBlob *>	m_UserDataBlobs;
	CSIBCArray<CSLXSILimit *>		m_XSILimits;
	CSIBCString						*m_szFullName;
	void							*m_pReserved;
};

/** 
* Describes a dotXSI template
*
* The CSLTemplate class is the base class for all classes in the
* semantic layer that describes a specific dotXSI template.
*
* The class contains generic data manipulation, such as FCurve
* manipulation, custom parameter set manipulation and I/O synchronization.
*
* This class is pure virtual, meaning you can't instantiate it directly.
* However, since you can obtain a pointer to a CSLTemplate, if you want
* to know which derived class was instantied, use the function Type.
*
*/
class XSIEXPORT CSLTemplate : public CSIBCNode
{
public:
	/** 
	* This enum lists all templates supported by the semantic layer.
	*/
	enum ETemplateType
	{
		SI_ACTION_FCURVE,	/*!< Action FCUrve template */
		SI_ANGLE,			/*!< SI_Angle template */
		SI_AMBIENCE,		/*!< SI_Ambience template */
		SI_BASE_SHAPE,		/*!< Base Shape template */
		SI_CAMERA,			/*!< SI_Camera template */
		SI_CLUSTER,			/*!< SI_Cluster template */
		SI_CONSTRAINT,		/*!< SI_Constraint template */
		SI_COORDINATE_SYSTEM,	/*!< SI_CoordinateSystem template */
		SI_CURVE_LIST,		/*!< SI_CurveList template */
		SI_CUSTOM_PSET,		/*!< XSI_CustomPSet template */
		SI_CUSTOM_PARAM_INFO,	/*!< XSI_CustomPSetInfo template */
		SI_DIRECTIONAL_LIGHT,	/*!< Directional light template */
		SI_ENVELOPE,			/*!< SI_Envelope template */
		SI_ENVELOPE_LIST,		/*!< SI_EnvelopeList template */
		SI_FCURVE,				/*!< SI_FCurve template */
		SI_FILE_INFO,			/*!< SI_FileInfo template */
		SI_FOG,					/*!< SI_Fog template */
		SI_GLOBAL_MATERIAL,		/*!< SI_GlobalMaterial template */
		SI_IK_EFFECTOR,			/*!< SI_IK_Effector template */
		SI_IK_JOINT,			/*!< SI_IK_Joint template */
		SI_IK_ROOT,				/*!< SI_IK_Root template */
		SI_INFINITE_LIGHT,		/*!< Infinite light*/
		SI_INSTANCE,			/*!< SI_Instance template*/
		SI_LIGHT_INFO,			/*!< Light info template*/
		SI_MATERIAL,			/*!< Legacy SI_Material template*/
		SI_MATERIAL_LIBRARY,	/*!< SI_MaterialLibrary template*/
		SI_MESH,				/*!< SI_Mesh template*/
		SI_MODEL,				/*!< SI_Model template*/
		SI_NULL_OBJECT,			/*!< SI_Null template*/
		SI_NURBS_CURVE,			/*!< SI_NurbsCurve template*/
		SI_NURBS_SURFACE,		/*!< SI_NurbsSurface template*/
		SI_POINT_LIGHT,			/*!< Point Light */
		SI_POLYGON_LIST,		/*!< SI_PolygonList template*/
		SI_SCENE_INFO,			/*!< SI_SceneInfo template*/
		SI_SHAPE,				/*!< SI_Shape template*/
		SI_SHAPE35,				/*!< SI_Shape dotXSI Version 3.5+*/
		SI_SHAPE_ANIMATION,		/*!< SI_ShapeAnimation Template*/
		SI_SPOT_LIGHT,			/*!< Spot light*/
		SI_SURFACE_MESH,		/*!< SI_SurfaceMesh template*/
		SI_TEXTURE_2D,			/*!< Legacy SI_Texture2D template*/
		SI_TRANSFORM,			/*!< SI_Transform template*/
		SI_TRIANGLE_LIST,		/*!< SI_TriangleList template*/
		SI_TRIANGLE_STRIP,		/*!< SI_TriangleStrip template*/
		SI_TRIANGLE_STRIP_LIST,	/*!< SI_TriangleStripList template*/
		SI_VISIBILITY,			/*!< SI_Visibility template*/
		XSI_ACTION,				/*!< XSI_Action template*/
		XSI_ACTIONCLIP,			/*!< XSI_ActionClip template*/
		XSI_CUSTOM_PARAM_INFO,	/*!< XSI_CustomPSet template*/
		XSI_EXTRAPOLATION,		/*!< XSI_Extrapolation template*/
		XSI_FXOPERATOR,			/*!< XSI_FXOperator template*/
		XSI_FXTREE,				/*!< XSI_FXTree template*/
		XSI_IMAGE,				/*!< XSI_Image template*/
		XSI_IMAGE_DATA,			/*!< XSI_ImageData template*/
		XSI_IMAGE_FX,			/*!< XSI_ImageFX template*/
		XSI_IMAGE_LIBRARY,		/*!< XSI_ImageLibrary template*/
		XSI_MATERIAL,			/*!< XSI_Material template*/
		XSI_MATERIAL_INFO,		/*!< XSI_MaterialInfo template*/
		XSI_MIXER,				/*!< XSI_Mixer template*/
		XSI_NURBS_PROJECTION,	/*!< XSI_NurbsProjection template*/
		XSI_NURBS_TRIM,			/*!< XSI_NurbsTrim template*/
		XSI_SHADER,				/*!< XSI_Shader template*/
		XSI_SHADER_INSTANCE_DATA,	/*!< XSI_ShaderInstanceData template*/
		XSI_STATIC_VALUES,		/*!< XSI_StaticValue template*/
		XSI_TIMECONTROL,		/*!< XSI_TimeControl template*/
		XSI_TRACK,				/*!< XSI_Track template*/
		XSI_USER_DATA,			/*!< XSI_UserData template*/
		XSI_USER_DATA_LIST,		/*!< XSI_UserDataList template*/
		XSI_SUB_COMPONENT_ATTRIBUTE_LIST,	/*!< XSI_SubComponentAttributeList template*/
		XSI_TRIANGLE_STRIP_LIST,			/*!< XSI_TriangleStripList template*/
		XSI_TRIANGLE_LIST,					/*!< XSI_TriangleList template*/
		XSI_POLYGON_LIST,					/*!< XSI_PolygonList template*/
		XSI_VERTEX_LIST,					/*!< XSI_VertexList template*/
		XSI_SHAPE,							/*!< XSI_Shape template*/
		XSI_MESH,							/*!< XSI_Mesh template*/
		XSI_SHAPE_ANIMATION,				/*!< XSI_ShapeAnimation template*/
		XSI_CLUSTER_INFO,					/*!< XSI_ClusterInfo template*/
		XSI_USER_DATA_BLOB,					/*!< XSI_UserDataBlob template*/
		XSI_TRANSFORM,						/*!< XSI_Transform template*/
		XSI_LIMIT,							/*!< XSI_Limit template*/
		XSI_BASEPOSE,						/*!< XSI_BasePose template*/
	};

	/** 
	* The EFCurveType enum lists all animatable parameters.
	*/
	enum EFCurveType
	{
		SI_COLOR_R,			/*!< Color */
		SI_COLOR_G,			/*!< Color */
		SI_COLOR_B,			/*!< Color */
		SI_POSITION_X,		/*!< Camera position */
		SI_POSITION_Y,		/*!< Camera position */
		SI_POSITION_Z,		/*!< Camera position */
		SI_ROLL,			/*!< Camera roll */
		SI_FOV,				/*!< Camera fov */
		SI_NEAR,			/*!< Camera near plane */
		SI_FAR,				/*!< Camera far plane */
		SI_INTEREST_X,		/*!< Camera interest position*/
		SI_INTEREST_Y,		/*!< Camera interest position*/
		SI_INTEREST_Z,		/*!< Camera interest position*/
		SI_CONE,			/*!< Light cone angle*/
		SI_SPREAD,			/*!< Light spread*/
		SI_ORIENTATION_X,	/*!< Light orientation*/
		SI_ORIENTATION_Y,	/*!< Light orientation*/
		SI_ORIENTATION_Z,	/*!< Light orientation*/
		SI_DIFFUSE_R,		/*!< Material diffuse*/
		SI_DIFFUSE_G,		/*!< Material diffuse*/
		SI_DIFFUSE_B,		/*!< Material diffuse*/
		SI_DIFFUSE,			/*!< Material diffuse*/
		SI_PARAMETER,		/*!< Named parameter*/
		SI_POWER,			/*!< Material specular power*/
		SI_SPECULAR_R,		/*!< Material specular*/
		SI_SPECULAR_G,		/*!< Material specular*/
		SI_SPECULAR_B,		/*!< Material specular*/
		SI_EMISSIVE_R,		/*!< Material emissive*/
		SI_EMISSIVE_G,		/*!< Material emissive*/
		SI_EMISSIVE_B,		/*!< Material emissive*/
		SI_AMBIENT_R,		/*!< Material ambient*/
		SI_AMBIENT_G,		/*!< Material ambient*/
		SI_AMBIENT_B,		/*!< Material ambient*/
		SI_SCALING_X,		/*!< Scaling*/
		SI_SCALING_Y,		/*!< Scaling*/
		SI_SCALING_Z,		/*!< Scaling*/
		SI_ROTATION_X,		/*!< Rotation*/
		SI_ROTATION_Y,		/*!< Rotation*/
		SI_ROTATION_Z,		/*!< Rotation*/
		SI_TRANSLATION_X,	/*!< Translation*/
		SI_TRANSLATION_Y,	/*!< Translation*/
		SI_TRANSLATION_Z,	/*!< Translation*/
		SI_SHPANIM,			/*!< ShapeAnimation*/
		SI_NODEVIS,			/*!< Visibility*/
		SI_HUE,				/*!< ImageFX hue*/
		SI_GAIN,			/*!< ImageFX gain*/
		SI_SATURATION,		/*!< ImageFX saturation*/
		SI_BRIGHTNESS,		/*!< ImageFX brightness*/
		SI_BLUR_RADIUS,		/*!< ImageFX blur*/
		SI_BLUR_AMOUNT,		/*!< ImageFX blur*/
		SI_BLUR_ALPHA,		/*!< ImageFX blur*/
		SI_SCALING_TYPE,	/*!< ImageFX scale type*/
		SI_SCALE_X,			/*!< ImageFX scale*/
		SI_SCALE_Y,			/*!< ImageFX scale*/
		SI_FLIP_X,			/*!< ImageFX flip*/
		SI_FLIP_Y,			/*!< ImageFX flip*/
		SI_RGBA2GRAYSCALE,	/*!< ImageFX rgba to grayscale*/
		SI_BITS_PER_CHANNEL,	/*!< ImageFX bits per channel*/
		SI_CROP_MIN_X,		/*!< ImageClip crop*/
		SI_CROP_MAX_X,		/*!< ImageClip crop*/
		SI_CROP_MIN_Y,		/*!< ImageClip crop*/
		SI_CROP_MAX_Y,		/*!< ImageClip crop*/
		SI_FALLOFF_ACTIVE,	/*!< Light falloff active*/
		SI_FALLOFF_START,	/*!< Light falloff start*/
		SI_FALLOFF_END,		/*!< Light falloff end*/
		SI_SHADOWS_ENABLED,	/*!< Light shadows enabled*/
		SI_UMBRA,			/*!< Light umbra*/
		SI_LIGHT_AS_ENERGY,	/*!< Light as energy*/
		SI_ENERGY_FACTOR,	/*!< Light energy factor*/
		SI_INTENSITY,		/*!< Light intensity*/
	};

	/** 
	* The EFCurveInterpolationType enum contains possible interpolation types for an fcurve
	*/
	enum EFCurveInterpolationType
	{
		SI_CONSTANT,
		SI_LINEAR,
		SI_HERMITE,
		SI_CUBIC,
	};

	/** 
	* The ERotationOrder enum contains possible rotation order for the XSITransform template.
	*/
	enum ERotationOrder
	{
		SI_XYZ,
		SI_XZY,
		SI_YXZ,
		SI_YZX,
		SI_ZXY,
		SI_ZYX,
	};

    CSLTemplate(CSLScene* in_pScene, CSLModel *in_pParentModel, CdotXSITemplate* in_pTemplate);
    virtual ~CSLTemplate();
    SI_Void SetName(SI_Char *in_szNewName);
    SI_Char* GetName();

	/*!	Return the model parent of this template.
		\return	CSLModel*	Pointer to the parent model.
	*/
    CSLModel* ParentModel();

	/*!	Return the current scene object.
		\return	CSLScene*	Pointer to the scene.
	*/
    CSLScene* Scene();

	/*!	Return the FTK object associated with this template.
		\return	CdotXSITemplate*	Pointer to a FTK template.
	*/
    CdotXSITemplate* Template();

	// FCurve manipulation functions
	/*!	Create a new FCurve on this template.
		\param	in_Type		Type of fcurve to create
		\param	in_InterpolationType	Type of interpolation for the fcurve
		\return	CSLFCurve*	Pointer to the newly created fcurve.
	*/
    CSLFCurve* CreateFCurve(EFCurveType in_Type, EFCurveInterpolationType in_InterpolationType);

	/*!	Create a new FCurve on this template.
		\param	in_szParameterName	Name of the parameter
		\param	in_InterpolationType	Type of interpolation for the fcurve
		\return	CSLFCurve*	Pointer to the newly created fcurve.
	*/
    CSLFCurve* CreateParameterFCurve(SI_Char* in_szParameterName, EFCurveInterpolationType in_InterpolationType);


	/*!	Destroy an existing FCurve on this template.
		\param	in_Type	Type of fcurve to destroy
		\return	SI_Error	Error code.
	*/
	SI_Error DestroyFCurve(EFCurveType in_Type);

	/*!	List all fcurves on the template.
		\return	CSLFCurve**	Array of pointers to fcurves.
	*/
	CSLFCurve **FCurves();

	/*!	Return the number of fcurves currently present on the template.
		\return	SI_Int	Number of fcurves.
	*/
	SI_Int GetFCurveCount();

	/*!	Return a specific fcurve on the template.
		\param	in_Type	Type of fcurve to retrieve.
		\return	CSLFCurve*	Pointer to the specified fcurve.
	*/
	CSLFCurve* GetSpecificFCurve(EFCurveType in_Type);

	/*!	Return a specific fcurve on the template.
		\param	in_szParameterName	Name of the parameter's FCurve to find
		\return	CSLFCurve*	Pointer to the specified fcurve.
	*/
	CSLFCurve* GetParameterFCurve(SI_Char* in_szParameterName);

	/*!	Remove all fcurves on the template.
		\return	SI_Error	Error code.
	*/
	SI_Error ClearFCurves();

	// Custom parameter set manipulation functions.
	//todo : respect naming convention.

	
	/*!	Adds a custom pset to the template
		\return	CSLCustomPSet*	Pointer to the newly created custom pset, NULL if failed
	*/
	CSLCustomPSet* AddCustomPSet();

	/*!	Clears all the custom psets on that template
		\return	SI_Error whether the function failed or succeeded
	*/
	SI_Error ClearCustomPSets();

	/*!	Return the list of custom psets on that template
		\return	CSLCustomPSet**	array of custom psets instances
	*/
	CSLCustomPSet** GetCustomPSetList();

	/*!	Return the number of custom psets on that template
		\return	SI_Int the number of custom psets on that template
	*/
	SI_Int GetCustomPSetCount();

	/*!	Removes a custom pset by index
		\return	SI_Error whether the function failed or succeeded
	*/
	SI_Error RemoveCustomPSet(SI_Int in_nIndex);

	/*!	Removes a custom pset by name
		\return	SI_Error whether the function failed or succeeded
	*/
	SI_Error RemoveCustomPSet(CSLCustomPSet *in_pToRemove);

	// Connection functions.
	/*!	Connects an fcurve to this template
		\param	CSLFCurve*	the fcurve to connect
		\return	CSLFCurve*	Pointer to the specified fcurve.
	*/
	CSLFCurve *ConnectFCurve(CSLFCurve *in_pNewFCurve);

	/*!	Connects a custom pset to this template
		\param	CSLCustomPSet*	the custom pset to connect
		\return	CSLCustomPSet*	Pointer to the specified custom pset
	*/
	CSLCustomPSet *ConnectCustomPSet(CSLCustomPSet *in_pNewCustomPSet);

	/*!	Clean invalid data from the template.
		\return	SI_Error	Error code
	*/
	virtual SI_Error Fix();

	/*!	Synchronize will ensure that the animated parameter is up to date.
		\return	SI_Error	Error code
	*/
    virtual SI_Error Synchronize();

	/*!	Indicate which dotXSI template this object describes.
		\return	ETemplateType	Type of template
	*/
    virtual ETemplateType Type() = 0;

	/*!	Return a proxy to the specified parameter.
		\param	in_szName	Name of the parameter
		\return	CSLAnimatableType*	Pointer to a parameter proxy.
	*/
    virtual CSLAnimatableType* ParameterFromName(SI_Char *in_szName);

	/*!	Return a proxy to the specified parameter.
		\param	in_Type	Type of parameter
		\param	in_szParameterName	Name of the parameter if the type is SI_PARAMETER
		\return	CSLAnimatableType*	Pointer to a parameter proxy.
	*/
    virtual CSLAnimatableType* ParameterFromType(EFCurveType in_Type, SI_Char *in_szParameterName);

	/*!	Return the object's fullname (XSI path).
		\return	CSIBCString	String.
	*/
	virtual SI_Char* FullName();

	/*!	Connects a user data blob to this template
		\param	CSLUserDataBlob*	the user data blob to connect
		\return	CSLUserDataBlob*	Pointer to the specified user data blob
	*/
	CSLUserDataBlob *ConnectUserDataBlob(CSLUserDataBlob *in_pNewUserDataBlob);


	/*!	Adds a user data blob to the template
		\return	CSLUserDataBlob*	Pointer to the newly created user data blob, NULL if failed
	*/
	CSLUserDataBlob* AddUserDataBlob();

	/*!	Clears all the user data blobs on that template
		\return	SI_Error whether the function failed or succeeded
	*/
	SI_Error ClearUserDataBlobs();

	/*!	Return the list of user data blobs on that template
		\return	CSLUserDataBlob**	array of user data blobs instances
	*/
	CSLUserDataBlob** GetUserDataBlobList();

	/*!	Return the number of user data blobs on that template
		\return	SI_Int the number of user data blobs on that template
	*/
	SI_Int GetUserDataBlobCount();

	/*!	Removes a user data blob by index
		\return	SI_Error whether the function failed or succeeded
	*/
	SI_Error RemoveUserDataBlob(SI_Int in_nIndex);

	/*!	Removes a user data blob by name
		\return	SI_Error whether the function failed or succeeded
	*/
	SI_Error RemoveUserDataBlob(CSLUserDataBlob *in_pToRemove);

	/*!	Return the name (string) of a specified parameter.
		\param	in_pParameter	The parameter
		\return	SI_Char*	Name of the parameter.
	*/
    virtual const SI_Char* GetParameterName(CSLAnimatableType* in_pParameter);
 
	////////////// LIMIT
	/*!	Connects a limit to this template
		\param	CSLXSILimit*	the limit to connect
		\return	CSLXSILimit*	Pointer to the specified limit
	*/
	CSLXSILimit *ConnectXSILimit(CSLXSILimit *in_pXSILimit);


	/*!	Adds a limit to this template
		\return	CSLXSILimit*	Pointer to the newly created limit, NULL if failed
	*/
	CSLXSILimit* AddXSILimit(CSLAnimatableType* in_pParameter);

	/*!	Clears all limits on that template
		\return	SI_Error whether the function failed or succeeded
	*/
	SI_Error ClearXSILimits();

	/*!	Return the list of limits on that template
		\return	CSLXSILimit**	array of XSI limits
	*/
	CSLXSILimit** GetXSILimitList();

	/*!	Return the number of limits on that template
		\return	SI_Int the number of limits on that template
	*/
	SI_Int GetXSILimitCount();

	/*!	Removes a limit by index
		\return	SI_Error whether the function failed or succeeded
	*/
	SI_Error RemoveXSILimit(SI_Int in_nIndex);

	/*!	Removes a limit
		\return	SI_Error whether the function failed or succeeded
	*/
	SI_Error RemoveXSILimit(CSLXSILimit *in_pToRemove);

protected:
	/*! Resets the Parent to the given parent
	\param in_pNewParent
	\return SI_SUCCESS if successfull
	*/

	SI_Error SetParent( CSLModel *in_pNewParent );

private:
	CdotXSITemplate* m_pTemplate;
    CSLScene* m_pScene;
    CSLModel* m_pParentModel;
    CSIBCArray<CSLCustomPSet *> m_CustomPSets;
	CSIBCArray<CSLFCurve *> m_FCurves;

	// This member is here to allow future improvement of the class.
	CSLTemplateExtension_5_0 *m_pExtension; // was void *m_pReserved
};

#endif
