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

#ifndef _RTP_BASICPROCESSESTYPES_
#define _RTP_BASICPROCESSESTYPES_

#include <SIBCUtil.h>
#include <SIBCMatrix.h>
#include <SIRTNode.h>
#include <Material.h>
#include <Camera.h>
#include <XSIShader.h>
#include <SIDisplayDriver.h>
#include <Instance.h>
#include <SIBaseApplication.h>

#define MAX_REALTIME_SHADER_TEXTURES 4
#define	APPID_XSI				0x00000000
#define	APPID_XSIVIEWER			0x00000100
#define	APPID_XSIVIEWER_OGL		0x00000101
#define	APPID_XSIVIEWER_D3D		0x00000102
#define	APPID_XSIVIEWER_XBOX	0x00000103
#define	APPID_XSIVIEWER_PS2GL	0x00000104
#define APPID_CROWD				0x00000200

// realtime shader stuff

#ifndef RTSHADER_TYPES_H


/**
* Realtime Shader requirement flags
*/
enum RTSRequirement
{
	RTS_REQUIRE_ZSORT								=	0,			/*!< Requires ZSort*/
	RTS_REQUIRE_VNODEID								=	1,			/*!< VNode ID*/
	RTS_REQUIRE_UNUSED								=   0xffffffff	
};

/**
* Structure holding UV values. 
*\see S3DVertexArrayVertex
*/
typedef struct tagVertexArrayUV
{
	SI_Float u;
	SI_Float v;
} S3DVertexArrayUV;

/**
* Structure holding vertex data (position, normal, UV coordinates and color info. 
*/
typedef struct tagVertexArrayVertex
{
	SI_Float		m_VtxX;		/*!< Vertex X position*/
	SI_Float		m_VtxY;		/*!< Vertex Y position*/
	SI_Float		m_VtxZ;		/*!< Vertex Z position*/

	SI_Float		m_NrmX;		/*!< Normal X position*/
	SI_Float		m_NrmY;		/*!< Normal Y position*/
	SI_Float		m_NrmZ;		/*!< Normal Z position*/

	S3DVertexArrayUV m_Txt[MAX_REALTIME_SHADER_TEXTURES]; /*!< 4 sets of Texture UV \sa S3DVertexArrayUV*/

	SI_UByte			m_ColR;	/*!< Color red component*/
	SI_UByte			m_ColG;	/*!< Color green component*/
	SI_UByte			m_ColB;	/*!< Color blue component*/
	SI_UByte			m_ColA; /*!< Color alpha component*/

	SI_ULong		m_Vref;	/*!< Vertex index into the mesh*/
	SI_ULong		m_Nref;	/*!< Normal index into the mesh*/

	SI_Void*		m_Reserved;	/*!< unused*/

} S3DVertexArrayVertex;

/**
* Header describing vertex buffer for.RT Shaders
*/
typedef struct tagVertexArrayHeader // (16 bytes)
{
	// Nb vertices in the next header. 
	SI_Long		m_NbVertex;			/*!< Number of vertices to follow */
	SI_Long		m_bUseNormals;		/*!< Are normals present/valid?*/
	SI_Long		m_bUseColors;		/*!< Are colors present/valid?*/
	SI_Long		m_NbTextures;		/*!< Number of UVs at this vertex.*/
	SI_Long		m_StripType;		/*!< OpenGL strip (glbegin) type.*/
} S3DVertexArrayHeader;

/**
* Header describing system parameters for realtime shaders
*/
#ifdef CurrentTime
#undef CurrentTime
#endif // CurrentTime


typedef struct
{
	SI_Long		NbParams;			/*!< Number of parameters */
	SI_Long		OglContext;			/*!< Context OGL (HGLRC)*/
	SI_Long		Invalidated;		/*!< Context OGL valid (BOOL)*/
	SI_Void		*Instance;			/*!< Phenomenon instance "ID" (ULONG)*/
	float		CurrentTime;		/*!< System time (FLOAT)		Note: As retreived from SIGetCurrentTime().*/
	SI_Long		AppID;				/*!< Application ID*/
	SI_Void		*Handle;			/*!< Object handle*/
} SystemParams;

/**
* Header describing texture information in realtime shaders
*/
typedef struct tagTexture
{
	wchar_t	*m_Path;			/*!< image path*/
	SI_Long	m_Width;			/*!< image width*/
	SI_Long	m_Height;			/*!< image height*/
	SI_Long	m_NbChannels;		/*!< Number of color channels in image*/
	SI_Long	m_PixelType;		/*!< Pixel type*/
	SI_Long	m_BufferSize;		/*!< Size of image buffer*/
	SI_Byte	*m_Buffer;			/*!< Image buffer*/
	SI_Long	m_Dirty;			/*!< Flag indicating if the buffer has changed*/
} S3DTexture;

#endif //RTSHADER_TYPES_H

typedef struct tagSI_RTPParserData
{
	CSIRTNode		*m_pParentNode;
	CSIRTProcess	*m_pParentProcess;
} SI_RTPParserData;

/**
* Projection type for the CSIRTP_CameraDisplay process
*/
enum SI_CameraProjectionType
{
	SI_CAMERA_ORTHO,		/*!< Orthographic projection*/
	SI_CAMERA_PERSP			/*!< Perspective projection*/
};

/**
* Data passed to a CSIRTP_CameraDisplay process
*/
typedef struct tagCSIRTP_CameraDisplayData
{
	CSLCamera					*m_pCamera;			/*!< Scene camera*/
	CSIDisplayDriver			*m_pDisplay;		/*!< Pointer to the display driver*/
	SI_CameraProjectionType		m_lProjectionType;	/*!< Projection type*/
} CSIRTP_CameraDisplayData;


/**
* Manipulation type for the CSIRTP_CameraManipulate process
*
* -	Camera manipulation 
*	-	Softimage
*		-	Orbit
*		-	Zoom
*		-	Pan
*		-	Dolly
*		-	Frame all
*		-	Select object
*	-	FPS
*		-	Strafe
*		-	Forward/backward
*		-	Camera look
*		-	Select object
*	-	Simple mode
*		-	LMB + mouse = orient
*		-	RMB + mouse = pan
*		-	MMB + mouse = zoom
*/

enum SI_CameraManipulationType
{
	SI_CAMERA_MANIPULATION_NONE,		/*!< No manipulation*/
	SI_CAMERA_MANIPULATION_SOFTIMAGE,	/*!< Softimage manipulation mode (XSI keys)*/
	SI_CAMERA_MANIPULATION_FPS,			/*!< First-person manipulation*/
	SI_CAMERA_MANIPULATION_IDIOT,		/*!< Simple mouse driven manipulation*/
};

/** 
* This enum lists all possible camera manipulation modes.
*/
enum EManipulationMode
{
	SI_PASSTHROUGH,		/*!< Passthrough mode will ignore all camera animation and will only be driven by the manipulator*/
	SI_ABSOLUTE,		/*!< Absolute will ignore all manipulator input and the camera will be animated by it's animation only*/
	SI_RELATIVE,		/*!< Relative will mix both the animation and the user input*/
};

/**
* Data holding information about camera a manipulator
*/
typedef struct tagCSIRTP_CameraManipulateData
{
	CSLCamera					*m_pCamera;				/*!< The scene camera*/
	SI_CameraManipulationType	m_lManipulationType;	/*!< Manipulation type*/
	SI_Float					m_fOrbitSpeed;			/*!< Orbit speed*/
	SI_Float					m_fDollySpeed;			/*!< Dolly speed*/
	SI_Float					m_fPanSpeed;			/*!< Pan speed*/
	SI_Float					m_fZoomSpeed;			/*!< Zoom speed*/
	SI_Float					m_fLookSpeed;			/*!< Look speed*/
	SI_Float					m_fForwardSpeed;		/*!< Forward speed*/
	SI_Float					m_fStrafeSpeed;			/*!< Strafe speed*/
	SI_GamePadInfo				m_pGamePadInfo;			/*!< Gamepad information structure*/
	EManipulationMode			m_eManipulationMode;	/*!< Manipulation mode*/
	EManipulationMode			m_ePreviousMode;		/*!< Previous Manipulation mode*/
	CSIRTProcess*				m_pLinkedCameraManipulator;	/*!< Linked Camera Manipulator*/
	CSIRTProcess*				m_pTargetCameraManipulator;	/*!< Target Camera Manipulator*/

} CSIRTP_CameraManipulateData;

/**
* Internal structure used to copy a scene camera to the master camera
*/
typedef struct tagCSIRTP_CameraCopyData
{
	CSLCamera					*m_pSrcCamera;	/*!< The source camera*/
	CSLCamera					*m_pDstCamera;	/*!< The destination camera*/
	CSIRTNode					*m_pSrcNode;	/*!< The source node*/
} CSIRTP_CameraCopyData;

/**
* Structure holding information about a list of triangles
*/
typedef struct tagMaterialCluster
{
	CSLBaseMaterial*		m_pMaterial;		/*!< The material assigned to this cluster*/
	CSIRTProcess*			m_pMaterialProcess;	/*!< The CSIRTP_Material process driving this material*/
	CSIBCMatrix4x4*			m_pGlobalMatrix;	/*!< The global matrix used to transform the cluster*/
	SI_Long					m_lOffset;			/*!< Offset in bytes to reach the vertex data in m_pVertexData*/
	SI_Char*				m_pVertexData;		/*!< Vertex data See the Realtime Shader SDK documentation for more information on the packaging of vertex data*/
	CSLModel*				m_pOwner;			/*!< The models that owns this cluster*/
	CSIBCArray<SI_Int>		m_TextureDomains;	/*!< Array of texture domains*/
} CSIMaterialCluster;

/**
* Structure representing the bounding volume of objects
*/
typedef struct tagCSIRTP_BoundingVolume
{
	CSIBCVector3D	m_vMinimumExtent;				/*!< The minimum extent of a surrounding box */
	CSIBCVector3D	m_vMaximumExtent;				/*!< The maximum extent of a surrounding box */
	CSIBCVector3D	m_vMinimumExtentTransformed;	/*!< The minimum extent of a surrounding box after being transformed*/
	CSIBCVector3D	m_vMaximumExtentTransformed;	/*!< The maximum extent of a surrounding box after being transformed*/

	CSIBCVector3D	m_vCenter;						/*!< The center of the object*/
	CSIBCVector3D	m_vCenterTransformed;			/*!< The transformed center of the object*/

	SI_Float		m_fRadius;						/*!< The squared radius of a bounding sphere*/

	SI_Bool			m_bHandledByDeformer;			/*!< TRUE is this bounding volume is being updated by a deformer (like a envelope operator). FAISE it is being updated by the Transform process*/

} CSIRTP_BoundingVolume;

/**
* Structure holding information about a shader display
*/
typedef struct tagCSIRTP_ShaderDisplayData
{
	SI_ShaderRuntimeInfo	m_ShaderRuntimeInfo;	/*!< Shader runtime informatio structure*/
	SystemParams			m_SystemParams;			/*!< System parameters: See the Realtime Shader SDK documentation for more information on system parameters */
	SI_Int					m_lNbStripes;			/*!< Number of stripes in the structure */
	unsigned char			*m_pStripeData;			/*!< Actual stripe (vertex data)*/
	void					*m_pShaderPPG;			/*!< PropertyPage parameters*/
	CSLXSIShader			*m_pShader;				/*!< Shader*/
} CSIRTP_ShaderDisplayData;

/**
* Structure holding information about instances
*/

typedef struct tagCSIRTP_SI_InstanceData
{
	CSLInstance				*m_pInstance;	/*!< Semantic layer Instance object*/
	CSLScene				*m_pModel;		/*!< Instanced model*/

} CSIRTP_SI_InstanceData;



#endif
